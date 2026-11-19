#include "global.h"

#include "overlay_07_battle_anim_funcs.h"
#include "unk_02014DA0.h"
#include "overlay_07_battle_anim_system.h"

s32 ov07_0222050C(void *, SPLEmitter *, s32, s32, s32, s32);

void ov07_022207D0(void *arg0, s32 arg1, s32 arg2);

void ov07_022207D0(void *arg0, s32 arg1, s32 arg2) {
    s32 v0;
    void *v1;

    v1 = sub_02015504();
    v0 = ov07_0221C468(v1);
    ov07_0222050C(v1, arg0, v0, ov07_0221C470(v1), arg1, arg2);
}
