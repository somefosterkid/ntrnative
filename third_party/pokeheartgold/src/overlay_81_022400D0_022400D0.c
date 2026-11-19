#include "global.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

#include "bg_window.h"
#include "list_menu_2d.h"
#include "options.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "text.h"

void ov80_0222A7CC(MessageFormat *template, u32 idx);


void ov81_022413E0(Window *window);

void *ov81_02242FC8(BattleFactoryAppPokeballSprite *ballSprite);

u32 ov81_022400D0(BattleFactoryApp *app);

u32 ov81_022400D0(BattleFactoryApp *app) {
    u8 printerID;
    u8 textFrameDelay;
    Pokemon *mon;
    BoxPokemon *boxMon;
    u32 frame;
    Options *options;

    switch (app->subState) {
    case 0:
        if (app->unk_37C != NULL) {
            Delete2dMenu(app->menu, NULL);
            ov81_022413E0(app->unk_174);
            ov81_02242FC8(app->unk_37C);
            app->unk_37C = NULL;
        }
        ov80_0222A7CC(app->strTemplate, 0);
        mon = Party_GetMonByIndex(app->personalParty, app->selectedIndices[0] + 2);
        boxMon = Mon_GetBoxMon(mon);
        ov81_022408B8(app, 1, boxMon);
        mon = Party_GetMonByIndex(app->receivableParty, app->selectedIndices[1]);
        boxMon = Mon_GetBoxMon(mon);
        ov81_022408B8(app, 2, boxMon);
        frame = Options_GetFrame(app->options);
        ov81_02243028(&app->windows[7], frame);
        options = Save_PlayerData_GetOptionsAddr(app->saveData);
        textFrameDelay = Options_GetTextFrameDelay(options);
        printerID = ov81_0224080C(app, &app->windows[7], 0x10, 1, 1, textFrameDelay, 1, 2, 0xf, 1);
        app->printerID = printerID;
        ScheduleWindowCopyToVram(&app->windows[7]);
        app->animationTimer = 0x50;
        app->subState++;
        break;
    case 1:
        textFrameDelay = TextPrinterCheckActive(app->printerID);
        if (textFrameDelay == 0) {
            app->animationTimer = 0;
            return 1;
        }
        break;
    }
    return 0;
}
