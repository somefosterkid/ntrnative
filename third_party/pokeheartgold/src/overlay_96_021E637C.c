#include "global.h"

#include "sound_02004A44.h"
#include "sprite.h"
#include "unk_02005D10.h"
#include "error_handling.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "pokeathlon/pokeathlon.h"
#include "overlay_96_unk_struct_ov96_021e9a78.h"

void ov96_021E65D8(void *arg0);

s32 ov96_021E661C(void *arg0);

s32 ov96_021E637C(void *arg0);

void ov96_021E6454(void *arg0, s32 arg1);
void ov96_021E64B8(void *arg0);
void ov96_021E81D8(SysTask *task, void *data);
Sprite *ov96_021EA7A4(UnkStruct_ov96_021E9A78 *, SpriteList *, s32, s32);
s32 ov96_021E64F8(void *arg0, void *arg1, UnkStruct_ov96_021E9A78 *arg2, void *arg3, s32 arg4);
void ov96_021E6550(void *param_1);
void ov96_021E658C(void *param_1, s32 param_2, s32 param_3);
void ov96_021E65A4(void *param_1);
void ov96_021E6670(void *arg0, s32 arg1);

s32 ov96_021E637C(void *arg0) {
    s32 v2;
    u8 v0;
    u8 v1;

    v1 = ((PokeathlonCourseData *)arg0)->unk_729;
    v2 = 0;
    switch (v1) {
    case 0:
        PlaySE(0x89AU);
        ov96_021E65D8(arg0);
        ((PokeathlonCourseData *)arg0)->unk_729 = (u8)(((PokeathlonCourseData *)arg0)->unk_729 + 1);
        break;
    case 1:
        if (ov96_021E661C(arg0) != 0) {
            ((PokeathlonCourseData *)arg0)->unk_729 = (u8)(((PokeathlonCourseData *)arg0)->unk_729 + 1);
            sub_020053A8(7U, 0U);
        }
        break;
    case 2:
        v0 = ((PokeathlonCourseData *)arg0)->hud.unk_4;
        if (v0 == 0) {
            PlaySE(0x89DU);
            Sprite_SetDrawFlag(((PokeathlonCourseData *)arg0)->hud.unk_0, 1);
            Sprite_SetAnimCtrlSeq(((PokeathlonCourseData *)arg0)->hud.unk_0, 0);
        } else if (v0 == 21) {
            PlaySE(0x89DU);
            Sprite_SetAnimCtrlSeq(((PokeathlonCourseData *)arg0)->hud.unk_0, 1);
        } else if (v0 == 42) {
            PlaySE(0x89DU);
            Sprite_SetAnimCtrlSeq(((PokeathlonCourseData *)arg0)->hud.unk_0, 2);
        }
        ((PokeathlonCourseData *)arg0)->hud.unk_4 = (u8)(((PokeathlonCourseData *)arg0)->hud.unk_4 + 1);
        if ((u32) ((PokeathlonCourseData *)arg0)->hud.unk_4 > 0x3FU) {
            PlaySE(0x892U);
            Sprite_SetDrawFlag(((PokeathlonCourseData *)arg0)->hud.unk_0, 0);
            ((PokeathlonCourseData *)arg0)->hud.unk_4 = 0U;
            v2 = 1;
        }
        break;
    }
    return v2;
}

void ov96_021E6454(void *arg0, s32 arg1) {
    s32 v0;

    v0 = (s32)(arg1 + 0x1D) / 30;
    if (v0 <= 0) {
        Sprite_SetDrawFlag(((PokeathlonCourseData *)arg0)->hud.unk_C, 0);
        return;
    }
    if (v0 <= 3) {
        if (*((u8 *)&((PokeathlonCourseData *)arg0)->filler_72B[0]) != v0) {
            PlaySE(0x897U);
            *((u8 *)&((PokeathlonCourseData *)arg0)->filler_72B[0]) = (u8)v0;
        }
        Sprite_SetDrawFlag(((PokeathlonCourseData *)arg0)->hud.unk_C, 1);
        Sprite_SetAnimCtrlSeq(((PokeathlonCourseData *)arg0)->hud.unk_C, v0 + 1);
        return;
    }
    Sprite_SetDrawFlag(((PokeathlonCourseData *)arg0)->hud.unk_C, 0);
}

void ov96_021E64B8(void *arg0) {
    u8 v1;
    void *v0;

    v1 = 0;
    ((PokeathlonCourseData *)arg0)->unk_D30 = 0;
    do {
        v0 = arg0 + (v1 * 0x10);
        if (*(s32 *)((u8 *)v0 + 0xD34) != 0) {
            GF_AssertFail();
        }
        *(s32 *)((u8 *)v0 + 0xD38) = 0;
        *(s32 *)((u8 *)v0 + 0xD3C) = 0;
        *(s32 *)((u8 *)v0 + 0xD40) = 0;
        v1 += 1;
    } while ((u32)v1 < 3U);
}

s32 ov96_021E64F8(void *arg0, void *arg1, UnkStruct_ov96_021E9A78 *arg2, void *arg3, s32 arg4) {
    u32 v0;
    u32 v1;
    void *v2;
    void *v3;

    v0 = *((u32 *)&((PokeathlonCourseData *)arg0)->unk_D30);
    if (v0 >= 3U) {
        GF_AssertFail();
        return 0;
    }
    v3 = arg0 + 0xD34;
    v1 = (u32)(v0 << 0x18) >> 0x14;
    *(s32 *)(v3 + v1) = 1;
    v2 = v3 + v1;
    *(void **)((u8 *)v2 + 4) = arg1;
    *(void **)((u8 *)v2 + 8) = ov96_021EA7A4(arg2, arg3, arg4, *((s32 *)&((PokeathlonCourseData *)arg0)->heapId));
    *(SysTask **)((u8 *)v2 + 0xC) = SysTask_CreateOnMainQueue(ov96_021E81D8, v2, 1U);
    *((u32 *)&((PokeathlonCourseData *)arg0)->unk_D30) = (u32)(*((u32 *)&((PokeathlonCourseData *)arg0)->unk_D30) + 1);
    return *(s32 *)((u8 *)v2 + 8);
}

void ov96_021E6550(void *param_1) {
    u32 uVar1;
    void *iVar2;

    uVar1 = 0;
    do {
        iVar2 = param_1 + uVar1 * 0x10;
        *(u32 *)(iVar2 + 0xd34) = 0;
        *(u32 *)(iVar2 + 0xd38) = 0;
        *(u32 *)(iVar2 + 0xd3c) = 0;
        SysTask_Destroy(*(SysTask **)(iVar2 + 0xd40));
        *(u32 *)(iVar2 + 0xd40) = 0;
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 3);
    return;
}

void ov96_021E658C(void *param_1, s32 param_2, s32 param_3) {
    Sprite_TryChangeAnimSeq(*(Sprite **)(param_1 + param_2 * 0x10 + 0xd3c), param_3);
    return;
}

void ov96_021E65A4(void *param_1) {
    u32 uVar1;
    void *iVar2;

    uVar1 = 0;
    do {
        iVar2 = param_1 + uVar1 * 0x10;
        if (*(s32 *)(iVar2 + 0xd34) == 0) {
            GF_AssertFail();
        }
        Sprite_SetAnimActiveFlag(*(Sprite **)(iVar2 + 0xd3c), 0);
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 3);
    return;
}

void ov96_021E65D8(void *arg0) {
    u8 v0;
    u8 v2;
    void *v1;

    v0 = ((PokeathlonCourseData *)arg0)->unk_728;
    if (v0 != 0) {
        v2 = 0;
        while ((u32)v2 < (u32) ((PokeathlonCourseData *)arg0)->unk_728) {
            v1 = arg0 + (v2 * 4);
            Sprite_SetDrawFlag(*(Sprite **)((u8 *)v1 + 0x71C), 1);
            Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)v1 + 0x71C), 0);
            v2 += 1;
        }
    }
}

s32 ov96_021E661C(void *arg0) {
    u8 v0;

    if (*(u8 *)((u8 *)arg0 + 0x728) == 0) {
        return 1;
    }
    if (Sprite_IsAnimated(*(Sprite **)((u8 *)arg0 + 0x71C)) == 0) {
        v0 = 0;
        while ((u32)v0 < (u32) * (u8 *)((u8 *)arg0 + 0x728)) {
            Sprite_SetDrawFlag(*(Sprite **)((u8 *)(arg0 + (v0 * 4)) + 0x71C), 0);
            v0 += 1;
        }
        return 1;
    }
    return 0;
}

void ov96_021E6670(void *arg0, s32 arg1) {
    *(s32 *)((u8 *)arg0 + 0x718) = arg1;
}
