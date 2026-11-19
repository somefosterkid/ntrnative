#include "global.h"

#include "overlay_102_app_021e937c.h"

u16 ov102_021E8FC8(void *);

void ov102_021E94A4(void *);

void ov102_021EB530(void *, u32);

void ov102_021E9FAC(u32 param_1, UnkStruct_ov102_021E978C *param_2);

void ov102_021E9FAC(u32 param_1, UnkStruct_ov102_021E978C *param_2) {
    u32 uVar1;
    Ov102App_021E937C *iVar2;

    iVar2 = param_2->unk_0;
    uVar1 = ov102_021E8FC8(iVar2->unk_18);
    ov102_021EB530(iVar2->unk_1E4, uVar1);
    ov102_021E94A4(param_2);
    return;
}

u16 ov102_021E8FD0(void *);

void ov102_021EB9C0(void *, u32);

void ov102_021E9FCC(u32 param_1, UnkStruct_ov102_021E978C *param_2);

void ov102_021E9FCC(u32 param_1, UnkStruct_ov102_021E978C *param_2) {
    u32 uVar1;
    Ov102App_021E937C *iVar2;

    iVar2 = param_2->unk_0;
    uVar1 = ov102_021E8FD0(iVar2->unk_18);
    ov102_021EB9C0(iVar2->unk_1E8, uVar1);
    ov102_021E94A4(param_2);
    return;
}

u32 ov102_021E9028(void *);

void ov102_021EBF38(u8 *, u32);

void ov102_021E9FEC(u32 param_1, UnkStruct_ov102_021E978C *param_2);

void ov102_021E9FEC(u32 param_1, UnkStruct_ov102_021E978C *param_2) {
    u32 uVar1;
    Ov102App_021E937C *iVar2;

    iVar2 = param_2->unk_0;
    uVar1 = ov102_021E9028(iVar2->unk_18);
    ov102_021EBF38(iVar2->unk_1EC, uVar1);
    ov102_021E94A4(param_2);
    return;
}
