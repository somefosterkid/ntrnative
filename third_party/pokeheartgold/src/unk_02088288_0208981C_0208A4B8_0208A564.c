#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "wild_encounter.h"
#include "pokemon_summary_app.h"

void sub_0208C068(PokemonSummary *arg0);

void sub_0208DB1C(PokemonSummary *summaryScreen);

s32 sub_0208A79C(PokemonSummary *);

s32 sub_0208A564(PokemonSummary *arg0);

s32 sub_0208A564(PokemonSummary *arg0) {
    u8 temp_r1;

    temp_r1 = arg0->unk7BE;
    switch (temp_r1) {
    case 0:
        Sprite_SetDrawFlag(arg0->sprites[9], 0);
        Sprite_SetAnimCtrlSeq(arg0->sprites[9], 0);
        ScheduleSetBgPosText(arg0->bgConfig, 5U, BG_POS_OP_SET_ROT, 0);
        ScheduleSetBgPosText(arg0->bgConfig, 5U, BG_POS_OP_SET_XSCALE, 0);
        ClearWindowTilemapAndScheduleTransfer(&arg0->windows[4]);
        arg0->unk7BE = 1U;
        break;
    case 1:
        if (Bg_GetXpos(arg0->bgConfig, GF_BG_LYR_SUB_1) >= 0x80) {
            ScheduleSetBgPosText(arg0->bgConfig, 5U, BG_POS_OP_SET_ROT, 0x80);
            arg0->unk7BE = 2U;
        } else {
            ScheduleSetBgPosText(arg0->bgConfig, 5U, BG_POS_OP_ADD_ROT, 0x40);
        }
        break;
    case 2:
        sub_0208DB1C(arg0);
        sub_0208A79C(arg0);
        thunk_Sprite_SetDrawFlag(arg0->sprites[11], 1);
        sub_0208C068(arg0);
        if (arg0->mon.type1 != arg0->mon.type2) {
            thunk_Sprite_SetDrawFlag(arg0->sprites[12], 1);
        }
        Sprite_SetDrawFlag(arg0->sprites[9], 1);
        arg0->unk7BE = 0U;
        return 1;
    }
    return 0;
}
