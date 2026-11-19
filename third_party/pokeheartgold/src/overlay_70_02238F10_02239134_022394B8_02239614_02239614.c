#include "global.h"
struct GTSApplicationState;

#include "gf_gfx_planes.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_02239614(struct GTSApplicationState *arg0);

void ov70_02239614(GTSApplicationState *arg0) {
    arg0->genericMessageBuffer = String_New(0xB4U, HEAP_ID_61);
    arg0->title = NewString_ReadMsgData(arg0->gtsMessageLoader, 0x27);
}

void ov70_0223963C(struct GTSApplicationState *arg0);

void ov70_0223963C(GTSApplicationState *arg0) {
    String_Delete(arg0->genericMessageBuffer);
    String_Delete(arg0->title);
}

s32 ov70_02238E50(void *, s32, s32);

s32 ov70_02239658(GTSApplicationState *arg0);

s32 ov70_02239658(GTSApplicationState *arg0) {
    if (arg0->hasAvatarFinishedMoving != 0) {
        GfGfx_EngineATogglePlanes(1U, 0U);
        GfGfx_EngineATogglePlanes(2U, 0U);
        GfGfx_EngineATogglePlanes(4U, 0U);
        ov70_02238E50(arg0, 7, 0xB);
        arg0->returnAfterNetworkScreen = 1;
        arg0->currentScreenInstruction = 9;
    }
    return 3;
}

s32 ov70_02238D84(GTSApplicationState *, s32, s32);

s32 ov70_02239698(void *arg0);

s32 ov70_02239698(void *arg0) {
    ov70_02238D84(arg0, 0xA, 2);
    return 3;
}
