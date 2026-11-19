#include "global.h"

#include "battle/battle_controller.h"
#include "battle/battle_hp_bar.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "render_window.h"
#include "sys_task_api.h"
#include "text.h"
#include "unk_02005D10.h"
#include "overlay_12_play_level_up_animation_data.h"
#include "overlay_12_battle_message_wait_task.h"

typedef struct UnkStruct_ov12_02260614_ptr {
    BattleSystem *unk_0;
    u8 unk_4;
    u8 unk_5;
    u8 unk_6;
    u8 unk_7;
    u8 unk_8;
} UnkStruct_ov12_02260614_ptr;

void ov12_02260584(SysTask *arg0, void *arg1);

void ov12_02260584(SysTask *task, void *ptr) {
    u8 v0;

    v0 = ((PlayLevelUpAnimationData *)ptr)->state;
    switch (v0) {
    case 0:
        BattleHpBar_BeginExpBarFullFlashEffect(((BattleHpBar *)((PlayLevelUpAnimationData *)ptr)->healthbox), &((PlayLevelUpAnimationData *)ptr)->flashComplete);
        PlaySE(0x70CU);
        ((PlayLevelUpAnimationData *)ptr)->state = (u8)(((PlayLevelUpAnimationData *)ptr)->state + 1);
        return;
    case 1:
        if (((PlayLevelUpAnimationData *)ptr)->flashComplete == 1) {
            BattleController_EmitClearCommand(*(BattleSystem **)ptr, (s32) ((PlayLevelUpAnimationData *)ptr)->battler, (s32) ((PlayLevelUpAnimationData *)ptr)->command);
            Heap_Free(ptr);
            SysTask_Destroy(task);
        }
        return;
    }
}

void ov12_022605D0(SysTask *arg0, void *arg1);

void ov12_022605D0(SysTask *task, void *ptr) {
    if (ov12_0223BB78(*(BattleSystem **)ptr) != NULL) {
        sub_0200F450(ov12_0223BB78(*(BattleSystem **)ptr));
        ov12_0223BB80(*(BattleSystem **)ptr, NULL);
    }
    if (TextPrinterCheckActive(((BattleMessageWaitTask *)ptr)->msgIdx) == 0) {
        BattleController_EmitClearCommand(*(BattleSystem **)ptr, (s32) ((BattleMessageWaitTask *)ptr)->battler, (s32) ((BattleMessageWaitTask *)ptr)->command);
        Heap_Free(ptr);
        SysTask_Destroy(task);
    }
}

void ov12_02260614(SysTask *task, void *param);

void ov12_02260614(SysTask *task, void *param) {
    UnkStruct_ov12_02260614_ptr *ptr = param;
    u8 v0;

    v0 = ptr->unk_7;
    switch (v0) {
    case 0:
        if (TextPrinterCheckActive(ptr->unk_6) == 0) {
            ptr->unk_7 = (u8)(ptr->unk_7 + 1);
            ptr->unk_8 = 0U;
            return;
        }
        return;
    case 1:
        ptr->unk_8 = (u8)(ptr->unk_8 + 1);
        if (ptr->unk_8 == 0x28) {
            BattleController_EmitAlertMessageAck(ptr->unk_0, (s32) ptr->unk_5);
            BattleController_EmitClearCommand(ptr->unk_0, (s32) ptr->unk_5, (s32) ptr->unk_4);
            Heap_Free(ptr);
            SysTask_Destroy(task);
        }
        break;
    }
}
