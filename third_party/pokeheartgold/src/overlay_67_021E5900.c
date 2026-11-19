#include "global.h"

#include "frontier_data.h"
#include "heap.h"
#include "overlay_67_app.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "system.h"
#include "unk_0202D230.h"
#include "unk_02030A98.h"

s32 ov67_021E5900(OverlayManager *man, s32 *state);

s32 ov67_021E5900(OverlayManager *man, s32 *state) {
    Ov67App *v0;
    void *v1;

    v1 = OverlayManager_GetArgs(man);
    Heap_Create(HEAP_ID_3, HEAP_ID_72, 0x20000U);
    v0 = OverlayManager_CreateAndGetData(man, sizeof(Ov67App), HEAP_ID_72);
    memset(v0, 0, sizeof(Ov67App));
    v0->isRankingsScreen = (u16) * (u16 *)((u8 *)v1 + 4);
    v0->unk_A = (s8) * (u16 *)((u8 *)v1 + 6);
    v0->options = Save_PlayerData_GetOptionsAddr(*(SaveData **)v1);
    v0->frontierSave = Save_Frontier_GetStatic(*(SaveData **)v1);
    v0->frontierData = Save_FrontierData_Get(*(SaveData **)v1);
    v0->downloadData = sub_0202D928(*(SaveData **)v1);
    v0->heapID = HEAP_ID_72;
    SetKeyRepeatTimers(4, 8);
    return 1;
}
