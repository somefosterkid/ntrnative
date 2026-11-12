// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/symbol_save_field.c, which is the same
// code in Black 2/White 2's overlay 12. The names are Black's dsd names, and swan's
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0) for the save's functions.
#include "types.h"
#include "field/entree_forest.h"
#include "field/symbol_save_field.h"
#include "gfl/std.h"
#include "save/save_control.h"

static u8 func_ov010_021673e0(AreaNPCSave *npcData, EntreeForestPokemon *dest, int max, int start, int count);
static u32 func_ov010_0216742c(AreaNPCSave *npcData, EntreeForestPokemon *dest, int max, u8 *count);
static u32 func_ov010_02167444(AreaNPCSave *npcData, EntreeForestPokemon *dest, int max, u8 *count);
static u16 func_ov010_0216745c(AreaNPCSave *npcData, EntreeForestPokemon *dest, int max, int area, u8 *count);
static u16 func_ov010_0216747c(AreaNPCSave *npcData, EntreeForestPokemon *dest, int max, int area, u8 *count);

u32 func_ov010_021673a8(AreaNPCSave *npcData) {
    u8 value;

    func_0200e06c(npcData);
    value = npcData->unk848;
    decryptNpcData(npcData);
    return value;
}

u32 func_ov010_021673c4(AreaNPCSave *npcData) {
    u8 value;

    func_0200e06c(npcData);
    value = npcData->unk849;
    decryptNpcData(npcData);
    return value;
}

// Copies count Pokémon from start, and returns how many there are before the first empty one
static u8 func_ov010_021673e0(AreaNPCSave *npcData, EntreeForestPokemon *dest, int max, int start, int count) {
    int i = 0;

    MI_CpuFill8(dest, 0, max * sizeof(EntreeForestPokemon));
    func_0200e06c(npcData);
    MI_CpuCopy8(&npcData->pokemon[start], dest, count * sizeof(EntreeForestPokemon));
    decryptNpcData(npcData);
    for (; i < count; i++) {
        if (dest[i].species == 0) {
            break;
        }
    }
    return i;
}

static u32 func_ov010_0216742c(AreaNPCSave *npcData, EntreeForestPokemon *dest, int max, u8 *count) {
    *count = func_ov010_021673e0(npcData, dest, max, 0, 10);
    return 0;
}

static u32 func_ov010_02167444(AreaNPCSave *npcData, EntreeForestPokemon *dest, int max, u8 *count) {
    *count = func_ov010_021673e0(npcData, dest, max, 10, 10);
    return 10;
}

static u16 func_ov010_0216745c(AreaNPCSave *npcData, EntreeForestPokemon *dest, int max, int area, u8 *count) {
    u16 start = area * 10 + 20;

    *count = func_ov010_021673e0(npcData, dest, max, start, 10);
    return start;
}

static u16 func_ov010_0216747c(AreaNPCSave *npcData, EntreeForestPokemon *dest, int max, int area, u8 *count) {
    u16 start = area * 20 + 50;

    *count = func_ov010_021673e0(npcData, dest, max, start, 20);
    return start;
}

u32 func_ov010_0216749c(AreaNPCSave *npcData, EntreeForestPokemon *dest, int max, int area, u8 *count) {
    u8 extra = 0;
    u32 start;

    MI_CpuFill8(dest, 0, max * sizeof(EntreeForestPokemon));
    if (area == 0) {
        start = func_ov010_0216742c(npcData, dest, max, count);
        func_ov010_02167444(npcData, dest + 10, max - 10, &extra);
        *count += extra;
    } else if (area >= 1 && area < 4) {
        start = func_ov010_0216745c(npcData, dest, max, area - 1, count);
    } else {
        start = func_ov010_0216747c(npcData, dest, max, area - 4, count);
    }
    return start;
}
