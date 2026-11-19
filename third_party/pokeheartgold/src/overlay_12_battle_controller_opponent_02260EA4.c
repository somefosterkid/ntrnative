#include "global.h"

#include "battle/battle_system.h"
#include "battle/battle.h"
#include "bg_window.h"
#include "filesystem.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_07_ball_rotation.h"
#include "overlay_12_battle_controller_opponent.h"

void ov12_02260EA4(BattleSystem *battleSystem, OpponentData *battlerData);

struct UnkBallData;
struct BattleMonOBJData;
typedef struct BattleMonOBJData BattleMonOBJData;
u8 ov12_02261258(OpponentData *battlerData);
u8 ov12_02261264(OpponentData *battlerData);
Pokepic *ov12_02261270(OpponentData *battlerData);
BattleHpBar *OpponentData_GetHpBar(OpponentData *battlerData);
OpponentDataCursorPos *ov12_02261280(OpponentData *battlerData);
void ov12_02261284(OpponentData *battlerData);

void ov12_02260EA4(BattleSystem *battleSystem, OpponentData *battlerData) {
    u32 v0;
    u32 v1;

    v1 = BattleSystem_GetBattleType(battleSystem);
    v0 = BattleSystem_GetBattleSpecial(battleSystem);
    if (0x40 & v1) {
        if (battlerData->battler != 0) {
            battlerData->taskFuncs.setCommandSelection = ov12_0225E104;
            battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E404;
            battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E6FC;
            battlerData->taskFuncs.showBagMenu = ov12_0225F3A4;
            battlerData->taskFuncs.showPartyMenu = ov12_0225F8AC;
            battlerData->taskFuncs.showYesNoMenu = ov12_0225FC80;
            battlerData->bootState = 1;
            return;
        }
        if (v0 & 0x10) {
            battlerData->taskFuncs.setCommandSelection = ov12_0225E1FC;
            battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E4EC;
            battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E760;
            battlerData->taskFuncs.showBagMenu = ov12_0225F434;
            battlerData->taskFuncs.showPartyMenu = ov12_0225F980;
            battlerData->taskFuncs.showYesNoMenu = ov12_0225FCC0;
            battlerData->bootState = 0;
            return;
        }
        battlerData->taskFuncs.setCommandSelection = ov12_0225DAD4;
        battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E250;
        battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E568;
        battlerData->taskFuncs.showBagMenu = ov12_0225E830;
        battlerData->taskFuncs.showPartyMenu = ov12_0225F4E0;
        battlerData->taskFuncs.showYesNoMenu = ov12_0225FA44;
        battlerData->bootState = 0;
        return;
    }
    if ((0x88 & v1) == 0x88) {
        if (1 & battlerData->battlerType) {
            if (BattleSystem_IsInitialized(battleSystem) != 0) {
                battlerData->taskFuncs.setCommandSelection = ov12_0225E104;
                battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E404;
                battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E6FC;
                battlerData->taskFuncs.showBagMenu = ov12_0225F3A4;
                battlerData->taskFuncs.showPartyMenu = ov12_0225F8AC;
                battlerData->taskFuncs.showYesNoMenu = ov12_0225FC80;
                battlerData->bootState = 1;
                return;
            }
            battlerData->taskFuncs.setCommandSelection = ov12_0225E134;
            battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E4CC;
            battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E740;
            battlerData->taskFuncs.showBagMenu = ov12_0225F3FC;
            battlerData->taskFuncs.showPartyMenu = ov12_0225F960;
            battlerData->taskFuncs.showYesNoMenu = ov12_0225FCA0;
            battlerData->bootState = 2;
            return;
        }
        if (v0 & 0x10) {
            battlerData->taskFuncs.setCommandSelection = ov12_0225E1FC;
            battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E4EC;
            battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E760;
            battlerData->taskFuncs.showBagMenu = ov12_0225F434;
            battlerData->taskFuncs.showPartyMenu = ov12_0225F980;
            battlerData->taskFuncs.showYesNoMenu = ov12_0225FCC0;
            battlerData->bootState = 0;
            return;
        }
        if (battlerData->battlerType != ov12_0223AB0C(battleSystem, ov12_0223BFC0(battleSystem) * 2)) {
            battlerData->taskFuncs.setCommandSelection = ov12_0225E134;
            battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E4CC;
            battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E740;
            battlerData->taskFuncs.showBagMenu = ov12_0225F3FC;
            battlerData->taskFuncs.showPartyMenu = ov12_0225F960;
            battlerData->taskFuncs.showYesNoMenu = ov12_0225FCA0;
            battlerData->bootState = 2;
            return;
        }
        battlerData->taskFuncs.setCommandSelection = ov12_0225DAD4;
        battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E250;
        battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E568;
        battlerData->taskFuncs.showBagMenu = ov12_0225E830;
        battlerData->taskFuncs.showPartyMenu = ov12_0225F4E0;
        battlerData->taskFuncs.showYesNoMenu = ov12_0225FA44;
        battlerData->bootState = 0;
        return;
    }
    if (8 & v1) {
        if (v0 & 0x10) {
            battlerData->taskFuncs.setCommandSelection = ov12_0225E1FC;
            battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E4EC;
            battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E760;
            battlerData->taskFuncs.showBagMenu = ov12_0225F434;
            battlerData->taskFuncs.showPartyMenu = ov12_0225F980;
            battlerData->taskFuncs.showYesNoMenu = ov12_0225FCC0;
            battlerData->bootState = 0;
            return;
        }
        if (battlerData->battlerType != ov12_0223AB0C(battleSystem, (s32)ov12_0223BFC0(battleSystem))) {
            battlerData->taskFuncs.setCommandSelection = ov12_0225E134;
            battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E4CC;
            battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E740;
            battlerData->taskFuncs.showBagMenu = ov12_0225F3FC;
            battlerData->taskFuncs.showPartyMenu = ov12_0225F960;
            battlerData->taskFuncs.showYesNoMenu = ov12_0225FCA0;
            battlerData->bootState = 2;
            return;
        }
        battlerData->taskFuncs.setCommandSelection = ov12_0225DAD4;
        battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E250;
        battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E568;
        battlerData->taskFuncs.showBagMenu = ov12_0225E830;
        battlerData->taskFuncs.showPartyMenu = ov12_0225F4E0;
        battlerData->taskFuncs.showYesNoMenu = ov12_0225FA44;
        battlerData->bootState = 0;
        return;
    }
    if (v1 & 4) {
        if (v0 & 0x10) {
            battlerData->taskFuncs.setCommandSelection = ov12_0225E1FC;
            battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E4EC;
            battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E760;
            battlerData->taskFuncs.showBagMenu = ov12_0225F434;
            battlerData->taskFuncs.showPartyMenu = ov12_0225F980;
            battlerData->taskFuncs.showYesNoMenu = ov12_0225FCC0;
            battlerData->bootState = 0;
            return;
        }
        if (1 & battlerData->battlerType) {
            battlerData->taskFuncs.setCommandSelection = ov12_0225E134;
            battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E4CC;
            battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E740;
            battlerData->taskFuncs.showBagMenu = ov12_0225F3FC;
            battlerData->taskFuncs.showPartyMenu = ov12_0225F960;
            battlerData->taskFuncs.showYesNoMenu = ov12_0225FCA0;
            battlerData->bootState = 2;
            return;
        }
        battlerData->taskFuncs.setCommandSelection = ov12_0225DAD4;
        battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E250;
        battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E568;
        battlerData->taskFuncs.showBagMenu = ov12_0225E830;
        battlerData->taskFuncs.showPartyMenu = ov12_0225F4E0;
        battlerData->taskFuncs.showYesNoMenu = ov12_0225FA44;
        battlerData->bootState = 0;
        return;
    }
    if (0x200 & v1) {
        if (battlerData->battlerType & 1) {
            battlerData->taskFuncs.setCommandSelection = ov12_0225E1D4;
            *((s32 **)&battlerData->taskFuncs.showMoveSelectMenu) = NULL;
            *((s32 **)&battlerData->taskFuncs.showTargetSelectMenu) = NULL;
            *((s32 **)&battlerData->taskFuncs.showBagMenu) = NULL;
            *((s32 **)&battlerData->taskFuncs.showPartyMenu) = NULL;
            *((s32 **)&battlerData->taskFuncs.showYesNoMenu) = NULL;
            battlerData->bootState = 1;
            return;
        }
        battlerData->taskFuncs.setCommandSelection = ov12_0225DAD4;
        *((s32 **)&battlerData->taskFuncs.showMoveSelectMenu) = NULL;
        *((s32 **)&battlerData->taskFuncs.showTargetSelectMenu) = NULL;
        *((s32 **)&battlerData->taskFuncs.showBagMenu) = NULL;
        *((s32 **)&battlerData->taskFuncs.showPartyMenu) = NULL;
        *((s32 **)&battlerData->taskFuncs.showYesNoMenu) = NULL;
        battlerData->bootState = 0;
        return;
    }
    if (0x20 & v1) {
        if (battlerData->battlerType & 1) {
            battlerData->taskFuncs.setCommandSelection = ov12_0225E154;
            *((s32 **)&battlerData->taskFuncs.showMoveSelectMenu) = NULL;
            *((s32 **)&battlerData->taskFuncs.showTargetSelectMenu) = NULL;
            *((s32 **)&battlerData->taskFuncs.showBagMenu) = NULL;
            *((s32 **)&battlerData->taskFuncs.showPartyMenu) = NULL;
            *((s32 **)&battlerData->taskFuncs.showYesNoMenu) = NULL;
            battlerData->bootState = 1;
            return;
        }
        battlerData->taskFuncs.setCommandSelection = ov12_0225DAD4;
        *((s32 **)&battlerData->taskFuncs.showMoveSelectMenu) = NULL;
        *((s32 **)&battlerData->taskFuncs.showTargetSelectMenu) = NULL;
        *((s32 **)&battlerData->taskFuncs.showBagMenu) = NULL;
        *((s32 **)&battlerData->taskFuncs.showPartyMenu) = NULL;
        battlerData->taskFuncs.showYesNoMenu = ov12_0225FA44;
        battlerData->bootState = 0;
        return;
    }
    if (battlerData->battlerType & 1) {
        battlerData->taskFuncs.setCommandSelection = ov12_0225E104;
        battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E404;
        battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E6FC;
        battlerData->taskFuncs.showBagMenu = ov12_0225F3A4;
        battlerData->taskFuncs.showPartyMenu = ov12_0225F8AC;
        battlerData->taskFuncs.showYesNoMenu = ov12_0225FC80;
        battlerData->bootState = 1;
        return;
    }
    if (v0 & 0x10) {
        battlerData->taskFuncs.setCommandSelection = ov12_0225E1FC;
        battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E4EC;
        battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E760;
        battlerData->taskFuncs.showBagMenu = ov12_0225F434;
        battlerData->taskFuncs.showPartyMenu = ov12_0225F980;
        battlerData->taskFuncs.showYesNoMenu = ov12_0225FCC0;
        battlerData->bootState = 0;
        return;
    }
    battlerData->taskFuncs.setCommandSelection = ov12_0225DAD4;
    battlerData->taskFuncs.showMoveSelectMenu = ov12_0225E250;
    battlerData->taskFuncs.showTargetSelectMenu = ov12_0225E568;
    battlerData->taskFuncs.showBagMenu = ov12_0225E830;
    battlerData->taskFuncs.showPartyMenu = ov12_0225F4E0;
    battlerData->taskFuncs.showYesNoMenu = ov12_0225FA44;
    battlerData->bootState = 0;
}

u8 ov12_02261258(OpponentData *battlerData) {
    return battlerData->battlerType;
}

u8 ov12_02261264(OpponentData *battlerData) {
    return battlerData->bootState;
}

Pokepic *ov12_02261270(OpponentData *battlerData) {
    if (battlerData->monSprite) {
        return battlerData->monSprite;
    } else {
        return battlerData->monSpriteBackup;
    }
}

BattleHpBar *OpponentData_GetHpBar(OpponentData *battlerData) {
    return &battlerData->healthbox;
}

OpponentDataCursorPos *ov12_02261280(OpponentData *battlerData) {
    return &battlerData->savedCursorPos;
}

void ov12_02261284(OpponentData *battlerData) {
    MI_CpuClear8(&battlerData->savedCursorPos, sizeof(OpponentDataCursorPos));
}
