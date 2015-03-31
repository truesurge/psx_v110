#pragma once
#include "defines.h"
#include "structures.h"

class psx_counters
{
public:
	
	u32 cycles;

	void Manage(); 

	psx_counters();
};

