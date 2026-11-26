#include "global.h"

#include "overlay_08_battle_bag.h"

u16 *ov08_022245DC(BattleBag *battleBag, u32 button, s32 buttonState, u32 unused);

#if defined(SDK_PORT) || defined(NONMATCHING)
u16 ov08_02224684(BattleBag *battleBag, s32 button, u32 buttonState, int screen);
#else
s16 ov08_02224684(BattleBag *battleBag, s32 button, u32 buttonState, int screen);
#endif

void ov08_022246F4(BattleBag *battleBag, u16 *buttonData, s32 button, u32 buttonState);

extern u8 ov08_02225E9C[];

extern u8 ov08_02225E9E[];

extern u8 ov08_02225E9F[];

void ov08_02224768(BattleBag *battleBag, u16 *buttonData, u8 button, u8 buttonState, u8 screen);

void ov08_02224768(BattleBag *battleBag, u16 *buttonData, u8 button, u8 buttonState, u8 screen) {
    s16 sVar1;
    u16 *src;
    s32 uVar3;
    s32 iVar4;
    u32 uVar5;
    u8 *tbl;

    src = ov08_022245DC(battleBag, button, buttonState, buttonState);
    sVar1 = ov08_02224684(battleBag, button, buttonState, screen);
    uVar5 = (u32)(sVar1 << 0x1c) >> 0x10;
    iVar4 = button * 4;
    uVar3 = 0;
    if (ov08_02225E9E[iVar4] * ov08_02225E9F[iVar4] > 0) {
        tbl = ov08_02225E9C + iVar4;
        do {
            buttonData[uVar3] = (src[uVar3] & 0xfff) | uVar5;
            uVar3 = uVar3 + 1 & 0xffff;
        } while (uVar3 < tbl[2] * tbl[3]);
    }
    ov08_022246F4(battleBag, buttonData, button, buttonState);
    return;
}
