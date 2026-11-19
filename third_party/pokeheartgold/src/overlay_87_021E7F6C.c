#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "sprite.h"
#include "overlay_87_scratch_off_card_app.h"

struct ScratchOffCardsAppSpriteManager;

Sprite *ov87_021E7DF8(ScratchOffCardsAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u32 resourcePriority, u32 priority, u8 onSubScreen);

void ov87_021E7FEC(ScratchOffCardsAppSprite *appSprite, int x, int y);

ScratchOffCardsAppSprite *ov87_021E7F6C(ScratchOffCardsAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u16 x, u16 y, u8 onSubScreen, u32 resourcePriority, u32 priority);

struct CoordinatesS16;
void *ov87_021E7FC0(ScratchOffCardsAppSprite *appSprite);
void ov87_021E7FD4(ScratchOffCardsAppSprite *appSprite, BOOL draw);
void ov87_021E7FE0(ScratchOffCardsAppSprite *appSprite, BOOL draw);
void ov87_021E8014(ScratchOffCardsAppSprite *appSprite, int *x, int *y);
void ov87_021E803C(ScratchOffCardsAppSprite *appSprite, u32 animID);

ScratchOffCardsAppSprite *ov87_021E7F6C(ScratchOffCardsAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u16 x, u16 y, u8 onSubScreen, u32 resourcePriority, u32 priority) {
    ScratchOffCardsAppSprite *appSprite = Heap_Alloc((enum HeapID)122, sizeof(ScratchOffCardsAppSprite));
    memset(appSprite, 0, sizeof(ScratchOffCardsAppSprite));
    appSprite->sprite = ov87_021E7DF8(spriteMan, resourceID, animID, resourcePriority, priority, onSubScreen);
    appSprite->onSubScreen = onSubScreen;

    ov87_021E7FEC(appSprite, x, y);

    return appSprite;
}

void *ov87_021E7FC0(ScratchOffCardsAppSprite *appSprite) {
    Sprite_Delete(appSprite->sprite);
    Heap_Free(appSprite);

    return 0L;
}

void ov87_021E7FD4(ScratchOffCardsAppSprite *appSprite, BOOL draw) {
    Sprite_SetDrawFlag(appSprite->sprite, draw);
}

void ov87_021E7FE0(ScratchOffCardsAppSprite *appSprite, BOOL draw) {
    Sprite_SetDrawPriority(appSprite->sprite, draw);
}

void ov87_021E7FEC(ScratchOffCardsAppSprite *appSprite, int x, int y) {
    VecFx32 pos;
    pos.x = x * ((fx32)0x0000000000001000L);
    pos.y = y * ((fx32)0x0000000000001000L);

    if (appSprite->onSubScreen == 1) {
        pos.y += ((fx32)(((512) > 0) ? ((512) * (1 << 12) + 0.5f) : ((512) * (1 << 12) - 0.5f)));
    }

    Sprite_SetMatrix(appSprite->sprite, &pos);
}

void ov87_021E8014(ScratchOffCardsAppSprite *appSprite, int *x, int *y) {
    const VecFx32 *pos = Sprite_GetMatrixPtr(appSprite->sprite);

    *x = pos->x / ((fx32)0x0000000000001000L);
    *y = pos->y / ((fx32)0x0000000000001000L);
}

void ov87_021E803C(ScratchOffCardsAppSprite *appSprite, u32 animID) {
    Sprite_SetAnimSpeed(appSprite->sprite, ((fx32)0x0000000000001000L));
    Sprite_TryChangeAnimSeq(appSprite->sprite, animID);
}
