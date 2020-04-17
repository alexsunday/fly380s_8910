/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#include "cfw_config.h"
#include "cfw_event.h"
#include "osi_log.h"
#include "osi_api.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef CONFIG_EVENT_NAME_ENABLED

static const osiValueStrMap_t gCfwEventNames[] = {
    {OSI_VSMAP_CONST_DECL(EV_SHL_CFW_STATUS_IND)}, // 10
    {OSI_VSMAP_CONST_DECL(EV_COS_CALLBACK)},
    {OSI_VSMAP_CONST_DECL(EV_MESSAGE_EVENT)},

    {OSI_VSMAP_CONST_DECL(EV_DM_POWER_ON_IND)}, // 51
    {OSI_VSMAP_CONST_DECL(EV_DM_POWER_OFF_IND)},
    {OSI_VSMAP_CONST_DECL(EV_TIM_ALRAM_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_UART_RECEIVE_DATA_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_EXTI_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_GPIO_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_RECORD_END_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_STREAM_END_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_GPADC_IND)},
    {OSI_VSMAP_CONST_DECL(EV_TIM_SET_TIME_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_SPEECH_IND)},

    {OSI_VSMAP_CONST_DECL(EV_DM_AUDIO_CONTROL_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_SET_AUDIO_VOLUME_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_SET_MIC_GAIN_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_SET_AUDIO_MODE_IND)},

    {OSI_VSMAP_CONST_DECL(EV_DM_AUD_SETUP_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_AUD_STREAM_START_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_AUD_STREAM_STOP_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_AUD_STREAM_PAUSE_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_AUD_STREAM_RECORD_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_AUD_TEST_MODE_SETUP_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_AUD_FORCE_RECV_MIC_SEL_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_AUD_SET_CUR_OUTPUT_DEVICE_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_AUD_MUTE_OUTPUT_DEVICE_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_AUD_LOUDSPK_WITH_EP_IND)},

    {OSI_VSMAP_CONST_DECL(EV_DM_VOIS_RECORD_START_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_VOIS_RECORD_STOP_IND)},

    {OSI_VSMAP_CONST_DECL(EV_DM_ARS_SETUP_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_ARS_RECORD_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_ARS_PAUSE_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_ARS_STOP_IND)},

    {OSI_VSMAP_CONST_DECL(EV_DM_READ_IPC_DATA_IND)},

    {OSI_VSMAP_CONST_DECL(EV_KEY_DOWN)},
    {OSI_VSMAP_CONST_DECL(EV_KEY_PRESS)},
    {OSI_VSMAP_CONST_DECL(EV_KEY_UP)},
    {OSI_VSMAP_CONST_DECL(EV_TIMER)},
    {OSI_VSMAP_CONST_DECL(EV_DOUBLE_KEY_DOWN)},
    {OSI_VSMAP_CONST_DECL(EV_DOUBLE_KEY_UP)},
    {OSI_VSMAP_CONST_DECL(EV_DM_UART_MUX_RECEIVE_DATA_IND)},
    {OSI_VSMAP_CONST_DECL(EV_DM_READ_UART_DATA_IND)},

    {OSI_VSMAP_CONST_DECL(EV_PM_BC_IND)}, // 101

    {OSI_VSMAP_CONST_DECL(EV_CFW_NW_SET_BAND_RSP)}, // 501
    {OSI_VSMAP_CONST_DECL(EV_CFW_NW_GET_IMEI_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_NW_GET_SIGNAL_QUALITY_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_NW_SET_REGISTRATION_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_NW_DEREGISTER_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_NW_GET_AVAIL_OPERATOR_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_NW_ABORT_LIST_OPERATORS_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SET_COMM_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_NW_STORELIST_IND)},
    // {OSI_VSMAP_CONST_DECL(EV_CFW_XCPU_TEST_RSP)},
    // {OSI_VSMAP_CONST_DECL(EV_CFW_WCPU_TEST_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_NW_GET_QSCANF_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_NW_CELL_LIST_INFO_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_NW_GET_FREQSCAN_RSP)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP)}, // 551
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_INITIATE_SPEECH_CALL_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_RELEASE_CALL_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_CALL_SWAP_RSP)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_QUERY_CLIP_RSP)}, // 601
    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_SET_CALL_WAITING_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_QUERY_CALL_WAITING_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_SET_CALL_FORWARDING_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_QUERY_CALL_FORWARDING_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_SET_FACILITY_LOCK_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_GET_FACILITY_LOCK_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_CHANGE_PWD_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_QUERY_FACILITY_LOCK_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_QUERY_CLIR_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_QUERY_COLP_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_SEND_USSD_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_TERMINATE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_QUERY_COLR_RSP)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_ADD_PBK_RSP)}, // 701
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_DELETE_PBK_ENTRY_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_PBK_ENTRY_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_LIST_PBK_ENTRY_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_PBK_STRORAGE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_PROVIDER_ID_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_CHANGE_PWD_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_AUTH_STATUS_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_ENTER_AUTH_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_SET_FACILITY_LOCK_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_FACILITY_LOCK_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_DELETE_MESSAGE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_WRITE_MESSAGE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_READ_MESSAGE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_LIST_MESSAGE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_READ_BINARY_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_UPDATE_BINARY_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_MR_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_SET_MR_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_COMPOSE_PDU_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SAT_ACTIVATION_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SAT_GET_STATUS_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SAT_RESPONSE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SAT_GET_INFORMATION_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_PREF_OPT_LIST_MAXNUM_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_SET_PBKMODE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_PBKMODE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_INIT_SMS_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_RESET_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_PUCT_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_SET_PUCT_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_ACMMAX_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_SET_ACMMAX_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_SET_ACM_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_ACM_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_READ_EF_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_FILE_STATUS_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_READ_RECORD_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_UPDATE_RECORD_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_SPN_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_SMS_TOTAL_NUM_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_ICCID_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_CLOSE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_SEARCH_PBK_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_POWER_OFF_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_READ_ELEMENTARY_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_GET_EID_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_READ_VOICEMIAL_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_UPDATE_VOICEMIAL_RSP)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_DELETE_MESSAGE_RSP)}, // 761
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_GET_STORAGE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_SET_STORAGE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_LIST_MESSAGE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_READ_MESSAGE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_SEND_MESSAGE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_SET_CENTER_ADDR_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_GET_CENTER_ADDR_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_SET_TEXT_MODE_PARAM_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_GET_TEXT_MODE_PARAM_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_WRITE_MESSAGE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_COPY_MESSAGE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_SET_UNREAD2READ_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_SET_UNSENT2SENT_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_SET_READ2UNREAD_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_SET_PARAM_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_PATCH_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CB_READ_SIM_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_MOVE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CB_SET_MID_SIM_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_PATCHEX_RSP)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_ADD_RSP)}, // 801
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_UPDATE_ENTRY_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_DELETE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_DELETE_BATCH_ENTRIES_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_FIND_ENTRY_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_LIST_ENTRIES_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_GET_ENTRY_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_COPY_ENTRIES_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_ADD_ENTRY_ITEM_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_GET_ENTRY_ITEMS_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_DELETE_ENTRY_ITEM_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_ADD_CALLLOG_ENTTRY_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_GET_CALLLOG_ENTRY_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_LIST_CALLLOG_ENTRIES_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_DELETE_CALLLOG_ALL_ENTRIES_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_CANCEL_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_DELETE_CALLLOG_OLDEST_ENTRY_RSP)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_CTX_MODIFY_ACC_RSP)}, // 851
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_ATT_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_ACT_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_CXT_ACTIVE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_MOD_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_WIFI_CONNECTED_RSP)},

    {OSI_VSMAP_CONST_DECL(EV_TCPIP_CFW_GPRS_ACT)},
    {OSI_VSMAP_CONST_DECL(EV_TCPIP_CFW_GPRS_DEACT)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_TCPIP_SOCKET_CONNECT_RSP)}, // 1001
    {OSI_VSMAP_CONST_DECL(EV_CFW_TCPIP_SOCKET_CLOSE_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_TCPIP_SOCKET_SEND_RSP)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_FTP_LONGIN_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_FTP_FILE_DOWNLOAD_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_FTP_LIST_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_FTP_CHANGEDIR_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_FTP_REC_DATA_IND)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_ME_SET_FACILITY_LOCK_RSP)}, // 1101
    {OSI_VSMAP_CONST_DECL(EV_CFW_ME_GET_FACILITY_LOCK_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_ME_CHANGE_PWD_RSP)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_TSM_INFO_IND)}, // 1151
    {OSI_VSMAP_CONST_DECL(EV_CFW_TSM_INFO_END_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_TSM_SYNC_INFO_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_TSM_SYNC_INFO_END_RSP)},

    {OSI_VSMAP_CONST_DECL(EV_RMT_STTPC_REQ)}, // 1201
    {OSI_VSMAP_CONST_DECL(EV_RMT_STTPC_RSP)},

    {OSI_VSMAP_CONST_DECL(EV_RMT_APP_REQ)},
    {OSI_VSMAP_CONST_DECL(EV_RMT_APP_RSP)},

    {OSI_VSMAP_CONST_DECL(EV_DIAG_RECEIVE_DATA)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_WIFI_INIT_RSP)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_WIFI_DEINIT_RSP)},

    {OSI_VSMAP_CONST_DECL(EV_BT_TETHER_DATA_IND)}, // 1401

    {OSI_VSMAP_CONST_DECL(EV_CFW_INIT_IND)}, // 5001
    {OSI_VSMAP_CONST_DECL(EV_CFW_SRV_STATUS_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_NW_SIGNAL_QUALITY_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_NW_REG_STATUS_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_SPEECH_CALL_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_CRSSINFO_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_NEW_SMS_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SMS_INFO_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_CALL_INFO_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SIM_STATUS_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SS_USSD_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_RELEASE_CALL_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_ERROR_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_SAT_CMDTYPE_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PBK_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_NW_NETWORKINFO_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_CXT_ACTIVE_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_CXT_DEACTIVE_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_MOD_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_STATUS_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_DATA_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_CTRL_RELEASE_IND)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_TCPIP_REV_DATA_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_TCPIP_CLOSE_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_TCPIP_ERR_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_TCPIP_ACCEPT_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_ICMP_DATA_IND)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_DNS_RESOLV_SUC_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_DNS_RESOLV_ERR_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PPP_DATA_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_PPP_TERMINATE_IND)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_NEW_EMS_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_ATT_STATUS_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_STATUS_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_PROGRESS_IND)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_CB_MSG_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CB_PAGE_IND)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_BLUETOOTH_DATA_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_BLUETOOTH_WAKEUP_IND)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_REG_UPDATE_IND)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_NEW_SMS_PDU_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_ICMPV4_DATA_REQ)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_ICMPV4_DATA_RPY)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_EXIT_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_ERRC_CONNSTATUS_IND)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_PDN_CTX_ACTIVE_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_SEC_CTX_ACTIVE_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_SEC_CTX_DEACTIVE_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_CTX_MODIFY_IND)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_EMC_NUM_LIST_IND)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_EDRX_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_CCIOTOPT_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_APNCR_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_CRTDCP_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_GPRS_NONIPDATA_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_IPERF_REV_DATA_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CALL_CONTROL_CCM_IND)},

    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_AUDIOON_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_ALERT_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_IMS_STATE_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_CALL_PATH_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_CALL_TI_ASSIGNED_IND)},
    {OSI_VSMAP_CONST_DECL(EV_CFW_CC_AUDIO_RESTART_IND)},
};
#endif

const char *CFW_EventName(uint32_t id)
{
#ifdef CONFIG_EVENT_NAME_ENABLED
#if 0 // When new event names are added, open this piece of code to check order
    if (!osiVsmapIsSorted(gCfwEventNames, OSI_ARRAY_SIZE(gCfwEventNames)))
        osiPanic();
#endif
    return osiVsmapBsearch(id, gCfwEventNames, OSI_ARRAY_SIZE(gCfwEventNames),
                           "unknown event");
#else
    return "";
#endif
}
