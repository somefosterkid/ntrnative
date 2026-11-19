#include "global.h"

#include "pokemon_summary_app.h"
#include "unk_02088288.h"

#include "touchscreen.h"

extern s32 _02104FD0;

extern TouchscreenHitbox _02104FFC;

u32 sub_0208E4DC(PokemonSummary *arg0);

u32 sub_0208E4DC(PokemonSummary *arg0) {
    s32 v0;

    if (arg0->unk7BF_0 == 0xF) {
        return -1U;
    }
    if (arg0->args->unk11 != 2) {
        v0 = TouchscreenHitbox_FindRectAtTouchNew(&_02104FFC);
        if (v0 != -1) {
            return *(u8 *)(&_02104FD0 + v0);
        }
    } else {
        v0 = TouchscreenHitbox_FindRectAtTouchNew(&_02104FFC);
        if ((v0 >= 4) && (v0 <= 9)) {
            return -1U;
        }
        if (v0 != -1) {
            v0 = (s32) * (u8 *)(&_02104FD0 + v0);
        }
    }
    return v0;
}
