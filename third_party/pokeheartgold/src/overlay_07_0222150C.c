#include "global.h"
struct GenericEmitterCallbackData;
#include "library/spl_emitter.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "particle_system.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_generic_emitter.h"

s32 ov07_02221C74(s32, void *, void *);

s32 ov07_02221C8C(s32);

s32 ov07_02221CB4(void);

void ov07_0222150C(void *arg0, struct GenericEmitterCallbackData *arg1);

struct BattleAnimSystem;
struct ParticleSystem;
void ov07_02221EEC(int funcID, SPLEmitter *emitter, GenericEmitterCallbackData *data);
void ov07_02221540(SPLEmitter *emitter, GenericEmitterCallbackData *data);

void ov07_0222150C(void *arg0, GenericEmitterCallbackData *arg1) {
    s32 v0;
    s32 v1;
    s32 v2;

    v0 = ov07_02221CB4();
    v2 = 0;
    if (v0 > 0) {
        do {
            v1 = ov07_02221C8C(v2);
            if (v1 == (*((s32 *)&arg1->params[4]) & v1)) {
                ov07_02221C74(v2, arg0, arg1);
            }
            v2 += 1;
        } while (v2 < v0);
    }
}

void ov07_02221540(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    ov07_02221EEC(data->params[GENERIC_EMITTER_CALLBACK_PARAM_CAMERA], emitter, data);
}
