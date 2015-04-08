#include "psx_gpu.h"
#include "psx_cpu.h"
#include "psx_counters.h"
#include <cstdio>

extern psx_counters counters;
extern psx_cpu R3000A;
extern psx_gpu GL;

u16 swap16(u16 val)
{
    return ((val & 0xff) << 8) | ((val & 0xff00) >> 8);
}
u32 swap32(u32 val)
{
    u32 result = 0;
    result |= (val & 0x000000FF) << 24;
    result |= (val & 0x0000FF00) << 8;
    result |= (val & 0x00FF0000) >> 8;
    result |= (val & 0xFF000000) >> 24;
    return result;
}

/* CONTROL */

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
	if (transfer.size()!=0)
	{
		u32 ret = ( swap16(transfer[0]>>16) | (transfer[1]) );
		transfer.erase(transfer.begin());
		return swap32(ret);
	}

	GP1.full |= 0x14000000;
	return 0;
}

void psx_gpu::WriteGP0(u32 val)
{
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
	case 0x64:
		TILE();
		break;
	case 0xA0:
		A0();
		break;
	case 0xC0:
		C0();
		break;
	case 0xE1:
		SET_TP();
		break;
	case 0xE2:
	case 0xE3:
	case 0xE4:
	case 0xE5:
	case 0xE6:
				cmd.erase(cmd.begin());
		break;
	default:
		printf("GPU : GP0 = %08X\n", val);
		_getch();
		cmd.erase(cmd.begin());
		break;
	}
}

void psx_gpu::WriteList(u32* ptr, u32 size)
{
	while (size--)
		WriteGP0(*ptr++);
}

psx_gpu::~psx_gpu()
{
}

psx_gpu::psx_gpu()
{
	GP1.full = 0x14802000;

	glfwInit();
	screen = glfwCreateWindow(480,360, "PSX VRAM OUT", NULL, NULL);
	//glfwHideWindow(screen);
	glfwSetWindowPos(screen, 100, 100);
	glfwMakeContextCurrent(screen);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 640, 480, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0, 0);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(screen);
}

void psx_gpu::ShowVram()
{
	for(int i=0;i<1024-640;i++)
		for(int j=0;j<512;j++)
		{
			glBegin(GL_POINTS);
			COLOR16(vram[i+640][j].full);
			glVertex2i(640+i,j);
			glEnd();
		}
}

/* DRAW */

void psx_gpu::RECT()
{
	REQUIRE(3);
	printf("GL : RECT\n");
	//_getch();
	int x, y;
	int w, h;
	x = CMD_X(1); y = CMD_Y(1);
	w = CMD_W(2); h = CMD_H(2);

	u8 r=CMD_R(0);
	u8 g=CMD_G(0);
	u8 b=CMD_B(0);

	for(int i=0;i<((w*3)/2);i+=2)
		for(int j=0;j<h;j++)
		{

			if(i%2==0) 
			{
				vram[x+i*2+0][y+j].l=r;
				vram[x+i*2+0][y+j].r=g;
				vram[x+i*2+1][y+j].l=b;
			}
			if(i%2==1)
			{
				vram[x+(i-1)*2+1][y+j].r=r;
				vram[x+i*2    +0][y+j].l=g;
				vram[x+i*2    +1][y+j].r=b;
			}

		}

	CLEAR();
}

void psx_gpu::POLY_F4()
{
	REQUIRE(5);
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
	REQUIRE(4);

	u16 x,y;
	u16 w,h;
	u16 tx,ty;
	u16 cx,cy;

	tclut clut;	clut.full = cmd[2]>>16;

	x=CMD_X(1);	y=CMD_Y(1);

	w=CMD_W(3);	h=CMD_H(3);

	cx=clut.x*16;
	cy=clut.y;

	tx=tex_page.xb*64;
	ty=tex_page.yb*256;

	u16 table[16];
	u16 image[256*4][256];

	for(int i=0;i<16;i++)
		table[i]=vram[cx+i][cy].full;

	for(int i=0;i<w/4;i++)
		for(int j=0;j<h;j++)
		{
			image[i*4+0][j]=table[vram[tx+i][ty+j].ll];
			image[i*4+1][j]=table[vram[tx+i][ty+j].ml];
			image[i*4+2][j]=table[vram[tx+i][ty+j].mr];
 			image[i*4+3][j]=table[vram[tx+i][ty+j].rr];
		}

	glBegin(GL_POINTS);
	for(int j=0;j<h;j++)
		for(int i=0;i<w;i++)
		{
			if(i==w-1) continue;
			if( (image[i][j]==0) && (image[i+1][j]==0) ) continue;
			COLOR16_TILE(image[i][j]);
			glVertex2i(x+i,y+j);
		}
	glEnd();

	CLEAR();
}

void psx_gpu::POLY_FT4()
{
	REQUIRE(9);
	
	tclut clut;
	tpage page;

	clut.full = cmd[2]>>16;
	page.full = cmd[4]>>16;

	u32 x=CMD_X(1);	
	u32 y=CMD_Y(1);

	u32 w=(CMD_X(3)-x);
	u32 h=(CMD_Y(7)-y);

	u32 tx=page.xb*64;
	u32 ty=page.yb*256;

	u32 cx=clut.x*16;
	u32 cy=clut.y;

	u16 table[16];
	u16 image[256*4][256];

	for(int i=0;i<16;i++)
		table[i]=vram[cx+i][cy].full;

	for(int i=0;i<w/4;i++)
		for(int j=0;j<h;j++)
		{
			image[i*4+0][j]=table[vram[tx+i][ty+j].ll];
			image[i*4+1][j]=table[vram[tx+i][ty+j].ml];
			image[i*4+2][j]=table[vram[tx+i][ty+j].mr];
 			image[i*4+3][j]=table[vram[tx+i][ty+j].rr];
		}

	glBegin(GL_POINTS);
	for(int i=0;i<w;i++)
		for(int j=0;j<h;j++)
		{
			if(image[i][j]==0) continue;
			COLOR16_REV(image[i][j]);
			glVertex2i(x+i,y+j);
		}
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

	u32 x = CMD_X(1);
	u32 y = CMD_Y(1);

	//printf("A0 x=%d y=%d\n",x,y);

	u16 *current = (u16*)&cmd[3];
	glBegin(GL_POINTS);
	for (int j = 0; j < h; j++)
		for (int i = 0; i < w; i++)
		{
			//if(*current==0) continue;
			//COLOR16(*current);
			//glVertex2i(i + x, j + y);
			vram[i + x][j + y].full = *current;
			current++;
		}
	glEnd();
 	CLEAR();
}

void psx_gpu::C0()
{
	REQUIRE(3);
	
	u16 x,y;
	u16 w,h;
	
	x=CMD_X(1);
	y=CMD_Y(1);
	w=CMD_W(2);
	h=CMD_H(2);
	
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			transfer.push_back(swap16(vram[x+i][y+j].full));

	CLEAR();
}

void psx_gpu::SET_TP()
{
	tex_page.full = cmd[0]&0xFFFF;
	CLEAR();
}
