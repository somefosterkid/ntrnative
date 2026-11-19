#include "global.h"

#include "filesystem.h"
#include "sprite_system.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "overlay_113.h"

void ov113_021E6930(UnownReportData *arg0);

void ov113_021E6930(UnownReportData *arg0) {
    SpriteSystem_FreeResourcesAndManager(arg0->spriteSystem, arg0->spriteManager2);
    SpriteSystem_DestroySpriteManager(arg0->spriteSystem, arg0->spriteManager1);
    arg0->spriteManager2 = NULL;
    arg0->spriteManager1 = NULL;
    NARC_Delete(arg0->narc);
    SpriteSystem_Free(arg0->spriteSystem);
    arg0->spriteSystem = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(arg0->heapID);
}
