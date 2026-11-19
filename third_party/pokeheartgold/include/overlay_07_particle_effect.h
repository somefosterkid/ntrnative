#ifndef POKEHEARTGOLD_OVERLAY_07_PARTICLE_EFFECT_H
#define POKEHEARTGOLD_OVERLAY_07_PARTICLE_EFFECT_H

#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"

#include "particle_system.h"
#include "sys_task.h"

// the battle's particle effects (overlay 7): a context of 16 effect slots (ov07_0223458C) and the effect records
// (ov07_02234658); DS sizes 0x48 and 0x24

typedef struct UnkStruct_ov12_02237EFC {
    int unk_00;
} UnkStruct_ov12_02237EFC;

typedef struct ParticleEffectTemplate {
    int resourceID;
    int cameraProjection;
} ParticleEffectTemplate;

typedef struct UnkStruct_ov12_02238004_t {
    int unk_00;                                     // 0x00: the slot index in the context
    enum HeapID heapID;                             // 0x04
    ParticleEffectTemplate particleEffectTemplate;  // 0x08
    ParticleSystem *unk_10;                         // 0x10
    SPLEmitter *unk_14;                             // 0x14
    SysTask *unk_18;                                // 0x18: the run task (ov07_022346C0)
    int unk_1C;                                     // 0x1C: the emitter count (ov07_02234718)
    BOOL unk_20;                                    // 0x20: active
} UnkStruct_ov12_02238004; // DS size 0x24

typedef struct UnkStruct_ov12_02237F98_t {
    enum HeapID heapID;                    // 0x00
    UnkStruct_ov12_02237EFC unk_04;        // 0x04
    UnkStruct_ov12_02238004 *unk_08[16];   // 0x08
} UnkStruct_ov12_02237F98; // DS size 0x48

UnkStruct_ov12_02237F98 *ov07_0223458C(enum HeapID heapID, UnkStruct_ov12_02237EFC *param1);
UnkStruct_ov12_02238004 *ov07_022345C8(UnkStruct_ov12_02237F98 *param0, ParticleEffectTemplate *particleEffectTemplate);
void ov07_02234604(UnkStruct_ov12_02237F98 *param0);
void ov07_02234628(UnkStruct_ov12_02237F98 *param0);
UnkStruct_ov12_02238004 *ov07_02234658(enum HeapID heapID, ParticleEffectTemplate *param1);
void ov07_02234694(UnkStruct_ov12_02238004 *param0);
BOOL ov07_022346BC(UnkStruct_ov12_02238004 *param0);
void ov07_022346C0(SysTask *task, void *data);
int ov07_02234718(int param0);

#endif // POKEHEARTGOLD_OVERLAY_07_PARTICLE_EFFECT_H
