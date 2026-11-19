#include "global.h"

#include "gf_gfx_loader.h"
#include "overlay_96_ov96hurdle_work.h"

s32 ov96_02207B8C(Ov96HurdleRunner *arg0);

void ov96_02207BE4(Ov96HurdleWork *param_1);
s32 sub_0200606C(u32, s32);
void ov96_02207C38(s32 arg0, s32 arg1, u32 arg2, s32 arg3);

s32 ov96_02207B8C(Ov96HurdleRunner *arg0) {
    s32 v0;
    s32 v1;

    v1 = 0;
    if ((s32)arg0->pos.y > 0x198000) {
        v0 = arg0->pos.x;
        if ((v0 >= 0xD0000) && (v0 <= 0x130000)) {
            v1 = 1;
        }
    }
    if (v1 == 0) {
        return 0;
    }
    if (arg0->unk_AA == 0) {
        return 0;
    }
    if (arg0->unk_A6 == 0) {
        return 1;
    }
    return 0;
}

void ov96_02207BD4(void);

void ov96_02207BD4(void) {
    return;
}

s32 ov96_02207BD8(s32 param_1);

s32 ov96_02207BD8(s32 param_1) {
    return (0x100 - param_1) * 8 + 5;
}

void ov96_02207BE4(Ov96HurdleWork *param_1) {
    u8 remainder;
    u8 *iVar1;
    s32 iVar2;
    if ((3 + 1) <= (param_1->unk_512)) {
        iVar1 = (u8 *)param_1->unk_1C->pRawData;
        iVar2 = ((u32)(param_1->unk_511)) * 0x800;
        DC_FlushRange((void *)(iVar1 + iVar2), 0x800);
        GX_LoadBG0Char((void *)(iVar1 + iVar2), 0x1800, 0x800);
        param_1->unk_512 = 0;

        remainder = ((s32)((param_1->unk_511) + 1)) % ((s32)6);
        param_1->unk_511 = remainder;
        return;
    }
    param_1->unk_512 = (param_1->unk_512) + 1;
    return;
}

void ov96_02207C38(s32 arg0, s32 arg1, u32 arg2, s32 arg3) {
    s32 v0;

    if ((arg0 >= 0) && (arg0 < 0x100) && (arg1 >= 0) && (arg1 < 0xC0)) {
        if (arg3 != 0) {
            v0 = 3;
        } else {
            v0 = 4;
        }
        sub_0200606C((u16)arg2, v0);
    }
}
