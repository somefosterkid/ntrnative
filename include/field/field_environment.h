// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_environment.h, the lines in use
// so far.
#ifndef BWPORT_FIELD_FIELD_ENVIRONMENT_H
#define BWPORT_FIELD_FIELD_ENVIRONMENT_H

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

void *Field_GetWeatherSystem(Field *field);
u16 func_ov021_021a1bf4(void *weatherSystem);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 Field_GetWeatherForZone(Field *field, u16 zoneId);

#endif // BWPORT_FIELD_FIELD_ENVIRONMENT_H
