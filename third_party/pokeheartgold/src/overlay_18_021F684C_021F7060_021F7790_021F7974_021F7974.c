#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"

extern u32 ov18_021FB84C;

extern u32 ov18_021FB8A4;

u32 ov18_021F7974(PokedexAppData *pokedexApp, int a1) {
    u8 uVar3;
    BOOL BVar4;
    s32 iVar5;
    u32 uVar6;
    u32 uStack_14;
    u32 uStack_18;
    BVar4 = System_GetTouchNewCoords(&uStack_14, &uStack_18);
    if (BVar4 == 1) {
        iVar5 = TouchscreenHitbox_FindRectAtTouchNew((TouchscreenHitbox *)(&ov18_021FB8A4));
        if (iVar5 == (-1)) {
            return 0xffffffff;
        }
        uVar6 = *((u32 *)((&ov18_021FB84C) + iVar5));
        if (uVar6 == 0) {
            uStack_14 = uStack_14 - 0x1b;
            uStack_18 = uStack_18 - 4;
            uVar3 = (uStack_14 / 40) + (uStack_18 / 40) * 5;
            if (uVar3 == pokedexApp->unk_185A) {
                if ((pokedexApp->unk_185A == 0) && (pokedexApp->unk_1859 == 0)) {
                    PlaySE(0x8e9);
                    return 2;
                }
                return 4;
            }
            pokedexApp->unk_185A = uVar3;
            PlaySE(0x8e9);
        } else if (uVar6 == 2) {
            PlaySE(0x8e9);
        } else if (uVar6 == 5) {
            PlaySE(0x940);
        }
        return uVar6;
    }
    if ((gSystem.newAndRepeatedKeys & 0x40) != 0) {
        if (pokedexApp->unk_185A < 5) {
            return 10;
        }
        pokedexApp->unk_185A = pokedexApp->unk_185A - 5;
        PlaySE(0x8e8);
        return 0;
    }
    if ((gSystem.newAndRepeatedKeys & 0x80) != 0) {
        if ((pokedexApp->unk_185A >= 10) && (pokedexApp->unk_185A < 0xf)) {
            return 0xc;
        }
        pokedexApp->unk_185A += 5;
        PlaySE(0x8e8);
        return 0;
    }
    if ((gSystem.newAndRepeatedKeys & 0x20) != 0) {
        if (pokedexApp->unk_185A != 0) {
            pokedexApp->unk_185A--;
            PlaySE(0x8e8);
            return 0;
        }
        if (pokedexApp->unk_1859 != 0) {
            pokedexApp->unk_185A = 0xe;
            return 9;
        }
        return 0xffffffff;
    }
    if ((gSystem.newAndRepeatedKeys & 0x10) != 0) {
        iVar5 = pokedexApp->unk_185A + 1;
        if (iVar5 != 0xf) {
            pokedexApp->unk_185A = (u8)iVar5;
            PlaySE(0x8e8);
            return 0;
        }
        uVar6 = ov18_021F8950(pokedexApp, a1);
        if ((pokedexApp->unk_1859 + 1) <= uVar6) {
            pokedexApp->unk_185A = 0;
            return 0xb;
        }
        return 0xffffffff;
    }
    if ((gSystem.newKeys & 1) != 0) {
        return 4;
    }
    if ((gSystem.newKeys & 2) != 0) {
        PlaySE(0x940);
        return 6;
    }
    if ((gSystem.newKeys & 0x400) != 0) {
        return 3;
    }
    if ((gSystem.newKeys & 0x800) != 0) {
        PlaySE(0x8e9);
        return 2;
    }
    if ((gSystem.newAndRepeatedKeys & 0x200) != 0) {
        return 9;
    }
    if ((gSystem.newAndRepeatedKeys & 0x100) != 0) {
        return 0xb;
    }
    if ((gSystem.newKeys & 4) != 0) {
        return 8;
    }
    return 0xffffffff;
}
