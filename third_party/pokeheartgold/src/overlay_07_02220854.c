#include "global.h"

#include "library/spl_emitter.h"
#include "overlay_07_battle_anim_funcs.h"
#include "unk_02014DA0.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_generic_emitter.h"

void ov07_022207D0(SPLEmitter *emitter, int behaviorMode, int targetMode);

void ov07_02220854(SPLEmitter *emitter);
void ov07_02220860(SPLEmitter *emitter);
void ov07_0222086C(SPLEmitter *emitter);
void ov07_02220878(SPLEmitter *emitter);

s32 ov07_0222050C(void *, SPLEmitter *, s32, s32, s32, s32);
void ov07_02220884(void *arg0);

void ov07_02220978(SPLEmitter *emitter, GenericEmitterCallbackData *data);
void ov07_02220CAC(SPLEmitter *emitter, GenericEmitterCallbackData *data);
void ov07_0222150C(void *emitter, GenericEmitterCallbackData *data);
void ov07_02221540(SPLEmitter *emitter, GenericEmitterCallbackData *data);
void ov07_02221550(SPLEmitter *emitter, GenericEmitterCallbackData *data);
void ov07_022208B4(SPLEmitter *emitter, GenericEmitterCallbackData *data);

void ov07_02220854(SPLEmitter *emitter) {
    ov07_022207D0(emitter, 2, 2);
}

void ov07_02220860(SPLEmitter *emitter) {
    ov07_022207D0(emitter, 2, 3);
}

void ov07_0222086C(SPLEmitter *emitter) {
    ov07_022207D0(emitter, 2, 4);
}

void ov07_02220878(SPLEmitter *emitter) {
    ov07_022207D0(emitter, 2, 5);
}

void ov07_02220884(void *arg0) {
    s32 v1;
    void *v0;

    v0 = sub_02015504();
    v1 = ov07_0221C468(v0);
    ov07_0222050C(v0, arg0, ov07_0221C470(v0), v1, 0, 0);
}

void ov07_022208B4(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    if (data->params[GENERIC_EMITTER_CALLBACK_PARAM_POSITION] != 0) {
        ov07_02220978(emitter, data);
    }
    if (data->params[GENERIC_EMITTER_CALLBACK_PARAM_AXIS] != 0) {
        ov07_02220CAC(emitter, data);
    }
    if (data->params[GENERIC_EMITTER_CALLBACK_PARAM_BEHAVIOR] != 0) {
        ov07_0222150C(emitter, data);
    }
    ov07_02221540(emitter, data);
    if (data->params[GENERIC_EMITTER_CALLBACK_PARAM_PRIORITY] != 0) {
        ov07_02221550(emitter, data);
    }
    return;
}
