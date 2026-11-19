#include "global.h"

#include "overlay_72_wfc_connect.h"

s32 ov72_0223B0C4(NintendoWfcYesNoMenu *);

s32 ov72_0223B1C8(NintendoWfcYesNoMenu *);

s32 ov72_0223B2E4(NintendoWfcYesNoMenu *);

void ov72_0223AF1C(NintendoWfcYesNoMenu *arg0);

void ov72_0223AF1C(NintendoWfcYesNoMenu *arg0) {
    arg0->state = 0;
    arg0->choice = arg0->initialChoice;
    ov72_0223B0C4(arg0);
    ov72_0223B1C8(arg0);
    ov72_0223B2E4(arg0);
}
