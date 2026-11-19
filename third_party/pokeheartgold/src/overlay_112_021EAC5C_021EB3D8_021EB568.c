#include "global.h"

#include "error_handling.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021EA570();

s32 ov112_021EB568(Ov112PokewalkerApp *arg0);

s32 ov112_021EB568(Ov112PokewalkerApp *arg0) {
    s32 temp_r0;

    ov112_021EA570();
    temp_r0 = arg0->unk_1EC50;
    switch (temp_r0) {
    case 0:
        arg0->unk_4 = 3;
        arg0->unk_10 = 1;
        break;
    case 1:
        arg0->unk_4 = 3;
        arg0->unk_10 = 2;
        break;
    case 2:
        arg0->unk_4 = 0xB;
        break;
    default:
        if (temp_r0 >= 3) {
            GF_AssertFail();
        }
        break;
    }
    return 0x1A;
}
