#include "global.h"

#include "overlay_72_wfc_connect.h"

extern s32 (*ov72_0223B7FC[])(NintendoWfcYesNoMenu *);

s32 ov72_0223AF48(NintendoWfcYesNoMenu *p);

s32 ov72_0223AF48(NintendoWfcYesNoMenu *p) {
    if (ov72_0223B7FC[p->state](p) == 1) {
        return p->choice + 1;
    }
    return 0;
}
