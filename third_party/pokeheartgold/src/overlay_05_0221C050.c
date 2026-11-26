#include "global.h"

#include "bg_window.h"
#include "math_util.h"
#include "overlay_05.h"
#include "screen_fade.h"
#include "unk_02005D10.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "battle/trainer_intro_data.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov05_0221D3AC(void *, u8, s16, s16);
#else
s32 ov05_0221D3AC(void *, s32, s16, s32);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov05_0221D664(void *, s32, s16, s16);
#else
s32 ov05_0221D664(void *, s32, s16, s32);
#endif

s32 ov05_0221CCF4(void);

u8 ov05_0221C5C4(void *);
s32 ov05_0221C6C8();

s32 ov05_0221C050(UnkStruct_ov10_0221FB28 *arg0) {
    if (Bg_GetXpos(arg0->unk_0C, GF_BG_LYR_MAIN_3) <= 0x18) {
        arg0->unk_B82 = 0U;
        arg0->unk_B78 = 0;
        arg0->unk_B7A = 0;
        ScheduleSetBgPosText(arg0->unk_0C, 3U, BG_POS_OP_SET_ROT, 0x18);
        ScheduleSetBgPosText(arg0->unk_0C, 2U, BG_POS_OP_SET_ROT, -0x18);
        ov05_0221D3AC(arg0, 0, 0, 0);
        ov05_0221D3AC(arg0, 1, 0, 0);
        ov05_0221D3AC(arg0, 2, 0, 0);
        ov05_0221D3AC(arg0, 3, 0, 0);
        ov05_0221D664(arg0, 0, 0, 0);
        ov05_0221D664(arg0, 1, 0, 0);
        ov05_0221D664(arg0, 2, 0, 0);
        ov05_0221D664(arg0, 3, 0, 0);
        return 1;
    }
    ScheduleSetBgPosText(arg0->unk_0C, 3U, BG_POS_OP_SUB_ROT, arg0->unk_B74);
    ScheduleSetBgPosText(arg0->unk_0C, 2U, BG_POS_OP_ADD_ROT, arg0->unk_B74);
    arg0->unk_B82 = (u8)(arg0->unk_B82 + 1);
    ov05_0221D3AC(arg0, 0, (s16)((arg0->unk_B74 * arg0->unk_B82) - 0x100), 0);
    ov05_0221D3AC(arg0, 1, (s16)((arg0->unk_B74 * arg0->unk_B82) - 0x100), 0);
    ov05_0221D3AC(arg0, 2, (s16)(0x100 - (arg0->unk_B74 * arg0->unk_B82)), 0);
    ov05_0221D3AC(arg0, 3, (s16)(0x100 - (arg0->unk_B74 * arg0->unk_B82)), 0);
    ov05_0221D664(arg0, 0, (s16)((arg0->unk_B74 * arg0->unk_B82) - 0x100), 0);
    ov05_0221D664(arg0, 1, (s16)((arg0->unk_B74 * arg0->unk_B82) - 0x100), 0);
    ov05_0221D664(arg0, 2, (s16)(0x100 - (arg0->unk_B74 * arg0->unk_B82)), 0);
    ov05_0221D664(arg0, 3, (s16)(0x100 - (arg0->unk_B74 * arg0->unk_B82)), 0);
    return 0;
}

s32 ov05_0221DAE0(UnkStruct_ov10_0221FB28 *arg0);

s32 ov05_0221C21C(UnkStruct_ov10_0221FB28 *arg0) {
    s16 value;
    s16 v3;
    s8 v1;
    s8 v2;
    u8 v0;

    if ((ov05_0221DAE0(arg0) == 0) && ((u32)arg0->unk_B82 > 0x18U)) {
        arg0->unk_B82 = 0U;
        return 1;
    }
    if (arg0->unk_B82 == 0) {
        if (*(u8 *)((u8 *)(*(void **)arg0) + 0x28) == 1) {
            PlaySE(0x715U);
        } else {
            PlaySE(0x852U);
        }
    }
    v0 = arg0->unk_B82;
    if (v0 == 0x18) {
        ScheduleSetBgPosText(arg0->unk_0C, 3U, BG_POS_OP_SET_ROT, 0x18);
        ScheduleSetBgPosText(arg0->unk_0C, 2U, BG_POS_OP_SET_ROT, -0x18);
        ScheduleSetBgPosText(arg0->unk_0C, 3U, BG_POS_OP_SET_XSCALE, 0);
        ScheduleSetBgPosText(arg0->unk_0C, 2U, BG_POS_OP_SET_XSCALE, 0);
        ov05_0221D3AC(arg0, 0, 0, 0);
        ov05_0221D3AC(arg0, 1, 0, 0);
        ov05_0221D3AC(arg0, 2, 0, 0);
        ov05_0221D3AC(arg0, 3, 0, 0);
        ov05_0221D664(arg0, 0, 0, 0);
        ov05_0221D664(arg0, 1, 0, 0);
        ov05_0221D664(arg0, 2, 0, 0);
        ov05_0221D664(arg0, 3, 0, 0);
    } else if (!((u32)v0 >= 0x18U)) {
        v1 = arg0->unk_B7C;
        v3 = ((s32)LCRandom() % v1) - ((s32)(v1 + ((u32)v1 >> 0x1F)) >> 1);
        v2 = arg0->unk_B7D;
        value = ((s32)LCRandom() % v2) - ((s32)(v2 + ((u32)v2 >> 0x1F)) >> 1);
        if ((((s32)v3 < 0) && ((s32)arg0->unk_B78 < 0)) || (((s32)v3 > 0) && ((s32)arg0->unk_B78 > 0))) {
            v3 *= -1;
        }
        if ((((s32)value < 0) && ((s32)arg0->unk_B7A < 0)) || (((s32)value > 0) && ((s32)arg0->unk_B7A > 0))) {
            value *= -1;
        }
        ScheduleSetBgPosText(arg0->unk_0C, 3U, BG_POS_OP_SET_ROT, v3 + 0x18);
        ScheduleSetBgPosText(arg0->unk_0C, 2U, BG_POS_OP_SET_ROT, v3 - 0x18);
        ScheduleSetBgPosText(arg0->unk_0C, 3U, BG_POS_OP_SET_XSCALE, (s32)value);
        ScheduleSetBgPosText(arg0->unk_0C, 2U, BG_POS_OP_SET_XSCALE, (s32)value);
        ov05_0221D3AC(arg0, 0, v3, (s32)value);
        ov05_0221D3AC(arg0, 1, v3, (s32)value);
        ov05_0221D3AC(arg0, 2, v3, (s32)value);
        ov05_0221D3AC(arg0, 3, v3, (s32)value);
        ov05_0221D664(arg0, 0, v3, (s32)value);
        ov05_0221D664(arg0, 1, v3, (s32)value);
        ov05_0221D664(arg0, 2, v3, (s32)value);
        ov05_0221D664(arg0, 3, v3, (s32)value);
        arg0->unk_B78 = v3;
        arg0->unk_B7A = value;
    }
    arg0->unk_B82 = (u8)(arg0->unk_B82 + 1);
    return 0;
}

s32 ov05_0221C430(UnkStruct_ov10_0221FB28 *arg0) {
    if (arg0->unk_B82 == 8) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7FFFU, 6, 1, *(enum HeapID *)((u8 *)(*(void **)arg0) + 0x24));
    }
    if (((u32)arg0->unk_B82 >= 8U) && (IsPaletteFadeFinished() == 1)) {
        ((UnkStruct_ov10_0221FB28 *)arg0)->unk_B7F = 2;
        return 1;
    }
    if (arg0->unk_B82 == 0) {
        PlaySE(0x719U);
    }
    ScheduleSetBgPosText(arg0->unk_0C, 3U, BG_POS_OP_ADD_ROT, 0x10);
    ScheduleSetBgPosText(arg0->unk_0C, 2U, BG_POS_OP_SUB_ROT, 0x10);
    arg0->unk_B82 = (u8)(arg0->unk_B82 + 1);
    ov05_0221D3AC(arg0, 0, (s16)(0 - (arg0->unk_B82 * 0x10)), 0);
    ov05_0221D3AC(arg0, 1, (s16)(0 - (arg0->unk_B82 * 0x10)), 0);
    ov05_0221D3AC(arg0, 2, (s16)(arg0->unk_B82 * 0x10), 0);
    ov05_0221D3AC(arg0, 3, (s16)(arg0->unk_B82 * 0x10), 0);
    ov05_0221D664(arg0, 0, (s16)(0 - (arg0->unk_B82 * 0x10)), 0);
    ov05_0221D664(arg0, 1, (s16)(0 - (arg0->unk_B82 * 0x10)), 0);
    ov05_0221D664(arg0, 2, (s16)(arg0->unk_B82 * 0x10), 0);
    ov05_0221D664(arg0, 3, (s16)(arg0->unk_B82 * 0x10), 0);
    return 0;
}

s32 ov05_0221C558(UnkStruct_ov10_0221FB28 *arg0) {
    arg0->unk_B81 = 0x10;
    return 2;
}

s32 ov05_0221C568(UnkStruct_ov10_0221FB28 *arg0) {
    void *v0;
    v0 = arg0->trainerIntroData;
    if (ov05_0221CCF4() == 1) {
        if ((arg0->trainerIntroData->battleResult) == 1) {
            arg0->unk_B7E = 0;
        } else {
            arg0->unk_B7E = 1;
        }
    } else if ((arg0->trainerIntroData->battleResult) == 1) {
        arg0->unk_B7E = 1;
    } else {
        arg0->unk_B7E = 0;
    }
    return 2;
}

void ov05_0221C5A8(UnkStruct_ov10_0221FB28 *arg0) {
    if (arg0->unk_B7E == 0) {
        ov05_0221C5C4(arg0);
        return;
    }
    ov05_0221C6C8();
}
