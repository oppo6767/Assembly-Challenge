#include "common.h"
#include "obj.h"

struct objbuf Text_buf = {'T',}; // 텍스트 버퍼
struct objbuf* Txbp = &Text_buf; // 텍스트 버퍼 주소
struct objbuf Data_buf = {'D',}; // 데이터 버퍼
struct objbuf* Dtbp = &Data_buf; // 데이터 버퍼 주소
FILE* Ofp;

// 목적 프로그램 파일 생성 + 헤드(H) 레코드 출력 - 매개변수: 파일 이름, 프로그램 이름, 프로그램 시작 주소, 프로그램 크기
void put_obj_head(char* fname, char* pname, int paddr, int psize) {
    char* ext; // 확장자 저장 변수

    /**
     * 문자열 뒤쪽에서부터 .를 찾아서 해당 위치의 포인터를 반환
     * - 만약 참이면 obj에 ext+1(.다음 문자 위치)을 복사
     * - 찾으면 ext+1(. 다음 위치)에 "obj"를 복사 (s_1.s -> s_1.obj)
     */
    if ((ext = strrchr(fname, '.'))) strcpy(ext +1, "obj");
    else strcat(fname, ".obj");

    // 목적 프로그램 파일을 쓰기용으로 만들 수 없으면 오류 출력 및 종료
    if (!(Ofp = fopen(fname, "w"))) {
        fprintf(stderr, "Can't open object file '%s' ...\n", fname);
        exit(2);
    }

    // 헤드(H) 레코드 출력 - H:프로그램 이름:시작 주소:프로그램 크기
    fprintf(Ofp, "H:%s:%.7X:%.7X\n", pname, paddr, psize);
}

// 기계 명령어 목적 코드를 텍스트(T) 버퍼에 추가
void put_obj_text(int objaddr, unsigned char obj[]) {
    // 목적 코드 길이(obj[0])가 0이면 추가할 게 없으므로 종료
    if (!obj[0]) return;

    // 반대쪽 데이터(D) 버퍼에 모인 레코드를 먼저 출력하고 비운 뒤, 텍스트 버퍼에 추가
    flush_objbuf(Dtbp);
    put_obj_comm(Txbp, objaddr, obj);
}

void put_obj_data(int objaddr, unsigned char obj[]) {
    // 목적 코드 길이(obj[0])가 0이면 추가할 게 없으므로 종료
    if (!obj[0]) return;

    // 반대쪽 텍스트(T) 버퍼에 모인 레코드를 먼저 출력하고 비운 뒤, 데이터 버퍼에 추가
    flush_objbuf(Txbp);
    put_obj_comm(Dtbp, objaddr, obj);
}

// 목적코드 공통 내용 삽입 - 매개변수: 텍스트 버퍼/데이터 버퍼 주소, 목적코드 주소, 목적 코드 길이
// 텍스트/데이터 공통: 버퍼에 목적 코드 쌓기 - 매개변수: 쌓을 버퍼(Txbp/Dtbp), 목적 코드 주소, 목적 코드 배열(obj[0] 길이, obj[1~] 바이트)
void put_obj_comm(struct objbuf* obp, int objaddr, unsigned char obj[]) {
    int i;     // 반복 시작 값 변수
    char* bp;  // 버퍼 포인터

    // 텍스트 버퍼/데이터 버퍼 주소의 목적코드 길이가 존재하며
    // 버퍼 글자 수 + 이번에 쓸 글자 수(바이트 수 * 2 + '^' 1)가 버퍼 최대 글자 수(MAX_CNUM)보다 크거나
    // 이번 목적 코드 주소가 앞 코드에 바로 이어지지 않으면 (중간에 RD 등으로 끊김)
    // - 지금까지 모은 레코드를 파일에 출력하고 버퍼 비우기
    if (obp->olen && (obp->blen + obj[0]*2 + 1 > MAX_CNUM || obp->naddr != objaddr))
        flush_objbuf(obp);

    // 텍스트 버퍼/데이터 버퍼 주소의 목적코드 길이가 존재 안하면
    // - 시작주소와 다음 주소에 목적코드 주소 저장
    if (!obp->olen) {
        obp->saddr = objaddr;
        obp->naddr = objaddr;
    }

    /**
     * 버퍼 포인터를 버퍼에서 지금까지 쓴 끝 위치로 이동
     * obj[1]부터 한 바이트씩 16진수 2글자로 버퍼에 쓴다
     * - bp는 2글자씩 이동하고, i가 바이트 수(obj[0])보다 커지면 종료한다
    */
    bp = obp->buf + obp->blen;
    for (i = 1; i <= obj[0]; i++, bp += 2)
        sprintf(bp, "%.2X", obj[i]);

    // 바이트 묶음 뒤에 구분자 ^ 붙이기
    strcpy(bp, "^");
    obp->olen  += obj[0];          // 레코드 바이트 수 += 이번에 추가한 바이트 수
    obp->blen  += obj[0] * 2 + 1;  // 버퍼 글자 수 += 바이트 수 * 2 + '^' 1
    obp->naddr += obj[0];          // 다음에 이어질 주소 = 지금 주소 + 추가한 바이트 수
}

// ? 목적버퍼 - 매개변수: 데이터/텍스트 버퍼
// 목적 코드 버퍼 비우기: 모인 레코드를 파일에 출력하고 버퍼 초기화 - 매개변수: 데이터/텍스트 버퍼
void flush_objbuf(struct objbuf* obp) {
    /**
     * 데이터/텍스트 버퍼의 목적코드 길이가 있으면
     * - 맨 끝에 붙은 '^'를 \0으로 바꿔서 지우기
     * - 열린 파일에 타입, 시작주소, 목적 코드 길이, 퍼버를 작성
     * - 레코드 출력 - 타입(T/D):시작 주소:바이트 수:목적 코드
     */
    if (obp->olen) {
        obp->buf[obp->blen -1] = '\0';
        fprintf(Ofp, "%c:%.7X:%.2X:%s\n", obp->type, obp->saddr, obp->olen, obp->buf);
        obp->olen = 0;
        obp->blen = 0;
    }
}

// 목적 프로그램 마무리: 남은 버퍼 출력 + 엔드(E) 레코드 출력 + 파일 닫기 - 매개변수: 실행 시작 주소
void put_obj_end(int goaddr) {
    flush_objbuf(Txbp);               // 텍스트 버퍼에 남은 레코드 출력
    flush_objbuf(Dtbp);               // 데이터 버퍼에 남은 레코드 출력
    fprintf(Ofp, "E:%.7X\n", goaddr); // 엔드(E) 레코드 출력 - 실행 시작 주소(END 피연산자)
    fclose(Ofp);                      // 열린 파일 닫기
    Ofp = NULL;                       // 열린 파일 포인터에 NULL로 초기화
}