;************************************************************************************
;*																					*
;*	file name	: definition of M16C/62P's SFR										*
;*																					*
;*	Copyright	: MITSUBISHI ELECTRIC CORPORATION									*
;*																					*
;*	Version		: 1.00 ( 2002- 7-22 ) Initial										*
;*				: 2.00 ( 2002-12-27 ) 												*
;*				: 		cm3		 register delete									*
;*				: 		vcr1	 register add										*
;*				: 		vcr2	 register add										*
;*				: 		d4int	 register add										*
;*				: 		u1bcnic	 register add										*
;*				: 		u0bcnic	 register add										*
;*				: 		wdc5	 bit add	(wdc register) 							*
;*				: 		plc06	 bit add	(plc0 register) 						*
;*				: 		pm21	 bit add	(pm2 register) 							*
;*				: 		pm22	 bit add	(pm2 register) 							*
;*				: 		fidr 	 register 											*
;*				: 				 3B4h => 1B4h										*
;*				:		fmr03	 bit delete	(fmr0 register)							*
;*				:		fmstp	 bit add	(fmr0 register)							*
;************************************************************************************
;
;  note:
;	This data is a freeware that SFR for M16C/62P groups is described.
;	Mitsubishi Electric Corporation assumes no responsibility for any damage
;	that occurred by this data.
;
;-------------------------------------------------------
;	Processor mode register 0
;-------------------------------------------------------
pm0				.equ		0004h
;
pm00			.btequ		0,pm0		; Processor mode bit
pm01			.btequ		1,pm0		; Processor mode bit
pm02			.btequ		2,pm0		; R/W mode select bit
pm03			.btequ		3,pm0		; Software reset bit
pm04			.btequ		4,pm0		; Multiplexed bus space select bit
pm05			.btequ		5,pm0		; Multiplexed bus space select bit
pm06			.btequ		6,pm0		; Port P4_0 to P4_3 function select bit
pm07			.btequ		7,pm0		; BCLK output disable bit
;
;-------------------------------------------------------
;	Processor mode register 1
;-------------------------------------------------------
pm1				.equ		0005h
;
pm10			.btequ		0,pm1		; CS2 area switching bit
pm11			.btequ		1,pm1		; Port P3_4 to P3_7 function select bit
pm12			.btequ		2,pm1		; Watch dog timer function select bit
pm13			.btequ		3,pm1		; Internal reserved area expansion bit
pm14			.btequ		4,pm1		; Memory area expansion bit
pm15			.btequ		5,pm1		; Memory area expansion bit
pm17			.btequ		7,pm1		; Wait bit
;
;-------------------------------------------------------
;	System clock control register 0
;-------------------------------------------------------
cm0				.equ		0006h
;
cm00			.btequ		0,cm0		; Clock output function select bit
cm01			.btequ		1,cm0		; Clock output function select bit
cm02			.btequ		2,cm0		; WAIT peripheral function clock stop bit
cm03			.btequ		3,cm0		; Xcin-Xcout drive capacity select bit
cm04			.btequ		4,cm0		; Port Xc select bit
cm05			.btequ		5,cm0		; Main clock stop bit
cm06			.btequ		6,cm0		; Main clock division select bit 0
cm07			.btequ		7,cm0		; System clock select bit
;
;-------------------------------------------------------
;	System clock control register 1
;-------------------------------------------------------
cm1				.equ		0007h
;
cm10			.btequ		0,cm1		; All clock stop control bit
cm11			.btequ		1,cm1		; System clock select bit
cm15			.btequ		5,cm1		; Xin-Xout drive capacity select bit
cm16			.btequ		6,cm1		; Main clock division select bit 1
cm17			.btequ		7,cm1		; Main clock division select bit 1
;
;-------------------------------------------------------
;	Chip select control register
;-------------------------------------------------------
csr				.equ		0008h
;
cs0				.btequ		0,csr		; CS0~ output enable bit
cs1				.btequ		1,csr		; CS1~ output enable bit
cs2				.btequ		2,csr		; CS2~ output enable bit
cs3				.btequ		3,csr		; CS3~ output enable bit
cs0w			.btequ		4,csr		; CS0~ wait bit
cs1w			.btequ		5,csr		; CS1~ wait bit
cs2w			.btequ		6,csr		; CS2~ wait bit
cs3w			.btequ		7,csr		; CS3~ wait bit
;
;-------------------------------------------------------
;	Address match interrupt enable register
;-------------------------------------------------------
aier			.equ		0009h
;
aier0			.btequ		0,aier		; Address match interrupt 0 enable bit
aier1			.btequ		1,aier		; Address match interrupt 1 enable bit
;
;-------------------------------------------------------
;	Protect register
;-------------------------------------------------------
prcr			.equ		000ah
;
prc0			.btequ		0,prcr		; Enable writting to system clock control registers 0 and 1
prc1			.btequ		1,prcr		; Enable writting to processor mode registers 0 and 1
prc2			.btequ		2,prcr		; Enable writting to port P9 direction register and SI/Oi control register(i=3,4)
;
;-------------------------------------------------------
;	Data bank register
;-------------------------------------------------------
dbr				 .equ		 000bh
;
ofs				 .btequ		 2,dbr		; Off set bit
bsr0			 .btequ		 3,dbr		; Bank select bit
bsr1			 .btequ		 4,dbr		; Bank select bit
bsr2			 .btequ		 5,dbr		; Bank select bit
;
;-------------------------------------------------------
;	Oscillation stop detection register
;-------------------------------------------------------
cm2				.equ		000ch
;
cm20			.btequ		0,cm2		; Oscillation stop detection bit
cm21			.btequ		1,cm2		; Main clock switch bit
cm22			.btequ		2,cm2		; Oscillation stop detection status
cm23			.btequ		3,cm2		; Clock monitor bit
cm27			.btequ		7,cm2		; Operation select bit(when an oscillation stop is detected)
;
;-------------------------------------------------------
;	Watchdog timer start register
;-------------------------------------------------------
wdts			.equ		000eh
;
;-------------------------------------------------------
;	Watchdog timer control register
;-------------------------------------------------------
wdc				.equ		000fh
;
wdc5			.btequ		5,wdc		; Cold start / warm start discrimination flag
wdc7			.btequ		7,wdc		; Prescaler select bit
;
;-------------------------------------------------------
;	Address match interrupt register 0
;-------------------------------------------------------
rmad0			.equ		0010h
rmad0l			.equ		rmad0		; Address match interrupt register 0L
rmad0m			.equ		rmad0+1		; Address match interrupt register 0M
rmad0h			.equ		rmad0+2		; Address match interrupt register 0H
;
;-------------------------------------------------------
;	Address match interrupt register 1
;-------------------------------------------------------
rmad1			.equ		0014h
rmad1l			.equ		rmad1		; Address match interrupt register 1L
rmad1m			.equ		rmad1+1		; Address match interrupt register 1M
rmad1h			.equ		rmad1+2		; Address match interrupt register 1H
;
;-------------------------------------------------------
;	Power supply detection register 1
;-------------------------------------------------------
vcr1			.equ		0019h
;
vc13			.btequ		3,vcr1		; Power supply down monitor flag
;
;-------------------------------------------------------
;	Power supply detection register 2
;-------------------------------------------------------
vcr2			.equ		001ah
;
vc25			.btequ		5,vcr2		; RAM retention limit detection monitor bit
vc26			.btequ		6,vcr2		; Reset area monitor bit
vc27			.btequ		7,vcr2		; Power supply down monitor bit
;
;-------------------------------------------------------
;	Chip select expansion control register
;-------------------------------------------------------
cse				.equ		001bh
;
cse00w			.btequ		0,cse		; CS0~ wait expansion bit
cse01w			.btequ		1,cse		; CS0~ wait expansion bit
cse10w			.btequ		2,cse		; CS1~ wait expansion bit
cse11w			.btequ		3,cse		; CS1~ wait expansion bit
cse20w			.btequ		4,cse		; CS2~ wait expansion bit
cse21w			.btequ		5,cse		; CS2~ wait expansion bit
cse30w			.btequ		6,cse		; CS3~ wait expansion bit
cse31w			.btequ		7,cse		; CS3~ wait expansion bit
;
;-------------------------------------------------------
;	PLL control register 0
;-------------------------------------------------------
plc0			.equ		001ch
;
plc00			.btequ		0,plc0		; Programmable counter select bit
plc01			.btequ		1,plc0		; Programmable counter select bit
plc02			.btequ		2,plc0		; Programmable counter select bit
plc07			.btequ		7,plc0		; Operation enable bit
;
;-------------------------------------------------------
;	Processor mode register 2
;-------------------------------------------------------
pm2				.equ		001eh
;
pm20			.btequ		0,pm2		; Specifying wait when accessing SFR at PLL operation
pm21			.btequ		1,pm2		; System clock protective bit
pm22			.btequ		2,pm2		; WDT count source protective bit
;
;-------------------------------------------------------
;	Power supply down detection register
;-------------------------------------------------------
d4int			.equ		001fh
;
d40				.btequ		0,d4int		; Power supply down detection interrupt enable bit
d41				.btequ		1,d4int		; STOP mode deactivation control bit
d42				.btequ		2,d4int		; Power supply change detection flag
d43				.btequ		3,d4int		; WDT overflow detect flag
df0				.btequ		4,d4int		; Sampling clock select bit
df1				.btequ		5,d4int		; Sampling clock select bit
;
;-------------------------------------------------------
;	DMA0 source pointer
;-------------------------------------------------------
sar0			.equ		0020h
;
sar0l			.equ		sar0		; DMA0 source pointer L
sar0m			.equ		sar0+1		; DMA0 source pointer M
sar0h			.equ		sar0+2		; DMA0 source pointer H
;
;-------------------------------------------------------
;	DMA0 destination pointer
;-------------------------------------------------------
dar0			.equ		0024h
;
dar0l			.equ		dar0		; DMA0 destination pointer L
dar0m			.equ		dar0+1		; DMA0 destination pointer M
dar0h			.equ		dar0+2		; DMA0 destination pointer H
;
;-------------------------------------------------------
;	DMA0 transfer counter
;-------------------------------------------------------
tcr0			.equ		0028h
;
tcr0l			.equ		tcr0		; DMA0 transfer counter L
tcr0h			.equ		tcr0+1		; DMA0 transfer counter H
;
;-------------------------------------------------------
;	DMA0 control register
;-------------------------------------------------------
dm0con			.equ		002ch
;
dmbit_dm0con	.btequ		0,dm0con	; Transfer unit bit select bit
dmasl_dm0con	.btequ		1,dm0con	; Repeat transfer mode select bit
dmas_dm0con		.btequ		2,dm0con	; DMA request bit
dmae_dm0con		.btequ		3,dm0con	; DMA enable bit
dsd_dm0con		.btequ		4,dm0con	; Source address direction select bit
dad_dm0con		.btequ		5,dm0con	; Destination address direction select bit
;
;-------------------------------------------------------
;	DMA1 source pointer
;-------------------------------------------------------
sar1			.equ		0030h
;
sar1l			.equ		sar1		; DMA1 source pointer L
sar1m			.equ		sar1+1		; DMA1 source pointer M
sar1h			.equ		sar1+2		; DMA1 source pointer H
;
;-------------------------------------------------------
;	DMA1 destination pointer
;-------------------------------------------------------
dar1			.equ		0034h
;
dar1l			.equ		dar1		; DMA1 destination pointer L
dar1m			.equ		dar1+1		; DMA1 destination pointer M
dar1h			.equ		dar1+2		; DMA1 destination pointer H
;
;-------------------------------------------------------
;	DMA1 transfer counter
;-------------------------------------------------------
tcr1			.equ		0038h
;
tcr1l			.equ		tcr1		; DMA1 transfer counter L
tcr1h			.equ		tcr1+1		; DMA1 transfer counter H
;
;-------------------------------------------------------
;	DMA1 control register
;-------------------------------------------------------
dm1con			.equ		003ch
;
dmbit_dm1con	.btequ		0,dm1con	; Transfer unit bit select bit
dmasl_dm1con	.btequ		1,dm1con	; Repeat transfer mode select bit
dmas_dm1con		.btequ		2,dm1con	; DMA request bit
dmae_dm1con		.btequ		3,dm1con	; DMA enable bit
dsd_dm1con		.btequ		4,dm1con	; Source address direction select bit
dad_dm1con		.btequ		5,dm1con	; Destination address direction select bit
;
;-------------------------------------------------------
;	INT3 interrupt control register
;-------------------------------------------------------
int3ic			.equ		0044h
;
ilvl0_int3ic	.btequ		0,int3ic	; Interrupt priority level select bit
ilvl1_int3ic	.btequ		1,int3ic	; Interrupt priority level select bit
ilvl2_int3ic	.btequ		2,int3ic	; Interrupt priority level select bit
ir_int3ic		.btequ		3,int3ic	; Interrupt request bit
pol_int3ic		.btequ		4,int3ic	; Polarity select bit
;
;-------------------------------------------------------
;	Timer B5 interrupt control register
;-------------------------------------------------------
tb5ic			.equ		0045h
;
ilvl0_tb5ic		.btequ		0,tb5ic		; Interrupt priority level select bit
ilvl1_tb5ic		.btequ		1,tb5ic		; Interrupt priority level select bit
ilvl2_tb5ic		.btequ		2,tb5ic		; Interrupt priority level select bit
ir_tb5ic		.btequ		3,tb5ic		; Interrupt request bit
;
;-------------------------------------------------------
;	Timer B4 interrupt control register
;-------------------------------------------------------
tb4ic			.equ		0046h
;
ilvl0_tb4ic		.btequ		0,tb4ic		; Interrupt priority level select bit
ilvl1_tb4ic		.btequ		1,tb4ic		; Interrupt priority level select bit
ilvl2_tb4ic		.btequ		2,tb4ic		; Interrupt priority level select bit
ir_tb4ic		.btequ		3,tb4ic		; Interrupt request bit
;
;-------------------------------------------------------
;	Timer B3 interrupt control register
;-------------------------------------------------------
tb3ic			.equ		0047h
;
ilvl0_tb3ic		.btequ		0,tb3ic		; Interrupt priority level select bit
ilvl1_tb3ic		.btequ		1,tb3ic		; Interrupt priority level select bit
ilvl2_tb3ic		.btequ		2,tb3ic		; Interrupt priority level select bit
ir_tb3ic		.btequ		3,tb3ic		; Interrupt request bit
;
;-------------------------------------------------------
;	UART1 BUS collision detection interrupt control register
;-------------------------------------------------------
u1bcnic			.equ		0046h
;
ilvl0_u1bcnic	.btequ		0,u1bcnic		; Interrupt priority level select bit
ilvl1_u1bcnic	.btequ		1,u1bcnic		; Interrupt priority level select bit
ilvl2_u1bcnic	.btequ		2,u1bcnic		; Interrupt priority level select bit
ir_u1bcnic		.btequ		3,u1bcnic		; Interrupt request bit
;
;-------------------------------------------------------
;	UART0 BUS collision detection interrupt control register
;-------------------------------------------------------
u0bcnic			.equ		0047h
;
ilvl0_u0bcnic	.btequ		0,u0bcnic		; Interrupt priority level select bit
ilvl1_u0bcnic	.btequ		1,u0bcnic		; Interrupt priority level select bit
ilvl2_u0bcnic	.btequ		2,u0bcnic		; Interrupt priority level select bit
ir_u0bcnic		.btequ		3,u0bcnic		; Interrupt request bit
;
;-------------------------------------------------------
;	SI/O4 interrupt control register
;-------------------------------------------------------
s4ic			.equ		0048h
;
ilvl0_s4ic		.btequ		0,s4ic		; Interrupt priority level select bit
ilvl1_s4ic		.btequ		1,s4ic		; Interrupt priority level select bit
ilvl2_s4ic		.btequ		2,s4ic		; Interrupt priority level select bit
ir_s4ic			.btequ		3,s4ic		; Interrupt request bit
pol_s4ic		.btequ		4,s4ic		; Polarity select bit
;
;-------------------------------------------------------
;	SI/O3 interrupt control register
;-------------------------------------------------------
s3ic			.equ		0049h
;
ilvl0_s3ic		.btequ		0,s3ic		; Interrupt priority level select bit
ilvl1_s3ic		.btequ		1,s3ic		; Interrupt priority level select bit
ilvl2_s3ic		.btequ		2,s3ic		; Interrupt priority level select bit
ir_s3ic			.btequ		3,s3ic		; Interrupt request bit
pol_s3ic		.btequ		4,s3ic		; Polarity select bit
;
;-------------------------------------------------------
;	INT5 interrupt control register
;-------------------------------------------------------
int5ic			.equ		0048h
;
ilvl0_int5ic	.btequ		0,int5ic	; Interrupt priority level select bit
ilvl1_int5ic	.btequ		1,int5ic	; Interrupt priority level select bit
ilvl2_int5ic	.btequ		2,int5ic	; Interrupt priority level select bit
ir_int5ic		.btequ		3,int5ic	; Interrupt request bit
pol_int5ic		.btequ		4,int5ic	; Polarity select bit
;
;-------------------------------------------------------
;	INT4 interrupt control register
;-------------------------------------------------------
int4ic			.equ		0049h
;
ilvl0_int4ic	.btequ		0,int4ic	; Interrupt priority level select bit
ilvl1_int4ic	.btequ		1,int4ic	; Interrupt priority level select bit
ilvl2_int4ic	.btequ		2,int4ic	; Interrupt priority level select bit
ir_int4ic		.btequ		3,int4ic	; Interrupt request bit
pol_int4ic		.btequ		4,int4ic	; Polarity select bit
;
;-------------------------------------------------------
;	UART2 BUS collision detection interrupt control register
;-------------------------------------------------------
bcnic			.equ		004ah
;
ilvl0_bcnic		.btequ		0,bcnic		; Interrupt priority level select bit
ilvl1_bcnic		.btequ		1,bcnic		; Interrupt priority level select bit
ilvl2_bcnic		.btequ		2,bcnic		; Interrupt priority level select bit
ir_bcnic		.btequ		3,bcnic		; Interrupt request bit
;
;-------------------------------------------------------
;	DMA0 interrupt control register
;-------------------------------------------------------
dm0ic			.equ		004bh
;
ilvl0_dm0ic		.btequ		0,dm0ic		; Interrupt priority level select bit
ilvl1_dm0ic		.btequ		1,dm0ic		; Interrupt priority level select bit
ilvl2_dm0ic		.btequ		2,dm0ic		; Interrupt priority level select bit
ir_dm0ic		.btequ		3,dm0ic		; Interrupt request bit
;
;-------------------------------------------------------
;	DMA1 interrupt control register
;-------------------------------------------------------
dm1ic			.equ		004ch
;
ilvl0_dm1ic		.btequ		0,dm1ic		; Interrupt priority level select bit
ilvl1_dm1ic		.btequ		1,dm1ic		; Interrupt priority level select bit
ilvl2_dm1ic		.btequ		2,dm1ic		; Interrupt priority level select bit
ir_dm1ic		.btequ		3,dm1ic		; Interrupt request bit
;
;-------------------------------------------------------
;	Key input interrupt control register
;-------------------------------------------------------
kupic			.equ		004dh
;
ilvl0_kupic		.btequ		0,kupic		; Interrupt priority level select bit
ilvl1_kupic		.btequ		1,kupic		; Interrupt priority level select bit
ilvl2_kupic		.btequ		2,kupic		; Interrupt priority level select bit
ir_kupic		.btequ		3,kupic		; Interrupt request bit
;
;-------------------------------------------------------
;	A-D interrupt control register
;-------------------------------------------------------
adic			.equ		004eh
;
ilvl0_adic		.btequ		0,adic		; Interrupt priority level select bit
ilvl1_adic		.btequ		1,adic		; Interrupt priority level select bit
ilvl2_adic		.btequ		2,adic		; Interrupt priority level select bit
ir_adic			.btequ		3,adic		; Interrupt request bit
;
;-------------------------------------------------------
;	UART2 transmit interrupt control register
;-------------------------------------------------------
s2tic			.equ		004fh
;
ilvl0_s2tic		.btequ		0,s2tic		; Interrupt priority level select bit
ilvl1_s2tic		.btequ		1,s2tic		; Interrupt priority level select bit
ilvl2_s2tic		.btequ		2,s2tic		; Interrupt priority level select bit
ir_s2tic		.btequ		3,s2tic		; Interrupt request bit
;
;-------------------------------------------------------
;	UART2 receive interrupt control register
;-------------------------------------------------------
s2ric			.equ		0050h
;
ilvl0_s2ric		.btequ		0,s2ric		; Interrupt priority level select bit
ilvl1_s2ric		.btequ		1,s2ric		; Interrupt priority level select bit
ilvl2_s2ric		.btequ		2,s2ric		; Interrupt priority level select bit
ir_s2ric		.btequ		3,s2ric		; Interrupt request bit
;
;-------------------------------------------------------
;	UART0 transmit interrupt control register
;-------------------------------------------------------
s0tic			.equ		0051h
;
ilvl0_s0tic		.btequ		0,s0tic		; Interrupt priority level select bit
ilvl1_s0tic		.btequ		1,s0tic		; Interrupt priority level select bit
ilvl2_s0tic		.btequ		2,s0tic		; Interrupt priority level select bit
ir_s0tic		.btequ		3,s0tic		; Interrupt request bit
;
;-------------------------------------------------------
;	UART0 receive interrupt control register
;-------------------------------------------------------
s0ric			.equ		0052h
;
ilvl0_s0ric		.btequ		0,s0ric		; Interrupt priority level select bit
ilvl1_s0ric		.btequ		1,s0ric		; Interrupt priority level select bit
ilvl2_s0ric		.btequ		2,s0ric		; Interrupt priority level select bit
ir_s0ric		.btequ		3,s0ric		; Interrupt request bit
;
;-------------------------------------------------------
;	UART1 transmit interrupt control register
;-------------------------------------------------------
s1tic			.equ		0053h
;
ilvl0_s1tic		.btequ		0,s1tic		; Interrupt priority level select bit
ilvl1_s1tic		.btequ		1,s1tic		; Interrupt priority level select bit
ilvl2_s1tic		.btequ		2,s1tic		; Interrupt priority level select bit
ir_s1tic		.btequ		3,s1tic		; Interrupt request bit
;
;-------------------------------------------------------
;	UART1 receive interrupt control register
;-------------------------------------------------------
s1ric			.equ		0054h
;
ilvl0_s1ric		.btequ		0,s1ric		; Interrupt priority level select bit
ilvl1_s1ric		.btequ		1,s1ric		; Interrupt priority level select bit
ilvl2_s1ric		.btequ		2,s1ric		; Interrupt priority level select bit
ir_s1ric		.btequ		3,s1ric		; Interrupt request bit
;
;-------------------------------------------------------
;	Timer A0 interrupt control register
;-------------------------------------------------------
ta0ic			.equ		0055h
;
ilvl0_ta0ic		.btequ		0,ta0ic		; Interrupt priority level select bit
ilvl1_ta0ic		.btequ		1,ta0ic		; Interrupt priority level select bit
ilvl2_ta0ic		.btequ		2,ta0ic		; Interrupt priority level select bit
ir_ta0ic		.btequ		3,ta0ic		; Interrupt request bit
;
;-------------------------------------------------------
;	Timer A1 interrupt control register
;-------------------------------------------------------
ta1ic			.equ		0056h
;
ilvl0_ta1ic		.btequ		0,ta1ic		; Interrupt priority level select bit
ilvl1_ta1ic		.btequ		1,ta1ic		; Interrupt priority level select bit
ilvl2_ta1ic		.btequ		2,ta1ic		; Interrupt priority level select bit
ir_ta1ic		.btequ		3,ta1ic		; Interrupt request bit
;
;-------------------------------------------------------
;	Timer A2 interrupt control register
;-------------------------------------------------------
ta2ic			.equ		0057h
;
ilvl0_ta2ic		.btequ		0,ta2ic		; Interrupt priority level select bit
ilvl1_ta2ic		.btequ		1,ta2ic		; Interrupt priority level select bit
ilvl2_ta2ic		.btequ		2,ta2ic		; Interrupt priority level select bit
ir_ta2ic		.btequ		3,ta2ic		; Interrupt request bit
;
;-------------------------------------------------------
;	Timer A3 interrupt control register
;-------------------------------------------------------
ta3ic			.equ		0058h
;
ilvl0_ta3ic		.btequ		0,ta3ic		; Interrupt priority level select bit
ilvl1_ta3ic		.btequ		1,ta3ic		; Interrupt priority level select bit
ilvl2_ta3ic		.btequ		2,ta3ic		; Interrupt priority level select bit
ir_ta3ic		.btequ		3,ta3ic		; Interrupt request bit
;
;-------------------------------------------------------
;	Timer A4 interrupt control register
;-------------------------------------------------------
ta4ic			.equ		0059h
;
ilvl0_ta4ic		.btequ		0,ta4ic		; Interrupt priority level select bit
ilvl1_ta4ic		.btequ		1,ta4ic		; Interrupt priority level select bit
ilvl2_ta4ic		.btequ		2,ta4ic		; Interrupt priority level select bit
ir_ta4ic		.btequ		3,ta4ic		; Interrupt request bit
;
;-------------------------------------------------------
;	Timer B0 interrupt control register
;-------------------------------------------------------
tb0ic			.equ		005ah
;
ilvl0_tb0ic		.btequ		0,tb0ic		; Interrupt priority level select bit
ilvl1_tb0ic		.btequ		1,tb0ic		; Interrupt priority level select bit
ilvl2_tb0ic		.btequ		2,tb0ic		; Interrupt priority level select bit
ir_tb0ic		.btequ		3,tb0ic		; Interrupt request bit
;
;-------------------------------------------------------
;	Timer B1 interrupt control register
;-------------------------------------------------------
tb1ic			.equ		005bh
;
ilvl0_tb1ic		.btequ		0,tb1ic		; Interrupt priority level select bit
ilvl1_tb1ic		.btequ		1,tb1ic		; Interrupt priority level select bit
ilvl2_tb1ic		.btequ		2,tb1ic		; Interrupt priority level select bit
ir_tb1ic		.btequ		3,tb1ic		; Interrupt request bit
;
;-------------------------------------------------------
;	Timer B2 interrupt control register
;-------------------------------------------------------
tb2ic			.equ		005ch
;
ilvl0_tb2ic		.btequ		0,tb2ic		; Interrupt priority level select bit
ilvl1_tb2ic		.btequ		1,tb2ic		; Interrupt priority level select bit
ilvl2_tb2ic		.btequ		2,tb2ic		; Interrupt priority level select bit
ir_tb2ic		.btequ		3,tb2ic		; Interrupt request bit
;
;-------------------------------------------------------
;	INT0 interrupt control register
;-------------------------------------------------------
int0ic			.equ		005dh
;
ilvl0_int0ic	.btequ		0,int0ic	; Interrupt priority level select bit
ilvl1_int0ic	.btequ		1,int0ic	; Interrupt priority level select bit
ilvl2_int0ic	.btequ		2,int0ic	; Interrupt priority level select bit
ir_int0ic		.btequ		3,int0ic	; Interrupt request bit
pol_int0ic		.btequ		4,int0ic	; Polarity select bit
;
;-------------------------------------------------------
;	INT1 interrupt control register
;-------------------------------------------------------
int1ic			.equ		005eh
;
ilvl0_int1ic	.btequ		0,int1ic	; Interrupt priority level select bit
ilvl1_int1ic	.btequ		1,int1ic	; Interrupt priority level select bit
ilvl2_int1ic	.btequ		2,int1ic	; Interrupt priority level select bit
ir_int1ic		.btequ		3,int1ic	; Interrupt request bit
pol_int1ic		.btequ		4,int1ic	; Polarity select bit
;
;-------------------------------------------------------
;	INT2 interrupt control register
;-------------------------------------------------------
int2ic			.equ		005fh
;
ilvl0_int2ic	.btequ		0,int2ic	; Interrupt priority level select bit
ilvl1_int2ic	.btequ		1,int2ic	; Interrupt priority level select bit
ilvl2_int2ic	.btequ		2,int2ic	; Interrupt priority level select bit
ir_int2ic		.btequ		3,int2ic	; Interrupt request bit
pol_int2ic		.btequ		4,int2ic	; Polarity select bit
;
;-------------------------------------------------------
;	Flash identification register
;-------------------------------------------------------
fidr			.equ		01b4h
;
fidr0			.btequ		0,fidr		; Flash module type identification value
fidr1			.btequ		1,fidr		; Flash module type identification value
;
;-------------------------------------------------------
;	Flash memory control register 1
;-------------------------------------------------------
fmr1			.equ		01b5h
;
fmr11			.btequ		1,fmr1		; EW1 mode select bit
fmr16			.btequ		6,fmr1		; Lock bit status flag
;
;-------------------------------------------------------
;	Flash memory control register 0
;-------------------------------------------------------
fmr0			.equ		01b7h
;
fmr00			.btequ		0,fmr0		; RY/BY~ status flag
fmr01			.btequ		1,fmr0		; EW0 mode select bit
fmr02			.btequ		2,fmr0		; Lock bit disable bit
fmstp			.btequ		3,fmr0		; Flash memory stop bit
fmr05			.btequ		5,fmr0		; User ROM area select bit
fmr06			.btequ		6,fmr0		; Program status flag
fmr07			.btequ		7,fmr0		; Erase status flag
;
;-------------------------------------------------------
;	Address match interrupt register 2
;-------------------------------------------------------
rmad2			.equ		01b8h
rmad2l			.equ		rmad2		; Address match interrupt register 2L
rmad2m			.equ		rmad2+1		; Address match interrupt register 2M
rmad2h			.equ		rmad2+2		; Address match interrupt register 2H
;
;-------------------------------------------------------
;	Address match interrupt enable register 2
;-------------------------------------------------------
aier2			.equ		01bbh
;
aier20			.btequ		0,aier2		; Address match interrupt 2 enable bit
aier21			.btequ		1,aier2		; Address match interrupt 3 enable bit
;
;-------------------------------------------------------
;	Address match interrupt register 3
;-------------------------------------------------------
rmad3			.equ		01bch
rmad3l			.equ		rmad3		; Address match interrupt register 3L
rmad3m			.equ		rmad3+1		; Address match interrupt register 3M
rmad3h			.equ		rmad3+2		; Address match interrupt register 3H
;
;-------------------------------------------------------
;	Peripheral clock select register
;-------------------------------------------------------
pclkr			.equ		025eh
;
pclk0			.btequ		0,pclkr		; TimerA,B clock select bit
pclk1			.btequ		1,pclkr		; SI/O clock select bit
;
;-------------------------------------------------------
;	Timer B3,B4,B5 count start flag
;-------------------------------------------------------
tbsr			.equ		0340h
;
tb3s			.btequ		5,tbsr		; Timer B3 count start flag
tb4s			.btequ		6,tbsr		; Timer B4 count start flag
tb5s			.btequ		7,tbsr		; Timer B5 count start flag
;
;--------------------------------------------------------------
;	Timer A1-1 register : Read and write data in 16-bit unit.
;--------------------------------------------------------------
ta11			.equ		0342h
;
;--------------------------------------------------------------
;	Timer A2-1 register : Read and write data in 16-bit unit.
;--------------------------------------------------------------
ta21			.equ		0344h
;
;--------------------------------------------------------------
;	Timer A4-1 register : Read and write data in 16-bit unit.
;--------------------------------------------------------------
ta41			.equ		0346h
;
;-------------------------------------------------------
;	Three-phase PWM control register 0
;-------------------------------------------------------
invc0			.equ		0348h
;
inv00			.btequ		0,invc0		; Effective interrupt output polarity select bit
inv01			.btequ		1,invc0		; Effective interrupt output specification bit
inv02			.btequ		2,invc0		; Mode select bit
inv03			.btequ		3,invc0		; Output control bit
inv04			.btequ		4,invc0		; Positive and negative phases concurrent L output disable function enable bit
inv05			.btequ		5,invc0		; Positive and negative phases concurrent L output detect flag
inv06			.btequ		6,invc0		; Modulation mode select bit
inv07			.btequ		7,invc0		; Software trigger bit
;
;-------------------------------------------------------
;	Three-phase PWM control register 1
;-------------------------------------------------------
invc1			.equ		0349h
;
inv10			.btequ		0,invc1		; Timer Ai start trigger signal select bit
inv11			.btequ		1,invc1		; Timer A1-1,A2-1,A4-1 control bit
inv12			.btequ		2,invc1		; Dead time timer count source select bit
inv13			.btequ		3,invc1		; Carrier wave detect flag
inv14			.btequ		4,invc1		; Output porality control bit
inv15			.btequ		5,invc1		; Dead time invalid bit
inv16			.btequ		6,invc1		; Dead time timer trigger select bit
inv17			.btequ		7,invc1		; Waveform reflect timing select bit
;
;-------------------------------------------------------
;	Three-phase output buffer register 0
;-------------------------------------------------------
idb0			.equ		034ah
;
du0				.btequ		0,idb0		; U  phase output buffer 0
dub0			.btequ		1,idb0		; U~ phase output buffer 0
dv0				.btequ		2,idb0		; V  phase output buffer 0
dvb0			.btequ		3,idb0		; V~ phase output buffer 0
dw0				.btequ		4,idb0		; W  phase output buffer 0
dwb0			.btequ		5,idb0		; W~ phase output buffer 0
;
;-------------------------------------------------------
;	Three-phase output buffer register 1
;-------------------------------------------------------
idb1			.equ		034bh
;
du1				.btequ		0,idb1		; U  phase output buffer 1
dub1			.btequ		1,idb1	  	; U~ phase output buffer 1
dv1				.btequ		2,idb1	  	; V  phase output buffer 1
dvb1			.btequ		3,idb1	  	; V~ phase output buffer 1
dw1				.btequ		4,idb1	  	; W  phase output buffer 1
dwb1			.btequ		5,idb1	  	; W~ phase output buffer 1
;
;-------------------------------------------------------
;	Dead time timer ; Use "MOV" instruction when writing to this register.
;-------------------------------------------------------
dtt				.equ		034ch
;
;-------------------------------------------------------
;	Timer B2 interrupt occurrences frequency set counter ; Use "MOV" instruction when writing to this register.
;-------------------------------------------------------
ictb2			.equ		034dh
;
;--------------------------------------------------------------
;	Timer B3 register : Read and write data in 16-bit unit.
;--------------------------------------------------------------
tb3				.equ		0350h
;
;--------------------------------------------------------------
;	Timer B4 register : Read and write data in 16-bit unit.
;--------------------------------------------------------------
tb4				.equ		0352h
;
;--------------------------------------------------------------
;	Timer B5 register : Read and write data in 16-bit unit.
;--------------------------------------------------------------
tb5				.equ		0354h
;
;-------------------------------------------------------
;	Timer B3 mode register
;-------------------------------------------------------
tb3mr			.equ		035bh
;
tmod0_tb3mr		.btequ		0,tb3mr		; Operation mode select bit
tmod1_tb3mr		.btequ		1,tb3mr		; Operation mode select bit
mr0_tb3mr		.btequ		2,tb3mr		;
mr1_tb3mr		.btequ		3,tb3mr		;
mr2_tb3mr		.btequ		4,tb3mr		;
mr3_tb3mr		.btequ		5,tb3mr		;
tck0_tb3mr		.btequ		6,tb3mr		; Count source select bit
tck1_tb3mr		.btequ		7,tb3mr		; Count source select bit
;
;-------------------------------------------------------
;	Timer B4 mode register
;-------------------------------------------------------
tb4mr			.equ		035ch
;
tmod0_tb4mr		.btequ		0,tb4mr		; Operation mode select bit
tmod1_tb4mr		.btequ		1,tb4mr		; Operation mode select bit
mr0_tb4mr		.btequ		2,tb4mr		;
mr1_tb4mr		.btequ		3,tb4mr		;
mr3_tb4mr		.btequ		5,tb4mr		;
tck0_tb4mr		.btequ		6,tb4mr		; Count source select bit
tck1_tb4mr		.btequ		7,tb4mr		; Count source select bit
;
;-------------------------------------------------------
;	Timer B5 mode register
;-------------------------------------------------------
tb5mr			.equ		035dh
;
tmod0_tb5mr		.btequ		0,tb5mr		; Operation mode select bit
tmod1_tb5mr		.btequ		1,tb5mr		; Operation mode select bit
mr0_tb5mr		.btequ		2,tb5mr		;
mr1_tb5mr		.btequ		3,tb5mr		;
mr3_tb5mr		.btequ		5,tb5mr		;
tck0_tb5mr		.btequ		6,tb5mr		; Count source select bit
tck1_tb5mr		.btequ		7,tb5mr		; Count source select bit
;
;-------------------------------------------------------
;	Interrupt request cause select register 2
;-------------------------------------------------------
ifsr2a			.equ		035eh
;
ifsr26			.btequ		6,ifsr2a	; Interrupt request cause select bit
ifsr27			.btequ		7,ifsr2a	; Interrupt request cause select bit
;
;-------------------------------------------------------
;	Interrupt request cause select register
;-------------------------------------------------------
ifsr			.equ		035fh
;
ifsr0			.btequ		0,ifsr		; INT0~ interrupt polarity switching bit
ifsr1			.btequ		1,ifsr		; INT1~ interrupt polarity switching bit
ifsr2			.btequ		2,ifsr		; INT2~ interrupt polarity switching bit
ifsr3			.btequ		3,ifsr		; INT3~ interrupt polarity switching bit
ifsr4			.btequ		4,ifsr		; INT4~ interrupt polarity switching bit
ifsr5			.btequ		5,ifsr		; INT5~ interrupt polarity switching bit
ifsr6			.btequ		6,ifsr		; Interrupt request cause select bit
ifsr7			.btequ		7,ifsr		; Interrupt request cause select bit
;
;-------------------------------------------------------
;	SI/O3 transmit/receive register
;-------------------------------------------------------
s3trr			.equ		0360h
;
;-------------------------------------------------------
;	SI/O3 control register
;-------------------------------------------------------
s3c				.equ		0362h
;
sm30			.btequ		0,s3c		; Internal synchronous clock select bit
sm31			.btequ		1,s3c		; Internal synchronous clock select bit
sm32			.btequ		2,s3c		; Sout3 output disable bit
sm33			.btequ		3,s3c		; SI/O3 port select bit
sm34			.btequ		4,s3c		; CLK polarity select bit
sm35			.btequ		5,s3c		; Transfer direction select bit
sm36			.btequ		6,s3c		; Synchronous clock select bit
sm37			.btequ		7,s3c		; Sout3 initial value set bit
;
;-------------------------------------------------------
;	SI/O3 bit rate generator ; Use "MOV" instruction when writing to this register.
;-------------------------------------------------------
s3brg			.equ		0363h
;
;-------------------------------------------------------
;	SI/O4 transmit/receive register
;-------------------------------------------------------
s4trr			.equ		0364h
;
;-------------------------------------------------------
;	SI/O4 control register
;-------------------------------------------------------
s4c				.equ		0366h
;
sm40			.btequ		0,s4c		; Internal synchronous clock select bit
sm41			.btequ		1,s4c		; Internal synchronous clock select bit
sm42			.btequ		2,s4c		; Sout4 output disable bit
sm43			.btequ		3,s4c		; SI/O4 port select bit
sm44			.btequ		4,s4c		; CLK polarity select bit
sm45			.btequ		5,s4c		; Transfer direction select bit
sm46			.btequ		6,s4c		; Synchronous clock select bit
sm47			.btequ		7,s4c		; Sout4 initial value set bit
;
;-------------------------------------------------------
;	SI/O4 bit rate generator ; Use "MOV" instruction when writing to this register.
;-------------------------------------------------------
s4brg			.equ		0367h
;
;-------------------------------------------------------
;	UART0 special mode register 4
;-------------------------------------------------------
u0smr4			.equ		036ch
;
stareq_u0smr4	.btequ		0,u0smr4	; Start condition generate bit
rstareq_u0smr4	.btequ		1,u0smr4	; Restart condition generate bit
stpreq_u0smr4	.btequ		2,u0smr4	; Stop condition generate bit
stspsel_u0smr4	.btequ		3,u0smr4	; SCL,SDA output select bit
ackd_u0smr4		.btequ		4,u0smr4	; ACK data bit
ackc_u0smr4		.btequ		5,u0smr4	; ACK data output enable bit
sclhi_u0smr4	.btequ		6,u0smr4	; SCL output stop enable bit
swc9_u0smr4		.btequ		7,u0smr4	; Final bit L hold enable bit
;
;-------------------------------------------------------
;	UART0 special mode register 3
;-------------------------------------------------------
u0smr3			.equ		036dh
;
ckph_u0smr3		.btequ		1,u0smr3	; Clock phase set bit
nodc_u0smr3		.btequ		3,u0smr3	; Clock output select bit
dl0_u0smr3		.btequ		5,u0smr3	; SDA0(TxD0) digital delay setup bit
dl1_u0smr3		.btequ		6,u0smr3	; SDA0(TxD0) digital delay setup bit
dl2_u0smr3		.btequ		7,u0smr3	; SDA0(TxD0) digital delay setup bit
;
;-------------------------------------------------------
;	UART0 special mode register 2
;-------------------------------------------------------
u0smr2			.equ		036eh
;
iicm2_u0smr2	.btequ		0,u0smr2	; IIC mode selection bit 2
csc_u0smr2		.btequ		1,u0smr2	; Clock-synchronous bit
swc_u0smr2		.btequ		2,u0smr2	; SCL wait output bit
als_u0smr2		.btequ		3,u0smr2	; SDA output stop bit
stac_u0smr2		.btequ		4,u0smr2	; UART0 initialization bit
swc2_u0smr2		.btequ		5,u0smr2	; SCL wait output bit 2
sdhi_u0smr2		.btequ		6,u0smr2	; SDA output disable bit
;
;-------------------------------------------------------
;	UART0 special mode register
;-------------------------------------------------------
u0smr			.equ		036fh
;
iicm_u0smr		.btequ		0,u0smr		; IIC mode selection bit
abc_u0smr		.btequ		1,u0smr		; Arbitration lost detecting flag control bit
bbs_u0smr		.btequ		2,u0smr		; Bus busy flag
lsyn_u0smr		.btequ		3,u0smr		; SCLL sync output enable bit
abscs_u0smr		.btequ		4,u0smr		; Bus collision detect sampring clock select bit
acse_u0smr		.btequ		5,u0smr		; Auto clear function select bit of transmit enable bit
sss_u0smr		.btequ		6,u0smr		; Transmit start condition select bit
;
;-------------------------------------------------------
;	UART1 special mode register 4
;-------------------------------------------------------
u1smr4			.equ		0370h
;
stareq_u1smr4	.btequ		0,u1smr4	; Start condition generate bit
rstareq_u1smr4	.btequ		1,u1smr4	; Restart condition generate bit
stpreq_u1smr4	.btequ		2,u1smr4	; Stop condition generate bit
stspsel_u1smr4	.btequ		3,u1smr4	; SCL,SDA output select bit
ackd_u1smr4		.btequ		4,u1smr4	; ACK data bit
ackc_u1smr4		.btequ		5,u1smr4	; ACK data output enable bit
sclhi_u1smr4	.btequ		6,u1smr4	; SCL output stop enable bit
swc9_u1smr4		.btequ		7,u1smr4	; Final bit L hold enable bit
;
;-------------------------------------------------------
;	UART1 special mode register 3
;-------------------------------------------------------
u1smr3			.equ		0371h
;
ckph_u1smr3		.btequ		1,u1smr3	; Clock phase set bit
nodc_u1smr3		.btequ		3,u1smr3	; Clock output select bit
dl0_u1smr3		.btequ		5,u1smr3	; SDA1(TxD1) digital delay setup bit
dl1_u1smr3		.btequ		6,u1smr3	; SDA1(TxD1) digital delay setup bit
dl2_u1smr3		.btequ		7,u1smr3	; SDA1(TxD1) digital delay setup bit
;
;-------------------------------------------------------
;	UART1 special mode register 2
;-------------------------------------------------------
u1smr2			.equ		0372h
;
iicm2_u1smr2	.btequ		0,u1smr2	; IIC mode selection bit 2
csc_u1smr2		.btequ		1,u1smr2	; Clock-synchronous bit
swc_u1smr2		.btequ		2,u1smr2	; SCL wait output bit
als_u1smr2		.btequ		3,u1smr2	; SDA output stop bit
stac_u1smr2		.btequ		4,u1smr2	; UART0 initialization bit
swc2_u1smr2		.btequ		5,u1smr2	; SCL wait output bit 2
sdhi_u1smr2		.btequ		6,u1smr2	; SDA output disable bit
;
;-------------------------------------------------------
;	UART1 special mode register
;-------------------------------------------------------
u1smr			.equ		0373h
;
iicm_u1smr		.btequ		0,u1smr		; IIC mode selection bit
abc_u1smr		.btequ		1,u1smr		; Arbitration lost detecting flag control bit
bbs_u1smr		.btequ		2,u1smr		; Bus busy flag
lsyn_u1smr		.btequ		3,u1smr		; SCLL sync output enable bit
abscs_u1smr		.btequ		4,u1smr		; Bus collision detect sampring clock select bit
acse_u1smr		.btequ		5,u1smr		; Auto clear function select bit of transmit enable bit
sss_u1smr		.btequ		6,u1smr		; Transmit start condition select bit
;
;-------------------------------------------------------
;	UART2 special mode register 4
;-------------------------------------------------------
u2smr4			.equ		0374h
;
stareq_u2smr4	.btequ		0,u2smr4	; Start condition generate bit
rstareq_u2smr4	.btequ		1,u2smr4	; Restart condition generate bit
stpreq_u2smr4	.btequ		2,u2smr4	; Stop condition generate bit
stspsel_u2smr4	.btequ		3,u2smr4	; SCL,SDA output select bit
ackd_u2smr4		.btequ		4,u2smr4	; ACK data bit
ackc_u2smr4		.btequ		5,u2smr4	; ACK data output enable bit
sclhi_u2smr4	.btequ		6,u2smr4	; SCL output stop enable bit
swc9_u2smr4		.btequ		7,u2smr4	; Final bit L hold enable bit
;
;-------------------------------------------------------
;	UART2 special mode register 3
;-------------------------------------------------------
u2smr3			.equ		0375h
;
ckph_u2smr3		.btequ		1,u2smr3	; Clock phase set bit
nodc_u2smr3		.btequ		3,u2smr3	; Clock output select bit
dl0_u2smr3		.btequ		5,u2smr3	; SDA2(TxD2) digital delay setup bit
dl1_u2smr3		.btequ		6,u2smr3	; SDA2(TxD2) digital delay setup bit
dl2_u2smr3		.btequ		7,u2smr3	; SDA2(TxD2) digital delay setup bit
;
;-------------------------------------------------------
;	UART2 special mode register 2
;-------------------------------------------------------
u2smr2			.equ		0376h
;
iicm2_u2smr2	.btequ		0,u2smr2	; IIC mode selection bit 2
csc_u2smr2		.btequ		1,u2smr2	; Clock-synchronous bit
swc_u2smr2		.btequ		2,u2smr2	; SCL wait output bit
als_u2smr2		.btequ		3,u2smr2	; SDA output stop bit
stac_u2smr2		.btequ		4,u2smr2	; UART0 initialization bit
swc2_u2smr2		.btequ		5,u2smr2	; SCL wait output bit 2
sdhi_u2smr2		.btequ		6,u2smr2	; SDA output disable bit
;
;-------------------------------------------------------
;	UART2 special mode register
;-------------------------------------------------------
u2smr			.equ		0377h
;
iicm_u2smr		.btequ		0,u2smr		; IIC mode selection bit
abc_u2smr		.btequ		1,u2smr		; Arbitration lost detecting flag control bit
bbs_u2smr		.btequ		2,u2smr		; Bus busy flag
lsyn_u2smr		.btequ		3,u2smr		; SCLL sync output enable bit
abscs_u2smr		.btequ		4,u2smr		; Bus collision detect sampring clock select bit
acse_u2smr		.btequ		5,u2smr		; Auto clear function select bit of transmit enable bit
sss_u2smr		.btequ		6,u2smr		; Transmit start condition select bit
;
;-------------------------------------------------------
;	UART2 transmit/receive mode register
;-------------------------------------------------------
u2mr			.equ		0378h
;
smd0_u2mr		.btequ		0,u2mr		; Serial I/O mode select bit
smd1_u2mr		.btequ		1,u2mr		; Serial I/O mode select bit
smd2_u2mr		.btequ		2,u2mr		; Serial I/O mode select bit
ckdir_u2mr		.btequ		3,u2mr		; Internal/external clock select bit
stps_u2mr		.btequ		4,u2mr		; Stop bit length select bit
pry_u2mr		.btequ		5,u2mr		; Odd/even parity select bit
prye_u2mr		.btequ		6,u2mr		; Parity enable bit
iopol_u2mr		.btequ		7,u2mr		; TxD,RxD I/O polarity reverse bit
;
;-------------------------------------------------------
;	UART2 bit rate generator ; Use "MOV" instruction when writing to this register.
;-------------------------------------------------------
u2brg			.equ		0379h
;
;-------------------------------------------------------
;	UART2 transmit buffer register ; Use "MOV" instruction when writing to this register.
;-------------------------------------------------------
u2tb			.equ		037ah
u2tbl			.equ		u2tb		;		Low
u2tbh			.equ		u2tb+1		;		High
;
;-------------------------------------------------------
;	UART2 transmit/receive control register 0
;-------------------------------------------------------
u2c0			.equ		037ch
;
clk0_u2c0		.btequ		0,u2c0		; BRG count source select bit
clk1_u2c0		.btequ		1,u2c0		; BRG count source select bit
crs_u2c0		.btequ		2,u2c0		; CTS~/RTS~ function select bit
txept_u2c0		.btequ		3,u2c0		; Transmit register empty flag
crd_u2c0		.btequ		4,u2c0		; CTS~/RTS~ disable bit
nch_u2c0		.btequ		5,u2c0		; Data output select bit
ckpol_u2c0		.btequ		6,u2c0		; CLK polarity select bit
uform_u2c0		.btequ		7,u2c0		; Transfer format select bit
;
;-------------------------------------------------------
;	UART2 transmit/receive control register 1
;-------------------------------------------------------
u2c1			.equ		037dh
;
te_u2c1			.btequ		0,u2c1		; Transmit enable bit
ti_u2c1			.btequ		1,u2c1		; Transmit buffer empty flag
re_u2c1			.btequ		2,u2c1		; Receive enable bit
ri_u2c1			.btequ		3,u2c1		; Receive complete flag
u2irs			.btequ		4,u2c1		; UART2 transmit interrupt cause select bit
u2rrm			.btequ		5,u2c1		; UART2 continuous receive mode enable bit
u2lch			.btequ		6,u2c1		; Data logic select bit
u2ere			.btequ		7,u2c1		; Error signal output enable bit
;
;-------------------------------------------------------
;	UART2 receive buffer register
;-------------------------------------------------------
u2rb			.equ		037eh
u2rbl			.equ		u2rb		;		Low
u2rbh			.equ		u2rb+1		;		High
abt_u2rb		.btequ		3,u2rbh		; Arbitrastion lost detecting flag
oer_u2rb		.btequ		4,u2rbh		; Overrun error flag
fer_u2rb		.btequ		5,u2rbh		; Framing error flag
per_u2rb		.btequ		6,u2rbh		; Parity error flag
sum_u2rb		.btequ		7,u2rbh		; Error sum flag
;
;-------------------------------------------------------
;	Count start flag
;-------------------------------------------------------
tabsr			.equ		0380h
;
ta0s			.btequ		0,tabsr		; Timer A0 count start flag
ta1s			.btequ		1,tabsr		; Timer A1 count start flag
ta2s			.btequ		2,tabsr		; Timer A2 count start flag
ta3s			.btequ		3,tabsr		; Timer A3 count start flag
ta4s			.btequ		4,tabsr		; Timer A4 count start flag
tb0s			.btequ		5,tabsr		; Timer B0 count start flag
tb1s			.btequ		6,tabsr		; Timer B1 count start flag
tb2s			.btequ		7,tabsr		; Timer B2 count start flag
;
;-------------------------------------------------------
;	Clock prescaler reset flag
;-------------------------------------------------------
cpsrf			.equ		0381h
;
cpsr			.btequ		7,cpsrf		; Clock prescaler reset flag
;
;-------------------------------------------------------
;	One-shot start flag
;-------------------------------------------------------
onsf			.equ		0382h
;
ta0os			.btequ		0,onsf		; Timer A0 one-shot start flag
ta1os			.btequ		1,onsf		; Timer A1 one-shot start flag
ta2os			.btequ		2,onsf		; Timer A2 one-shot start flag
ta3os			.btequ		3,onsf		; Timer A3 one-shot start flag
ta4os			.btequ		4,onsf		; Timer A4 one-shot start flag
tazie			.btequ		5,onsf		; Z-phase input enable bit
ta0tgl			.btequ		6,onsf		; Timer A0 event/trigger select bit
ta0tgh			.btequ		7,onsf		; Timer A0 event/trigger select bit
;
;-------------------------------------------------------
;	Trigger select register
;-------------------------------------------------------
trgsr			.equ		0383h
;
ta1tgl			.btequ		0,trgsr		; Timer A1 event/trigger select bit
ta1tgh			.btequ		1,trgsr		; Timer A1 event/trigger select bit
ta2tgl			.btequ		2,trgsr		; Timer A2 event/trigger select bit
ta2tgh			.btequ		3,trgsr		; Timer A2 event/trigger select bit
ta3tgl			.btequ		4,trgsr		; Timer A3 event/trigger select bit
ta3tgh			.btequ		5,trgsr		; Timer A3 event/trigger select bit
ta4tgl			.btequ		6,trgsr		; Timer A4 event/trigger select bit
ta4tgh			.btequ		7,trgsr		; Timer A4 event/trigger select bit
;
;-------------------------------------------------------
;	Up/down flag ; Use "MOV" instruction when writing to this register.
;-------------------------------------------------------
udf				.equ		0384h
;
;-----------------------------------------------------------
;	Timer A0 register : Read and write data in 16-bit unit.
;-----------------------------------------------------------
ta0				.equ		0386h
;
;-----------------------------------------------------------
;	Timer A1 register : Read and write data in 16-bit unit.
;-----------------------------------------------------------
ta1				.equ		0388h
;
;-----------------------------------------------------------
;	Timer A2 register : Read and write data in 16-bit unit.
;-----------------------------------------------------------
ta2				.equ		038ah
;
;-----------------------------------------------------------
;	Timer A3 register : Read and write data in 16-bit unit.
;-----------------------------------------------------------
ta3				.equ		038ch
;
;-----------------------------------------------------------
;	Timer A4 register : Read and write data in 16-bit unit.
;-----------------------------------------------------------
ta4				.equ		038eh
;
;-----------------------------------------------------------
;	Timer B0 register : Read and write data in 16-bit unit.
;-----------------------------------------------------------
tb0				.equ		0390h
;
;-----------------------------------------------------------
;	Timer B1 register : Read and write data in 16-bit unit.
;-----------------------------------------------------------
tb1				.equ		0392h
;
;-----------------------------------------------------------
;	Timer B2 register : Read and write data in 16-bit unit.
;-----------------------------------------------------------
tb2				.equ		0394h
;
;-----------------------------------------------------------
;	Timer A0 mode register
;-----------------------------------------------------------
ta0mr			.equ		0396h
;
tmod0_ta0mr		.btequ		0,ta0mr		; Operation mode select bit
tmod1_ta0mr		.btequ		1,ta0mr		; Operation mode select bit
mr0_ta0mr		.btequ		2,ta0mr		;
mr1_ta0mr		.btequ		3,ta0mr		;
mr2_ta0mr		.btequ		4,ta0mr		;
mr3_ta0mr		.btequ		5,ta0mr		;
tck0_ta0mr		.btequ		6,ta0mr		; Count source select bit
tck1_ta0mr		.btequ		7,ta0mr		; Count source select bit
;
;-----------------------------------------------------------
;	Timer A1 mode register
;-----------------------------------------------------------
ta1mr			.equ		0397h
;
tmod0_ta1mr		.btequ		0,ta1mr		; Operation mode select bit
tmod1_ta1mr		.btequ		1,ta1mr		; Operation mode select bit
mr0_ta1mr		.btequ		2,ta1mr		;
mr1_ta1mr		.btequ		3,ta1mr		;
mr2_ta1mr		.btequ		4,ta1mr		;
mr3_ta1mr		.btequ		5,ta1mr		;
tck0_ta1mr		.btequ		6,ta1mr		; Count source select bit
tck1_ta1mr		.btequ		7,ta1mr		; Count source select bit
;
;-----------------------------------------------------------
;	Timer A2 mode register
;-----------------------------------------------------------
ta2mr			.equ		0398h
;
tmod0_ta2mr		.btequ		0,ta2mr		; Operation mode select bit
tmod1_ta2mr		.btequ		1,ta2mr		; Operation mode select bit
mr0_ta2mr		.btequ		2,ta2mr		;
mr1_ta2mr		.btequ		3,ta2mr		;
mr2_ta2mr		.btequ		4,ta2mr		;
mr3_ta2mr		.btequ		5,ta2mr		;
tck0_ta2mr		.btequ		6,ta2mr		; Count source select bit
tck1_ta2mr		.btequ		7,ta2mr		; Count source select bit
;
;-----------------------------------------------------------
;	Timer A3 mode register
;-----------------------------------------------------------
ta3mr			.equ		0399h
;
tmod0_ta3mr		.btequ		0,ta3mr		; Operation mode select bit
tmod1_ta3mr		.btequ		1,ta3mr		; Operation mode select bit
mr0_ta3mr		.btequ		2,ta3mr		;
mr1_ta3mr		.btequ		3,ta3mr		;
mr2_ta3mr		.btequ		4,ta3mr		;
mr3_ta3mr		.btequ		5,ta3mr		;
tck0_ta3mr		.btequ		6,ta3mr		; Count source select bit
tck1_ta3mr		.btequ		7,ta3mr		; Count source select bit
;
;-----------------------------------------------------------
;	Timer A4 mode register
;-----------------------------------------------------------
ta4mr			.equ		039ah
;
tmod0_ta4mr		.btequ		0,ta4mr		; Operation mode select bit
tmod1_ta4mr		.btequ		1,ta4mr		; Operation mode select bit
mr0_ta4mr		.btequ		2,ta4mr		;
mr1_ta4mr		.btequ		3,ta4mr		;
mr2_ta4mr		.btequ		4,ta4mr		;
mr3_ta4mr		.btequ		5,ta4mr		;
tck0_ta4mr		.btequ		6,ta4mr		; Count source select bit
tck1_ta4mr		.btequ		7,ta4mr		; Count source select bit
;
;-----------------------------------------------------------
;	Timer B0 mode register
;-----------------------------------------------------------
tb0mr			.equ		039bh
;
tmod0_tb0mr		.btequ		0,tb0mr		; Operation mode select bit
tmod1_tb0mr		.btequ		1,tb0mr		; Operation mode select bit
mr0_tb0mr		.btequ		2,tb0mr		;
mr1_tb0mr		.btequ		3,tb0mr		;
mr2_tb0mr		.btequ		4,tb0mr		;
mr3_tb0mr		.btequ		5,tb0mr		;
tck0_tb0mr		.btequ		6,tb0mr		; Count source select bit
tck1_tb0mr		.btequ		7,tb0mr		; Count source select bit
;
;-----------------------------------------------------------
;	Timer B1 mode register
;-----------------------------------------------------------
tb1mr			.equ		039ch
;
tmod0_tb1mr		.btequ		0,tb1mr		; Operation mode select bit
tmod1_tb1mr		.btequ		1,tb1mr		; Operation mode select bit
mr0_tb1mr		.btequ		2,tb1mr		;
mr1_tb1mr		.btequ		3,tb1mr		;
mr3_tb1mr		.btequ		5,tb1mr		;
tck0_tb1mr		.btequ		6,tb1mr		; Count source select bit
tck1_tb1mr		.btequ		7,tb1mr		; Count source select bit
;
;-----------------------------------------------------------
;	Timer B2 mode register
;-----------------------------------------------------------
tb2mr			.equ		039dh
;
tmod0_tb2mr		.btequ		0,tb2mr		; Operation mode select bit
tmod1_tb2mr		.btequ		1,tb2mr		; Operation mode select bit
mr0_tb2mr		.btequ		2,tb2mr		;
mr1_tb2mr		.btequ		3,tb2mr		;
mr3_tb2mr		.btequ		5,tb2mr		;
tck0_tb2mr		.btequ		6,tb2mr		; Count source select bit
tck1_tb2mr		.btequ		7,tb2mr		; Count source select bit
;
;-----------------------------------------------------------
;	Timer B2 special mode register
;-----------------------------------------------------------
tb2sc			.equ		039eh
;
pwcon			.btequ		0,tb2sc		; Timer B2 reload taiming switching bit
ivpcr1			.btequ		1,tb2sc		; Three phase output port NMI control bit 1
;
;-------------------------------------------------------
;	UART0 transmit/receive mode register
;-------------------------------------------------------
u0mr			.equ		03a0h
;
smd0_u0mr		.btequ		0,u0mr		; Serial I/O mode select bit
smd1_u0mr		.btequ		1,u0mr		; Serial I/O mode select bit
smd2_u0mr		.btequ		2,u0mr		; Serial I/O mode select bit
ckdir_u0mr		.btequ		3,u0mr		; Internal/external clock select bit
stps_u0mr		.btequ		4,u0mr		; Stop bit length select bit
pry_u0mr		.btequ		5,u0mr		; Odd/even parity select bit
prye_u0mr		.btequ		6,u0mr		; Parity enable bit
iopol_u0mr		.btequ		7,u0mr		; TxD,RxD I/O polarity reverse bit
;
;-------------------------------------------------------
;	UART0 bit rate generator ; Use "MOV" instruction when writing to this register.
;-------------------------------------------------------
u0brg			.equ		03a1h
;
;-------------------------------------------------------
;	UART0 transmit buffer register ; Use "MOV" instruction when writing to this register.
;-------------------------------------------------------
u0tb			.equ		03a2h
u0tbl			.equ		u0tb		;		Low
u0tbh			.equ		u0tb+1		;		High
;
;-------------------------------------------------------
;	UART0 transmit/receive control register 0
;-------------------------------------------------------
u0c0			.equ		03a4h
;
clk0_u0c0		.btequ		0,u0c0		; BRG count source select bit
clk1_u0c0		.btequ		1,u0c0		; BRG count source select bit
crs_u0c0		.btequ		2,u0c0		; CTS~/RTS~ function select bit
txept_u0c0		.btequ		3,u0c0		; Transmit register empty flag
crd_u0c0		.btequ		4,u0c0		; CTS~/RTS~ disable bit
nch_u0c0		.btequ		5,u0c0		; Data output select bit
ckpol_u0c0		.btequ		6,u0c0		; CLK polarity select bit
uform_u0c0		.btequ		7,u0c0		; Transfer format select bit
;
;-------------------------------------------------------
;	UART0 transmit/receive control register 1
;-------------------------------------------------------
u0c1			.equ		03a5h
;
te_u0c1			.btequ		0,u0c1		; Transmit enable bit
ti_u0c1			.btequ		1,u0c1		; Transmit buffer empty flag
re_u0c1			.btequ		2,u0c1		; Receive enable bit
ri_u0c1			.btequ		3,u0c1		; Receive complete flag
u0lch			.btequ		6,u0c1		; Data logic select bit
u0ere			.btequ		7,u0c1		; Error signal output enable bit
;
;-------------------------------------------------------
;	UART0 receive buffer register
;-------------------------------------------------------
u0rb			.equ		03a6h
u0rbl			.equ		u0rb		;		Low
u0rbh			.equ		u0rb+1		;		High
abt_u0rb		.btequ		3,u0rbh		; Arbitrastion lost detecting flag
oer_u0rb		.btequ		4,u0rbh		; Overrun error flag
fer_u0rb		.btequ		5,u0rbh		; Framing error flag
per_u0rb		.btequ		6,u0rbh		; Parity error flag
sum_u0rb		.btequ		7,u0rbh		; Error sum flag
;
;-------------------------------------------------------
;	UART1 transmit/receive mode register
;-------------------------------------------------------
u1mr			.equ		03a8h
;
smd0_u1mr		.btequ		0,u1mr		; Serial I/O mode select bit
smd1_u1mr		.btequ		1,u1mr		; Serial I/O mode select bit
smd2_u1mr		.btequ		2,u1mr		; Serial I/O mode select bit
ckdir_u1mr		.btequ		3,u1mr		; Internal/external clock select bit
stps_u1mr		.btequ		4,u1mr		; Stop bit length select bit
pry_u1mr		.btequ		5,u1mr		; Odd/even parity select bit
prye_u1mr		.btequ		6,u1mr		; Parity enable bit
iopol_u1mr		.btequ		7,u1mr		; TxD,RxD I/O polarity reverse bit
;
;-------------------------------------------------------
;	UART1 bit rate generator ; Use "MOV" instruction when writing to this register.
;-------------------------------------------------------
u1brg			.equ		03a9h
;
;-------------------------------------------------------
;	UART1 transmit buffer register ; Use "MOV" instruction when writing to this register.
;-------------------------------------------------------
u1tb			.equ		03aah
u1tbl			.equ		u1tb		;		Low
u1tbh			.equ		u1tb+1		;		High
;
;-------------------------------------------------------
;	UART1 transmit/receive control register 0
;-------------------------------------------------------
u1c0			.equ		03ach
;
clk0_u1c0		.btequ		0,u1c0		; BRG count source select bit
clk1_u1c0		.btequ		1,u1c0		; BRG count source select bit
crs_u1c0		.btequ		2,u1c0		; CTS~/RTS~ function select bit
txept_u1c0		.btequ		3,u1c0		; Transmit register empty flag
crd_u1c0		.btequ		4,u1c0		; CTS~/RTS~ disable bit
nch_u1c0		.btequ		5,u1c0		; Data output select bit
ckpol_u1c0		.btequ		6,u1c0		; CLK polarity select bit
uform_u1c0		.btequ		7,u1c0		; Transfer format select bit
;
;-------------------------------------------------------
;	UART1 transmit/receive control register 1
;-------------------------------------------------------
u1c1			.equ		03adh
;
te_u1c1			.btequ		0,u1c1		; Transmit enable bit
ti_u1c1			.btequ		1,u1c1		; Transmit buffer empty flag
re_u1c1			.btequ		2,u1c1		; Receive enable bit
ri_u1c1			.btequ		3,u1c1		; Receive complete flag
u1lch			.btequ		6,u1c1		; Data logic select bit
u1ere			.btequ		7,u1c1		; Error signal output enable bit
;
;-------------------------------------------------------
;	UART1 receive buffer register
;-------------------------------------------------------
u1rb			.equ		03aeh
u1rbl			.equ		u1rb		;		Low
u1rbh			.equ		u1rb+1		;		High
abt_u1rb		.btequ		3,u1rbh		; Arbitrastion lost detecting flag
oer_u1rb		.btequ		4,u1rbh		; Overrun error flag
fer_u1rb		.btequ		5,u1rbh		; Framing error flag
per_u1rb		.btequ		6,u1rbh		; Parity error flag
sum_u1rb		.btequ		7,u1rbh		; Error sum flag
;
;-------------------------------------------------------
;	UART transmit/receive control register 2
;-------------------------------------------------------
ucon			.equ		03b0h
;
u0irs			.btequ		0,ucon		; UART0 transmit interrupt cause select bit
u1irs			.btequ		1,ucon		; UART1 transmit interrupt cause select bit
u0rrm			.btequ		2,ucon		; UART0 continuous receive mode enable bit
u1rrm			.btequ		3,ucon		; UART1 continuous receive mode enable bit
clkmd0			.btequ		4,ucon		; CLK/CLKS select bit 0
clkmd1			.btequ		5,ucon		; CLK/CLKS select bit 1
rcsp			.btequ		6,ucon		; Separate CTS~/RTS~ bit
;
;--------------------------------------------------------
;	DMA0 request cause select register
;--------------------------------------------------------
dm0sl			.equ		03b8h
;
dsel0_dm0sl		.btequ		0,dm0sl		; DMA request cause select bit
dsel1_dm0sl		.btequ		1,dm0sl		; DMA request cause select bit
dsel2_dm0sl		.btequ		2,dm0sl		; DMA request cause select bit
dsel3_dm0sl		.btequ		3,dm0sl		; DMA request cause select bit
dms_dm0sl		.btequ		6,dm0sl		; DMA request cause expansion select bit
dsr_dm0sl		.btequ		7,dm0sl		; Software DMA request bit
;
;--------------------------------------------------------
;	DMA1 request cause select register
;--------------------------------------------------------
dm1sl			.equ		03bah
;
dsel0_dm1sl		.btequ		0,dm1sl		; DMA request cause select bit
dsel1_dm1sl		.btequ		1,dm1sl		; DMA request cause select bit
dsel2_dm1sl		.btequ		2,dm1sl		; DMA request cause select bit
dsel3_dm1sl		.btequ		3,dm1sl		; DMA request cause select bit
dms_dm1sl		.btequ		6,dm1sl		; DMA request cause expansion select bit
dsr_dm1sl		.btequ		7,dm1sl		; Software DMA request bit
;
;-------------------------------------------------------
;	CRC data register
;-------------------------------------------------------
crcd			.equ		03bch
crcdl			.equ		crcd		;		Low
crcdh			.equ		crcd+1		;		High
;
;-------------------------------------------------------
;	CRC input register
;-------------------------------------------------------
crcin			.equ		03beh
;
;-------------------------------------------------------
;	A-D register 0
;-------------------------------------------------------
ad0				.equ		03c0h
ad0l			.equ		ad0			;		Low
ad0h			.equ		ad0+1		;		High
;
;-------------------------------------------------------
;	A-D register 1
;-------------------------------------------------------
ad1				.equ		03c2h
ad1l			.equ		ad1			;		Low
ad1h			.equ		ad1+1		;		High
;
;-------------------------------------------------------
;	A-D register 2
;-------------------------------------------------------
ad2				.equ		03c4h
ad2l			.equ		ad2			;		Low
ad2h			.equ		ad2+1		;		High
;
;-------------------------------------------------------
;	A-D register 3
;-------------------------------------------------------
ad3				.equ		03c6h
ad3l			.equ		ad3			;		Low
ad3h			.equ		ad3+1		;		High
;
;-------------------------------------------------------
;	A-D register 4
;-------------------------------------------------------
ad4				.equ		03c8h
ad4l			.equ		ad4			;		Low
ad4h			.equ		ad4+1		;		High
;
;-------------------------------------------------------
;	A-D register 5
;-------------------------------------------------------
ad5				.equ		03cah
ad5l			.equ		ad5			;		Low
ad5h			.equ		ad5+1		;		High
;
;-------------------------------------------------------
;	A-D register 6
;-------------------------------------------------------
ad6				.equ		03cch
ad6l			.equ		ad6			;		Low
ad6h			.equ		ad6+1		;		High
;
;-------------------------------------------------------
;	A-D register 7
;-------------------------------------------------------
ad7				.equ		03ceh
ad7l			.equ		ad7			;		Low
ad7h			.equ		ad7+1		;		High
;
;-------------------------------------------------------
;	A-D control register 2
;-------------------------------------------------------
adcon2			.equ		03d4h
;
smp				.btequ		0,adcon2	; A-D conversion method select bit
adgsel0			.btequ		1,adcon2	; A-D input group select bit
adgsel1			.btequ		2,adcon2	; A-D input group select bit
cks2			.btequ		4,adcon2	; Frequency select bit 2
;
;-------------------------------------------------------
;	A-D control register 0
;-------------------------------------------------------
adcon0			.equ		03d6h
;
ch0				.btequ		0,adcon0	; Analog input pin select bit
ch1				.btequ		1,adcon0	; Analog input pin select bit
ch2				.btequ		2,adcon0	; Analog input pin select bit
md0				.btequ		3,adcon0	; A-D operation mode select bit 0
md1				.btequ		4,adcon0	; A-D operation mode select bit 0
trg				.btequ		5,adcon0	; Trigger select bit
adst			.btequ		6,adcon0	; A-D conversion start flag
cks0			.btequ		7,adcon0	; Frequency select bit 0
;
;-------------------------------------------------------
;	A-D control register 1
;-------------------------------------------------------
adcon1			.equ		03d7h
;
scan0			.btequ		0,adcon1	; A-D sweep pin select bit
scan1			.btequ		1,adcon1	; A-D sweep pin select bit
md2				.btequ		2,adcon1	; A-D operation mode select bit 1
bits			.btequ		3,adcon1	; 8/10-bit mode select bit
cks1			.btequ		4,adcon1	; Frequency select bit 1
vcut			.btequ		5,adcon1	; Vref connect bit
opa0			.btequ		6,adcon1	; External op-amp connection mode bit
opa1			.btequ		7,adcon1	; External op-amp connection mode bit
;
;-------------------------------------------------------
;	D-A register 0
;-------------------------------------------------------
da0				.equ		03d8h
;
;-------------------------------------------------------
;	D-A register 1
;-------------------------------------------------------
da1				.equ		03dah
;
;-------------------------------------------------------
;	D-A control register
;-------------------------------------------------------
dacon			.equ		03dch
;
da0e			.btequ		0,dacon		; D-A0 output enable bit
da1e			.btequ		1,dacon		; D-A1 output enable bit
;
;-------------------------------------------------------
;	Port P14 control register (128-pin version)
;-------------------------------------------------------
pc14			.equ		03deh
;
p140			.btequ		0,pc14		; Port P14_0 register
p141			.btequ		1,pc14		; Port P14_1 register
pd140			.btequ		4,pc14		; Port P14_0 direction register
pd141			.btequ		5,pc14		; Port P14_1 direction register
;
;-------------------------------------------------------
;	Pull-up control register 3 (128-pin version)
;-------------------------------------------------------
pur3			.equ		03dfh
;
pu30			.btequ		0,pur3		; P11_0 to P11_3 pull-up
pu31			.btequ		1,pur3		; P11_4 to P11_7 pull-up
pu32			.btequ		2,pur3		; P12_0 to P12_3 pull-up
pu33			.btequ		3,pur3		; P12_4 to P12_7 pull-up
pu34			.btequ		4,pur3		; P13_0 to P13_3 pull-up
pu35			.btequ		5,pur3		; P13_4 to P13_7 pull-up
pu36			.btequ		6,pur3		; P14_0,P14_1 pull-up
pu37			.btequ		7,pur3		; P11 to P14 enabling bit
;
;-------------------------------------------------------
;	Port P0 register
;-------------------------------------------------------
p0				.equ		03e0h
;
p0_0			.btequ		0,p0		; Port P0_0 register
p0_1			.btequ		1,p0		; Port P0_1 register
p0_2			.btequ		2,p0		; Port P0_2 register
p0_3			.btequ		3,p0		; Port P0_3 register
p0_4			.btequ		4,p0		; Port P0_4 register
p0_5			.btequ		5,p0		; Port P0_5 register
p0_6			.btequ		6,p0		; Port P0_6 register
p0_7			.btequ		7,p0		; Port P0_7 register
;
;-------------------------------------------------------
;	Port P1 register
;-------------------------------------------------------
p1				.equ		03e1h
;
p1_0			.btequ		0,p1		; Port P1_0 register
p1_1			.btequ		1,p1		; Port P1_1 register
p1_2			.btequ		2,p1		; Port P1_2 register
p1_3			.btequ		3,p1		; Port P1_3 register
p1_4			.btequ		4,p1		; Port P1_4 register
p1_5			.btequ		5,p1		; Port P1_5 register
p1_6			.btequ		6,p1		; Port P1_6 register
p1_7			.btequ		7,p1		; Port P1_7 register
;
;-------------------------------------------------------
;	Port P0 direction register
;-------------------------------------------------------
pd0				.equ		03e2h
;
pd0_0			.btequ		0,pd0		; Port P0_0 direction register
pd0_1			.btequ		1,pd0		; Port P0_1 direction register
pd0_2			.btequ		2,pd0		; Port P0_2 direction register
pd0_3			.btequ		3,pd0		; Port P0_3 direction register
pd0_4			.btequ		4,pd0		; Port P0_4 direction register
pd0_5			.btequ		5,pd0		; Port P0_5 direction register
pd0_6			.btequ		6,pd0		; Port P0_6 direction register
pd0_7			.btequ		7,pd0		; Port P0_7 direction register
;
;-------------------------------------------------------
;	Port P1 direction register
;-------------------------------------------------------
pd1				.equ		03e3h
;
pd1_0			.btequ		0,pd1		; Port P1_0 direction register
pd1_1			.btequ		1,pd1		; Port P1_1 direction register
pd1_2			.btequ		2,pd1		; Port P1_2 direction register
pd1_3			.btequ		3,pd1		; Port P1_3 direction register
pd1_4			.btequ		4,pd1		; Port P1_4 direction register
pd1_5			.btequ		5,pd1		; Port P1_5 direction register
pd1_6			.btequ		6,pd1		; Port P1_6 direction register
pd1_7			.btequ		7,pd1		; Port P1_7 direction register
;
;-------------------------------------------------------
;	Port P2 register
;-------------------------------------------------------
p2				.equ		03e4h
;
p2_0			.btequ		0,p2		; Port P2_0 register
p2_1			.btequ		1,p2		; Port P2_1 register
p2_2			.btequ		2,p2		; Port P2_2 register
p2_3			.btequ		3,p2		; Port P2_3 register
p2_4			.btequ		4,p2		; Port P2_4 register
p2_5			.btequ		5,p2		; Port P2_5 register
p2_6			.btequ		6,p2		; Port P2_6 register
p2_7			.btequ		7,p2		; Port P2_7 register
;
;-------------------------------------------------------
;	Port P3 register
;-------------------------------------------------------
p3				.equ		03e5h
;
p3_0			.btequ		0,p3		; Port P3_0 register
p3_1			.btequ		1,p3		; Port P3_1 register
p3_2			.btequ		2,p3		; Port P3_2 register
p3_3			.btequ		3,p3		; Port P3_3 register
p3_4			.btequ		4,p3		; Port P3_4 register
p3_5			.btequ		5,p3		; Port P3_5 register
p3_6			.btequ		6,p3		; Port P3_6 register
p3_7			.btequ		7,p3		; Port P3_7 register
;
;-------------------------------------------------------
;	Port P2 direction register
;-------------------------------------------------------
pd2				.equ		03e6h
;
pd2_0			.btequ		0,pd2		; Port P2_0 direction register
pd2_1			.btequ		1,pd2		; Port P2_1 direction register
pd2_2			.btequ		2,pd2		; Port P2_2 direction register
pd2_3			.btequ		3,pd2		; Port P2_3 direction register
pd2_4			.btequ		4,pd2		; Port P2_4 direction register
pd2_5			.btequ		5,pd2		; Port P2_5 direction register
pd2_6			.btequ		6,pd2		; Port P2_6 direction register
pd2_7			.btequ		7,pd2		; Port P2_7 direction register
;
;-------------------------------------------------------
;	Port P3 direction register
;-------------------------------------------------------
pd3				.equ		03e7h
;
pd3_0			.btequ		0,pd3		; Port P3_0 direction register
pd3_1			.btequ		1,pd3		; Port P3_1 direction register
pd3_2			.btequ		2,pd3		; Port P3_2 direction register
pd3_3			.btequ		3,pd3		; Port P3_3 direction register
pd3_4			.btequ		4,pd3		; Port P3_4 direction register
pd3_5			.btequ		5,pd3		; Port P3_5 direction register
pd3_6			.btequ		6,pd3		; Port P3_6 direction register
pd3_7			.btequ		7,pd3		; Port P3_7 direction register
;
;-------------------------------------------------------
;	Port P4 register
;-------------------------------------------------------
p4				.equ		03e8h
;
p4_0			.btequ		0,p4		; Port P4_0 register
p4_1			.btequ		1,p4		; Port P4_1 register
p4_2			.btequ		2,p4		; Port P4_2 register
p4_3			.btequ		3,p4		; Port P4_3 register
p4_4			.btequ		4,p4		; Port P4_4 register
p4_5			.btequ		5,p4		; Port P4_5 register
p4_6			.btequ		6,p4		; Port P4_6 register
p4_7			.btequ		7,p4		; Port P4_7 register
;
;-------------------------------------------------------
;	Port P5 register
;-------------------------------------------------------
p5				.equ		03e9h
;
p5_0			.btequ		0,p5		; Port P5_0 register
p5_1			.btequ		1,p5		; Port P5_1 register
p5_2			.btequ		2,p5		; Port P5_2 register
p5_3			.btequ		3,p5		; Port P5_3 register
p5_4			.btequ		4,p5		; Port P5_4 register
p5_5			.btequ		5,p5		; Port P5_5 register
p5_6			.btequ		6,p5		; Port P5_6 register
p5_7			.btequ		7,p5		; Port P5_7 register
;
;-------------------------------------------------------
;	Port P4 direction register
;-------------------------------------------------------
pd4				.equ		03eah
;
pd4_0			.btequ		0,pd4		; Port P4_0 direction register
pd4_1			.btequ		1,pd4		; Port P4_1 direction register
pd4_2			.btequ		2,pd4		; Port P4_2 direction register
pd4_3			.btequ		3,pd4		; Port P4_3 direction register
pd4_4			.btequ		4,pd4		; Port P4_4 direction register
pd4_5			.btequ		5,pd4		; Port P4_5 direction register
pd4_6			.btequ		6,pd4		; Port P4_6 direction register
pd4_7			.btequ		7,pd4		; Port P4_7 direction register
;
;-------------------------------------------------------
;	Port P5 direction register
;-------------------------------------------------------
pd5				.equ		03ebh
;
pd5_0			.btequ		0,pd5		; Port P5_0 direction register
pd5_1			.btequ		1,pd5		; Port P5_1 direction register
pd5_2			.btequ		2,pd5		; Port P5_2 direction register
pd5_3			.btequ		3,pd5		; Port P5_3 direction register
pd5_4			.btequ		4,pd5		; Port P5_4 direction register
pd5_5			.btequ		5,pd5		; Port P5_5 direction register
pd5_6			.btequ		6,pd5		; Port P5_6 direction register
pd5_7			.btequ		7,pd5		; Port P5_7 direction register
;
;-------------------------------------------------------
;	Port P6 register
;-------------------------------------------------------
p6				.equ		03ech
;
p6_0			.btequ		0,p6		; Port P6_0 register
p6_1			.btequ		1,p6		; Port P6_1 register
p6_2			.btequ		2,p6		; Port P6_2 register
p6_3			.btequ		3,p6		; Port P6_3 register
p6_4			.btequ		4,p6		; Port P6_4 register
p6_5			.btequ		5,p6		; Port P6_5 register
p6_6			.btequ		6,p6		; Port P6_6 register
p6_7			.btequ		7,p6		; Port P6_7 register
;
;-------------------------------------------------------
;	Port P7 register
;-------------------------------------------------------
p7				.equ		03edh
;
p7_0			.btequ		0,p7		; Port P7_0 register
p7_1			.btequ		1,p7		; Port P7_1 register
p7_2			.btequ		2,p7		; Port P7_2 register
p7_3			.btequ		3,p7		; Port P7_3 register
p7_4			.btequ		4,p7		; Port P7_4 register
p7_5			.btequ		5,p7		; Port P7_5 register
p7_6			.btequ		6,p7		; Port P7_6 register
p7_7			.btequ		7,p7		; Port P7_7 register
;
;-------------------------------------------------------
;	Port P6 direction register
;-------------------------------------------------------
pd6				.equ		03eeh
;
pd6_0			.btequ		0,pd6		; Port P6_0 direction register
pd6_1			.btequ		1,pd6		; Port P6_1 direction register
pd6_2			.btequ		2,pd6		; Port P6_2 direction register
pd6_3			.btequ		3,pd6		; Port P6_3 direction register
pd6_4			.btequ		4,pd6		; Port P6_4 direction register
pd6_5			.btequ		5,pd6		; Port P6_5 direction register
pd6_6			.btequ		6,pd6		; Port P6_6 direction register
pd6_7			.btequ		7,pd6		; Port P6_7 direction register
;
;-------------------------------------------------------
;	Port P7 direction register
;-------------------------------------------------------
pd7				.equ		03efh
;
pd7_0			.btequ		0,pd7		; Port P7_0 direction register
pd7_1			.btequ		1,pd7		; Port P7_1 direction register
pd7_2			.btequ		2,pd7		; Port P7_2 direction register
pd7_3			.btequ		3,pd7		; Port P7_3 direction register
pd7_4			.btequ		4,pd7		; Port P7_4 direction register
pd7_5			.btequ		5,pd7		; Port P7_5 direction register
pd7_6			.btequ		6,pd7		; Port P7_6 direction register
pd7_7			.btequ		7,pd7		; Port P7_7 direction register
;
;-------------------------------------------------------
;	Port P8 register
;-------------------------------------------------------
p8				.equ		03f0h
;
p8_0			.btequ		0,p8		; Port P8_0 register
p8_1			.btequ		1,p8		; Port P8_1 register
p8_2			.btequ		2,p8		; Port P8_2 register
p8_3			.btequ		3,p8		; Port P8_3 register
p8_4			.btequ		4,p8		; Port P8_4 register
p8_5			.btequ		5,p8		; Port P8_5 register
p8_6			.btequ		6,p8		; Port P8_6 register
p8_7			.btequ		7,p8		; Port P8_7 register
;
;-------------------------------------------------------
;	Port P9 register
;-------------------------------------------------------
p9				.equ		03f1h
;
p9_0			.btequ		0,p9		; Port P9_0 register
p9_1			.btequ		1,p9		; Port P9_1 register
p9_2			.btequ		2,p9		; Port P9_2 register
p9_3			.btequ		3,p9		; Port P9_3 register
p9_4			.btequ		4,p9		; Port P9_4 register
p9_5			.btequ		5,p9		; Port P9_5 register
p9_6			.btequ		6,p9		; Port P9_6 register
p9_7			.btequ		7,p9		; Port P9_7 register
;
;-------------------------------------------------------
;	Port P8 direction register
;-------------------------------------------------------
pd8				.equ		03f2h
;
pd8_0			.btequ		0,pd8		; Port P8_0 direction register
pd8_1			.btequ		1,pd8		; Port P8_1 direction register
pd8_2			.btequ		2,pd8		; Port P8_2 direction register
pd8_3			.btequ		3,pd8		; Port P8_3 direction register
pd8_4			.btequ		4,pd8		; Port P8_4 direction register
pd8_6			.btequ		6,pd8		; Port P8_6 direction register
pd8_7			.btequ		7,pd8		; Port P8_7 direction register
;
;-------------------------------------------------------
;	Port P9 direction register
;-------------------------------------------------------
pd9				.equ		03f3h
;
pd9_0			.btequ		0,pd9		; Port P9_0 direction register
pd9_1			.btequ		1,pd9		; Port P9_1 direction register
pd9_2			.btequ		2,pd9		; Port P9_2 direction register
pd9_3			.btequ		3,pd9		; Port P9_3 direction register
pd9_4			.btequ		4,pd9		; Port P9_4 direction register
pd9_5			.btequ		5,pd9		; Port P9_5 direction register
pd9_6			.btequ		6,pd9		; Port P9_6 direction register
pd9_7			.btequ		7,pd9		; Port P9_7 direction register
;
;-------------------------------------------------------
;	Port P10 register
;-------------------------------------------------------
p10				.equ		03f4h
;
p10_0			.btequ		0,p10		; Port P10_0 register
p10_1			.btequ		1,p10		; Port P10_1 register
p10_2			.btequ		2,p10		; Port P10_2 register
p10_3			.btequ		3,p10		; Port P10_3 register
p10_4			.btequ		4,p10		; Port P10_4 register
p10_5			.btequ		5,p10		; Port P10_5 register
p10_6			.btequ		6,p10		; Port P10_6 register
p10_7			.btequ		7,p10		; Port P10_7 register
;
;-------------------------------------------------------
;	Port P11 register
;-------------------------------------------------------
p11				.equ		03f5h
;
p11_0			.btequ		0,p11		; Port P11_0 register
p11_1			.btequ		1,p11		; Port P11_1 register
p11_2			.btequ		2,p11		; Port P11_2 register
p11_3			.btequ		3,p11		; Port P11_3 register
p11_4			.btequ		4,p11		; Port P11_4 register
p11_5			.btequ		5,p11		; Port P11_5 register
p11_6			.btequ		6,p11		; Port P11_6 register
p11_7			.btequ		7,p11		; Port P11_7 register
;
;-------------------------------------------------------
;	Port P10 direction register
;-------------------------------------------------------
pd10			.equ		03f6h
;
pd10_0			.btequ		0,pd10		; Port P10_0 direction register
pd10_1			.btequ		1,pd10		; Port P10_1 direction register
pd10_2			.btequ		2,pd10		; Port P10_2 direction register
pd10_3			.btequ		3,pd10		; Port P10_3 direction register
pd10_4			.btequ		4,pd10		; Port P10_4 direction register
pd10_5			.btequ		5,pd10		; Port P10_5 direction register
pd10_6			.btequ		6,pd10		; Port P10_6 direction register
pd10_7			.btequ		7,pd10		; Port P10_7 direction register
;
;-------------------------------------------------------
;	Port P11 direction register
;-------------------------------------------------------
pd11			.equ		03f7h
;
pd11_0			.btequ		0,pd11		; Port P11_0 direction register
pd11_1			.btequ		1,pd11		; Port P11_1 direction register
pd11_2			.btequ		2,pd11		; Port P11_2 direction register
pd11_3			.btequ		3,pd11		; Port P11_3 direction register
pd11_4			.btequ		4,pd11		; Port P11_4 direction register
pd11_5			.btequ		5,pd11		; Port P11_5 direction register
pd11_6			.btequ		6,pd11		; Port P11_6 direction register
pd11_7			.btequ		7,pd11		; Port P11_7 direction register
;
;-------------------------------------------------------
;	Port P12 register
;-------------------------------------------------------
p12				.equ		03f8h
;
p12_0			.btequ		0,p12		; Port P12_0 register
p12_1			.btequ		1,p12		; Port P12_1 register
p12_2			.btequ		2,p12		; Port P12_2 register
p12_3			.btequ		3,p12		; Port P12_3 register
p12_4			.btequ		4,p12		; Port P12_4 register
p12_5			.btequ		5,p12		; Port P12_5 register
p12_6			.btequ		6,p12		; Port P12_6 register
p12_7			.btequ		7,p12		; Port P12_7 register
;
;-------------------------------------------------------
;	Port P13 register
;-------------------------------------------------------
p13				.equ		03f9h
;
p13_0			.btequ		0,p13		; Port P13_0 register
p13_1			.btequ		1,p13		; Port P13_1 register
p13_2			.btequ		2,p13		; Port P13_2 register
p13_3			.btequ		3,p13		; Port P13_3 register
p13_4			.btequ		4,p13		; Port P13_4 register
p13_5			.btequ		5,p13		; Port P13_5 register
p13_6			.btequ		6,p13		; Port P13_6 register
p13_7			.btequ		7,p13		; Port P13_7 register
;
;-------------------------------------------------------
;	Port P12 direction register
;-------------------------------------------------------
pd12			.equ		03fah
;
pd12_0			.btequ		0,pd12		; Port P12_0 direction register
pd12_1			.btequ		1,pd12		; Port P12_1 direction register
pd12_2			.btequ		2,pd12		; Port P12_2 direction register
pd12_3			.btequ		3,pd12		; Port P12_3 direction register
pd12_4			.btequ		4,pd12		; Port P12_4 direction register
pd12_5			.btequ		5,pd12		; Port P12_5 direction register
pd12_6			.btequ		6,pd12		; Port P12_6 direction register
pd12_7			.btequ		7,pd12		; Port P12_7 direction register
;
;-------------------------------------------------------
;	Port P13 direction register
;-------------------------------------------------------
pd13			.equ		03fbh
;
pd13_0			.btequ		0,pd13		; Port P13_0 direction register
pd13_1			.btequ		1,pd13		; Port P13_1 direction register
pd13_2			.btequ		2,pd13		; Port P13_2 direction register
pd13_3			.btequ		3,pd13		; Port P13_3 direction register
pd13_4			.btequ		4,pd13		; Port P13_4 direction register
pd13_5			.btequ		5,pd13		; Port P13_5 direction register
pd13_6			.btequ		6,pd13		; Port P13_6 direction register
pd13_7			.btequ		7,pd13		; Port P13_7 direction register
;
;-------------------------------------------------------
;	Pull-up control register 0
;-------------------------------------------------------
pur0			.equ		03fch
;
pu00			.btequ		0,pur0		; P0_0 to P0_3 pull-up
pu01			.btequ		1,pur0		; P0_4 to P0_7 pull-up
pu02			.btequ		2,pur0		; P1_0 to P1_3 pull-up
pu03			.btequ		3,pur0		; P1_4 to P1_7 pull-up
pu04			.btequ		4,pur0		; P2_0 to P2_3 pull-up
pu05			.btequ		5,pur0		; P2_4 to P2_7 pull-up
pu06			.btequ		6,pur0		; P3_0 to P3_3 pull-up
pu07			.btequ		7,pur0		; P3_4 to P3_7 pull-up
;
;-------------------------------------------------------
;	Pull-up control register 1
;-------------------------------------------------------
pur1			.equ		03fdh
;
pu10			.btequ		0,pur1		; P4_0 to P4_3 pull-up
pu11			.btequ		1,pur1		; P4_4 to P4_7 pull-up
pu12			.btequ		2,pur1		; P5_0 to P5_3 pull-up
pu13			.btequ		3,pur1		; P5_4 to P5_7 pull-up
pu14			.btequ		4,pur1		; P6_0 to P6_3 pull-up
pu15			.btequ		5,pur1		; P6_4 to P6_7 pull-up
pu16			.btequ		6,pur1		; P7_0 to P7_3 pull-up (Except P7_0,P7_1 ; P7_0,P7_1 -> N-channel open drain ports)
pu17			.btequ		7,pur1		; P7_4 to P7_7 pull-up
;
;-------------------------------------------------------
;	Pull-up control register 2
;-------------------------------------------------------
pur2			.equ		03feh
;
pu20			.btequ		0,pur2		; P8_0 to P8_3 pull-up
pu21			.btequ		1,pur2		; P8_4 to P8_7 pull-up (Except P8_5)
pu22			.btequ		2,pur2		; P9_0 to P9_3 pull-up
pu23			.btequ		3,pur2		; P9_4 to P9_7 pull-up
pu24			.btequ		4,pur2		; P10_0 to P10_3 pull-up
pu25			.btequ		5,pur2		; P10_4 to P10_7 pull-up
;
;-------------------------------------------------------
;	Port control register
;-------------------------------------------------------
pcr				.equ		03ffh
;
pcr0			.btequ		0,pcr		; Port P1 control register
