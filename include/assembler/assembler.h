#pragma once

extern int LOC;         // LOC = Location Counter (현재 주소)
extern int LDaddr;      // LDaddr = LoaD address (시작 주소)
extern int GOaddr;      // GOaddr = GO address (실행 시작 주소)
extern int PGsize;      // PGsize = ProGram size (프로그램 크기)

extern char* null_LNO;  // 줄 번호가 없을 때 출력할 빈칸
extern char* null_LBL;  // 라벨이 없을 때 출력할 빈칸
extern char* null_OPR;  // 피연산자가 없을 때 출력할 빈칸

//extern void OnePassAssemble(char *sfile);
extern void TwoPassAssemble(char* sfile);
extern void Pass_1(char* sfile);
extern void Pass_2(char* sfile);