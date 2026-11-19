#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "options.h"
#include "overlay_80_02237A70.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "poke_overlay.h"
#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
#include "unk_02096910.h"
#include "battle_castle_app_args.h"
#include "overlay_83_battle_castle_self_app.h"

s32 ov83_0223F1C8(void);

void ov83_0223F200(BattleCastleSelfApp *arg0);

void ov83_022477E4(s32 *arg0);

FS_EXTERN_OVERLAY(OVY_80);

s32 ov83_0223DD60(OverlayManager *arg0);

s32 ov83_0223DD60(OverlayManager *arg0) {
    s32 var_r3;
    s8 var_r0;
    BattleCastleSelfApp *temp_r0;
    BattleCastleAppArgs *args;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_80), OVY_LOAD_ASYNC);
    ov83_0223F1C8();
    Heap_Create(HEAP_ID_3, HEAP_ID_107, 0x30000U);
    temp_r0 = OverlayManager_CreateAndGetData(arg0, sizeof(BattleCastleSelfApp), HEAP_ID_107);
    memset(temp_r0, 0, sizeof(BattleCastleSelfApp));
    temp_r0->bgConfig = BgConfig_Alloc(HEAP_ID_107);
    temp_r0->appMan = arg0;
    args = OverlayManager_GetArgs(arg0);
    temp_r0->saveData = args->saveData;
    temp_r0->castleSave = sub_02030CC8(temp_r0->saveData);
    temp_r0->persistentSave = sub_02030E08(temp_r0->saveData);
    temp_r0->challengeType = args->challengeType;
    temp_r0->selectedMonSlotOut = &args->selectedMonSlot;
    temp_r0->options = Save_PlayerData_GetOptionsAddr(temp_r0->saveData);
    temp_r0->party = args->party;
    temp_r0->slotID = 0xFF;
    temp_r0->partnersCP = args->partnersCP;
    temp_r0->frontier = Save_Frontier_GetStatic(temp_r0->saveData);
    var_r3 = 0;
    do {
        temp_r0->partnersRanks[var_r3] = 1;
        var_r3 += 1;
    } while (var_r3 < 3);
    if (ov80_02237D8C(temp_r0->challengeType) == 0) {
        var_r0 = 3;
    } else {
        var_r0 = 4;
    }
    temp_r0->numSlots = var_r0;
    temp_r0->exitSlot = 4U;
    temp_r0->previousSlot = (s8)(temp_r0->exitSlot - 1);
    ov83_022477E4(&temp_r0->unk_868);
    ov83_0223F200(temp_r0);
    if (ov80_02237D8C(temp_r0->challengeType) == 1) {
        sub_02096910(temp_r0);
    }
    return 1;
}
