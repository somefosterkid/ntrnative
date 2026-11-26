#include "global.h"

#include "text.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021EB1D8(Ov112PokewalkerApp *arg0) {
    ov112_021EC460(arg0, 2, 0, 6);
    return 0x18;
}

s32 ov112_021EB1E8(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        return 7;
    }
    return 5;
}
