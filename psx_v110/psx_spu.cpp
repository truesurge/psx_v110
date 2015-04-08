#include "defines.h"
#include "psx_spu.h"

static int   spu_sbaddr;
static short spureg[(0x1e00 - 0x1c00) / 2];
static short spumem[512*1024];

void SPU_writeregister(unsigned long reg, unsigned short val) {
	spureg[(reg - 0x1f801c00) / 2] = val;
	switch (reg) {
	case 0x1f801da6: // spu sbaddr
		spu_sbaddr = val * 8;
		break;
	case 0x1f801da8: // spu data
		spumem[spu_sbaddr / 2] = (short)val;
		spu_sbaddr += 2;
		if (spu_sbaddr > 0x7ffff) spu_sbaddr = 0;
		break;
	}
}

unsigned short SPU_readregister(unsigned long reg) {
	switch (reg){
	case 0x1f801da6: // spu sbaddr
		return spu_sbaddr / 8;
	case 0x1f801da8: // spu data
	{
						 int ret = spumem[spu_sbaddr / 2];
						 spu_sbaddr += 2;
						 if (spu_sbaddr > 0x7ffff) spu_sbaddr = 0;
						 return ret;
	}
	default:
		return spureg[(reg - 0x1f801c00) / 2];
	}
	return 0;
}