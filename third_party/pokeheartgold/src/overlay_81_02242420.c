#include "global.h"
struct BattleFactoryApp;
#include "bg_window.h"
#include "overlay_81.h"
#include "palette.h"
#include "overlay_81_battle_factory_app.h"

s32 ov81_022409B0(void *, void *, u8, s32, s32, s32, s32, s32, s32, Party *);

void ov81_02242500(struct BattleFactoryApp *arg0);

void ov81_02242420(BattleFactoryApp *app, u16 arg1) {
    u32 v1;
    short new_var;
    v1 = 0;
    new_var = 0U;
    if (((u32)(app->numMonsToFade)) > new_var) {
        do {
            if (app->selectedIndices[0] == v1) {
                ov81_022409B0(app, &app->windows[v1 + 2], arg1, 4, 0, 0xF, 2, 0, 0, app->receivableParty);
            } else {
                ov81_022409B0(app, &app->windows[v1 + 2], v1, 4, 0, 0xF, 2, 0, 0, app->personalParty);
            }
            v1 += 1;
        } while (((u32)v1) < ((u32)(app->numMonsToFade)));
    }
}

void ov81_022424AC(BattleFactoryApp *app, u32 arg1) {
    s32 v0;

    v0 = app->selectedIndices[0] + 2;
    if (arg1 >= 4U) {
        ClearWindowTilemapAndScheduleTransfer(&app->windows[v0]);
        return;
    }
    ov81_022409B0(app, &app->windows[v0], (u8)arg1, 4, 0, 0xF, 2, 0, 0, app->receivableParty);
    ScheduleWindowCopyToVram(&app->windows[v0]);
}

void ov81_02242500(BattleFactoryApp *arg0) {
    PaletteData_LoadPaletteSlotFromHardware(arg0->plttData, PLTTBUF_MAIN_OBJ, 0U, 0x200U);
}

void ov81_02242514(BattleFactoryApp *app) {
    PaletteData *data;

    data = app->plttData;
    if ((data != NULL) && (app->unk_478 != 0xFF) && (PaletteData_GetSelectedBuffersBitmask(data) == 0)) {
        if (app->unk_478 == 0) {
            PaletteData_BeginPaletteFade(app->plttData, 4U, 8U, 2, 0U, 0x10U, 0xFFFFU);
        } else {
            PaletteData_BeginPaletteFade(app->plttData, 4U, 8U, 2, 0x10U, 0U, 0xFFFFU);
        }
        app->unk_478 = (s32)(1 ^ app->unk_478);
    }
}
