#include "global.h"

#include "save.h"
#include "text.h"
#include "unk_02005D10.h"
#include "overlay_112_pokewalker_app.h"
struct Ov112PokewalkerApp;

s32 ov112_021EEE20(Ov112PokewalkerApp *arg0) {
    ov112_021EA688(arg0, 8);
    return 1;
}

s32 ov112_021EEE2C(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 0xD);
    ov112_021EA5A4(arg0, 0);
    return 1;
}

s32 sub_02006B84(void);

s32 ov112_021EEE4C(Ov112PokewalkerApp *arg0) {
    if ((TextPrinterCheckActive((u8) arg0->unk_1E524) == 0) && (arg0->unk_1F2D6 == 0x1F) && (sub_02006B84() == 0)) {
        IsFanfarePlaying();
        ov112_021EA17C(arg0);
        return 2;
    }
    return 1;
}

s32 ov112_021EEE8C(void *arg0);

s32 ov112_021EEE8C(void *arg0) {
    SaveGameNormal(*(SaveData **)((u8 *)arg0 + 0x20));
    Save_ClearStatusFlags(*(SaveData **)((u8 *)arg0 + 0x20));
    return 3;
}

void ov112_021EA19C(Ov112PokewalkerApp *arg0);

s32 ov112_021EEEA0(Ov112PokewalkerApp *arg0);

s32 ov112_021EEEA0(Ov112PokewalkerApp *arg0) {
    ov112_021EA19C(arg0);
    return 4;
}

s32 ov112_021EEEAC(void *arg0);

s32 ov112_021EEEAC(void *arg0) {
    s32 v0;

    if (*(s32 *)((u8 *)arg0 + 0x10) == 4) {
        v0 = 0;
    } else {
        v0 = 8;
    }
    *(s32 *)((u8 *)arg0 + 4) = v0;
    return 0xA;
}
