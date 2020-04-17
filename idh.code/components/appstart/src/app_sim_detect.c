/* Copyright (C) 2019 RDA Technologies Limited and/or its affiliates("RDA").
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

#include "app_internal.h"
#include "hal_config.h"
#include "drv_gpio.h"
#include "osi_api.h"
#include "osi_log.h"
#include "app_config.h"

#define CONFIG_SIM_DETECT_DEBOUNCE_TIMEOUT (400) // ms

typedef struct
{
    int sim_id;
    simDetectCB_t cb;
    bool connected;
    bool connect_debounce;
    osiTimer_t *debounce_timer;
    drvGpio_t *gpio;
} simDetect_t;

static void _debounceCB(void *sd_)
{
    simDetect_t *sd = (simDetect_t *)sd_;
    sd->connect_debounce = false;
    if (drvGpioRead(sd->gpio))
    {
        OSI_LOGI(0, "SIM %d Plug In", sd->sim_id);
        if (sd->cb)
            sd->cb(sd->sim_id, true);
    }
}

static void _simDetectCB(void *sd_)
{
    simDetect_t *sd = (simDetect_t *)sd_;
    bool connect = drvGpioRead(sd->gpio);

    if (connect == sd->connected)
        return;

#ifdef CONFIG_APP_SSIM_SUPPORT
    OSI_LOGI(0, "Soft sim hot plug");
    osiShutdown(OSI_SHUTDOWN_RESET);
    return;
#endif

    sd->connected = connect;
    if (sd->connected)
    {
        sd->connect_debounce = true;
        osiTimerStart(sd->debounce_timer, CONFIG_SIM_DETECT_DEBOUNCE_TIMEOUT);
    }
    else
    {
        if (sd->connect_debounce)
            osiTimerStop(sd->debounce_timer);
        else
        {
            OSI_LOGI(0, "SIM %d Plug Out", sd->sim_id);
            if (sd->cb)
                sd->cb(sd->sim_id, false);
        }
        sd->connect_debounce = false;
    }
}

static bool _simDetectInit(simDetect_t *sd, int gpio_id)
{
    drvGpioConfig_t cfg = {
        .mode = DRV_GPIO_INPUT,
        .intr_enabled = false,
        .intr_level = false,
        .rising = true,
        .falling = true,
        .debounce = true,
    };

    sd->debounce_timer = osiTimerCreate(NULL, _debounceCB, sd);
    if (sd->debounce_timer == NULL)
        return false;

    sd->gpio = drvGpioOpen(gpio_id, &cfg, _simDetectCB, sd);
    if (sd->gpio == NULL)
        goto gpio_open_fail;
    sd->connected = drvGpioRead(sd->gpio);
    cfg.intr_enabled = true;
    if (drvGpioReconfig(sd->gpio, &cfg))
        return true;

    drvGpioClose(sd->gpio);
    sd->gpio = NULL;

gpio_open_fail:
    osiTimerDelete(sd->debounce_timer);
    return false;
}

static simDetect_t gSim[2] = {};
bool appSimDetectRegister(int id, int gpio_id, simDetectCB_t cb)
{
    if (id != 1 && id != 2)
        return false;

    simDetect_t *sim = &gSim[id - 1];
    if (sim->gpio != NULL)
    {
        drvGpioClose(sim->gpio);
        osiTimerDelete(sim->debounce_timer);
    }
    sim->debounce_timer = NULL;
    sim->gpio = NULL;
    sim->sim_id = id;
    sim->cb = cb;
    return _simDetectInit(sim, gpio_id);
}
