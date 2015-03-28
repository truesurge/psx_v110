#pragma once
#include "defines.h"
#include "structures.h"
#include <cstdio>
#include <cstdlib>
#include <queue>

class psx_cd
{
public:
	u8 cmd;
	
	t_cdindex index;
	t_cdintenable int_enable;
	t_cdintflag int_flag;

	u8 R0();
	u8 R1();
	u8 R2();
	u8 R3();

	void W0(u8 cmd);
	void W1(u8 cmd);
	void W2(u8 cmd);
	void W3(u8 cmd);

	psx_cd();

	std::queue<u8> params;
	std::queue<u8> response;

};

