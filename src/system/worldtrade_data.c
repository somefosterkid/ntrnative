// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/worldtrade_data.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/worldtrade_data.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "save/save_control.h"
#include "nitro/mi.h"

// The Global Trade Station's save block, which keeps the Pokémon the player deposited. The ROM has no string for
// the file, so the name is a guess (swan's func_0200ad6c). Block 0x2e.

#define SAVE_BLOCK_WORLDTRADE 0x2e

// pokebw2's SaveControl_GetWorldTradeData
WorldTradeData *func_0200ad6c(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_WORLDTRADE);
}

// pokebw2's WorldTradeData_GetBlockSize
u32 func_0200ad78(void) {
    return sizeof(WorldTradeData);
}

// pokebw2's WorldTradeData_Init
void func_0200ad7c(WorldTradeData *data) {
    MIi_CpuClearFast(0, data, sizeof(WorldTradeData));
}

u16 func_0200ad8c(WorldTradeData *data) {
    return data->flag;
}

void func_0200ad94(WorldTradeData *data, u16 flag) {
    data->flag = flag;
}

// pokebw2's WorldTradeData_GetPkm
void func_0200ad9c(WorldTradeData *data, PartyPkm *pkm) {
    MI_CpuCopy8(&data->pkm, pkm, PokeParty_GetPkmRawSize());
}

PartyPkm *func_0200adb4(WorldTradeData *data) {
    return &data->pkm;
}

// pokebw2's WorldTradeData_SetPkm
void func_0200adb8(WorldTradeData *data, PartyPkm *pkm, u32 box) {
    MI_CpuCopy8(pkm, &data->pkm, PokeParty_GetPkmRawSize());
}

void func_0200add0(WorldTradeData *data, u32 value) {
    data->unkF0 = value;
}

void func_0200add8(WorldTradeData *data, u32 value) {
    data->unkF4 = value;
}

u16 func_0200ade0(WorldTradeData *data) {
    return data->unkEE;
}
