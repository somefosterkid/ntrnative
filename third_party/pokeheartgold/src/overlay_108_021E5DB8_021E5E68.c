#include "global.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E7F7C(SafariAreaCustomizer *app);

u32 ov108_021E80F4(SafariAreaCustomizer *param_1);

extern void ov108_021E7700(void *data, int a1, int a2, int a3);

void ov108_021E7EB0(SafariAreaCustomizer *base);

extern void ov108_021E7CD8(void *data, u32 a1);

int ov108_021E5E68(SafariAreaCustomizer *data);

int ov108_021E5E68(SafariAreaCustomizer *data) {
    switch (data->unk_8) {
    case 0:
        ov108_021E7F7C(data);
        (data->unk_8)++;
        break;

    case 1:
        if (ov108_021E80F4(data)) {
            (data->unk_8)++;
        }
        break;

    default:
        reg_GXS_DB_DISPCNT &= 0xFFFF1FFF;
        data->unk_184E1 = 0;
        data->unk_8 = 0;
        if (data->unk_184E2_3 == 0) {
            data->unk_C = 3;
            ov108_021E7700(data, 0, 1, 1);
            ov108_021E7EB0(data);
            data->unk_184E2_3 = 1;
        } else {
            data->unk_C = 2;
            ov108_021E7700(data, 0, 0, 1);
            ov108_021E7CD8(data, data->areaIdx);
            data->unk_184E2_3 = 0;
        }
        return 4;
    }

    return 2;
}
