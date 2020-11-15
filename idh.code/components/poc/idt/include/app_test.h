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

#ifndef _APP_TEST_H_
#define _APP_TEST_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "osi_api.h"
#include "osi_event_hub.h"
#include "osi_generic_list.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief opaque data structure of AT engine
 */
void appTestStart(void);

void lvPocGuiIdtComTest_Init();

#ifdef __cplusplus
}
#endif

#endif