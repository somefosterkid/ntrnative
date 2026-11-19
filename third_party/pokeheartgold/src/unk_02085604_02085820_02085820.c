#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "sprite.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "palette.h"

struct TextOBJ;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

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

typedef struct TextOBJ TextOBJ;

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

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
    TextOBJ *unk_90[2];
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

void sub_02085808(UnkStruct_02089688 *param0, int param1);

void *sub_0203A4AC(enum HeapID heapID);

void sub_02086490(UnkStruct_02089688 *param0);

void sub_02086758(UnkStruct_02089688 *param0);

void sub_020868A0(UnkStruct_02089688 *param0);

void sub_020869BC(UnkStruct_02089688 *param0);

void sub_02086DA4(UnkStruct_02089688 *param0);

void sub_02086DE4(UnkStruct_02089688 *param0, int param1);

void sub_02086F44(UnkStruct_02089688 *param0);

void sub_02086FCC(UnkStruct_02089688 *param0);

void sub_02087064(UnkStruct_02089688 *param0);

void sub_020871C4(BgConfig *param0, Window *param1, int param2, int param3, int param4, int param5, int param6, int param7, int param8);

BOOL sub_02085820(UnkStruct_02089688 *param0);

BOOL sub_02085820(UnkStruct_02089688 *param0) {
    sub_02086490(param0);
    sub_02086DA4(param0);
    sub_02086758(param0);
    sub_02086DE4(param0, 0);
    sub_020868A0(param0);
    sub_020869BC(param0);
    sub_02086F44(param0);
    sub_02086FCC(param0);
    sub_02087064(param0);
    sub_020871C4(param0->unk_2EC.unk_0C, &param0->unk_2EC.unk_B0, 4, 2, 21, 27, 2, 100, param0->unk_3B8.unk_2C);

    if (param0->unk_3B8.unk_30 != 0) {
        NNSG2dPaletteData *v0;
        void *v1 = sub_0203A4AC(HEAP_ID_108);

        NNS_G2dGetUnpackedPaletteData(v1, &v0);
        PaletteData_LoadPalette(param0->unk_2EC.unk_10, v0->pRawData, PLTTBUF_SUB_OBJ, ((14) * (16)), ((16) * sizeof(u16)));
        Heap_Free(v1);
    }

    sub_02085808(param0, 1);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((0) << 10) | ((0) << 5) | (0)), 6, 1, HEAP_ID_108);

    return 0;
}

BOOL sub_020858DC(UnkStruct_02089688 *param0);

BOOL sub_020858DC(UnkStruct_02089688 *param0) {
    switch (param0->unk_2C8) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((0) << 10) | ((0) << 5) | (0)), 6, 1, HEAP_ID_108);
        param0->unk_2C8++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == 1) {
            param0->unk_2C8++;
        }
        break;
    default:
        return 1;
    }

    return 0;
}
