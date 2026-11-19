#include "global.h"

#include "error_handling.h"
#include "sprite.h"

void ov96_021EAB94(s32 *param_1, BOOL param_2);

void ov96_021EAC0C(s32 *param_1, s32 param_2);

void ov96_021EAD88(s32 *param_1, s32 param_2, s32 param_3, s32 param_4);

void ov96_021EAE4C(s32 *param_1, s32 param_2, s32 param_3);

void ov96_021EAE9C(u32 *param_1, s32 *param_2, s32 *param_3);

void ov96_021EAED4(s32 *param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5);

struct Sprite *ov96_021EB5B8(s32 param_1);

void ov96_021FB8B4(u32 *param_1, s32 param_2);

void ov96_021FB8FC(u32 *param_1, s32 param_2, u32 param_3);

void sub_0200606C(u32, s32);

void sub_020061D0(s32, s32);

extern const u8 ov96_0221C55C[];

extern const u8 ov96_0221C589[];

extern const u8 ov96_0221DC28[];

extern const u8 ov96_0221DC2C[];

void ov96_021FA83C(s32 param_1, u32 *param_2, s32 param_3);

void ov96_021FA83C(s32 param_1, u32 *param_2, s32 param_3) {
    BOOL bVar1;
    Sprite *pSVar2;
    VecFx32 *pVVar3;
    s32 iVar4;
    s32 iVar5;
    s32 iStack_30;
    s32 iStack_34;
    VecFx32 VStack_20;
    VecFx32 VStack_2c;

    switch (param_2[1]) {
    case 1:
        *(u8 *)(param_2 + 3) = *(u8 *)(param_2 + 3) + 1;
        iVar4 = param_2[2];
        switch (iVar4) {
        case 1:
            iVar4 = (s32)((s8 *)(((u8 *)ov96_0221C55C) + (u32) * (u8 *)(param_2 + 0xd) * 9))[*(u8 *)(param_2 + 3) - 1];
            iVar5 = param_2[5] - iVar4;
            if (*(u8 *)(param_2 + 0xd) == 0) {
                ov96_021FB8FC(param_2, 0, param_2[5]);
                ov96_021EAE4C((s32 *)*param_2, param_2[4], iVar5);
            } else {
                ov96_021FB8B4(param_2, iVar4);
                ov96_021EAD88((s32 *)*param_2, param_2[4], iVar5, 0);
            }
            if (*(u8 *)(param_2 + 3) >= *(u16 *)(param_2 + 0xe)) {
                param_2[2] = 2;
                *(u8 *)(param_2 + 3) = 0;
                if (*(u8 *)(param_2 + 0xd) == 0) {
                    ov96_021EAB94((s32 *)*param_2, 0);
                }
            }
            break;
        case 2:
            if (*(u8 *)(param_2 + 3) >= *(u16 *)((u8 *)param_2 + 0x3a)) {
                param_2[2] = 3;
                *(u8 *)(param_2 + 3) = 0;
                if (*(u8 *)(param_2 + 0xd) == 0) {
                    ov96_021EAB94((s32 *)*param_2, 1);
                    sub_0200606C(0x8aa, *(u8 *)(((u8 *)ov96_0221DC2C) + param_1));
                    sub_020061D0(*(u8 *)(param_1 + ((u8 *)ov96_0221DC2C)), (s32) * (s8 *)(param_1 + ((u8 *)ov96_0221DC28)));
                }
            }
            break;
        case 3:
            iVar4 = (s32)((s8 *)(((u8 *)ov96_0221C589) + (u32) * (u8 *)(param_2 + 0xd) * 9))[*(u8 *)(param_2 + 3) - 1];
            iVar5 = param_2[5] - iVar4;
            if (*(u8 *)(param_2 + 0xd) == 0) {
                ov96_021FB8FC(param_2, 1, param_2[5]);
                ov96_021EAE4C((s32 *)*param_2, param_2[4], iVar5);
            } else {
                ov96_021FB8B4(param_2, iVar4);
                ov96_021EAD88((s32 *)*param_2, param_2[4], iVar5, 0);
            }
            if (*(u8 *)(param_2 + 3) >= *(u16 *)(param_2 + 0xe)) {
                param_2[1] = 0;
                param_2[2] = 0;
                *(u8 *)(param_2 + 3) = 0;
                ov96_021EAD88((s32 *)*param_2, param_2[4], param_2[5], 0);
                if (*(u8 *)(param_2 + 0xd) != 0) {
                    sub_0200606C(0x8a8, *(u8 *)(((u8 *)ov96_0221DC2C) + param_1));
                    sub_020061D0(*(u8 *)(param_1 + ((u8 *)ov96_0221DC2C)), (s32) * (s8 *)(param_1 + ((u8 *)ov96_0221DC28)));
                }
                ov96_021FB8B4(param_2, 0);
                *(u16 *)(param_2 + 0xf) = 0;
                *(u16 *)((u8 *)param_2 + 0x3e) = 0;
            } else {
                if ((s32)(u32) * (u8 *)(param_2 + 3) < (s32)(*(u16 *)(param_2 + 0xe) - 5)) {
                    *(u16 *)(param_2 + 0xf) = 0;
                }
            }
            break;
        default:
            GF_AssertFail();
            break;
        }
        break;
    case 2:
        *(u8 *)(param_2 + 3) = *(u8 *)(param_2 + 3) + 1;
        if (((u8 *)param_2)[0xd] == 0) {
            switch (((u8 *)param_2)[0xe]) {
            case 1:
                ((u8 *)param_2)[0xe] = 3;
                break;
            case 3:
                ((u8 *)param_2)[0xe] = 2;
                break;
            case 2:
                ((u8 *)param_2)[0xe] = 4;
                break;
            case 4:
                ((u8 *)param_2)[0xe] = 1;
            }
            ov96_021EAC0C((s32 *)*param_2, (u32)((u8 *)param_2)[0xe]);
        }
        ((u8 *)param_2)[0xd] = (((u8 *)param_2)[0xd] + 1) % 4;
        if (0x1e < *(u8 *)(param_2 + 3)) {
            *(u8 *)(param_2 + 3) = 0;
            param_2[1] = 0;
            param_2[2] = 0;
            ov96_021EAC0C((s32 *)*param_2, 1);
        }
        break;
    case 3:
        iVar4 = param_2[6];
        bVar1 = FALSE;
        if (iVar4 < 0x78) {
            param_3 = 1;
            if (0x78 < iVar4 + 1) {
                param_3 = iVar4 + -0x78;
            }
            bVar1 = TRUE;
        } else if (0x78 < iVar4) {
            param_3 = -1;
            if (iVar4 + -1 < 0x78) {
                param_3 = 0x78 - iVar4;
            }
            bVar1 = TRUE;
        }
        if (bVar1) {
            param_2[6] = param_2[6] + param_3;
            param_2[7] = param_2[7] + param_3;
            ov96_021EAED4((s32 *)*param_2, 0, param_3, 1, 1);
        }
        break;
    case 0:
        *(u16 *)(param_2 + 0xf) = 0;
        *(u16 *)((u8 *)param_2 + 0x3e) = 0;
        break;
    }
    pSVar2 = ov96_021EB5B8(param_2[8]);
    pVVar3 = Sprite_GetMatrixPtr(pSVar2);
    VStack_20 = *pVVar3;
    VStack_20.y = param_2[7] << 0xc;
    Sprite_SetMatrix(pSVar2, &VStack_20);
    ov96_021EAE9C((u32 *)*param_2, &iStack_30, &iStack_34);
    pSVar2 = ov96_021EB5B8(param_2[10]);
    pVVar3 = Sprite_GetMatrixPtr(pSVar2);
    VStack_2c = *pVVar3;
    VStack_2c.y = iStack_34 << 0xc;
    Sprite_SetMatrix(pSVar2, &VStack_2c);
    return;
}
