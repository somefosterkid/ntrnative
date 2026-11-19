#include "global.h"

#include "pokemon.h"

void ov97_0221EB38(BoxPokemon *arg0, void *arg1);

void ov97_0221EB38(BoxPokemon *arg0, void *arg1) {
    struct PokeathlonPerformanceStars sp0;

    CalcBoxmonPokeathlonStars(&sp0, arg0, NULL, HEAP_ID_POKEATHLON);
    *(s8 *)arg1 = (s8)((u32)(sp0.stars << 0x1D) >> 0x1D);
    *(s8 *)((u8 *)arg1 + 1) = (s8)((u32)(sp0.stars << 0x11) >> 0x1D);
    *(s8 *)((u8 *)arg1 + 2) = (s8)((u32)(sp0.stars << 0x14) >> 0x1D);
    *(s8 *)((u8 *)arg1 + 3) = (s8)((u32)(sp0.stars << 0x1A) >> 0x1D);
    *(s8 *)((u8 *)arg1 + 4) = (s8)((u32)(sp0.stars << 0x17) >> 0x1D);
    *(u16 *)((u8 *)arg1 + 6) = (u16)((*(u16 *)((u8 *)arg1 + 6) & ~7) | (7 & sp0.color[0]));
    *(u16 *)((u8 *)arg1 + 6) = (u16)((*(u16 *)((u8 *)arg1 + 6) & ~0x38) | ((u32)(sp0.color[4] << 0x1D) >> 0x1A));
    *(u16 *)((u8 *)arg1 + 6) = (u16)((*(u16 *)((u8 *)arg1 + 6) & 0xFFFFFE3F) | ((u32)(sp0.color[3] << 0x1D) >> 0x17));
    *(u16 *)((u8 *)arg1 + 6) = (u16)((*(u16 *)((u8 *)arg1 + 6) & 0xFFFFF1FF) | ((u32)(sp0.color[1] << 0x1D) >> 0x14));
    *(u16 *)((u8 *)arg1 + 6) = (u16)((*(u16 *)((u8 *)arg1 + 6) & 0xFFFF8FFF) | ((u32)(sp0.color[2] << 0x1D) >> 0x11));
}
