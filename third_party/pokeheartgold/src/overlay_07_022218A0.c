#include "global.h"

#include "library/spl_emitter.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "overlay_manager.h"
#include "particle_system.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_generic_emitter.h"

struct BattleAnimSystem;

struct ParticleSystem;

void sub_02015628(SPLEmitter *emitter, VecFx32 *target);

void sub_02015640(SPLEmitter *emitter, VecFx32 *target);

s8 ov07_02221664(GenericEmitterCallbackData *data);

void ov07_02231B90(BattleAnimSystem *system, int battler, VecFx32 *pos);

extern void ov07_022217A4(GenericEmitterCallbackData *data, int params[]);

BOOL ov07_022218A0(SPLEmitter *emitter, GenericEmitterCallbackData *data);

struct OverlayManager;
BOOL ov07_02221AD4(OverlayManager *appMan, int *state);
BOOL ov07_02221AD8(OverlayManager *appMan, int *state);
BOOL ov07_02221ADC(OverlayManager *appMan, int *state);
BOOL ov07_02221AE0(OverlayManager *appMan, int *state);

void sub_02015708(SPLEmitter *emitter, VecFx32 *target);
void sub_02015720(SPLEmitter *emitter, VecFx32 *target);
BOOL ov07_02221AE4(SPLEmitter *emitter, GenericEmitterCallbackData *data);
void sub_02015754(SPLEmitter *emitter, fx16 *force);
void sub_0201576C(SPLEmitter *emitter, fx16 *force);
BOOL ov07_02221BF0(SPLEmitter *emitter, GenericEmitterCallbackData *data);

BOOL ov07_022218A0(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    int params[5];
    VecFx32 pos = { 0, 0, 0 };

    ov07_022217A4(data, params);
    data->dir = ov07_02221664(data);

    if (params[1] == 1) {
        data->dir = 1;
    }

    switch (params[0]) {
    case 0:
        sub_02015640(emitter, &pos);
        pos.x *= data->dir;
        pos.y *= data->dir;
        pos.z *= data->dir;
        break;
    case 1:
        VEC_Set(&pos, params[2], params[3], params[4]);
        pos.x *= data->dir;
        pos.y *= data->dir;
        pos.z *= data->dir;
        break;
    case 2:
        ov07_02231B90(data->battleAnimSys, data->startBattler, &pos);
        break;
    case 3:
        ov07_02231B90(data->battleAnimSys, data->endBattler, &pos);
        break;
    case 0xFF:
        break;
    }

    pos.x -= data->position.x;
    pos.y -= data->position.y;

    sub_02015628(emitter, &pos);
    return 1;
}

void sub_02015674(SPLEmitter *emitter, fx16 *force);

void sub_0201568C(SPLEmitter *emitter, fx16 *force);

BOOL ov07_02221968(SPLEmitter *emitter, GenericEmitterCallbackData *data);

BOOL ov07_02221968(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    int params[5];
    fx16 force = 0;

    ov07_022217A4(data, params);
    data->dir = ov07_02221664(data);

    if (params[1] == 1) {
        data->dir = 1;
    }

    switch (params[0]) {
    case 0:
        sub_0201568C(emitter, &force);
        force *= data->dir;
        break;
    case 1:
        force = params[2];
        force *= data->dir;
        break;
    case 2:
    case 3:
    case 0xFF:
        break;
    }

    sub_02015674(emitter, &force);
    return 1;
}

void sub_020156A8(SPLEmitter *emitter, u16 *angle);

void sub_020156BC(SPLEmitter *emitter, u16 *angle);

BOOL ov07_022219EC(SPLEmitter *emitter, GenericEmitterCallbackData *data);

BOOL ov07_022219EC(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    int params[5];
    u16 angle = 0;

    ov07_022217A4(data, params);
    data->dir = ov07_02221664(data);

    if (params[1] == 1) {
        data->dir = 1;
    }

    switch (params[0]) {
    case 0:
        sub_020156BC(emitter, &angle);
        angle *= data->dir;
        break;
    case 1:
        angle = params[2];
        angle *= data->dir;
        break;
    case 2:
    case 3:
    case 0xFF:
        break;
    }

    sub_020156A8(emitter, &angle);
    return 1;
}

void sub_020156D8(SPLEmitter *emitter, u16 *axis);

void sub_020156EC(SPLEmitter *emitter, u16 *axis);

BOOL ov07_02221A6C(SPLEmitter *emitter, GenericEmitterCallbackData *data);

BOOL ov07_02221A6C(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    int params[5];
    u16 axis = 0;

    ov07_022217A4(data, params);
    data->dir = ov07_02221664(data);

    if (params[1] == 1) {
        data->dir = 1;
    }

    switch (params[0]) {
    case 0:
        sub_020156EC(emitter, &axis);
        break;
    case 1:
        axis = params[2];
        break;
    case 2:
    case 3:
    case 0xFF:
        break;
    }

    sub_020156D8(emitter, &axis);
    return 1;
}

BOOL ov07_02221AD4(OverlayManager *appMan, int *state) {
    return 1;
}

BOOL ov07_02221AD8(OverlayManager *appMan, int *state) {
    return 1;
}

BOOL ov07_02221ADC(OverlayManager *appMan, int *state) {
    return 1;
}

BOOL ov07_02221AE0(OverlayManager *appMan, int *state) {
    return 1;
}

BOOL ov07_02221AE4(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    int params[5];
    VecFx32 target = { 0, 0, 0 };

    ov07_022217A4(data, params);
    data->dir = ov07_02221664(data);

    if (params[1] == 1) {
        data->dir = 1;
    }

    switch (params[0]) {
    case 0:
        sub_02015720(emitter, &target);
        target.x *= data->dir;
        target.y *= data->dir;
        target.z *= data->dir;
        break;
    case 1:
        VEC_Set(
            &target,
            params[2],
            params[3],
            params[4]);
        target.x *= data->dir;
        target.y *= data->dir;
        target.z *= data->dir;
        break;
    case 4:
        ov07_02231B90(data->battleAnimSys, data->endBattler, &target);
        target.x = (target.x * params[2]) / params[3];
        target.y = (target.y * params[2]) / params[3];
        target.z = (target.z * params[2]) / params[3];
        break;
    case 2:
        ov07_02231B90(data->battleAnimSys, data->startBattler, &target);
        break;
    case 3:
        ov07_02231B90(data->battleAnimSys, data->endBattler, &target);
        break;
    case 0xFF:
        break;
    }

    target.x -= data->position.x;
    target.y -= data->position.y;
    target.z -= data->position.z;

    sub_02015708(emitter, &target);
    return 1;
}

BOOL ov07_02221BF0(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    int params[5];
    fx16 force = 0;

    ov07_022217A4(data, params);
    data->dir = ov07_02221664(data);

    if (params[1] == 1) {
        data->dir = 1;
    }

    switch (params[0]) {
    case 0:
        sub_0201576C(emitter, &force);
        force *= data->dir;
        break;
    case 1:
        force = params[2];
        force *= data->dir;
        break;
    case 2:
    case 3:
    case 0xFF:
        break;
    }

    sub_02015754(emitter, &force);
    return 1;
}
