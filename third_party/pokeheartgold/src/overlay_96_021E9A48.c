#include "global.h"

#include "pokeathlon/pokeathlon.h"

void *ov96_021E9A48(int idx, void *data, int size);

void *ov96_021E9A48(int idx, void *data, int size) {
    return PokeathlonCourse_GetFieldData_AtIndex(data, 0);
}
