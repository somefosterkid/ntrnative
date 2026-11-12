#ifndef BWPORT_FIELD_EVENT_COMM_TVT_H
#define BWPORT_FIELD_EVENT_COMM_TVT_H

// Overlay 10's event_comm_tvt.c (pokebw2's ov017 file): the Xtransceiver event of the C-Gear

#include "types.h"
#include "struct_decls.h"

// pokebw2's EventCommTvt_Create; Black's creates the event, or puts it into the event it is given (as CallIRC)
GameEvent *func_ov010_0215fbf4(GameSystem *gsys, Field *field, GameEvent *event, BOOL create);

#endif // BWPORT_FIELD_EVENT_COMM_TVT_H
