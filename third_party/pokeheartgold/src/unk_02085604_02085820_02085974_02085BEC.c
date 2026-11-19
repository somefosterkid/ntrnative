#include "global.h"

#include "bg_window.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "pm_string.h"
#include "sprite_system.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_struct_02089688.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct TextOBJ;

struct TouchHitboxController;

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

typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    BgConfig *unk_0C;
    PaletteData *unk_10;
    TouchHitboxController *unk_14;
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

void sub_02086BB4(UnkStruct_02089688 *param0);

void sub_02086C8C(UnkStruct_02089688 *param0);

extern BOOL (*const _0210271C[4])(UnkStruct_02089688 *);

BOOL sub_02085BEC(UnkStruct_02089688 *param0);

BOOL sub_02085BEC(UnkStruct_02089688 *param0) {
    BOOL v0 = _0210271C[param0->unk_2C0](param0);

    sub_02086BB4(param0);
    sub_02086C8C(param0);
    SpriteSystem_DrawSprites(param0->unk_2EC.unk_08);

    return v0;
}
