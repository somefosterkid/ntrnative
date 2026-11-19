#include "global.h"

#include "math_util.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021EA9A0(u8 *, s32);

void ov112_021EA76C(u8 *arg0, s32 arg1);

s32 ov112_021EA838(Ov112PokewalkerApp *arg0);

void ov112_021EA76C(u8 *arg0, s32 arg1) {
    s32 v0;
    s32 v1;
    u8 *v2;
    void *v3;

    v0 = ov112_021EA9A0(arg0 + 0x1EC80, arg1);
    if (v0 >= 0) {
        v2 = arg0 + 0x1EC80;
        v1 = v0 * 0x10;
        v3 = (void *)(v2 + v1);
        *(u8 *)(v2 + v1) = 1;
        *(s8 *)((u8 *)v3 + 3) = (s8)((MTRandom() % 6U) + 2);
        *(s16 *)((u8 *)v3 + 0xC) = (s16)(MTRandom() % 300U);
        *(s16 *)((u8 *)v3 + 4) = 0x80;
        *(s16 *)((u8 *)v3 + 6) = 0x74;
        *(s8 *)((u8 *)v3 + 2) = (s8)((MTRandom() % 40U) + 0x28);
        *(s16 *)((u8 *)v3 + 0xE) = (s16)((MTRandom() % 60U) + 0x78);
    }
}

void ov112_021EA7D0(u8 *arg0, s32 arg1);

void ov112_021EA7D0(u8 *arg0, s32 arg1) {
    s32 v0;
    s32 v1;
    u8 *v2;
    void *v3;

    v0 = ov112_021EA9A0(arg0 + 0x1EC80, arg1);
    if (v0 >= 0) {
        v2 = arg0 + 0x1EC80;
        v1 = v0 * 0x10;
        v3 = (void *)(v2 + v1);
        *(u8 *)(v2 + v1) = 1;
        *(s8 *)((u8 *)v3 + 3) = (s8)(-1 * ((MTRandom() % 5U) + 5));
        *(s16 *)((u8 *)v3 + 0xC) = (s16)(MTRandom() % 300U);
        *(s16 *)((u8 *)v3 + 4) = 0x80;
        *(s16 *)((u8 *)v3 + 6) = -0x6D;
        *(s8 *)((u8 *)v3 + 2) = (s8)((MTRandom() % 40U) + 0x28);
        *(s16 *)((u8 *)v3 + 0xE) = (s16)((MTRandom() % 60U) + 0x78);
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
