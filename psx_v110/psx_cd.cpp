#include "psx_cd.h"
#include "psx_mem.h"
#include <cstdio>
#include "interpreter.h"
#include "psx_cpu.h"
#include <cstdlib>
#include <conio.h>

extern psx_mem mem;
extern psx_cpu R3000A;

u32 version[4] = {0x99,0x2,0x1,0xC3}; //01 Feb 1999, ver C3

psx_cd::psx_cd()
{
	index.param_fifo_empty = 1;
}

u8 psx_cd::R0()
{
	//printf("CD : REG0?\n");
	//_getch();
	index.busy = 0;
	index.full |= 0x18;
	index.full |= 0x20;
	return index.full;
}
u8 psx_cd::R1()
{
	//printf("CD : REG1?");
	//_getch();
	if (index.port==1)
	{
		if (response.empty())
		{
			return 0;
		}
		else
		{
			u32 result = response.front();
			response.pop();
			//printf("CD : Responce returned %X\n", result);
			return result;
		}
	}
	return 0;
}
u8 psx_cd::R2()
{
	//printf("CD : REG2?\n");
	//_getch();
	return 0;
}
u8 psx_cd::R3()
{
	//printf("CD : REG3?\n");
	//_getch();
	return int_flag.full;
}


void psx_cd::W0(u8 val)
{
	//printf("CD : REG0=%X\n", val);
	//_getch();
	index.full &= 0xFC;
	index.full |= val&3;
}
void psx_cd::W1(u8 val)
{
	if (index.port == 0)
	{
		switch (val)
		{
		case 0x1:
			printf("CD : GetStat \n");
			int_flag.int_type = 3;
			response.push(index.full);
			mem.int_reg.cdrom = mem.int_mask.cdrom;
			if(mem.int_mask.cdrom) Interrupt(INT_GENERAL);
			break;
		case 0x19:
			printf("CD : Test - ");
			switch (params.front())
			{
			case 0x20:
				printf("GetVersion \n");

				int_flag.int_type = 3;

				response.push(version[0]);
				response.push(version[1]);
				response.push(version[2]);
				response.push(version[3]);

				mem.int_reg.cdrom = mem.int_mask.cdrom;
				Interrupt(INT_GENERAL);
			}
			break;
		default:
			printf("CD : Push cmd %X\n", val);
			break;
		}
	}
}
void psx_cd::W2(u8 val)
{
	if (index.port == 0)
	{
		//printf("CD : Push param %X\n", val);
		params.push(val);
	}
	//else 
		//printf("CD : R2 = %X\n", val);
}
void psx_cd::W3(u8 val)
{
	//printf("CD : REG3=%X\n", val);
	int_flag.full &= 0xF8;
	int_flag.full |= val & 7;
	//_getch();
}