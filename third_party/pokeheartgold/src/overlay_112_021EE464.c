#include "global.h"

#include "message_format.h"
#include "pokewalker.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021E9290(void *, void *, void *, s32);

s32 ov112_021EE464(Ov112PokewalkerApp *arg0) {
    void *sum;
    sum = (void *)&arg0->unk_1F2E8[0];
    Pokewalker_TryGetBoxMon(arg0->pokewalker, sum);
    ov112_021EA5A4(arg0, 0);
    BufferBoxMonNickname(arg0->msgFormat, 1U, sum);
    ov112_021E9290(sum, (void *)&arg0->unk_1D7AC[0], (void *)&arg0->unk_1D79C[0], 0);
    ov112_021EC460(arg0, 2, 0x1A, 0x35);
    return 0x40;
}

s32 ov112_021EE4B8(Ov112PokewalkerApp *arg0) {
    arg0->unk_1F2D0 = 0;
    ov112_021EA688(arg0, 8);
    return 0x36;
}
