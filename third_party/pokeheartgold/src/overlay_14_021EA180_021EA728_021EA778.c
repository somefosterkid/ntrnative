#include "global.h"

#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021EA778(UnkStruct_ov14_boxapp *param_1);

void ov14_021F3B3C(void *arg0);
u32 ov14_021E7960(s32, s32);
s32 ov14_021E79AC(s32, s32, u8 *);
void ov14_021F395C(struct UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021F3B5C(void *);

s32 ov14_021E6AA0(UnkStruct_ov14_boxapp *, s32, s32);
void ov14_021E7AD4(UnkStruct_ov14_boxapp *, u16, u32, u32);
void ov14_021F29E4(UnkStruct_ov14_boxsys *, u32, u32);
s32 ov14_021F2A04(struct UnkStruct_ov14_boxsys *, u32);
void ov14_021F2A18(struct UnkStruct_ov14_boxsys *, u32, u32);
void ov14_021F391C(UnkStruct_ov14_boxsys *, u32);
void ov14_021F40E8(UnkStruct_ov14_boxapp *, u32);

void ov14_021E8620(UnkStruct_0201956C *arg0);

s32 ov14_021E7B8C(UnkStruct_ov14_boxapp *);

void sub_02019978(void *, s32);

extern u32 ov14_021F7C08;
s32 ov14_021EA778(UnkStruct_ov14_boxapp *param_1) {
    UnkStruct_ov14_boxapp *new_var2;
    u16 uVar5;
    s32 iVar3;
    u8 *iVar4;
    u16 uVar1;
    u32 auStack_1c;
    BOOL BVar2;
    UnkStruct_0201956C **new_var;
    iVar4 = ((u8 *)param_1->sys);
    sub_02019978(((void *)((UnkStruct_ov14_boxsys *)iVar4)->unk_2F0), 0xe);
    new_var2 = param_1;
    sub_02019978(((u8 *)param_1->sys->unk_2F0), 0xf);
    switch (((UnkStruct_ov14_boxsys *)iVar4)->unk_10) {
    case 0:
        ov14_021F391C(*((void **)&param_1->sys), 1);
        ov14_021F29E4(*((void **)&param_1->sys), 0xb, 1);
        ov14_021F2A18(*((void **)&param_1->sys), 0xb, 1);
        ((UnkStruct_ov14_boxsys *)iVar4)->unk_10 = 1;
        break;

    case 1:
        iVar3 = ov14_021F2A04(*((void **)&param_1->sys), 0xb);
        if (iVar3 == 0) {
            ov14_021F391C(*((void **)&param_1->sys), 0);
            ov14_021F3B3C(((void *)param_1->sys));
            ((UnkStruct_ov14_boxsys *)iVar4)->unk_10 = 2;
        }
        break;

    case 2:
        u32 uStack_20;
        BVar2 = System_GetTouchHeldCoords(&auStack_1c, &uStack_20);
        if (BVar2 == 0) {
            uVar1 = (iVar3 = ov14_021E7960((s32)((s16)(*((u32 *)&param_1->sys->unk_40B8))), (s32)((s16)(*((u32 *)&param_1->sys->unk_40BC)))));
            if (uVar1 == 0xff) {
                uVar1 = (int)ov14_021E79AC((s32)((s16)(*((u32 *)&param_1->sys->unk_40B8))), (s32)((s16)(*((u32 *)&param_1->sys->unk_40BC))), (unsigned char *)(&ov14_021F7C08));
            }
            uVar5 = uVar1;
            if (uVar1 == 0xff) {
                uVar1 = (u16)(param_1->unk21);
            } else {
                iVar3 = ov14_021E6AA0(param_1, param_1->unk21, uVar1);
                if (iVar3 == 0) {
                    uVar1 = (u16)(param_1->unk21);
                }
            }
            ov14_021E7AD4(param_1, uVar5, uVar1, 2);
            ((UnkStruct_ov14_boxsys *)iVar4)->unk_10 = 3;
        } else {
            ov14_021F395C(*((void **)&param_1->sys), (s32)((s16)auStack_1c), (s32)((s16)uStack_20));
            ov14_021F3B5C(*((void **)&((UnkStruct_ov14_boxapp *)new_var2)->sys));
            *((u32 *)&param_1->sys->unk_40B8) = auStack_1c;
            *((u32 *)&param_1->sys->unk_40BC) = uStack_20;
        }
        break;

    case 3:
        iVar3 = ov14_021E7B8C(param_1);
        if (iVar3 == 0) {
            PlaySE(0x5ea);
            new_var = (UnkStruct_0201956C **)((((u8 *)param_1->sys)) + 0x2f0);
            ov14_021E8620(*new_var);
            ((UnkStruct_ov14_boxsys *)iVar4)->unk_10 = 4;
        }
        break;

    case 4:
        ov14_021F391C(*((void **)&param_1->sys), 1);
        ov14_021F29E4(*((void **)&param_1->sys), 0xb, 2);
        ov14_021F40E8(param_1, 0);
        ((UnkStruct_ov14_boxsys *)iVar4)->unk_10 = 5;
        break;

    case 5:
        iVar3 = ov14_021F2A04(*((void **)&param_1->sys), 0xb);
        if (iVar3 == 0) {
            ov14_021F2A18(*((void **)&param_1->sys), 0xb, 0);
            ov14_021F391C(*((void **)&param_1->sys), 0);
            ((UnkStruct_ov14_boxsys *)iVar4)->unk_10 = 6;
        }
        break;

    case 6:
        ((UnkStruct_ov14_boxsys *)iVar4)->unk_10 = 0;
        return 0;
    }

    return 1;
}
