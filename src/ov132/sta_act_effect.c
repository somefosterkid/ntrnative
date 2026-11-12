// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov209/sta_act_effect.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/sta_act_effect.h"
#include "constants/arc.h"
#include "gfl/heap.h"
#include "gfl/particle.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx_vec.h"

// Overlay 209's sta_act_effect.c: the stage's particle effects

// pokebw2's StaActEffect_InitSystem
StaActEffectSys *func_ov132_021f9e50(HeapID heapId) {
    u8 i;
    StaActEffectSys *sys = GFL_HeapAllocate(heapId, sizeof(StaActEffectSys), FALSE, "sta_act_effect.c", 76);

    sys->heapId = heapId;
    func_02050b80(heapId);
    for (i = 0; i < STA_ACT_EFFECT_MAX; i++) {
        sys->effects[i].active = FALSE;
    }
    return sys;
}

// pokebw2's StaActEffect_TermSystem
void func_ov132_021f9e94(StaActEffectSys *sys) {
    u8 i;

    for (i = 0; i < STA_ACT_EFFECT_MAX; i++) {
        if (sys->effects[i].active == TRUE) {
            func_ov132_021f9f58(sys, &sys->effects[i]);
        }
    }
    func_02050ed4();
    GFL_HeapFree(sys);
}

// pokebw2's StaActEffect_UpdateSystem
void func_ov132_021f9ecc(StaActEffectSys *sys) {
    func_02051574();
}

// pokebw2's StaActEffect_DrawSystem
void func_ov132_021f9ed4(StaActEffectSys *sys) {
    func_02051534();
}

// pokebw2's StaActEffect_AddEffect
StaActEffect *func_ov132_021f9edc(StaActEffectSys *sys, u32 fileId) {
    u8 i;
    StaActEffect *effect = NULL;
    void *res;

    for (i = 0; i < STA_ACT_EFFECT_MAX; i++) {
        if (sys->effects[i].active == FALSE) {
            effect = &sys->effects[i];
        }
    }
    effect->ptc = func_02050c04(effect->ptcWork, sizeof(effect->ptcWork), FALSE, sys->heapId);
    res = func_0205121c(0x2F, fileId, sys->heapId);
    func_02051230(effect->ptc, res, FALSE, GFL_VBlankGetTCBMgr());
    for (i = 0; i < STA_ACT_EFFECT_EMITTER_MAX; i++) {
        effect->emitters[i].emitter = NULL;
    }
    effect->active = TRUE;
    return effect;
}

// pokebw2's StaActEffect_DelEffect
void func_ov132_021f9f58(StaActEffectSys *sys, StaActEffect *effect) {
    effect->active = FALSE;
    func_02051620(effect->ptc);
    func_02050db8(effect->ptc);
}

// pokebw2's StaActEffect_CreateEmitter
void func_ov132_021f9f74(StaActEffect *effect, u16 emitterNo, VecFx32 *pos) {
    SPLEmitter *emitter = func_020515b4(effect->ptc, emitterNo, pos);

    if (emitter == NULL || emitterNo < STA_ACT_EFFECT_EMITTER_MAX) {
        effect->emitters[emitterNo].emitter = emitter;
        VEC_Set(&effect->emitters[emitterNo].pos, pos->x, pos->y, pos->z);
    }
}

// pokebw2's StaActEffect_DeleteEmitter
void func_ov132_021f9fb4(StaActEffect *effect, u16 emitterNo) {
    if (effect->emitters[emitterNo].emitter != NULL) {
        func_02051630(effect->ptc, effect->emitters[emitterNo].emitter);
    }
}

// pokebw2's StaActEffect_SetPosition
void func_ov132_021f9fd0(StaActEffect *effect, u16 emitterNo, VecFx32 *pos) {
    if (effect->emitters[emitterNo].emitter != NULL) {
        func_02051838(effect->emitters[emitterNo].emitter, pos);
    }
}
