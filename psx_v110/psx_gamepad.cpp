#include "psx_gamepad.h"
#include "interpreter.h"
#include "psx_counters.h"
#include "psx_gpu.h"
#include "psx_cpu.h"
#include "psx_mem.h"
#include <cstdio>
#include <conio.h>

extern psx_cpu R3000A;
extern psx_counters counters;
extern psx_mem mem;
extern psx_gpu GL;

psx_gamepad pad;

// Status Flags
#define TX_RDY		0x0001
#define RX_RDY		0x0002
#define TX_EMPTY	0x0004
#define PARITY_ERR	0x0008
#define RX_OVERRUN	0x0010
#define FRAMING_ERR	0x0020
#define SYNC_DETECT	0x0040
#define DSR			0x0080
#define CTS			0x0100
#define IRQ			0x0200

// Control Flags
#define TX_PERM		0x0001
#define DTR			0x0002
#define RX_PERM		0x0004
#define BREAK		0x0008
#define RESET_ERR	0x0010
#define RTS			0x0020
#define SIO_RESET	0x0040

//u8 ReadKeys()
//{ 
//	if(_kbhit())
//	{
//		char key=_getch();
//		if(key=='d') return 0x10;
//		if(key=='a') return 0x40;
//	}
//	return 0xFF;
//}

void ReadKeys(GLFWwindow *screen, int k, int s, int a, int m)
{
	pad.state[0].full=0xFFFF;

	if((k==GLFW_KEY_D)) pad.state[0].left=0;
	if((k==GLFW_KEY_A)) pad.state[0].right=0;
	if((k==GLFW_KEY_K)) pad.state[0].cross=0;
}

psx_gamepad::psx_gamepad()
{
	pad.state[0].full=0xFFFF;

	glfwSetKeyCallback(GL.screen,ReadKeys);

	StatReg = 0 | TX_RDY | TX_EMPTY;
}

u16 psx_gamepad::ReadStat()
{
	//printf("Stat?\n");
	return StatReg;
}

u16 psx_gamepad::ReadCtrl()
{
	return CtrlReg;
}

u8 psx_gamepad::ReadData()
{
	StatReg&=~RX_RDY;

	if(buffer.size()!=0)
	{
		u8 ret = buffer[0];
		buffer.erase(buffer.begin());
		//printf("Data ? %x\n",ret);
		return ret;
	}
	return 0;
}

u16 psx_gamepad::ReadMode()
{
	return Mode;
}

void psx_gamepad::WriteStat(u16 val)
{
}

void psx_gamepad::WriteData(u8 val)
{
	//printf("Data = %x\n",val);
	static int num=0; 

	switch(val)
	{
	case 0x1 :

		StatReg|=RX_RDY;

		buffer.push_back(0xFF); //        HIGH-Z

		buffer.push_back(0x41); //   |    Digital Controller ID 
		buffer.push_back(0x5A); //  \|/ 
		if(num==100)
		{
			buffer.push_back(0xFF);
			buffer.push_back(0xBF);
			pad.state[0].full=0xFFFF;
			num=0;
		}
		else 
		{
			buffer.push_back(0xFF);
			buffer.push_back(0xFF);
			num++;
		}


		counters.delay=500;

		break;
	default:

		StatReg|=RX_RDY;

		break;
	}
}

void psx_gamepad::WriteCtrl(u16 value)
{
	//printf("Ctrl = %x\n",value);
	CtrlReg = value & ~RESET_ERR;
	if (value & RESET_ERR) StatReg &= ~IRQ;
	if ((CtrlReg & SIO_RESET) || (!CtrlReg)) 
	{
		StatReg = TX_RDY | TX_EMPTY;
		counters.delay=500;
	}
}

void psx_gamepad::WriteMode(u16 val)
{
	//printf("Mode = %x\n",val);
	Mode = val;
}

psx_gamepad::~psx_gamepad()
{
}
