#pragma once
#include "defines.h"
#include "structures.h"
#include "psx_gpu.h"
#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <vector>

union padstate
{
	u16 full;
	struct
	{
		u8 l;
		u8 r;
	};
	struct
	{
		unsigned select :1 ;
		unsigned  :1 ;
		unsigned  :1 ;
		unsigned start :1 ;
		unsigned up :1 ;
		unsigned right :1 ;
		unsigned down:1 ;
		unsigned left:1 ;
		unsigned l2:1 ;
		unsigned r2:1 ;
		unsigned l1:1 ;
		unsigned r1:1 ;
		unsigned triangle:1 ;
		unsigned circle:1 ;
		unsigned cross:1 ;
		unsigned square:1 ;
	};
};

class psx_gamepad
{
public:
	u16 Data;
	u16 Baud;
	u16 StatReg; 
	u16 CtrlReg; 
	u16 Mode;
	std::vector<u8> buffer; 

	padstate state[2];

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

