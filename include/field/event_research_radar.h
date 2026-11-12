// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_research_radar.h, with Black's
// names.
#ifndef BWPORT_FIELD_EVENT_RESEARCH_RADAR_H
#define BWPORT_FIELD_EVENT_RESEARCH_RADAR_H

#include "types.h"
#include "struct_decls.h"

// The event that closes the field, runs the Research Radar and opens the field again
GameEvent *func_ov010_02167044(GameSystem *gsys, Field *field);

#endif // BWPORT_FIELD_EVENT_RESEARCH_RADAR_H
