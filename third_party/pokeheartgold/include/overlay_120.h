#ifndef POKEHEARTGOLD_OVERLAY_120_H
#define POKEHEARTGOLD_OVERLAY_120_H

#include "global.h"
#include "bg_window.h"
#include "field_system_def.h"
#include "filesystem.h"
#include "linear_interpolation_task.h"
#include "overlay_01_encounter_effect.h"
#include "overlay_01_unk_struct_ov5_021de47c.h"
#include "overlay_01_unk_struct_ov5_021de5a4.h"
#include "overlay_01_unk_struct_ov83_0223_da94.h"
#include "quadratic_interpolation_task.h"
#include "screen_shake_effect.h"
#include "sprite.h"
#include "sys_task.h"

// 0x34 bytes: one sliding tile (ov120_0225F020)
typedef struct Ov120Tile {
    LinearInterpolationTaskS32 width;
    LinearInterpolationTaskS32 height;
    Window *window;
    s16 x;
    s16 y;
    u8 active;
    u8 color;
} Ov120Tile;

// 0x108 bytes: shuffled tile order
typedef struct Ov120Shuffle {
    u16 order[0x80];
    s32 pos;
    u32 count;
} Ov120Shuffle;

// 0x114 bytes (ov120_0225F0FC)
typedef struct Ov120Wipe {
    Window *window;
    Ov120Shuffle shuffle;
    u8 count;
    u8 interval;
    s8 timer;
    u8 color;
    u8 active;
} Ov120Wipe;

// 0xcc bytes (ov120_0225F268)
typedef struct Ov120Tiles {
    Window *window;
    Ov120Tile *tiles[48];
    u8 count;
    u8 done;
    u8 interval;
    u8 steps;
    s8 timer;
    u8 color;
    u8 active;
    u8 reverse : 1;
} Ov120Tiles;

// 8 bytes (ov120_0225FA74 / ov120_0225FBC4)
typedef struct Ov120TilesWork {
    Window *window;
    Ov120Tiles *tiles;
} Ov120TilesWork;

typedef struct Ov120StepWork Ov120StepWork;
typedef BOOL (*Ov120StepFunc)(Ov120StepWork *work, s32 *state);

typedef struct Ov120StepTask {
    Ov120StepFunc func;
    s32 state;
} Ov120StepTask;

// 0x19c bytes (ov120_0225F714)
struct Ov120StepWork {
    Window *window;
    Ov120Wipe *wipe;
    struct HBlankSystem *hBlankSystem;
    s32 stepIndex;
    UnkStruct_ov5_021DE47C spriteEnv;
    UnkStruct_ov5_021DE5A4 spriteRes;
    Sprite *sprite;
    Ov120StepTask steps[3];
};

typedef struct Ov120Wobble {
    s16 value;
    s16 base;
    s16 amplitude;
    s16 speed;
    u16 angle;
} Ov120Wobble;

typedef struct Ov120BgLoader {
    EncounterEffect *effect;
    void *ncgr1;
    void *ncgr2;
    NNSG2dCharacterData *charData1;
    NNSG2dCharacterData *charData2;
    SysTask *task;
    SysTask *vblankTask;
    u16 state;
    u16 vblankState;
} Ov120BgLoader;

// 0x70 bytes (ov120_0225FF00)
typedef struct Ov120CrossWork {
    ScreenShakeEffect shake;
    QuadraticInterpolationTaskFX32 scroll1;
    QuadraticInterpolationTaskFX32 scroll3;
    Ov120Wobble wobble;
    s32 delay;
    u8 scroll1Active;
    u8 scroll3Active;
    u8 scroll1Done;
    u8 scroll3Done;
    Ov120BgLoader loader;
} Ov120CrossWork;

Ov120Tile *ov120_0225F020(enum HeapID heapID);
void ov120_0225F038(Ov120Tile *tile);
void ov120_0225F040(Ov120Tile *tile, s32 x, s32 y, s32 numSteps, Window *window, s32 widthStart, s32 heightStart, s32 widthEnd, s32 heightEnd, u8 color);
u32 ov120_0225F08C(Ov120Tile *tile);
Ov120Wipe *ov120_0225F0FC(enum HeapID heapID);
void ov120_0225F118(Ov120Wipe *wipe);
void ov120_0225F120(Ov120Wipe *wipe, s8 interval, Window *window, s8 color);
s32 ov120_0225F14C(Ov120Wipe *wipe);
void ov120_0225F1E8(Ov120Shuffle *shuffle, u32 count);
u16 ov120_0225F240(Ov120Shuffle *shuffle);
Ov120Tiles *ov120_0225F268(enum HeapID heapID);
void ov120_0225F294(Ov120Tiles *tiles);
void ov120_0225F2B8(Ov120Tiles *tiles, u8 interval, u8 steps, Window *window, u8 color, u32 reverse);
BOOL ov120_0225F318(Ov120Tiles *tiles);
BOOL ov120_0225F4DC(Ov120Tiles *tiles);
void ov120_0225F6AC(Ov120Wobble *wobble, s16 base, s32 end, s16 speed);
void ov120_0225F6BC(Ov120Wobble *wobble);
void ov120_0225F6FC(Ov120StepTask *task, Ov120StepFunc func);
BOOL ov120_0225F704(Ov120StepTask *task, Ov120StepWork *work);
void ov120_0225F714(SysTask *task, EncounterEffect *effect);
BOOL ov120_0225F8B0(Ov120StepWork *work, s32 *state);
BOOL ov120_0225F90C(Ov120StepWork *work, s32 *state);
BOOL ov120_0225F970(Ov120StepWork *work, s32 *state);
void ov120_0225F9D4(NARC *narc, Sprite **spriteOut, UnkStruct_ov5_021DE47C *env, UnkStruct_ov5_021DE5A4 *res, s32 which);
void ov120_0225FA5C(SysTask *task, EncounterEffect *effect);
void ov120_0225FA68(SysTask *task, EncounterEffect *effect);
void ov120_0225FA74(SysTask *task, EncounterEffect *effect, BOOL reverse);
void ov120_0225FBAC(SysTask *task, EncounterEffect *effect);
void ov120_0225FBB8(SysTask *task, EncounterEffect *effect);
void ov120_0225FBC4(SysTask *task, EncounterEffect *effect, BOOL reverse);
void ov120_0225FCFC(SysTask *task, EncounterEffect *effect);
void ov120_0225FD08(SysTask *task, EncounterEffect *effect);
void *ov120_0225FD14(NARC *narc, s32 fileId, NNSG2dCharacterData **ppCharData, enum HeapID heapID);
void ov120_0225FD2C(BgConfig *bgConfig, NARC *narc, s32 fileId, u16 bgId);
void ov120_0225FECC(Ov120BgLoader *loader, EncounterEffect *effect);
BOOL ov120_0225FEE8(Ov120BgLoader *loader);
void ov120_0225FF00(SysTask *task, EncounterEffect *effect, BOOL reverse);

// ov01 helpers used by the transition effects
void ov01_021EFCDC(EncounterEffect *encEffect, SysTask *effectTask);
void ov01_021EFCF8(s32 screen, s32 color, s32 otherColor, BOOL *done, s32 numFlashes);
void ov01_021EFE34(LinearInterpolationTaskS32 *task, fx32 start, fx32 end, int numSteps);
BOOL ov01_021EFE44(LinearInterpolationTaskS32 *task);
void ov01_021EFEC8(QuadraticInterpolationTaskFX32 *task, fx32 startValue, fx32 endValue, fx32 initialRate, int numSteps);
BOOL ov01_021EFF28(QuadraticInterpolationTaskFX32 *task);
void ov01_021F05C4(UnkStruct_ov5_021DE47C *env, s32 param1, s32 param2);
void ov01_021F05F4(UnkStruct_ov5_021DE47C *env);
void ov01_021F06EC(UnkStruct_ov5_021DE47C *env, UnkStruct_ov5_021DE5A4 *res);
Sprite *ov01_021F0718(UnkStruct_ov5_021DE47C *env, UnkStruct_ov5_021DE5A4 *res, fx32 x, fx32 y, fx32 z, int param5);
void ov01_021F12D0(ScreenShakeEffect *screenShake);
void ov01_021F12E8(ScreenShakeEffect *screenShake, u8 startX, u8 endX, u16 angleIncrement, fx32 amplitude, s16 shakeSpeed, void *bg, u32 defaultValue, u32 priority);

#endif // POKEHEARTGOLD_OVERLAY_120_H
