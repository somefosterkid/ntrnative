// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_worldtrade.h, the lines in use
// so far. pokebw2's EventWorldTrade_CreateFromArgs, in overlay 4, is Black's func_ov004_021542c0.
#ifndef BWPORT_FIELD_EVENT_WORLDTRADE_H
#define BWPORT_FIELD_EVENT_WORLDTRADE_H

#include "types.h"
#include "struct_decls.h"

typedef struct {
    Field *field;
    u32 unused;
} EventWorldTradeArgs;

GameEvent *func_ov004_021542cc(GameSystem *gsys, Field *field, u32 unused);
GameEvent *func_ov004_021542c0(GameSystem *gsys, void *args);

#endif // BWPORT_FIELD_EVENT_WORLDTRADE_H
