#include "global.h"

#include "heap.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "system.h"
#include "vram_transfer_manager.h"

void ov83_02243E30(BattleCastleOpponentApp *app);

FS_EXTERN_OVERLAY(OVY_80);

s32 ov83_02243268(OverlayManager *arg0);

s32 ov83_02243268(OverlayManager *man) {
    BattleCastleOpponentApp *app;

    app = OverlayManager_GetData(man);
    *app->selectedMonSlotOut = app->selectedMonSlot;
    reg_G2_BLDCNT = 0;
    GF_DestroyVramTransferManager();
    ov83_02243E30(app);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_107);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_80));
    return 1;
}
