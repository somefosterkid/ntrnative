#include "global.h"

#include "bg_window.h"
#include "save.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"
struct Ov112PokewalkerApp;

s32 ov112_021EEEC0(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 0xD);
    ov112_021EA5A4(arg0, 0);
    return 6;
}

s32 ov112_021EEEE0(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) arg0->unk_1E524) == 0) {
        ov112_021EA17C(arg0);
        return 7;
    }
    return 6;
}

s32 ov112_021EEF08(void *arg0);

s32 ov112_021EEF08(void *arg0) {
    SaveGameNormal(*(SaveData **)((u8 *)arg0 + 0x20));
    Save_ClearStatusFlags(*(SaveData **)((u8 *)arg0 + 0x20));
    return 8;
}

void ov112_021EA19C(Ov112PokewalkerApp *arg0);

s32 ov112_021EEF1C(Ov112PokewalkerApp *arg0);

s32 ov112_021EEF1C(Ov112PokewalkerApp *arg0) {
    ov112_021EA19C(arg0);
    return 9;
}

s32 ov112_021EEF28(void *arg0);

s32 ov112_021EEF28(void *arg0) {
    *(s32 *)((u8 *)arg0 + 4) = 0xA;
    return 0xA;
}

s32 ov112_021EEF30(Ov112PokewalkerApp *arg0) {
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
    ov112_021EA688(arg0, 6);
    return 0xB;
}
