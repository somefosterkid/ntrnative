#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_82.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "system.h"
#include "overlay_82_battle_hall_app.h"

void ov82_0223E8C4(BattleHallApp *arg0);

FS_EXTERN_OVERLAY(OVY_80);

s32 ov82_0223DF74(OverlayManager *man) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    **((s16 **)&((BattleHallApp *)v0)->selectedCellPtr) = (s16) ((BattleHallApp *)v0)->cursorPos;
    ov82_0223E8C4(v0);
    OverlayManager_FreeData(man);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_105);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_80));
    return 1;
}
