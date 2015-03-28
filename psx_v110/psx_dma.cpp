#include "psx_dma.h"
#include "psx_mem.h"
#include "psx_gpu.h"
#include "interpreter.h"
#include "psx_counters.h"

extern psx_counters counters;
extern psx_mem mem;
extern psx_gpu GL;

psx_dma::psx_dma()
{
}

void psx_dma::Channel2()
{
	if (chcr[2].full == 0x01000201)
	{
		//printf("DMA : Channel 2 started [vram transfer] \n");
		u32 *ptr = (u32*)(mem.ram + (madr[2] & 0x1fffff));
		u32 size = bcr[2].ba * bcr[2].bs;
		GL.WriteList(ptr, size);
		madr[2] += size;
	}

	if (chcr[2].full == 0x01000401)
	{
		//printf("DMA : Channel 2 started [linked list] \n");
		u32 *ptr = (u32*)(mem.ram + (madr[2] & 0x1fffff));
		u32 nextaddr;
		u32 size;
		do
		{
			size = ptr[0] >> 24;
			nextaddr = ptr[0] & 0xffffff;
			madr[2] = nextaddr;
			GL.WriteList(ptr + 1, size);
			ptr = (u32*)(mem.ram + (nextaddr & 0x1fffff));
		} while (nextaddr != 0xffffff);
	}

	chcr[2].busy1 = 0;
	chcr[2].busy2 = 0;
	dicr.irq_2 = 1;
	if (dicr.force_irq == 1)
	{
		dicr.master_irq = 1;
		mem.int_reg.dma = mem.int_mask.dma;
		//if(mem.int_mask.dma) Interrupt(INT_GENERAL);
	}
	else if (dicr.master_enable | (((dicr.full & 0x7F0000) >> 0x10) & ((dicr.full & 0x7F000000) >> 0x18)))
	{
		dicr.master_irq = 1;
		mem.int_reg.dma = mem.int_mask.dma;
		//if(mem.int_mask.dma) Interrupt(INT_GENERAL);
	}
	else dicr.master_irq = 0;
	GL.GP1.dma_directon = 0;
}

void psx_dma::Channel6()
{
	if (chcr[6].full == 0x11000002)
	{
		//printf("DMA : Channel 6 started\n");
		u32 *_mem = (u32*)(mem.ram + (madr[6] & 0x1fffff));
		u32 size = bcr[6].full;
		while (bcr[6].full--)
		{
			*_mem-- = ((madr[6] - 4) & 0xffffff);
			madr[6] -= 4;
		}
		_mem++; *_mem = 0xffffff;
		counters.Update(size*2);
	}

	chcr[6].busy1 = 0;
	chcr[6].busy2 = 0;
	dicr.irq_6 = 1;
	if (dicr.force_irq == 1)
	{
		dicr.master_irq = 1;
		mem.int_reg.dma = mem.int_mask.dma;
		//if(mem.int_mask.dma) Interrupt(INT_GENERAL);
	}
	else if (dicr.master_enable | (((dicr.full & 0x7F0000) >> 0x10) & ((dicr.full & 0x7F000000) >> 0x18)))
	{
		dicr.master_irq = 1;
		mem.int_reg.dma = mem.int_mask.dma;
		//if(mem.int_mask.dma) Interrupt(INT_GENERAL);
	}
	else dicr.master_irq = 0;
	GL.GP1.dma_directon = 0;
}

psx_dma::~psx_dma()
{
}
