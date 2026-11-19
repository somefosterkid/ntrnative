#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"

struct PaletteAnimator;
typedef struct PaletteAnimator PaletteAnimator;
typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);
struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};

void sub_0201649C(PaletteAnimator *param0, u8 param1);

void DestroySysTaskAndEnvironment(Sprite *gfxElem);
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
void sub_020164C4(ChooseStarterCursor *cursor);

void sub_0201649C(PaletteAnimator *param0, u8 param1) {

    switch (param1) {
    case 0:
        param0->unk_4E = 0;
        break;
    case 1:
        param0->unk_4E = 2;
        break;
    case 2:
        param0->unk_4E = 3;
    }
}

void sub_020164C4(ChooseStarterCursor *cursor) {
    DestroySysTaskAndEnvironment(cursor->sprite);
}
