#pragma once
#include <cstdlib>
#include <conio.h>
#include <vector>
#include "structures.h"
#include "defines.h"
#define GLFW_INCLUDE_GLU
#include "include\glfw3.h"

class psx_gpu
{
public:
	GLFWwindow *screen;
	u16 vram[1024][512];
	std::vector<u32> cmd;

	t_gpustat GP1;
	u32       GP0;

	u32 ReadGP1();
	void WriteGP1(u32 cmd);

	u32 ReadGP0();
	void WriteGP0(u32 cmd);
	void WriteList(u32* prt, u32 size);

	void POLY_F4();
	void POLY_F3();
	void POLY_FT4();
	void POLY_G3();
	void POLY_G4();
	void RECT();
	void TILE();
	void LINE();
	void A0();
	void C0();

	psx_gpu();
	~psx_gpu();
};

