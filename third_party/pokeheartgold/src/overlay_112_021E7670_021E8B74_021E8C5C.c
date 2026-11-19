#include "global.h"

#include "pokemon.h"

void ov112_021E8C5C(const u8 *arg0, BoxPokemon *arg1, u16 *arg2);

void ov112_021E8C5C(const u8 *arg0, BoxPokemon *boxMon, u16 *arg2) {
    s32 v6;
    u16 *v5;
    u32 v1;
    s32 v3;
    u32 v2;
    u8 v0;
    s32 v4;
    v5 = arg2;
    v2 = GetBoxMonData(boxMon, 0xB1, 0L);
    v1 = GetBoxMonData(boxMon, 0xB2, 0L);
    v4 = 0;
    v3 = 0;
    do {
        v0 = arg0[v3];
        if ((v2 == v0) || (v1 == v0)) {
            v4 = 1;
        }
        v3 += 1;
    } while (v3 < 3);
    if (v4 != 0) {
        v6 = 0;
        do {
            *v5 = (u16)(((s32)((*v5) * 0x4B)) / 100);
            v6 += 1;
            v5 += 1;
        } while (v6 < 3);
    }
}
