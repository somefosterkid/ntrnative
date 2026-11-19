#include "global.h"

#include "touchscreen.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E61E8(SafariAreaCustomizer *arg0);

void ov108_021E6238(void *);


u32 ov108_021E6240(SafariAreaCustomizer *param_1);

u32 ov108_021E6240(SafariAreaCustomizer *param_1) {
    BOOL BVar1;

    if (param_1->inTouchMode == 1) {
        BVar1 = System_GetTouchHeld();
        if (BVar1 != 0) {
            return 0;
        }
        if (gSystem.heldKeys != 0) {
            if (param_1->unk_4C4 != NULL) {
                param_1->unk_4C4(param_1);
            }
            ov108_021E61E8(param_1);
            param_1->inTouchMode = 0;
            return 0;
        }
    } else {
        if (gSystem.heldKeys != 0) {
            return 0;
        }
        BVar1 = System_GetTouchHeld();
        if (BVar1 != 0) {
            if (param_1->unk_4C8 != NULL) {
                param_1->unk_4C8(param_1);
            }
            ov108_021E6238(param_1);
            param_1->inTouchMode = 1;
            return 0;
        }
    }
    return 0;
}
