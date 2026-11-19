#include "global.h"
#include "render_window.h"
#include "overlay_70_gtsapplication_state.h"
void ov70_022420C4(void *, s16);

u32 ov70_0223EF10(GTSApplicationState *param_1);

s32 ov70_0223F3D8(void *, void *, void *, void *, s32, s32, s32);
u32 ov70_02242144(void *);
s32 ov70_0223EF38(GTSApplicationState *arg0);
s32 ov70_02238D84(GTSApplicationState *, s32, s32);
s32 ov70_0223E8E8(GTSApplicationState *, s32, s32, s32, s32);
s32 ov70_0223EFB8(GTSApplicationState *arg0);

u32 ov70_0223EF10(GTSApplicationState *param_1) {
    param_1->listMenuCursorIndex = 0xffff;
    ov70_022420C4(param_1->unk_11A8, 1);
    param_1->currentScreenInstruction = 9;
    return 3;
}

s32 ov70_0223EF38(GTSApplicationState *arg0) {
    u32 v0;

    v0 = ov70_02242144(arg0->unk_11A8);
    switch (v0) {
    case -2:
        ClearFrameAndWindow2((void *)&arg0->bottomInstructionWindow, 0);
        arg0->currentScreenInstruction = 0;
        break;
    case 0:
    case 1:
    case 2:
        arg0->depositRequirements.gender = (s8)(v0 + 1);
        arg0->currentScreenInstruction = 0xA;
        ov70_0223F3D8(arg0->gtsMessageLoader, arg0->speciesMessageLoader, arg0->stringTemplate, (void *)&arg0->infoWindows[13], (s32) arg0->depositRequirements.species, (s32) arg0->depositRequirements.gender, -1);
        break;
    }
    return 3;
}

s32 ov70_0223EFB8(GTSApplicationState *arg0) {
    ov70_0223E8E8(arg0, 0xB, 1, 0, 0xF0F);
    ov70_02238D84(arg0, 0x10, 0xB);
    return 3;
}
