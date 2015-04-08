#pragma once
#include "defines.h"
#include "structures.h"

union Mode
{
	u32 full;
	struct
	{
		unsigned sync_enable : 1;
		unsigned sync_mode   : 2;
		unsigned reset       : 1;
		unsigned irq_on_tg   : 1;
		unsigned irq_on_ff   : 1;
		unsigned irq_repeat  : 1;
		unsigned irq_toggle  : 1;
		unsigned clock_src   : 2;
		unsigned irq		 : 1;
		unsigned reached_tg  : 1;
		unsigned reached_ff  : 1;
		unsigned			 : 19;

	};
};

struct Counter
{
	u32 value;
	u32 target;
	Mode mode;
};

class psx_counters
{
public:
	u32    hsync_cycles;
	u32    vsync_cycles;
	u32	   frames;
	double t1,t2;

	Counter Counters[3];

	void WriteMode(u8 n,u32 val);
	void WriteTarget(u8 n,u32 val);
	u16  GetValue(u8 n);
	void Advance(int n);
	void Manage(); 

	psx_counters();
};

