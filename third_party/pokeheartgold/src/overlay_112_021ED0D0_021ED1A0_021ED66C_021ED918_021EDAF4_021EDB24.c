#include "global.h"

#include "pokewalker.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "overlay_112_hitbox.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021E7464(void);

s32 ov112_021E7668(void);

void ov112_021E7670(Ov112PokewalkerApp *, s32);

u8 ov112_021E768C(void);

u32 ov112_021E76A8(void);

void ov112_021EA76C(Ov112PokewalkerApp *);

void ov112_021EAA10(Ov112PokewalkerApp *);

void ov112_021ED314(u32);

u32 ov112_021EDB24(Ov112PokewalkerApp *param_1, s32 arg1) {
    u16 uVar1;
    u32 uVar2;
    u32 uVar3;
    s32 iVar4;

    ov112_021E7670(param_1, arg1);
    uVar1 = ov112_021E768C();
    uVar2 = ov112_021ED330(param_1, (u8)uVar1);
    if (uVar2 != 0) {
        ov112_021EA76C(param_1);
    }
    ov112_021EA60C(param_1);
    ov112_021ED314(uVar2);
    ov112_021EDAF4(param_1, uVar2);
    if (uVar2 > 0xC8) {
        if (param_1->unk_1F2D4 == 0) {
            ov112_021EAA98(param_1, param_1->unk_1F2C0, 2);
            param_1->unk_1F2D4 = 1;
        }
    } else {
        ov112_021EAA10(param_1);
        param_1->unk_1F2D4 = 0;
    }
    uVar3 = ov112_021E76A8();
    switch (uVar3) {
    case 2:
        if (param_1->unk_16 == 1) {
            param_1->unk_14 = 0x2a;
        }
        break;
    case 4:
    case 5:
    case 7:
    case 8:
    case 9:
    case 0xb:
    case 0xc:
    case 0xd:
        param_1->unk_14 = (s16)uVar3;
        break;
    case 0xf:
        ManagedSprite_SetDrawFlag(param_1->sprites[2], 0);
        sub_02032674(param_1->pokewalker, &param_1->unk_1D758, &param_1->unk_1D75C);
        ov112_021ED590(param_1);
        sub_02032644(param_1->pokewalker);
        ov112_021EA688(param_1, 8);
        ov112_021EAA98(param_1, param_1->unk_1F2C0, 0);
        return 0x23;
    }
    if (param_1->unk_14 != 0) {
        ManagedSprite_SetDrawFlag(param_1->sprites[2], 0);
        param_1->unk_C = 0x3b;
        ov112_021E7464();
        return 0x20;
    }
    iVar4 = ov112_021E7668();
    if (iVar4 == 0) {
        ov112_021E98E8(param_1, 1);
        iVar4 = ov112_021E9888(5);
        if (((iVar4 == 0) || ((gSystem.newKeys & 1) != 0)) || ((gSystem.newKeys & 2) != 0)) {
            param_1->unk_14 = 0x21;
            param_1->unk_C = 0x42;
            ov112_021E7464();
            ov112_021EC440(param_1, 3, 3, 0x21);
            PlaySE(0x5dd);
            return 0x41;
        }
    } else {
        if (param_1->unk_16 == 0) {
            ov112_021EA10C(param_1, 2, 6);
            param_1->unk_16 = 1;
        }
        ov112_021E98E8(param_1, 0);
    }
    return 0x1f;
}
