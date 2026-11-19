#include "global.h"
struct BattleFactoryApp;

#include "bg_window.h"
#include "message_format.h"
#include "overlay_81.h"
#include "pokemon_types_def.h"
#include "overlay_81_battle_factory_app.h"

u8 ov81_0224086C(BattleFactoryApp *app, s32 param_2) {
    u8 uVar1;

    uVar1 = ov81_0224080C(app, &app->windows[7], param_2, 1, 1, 0xff, 1, 2, 0xf, 1);
    ScheduleWindowCopyToVram(&app->windows[7]);
    return uVar1;
}

void ov81_022408A0(BattleFactoryApp *arg0, u32 idx, s32 num) {
    BufferIntegerAsString(*((MessageFormat * *)&arg0->strTemplate), idx, num, 1U, PRINTING_MODE_LEFT_ALIGN, 1);
}

void ov81_022408B8(BattleFactoryApp *app, u32 fieldno, BoxPokemon *boxmon) {
    BufferBoxMonSpeciesName(app->strTemplate, fieldno, boxmon);
}
