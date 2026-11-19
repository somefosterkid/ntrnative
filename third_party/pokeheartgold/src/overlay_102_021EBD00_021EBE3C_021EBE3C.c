#include "global.h"

#include "bg_window.h"

typedef struct UnkStruct_ov102_021EBE3C {
    u8 unk_0[0x40];
    u8 unk_40;
    u8 unk_41[0x3F];
    u32 unk_80;
} UnkStruct_ov102_021EBE3C;

void ov102_021EC37C(void *, u32, u32, u32, u32, u32);

void ov102_021EBE3C(UnkStruct_ov102_021EBE3C *param_1);

void ov102_021EBE3C(UnkStruct_ov102_021EBE3C *param_1) {
    G2_SetBlendAlpha(4, 0x3f, 0, 0x10);
    ToggleBgLayer(2, 1);
    param_1->unk_80 = 0;
    ov102_021EC37C((u8 *)&param_1->unk_40, 4, 0x3f, 0, 0x7c, 0xc);
    return;
}
