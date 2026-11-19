#include "global.h"

#include "bg_window.h"

typedef struct UnkStruct_ov65_0221F258 {
    u8 unk_0[0x180];
    BgConfig *unk_180;
    u8 unk_184[0x4];
    void *unk_188;
    u8 unk_18C[0x4];
    void *unk_190;
    u8 unk_194[0x420];
    u8 unk_5B4;
    u8 unk_5B5[0x1C6B];
    s32 *unk_2220;
} UnkStruct_ov65_0221F258;

u8 ov65_0221FB90(void *, s32, s32, void *, void *);

extern s32 ov65_0221F2A8;

s32 ov65_0221F258(UnkStruct_ov65_0221F258 *arg0);

s32 ov65_0221F258(UnkStruct_ov65_0221F258 *arg0) {
    FillBgTilemapRect(arg0->unk_180, 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0U);
    ov65_0221FB90((void *)&arg0->unk_5B4, 0x24, 1, arg0->unk_190, arg0->unk_188);
    arg0->unk_2220 = &ov65_0221F2A8;
    return 0;
}
