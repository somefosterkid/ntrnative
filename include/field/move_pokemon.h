#ifndef BWPORT_FIELD_MOVE_POKEMON_H
#define BWPORT_FIELD_MOVE_POKEMON_H

// Overlay 10's move_pokemon.c (named by its string): the roaming Pokémon, Thundurus in slot 0 and Tornadus in slot 1,
// which move between the zones of ROAMING_POKEMON_ZONES. EncountSave_GetRoamingPkmZone and GetDefaultWeatherValue are
// swan's names (https://github.com/ds-pokemon-hacking/swan, GPL-3.0); Black 2 keeps the first, and the second as a
// stub, in what pokebw2 calls field_encount_st.c

#include "types.h"
#include "struct_decls.h"

extern const u16 ROAMING_POKEMON_ZONES[17];

void func_ov010_0215e358(EncountSave *save, u8 season, u8 slot);
// Moves the roaming Pokémon on
void func_ov010_0215e364(GameData *gameData);
void func_ov010_0215e3a0(GameData *gameData);
u16 EncountSave_GetRoamingPkmZone(EncountSave *save, u8 slot);
// Whether a Pokémon roams
BOOL func_ov010_0215e404(EncountSave *save);
void func_ov010_0215e428(EncountSave *save, u16 zoneId);
// Starts the roaming Pokémon of the slot
void func_ov010_0215e440(GameData *gameData, u8 slot);
void func_ov010_0215e580(GameData *gameData);
// Updates the roaming Pokémon after a wild battle against it
void func_ov010_0215e5b0(GameData *gameData, BtlSetup *setup);
// The weather a roaming Pokémon brings to its zone, 6 or 7, else WEATHER_NONE
u32 GetDefaultWeatherValue(GameData *gameData, u16 zoneId);
BOOL func_ov010_0215e6f8(GameData *gameData);

#endif // BWPORT_FIELD_MOVE_POKEMON_H
