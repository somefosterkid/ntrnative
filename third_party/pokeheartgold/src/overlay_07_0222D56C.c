#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"

void Sprite_DeleteAndFreeResources(Sprite *gfxElem);
typedef struct ChooseStarterRotation {
    fx32 verticalOffset;
    fx32 verticalOffsetMultiplier;
    int frameCountMax;
    int frameCount;
} ChooseStarterRotation;
typedef struct ChooseStarterCursor {
    Sprite *sprite;
    SpriteResource *resources[6];
    VecFx32 position;
    SysTask *movementTask;
    ChooseStarterRotation starterRotation;
} ChooseStarterCursor;

void ov07_0222D56C(ChooseStarterCursor *cursor);

void ov07_0222D56C(ChooseStarterCursor *cursor) {
    Sprite_DeleteAndFreeResources(cursor->sprite);
}
