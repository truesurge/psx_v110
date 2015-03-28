#pragma once
#include "defines.h"
#include "structures.h"

class psx_counters
{
public:
	
	u32 vsync_count;
	float hsync_count;

	//u32 VSYNC,HSYNC;

	u32 t0;
	u32 t1;
	u32 t2;

	u32 t1_target;

	void Update(float cycles);

	psx_counters();
};

