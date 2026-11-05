#include "common.h"
#include "obj.h"

struct objbuf Text_buf = {'T',}, *Txbp = &Text_buf;
struct objbuf Data_buf = {'D',}, *Dtbp = &Data_buf;
FILE* Ofp;

void put_obj_head(char* fname, char* pname, int paddr, int psize) {
    char* ext;

    if ((ext = strrchr(fname, '.'))) strcpy(ext +1, "obj");
    else strcat(fname, ".obj");

    if (!(Ofp = fopen(fname, "w"))) {
        fprintf(stderr, "Can't open object file '%s' ...\n", fname);
        exit(2);
    }

    fprintf(Ofp, "H:%s:%.7X:%.7X\n", pname, paddr, psize);
}

void put_obj_text(int objaddr, unsigned char obj[]) {
    if (!obj[0]) return;
    flush_objbuf(Dtbp);
    put_obj_comm(Txbp, objaddr, obj);
}

void put_obj_data(int objaddr, unsigned char obj[]) {
    if (!obj[0]) return;
    flush_objbuf(Txbp);
    put_obj_comm(Dtbp, objaddr, obj);
}

void put_obj_comm(struct objbuf* obp, int objaddr, unsigned char obj[]) {
    int i;
    char* bp;

    if (obp->olen && (obp->blen + obj[0]*2 + 1 > MAX_CNUM || obp->naddr != objaddr))
        flush_objbuf(obp);

    if (!obp->olen)
        obp->saddr = obp->naddr = objaddr;

    bp = obp->buf + obp->blen;
    for (i = 1; i <= obj[0]; i++, bp += 2)
        sprintf(bp, "%.2X", obj[i]);

    strcpy(bp, "^");
    obp->olen  += obj[0];
    obp->blen  += obj[0] * 2 + 1;
    obp->naddr += obj[0];
}

void flush_objbuf(struct objbuf* obp) {
    if (obp->olen) {
        obp->buf[obp->blen -1] = '\0';
        fprintf(Ofp, "%c:%.7X:%.2X:%s\n", obp->type, obp->saddr, obp->olen, obp->buf);
        obp->olen = obp->blen = 0;
    }
}

void put_obj_end(int goaddr) {
    flush_objbuf(Txbp);
    flush_objbuf(Dtbp);
    fprintf(Ofp, "E:%.7X\n", goaddr);
    fclose(Ofp);
    Ofp = NULL;
}