#include "global.h"

#include "error_handling.h"

void ov96_021E8988(s32 *param_1, s32 param_2, void *param_3, size_t param_4);

void ov96_021E8988(s32 *param_1, s32 param_2, void *param_3, size_t param_4) {
    size_t sVar1;
    void *pvVar2;
    s32 iVar3;
    s32 *piVar4;
    void *(*pcVar5)();
    s32 *piVar6;

    piVar6 = NULL;
    iVar3 = 0;
    piVar4 = param_1;
    do {
        if (*piVar4 == 0) {
            piVar6 = param_1 + iVar3 * 0xe;
            break;
        }
        iVar3 = iVar3 + 1;
        piVar4 = piVar4 + 0xe;
    } while (iVar3 < 8);
    if (piVar6 == NULL) {
        GF_AssertFail();
    }
    iVar3 = (param_2 + -0x16) * 0xc;
    sVar1 = (**(size_t(**)())(param_1[0x70] + iVar3 + 4))();
    if ((sVar1 != 0xffff) && (sVar1 != param_4)) {
        GF_AssertFail();
    }
    pcVar5 = *(void *(**)())(param_1[0x70] + iVar3 + 8);
    if (pcVar5 != NULL) {
        pvVar2 = (void *)(*pcVar5)(0, param_1[0x72], param_4);
        if (0 < (s32)param_4) {
            memcpy(pvVar2, param_3, param_4);
        }
        piVar6[0xd] = (s32)pvVar2;
    } else {
        piVar6[0xd] = (s32)param_3;
    }
    piVar6[1] = param_2 + -0x16;
    piVar6[0xc] = param_4;
    *piVar6 = 1;
    return;
}
