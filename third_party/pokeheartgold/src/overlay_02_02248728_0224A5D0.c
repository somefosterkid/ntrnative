#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "heap.h"
#include "sys_task.h"
#include "gf_gfx_planes.h"

struct BgConfig;
struct PokemonAnim;
struct Pokepic;
struct PokepicTemplate;
struct PokemonSpriteTransforms;
struct TransformData;
typedef struct PokepicTemplate PokepicTemplate;
typedef struct BgConfig BgConfig;
void BgSetPosTextAndCommit(BgConfig *bgConfig, u8 bgLayer, u8 bgOffsetUpdateOp, int val);
void BgCommitTilemapBufferToVram(BgConfig *bgConfig, u8 bgLayer);
void BG_LoadScreenTilemapData(BgConfig *bgConfig, u8 bgLayer, void *src, u32 size);
void BgTilemapRectChangePalette(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 width, u8 height, u8 palette);
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef struct Pokepic Pokepic;
typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;
typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);
struct PokepicTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
};
struct PokemonSpriteTransforms {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};
typedef struct PokemonSpriteShadow {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
} PokemonSpriteShadow;
struct Pokepic {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokepicTemplate template;
    PokepicTemplate templateBackup;
    PokemonSpriteTransforms transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokemonSpriteShadow shadow;
    PokemonSpriteShadow shadowBackup;
    SpriteAnimFrame animFrames[10];
};
typedef struct TransformData TransformData;
typedef struct PokemonAnim PokemonAnim;
struct TransformData {
    BOOL active;
    int vars[8];
    int *transformMemberPtr;
    int *animMemberPtr;
    u8 calcType;
    u8 startDelay;
    u8 pad_2E[2];
    int originalValue;
    int dummy_34;
    int dummy_38;
    int offsetX;
    int offsetY;
    int scaleX;
    int scaleY;
    int rotationZ;
    void *func;
};
struct PokemonAnim {
    Pokepic *sprite;
    SysTask *task;
    void *scriptData;
    u32 *scriptPtr;
    BOOL active;
    int animNum;
    int waitFrame;
    int endAnim;
    BOOL completed;
    int vars[8];
    int commandCount;
    int loopMax;
    int loopCounter;
    u32 *loopStart;
    int startDelay;
    int originalX;
    int originalY;
    int translateX;
    int translateY;
    int offsetX;
    int offsetY;
    int scaleX;
    int scaleY;
    int rotationZ;
    struct { BOOL active; int vars[8]; int *transformMemberPtr; int *animMemberPtr; u8 calcType; u8 startDelay; u8 unk_30_pad[2]; int originalValue; int dummy_34; int dummy_38; int offsetX; int offsetY; int scaleX; int scaleY; int rotationZ; void *func; } transforms[4];
    u8 flipSprite;
    u8 waitForTransform;
    u8 yNormalization;
    u8 fadeActive;
};
enum PokemonAnimConstants {
    TRANSFORM_FUNC_CURVE = 0,
    TRANSFORM_FUNC_CURVE_EVEN = 1,
    TRANSFORM_FUNC_LINEAR = 2,
    TRANSFORM_FUNC_LINEAR_EVEN = 3,
    TRANSFORM_FUNC_LINEAR_BOUNDED = 4,
    ANIM_TRANSLATE_X = 8,
    ANIM_TRANSLATE_Y = 9,
    ANIM_OFFSET_X = 10,
    ANIM_OFFSET_Y = 11,
    ANIM_SCALE_X = 12,
    ANIM_SCALE_Y = 13,
    ANIM_ROTATION_Z = 14,
    COMPARISON_RESULT_LESS_THAN = 15,
    COMPARISON_RESULT_GREATER_THAN = 16,
    COMPARISON_RESULT_EQUAL = 17,
    ANIM_READ_TYPE_VALUE = 18,
    ANIM_READ_TYPE_VAR = 19,
    ANIM_READ_TYPE_VALUE2 = 20,
    ANIM_READ_TYPE_VAR2 = 21,
    ANIM_ATTRIBUTE_SET = 22,
    ANIM_ATTRIBUTE_ADD = 23,
    TRANSFORM_CALC_SET = 24,
    TRANSFORM_CALC_ADD = 25,
    TRANSFORM_CALC_INCREMENT = 26,
    Y_NORMALIZATION_NEGATIVE_SCALE = 27,
    Y_NORMALIZATION_OFF = 28,
    Y_NORMALIZATION_ON = 29,
    TRANSFORM_CURVE_SIN = 30,
    TRANSFORM_CURVE_COS = 31,
    TRANSFORM_CURVE_NEGATIVE_SIN = 32,
    TRANSFORM_CURVE_NEGATIVE_COS = 33,
    TRANSFORM_TYPE_OFFSET_X = 35,
    TRANSFORM_TYPE_OFFSET_Y = 36,
    TRANSFORM_TYPE_SCALE_X = 37,
    TRANSFORM_TYPE_SCALE_Y = 38,
    TRANSFORM_TYPE_ROTATION_Z = 39,
    ANIM_VAR_OFFSET = 268435455,
    ANIM_VAR_0 = 268435455,
    ANIM_VAR_1 = 268435456,
    ANIM_VAR_2 = 268435457,
    ANIM_VAR_3 = 268435458,
    ANIM_VAR_4 = 268435459,
    ANIM_VAR_5 = 268435460,
    ANIM_VAR_6 = 268435461,
    ANIM_VAR_7 = 268435462,
};
void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 funcType);

void ov02_0224A5D0(BgConfig *bgConfig, NARC *narc, u32 memberIndex, NNSG2dScreenData **screenData);
void ov02_0224A63C(BgConfig *bgConfig);

void ov02_0224A5D0(BgConfig *bgConfig, NARC *narc, u32 memberIndex, NNSG2dScreenData **screenData) {
    void *nscrFile;

    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, 0, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, 3, 0);

    nscrFile = NARC_AllocAndReadWholeMember(narc, memberIndex, HEAP_ID_FIELD1);
    NNS_G2dGetUnpackedScreenData(nscrFile, screenData);

    BG_LoadScreenTilemapData(bgConfig, GF_BG_LYR_MAIN_3, (void *)(*screenData)->rawData, (*screenData)->szByte);
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 32, 32, 12);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_3);
    Heap_Free(nscrFile);
}

void ov02_0224A63C(BgConfig *bgConfig) {
    BgClearTilemapBufferAndCommit(bgConfig, TRANSFORM_FUNC_LINEAR_EVEN);
}
