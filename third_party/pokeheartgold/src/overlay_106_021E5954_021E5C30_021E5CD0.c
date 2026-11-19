#include "global.h"

#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

s32 ov106_021E5CD0(LegendaryCinematic *cinematic, s32 index) {
    s32 var_r4;
    u32 var_r2;
    NNSG3dAnmObj *var_r5;
    LegendaryCinematicModel *model;
    var_r2 = 0;
    model = &cinematic->models[index];
    do {
        var_r5 = model->anmObj[var_r2];
        if (var_r5 != NULL) {
            var_r4 = var_r5->frame + 0x1000;
            if (var_r4 < NNS_G3dAnmObjGetNumFrame(var_r5)) {
                return 1;
            }
        }
        var_r2 += 1;
    } while (var_r2 < 4U);
    return 0;
}
