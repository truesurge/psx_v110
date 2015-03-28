#include "psx_mem.h"
#include "psx_cpu.h"
#include "psx_gpu.h"
#include "psx_dma.h"
#include "psx_cd.h"
#include "psx_spu.h"
#include "psx_counters.h"
#include "psx_gamepad.h"
#include <cstdio>
#include <cstdlib>
#include <conio.h>

extern psx_cpu R3000A;
extern psx_gpu GL;
extern psx_dma dma;
extern psx_counters counters;
extern psx_cd cd;

psx_gamepad pad;

psx_mem::psx_mem()
{
	FILE* file;
	fopen_s(&file, "./bios/dtlh3002.bin", "rb");
	if (file == NULL)
	{
		printf("Place scph101.bin in bios folder\n");
		exit(1);
	}
	fread(rom, 512 * 1024, 1, file);
	fclose(file);
}

void psx_mem::W8(u32 address, u8 val)
{
	u32 real = address & 0x1FFFFFFF;
	if (real <= 0x200000)  { if (COP0_SR.Isc) return; *(u8*)(ram + (real & 0x3FFFFF)) = val; return; };
	if ((real >= 0x1F800000) && (real <= 0x1F800400)) { if (COP0_SR.Isc) return; *(u8*)(scratch + (real & 0x3FF)) = val; return; };
	if ((real >= 0x1F801000) && (real <= 0x1F804000))
	{
		switch (real)
		{
		case CDREG0:
			cd.W0(val);
			break;
		case CDREG1:
			cd.W1(val);
			break;
		case CDREG2:
			cd.W2(val);
			break;
		case CDREG3:
			cd.W3(val);
			break;
		case 0x1F801040:
			printf("PAD 8 DATA = %x\n",val);
			pad.WriteData(val);
			break;
		case 0x1F801044:
			printf("PAD 8 STAT = %x\n",val);
			pad.WriteStat(val);
			break;
		case 0x1F801048:
			printf("PAD 8 MODE = %x\n",val);
			pad.WriteMode(val);
			break;
		case 0x1F80104A:
			printf("PAD 8 CTRL = %x\n",val);
			pad.WriteCtrl(val);
			break;
		case 0x1F02041:
			break;
		default:
			printf("W8 : WTF 0x%08X = %X \n", address, val);
			*(u8*)(io + (real & 0x1FFF)) = val;
			break;
		}
	}
}

void psx_mem::W16(u32 address, u16 val)
{
	u32 real = address & 0x1FFFFFFF;
	if (real <= 0x200000)  { if (COP0_SR.Isc) return; *(u16*)(ram + (real & 0x3FFFFF)) = val; return; };
	if ((real >= 0x1F800000) && (real <= 0x1F800400)) { if (COP0_SR.Isc) return; *(u16*)(scratch + (real & 0x3FF)) = val; return; };
	if ((real >= 0x1F801000) && (real <= 0x1F804000))
	{
		switch (real)
		{
		case 0x1F801040:
			printf("PAD 16 DATA = %x\n",val);
			pad.WriteData((u8)val);
			pad.WriteData((u8)(val>>8));
			break;
		case 0x1F801044:
			printf("PAD 16 STAT = %x\n",val);
			pad.WriteStat(val);
			break;
		case 0x1F801048:
			printf("PAD 16 MODE = %x\n",val);
			pad.WriteMode(val);
			break;
		case 0x1F80104A:
			printf("PAD 16 CTRL = %x\n",val);
			pad.WriteCtrl(val);
			break;
		case I_MASK:
			int_mask.full = val;
			break;
		case I_STAT:
			int_reg.full &= int_mask.full & val;
			break;
		default:
		{
				   if (address >= 0x1f801c00 && address < 0x1f801e00) {
					   SPU_writeRegister(address, val);
					   return;
				   } 
				   else
				   {
					   printf("W16 : WTF 0x%08X = %X \n", address, val);
					   *(u16*)(io + (real & 0x1FFF)) = val;
					   break;
				   }
		}
		}
	}
}

void psx_mem::W32(u32 address, u32 val)
{
	u32 real = address & 0x1FFFFFFF;
	if (real <= 0x200000)  { if (COP0_SR.Isc) return; *(u32*)(ram + (real & 0x3FFFFF)) = val; return; };
	if ((real >= 0x1F800000) && (real <= 0x1F800400)) { if (COP0_SR.Isc) return; *(u32*)(scratch + (real & 0x3FF)) = val; return; };
	if ((real >= 0x1F801000) && (real <= 0x1F804000))
	{
		switch (real)
		{
		case DICR:
			dma.dicr.full=(dma.dicr.full& (0x80000000))|(dma.dicr.full& ~val & 0x7f000000)|		(val & 0x00ffffff);
			if ((dma.dicr.full& 0x80000000) != 0 && (dma.dicr.full& 0x7f000000) == 0) {
				dma.dicr.full&= ~0x80000000;
			}
			break;
		case DPCR:
			dma.dpcr.full |= val;
			break;
		case DMA2_BCR:
			dma.bcr[2].full = val;
			break;
		case DMA2_MADR:
			dma.madr[2] = val;
			break;
		case DMA2_CHCR:
			dma.chcr[2].full = val;
			dma.Channel2();
			break;
		case DMA6_BCR:
			dma.bcr[6].full = val;
			break;
		case DMA6_MADR:
			dma.madr[6] = val;
			break;
		case DMA6_CHCR:
			dma.chcr[6].full = val;
			dma.Channel6();
			break;
		case I_MASK:
			int_mask.full = val;
			break;
		case I_STAT:
			int_reg.full &= int_mask.full & val;
			break;
		case GPU0:
			GL.WriteGP0(val);
			break;
		case GPU1:
			GL.WriteGP1(val);
			break;
		case T1_VALUE:
			counters.t1 = val;
			break;
		case JOY_STAT:
			printf("JOYSTAT 32\n");
			break;
		case JOY_CTRL:
			printf("JOYCTRL 32\n");
			break;
		case JOY_MODE:
			printf("JOYMODE 32\n");
			break;
		case JOY_DATA:
			printf("JOYDATA 32\n");
			break;
		default:
			//printf("W32 : WTF 0x%08X = %X \n", address, val);
			*(u32*)(io + (real & 0x1FFF)) = val;
			break;
		}
	}
}

u8 psx_mem::R8(u32 address)
{
	u32 real = address & 0x1FFFFFFF;
	if (real <= 0x200000) return *(u8*)(ram + (real & 0x3FFFFF));
	if ((real >= 0x1F800000) && (real <= 0x1F800400)) return *(u8*)(scratch + (real & 0x3FF));
	if ((real >= 0x1FC00000) && (real <= 0x1FC80000)) return *(u8*)(rom + (real & 0x7FFFF));
	if ((real >= 0x1F801000) && (real <= 0x1F804000))
	{
		switch (real)
		{
		case CDREG0:
			return cd.R0();
			break;
		case CDREG1:
			return cd.R1();
			break;
		case CDREG2:
			return cd.R2();
			break;
		case CDREG3:
			return cd.R3();
			break;
		default:
			printf("R8 : WTF 0x%08X ? \n", address);
			return *(u8*)(io + (real & 0x1FFF));
			break;
		}
	}
	return 0;
}

u16 psx_mem::R16(u32 address)
{
	u32 real = address & 0x1FFFFFFF;
	if (real <= 0x200000) return *(u32*)(ram + (real & 0x3FFFFF));
	if ((real >= 0x1F800000) && (real <= 0x1F800400)) return *(u16*)(scratch + (real & 0x3FF));
	if ((real >= 0x1FC00000) && (real <= 0x1FC80000)) return *(u16*)(rom + (real & 0x7FFFF));
	if ((real >= 0x1F801000) && (real <= 0x1F804000))
	{
		switch (real)
		{
		case 0x1F801040:
			printf("PAD 16 DATA?\n");
			return (((u16)pad.ReadData()) | ((u16)pad.ReadData() >> 8));
			break;
		case 0x1F801044:
			printf("PAD 16 STAT?\n");
			return pad.ReadStat();
			break;
		case 0x1F801048:
			printf("PAD 16 MODE?\n");
			return pad.ReadMode();
			break;
		case 0x1F80104A:
			printf("PAD 16 CTRL?\n");
			return pad.ReadCtrl();
			break;
		case I_MASK:
			return int_mask.full;
			break;
		case I_STAT:
			return int_reg.full;
			break;
		default:
		{
				   if (address >= 0x1f801c00 && address < 0x1f801e00) 
					   return SPU_readRegister(address);
				   else
				   {
					   printf("R16 : WTF 0x%08X ? \n", address);
					   return *(u16*)(io + (real & 0x1FFF));
					   break;
				   }
		}
		}
	}
	return 0;
}

u32 psx_mem::R32(u32 address)
{
	u32 real = address & 0x1FFFFFFF;
	if (real <= 0x200000) return *(u32*)(ram+(real&0x3FFFFF));
	if ((real >= 0x1F800000) && (real <= 0x1F800400)) return *(u32*)(scratch + (real & 0x3FF));
	if ((real >= 0x1FC00000) && (real <= 0x1FC80000)) return *(u32*)(rom + (real & 0x7FFFF));
	if ((real >= 0x1F801000) && (real <= 0x1F804000))
	{
		switch (real)
		{
		case DICR:
			return dma.dicr.full;
			break;
		case DPCR:
			return dma.dpcr.full;
			break;
		case DMA2_BCR:
			return dma.bcr[2].full;
			break;
		case DMA2_MADR:
			return dma.madr[2];
			break;
		case DMA2_CHCR:
			return dma.chcr[2].full;
			break;
		case DMA6_BCR:
			return dma.bcr[6].full;
			break;
		case DMA6_MADR:
			return dma.madr[6];
			break;
		case DMA6_CHCR:
			return dma.chcr[6].full;
			break;
		case I_MASK:
			return int_mask.full;
			break;
		case I_STAT:
			return int_reg.full;
			break;
		case GPU0:
			return GL.ReadGP0();
			break;
		case GPU1 :
			return GL.ReadGP1();
			break;
		case T1_VALUE:
			return 1;
			break;
		case JOY_STAT:
			printf("JOYSTAT 32\n");
			break;
		case JOY_CTRL:
			printf("JOYCTRL 32\n");
			break;
		case JOY_MODE:
			printf("JOYMODE 32\n");
			break;
		case JOY_DATA:
			printf("JOYDATA 32\n");
			break;
		default:
			printf("R32 : WTF 0x%08X ? \n", address);
			return *(u32*)(io + (real & 0x1FFF));
			break;
		}
	}
	return 0;
}

psx_mem::~psx_mem()
{
}
