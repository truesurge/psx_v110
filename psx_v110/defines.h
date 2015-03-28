#pragma once

/* MISC*/
#define gSYSCALLS

#define u8  unsigned char 
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long

#define s8  char
#define s16 short
#define s32 int 
#define s64 long long

#define LOOP while (1)

/* CPU */

#define PC   R3000A.pc
#define SLOT R3000A.slot
#define RA   R3000A.reg[31]
#define R9   R3000A.reg[9]
#define R4   R3000A.reg[4]

/* OPCODE */

#define RS				R3000A.reg[R3000A.opcode.rs]
#define RT				R3000A.reg[R3000A.opcode.rt]
#define RD				R3000A.reg[R3000A.opcode.rd]

#define RD_PART			R3000A.opcode.rd

#define OP				R3000A.opcode.op
#define IMMEDIATE		R3000A.opcode.immediate
#define TARGET			R3000A.opcode.target
#define FUNCT			R3000A.opcode.funct
#define SHAMT			R3000A.opcode.shamt
#define PC				R3000A.pc
#define SLOT    		R3000A.slot
#define HI				R3000A.hi
#define LO				R3000A.lo
#define CHECK_RT		if (R3000A.opcode.rt==0) return
#define CHECK_RD	    if (R3000A.opcode.rd==0) return

/* COPROCESSOR 0 */

#define COP0_SR      R3000A.sr
#define COP0_EPC     R3000A.epc
#define COP0_CAUSE   R3000A.cause

#define SR           sr
#define EPC          epc
#define CAUSE        cause

/* INTERRUPTS */

#define INT_SYSCALL		8
#define INT_GENERAL		0

/* NAMES */

#define JOY_DATA	0x1F801040
#define JOY_STAT	0x1F801044
#define JOY_MODE	0x1F801048
#define JOY_CTRL	0x1F80104A
#define JOY_BAUD	0x1F80104E

#define I_STAT		0x1F801070
#define I_MASK		0x1F801074

#define DMA1_MADR	0x1F801090
#define DMA2_MADR	0x1F8010A0
#define DMA3_MADR	0x1F8010B0
#define DMA4_MADR	0x1F8010C0
#define DMA5_MADR	0x1F8010D0
#define DMA6_MADR	0x1F8010E0
#define DMA0_MADR	0x1F801080

#define DMA1_BCR	0x1F801094
#define DMA2_BCR	0x1F8010A4
#define DMA3_BCR	0x1F8010B4
#define DMA4_BCR	0x1F8010C4
#define DMA5_BCR	0x1F8010D4
#define DMA6_BCR	0x1F8010E4
#define DMA0_BCR	0x1F801084

#define DMA1_CHCR	0x1F801098
#define DMA2_CHCR	0x1F8010A8
#define DMA3_CHCR	0x1F8010B8
#define DMA4_CHCR	0x1F8010C8
#define DMA5_CHCR	0x1F8010D8
#define DMA6_CHCR	0x1F8010E8
#define DMA0_CHCR	0x1F801088

#define DPCR		0x1F8010F0
#define DICR		0x1F8010F4

#define GPU0	    0x1F801810
#define GPU1	    0x1F801814

#define CDREG0		0x1F801800
#define CDREG1		0x1F801801
#define CDREG2		0x1F801802
#define CDREG3		0x1F801803


#define T0_VALUE  0x1F801100
#define T1_VALUE  0x1F801110
#define T2_VALUE  0x1F801120

#define T1_MODE   0x1F801114

#define T0_TARGET  0x1F801108
#define T1_TARGET  0x1F801118
#define T2_TARGET  0x1F801128

/* SCREEN */

//   PAL  : start 256 313 scanlines per frame(13Ah)
//   NTSC : start 243 263 scanlines per frame(107h)
//
//
//   PAL  : 2167 cycles per scanline(or 3406.1 or so ? )
//	 NTSC : 2171 cycles per scanline(or 3413.6 or so ? )
//  
//   565, 045

//#define VSYNC         250 * 1500

#define VSYNC         248 * 1584

/* GPU */

#define CMD_B(n) (((cmd[n] & 0xFF0000) >> 0x10)*1)
#define CMD_G(n) (((cmd[n] & 0x00FF00) >> 0x08)*1)
#define CMD_R(n) ((cmd[n] & 0x0000FF)*1)

#define CMD_W(n)   (cmd[n]&0x0000FFFF)
#define CMD_H(n)   (cmd[n]>>16)

#define CMD_X(n)   (cmd[n]&0x0000FFFF)
#define CMD_Y(n)   (cmd[n]>>16)

#define CMD_TX(n)  (cmd[n]&0xFF)
#define CMD_TY(n)  ((cmd[n]&0xFF00)>>0x8)

#define COLOR16(n) glColor3f((((n) & 0x7C00) >> 10)/31.0, (((n) & 0x3E0) >> 5)/31.0, ((n) & 0x1F)/31.0);
#define COLOR24(n) glColor3f(CMD_R(n)/255.0, CMD_G(n)/255.0, CMD_B(n)/255.0);
#define VERTEX(n)  glVertex2sv((GLshort*)&cmd[n]);

#define CLEAR()	   cmd.clear()
#define REQUIRE(n) if (cmd.size() < n) return