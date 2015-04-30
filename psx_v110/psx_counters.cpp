#include "psx_counters.h"
#include "psx_gamepad.h"
#include "psx_mem.h"
#include "psx_gpu.h"
#include "interpreter.h"

extern psx_mem mem;
extern psx_gpu GL;
extern psx_gamepad pad;

psx_counters::psx_counters()
{
	frames=0;
	vsync_cycles=0;
	delay=-1;

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
}

void psx_counters::Tick()
{
	static u8  w8=0;
	static u16 hc=0;

	//T1 hsync / sysclock;
	if(( Counters[1].mode.clock_src == 1 ) || ( Counters[1].mode.clock_src == 3 ) )
	{
		hc++;
		if(hc>=HSYNC) 
		{
			Counters[1].value+=2;
			hc=0;
		}
	}
	else Counters[1].value+=2;

	if(Counters[1].value>=0xFFFF)
	{
		Counters[1].mode.reached_ff=1;
		if(Counters[1].mode.irq_on_ff) mem.int_reg.tmr1=1;
		Counters[1].value=0;
	}

	if(Counters[1].mode.irq_on_tg && (Counters[1].value>=Counters[1].target) && Counters[1].target) 
	{
		Counters[1].mode.reached_tg=1;
		mem.int_reg.tmr1=1;
		Counters[1].value=0;
	}

	//T2 mult8 / sysclock;
	if(( Counters[2].mode.clock_src == 2 ) || ( Counters[2].mode.clock_src == 3 ) )
	{
		w8++;
		if(w8>=7) 
		{
			Counters[2].value+=2;
			w8=0;
		}
	}
	else Counters[2].value+=2;

	if(Counters[2].value>=0xFFFF)
	{
		Counters[2].mode.reached_ff=1;
		if(Counters[2].mode.irq_on_ff) mem.int_reg.tmr2=1;
		Counters[2].value=0;
	}

	if(Counters[2].mode.irq_on_tg && (Counters[2].value>=Counters[2].target) && Counters[2].target) 
	{
		Counters[2].mode.reached_tg=1;
		mem.int_reg.tmr2=1;
		Counters[2].value=0;
	}

	if(delay==0)
	{
		pad.StatReg|=0x0200;
		mem.int_reg.joy=1;
	}
	else delay--;
}

void psx_counters::WriteMode(u8 n,u32 val)
{
	//printf("T%d Mode = %x\n",n,val);
	//_getch();
	Counters[n].mode.full=val;
	Counters[n].value=0;
}

void psx_counters::WriteTarget(u8 n,u32 val)
{
	//printf("T%d Target = %x\n",n,val);
	Counters[n].target = val;
}

u16 psx_counters::GetValue(u8 n)
{
	return Counters[n].value ;
}

void psx_counters::Manage()
{
	static int time=0;

	//PROOF
	if ( vsync_cycles >= VSYNC )
	{
		static double t1 = glfwGetTime();
		frames++;
		
		if(mem.int_mask.vsync)
		{
			mem.int_reg.vsync = 1;
			Interrupt(INT_GENERAL);
			GL.ShowVram();
			glfwSwapBuffers(GL.screen);
			glfwPollEvents();
		}
		vsync_cycles = 0;
		
		if(frames==60) //pal
		{
			time+=1;
			double t2 = glfwGetTime();
			char title[200];
			sprintf(title,"FPS : %.1f, %d sec",60.0/(t2-t1),time);
			glfwSetWindowTitle(GL.screen,title);
			t1=t2;
			frames=0;
		}
	}
}