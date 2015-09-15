#include <string.h>
#include <stdio.h>
#include "uart.h"
#include "PlatformConfig.h"

#define TIME_OUT 500000

void ShutdownFFUART()
{
	*FFMCR = 0;
	*FFIER = 0;
}

void ShutdownALTUART()
{
	*FFMCR = 0;
	*FFIER = 0;
}

#define MFPR_88          *(volatile unsigned int*)(0xD401E160) // MFP_88 UART2_RXD
#define MFPR_89          *(volatile unsigned int*)(0xD401E164) // MFP_89 UART2_TXD

// Numerator / Denominator = InputClock / (2 x OutputClock)
// OutputClock = (Denominator x InputClock) / (2 x Numerator)

// FFUART configuration
void InitializeFFUART(int BaudRate)
{
	int DivisorVal;
	unsigned int Regval = 0;

    /* Init UART port */
    //InitUartPort();

    // Pin mux select
    MFPR_88 = 0x02;
    MFPR_89 = 0x02;

    *(VUINT_T *)APBC_UART2_CLK_RST = (BIT4 | BIT1 | BIT0);

  	// Configure the UART registers to Reset Values.
	// Ignore TBR, IIR, FOR and ACR.
	*FFLCR = 0;
	*FFIER = 0;

	//Reset Tx/Rx FIFOs
	*FFFCR = (UartFcrResetRf | UartFcrResetTf);
	*FFSPR = 0;
	Regval = *FFRBR;	// Read RBR to empty.
	Regval = *FFLSR;	// Read LSR to clear status bits.
	Regval = *FFMSR;	// Read MSR to clear status bits.

	// DLAB = 1, 1 Stop bit, 8-bit character
    *FFLCR  = (UartLcrWls | UartLcrDlab);

	// Setup divisor using slow cloclk
    DivisorVal = UARTCLK_L /(BaudRate * 16);

	// Setup Divisor Latch Register
	*FFDLL  = DivisorVal & 0xff;
	*FFDLH  = (DivisorVal & 0xff00) >> 8;

	// Set DLAB in LCR = 0 to allow access to THR and RBR
	*FFLCR &= 0x7f;
	*FFMCR  |= UartMcrEi;	// Enable FFUART interrupts

	// Enable and clear FIFOs and set INT trigger Level to be 8 bytes
	*FFFCR  = (UartFcrFe | UartFcrTIL | UartFcrITL_8);

	//	enable FFUART and individual interrupts
	*FFIER  = (UartIerRavie | UartIerRlse | UartIerUue);

}

UINT_T ReadFFUartLineStatus()
{
    return (*(UINT_T *)FFLSR);
}

UINT8_T ReadFFUartReceiveFIFO()
{
     return (*(UINT8_T *)FFRBR);
}

UINT8_T ReadCharFromFFUart(void)
{
    while((ReadFFUartLineStatus() & 0x1) != 0x1);
    return ReadFFUartReceiveFIFO();
}


UINT_T WriteCharToFFUart(INT8_T WriteChar)
{
	UINT_T Retval = NoError;
    UINT_T To = 0;

	for (;;)
	{
		if ((ReadFFUartLineStatus() & 0x40) == 0x40)
		{
		  	*FFTHR = WriteChar;	// Write to Transmit FIFO
			break;
		}

		To++;

		if(To >= 500000)
        {
            return TimeOutError;
        }
	}

  	return (Retval);
}

UINT_T PutStr(UINT8_T* message)
{
	UINT_T Retval = 0;
	UINT_T size;
	UINT_T i;

	size = strlen((char *)message);
	for(i = 0; i < size ; i++) {
		Retval = WriteCharToFFUart((char)message[i]);
		if (Retval != NoError) break;
	}
	return Retval;
}

/* retarget printf */
int _write(int fd, char * str, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        if (str[i]=='\n')
        {
           WriteCharToFFUart('\r');
        }
        WriteCharToFFUart(str[i]);
    }
    return len;
}
