#include "global.h"

#include "party.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F2A18(struct UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);
void ov14_021F3190(struct UnkStruct_ov14_boxsys *param_1, u32 param_2, s32 param_3);
void ov14_021F2FDC(UnkStruct_ov14_boxapp *param_1);
void ov14_021F3044(UnkStruct_ov14_boxapp *param_1);
void ov14_021F30B0(UnkStruct_ov14_boxapp *param_1);
void ov14_021F311C(UnkStruct_ov14_boxapp *param_1);

struct UnkStruct_ov14_boxsys;

extern u8 ov14_021F80BC[];

void ov14_021F2FDC(UnkStruct_ov14_boxapp *param_1) {
    u32 uVar1;
    u32 uVar2;
    s16 *psVar3;
    u32 uVar4;

    uVar1 = Party_GetCount(param_1->party);
    psVar3 = (s16 *)ov14_021F80BC;
    uVar2 = 0;
    do {
        uVar4 = param_1->sys->unk_4094[0x1E + uVar2];
        ManagedSprite_SetPositionXY(param_1->sys->sprites[uVar4], *psVar3, psVar3[1]);
        ov14_021F3190(param_1->sys, uVar2 + 0x1e, 1);
        if (uVar2 < uVar1) {
            ov14_021F2A18(param_1->sys, uVar4, 1);
        } else {
            ov14_021F2A18(param_1->sys, uVar4, 0);
        }
        uVar2 = uVar2 + 1;
        psVar3 = psVar3 + 2;
    } while (uVar2 < 6);
    return;
}

void ov14_021F3044(UnkStruct_ov14_boxapp *param_1) {
    u32 uVar1;
    u32 uVar2;
    s16 *psVar3;
    u32 uVar4;

    uVar1 = Party_GetCount(param_1->party);
    psVar3 = (s16 *)ov14_021F80BC;
    uVar2 = 0;
    do {
        uVar4 = param_1->sys->unk_4094[0x1E + uVar2];
        ManagedSprite_SetPositionXY(param_1->sys->sprites[uVar4], *psVar3 + 0x98, psVar3[1]);
        ov14_021F3190(param_1->sys, uVar2 + 0x1e, 1);
        if (uVar2 < uVar1) {
            ov14_021F2A18(param_1->sys, uVar4, 1);
        } else {
            ov14_021F2A18(param_1->sys, uVar4, 0);
        }
        uVar2 = uVar2 + 1;
        psVar3 = psVar3 + 2;
    } while (uVar2 < 6);
    return;
}

void ov14_021F30B0(UnkStruct_ov14_boxapp *param_1) {
    u32 uVar1;
    u32 uVar2;
    s16 *psVar3;
    u32 uVar4;

    uVar1 = Party_GetCount(param_1->party);
    psVar3 = (s16 *)ov14_021F80BC;
    uVar2 = 0;
    do {
        uVar4 = param_1->sys->unk_4094[0x1E + uVar2];
        ManagedSprite_SetPositionXY(param_1->sys->sprites[uVar4], *psVar3, psVar3[1] + -0x90);
        ov14_021F3190(param_1->sys, uVar2 + 0x1e, 1);
        if (uVar2 < uVar1) {
            ov14_021F2A18(param_1->sys, uVar4, 1);
        } else {
            ov14_021F2A18(param_1->sys, uVar4, 0);
        }
        uVar2 = uVar2 + 1;
        psVar3 = psVar3 + 2;
    } while (uVar2 < 6);
    return;
}

void ov14_021F311C(UnkStruct_ov14_boxapp *param_1) {
    u32 uVar1;
    u32 uVar2;
    s16 *psVar3;
    u32 uVar4;

    uVar1 = Party_GetCount(param_1->party);
    psVar3 = (s16 *)ov14_021F80BC;
    uVar2 = 0;
    do {
        uVar4 = param_1->sys->unk_4094[0x1E + uVar2];
        ManagedSprite_SetPositionXY(param_1->sys->sprites[uVar4], *psVar3 + 0x98, psVar3[1] + -0x90);
        ov14_021F3190(param_1->sys, uVar2 + 0x1e, 1);
        if (uVar2 < uVar1) {
            ov14_021F2A18(param_1->sys, uVar4, 1);
        } else {
            ov14_021F2A18(param_1->sys, uVar4, 0);
        }
        uVar2 = uVar2 + 1;
        psVar3 = psVar3 + 2;
    } while (uVar2 < 6);
    return;
}
