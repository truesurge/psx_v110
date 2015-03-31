#include "interpreter.h"
#include "psx_mem.h"
#include "psx_cpu.h"
#include "defines.h"
#include "syscalls.h"
#include <cstdio>
#include <conio.h>

extern psx_cpu R3000A;
extern psx_mem mem;

void TestJump()
{
	if (PC == 0x000000b0)
	{
		switch (R9)
		{
		case 0x3d:
			putchar(R4);
			break;
		default:
			break;
		}
	}
};

void Interrupt(int code)
{

	COP0_CAUSE.full |= 0x200;
	COP0_SR.full	|= 0x200;
	COP0_SR.full	|= 0x800;

	COP0_CAUSE.EXCODE=code;

	if(SLOT==0)
	{
		COP0_CAUSE.BD = 0;
		COP0_EPC.full = PC+4;
	}
	else 
	{
		COP0_CAUSE.BD = 1;
		COP0_EPC.full = PC+8;		
	}

	COP0_SR.IEo=COP0_SR.IEp;
	COP0_SR.KUo=COP0_SR.KUp;
	COP0_SR.IEp=COP0_SR.IEc;
	COP0_SR.KUp=COP0_SR.KUc;
	COP0_SR.IEc=0;
	COP0_SR.KUc=0;

	SLOT = 0x80000080;
}

void(*main[])() =
{
	psxSPECIAL, psxBCOND, psxJ, psxJAL, psxBEQ, psxBNE, psxBLEZ, psxBGTZ,
	psxADDI, psxADDIU, psxSLTI, psxSLTIU, psxANDI, psxORI, psxXORI, psxLUI,
	psxCOP0, psxNOP, psxCOP2, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP,
	psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP,
	psxLB, psxLH, psxLWL, psxLW, psxLBU, psxLHU, psxLWR, psxNOP,
	psxSB, psxSH, psxSWL, psxSW, psxNOP, psxNOP, psxSWR, psxNOP,
	psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP,
	psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP
};

void(*spec[])() =
{
	psxSLL, psxNOP, psxSRL, psxSRA, psxSLLV, psxNOP, psxSRLV, psxSRAV,
	psxJR, psxJALR, psxNOP, psxNOP, psxSYSCALL, psxNOP, psxNOP, psxNOP,
	psxMFHI, psxMTHI, psxMFLO, psxMTLO, psxNOP, psxNOP, psxNOP, psxNOP,
	psxMULT, psxMULTU, psxDIV, psxDIVU, psxNOP, psxNOP, psxNOP, psxNOP,
	psxADD, psxADDU, psxSUB, psxSUBU, psxAND, psxOR, psxXOR, psxNOR,
	psxNOP, psxNOP, psxSLT, psxSLTU, psxNOP, psxNOP, psxNOP, psxNOP,
	psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP,
	psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP

};

void(*bcnd[])() =
{
	psxBLTZ, psxBGEZ, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP,
	psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP,
	psxBLTZAL, psxBGEZAL, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP,
	psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP,
	psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP
};

void(*cop0[])() =
{
	psxMFC0, psxNOP, psxMFC0, psxNOP, psxMTC0, psxNOP, psxMTC0, psxNOP,
	psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP,
	psxRFE0, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP,
	psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP, psxNOP
};

void psxSPECIAL() { spec[R3000A.opcode.funct](); };
void psxBCOND()   { bcnd[R3000A.opcode.rt](); };
void psxCOP0()    { cop0[R3000A.opcode.rs](); };
void psxCOP2()	  { };// printf("OEUF : GTE Instruction\n"); _getch(); };
void psxNOP()	  { };

/* 1 */

void psxLB()  { CHECK_RT; RT = (s32)(s8)mem.R8(RS + (s16)IMMEDIATE); };
void psxLBU() { CHECK_RT; RT = (u32)(u8)mem.R8(RS + (s16)IMMEDIATE); };
void psxLH()  { CHECK_RT; RT = (s32)(s16)mem.R16(RS + (s16)IMMEDIATE); };
void psxLHU() { CHECK_RT; RT = (u32)(u16)mem.R16(RS + (s16)IMMEDIATE); };
void psxLW()  { CHECK_RT; RT = mem.R32(RS + (s16)IMMEDIATE); };
void psxLWL()
{
	u32 address = RS + (s16)IMMEDIATE;
	u32 val = mem.R32(address & ~0x3);
	u32 reg = RT;

	switch (address % 4)
	{
	case 0: reg = val; break;
	case 1: reg = ((reg & 0x000000ff) | (val << 8));  break;
	case 2: reg = ((reg & 0x0000ffff) | (val << 16)); break;
	case 3: reg = ((reg & 0x00ffffff) | (val << 24)); break;
	}

	RT = reg;
};
void psxLWR()
{
	u32 address = RS + (s16)IMMEDIATE;
	u32 val = mem.R32(address & ~0x3);
	u32 reg = RT;

	switch (address % 4) {
	case 0: reg = (reg & 0xffffff00) | (val >> 24); break;
	case 1: reg = (reg & 0xffff0000) | (val >> 16); break;
	case 2: reg = (reg & 0xff000000) | (val >> 8); break;
	case 3: reg = val; break;
	}

	RT = reg;
};
void psxSB()  { mem.W8((u32)RS + (s16)IMMEDIATE, (u8)RT); };
void psxSH()  { mem.W16((u32)RS + (s16)IMMEDIATE, (u16)RT); };
void psxSW()  { mem.W32((u32)RS + (s16)IMMEDIATE, (u32)RT); };
void psxSWL()
{
	u32 newval;
	u32 address = RS + (s16)IMMEDIATE;
	u32 treg = RT;
	u32 word = mem.R32(address & ~0x3);

	switch (address % 4) {
	case 0:	newval = treg; break;
	case 1:	newval = (word & 0xff000000) | (treg >> 8); break;
	case 2:	newval = (word & 0xffff0000) | (treg >> 16); break;
	case 3:	newval = (word & 0xffffff00) | (treg >> 24); break;
	}

	mem.W32(address & ~0x3, newval);
};
void psxSWR()
{
	u32 newval;
	u32 address = RS + (s16)IMMEDIATE;
	u32 treg = RT;
	u32 word = mem.R32(address & ~0x3);

	switch (address % 4) {
	case 0:	newval = (word & 0x00ffffff) | (treg << 24); break;
	case 1:	newval = (word & 0x0000ffff) | (treg << 16); break;
	case 2:	newval = (word & 0x000000ff) | (treg << 8); break;
	case 3:	newval = treg; break;
	}

	mem.W32(address & ~0x3, newval);
};

/* 2 */

void psxADDI() { CHECK_RT; RT = (u32)RS + (s32)(s16)IMMEDIATE; };
void psxADDIU(){ CHECK_RT; RT = (u32)RS + (s32)(s16)IMMEDIATE; };
void psxSLTI() { CHECK_RT; RT = (s32)RS <           IMMEDIATE; };
void psxSLTIU(){ CHECK_RT; RT = (u32)RS <      (u32)IMMEDIATE; };
void psxANDI() { CHECK_RT; RT = (u32)RS &      (u32)IMMEDIATE; };
void psxORI()  { CHECK_RT; RT = (u32)RS | (u32)IMMEDIATE; };
void psxXORI() { CHECK_RT; RT = (u32)RS ^ (u32)IMMEDIATE; };
void psxLUI()  { CHECK_RT; RT = (s16)IMMEDIATE << 16; };

/* 3 */

void psxADD()  { CHECK_RD; RD = RS + RT; };
void psxADDU() { CHECK_RD; RD = RS + RT; };
void psxSUB()  { CHECK_RD; RD = RS - RT; };
void psxSUBU() { CHECK_RD; RD = RS - RT; };
void psxSLT()  { CHECK_RD; RD = (s32)RS < (s32)RT; };
void psxSLTU() { CHECK_RD; RD = RS < RT; };
void psxAND()  { CHECK_RD; RD = RS & RT; };
void psxOR()   { CHECK_RD; RD = RS | RT; };
void psxXOR()  { CHECK_RD; RD = RS ^ RT; };
void psxNOR()  { CHECK_RD; RD = ~(RS | RT); };

/* 4 */

void psxSLL()    { CHECK_RD; RD = RT << SHAMT; };
void psxSRL()	 { CHECK_RD; RD = RT >> SHAMT; };
void psxSRA()	 { CHECK_RD; RD = (s32)RT >> SHAMT; };
void psxSLLV()	 { CHECK_RD; RD = (u32)RT << (u32)RS; };
void psxSRLV()	 { CHECK_RD; RD = (u32)RT >> (u32)RS; };
void psxSRAV()	 { CHECK_RD; RD = (s32)RT >> (u32)RS; };

/* 5 */

void psxMULT()	 
{ 
	s64 m = (s64)((s64)(s32)RS*(s64)(s32)RT);
	LO = (u32)(m & 0xffffffff);
	HI = (u32)((m >> 32) & 0xffffffff);
};
void psxMULTU()	 
{ 
	u64 m = (u64)((u64)RS*(u64)RT);
	LO = (u32)(m & 0xffffffff);
	HI = (u32)((m >> 32) & 0xffffffff);
};
void psxDIV()	 
{
	if (RT == 0) return;
	LO = (s32)((s32)RS / (s32)RT);
	HI = (s32)((s32)RS % (s32)RT);
};
void psxDIVU()	 
{
	if (RT == 0) return;
	LO = (RS / RT);
	HI = (RS % RT);
};
void psxMFHI()	 { CHECK_RD; RD = HI; };
void psxMFLO()	 { CHECK_RD; RD = LO; };
void psxMTHI()	 { HI = RD; };
void psxMTLO()	 { LO = RD; };

/* 6 */

void psxJ()      { SLOT = (PC & 0xF0000000) + (TARGET << 2); };
void psxJAL()    { SLOT = (PC & 0xF0000000) + (TARGET << 2); RA = PC + 8; };
void psxJR()     { SLOT = RS; };
void psxJALR()   { SLOT = RS; RD = PC + 8; };

void psxBEQ()    { if (RS == RT)      SLOT = PC + ((s16)IMMEDIATE << 2) + 4; else SLOT = PC + 8; };
void psxBNE()    { if (RS != RT)      SLOT = PC + ((s16)IMMEDIATE << 2) + 4; else SLOT = PC + 8; };
void psxBLEZ()   { if ((s32)RS <= 0)  SLOT = PC + ((s16)IMMEDIATE << 2) + 4; else SLOT = PC + 8; };
void psxBGTZ()   { if ((s32)RS >  0)  SLOT = PC + ((s16)IMMEDIATE << 2) + 4; else SLOT = PC + 8; };
void psxBLTZ()   { if ((s32)RS <  0)  SLOT = PC + ((s16)IMMEDIATE << 2) + 4; else SLOT = PC + 8; };
void psxBGEZ()   { if ((s32)RS >= 0)  SLOT = PC + ((s16)IMMEDIATE << 2) + 4; else SLOT = PC + 8; };

void psxBLTZAL() { RA = PC + 8; if ((s32)RS <  0) SLOT = PC + ((s16)IMMEDIATE << 2) + 4; else PC = PC + 8; };
void psxBGEZAL() { RA = PC + 8; if ((s32)RS >= 0) SLOT = PC + ((s16)IMMEDIATE << 2) + 4; else PC = PC + 8; };
void psxSYSCALL()
{
	Interrupt(INT_SYSCALL);
};

/* 7 */
void psxMTC0()   
{
	switch (RD_PART)
	{
	case 12:
		COP0_SR.full = RT;
		break;
	case 13:
		COP0_CAUSE.full = RT;
		break;
	case 14:
		COP0_EPC.full = RT;
		break;
	default:
		R3000A.cop[RD_PART] = RT;
		break;
	};
};

void psxMFC0()
{
	switch (RD_PART)
	{
	case 12:
		RT=COP0_SR.full;
		break;
	case 13:
		RT = COP0_CAUSE.full;
		break;
	case 14:
		RT = COP0_EPC.full;
		break;
	default:
		RT = R3000A.cop[RD_PART];
		break;
	};
};

void psxRFE0()
{
	COP0_SR.IEc=COP0_SR.IEp;
	COP0_SR.KUc=COP0_SR.KUp;
	COP0_SR.IEp=COP0_SR.IEo;
	COP0_SR.KUp=COP0_SR.KUo;
};