#include "global.h"

#include "bg_window.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021EC460(Ov112PokewalkerApp *, s32, s32, s32);

s32 ov112_021EB208(void *arg0);

s32 ov112_021EB208(void *arg0) {
    FillBgTilemapRect(*(BgConfig **)((u8 *)arg0 + 0x18), 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(*(BgConfig **)((u8 *)arg0 + 0x18), 1U);
    ov112_021EC460(arg0, 0, 8, 7);
    return 0x18;
}

s32 ov112_021E7CA4(void *, s32, s32);

s32 ov112_021E9F40(void *, s32);

u8 ov112_021E9FA4(void *, s32, void *, s32, s32);

s32 ov112_021EA51C(void *, s32);

s32 ov112_021EB244(Ov112PokewalkerApp *arg0);

s32 ov112_021EB244(Ov112PokewalkerApp *arg0) {
    ov112_021E7CA4(arg0, 2, 0xA);
    ov112_021E9FA4(arg0, 5, arg0->unk_1E46C, 4, 0xF0100);
    ov112_021E9F40(arg0, 1);
    arg0->unk_1EC50 = 0;
    ov112_021EA51C(arg0, 2);
    return 8;
}
