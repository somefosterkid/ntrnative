#include "global.h"
#include "overlay_108_safari_decoration_app.h"

#include "bg_window.h"
#include "yes_no_prompt.h"
#include "overlay_108_021E8850.h"
#include "sprite_system.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"

extern ResdatIdList ov108_021EAAB0;
extern OamCharTransferParam ov108_021EAAD0;
extern OamManagerParam ov108_021EABA8;
void ov108_021E9DE0(SafariDecorationApp *param_1) {
    s32 iVar1;
    Window *window;

    YesNoPrompt_Destroy(param_1->yesNoPrompt);
    iVar1 = 0;
    window = param_1->windows;
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 0xc);
    return;
}

void ov108_021E9E10(SafariDecorationApp *arg0) {
    GF_CreateVramTransferManager(0x20U, arg0->heapID);
    arg0->spriteSystem = SpriteSystem_Alloc(arg0->heapID);
    SpriteSystem_Init(arg0->spriteSystem, &ov108_021EABA8, &ov108_021EAAD0, 1);
    arg0->spriteManager = SpriteManager_New(arg0->spriteSystem);
    SpriteSystem_InitSprites(arg0->spriteSystem, arg0->spriteManager, 4);
    sub_0200D2A4(arg0->spriteSystem, arg0->spriteManager, &ov108_021EAAB0, 0, 0);
    thunk_ClearMainOAM(arg0->heapID);
    thunk_ClearSubOAM(arg0->heapID);
}

void ov108_021E9E80(SafariDecorationApp *arg0) {
    SpriteSystem_DestroySpriteManager(arg0->spriteSystem, arg0->spriteManager);
    SpriteSystem_Free(arg0->spriteSystem);
    arg0->spriteSystem = NULL;
    arg0->spriteManager = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(arg0->heapID);
    thunk_ClearSubOAM(arg0->heapID);
}
