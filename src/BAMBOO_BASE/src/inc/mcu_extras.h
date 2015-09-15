/******************************************************************************
**  COPYRIGHT  2007 Marvell Inernational Ltd.
**  All Rights Reserved
******************************************************************************/

// mcu_extras.h:
// these are definitions that should have been in mcu.h.
// remove these from here if they appear in updates to mcu.h


#ifndef __MCU_EXTRAS__
#define __MCU_EXTRAS__

#include "mcu.h"

// note: indented "defines" are not used during init...

// FIXME: missing from mcu.h:
#define MCU_BASE 0xB0000000
  #define	MCU_PHY_CONTROL_11					0x0210	/* 32 bit	PHY Control Register 11 */
  #define	MCU_SDRAM_CONTROL_6					0x0760	/* 32 bit	SDRAM Control Register 6 */
  #define	MCU_SDRAM_CONTROL_7					0x0770	/* 32 bit	SDRAM Control Register 7 */
// end of missing from mch.h

#define MCU_REG_SDRAM_CONFIG_0				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_CONFIG_0         ))
#define MCU_REG_SDRAM_CONFIG_1				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_CONFIG_1         ))
#define MCU_REG_SDRAM_TIMING_1				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_TIMING_1         ))
#define MCU_REG_SDRAM_TIMING_2				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_TIMING_2         ))
#define MCU_REG_SDRAM_TIMING_3				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_TIMING_3         ))
#define MCU_REG_SDRAM_TIMING_4				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_TIMING_4         ))
#define MCU_REG_SDRAM_TIMING_5				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_TIMING_5         ))
#define MCU_REG_SDRAM_TIMING_6				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_TIMING_6         ))
#define MCU_REG_SDRAM_CONTROL_1				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_CONTROL_1        ))
#define MCU_REG_SDRAM_CONTROL_2				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_CONTROL_2        ))
#define MCU_REG_SDRAM_CONTROL_3				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_CONTROL_3        ))
#define MCU_REG_SDRAM_CONTROL_4				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_CONTROL_4        ))
#define MCU_REG_SDRAM_CONTROL_5				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_CONTROL_5        ))
#define MCU_REG_SDRAM_CONTROL_6				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_CONTROL_6        ))
#define MCU_REG_SDRAM_CONTROL_7				((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_CONTROL_7        ))
#define MCU_REG_SDRAM_CONTROL_13	 		((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_CONTROL_13       ))
#define MCU_REG_SDRAM_CONTROL_14	 		((unsigned volatile long*)( MCU_BASE + MCU_SDRAM_CONTROL_14       ))

#define MCU_REG_ERROR_STATUS				((unsigned volatile long*)( MCU_BASE + MCU_ERROR_STATUS         ))

#define MCU_REG_MMU_MMAP0					((unsigned volatile long*)( MCU_BASE + MCU_MMAP0                  ))
#define MCU_REG_MMU_MMAP1					((unsigned volatile long*)( MCU_BASE + MCU_MMAP1                  ))

#define MCU_REG_USER_INITIATED_COMMAND		((unsigned volatile long*)( MCU_BASE + MCU_USER_INITIATED_COMMAND ))

#define	MCU_REG_DRAM_STATUS					((unsigned volatile long*)( MCU_BASE + MCU_DRAM_STATUS            ))
#define MCU_REG_PHY_CONTROL_3				((unsigned volatile long*)( MCU_BASE + MCU_PHY_CONTROL_3          ))
#define MCU_REG_PHY_CONTROL_7				((unsigned volatile long*)( MCU_BASE + MCU_PHY_CONTROL_7          ))
#define MCU_REG_PHY_CONTROL_8				((unsigned volatile long*)( MCU_BASE + MCU_PHY_CONTROL_8          ))
#define MCU_REG_PHY_CONTROL_9				((unsigned volatile long*)( MCU_BASE + MCU_PHY_CONTROL_9          ))

#define MCU_REG_PHY_CONTROL_10				((unsigned volatile long*)( MCU_BASE + MCU_PHY_CONTROL_10          ))
#define MCU_REG_PHY_CONTROL_11				((unsigned volatile long*)( MCU_BASE + MCU_PHY_CONTROL_11         ))
#define MCU_REG_PHY_CONTROL_12				((unsigned volatile long*)( MCU_BASE + MCU_PHY_CONTROL_11         ))
#define MCU_REG_PHY_CONTROL_13				((unsigned volatile long*)( MCU_BASE + MCU_PHY_CONTROL_13         ))
#define MCU_REG_PHY_CONTROL_14				((unsigned volatile long*)( MCU_BASE + MCU_PHY_CONTROL_14         ))

#define MCU_REG_PHY_DLL_CONTROL_1			((unsigned volatile long*)( MCU_BASE + MCU_PHY_DLL_CONTROL_1      ))

#define MCU_REG_PHY_CONTROL_TEST			((unsigned volatile long*)( MCU_BASE + MCU_PHY_CONTROL_TEST         ))

#define MCU_REG_TEST_MODE_0					((unsigned volatile long*)( MCU_BASE + MCU_TEST_MODE_0          ))
#define MCU_REG_TEST_MODE_1					((unsigned volatile long*)( MCU_BASE + MCU_TEST_MODE_1          ))

#define MCU_REG_MCB_CONTROL_1				((unsigned volatile long*)( MCU_BASE + MCU_MCB_CONTROL_1          ))
#define MCU_REG_MCB_CONTROL_2				((unsigned volatile long*)( MCU_BASE + MCU_MCB_CONTROL_2          ))
#define MCU_REG_MCB_CONTROL_3				((unsigned volatile long*)( MCU_BASE + MCU_MCB_CONTROL_3          ))
#define MCU_REG_MCB_CONTROL_4				((unsigned volatile long*)( MCU_BASE + MCU_MCB_CONTROL_4          ))

#define MCU_REG_PERF_COUNT_CONTROL_0		((unsigned volatile long*)( MCU_BASE + MCU_PERF_COUNT_CONTROL_0   ))
#define MCU_REG_PERF_COUNT_CONTROL_1		((unsigned volatile long*)( MCU_BASE + MCU_PERF_COUNT_CONTROL_1   ))

#define MCU_REG_PERF_COUNT_STAT				((unsigned volatile long*)( MCU_BASE + MCU_PERF_COUNT_STAT   ))
#define MCU_REG_PERF_COUNT_SEL				((unsigned volatile long*)( MCU_BASE + MCU_PERF_COUNT_SEL   ))
#define MCU_REG_PERF_COUNTER				((unsigned volatile long*)( MCU_BASE + MCU_PERF_COUNTER   ))
//#define MCU_REG_PHY_DLL_CONTROL_2			((unsigned volatile long*)( MCU_BASE + MCU_PHY_DLL_CONTROL_2      ))
//#define MCU_REG_PHY_DLL_CONTROL_3			((unsigned volatile long*)( MCU_BASE + MCU_PHY_DLL_CONTROL_3      ))


#define ASPEN_DEFAULT_DCLK					156000000
#define ASPEN_DEFAULT_FCLK					156000000		// for refresh cycle calculations


#endif
