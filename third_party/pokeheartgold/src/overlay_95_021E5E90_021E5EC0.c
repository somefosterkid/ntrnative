#include "global.h"

#include "overlay_95_egg_hatch_app.h"
#include "unk_02014DA0.h"

void ov95_021E5E58(SPLEmitter *);

void ov95_021E5EC0(EggHatchParticleSystem *arg0, s32 arg1);

void ov95_021E5EC0(EggHatchParticleSystem *arg, s32 res_no) {
    sub_02015494(arg->ps, res_no, ov95_021E5E58, arg);
    sub_02015528(arg->ps, 1);
}
