#include "global.h"

#include "pokewalker.h"
#include "overlay_112_pokewalker_app.h"
#include "unk_02005D10.h"

struct POKEWALKER;

extern void ov112_021EE7A8(Ov112PokewalkerApp *work);

extern void ov112_021E9290(void *a0, void *a1, void *a2, int a3);

extern void ov112_021EAA10(Ov112PokewalkerApp *work);

int ov112_021EED08(Ov112PokewalkerApp *work) {
    int state;
    work->unk_1E430 = NULL;
    state = work->unk_10;
    if (state == 1) {
        ov112_021EE7A8(work);
        ov112_021EEAF0(work, 1);
        ov112_021EEA7C(work);
        ov112_021EE920(work);
        if (!((u32)(work->unk_10E7 << 29) >> 31)) {
            sub_020326A4(work->pokewalker, 1, 0);
        }
        if (work->unk_1E430 != NULL) {
            ov112_021E9290(work->unk_1E430, work->unk_1D7AC, work->unk_1D79C, 0);
            ov112_021ED0C8(work);
            ov112_021EAA10(work);
            work->unk_1F2C0[2] = 0;
            PlayFanfare(0x4A2);
        } else {
            ov112_021EAA10(work);
            work->unk_1F2D6 = 0x1F;
        }
        ov112_021E95A0(work);
        work->unk_8 = 0;
    } else if (state == 2) {
        ov112_021EEA7C(work);
        ov112_021EEAF0(work, 0);
        work->unk_8 = 5;
    } else if (state == 4) {
        ov112_021EE7A8(work);
        ov112_021E9290(work->unk_1E430, work->unk_1D7AC, work->unk_1D79C, 0);
        ov112_021ED0C8(work);
        work->unk_8 = 0;
        sub_020326A4(work->pokewalker, 1, 0);
    }
    work->unk_10E7 &= ~4;
    return 2;
}
