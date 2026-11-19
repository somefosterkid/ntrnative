#include "global.h"
#include "overlay_108_safari_area_customizer.h"

s32 ov108_021E8270(SafariAreaCustomizer *arg0);

void ov108_021E81A8(SafariAreaCustomizer *app);

s32 ov108_021E6010(SafariAreaCustomizer *arg0);

s32 ov108_021E6010(SafariAreaCustomizer *arg0) {
    s32 temp_r1;
    temp_r1 = arg0->unk_8;
    switch (temp_r1) {
    case 0:
        ov108_021E81A8(arg0);
        arg0->unk_8 = (s32)((arg0->unk_8) + 1);
        break;
    case 1:
        if (ov108_021E8270(arg0) != 0) {
            arg0->unk_8 = (s32)((arg0->unk_8) + 1);
        }

        break;
    default:
        arg0->unk_184E2_2 ^= 1;
        arg0->unk_8 = 0;
        return 0;

        break;
    }
    return 3;
}
