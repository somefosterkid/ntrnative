#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"

void Camera_Delete(Sprite *gfxElem);
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
void Camera_PushLookAtToNNSGlb(void);

void ov48_02259C38(ChooseStarterCursor *cursor);
void ov48_02259C44(void);

void ov48_02259C38(ChooseStarterCursor *cursor) {
    Camera_Delete(cursor->sprite);
}

void ov48_02259C44(void) {
    Camera_PushLookAtToNNSGlb();
}
