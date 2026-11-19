#include "global.h"
struct EvolutionData;

#include "bg_window.h"
#include "error_handling.h"
#include "message_format.h"
#include "sprite_system.h"
#include "sprite.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "evolution_data.h"

void sub_020757AC(struct EvolutionData *);

void sub_02075804(struct EvolutionData *, Window *, struct MsgData *, u32);

void sub_0207584C(struct EvolutionData *arg0, s32 arg1);

extern TouchscreenHitbox _020FFECC;
s32 sub_020758D0(struct EvolutionData *arg0);

s32 sub_020759CC(struct EvolutionData *arg0);

void sub_0207584C(struct EvolutionData *arg0, s32 arg1) {
    s32 v1;
    s32 v0;

    v1 = 0x49D;
    v0 = 0x49E;
    if (arg0->unk_B8 == FALSE) {
        GF_AssertFail();
    }
    if (arg1 == 0) {
        BufferMoveName(arg0->strTemplate, 0U, arg0->moveID);
        v1 = 0x4A2;
        v0 = 0x4A3;
    }
    sub_02075804(arg0, &arg0->windows[0], arg0->msgLoader, v1);
    sub_02075804(arg0, &arg0->windows[1], arg0->msgLoader, v0);
    arg0->unk_8A = 0;
    arg0->unk_8B = 1;
    sub_020757AC(arg0);
    ToggleBgLayer(5U, 1U);
    ToggleBgLayer(6U, 1U);
    ManagedSprite_SetDrawFlag(arg0->unk_B4, 1);
}

s32 sub_020758D0(EvolutionData *arg0) {
    s32 v0;
    s32 v1;

    v1 = 0;
    v0 = TouchscreenHitbox_FindRectAtTouchNew(&_020FFECC);
    if (arg0->unk_B8 == FALSE) {
        GF_AssertFail();
    }
    if (v0 != -1) {
        PlaySE(0x5DCU);
        switch (v0) {
        case 0:
            v1 = 1;
            arg0->unk_8B = 1U;
            break;
        case 1:
            arg0->unk_8B = 2U;
            v1 = 1;
            break;
        default:
            GF_AssertFail();
            break;
        }
    } else if (0x40 & gSystem.newKeys) {
        if (arg0->unk_8B != 1) {
            arg0->unk_8B = 1U;
            sub_020757AC(arg0);
            PlaySE(0x5DCU);
        }
    } else if (0x80 & gSystem.newKeys) {
        if (arg0->unk_8B != 2) {
            arg0->unk_8B = 2U;
            sub_020757AC(arg0);
            PlaySE(0x5DCU);
        }
    } else if (gSystem.newKeys & 1) {
        if (arg0->unk_8B == 0) {
            GF_AssertFail();
            arg0->unk_8B = 2U;
        }
        v1 = 1;
    } else if (gSystem.newKeys & 2) {
        arg0->unk_8B = 2U;
        v1 = 1;
    }
    if (v1 != 0) {
        sub_020757AC(arg0);
        ManagedSprite_SetAnim(arg0->unk_B4, 3);
        PlaySE(0x5DCU);
    }
    return v1;
}

s32 sub_020759CC(EvolutionData *arg0) {
    if (arg0->unk_B8 == FALSE) {
        GF_AssertFail();
    }
    if (ManagedSprite_GetActiveAnim(arg0->unk_B4) != 3) {
        GF_AssertFail();
    }
    if (ManagedSprite_IsAnimated(arg0->unk_B4) == 0) {
        return 1;
    }
    return 0;
}
