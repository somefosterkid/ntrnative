#include "global.h"
#include "overlay_27_ov27task.h"
struct Ov27Task;

s32 ov27_0225A594(Ov27Task *arg0);

s32 ov27_0225A61C(Ov27Task *, s32);

void ov27_0225A66C(Ov27Task *arg0);

void ov27_0225A66C(Ov27Task *arg0) {
    s32 v0;

    v0 = ov27_0225A594(arg0);
    if (arg0->unk_510 != v0) {
        ov27_0225A61C(arg0, v0);
        arg0->unk_510 = v0;
    }
}
