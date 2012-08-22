/*
 *
 *  oFono - Open Source Telephony
 *
 *  Copyright (C) 2009-2010  Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __ISIMODEM_GPDS_H
#define __ISIMODEM_GPDS_H

#ifdef __cplusplus
extern "C" {
#endif

#define GPDS_MAX_CONTEXT_COUNT			11
#define GPDS_TIMEOUT				3
#define GPDS_CTX_ACTIVATE_TIMEOUT		(6 * 30)	/* 6 * T3380 */
#define GPDS_CTX_DEACTIVATE_TIMEOUT		(6 * 8)		/* 6 * T3390 */
#define GPDS_ATTACH_TIMEOUT			(6 * 15)	/* 6 * T3310 */
#define GPDS_DETACH_TIMEOUT			(6 * 15)	/* 6 * T3321 */

#define GPDS_MAX_APN_STRING_LENGTH		100
#define GPDS_MAX_USERNAME_LENGTH		53
#define GPDS_MAX_PASSWORD_LENGTH		53

#define PN_GPDS					0x31
#define PN_PEP_TYPE_GPRS			0x04
#define PN_WRAN					0xB4

enum gpds_message_id {
	GPDS_LL_CONFIGURE_REQ =			0x00,
	GPDS_LL_CONFIGURE_RESP =		0x01,
	GPDS_CONTEXT_ID_CREATE_REQ =		0x02,
	GPDS_CONTEXT_ID_CREATE_RESP =		0x03,
	GPDS_CONTEXT_ID_CREATE_IND =		0x04,
	GPDS_CONTEXT_ID_DELETE_IND =		0x05,
	GPDS_CONTEXT_CONFIGURE_REQ =		0x06,
	GPDS_CONTEXT_CONFIGURE_RESP =		0x07,
	GPDS_CONTEXT_ACTIVATE_REQ =		0x08,
	GPDS_CONTEXT_ACTIVATE_RESP =		0x09,
	GPDS_CONTEXT_ACTIVATE_IND =		0x0A,
	GPDS_CONTEXT_DEACTIVATE_REQ =		0x0B,
	GPDS_CONTEXT_DEACTIVATE_RESP =		0x0C,
	GPDS_CONTEXT_DEACTIVATE_IND =		0x0D,
	GPDS_CONTEXT_MWI_ACT_REQUEST_IND =	0x0E,
	GPDS_CONTEXT_NWI_ACT_REJECT_REQ =	0x0F,
	GPDS_CONTEXT_NWI_ACT_REJECT_RESP =	0x10,
	GPDS_CONFIGURE_REQ =			0x11,
	GPDS_CONFIGURE_RESP =			0x12,
	GPDS_ATTACH_REQ =			0x13,
	GPDS_ATTACH_RESP =			0x14,
	GPDS_ATTACH_IND =			0x15,
	GPDS_DETACH_REQ =			0x16,
	GPDS_DETACH_RESP =			0x17,
	GPDS_DETACH_IND =			0x18,
	GPDS_STATUS_REQ =			0x19,
	GPDS_STATUS_RESP =			0x1A,
	GPDS_SMS_PDU_SEND_REQ =			0x1B,
	GPDS_SMS_PDU_SEND_RESP =		0x1C,
	GPDS_SMS_PDU_RECEIVE_IND =		0x1D,
	GPDS_TRANSFER_STATUS_IND =		0x1E,
	GPDS_CONTEXT_ACTIVATE_FAIL_IND =	0x1F,
	GPDS_LL_BIND_REQ =			0x20,
	GPDS_LL_BIND_RESP =			0x21,
	GPDS_CONTEXT_STATUS_REQ =		0x22,
	GPDS_CONTEXT_STATUS_RESP =		0x23,
	GPDS_CONTEXT_STATUS_IND =		0x24,
	GPDS_CONTEXT_ACTIVATING_IND =		0x25,
	GPDS_CONTEXT_MODIFY_REQ =		0x2A,
	GPDS_CONTEXT_MODIFY_RESP =		0x2B,
	GPDS_CONTEXT_MODIFY_IND =		0x2C,
	GPDS_ATTACH_FAIL_IND =			0x2D,
	GPDS_CONTEXT_DEACTIVATING_IND =		0x2F,
	GPDS_CONFIGURATION_INFO_REQ =		0x30,
	GPDS_CONFIGURATION_INFO_RESP =		0x31,
	GPDS_CONFIGURATION_INFO_IND =		0x32,
	GPDS_CONTEXT_AUTH_REQ =			0x33,
	GPDS_CONTEXT_AUTH_RESP =		0x34,
	GPDS_TEST_MODE_REQ =			0x35,
	GPDS_TEST_MODE_RESP =			0x36,
	GPDS_RADIO_ACTIVITY_IND =		0x37,
	GPDS_FORCED_READY_STATE_REQ =		0x38,
	GPDS_FORCED_READY_STATE_RESP =		0x39,
	GPDS_CONTEXTS_CLEAR_REQ =		0x3A,
	GPDS_CONTEXTS_CLEAR_RESP =		0x3B,
	GPDS_MBMS_SERVICE_SELECTION_REQ =	0x3C,
	GPDS_MBMS_SERVICE_SELECTION_RESP =	0x3D,
	GPDS_MBMS_STATUS_IND =			0x3E,
	GPDS_MBMS_CONTEXT_CREATE_REQ =		0x3F,
	GPDS_MBMS_CONTEXT_CREATE_RESP =		0x40,
	GPDS_MBMS_CONTEXT_ACTIVATE_REQ =	0x41,
	GPDS_MBMS_CONTEXT_ACTIVATE_RESP =	0x42,
	GPDS_MBMS_CONTEXT_DELETE_REQ =		0x43,
	GPDS_MBMS_CONTEXT_DELETE_RESP =		0x44,
	GPDS_MBMS_CONTEXT_DELETE_IND =		0x45,
	GPDS_MBMS_SERVICE_SELECTION_IND =	0x46,
	GPDS_MBMS_SERVICE_AVAILABLE_IND =	0x47,
	GPDS_TEST_REQ =				0x48,
	GPDS_TEST_RESP =			0x49
};

enum gpds_subblock {
	GPDS_COMP_INFO =			0x00,
	GPDS_QOS_REQ_INFO =			0x01,
	GPDS_QOS_MIN_INFO =			0x02,
	GPDS_QOS_NEG_INFO =			0x03,
	GPDS_PDP_ADDRESS_INFO =			0x04,
	GPDS_APN_INFO =				0x05,
	GPDS_QOS99_REQ_INFO =			0x06,
	GPDS_QOS99_MIN_INFO =			0x07,
	GPDS_QOS99_NEG_INFO =			0x08,
	GPDS_TFT_INFO =				0x09,
	GPDS_TFT_FILTER_INFO =			0x0A,
	GPDS_USER_NAME_INFO =			0x0B,
	GPDS_PASSWORD_INFO =			0x0C,
	GPDS_PDNS_ADDRESS_INFO =		0x0D,
	GPDS_SDNS_ADDRESS_INFO =		0x0E,
	GPDS_CHALLENGE_INFO =			0x0F,
	GPDS_DNS_ADDRESS_REQ_INFO =		0x90,
};

enum gpds_status {
	GPDS_ERROR =				0x00,
	GPDS_OK =				0x01,
	GPDS_FAIL =				0x02
};

enum gpds_isi_cause {
	GPDS_CAUSE_UNKNOWN =			0x00,
	GPDS_CAUSE_IMSI =			0x02,
	GPDS_CAUSE_MS_ILLEGAL =			0x03,
	GPDS_CAUSE_ME_ILLEGAL =			0x06,
	GPDS_CAUSE_GPRS_NOT_ALLOWED =		0x07,
	GPDS_NOT_ALLOWED =			0x08,
	GPDS_CAUSE_MS_IDENTITY =		0x09,
	GPDS_CAUSE_DETACH =			0x0A,
	GPDS_PLMN_NOT_ALLOWED =			0x0B,
	GPDS_LA_NOT_ALLOWED =			0x0C,
	GPDS_ROAMING_NOT_ALLOWED =		0x0D,
	GPDS_CAUSE_GPRS_NOT_ALLOWED_IN_PLMN =	0x0E,
	GPDS_CAUSE_MSC_NOT_REACH =		0x10,
	GPDS_CAUSE_PLMN_FAIL =			0x11,
	GPDS_CAUSE_NETWORK_CONGESTION =		0x16,
	GPDS_CAUSE_MBMS_BEARER_CAPABILITY_INSUFFICIENT = 0x18,
	GPDS_CAUSE_LLC_SNDCP_FAILURE =		0x19,
	GPDS_CAUSE_RESOURCE_INSUFF =		0x1A,
	GPDS_CAUSE_APN =			0x1B,
	GPDS_CAUSE_PDP_UNKNOWN =		0x1C,
	GPDS_CAUSE_AUTHENTICATION =		0x1D,
	GPDS_CAUSE_ACT_REJECT_GGSN =		0x1E,
	GPDS_CAUSE_ACT_REJECT =			0x1F,
	GPDS_CAUSE_SERV_OPT_NOT_SUPPORTED =	0x20,
	GPDS_CAUSE_SERV_OPT_NOT_SUBSCRIBED =	0x21,
	GPDS_CAUSE_SERV_OPT_OUT_OF_ORDER =	0x22,
	GPDS_CAUSE_NSAPI_ALREADY_USED =		0x23,
	GPDS_CAUSE_DEACT_REGULAR =		0x24,
	GPDS_CAUSE_QOS =			0x25,
	GPDS_CAUSE_NETWORK_FAIL =		0x26,
	GPDS_CAUSE_REACTIVATION_REQ =		0x27,
	GPDS_CAUSE_FEAT_NOT_SUPPORTED =		0x28,
	GPDS_CAUSE_TFT_SEMANTIC_ERROR =		0x29,
	GPDS_CAUSE_TFT_SYNTAX_ERROR =		0x2A,
	GPDS_CAUSE_CONTEXT_UNKNOWN =		0x2B,
	GPDS_CAUSE_FILTER_SEMANTIC_ERROR =	0x2C,
	GPDS_CAUSE_FILTER_SYNTAX_ERROR =	0x2D,
	GPDS_CAUSE_CONT_WITHOUT_TFT =		0x2E,
	GPDS_CAUSE_MULTICAST_MEMBERSHIP_TIMEOUT = 0x2F,
	GPDS_CAUSE_INVALID_MANDATORY_INFO =	0x60,
	GPDS_CAUSE_MSG_TYPE_NON_EXISTENTOR_NOT_IMPLTD =	0x61,
	GPDS_CAUSE_MSG_TYPE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE = 0x62,
	GPDS_CAUSE_IE_NON_EXISTENT_OR_NOT_IMPLEMENTED =	0x63,
	GPDS_CAUSE_CONDITIONAL_IE_ERROR =	0x64,
	GPDS_CUASEMSG_NOT_COMPATIBLE_WITH_PROTOCOL_STATE = 0x65,
	GPDS_CAUSE_UNSPECIFIED =		0x6F,
	GPDS_CAUSE_APN_INCOMPATIBLE_WITH_CURR_CTXT = 0x70,
	GPDS_CAUSE_FDN =			0xA0,
	GPDS_CAUSE_USER_ABORT =			0xA1,
	GPDS_CAUSE_CS_INACTIVE =		0xA2,
	GPDS_CAUSE_CSD_OVERRIDE =		0xA3,
	GPDS_CAUSE_APN_CONTROL =		0xA4,
	GPDS_CAUSE_CALL_CONTROL =		0xA5,
	GPDS_CAUSE_TEMPERATURE_LIMIT =		0xA6,
	GPDS_CAUSE_RETRY_COUNTER_EXPIRED =	0xC8,
	GPDS_CAUSE_NO_CONNECTION =		0xC9,
	GPDS_CAUSE_DETACHED =			0xF5,
	GPDS_CAUSE_NO_SERVICE_POWER_SAVE =	0xF7,
	GPDS_CAUSE_SIM_REMOVED =		0xF9,
	GPDS_CAUSE_POWER_OFF =			0xFA,
	GPDS_CAUSE_LAI_FORBIDDEN_NATIONAL_ROAM_LIST = 0xFB,
	GPDS_CAUSE_LAI_FORBIDDEN_REG_PROVISION_LIST = 0xFC,
	GPDS_CAUSE_ACCESS_BARRED =		0xFD,
	GPDS_CAUSE_FATAL_FAILURE =		0xFE,
	GPDS_CAUSE_AUT_FAILURE =		0xFF,
};

enum gpds_transfer_status {
	GPDS_TRANSFER_NOT_AVAIL =		0x00,
	GPDS_TRANSFER_AVAIL =			0x01,
};

enum gpds_transfer_cause {
	GPDS_TRANSFER_CAUSE_ATTACHED =			0x02,
	GPDS_TRANSFER_CAUSE_DETACHED =			0x03,
	GPDS_TRANSFER_CAUSE_RESUMED =			0x04,
	GPDS_TRANSFER_CAUSE_SUSPENDED_NO_COVERAGE =	0x05,
	GPDS_TRANSFER_CAUSE_SUSPENDED_CALL_SMS =	0x07,
	GPDS_TRANSFER_CAUSE_SUSPENDED_CALL =		0x08,
	GPDS_TRANSFER_CAUSE_SUSPENDED_RAU =		0x09,
	GPDS_TRANSFER_CAUSE_SUSPENDED_LU =		0x0A,
	GPDS_TRANSFER_CAUSE_DSAC_RESTRICTION =		0x0B,
};

enum gpds_context_type {
	GPDS_CONT_TYPE_NORMAL =			0x00,
	GPDS_CONT_TYPE_NWI =			0x01,
	GPDS_CONT_TYPE_SEC =			0x02
};

enum gpds_ppp_mode {
	GPDS_LL_FRAMED_PPP =			0x00,
	GPDS_LL_NONFRAMED_PPP =			0x01,
	GPDS_LL_PLAIN =				0x02
};

enum gpds_pdp_type {
	GPDS_PDP_TYPE_PPP =			0x01,
	GPDS_PDP_TYPE_IPV4 =			0x21,
	GPDS_PDP_TYPE_IPV6 =			0x57,
	GPDS_PDP_TYPE_DEFAULT =			0xFF
};

enum gpds_request_mode {
	GPDS_FOLLOW_OFF =			0x00,
	GPDS_FOLLOW_ON =			0x01
};

enum gpds_attach_status {
	GPDS_DETACHED =				0x00,
	GPDS_ATTACHED =				0x01
};

enum gpds_attach_mode {
	GPDS_ATTACH_MODE_MANUAL =		0x00,
	GPDS_ATTACH_MODE_AUTOMATIC =		0x01,
	GPDS_ATTACH_MODE_DEFAULT =		0xFF
};

enum gpds_mt_act_mode {
	GPDS_MT_ACT_MODE_REJECT =		0x00,
	GPDS_MT_ACT_MODE_ACCEPT =		0x01,
	GPDS_MT_ACT_MODE_DEFAULT =		0xFF
};

enum gpds_classc_mode {
	GPDS_CLASSC_MODE_GPRS =			0x00,
	GPDS_CLASSC_MODE_GSM =			0x01,
	GPDS_CLASSC_MODE_DEFAULT =		0xFF
};

enum gpds_aol_context {
	GPDS_AOL_CTX_NOT_ACTIVE =		0x00,
	GPDS_AOL_CTX_HPLMN_ACTIVE =		0x01,
	GPDS_AOL_CTX_VPLMN_ACTIVE =		0x02,
	GPDS_AOL_CTX_ACTIVE =			0x03,
	GPDS_AOL_CTX_DEFAULT =			0xFF
};

#ifdef __cplusplus
};
#endif

#endif /* !__ISIMODEM_GPDS_H */
