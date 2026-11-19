#include "global.h"

#include "bg_window.h"
#include "pm_string.h"
#include "unk_02005D10.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"




typedef struct {
    Window unk_00;
    u32 unk_10;
    String *unk_14;
    UnkStruct_ov114_0225CBF0 unk_18;
    s16 unk_2C;
    s16 unk_2E;
} UnkStruct_ov114_0225E1A4;

void ov90_02258CF0(UnkStruct_ov114_0225CBF0 *, u32, u32, u32, u32);

s32 ov90_02258D4C(UnkStruct_ov114_0225CBF0 *, s32);

void ov90_0225A60C(UnkStruct_ov114_0225E1A4 *, UnkStruct_ov114_0225CEF0 *, fx32);

extern const u32 ov90_0225C228[];

extern const u32 ov90_0225C234[];

extern const u32 ov90_0225C240[];

extern const u32 ov90_0225C24C[];

BOOL ov90_0225A428(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1);

BOOL ov90_0225A428(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1) {
    s32 iVar1;

    switch (param0->unk_2E) {
    case 0:
        iVar1 = ov90_02258D4C(&param0->unk_18, (s32)param0->unk_2C);
        ov90_0225A60C(param0, param1, (param0->unk_18).unk_00 >> 0xc);
        param0->unk_2C = param0->unk_2C + 1;
        if (iVar1 == 1) {
            PlaySE(0x581);
            ov90_02258CF0(&param0->unk_18, ov90_0225C228[param0->unk_10], ov90_0225C234[param0->unk_10], ov90_0225C240[param0->unk_10], 4);
            param0->unk_2E = param0->unk_2E + 1;
            param0->unk_2C = 0;
        }
        break;
    case 1:
        iVar1 = ov90_02258D4C(&param0->unk_18, (s32)param0->unk_2C);
        ov90_0225A60C(param0, param1, (param0->unk_18).unk_00 >> 0xc);
        param0->unk_2C = param0->unk_2C + 1;
        if (iVar1 == 1) {
            ov90_02258CF0(&param0->unk_18, ov90_0225C234[param0->unk_10], ov90_0225C228[param0->unk_10], ov90_0225C24C[param0->unk_10], 4);
            param0->unk_2E = param0->unk_2E + 1;
            param0->unk_2C = 0;
        }
        break;
    case 2:
        iVar1 = ov90_02258D4C(&param0->unk_18, (s32)param0->unk_2C);
        ov90_0225A60C(param0, param1, (param0->unk_18).unk_00 >> 0xc);
        param0->unk_2C = param0->unk_2C + 1;
        if (iVar1 == 1) {
            param0->unk_2E = param0->unk_2E + 1;
        }
        break;
    case 3:
        return 1;
    }
    return 0;
}
