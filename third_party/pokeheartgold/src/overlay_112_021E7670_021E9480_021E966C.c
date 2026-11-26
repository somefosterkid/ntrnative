#include "global.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021E7CA4(void *, s32, s32);

u8 ov112_021E9FA4(void *, s32, void *, s32, s32);

void ov112_021E966C(Ov112PokewalkerApp *arg0, s32 arg1) {
    ov112_021E7CA4(arg0, 2, 0xD);
    ov112_021EA51C(arg0, 0);
    ov112_021E9FA4(arg0, 3, arg0->unk_1E514, 4, 0xF0100);
    ov112_021E9FA4(arg0, 4, arg0->unk_1E518, 4, 0xF0100);
    ov112_021EA51C(arg0, arg1);
    arg0->unk_1EC50 = arg1;
    arg0->unk_1F2E2 = 0;
}
