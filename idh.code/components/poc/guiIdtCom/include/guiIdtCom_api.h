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

#ifndef _INCLUDE_GUIIDTCOM_API_H_
#define _INCLUDE_GUIIDTCOM_API_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

OSI_EXTERN_C_BEGIN
;
typedef enum
{
	LVPOCGUIIDTCOM_SIGNAL_START = (1 << 8) - 1,
    LVPOCGUIIDTCOM_SIGNAL_LOGIN_IND,
    LVPOCGUIIDTCOM_SIGNAL_EXIT_IND,
    LVPOCGUIIDTCOM_SIGNAL_SPEAK_START_IND,
    LVPOCGUIIDTCOM_SIGNAL_SPEAK_STOP_IND,
    LVPOCGUIIDTCOM_SIGNAL_GROUP_LIST_QUERY_IND,                //+5
    LVPOCGUIIDTCOM_SIGNAL_GROUP_COUNT_QUERY_IND,
    LVPOCGUIIDTCOM_SIGNAL_MEMBER_LIST_QUERY_IND,
    LVPOCGUIIDTCOM_SIGNAL_GROUP_MEMBER_QUERY_IND,
    LVPOCGUIIDTCOM_SIGNAL_SINGLE_CALL_IND,
    LVPOCGUIIDTCOM_SIGNAL_SINGLE_CALL_END_IND,                 //+10
    LVPOCGUIIDTCOM_SIGNAL_GROUP_EXIT_IND,
    LVPOCGUIIDTCOM_SIGNAL_GROUP_SWITCH_IND,
    LVPOCGUIIDTCOM_SIGNAL_MEMBER_INFO_IND,
    LVPOCGUIIDTCOM_SIGNAL_STOP_PLAY_IND,
    LVPOCGUIIDTCOM_SIGNAL_START_PLAY_IND,
    LVPOCGUIIDTCOM_SIGNAL_STOP_RECORD_IND,
    LVPOCGUIIDTCOM_SIGNAL_START_RECORD_IND,
    LVPOCGUIIDTCOM_SIGNAL_WRITE_DATA_IND,
    //
    FLY_PTT_SIG_LOGIN_REP,
    LVPOCGUIIDTCOM_SIGNAL_LOGIN_REP,
    LVPOCGUIIDTCOM_SIGNAL_EXIT_REP,
    LVPOCGUIIDTCOM_SIGNAL_SPEAK_START_REP,
    LVPOCGUIIDTCOM_SIGNAL_SPEAK_STOP_REP,
    LVPOCGUIIDTCOM_SIGNAL_LISTEN_START_REP,
    LVPOCGUIIDTCOM_SIGNAL_LISTEN_STOP_REP,
    LVPOCGUIIDTCOM_SIGNAL_GROUP_LIST_QUERY_REP,                //+5
    LVPOCGUIIDTCOM_SIGNAL_GROUP_COUNT_QUERY_REP,
    LVPOCGUIIDTCOM_SIGNAL_MEMBER_LIST_QUERY_REP,
    LVPOCGUIIDTCOM_SIGNAL_GROUP_MEMBER_QUERY_REP,
    LVPOCGUIIDTCOM_SIGNAL_SINGLE_CALL_REP,
    LVPOCGUIIDTCOM_SIGNAL_SINGLE_CALL_END_REP,                 //+10
    LVPOCGUIIDTCOM_SIGNAL_GROUP_EXIT_REP,
    LVPOCGUIIDTCOM_SIGNAL_GROUP_SWITCH_REP,
    LVPOCGUIIDTCOM_SIGNAL_MEMBER_INFO_REP,
    LVPOCGUIIDTCOM_SIGNAL_END,
} LvPocGuiIdtCom_SignalType_t;

typedef struct
{
	int status;
	unsigned short cause;
} LvPocGuiIdtCom_login_t;

void lvPocGuiIdtCom_Init(void);

bool lvPocGuiIdtCom_Msg(LvPocGuiIdtCom_SignalType_t signal, void * ctx);

void lvPocGuiIdtCom_log(void);


OSI_EXTERN_C_END

#endif
