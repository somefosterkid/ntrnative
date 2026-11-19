#include "global.h"

#include "bg_window.h"
#include "render_window.h"
#include "overlay_70_gtsapplication_state.h"
s32 ov70_02238D84(GTSApplicationState *, s32, s32);

s32 ov70_0223CAC4(GTSApplicationState *, s32, s32, s32, s32);

s32 ov70_0223C618(GTSApplicationState *arg0);

s32 ov70_0223C618(GTSApplicationState *arg0) {
    ov70_0223CAC4(arg0, 0xA, 1, 0, 0xF0F);
    ov70_02238D84(arg0, 0x15, 7);
    return 3;
}

s32 ov70_0223CD28(s32);

s32 ov70_022420C4(void *, s16);

s32 ov70_0223C640(GTSApplicationState *arg0);

s32 ov70_0223C640(GTSApplicationState *arg0) {
    ov70_022420C4(arg0->unk_11A8, 1);
    ov70_0223CD28(9);
    arg0->currentScreenInstruction = 8;
    return 3;
}

s32 ov70_0223F2BC(void *, void *, int, s32, s32, s32, s32);

u32 ov70_02242144(void *);

s32 ov70_0223C660(GTSApplicationState *arg0);

s32 ov70_0223C660(GTSApplicationState *arg0) {
    u32 v0;

    v0 = ov70_02242144(arg0->unk_11A8);
    switch (v0) {
    case -2:
        ov70_0223CD28(0);
        ClearFrameAndWindow2((void *)&arg0->bottomInstructionWindow, 0);
        arg0->currentScreenInstruction = 0;
        break;
    case 0:
    case 1:
    case 2:
        ov70_0223CD28(0);
        arg0->searchRequirements.gender = (s8)(v0 + 1);
        arg0->currentScreenInstruction = 0;
        FillWindowPixelBuffer((void *)&arg0->infoWindows[16], 0U);
        ov70_0223F2BC((void *)&arg0->infoWindows[16], arg0->gtsMessageLoader, arg0->searchRequirements.gender, 1, 0, 0, 0x10200);
        break;
    }
    return 3;
}

s32 ov70_0223C6F0(GTSApplicationState *arg0);

s32 ov70_0223C6F0(GTSApplicationState *arg0) {
    ov70_0223CAC4(arg0, 0xB, 1, 0, 0xF0F);
    ov70_02238D84(arg0, 0x15, 0xA);
    return 3;
}
