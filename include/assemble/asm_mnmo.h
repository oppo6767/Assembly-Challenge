#pragma once
#include "optab.h"

#define MAX_DNUM 8

extern unsigned char OBJC[MAX_DNUM*4 + 1];
extern int asm_mnemonic(Optab* op);