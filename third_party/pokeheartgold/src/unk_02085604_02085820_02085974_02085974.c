#include "global.h"

#include "sprite_system.h"
#include "unk_struct_02089688.h"
#include "options.h"
#include "pm_string.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "touch_hitbox_controller.h"

struct BgConfig;

struct CharTransferAllocation;

struct FontOAM;

struct ManagedSprite;

struct NARC;

struct Options;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct TouchScreenActions;

struct UnkStruct_02012744_t;

struct Window;

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

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchScreenHitTable;

typedef struct TouchScreenActions TouchScreenActions;

typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    BgConfig *unk_0C;
    PaletteData *unk_10;
    TouchScreenActions *unk_14;
    TouchScreenRect unk_18[28];
    BOOL unk_88;
    UnkStruct_02012744 *unk_8C;
    FontOAM *unk_90[2];
    CharTransferAllocation unk_98[2];
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
    TouchScreenRect *unk_10;
    UnkStruct_0208AF44_sub1 unk_14;
} UnkStruct_0208AF44;

void sub_02085808(UnkStruct_02089688 *param0, int param1);

void sub_02086384(UnkStruct_02089688 *param0);

int sub_02086398(UnkStruct_02089688 *param0, int param1);

int sub_020863C0(UnkStruct_02089688 *param0, int param1);

void sub_02086AB4(UnkStruct_02089688 *param0, int param1, BOOL param2);

void sub_02086AE4(UnkStruct_02089688 *param0, int param1);

int sub_02086D98(int param0, BOOL param1);

void sub_02086F44(UnkStruct_02089688 *param0);

void sub_02086180(u32 param0, enum TouchScreenButtonState param1, void *param2);

BOOL sub_02085974(UnkStruct_02089688 *param0);

BOOL sub_02085974(UnkStruct_02089688 *param0) {
    int v0;
    static f32 v1[] = {
        0.5f, 0.2f, 0.5f, 1.0f, 1.2f, 1.0f, 1.0f
    };
    static f32 v2[] = {
        0.8f, 0.6f, 0.4f, 0.2f, 0.8f, 1.0f, 1.0f
    };

    switch (param0->unk_2C8) {
    case 0:
        sub_02086AB4(param0, 0, 0);
        {
            for (v0 = 0; v0 < param0->unk_2D0; v0++) {
                if (param0->unk_00[v0].unk_14.unk_04 == 0) {
                    continue;
                }

                ManagedSprite_OffsetPositionXY(param0->unk_00[v0].unk_0C, param0->unk_00[v0].unk_14.unk_00, param0->unk_00[v0].unk_14.unk_02);
                param0->unk_00[v0].unk_14.unk_04--;

                if ((v0 >= param0->unk_2DC) && (v0 < param0->unk_2E0)) {
                    ManagedSprite_SetAffineScale(param0->unk_00[v0].unk_0C, v1[param0->unk_00[v0].unk_14.unk_05], v1[param0->unk_00[v0].unk_14.unk_05]);
                    param0->unk_00[v0].unk_14.unk_05++;
                }

                if ((v0 >= param0->unk_2E4) && (v0 < param0->unk_2E8)) {
                    ManagedSprite_SetAffineScale(param0->unk_00[v0].unk_0C, v2[param0->unk_00[v0].unk_14.unk_05], v2[param0->unk_00[v0].unk_14.unk_05]);
                    param0->unk_00[v0].unk_14.unk_05++;
                }
            }

            for (v0 = 0; v0 < param0->unk_3EC; v0++) {
                if (param0->unk_1C0[v0].unk_14.unk_04 == 0) {
                    continue;
                }

                ManagedSprite_OffsetPositionXY(param0->unk_1C0[v0].unk_0C, param0->unk_1C0[v0].unk_14.unk_00, param0->unk_1C0[v0].unk_14.unk_02);
                param0->unk_1C0[v0].unk_14.unk_04--;
            }

            if (param0->unk_00[0].unk_14.unk_04 == 0) {
                for (v0 = param0->unk_2DC; v0 < param0->unk_2E0; v0++) {
                    ManagedSprite_SetAnim(param0->unk_00[v0].unk_0C, sub_02086D98(param0->unk_00[v0].unk_00, param0->unk_00[v0].unk_08));
                    ManagedSprite_TickFrame(param0->unk_00[v0].unk_0C);
                }

                for (v0 = param0->unk_2E4; v0 < param0->unk_2E8; v0++) {
                    ManagedSprite_SetAnim(param0->unk_00[v0].unk_0C, sub_02086D98(param0->unk_00[v0].unk_00, param0->unk_00[v0].unk_08));
                    ManagedSprite_TickFrame(param0->unk_00[v0].unk_0C);
                }

                param0->unk_2C8++;
            }

            param0->unk_2CC++;
        }
        break;
    case 1:
        for (v0 = param0->unk_2DC; v0 < param0->unk_2E0; v0++) {
            if (param0->unk_00[v0].unk_14.unk_05 == 6) {
                continue;
            }

            ManagedSprite_SetAffineScale(param0->unk_00[v0].unk_0C, v1[param0->unk_00[v0].unk_14.unk_05], v1[param0->unk_00[v0].unk_14.unk_05]);
            param0->unk_00[v0].unk_14.unk_05++;
        }

        for (v0 = param0->unk_2E4; v0 < param0->unk_2E8; v0++) {
            if (param0->unk_00[v0].unk_14.unk_05 == 6) {
                continue;
            }

            ManagedSprite_SetAffineScale(param0->unk_00[v0].unk_0C, v2[param0->unk_00[v0].unk_14.unk_05], v2[param0->unk_00[v0].unk_14.unk_05]);
            param0->unk_00[v0].unk_14.unk_05++;
        }

        param0->unk_2CC++;

        if (param0->unk_2CC == 6) {
            param0->unk_2C8++;
        }
        break;
    default:
        sub_02086F44(param0);

        if (param0->unk_3AC.unk_08 == 0) {
            sub_02086AE4(param0, sub_02086398(param0, param0->unk_3AC.unk_04));
        } else {
            sub_02086AE4(param0, sub_020863C0(param0, param0->unk_3AC.unk_04));
        }

        if (param0->unk_2D4 != 0) {
            sub_02086AB4(param0, 0, 1);
        }

        sub_02086384(param0);
        sub_02085808(param0, 1);
        break;
    }

    return 0;
}
