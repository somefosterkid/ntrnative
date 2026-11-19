#include "global.h"

#include "sprite.h"

typedef struct TradeRoomMonDisplayData {
    u16 species;
    u16 heldItemType;
    u8 pokeBall;
    u8 isEgg;
    u8 form;
    u16 gender;
    u16 iconShouldFlip;
    int ballCapsuleId;
} TradeRoomMonDisplayData;

extern void ov65_0221ED80(VecFx32 path[], int startX, int startY, int endX, int endY);
extern void ov65_0221ED94(Sprite *iconSprite, Sprite *itemSprite, Sprite *capsuleSprite, int step, VecFx32 path[], TradeRoomMonDisplayData *displayData);

void ov65_0221ED80(VecFx32 path[], int startX, int startY, int endX, int endY) {
    path[0].x = ((fx32)0x0000000000001000L) * startX;
    path[0].y = ((fx32)0x0000000000001000L) * startY;
    path[1].x = ((fx32)0x0000000000001000L) * endX;
    path[1].y = ((fx32)0x0000000000001000L) * endY;
}

void ov65_0221ED94(Sprite *iconSprite, Sprite *itemSprite, Sprite *capsuleSprite, int step, VecFx32 path[], TradeRoomMonDisplayData *displayData) {
    VecFx32 iconPos;

    iconPos.x = path[0].x + ((path[1].x - path[0].x) / 20) * step;
    iconPos.y = path[0].y + ((path[1].y - path[0].y) / 20) * step;

    Sprite_SetMatrix(iconSprite, &iconPos);

    if (displayData->heldItemType) {
        VecFx32 itemPos;
        itemPos.x = iconPos.x + 20 * ((fx32)0x0000000000001000L);
        itemPos.y = iconPos.y + (16 + 6) * ((fx32)0x0000000000001000L);
        Sprite_SetMatrix(itemSprite, &itemPos);
    }

    if (displayData->ballCapsuleId) {
        VecFx32 capsulePos;
        capsulePos.x = iconPos.x + (20 + 8) * ((fx32)0x0000000000001000L);
        capsulePos.y = iconPos.y + (16 + 6) * ((fx32)0x0000000000001000L);
        Sprite_SetMatrix(capsuleSprite, &capsulePos);
    }
}
