#include "global.h"

#include "error_handling.h"
#include "oam_manager.h"


BOOL sub_0200B310(const GXOamAttr *param_1, u16 param_2, BOOL doubleAffine);

BOOL sub_0200B310(const GXOamAttr *param_1, u16 param_2, BOOL doubleAffine) {
    BOOL BVar1;

    BVar1 = NNS_G2dEntryOamManagerOamWithAffineIdx(&_021D0EB0->subScreenOam, param_1, param_2);
    if (BVar1 == 0) {
        GF_AssertFail();
    }
    return BVar1;
}
