#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "system.h"
#include "vram_transfer_manager.h"

s32 ov83_02243E30(void *);

FS_EXTERN_OVERLAY(OVY_80);

s32 ov83_02243268(OverlayManager *arg0);

s32 ov83_02243268(OverlayManager *man) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    **(s16 **)((u8 *)v0 + 0x548) = (s16) * (u8 *)((u8 *)v0 + 0xD);
    reg_G2_BLDCNT = 0;
    GF_DestroyVramTransferManager();
    ov83_02243E30(v0);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_107);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_80));
    return 1;
}
