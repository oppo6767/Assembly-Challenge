#pragma once

#define OP_XBIT 0x800000 // X 레지스터 사용할 경우 해당 X비트 = 1

extern int cal_nm_dmpr(char *oprnd);
extern int cal_nm_oprnd(char *oprnd);