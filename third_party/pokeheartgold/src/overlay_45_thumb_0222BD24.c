#include "global.h"
#include "sprite.h"
#include "overlay_87_scratch_off_card_app.h"
#include "overlay_45_wifi_plaza_system.h"

void ov45_0222BD24(UnkStruct_ov66_0222FBC8 *arg0);

struct CoordinatesS16;
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
u8 ov45_0222BD2C(BattleFactoryAppCursor *cursor);

void ov45_0222BD24(UnkStruct_ov66_0222FBC8 *arg0) {
    arg0->unk_02 = 1;
}

u8 ov45_0222BD2C(BattleFactoryAppCursor *cursor) {
    return cursor->currentSlot;
}
