#include "global.h"
struct GTSApplicationState;

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_02239D44(void *, u32);

extern u32 ov70_022453B8;

void ov70_0223A0D4(GTSApplicationState *param_1);

void ov70_0223A1A0(GTSApplicationState *param_1);

void ov70_0223A1E4(GTSApplicationState *arg0);
void ov70_0223A224(struct GTSApplicationState *arg0);
s32 ov70_0223A258(void *arg0);

void ov70_0223A0D4(GTSApplicationState *param_1) {
    s32 *piVar1;
    s32 *piVar2;
    u32 *puVar3;
    Window *window;
    s32 iVar4;
    s32 iStack_1c;

    AddWindowParameterized(param_1->bgConfig, &param_1->bottomInstructionWindow, 0, 2, 0x15, 0x1b, 2, 0xd, 0x28);
    FillWindowPixelBuffer(&param_1->bottomInstructionWindow, 0);
    AddWindowParameterized(param_1->bgConfig, param_1->menuButtonWindows, 0, 0x15, 0xf, 0xa, 4, 0xd, 0x5e);
    iStack_1c = 0;
    puVar3 = (u32 *)&ov70_022453B8;
    iVar4 = 0x86;
    window = &param_1->infoWindows[13];
    do {
        AddWindowParameterized(param_1->bgConfig, window, 3, (u8)*puVar3, (u8)puVar3[1], (u8)puVar3[2], (u8)puVar3[3], 0xd, (u16)iVar4);
        FillWindowPixelBuffer(window, 0);
        piVar1 = puVar3 + 2;
        piVar2 = puVar3 + 3;
        iVar4 = iVar4 + *piVar1 * *piVar2;
        puVar3 = puVar3 + 4;
        window = window + 1;
        iStack_1c = iStack_1c + 1;
    } while (iStack_1c < 0xe);
    ov70_02239D44(param_1, 2);
    return;
}

void ov70_0223A1A0(GTSApplicationState *param_1) {
    s32 iVar1;
    Window *window;

    RemoveWindow(&param_1->unk_1198);
    RemoveWindow(&param_1->bottomInstructionWindow);
    RemoveWindow(param_1->menuButtonWindows);
    iVar1 = 0;
    window = &param_1->infoWindows[13];
    do {
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 0xe);
    return;
}

void ov70_0223A1E4(GTSApplicationState *arg0) {
    s32 v0;
    void **v1;

    v1 = (void **)arg0;
    *(String **)((u8 *)v1 + offsetof(GTSApplicationState, genericMessageBuffer)) = String_New(0xB4U, HEAP_ID_61);
    *(String **)((u8 *)v1 + offsetof(GTSApplicationState, title)) = NewString_ReadMsgData(*(MsgData **)((u8 *)v1 + offsetof(GTSApplicationState, gtsMessageLoader)), 0x27);
    v0 = 0;
    do {
        arg0->trainerLocationStrings[v0] = String_New(0x14U, HEAP_ID_61);
        v0 += 1;
        v1 += 1;
    } while (v0 < 0xA);
}

void ov70_0223A224(GTSApplicationState *arg0) {
    s32 v0;

    v0 = 0;
    do {
        String_Delete(arg0->trainerLocationStrings[v0]);
        v0 += 1;
    } while (v0 < 0xA);
    String_Delete(arg0->genericMessageBuffer);
    String_Delete(arg0->title);
}

s32 ov70_0223A258(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x2C) = 1;
    return 3;
}
