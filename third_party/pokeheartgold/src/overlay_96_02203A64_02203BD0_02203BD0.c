#include "global.h"

#include "bg_window.h"

typedef struct UnkStruct_ov96_02203BD0 {
    u8 unk_0[0x8];
    BgConfig *unk_8;
    u8 unk_C[0x14];
    u8 *unk_20;
} UnkStruct_ov96_02203BD0;

void ov96_02203BD0(UnkStruct_ov96_02203BD0 *arg0, s32 arg1);

void ov96_02203BD0(UnkStruct_ov96_02203BD0 *arg0, s32 arg1) {
    u8 sp1C[8];
    u8 *new_var;
    u8(*p)[8] = &sp1C;
    u16 t;
    u8 i;
    t = (u16)(arg1 / 100);
    (*p)[3] = (t % 5) * 4;
    new_var = (*p) + 3;
    (*p)[0] = (t / 5) * 8;
    t = (u16)(((u16)(arg1 - (100 * t))) / 10U);
    (*p)[4] = (t % 5) * 4;
    (*p)[1] = (t / 5) * 8;
    t = (u16)(arg1 % 10);
    (*p)[5] = (t % 5) * 4;
    (*p)[2] = (t / 5) * 8;
    i = 0;
    do {
        CopyRectToBgTilemapRect(arg0->unk_8, 5U, (u8)((i * 4) + 0x13), 0x10U, 4U, 8U, (const void *)((arg0->unk_20) + 0xC), *(new_var + i), sp1C[i], 0x14U, 0x14U);
        i += 1;
    } while (i < 3U);
    ScheduleBgTilemapBufferTransfer(arg0->unk_8, 5U);
}
