#include "global.h"

#include "sys_task_api.h"
#include "overlay_07_particle_effect.h"

// the particle system calls (unk_02014DA0.h declares their first parameter as SPLEmitter *; they take the ParticleSystem)
s32 sub_020154B0(ParticleSystem *particleSystem);
SPLEmitter *sub_02015494(ParticleSystem *particleSystem, s32 resourceID, void (*callback)(SPLEmitter *), void *param);
void sub_02015528(ParticleSystem *particleSystem, int projection);

void ov07_0221FF2C(ParticleSystem *ps);

void ov07_022346E4(UnkStruct_ov12_02238004 *arg, void (*fp_callback)(SPLEmitter *));

void ov07_022346C0(SysTask *task, void *param) {
    UnkStruct_ov12_02238004 *arg1 = param;
    if (sub_020154B0(arg1->unk_10) == 0) {
        arg1->unk_20 = 0;
        ov07_0221FF2C(arg1->unk_10);
        SysTask_Destroy(task);
    }
}

void ov07_022346E4(UnkStruct_ov12_02238004 *arg, void (*fp_callback)(SPLEmitter *)) {
    s32 res_no;

    res_no = 0;
    if ((s32)arg->unk_1C > 0) {
        do {
            sub_02015494(arg->unk_10, res_no, fp_callback, arg);
            res_no += 1;
        } while (res_no < (s32)arg->unk_1C);
    }
    sub_02015528(arg->unk_10, arg->particleEffectTemplate.cameraProjection);
}
