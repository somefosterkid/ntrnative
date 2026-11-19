#include "global.h"
#include "pokewalker.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "overlay_112_pokewalker_app.h"
#include "overlay_112_hitbox.h"

void ov112_021E7484(u32);

u32 ov112_021EC000(Ov112PokewalkerApp *param_1);

s32 ov112_021E7CC8();
s32 ov112_021EC038(void);

void ov112_021E7464(void);
s32 ov112_021E7668(void);
u32 ov112_021E76A8(void);
void ov112_021E98E8(u8 *, u16);
void ov112_021EC134(u8 *param_1);
void ov112_021EC440(u8 *, u32, u32, u32);
s32 ov112_021EC044(u8 *param_1);

u32 ov112_021EC000(Ov112PokewalkerApp *param_1) {
    ov112_021E7484(1);
    MIi_CpuClearFast(0, &param_1->unk_AABC, 0x6c8);
    MIi_CpuClearFast(0, param_1->unk_9D70, 0xd4c);
    return 0x10;
}

s32 ov112_021EC038(void) {
    ov112_021E7CC8();
    return 0x11;
}

s32 ov112_021EC044(u8 *param_1) {
    u32 uVar1;
    s32 iVar2;
    s32 iVar3;

    iVar3 = 0x11;
    uVar1 = ov112_021E76A8();
    switch (uVar1) {
    case 2:
        iVar3 = 0x12;
        break;
    case 4:
        iVar3 = 0x12;
        break;
    case 5:
        iVar3 = 0x12;
        break;
    case 7:
        iVar3 = 0x12;
        break;
    case 8:
        iVar3 = 0x12;
        break;
    case 9:
        iVar3 = 0x12;
        break;
    case 0xb:
        iVar3 = 0x12;
        break;
    case 0xc:
        iVar3 = 0x12;
        break;
    case 0xf:
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(param_1 + 0x1e538), 0);
        ov112_021EC134(param_1);
        sub_02032624(*(POKEWALKER **)(param_1 + 0x1e440));
        *(u32 *)(param_1 + 0x1d758) = 0;
        *(u32 *)(param_1 + 0x1d75c) = 0;
        ov112_021E7464();
        return 0x14;
    }
    if (iVar3 != 0x11) {
        ov112_021E7464();
    }
    iVar2 = ov112_021E7668();
    if (iVar2 == 0) {
        ov112_021E98E8(param_1, 1);
        iVar2 = ov112_021E9888(5);
        if (((iVar2 == 0) || ((((u32 *)&gSystem)[0x12] & 1) != 0)) || ((((u32 *)&gSystem)[0x12] & 2) != 0)) {
            ov112_021E7464();
            ov112_021EC440(param_1, 1, 3, 0x12);
            PlaySE(0x5dc);
            return 0x21;
        }
    } else {
        ov112_021E98E8(param_1, 0);
    }
    return iVar3;
}
