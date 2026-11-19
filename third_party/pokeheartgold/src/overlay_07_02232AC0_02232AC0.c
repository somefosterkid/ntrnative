#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_07_ball_capsule_seal_effect.h"
#include "sys_task_api.h"
#include "unk_02014DA0.h"

static void ov07_02232AC0(SysTask *arg0, void *arg1);

static void ov07_02232AC0(SysTask *task, void *arg1) {
    BallCapsuleSealTask *data = arg1;

    if (data->delay == 0) {
        *data->activeFlag = 1;
        sub_02015494(data->particleSystem, 0, data->callback, data);
        sub_02015528(data->particleSystem, 1);
        SysTask_Destroy(task);
        Heap_Free(data);
        return;
    }
    data->delay--;
}

s32 ov07_02232608(s32);

s32 ov07_02234530(SEAL *);

void ov07_02232C64(SPLEmitter *);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov07_02232AFC(BallCapsuleSealEffect *arg0, void (*arg1)(SPLEmitter *));
#else
void ov07_02232AFC(void *arg0, void (*arg1)(SPLEmitter *));
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov07_02232AFC(BallCapsuleSealEffect *sealEffect, void (*fp_callback)(SPLEmitter *)) {
    s32 v1;
    s32 res_no;
    s32 v4;
    BallCapsuleSealTask *data;

    if (sealEffect->hasCapsule == 0) {
        v1 = ov07_02232608(sealEffect->ballType);
        for (res_no = 0; res_no < v1; res_no++) {
            sub_02015494(sealEffect->sealParticleSystems[0], res_no, fp_callback, sealEffect);
        }
        sealEffect->sealActiveFlags[0] = 1;
        sub_02015528(sealEffect->sealParticleSystems[0], 1);
        return;
    }
    for (v4 = 0; v4 < sealEffect->sealCount; v4++) {
        data = Heap_Alloc(sealEffect->heapID, sizeof(BallCapsuleSealTask));
        if (data == NULL) {
            GF_AssertFail();
        }
        data->sealIndex = v4;
        data->battlerType = sealEffect->ballCapCfg.battlerType;
        data->callback = ov07_02232C64;
        sealEffect->sealActiveFlags[v4] = 0;
        data->activeFlag = &sealEffect->sealActiveFlags[v4];
        data->particleSystem = sealEffect->sealParticleSystems[v4];
        data->seal = sealEffect->seals[v4];
        if (data->seal == NULL) {
            GF_AssertFail();
        }
        data->delay = ov07_02234530(data->seal);
        SysTask_CreateOnMainQueue(ov07_02232AC0, data, 0x3E9U);
    }
}
#else
void ov07_02232AFC(void *arg, void (*fp_callback)(SPLEmitter *)) {
    void *v0;
    s32 v1;
    s32 v2;
    s32 res_no;
    s32 v4;
    BallCapsuleSealTask *data;
    void *v3;

    if (*(s32 *)((u8 *)arg + 0x94) == 0) {
        v1 = ov07_02232608(*(s32 *)((u8 *)arg + 0x98));
        res_no = 0;
        if (v1 > 0) {
            do {
                sub_02015494(*(ParticleSystem **)((u8 *)arg + 0x14), res_no, fp_callback, arg);
                res_no += 1;
            } while (res_no < v1);
        }
        *(s32 *)((u8 *)arg + 0x5C) = 1;
        sub_02015528(*(ParticleSystem **)((u8 *)arg + 0x14), 1);
        return;
    }
    v4 = 0;
    if ((s32) * (s32 *)((u8 *)arg + 0x10) > 0) {
        v0 = arg;
        v3 = arg;
        v0 = arg + 0x5C;
        do {
            data = Heap_Alloc(*(enum HeapID *)arg, sizeof(BallCapsuleSealTask));
            if (data == NULL) {
                GF_AssertFail();
            }
            data->sealIndex = v4;
            data->battlerType = *(s32 *)((u8 *)arg + 4);
            data->callback = ov07_02232C64;
            *(s32 *)((u8 *)v3 + 0x5C) = 0;
            data->activeFlag = v0;
            data->particleSystem = *(ParticleSystem **)((u8 *)v3 + 0x14);
            v2 = *(s32 *)((u8 *)v3 + 0x38);
            data->seal = (SEAL *)v2;
            if (v2 == 0) {
                GF_AssertFail();
            }
            data->delay = ov07_02234530(data->seal);
            SysTask_CreateOnMainQueue(ov07_02232AC0, data, 0x3E9U);
            v4 += 1;
            v0 += 4;
            v3 += 4;
        } while (v4 < (s32) * (s32 *)((u8 *)arg + 0x10));
    }
}
#endif
