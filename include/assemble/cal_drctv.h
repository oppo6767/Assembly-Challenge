#pragma once

// MAX_DNUM = MAXimum Data NUMber (데이터 최대 개수)
#define MAX_DNUM 8 // DD/DW/DB 한 줄에 쓸 수 있는 값 최대 개수

extern int cal_dc_oprnd(char* oprnd, int unit, unsigned char obj[]);
extern int cal_dc_oprnd_string(char* oprnd, unsigned char obj[]);