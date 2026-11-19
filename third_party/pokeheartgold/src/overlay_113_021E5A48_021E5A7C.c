#include "global.h"
#include "overlay_113.h"

#include "menu_input_state.h"
#include "options.h"
#include "player_data.h"
#include "pokedex.h"
#include "save_vars_flags.h"
#include "sys_vars.h"

int ov113_021E5A48(Pokedex *, u8 *);

u8 ov113_021E5D80(UnownReportData *);

void ov113_021E5A7C(UnownReportData *data);

void ov113_021E5A7C(UnownReportData *data) {
    u8 extra[2];
    Options *options;
    s32 i;
    u8 n;

    data->menuInputState = MenuInputStateMgr_GetState(data->args->menuInputStatePtr);
    options = Save_PlayerData_GetOptionsAddr(data->args->saveData);
    data->textFrameDelay = Options_GetTextFrameDelay(options);
    data->frame = Options_GetFrame(options);
    data->pokedex = Save_Pokedex_Get(data->args->saveData);
    data->reportLevel = Save_VarsFlags_GetUnownReportLevel(Save_VarsFlags_Get(data->args->saveData));
    data->numExtraPages = ov113_021E5D80(data);
    data->numFormsSeen = Pokedex_GetSeenFormNum_Unown(data->pokedex, 1);
    if (data->reportLevel >= 4) {
        for (i = 0; i <= 25; i++) {
            data->forms[i] = i;
        }
        n = ov113_021E5A48(data->pokedex, extra);
        for (i = 0; i < n; i++) {
            data->forms[26 + i] = extra[i];
        }
    } else {
        for (i = 0; i < data->numFormsSeen; i++) {
            data->forms[i] = Pokedex_GetSeenFormByIdx_Unown(data->pokedex, i, 1);
        }
    }
    data->numFormPages = data->numFormsSeen / 14;
    if (data->numFormsSeen != 0 && data->numFormsSeen % 14 != 0) {
        data->numFormPages++;
    }
    data->numPages = data->numFormPages + data->numExtraPages + 1;
}
