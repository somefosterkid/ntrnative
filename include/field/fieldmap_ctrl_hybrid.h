// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/fieldmap_ctrl_hybrid.h: the lines in
// use so far.
#ifndef BWPORT_FIELD_FIELDMAP_CTRL_HYBRID_H
#define BWPORT_FIELD_FIELDMAP_CTRL_HYBRID_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"

// Moves the player to the other kind of movement where it stands, if it can
void func_ov021_021a6dc4(FieldmapCtrlHybrid *controller, Field *field);

struct FieldmapCtrlHybrid {
    // The controller type in use, 0 for the grid and 1 for a rail, 2 until the first is set
    u32 activeType;
    FieldPlayer *player;
    // On a rail, what func_ov036_0219ad00 last returned: then the player keeps moving its way
    u32 unk08;
};

void FieldmapCtrlHybrid_Create(Field *field, VecFx32 *pos, u16 dir);
void FieldmapCtrlHybrid_Update(Field *field, VecFx32 *pos);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void FieldmapCtrlHybrid_Free(Field *field);
VecFx32 *FieldmapCtrlHybrid_GetPos(Field *field);

#endif // BWPORT_FIELD_FIELDMAP_CTRL_HYBRID_H
