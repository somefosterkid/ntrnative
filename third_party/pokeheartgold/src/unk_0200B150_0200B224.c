#include "global.h"

#include "unk_0200B150.h"
#include "oam_manager.h"


void OamManager_ApplyAndResetBuffers(void) {
    if (_021D0EB0 != NULL) {
        NNS_G2dApplyAndResetOamManagerBuffer(&_021D0EB0->mainScreenOam);
        NNS_G2dApplyAndResetOamManagerBuffer(&_021D0EB0->subScreenOam);
    }
    return;
}
