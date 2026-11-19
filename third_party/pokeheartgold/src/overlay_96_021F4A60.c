#include "global.h"

#include "bg_window.h"

typedef struct UnkStruct_ov96_021F4A9C {
    u8 unk_0[0x8];
    BgConfig *unk_8;
    u8 unk_C[0x5C];
    u8 unk_68;
    u8 unk_69[0xE7];
    u8 unk_150;
    u8 unk_151;
} UnkStruct_ov96_021F4A9C;

void ov96_021F4724(void *arg0);

void ov96_021F48FC(u32 *, BgConfig *);

void ov96_021F4990(u32 *param_1, BgConfig *param_2);

extern u32 ov96_0221DC04;

void ov96_021F4A60(u32 *param_1, BgConfig *param_2);

s32 ov96_021F4EF8(void *, BgConfig *);
void ov96_021F4A9C(UnkStruct_ov96_021F4A9C *arg0);

void ov96_021F4A60(u32 *param_1, BgConfig *param_2) {
    u16 remainder;

    ov96_021F4724(param_1);
    if (*(s32 *)(&ov96_0221DC04 + (u32) * (u16 *)(param_1 + 6)) == 3) {
        ov96_021F48FC(param_1, param_2);
    } else {
        ov96_021F4990(param_1, param_2);
    }
    remainder = (s32)(*(u16 *)(param_1 + 6) + 1) % (s32)(3);
    *(u16 *)(param_1 + 6) = remainder;
    return;
}

void ov96_021F4A9C(UnkStruct_ov96_021F4A9C *arg0) {
    if (arg0->unk_151 != arg0->unk_150) {
        ov96_021F4EF8((void *)&arg0->unk_68, arg0->unk_8);
        BgTilemapRectChangePalette(arg0->unk_8, 6U, 3U, 7U, 0xCU, 8U, 3U);
        ScheduleBgTilemapBufferTransfer(arg0->unk_8, 6U);
    }
}
