﻿#include "lvgl.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

const LV_ATTRIBUTE_MEM_ALIGN uint8_t ic_qs_gps_on_sprd_map[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
  0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x12, 0x04, 0x03, 0x00, 0x00, 0x00, 0xa4, 0xe0, 0x93,
  0x64, 0x00, 0x00, 0x00, 0x03, 0x73, 0x42, 0x49, 0x54, 0x08, 0x08, 0x08, 0xdb, 0xe1, 0x4f, 0xe0,
  0x00, 0x00, 0x00, 0x24, 0x50, 0x4c, 0x54, 0x45, 0xff, 0xff, 0xff, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa,
  0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa,
  0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0x74, 0xea, 0x87, 0x50,
  0x00, 0x00, 0x00, 0x0c, 0x74, 0x52, 0x4e, 0x53, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x88,
  0x99, 0xcc, 0xdd, 0xff, 0x0e, 0x95, 0xa4, 0x91, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73,
  0x00, 0x00, 0x1c, 0x20, 0x00, 0x00, 0x1c, 0x20, 0x01, 0xcd, 0x0f, 0x9b, 0x9e, 0x00, 0x00, 0x00,
  0x1f, 0x74, 0x45, 0x58, 0x74, 0x53, 0x6f, 0x66, 0x74, 0x77, 0x61, 0x72, 0x65, 0x00, 0x4d, 0x61,
  0x63, 0x72, 0x6f, 0x6d, 0x65, 0x64, 0x69, 0x61, 0x20, 0x46, 0x69, 0x72, 0x65, 0x77, 0x6f, 0x72,
  0x6b, 0x73, 0x20, 0x38, 0xb5, 0x68, 0xd2, 0x78, 0x00, 0x00, 0x00, 0x5c, 0x49, 0x44, 0x41, 0x54,
  0x08, 0x99, 0x63, 0x60, 0xc0, 0x01, 0xca, 0xa1, 0x34, 0x63, 0xe5, 0xee, 0xe9, 0x02, 0x60, 0x96,
  0xd6, 0x34, 0xc1, 0xc8, 0x45, 0x60, 0xa1, 0x15, 0x0c, 0xc6, 0x0c, 0x5d, 0x20, 0x41, 0xf6, 0x05,
  0x9a, 0xbb, 0x27, 0x73, 0x15, 0x00, 0x59, 0x5c, 0x01, 0xab, 0x77, 0xef, 0x62, 0x5d, 0x00, 0x64,
  0x55, 0x0b, 0xef, 0xde, 0xbd, 0xdb, 0x70, 0x3b, 0x8c, 0xe5, 0xb8, 0x1d, 0x45, 0x96, 0x7d, 0x81,
  0xd5, 0xee, 0xc5, 0x60, 0x1d, 0x40, 0x53, 0x4c, 0x18, 0xba, 0x20, 0x26, 0x2f, 0x13, 0xcc, 0x5c,
  0x84, 0x66, 0x1b, 0xc2, 0x05, 0x18, 0x00, 0x00, 0x8d, 0x47, 0x1e, 0x29, 0x26, 0x44, 0x24, 0x9e,
  0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82,
};

lv_img_dsc_t ic_qs_gps_on_sprd = {
  .header.always_zero = 0,
  .header.w = 18,
  .header.h = 18,
  .data_size = 300,
  .header.cf = LV_IMG_CF_RAW_ALPHA,
  .data = ic_qs_gps_on_sprd_map,
};