#include "global.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EA570(Ov112PokewalkerApp *);

s32 ov112_021E7CA4(void *, s32, s32);
u8 ov112_021E9FA4(void *, s32, String *, s32, s32);
extern void *ov112_021FF090;

s32 ov112_021EB388(Ov112PokewalkerApp *arg0) {
    s32 v0;

    ov112_021EA570(arg0);
    if (arg0->unk_1EC50 == 0) {
        v0 = 3;
    } else {
        v0 = 0xB;
    }
    arg0->unk_4 = v0;
    return 0x1A;
}

s32 ov112_021EB3A8(Ov112PokewalkerApp *arg0) {
    ov112_021EC460(arg0, 0, 2, 0xC);
    return 0x18;
}

s32 ov112_021EB3B8(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        return 0xC;
    }
    return 0xB;
}

s32 ov112_021EB3D8(Ov112PokewalkerApp *arg0) {
    ov112_021E7CA4(arg0, 2, 0xB);
    ov112_021E7CA4(arg0, 5, 0x10);
    ov112_021E9FA4(arg0, 3, arg0->unk_1E464, 4, 0xF0100);
    ov112_021E9FA4(arg0, 4, arg0->unk_1E460, 4, 0xF0100);
    ov112_021E9F40(arg0, 1);
    arg0->unk_1EC50 = 0;
    ov112_021EA51C(arg0, *(s32 *)(&ov112_021FF090 + arg0->unk_1EC50));
    return 0xD;
}
