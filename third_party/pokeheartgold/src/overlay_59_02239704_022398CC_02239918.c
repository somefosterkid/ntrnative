#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "list_menu_items.h"
#include "sprite_system.h"
#include "touchscreen_list_menu.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"

void ov59_02239918(Ov59ApricornApp *arg0);

void ov59_02239918(Ov59ApricornApp *arg0) {
    if (arg0->mode != 0) {
        ListMenuItems_Delete(arg0->listMenuItems);
        arg0->listMenuItems = NULL;
    }
}

extern ResdatIdList ov59_0223C648;

extern OamCharTransferParam ov59_0223C678;

extern OamManagerParam ov59_0223C750;

void ov59_02239938(Ov59ApricornApp *arg0);

void ov59_02239938(Ov59ApricornApp *arg0) {
    GF_CreateVramTransferManager(0x20U, arg0->heapId);
    arg0->spriteSystem = SpriteSystem_Alloc(arg0->heapId);
    SpriteSystem_Init(arg0->spriteSystem, &ov59_0223C750, &ov59_0223C678, 3);
    thunk_ClearMainOAM(arg0->heapId);
    thunk_ClearSubOAM(arg0->heapId);
    arg0->spriteManager = SpriteManager_New(arg0->spriteSystem);
    SpriteSystem_InitSprites(arg0->spriteSystem, arg0->spriteManager, 0xF);
    sub_0200D2A4(arg0->spriteSystem, arg0->spriteManager, &ov59_0223C648, 0, 0);
    arg0->menuSpawner = TouchscreenListMenuSpawner_Create(arg0->heapId, NULL);
}
