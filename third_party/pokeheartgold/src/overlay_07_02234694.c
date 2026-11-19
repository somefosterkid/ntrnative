#include "global.h"

#include "sys_task_api.h"
#include "overlay_07_particle_effect.h"

void ov07_022346E4(UnkStruct_ov12_02238004 *arg0, void (*callback)(SPLEmitter *));

void ov07_02234710(SPLEmitter *emitter);

void ov07_02234694(UnkStruct_ov12_02238004 *arg0) {
    arg0->unk_20 = 1;
    ov07_022346E4(arg0, ov07_02234710);
    arg0->unk_18 = SysTask_CreateOnMainQueue(ov07_022346C0, arg0, 0x3E8U);
}

BOOL ov07_022346BC(UnkStruct_ov12_02238004 *param0) {
    return param0->unk_20;
}
