#pragma once

#include "defines.h"
#include "structures.h"

class psx_cpu
{
public:
	t_opcode opcode;

	u32 pc, slot;
	u32 hi, lo;
	u32 reg[32],cop[32];
	u32 cycles; 

	t_sr sr;
	t_epc epc;
	t_cause cause;
	
	bool proceeding;

	u32 vsync_counter;

	void Fetch();
	void Execute();

	psx_cpu();
};

