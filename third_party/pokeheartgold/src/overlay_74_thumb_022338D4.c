#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "main.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "title_screen.h"

s32 ov74_02236034(s32);

s32 ov74_022338D4(OverlayManager *arg0);

BOOL PmAgbCartridgeHasFlash(void);

s32 ov74_022338D4(OverlayManager *man) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    String_Delete(*(String **)((u8 *)v0 + 0x12608));
    String_Delete(*(String **)((u8 *)v0 + 0x1260C));
    Heap_Free(*(void **)((u8 *)v0 + 0x20));
    RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_TitleScreen);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_76);
    ov74_02236034(0);
    return 1;
}

BOOL PmAgbCartridgeHasFlash(void) {
    if (CTRDG_IdentifyAgbBackup(CTRDG_BACKUP_TYPE_FLASH_1M) != 0) {
        return 0;
    }

    return 1;
}
