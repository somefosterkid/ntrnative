#include "global.h"

#include "error_handling.h"
#include "message_format.h"
#include "pokemon.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "overlay_111.h"

BOOL ov111_021E59E4(BugContestSwapMon *arg0);

BOOL ov111_021E59E4(BugContestSwapMon *arg0) {
    MessageFormat *temp_r6;
    Pokemon *temp_r5;
    s32 var_r5;
    u32 temp_r0;
    var_r5 = -1;
    temp_r6 = ov111_021E6A2C(arg0->text);
    if (ov111_021E6888(arg0->text) != 0) {
        return 0;
    }
    temp_r0 = arg0->substate;
    switch (temp_r0) {
    case 0:
        temp_r5 = arg0->args->newlyCaughtMon;
        ov111_021E6738(arg0->panels, 0, 0x80, 0x48, temp_r5);
        BufferBoxMonSpeciesName(temp_r6, 0U, Mon_GetBoxMon(temp_r5));
        var_r5 = 0;
        arg0->substate = 2;
        break;

    case 1:
        if (ov111_021E6888(arg0->text) == 0) {
            arg0->substate = 2;
        }
        break;

    case 2:
        if ((System_GetTouchNew() != 0) || (gSystem.newKeys & 1)) {
            PlaySE(0x5DCU);
            arg0->substate = 3;
        }
        break;

    case 3:
        return 1;

    default:
        GF_AssertFail();
        break;
    }
    if (var_r5 != -1) {
        ov111_021E68FC(arg0->text, var_r5);
    }
    return 0;
}
