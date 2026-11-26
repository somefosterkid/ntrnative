#include "global.h"

#include "unk_02005D10.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021EF994(Ov112PokewalkerApp *arg0) {
    s32 v0;

    if (arg0->unk_9D7A == 0) {
        v0 = ov112_021EA08C(arg0, 0, 0x2E);
    } else {
        PlayFanfare(0x4A1U);
        v0 = ov112_021EA08C(arg0, 0, 0x2D);
    }
    arg0->unk_1E524 = v0;
    arg0->unk_C = 6;
    return 0x23;
}
