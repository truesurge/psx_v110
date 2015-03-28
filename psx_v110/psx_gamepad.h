#pragma once
#include "defines.h"
#include "structures.h"
#include <cstdio>
#include <cstdlib>
#include <conio.h>

class psx_gamepad
{
public:
	u16 Data;
	u16 Baud;
	u16 Stat;
	u16 Ctrl;
	u16 Mode;

    u16 ReadStat();
	u16 ReadCtrl();
	u16 ReadMode();
	u8  ReadData();
	
	void WriteStat(u16 val);
	void WriteCtrl(u16 val);
	void WriteMode(u16 val);
	void WriteData(u8 val);

	psx_gamepad();
	~psx_gamepad();
};

