#include "global.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E61E8(SafariAreaCustomizer *arg0);

void *ov108_021E6238(void *);

void ov108_021E767C(SafariAreaCustomizer *param_1, s8 param_2);

u8 ov108_021E7700(void *, s32, s32, s32);

void ov108_021E7C5C(SafariAreaCustomizer *arg0);

s32 ov108_021E7CD8(void *, unsigned int);

void ov108_021E7BFC(SafariAreaCustomizer *arg0);

void ov108_021E7BFC(SafariAreaCustomizer *arg0) {
    ov108_021E7C5C(arg0);
    ov108_021E7CD8(arg0, arg0->areaIdx);
    ov108_021E767C(arg0, 0);
    ov108_021E7700(arg0, 0, 0, 1);
    if (arg0->inTouchMode == 0) {
        ov108_021E61E8(arg0);
    } else {
        ov108_021E6238(arg0);
    }
    arg0->unk_184E4 = 0;
    arg0->unk_184E6 = -0xC0;
    arg0->unk_184E2_0 = 1;
}
