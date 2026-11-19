#include "global.h"

#include "heap.h"
#include "overlay_58.h"
#include "overlay_manager.h"
#include "party_menu.h"

BOOL ov58_022378C0(OverlayManager **appManPtr);

u32 ov58_02237C4C(Ov58ApricornBoxWork *param_1);

extern OverlayManagerTemplate ov58_02237D2C;
s32 ov58_02237C88(Ov58ApricornBoxWork *parentWork);

u32 ov58_02237C4C(Ov58ApricornBoxWork *param_1) {
    u8 cVar1;
    s32 iVar2;

    iVar2 = ov58_022378C0(&param_1->childMan);
    if (iVar2 == 0) {
        return 4;
    }
    cVar1 = param_1->partyMenuArgs->partySlot;
    param_1->unk_1C = cVar1;
    Heap_Free(param_1->partyMenuArgs);
    param_1->partyMenuArgs = NULL;
    if (cVar1 == 7) {
        cVar1 = param_1->args->unk0;
        if (cVar1 == 3) {
            return 7;
        }
        return 1;
    }
    return 5;
}

s32 ov58_02237C88(Ov58ApricornBoxWork *parentWork) {
    parentWork->childMan = OverlayManager_New(&ov58_02237D2C, parentWork, parentWork->heapId);
    return 6;
}
