#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

s32 sub_0208B400(void *);

void sub_0208DB58(PokemonSummary *);

s32 sub_0208A63C(PokemonSummary *arg0);

s32 sub_0208A63C(PokemonSummary *arg0) {
    BgConfig *temp_r0_2;
    u8 temp_r0;
    temp_r0 = arg0->unk7BE;
    switch (temp_r0) {
    case 0:
        thunk_Sprite_SetDrawFlag(arg0->sprites[11], 0);
        thunk_Sprite_SetDrawFlag(arg0->sprites[12], 0);
        thunk_Sprite_SetDrawFlag(arg0->sprites[18], 0);
        thunk_Sprite_SetDrawFlag(arg0->sprites[19], 0);
        Sprite_SetDrawFlag(arg0->sprites[9], 0);
        sub_0208DB58(arg0);
        sub_0208DAD4(arg0);
        arg0->unk7BE = 1U;
        break;

    case 1:
        temp_r0_2 = arg0->bgConfig;
        if (Bg_GetXpos(arg0->bgConfig, GF_BG_LYR_SUB_1) <= 0x80) {
            ScheduleSetBgPosText(arg0->bgConfig, 5U, BG_POS_OP_SET_ROT, 0);
            arg0->unk7BE = 2U;
        } else {
            ScheduleSetBgPosText(arg0->bgConfig, 5U, BG_POS_OP_SUB_ROT, 0x40);
        }
        break;

    case 2:
        ScheduleWindowCopyToVram(&arg0->windows[32]);
        ScheduleWindowCopyToVram(&arg0->windows[6]);
        ScheduleWindowCopyToVram(&arg0->windows[31]);
        ScheduleWindowCopyToVram(&arg0->windows[4]);
        Sprite_SetDrawFlag(arg0->sprites[9], 1);
        sub_0208B400(arg0);
        arg0->unk7BE = 0U;
        return 1;
    }

    return 0;
}
