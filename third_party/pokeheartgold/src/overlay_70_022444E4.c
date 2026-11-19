#include "global.h"
struct GTSApplicationState;
#include "bg_window.h"
#include "list_menu_2d.h"
#include "msgdata.h"
#include "pm_string.h"
#include "system.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"
#include "yes_no_prompt.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_022444E4(s32 arg0, void *arg1);

void ov70_02245050(Window *, String *, u32, u32, u32, u32);
void ov70_0224458C(GTSApplicationState *param_1);
void ov70_02244644(GTSApplicationState *param_1);

void ov70_02244670(struct GTSApplicationState *arg0);
void ov70_022446A8(struct GTSApplicationState *arg0);
s32 ov70_02238D84(void *, s32, s32);
s32 ov70_02244FA4(void *, MsgData *, s32, s32, s32);
s32 ov70_022446D0(struct GTSApplicationState *arg0);
s32 ov70_02238C8C(GTSApplicationState *arg0);
s32 ov70_02238E50(void *, s32, s32);
s32 ov70_02238F64(GTSApplicationState *);
s32 ov70_02244708(struct GTSApplicationState *arg0);
s32 ov70_02244774(struct GTSApplicationState *arg0);
s32 ov70_022447A0(GTSApplicationState *arg0);
s32 ov70_02244804(struct GTSApplicationState *arg0);
s32 ov70_02244834(GTSApplicationState *arg0);
s32 ov70_02244854(struct GTSApplicationState *arg0);
SysTask *ov70_02245124(void *);
void ov70_02244888(GTSApplicationState *arg0);

void ov70_022444E4(s32 arg0, void *arg1) {
    u32 v0;

    if ((*(s32 *)((u8 *)arg1 + 4) != 0) && (*(u8 *)((u8 *)arg1 + 0x32B) = (u8)(*(u8 *)((u8 *)arg1 + 0x32B) ^ 1), ((1 & *(u8 *)((u8 *)arg1 + 0x32B)) == 0))) {
        if ((u32) * (u32 *)((u8 *)arg1 + 0x32C) <= 1U) {
            GX_LoadBGPltt(arg1 + 0x88 + (*(s16 *)((u8 *)arg1 + 0x328) << 5), 0U, 0x20U);
        }
        v0 = *(u32 *)((u8 *)arg1 + 0x32C);
        if ((v0 == 0) || (v0 == 2)) {
            GXS_LoadBGPltt(arg1 + 0x88 + (*(s16 *)((u8 *)arg1 + 0x328) << 5), 0U, 0x20U);
        }
        if (*(s8 *)((u8 *)arg1 + 0x32A) == 0) {
            *(s16 *)((u8 *)arg1 + 0x328) = (s16)(*(s16 *)((u8 *)arg1 + 0x328) + 1);
            if ((s32) * (s16 *)((u8 *)arg1 + 0x328) >= 0x15) {
                *(s16 *)((u8 *)arg1 + 0x328) = 0x13;
                *(s8 *)((u8 *)arg1 + 0x32A) = (s8)(1 ^ *(s8 *)((u8 *)arg1 + 0x32A));
            }
        } else {
            *(s16 *)((u8 *)arg1 + 0x328) = (s16)(*(s16 *)((u8 *)arg1 + 0x328) - 1);
            if ((s32) * (s16 *)((u8 *)arg1 + 0x328) < 0) {
                *(s16 *)((u8 *)arg1 + 0x328) = 1;
                *(s8 *)((u8 *)arg1 + 0x32A) = (s8)(*(s8 *)((u8 *)arg1 + 0x32A) ^ 1);
            }
        }
    }
}

void ov70_0224458C(GTSApplicationState *param_1) {
    AddWindowParameterized(param_1->bgConfig, &param_1->networkErrWindow, 0, 4, 4, 0x17, 0x10, 0xd, 0x94);
    FillWindowPixelBuffer(&param_1->networkErrWindow, 0);
    AddWindowParameterized(param_1->bgConfig, &param_1->titleWindow, 0, 4, 1, 0x18, 2, 0xd, 0x204);
    FillWindowPixelBuffer(&param_1->titleWindow, 0);
    ov70_02245050(&param_1->titleWindow, param_1->title, 0, 1, 1, 0xf0e00);
    AddWindowParameterized(param_1->bgConfig, &param_1->bottomInstructionWindow, 0, 2, 0x13, 0x1b, 4, 0xd, 0x28);
    FillWindowPixelBuffer(&param_1->bottomInstructionWindow, 0);
    return;
}

void ov70_02244644(GTSApplicationState *param_1) {
    RemoveWindow(&param_1->bottomInstructionWindow);
    RemoveWindow(&param_1->titleWindow);
    RemoveWindow(&param_1->networkErrWindow);
    return;
}

void ov70_02244670(GTSApplicationState *arg0) {
    arg0->genericMessageBuffer = String_New(0xB4U, HEAP_ID_61);
    arg0->shortErrorBuffer = String_New(0x100U, HEAP_ID_61);
    arg0->title = NewString_ReadMsgData(arg0->gtsMessageLoader, 0x1F);
}

void ov70_022446A8(GTSApplicationState *arg0) {
    String_Delete(arg0->title);
    String_Delete(arg0->shortErrorBuffer);
    String_Delete(arg0->genericMessageBuffer);
}

s32 ov70_022446D0(GTSApplicationState *arg0) {
    ov70_02244FA4(arg0, arg0->disconnStatusMessageLoader, 0x11, 1, 0xF0F);
    ov70_02238D84(arg0, 0xD, 1);
    *((s16 *)&arg0->deferredBoxId) = 1;
    return 3;
}

s32 ov70_02244708(GTSApplicationState *arg0) {
    s32 v0;

    v0 = ov70_02238C8C(arg0);
    if (v0 == 1) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        ov70_02244FA4(arg0, arg0->connProgressMessageLoader, 1, 1, 0xF0F);
        ov70_02238D84(arg0, 0xC, 2);
        ov70_02238F64(arg0);
    } else if (v0 == 2) {
        YesNoPrompt_Destroy(arg0->unk_11C8);
        sub_0203946C();
        ov70_02238E50(arg0, 0, 0);
        *((s32 *)&arg0->currentScreenInstruction) = 0xB;
    }
    return 3;
}

s32 ov70_02244774(GTSApplicationState *arg0) {
    ov70_02244FA4(arg0, arg0->gtsMessageLoader, 7, 1, 0xF0F);
    ov70_02238D84(arg0, 0xD, 0x10);
    return 3;
}

s32 ov70_022447A0(GTSApplicationState *arg0) {
    s32 v0;

    v0 = Handle2dMenuInput_DeleteOnFinish(arg0->unk_11B0, HEAP_ID_61);
    if (v0 != -1) {
        if (v0 == -2) {
            if (DWC_CheckInet() == 0) {
                arg0->currentScreenInstruction = 0;
            } else {
                ov70_02238E50(arg0, 7, 0xB);
                arg0->returnAfterNetworkScreen = 1;
                arg0->currentScreenInstruction = 0xB;
            }
        } else {
            if (DWC_CheckInet() != 0) {
                DWC_CleanupInet();
            }
            sub_0203946C();
            ov70_02238E50(arg0, 0, 0);
            arg0->currentScreenInstruction = 0xB;
        }
    }
    return 3;
}

s32 ov70_02244804(GTSApplicationState *arg0) {
    ov70_02244FA4(arg0, arg0->disconnStatusMessageLoader, 0x1A, 1, 0xF0F);
    ov70_02238D84(arg0, 0xC, 0x12);
    return 3;
}

s32 ov70_02244834(GTSApplicationState *arg0) {
    sub_0203946C();
    DWC_CleanupInet();
    ov70_02238E50(arg0, 0, 0);
    arg0->currentScreenInstruction = 0x13;
    return 3;
}

s32 ov70_02244854(GTSApplicationState *arg0) {
    sub_0203A914();
    ov70_02244FA4(arg0, arg0->disconnStatusMessageLoader, 0x1B, 1, 0xF0F);
    ov70_02238D84(arg0, 0x14, 0xB);
    return 3;
}

void ov70_02244888(GTSApplicationState *arg0) {
    sub_02039418(*(SaveData **)((u8 *)(arg0->playerData) + 0x20));
    Sys_SetSleepDisableFlag(4);
    ov70_02245124(arg0);
    DWC_InitInetEx(&arg0->dwcInetControlObject, 2U, 1U, 0x14U);
    DWC_SetAuthServer(DWC_CONNECTINET_AUTH_RELEASE);
    DWC_ConnectInetAsync();
    arg0->currentScreenInstruction = 3;
}
