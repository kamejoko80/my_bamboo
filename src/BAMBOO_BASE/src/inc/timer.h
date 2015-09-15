/*************************************************************
 * Timer.h
 *
 * Contents:
 *      Definitions and functions declarations used fin the
 *      Boot ROM development
 *
 *************************************************************/
#ifndef __timer_h
#define __timer_h

#include "TMR.h"
#include "PlatformConfig.h"

#define TMR_CR0	(volatile UINT_T *)TMR1_CRN
#define TIMER_CER (volatile UINT_T *)TMR1_CER
#define APB_CU_CR (volatile UINT_T *)(APBC_TIMERS1_CLK_RST)
#define TIMER_CLEAR (*TMR_CR0 = 0)
#define TIMER_GET   (*TMR_CR0)
#define TIMER_ENABLE (*TIMER_CER |= 1)

#define Delay_us(x) Delay(x)


// Prototypes

UINT_T GetOSCR0(void);
UINT_T OSCR0IntervalInSec(UINT_T Before, UINT_T After);
UINT_T OSCR0IntervalInMilli(UINT_T Before, UINT_T After);
UINT_T OSCR0IntervalInMicro(UINT_T Before, UINT_T After);
UINT32 InitSODTimer( void );
UINT32 GetSODTimerValue( void );
UINT_T GetElapsedTimeMilliSecFromSOD( void );
UINT_T GetElapsedTimeUSecFromSOD( void );
UINT_T GetElapsedTimeSecondsFromSOD( void );
void Delay(UINT_T uSec);
#endif // Misc_h
