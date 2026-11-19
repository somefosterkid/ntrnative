#include "global.h"
struct PokemonSummary;

#include "bg_window.h"
#include "sprite.h"
#include "unk_0201956C.h"
#include "pokemon_summary_app.h"
#include "pokemon_summary_args.h"
#include "unk_02088288.h"
#include "unk_0208C3E4.h"

void sub_0208AF70(PokemonSummary *arg0);

void sub_0208AF08(PokemonSummary *arg0, s32 arg1);

void sub_0208AFA0(struct PokemonSummary *arg0, s32 arg1);

void sub_0208AF08(PokemonSummary *arg0, s32 arg1) {
    if (arg1 == 1) {
        sub_020196E8(arg0->cellManager, 0, 0, 0x12);
        Sprite_SetDrawFlag(arg0->sprites[17], 0);
        sub_0208DC68(arg0, 1);
        return;
    }
    if (arg0->args->moveToLearn != 0) {
        sub_020196E8(arg0->cellManager, 1, 0, 0x12);
        Sprite_SetDrawFlag(arg0->sprites[17], 1);
    } else {
        sub_0208AF70(arg0);
    }
    sub_0208DC68(arg0, 0);
}

void sub_0208AF70(PokemonSummary *arg0) {
    FillBgTilemapRect(arg0->bgConfig, 6U, 0x3006U, 0U, 0x12U, 0x11U, 6U, 0x11U);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 6U);
}

void sub_0208AFA0(PokemonSummary *arg0, s32 arg1) {
    if (arg0->args->unk12 != 1) {
        if (arg1 == 1) {
            sub_020196E8(arg0->cellManager, 2, 1, 0x10);
            sub_0208DD20(arg0, 1);
            return;
        }
        sub_020196E8(arg0->cellManager, 3, 1, 0x10);
        sub_0208DD20(arg0, 0);
    }
}
