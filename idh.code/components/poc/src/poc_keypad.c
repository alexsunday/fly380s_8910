﻿/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
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

// #define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG

#include "poc_keypad.h"
#ifdef CONFIG_POC_KEYPAD_SUPPORT

#include "osi_log.h"
#include "osi_api.h"
#include "osi_pipe.h"
#include "ml.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lv_include/lv_poc_type.h"
#include "lv_include/lv_poc_lib.h"
#include "guiBndCom_api.h"
#include "lv_include/lv_poc.h"
#include "lv_gui_main.h"
#include "poc_audio_recorder.h"

#define POC_RECORD_OR_SPEAK_CALL 1//1-正常对讲，0-自录自播
#define POC_RECORDER_PLAY_MODE 1//1-play 0-poc

static lv_indev_state_t preKeyState = 0xff;
static uint32_t   preKey      = 0xff;
static lv_indev_state_t prvPttKeyState = 0xff;
static lv_indev_state_t prvPowerKeyState = 0xff;
static osiTimer_t * prvPowerTimer = NULL;
static bool isReadyPowerOff = false;

static void poc_power_on_charge_set_lcd_status(uint8_t lcdstatus);

static void prvPowerKeyCb(void *ctx)
{
	isReadyPowerOff = true;
	if(!lv_poc_charge_poweron_status())//正常开机
	{
		lv_poc_refr_task_once(lv_poc_shutdown_note_activity_open,
			LVPOCLISTIDTCOM_LIST_PERIOD_10, LV_TASK_PRIO_HIGH);
	}
	else//充电开机
	{
		osiSetBootCause(OSI_BOOTCAUSE_PWRKEY);/*as reboot*/
		osiShutdown(OSI_SHUTDOWN_RESET);//重启设备
	}
}

bool pocKeypadHandle(uint32_t id, lv_indev_state_t state, void *p)
{
	bool ret = false;
	if(id == LV_GROUP_KEY_POC) //poc
	{
		if(prvPttKeyState != state)// && !lvPocGuiIdtCom_listen_status())/*当正在接听是ppt键无效*/
		{
			if(state == LV_INDEV_STATE_PR)
			{
				#if POC_RECORD_OR_SPEAK_CALL
				OSI_LOGI(0, "[gic][gicmic] send LVPOCGUIIDTCOM_SIGNAL_SPEAK_START_IND\n");
				lvPocGuiBndCom_Msg(LVPOCGUIIDTCOM_SIGNAL_SPEAK_START_IND, NULL);
				#else
					#if POC_RECORDER_PLAY_MODE
					lv_poc_start_recordwriter();//自录
					#else
					lv_poc_start_recordwriter_pocmode();
					#endif
				#endif
			}
			else
			{
				#if POC_RECORD_OR_SPEAK_CALL
				OSI_LOGI(0, "[gic][gicmic] send LVPOCGUIIDTCOM_SIGNAL_SPEAK_STOP_IND\n");
				lvPocGuiBndCom_Msg(LVPOCGUIIDTCOM_SIGNAL_SPEAK_STOP_IND, NULL);
				#else
					#if POC_RECORDER_PLAY_MODE
					lv_poc_start_playfile();//自播
					#else
					lv_poc_start_playfile_pocmode();
					#endif
				#endif
			}
		}
		prvPttKeyState = state;
		ret = false;
	}
	else if(id == 0xf0)
	{
		if(prvPowerTimer == NULL)
		{
			prvPowerTimer = osiTimerCreate(NULL, prvPowerKeyCb, NULL);
		}

		if(prvPowerKeyState != state)
		{
			if(state == LV_INDEV_STATE_PR)
			{
				if(prvPowerTimer != NULL)
				{
					osiTimerStart(prvPowerTimer, LONGPRESS_SHUTDOWN_TIME);
					isReadyPowerOff = false;
				}
			}
			else
			{
				if(prvPowerTimer != NULL)
				{
		            if (isReadyPowerOff)
		            {
		                //osiDelayUS(1000 * 100);
		                //osiShutdown(OSI_SHUTDOWN_POWER_OFF);
		            }
		            else
		            {
						osiTimerStop(prvPowerTimer);
						 if(lv_poc_charge_poweron_status())//充电开机
						{
							poc_power_on_charge_set_lcd_status(!poc_get_lcd_status());
						}
						 else
						{
							poc_set_lcd_status(!poc_get_lcd_status());
						}
		            }
				}
			}
		}
		prvPowerKeyState = state;
		ret = true;
	}
	preKey = id;
	preKeyState = state;
	return ret;
}

bool pocGetPttKeyState(void)
{
	return prvPttKeyState == KEY_STATE_PRESS ? true : false;
}

/*
	  name : poc_power_on_charge_set_lcd_status
	 param : none
	author : wangls
  describe : 充电时开关屏幕
	  date : 2020-07-10
*/
static
void poc_power_on_charge_set_lcd_status(uint8_t lcdstatus)
{

	if(lcdstatus != 0)
	{
		lvGuiChargeScreenOn();
	}
	else
	{
		lvGuiScreenOff();
	}

}

#endif
