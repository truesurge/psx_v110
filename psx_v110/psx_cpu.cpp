#include "psx_cpu.h"
#include "psx_gpu.h"
#include "psx_mem.h"
#include "interpreter.h" 
#include "psx_counters.h"

#include <cstdlib>
#include <cstdio>
#include <conio.h>

extern psx_mem mem;
extern psx_counters counters;

psx_cpu::psx_cpu()
{
	pc = 0xBFC00000;
	slot = 0;

	sr.full = 0;
	epc.full = 0;
	cause.full = 0;

	proceeding = 0;
}

void psx_cpu::Fetch()
{
	opcode.full = mem.R32(pc);
}

void psx_cpu::Execute()
{
	if (slot == 0)
	{
		main[opcode.op]();
		pc += 4;
	}
	else
	{
		main[opcode.op]();
		pc = slot;
		slot = 0;
		JumpRoutine();
	}
	counters.Update(2);
}
