#include "global.h"

#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_0205A44C.h"

void *sub_02035878(void);

s32 sub_02059F54();

s32 sub_0205A034(void *, s32 *, s32);

s32 sub_0205ABBC(void *);

extern void *_021D41CC;

extern s32 sub_02059F30;

extern s32 sub_02059FF8;

void sub_02059F78(void *arg0);

void sub_02059F78(void *arg0) {
    s32 v0;
    void *v1;

    v0 = *(s32 *)((u8 *)arg0 + 0x14);
    if (v0 > 0) {
        *(s32 *)((u8 *)arg0 + 0x14) = (s32)(v0 - 1);
        return;
    }
    _021D41CC += 1;
    v1 = sub_02035878();
    if ((sub_020376F8() != 0) && (sub_02059F54() == 1) && (*(u8 *)((u8 *)v1 + 0x1C) != 4)) {
        sub_02034434();
        sub_020398D4(1, 1);
        sub_0205A904(0xB);
        sub_0205A034(arg0, &sub_02059FF8, 0);
    }
    if (sub_02037FCC() == 0) {
        sub_02037FF0();
        sub_0205ABBC(arg0);
        sub_0205A904(0);
        sub_0205A034(arg0, &sub_02059F30, 2);
    }
}
