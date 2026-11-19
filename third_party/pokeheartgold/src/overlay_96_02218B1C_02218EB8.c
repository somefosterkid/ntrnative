#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96_rd_obj.h"

void ov96_02218578(Ov96RdObj *obj, s32 state);

s32 ov96_02218934(Ov96RdObj *obj);

s32 ov96_02219030(void *);

void ov96_02218EB8(Ov96RdObj *arg0);

void ov96_02218EB8(Ov96RdObj *arg0) {
    s32 temp_r0;
    s32 var_r1;

    temp_r0 = arg0->state;
    var_r1 = 1;
    if ((temp_r0 != 7) && (temp_r0 != 6)) {
        var_r1 = 0;
    }
    if (var_r1 == 0) {
        GF_AssertFail();
    }
    {
        VecFx32 sp0 = { 0, 0, 0 };
        arg0->vel = sp0;
    }
    arg0->pos20 = arg0->pos2C;
    ov96_02218578(arg0, 1);
    if (ov96_02218934(arg0) != 0) {
        ov96_02219030(arg0);
    }
}
