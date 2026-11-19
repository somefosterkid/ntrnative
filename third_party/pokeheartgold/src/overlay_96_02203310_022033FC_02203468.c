#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_02203468work.h"

void ov96_02203468(PokeathlonCourseData *arg0);

void ov96_02203468(PokeathlonCourseData *arg0) {
    Ov96_02203468Work *work = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    int i;
    BOOL reset;
    for (i = 0; i < 12; i++) {
        reset = FALSE;
        if (work->balls[i].state == 1) {
            VEC_MultAdd(FX32_ONE, &work->balls[i].vel, &work->balls[i].pos, &work->balls[i].pos);
        } else if (work->balls[i].state == 3) {
            work->balls[i].counter++;
            if (work->balls[i].counter >= 8) {
                work->balls[i].counter = 0;
                work->balls[i].state = 0;
            }
        }
        if (work->balls[i].pos.x < FX32_ONE || work->balls[i].pos.x > FX32_CONST(64) || work->balls[i].pos.z < FX32_ONE || work->balls[i].pos.z > FX32_CONST(64)) {
            reset = TRUE;
        }
        if (reset) {
            work->balls[i].state = 0;
            work->balls[i].vel.x = 0;
            work->balls[i].vel.z = 0;
            work->balls[i].vel.y = 0;
            work->balls[i].accel.x = 0;
            work->balls[i].accel.z = 0;
            work->balls[i].accel.y = 0;
        }
    }
}
