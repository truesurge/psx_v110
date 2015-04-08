#pragma once
#include "defines.h"
#include "structures.h"
#include <cstdio>
#include <cstdlib>
#include <conio.h>

//union sio_stat
//{
//	u16 full;
//	struct
//	{
//		unsigned TX_ready_1:1;
//		unsigned RX_not_emp:1;
//		unsigned TX_ready_2:1;
//		unsigned RX_par_err:1;
//		unsigned RX_overrun:1;
//		unsigned RX_bad    :1;
//		unsigned RX_inp    :1;
//		unsigned DSR       :1;
//		unsigned CTS       :1;
//		unsigned IRQ       :1;
//		unsigned           :6;
//	};
//};
//
//union sio_ctrl
//{
//	u16 full;
//	struct
//	{
//		unsigned TXEN  :1;
//		unsigned DTR   :1;
//		unsigned RXEN  :1;
//		unsigned tx_out:1;
//		unsigned ack   :1;
//		unsigned RTS   :1;
//		unsigned reset :1;
//		unsigned u     :1;
//		unsigned rx_md :2;
//		unsigned tx_int:1;
//		unsigned rx_int:1;
//		unsigned DSR   :1;
//		unsigned nu    :3;
//	};
//};

class psx_gamepad
{
public:
	u16 Data;
	u16 Baud;
	u16 StatReg; //sio_stat Stat;
	u16 CtrlReg; //sio_ctrl Ctrl;
	u16 Mode;

    u16 ReadStat();
	u16 ReadCtrl();
	u16 ReadMode();
	u8  ReadData();
	
	void WriteStat(u16 val);
	void WriteCtrl(u16 val);
	void WriteMode(u16 val);
	void WriteData(u8 val);

	psx_gamepad();
	~psx_gamepad();
};

