// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/game_beacon_set.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_GAME_BEACON_SET_H
#define BWPORT_FIELD_GAME_BEACON_SET_H

#include "types.h"
#include "struct_decls.h"
#include "system/game_beacon.h"

// The request the beacon search (game_beacon_search.c) passes to func_ov010_02164fd0 when its comm ends: the game
// system and the MAC address of the beacon it found
typedef struct {
    GameSystem *gsys;
    u8 addr[6];
} GameBeaconReq;

void func_ov010_02165078(u16 value);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov010_0216505c(u16 value);
void func_ov010_021653f4(u16 value);
void func_ov010_021654ac(const StrBuf *name);
void func_ov010_02165424(const StrBuf *name);
void func_ov010_02165468(const StrBuf *name);
void func_ov010_021654f0(const StrBuf *name);
void func_ov010_02165534(u16 value);
// That the player is in a musical with the Pokémon of that name
void func_ov010_0216576c(const StrBuf *name);

void GameBeacon_BroadcastFerrisWheel(void);
void func_ov010_02165174(u16 itemId);
void func_ov010_021651ac(u16 itemId);
void func_ov010_021651e4(u16 itemId);
void func_ov010_0216521c(u16 itemId);
void func_ov010_02165254(u16 itemId);
void func_ov010_0216528c(u16 itemId);
void func_ov010_021652c4(u32 passPower);
void func_ov010_0216530c(u16 value);
void func_ov010_02165344(void);
void func_ov010_02165374(u32 value);
void func_ov010_021653c4(void);
void func_ov010_0216556c(void);
void func_ov010_0216559c(u16 species);
void func_ov010_021655d4(void);
void func_ov010_02165604(void);
void func_ov010_02165634(u32 value);
void func_ov010_0216566c(void);
void func_ov010_021656a0(void);
void func_ov010_021656d0(u8 value);
void func_ov010_0216573c(void);
void func_ov010_021657b0(u32 passPower);
void func_ov010_021657e8(GameData *gameData, u16 value);
BOOL func_ov010_021658d4(GameBeaconInfoTable *table, GameBeacon *dest, u16 *time);
u32 func_ov010_02165a10(GameBeaconInfoTable *table, GameBeacon *beacon, u16 value, u32 *notFound);
int func_ov010_02165888(GameBeaconInfoTable *table);
void func_ov010_02165940(GameBeacon *info);

void func_ov010_02165844(StrBuf *name);

#endif // BWPORT_FIELD_GAME_BEACON_SET_H
