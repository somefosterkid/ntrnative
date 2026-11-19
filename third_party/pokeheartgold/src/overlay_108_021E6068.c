#include "global.h"
#include "overlay_108_safari_area_customizer.h"
struct SafariAreaCustomizer;

u32 ov108_021E6240(SafariAreaCustomizer *param_1);

s32 ov108_021E6894(SafariAreaCustomizer *arg0);

s32 ov108_021E69A0(SafariAreaCustomizer *arg0);

s32 ov108_021E6068(SafariAreaCustomizer *arg0);

s32 ov108_021E6068(SafariAreaCustomizer *arg0) {
    if (ov108_021E6240(arg0) != 0) {
        return 0;
    }
    if (arg0->inTouchMode == 0) {
        return ov108_021E6894(arg0);
    }
    return ov108_021E69A0(arg0);
}
