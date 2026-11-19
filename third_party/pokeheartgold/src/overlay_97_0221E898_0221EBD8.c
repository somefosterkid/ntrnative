#include "global.h"
#include "pokemon.h"

void ov97_0221EBD8(BoxPokemon *arg0, void *arg1);

void ov97_0221EBD8(BoxPokemon *arg0, void *arg1) {
    struct PokeathlonTodayPerformance sp0;

    CalcBoxMonPokeathlonPerformance(arg0, &sp0);
    *(s8 *)arg1 = (s8) ((u32) (*(u16 *)(&sp0) << 0x1A) >> 0x1D);
    *(s8 *)((u8 *)arg1 + 1) = (s8) ((u32) (*(u16 *)((u8 *)(&sp0) + 0x10) << 0x1A) >> 0x1D);
    *(s8 *)((u8 *)arg1 + 2) = (s8) ((u32) (*(u16 *)((u8 *)(&sp0) + 0xC) << 0x1A) >> 0x1D);
    *(s8 *)((u8 *)arg1 + 3) = (s8) ((u32) (*(u16 *)((u8 *)(&sp0) + 4) << 0x1A) >> 0x1D);
    *(s8 *)((u8 *)arg1 + 4) = (s8) ((u32) (*(u16 *)((u8 *)(&sp0) + 8) << 0x1A) >> 0x1D);
}
