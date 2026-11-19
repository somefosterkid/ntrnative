#include "global.h"

#include "pokeathlon/pokeathlon.h"

void ov96_021E67AC(PokeathlonCourseData *data) {
    if (data->field_1F4 != 0) {
        (**(void (**)())(data->state + 4))(data, 0);
    }
    return;
}
