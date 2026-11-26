#include "global.h"

#include "overlay_manager.h"
#include "pokeathlon/pokeathlon.h"

struct OverlayManager;

BOOL ov96_021EEEBC(PokeathlonCourseData *data, int unused);

void ov96_021EF05C(PokeathlonCourseData *data);
BOOL ov96_021EEEC0(PokeathlonCourseData *data, int unused);

BOOL ov96_021EEEBC(PokeathlonCourseData *data, int unused) {
    return TRUE;
}

BOOL ov96_021EEEC0(PokeathlonCourseData *data, int unused) {
    ov96_021EF05C(data);
    return 1;
}
