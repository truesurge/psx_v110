#include "psx_gamepad.h"
#include "interpreter.h"
#include "psx_counters.h"
#include "psx_cpu.h"
#include "psx_mem.h"
#include <cstdio>

extern psx_cpu R3000A;
extern psx_counters counters;
extern psx_mem mem;

// Status Flags
#define TX_RDY		0x0001
#define RX_RDY		0x0002
#define TX_EMPTY	0x0004
#define PARITY_ERR	0x0008
#define RX_OVERRUN	0x0010
#define FRAMING_ERR	0x0020
#define SYNC_DETECT	0x0040
#define DSR			0x0080
#define CTS			0x0100
#define IRQ			0x0200

// Control Flags
#define TX_PERM		0x0001
#define DTR			0x0002
#define RX_PERM		0x0004
#define BREAK		0x0008
#define RESET_ERR	0x0010
#define RTS			0x0020
#define SIO_RESET	0x0040

psx_gamepad::psx_gamepad()
{
	StatReg = 0 | TX_RDY | TX_EMPTY;
}

u16 psx_gamepad::ReadStat()
{
	return StatReg;
}

u16 psx_gamepad::ReadCtrl()
{
	return CtrlReg;
}

u8 psx_gamepad::ReadData()
{
	printf("Data?\n");
	mem.int_reg.joy=0;
	_getch();
	return 0x41;
}

u16 psx_gamepad::ReadMode()
{

	return Mode;
}

void psx_gamepad::WriteStat(u16 val)
{
}

void psx_gamepad::WriteData(u8 val)
{
	switch(val)
	{
	case 0x1:
		printf("Joy StartPad();\n");
		StatReg|=IRQ;
		mem.int_reg.joy=1;
		break;
	default:
		printf("Joy data = %x\n",val);
		break;
	}
	StatReg|=RX_RDY;
}

void psx_gamepad::WriteCtrl(u16 value)
{
	CtrlReg = value & ~RESET_ERR;
	if (value & RESET_ERR) StatReg &= ~IRQ;
	if ((CtrlReg & SIO_RESET) || (!CtrlReg)) 
	{
		StatReg = TX_RDY | TX_EMPTY;
		mem.int_reg.joy=1;
	}
}

void psx_gamepad::WriteMode(u16 val)
{
	Mode = val;
}

psx_gamepad::~psx_gamepad()
{
}
