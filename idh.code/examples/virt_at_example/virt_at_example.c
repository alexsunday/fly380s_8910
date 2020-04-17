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

#define OSI_LOG_TAG OSI_MAKE_LOG_TAG('T', 'V', 'A', 'T')

#include "osi_api.h"
#include "osi_log.h"
#include "osi_pipe.h"
#include "at_engine.h"
#include <string.h>

static void prvVirtAtRespCallback(void *param, unsigned event)
{
    osiPipe_t *pipe = (osiPipe_t *)param;
    char buf[256];
    for (;;)
    {
        int bytes = osiPipeRead(pipe, buf, 255);
        if (bytes <= 0)
            break;

        buf[bytes] = '\0';
        OSI_LOGXI(OSI_LOGPAR_IS, 0, "VAT1 <--(%d): %s", bytes, buf);
    }
}

static void prvVirtAtEntry(void *param)
{
    osiPipe_t *at_rx_pipe = osiPipeCreate(1024);
    osiPipe_t *at_tx_pipe = osiPipeCreate(1024);
    osiPipeSetReaderCallback(at_tx_pipe, OSI_PIPE_EVENT_RX_ARRIVED,
                             prvVirtAtRespCallback, at_tx_pipe);

    atDeviceVirtConfig_t cfg = {
        .name = OSI_MAKE_TAG('V', 'A', 'T', '1'),
        .rx_pipe = at_rx_pipe,
        .tx_pipe = at_tx_pipe,
    };
    atDevice_t *device = atDeviceVirtCreate(&cfg);
    atDispatch_t *dispatch = atDispatchCreate(device);
    atDeviceSetDispatch(device, dispatch);
    atDeviceOpen(device);

    for (;;)
    {
        const char *cmd = "AT\r\n";
        OSI_LOGXI(OSI_LOGPAR_S, 0, "VAT1 -->: %s", cmd);
        osiPipeWriteAll(at_rx_pipe, cmd, strlen(cmd), OSI_WAIT_FOREVER);
        osiThreadSleep(2000);
    }

    osiThreadExit();
}

int appimg_enter(void *param)
{
    OSI_LOGI(0, "application image enter, param 0x%x", param);
    osiThreadCreate("vat", prvVirtAtEntry, NULL, OSI_PRIORITY_NORMAL, 1024, 4);
    return 0;
}

void appimg_exit(void)
{
    OSI_LOGI(0, "application image exit");
}
