#include "common.h"
#include "assembler.h"

int main(int argc, char* argv[]) {
    // 인자가 2개가 아니면 오류 발생 및 종료
    if (argc != 2) {
        fprintf(stderr, "asm_s10 [*.s] ...\n");
        exit(1);
    }
    
    // 어셈블리 코드 번역 (목적 코드 생성)
    OnePassAssemble(argv[1]);
    
    return 0;
}