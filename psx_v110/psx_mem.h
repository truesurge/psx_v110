#pragma once
#include "defines.h"
#include "structures.h"

class psx_mem
{
public:

	u8     rom[1024 * 512  + 1024]; // BIOS Rom image 512k
	u8     ram[1024 * 2048 + 1024]; // Ram 2048k
	u8 scratch[1024 * 1    + 1024]; // Scratchpad 1k
	u8      io[1024 * 16   + 1024]; // 16k IO area

	t_intreg int_reg, int_mask;

	u8   R8(u32 address);
	u16 R16(u32 address);
	u32 R32(u32 address);

	void W8 (u32 address, u8  val);
	void W16(u32 address, u16 val);
	void W32(u32 address, u32 val);

	psx_mem();
	~psx_mem();
};

