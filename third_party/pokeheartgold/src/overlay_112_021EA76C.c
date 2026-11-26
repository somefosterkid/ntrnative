#include "global.h"

#include "math_util.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021EA9A0(void *, s32);

void ov112_021EA76C(Ov112PokewalkerApp *arg0, s32 arg1);

void ov112_021EA76C(Ov112PokewalkerApp *arg0, s32 arg1) {
    s32 v0;
    Ov112PokewalkerRec *v3;

    v0 = ov112_021EA9A0(arg0->unk_1EC80, arg1);
    if (v0 >= 0) {
        v3 = &arg0->unk_1EC80[v0];
        v3->unk_0[0] = 1;
        v3->unk_3 = (s8)((MTRandom() % 6U) + 2);
        v3->unk_C = (s16)(MTRandom() % 300U);
        v3->unk_4 = 0x80;
        v3->unk_6 = 0x74;
        v3->unk_2 = (s8)((MTRandom() % 40U) + 0x28);
        v3->unk_E = (s16)((MTRandom() % 60U) + 0x78);
    }
}

void ov112_021EA7D0(Ov112PokewalkerApp *arg0, s32 arg1);

void ov112_021EA7D0(Ov112PokewalkerApp *arg0, s32 arg1) {
    s32 v0;
    Ov112PokewalkerRec *v3;

    v0 = ov112_021EA9A0(arg0->unk_1EC80, arg1);
    if (v0 >= 0) {
        v3 = &arg0->unk_1EC80[v0];
        v3->unk_0[0] = 1;
        v3->unk_3 = (s8)(-1 * ((MTRandom() % 5U) + 5));
        v3->unk_C = (s16)(MTRandom() % 300U);
        v3->unk_4 = 0x80;
        v3->unk_6 = -0x6D;
        v3->unk_2 = (s8)((MTRandom() % 40U) + 0x28);
        v3->unk_E = (s16)((MTRandom() % 60U) + 0x78);
    }
}

s32 ov112_021EA838(Ov112PokewalkerApp *arg0) {
    s32 v0;
    s32 v1;
    void *v2;

    v1 = 0;
    v2 = arg0->unk_1EC80;
    v0 = 0;
    do {
        if (*(u8 *)v2 != 0) {
            v1 += 1;
            *(s16 *)((u8 *)v2 + 0xE) = (s16)(*(s16 *)((u8 *)v2 + 0xE) - 0x14);
        }
        v0 += 1;
        v2 += 0x10;
    } while (v0 < 0x64);
    return v1;
}
