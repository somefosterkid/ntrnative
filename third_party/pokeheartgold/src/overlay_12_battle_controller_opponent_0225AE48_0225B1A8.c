#include "global.h"

#include "battle/battle_controller.h"
#include "battle/battle_display.h"
#include "battle/battle_system.h"

#include "render_window.h"

void ov12_0225B1A8(BattleSystem *battleSystem, OpponentData *arg1) {
    BattleMessage msg;
    MsgData *data;

    if (arg1->bootState == 0) {
        data = BattleSystem_GetMessageLoader(battleSystem);
        msg.id = 0x39B;
        msg.tag = 0;
        BattleSystem_PrintBattleMessage(battleSystem, data, &msg, 0U);
        ov12_0223BB80(battleSystem, WaitingIcon_New(BattleSystem_GetWindow(battleSystem, 0), 1U));
    }
    BattleController_EmitClearCommand(battleSystem, (s32)arg1->battler, 0x37);
}
