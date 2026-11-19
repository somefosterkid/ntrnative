#ifndef POKEHEARTGOLD_OVERLAY_117_H
#define POKEHEARTGOLD_OVERLAY_117_H

#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "overlay_01_unk_struct_ov5_021de47c.h"
#include "overlay_01_unk_struct_ov5_021de5a4.h"
#include "pm_string.h"
#include "quadratic_interpolation_task.h"
#include "save_layout.h"
#include "screen_shake_effect.h"
#include "sprite.h"
#include "sys_task.h"

struct EncounterEffect;

// One of the five 8-byte records ov117_0225F4D4..ov117_0225F514 pass to ov117_0225F524: the portrait's resource ids
// (the same four ids the gym leader effect keeps in GymLeaderEncounterParam) and the trainer whose name is shown.
typedef struct ExecutiveEncounterParam {
    u8 mugshotPlttIdx;
    u8 mugshotTileIdx;
    u8 mugshotCellIdx;
    u8 mugshotAnimIdx;
    u32 trainerID;
} ExecutiveEncounterParam;

// sizeof(ExecutiveEncounterParam) == 8

// The work of that effect (ov117_0225F524 allocates it in EncounterEffect.param).
typedef struct ExecutiveEncounterEffect {
    /* 0x000 */ Sprite *sprite;
    /* 0x004 */ UnkStruct_ov5_021DE47C renderer;
    /* 0x140 */ UnkStruct_ov5_021DE5A4 resources;
    /* 0x174 */ QuadraticInterpolationTaskFX32 interpTask;
    /* 0x18C */ ScreenShakeEffect screenShake;
    /* 0x198 */ Window nameWindow;
    /* 0x1A8 */ VecFx32 spritePos;
    /* 0x1B4 */ int timer;
    /* 0x1B8 */ BOOL shaking;
} ExecutiveEncounterEffect;

// sizeof(ExecutiveEncounterEffect) == 0x1bc on the DS (the offsets above; a host with 8-byte pointers has a bigger one:
// the allocation uses sizeof)
#ifdef __MWERKS__
LAYOUT_CHECK(ExecutiveEncounterEffect_size, sizeof(ExecutiveEncounterEffect) == 0x1BC);
#endif

String *ov117_0225F470(u32 arg0, enum HeapID arg1);
void ov117_0225F420(struct EncounterEffect *encEffect, u8 bgId, s32 screenFileId);
void ov117_0225F524(SysTask *task, struct EncounterEffect *encEffect, const ExecutiveEncounterParam *param);
void ov117_0225F4D4(SysTask *task, struct EncounterEffect *encEffect);
void ov117_0225F4E4(SysTask *task, struct EncounterEffect *encEffect);
void ov117_0225F4F4(SysTask *task, struct EncounterEffect *encEffect);
void ov117_0225F504(SysTask *task, struct EncounterEffect *encEffect);
void ov117_0225F514(SysTask *task, struct EncounterEffect *encEffect);

#endif // POKEHEARTGOLD_OVERLAY_117_H
