#include "global.h"

#include "filesystem.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E7B74(SafariAreaCustomizer *arg0, NARC *narc, s32 arg2, s32 arg3);

void ov108_021E7BB4(SafariAreaCustomizer *arg0, u8 arg1, u8 arg2);

void ov108_021E7BB4(SafariAreaCustomizer *arg0, u8 arg1, u8 arg2) {
    NARC *temp_r7;
    u32 var_r4;

    temp_r7 = NARC_New(NARC_a_1_6_6, arg0->heapID);
    if (arg1 >= 6U) {
        var_r4 = 0;
        do {
            ov108_021E7B74(arg0, temp_r7, (u8)var_r4, arg0->areaSet.areas[var_r4].area_no);
            var_r4 += 1;
        } while ((s32)var_r4 < 6);
    } else {
        ov108_021E7B74(arg0, temp_r7, (u8)arg1, arg2);
    }
    NARC_Delete(temp_r7);
}
