#include "global.h"

#include "overlay_102_app_021e937c.h"

void ov102_021E94A4(void *);

void ov102_021EAE40(u8 *, u32);

void ov102_021EA128(u32 param_1, UnkStruct_ov102_021E978C *param_2);

void ov102_021EA128(u32 param_1, UnkStruct_ov102_021E978C *param_2) {
    Ov102App_021E937C *view = param_2->unk_0;

    ov102_021EAE40(view->unk_1E0, 1);
    ov102_021E94A4(param_2);
    return;
}

u32 ov102_021E9090(s32);

void ov102_021EB170(void *, u32);

void ov102_021EA144(u32 param_1, s32 *param_2);

void ov102_021EA144(u32 param_1, s32 *param_2) {
    u32 uVar1;
    s32 iVar2;

    iVar2 = *param_2;
    uVar1 = ov102_021E9090(param_2[1]);
    ov102_021EB170(*(void **)(iVar2 + 0x1e0), uVar1);
    ov102_021E94A4(param_2);
    return;
}
