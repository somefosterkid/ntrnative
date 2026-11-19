#include "global.h"

#include "overlay_102_app_021e937c.h"

void ov102_021E94A4(void *);

void ov102_021EA71C(void *, u8);

s32 ov102_021EA754(void *);

void ov102_021EAA3C(void *);

void ov102_021EADF8(void *, u32);

void ov102_021EAE40(u8 *, u32);

void ov102_021EAF50(void *);

void ov102_021EAF5C(void *, u32);

void ov102_021EAFAC(void *);

void ov102_021EB67C(void *);

void ov102_021EBB1C(void *);

void ov102_021EBB2C(u8 *);

s32 ov102_021EBB44(void *);

void ov102_021EBB88(u8 *);

s32 ov102_021EBBC4(void *);

void ov102_021EBEA4(void *);

s32 ov102_021EBEC8(u8 *);

void ov102_021EBEF4(u8 *, u32);

void ov102_021EC290(void *, u32);

void ov102_021E9D44(u32 param_1, UnkStruct_ov102_021E978C *param_2);

void ov102_021E9D44(u32 param_1, UnkStruct_ov102_021E978C *param_2) {
    s32 iVar1;
    s32 iVar2;
    Ov102App_021E937C *iVar3;

    iVar3 = param_2->unk_0;
    switch (param_2->unk_14) {
    case 0:
        ov102_021EB67C(iVar3->unk_1E4);
        ov102_021EBEF4(iVar3->unk_1EC, 0);
        ov102_021EC290(iVar3->unk_1F0, 0);
        ov102_021EBEA4(iVar3->unk_1EC);
        ov102_021EADF8(iVar3->unk_1E0, 0);
        ov102_021EAE40(iVar3->unk_1E0, 0);
        ov102_021EA71C(iVar3->unk_1E0, 1);
        param_2->unk_14 = param_2->unk_14 + 1;
        return;
    case 1:
        iVar2 = ov102_021EBEC8(iVar3->unk_1EC);
        iVar1 = ov102_021EA754(iVar3->unk_1E0);
        if ((iVar2 != 0) && (iVar1 != 0)) {
            ov102_021EBB1C(iVar3->unk_1E8);
            ov102_021EBB88(iVar3->unk_1E8);
            param_2->unk_14 = param_2->unk_14 + 1;
            return;
        }
        break;
    case 2:
        iVar2 = ov102_021EBBC4(iVar3->unk_1E8);
        if (iVar2 != 0) {
            ov102_021EBB2C(iVar3->unk_1E8);
            param_2->unk_14 = param_2->unk_14 + 1;
            return;
        }
        break;
    case 3:
        iVar2 = ov102_021EBB44(iVar3->unk_1E8);
        if (iVar2 != 0) {
            ov102_021EAA3C(iVar3->unk_1E0);
            ov102_021EAF5C(iVar3->unk_1E0, 1);
            ov102_021EAFAC(iVar3->unk_1E0);
            ov102_021EAF50(iVar3->unk_1E0);
            iVar3->unk_1F4 = 0;
            ov102_021E94A4(param_2);
        }
    }
    return;
}
