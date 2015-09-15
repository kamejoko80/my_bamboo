/******************************************************************************
**	(C) Copyright 2007 Marvell International Ltd.  
**	All Rights Reserved
******************************************************************************/

/******************************************************************************
 *
 *	uart_regs.h
 *
 *	Specifies the register offsets for the UART peripherals
 *
 ******************************************************************************/

/*
 * Note On Tavor L UART 2 is at base 0xD4017000 and int 27
 *      On ASPEN this is called UART 1 in the docs but it
 *         is the same peripheral
 *
 */

/*
 *
 *	THE BASE ADDRESSES
 *
 */

#define	UART1_BASE	0xD4017000
#define	UART2_BASE	0xD4018000
#define	UART3_BASE	0xD4026000

/* Used for ASPEN in PlatformConfig.c (UART1)*/
#define APBC_UART1_CLK_RST 	0xD4015000
#define APBC_UART2_CLK_RST  0xD4015004
#define APBC_UART3_CLK_RST  0xD4015070


/*
 *
 *	THE REGISTER DEFINES
 *
 */
#define	UART3_RBR	(UART3_BASE+0x0000)	/* 32 bit	Receive Buffer Register */
#define	UART2_RBR	(UART2_BASE+0x0000)	/* 32 bit	Receive Buffer Register */
#define	UART1_RBR	(UART1_BASE+0x0000)	/* 32 bit	Receive Buffer Register */

#define	UART3_THR	(UART3_BASE+0x0000)	/* 32 bit	Transmit Holding Register */
#define	UART2_THR	(UART2_BASE+0x0000)	/* 32 bit	Transmit Holding Register */
#define	UART1_THR	(UART1_BASE+0x0000)	/* 32 bit	Transmit Holding Register */

#define	UART3_DLL	(UART3_BASE+0x0000)	/* 32 bit	Divisor Latch Low Register */
#define	UART2_DLL	(UART2_BASE+0x0000)	/* 32 bit	Divisor Latch Low Register */
#define	UART1_DLL	(UART1_BASE+0x0000)	/* 32 bit	Divisor Latch Low Register */

#define	UART3_DLH	(UART3_BASE+0x0004)	/* 32 bit	Divisor Latch High Register */
#define	UART2_DLH	(UART2_BASE+0x0004)	/* 32 bit	Divisor Latch High Register */
#define	UART1_DLH	(UART1_BASE+0x0004)	/* 32 bit	Divisor Latch High Register */

#define	UART3_IER	(UART3_BASE+0x0004)	/* 32 bit	Interrupt Enable Register */
#define	UART2_IER	(UART2_BASE+0x0004)	/* 32 bit	Interrupt Enable Register */
#define	UART1_IER	(UART1_BASE+0x0004)	/* 32 bit	Interrupt Enable Register */

#define	UART3_IIR	(UART3_BASE+0x0008)	/* 32 bit	Interrupt Identification Register */
#define	UART2_IIR	(UART2_BASE+0x0008)	/* 32 bit	Interrupt Identification Register */
#define	UART1_IIR	(UART1_BASE+0x0008)	/* 32 bit	Interrupt Identification Register */

#define	UART3_FCR	(UART3_BASE+0x0008)	/* 32 bit	FIFO Control Register */
#define	UART2_FCR	(UART2_BASE+0x0008)	/* 32 bit	FIFO Control Register */
#define	UART1_FCR	(UART1_BASE+0x0008)	/* 32 bit	FIFO Control Register */

#define	UART3_ISR	(UART3_BASE+0x0020)	/* 32 bit	FIFO Control Register */
#define	UART2_ISR	(UART2_BASE+0x0020)	/* 32 bit	FIFO Control Register */
#define	UART1_ISR	(UART1_BASE+0x0020)	/* 32 bit	FIFO Control Register */

#define	UART3_FOR	(UART3_BASE+0x0024)	/* 32 bit	Receive FIFO Occupancy Register */
#define	UART2_FOR	(UART2_BASE+0x0024)	/* 32 bit	Receive FIFO Occupancy Register */
#define	UART1_FOR	(UART1_BASE+0x0024)	/* 32 bit	Receive FIFO Occupancy Register */

#define	UART3_ABR	(UART3_BASE+0x0028)	/* 32 bit	Auto-Baud Control Register */
#define	UART2_ABR	(UART2_BASE+0x0028)	/* 32 bit	Auto-Baud Control Register */
#define	UART1_ABR	(UART1_BASE+0x0028)	/* 32 bit	Auto-Baud Control Register */

#define	UART3_ACR	(UART3_BASE+0x002C)	/* 32 bit	Auto-Baud Count Register */
#define	UART2_ACR	(UART2_BASE+0x002C)	/* 32 bit	Auto-Baud Count Register */
#define	UART1_ACR	(UART1_BASE+0x002C)	/* 32 bit	Auto-Baud Count Register */

#define	UART3_LCR	(UART3_BASE+0x000C)	/* 32 bit	Line Control Register */
#define	UART2_LCR	(UART2_BASE+0x000C)	/* 32 bit	Line Control Register */
#define	UART1_LCR	(UART1_BASE+0x000C)	/* 32 bit	Line Control Register */

#define	UART3_LSR	(UART3_BASE+0x0014)	/* 32 bit	Line Status Register */
#define	UART2_LSR	(UART2_BASE+0x0014)	/* 32 bit	Line Status Register */
#define	UART1_LSR	(UART1_BASE+0x0014)	/* 32 bit	Line Status Register */

#define	UART3_MCR	(UART3_BASE+0x0010)	/* 32 bit	Modem Control Register */
#define	UART2_MCR	(UART2_BASE+0x0010)	/* 32 bit	Modem Control Register */
#define	UART1_MCR	(UART1_BASE+0x0010)	/* 32 bit	Modem Control Register */

#define	UART3_MSR	(UART3_BASE+0x0018)	/* 32 bit	Modem Status Register */
#define	UART2_MSR	(UART2_BASE+0x0018)	/* 32 bit	Modem Status Register */
#define	UART1_MSR	(UART1_BASE+0x0018)	/* 32 bit	Modem Status Register */

#define	UART3_SCR	(UART3_BASE+0x001C)	/* 32 bit	Scratchpad Register */
#define	UART2_SCR	(UART2_BASE+0x001C)	/* 32 bit	Scratchpad Register */
#define	UART1_SCR	(UART2_BASE+0x001C)	/* 32 bit	Scratchpad Register */


// UART registers
#ifdef USE_UART1
#define FFRBR      (volatile unsigned long *)UART1_RBR  // Receive Buffer Register (read only)
#define FFTHR      (volatile unsigned long *)UART1_THR  // Transmit Holding Register (write only)
#define FFIER      (volatile unsigned long *)UART1_IER  // Interrupt Enable Register (read/write)
#define FFIIR      (volatile unsigned long *)UART1_IIR  // Interrupt ID Register (read only)
#define FFFCR      (volatile unsigned long *)UART1_FCR  // FIFO Control Register (write only)
#define FFLCR      (volatile unsigned long *)UART1_LCR  // Line Control Register (read/write)
#define FFMCR      (volatile unsigned long *)UART1_MCR  // Modem Control Register (read/write)
#define FFLSR      (volatile unsigned long *)UART1_LSR  // Line Status Register (read only)
#define FFMSR      (volatile unsigned long *)UART1_MSR  // Modem Status Register (read only)
#define FFSPR      (volatile unsigned long *)UART1_SCR  // Scratch Pad Register (read/write)
#define FFFOR      (volatile unsigned long *)UART1_FOR  // Receive FIFO Occupancy Register (read/write)
#define FFDLL      (volatile unsigned long *)UART1_DLL  // baud divisor lower byte (read/write)
#define FFDLH      (volatile unsigned long *)UART1_DLH  // baud divisor higher byte (read/write)
#define FFISR	   (volatile unsigned long *)UART1_ISR  // Infrared Selection Register
#else /* UART2 */
#define FFRBR      (volatile unsigned long *)UART2_RBR  // Receive Buffer Register (read only)
#define FFTHR      (volatile unsigned long *)UART2_THR  // Transmit Holding Register (write only)
#define FFIER      (volatile unsigned long *)UART2_IER  // Interrupt Enable Register (read/write)
#define FFIIR      (volatile unsigned long *)UART2_IIR  // Interrupt ID Register (read only)
#define FFFCR      (volatile unsigned long *)UART2_FCR  // FIFO Control Register (write only)
#define FFLCR      (volatile unsigned long *)UART2_LCR  // Line Control Register (read/write)
#define FFMCR      (volatile unsigned long *)UART2_MCR  // Modem Control Register (read/write)
#define FFLSR      (volatile unsigned long *)UART2_LSR  // Line Status Register (read only)
#define FFMSR      (volatile unsigned long *)UART2_MSR  // Modem Status Register (read only)
#define FFSPR      (volatile unsigned long *)UART2_SCR  // Scratch Pad Register (read/write)
#define FFFOR      (volatile unsigned long *)UART2_FOR  // Receive FIFO Occupancy Register (read/write)
#define FFDLL      (volatile unsigned long *)UART2_DLL  // baud divisor lower byte (read/write)
#define FFDLH      (volatile unsigned long *)UART2_DLH  // baud divisor higher byte (read/write)
#define FFISR	   (volatile unsigned long *)UART2_ISR  // Infrared Selection Register
#endif
