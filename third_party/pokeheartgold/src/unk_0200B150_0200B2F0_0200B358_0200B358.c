#include "global.h"

#include "error_handling.h"
#include "oam_manager.h"


u16 sub_0200B358(const MtxFx22 *param_1);

u16 sub_0200B358(const MtxFx22 *param_1) {
    u16 uVar1;

    uVar1 = NNS_G2dEntryOamManagerAffine(&_021D0EB0->subScreenOam, param_1);
    if (uVar1 == 0xfffe) {
        GF_AssertFail();
    }
    return uVar1;
}
