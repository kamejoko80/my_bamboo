/******************************************************************************
 *
 *  (C)Copyright 2005 - 2008 Marvell. All Rights Reserved.
 *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF MARVELL.
 *  The copyright notice above does not evidence any actual or intended
 *  publication of such source code.
 *  This Module contains Proprietary Information of Marvell and should be
 *  treated as Confidential.
 *  The information in this file is provided for the exclusive use of the
 *  licensees of Marvell.
 *  Such users have the right to use, modify, and incorporate this code into
 *  products for purposes authorized by the license agreement provided they
 *  include this notice and the associated copyright notice with any such
 *  product.
 *  The information in this file is provided "AS IS" without warranty.
 *
 ******************************************************************************/
/*
 *  Use this file for BootLoader definitions
 */
#include "tim.h"


#define TEMP_BUFFER_AREA (DDR_PHY_ADDR+DDR_SPARE_AREA_ADDR)

#define IMAGE_DOWNLOAD_AREA (DDR_PHY_ADDR+DDR_DOWNLOAD_AREA_ADDR)

//map out DDR for both MBH and images
#define MASTER_BLOCK_HEADER_ADDR IMAGE_DOWNLOAD_AREA
#define FBF_DEVICE_HEADER_TEMP_AREA (IMAGE_DOWNLOAD_AREA+0x2000)
#define MBH_IMAGE_DOWNLOAD_AREA (IMAGE_DOWNLOAD_AREA+0x20000)

#define MY_CONSUMER_ID  OBMIDENTIFIER

typedef enum {
  UPGRADESW = 1,
  DOWNLOAD,
  SINGLE_TIM_BOOT,
  DUAL_TIM_BOOT,
  MAX_OP_MODES
}OPERATING_MODE_T;