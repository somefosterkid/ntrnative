#include "global.h"

#include "constants/heap.h"

#include "msgdata.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "overlay_112_hitbox.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021E7464(void);

s32 ov112_021E7668(void);

void ov112_021E7670(void);

u8 ov112_021E768C(void);

u32 ov112_021E76A8(void);

void ov112_021EA7D0(Ov112PokewalkerApp *);

void ov112_021ED314(s32);

void ov112_021ED530(Ov112PokewalkerApp *);

u32 ov112_021ED66C(Ov112PokewalkerApp *param_1) {
    s32 iVar1;
    u32 uVar2;

    ov112_021E7670();
    iVar1 = ov112_021E768C();
    if (iVar1 != 0) {
        ov112_021EA7D0(param_1);
    }
    ov112_021EA60C(param_1);
    ov112_021ED314(iVar1);
    uVar2 = ov112_021E76A8();
    switch (uVar2) {
    case 2:
        if (param_1->unk_16 == 1) {
            param_1->unk_14 = 0x2a;
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 0xb:
    case 0xc:
    case 0xd:
        param_1->unk_14 = (s16)uVar2;
        break;
    case 0xf:
        ManagedSprite_SetDrawFlag(param_1->sprites[2], 0);
        iVar1 = ov112_021ED610(param_1);
        if ((iVar1 != 0) && ((((u32) * &param_1->unk_10E7) << 29) >> 31 == 1)) {
            GetSpeciesNameIntoArray(param_1->unk_9D44, HEAP_ID_154, param_1->unk_9D54);
        }
        ov112_021EAA98(param_1, param_1->unk_1F2C0, 1);
        ov112_021ED530(param_1);
        ov112_021ED590(param_1);
        ov112_021ED7DC(param_1);
        param_1->unk_1EC7C = param_1->unk_10F0 - param_1->unk_1D758;
        return 0x25;
    }
    if (param_1->unk_14 != 0) {
        ManagedSprite_SetDrawFlag(param_1->sprites[2], 0);
        param_1->unk_C = 0x3c;
        ov112_021E7464();
        return 0x20;
    }
    iVar1 = ov112_021E7668();
    if (iVar1 == 0) {
        ov112_021E98E8(param_1, 1);
        int hit = ov112_021E9888(5);
        if (((hit == 0) || ((gSystem.newKeys & 1) != 0)) || ((gSystem.newKeys & 2) != 0)) {
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
    return 0x1b;
}
