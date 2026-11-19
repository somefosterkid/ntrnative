#include "global.h"
struct Ov27Task;

#include "error_handling.h"
#include "screen_fade.h"
#include "bg_window.h"
#include "overlay_27_ov27task.h"

u32 ov27_0225A7B0(Ov27Task *arg0);

void ov27_0225A7DC(Ov27Task *arg0);

void ov27_0225B404(Ov27Task *arg0);

s32 ov27_0225B4D8(Ov27Task *);

void ov27_0225A7FC(Ov27Task *arg0);

u32 ov27_0225A594(Ov27Task *);
void ov27_0225A61C(Ov27Task *, u32);
void ov27_0225A690(Ov27Task *, u32);
void ov27_0225B398(Ov27Task *, u32);
void ov27_0225A86C(Ov27Task *param_1);

void ov27_0225A7FC(Ov27Task *arg0) {
    u32 v1;
    FieldSystem *v0;
    FieldSystem *v2;

    v0 = arg0->fieldSystem;
    v1 = v0->unkD2_0;
    switch (v1) {
    case 0:
        ov27_0225A7B0(arg0);
        return;
    case 1:
        ov27_0225A7DC(arg0);
        v2 = arg0->fieldSystem;
        v2->unkD2_0 = 2;
        return;
    case 2:
        if ((v0->runningFieldMap != 0) && (IsPaletteFadeFinished() != 0) && (ov27_0225B4D8(arg0) != 0)) {
            ov27_0225B404(arg0);
            return;
        }
        return;
    default:
    case 3:
    case 4:
        GF_AssertFail();
        break;
    }
}

void ov27_0225A86C(Ov27Task *param_1) {
    u32 uVar1;

    uVar1 = ov27_0225A594(param_1);
    ov27_0225A61C(param_1, uVar1);
    ov27_0225B398(param_1, 0xffffffff);
    CopyWindowToVram(&param_1->unk_3D0);
    ov27_0225A690(param_1, 1);
    return;
}
