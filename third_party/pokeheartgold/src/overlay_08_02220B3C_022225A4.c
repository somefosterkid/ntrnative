#include "global.h"

#include "battle/battle_system.h"

#include "bag_cursor.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"
#include "battle/battle_bag_context.h"

u8 ov08_0221D5DC(BattleParty *battleParty, s32);

s32 ov08_02221E6C(BattleParty *battleParty, u8, u8, u8);

void ov08_022225A4(BattleParty *battleParty);

void ov08_022225A4(BattleParty *battleParty) {
    if (ov08_0221D5DC(battleParty, battleParty->context->selectedPartyIndex) == 2) {
        ov08_02221E6C(battleParty, battleParty->context->selectedPartyIndex, 0, 1);
        return;
    }
    ov08_02221E6C(battleParty, battleParty->context->selectedPartyIndex, 0, 0);
}

struct BattleSubMenuCursor;

struct MessagePrinter;

struct Indicator;

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

typedef struct Indicator Indicator;

typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;

BOOL ov08_02223B78(BattleBag *battleBag);

static void ov08_02222670(SysTask *task, void *taskParam);

void ov08_022225D4(BattleBagContext *context);

void ov08_022225D4(BattleBagContext *context) {
    BattleBag *battleBagTask = SysTask_GetData(CreateSysTaskAndEnvironment(ov08_02222670, sizeof(BattleBag), 100, context->heapID));
    memset(battleBagTask, 0, sizeof(BattleBag));

    battleBagTask->context = context;
    battleBagTask->background = BattleSystem_GetBgConfig(context->battleSys);
    battleBagTask->palette = BattleSystem_GetPaletteData(context->battleSys);
    battleBagTask->currentState = 0;

    BagCursor *cursor = BattleSystem_GetBagCursor(context->battleSys);

    for (u8 i = 0; i < 5; i++) {
        BagCursor_Battle_PocketGetPosition(cursor, i, &battleBagTask->context->pocketCurrentPagePositions[i], &battleBagTask->context->pocketCurrentPages[i]);
    }

    battleBagTask->context->lastUsedItem = BagCursor_Battle_GetLastUsedItem(cursor);
    battleBagTask->context->lastUsedItemPocket = BagCursor_Battle_GetLastUsedPocket(cursor);

    ov08_02223B78(battleBagTask);

    if (BattleSystem_GetBattleType(battleBagTask->context->battleSys) & (1 << 10)) {
        battleBagTask->context->isInCatchTutorial = 1;
    }
}

void ov08_02224974(BattleBag *battleBag);

u8 ov08_0222276C(BattleBag *battleBag);

u8 ov08_02222840(BattleBag *battleBag);

u32 ov08_02222918(BattleBag *battleBag);

s32 ov08_02222AF0(BattleBag *battleBag);

u8 ov08_02222D78(BattleBag *battleBag);

u8 ov08_02222D84(BattleBag *battleBag);

u8 ov08_02222D90(BattleBag *battleBag);

u8 ov08_02222A78(BattleBag *battleBag);

u8 ov08_02222D9C(BattleBag *battleBag);

u8 ov08_02222DAC(BattleBag *battleBag);

u8 ov08_02222DC4(BattleBag *battleBag);

u8 ov08_02222DEC(BattleBag *battleBag);

u8 ov08_02222EC4(BattleBag *battleBag);

u8 ov08_02222E04(BattleBag *battleBag);

BOOL ov08_02222E2C(SysTask *task, BattleBag *battleBag);

static void ov08_02222670(SysTask *task, void *taskParam) {
    BattleBag *battleBag = taskParam;

    switch (battleBag->currentState) {
    case 0:
        battleBag->currentState = ov08_0222276C(battleBag);
        break;
    case 1:
        battleBag->currentState = ov08_02222840(battleBag);
        break;
    case 2:
        battleBag->currentState = ov08_02222918(battleBag);
        break;
    case 3:
        battleBag->currentState = ov08_02222AF0(battleBag);
        break;
    case 4:
        battleBag->currentState = ov08_02222D78(battleBag);
        break;
    case 5:
        battleBag->currentState = ov08_02222D84(battleBag);
        break;
    case 6:
        battleBag->currentState = ov08_02222D90(battleBag);
        break;
    case 7:
        battleBag->currentState = ov08_02222A78(battleBag);
        break;
    case 8:
        battleBag->currentState = ov08_02222D9C(battleBag);
        break;
    case 9:
        battleBag->currentState = ov08_02222DAC(battleBag);
        break;
    case 10:
        battleBag->currentState = ov08_02222DC4(battleBag);
        break;
    case 11:
        battleBag->currentState = ov08_02222DEC(battleBag);
        break;
    case 12:
        battleBag->currentState = ov08_02222EC4(battleBag);
        break;
    case 13:
        battleBag->currentState = ov08_02222E04(battleBag);
        break;
    case 14:
        if (ov08_02222E2C(task, battleBag) == 1) {
            return;
        }
    }

    ov08_02224974(battleBag);
    SpriteSystem_DrawSprites(battleBag->spriteManager);
}
