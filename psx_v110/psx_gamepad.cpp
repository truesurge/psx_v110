#include "psx_gamepad.h"
#include "interpreter.h"
#include "psx_cpu.h"
#include "psx_mem.h"
#include <cstdio>

extern psx_cpu R3000A;
extern psx_mem mem;

psx_gamepad::psx_gamepad()
{
	Stat = Mode = Ctrl = 0;
	Stat |= 5;
	Ctrl |= 1;
}

u16 psx_gamepad::ReadStat()
{
	_getch();
	return Stat;
}

u16 psx_gamepad::ReadCtrl()
{
	_getch();
	return 0x3003;
}

u8 psx_gamepad::ReadData()
{
	_getch();
	return 0;
}

u16 psx_gamepad::ReadMode()
{
	_getch();
	return Mode;
}

void psx_gamepad::WriteData(u8 val)
{
	_getch();
	
	Data = val;

	Ctrl |= 0x200;
	Ctrl |= 0x400;

	Stat |= 5;
	Stat |= 0x100;

	mem.int_reg.joy=1;
	Interrupt(INT_GENERAL);
}


void psx_gamepad::WriteStat(u16 val)
{
	_getch();
}

void psx_gamepad::WriteCtrl(u16 val)
{
	_getch();
	Ctrl = val;
}

void psx_gamepad::WriteMode(u16 val)
{
	_getch();
	Mode = val;
}

psx_gamepad::~psx_gamepad()
{
}
