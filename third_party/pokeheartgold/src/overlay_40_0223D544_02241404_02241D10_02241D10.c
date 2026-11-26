#include "global.h"

#include "unk_02005D10.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

#include "overlay_40_unk_struct_ov62_02241204.h"

void ov40_0222BF80(UnkStruct_0208C06C *, int);

void ov40_0222DF60(UnkStruct_0208C06C *, int);

void ov40_02230944(UnkStruct_0208C06C *);

s32 ov40_0223DB94(void *);

void ov40_0223DDE8(UnkStruct_0208C06C *, int, int);

void ov40_0223DEB8(UnkStruct_0208C06C *);

u8 sub_02031620(void *);

u8 sub_0203162C(void *);

void ov40_02241D10(u32 param_1, u32 param_2, UnkStruct_0208C06C *param_3);

void ov40_02241D10(u32 param_1, u32 param_2, UnkStruct_0208C06C *param_3) {
    u8 uVar1;
    s32 iVar2;
    UnkStruct_ov62_02241204 *iVar3;
    iVar3 = param_3->unk_860;
    if (param_2 == 0) {
        switch (param_1) {
        case 0:
            ov40_02230944(param_3);
            iVar3->unk_4C5 = (s8)param_1;
            ov40_0222BF80(param_3, 5);
            return;

        case 1:
            ov40_02230944(param_3);
            iVar3->unk_4C5 = (s8)param_1;
            ov40_0222BF80(param_3, 5);
            return;

        case 2:
            iVar2 = sub_02031620(param_3->unk_88C[0]);
            ov40_02230944(param_3);
            if (iVar2 == 0) {
                PlaySE(0x0000057c);
                ov40_0222DF60(param_3, 128);
                return;
            }
            uVar1 = -1;
            if (iVar3->unk_4C3 == uVar1) {
                uVar1 = sub_02031620(param_3->unk_88C[0]);
                iVar3->unk_4C3 = uVar1;
                uVar1 = sub_0203162C(param_3->unk_88C[0]);
                iVar3->unk_4C4 = uVar1;
            } else {
                iVar3->unk_4C3 = 0xff;
                iVar3->unk_4C4 = 0xff;
            }
            ov40_0223DDE8(param_3, iVar3->unk_4C3, iVar3->unk_4C4);
            ov40_0223DEB8(param_3);
            return;

        case 3:
            ov40_02230944(param_3);
            ov40_0222BF80(param_3, 4);
            return;

        case 4:
            ov40_02230944(param_3);
            iVar2 = ov40_0223DB94(param_3);
            if (iVar2 == 0) {
                PlaySE(0x0000057c);
                ov40_0222DF60(param_3, 0x74);
                return;
            }
            ov40_0222BF80(param_3, 0xc);
        }
    }
    return;
}
