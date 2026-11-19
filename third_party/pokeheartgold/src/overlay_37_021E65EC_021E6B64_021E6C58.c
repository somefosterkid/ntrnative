#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "constants/heap.h"

typedef struct UnkStruct_ov37_021E6C58 {
    u8 unk_0[0x30];
    s32 unk_30;
    u8 unk_34[0x2DC];
    s32 unk_310;
} UnkStruct_ov37_021E6C58;

s32 ov37_021E68AC(void *);

s32 ov37_021E76A0(s32);

s32 ov37_021E7844(void *, s32);

s32 ov37_021E6C58(UnkStruct_ov37_021E6C58 *arg0, s32 arg1);

s32 ov37_021E6C58(UnkStruct_ov37_021E6C58 *arg0, s32 arg1) {
    if (ov37_021E76A0(arg0->unk_30) != 0) {
        arg0->unk_310 = 0;
        ov37_021E7844(arg0, 0xC);
    }
    ov37_021E68AC(arg0);
    return arg1;
}

struct BgConfig;

struct Options;

struct SpriteResource;

struct String;

enum FadeType {
    FADE_TYPE_BRIGHTNESS_OUT = 0,
    FADE_TYPE_BRIGHTNESS_IN = 1,
    FADE_TYPE_DOWNWARD_OUT = 2,
    FADE_TYPE_DOWNWARD_IN = 3,
    FADE_TYPE_UPWARD_OUT = 4,
    FADE_TYPE_UNK_5 = 5,
    FADE_TYPE_UNK_6 = 6,
    FADE_TYPE_UNK_7 = 7,
    FADE_TYPE_UNK_8 = 8,
    FADE_TYPE_UNK_9 = 9,
    FADE_TYPE_UNK_10 = 10,
    FADE_TYPE_UNK_11 = 11,
    FADE_TYPE_UNK_12 = 12,
    FADE_TYPE_UNK_13 = 13,
    FADE_TYPE_UNK_14 = 14,
    FADE_TYPE_UNK_15 = 15,
    FADE_TYPE_CIRCLE_OUT = 16,
    FADE_TYPE_CIRCLE_IN = 17,
    FADE_TYPE_TOP_HALF_CIRCLE_OUT = 18,
    FADE_TYPE_TOP_HALF_CIRCLE_IN = 19,
    FADE_TYPE_UNK_20 = 20,
    FADE_TYPE_UNK_21 = 21,
    FADE_TYPE_UNK_22 = 22,
    FADE_TYPE_UNK_23 = 23,
    FADE_TYPE_UNK_24 = 24,
    FADE_TYPE_UNK_25 = 25,
    FADE_TYPE_UNK_26 = 26,
    FADE_TYPE_UNK_27 = 27,
    FADE_TYPE_UNK_28 = 28,
    FADE_TYPE_UNK_29 = 29,
    FADE_TYPE_UNK_30 = 30,
    FADE_TYPE_UNK_31 = 31,
    FADE_TYPE_UNK_32 = 32,
    FADE_TYPE_UNK_33 = 33,
    FADE_TYPE_UNK_34 = 34,
    FADE_TYPE_UNK_35 = 35,
    FADE_TYPE_UNK_36 = 36,
    FADE_TYPE_UNK_37 = 37,
    FADE_TYPE_UNK_38 = 38,
    FADE_TYPE_UNK_39 = 39,
    FADE_TYPE_CLAMP_OUT = 40,
    FADE_TYPE_CLAMP_IN = 41,
    FADE_TYPE_MAX = 42,
};

enum FadeMode {
    FADE_BOTH_SCREENS = 0,
    FADE_MAIN_THEN_SUB,
    FADE_SUB_THEN_MAIN,
    FADE_MAIN_ONLY,
    FADE_SUB_ONLY,
};

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

int ov37_021E6C84(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6C84(UnkStruct_02095EAC *param0, int param1) {
    if (++param0->unk_374 > 60) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_CIRCLE_OUT, FADE_TYPE_CIRCLE_OUT, (((0) << 10) | ((0) << 5) | (0)), 16, 1, HEAP_ID_39);
        param1 = 3;
    }

    ov37_021E68AC(param0);
    return param1;
}
