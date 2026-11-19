#include "global.h"

#include "application/pokedex/pokedex_internal.h"

void ov18_021F39C4(PokedexAppData *pokedexApp, int a1, u32 value) {
    BOOL started;

    u32 digit;
    int i;
    u32 divisor;

    if (value == 9999) {
        value = 99990;
    } else {
        value = (value * 220462 + 50000) / 100000;
    }
    started = FALSE;
    divisor = 10000;
    for (i = 0; i < 5U; i++) {
        digit = value / divisor;
        if (digit != 0 || started == TRUE) {
            started = TRUE;
            ov18_021F118C(pokedexApp, a1 + i, digit + 0x2B);
            ov18_021F11C0(pokedexApp, a1 + i, 1);
        } else {
            ov18_021F11C0(pokedexApp, a1 + i, 0);
        }
        value %= divisor;
        divisor /= 10;
        if (i == 2) {
            started = TRUE;
        }
    }
}
