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

void sub_0201560C(SPLEmitter *emitter, VecFx16 *maginutde);

s8 ov07_02221664(GenericEmitterCallbackData *data);

extern void ov07_022217A4(GenericEmitterCallbackData *data, int params[]);

BOOL ov07_022217B4(SPLEmitter *emitter, GenericEmitterCallbackData *data);

struct OverlayManager;
BOOL ov07_02221898(OverlayManager *appMan, int *state);
BOOL ov07_0222189C(OverlayManager *appMan, int *state);

BOOL ov07_022217B4(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    int params[5];
    VecFx16 mag = { 0, 0, 0 };

    ov07_022217A4(data, params);
    data->dir = ov07_02221664(data);

    if (params[1] == 1) {
        data->dir = 1;
    }

    switch (params[0]) {
    case 0:
        sub_0201560C(emitter, &mag);
        mag.x *= data->dir;
        mag.y *= data->dir;
        mag.z *= data->dir;
        break;
    case 1:
        VEC_Fx16Set(&mag, params[2], params[3], params[4]);

        mag.x *= data->dir;
        mag.y *= data->dir;
        mag.z *= data->dir;

        sub_0201560C(emitter, &mag);
        return 1;
    case 2:
    case 3:
    case 0xFF:
        break;
    }

    mag.x -= data->position.x;
    mag.y -= data->position.y;
    mag.z -= data->position.z;

    sub_0201560C(emitter, &mag);
    return 1;
}

BOOL ov07_02221898(OverlayManager *appMan, int *state) {
    return 1;
}

BOOL ov07_0222189C(OverlayManager *appMan, int *state) {
    return 1;
}
