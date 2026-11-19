#include "global.h"
#include "overlay_70_gtsapplication_state.h"

#include "bg_window.h"
void ov70_0223CD28(u32);

void ov70_0223F370(void *, void *, s32, u32, u32, u32, u32);

void ov70_0223F828(u8 *, s32, u32);

s32 ov70_02242144(void *);

u32 ov70_0223C744(GTSApplicationState *param_1);

u32 ov70_0223C744(GTSApplicationState *param_1) {
    u32 iVar1;
    iVar1 = ov70_02242144(param_1->unk_11A8);
    switch (iVar1) {
    case 0xb:
    case -2:
        ov70_0223CD28(0);
        param_1->currentScreenInstruction = 0;
        break;
    case -1:
        break;
    default:
        ov70_0223CD28(0);
        ov70_0223F828((u8 *)&param_1->searchRequirements, iVar1, 1);
        param_1->currentScreenInstruction = 0;
        FillWindowPixelBuffer((Window *)((u8 *)&param_1->infoWindows[18]), 0);
        ov70_0223F370((u8 *)&param_1->infoWindows[18], param_1->gtsMessageLoader, iVar1, 0, 0, 0x10200, 1);
        break;
    }
    return 3;
}
