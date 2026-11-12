// After pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/pokemon_league_gimmicks.h. Black
// keeps the gimmicks of all the Elite Four's rooms in overlay 56, where Black 2 has an overlay per room.
#ifndef BWPORT_FIELD_POKEMON_LEAGUE_GIMMICKS_H
#define BWPORT_FIELD_POKEMON_LEAGUE_GIMMICKS_H

#include "types.h"
#include "field/field_async_proc.h"
#include "struct_decls.h"

// The rooms are numbered from 0 here, the scripts number them from 1
GameEvent *func_ov056_021f36c0(GameSystem *gsys, Field *field, u16 room);
u32 func_ov056_021f36c8(u16 room);

// The rooms' processes, in the order of the rooms' numbers 1, 4, 2 and 3
extern const FieldAsyncProcDef data_ov056_021f4344;
extern const FieldAsyncProcDef data_ov056_021f435c;
extern const FieldAsyncProcDef data_ov056_021f4374;
extern const FieldAsyncProcDef data_ov056_021f438c;

#endif // BWPORT_FIELD_POKEMON_LEAGUE_GIMMICKS_H
