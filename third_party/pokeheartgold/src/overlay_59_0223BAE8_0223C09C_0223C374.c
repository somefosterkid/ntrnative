#include "global.h"
#include "sprite.h"
#include "overlay_87_scratch_off_card_app.h"

struct BattleFactoryAppCursorSprite;

struct CoordinatesS16;

struct Sprite;

void PokepicManager_HandleLoadImgAndOrPltt(Sprite *sprite, BOOL draw);

typedef struct BattleFactoryAppCursorSprite {
    u8 numSlots;
    u8 cursorMode;
    u8 currentSlot;
    u8 selectionActive;
    const CoordinatesS16 *positions;
    const u8 *animIDs;
    Sprite *sprite;
    u8 numBallSlots;
} BattleFactoryAppCursor;

void ov59_0223C374(BattleFactoryAppCursor *cursor, BOOL draw);

void ov59_0223C374(BattleFactoryAppCursor *cursor, BOOL draw)
{
    PokepicManager_HandleLoadImgAndOrPltt(cursor->sprite, draw);
}
