#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "filesystem_files_def.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "bg_window.h"
#include "field/overlay_01_021FB878.h"

struct BgConfig;
struct Pokemon;
struct Pokepic;
struct PokepicManager;
struct PokemonSpriteTransforms;
struct PoseStep;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SysTask;
struct SysTaskManager;
typedef struct {
    UnkStruct_ov22_02259560 *unk_00;
    int unk_04;
} UnkStruct_ov22_02259484;
typedef struct {
    UnkStruct_ov22_02259484 *unk_00;
    UnkStruct_ov22_02259560 unk_04;
    UnkStruct_ov22_02259560 unk_14;
    int unk_24;
    int unk_28;
} UnkStruct_ov22_02257964_sub1;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov22_022596B0;
typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;
typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);
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
typedef struct {
    Pokepic *unk_00;
    TouchscreenHitbox unk_04;
    UnkStruct_ov22_022596B0 unk_08;
    Pokemon *unk_0C;
} UnkStruct_020298D8;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef struct {
    int unk_00;
    SoftwareSprite *unk_04;
} UnkStruct_ov22_02255040;
typedef struct {
    UnkStruct_ov22_02255040 *unk_00;
    int unk_04;
} UnkStruct_ov22_02254DE0;
typedef struct {
    BgConfig *unk_00;
    enum NarcId narcID;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
} UnkStruct_ov22_022597BC;
typedef struct {
    SoftwareSpriteManager *unk_00;
    SoftwareSpriteChars **unk_04;
    SoftwareSpritePalette **unk_08;
    u8 *unk_0C;
    PokepicManager *unk_10;
    BgConfig *unk_14;
    UnkStruct_ov22_02254DE0 *unk_18;
    UnkStruct_ov22_022597BC unk_1C;
    int unk_48;
    UnkStruct_020298D8 unk_4C;
} UnkStruct_ov22_02257964_sub2;
typedef struct {
    UnkStruct_ov22_02257964_sub1 unk_00;
    UnkStruct_ov22_02257964_sub2 unk_2C;
} UnkStruct_ov22_02257964;
void ov41_022495F0(UnkStruct_020298D8 *param0, PokepicManager *param1, Pokemon *param2, PokepicTemplate *param3, int param4);
void ov41_02249604(UnkStruct_020298D8 *param0, PokepicManager *param1, Pokemon *param2, PokepicTemplate *param3, int param4, BOOL param5);
void ov41_02249A50(UnkStruct_ov22_02259560 *param0, UnkStruct_ov22_02259560 *param1);
typedef struct SysTaskManager SysTaskManager;
struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};
void ov41_02249A60(UnkStruct_ov22_02259560 *arg0);
void ov41_022496E8(Field3dModel *model);
typedef struct PoseStep PoseStep;
typedef struct CutsceneModel {
    Field3dObject object;
    Field3dModel model;
    Field3DModelAnimation anims[10];
    fx32 scale;
    fx32 animSpeed;
    int pose;
    int poseFrame;
    BOOL playing;
    BOOL looping;
    BOOL loopSecondaryAnim;
    BOOL playSecondaryAnim;
    BOOL useAllAnimTracks;
    BOOL loaded;
    int poseRepeatsLeft;
    int poseRepeatCount;
    int poseStepIndex;
    PoseStep *poseSteps;
} CutsceneModel;

void ov41_02247FE0(UnkStruct_ov22_02257964 *param0, Pokemon *param1, PokepicTemplate *param2, int param3);
void ov41_02247FFC(UnkStruct_ov22_02257964 *param0, Pokemon *param1, PokepicTemplate *param2, int param3);
void ov41_02248020(UnkStruct_ov22_02257964 *param0, UnkStruct_ov22_02259560 *param1);
void ov41_02248030(void *arg0);
void ov41_02248038(CutsceneModel *model);

void ov41_02247FE0(UnkStruct_ov22_02257964 *param0, Pokemon *param1, PokepicTemplate *param2, int param3) {
    ov41_022495F0(&param0->unk_2C.unk_4C, param0->unk_2C.unk_10, param1, param2, param3);
}

void ov41_02247FFC(UnkStruct_ov22_02257964 *param0, Pokemon *param1, PokepicTemplate *param2, int param3) {
    ov41_02249604(&param0->unk_2C.unk_4C, param0->unk_2C.unk_10, param1, param2, param3, 1);
}

void ov41_02248020(UnkStruct_ov22_02257964 *param0, UnkStruct_ov22_02259560 *param1) {
    ov41_02249A50(param1, param0->unk_00.unk_14.unk_0C);
}

void ov41_02248030(void *arg0) {
    ov41_02249A60(arg0);
}

void ov41_02248038(CutsceneModel *model) {
    ov41_022496E8(&model->model);
}
