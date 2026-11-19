#include "global.h"

#include "bg_window.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

s32 ov81_022409B0(void *, void *, u8, s32, s32, s32, s32, s32, s32, void *);

void ov81_02242300(BattleFactoryApp *app, u32 arg1, u32 arg2) {
    u8 v0;
    u16 v1;
    u16 v4;

    if (arg1 < arg2) {
        v4 = 0;
        v0 = 0;
        if ((s32) * &app->numMonsSelected > 0) {
            do {
                v1 = *((u16 *)&app->selectedIndices[v4]);
                if (arg1 == v1) {
                    v0 = 1;
                }
                ov81_022409B0(app, &app->windows[v4 + 2], (u8)v1, 4, 0, 0xF, 2, 0, 0, *((void **)&app->personalParty));
                v4 += 1;
            } while ((s32)v4 < (s32) * &app->numMonsSelected);
        }
        if (v0 == 0) {
            ov81_022409B0(app, &app->windows[v4 + 2], (u8)arg1, 4, 0, 0xF, 2, 0, 0, *((void **)&app->personalParty));
            v4 += 1;
        }
        if ((u32)v4 < (u32) app->numMonsToFade) {
            do {
                FillWindowPixelBuffer(&app->windows[v4 + 2], 0U);
                ScheduleWindowCopyToVram(&app->windows[v4 + 2]);
                v4 += 1;
            } while ((u32)v4 < (u32) app->numMonsToFade);
        }
    }
}

void ov81_022423D0(BattleFactoryApp *app, struct Party *arg1) {
    u32 v0;

    v0 = 0;
    while ((u32)v0 < (u32) app->numMonsToFade) {
        ov81_022409B0(app, &app->windows[v0 + 2], v0, 4, 0, 0xF, 2, 0, 0, arg1);
        v0 += 1;
    }
}
