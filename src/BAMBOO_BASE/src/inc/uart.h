#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#include "PlatformConfig.h"
#include "uart_regs.h"

#define MAXCHAR 	512


/**
 * Misc.
 **/
#define MinImageSize       0x00000010
#define MainBufferSize     0x00001000
#define BufferSizeMask     0x00000FFF

/**
 * UART register definitions
 **/
#define UartFIFO         0x00
#define UartIER          0x04
#define UartIIR          0x08
#define UartFCR          0x08
#define UartLCR          0x0c
#define UartMCR          0x10
#define UartLSR          0x14
#define UartMSR          0x18
#define UartSCR          0x1c
#define UartSIR          0x20
#define UartDivLO        0x00
#define UartDivHI        0x04

/**
 * Uart Interrupt Identification Register (IIR)
 **/

#define UartIerFifo      (BIT6 | BIT7)
#define UartIirEoc       BIT5
#define UartIirAbl       BIT4
#define UartIirTod     	 BIT3
#define UartIirEseRE     (BIT1 | BIT2)
#define UartIirEseRF     BIT2
#define UartIirEseTF     BIT1
#define UartIirnIP       BIT0


/**
 * Uart Interrupt Enable Register (IER)
 **/
#define UartIerDmae      BIT7
#define UartIerUue       BIT6
#define UartIerNrze      BIT5
#define UartIerRtoie     BIT4
#define UartIerMie       BIT3
#define UartIerRlse      BIT2
#define UartIerTie       BIT1
#define UartIerRavie     BIT0

/**
 * Uart FIFO Control Register (FCR)
 **/
#define UartFcrITL_8	 BIT6
#define UartFcrITL_16	 BIT7
#define UartFcrITL_32	 (BIT6 | BIT7)
#define UartFcrTIL   	 BIT3
#define UartFcrResetTf   BIT2
#define UartFcrResetRf   BIT1
#define UartFcrFe        BIT0

/**
 * Uart Line Control Register (LCR)
 **/
#define UartLcrDlab      BIT7
#define UartLcrSb        BIT6
#define UartLcrStkyp     BIT5
#define UartLcrEps       BIT4
#define UartLcrPen       BIT3
#define UartLcrStb       BIT2
#define UartLcrWls       (BIT1|BIT0)

/**
 * Uart Line Status Register (LSR)
 **/
#define UartLsrFifoe     BIT7
#define UartLsrTemt      BIT6
#define UartLsrTdrq      BIT5
#define UartLsrBi        BIT4
#define UartLsrFe        BIT3
#define UartLsrPe        BIT2
#define UartLsrOe        BIT1
#define UartLsrDr        BIT0

/**
 * Uart Modem Control Register (MSR)
 **/
#define UartMcrEi        BIT3


/**
 * Function Prototypes
 **/
void ShutdownFFUART();
void InitializeFFUART(int BaudRate);
UINT_T WriteCharToFFUart(INT8_T WriteChar);
UINT_T PutStr(UINT8_T* message);

#endif /* UART_H_INCLUDED */
