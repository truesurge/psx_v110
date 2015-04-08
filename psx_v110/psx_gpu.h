#pragma once
#include <cstdlib>
#include <conio.h>
#include <vector>
#include "structures.h"
#include "defines.h"
#define GLFW_INCLUDE_GLU
#include "include\glfw3.h"

union fpixel
{
	u16 full;
	struct
	{
		unsigned r : 5;
		unsigned g : 5;
		unsigned b : 5;
		unsigned a : 1;
	};
	struct
	{
		unsigned l : 8;
		unsigned r : 8;
	};
	struct
	{
		unsigned ll : 4;
		unsigned ml : 4;
		unsigned mr : 4;
		unsigned rr : 4;
	};
};

union tclut
{
	u16 full;
	struct
	{
		unsigned x : 6 ;
		unsigned y : 9 ;
		unsigned zr : 1;
	};
};

union tpage
{
	u16 full;
	struct
	{
		unsigned xb  : 4;  // 0-3   Texture page X Base   (N*64) (ie. in 64-halfword steps)    ;GPUSTAT.0-3
		unsigned yb  : 1;  // 4    Texture page Y Base   (N*256) (ie. 0 or 256)               ;GPUSTAT.4
		unsigned st  : 2;  // 5-6   Semi Transparency     (0=B/2+F/2, 1=B+F, 2=B-F, 3=B+F/4)   ;GPUSTAT.5-6
		unsigned col : 2;  // 7-8   Texture page colors   (0=4bit, 1=8bit, 2=15bit, 3=Reserved);GPUSTAT.7-8
		unsigned : 7 ;
	};
};

class psx_gpu
{
public:
	GLFWwindow *screen;
	fpixel vram[1024][512];

	std::vector<u32> cmd;
	std::vector<u16> transfer;

	tpage tex_page;

	t_gpustat GP1;
	u32       GP0;

	u32 ReadGP1();
	void WriteGP1(u32 cmd);

	u32 ReadGP0();
	void WriteGP0(u32 cmd);
	void WriteList(u32* prt, u32 size);
	
	void ShowVram();

	void POLY_F4();
	void POLY_F3();
	void POLY_FT4();
	void POLY_G3();
	void POLY_G4();
	void RECT();
	void TILE();
	void LINE();
	void SET_TP();
	void SET_CLUT();
	void A0();
	void C0();

	psx_gpu();
	~psx_gpu();
};

