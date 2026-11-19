#include "global.h"
#include "battle/battle_bag_context.h"
#include "font.h"
#include "overlay_08_battle_bag.h"

void ov08_022235D4(BattleBag *battleBag, u32 itemIndex, u32 slot, u32 windowIndex, enum Font font, u32 color);

void ov08_02223678(BattleBag *battleBag, u32 itemIndex, u32 slot, u32 windowIndex, enum Font font, u32 yOffset, u32 color);

void ov08_0222370C(BattleBag *battleBag, u32 arg1);

void ov08_0222370C(BattleBag *battleBag, u32 arg1) {
    s32 v0;
    s32 v1;
    s32 v2;

    v0 = 6 * battleBag->context->pocketCurrentPages[battleBag->currentBattlePocket];
    if (battleBag->useAltPocketMenuWindows == 0) {
        v2 = 0;
    } else {
        v2 = 0xC;
    }
    v1 = arg1 * 2;
    ov08_022235D4(battleBag, arg1 + v0, arg1, v2 + v1, FONT_UNK_4, 0x30201);
    ov08_02223678(battleBag, arg1 + v0, arg1, v2 + 1 + v1, FONT_SYSTEM, 4, 0x10200);
}
