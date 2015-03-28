#include "psx_gpu.h"
#include "psx_cpu.h"
#include "psx_counters.h"
#include <cstdio>

extern psx_counters counters;
extern psx_cpu R3000A;

void psx_gpu::RECT()
{
	REQUIRE(3);
	//printf("GL : RECT\n");
	int x, y;
	int w, h;
	x = CMD_X(1); y = CMD_Y(1);
	w = CMD_W(2); h = CMD_H(2);
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x+w, y);
	glVertex2i(x+w, y+h);
	glVertex2i(x, y+h);
	glEnd();
	CLEAR();
}

void psx_gpu::POLY_F4()
{
	REQUIRE(5);
	//printf("GL : POLY_F4\n");
	glBegin(GL_QUADS);
	glShadeModel(GL_FLAT);
	COLOR24(0);
	VERTEX(1);
	VERTEX(2);
	VERTEX(4);
	VERTEX(3);
	glEnd();
	CLEAR();
}

void psx_gpu::POLY_F3()
{
	REQUIRE(4);
	//printf("GL : POLY_F3\n");
	glBegin(GL_QUADS);
	glShadeModel(GL_FLAT);
	COLOR24(0);
	VERTEX(1);
	VERTEX(2);
	VERTEX(3);
	glEnd();
	CLEAR();
}

void psx_gpu::LINE()
{
	REQUIRE(3);
	//printf("GL : LINE\n");
	glBegin(GL_LINES);
	glShadeModel(GL_FLAT);
	COLOR24(0);
	VERTEX(1);
	VERTEX(2);
	glEnd();
	CLEAR();
}

void psx_gpu::POLY_G3()
{
	REQUIRE(6);
	//printf("GL : POLY_G3\n");
	glBegin(GL_TRIANGLES);
	glShadeModel(GL_SMOOTH);
	COLOR24(0);
	VERTEX(1);
	COLOR24(2);
	VERTEX(3);
	COLOR24(4);
	VERTEX(5);
	glEnd();
	CLEAR();
}

void psx_gpu::POLY_G4()
{
	REQUIRE(8);
	//printf("GL : POLY_G4\n");
	glBegin(GL_TRIANGLES);
	glShadeModel(GL_SMOOTH);
	COLOR24(0);
	VERTEX(1);
	COLOR24(2);
	VERTEX(3);
	COLOR24(4);
	VERTEX(5);
	COLOR24(2);
	VERTEX(3);
	COLOR24(4);
	VERTEX(5);
	COLOR24(6);
	VERTEX(7);
	glEnd();
	CLEAR();
}

void psx_gpu::TILE()
{
	printf("GL : TILE\n");
}

void psx_gpu::POLY_FT4()
{
	REQUIRE(9);
	//printf("GL : POLY_FT4\n");
	glBegin(GL_QUADS);
	glShadeModel(GL_FLAT);
	COLOR24(0);
	VERTEX(1);
	VERTEX(3);
	VERTEX(7);
	VERTEX(5);
	glEnd();
	CLEAR();
}

void psx_gpu::A0()
{
	REQUIRE(3);
	int w = CMD_W(2);
	int h = CMD_H(2);
	int size= w*h/2;
	REQUIRE(3 + size);
	//printf("GL : A0\n");
	u32 x = CMD_X(1);
	u32 y = CMD_Y(1);
	u16 *current = (u16*)&cmd[3];
	glBegin(GL_POINTS);
	for (int j = 0; j < h; j++)
	for (int i = 0; i < w; i++)
	{
		COLOR16(*current);
		glVertex2i(i + x, j + y);
		vram[i + x][j + y] = *current;
		current++;
	}
	glEnd();
	CLEAR();
}

void psx_gpu::C0()
{
	REQUIRE(3);
	//printf("GL : C0\n");
	CLEAR();
}

psx_gpu::psx_gpu()
{
	GP1.full = 0x14802000;

	glfwInit();
	screen = glfwCreateWindow(1024, 512, "OUT", NULL, NULL);
	//glfwHideWindow(screen);
	glfwSetWindowPos(screen, 100, 100);
	glfwMakeContextCurrent(screen);

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 1024, 512, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0, 0);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(screen);
}

u32 psx_gpu::ReadGP1()
{
	//printf("GPU : GP1?\n");
	static int iNumRead = 0;

	GP1.ready_for_cmd = 1;
	GP1.ready_for_vram = 1;
	GP1.ready_for_dma = 1;

	if ((iNumRead++) == 2)
	{
		iNumRead = 0;
		GP1.draw_even_lines = !GP1.draw_even_lines;
	}

	return (GP1.full);
}

void psx_gpu::WriteGP1(u32 val)
{
	
	switch (val >> 24)
	{
	case 0x0:
		GP1.full = 0x14802000;
		break;
	case 0x1:
		GP0 = 0;
		break;
	case 0x2:
		GP1.irq = 0;
		break;
	case 0x3:
		GP1.display_enable = val & 0x1;
		break;
	case 0x4:
		GP1.dma_directon = val & 0x3;
		break;
	default :
		//printf("GPU : GP1 = %08X\n", val);
		break;
	}
}

u32 psx_gpu::ReadGP0()
{
	GP1.full |= 0x14000000;
	return GP0;
}

void psx_gpu::WriteGP0(u32 val)
{
	//printf("GPU : GP0 = %08X\n", val);
	cmd.push_back(val);
	switch (cmd[0] >> 24)
	{
	case 0x0:
		GP1.full = 0x14802000;
		cmd.erase(cmd.begin());
		break;
	case 0x01:
		cmd.clear();
		break;
	case 0x2:
		RECT();
		break;
	case 0x20:
	case 0x22:
		POLY_F3();
		break;
	case 0x28:
	case 0x29:
	case 0x2A:
	case 0x2B:
		POLY_F4();
		break;
	case 0x2c:
	case 0x2d:
	case 0x2e:
	case 0x2f:
		POLY_FT4();
		break;
	case 0x30:
	case 0x31:
	case 0x32:
	case 0x33:
		POLY_G3();
		break;
	case 0x38:
	case 0x39:
	case 0x3A:
	case 0x3B:
		POLY_G4();
		break;
	case 0x40:
	case 0x42:
		LINE();
		break;
	case 0xA0:
		A0();
		break;
	case 0xC0:
		C0();
		break;
	case 0xE1:
		GP1.full &= 0xFFFFFC00;
		GP1.full |= (val & 0x3FF);
		cmd.erase(cmd.begin());
		break;
	case 0xE2:
	case 0xE3:
	case 0xE4:
	case 0xE5:
	case 0xE6:
	default:
		cmd.erase(cmd.begin());
		//_getch();
		break;
	}
}

void psx_gpu::WriteList(u32* ptr, u32 size)
{
	counters.Update(size*2);
	while (size--)
		WriteGP0(*ptr++);
}

psx_gpu::~psx_gpu()
{
}
