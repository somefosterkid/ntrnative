#include "global.h"
struct PokemonSummary;

#include "sprite.h"
#include "system.h"
#include "unk_02005D10.h"
#include "pokemon_summary_app.h"

s32 sub_0208AB58(void *, s32);

s32 sub_0208AEC4(void *);

s32 sub_0208B0F4(PokemonSummary *, s32);

s32 sub_0208931C(struct PokemonSummary *arg0);

s32 sub_0208931C(PokemonSummary *arg0) {
    s32 temp_r0;
    if (gSystem.newAndRepeatedKeys & 0x20) {
        sub_0208AB58(arg0, -1);
        return 0xC;
    }
    if (0x10 & gSystem.newAndRepeatedKeys) {
        sub_0208AB58(arg0, 1);
        return 0xC;
    }
    if (gSystem.newAndRepeatedKeys & 0x40) {
        sub_0208AB58(arg0, -3);
        return 0xC;
    }
    if (0x80 & gSystem.newAndRepeatedKeys) {
        sub_0208AB58(arg0, 3);
        return 0xC;
    }
    if (3 & gSystem.newKeys) {
        PlaySE(0x940U);
        return sub_0208B0F4(arg0, 0xB);
    }
    temp_r0 = sub_0208AEC4(arg0);
    switch (temp_r0) {
    case 9:
        if (Sprite_GetDrawFlag(arg0->sprites[64]) == 1) {
            PlaySE(0x5DCU);
            Sprite_SetAnimCtrlSeq(arg0->sprites[64], 2);
            return 0xD;
        }
        break;

    case -1:
        break;

    case 10:
        if (Sprite_GetDrawFlag(arg0->sprites[65]) == 1) {
            PlaySE(0x5DCU);
            Sprite_SetAnimCtrlSeq(arg0->sprites[65], 3);
            return 0xE;
        }
        break;

    case 11:
        PlaySE(0x940U);
        return sub_0208B0F4(arg0, 0xB);

    default:
        if ((arg0->ribbonCursor) != temp_r0) {
            arg0->ribbonCursor = (u8)temp_r0;
            PlaySE(0x5DCU);
            sub_0208AB58(arg0, 0);
        }
        return 0xC;
        break;
    }

    return 0xC;
}
