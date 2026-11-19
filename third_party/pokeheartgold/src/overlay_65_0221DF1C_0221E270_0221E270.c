#include "global.h"

#include "bg_window.h"

typedef struct UnkStruct_ov65_0221E270 {
    u8 unk_0[0x74];
    s32 unk_74;
    u8 unk_78[0x108];
    BgConfig *unk_180;
    void *unk_184;
    u8 unk_188[0x8];
    void *unk_190;
    u8 unk_194[0x420];
    u8 unk_5B4;
    u8 unk_5B5[0x1C6B];
    s32 *unk_2220;
} UnkStruct_ov65_0221E270;

u8 ov65_0221FB90(void *, s32, s32, void *, void *);

extern s32 ov65_0221E2C0;

s32 ov65_0221E270(UnkStruct_ov65_0221E270 *arg0);

s32 ov65_0221E270(UnkStruct_ov65_0221E270 *arg0) {
    FillBgTilemapRect(arg0->unk_180, 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0U);
    ov65_0221FB90((void *)&arg0->unk_5B4, arg0->unk_74, 1, arg0->unk_190, arg0->unk_184);
    arg0->unk_2220 = &ov65_0221E2C0;
    return 0;
}
