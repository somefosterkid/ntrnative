#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "library/spl_emitter.h"

#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_ball_rotation.h"
#include "particle_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_value_lerp_context.h"
#include "overlay_07_palette_fade.h"

struct BallRotation;

struct BattleMonOBJData;

struct PaletteFadeContext;

struct ParticleSystem;

struct UnkStruct_ov12_02236648_t;

typedef struct UnkStruct_ov12_02236648_t UnkStruct_ov12_02236648;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    int unk_04;
    enum HeapID heapID;
    int unk_0C;
    int unk_10;
} UnkStruct_ov12_02236690;

typedef struct BattleMonOBJData BattleMonOBJData;

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

void UnkBallData_SetBallAnimation(BallRotation *param0, int param1);

void UnkBallData_SetBallAnimation(BallRotation *param0, int param1) {
    param0->unk_00 = param1;
    param0->unk_04 = 0;
}
