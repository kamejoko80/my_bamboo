/******************************************************************************
**  COPYRIGHT  2007 Marvell Inernational Ltd.
**  All Rights Reserved
******************************************************************************/


#include "predefines.h"
#include "mcu.h"
#include "mcu_extras.h"
#include "processor_enums.h"
#include "sdram_specs.h"
#include "sdram_support.h"
#include "sdram_registers_defaults.h"
#include "tim.h"
#include "Errors.h"
#include "timer.h"

// This routine will configure and enable SDRam DDR.
// It has the ability to select different DDR frequencies and geometries if the TIM area contains these specifiers.
// If no specifiers are found, then default values will be used.

// Overview of the SDRam DDR initialization:
//
// If TIM SDRam configuration records are available:
// 0. Make sure the required clocks are enabled. (requires TIM Clocks Enable records)
// 1. Make sure the memory voltage planes are up. (requires TIM Voltage records)
// 2. Make sure the clocks are at the correct frequency. (requires TIM Frequency records)
// 3. Program the MCU registers. (can override default values using optional TIM SDRam records
//
// otherwise (when no TIM SDRam configuration records are available)
// 1. Program the MCU registers using hardcoded default values.
//    - no clock enable, voltage or frequency changing occurs in this mode.
//

#define DDR_400MHz

struct sdram_conf
{
    ASPEN_MCU_REGID_T id;
    UINT_T            regValue;
};

const struct sdram_conf Tim_Table[] = {

#if defined(DDR_400MHz)

     {ASPEN_ADRMAPREG0_ID,  0x000C0001}, // Fix
     {ASPEN_SDRCFGREG0_ID,  0x00042330},
     {ASPEN_SDRTMGREG1_ID,  0x4CD700C8},
     {ASPEN_SDRTMGREG2_ID,  0x54560332},
     {ASPEN_SDRTMGREG3_ID,  0xC0C83742},
     {ASPEN_SDRTMGREG4_ID,  0x3538A0A0},
     {ASPEN_SDRTMGREG5_ID,  0x00120121},
     {ASPEN_SDRCTLREG1_ID,  0x00000000},
     {ASPEN_SDRCTLREG2_ID,  0x00080000},
     {ASPEN_SDRCTLREG3_ID,  0xC0000000},
     {ASPEN_SDRCTLREG4_ID,  0x20814004},
     {ASPEN_SDRCTLREG6_ID,  0x00000001},
     {ASPEN_SDRCTLREG7_ID,  0x02000002},
     {ASPEN_PHYCTLREG14_ID, 0x80000000},
     {ASPEN_PHYCTLREG7_ID,  0x177C2779},
     {ASPEN_PHYCTLREG8_ID,  0x07700770}, // Fix
     {ASPEN_PHYCTLREG9_ID,  0x00000077},
     {ASPEN_PHYCTLREG14_ID, 0xa0000000}, // Reset DLL
     {ASPEN_OPDELAY_ID,     10        },
     {ASPEN_PHYCTLREG14_ID, 0x80000000}, // Sync clock
     {ASPEN_OPDELAY_ID,     10        },
     {ASPEN_PHYCTLREG13_ID, 0x20000008 | (2 << 4)}, // update 512 cycles, 90 degree 0x20000108
     {ASPEN_OPDELAY_ID,     10        },
     {ASPEN_DLLCTLREG1_ID,  0x20000008 | (2 << 4)},
     {ASPEN_OPDELAY_ID,     10        },
     {ASPEN_PHYCTLREG14_ID, 0x88000000}, // update en static
     {ASPEN_OPDELAY_ID,     10        },
     {ASPEN_PHYCTLREG14_ID, 0x88000000}, // clear update en static
     {ASPEN_OPDELAY_ID,     10        },
     {ASPEN_OPDELAY_ID,     10        },
     {ASPEN_PHYCTLREG10_ID, 0x00103118},
     {ASPEN_PHYCTLREG3_ID,  0x20004433},  // 11,22,33,...77
     {ASPEN_USRCMDREG0_ID,  0x00000001},  // issue initialize command
     {ASPEN_OPDELAY_ID,     1000      },
     {0,                    0x00000000}

#else /* 156MHz */

     {ASPEN_ADRMAPREG0_ID,  0x000C0001},
     {ASPEN_SDRCFGREG0_ID,  0x00042330},
     {ASPEN_SDRTMGREG1_ID,  0x488C00C8},
     {ASPEN_SDRTMGREG2_ID,  0x323301A2},
     {ASPEN_SDRTMGREG3_ID,  0xC0C81C42},
     {ASPEN_SDRTMGREG4_ID,  0x32A85050},
     {ASPEN_SDRTMGREG5_ID,  0x00090091},
     {ASPEN_SDRCTLREG1_ID,  0x00000000},
     {ASPEN_SDRCTLREG2_ID,  0x00080000},
     {ASPEN_SDRCTLREG3_ID,  0xC0000000},
     {ASPEN_SDRCTLREG4_ID,  0x20814004},
     {ASPEN_SDRCTLREG6_ID,  0x00000001},
     {ASPEN_SDRCTLREG7_ID,  0x01000001},
     {ASPEN_PHYCTLREG14_ID, 0x80000000},
     {ASPEN_PHYCTLREG7_ID,  0x177C2779},
     {ASPEN_PHYCTLREG8_ID,  0x07700770},
     {ASPEN_PHYCTLREG9_ID,  0x00000077},
     {ASPEN_PHYCTLREG14_ID, 0xa0000000}, // Reset DLL
     {ASPEN_OPDELAY_ID,     10        },
     {ASPEN_PHYCTLREG14_ID, 0x80000000}, // Sync clock
     {ASPEN_OPDELAY_ID,     10        },
     {ASPEN_PHYCTLREG13_ID, 0x20000008 | (0x13 << 4)}, // update 512 cycles, 90 degree 0x20000108
     {ASPEN_OPDELAY_ID,     10        },
     {ASPEN_DLLCTLREG1_ID,  0x20000008 | (0x10 << 4)},
     {ASPEN_OPDELAY_ID,     10        },
     {ASPEN_PHYCTLREG14_ID, 0x88000000}, // update en static
     {ASPEN_OPDELAY_ID,     10        },
     {ASPEN_PHYCTLREG14_ID, 0x88000000}, // clear update en static
     {ASPEN_OPDELAY_ID,     10        },
     {ASPEN_OPDELAY_ID,     10        },
     {ASPEN_PHYCTLREG10_ID, 0x00103118},
     {ASPEN_PHYCTLREG3_ID,  0x20004433},  // 11,22,33,...77
     {ASPEN_USRCMDREG0_ID,  0x00000001},  // issue init command
     {ASPEN_OPDELAY_ID,     1000      },
     {0,                    0x00000000}
#endif

};

void delay(UINT_T n)
{
   UINT_T i, j;
   for (i=1; i<=n; i++)
   {
     for (j=1; j<100;j++);
   }
}

// FIXME: need to run the integrity check after programming the registers.
UINT_T ConfigureMemoryController(void *pTIM, unsigned long startaddr, unsigned long ulDDRScratchAreaLength)
{
	// variables for walking through the ddr records
	DDRGSpecList_T	*pDDRGSpecs;
	DDRGSpec_T		*pDDRGSpec;
	DDRTSpecList_T	*pDDRTSpecs;
	DDRTSpec_T		*pDDRTSpec;
	DDRCSpecList_T	*pDDRCSpecs, *pDDRCStartAddress;
	DDRCSpec_T		*pDDRCSpec;
	CMCCSpecList_T	*pCMCCSpecs;
	CMCCSpec_T		*pCMCCSpec;
	int				nRecs;
	int				r;
	UINT_T			status;
	UINT_T			id, value;
	unsigned long	faultaddr;
	unsigned long	baseaddr = 0xffffffff;
	unsigned long	loop;

	// some flags that enable / disable functionality within this routine.
	// these can be over written by time records.
	unsigned long	fDoSDRamInit = 1;				// usually only overridden for testing on first silicon.
	unsigned long	fDoMemoryCheck = 1;				// usually only overridden for testing on first silicon.
	unsigned long 	readTarget;

	// first see if the DDR has already been configured
	// if( *MCU_REG_MMU_MMAP0 && 0x1 ) return NoError;
	// default values for all the registers have been loaded.
	// now see if any of these fields are overriden by a tim record.

   *MCU_REG_MMU_MMAP0 = 0x000C0001;


	// all the values to use for initialization are set.
	// the actual MCU initialization process begins with writing these values to the MCU.
	// that is followed by a memory check.
	// stay in this init / validate loop until the memory check returns good results.
    status = 0;

	if( fDoSDRamInit )
	{
        // Note: we will try to initialize the memory controller a number of times, to work around a harware bug.
        // On a cold boot, even though MCU_DRAM_STATUS_INIT_DONE is set, the PLL may not be stable
        // enough to allow reliable memory operation and it takes a while to settle down.

        r = 0;

		while(1)
		{
            id = Tim_Table[r].id;
			value = Tim_Table[r].regValue;

            // run until the end of Tim table
			if ((id == 0) && (value == 0))
            {
               break;
            }

			switch(id)
			{
                case ASPEN_SDRCFGREG0_ID: 	*MCU_REG_SDRAM_CONFIG_0         = value; break;
				case ASPEN_SDRCFGREG1_ID: 	*MCU_REG_SDRAM_CONFIG_1         = value; break;
				case ASPEN_SDRTMGREG1_ID: 	*MCU_REG_SDRAM_TIMING_1         = value; break;
				case ASPEN_SDRTMGREG2_ID: 	*MCU_REG_SDRAM_TIMING_2         = value; break;
				case ASPEN_SDRTMGREG3_ID: 	*MCU_REG_SDRAM_TIMING_3         = value; break;
				case ASPEN_SDRTMGREG4_ID: 	*MCU_REG_SDRAM_TIMING_4         = value; break;
				case ASPEN_SDRTMGREG5_ID: 	*MCU_REG_SDRAM_TIMING_5         = value; break;
				case ASPEN_SDRTMGREG6_ID: 	*MCU_REG_SDRAM_TIMING_6         = value; break;
				case ASPEN_SDRCTLREG1_ID: 	*MCU_REG_SDRAM_CONTROL_1        = value; break;
				case ASPEN_SDRCTLREG2_ID: 	*MCU_REG_SDRAM_CONTROL_2        = value; break;
				case ASPEN_SDRCTLREG3_ID: 	*MCU_REG_SDRAM_CONTROL_3        = value; break;
				case ASPEN_SDRCTLREG4_ID: 	*MCU_REG_SDRAM_CONTROL_4        = value; break;
				case ASPEN_SDRCTLREG5_ID: 	*MCU_REG_SDRAM_CONTROL_5        = value; break;
				case ASPEN_SDRCTLREG6_ID: 	*MCU_REG_SDRAM_CONTROL_6        = value; break;
				case ASPEN_SDRCTLREG7_ID: 	*MCU_REG_SDRAM_CONTROL_7        = value; break;
				case ASPEN_SDRCTLREG13_ID: 	*MCU_REG_SDRAM_CONTROL_13       = value; break;
				case ASPEN_SDRCTLREG14_ID: 	*MCU_REG_SDRAM_CONTROL_14       = value; break;
				case ASPEN_SDRERRREG_ID:	*MCU_REG_ERROR_STATUS           = value; break;
				case ASPEN_ADRMAPREG0_ID:	*MCU_REG_MMU_MMAP0              = value; break;
				case ASPEN_ADRMAPREG1_ID:	*MCU_REG_MMU_MMAP1              = value; break;
				case ASPEN_USRCMDREG0_ID:	*MCU_REG_USER_INITIATED_COMMAND = value; break;
				case ASPEN_SDRSTAREG_ID:	*MCU_REG_DRAM_STATUS            = value; break;
				case ASPEN_PHYCTLREG3_ID:	*MCU_REG_PHY_CONTROL_3          = value; break;
				case ASPEN_PHYCTLREG7_ID:	*MCU_REG_PHY_CONTROL_7          = value; break;
				case ASPEN_PHYCTLREG8_ID:	*MCU_REG_PHY_CONTROL_8          = value; break;
				case ASPEN_PHYCTLREG9_ID:	*MCU_REG_PHY_CONTROL_9          = value; break;
				case ASPEN_PHYCTLREG10_ID:	*MCU_REG_PHY_CONTROL_10         = value; break;
				case ASPEN_PHYCTLREG11_ID:	*MCU_REG_PHY_CONTROL_11         = value; break;
				case ASPEN_PHYCTLREG12_ID:	*MCU_REG_PHY_CONTROL_12         = value; break;
				case ASPEN_PHYCTLREG13_ID:  *MCU_REG_PHY_CONTROL_13         = value; break;
				case ASPEN_PHYCTLREG14_ID:	*MCU_REG_PHY_CONTROL_14         = value; break;
				case ASPEN_DLLCTLREG1_ID:   *MCU_REG_PHY_DLL_CONTROL_1      = value; break;
				case ASPEN_PHYCTLREGTST_ID:	*MCU_REG_PHY_CONTROL_TEST       = value; break;
				case ASPEN_TSTMODREG0_ID:	*MCU_REG_TEST_MODE_0            = value; break;
				case ASPEN_TSTMODREG1_ID:	*MCU_REG_TEST_MODE_1            = value; break;
				case ASPEN_MCBCTLREG1_ID:	*MCU_REG_MCB_CONTROL_1          = value; break;
				case ASPEN_MCBCTLREG2_ID:	*MCU_REG_MCB_CONTROL_2          = value; break;
				case ASPEN_MCBCTLREG3_ID:	*MCU_REG_MCB_CONTROL_3          = value; break;
				case ASPEN_MCBCTLREG4_ID:	*MCU_REG_MCB_CONTROL_4          = value; break;
				case ASPEN_PRFCTLREG0_ID:	*MCU_REG_PERF_COUNT_CONTROL_0   = value; break;
				case ASPEN_PRFCTLREG1_ID:	*MCU_REG_PERF_COUNT_CONTROL_1   = value; break;
				case ASPEN_PRFSTAREG_ID:	*MCU_REG_PERF_COUNT_STAT        = value; break;
				case ASPEN_PRFSELREG_ID:	*MCU_REG_PERF_COUNT_SEL         = value; break;
				case ASPEN_OPDELAY_ID:		delay(value); break;
				case ASPEN_OPREAD_ID:		readTarget = *(unsigned long*) value; break;//TBD
				default: break;
			}
			r++;
		}

		// wait for DDR Initialization to complete:
		while( ((*MCU_REG_DRAM_STATUS) & MCU_DRAM_STATUS_INIT_DONE ) == 0 )
		{
			// waiting for hardware to assert the INIT_DONE bit
		}

		// finally, do some dummy reads in (and discard the values)
		baseaddr = ( *MCU_REG_MMU_MMAP0	) & 0xff800000;

		for (nRecs = 1; nRecs <= 128; nRecs++)
        {
            r = *(unsigned volatile long*)baseaddr;
        }

		// now do an integrity check to ensure the memory is reliable
	    // loop, and the save location for loop, is for debug use, to see how many times
		// the mcu must be init before working OK.
        status = CheckMemoryReliability(startaddr, ulDDRScratchAreaLength, &faultaddr);

	}//EndIf

	if( fDoSDRamInit && (status == 0) )
		return NoError;
	else
		return DDR_NotInitializedError;

}

UINT_T GetDDRSize()
{
    // This supports CS0 only
	UINT_T size;
	unsigned volatile long * pReg;
	UINT_T bfs;		// lsb location of bit field
	UINT_T bfl;		// number of bits in the fields
	// Read Memory Address Map Register 0 - CS0 (offset 0x0100)
    // (MEMORY_ADDRESS_MAP)
    pReg = MCU_REG_MMU_MMAP0;
	bfs = 16;
	bfl = 4;
	size = ReadBitField(pReg, bfs, bfl);
    switch(size)
	{
		case(0xF): {size = 0x80000000; break;} //(UINT)(2048*(1024*1024))
		case(0xE): {size = 1024*(1024*1024); break;}
		case(0xD): {size =  512*(1024*1024); break;}
		case(0xC): {size =  256*(1024*1024); break;}
		case(0xB): {size =  128*(1024*1024); break;}
		case(0xA): {size =   64*(1024*1024); break;}
		case(0x9): {size =   32*(1024*1024); break;}
		case(0x8): {size =   16*(1024*1024); break;}
		case(0x7): {size =    8*(1024*1024); break;}
		default:   {size =    0; break;}
	}
    return size;
}

