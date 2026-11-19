#include "global.h"

#include "list_menu_items.h"
#include "overlay_70_gtsapplication_state.h"
struct GTSApplicationState;

s32 ov70_0223D924(s32 arg0, s32 arg1, s32 arg2);

s32 ov70_0223D924(s32 arg0, s32 arg1, s32 arg2) {
    s32 v0;

    v0 = arg0 + arg2;
    if (v0 < 0) {
        return arg1 - 1;
    }
    if (v0 == arg1) {
        v0 = 0;
    }
    return v0;
}

void *ov70_02238CAC(struct GTSApplicationState *, u8, u8);

s32 ov70_0223D934(GTSApplicationState *arg0);

s32 ov70_0223D934(GTSApplicationState *arg0) {
    arg0->unk_11AC = ListMenuItems_New(3U, HEAP_ID_61);
    ListMenuItems_AppendFromMsgData(arg0->unk_11AC, arg0->gtsMessageLoader, 0x62, 1);
    ListMenuItems_AppendFromMsgData(arg0->unk_11AC, arg0->gtsMessageLoader, 0x63, 2);
    ListMenuItems_AppendFromMsgData(arg0->unk_11AC, arg0->gtsMessageLoader, 0x64, 3);
    arg0->unk_11D0 = ov70_02238CAC(arg0, 3, 0xA);
    arg0->currentScreenInstruction = 8;
    return 3;
}
