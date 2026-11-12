// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/sta_act_effect.h: the lines in
// use so far.
#ifndef BWPORT_APP_MUSICAL_STA_ACT_EFFECT_H
#define BWPORT_APP_MUSICAL_STA_ACT_EFFECT_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/particle.h"
#include "nitro/fx/fx.h"
#include "spl/spl.h"
#include "struct_decls.h"
#include "work_area.h"

#define STA_ACT_EFFECT_MAX 8
#define STA_ACT_EFFECT_EMITTER_MAX 4

typedef struct {
    SPLEmitter *emitter;
    VecFx32 pos;
} StaActEffectEmitter;

struct StaActEffect {
    BOOL active;
    // the SPL manager's work, whose emitters and particles hold pointers
    u8 ptcWork[WORK_AREA_SIZE(0x4800)];
    ParticleSystem *ptc;
    StaActEffectEmitter emitters[STA_ACT_EFFECT_EMITTER_MAX];
};

struct StaActEffectSys {
    HeapID heapId;
    StaActEffect effects[STA_ACT_EFFECT_MAX];
};

// pokebw2's StaActEffect_InitSystem
StaActEffectSys *func_ov132_021f9e50(HeapID heapId);
// pokebw2's StaActEffect_TermSystem
void func_ov132_021f9e94(StaActEffectSys *sys);
// pokebw2's StaActEffect_DelEffect
void func_ov132_021f9f58(StaActEffectSys *sys, StaActEffect *effect);
// pokebw2's StaActEffect_CreateEmitter
void func_ov132_021f9f74(StaActEffect *effect, u16 emitterNo, VecFx32 *pos);
// pokebw2's StaActEffect_DeleteEmitter
void func_ov132_021f9fb4(StaActEffect *effect, u16 emitterNo);
// pokebw2's StaActEffect_SetPosition
void func_ov132_021f9fd0(StaActEffect *effect, u16 emitterNo, VecFx32 *pos);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's StaActEffect_UpdateSystem
void func_ov132_021f9ecc(StaActEffectSys *sys);
// pokebw2's StaActEffect_DrawSystem
void func_ov132_021f9ed4(StaActEffectSys *sys);
// pokebw2's StaActEffect_AddEffect
// Adds an effect with the particle resource of ARCID_MUSICAL's file fileId
StaActEffect *func_ov132_021f9edc(StaActEffectSys *sys, u32 fileId);

#endif // BWPORT_APP_MUSICAL_STA_ACT_EFFECT_H
