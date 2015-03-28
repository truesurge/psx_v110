#include <cstdlib>
#include <cstdio>
#include "psx_cpu.h"
#include "psx_mem.h"
#include "psx_gpu.h"
#include "psx_dma.h"
#include "psx_cd.h"
#include "psx_counters.h"

psx_cpu R3000A;
psx_mem mem;
psx_gpu GL;
psx_dma dma;
psx_counters counters;
psx_cd cd;

int main()
{

	LOOP
	{
		R3000A.Fetch();
		R3000A.Execute();
	}
	return 0;
}

