#include "global.h"

#include "overlay_102_app_021e937c.h"

void ov102_021E94A4(void *);

void ov102_021EAF5C(void *, u32);

void ov102_021EAFAC(void *);

void ov102_021EB524(void *);

void ov102_021E9944(u32 param_1, UnkStruct_ov102_021E978C *param_2);

void ov102_021E9944(u32 param_1, UnkStruct_ov102_021E978C *param_2) {
    Ov102App_021E937C *iVar1;

    iVar1 = param_2->unk_0;
    iVar1->unk_1F4 = 0;
    ov102_021EB524(iVar1->unk_1E4);
    ov102_021EAFAC(iVar1->unk_1E0);
    ov102_021EAF5C(iVar1->unk_1E0, 1);
    ov102_021E94A4(param_2);
    return;
}
