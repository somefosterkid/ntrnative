#include "global.h"

#include "overlay_58.h"
#include "overlay_manager.h"

BOOL ov58_022378C0(OverlayManager **appManPtr);

s32 ov58_02237CA0(Ov58ApricornBoxWork *arg0);

s32 ov58_02237CA0(Ov58ApricornBoxWork *arg0) {
    if (ov58_022378C0(&arg0->childMan) == 0) {
        return 6;
    }
    if (arg0->unk_18 == 3) {
        return 3;
    }
    if (arg0->args->unk0 == 3) {
        return 7;
    }
    return 1;
}
