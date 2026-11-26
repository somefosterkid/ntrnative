#include "global.h"

#include "frontier/overlay_80.h"

#include "overlay_40.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_40_unk_struct_ov62_02241204.h"

s32 ov39_02227534(void *, void *);

s32 ov39_02227D44(void *, u8 **);

void ov40_0222BF80(UnkStruct_0208C06C *, u32);

void ov40_0222DED0(UnkStruct_0208C06C *, u32);

void ov40_0222DFB0(UnkStruct_0208C06C *);

void ov40_0222FB28(UnkStruct_0208C06C *, u32);

void ov40_02230CDC(UnkStruct_0208C06C *, u32, u32, u32);

void *ov40_0223D540(UnkStruct_0208C06C *);

BOOL ov40_0223D5CC(UnkStruct_0208C06C *param0);

void ov40_02242E4C(UnkStruct_ov62_02241204 *, UnkStruct_0208C06C *);

void sub_0202FC90(void *, u32, u8 *, void *, u32);

void *sub_020314A4(u32);

void sub_020314BC(void *);

void sub_020314C4(void *, void *);

void sub_020879E0(void *, u32);

void sub_02087A08(void *, u32, u32);

u32 ov40_022417DC(UnkStruct_0208C06C *param_1) {
    s32 iVar1;
    void *uVar2;
    BOOL BVar3;
    UnkStruct_ov62_02241204 *iVar4;
    u8 *iStack_10;
    u8 auStack_14[4];

    iVar4 = param_1->unk_860;
    switch (param_1->unk_08) {
    case 0:
        ov40_0222DED0(param_1, 0x77);
        PlaySE(0x57d);
        param_1->unk_08 = param_1->unk_08 + 1;
        break;
    case 1:
        iVar1 = ov40_0223D5CC(param_1);
        if (iVar1 == 0) {
            return 0;
        }
        sub_0202FC90(param_1->saveData, 0x6d, auStack_14, 0, 0);
        uVar2 = sub_020314A4(0x6d);
        iVar4->unk_1D0 = uVar2;
        sub_020314C4(iVar4->unk_1D0, param_1->saveData);
        uVar2 = ov40_0223D540(param_1);
        iVar1 = ov39_02227534(uVar2, iVar4->unk_1D0);
        if (iVar1 == 1) {
            sub_020314BC(iVar4->unk_1D0);
            param_1->unk_08 = param_1->unk_08 + 1;
        }
        break;
    case 2:
        iVar1 = ov40_0223D5CC(param_1);
        if (iVar1 == 0) {
            return 0;
        }
        sub_0202FC24();
        ov40_0222DFB0(param_1);
        uVar2 = ov40_0223D540(param_1);
        iVar1 = ov39_02227D44(uVar2, &iStack_10);
        if (iVar1 == 1) {
            StopSE(0x57d, 0);
            ov40_02230CDC(param_1, 6, *(u32 *)(iStack_10 + 0xc), *(u32 *)(iStack_10 + 4));
            sub_020879E0(param_1->unk_6F0, 0);
            sub_02087A08(param_1->unk_6F0, 0, 0);
            param_1->unk_08 = 0xff;
        } else {
            ov40_0222DED0(param_1, 0x77);
            ov40_02242E4C(iVar4, param_1);
            StopSE(0x57d, 0);
            PlaySE(0x577);
            param_1->unk_08 = param_1->unk_08 + 1;
        }
        break;
    case 3:
        BVar3 = System_GetTouchNew();
        if (BVar3 != 0) {
            ov40_0222DFB0(param_1);
            param_1->unk_08 = param_1->unk_08 + 1;
        }
        break;
    default:
        sub_020879E0(param_1->unk_6F0, 0);
        sub_02087A08(param_1->unk_6F0, 0, 0);
        ov40_0222FB28(param_1, 0x23);
        ov40_0222BF80(param_1, 4);
    }
    return 0;
}
