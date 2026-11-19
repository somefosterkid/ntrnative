#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "unk_struct_02089688.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct FontOAM;

struct TouchScreenActions;

struct UnkStruct_02012744_t;

typedef struct {
    int unk_00;
    int unk_04[5];
    int unk_18;
    String *unk_1C;
    Options *options;
    int unk_24;
    u32 unk_28;
    u32 unk_2C;
    u32 unk_30;
} UnkStruct_02089438;

typedef struct TouchScreenActions TouchScreenActions;

void TouchHitboxController_IsTriggered(TouchScreenActions *buttonAction);

typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    BgConfig *unk_0C;
    PaletteData *unk_10;
    TouchScreenActions *unk_14;
    TouchscreenHitbox unk_18[28];
    BOOL unk_88;
    UnkStruct_02012744 *unk_8C;
    FontOAM *unk_90[2];
    UnkStruct_02021AC8 unk_98[2];
    Window unk_B0;
} UnkStruct_02089688_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_02089688_sub2;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
} UnkStruct_0208AF44_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    BOOL unk_08;
    ManagedSprite *unk_0C;
    TouchscreenHitbox *unk_10;
    UnkStruct_0208AF44_sub1 unk_14;
} UnkStruct_0208AF44;

void sub_02085C20(UnkStruct_02089688 *param0);

void sub_02086328(UnkStruct_02089688 *param0);

BOOL sub_02085938(UnkStruct_02089688 *param0);

BOOL sub_02085938(UnkStruct_02089688 *param0) {
    switch (param0->unk_2C8) {
    case 0:
        if (IsPaletteFadeFinished() == TRUE) {
            param0->unk_2C8++;
        }
        break;
    default:
        sub_02086328(param0);
        TouchHitboxController_IsTriggered(param0->unk_2EC.unk_14);
        sub_02085C20(param0);
        break;
    }

    return 0;
}
