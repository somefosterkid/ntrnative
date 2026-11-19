#include "global.h"

#include "math_util.h"
#include "pokewalker.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "overlay_112_hitbox.h"

void _u32_div_f(u32, u32);

void ov112_021E7464(void);

s32 ov112_021E7668(void);

void ov112_021E7670(u8 *);

u8 ov112_021E768C(void);

u32 ov112_021E76A8(void);

void ov112_021E98E8(u8 *, u16);

void ov112_021EA10C(u8 *, u32, u32);

void ov112_021EA60C(u8 *);

void ov112_021EA688(u8 *, u32);

void ov112_021EA76C(u8 *);

void ov112_021EAA10(u8 *);

void ov112_021EAA98(u8 *, u8 *, u32);

void ov112_021EC440(u8 *, u32, u32, u32);

void ov112_021ED314(s32 param_1);

u32 ov112_021ED330(u8 *param_1, u32 param_2);

void ov112_021EDAF4(u8 *, u32);

extern u8 gSystem;

u32 ov112_021ED35C(u8 *param_1);

u32 ov112_021ED35C(u8 *param_1) {
    u32 uVar1;
    u32 uVar3;
    s32 iVar4;

    ov112_021E7670(param_1);
    uVar1 = ov112_021E768C();
    uVar1 = ov112_021ED330(param_1, uVar1);
    uVar1 = uVar1 & 0xff;
    if (uVar1 > MTRandom() % 0xff) {
        ov112_021EA76C(param_1);
    }
    ov112_021EA60C(param_1);
    ov112_021ED314(uVar1);
    ov112_021EDAF4(param_1, uVar1);
    if (uVar1 > 0xC8) {
        if (*(u16 *)(param_1 + 0x1f2d4) == 0) {
            ov112_021EAA98(param_1, param_1 + 0x1f2c0, 2);
            *(u16 *)(param_1 + 0x1f2d4) = 1;
        }
    } else {
        ov112_021EAA10(param_1);
        *(u16 *)(param_1 + 0x1f2d4) = 0;
    }
    uVar3 = ov112_021E76A8();
    switch (uVar3) {
    case 2:
        if (*(u16 *)(param_1 + 0x16) == 1) {
            *(u16 *)(param_1 + 0x14) = 0x2a;
        }
        break;
    case 4:
    case 7:
    case 8:
    case 9:
    case 0xb:
    case 0xc:
    case 0xd:
        *(s16 *)(param_1 + 0x14) = (s16)uVar3;
        break;
    case 0xf:
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(param_1 + 0x1e538), 0);
        sub_02032644(*(POKEWALKER **)(param_1 + 0x1e440));
        ov112_021EA688(param_1, 8);
        ov112_021EAA98(param_1, param_1 + 0x1f2c0, 0);
        return 0x23;
    }
    if (*(u16 *)(param_1 + 0x14) != 0) {
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(param_1 + 0x1e538), 0);
        *(u32 *)(param_1 + 0xc) = 0x3a;
        ov112_021E7464();
        return 0x20;
    }
    iVar4 = ov112_021E7668();
    if (iVar4 == 0) {
        ov112_021E98E8(param_1, 1);
        iVar4 = ov112_021E9888(5);
        if (((iVar4 == 0) || ((((u32 *)&gSystem)[0x12] & 1) != 0)) || ((((u32 *)&gSystem)[0x12] & 2) != 0)) {
            *(u16 *)(param_1 + 0x14) = 0x21;
            ov112_021E7464();
            ov112_021EC440(param_1, 3, 3, 0x21);
            PlaySE(0x5dd);
            return 0x41;
        }
    } else {
        if (*(u16 *)(param_1 + 0x16) == 0) {
            ov112_021EA10C(param_1, 2, 6);
            *(u16 *)(param_1 + 0x16) = 1;
        }
        ov112_021E98E8(param_1, 0);
    }
    return 0x19;
}
