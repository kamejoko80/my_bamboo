/*********************************************************
  START UP CODE
**********************************************************/

.arm
.align 4

/*********************************************************
  MACRO DEFINITION
**********************************************************/

#define Mode_USR        0xD0
#define Mode_FIQ        0xD1
#define Mode_IRQ        0xD2
#define Mode_SVC        0xD3
#define Mode_Abort      0xD7
#define Mode_Undef      0xDB
#define Mode_System     0xDF

#define IrqStackSize    0x1100
#define SvcStackSize    0x1400

#define ISRAM_START     0xD1020000
#define ISRAM_SIZE      0x00020000
#define L2_START        0xD1020000
#define STACK_START     0xD1028000
#define STACK_SIZE      0x00002000
#define BL_STACK_START  0xD1020000
#define BL_STACK_SIZE   0x00002800

#define DDR_STACK_START (0x00f00000-0x00200000)
#define DDR_STACK_SIZE   0x00200000
#define IRQ_STACK_SIZE   0x00002000


/*********************************************************
  MACRO DEFINITION
**********************************************************/
 .macro DELAY
    mov	r8, #0x00010000
    // loop here
	subs r8, r8, #1
	subne pc, pc, #12  // Repeat decrement if not 0
 .endm

/*********************************************************
  IMPORT EXPORT
**********************************************************/
 .extern IrqHandler
 .extern main

__main:

 .global ExceptionVectors
ExceptionVectors:
  	B  ResetHandler       // 0x00
   	B  UnHandlerException // 0x04 UndefinedHandler
   	B  UnHandlerException // 0x08 SwiHandler
   	B  UnHandlerException // 0x0C PrefetchHandler
   	B  UnHandlerException // 0x10 AbortHandler
   	B  UnHandlerException // 0x14 Reserved Vector
   	B  SaveProgramState   //SaveProgramState   // 0x18
   	B  UnHandlerException // 0x1C FiqHandler FIQ interrupts not anabled

ResetHandler:

    /* Read control register 1 */
    // mrc p15, 0, r0, c1, c0, 0

    /* Init stack */
	ldr	r7, =DDR_STACK_START
	ldr r8, =DDR_STACK_SIZE
   	mov r9, r7
   	add r9, r9, r8
  	sub r9, r9, #4
    mov r7, #Mode_IRQ
    msr CPSR_c, r7
    mov SP, r9
    mov r7, #Mode_SVC
    msr CPSR_c, r7
    sub r9, r9, #IrqStackSize
    mov SP, r9

#if 0

@************************************************************
@* Initialize INT. Cntl. Mask all Ints, and set them to IRQ
@************************************************************

@ there's a relationship between the MCU and the ability to wake from idle:
@ the MCU must be running. take care of that here.
	ldr		r3,		=0xb0000000		@ mcu_base
	mov		r4,		#1				@ sdram_init_req bit.
	str		r4,		[r3, #0x0120]	@ mcu:user_initiated_command register
									@ if the mcu is already running,
									@ this shouldn't hurt anything.

	@ wait for the MCU to finish initializing.
mcu0:
	ldr		r4,		[r3, #0x01b0]	@ mcu:dram_status register
	tst		r4,		#1				@ test the "init done" bit
	beq		mcu0						@ branch if "init done" isn't set yet.



@
@ PLL Control and Divisor Setup
@
	@ start of PLL1 configuration and idle mode entry preparation.



@;
@; MPMU:PLL1_REG1: set some VCO select, voltage and current control parameters
	ldr		r3,		=0xd4050000		@ MPMU base
	ldr		r4,		=0x91040664		@ 1 0 010 0010 0000 100 0000 0110 01 10 010 0
									@ 31         1   pll1_ovrd_input (override input bits from pll1_1_reg, pll1_2_reg and pll1_ssc_reg)
									@ 30         0   reserved
									@ 29:27    010   intpl (interpolater bias current selection)
									@ 26:23   0010   vco_div_sel_se (post divider for single ended output)
									@ 22:19   0000   vcodiv_se_diff [1] (post divider for differential output)
									@ 18:16    100   vco_vrng (pll v to i gain)
									@ 15:12   0000   test_mon (dc points test control register)
									@ 11:08   0110   kvco (VCO select)
									@ 07:06     01   vddl (internal VDD supply control)
									@ 05:04     10   vddm (VCO supply control)
									@ 03:01    010   icp [7.5 uA] (charge pump current control)
									@ 00         0   vcofbst
	str		r4,		[r3, #0x50]		@ MPMU:PLL1_REG1
	ldr		r5,		[r3, #0x50]		@ make sure write completes.



@;
@; MPMU:PLL1_REG2: set other VCO enable parameters
@	ldr		r4,		=0x86000030		; 1 0 0 0 0 1 1 00000000000000000 0 0 1 1 0 0 0 0
	ldr		r4,		=0x84000030		@ 1 0 0 0 0 1 0 00000000000000000 0 0 1 1 0 0 0 0
									@ 31         1   cap_sel
									@ 30         0   gate_clk_ctrl_en
									@ 29         0   gate_clk_ctrl
									@ 28         0   freq_offset_mode_selection
									@ 27         0   freq_offset_valid
									@ 26         1   reset_ext
									@ 25         1(0)clk_det_en
									@ 24:08      0   freq_offset
									@ 07         0   freq_offset_en
									@ 06         0   diffclk_en
									@ 05         1   sel_vco_clk_se
									@ 04         1   sel_vco_clk_diff
									@ 03         0   reserved
									@ 02         0   bypass_en
									@ 01         0   bypass_fbdiv
									@ 00         0   reserved
	str		r4,		[r3, #0x54]		@ MPMU:PLL1_REG2
	ldr		r5,		[r3, #0x54]		@ make sure write completes.



@;
@; MPMU:FCCR: step1 of changing PLL1 dividers
	ldr		r4,		=0x0000c000		@ 000 000 000 0000000 1 1 00000 000000000
									@ 31:29    000   coreclksel
									@ 28:26    000   reserved
									@ 25:23    000   axiclksel
									@ 22:16      0   reserved
									@ 15         1   MFC
									@ 14         1   pll1cen
									@ 13:09      0   pll1refd FIXME: doc says must be programmed with 4
									@ 08:00      0   pll1fbd  FIXME: doc says must be programmed with 0x8e
	str		r4,		[r3, #0x08]		@ MPMU:FCCR
	ldr		r5,		[r3, #0x08]		@ make sure write completes.

	DELAY							@ FIXME: is delay necessary? how long? delays required after any other writes?



@;
@; MPMU:FCCR: step2 of changing PLL1 dividers
	ldr		r4,		=0x0000c290		@ 000 000 000 0000000 1 1 00001 010010000
									@ 31:29    000   coreclksel
									@ 28:26    000   reserved
									@ 25:23    000   axiclksel
									@ 22:16      0   reserved
									@ 15         1   MFC
									@ 14         1   pll1cen
									@ 13:09      1   pll1refd
									@ 08:00   0x90   pll1fbd  FIXME: doc says must be programmed with 0x8e
	str		r4,		[r3, #0x08]		@ MPMU:FCCR
	ldr		r5,		[r3, #0x08]		@ make sure write completes.

	DELAY							@ FIXME: is delay necessary? how long? delays required after any other writes? is there a status bit that can be polled instead?


@;
@; MPMU:AWUCRM: set shutdown and wake parameters
	ldr		r3,		=0xd4051000		@ MPMU base
	@ note: in the MPMU:AWUCRM register, a "1" enables the wakeup source.
	ldr		r4,		=0x00000100		@ 000000 0 0 0 0 0 00  0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 <= timers only. works when int36 disabled.
									@ 31:26      0   reserved
									@ 25         0   ap async int
									@ 24         0   ap full idle
									@ 23         0   sd1
									@ 22         0   sd2
									@ 21         0   keypress
									@ 20:19      0   reserved
									@ 18         0   wdt
									@ 17         0   rtc_alarm
									@ 16         0   reserved
									@ 15         0   sd4
									@ 14         0   sd3
									@ 13         0   msp
									@ 12         0   xd
									@ 11         0   cf
									@ 10         0   ap1_timer_3
									@ 09         0   ap1_timer_2
									@ 08         1   ap1_timer_1 <= only this one on for now
									@ 07         0   wakeup7
									@ 06         0   wakeup6
									@ 05         0   wakeup5
									@ 04         0   wakeup4
									@ 03         0   wakeup3
									@ 02         0   wakeup2
									@ 01         0   wakeup1
									@ 00         0   wakeup0
	str		r4,		[r3, #0x4c]		@ MPMU:AWUCRM	; FIXME: does this "mask" register enable or disable? to me, mask means disable.
	ldr		r5,		[r3, #0x4c]		@ make sure write completes.



@;
@; MPMU:ACGR: ensure required clocks are enabled
	ldr		r4,		=0x001effde		@ 000000000 0 0 1 1 1 1 0 1 1 1 1 1 1 1 1 1 1 0 1 1 1 1 0
									@ 31:23      0   reserved
									@ 22         0   apb2_1m_en
									@ 21         0   apb2_500k_en
									@ 20         0   apb2_26m_en
									@ 19         0   apmu_asysclk
									@ 18         0   apmu_156m
									@ 17         0   apmu_39m
									@ 16         0   reserved
									@ 15         0   apmu_624m
									@ 14         0   apmu_pll2
									@ 13         0   apmu_312m
									@ 12         0   apmu_104m
									@ 11         0   apmu_52m		; FIXME: how is this different from bit 7?
									@ 10         0   apmu_48m
									@ 09         0   gpc
									@ 08         0   ap_fuart
									@ 07         0   ap_52m			; FIXME: how is this different from bit 11?
									@ 06         0   ap_i2c
									@ 05         0   reserved
									@ 04         0   ap_26m
									@ 03         0   ap_13m
									@ 02         0   ap_6.5m
									@ 01         0   ap_suart
									@ 00         0   reserved
	str		r4,		[r3, #0x24]		@ MPMU:ACGR	; FIXME: which clocks are really needed for this?
												@		 definitely OST, PLL1, PLL2, MPMU, ICU, and APMU. anything else?
												@        Should just or-in the required fields.
	ldr		r5,		[r3, #0x24]		@ make sure write completes.



@;
@; MPMU:POCR: set osc stabilization and PLL lock-wait parameters
	ldr		r3,		=0xd4050000		@ MPM base
	ldr		r4,		=0x0001003f		@ 0 0000000 00000001 0000 000000111111
									@ 31         0   force
									@ 30:24      0   reserved
									@ 23:16      1   osc25m
									@ 15:12      0   reserved
									@ 11:00     3f   plllock
	str		r4,		[r3, #0x0c]		@ MPMU:POCR
	ldr		r5,		[r3, #0x0c]		@ make sure write completes.


@;
@; APMU:MC_HW_SLP_TYPE: prepare MC (HW side) for a sleep event
	mov		r4,		#0x00			@ FIXME: why is writing both HW & SW sleep types necessary?
									@ 0-0 000    APMU:MC_HW_SLP_TYPE
									@ 31:03      0   reserved
									@ 02:00      0   mc_hw_slp_type (self refresh power down)
	str		r4,		[r3, #0xb0]		@ APMU:MC_HW_SLP_TYPE
	ldr		r5,		[r3, #0xb0]		@ make sure write completes.

@;
@; MPMU:APCR: set wake detect parameters
	ldr		r3,		=0xd4051000		@ MPMU base
	ldr		r4,		=0xbe424000		@ 1 0 1 1 1 1 1 0 0 1 0 0 0 0 1 0 0 1 00000000000000
									@ 31         1   axisdd
									@ 30         0   reserved
									@ 29         1   slpen
									@ 28         1   set always

									@ 27         1   ddrcorsd
									@ 26         1   apbsd
									@ 25         1   setalways
									@ 24         0   intclr

									@ 23         0   slpwp0
									@ 22         1   slpwp1
									@ 21         0   slpwp2
									@ 20         0   slpwp3

									@ 19         0   reserved
									@ 18         0   slpwp4
									@ 17         1   slpwp5
									@ 16         0   slpwp6

									@ 15         0   slpwp7
									@ 14         1   setalways
									@ 13:00      0   reserved
	str		r4,		[r3]			@ MPMU APCR
	ldr		r5,		[r3]			@ make sure write completes.


@; APMU:IDLE_CFG: set idle mode properties
	ldr		r3,		=0xd4282800		@ APMU base
	ldr		r4,		=0x00300302		@ 0000 0000 0011 0000 0000 0010 0000 0010
									@ 31:22      0   reserved
									@ 21         1   dis_mc_sw_req
									@ 20         1   mc_wake_en
									@ 19:12      0   reserved
									@ 11         0   mask_jtag_idle
									@ 10         0   L2_clk_dis
									@ 09         1   L2_resetn
									@ 08:02      0   reserved
									@ 01         1   idle
									@ 00         0   reserved
	str		r4,		[r3, #0x18]		@ APMU:IDLE_CFG
	ldr		r5,		[r3, #0x18]		@ make sure write completes.
	@ end of PLL1 configuration and idle mode entry preparation.


@
@ Now PLL Control and Divisors are set.


@
@ Every thing is ready
@ Simply must enter and exit core idle for these values to take effect.





@ set up a wake-event stuff.


@ => timer wake

	@ must get the timer running.

	@ enable clocks to the timer unit
	ldr		r3,		=0xd4051024		@ mpmu_acgr
	mov		r4,		#0xffffffff		@ all clocks. later figure out exactly which one needs to be on.
	str		r4,		[r3]

	ldr		r3,		=0xd4015034		@ apbc_timer_clk_res_ctrl
	mov		r4,		#0x03			@ clocked and not reset
	str		r4,		[r3]



	ldr		r3,		=0xd4282000		@ ICU base
	mov		r4,		#0x4f			@ must mask, per JIRA sighting DPF-789
	str		r4,		[r3, #0x90]		@ ICU register for: PMU

	mov		r4,		#0x5f			@ unmask, set as IRQ, priority 15
	str		r4,		[r3, #0x34]		@ ICU register for: Hardware Timer 1



	@ timer config.
	ldr		r3,		=0xd4014000		@ base addr OST


	@ clear match status
	mov		r4,		#0x1
	str		r4,		[r3, #0x74]		@ timer interrupt clear

	@ allow matches to assert interrupts
	mov		r4,		#0x1
	str		r4,		[r3, #0x40]		@ timer interrupt enable

	@ set the match for a little ways into the future.
	ldr		r4,		[r3, #0x28]		@ get current oscr0 counter register (running at 13Mhz)
	add		r4,		r4,	 #0x00280000@ a short bit into the future ~200ms?
	str		r4,		[r3, #0x04]		@ oscmr0 match register


	@ enable the timer
	mov		r4,		#0x1
	str		r4,		[r3, #0x84]		@ count enable



@;
@; idle/sleep entry via wfi:
    mov		r3,		#0
    mcr		p15,	0,	r3,		c7,		c0,		4

    @ xdb loses the connection when the above instruction is executed.
    @ presume that is the idle entry, until proven otherwise.


@ executing here? that means we woke from core idle.
@ the new PLL control and divisor config should be in effect.

@
@------------------------------------------------------------------------------------------------
@   End of PLL1 Reconfig via WFI
@------------------------------------------------------------------------------------------------

#endif

@------------------------------------------------------------------------------------------------
@   Workaround for Aspen B0 DPF-845, Clock gating needs to be enabled (by clearing bit 6)
@------------------------------------------------------------------------------------------------
	ldr		r0, =0xd4282c08		@ AP_CPU_CONF, or just CPU_CONF
	ldr		r1, [r0]			@ get current CPU_CONF contents
	bic		r1, r1, #(1<<6)		@ CG_BYPASS0 is the bit that needs to be cleared.
	str		r1, [r0]			@ update the CPU_CONF register.

	ldr		r2, [r0]			@ read value back and make use of it
	add		r2, r2, #1			@ to ensure the write completes.

@************************************************************
@ Section initialization
@ 1> Copy RW data to RAM
@ 2> Clear out BSS section
@************************************************************

    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs    r3, r2
    ble    .flash_to_ram_loop_end
.flash_to_ram_loop:
    subs    r3, #4
    ldr    r0, [r1, r3]
    str    r0, [r2, r3]
    bgt    .flash_to_ram_loop
.flash_to_ram_loop_end:

    /* Zero fill the bss segment. */
    ldr   r1, = __bss_start__
    ldr   r2, = __bss_end__
    movs  r3, #0
    b  .fill_zero_bss
.loop_zero_bss:
    str  r3, [r1], #4

.fill_zero_bss:
    cmp  r1, r2
    bcc  .loop_zero_bss

@************************************************************
@ Disable Irq Interrupts
@************************************************************

   	MRS    	r0,CPSR        @ get the processor status
   	ORR    	r0,r0,#0x80    @ Disable IRQ interrupts
   	MSR    	CPSR_cf,r0     @ SVC 32 mode with interrupts disabled

@************************************************************
@ Enable Irq Interrupts
@************************************************************

   	MRS    	r0, CPSR        @ get the processor status
   	BIC    	r0, r0, #0x80   @ Enable IRQ interrupts
   	MSR    	CPSR_cf, r0     @ SVC 32 mode with interrupts enabled

@************************************************************
@ Relocate vector table base to 0x00000000
@************************************************************

    MRC p15, 0, R0, c1, c0, 0 // Read control register
    LDR R1, =~(1<<13)
    AND R0, R1
    MCR p15, 0, R0, c1, c0, 0 // Write control register

@************************************************************
@ Jump to main
@************************************************************

    B  main
    B  .

@************************************************************
@ IRQ Vector Handler - Save Program State
@************************************************************

    .global SaveProgramState
SaveProgramState:
	SUB	R14, R14, #4	    // Adjust return address before saving it
	STMFD	SP!, {R14}
	MRS	R14, SPSR
	STMFD   SP!, {R0-R12,R14}

	BL IrqHandler

	@ Modify CPS register
	MRS	r12, CPSR
	ORR r12,r12,#0x80
	MSR	CPSR_c, r12

	LDMFD 	SP!, {R0-R12,R14}
	MSR	SPSR_cxsf, R14
	LDMFD 	SP!, {PC}^

@************************************************************
@ Unhandler exception
@************************************************************

UnHandlerException:
    bl led_blink

@************************************************************
@ LED blink
@************************************************************

    .global led_blink

led_blink:
    bl led_init
LED_LOOP:
    bl  led_on
    bl  delay
    bl  led_off
    bl  delay
    b   LED_LOOP

@************************************************************
@ LED test function
@************************************************************

    .global led_test

led_test:
    bl  led_init
FOEVER_LOOP:
    bl  led_on
    bl  delay
    bl  led_off
    bl  delay
    b   FOEVER_LOOP

delay:
    push {lr}
    ldr r4, =0x0001FFFF
LOOP:
    sub r4, #1
    cmp r4, #0
    bgt LOOP
    pop {lr}
    bx lr

led_init:
    push {lr}
    ldr r5, =0xD4015008
    ldr r6, =0x7
    str r6, [r5]
    ldr r6, =0x3
    str r6, [r5]

    mov r4, #0x100000
    ldr r5, =0xD401905C
    str r4, [r5]

    mov r4, #0x100000
    ldr r5, =0xD4019020
    str r4, [r5]
    pop {lr}
    bx lr

led_on:
    push {lr}
    mov r4, #0x100000
    ldr r5, =0xD401902C
    str r4, [r5]
    pop {lr}
    bx lr

led_off:
    push {lr}
    mov r4, #0x100000
    ldr r5, =0xD4019020
    str r4, [r5]
    pop {lr}
    bx lr

.end
