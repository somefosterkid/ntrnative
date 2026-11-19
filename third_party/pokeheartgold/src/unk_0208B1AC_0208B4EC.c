#include "global.h"

#include "sprite.h"
#include "unk_0208B1AC.h"
#include "move.h"
#include "pokemon_summary_app.h"


s32 sub_0208BA60();
s32 sub_0208BA88(PokemonSummary *, s32, s32, u8);
s32 sub_0208BE70(void *);

void sub_0208B4EC(PokemonSummary *arg0) {
    u32 v0;

    v0 = arg0->mon.status;
    if (v0 == 7) {
        Sprite_SetDrawFlag(arg0->sprites[20], 0);
        Sprite_SetDrawFlag(arg0->sprites[72], 0);
        return;
    }
    if (v0 != 0) {
        Sprite_SetDrawFlag(arg0->sprites[72], 0);
        Sprite_SetDrawFlag(arg0->sprites[20], 1);
        Sprite_SetAnimCtrlSeq(arg0->sprites[20], arg0->mon.status);
        return;
    }
    Sprite_SetDrawFlag(arg0->sprites[72], 1);
    Sprite_SetDrawFlag(arg0->sprites[20], 0);
}

void sub_0208B55C(PokemonSummary *arg0) {
    u16 moveId;
    u16 v0;

    sub_0208BA60();
    v0 = 0;
    do {
        moveId = arg0->mon.moves[v0];
        if (moveId != 0) {
            sub_0208BA88(arg0, (u8)(v0 + 0xD), (u8)(v0 + 5), (u8)GetMoveAttr(moveId, MOVEATTR_TYPE));
        }
        v0 += 1;
    } while ((u32)v0 < 4U);
    sub_0208BE70(arg0);
}
