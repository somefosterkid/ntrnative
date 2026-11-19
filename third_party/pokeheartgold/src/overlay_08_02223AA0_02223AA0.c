#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "battle/battle.h"
#include "battle/battle_cursor.h"
#include "battle/battle_finger.h"

#include "bag_types_def.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_loader.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_bag_context.h"

typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;

typedef u16 charcode_t;

typedef u32 TextColor;

void ov08_022234A4(BattleBag *battleBag, u8 windowIndex, u32 textID, enum Font font, u32 yOffset, TextColor color);

void ov08_02223678(BattleBag *battleBag, u32 itemIndex, u32 slot, u32 windowIndex, enum Font font, u32 yOffset, TextColor color);

void ov08_022239CC(BattleBag *battleBag, u32 slot);

void ov08_02223A3C(BattleBag *battleBag, u32 slot);

void ov08_02223AA0(BattleBag *battleBag);

void ov08_02223AA0(BattleBag *battleBag) {
    u32 slot;

    for (u32 i = 0; i < 4; i++) {
        FillWindowPixelBuffer(&battleBag->windows[i], 0);
    }

    slot = battleBag->context->pocketCurrentPages[battleBag->currentBattlePocket] * 6 + battleBag->context->pocketCurrentPagePositions[battleBag->currentBattlePocket];

    ov08_022239CC(battleBag, slot);
    ov08_02223678(battleBag, slot, 0, BATTLE_BAG_USE_ITEM_MENU_WINDOW_ITEM_AMOUNT, FONT_SYSTEM, 0, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
    ov08_02223A3C(battleBag, slot);
    ov08_022234A4(battleBag, BATTLE_BAG_USE_ITEM_MENU_WINDOW_ITEM_USE, 31, (enum Font)4, 5, ((TextColor)(((3 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((1 & 0xFF) << 0))));
}
