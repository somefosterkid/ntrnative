#include "global.h"
#include "sprite.h"

struct BattleFactoryAppCursorSprite;
struct CoordinatesS16;
struct Sprite;
void DoScheduledBgGpuUpdates(Sprite *sprite, void *draw);
typedef struct CoordinatesS16 CoordinatesS16;
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

void ov46_022592E0(BattleFactoryAppCursor *cursor, void *draw);

void ov46_022592E0(BattleFactoryAppCursor *cursor, void *draw)
{
    DoScheduledBgGpuUpdates(cursor->sprite, draw);
}
