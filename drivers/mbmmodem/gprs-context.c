/*
 *
 *  oFono - Open Source Telephony
 *
 *  Copyright (C) 2008-2009  Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <glib.h>

#include <ofono/log.h>
#include <ofono/modem.h>
#include <ofono/gprs-context.h>

#include "gatchat.h"
#include "gatresult.h"

#include "mbmmodem.h"

#define MBM_E2NAP_DISCONNECTED 0
#define MBM_E2NAP_CONNECTED 1
#define MBM_E2NAP_CONNECTING 2

#define AUTH_BUF_LENGTH OFONO_GPRS_MAX_USERNAME_LENGTH + \
			OFONO_GPRS_MAX_PASSWORD_LENGTH + 128

static const char *none_prefix[] = { NULL };

struct gprs_context_data {
	GAtChat *chat;
	unsigned int active_context;
};

static void at_enap_down_cb(gboolean ok, GAtResult *result, gpointer user_data)
{
	struct cb_data *cbd = user_data;
	ofono_gprs_context_cb_t cb = cbd->cb;
	struct ofono_gprs_context *gc = cbd->user;
	struct gprs_context_data *gcd = ofono_gprs_context_get_data(gc);
	struct ofono_error error;

	if (ok)
		gcd->active_context = 0;

	dump_response("enap_down_cb", ok, result);
	decode_at_error(&error, g_at_result_final_response(result));

	cb(&error, cbd->data);
}

static void mbm_enap_up_cb(gboolean ok, GAtResult *result, gpointer user_data)
{
	struct cb_data *cbd = user_data;
	ofono_gprs_context_up_cb_t cb = cbd->cb;
	struct ofono_gprs_context *gc = cbd->user;
	const char *interface = NULL;
	struct ofono_error error;

	dump_response("enap_up_cb", ok, result);
	decode_at_error(&error, g_at_result_final_response(result));

	if (ok) {
		struct ofono_modem *modem = ofono_gprs_context_get_modem(gc);
		interface = ofono_modem_get_string(modem, "NetworkInterface");
	} else {
		struct gprs_context_data *gcd = ofono_gprs_context_get_data(gc);
		gcd->active_context = 0;
	}

	cb(&error, interface, FALSE, NULL, NULL, NULL, NULL, cbd->data);
}

static void mbm_cgdcont_cb(gboolean ok, GAtResult *result, gpointer user_data)
{
	struct cb_data *cbd = user_data;
	ofono_gprs_context_up_cb_t cb = cbd->cb;
	struct ofono_gprs_context *gc = cbd->user;
	struct gprs_context_data *gcd = ofono_gprs_context_get_data(gc);
	struct cb_data *ncbd;
	char buf[64];

	dump_response("cgdcont_cb", ok, result);

	if (!ok) {
		struct ofono_error error;

		gcd->active_context = 0;

		decode_at_error(&error, g_at_result_final_response(result));
		cb(&error, NULL, 0, NULL, NULL, NULL, NULL, cbd->data);
		return;
	}

	ncbd = g_memdup(cbd, sizeof(struct cb_data));

	sprintf(buf, "AT*ENAP=1,%u", gcd->active_context);

	if (g_at_chat_send(gcd->chat, buf, none_prefix,
				mbm_enap_up_cb, ncbd, g_free) > 0)
		return;

	if (ncbd)
		g_free(ncbd);

	gcd->active_context = 0;

	CALLBACK_WITH_FAILURE(cb, NULL, 0, NULL, NULL, NULL, NULL, cbd->data);
}

static void mbm_gprs_activate_primary(struct ofono_gprs_context *gc,
				const struct ofono_gprs_primary_context *ctx,
				ofono_gprs_context_up_cb_t cb, void *data)
{
	struct gprs_context_data *gcd = ofono_gprs_context_get_data(gc);
	struct cb_data *cbd = cb_data_new(cb, data);
	char buf[AUTH_BUF_LENGTH];
	int len;

	if (!cbd)
		goto error;

	gcd->active_context = ctx->cid;

	cbd->user = gc;

	sprintf(buf, "AT*EIAAUW=%d,1,\"%s\",\"%s\"", ctx->cid,
		ctx->username, ctx->password);

	if (g_at_chat_send(gcd->chat, buf, none_prefix,
				NULL, NULL, NULL) == 0)
		goto error;
						
	len = sprintf(buf, "AT+CGDCONT=%u,\"IP\"", ctx->cid);

	if (ctx->apn)
		snprintf(buf + len, sizeof(buf) - len - 3, ",\"%s\"",
				ctx->apn);

	if (g_at_chat_send(gcd->chat, buf, none_prefix,
				mbm_cgdcont_cb, cbd, g_free) > 0)
		return;
error:
	if (cbd)
		g_free(cbd);

	CALLBACK_WITH_FAILURE(cb, NULL, 0, NULL, NULL, NULL, NULL, data);
}

static void mbm_gprs_deactivate_primary(struct ofono_gprs_context *gc,
					unsigned int cid,
					ofono_gprs_context_cb_t cb, void *data)
{
	struct gprs_context_data *gcd = ofono_gprs_context_get_data(gc);
	struct cb_data *cbd = cb_data_new(cb, data);

	if (!cbd)
		goto error;

	cbd->user = gc;

	if (g_at_chat_send(gcd->chat, "AT*ENAP=0", none_prefix,
				at_enap_down_cb, cbd, g_free) > 0)
		return;

error:
	if (cbd)
		g_free(cbd);

	CALLBACK_WITH_FAILURE(cb, data);
}

static void e2nap_notifier(GAtResult *result, gpointer user_data)
{
	struct ofono_gprs_context *gc = user_data;
	struct gprs_context_data *gcd = ofono_gprs_context_get_data(gc);
	GAtResultIter iter;
	int state;

	if (gcd->active_context == 0)
		return;

	g_at_result_iter_init(&iter, result);

	if (g_at_result_iter_next(&iter, "*E2NAP:") == FALSE)
		return;

	g_at_result_iter_next_number(&iter, &state);

	switch (state) {
	case MBM_E2NAP_DISCONNECTED:
		ofono_gprs_context_deactivated(gc, gcd->active_context);
		gcd->active_context = 0;
		break;
	case MBM_E2NAP_CONNECTED:
		ofono_debug("MBM Context: connected");
		break;
	case MBM_E2NAP_CONNECTING:
		ofono_debug("MBM Context: connecting");
		break;
	default:
		break;
	};
}

static int mbm_gprs_context_probe(struct ofono_gprs_context *gc,
					unsigned int vendor, void *data)
{
	GAtChat *chat = data;
	struct gprs_context_data *gcd;

	gcd = g_new0(struct gprs_context_data, 1);
	gcd->chat = chat;

	g_at_chat_register(chat, "*E2NAP:", e2nap_notifier, FALSE, gc, NULL);

	g_at_chat_send(chat, "AT*E2NAP=1", NULL, NULL, NULL, NULL);

	ofono_gprs_context_set_data(gc, gcd);

	return 0;
}

static void mbm_gprs_context_remove(struct ofono_gprs_context *gc)
{
	struct gprs_context_data *gcd = ofono_gprs_context_get_data(gc);

	ofono_gprs_context_set_data(gc, NULL);
	g_free(gcd);
}

static struct ofono_gprs_context_driver driver = {
	.name			= "mbm",
	.probe			= mbm_gprs_context_probe,
	.remove			= mbm_gprs_context_remove,
	.activate_primary	= mbm_gprs_activate_primary,
	.deactivate_primary	= mbm_gprs_deactivate_primary,
};

void mbm_gprs_context_init()
{
	ofono_gprs_context_driver_register(&driver);
}

void mbm_gprs_context_exit()
{
	ofono_gprs_context_driver_unregister(&driver);
}
