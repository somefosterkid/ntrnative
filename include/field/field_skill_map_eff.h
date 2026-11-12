// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_skill_map_eff.h: the prototypes
// of Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_SKILL_MAP_EFF_H
#define BWPORT_FIELD_FIELD_SKILL_MAP_EFF_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void *Field_GetSkillMapEff(Field *field);
void *FieldSkillMapEff_GetFlash(void *skillMapEff);

// The field's skill map effects (a flash with the flash permissions)
u32 *func_ov021_021c7f70(u32 flashPerms, HeapID heapId);

#endif // BWPORT_FIELD_FIELD_SKILL_MAP_EFF_H
