#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_manager.h"

BOOL ov96_021E6814(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E6814(PokeathlonCourseData *data, u8 *unused) {
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
