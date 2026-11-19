#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "poke_overlay.h"
#include "render_text.h"
#include "unk_02096910.h"
#include "overlay_82_battle_hall_app.h"

s32 ov80_0223792C(u8);

s32 ov82_0223E9B0(void);

s32 ov82_0223E9E8(void *);

FS_EXTERN_OVERLAY(OVY_80);

s32 ov82_0223DD60(OverlayManager *arg0, s32 *arg1);

s32 ov82_0223DD60(OverlayManager *arg0, s32 *arg1) {
    void *temp_r0;
    void *temp_r0_2;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_80), OVY_LOAD_ASYNC);
    ov82_0223E9B0();
    Heap_Create(HEAP_ID_3, HEAP_ID_105, 0x20000U);
    temp_r0 = OverlayManager_CreateAndGetData(arg0, 0x284U, HEAP_ID_105);
    memset(temp_r0, 0, 0x284U);
    ((BattleHallApp *)temp_r0)->bgConfig = BgConfig_Alloc(HEAP_ID_105);
    *(OverlayManager **)temp_r0 = arg0;
    temp_r0_2 = OverlayManager_GetArgs(arg0);
    ((BattleHallApp *)temp_r0)->saveData = (SaveData *)*(SaveData **)temp_r0_2;
    ((BattleHallApp *)temp_r0)->challengeType = (u8) * (u8 *)((u8 *)temp_r0_2 + 4);
    *((void **)&((BattleHallApp *)temp_r0)->selectedCellPtr) = (void *)(temp_r0_2 + 6);
    ((BattleHallApp *)temp_r0)->options = Save_PlayerData_GetOptionsAddr(((BattleHallApp *)temp_r0)->saveData);
    *((s32 *)&((BattleHallApp *)temp_r0)->party) = (s32) * (s32 *)((u8 *)temp_r0_2 + 0xC);
    *((s32 *)&((BattleHallApp *)temp_r0)->typeRanks) = (s32) * (s32 *)((u8 *)temp_r0_2 + 8);
    *((s32 *)&((BattleHallApp *)temp_r0)->partnersLevel) = (s32) * (s32 *)((u8 *)temp_r0_2 + 0x14);
    *((s16 *)&((BattleHallApp *)temp_r0)->currentStreak) = (s16)(*(u16 *)((u8 *)temp_r0_2 + 0x18) + 1);
    ((BattleHallApp *)temp_r0)->cursorPos = (u8) * (u8 *)((u8 *)temp_r0_2 + 5);
    ((BattleHallApp *)temp_r0)->partnersSelectedCell = 0xFF;
    ((BattleHallApp *)temp_r0)->selectionID = 0xFF;
    *((s8 *)&((BattleHallApp *)temp_r0)->savedCursorPos) = 0x75;
    ov82_0223E9E8(temp_r0);
    if (ov80_0223792C(((BattleHallApp *)temp_r0)->challengeType) == 1) {
        sub_02096910(temp_r0);
    }
    *arg1 = 0;
    TextFlags_SetCanTouchSpeedUpPrint(1);
    return 1;
}
