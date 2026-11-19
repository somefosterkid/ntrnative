#include "global.h"

#include "overlay_81.h"
#include "unk_02019BA4.h"
#include "options.h"
#include "overlay_81_battle_factory_app.h"

void ov81_0223FB3C(BattleFactoryApp *app) {
    *((u8 *)((u8 *)&app->numPokeballs + 0x1)) = (u8)(*((u8 *)((u8 *)&app->numPokeballs + 0x1)) & ~4);
    app->numMonsSelected = (u8)(app->numMonsSelected - 1);
    *((s16 *)&app->selectedIndices[0]) = 0;
    app->unk_468 = 0;
    GridInputHandler_SetNextInput(app->inputHandler, 0);
}

void ov81_0223FB64(BattleFactoryApp *app) {
    *((u8 *)((u8 *)&app->numPokeballs + 0x1)) = (u8)(8 | *((u8 *)((u8 *)&app->numPokeballs + 0x1)));
    *((s16 *)&app->selectedIndices[(app->numMonsSelected)]) = (s16) app->unk_468;
    app->numMonsSelected = (u8)(app->numMonsSelected + 1);
}

void ov81_0223FB88(BattleFactoryApp *app) {
    ov81_02243028(&app->windows[7], Options_GetFrame(app->options));
    *((s8 *)&app->printerID) = ov81_0224086C(app, 0xD);
}
