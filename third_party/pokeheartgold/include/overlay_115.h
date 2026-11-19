#ifndef POKEHEARTGOLD_OVERLAY_115_H
#define POKEHEARTGOLD_OVERLAY_115_H

#include "global.h"
#include "constants/heap.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
struct EncounterEffect;
#include "sys_task.h"
#include "bg_window.h"
#include "linear_interpolation_task.h"
#include "quadratic_interpolation_task.h"
#include "overlay_01_unk_struct_ov5_021de47c.h"
#include "overlay_01_unk_struct_ov5_021de5a4.h"
#include "overlay_01_unk_struct_ov5_021ded04.h"
#include "overlay_01_unk_struct_ov5_021e5128.h"
#include "quadratic_interpolation_task.h"

typedef struct GymLeaderEncounterParam {
    fx32 endX;
    u32 trainerID;
    u16 trainerClass;
    u16 unused;

    u8 mugshotPlttIdx;
    u8 mugshotTileIdx;
    u8 mugshotCellIdx;
    u8 mugshotAnimIdx;
    u8 bannerPlttIdx;
    u8 bannerTileIdx;
    u8 bannerTilemapIdx;
    u8 padding;
} GymLeaderEncounterParam;

// The work of the gym leader / rival mugshot effect (ov115_0225F220 allocates it in EncounterEffect.param).
typedef struct GymLeaderEncounterEffect {
    QuadraticInterpolationTaskFX32 mugshotXTask;
    LinearInterpolationTaskS32 brightnessTask;
    u8 unk_2C[0x14];
    UnkStruct_ov5_021DED04 *bannerEffect;
    UnkStruct_ov5_021DE47C renderer;
    UnkStruct_ov5_021DE5A4 resources[2];
    Sprite *mugshotSprite;
    UnkStruct_ov5_021E5128 scaleSprites;
    u8 unk_250[0x2C];
    Window nameWindow;
    BOOL bannerScrollEnabled;
    s32 bannerScroll;
    s32 timer;
} GymLeaderEncounterEffect;

// sizeof(GymLeaderEncounterEffect) == 0x298 on the DS

void ov115_0225F020(UnkStruct_ov5_021E5128 *param0, UnkStruct_ov5_021DE47C *param1, UnkStruct_ov5_021DE5A4 *param2, fx32 param3, fx32 param4, u32 param5);
void ov115_0225F09C(UnkStruct_ov5_021E5128 *param0);
u32 ov115_0225F0B4(UnkStruct_ov5_021E5128 *param0);
BOOL ov115_0225F220(struct EncounterEffect *encEffect, enum HeapID heapID, GymLeaderEncounterParam *param);

String *ov115_0225F158(u32 arg0, enum HeapID arg1);
String *ov115_0225F1BC(SaveData *arg0, enum HeapID arg1);

void ov115_0225F704(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F728(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F74C(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F770(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F794(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F7B8(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F7DC(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F800(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F824(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F848(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F86C(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F890(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F8B4(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F8D8(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F8FC(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F920(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F944(SysTask *arg0, struct EncounterEffect *arg1);
void ov115_0225F968(void *arg0);

void ov115_02260254(Sprite *param_1, enum HeapID param_2, s32 param_3, u8 param_4, u16 param_5);

void ov115_022602C0(void *arg0, void *arg1);
void ov115_022602E4(void *arg0, void *arg1);
void ov115_02260308(void *arg0, void *arg1);
void ov115_0226032C(void *arg0, void *arg1);
void ov115_02260350(void *arg0, void *arg1);

#endif // POKEHEARTGOLD_OVERLAY_115_H
