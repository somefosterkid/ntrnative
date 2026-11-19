#include "global.h"

#include "library/spl_emitter.h"
#include "overlay_07_battle_anim_system.h"

struct BattleAnimSystem;

void sub_02015708(SPLEmitter *emitter, VecFx32 *target);

void sub_02015720(SPLEmitter *emitter, VecFx32 *target);

extern BOOL ov07_0221F9A8(BattleAnimSystem *system, int params[], int count);

void ov07_02231B90(BattleAnimSystem *system, int battler, VecFx32 *pos);

void ov07_022202D8(int mode, s8 sign, VecFx32 *vec);

void ov07_0222036C(BattleAnimSystem *system, SPLEmitter *emitter, int attacker, int defender, int mode, s8 sign, VecFx32 *emitterPos);

void sub_02015628(SPLEmitter *emitter, VecFx32 *target);
void sub_02015640(SPLEmitter *emitter, VecFx32 *target);
void ov07_0222043C(BattleAnimSystem *system, SPLEmitter *emitter, int attacker, int defender, int mode, s8 sign, VecFx32 *emitterPos);

void ov07_0222036C(BattleAnimSystem *system, SPLEmitter *emitter, int attacker, int defender, int mode, s8 sign, VecFx32 *emitterPos) {
    VecFx32 target = { 0, 0, 0 };

    switch (mode) {
    case 0:
        break;
    case 1:
        sub_02015720(emitter, &target);
        target.x *= sign;
        target.y *= sign;
        target.z *= sign;
        break;
    case 2:
        target.x = ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) * sign;
        target.y = ((fx32)(((1 / 1.28) > 0) ? ((1 / 1.28) * (1 << 12) + 0.5f) : ((1 / 1.28) * (1 << 12) - 0.5f))) * sign;
        target.z = ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) * sign;
        break;
    case 3:
        ov07_02231B90(system, defender, &target);
        break;
    case 4:
        ov07_02231B90(system, attacker, &target);
        break;
    case 5: {
        int params[4] = { 0, 0, 0, 0 };

        ov07_0221F9A8(system, params, (sizeof(params) / sizeof(*(params))));
        VEC_Set(&target, params[0], params[1], params[2]);
        ov07_022202D8(params[3], sign, &target);
    } break;
    }

    target.x -= emitterPos->x;
    target.y -= emitterPos->y;
    target.z -= emitterPos->z;

    sub_02015708(emitter, &target);
}

void ov07_0222043C(BattleAnimSystem *system, SPLEmitter *emitter, int attacker, int defender, int mode, s8 sign, VecFx32 *emitterPos) {
    VecFx32 target = { 0, 0, 0 };

    switch (mode) {
    case 0:
        break;
    case 1:
        sub_02015640(emitter, &target);
        target.x *= sign;
        target.y *= sign;
        target.z *= sign;
        break;
    case 2:
        target.x = ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) * sign;
        target.y = ((fx32)(((1 / 1.28) > 0) ? ((1 / 1.28) * (1 << 12) + 0.5f) : ((1 / 1.28) * (1 << 12) - 0.5f))) * sign;
        target.z = ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) * sign;
        break;
    case 3:
        ov07_02231B90(system, defender, &target);
        break;
    case 4:
        ov07_02231B90(system, attacker, &target);
        break;
    case 5: {
        int params[4] = { 0, 0, 0, 0 };

        ov07_0221F9A8(system, params, (sizeof(params) / sizeof(*(params))));
        VEC_Set(&target, params[0], params[1], params[2]);
        ov07_022202D8(params[3], sign, &target);
    } break;
    }

    target.x -= emitterPos->x;
    target.y -= emitterPos->y;
    target.z -= emitterPos->z;

    sub_02015628(emitter, &target);
}
