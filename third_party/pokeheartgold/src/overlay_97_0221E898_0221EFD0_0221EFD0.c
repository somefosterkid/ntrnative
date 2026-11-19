#include "global.h"

#include "gf_gfx_planes.h"
#include "overlay_97_course_sub_screen.h"

void ov97_0221F74C(Ov97CourseSubScreen *arg0);

s32 ov97_0221F428(Ov97CourseSubScreen *, void *);

s32 ov97_0221F56C(Ov97CourseSubScreen *, void *);

void ov97_0221EFD0(Ov97CourseSubScreen *arg0, void *arg1);

void ov97_0221EFD0(Ov97CourseSubScreen *arg0, void *arg1) {
    if (*(u8 *)((u8 *)arg1 + 9) == 2) {
        ov97_0221F56C(arg0, arg1);
        ov97_0221F428(arg0, arg1);
        GfGfx_EngineBTogglePlanes(2U, 1U);
        GfGfx_EngineBTogglePlanes(4U, 1U);
        return;
    }
    ov97_0221F74C(arg0);
    GfGfx_EngineBTogglePlanes(2U, 0U);
    GfGfx_EngineBTogglePlanes(4U, 0U);
}
