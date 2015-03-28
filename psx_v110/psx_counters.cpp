#include "psx_counters.h"
#include "psx_mem.h"
#include "psx_gpu.h"
#include "interpreter.h"

extern psx_mem mem;
extern psx_gpu GL;

psx_counters::psx_counters()
{
	vsync_count = 0;
	hsync_count = 0;

	//   PAL  : 314 scanlines per frame(13Ah)
	//   NTSC : 263 scanlines per frame(107h)
	//
	//
	//   PAL  : 2167 cycles per scanline(or 3406.1 or so ? )
	//	 NTSC : 2171 cycles per scanline(or 3413.6 or so ? )
	//  
	//   565, 045

	//HSYNC = 2530;
	//VSYNC = 240 * HSYNC;

	t0 = 0;
	t1 = 0;
	t2 = 0;
}


void psx_counters::Update(float cycles)
{
	//return;
	vsync_count += cycles;

	if ( (int)(vsync_count) > VSYNC)
	{

		vsync_count = 0;

		if(mem.int_mask.vsync) 
		{	
			mem.int_reg.vsync = mem.int_mask.vsync;
			Interrupt(INT_GENERAL);
		}

		t1 += 1;
		if (t1 >= 0xFFFF) 
		    t1 = 0;

		//glfwSwapBuffers(GL.screen);
		//glfwPollEvents();
	}
}