#include "global.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "sys_task.h"
#include "unk_02014DA0.h"
#include "library/spl_emitter.h"
#include "overlay_manager.h"
#include "particle_system.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "overlay_07_generic_emitter.h"

struct BattleAnimSystem;

struct ParticleSystem;

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

extern BOOL ov07_0221F9A8(BattleAnimSystem *system, int params[], int count);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

u8 ov07_02231924(BattleAnimSystem *system, int battler);
s8 ov07_02221664(GenericEmitterCallbackData *data);
void ov07_022216A8(GenericEmitterCallbackData *data, int battler, VecFx32 *pos);
void ov07_02221734(GenericEmitterCallbackData *data, VecFx32 *pos);

struct OverlayManager;
void ov07_022217A4(GenericEmitterCallbackData *data, int params[5]);
BOOL ov07_022217B0(OverlayManager *appMan, int *state);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

void ov07_022208B4(SPLEmitter *emitter, GenericEmitterCallbackData *data);

void ov07_022215B4(SPLEmitter *emitter);

void ov07_022215B4(SPLEmitter *emitter) {
    BattleAnimSystem *system = sub_02015504();
    GenericEmitterCallbackData *data = Heap_Alloc(ov07_0221BFD0(system), sizeof(GenericEmitterCallbackData));

    data->battleAnimSys = system;
    data->particleSys = ov07_0221C478(data->battleAnimSys);

    ov07_0221F9A8(data->battleAnimSys, data->params, GENERIC_EMITTER_CALLBACK_PARAM_COUNT);

    switch (data->params[GENERIC_EMITTER_CALLBACK_PARAM_TARGET_MODE]) {
    case 0:
    default:
        data->startBattler = ov07_0221C468(data->battleAnimSys);
        data->endBattler = ov07_0221C470(data->battleAnimSys);
        break;
    case 1:
        data->startBattler = ov07_0221C470(data->battleAnimSys);
        data->endBattler = ov07_0221C468(data->battleAnimSys);
        break;
    case 2:
        data->startBattler = ov07_0221C468(data->battleAnimSys);
        data->endBattler = ov07_0221C470(data->battleAnimSys);
        break;
    case 3:
        data->startBattler = ov07_0221C470(data->battleAnimSys);
        data->endBattler = ov07_0221C468(data->battleAnimSys);
        break;
    case 4:
        data->startBattler = ov07_0221C468(data->battleAnimSys);
        data->endBattler = ov07_0221C470(data->battleAnimSys);
        break;
    }

    ov07_022208B4(emitter, data);
    Heap_Free(data);
}

s8 ov07_02221664(GenericEmitterCallbackData *data) {
    s8 dir = 1;
    int startType = ov07_02231924(data->battleAnimSys, data->startBattler);
    (void)(ov07_02231924(data->battleAnimSys, data->endBattler));

    switch (startType) {
    case 0:
    default:
        break;
    case 1:
        dir = -1;
        break;
    case 2:
        break;
    case 3:
        dir = -1;
        break;
    case 4:
        break;
    case 5:
        dir = -1;
        break;
    }

    return dir;
}

void ov07_022216A8(GenericEmitterCallbackData *data, int battler, VecFx32 *pos) {
    VecFx32 offset = { 0, 0, 0 };
    int params[4];
    int paramsUnused[4];

    for (int battlerType = 0; battlerType < 6; battlerType++) {
        if (battlerType == ov07_02231924(data->battleAnimSys, battler)) {
            ov07_0221F9A8(data->battleAnimSys, params, 4);
        } else {
            ov07_0221F9A8(data->battleAnimSys, paramsUnused, 4);
        }
    }

    data->dir = ov07_02221664(data);
    VEC_Set(
        &offset,
        params[1],
        params[2],
        params[3]);

    if (params[0] == 1) {
        data->dir = 1;
    }

    offset.x *= data->dir;
    offset.y *= data->dir;
    offset.z *= data->dir;

    pos->x += offset.x;
    pos->y += offset.y;
    pos->z += offset.z;
}

void ov07_02221734(GenericEmitterCallbackData *data, VecFx32 *pos) {
    int params[4];
    VecFx32 offset = { 0, 0, 0 };

    ov07_0221F9A8(data->battleAnimSys, params, 4);
    data->dir = ov07_02221664(data);
    VEC_Set(
        &offset,
        params[1],
        params[2],
        params[3]);

    if (params[0] == 1) {
        data->dir = 1;
    }

    offset.x *= data->dir;
    offset.y *= data->dir;
    offset.z *= data->dir;

    pos->x += offset.x;
    pos->y += offset.y;
    pos->z += offset.z;
}

void ov07_022217A4(GenericEmitterCallbackData *data, int params[5]) {
    ov07_0221F9A8(data->battleAnimSys, params, 5);
}

BOOL ov07_022217B0(OverlayManager *appMan, int *state) {
    return 1;
}
