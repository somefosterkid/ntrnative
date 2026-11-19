#include "global.h"
#include "constants/heap.h"
#include "options.h"
#include "pm_string.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct BgConfig;

struct CharTransferAllocation;

struct TextOBJ;

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

enum TouchScreenButtonState {
    TOUCH_BUTTON_NOT_PRESSED = -1,
    TOUCH_BUTTON_PRESSED,
    TOUCH_BUTTON_RELEASED,
    TOUCH_BUTTON_HELD,
    TOUCH_BUTTON_HELD_OUT_OF_BOUNDS
};

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
} TouchScreenRect, TouchscreenHitbox;

typedef void (*TouchScreenHitTableAction)(u32, enum TouchScreenButtonState, void *);

typedef struct TouchScreenActions TouchScreenActions;

TouchScreenActions *TouchHitboxController_Create(const TouchscreenHitbox *hitTable, u32 hitTableLength, TouchScreenHitTableAction action, void *context, u32 heapID);

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
    TextOBJ *unk_90[2];
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

void sub_02086180(u32 param0, enum TouchScreenButtonState param1, void *param2);

void sub_020860B8(UnkStruct_02089688 *param0);

static s16 hgasset_sub_020860B8_v1[][4] __attribute__((section(BWPORT_DATA_SECTION))) = {
            { 0,  0,  0, 0 },
            { 0,  0,  0, 0 },
            { 0, 0,  0, 0 },
            { 0, 0,  0, 0 },
            { 0, 0,  0, 0 },
            { 0,  0, 0, 0 },
            { 0,  0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0,  0, 0, 0 },
            { 0, 0, 0, 0 },
        };

void sub_020860B8(UnkStruct_02089688 *param0) {
    int v0;

    for (v0 = 0; v0 < 15 + 1; v0++) {
        param0->unk_00[v0].unk_10 = &param0->unk_2EC.unk_18[v0];
    }

    {
        __typeof__(hgasset_sub_020860B8_v1) v1; __builtin_memcpy((void *)&v1, &hgasset_sub_020860B8_v1, sizeof(v1));

        for (; v0 < 0x1c; v0++) {
            param0->unk_2EC.unk_18[v0].rect.top = v1[v0 - 16][1] - v1[v0 - 16][3];
            param0->unk_2EC.unk_18[v0].rect.left = v1[v0 - 16][0] - v1[v0 - 16][2];
            param0->unk_2EC.unk_18[v0].rect.bottom = v1[v0 - 16][1] + v1[v0 - 16][3];
            param0->unk_2EC.unk_18[v0].rect.right = v1[v0 - 16][0] + v1[v0 - 16][2];
        }
    }

    param0->unk_2EC.unk_14 = TouchHitboxController_Create(param0->unk_2EC.unk_18, 0x1c, sub_02086180, param0, HEAP_ID_108);
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_src_unk_02085604_020860B8_020860B8_c_0(void) {
    PORT_AssetAdd("IPKE", "src/unk_02085604_020860B8_020860B8.c:hgasset_sub_020860B8_v1", (void *)&hgasset_sub_020860B8_v1, sizeof(hgasset_sub_020860B8_v1), "u8");
    PORT_AssetAdd("IPGE", "src/unk_02085604_020860B8_020860B8.c:hgasset_sub_020860B8_v1", (void *)&hgasset_sub_020860B8_v1, sizeof(hgasset_sub_020860B8_v1), "u8");
}
