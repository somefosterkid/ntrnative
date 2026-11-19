#include "global.h"

#include "overlay_manager.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"

struct OverlayManager;

BOOL ov112_021EBA50(OverlayManager *appMan, int *state);

u8 ov112_021EA08C(Ov112PokewalkerApp *, s32, s32);
s32 ov112_021EBA54(Ov112PokewalkerApp *arg0);
s32 ov112_021EBA6C(Ov112PokewalkerApp *arg0);

BOOL ov112_021EBA50(OverlayManager *appMan, int *state) {
    return 1;
}

s32 ov112_021EBA54(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 0x93);
    return 1;
}

s32 ov112_021EBA6C(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        return 2;
    }
    return 1;
}
