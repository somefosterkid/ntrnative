#include "global.h"

#include "overlay_08_battle_bag.h"

void ov08_0222458C(u16 *buttonData, u16 *screenData, u8 xOffset, u8 yOffset, u8 width, u8 height);

void ov08_02224254(BattleBag *battleBag, void *param_2);

void ov08_02224254(BattleBag *battleBag, void *param_2) {
    ov08_0222458C(battleBag->menuPocketButtonData[0], param_2, 0, 0, 0x10, 9);
    ov08_0222458C(battleBag->menuPocketButtonData[1], param_2, 0, 9, 0x10, 9);
    ov08_0222458C(battleBag->menuPocketButtonData[2], param_2, 0, 0x12, 0x10, 9);
    ov08_0222458C(battleBag->useItemButtonData[0], param_2, 0, 0x1b, 0x1a, 5);
    ov08_0222458C(battleBag->useItemButtonData[1], param_2, 0, 0x20, 0x1a, 5);
    ov08_0222458C(battleBag->useItemButtonData[2], param_2, 0, 0x25, 0x1a, 5);
    ov08_0222458C(battleBag->useItemButtonData[3], param_2, 0, 0x2a, 0x1a, 5);
    ov08_0222458C(battleBag->cancelButtonData[0], param_2, 0, 0x39, 5, 5);
    ov08_0222458C(battleBag->cancelButtonData[1], param_2, 5, 0x39, 5, 5);
    ov08_0222458C(battleBag->cancelButtonData[2], param_2, 10, 0x39, 5, 5);
    ov08_0222458C(battleBag->pocketItemButtonData[0], param_2, 0x10, 0, 0x10, 6);
    ov08_0222458C(battleBag->pocketItemButtonData[1], param_2, 0x10, 6, 0x10, 6);
    ov08_0222458C(battleBag->pocketItemButtonData[2], param_2, 0x10, 0xc, 0x10, 6);
    ov08_0222458C(battleBag->pocketItemButtonData[3], param_2, 0x10, 0x12, 0x10, 6);
    ov08_0222458C(battleBag->pocketPrevPageButtonData[0], param_2, 0, 0x2f, 5, 5);
    ov08_0222458C(battleBag->pocketPrevPageButtonData[1], param_2, 5, 0x2f, 5, 5);
    ov08_0222458C(battleBag->pocketPrevPageButtonData[2], param_2, 10, 0x2f, 5, 5);
    ov08_0222458C(battleBag->pocketPrevPageButtonData[3], param_2, 0xf, 0x2f, 5, 5);
    ov08_0222458C(battleBag->pocketNextPageButtonData[0], param_2, 0, 0x34, 5, 5);
    ov08_0222458C(battleBag->pocketNextPageButtonData[1], param_2, 5, 0x34, 5, 5);
    ov08_0222458C(battleBag->pocketNextPageButtonData[2], param_2, 10, 0x34, 5, 5);
    ov08_0222458C(battleBag->pocketNextPageButtonData[3], param_2, 0xf, 0x34, 5, 5);
    ov08_0222458C(battleBag->menuScreenRecoverHPPocketIconData[0], param_2, 0x14, 0x2f, 4, 4);
    ov08_0222458C(battleBag->menuScreenRecoverHPPocketIconData[1], param_2, 0x18, 0x2f, 4, 4);
    ov08_0222458C(battleBag->menuScreenRecoverHPPocketIconData[2], param_2, 0x1c, 0x2f, 4, 4);
    ov08_0222458C(battleBag->menuScreenRecoverStatusPocketIconData[0], param_2, 0x14, 0x33, 4, 4);
    ov08_0222458C(battleBag->menuScreenRecoverStatusPocketIconData[1], param_2, 0x18, 0x33, 4, 4);
    ov08_0222458C(battleBag->menuScreenRecoverStatusPocketIconData[2], param_2, 0x1c, 0x33, 4, 4);
    ov08_0222458C(battleBag->menuScreenPokeBallsPocketIconData[0], param_2, 0x14, 0x37, 4, 4);
    ov08_0222458C(battleBag->menuScreenPokeBallsPocketIconData[1], param_2, 0x18, 0x37, 4, 4);
    ov08_0222458C(battleBag->menuScreenPokeBallsPocketIconData[2], param_2, 0x1c, 0x37, 4, 4);
    ov08_0222458C(battleBag->menuScreenBattleItemsIconData[0], param_2, 0x14, 0x3b, 4, 4);
    ov08_0222458C(battleBag->menuScreenBattleItemsIconData[1], param_2, 0x18, 0x3b, 4, 4);
    ov08_0222458C(battleBag->menuScreenBattleItemsIconData[2], param_2, 0x1c, 0x3b, 4, 4);
    return;
}
