#include "global.h"

#include "bg_window.h"

/* HDR-OVERLAY {"ov18_021E7698": {"file": "include/application/pokedex/pokedex_internal.h", "new": "s8 ov18_021E7698(int a0);", "old": "u8 ov18_021E7698(int a0);"}} */
#define ov18_021E7698 ov18_021E7698__hdr

/* HDR-OVERLAY defines done */
#include "application/pokedex/pokedex_internal.h"

/* HDR-OVERLAY end */
#undef ov18_021E7698

#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov18_021E7698(int a0);
#else
s8 ov18_021E7698(int a0);
#endif

/* HDR-OVERLAY done */
s32 ov18_021EF388(PokedexAppData *, s32, s32);

s32 ov18_021F9648(Window *, MsgData *, s32, s32, s32, s32, s32, s32);

void ov18_021EF764(PokedexAppData *pokedexApp);

void ov18_021EF764(PokedexAppData *pokedexApp) {
    s32 sp10;
    s32 var_r4;
    s32 var_r6;
    s32 temp_r7;

    ov18_021E613C(pokedexApp, 0U);
    FillWindowPixelBuffer(&pokedexApp->windows[0x41], 0U);
    FillWindowPixelBuffer(&pokedexApp->windows[0x43], 0U);
    ov18_021F9648(&pokedexApp->windows[0x41], pokedexApp->msgData, 0x1B, 0x18, 0, 0, 0x20100, 2);
    var_r4 = 0;
    do {
        temp_r7 = (u16)ov18_021E7698(var_r4);
        if (var_r4 == 0x1A) {
            var_r6 = 0x71;
        } else {
            var_r6 = var_r4 + 0x45;
        }
        sp10 = (s32)temp_r7 % 7;
        ov18_021F9648(&pokedexApp->windows[0x43], pokedexApp->msgData, var_r6, (sp10 << 5) + 0x18, ((s32)temp_r7 / 7) << 5, 4, 0x20100, 2);
        var_r4 = (s32)(u16)(var_r4 + 1);
    } while ((u32)var_r4 < 0x1BU);
    ov18_021EF388(pokedexApp, 0x11, 0x27);
    ov18_021EF388(pokedexApp, 0x13, 0x28);
    ov18_021EFC3C(pokedexApp, 0x42);
    ScheduleWindowCopyToVram(&pokedexApp->windows[0x41]);
    ScheduleWindowCopyToVram(&pokedexApp->windows[0x43]);
    ScheduleWindowCopyToVram(&pokedexApp->windows[0x42]);
}
