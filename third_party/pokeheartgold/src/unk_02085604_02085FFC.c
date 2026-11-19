#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
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
int sub_02086D98(int param0, BOOL param1);

void sub_02085FFC(UnkStruct_02089688 *param0);

void sub_02085FFC(UnkStruct_02089688 *param0) {
    int v0;
    int v1;
    int v2;

    param0->unk_268[0].unk_00 = 1;
    param0->unk_268[0].unk_14.unk_04 = 0;

    if (param0->unk_2D4 == 0) {
        v0 = param0->unk_214[0].unk_00 = param0->unk_2D0 - 1;
        v2 = param0->unk_00[v0].unk_04;

        param0->unk_3AC.unk_00 = 1;
        param0->unk_3AC.unk_04 = v2;
        param0->unk_3AC.unk_08 = 1;

        return;
    }

    v0 = param0->unk_214[0].unk_00;
    param0->unk_00[v0].unk_00 = 0;

    ManagedSprite_SetAnim(param0->unk_00[v0].unk_0C, sub_02086D98(param0->unk_00[v0].unk_00, param0->unk_00[v0].unk_08));
    v1 = param0->unk_00[v0].unk_04;

    if (v0 > param0->unk_3F0) {
        v0--;
        ManagedSprite_SetAnim(param0->unk_00[v0].unk_0C, sub_02086D98(param0->unk_00[v0].unk_00, param0->unk_00[v0].unk_08));

        v2 = param0->unk_00[v0].unk_04;

        if (v1 != v2) {
            param0->unk_3AC.unk_00 = 1;
            param0->unk_3AC.unk_04 = v2;
            param0->unk_3AC.unk_08 = 1;
        } else {
            param0->unk_3AC.unk_00 = 2;
            param0->unk_3AC.unk_04 = v0;
        }
    }
}
