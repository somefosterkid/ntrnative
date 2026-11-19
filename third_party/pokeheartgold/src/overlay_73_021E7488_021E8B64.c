#include "global.h"
struct WifiBattleTowerAppState;

#include "msgdata.h"
#include "pm_string.h"
#include "unk_02037C94.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

void ov73_021E8B64(struct WifiBattleTowerAppState *arg0);

void ov73_021E8B64(WifiBattleTowerAppState *arg0) {
    arg0->msgString = String_New(0xB4U, HEAP_ID_150);
    arg0->opponentIdxString = String_New(4U, HEAP_ID_150);
    arg0->rankString = String_New(3U, HEAP_ID_150);
    arg0->titleString = NewString_ReadMsgData(arg0->msgLoader1, 0xA);
    arg0->errorString = String_New(0x100U, HEAP_ID_150);
}

void ov73_021E8BBC(struct WifiBattleTowerAppState *arg0);

void ov73_021E8BBC(WifiBattleTowerAppState *arg0) {
    String_Delete(arg0->errorString);
    String_Delete(arg0->titleString);
    String_Delete(arg0->rankString);
    String_Delete(arg0->opponentIdxString);
    String_Delete(arg0->msgString);
}

s32 ov73_021E83EC(void *, s32, s32);

s32 ov73_021E9FF8(void *, MsgData *, s32, s32, s32);

s32 ov73_021E8BFC(WifiBattleTowerAppState *arg0);

s32 ov73_021E8BFC(WifiBattleTowerAppState *arg0) {
    ov73_021E9FF8(arg0, arg0->msgLoader3, 0x11, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x30, 1);
    return 3;
}

s32 ov73_021E83D4(YesNoPrompt *);

s32 ov73_021E844C(void *, s32, s32);

s32 ov73_021EA15C(void *);

s32 ov73_021E8C2C(WifiBattleTowerAppState *arg0);

s32 ov73_021E8C2C(WifiBattleTowerAppState *arg0) {
    s32 v0;

    v0 = ov73_021E83D4(arg0->yesNoPrompt);
    if (v0 != 0) {
        if (v0 == 2) {
            sub_0203946C();
            ov73_021E844C(arg0, 0, 0);
            arg0->state = NINTENDO_WFC_STATE_START_EXIT;
        } else {
            ov73_021E9FF8(arg0, arg0->msgLoader2, 1, 1, 0xF0F);
            ov73_021E83EC(arg0, 0x2F, 2);
            ov73_021EA15C(arg0);
        }
    }
    return 3;
}

s32 ov73_021E8C8C(WifiBattleTowerAppState *arg0);

s32 ov73_021E8C8C(WifiBattleTowerAppState *arg0) {
    ov73_021E9FF8(arg0, arg0->msgLoader2, 0xC, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x30, 0x34);
    return 3;
}
