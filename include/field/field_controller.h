// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_controller.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_CONTROLLER_H
#define BWPORT_FIELD_FIELD_CONTROLLER_H

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"

// Black 2's, not checked against Black
// Black's three tables (fieldmap_ctrl_grid.c, fieldmap_ctrl_nogrid_work.c) have these five fields
// What a kind of map's controller does: its type, and making, updating and freeing it
struct FieldmapCtrlVTable {
    u32 typeId;
    void (*create)(Field *field, VecFx32 *pos, u16 dir);
    void (*update)(Field *field, VecFx32 *pos);
    void (*free)(Field *field);
    VecFx32 *(*getPos)(Field *field);
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void *Field_GetController(Field *field);
void Field_SetController(Field *field, void *controller);
u32 Field_GetControllerTypeID(Field *field);
// Sets whether the grid map's controller stops updating
void FieldmapCtrlGrid_SetUpdateDisable(Field *field, BOOL disable);

#endif // BWPORT_FIELD_FIELD_CONTROLLER_H
