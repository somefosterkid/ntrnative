#include "global.h"

#include "bg_window.h"
#include "pokewalker.h"
#include "system.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021E9C10(Ov112PokewalkerApp *arg0, s32 arg1);

extern s32 ov112_021FF0AC[];

s32 ov112_021EAFFC(Ov112PokewalkerApp *arg0) {
    s32 temp_r1_2;
    s32 temp_r2;

    s32 temp_r1;
    if (0x40 & gSystem.newKeys) {
        temp_r1 = arg0->unk_1EC50;
        if (temp_r1 > 0) {
            arg0->unk_1EC50 = (s32)(temp_r1 - 1);
        }
        ov112_021EA51C(arg0, ov112_021FF0AC[*(s32 *)&((u8 *)arg0)[0x1EC50]]);
    } else if (0x80 & gSystem.newKeys) {
        temp_r1_2 = arg0->unk_1EC50;
        if (temp_r1_2 < 5) {
            arg0->unk_1EC50 = (s32)(temp_r1_2 + 1);
        }
        ov112_021EA51C(arg0, ov112_021FF0AC[*(s32 *)&((u8 *)arg0)[0x1EC50]]);
    } else if (gSystem.newKeys & 1) {
        temp_r2 = arg0->unk_1EC50;
        switch (temp_r2) {
        case 0:
            arg0->unk_4 = 2;
            sub_020326A4(arg0->pokewalker, 0U, 0U);
            ov112_021E9C10(arg0, 3);
            ov112_021E9A78(arg0, 1);
            FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
            BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
            return 0x1A;
        case 1:
            arg0->unk_4 = 3;
            arg0->unk_1D77C = 0;
            arg0->unk_10 = 0;
            ov112_021E9C10(arg0, 3);
            ov112_021E9A78(arg0, 1);
            FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
            BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
            sub_020326A4(arg0->pokewalker, 0U, 0U);
            return 4;
        case 2:
            sub_020326A4(arg0->pokewalker, 1U, 1U);
            arg0->unk_1D77C = 0;
            arg0->unk_10 = 0;
            ov112_021E9C10(arg0, 3);
            ov112_021E9A78(arg0, 7);
            FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
            BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
            return 0xF;
        case 3:
            ov112_021E9C10(arg0, 3);
            ov112_021E9A78(arg0, 1);
            FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
            BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
            return 0xA;
        case 4:
            ov112_021E9C10(arg0, 3);
            ov112_021E9A78(arg0, 1);
            FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
            BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
            return 0xF;
        }
    }
    return 3;
}
