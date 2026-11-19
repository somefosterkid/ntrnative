#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "save_vars_flags.h"

void ov96_021EF2AC(PokeathlonCourseData *arg0);

void ov96_021EF2AC(PokeathlonCourseData *data) {
    Save_VarsFlags_CheckFlagInArray(Save_VarsFlags_Get(PokeathlonCourse_GetSaveData(data)), 0xEFU);
}
