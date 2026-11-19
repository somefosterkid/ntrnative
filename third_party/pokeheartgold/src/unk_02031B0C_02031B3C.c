#include "global.h"
#include "sys_task.h"
#include "unk_02031B0C.h"

void sub_020325CC(ApricornRecord *);

void sub_02031B3C(SaveApricornBox *arg0);

void InitApricornBox(SaveApricornBox *dest);
void Save_ApricornBox_Init(SaveApricornBox *dest);

void sub_02031B3C(SaveApricornBox *arg0) {
    s32 v0;

    v0 = 0;
    do {
        sub_020325CC(&arg0->juiceRecords[v0]);
        v0 += 1;
    } while (v0 < 3);
}

void Save_ApricornBox_Init(SaveApricornBox *dest) {
    InitApricornBox(dest);
}

void sub_02031B5C(ApricornJuice *dest) {
    MI_CpuFill8(dest, 0U, sizeof(ApricornJuice));
}
