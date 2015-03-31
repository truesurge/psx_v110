#include "psx_counters.h"
#include "psx_mem.h"
#include "psx_gpu.h"
#include "interpreter.h"

extern psx_mem mem;
extern psx_gpu GL;

psx_counters::psx_counters()
{
	cycles=0;
}

void psx_counters::Manage()
{
	if ( cycles > VSYNC )
	{
		if(mem.int_mask.vsync)
		{
			mem.int_reg.vsync = mem.int_mask.vsync;
			Interrupt(INT_GENERAL);
			glfwSwapBuffers(GL.screen);
			glfwPollEvents();
		}
		cycles = 0;
	}
}