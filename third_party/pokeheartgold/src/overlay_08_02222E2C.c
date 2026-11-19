#include "global.h"

#include "battle/battle_system.h"

#include "bag_cursor.h"
#include "font.h"
#include "palette.h"
#include "systask_environment.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "battle/battle.h"
#include "battle/battle_cursor.h"
#include "battle/battle_finger.h"
#include "bag_types_def.h"
#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_bag_context.h"
#include "overlay_08_battle_bag.h"

s32 ov08_022230CC(BgConfig *);

s32 ov08_02223228(void *);

s32 ov08_02223464(void *);

s32 ov08_02223F34(void *);

void ov08_02224B7C(BattleSubMenuCursor *);

BOOL ov08_02224B8C(BattleSubMenuCursor *);

s32 ov08_02222E2C(SysTask *arg0, BattleBag *battleBag);

typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;
typedef u16 charcode_t;
enum BattleBagMenuScreenButton {
    BATTLE_BAG_MENU_SCREEN_BUTTON_RECOVER_HP_POCKET = 0,
    BATTLE_BAG_MENU_SCREEN_BUTTON_RECOVER_STATUS_POCKET,
    BATTLE_BAG_MENU_SCREEN_BUTTON_POKE_BALLS_POCKET,
    BATTLE_BAG_MENU_SCREEN_BUTTON_BATTLE_ITEMS_POCKET,
    BATTLE_BAG_MENU_SCREEN_BUTTON_LAST_USED_ITEM,
    BATTLE_BAG_MENU_SCREEN_BUTTON_CANCEL,
};
enum BattleBagPocketMenuScreenButton {
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_1 = 0,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_2,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_3,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_4,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_5,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_6,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_PREV_PAGE,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_NEXT_PAGE,
    BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_CANCEL,
};
enum BattleBagUseItemScreenButton {
    BATTLE_BAG_USE_ITEM_SCREEN_BUTTON_USE = 0,
    BATTLE_BAG_USE_ITEM_SCREEN_BUTTON_CANCEL,
};
void ov08_02224938(BattleBag *battleBag, u8 pressedButton, u8 unused);
enum BattlePocketIndex {
    BATTLE_POCKET_INDEX_RECOVER_HP_PP = 0,
    BATTLE_POCKET_INDEX_RECOVER_STATUS,
    BATTLE_POCKET_INDEX_POKE_BALLS,
    BATTLE_POCKET_INDEX_BATTLE_ITEMS,
    BATTLE_POCKET_INDEX_NONE,
};
u16 ov08_02223CD4(BattleBag *battleBag, u32 pagePosition);
enum CatchTutorialState {
    CATCH_TUTORIAL_STATE_MENU_SCREEN = 0,
    CATCH_TUTORIAL_STATE_SETUP_POCKET_MENU_SCREEN,
    CATCH_TUTORIAL_STATE_POCKET_MENU_SCREEN,
    CATCH_TUTORIAL_STATE_SETUP_USE_ITEM_SCREEN,
    CATCH_TUTORIAL_STATE_USE_ITEM_SCREEN,
};
u8 ov08_02222D84(BattleBag *battleBag);
u8 ov08_02222D90(BattleBag *battleBag);
u8 ov08_02222B8C(BattleBag *battleBag);
u8 ov08_02222EC4(BattleBag *battleBag);

s32 ov08_02222E2C(SysTask *task, BattleBag *battleBag) {
    BagCursor *cursor;
    u32 pocket;
    BattleBagContext *v0;
    BattleBagContext *v1;

    if (PaletteData_GetSelectedBuffersBitmask(battleBag->palette) != 0) {
        return 0;
    }
    ov08_02223F34(battleBag);
    ov08_02223464(battleBag);
    ov08_02223228(battleBag);
    ov08_022230CC(battleBag->background);
    battleBag->context->isCursorEnabled = ov08_02224B8C(battleBag->cursor);
    ov08_02224B7C(battleBag->cursor);
    FontID_Release(FONT_UNK_4);
    v0 = battleBag->context;
    if (v0->selectedBattleBagItem != 0) {
        cursor = BattleSystem_GetBagCursor(v0->battleSys);
        pocket = 0;
        do {
            v1 = battleBag->context;
            BagCursor_Battle_PocketSetPosition(cursor, pocket, v1->pocketCurrentPagePositions[pocket], v1->pocketCurrentPages[pocket]);
            pocket = (u32)(u8)(pocket + 1);
        } while (pocket < 5U);
        BagCursor_Battle_SetPocket(cursor, (u16) battleBag->currentBattlePocket);
    }
    battleBag->context->battleBagExited = 1;
    DestroySysTaskAndEnvironment(task);
    return 1;
}

u8 ov08_02222EC4(BattleBag *battleBag) {
    if (PaletteData_GetSelectedBuffersBitmask(battleBag->palette) != 0) {
        return 12;
    }

    switch (battleBag->catchTutorialState) {
    case CATCH_TUTORIAL_STATE_MENU_SCREEN:
        if (ov12_0226BD50(battleBag->catchTutorialCursor) == 1) {
            PlaySE(1501);
            battleBag->currentBattlePocket = BATTLE_POCKET_INDEX_POKE_BALLS;
            battleBag->queuedState = 12;
            ov08_02224938(battleBag, BATTLE_BAG_MENU_SCREEN_BUTTON_POKE_BALLS_POCKET, 0);
            battleBag->catchTutorialTickCount = 0;
            battleBag->catchTutorialState++;
            return 11;
        } else {
            battleBag->catchTutorialTickCount++;
        }
        break;
    case CATCH_TUTORIAL_STATE_SETUP_POCKET_MENU_SCREEN:
        ov08_02222D84(battleBag);
        battleBag->catchTutorialState++;
        break;
    case CATCH_TUTORIAL_STATE_POCKET_MENU_SCREEN:
        if (ov12_0226BD50(battleBag->catchTutorialCursor) == 1) {
            PlaySE(1501);
            battleBag->context->pocketCurrentPagePositions[battleBag->currentBattlePocket] = 0;
            battleBag->queuedState = 12;
            ov08_02224938(battleBag, 6 + BATTLE_BAG_POCKET_MENU_SCREEN_BUTTON_ITEM_1, 0);
            battleBag->catchTutorialTickCount = 0;
            battleBag->catchTutorialState++;
            return 11;
        } else {
            battleBag->catchTutorialTickCount++;
        }
        break;
    case CATCH_TUTORIAL_STATE_SETUP_USE_ITEM_SCREEN:
        ov08_02222D90(battleBag);
        battleBag->catchTutorialState++;
        break;
    case CATCH_TUTORIAL_STATE_USE_ITEM_SCREEN:
        if (ov12_0226BD50(battleBag->catchTutorialCursor) == 1) {
            PlaySE(1501);
            battleBag->context->selectedBattleBagItem = ov08_02223CD4(battleBag, battleBag->context->pocketCurrentPagePositions[battleBag->currentBattlePocket]);
            battleBag->context->selectedBattleBagPocket = battleBag->currentBattlePocket;
            ov08_02224938(battleBag, 15 + BATTLE_BAG_USE_ITEM_SCREEN_BUTTON_USE, 0);
            return ov08_02222B8C(battleBag);
        } else {
            battleBag->catchTutorialTickCount++;
        }
        break;
    }

    return 12;
}
