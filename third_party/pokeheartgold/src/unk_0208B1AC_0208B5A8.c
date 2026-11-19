#include "global.h"

#include "move.h"
#include "sprite_system.h"
#include "unk_0208B1AC.h"
#include "sprite.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

s32 sub_0208BA88(PokemonSummary *, s32, s32, u8);

void sub_0208B5A8(PokemonSummary *arg0) {
    s8 v0;
    u16 moveId;
    u16 v1;
    u16 v3;
    u16 v4;

    v3 = 0xB;
    do {
        thunk_Sprite_SetDrawFlag(arg0->sprites[v3], 0);
        v3 += 1;
    } while ((u32)v3 <= 0x13U);
    if (arg0->mon.isEgg != 0) {
        return;
    }
    v0 = arg0->page;
    switch (v0) {
    case 0:
        thunk_Sprite_SetDrawFlag(arg0->sprites[11], 1);
        if (arg0->mon.type1 == arg0->mon.type2) {
            Sprite_SetPositionXY(arg0->sprites[11], 0x6C, 0x30);
            return;
        }
        Sprite_SetPositionXY(arg0->sprites[11], 0x5B, 0x30);
        thunk_Sprite_SetDrawFlag(arg0->sprites[12], 1);
        Sprite_SetPositionXY(arg0->sprites[12], 0x7D, 0x30);
        return;
    case 1:
        v4 = 0;
        do {
            moveId = arg0->mon.moves[v4];
            if (moveId != 0) {
                sub_0208BA88(arg0, (u8)(v4 + 0xD), (u8)(v4 + 5), (u8)GetMoveAttr(moveId, MOVEATTR_TYPE));
                thunk_Sprite_SetDrawFlag(arg0->sprites[v4 + 13], 1);
                Sprite_SetPositionXY(arg0->sprites[v4 + 13], 0x18, (s16)((v4 << 5) + 0x10));
            }
            v4 += 1;
        } while ((u32)v4 < 4U);
        v1 = arg0->args->moveToLearn;
        if (v1 != 0) {
            sub_0208BA88(arg0, 0x11U, 9U, (u8)GetMoveAttr(v1, MOVEATTR_TYPE));
            thunk_Sprite_SetDrawFlag(arg0->sprites[17], 1);
            Sprite_SetPositionXY(arg0->sprites[17], 0x18, 0xA0);
        } else {
            thunk_Sprite_SetDrawFlag(arg0->sprites[17], 0);
        }
        Sprite_SetPositionXY(arg0->sprites[11], 0xC8, 0x18);
        Sprite_SetPositionXY(arg0->sprites[12], 0xEA, 0x18);
        Sprite_SetPositionXY(arg0->sprites[18], 0xE8, 0x28);
        return;
    }
}

void sub_0208B714(PokemonSummary *arg0) {
    s32 v1;
    s32 v3;

    v3 = 0;
    do {
        Sprite_SetDrawFlag(arg0->sprites[v3 + 79], 0);
        v3 += 1;
    } while (v3 < 0x19);
    v1 = 0;
    do {
        Sprite_SetDrawFlag(arg0->sprites[v1 + 104], 0);
        v1 += 1;
    } while (v1 < 5);
}

void sub_0208B74C(PokemonSummary *arg0) {
    s32 v0;

    v0 = 0;
    do {
        Sprite_SetDrawFlag(arg0->sprites[v0 + 109], 0);
        Sprite_SetAnimActiveFlag(arg0->sprites[v0 + 109], 1);
        Sprite_SetDrawPriority(arg0->sprites[v0 + 109], 6 - v0);
        v0 += 1;
    } while (v0 < 6);
}
