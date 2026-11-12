// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/worldtrade_data.h: the lines in use so
// far.
#ifndef BWPORT_SAVE_WORLDTRADE_DATA_H
#define BWPORT_SAVE_WORLDTRADE_DATA_H

#include "types.h"
#include "pml/poke_party.h"
#include "struct_decls.h"

// Black 2's; matching code confirms the offsets of pkm, flag, unkEE, unkF0 and unkF4, and its size in Black
struct WorldTradeData {
    // The deposited Pokémon
    PartyPkm pkm;
    u8 unkDC[0x10];
    u16 flag;
    u16 unkEE;
    u32 unkF0;
    u32 unkF4;
}; // size: 0xf8

// pokebw2's WorldTradeData_GetBlockSize
u32 func_0200ad78(void);
// pokebw2's WorldTradeData_Init
void func_0200ad7c(WorldTradeData *data);
u16 func_0200ad8c(WorldTradeData *data);
void func_0200ad94(WorldTradeData *data, u16 flag);
// pokebw2's WorldTradeData_SetPkm
// Its callers pass the box the Pokémon came from as well, which it ignores
void func_0200adb8(WorldTradeData *data, PartyPkm *pkm, u32 box);
void func_0200add0(WorldTradeData *data, u32 value);
u16 func_0200ade0(WorldTradeData *data);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WorldTradeData_GetPkm
// Copies the deposited Pokémon out, or in
void func_0200ad9c(WorldTradeData *data, PartyPkm *pkm);
PartyPkm *func_0200adb4(WorldTradeData *data);
void func_0200add8(WorldTradeData *data, u32 value);

#endif // BWPORT_SAVE_WORLDTRADE_DATA_H
