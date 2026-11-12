// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_gtsnego.h, the lines in use so
// far. pokebw2's EventGtsNego_CreateFromArgs, in overlay 1, is Black's func_ov001_02154694.
#ifndef BWPORT_FIELD_EVENT_GTSNEGO_H
#define BWPORT_FIELD_EVENT_GTSNEGO_H

#include "types.h"
#include "struct_decls.h"

// GTS Negotiation, started by the NetConnectGTSNegotiation script command (pokebw2's EventGtsNego_Create)
GameEvent *func_ov001_0215466c(GameSystem *gsys, Field *field);
GameEvent *func_ov001_02154694(GameSystem *gsys, void *field);

#endif // BWPORT_FIELD_EVENT_GTSNEGO_H
