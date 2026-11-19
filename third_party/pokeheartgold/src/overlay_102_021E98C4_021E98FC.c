#include "global.h"

#include "overlay_102_app_021e937c.h"

u16 ov102_021E8FC8(void *);

void ov102_021E94A4(void *);

void ov102_021EAF5C(void *, u32, u32, u32);

void ov102_021EAF7C(void *);

void ov102_021EB524(void *, u32);

void ov102_021EB530(void *, u32);

void ov102_021E98FC(u32 param_1, UnkStruct_ov102_021E978C *param_2, u32 param_3, u32 param_4);

void ov102_021E98FC(u32 param_1, UnkStruct_ov102_021E978C *param_2, u32 param_3, u32 param_4) {
    u32 uVar1;
    Ov102App_021E937C *iVar2;

    iVar2 = param_2->unk_0;
    iVar2->unk_1F4 = 1;
    ov102_021EAF5C(iVar2->unk_1E0, 0, param_3, param_4);
    ov102_021EAF7C(iVar2->unk_1E0);
    ov102_021EB524(iVar2->unk_1E4, 1);
    uVar1 = ov102_021E8FC8(iVar2->unk_18);
    ov102_021EB530(iVar2->unk_1E4, uVar1);
    ov102_021E94A4(param_2);
    return;
}
