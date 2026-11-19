#include "global.h"

#include "battle/battler_sprite_context.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_resources_header.h"
#include "overlay_07_battle_anim_system.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "camera.h"
#include "particle_system.h"
#include "overlay_07_bg_scroll_context.h"

struct BattleAnimSystem;

struct Pokepic;

struct PokepicDrawParam;

struct SPLEmitter;

struct SysTask;

struct SysTaskQueue;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

extern BOOL ov07_0221F9A8(BattleAnimSystem *system, int params[], int count);

u8 ov07_02231924(BattleAnimSystem *system, int battler);

void ov07_02231B90(BattleAnimSystem *system, int battler, VecFx32 *pos);

s8 ov07_0221FF34(BattleAnimSystem *system, int startBattler, int endBattler);

void ov07_0222036C(BattleAnimSystem *system, SPLEmitter *emitter, int attacker, int defender, int mode, s8 sign, VecFx32 *emitterPos);

void ov07_0222043C(BattleAnimSystem *system, SPLEmitter *emitter, int attacker, int defender, int mode, s8 sign, VecFx32 *emitterPos);

static inline BOOL BattleParticleUtil_GetExtraParamVec3(BattleAnimSystem *system, VecFx32 *out) {
    return ov07_0221F9A8(system, (int *)out, 3);
}

void ov07_0222050C(BattleAnimSystem *system, SPLEmitter *emitter, int attacker, int defender, int behaviorMode, int targetMode);

void ov07_0222050C(BattleAnimSystem *system, SPLEmitter *emitter, int attacker, int defender, int behaviorMode, int targetMode) {
    VecFx32 pos;
    VecFx16 dir;
    s8 sign = 1;

    ParticleSystem *ps = ov07_0221C478(system);
    int attackerType = ov07_02231924(system, attacker);
    int defenderType = ov07_02231924(system, defender);

    VecFx32 dirPlToEm, dirP1ToE1, dirP1ToP2, dirP1ToE2, dirP2ToP1, dirP2ToE1, dirP2ToE2;

    BattleParticleUtil_GetExtraParamVec3(system, &dirPlToEm);
    BattleParticleUtil_GetExtraParamVec3(system, &dirP1ToE1);
    BattleParticleUtil_GetExtraParamVec3(system, &dirP1ToE2);
    BattleParticleUtil_GetExtraParamVec3(system, &dirP1ToP2);
    BattleParticleUtil_GetExtraParamVec3(system, &dirP2ToE1);
    BattleParticleUtil_GetExtraParamVec3(system, &dirP2ToE2);
    BattleParticleUtil_GetExtraParamVec3(system, &dirP2ToP1);

    switch (attackerType) {
    case 0:
    default:
        VEC_Fx16Set(&dir, dirPlToEm.x, dirPlToEm.y, dirPlToEm.z);
        break;
    case 1:
        VEC_Fx16Set(&dir, -dirPlToEm.x, -dirPlToEm.y, -dirPlToEm.z);
        break;
    case 2:
        if (defenderType == 3) {
            VEC_Fx16Set(&dir, dirP1ToE1.x, dirP1ToE1.y, dirP1ToE1.z);
        } else if (defenderType == 5) {
            VEC_Fx16Set(&dir, dirP1ToE2.x, dirP1ToE2.y, dirP1ToE2.z);
        } else {
            VEC_Fx16Set(&dir, dirP1ToP2.x, dirP1ToP2.y, dirP1ToP2.z);
        }
        break;
    case 3:
        if (defenderType == 2) {
            VEC_Fx16Set(&dir, -dirP1ToE1.x, -dirP1ToE1.y, -dirP1ToE1.z);
        } else if (defenderType == 5) {
            VEC_Fx16Set(&dir, -dirP1ToP2.x, -dirP1ToP2.y, -dirP1ToP2.z);
        } else {
            VEC_Fx16Set(&dir, -dirP2ToE1.x, -dirP2ToE1.y, -dirP2ToE1.z);
        }
        break;
    case 4:
        if (defenderType == 3) {
            VEC_Fx16Set(&dir, dirP2ToE1.x, dirP2ToE1.y, dirP2ToE1.z);
        } else if (defenderType == 5) {
            VEC_Fx16Set(&dir, dirP2ToE2.x, dirP2ToE2.y, dirP2ToE2.z);
        } else {
            VEC_Fx16Set(&dir, dirP2ToP1.x, dirP2ToP1.y, dirP2ToP1.z);
        }
        break;
    case 5:
        if (defenderType == 3) {
            VEC_Fx16Set(&dir, -dirP2ToP1.x, -dirP2ToP1.y, -dirP2ToP1.z);
        } else if (defenderType == 2) {
            VEC_Fx16Set(&dir, -dirP1ToE2.x, -dirP1ToE2.y, -dirP1ToE2.z);
        } else {
            VEC_Fx16Set(&dir, -dirP2ToE2.x, -dirP2ToE2.y, -dirP2ToE2.z);
        }
        break;
    }

    sign = ov07_0221FF34(system, attacker, defender);

    VecFx32 explicitPos = { 0, 0, 0 };
    VecFx32 attackerPos;

    BattleParticleUtil_GetExtraParamVec3(system, &explicitPos);
    ov07_02231B90(system, attacker, &attackerPos);

    if (explicitPos.x == 0 && explicitPos.y == 0 && explicitPos.z == 0) {
        pos = attackerPos;

        SPL_SetEmitterPositionX(emitter, attackerPos.x);
        SPL_SetEmitterPositionY(emitter, attackerPos.y);
        SPL_SetEmitterPositionZ(emitter, attackerPos.z);
    } else {
        VEC_Set(&pos, explicitPos.x, explicitPos.y, explicitPos.z);

        if (explicitPos.x == ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) && explicitPos.y == ((fx32)(((1 / 1.28) > 0) ? ((1 / 1.28) * (1 << 12) + 0.5f) : ((1 / 1.28) * (1 << 12) - 0.5f)))) {
            sign = 1;
        }

        pos.x *= sign;
        pos.y *= sign;
        pos.z = attackerPos.z * sign;

        SPL_SetEmitterPositionX(emitter, pos.x);
        SPL_SetEmitterPositionY(emitter, pos.y);
        SPL_SetEmitterPositionZ(emitter, pos.z);
    }

    switch (behaviorMode) {
    case 0:
        break;
    case 1:
        ov07_0222036C(system, emitter, attacker, defender, targetMode, sign, &pos);
        break;
    case 2:
        ov07_0222043C(system, emitter, attacker, defender, targetMode, sign, &pos);
        break;
    }

    SPL_SetEmitterAxis(emitter, &dir);
}
