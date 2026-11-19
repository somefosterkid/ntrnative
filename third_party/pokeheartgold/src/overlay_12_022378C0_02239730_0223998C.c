#include "global.h"

#include "bg_window.h"
#include "overlay_12_022378C0.h"
#include "poke_overlay.h"
#include "sys_task_api.h"
#include "unk_02037C94.h"
#include "battle/battle.h"

void ov12_0223998C(SysTask *task, void *arg1) {
    *((s32 *)&((BattleSystem *)arg1)->unk2438) = (s32)(*((s32 *)&((BattleSystem *)arg1)->unk2438) + 3);
    BgSetPosTextAndCommit(((BattleSystem *)arg1)->bgConfig, 1U, BG_POS_OP_SET_XSCALE, *((s32 *)&((BattleSystem *)arg1)->unk2438));
    if (*((s32 *)&((BattleSystem *)arg1)->unk2438) == 0) {
        SysTask_Destroy(task);
    }
}

FS_EXTERN_OVERLAY(OVY_18);

void ov12_022399BC(void) {
    if (sub_02039998() == 0) {
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_18), OVY_LOAD_ASYNC);
    }
}
