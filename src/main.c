#include "common.h"
#include "assembler.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "asm_s10 [*.s] ...\n");
        exit(1);
    }
    
    OnePassAssemble(argv[1]);
    
    return 0;
}