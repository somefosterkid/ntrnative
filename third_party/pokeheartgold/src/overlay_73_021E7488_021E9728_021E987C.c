#include "global.h"

#include "bg_window.h"
#include "render_window.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

s32 ov73_021E83EC(void *, s32, s32);

s32 ov73_021E9FF8(void *, void *, s32, s32, s32);

s32 ov73_021E987C(WifiBattleTowerAppState *arg0);

s32 ov73_021E987C(WifiBattleTowerAppState *arg0) {
    ov73_021E9FF8(arg0, arg0->msgLoader1, 6, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x2F, 0x21);
    return 3;
}

s32 ov73_021EA0F0(void *, void *, s32, s32);

s32 ov73_021EA134(void *, s32, s32);

s32 ov73_021E98AC(WifiBattleTowerAppState *arg0);

s32 ov73_021E98AC(WifiBattleTowerAppState *arg0) {
    arg0->selectedOpponentIdx = 1;
    DrawFrameAndWindow1(&arg0->opponentWindow, 0, 0x1FU, 0xEU);
    ov73_021EA0F0(&arg0->opponentWindow, arg0->opponentIdxString, arg0->selectedOpponentIdx, 3);
    ov73_021EA134(arg0, 0, 1);
    CopyWindowToVram(&arg0->opponentWindow);
    arg0->state = NINTENDO_WFC_STATE_HANDLE_LEADERS_ROOM_INPUT;
    return 3;
}
