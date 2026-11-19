#include "global.h"

#include "touchscreen.h"
#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
u32 ov14_021E6AA0(UnkStruct_ov14_boxapp *param_1, u32 param_2, u32 param_3);
s32 ov14_021E7960(s16 param_1, s16 param_2);
s32 ov14_021E79AC(s32 param_1, s32 param_2, u8 *param_3);
void ov14_021E7AD4(UnkStruct_ov14_boxapp *param_1, u16 param_2, u32 param_3, u32 param_4);
u32 ov14_021E7B8C(UnkStruct_ov14_boxapp *param_1);
void ov14_021E7FB8(UnkStruct_0201956C *param_1);
void ov14_021E7FEC(UnkStruct_0201956C *param_1);
u32 ov14_021E80A8(UnkStruct_ov14_boxapp *param_1);
void ov14_021E8434(UnkStruct_0201956C *param_1);
u32 ov14_021E8514(void *param_1);
void ov14_021E88F8(UnkStruct_0201956C *param_1);
void ov14_021F29E4(UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);
s32 ov14_021F2A04(UnkStruct_ov14_boxsys *param_1, s32 param_2);
void ov14_021F2A18(UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);
void ov14_021F3488(UnkStruct_ov14_boxapp *param_1, u32 param_2, s32 param_3);
void ov14_021F391C(UnkStruct_ov14_boxsys *param_1, s32 param_2);
void ov14_021F395C(UnkStruct_ov14_boxsys *param_1, s16 param_2, s16 param_3);
void ov14_021F3B3C(UnkStruct_ov14_boxsys *param_1);
void ov14_021F3B5C(UnkStruct_ov14_boxsys *param_1);
void ov14_021F69F0(UnkStruct_ov14_boxapp *param_1, s32 param_2, u32 param_3);
void sub_02019978(UnkStruct_0201956C *, u32);
extern u8 ov14_021F7BF0[];

s32 ov14_021EACD4(UnkStruct_ov14_boxapp *param_1);

s32 ov14_021EACD4(UnkStruct_ov14_boxapp *param_1) {
    s32 iVar1;
    s32 iVar2;
    BOOL BVar4;
    u16 uVar7;
    u16 uVar5;
    UnkStruct_ov14_boxsys *sys;
    u32 auStack_1c;
    u32 uStack_20;

    sys = param_1->sys;
    iVar1 = ov14_021E8514(sys->unk_2F0);
    iVar2 = ov14_021E80A8(param_1);
    sub_02019978(param_1->sys->unk_2F0, 10);
    switch (sys->unk_10) {
    case 0:
        if (param_1->sys->unk_88C8 != 0) {
            ov14_021F391C(param_1->sys, 1);
            ov14_021F29E4(param_1->sys, 0xb, 1);
            ov14_021F2A18(param_1->sys, 0xb, 1);
            sys->unk_10 = 1;
        } else {
            ov14_021E8434(param_1->sys->unk_2F0);
            sys->unk_10 = 10;
        }
        break;
    case 1:
        if (ov14_021F2A04(param_1->sys, 0xb) == 1) {
            break;
        }
        ov14_021F391C(param_1->sys, 0);
        ov14_021F3B3C(param_1->sys);
        sys->unk_10 = 2;
    case 2:
        if (param_1->sys->unk_448[2] == 1 && iVar2 == 0) {
            param_1->sys->unk_448[2] = 2;
            ov14_021F69F0(param_1, 0x28, 2);
            ov14_021F3488(param_1, 0x81, 0);
        }
        BVar4 = System_GetTouchHeldCoords(&auStack_1c, &uStack_20);
        if (BVar4 == 0) {
            if (param_1->sys->unk_448[2] == 2) {
                uVar7 = ov14_021E7960((s16)param_1->sys->unk_40B8, (s16)param_1->sys->unk_40BC);
            } else {
                uVar7 = ov14_021E79AC((s16)param_1->sys->unk_40B8, (s16)param_1->sys->unk_40BC, ov14_021F7BF0);
            }
            uVar5 = uVar7;
            if (uVar7 == 0xff) {
                uVar7 = param_1->unk21;
            } else {
                iVar1 = ov14_021E6AA0(param_1, param_1->unk21, uVar7);
                if (iVar1 == 0) {
                    uVar7 = param_1->unk21;
                }
            }
            if (uVar7 == param_1->unk21) {
                ov14_021E8434(param_1->sys->unk_2F0);
                if (param_1->sys->unk_448[2] != 0) {
                    ov14_021E7FEC(param_1->sys->unk_2F0);
                    ov14_021F3488(param_1, 1, 1);
                }
            } else {
                ov14_021E88F8(param_1->sys->unk_2F0);
            }
            ov14_021E7AD4(param_1, uVar5, uVar7, 1);
            sys->unk_10 = 3;
        } else {
            if (iVar1 == 0) {
                if (param_1->sys->unk_448[2] == 0 && (auStack_1c < 0x10 || uStack_20 < 0x30 || auStack_1c >= 0x68)) {
                    param_1->sys->unk_448[2] = 1;
                    ov14_021E7FB8(param_1->sys->unk_2F0);
                }
            }
            ov14_021F395C(param_1->sys, (s16)auStack_1c, (s16)uStack_20);
            ov14_021F3B5C(param_1->sys);
            param_1->sys->unk_40B8 = auStack_1c;
            param_1->sys->unk_40BC = uStack_20;
        }
        break;
    case 3:
        if (ov14_021E7B8C(param_1) == 0) {
            sys->unk_10 = 10;
        }
        break;
    case 10:
        if (iVar1 == 0 && iVar2 == 0) {
            if (param_1->sys->unk_88C8 != 0) {
                ov14_021F391C(param_1->sys, 0);
                ov14_021F3B3C(param_1->sys);
            }
            sys->unk_10 = 0;
            return 0;
        }
    }
    return 1;
}
#else
u32 ov14_021E6AA0(UnkStruct_ov14_boxapp *param_1, u32 param_2, u32 param_3);

s32 ov14_021E7960(s32 param_1, s32 param_2);

s32 ov14_021E79AC(s32 param_1, s32 param_2, u8 *param_3);

void ov14_021E7AD4(UnkStruct_ov14_boxapp *param_1, u16 param_2, u32 param_3, u32 param_4);

u32 ov14_021E7B8C(UnkStruct_ov14_boxapp *param_1);

void ov14_021E7FB8(UnkStruct_0201956C *param_1);

void ov14_021E7FEC(UnkStruct_0201956C *param_1);

u32 ov14_021E80A8(UnkStruct_ov14_boxapp *param_1);

void ov14_021E8434(UnkStruct_0201956C *param_1);

u32 ov14_021E8514(void *param_1);

void ov14_021E88F8(UnkStruct_0201956C *param_1);

void ov14_021F29E4(UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);

s32 ov14_021F2A04(struct UnkStruct_ov14_boxsys *param_1, s32 param_2);

void ov14_021F2A18(struct UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);

void ov14_021F3488(UnkStruct_ov14_boxapp *param_1, u32 param_2, s32 param_3);

void ov14_021F391C(UnkStruct_ov14_boxsys *param_1, s32 param_2);

void ov14_021F395C(struct UnkStruct_ov14_boxsys *param_1, s16 param_2, s16 param_3);

void ov14_021F3B3C(void *param_1);

void ov14_021F3B5C(void *param_1);

void ov14_021F69F0(UnkStruct_ov14_boxapp *param_1, s32 param_2, u32 param_3);

void sub_02019978(UnkStruct_0201956C *, u32);

extern u8 ov14_021F7BF0[];

s32 ov14_021EACD4(UnkStruct_ov14_boxapp *param_1);

s32 ov14_021EACD4(UnkStruct_ov14_boxapp *param_1) {
    s32 iVar1;
    s32 iVar2;
    BOOL BVar4;
    u16 uVar7;
    u16 uVar5;
    u32 extraout_r1 = 0;
    u32 extraout_r1_00 = 0;
    u32 in_r3 = 0;
    u8 *iVar6;
    u32 auStack_1c;
    u32 uStack_20;

    iVar6 = ((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys);
    iVar1 = ov14_021E8514(*(void **)(iVar6 + 0x2f0));
    iVar2 = ov14_021E80A8(param_1);
    sub_02019978(*(void **)(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x2f0), 10);
    switch (*(u16 *)(iVar6 + 0x10)) {
    case 0:
        iVar1 = *((s32 *)&((UnkStruct_ov14_boxapp *)param_1)->sys);
        if (((UnkStruct_ov14_boxsys *)iVar1)->unk_88C8 != 0) {
            ov14_021F391C((UnkStruct_ov14_boxsys *)iVar1, 1);
            ov14_021F29E4(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys), 0xb, 1);
            ov14_021F2A18(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys), 0xb, 1);
            *(u16 *)(iVar6 + 0x10) = 1;
        } else {
            ov14_021E8434(((UnkStruct_ov14_boxsys *)iVar1)->unk_2F0);
            *(u16 *)(iVar6 + 0x10) = 10;
        }
        break;
    case 1:
        if (ov14_021F2A04(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys), 0xb) == 1) {
            break;
        }
        ov14_021F391C((UnkStruct_ov14_boxsys *)*((s32 *)&((UnkStruct_ov14_boxapp *)param_1)->sys), 0);
        ov14_021F3B3C(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys));
        *(u16 *)(iVar6 + 0x10) = 2;
    case 2:
        if ((*(u8 *)(*((s32 *)&((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x44a) == 1) && (iVar2 == 0)) {
            *(u8 *)(*((s32 *)&((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x44a) = 2;
            ov14_021F69F0(param_1, 0x28, 2);
            ov14_021F3488(param_1, 0x81, 0);
        }
        BVar4 = System_GetTouchHeldCoords(&auStack_1c, &uStack_20);
        if (BVar4 == 0) {
            iVar1 = *((s32 *)&((UnkStruct_ov14_boxapp *)param_1)->sys);
            if (*((u8 *)&((UnkStruct_ov14_boxsys *)iVar1)->unk_448[2]) == 2) {
                uVar7 = ov14_021E7960((s32)(s16) *((u32 *)&((UnkStruct_ov14_boxsys *)iVar1)->unk_40B8),
                    (s32)(s16) *((u32 *)&((UnkStruct_ov14_boxsys *)iVar1)->unk_40BC));
            } else {
                uVar7 = ov14_021E79AC((s32)(s16) *((u32 *)&((UnkStruct_ov14_boxsys *)iVar1)->unk_40B8),
                    (s32)(s16) *((u32 *)&((UnkStruct_ov14_boxsys *)iVar1)->unk_40BC),
                    (u8 *)((u8 *)ov14_021F7BF0));
            }
            uVar5 = uVar7;
            if (uVar7 == 0xff) {
                uVar7 = (u32) ((UnkStruct_ov14_boxapp *)param_1)->unk21;
            } else {
                iVar1 = ov14_021E6AA0(param_1, (u32) ((UnkStruct_ov14_boxapp *)param_1)->unk21, uVar7);
                if (iVar1 == 0) {
                    uVar7 = (u32) ((UnkStruct_ov14_boxapp *)param_1)->unk21;
                }
            }
            if (uVar7 == ((UnkStruct_ov14_boxapp *)param_1)->unk21) {
                ov14_021E8434(*(UnkStruct_0201956C **)(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x2f0));
                if (*(u8 *)(*((s32 *)&((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x44a) != 0) {
                    ov14_021E7FEC(*(UnkStruct_0201956C **)(*((s32 *)&((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x2f0));
                    ov14_021F3488(param_1, 1, 1);
                }
            } else {
                ov14_021E88F8(*(UnkStruct_0201956C **)(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x2f0));
            }
            ov14_021E7AD4(param_1, (u16)uVar5, uVar7, 1);
            *(u16 *)(iVar6 + 0x10) = 3;
        } else {
            if (iVar1 == 0) {
                iVar1 = *((s32 *)&((UnkStruct_ov14_boxapp *)param_1)->sys);
                if (*((u8 *)&((UnkStruct_ov14_boxsys *)iVar1)->unk_448[2]) == 0 && (auStack_1c < 0x10 || uStack_20 < 0x30 || auStack_1c >= 0x68)) {
                    *((u8 *)&((UnkStruct_ov14_boxsys *)iVar1)->unk_448[2]) = 1;
                    ov14_021E7FB8(*(UnkStruct_0201956C **)(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x2f0));
                }
            }
            ov14_021F395C(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys), (s16)auStack_1c, (s16)uStack_20);
            ov14_021F3B5C(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys));
            *(u32 *)(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x40b8) = auStack_1c;
            *(u32 *)(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x40bc) = uStack_20;
        }
        break;
    case 3:
        if (ov14_021E7B8C(param_1) == 0) {
            *(u16 *)(iVar6 + 0x10) = 10;
        }
        break;
    case 10:
        if ((iVar1 == 0) && (iVar2 == 0)) {
            if (*(u16 *)(*((s32 *)&((UnkStruct_ov14_boxapp *)param_1)->sys) + 0x88c8) != 0) {
                ov14_021F391C((UnkStruct_ov14_boxsys *)*((s32 *)&((UnkStruct_ov14_boxapp *)param_1)->sys), 0);
                ov14_021F3B3C(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys));
            }
            *(u16 *)(iVar6 + 0x10) = 0;
            return 0;
        }
    }
    return 1;
}
#endif
