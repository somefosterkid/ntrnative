#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "unk_0208B1AC.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

void sub_0208BBDC(PokemonSummary *arg0) {
    u32 v0;

    v0 = arg0->moveSlot;
    switch (v0) {
    case 4:
        if (arg0->args->unk12 == 2) {
            Sprite_SetPositionXY(arg0->sprites[9], 0x44, (s16)((v0 << 5) + 0x28));
        } else {
            Sprite_SetPositionXY(arg0->sprites[9], 0x44, (s16)((v0 << 5) + 0x18));
        }
        Sprite_SetAnimCtrlSeq(arg0->sprites[9], 0);
        return;
    case 5:
        if (arg0->args->unk12 == 2) {
            Sprite_SetPositionXY(arg0->sprites[9], 0xDC, 0xB0);
            Sprite_SetAnimCtrlSeq(arg0->sprites[9], 3);
            return;
        }
        return;
    default:
        Sprite_SetPositionXY(arg0->sprites[9], 0x44, (s16)((v0 << 5) + 0x18));
        Sprite_SetAnimCtrlSeq(arg0->sprites[9], 0);
        break;
    }
}
