#include "global.h"

#include "overlay_102_app_021e937c.h"

void ov102_021E94A4(void *);

void ov102_021EAE40(u8 *, u32);

void ov102_021EA05C(u32 param_1, UnkStruct_ov102_021E978C *param_2);

void ov102_021EA05C(u32 param_1, UnkStruct_ov102_021E978C *param_2) {
    Ov102App_021E937C *view = param_2->unk_0;

    ov102_021EAE40(view->unk_1E0, 0);
    ov102_021E94A4(param_2);
    return;
}
