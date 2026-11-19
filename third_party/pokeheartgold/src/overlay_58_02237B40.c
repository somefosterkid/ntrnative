#include "global.h"
#include "overlay_58.h"
#include "overlay_manager.h"

void ov58_02237B40(Ov58ApricornBoxWork *param_1);

void ov58_02237B94(Ov58ApricornBoxWork *work);
extern OverlayManagerTemplate _02237D1C;
s32 ov58_02237B98(Ov58ApricornBoxWork *parentWork);

void ov58_02237B40(Ov58ApricornBoxWork *param_1) {
    u16 *puVar1;

    puVar1 = param_1->args->unk8;
    if (puVar1 != NULL) {
        *puVar1 = 0;
    }
    switch (param_1->args->unk0) {
    case 0:
        param_1->unk_18 = 0;
        break;
    case 1:
        param_1->unk_18 = 1;
        break;
    case 2:
        param_1->unk_18 = 2;
        break;
    case 3:
        param_1->unk_19 = 1;
        param_1->unk_18 = 0;
        param_1->unk_14 = param_1->args->unkC;
        return;
    }
    param_1->unk_19 = 0;
    param_1->unk_14 = 8;
    return;
}

void ov58_02237B94(Ov58ApricornBoxWork *work) {
}

s32 ov58_02237B98(Ov58ApricornBoxWork *parentWork) {
    parentWork->childMan = OverlayManager_New(&_02237D1C, parentWork, parentWork->heapId);
    return 2;
}
