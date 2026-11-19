#include "global.h"
#include "sprite_system.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "overlay_59_juice_feed_app.h"

extern ResdatIdList ov59_0223C968;

extern OamCharTransferParam ov59_0223C988;

extern OamManagerParam ov59_0223CA20;

void ov59_0223B804(Ov59JuiceFeedApp *arg0);

void ov59_0223B804(Ov59JuiceFeedApp *arg0) {
    GF_CreateVramTransferManager(0x20U, arg0->heapId);
    arg0->spriteSystem = SpriteSystem_Alloc(arg0->heapId);
    SpriteSystem_Init(arg0->spriteSystem, &ov59_0223CA20, &ov59_0223C988, 3);
    thunk_ClearMainOAM(arg0->heapId);
    thunk_ClearSubOAM(arg0->heapId);
    arg0->spriteManager = SpriteManager_New(arg0->spriteSystem);
    SpriteSystem_InitSprites(arg0->spriteSystem, arg0->spriteManager, 0x22);
    sub_0200D2A4(arg0->spriteSystem, arg0->spriteManager, &ov59_0223C968, 0, 0);
}

void ov59_0223B878(Ov59JuiceFeedApp *arg0);

void ov59_0223B878(Ov59JuiceFeedApp *arg0) {
    SpriteSystem_DestroySpriteManager(arg0->spriteSystem, arg0->spriteManager);
    arg0->spriteManager = NULL;
    SpriteSystem_Free(arg0->spriteSystem);
    arg0->spriteSystem = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(arg0->heapId);
}
