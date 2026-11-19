#include "global.h"
struct TradeTubePhase;

#include "bg_window.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "overlay_71_trade_tube_phase.h"

typedef struct UnkStruct_ov71_02248B60_arg1 {
    u8 unk_0[0xC0];
    BgConfig *unk_C0;
    u8 unk_C4[0xA4];
    s32 unk_168;
} UnkStruct_ov71_02248B60_arg1;

void ov71_022472FC(void *animResources);

void ov71_02248B24(TradeTubePhase *ttPhase);

void ov71_02248B24(TradeTubePhase *ttPhase) {
    Sprite *sprite;
    Sprite *v0;
    s32 i;

    i = 0;
    do {
        sprite = ttPhase->sprites[i][0];
        if (sprite != NULL) {
            Sprite_Delete(sprite);
        }
        v0 = ttPhase->sprites[i][1];
        if (v0 != NULL) {
            Sprite_Delete(v0);
        }
        i += 1;
    } while (i < 20);
    ov71_022472FC(&ttPhase->animResources);
}

void ov71_02248B54(struct TradeTubePhase *arg0);

void ov71_02248B54(TradeTubePhase *arg0) {
    *((s32 *)&arg0->screenSwapPending) = 1;
}

void ov71_02248B60(SysTask *task, void *param);

void ov71_02248B60(SysTask *task, void *param) {
    UnkStruct_ov71_02248B60_arg1 *arg1 = param;
    if (arg1->unk_168 != 0) {
        arg1->unk_168 = 0;
        BgSetPosTextAndCommit(arg1->unk_C0, 2U, BG_POS_OP_SET_XSCALE, -0x100);
        BgSetPosTextAndCommit(arg1->unk_C0, 6U, BG_POS_OP_SET_XSCALE, 0);
        ToggleBgLayer(3U, 0U);
        ToggleBgLayer(7U, 1U);
        reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
        SysTask_Destroy(task);
    }
}
