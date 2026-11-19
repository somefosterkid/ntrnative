#include "global.h"

#include "overlay_102_app_021e937c.h"

void ov102_021E94A4(void *);

void ov102_021EB98C(u8 *, u32);

void ov102_021E98C4(u32 param_1, UnkStruct_ov102_021E978C *param_2);

void ov102_021E98C4(u32 param_1, UnkStruct_ov102_021E978C *param_2) {
    Ov102App_021E937C *view = param_2->unk_0;

    ov102_021EB98C(view->unk_1E8, 1);
    ov102_021E94A4(param_2);
    return;
}

void ov102_021EBEF4(u8 *, u32);

void ov102_021E98E0(u32 param_1, UnkStruct_ov102_021E978C *param_2);

void ov102_021E98E0(u32 param_1, UnkStruct_ov102_021E978C *param_2) {
    Ov102App_021E937C *view = param_2->unk_0;

    ov102_021EBEF4(view->unk_1EC, 1);
    ov102_021E94A4(param_2);
    return;
}
