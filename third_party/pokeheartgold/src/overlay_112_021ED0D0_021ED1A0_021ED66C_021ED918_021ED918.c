#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "overlay_112_hitbox.h"

void ov112_021E7464(void);

s32 ov112_021E7668(void);

void ov112_021E7670(u8 *);

u8 ov112_021E768C(void);

u32 ov112_021E76A8(void);

void ov112_021E98E8(u8 *, u16);

void ov112_021EA10C(u8 *, u32, u32);

void ov112_021EA60C(u8 *);

void ov112_021EA670(u8 *, u32);

void ov112_021EA7D0(u8 *);

void ov112_021EC440(u8 *, u32, u32, u32);

void ov112_021ED314(s32);

void ov112_021ED530(u8 *, s32);

void ov112_021ED5DC(void *);

extern u8 gSystem;

u32 ov112_021ED918(u8 *param_1);

u32 ov112_021ED918(u8 *param_1) {
    s32 iVar1;
    u32 uVar2;
    s64 sVar3;
    RTCDate RStack_1c;
    RTCTime RStack_28;
    ov112_021E7670(param_1);
    iVar1 = ov112_021E768C();
    if (iVar1 != 0) {
        ov112_021EA7D0(param_1);
    }
    ov112_021ED314(iVar1);
    ov112_021EA60C(param_1);
    uVar2 = ov112_021E76A8();
    switch (uVar2) {
    case 2:
        if ((*((u16 *)(param_1 + 0x16))) == 1) {
            *((u16 *)(param_1 + 0x14)) = 0x2a;
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
        *((s16 *)(param_1 + 0x14)) = (s16)uVar2;
        break;

    case 0xf:
        ManagedSprite_SetDrawFlag(*((ManagedSprite **)(param_1 + 0x1e538)), 0);
        RTC_GetDateTime(&RStack_1c, &RStack_28);
        sVar3 = RTC_ConvertDateTimeToSecond(&RStack_1c, &RStack_28);
        ov112_021ED530(param_1, (s32)(((u64)sVar3) >> 0x20));
        ov112_021ED5DC(param_1);
        ov112_021EA670(param_1, 6);
        return 0x29;
    }

    if ((*((u16 *)(param_1 + 0x14))) != 0) {
        ManagedSprite_SetDrawFlag(*((ManagedSprite **)(param_1 + 0x1e538)), 0);
        *((u32 *)(param_1 + 0xc)) = 0x3d;
        ov112_021E7464();
        return 0x20;
    }
    iVar1 = ov112_021E7668();
    if (iVar1 == 0) {
        ov112_021E98E8(param_1, 1);
        iVar1 = ov112_021E9888(5);
        if (((iVar1 == 0) || ((((u32 *)(&gSystem))[0x12] & 1) != 0)) || ((((u32 *)(&gSystem))[0x12] & 2) != 0)) {
            ov112_021E7464();
            *((u16 *)(param_1 + 0x14)) = 0x21;
            *((u32 *)(param_1 + 0xc)) = 0x42;
            ov112_021EC440(param_1, 3, 3, 0x21);
            PlaySE(0x5dd);
            return 0x41;
        }
    } else {
        if ((*((u16 *)(param_1 + 0x16))) == 0) {
            ov112_021EA10C(param_1, 2, 6);
            *((u16 *)(param_1 + 0x16)) = 1;
        }
        ov112_021E98E8(param_1, 0);
    }
    return 0x1d;
}
