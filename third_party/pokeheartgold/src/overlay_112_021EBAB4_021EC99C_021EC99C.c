#include "global.h"

#include "bg_window.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021E7CA4(void *, s32, s32);

extern void *ov112_021FF0C4;

s32 ov112_021EC99C(Ov112PokewalkerApp *arg0) {
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0x10U, 0x20U, 8U, 0x10U);
    ov112_021E7CA4(arg0, 5, 0x11);
    ov112_021E7CA4(arg0, 2, 0x17);
    arg0->unk_1EC76 = (s16)ov112_021EC950(arg0);
    ov112_021EC7D0(arg0);
    ov112_021E9F40(arg0, 6);
    ov112_021ECA18(arg0, arg0->unk_1EC54);
    ov112_021EA51C(arg0, *(s32 *)(&ov112_021FF0C4 + arg0->unk_1EC50));
    return 0xA;
}
