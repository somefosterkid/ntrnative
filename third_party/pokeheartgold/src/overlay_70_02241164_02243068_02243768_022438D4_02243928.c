#include "global.h"

#include "sprite.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

struct Sprite;

extern int ov70_02245F58[];

extern int ov70_02245E84[];

extern int ov70_02245FA0[];

extern u8 ov70_02245E26[];

extern u8 ov70_02245E27[];

extern void sub_02019688(void *a0, int a1, int a2, int a3, int a4);

extern void ov70_0224190C(void *work, int a);

extern void ov70_02243E8C(void *a, void *b, void *c, int d, int e, int f);

extern int ov70_02243FD4(void *work, int a);

extern void ov70_02243EB8(void *a, void *b, void *c, int d);

extern void ov70_02238F9C(void *sprite, int x, int y);

int ov70_02243928(UnkStruct_ov70_02242E58 *work);

int ov70_02243928(UnkStruct_ov70_02242E58 *work) {
    int i;
    int n;
    int idx;
    Sprite **new_var;
    sub_02019688(((void *)work->unk_1C), 0, 0x64, 0x20, 1);
    sub_02019B08(((void *)work->unk_1C), 0);
    ov70_0224190C(work, 5);
    MI_CpuFill8(work->unk_64, 1, 0x1A);
    ov70_02243E8C(((void *)work->unk_1C), ((void *)work->unk_24), work->unk_4, (work->unk_3C) + 0x6E, 2, 0xF0E02);
    i = 1;
    do {
        int r = ov70_02243FD4(work, (ov70_02245E84[work->unk_3C] + i) - 1);
        int color;
        if (r > 0) {
            color = 0xF0E02;
            work->unk_64[(i + ov70_02245E84[work->unk_3C]) - 1] = 1;
        } else {
            color = 0x80902;
            work->unk_64[(i + ov70_02245E84[work->unk_3C]) - 1] = 0;
        }
        ov70_02243E8C(((void *)work->unk_1C), ((void *)work->unk_24), work->unk_4 + i, ov70_02245FA0[(i + ov70_02245F58[(work->unk_3C) * 2]) - 1], 5, color);
        i++;
    } while (i < 4);
    ov70_02243EB8(((void *)work->unk_1C), work->unk_24, &work->unk_4[14], 0x44);
    sub_020196E8(((void *)work->unk_1C), 0, 0x10, 0);
    Sprite_SetAnimCtrlSeq(work->unk_C, 0x2F);
    idx = work->unk_3E;
    if (idx < 0) {
        work->unk_48 = 0;
    } else {
        work->unk_48 = idx;
    }
    n = (work->unk_48) * 2;
    ov70_02238F9C(((void *)work->unk_C), (ov70_02245E26[n] + 0x10) << 3, ov70_02245E27[n] << 3);
    Sprite_SetDrawFlag(work->unk_C, 1);
    new_var = &work->unk_18;
    if ((*new_var) != 0L) {
        Sprite_SetDrawFlag(*new_var, 1);
    }
    work->unk_4C = 0x15;
    return -1;
}

s32 ov70_02242390(UnkStruct_ov70_02242E58 *, u8 *);

u32 ov70_02243A6C(UnkStruct_ov70_02242E58 *param_1);

u32 ov70_02243A6C(UnkStruct_ov70_02242E58 *param_1) {
    u32 iVar1;

    iVar1 = ov70_02242390(param_1, param_1->unk_64);
    switch (iVar1) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        Sprite_SetDrawFlag(param_1->unk_C, 0);
        param_1->unk_4C = 0x16;
        param_1->unk_3E = (s16)iVar1;
        break;
    case -2:
        Sprite_SetDrawFlag(param_1->unk_C, 0);
        param_1->unk_4C = 0x16;
        param_1->unk_3E = -1;
        break;
    }
    return 0xffffffff;
}
