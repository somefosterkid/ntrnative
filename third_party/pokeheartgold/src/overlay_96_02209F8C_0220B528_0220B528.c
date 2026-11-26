#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_unk_struct_ov96_0220_b9_a8.h"

s32 ov96_0220B940(void *, s32);

s32 ov96_0220B95C(void *);

s32 ov96_0220B988(void *);

s32 ov96_0220B9A8(void *);

s32 ov96_0220BD38(void *);

s32 ov96_0220C680(void *);

s32 ov96_0220D2AC(void *, u32, int);

extern void *ov96_0221E5A4;

typedef struct UnkStruct_ov96_0220B528 {
    u32 a : 2;
    u32 state : 8;
    u32 cnt : 11;
    u32 rest : 11;
} UnkStruct_ov96_0220B528;

void ov96_0220B528(void *arg0, s32 arg1);

void ov96_0220B528(void *arg0, s32 arg1) {
    s16 sp0[2];
    NNSG2dAnimController *temp_r0_2;
    s32 temp_r2;
    u32 temp_r0;
    void *temp_r5;
    if (arg0 == 0L) {
        GF_AssertFail();
    }
    temp_r0 = ((u32)((*((s32 *)((u8 *)&((UnkStruct_ov96_0220B9A8 *)arg0)->pad_38[7] + 0x1))) << 0x16)) >> 0x18;
    switch (temp_r0) {
    case 0:
        ov96_0220B940(arg0, arg1);
        break;

    case 1:
        ov96_0220B95C(arg0);
        break;

    case 2:
        ov96_0220B988(arg0);
        break;

    case 3:
        ov96_0220B9A8(arg0);
        break;

    default:
        GF_AssertFail();
        break;
    }

    ov96_0221E5A4 = ov96_0221E5A4 + 1;
    if (arg1 == 0) {
        ov96_0220C680(arg0);
    }
    ov96_0220BD38(arg0);
    temp_r2 = *((s32 *)(0x40 + ((u8 *)arg0)));
    ov96_0220D2AC(&((UnkStruct_ov96_0220B9A8 *)arg0)->unk_34, ((u32)(temp_r2 << 0xB)) >> 0x15, (u8)(((u32)(temp_r2 << 0x1E)) >> 0x1E));
    ((UnkStruct_ov96_0220B528 *)(((u8 *)arg0) + 0x40))->cnt++;
    if (ManagedSprite_IsAnimated(((UnkStruct_ov96_0220B9A8 *)arg0)->spr10) != 0) {
        temp_r0_2 = (NNSG2dAnimController *)Sprite_GetCellAnim((((UnkStruct_ov96_0220B9A8 *)arg0)->spr10)->sprite);
        temp_r5 = NNS_G2dGetAnimCtrlCurrentElement(temp_r0_2);
        if (temp_r0_2->bActive == 0) {
            GF_AssertFail();
        }
        ManagedSprite_GetPositionXY(((UnkStruct_ov96_0220B9A8 *)arg0)->spr10, &sp0[1], &sp0[0]);
        sp0[1] = (s16)((sp0[1]) + (*((s16 *)(((u8 *)temp_r5) + 4))));
        sp0[0] = (sp0[0]) + (*((s16 *)(((u8 *)temp_r5) + 6)));
        ManagedSprite_SetPositionXY(((UnkStruct_ov96_0220B9A8 *)arg0)->spr14, sp0[1], sp0[0]);
        sp0[0] = (sp0[0]) - 0x18;
        ManagedSprite_SetPositionXY(((UnkStruct_ov96_0220B9A8 *)arg0)->spr18, sp0[1], sp0[0]);
    }
}

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_021E8228.c (tools/pokehg_abi.py)
void ov96_021E8228(PokeathlonCourseData *, u32, u32, s32, s32);
#else
void ov96_021E8228(PokeathlonCourseData *, u8, u8, s32, s32);
#endif

s32 ov96_0220C578(void *, s32);

s32 ov96_0220C714(void *, s32, s32);

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_0220C9CC_0220D07C_0220D33C.c (tools/pokehg_abi.py)
s32 ov96_0220D33C(u32, u32);
#else
s32 ov96_0220D33C(u32, u16);
#endif

typedef struct UnkStruct_ov96_0220B634_Slot {
    u32 pad0 : 18;
    u32 val : 8;
    u32 pad1 : 6;
} UnkStruct_ov96_0220B634_Slot;

typedef struct UnkStruct_ov96_0220B634_Anim {
    u8 pad0[0xC];
    PokeathlonCourseData *course;
    ManagedSprite *spr10;
    u8 pad14[0x20];
    UnkStruct_ov96_0220B634_Slot slots[3];
    u32 sel : 2;
    u32 mode : 8;
    u32 pad40 : 22;
    u32 pad44a : 8;
    u32 flag : 1;
    u32 pad44b : 7;
    u32 kind : 8;
    u32 cnt : 8;
} UnkStruct_ov96_0220B634_Anim;

void ov96_0220B634(UnkStruct_ov96_0220B634_Anim *p, s32 arg1, u8 arg2);

void ov96_0220B634(UnkStruct_ov96_0220B634_Anim *p, s32 arg1, u8 arg2) {
    s32 sp4;
    UnkStruct_ov96_0220B634_Slot *slot;

    sp4 = arg2;
    ov96_0221E5A4 = 0;
    if (p->mode == 2) {
        GF_AssertFail();
    }
    if (p->mode == 1) {
        slot = &p->slots[p->sel];
        slot->val = ov96_0220D33C(slot->val, p->kind);
        if (slot->val == 0) {
            ov96_0220C578(p, 2);
            return;
        }
    }
    if (p->flag == 0) {
        ov96_0220C714(p, arg1, sp4);
    }
    ManagedSprite_SetAnim(p->spr10, 2);
    ov96_021E8228(p->course, ov96_021E5F24(p->course), p->sel, 4, 1);
    ov96_0220C578(p, 1);
}
