#pragma once

void(*main[])();
void(*spec[])();
void(*bcnd[])();
void(*cop0[])();

void TestJump();
void Interrupt(int code);

void psxSPECIAL();
void psxBCOND();
void psxCOP0();
void psxNOP();

/* 1 */

void psxLB();
void psxLBU();
void psxLH();
void psxLHU();
void psxLW();
void psxLWR();
void psxLWL();
void psxSB();
void psxSH();
void psxSW();
void psxSWR();
void psxSWL();

/* 2 */

void psxADDI();
void psxADDIU();
void psxSLTI();
void psxSLTIU();
void psxANDI();
void psxORI();
void psxXORI();
void psxLUI();

/* 3 */

void psxADD();
void psxADDU();
void psxSUB();
void psxSUBU();
void psxSLT();
void psxSLTU();
void psxAND();
void psxOR();
void psxXOR();
void psxNOR();

/* 4 */

void psxSLL();
void psxSRL();
void psxSRA();
void psxSLLV();
void psxSRLV();
void psxSRAV();

/* 5 */

void psxMULT();
void psxMULTU();
void psxDIV();
void psxDIVU();
void psxMFHI();
void psxMFLO();
void psxMTHI();
void psxMTLO();

/* 6 */

void psxJ();
void psxJAL();
void psxJR();
void psxJALR();
void psxBEQ();
void psxBNE();
void psxBLEZ();
void psxBGTZ();
void psxBLTZ();
void psxBGEZ();
void psxBLTZAL();
void psxBGEZAL();
void psxSYSCALL();

/* 7 */

void psxLWC0();
void psxSWC0();
void psxMTC0();
void psxMFC0();
void psxCTC0();
void psxCFC0();
void psxRFE0();

/* 8 */

void psxCOP2();
