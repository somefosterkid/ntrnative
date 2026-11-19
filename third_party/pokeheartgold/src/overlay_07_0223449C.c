#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"

#include "palette.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_07_ball_rotation.h"

struct BallRotation;

struct BattleMonOBJData;

struct BattleSystem;

struct PaletteFadeContext;

struct ParticleSystem;

struct SpriteManager;

struct SpriteSystem;

struct UnkStruct_ov12_02236648_t;

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

void ManagedSprite_SetDrawFlag(ManagedSprite *managedSprite, int flag);

typedef struct UnkStruct_ov12_02236648_t UnkStruct_ov12_02236648;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    int unk_04;
    enum HeapID heapID;
    int unk_0C;
    int unk_10;
} UnkStruct_ov12_02236690;

typedef struct BattleSystem BattleSystem;

typedef struct BallThrow {
    int type;
    enum HeapID heapID;
    int mode;
    int target;
    int ballID;
    int bgPrio;
    int surface;
    SpriteSystem *cellActorSys;
    PaletteData *paletteSys;
    BattleSystem *battleSys;
} BallThrow;

typedef struct BattleMonOBJData BattleMonOBJData;

typedef struct ParticleSystem ParticleSystem;

typedef struct ValueLerpContext {
    s32 value;
    s32 data[4];
} ValueLerpContext;

typedef struct XYTransformContext {
    s16 x;
    s16 y;
    s32 data[8];
} XYTransformContext;

typedef struct PaletteFadeContext PaletteFadeContext;

struct UnkStruct_ov12_02236648_t {
    UnkStruct_ov12_02236690 unk_00;
    SPLEmitter *unk_14;
    ParticleSystem *unk_18;
    int unk_1C;
    int unk_20;
};

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    int unk_08;
    int unk_0C;
    int unk_10;
} UnkStruct_ov12_02237C54_sub1;

void ov07_0223449C(BallRotation *param0, int flag);

typedef struct MapPropAnimation {
    NNSG3dAnmObj *animationObj;
    BOOL loaded;
    int currentLoop;
    int animeArchiveID;
    BOOL paused;
    void *animationFile;
    int loopCount;
    BOOL looping;
    BOOL reversed;
    int pastoriaGymButtonGroup;
} MapPropAnimation;
void ov07_022344C0(MapPropAnimation *animation, const int pastoriaGymButtonGroup);

void ov07_0223449C(BallRotation *param0, int flag) {
    ManagedSprite_SetDrawFlag(param0->unk_30, flag);
}

void ManagedSprite_SetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);

void ov07_022344A8(BallRotation *param0, s16 x, s16 y);

void ov07_022344A8(BallRotation *param0, s16 x, s16 y) {
    ManagedSprite_SetPositionXY(param0->unk_30, x, y);
}

void ManagedSprite_SetAffineZRotation(ManagedSprite *managedSprite, u16 angle);

void ov07_022344B4(BallRotation *param0, u16 param1);

void ov07_022344B4(BallRotation *param0, u16 param1) {
    ManagedSprite_SetAffineZRotation(param0->unk_30, param1);
}

void ov07_022344C0(MapPropAnimation *animation, const int pastoriaGymButtonGroup) {
    animation->pastoriaGymButtonGroup = pastoriaGymButtonGroup;
}
