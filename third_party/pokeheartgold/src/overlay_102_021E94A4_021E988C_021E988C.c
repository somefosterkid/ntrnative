#include "global.h"

#include "overlay_102_app_021e937c.h"

u16 ov102_021E8FC0(void *);

void ov102_021E94A4(void *);

void ov102_021EAA3C(void *);

void ov102_021EAFF0(void *, u32);

void ov102_021E988C(u32 param_1, UnkStruct_ov102_021E978C *param_2);

void ov102_021E988C(u32 param_1, UnkStruct_ov102_021E978C *param_2) {
    u32 uVar1;
    Ov102App_021E937C *iVar2;

    iVar2 = param_2->unk_0;
    ov102_021EAA3C(iVar2->unk_1E0);
    uVar1 = ov102_021E8FC0(param_2->unk_4);
    ov102_021EAFF0(iVar2->unk_1E0, uVar1);
    ov102_021E94A4(param_2);
    return;
}
