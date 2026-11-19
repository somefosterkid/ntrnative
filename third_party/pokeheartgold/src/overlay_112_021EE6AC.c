#include "global.h"

#include "overlay_manager.h"
#include "bg_window.h"
#include "pokewalker.h"
#include "save.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"

struct OverlayManager;

BOOL ov112_021EE6AC(OverlayManager *appMan, int *state);

u8 ov112_021EA08C(Ov112PokewalkerApp *, s32, s32);
s32 ov112_021EA5A4(void *, s32);
s32 ov112_021EE6B0(Ov112PokewalkerApp *arg0);
s32 ov112_021EA17C(Ov112PokewalkerApp *);
s32 ov112_021EE6D0(Ov112PokewalkerApp *arg0);
s32 ov112_021EE6F8(Ov112PokewalkerApp *arg0);
s32 ov112_021EA19C();
s32 ov112_021EE70C(void);
s32 ov112_021EE718(Ov112PokewalkerApp *arg0);
s32 ov112_021EE73C(Ov112PokewalkerApp *arg0);
s32 ov112_021EE754(Ov112PokewalkerApp *arg0);
s32 ov112_021EE778(Ov112PokewalkerApp *arg0);

BOOL ov112_021EE6AC(OverlayManager *appMan, int *state) {
    return 1;
}

s32 ov112_021EE6B0(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 0xD);
    ov112_021EA5A4(arg0, 0);
    return 1;
}

s32 ov112_021EE6D0(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) arg0->unk_1E524) == 0) {
        ov112_021EA17C(arg0);
        return 2;
    }
    return 1;
}

s32 ov112_021EE6F8(Ov112PokewalkerApp *arg0) {
    SaveGameNormal(arg0->saveData);
    Save_ClearStatusFlags(arg0->saveData);
    return 3;
}

s32 ov112_021EE70C(void) {
    ov112_021EA19C();
    return 4;
}

s32 ov112_021EE718(Ov112PokewalkerApp *arg0) {
    if (sub_0203263C(arg0->pokewalker) == 1) {
        return 5;
    }
    arg0->unk_4 = 0xA;
    arg0->unk_10 = 3;
    return 7;
}

s32 ov112_021EE73C(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 0x19);
    return 6;
}

s32 ov112_021EE754(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        arg0->unk_4 = 0xB;
        return 7;
    }
    return 6;
}

s32 ov112_021EE778(Ov112PokewalkerApp *arg0) {
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
    return 8;
}
