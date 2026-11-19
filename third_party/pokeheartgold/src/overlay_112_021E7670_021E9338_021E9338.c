#include "global.h"

#include "pokewalker.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021E9338(Ov112PokewalkerApp *arg0);

s32 ov112_021E9338(Ov112PokewalkerApp *arg0) {
    u16 sp0[2];

    sub_02032688(arg0->pokewalker, &sp0[1], &sp0[0]);
    if (*(&sp0[1]) != 0) {
        return 1;
    }
    return 0;
}
