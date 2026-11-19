#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "system.h"
#include "vram_transfer_manager.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_0223F058(BattleCastleSelfApp *param_1);

FS_EXTERN_OVERLAY(OVY_80);

s32 ov83_0223DFBC(OverlayManager *arg0);

s32 ov83_0223DFBC(OverlayManager *man) {
    BattleCastleSelfApp *v0;

    v0 = OverlayManager_GetData(man);
    *v0->selectedMonSlotOut = v0->selectedMonSlot;
    reg_G2_BLDCNT = 0;
    GF_DestroyVramTransferManager();
    ov83_0223F058(v0);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_107);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_80));
    return 1;
}
