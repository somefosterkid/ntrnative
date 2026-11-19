#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "sprite_system.h"
#include "pokemon_summary_app.h"

s32 sub_0208A79C(void *);

s32 sub_0208C068(PokemonSummary *);

s32 sub_0208DB1C(void *);

void sub_0208A8F4(PokemonSummary *arg0);

void sub_0208A8F4(PokemonSummary *arg0) {
    ScheduleSetBgPosText(arg0->bgConfig, 2U, BG_POS_OP_SET_ROT, 0);
    sub_0208DB1C(arg0);
    sub_0208A79C(arg0);
    thunk_Sprite_SetDrawFlag(arg0->sprites[11], 1);
    sub_0208C068(arg0);
    if (arg0->mon.type1 != arg0->mon.type2) {
        thunk_Sprite_SetDrawFlag(arg0->sprites[12], 1);
    }
    Sprite_SetDrawFlag(arg0->sprites[9], 1);
}

s32 sub_0208C0E8(void *);

s32 sub_0208C208(void *);

void sub_0208A950(PokemonSummary *arg0);

void sub_0208A950(PokemonSummary *arg0) {
    sub_0208DB1C(arg0);
    sub_0208A79C(arg0);
    sub_0208C068(arg0);
    if (arg0->page == 1) {
        sub_0208C208(arg0);
        thunk_Sprite_SetDrawFlag(arg0->sprites[11], 1);
        if (arg0->mon.type1 != arg0->mon.type2) {
            thunk_Sprite_SetDrawFlag(arg0->sprites[12], 1);
        }
        ScheduleSetBgPosText(arg0->bgConfig, 2U, BG_POS_OP_SET_XSCALE, 0);
        return;
    }
    sub_0208C0E8(arg0);
    ScheduleSetBgPosText(arg0->bgConfig, 2U, BG_POS_OP_SET_XSCALE, 0x100);
}
