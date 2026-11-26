#include "global.h"

#include "pokeathlon/pokeathlon.h"

void *ov96_021E9A2C(int idx, void *data, int size);
void *ov96_021E9A38(int idx, void *data, int size);

void *ov96_021E9A2C(int idx, void *data, int size) {
    return PokeathlonCourse_GetField3A4(data);
}

void *ov96_021E9A38(int idx, void *data, int size) {
    return PokeathlonCourse_GetField974_AtIndex(data, idx);
}
