#include "global.h"

#include "sprite_system.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"

s32 ov96_021EFA04(void *);

extern s16 ov96_0221BA80;

extern ManagedSpriteTemplate ov96_0221DA80;

void ov96_021EF99C(void *arg0);

void ov96_021EF99C(void *arg0) {
    s32 sp4;
    fx32 yOffset;
    SpriteSystem *sys;
    SpriteManager *mgr;
    void *sp0;
    s16 *var_r4;
    ManagedSprite *temp_r0_2;
    ManagedSpriteTemplate *var_r5;
    ManagedSpriteTemplate *var_r7;
    s32 temp_r0;
    u32 *var_r6;
    sp0 = arg0;
    var_r5 = &ov96_0221DA80;
    var_r4 = &ov96_0221BA80;
    sp4 = 0;
    var_r6 = (u32 *)sp0;
    var_r7 = &ov96_0221DA80;
    do {
        sys = *((SpriteSystem **)&((UnkStruct_ov96_021EF610 *)sp0)->unk_10);
        mgr = *((SpriteManager **)&((UnkStruct_ov96_021EF610 *)sp0)->unk_14);
        var_r5->x = var_r4[0];
        var_r5->y = var_r4[1];
        yOffset = (fx32)((512 > 0) ? ((512 * (1 << 12)) + 0.5f) : ((512 * (1 << 12)) - 0.5f));
        temp_r0_2 = SpriteSystem_NewSpriteWithYOffset(sys, mgr, var_r7, yOffset);
        *(ManagedSprite **)((u8 *)var_r6 + offsetof(UnkStruct_ov96_021EF610, unk_38)) = temp_r0_2;
        ManagedSprite_SetAnimateFlag(temp_r0_2, 1);
        var_r4 += 2;
        var_r5 = (void *)(((u8 *)var_r5) + 0x34);
        var_r7 = (void *)(((u8 *)var_r7) + 0x34);
        var_r6 += 1;
        temp_r0 = sp4 + 1;
        sp4 = temp_r0;
    } while (temp_r0 < 4);
    ManagedSprite_SetAnimateFlag(*((ManagedSprite **)&((UnkStruct_ov96_021EF610 *)sp0)->unk_40), 0);
    ov96_021EFA04(sp0);
}
