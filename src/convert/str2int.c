#include "common.h"
#include "str2int.h"
#include "symtab.h"

/**
 * 문자열 → 정수 변환 (sp = String Pointer)
 * - 마지막 글자가 'h'가 아니면 → 10진수로 보고 dec2int
 * - 마지막 글자가 'h'이면     → 16진수로 보고 hex2int
 */
int str2int(char* sp) {
    int l = strlen(sp);
    if (sp[l - 1] != 'h') {
        return (dec2int(sp));
    } else {
        return (hex2int(sp));
    }
}

/**
 * 10진수 문자열 → 정수 변환   예) "123" → 123
 * - 문자열 끝(\0)까지 한 글자씩 확인
 * - 숫자면 그 값을 d에 저장하고, n = n * 10 + d 로 한 자리씩 쌓기
 * - 숫자가 아닌 글자가 있으면 변환 실패(INV_VAL) 반환
 * (sp_dec는 첫 글자의 주소, 한 칸씩 옮기며 *sp_dec로 글자 하나씩 읽음 ≒ sp_dec[i])
 */
int dec2int(char* sp_dec) {
    int d = 0;

    int n;
    for(n = 0; *sp_dec; sp_dec++) {  // 지금 글자가 \0이면 멈춤
        if (isdigit(*sp_dec)) {
            d = *sp_dec - '0';       // 글자('0'~'9') → 숫자(0~9)
        } else {
            return INV_VAL;
        }
        n = n * 10 + d;              // "123" → 1 → 12 → 123
    }
    return n;
}

/**
 * 16진수 문자열 → 정수 변환   예) "1Ah" → 26
 * - 마지막 글자 'h' 바로 앞까지 한 글자씩 확인
 * - 0~9 또는 A~F면 그 값을 x에 저장하고, n = n * 16 + x 로 한 자리씩 쌓기
 * - 그 외 글자가 있으면 변환 실패(INV_VAL) 반환
 */
int hex2int(char* sp_hex) {
    int x = 0;

    int n;
    for (n = 0; *(sp_hex + 1); sp_hex++) {             // 다음 글자가 \0이면 멈춤 → 마지막 'h'는 계산에서 제외
        if (isdigit(*sp_hex)) {
            x = *sp_hex - '0';                         // '0'~'9' → 0~9
        } else if ('A' <= *sp_hex && *sp_hex <= 'F') {
            x = *sp_hex - 'A' + 10;                    // 'A'~'F' → 10~15
        } else {
            return INV_VAL;
        }
        n = (n << 4) + x;                              // n << 4 = n * 16, "1Ah" → 1 → 26 (16진수 정수 값 변환 계산 식)
    }
    return n;
}