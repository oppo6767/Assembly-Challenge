#include "len_mnmo.h"

// 기계 명령어 길이 - SVM 기계 명령어는 항상 4바이트
// (패스_1은 길이만 필요하므로 목적 코드는 만들지 않음)
int len_mnemonic(Optab *op) {
    return(4);
}