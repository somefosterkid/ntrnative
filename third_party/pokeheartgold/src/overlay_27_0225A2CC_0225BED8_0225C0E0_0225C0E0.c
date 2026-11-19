#include "global.h"
#include "overlay_27_ov27task.h"
struct Ov27Task;

void ov27_0225C044(Ov27Task *param_1);

void ov27_0225C06C(Ov27Task *param_1);

void ov27_0225C088(Ov27Task *param_1);

void ov27_0225C0E0(Ov27Task *param_1);

void ov27_0225C0E0(Ov27Task *param_1) {
    u32 uVar1;
    uVar1 = param_1->unk_51C_1;
    if (uVar1 == 1) {
        ov27_0225C044(param_1);
        return;
    }
    if (uVar1 == 2) {
        ov27_0225C088(param_1);
        return;
    }
    if (uVar1 == 3) {
        ov27_0225C06C(param_1);
    }
    return;
}
