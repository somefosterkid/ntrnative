#include "global.h"

#include "bug_contest_internal.h"
#include "error_handling.h"
#include "field_types_def.h"
#include "pokemon_icon_idx.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0206D494.h"
#include "overlay_27_ov27task.h"

void ov27_0225A4B8(Ov27Task *arg0);

void ov27_0225AA7C(Ov27Task *arg0);

s32 ov27_0225A9C0(Ov27Task *, s32);

extern const u32 ov27_0225CF3C[];

extern s32 ov27_0225D038;

extern s32 ov27_0225D05C;

void ov27_0225B010(Ov27Task *arg0);

void ov27_0225B010(Ov27Task *arg0) {
    SpriteTemplate tmpl;
    s32 var_r7;
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    BugContest *temp_r0_8;
    s32 *var_r6_2;
    s32 *var_r6_3;
    f32 var_r0;
    f32 var_r0_2;
    f32 var_r0_3;
    f32 var_r0_4;
    f32 var_r0_5;
    u16 temp_r0_2;
    u16 temp_r0_3;
    u16 temp_r0_5;
    u16 temp_r0_6;
    u32 temp_r0_7;
    u32 temp_r1_2;
    SpriteResourcesHeader *var_r7_2;

    for (var_r7 = 0; var_r7 < 0xB; var_r7++) {
        CreateSpriteResourcesHeader((SpriteResourcesHeader *)((u8 *)arg0->unk_204) + var_r7, var_r7 + 0x64, var_r7 + 0x64, ov27_0225CF3C[var_r7], ov27_0225CF3C[var_r7], -1, -1, 0, 0, arg0->unk_144[0], arg0->unk_144[1], arg0->unk_144[2], arg0->unk_144[3], NULL, NULL);
    }
    tmpl.spriteList = arg0->unk_18;
    var_r7_2 = arg0->unk_204;
    sp2C = 0;
    tmpl.position.z = 0;
    tmpl.scale.x = 0x1000;
    tmpl.scale.y = 0x1000;
    tmpl.scale.z = 0x1000;
    tmpl.header = var_r7_2;
    tmpl.rotation = sp2C;
    tmpl.drawPriority = 1;
    tmpl.whichScreen = (NNS_G2D_VRAM_TYPE)2;
    var_r6_2 = &ov27_0225D038;
    tmpl.heapID = (enum HeapID)8;
    for (; sp2C < 9; sp2C++) {
        temp_r0_2 = *(u16 *)var_r6_2;
        tmpl.header = var_r7_2;
        if (temp_r0_2 != 0) {
            var_r0 = 0.5f + (f32)(temp_r0_2 << 0xC);
        } else {
            var_r0 = (f32)(temp_r0_2 << 0xC) - 0.5f;
        }
        tmpl.position.x = (s32)var_r0;
        temp_r0_3 = *(u16 *)((u8 *)var_r6_2 + 2);
        if (temp_r0_3 != 0) {
            var_r0_2 = 0.5f + (f32)(temp_r0_3 << 0xC);
        } else {
            var_r0_2 = (f32)(temp_r0_3 << 0xC) - 0.5f;
        }
        tmpl.position.y = (s32)var_r0_2 + 0x100000;
        arg0->unk_390[sp2C] = Sprite_CreateAffine(&tmpl);
        Sprite_SetAnimActiveFlag(arg0->unk_390[sp2C], 1);
        Sprite_SetPriority(arg0->unk_390[sp2C], 0U);
        Sprite_SetAffineOverwriteMode(arg0->unk_390[sp2C], 1U);
        var_r7_2++;
        var_r6_2 = (void *)((u8 *)var_r6_2 + (4));
    }
    sp30 = 9;
    var_r6_3 = &ov27_0225D05C;
    for (; sp30 < 0xF; sp30++) {
        tmpl.header = &arg0->unk_204[9];
        temp_r0_5 = *(u16 *)var_r6_3;
        if (temp_r0_5 != 0) {
            var_r0_3 = 0.5f + (f32)(temp_r0_5 << 0xC);
        } else {
            var_r0_3 = (f32)(temp_r0_5 << 0xC) - 0.5f;
        }
        tmpl.position.x = (s32)var_r0_3;
        temp_r0_6 = *(u16 *)((u8 *)var_r6_3 + 2);
        if (temp_r0_6 != 0) {
            var_r0_4 = 0.5f + (f32)(temp_r0_6 << 0xC);
        } else {
            var_r0_4 = (f32)(temp_r0_6 << 0xC) - 0.5f;
        }
        tmpl.position.y = (s32)var_r0_4 + 0x100000;
        arg0->unk_390[sp30] = Sprite_CreateAffine(&tmpl);
        Sprite_SetAnimActiveFlag(arg0->unk_390[sp30], 1);
        Sprite_SetPriority(arg0->unk_390[sp30], 2U);
        var_r6_3 = (void *)((u8 *)var_r6_3 + (4));
    }
    temp_r1_2 = arg0->unk_51C_1;
    if ((u32)(temp_r1_2 - 1) <= 2U) {
        if (temp_r1_2 == 1) {
            sp34 = 0x90;
        } else if ((u32)(temp_r1_2 - 2) <= 1U) {
            sp34 = 0x68;
        }
        sp2C = 100;
        tmpl.header = &arg0->unk_204[10];
        tmpl.position.x = (s32)(0.5f + (f32)(sp2C << 12));
        if (sp34 > 0) {
            var_r0_5 = 0.5f + (f32)(sp34 << 0xC);
        } else {
            var_r0_5 = (f32)(sp34 << 0xC) - 0.5f;
        }
        tmpl.position.y = (s32)var_r0_5 + 0x100000;
        arg0->unk_390[15] = Sprite_CreateAffine(&tmpl);
        Sprite_SetAnimActiveFlag(arg0->unk_390[15], 1);
        Sprite_SetPriority(arg0->unk_390[15], 0U);
        Sprite_SetAnimCtrlSeq(arg0->unk_390[15], 6);
    }
    temp_r0_7 = arg0->unk_51C_1;
    if (temp_r0_7 == 2) {
        temp_r0_8 = FieldSystem_BugContest_Get(arg0->fieldSystem);
        if (temp_r0_8 == NULL) {
            GF_AssertFail();
        }
        if (temp_r0_8->caught_poke) {
            Sprite_SetPositionXYWithSubscreenOffset(arg0->unk_390[6], 0x68, 0x88, FX32_CONST(256));
            Sprite_SetPalOffsetRespectVramOffset(arg0->unk_390[6], Pokemon_GetIconPalette(temp_r0_8->mon));
            Sprite_SetAnimCtrlSeq(arg0->unk_390[6], 6);
        }
    } else if (temp_r0_7 == 3) {
        Sprite_SetDrawFlag(arg0->unk_390[6], 0);
    }
    Sprite_SetAnimCtrlSeq(arg0->unk_390[9], 0);
    Sprite_SetAnimCtrlSeq(arg0->unk_390[10], 8);
    Sprite_SetAnimCtrlSeq(arg0->unk_390[11], 3);
    Sprite_SetAnimCtrlSeq(arg0->unk_390[12], 5);
    Sprite_SetAnimCtrlSeq(arg0->unk_390[14], 0xC);
    Sprite_SetDrawFlag(arg0->unk_390[14], 0);
    Sprite_SetPriority(arg0->unk_390[13], 1U);
    ov27_0225A4B8(arg0);
    ov27_0225A9C0(arg0, 1);
    ov27_0225AA7C(arg0);
}
