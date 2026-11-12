// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/gimmick_skyarrow_bridge.h.
#ifndef BWPORT_FIELD_GIMMICK_SKYARROW_BRIDGE_H
#define BWPORT_FIELD_GIMMICK_SKYARROW_BRIDGE_H

#include "types.h"
#include "struct_decls.h"

// The Skyarrow Bridge's gimmick, overlay 53: five models moving along curves, three of them with a 3D sound, the
// zone's music fading in as the camera climbs, and a sound when the player passes a point on the way down. Overlay
// 21's gimmick table calls these
void func_ov053_021f36c0(Field *field);
void func_ov053_021f3734(Field *field);
void func_ov053_021f3778(Field *field);

#endif // BWPORT_FIELD_GIMMICK_SKYARROW_BRIDGE_H
