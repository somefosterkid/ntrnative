#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_0208B1AC.h"
#include "pokemon_summary_app.h"

s32 sub_0208AA9C(PokemonSummary *arg0);

s32 sub_0208AA9C(PokemonSummary *arg0) {
    BgConfig *temp_r0_2;
    u8 temp_r0;
    temp_r0 = arg0->unk7BE;
    switch (temp_r0) {
    case 0:
        *((s8 *)&arg0->ribbonCursor) = 0;
        *((s8 *)&arg0->ribbonRow) = 0;
        ClearWindowTilemapAndScheduleTransfer(&arg0->pageWindows[1]);
        ClearWindowTilemapAndScheduleTransfer(&arg0->pageWindows[2]);
        sub_0208C2A0(arg0);
        arg0->unk7BE = 1U;
        break;

    case 1:
        temp_r0_2 = arg0->bgConfig;
        if (Bg_GetYpos(arg0->bgConfig, GF_BG_LYR_SUB_1) <= 0) {
            ScheduleSetBgPosText(arg0->bgConfig, 5U, BG_POS_OP_SET_XSCALE, 0);
            arg0->unk7BE = 2U;
        } else {
            ScheduleSetBgPosText(arg0->bgConfig, 5U, BG_POS_OP_SUB_XSCALE, 0x24);
        }
        break;

    case 2:
        ScheduleWindowCopyToVram(&arg0->windows[29]);
        ScheduleWindowCopyToVram(&arg0->windows[23]);
        ScheduleWindowCopyToVram(arg0->pageWindows);
        Sprite_SetPriority(arg0->sprites[9], 0U);
        thunk_Sprite_SetPaletteOverride(arg0->sprites[9], 2);
        arg0->unk7BE = 0U;
        return 1;
    }

    return 0;
}
