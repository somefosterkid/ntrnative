#include "global.h"

#include "error_handling.h"
#include "oam_manager.h"


u16 sub_0200B334(const MtxFx22 *mtx);

u16 sub_0200B334(const MtxFx22 *mtx) {
    u16 v0;

    v0 = NNS_G2dEntryOamManagerAffine(&_021D0EB0->mainScreenOam, mtx);
    if (v0 == 0xFFFE) {
        GF_AssertFail();
    }
    return v0;
}
