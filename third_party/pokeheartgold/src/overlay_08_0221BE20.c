#include "global.h"

#include "battle/battle_party_context.h"
#include "battle/battle_system.h"

#include "sys_task.h"
#include "systask_environment.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"

static void ov08_0221BE98(SysTask *task, void *taskParam);

void ov08_0221BE20(BattlePartyContext *arg0) {
    BattleParty *v0;

    if ((u32)arg0->selectedPartyIndex > 5U) {
        arg0->selectedPartyIndex = 0U;
    }
    v0 = SysTask_GetData(CreateSysTaskAndEnvironment(ov08_0221BE98, sizeof(BattleParty), 0U, arg0->heapID));
    memset(v0, 0, sizeof(BattleParty));
    v0->context = arg0;
    v0->background = BattleSystem_GetBgConfig(arg0->battleSys);
    v0->palette = BattleSystem_GetPaletteData(arg0->battleSys);
    v0->currentState = 0;
    v0->partySlotLearningMove = arg0->selectedPartyIndex;
    v0->hasVisitedContestHall = 0;
    v0->battlerSlot = ov12_0223AB0C(arg0->battleSys, arg0->battler);
}

struct BattlePartyContext;

struct BattleSubMenuCursor;

struct MessagePrinter;

struct MsgData;

struct MessageFormat;

void ov08_022220FC(BattleParty *battleParty);

void ov08_0222145C(BattleParty *battleParty);

u8 ov08_0221C048(BattleParty *battleParty);

u8 ov08_0221C14C(BattleParty *battleParty);

u8 ov08_0221C318(BattleParty *battleParty);

u8 ov08_0221C3C8(BattleParty *battleParty);

u8 ov08_0221CA08(BattleParty *battleParty);

u8 ov08_0221CA20(BattleParty *battleParty);

u8 ov08_0221CA34(BattleParty *battleParty);

u8 ov08_0221CA50(BattleParty *battleParty);

s32 ov08_0221C9C8(BattleParty *battleParty);

u8 ov08_0221C58C(BattleParty *battleParty);

u8 ov08_0221C604(BattleParty *battleParty);

u8 ov08_0221C6F8(BattleParty *battleParty);

u8 ov08_0221C814(BattleParty *battleParty);

u8 ov08_0221CA78(BattleParty *battleParty);

s32 ov08_0221C918(BattleParty *battleParty);

s32 ov08_0221C924(BattleParty *battleParty);

s32 ov08_0221C93C(BattleParty *battleParty);

s32 ov08_0221C930(BattleParty *battleParty);

s32 ov08_0221C948(BattleParty *battleParty);

s32 ov08_0221C954(BattleParty *battleParty);

s32 ov08_0221C978(BattleParty *battleParty);

u8 ov08_0221C9A4(BattleParty *battleParty);

u8 ov08_0221CA90(BattleParty *battleParty);

u8 ov08_0221CC38(BattleParty *battleParty);

u8 ov08_0221CD64(BattleParty *battleParty);

BOOL ov08_0221CD90(SysTask *task, BattleParty *battleParty);

u8 ov08_0221C488(BattleParty *battleParty);

static void ov08_0221BE98(SysTask *task, void *taskParam) {
    BattleParty *battleParty = taskParam;

    switch (battleParty->currentState) {
    case 0:
        battleParty->currentState = ov08_0221C048(battleParty);
        break;
    case 1:
        battleParty->currentState = ov08_0221C14C(battleParty);
        break;
    case 2:
        battleParty->currentState = ov08_0221C318(battleParty);
        break;
    case 3:
        battleParty->currentState = ov08_0221C3C8(battleParty);
        break;
    case 4:
        battleParty->currentState = ov08_0221C488(battleParty);
        break;
    case 5:
        battleParty->currentState = ov08_0221C58C(battleParty);
        break;
    case 6:
        battleParty->currentState = ov08_0221C918(battleParty);
        break;
    case 7:
        battleParty->currentState = ov08_0221C924(battleParty);
        break;
    case 8:
        battleParty->currentState = ov08_0221C930(battleParty);
        break;
    case 9:
        battleParty->currentState = ov08_0221C93C(battleParty);
        break;
    case 10:
        battleParty->currentState = ov08_0221C948(battleParty);
        break;
    case 11:
        battleParty->currentState = ov08_0221C954(battleParty);
        break;
    case 12:
        battleParty->currentState = ov08_0221C978(battleParty);
        break;
    case 13:
        battleParty->currentState = ov08_0221C9A4(battleParty);
        break;
    case 14:
        battleParty->currentState = ov08_0221C9C8(battleParty);
        break;
    case 15:
        battleParty->currentState = ov08_0221CA08(battleParty);
        break;
    case 16:
        battleParty->currentState = ov08_0221CA20(battleParty);
        break;
    case 17:
        battleParty->currentState = ov08_0221CA34(battleParty);
        break;
    case 18:
        battleParty->currentState = ov08_0221CA50(battleParty);
        break;
    case 19:
        battleParty->currentState = ov08_0221C604(battleParty);
        break;
    case 20:
        battleParty->currentState = ov08_0221C6F8(battleParty);
        break;
    case 21:
        battleParty->currentState = ov08_0221C814(battleParty);
        break;
    case 22:
        battleParty->currentState = ov08_0221CA78(battleParty);
        break;
    case 23:
        battleParty->currentState = ov08_0221CA90(battleParty);
        break;
    case 24:
        battleParty->currentState = ov08_0221CC38(battleParty);
        break;
    case 25:
        battleParty->currentState = ov08_0221CD64(battleParty);
        break;

    case 26:
        if (ov08_0221CD90(task, battleParty) == 1) {
            return;
        }
    }

    ov08_0222145C(battleParty);
    SpriteSystem_DrawSprites(battleParty->spriteManager);
    ov08_022220FC(battleParty);
}
