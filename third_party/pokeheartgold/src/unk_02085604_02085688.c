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
typedef struct {
    UnkStruct_0208AF44 unk_00[16];
    UnkStruct_0208AF44 unk_1C0[3];
    UnkStruct_0208AF44 unk_214[3];
    UnkStruct_0208AF44 unk_268[2];
    s16 unk_2A0[5];
    u16 unk_2AA[5][2];
    u8 pad_2BE[2];
    int unk_2C0;
    int unk_2C4;
    int unk_2C8;
    int unk_2CC;
    int unk_2D0;
    int unk_2D4;
    int unk_2D8;
    int unk_2DC;
    int unk_2E0;
    int unk_2E4;
    int unk_2E8;
    UnkStruct_02089688_sub1 unk_2EC;
    UnkStruct_02089688_sub2 unk_3AC;
    UnkStruct_02089438 unk_3B8;
    int unk_3EC;
    int unk_3F0;
} UnkStruct_02089688;
void sub_02085604(UnkStruct_02089688 *param0, int param1);

void sub_02085688(UnkStruct_02089688 *param0);
void sub_02085808(UnkStruct_02089688 *param0, int param1);

void sub_02085688(UnkStruct_02089688 *param0) {
    int v0;
    param0->unk_2EC.unk_88 = 1;
    u16 v1 = 0;
    int v2, v3, v4, v5;

    for (v0 = 0; v0 < 4 + 1; v0++) {
        param0->unk_2AA[v0][0] = v1;
        v1 += param0->unk_3B8.unk_04[v0];
        param0->unk_2AA[v0][1] = v1;
    }

    sub_02085604(param0, param0->unk_3B8.unk_24 + 1);

    for (v0 = 0; v0 < 4; v0++) {
        if (param0->unk_3B8.unk_04[v0] == 0) {
            break;
        }

        param0->unk_2D0 += param0->unk_3B8.unk_04[v0];
        param0->unk_3EC++;
    }

    param0->unk_3EC--;
    v2 = 8 * (param0->unk_2D0 + param0->unk_3EC);
    param0->unk_2A0[0] = 112 - v2 / 2;

    for (v0 = 0; v0 < 4; v0++) {
        v2 = 8 * param0->unk_3EC + (8 * (param0->unk_2D0 - param0->unk_3B8.unk_04[v0]) + 32 * param0->unk_3B8.unk_04[v0]);

        param0->unk_2A0[v0 + 1] = 112 - v2 / 2;
    }

    param0->unk_2A0[1] += 12;
    v3 = 0;

    for (v0 = 0; v0 < param0->unk_3EC; v0++) {
        v3 += param0->unk_3B8.unk_04[v0];
        param0->unk_1C0[v0].unk_00 = v3 - 1;
    }

    v5 = 0;
    v0 = 0;

    do {
        for (v4 = 0; v4 < param0->unk_3B8.unk_04[v5]; v4++) {
            param0->unk_00[v0].unk_04 = v5 + 1;
            v0++;
        }
        v5++;
    } while (v0 < param0->unk_2D0);

    for (v0 = 0; v0 < param0->unk_3B8.unk_24; v0++) {
        param0->unk_3F0 += param0->unk_3B8.unk_04[v0];
    }
}

void sub_02085808(UnkStruct_02089688 *param0, int param1) {
    param0->unk_2C0 = param1;
    param0->unk_2C4 = 0;
    param0->unk_2C8 = 0;
    param0->unk_2CC = 0;
}
