#include "global.h"

#include "text.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021EC460(Ov112PokewalkerApp *, s32, s32, s32);

s32 ov112_021EB5AC(void *arg0);

s32 ov112_021E7CA4(void *, s32, s32);
s32 ov112_021E9F40(void *, s32);
u8 ov112_021E9FA4(void *, s32, String *, s32, s32);
s32 ov112_021EA51C(void *, s32);
extern s32 ov112_021FF09C;
s32 ov112_021EB5DC(Ov112PokewalkerApp *arg0);

s32 ov112_021EB5AC(void *arg0) {
    ov112_021EC460(arg0, 0, 8, 0x11);
    return 0x18;
}

s32 ov112_021EB5BC(Ov112PokewalkerApp *arg0);

s32 ov112_021EB5BC(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        return 0x11;
    }
    return 0x10;
}

s32 ov112_021EB5DC(Ov112PokewalkerApp *arg0) {
    ov112_021E7CA4(arg0, 2, 0xC);
    ov112_021E7CA4(arg0, 5, 0x10);
    ov112_021E9FA4(arg0, 3, arg0->unk_1E45C, 4, 0xF0100);
    ov112_021E9FA4(arg0, 4, arg0->unk_1E460, 4, 0xF0100);
    ov112_021E9FA4(arg0, 5, arg0->unk_1E464, 4, 0xF0100);
    ov112_021E9F40(arg0, 1);
    arg0->unk_1EC50 = 0;
    ov112_021EA51C(arg0, *(s32 *)(&ov112_021FF09C + arg0->unk_1EC50));
    return 0x12;
}
