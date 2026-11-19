#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_manager.h"

s32 ov96_021E6814(PokeathlonCourseData *arg0);

s32 ov96_021E6814(PokeathlonCourseData *data) {
    PokeathlonCourseArgs *pokeathlonCourseArgs;

    if (OverlayManager_Run(data->subOverlay) != 0) {
        OverlayManager_Delete(data->subOverlay);
        pokeathlonCourseArgs = data->args;
        if (pokeathlonCourseArgs->mode == 0U) {
            if (data->unk_1FC != 0) {
                pokeathlonCourseArgs->shouldFreeHeap = 1;
                PokeathlonCourse_SetStateField07(data, 0x25U);
            } else {
                pokeathlonCourseArgs->shouldFreeHeap = 0;
                PokeathlonCourse_SetStateField07(data, 3U);
            }
        } else {
            pokeathlonCourseArgs->shouldFreeHeap = 0;
            PokeathlonCourse_SetStateField07(data, 2U);
        }
    }
    return 0;
}
