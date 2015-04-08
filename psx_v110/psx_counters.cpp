#include "psx_counters.h"
#include "psx_mem.h"
#include "psx_gpu.h"
#include "interpreter.h"

extern psx_mem mem;
extern psx_gpu GL;

psx_counters::psx_counters()
{
	vsync_cycles=0;
	frames=0;

	Counters[0].mode.full=0;
	Counters[1].mode.full=0;
	Counters[2].mode.full=0;

	Counters[0].target=0;
	Counters[1].target=0;
	Counters[2].target=0;

	Counters[0].value=0;
	Counters[1].value=0;
	Counters[2].value=0;
}

void psx_counters::Advance(int n)
{
	vsync_cycles+=n;

	if ( Counters[2].mode.full!=0 ) Counters[2].value += n ;
}

void psx_counters::WriteMode(u8 n,u32 val)
{
	//printf("T%d Mode = %x\n",n,val);

	if(n==2)
	{
		Counters[2].mode.full=val;
		Counters[2].value = 0;

		if( ( Counters[2].mode.clock_src == 2 ) || ( Counters[2].mode.clock_src == 3 ) ) // SYSCLOCK * 8
			Counters[2].target *= 8;
	}
}

void psx_counters::WriteTarget(u8 n,u32 val)
{
	//printf("T%d Target = %x\n",n,val);
	Counters[n].target = val;
}

u16 psx_counters::GetValue(u8 n)
{
	if(n==1) return (Counters[2].value) / 3;

	if(n==2)
	{
		if( ( Counters[2].mode.clock_src == 2 ) || ( Counters[2].mode.clock_src == 3 ) )
			return (Counters[2].value / 8 ) / 3;
		else return (Counters[2].value) / 3;
	}
}

void psx_counters::Manage()
{
	//PROOF
	if ( vsync_cycles >= VSYNC )
	{
			if(mem.int_mask.vsync)
			{
				Interrupt(INT_GENERAL);
				mem.int_reg.vsync = 1;
			}
			vsync_cycles = 0;
			frames++;
	}

	if(frames>=1)
	{
		frames=0;
		glfwSwapBuffers(GL.screen);
		glfwPollEvents();
	}
}