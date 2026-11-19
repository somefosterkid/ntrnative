#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "sprite.h"
#include "sprite_system.h"
#include "pokemon_summary_app.h"

s32 sub_0208AB58(void *, s32 arg1);

s32 sub_0208A9C4(PokemonSummary *arg0);

s32 sub_0208A9C4(PokemonSummary *arg0) {
    BgConfig *temp_r0_2;
    u8 temp_r0;
    u8 *new_var;
    temp_r0 = arg0->unk7BE;
    switch (temp_r0) {
    case 0:
        ScheduleSetBgPosText(arg0->bgConfig, 5U, BG_POS_OP_SET_ROT, 0);
        ScheduleSetBgPosText(arg0->bgConfig, 5U, BG_POS_OP_SET_XSCALE, 0);
        Sprite_SetPriority(arg0->sprites[9], 3U);
        thunk_Sprite_SetPaletteOverride(arg0->sprites[9], 0);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[29]);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[23]);
        new_var = &arg0->ribbonRow;
        ClearWindowTilemapAndScheduleTransfer(arg0->pageWindows);
        *new_var = 0;
        arg0->unk7BE = 1U;
        break;

    case 1:
        temp_r0_2 = arg0->bgConfig;
        if (Bg_GetYpos(arg0->bgConfig, GF_BG_LYR_SUB_1) >= 0x48) {
            ScheduleSetBgPosText(arg0->bgConfig, 5U, BG_POS_OP_SET_XSCALE, 0x48);
            arg0->unk7BE = 2U;
        } else {
            ScheduleSetBgPosText(arg0->bgConfig, 5U, BG_POS_OP_ADD_XSCALE, 0x24);
        }
        break;

    case 2:
        Sprite_SetDrawFlag(arg0->sprites[63], 1);
        Sprite_SetDrawFlag(arg0->sprites[66], 1);
        sub_0208AB58(arg0, 0);
        arg0->unk7BE = 0U;

        return 1;
    }

    return 0;
}
