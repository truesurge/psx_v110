#pragma once
#include "defines.h"
#include "structures.h"
#include <cstdio>
#include <conio.h>

class psx_dma
{
public:
	t_chcr chcr[7];
	u32 madr[7];
	t_bcr  bcr[7];

	t_dpcr dpcr;
	t_dicr dicr;

	void Channel0();
	void Channel1();
	void Channel2();
	void Channel3();
	void Channel4();
	void Channel5();
	void Channel6();

	psx_dma();
	~psx_dma();
};

