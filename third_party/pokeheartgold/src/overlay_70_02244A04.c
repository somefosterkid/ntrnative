#include "global.h"

#include "sav_system_info.h"
#include "unk_0202C034.h"
#include "unk_0203170C.h"
#include "unk_02037C94.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_022378C0(s32, s32, s32);

u32 ov70_02244A04(GTSApplicationState *param_1);

s32 ov70_022382C0(void);
s32 ov70_02244A4C(GTSApplicationState *arg0);
s32 ov70_02237F38(void);
s32 ov70_02237F58(void);
s32 ov70_02238F80(void *);
s32 ov70_02244A68(GTSApplicationState *arg0);
s32 ov70_02238304(void *, void *);
s32 ov70_02244B20(GTSApplicationState *arg0);

u32 ov70_02244A04(GTSApplicationState *param_1) {
    DWCUserData *userdata;
    s32 sVar1;
    u64 uVar2;

    userdata = sub_0202C08C(*(WiFiList **)((u8 *)param_1->playerData + 0x14));
    sVar1 = Save_SysInfo_GetDwcProfileId(*(SysInfo **)((u8 *)param_1->playerData + 4));
    if (sVar1 == 0) {
        Save_SysInfo_SetDwcProfileId(*(SysInfo **)((u8 *)param_1->playerData + 4), *(s32 *)((u8 *)param_1->playerData + 0x34));
    }
    sVar1 = Save_SysInfo_GetDwcProfileId(*(SysInfo **)((u8 *)param_1->playerData + 4));
    uVar2 = DWC_CreateFriendKey(userdata);
    ov70_022378C0(sVar1, (s32)uVar2, (s32)(uVar2 >> 0x20));
    param_1->currentScreenInstruction = 7;
    return 3;
}

s32 ov70_02244A4C(GTSApplicationState *arg0) {
    ov70_022382C0();
    arg0->currentScreenInstruction = 8;
    arg0->networkTimeoutCounter = 0;
    return 3;
}

s32 ov70_02244A68(GTSApplicationState *arg0) {
    s32 v1;
    u32 v0;

    if (ov70_02237F38() != 0) {
        v1 = ov70_02237F58();
        arg0->networkTimeoutCounter = 0;
        v0 = v1 + 0xF;
        switch (v0) {
        case 15:
            arg0->currentScreenInstruction = 9;
            break;
        case 16:
            ov70_02238F80(arg0);
            arg0->commsErrorMessage = v1;
            arg0->currentScreenInstruction = 0x15;
            break;
        case 14:
        case 17:
            ov70_02238F80(arg0);
            arg0->commsErrorMessage = v1;
            arg0->currentScreenInstruction = 0x15;
            break;
        case 0:
        case 3:
            ov70_02238F80(arg0);
            arg0->commsErrorMessage = v1;
            arg0->currentScreenInstruction = 0x15;
            break;
        case 1:
        case 13:
            ov70_02238F80(arg0);
            arg0->commsErrorMessage = v1;
            arg0->currentScreenInstruction = 0x15;
            break;
        default:
            ov70_02238F80(arg0);
            sub_020399EC();
            break;
        }
    } else {
        arg0->networkTimeoutCounter = (s32)(arg0->networkTimeoutCounter + 1);
        if (arg0->networkTimeoutCounter == 0xE10) {
            sub_020399EC();
        }
    }
    return 3;
}

s32 ov70_02244B20(GTSApplicationState *arg0) {
    sub_0203189C(*(SaveData **)((u8 *)arg0->playerData + 0x20), (Unk020317F4 *)&arg0->worldExchangeTrainer);
    ov70_02238304(&arg0->worldExchangeTrainer, &arg0->worldExchangeTrainerError);
    arg0->currentScreenInstruction = 0xA;
    arg0->networkTimeoutCounter = 0;
    return 3;
}
