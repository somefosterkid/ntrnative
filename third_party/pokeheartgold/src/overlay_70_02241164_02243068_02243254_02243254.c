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

extern int ov70_02243F54(void *work, int a);

extern void ov70_02243EB8(void *a, void *b, void *c, int d);

extern void ov70_02238F9C(void *sprite, int x, int y);

int ov70_02243254(UnkStruct_ov70_02242E58 *work);

int ov70_02243254(UnkStruct_ov70_02242E58 *work) {
    int i;
    int off;
    int n;
    int idx;

    sub_02019688(((void *)work->unk_1C), 0, 0x64, 0x20, 1);
    sub_02019B08(((void *)work->unk_1C), 0);
    ov70_0224190C(work, 5);
    MI_CpuFill8(work->unk_64, 1, 0x1A);
    ov70_02243E8C(((void *)work->unk_1C), ((void *)work->unk_24), work->unk_4, work->unk_3C + 0x6E, 2, 0xF0E02);
    i = 1;
    if (*(ov70_02245F58 + work->unk_3C * 2 + 1) >= 1) {
        off = 0x10;
        do {
            int r = ov70_02243F54(work, ov70_02245E84[work->unk_3C] + i - 1);
            int color;
            if (r > 0) {
                color = 0xF0E02;
                work->unk_64[ov70_02245E84[work->unk_3C] + i - 1] = 1;
            } else {
                color = 0x80902;
                work->unk_64[ov70_02245E84[work->unk_3C] + i - 1] = 0;
            }
            ov70_02243E8C(((void *)work->unk_1C), ((void *)work->unk_24), &work->unk_4[i], ov70_02245FA0[i + ov70_02245F58[work->unk_3C * 2] - 1], 5, color);
            i++;
            off += 0x10;
        } while (i <= ov70_02245F58[work->unk_3C * 2 + 1]);
    }
    ov70_02243EB8(((void *)work->unk_1C), ((u8 *)work->unk_24), &work->unk_4[14], 0x44);
    sub_020196E8(((void *)work->unk_1C), 0, 0x10, 0);
    Sprite_SetAnimCtrlSeq(work->unk_C, 0x2F);
    idx = work->unk_3E;
    if (idx < 0) {
        idx = 0;
        work->unk_48 = idx;
    } else {
        work->unk_48 = idx;
    }
    n = work->unk_48 * 2;
    ov70_02238F9C(((void *)work->unk_C), (ov70_02245E26[n] + 0x10) << 3, ov70_02245E27[n] << 3);
    Sprite_SetDrawFlag(work->unk_C, 1);
    work->unk_4C = 9;
    return -1;
}
