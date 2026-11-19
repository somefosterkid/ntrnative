#include "global.h"

#include "bg_window.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021E9A78(void *, u8);

s32 ov112_021E9C10(void *, s32);

s32 ov112_021EF6A0(void *arg0);

s32 ov112_021EF6A0(void *arg0) {
    FillBgTilemapRect(*(BgConfig **)((u8 *)arg0 + 0x18), 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(*(BgConfig **)((u8 *)arg0 + 0x18), 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(*(BgConfig **)((u8 *)arg0 + 0x18), 1U);
    BgCommitTilemapBufferToVram(*(BgConfig **)((u8 *)arg0 + 0x18), 2U);
    ov112_021E9C10(arg0, 6);
    ov112_021E9A78(arg0, 1);
    return 1;
}

s32 ov112_021E7CA4(void *, s32, s32);

u8 ov112_021E9FD8(Ov112PokewalkerApp *, s32, void *, s32);

s32 ov112_021EF700(Ov112PokewalkerApp *arg0);

s32 ov112_021EF700(Ov112PokewalkerApp *arg0) {
    s32 v0;

    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    v0 = arg0->unk_10;
    if (v0 == 2) {
        return 0x16;
    }
    if (v0 == 3) {
        return 0x1E;
    }
    ov112_021E7CA4(arg0, 2, 0xF);
    ov112_021E9FD8(arg0, 1, arg0->unk_1E494, 0);
    return 1;
}
