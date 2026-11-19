#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"

u16 ov102_021E87AC(void *);

s32 ov102_021E9050(u8 *arg0);

void sub_02090D78(Sprite *gfxElem);
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
void ov102_021E9084(ChooseStarterCursor *cursor);

s32 ov102_021E8F64(s32);
void ov102_021E9090(s32 arg0);

s32 ov102_021E9050(u8 *arg0) {
    if (ov102_021E87AC(arg0 + 0x54) != 0) {
        return 1;
    }
    return 0;
}

u16 ov102_021E87B0(u8 *);

s32 ov102_021E9064(u8 *arg0);

s32 ov102_021E9064(u8 *arg0) {
    s32 v0;

    v0 = ov102_021E87AC(arg0 + 0x54);
    if (v0 < ov102_021E87B0(arg0 + 0x54)) {
        return 1;
    }
    return 0;
}

void ov102_021E9084(ChooseStarterCursor *cursor) {
    sub_02090D78(cursor->sprite);
}

void ov102_021E9090(s32 arg0) {
    ov102_021E8F64(arg0 + 0x70);
}
