#include "global.h"

#include "error_handling.h"
#include "unk_02005D10.h"

void ov96_0220DC38(u32 param_1);

void ov96_0220DC38(u32 param_1) {
    s32 unaff_r4;
    int new_var;
    new_var = 0;
    switch (param_1) {
    case 3:
        unaff_r4 = new_var;
        break;

    case 4:
        unaff_r4 = 0x80;
        break;

    case 5:
        unaff_r4 = new_var;
        break;

    case 6:
        new_var = 0xc0;
        unaff_r4 = new_var;
        break;

    default:
        GF_AssertFail();
        break;
    }

    PlaySE_SetPitch(new_var = 0x88d, unaff_r4);
    return;
}
