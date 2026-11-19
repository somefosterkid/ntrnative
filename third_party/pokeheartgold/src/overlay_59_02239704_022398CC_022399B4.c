#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "sprite_system.h"
#include "touchscreen_list_menu.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"

void ov59_022399B4(Ov59ApricornApp *arg0);

void ov59_022399B4(Ov59ApricornApp *arg0) {
    TouchscreenListMenuSpawner_Destroy(arg0->menuSpawner);
    SpriteSystem_DestroySpriteManager(arg0->spriteSystem, arg0->spriteManager);
    arg0->spriteManager = NULL;
    SpriteSystem_Free(arg0->spriteSystem);
    arg0->spriteSystem = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(arg0->heapId);
}
