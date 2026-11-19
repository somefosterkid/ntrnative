#include "global.h"
struct UnkStruct_ov96_021EF610;

#include "sprite_system.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"

extern s32 ov96_0221BA80;

void ov96_021EF95C(void *arg0, s32 arg1);

void ov96_021EF95C(void *arg0, s32 arg1) {
    s32 v2;
    void *v0;
    u32 *v1;
    v1 = (u32 *)arg0;
    v2 = 0;
    v0 = &ov96_0221BA80;
    v0 = v0 + (arg1 * 0x10);
    do {
        ManagedSprite_SetPositionXYWithSubscreenOffset(*(ManagedSprite **)((u8 *)v1 + offsetof(UnkStruct_ov96_021EF610, unk_38)), *((s16 *)v0), *((s16 *)(((u8 *)v0) + 2)), 0x200000);
        v0 += 4;
        v1 += 1;
        v2 += 1;
    } while (v2 < 4);
}

void ov96_021EFA04(void *param_1);

void ov96_021EF98C(struct UnkStruct_ov96_021EF610 *param_1);

void ov96_021EF98C(UnkStruct_ov96_021EF610 *param_1) {
    param_1->filler_21 = 0;
    ov96_021EFA04(param_1);
    return;
}
