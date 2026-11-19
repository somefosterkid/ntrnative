#include "global.h"
struct WifiBattleTowerAppState;

#include "bg_window.h"
#include "render_window.h"
#include "unk_02037C94.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

s32 ov72_02237B80(s32);

s32 ov73_021EA15C(void *);

s32 ov73_021E9120(WifiBattleTowerAppState *arg0);

s32 ov73_021E9120(WifiBattleTowerAppState *arg0) {
    ov72_02237B80(arg0->selectedRank);
    arg0->state = NINTENDO_WFC_STATE_WAIT_ROOM_COUNT;
    arg0->timeoutCounter = 0;
    ov73_021EA15C(arg0);
    return 3;
}

s32 ov72_02237B54(void);

s32 ov72_02237B74(void);

s32 ov73_021EA180(void *);

s32 ov73_021E9144(WifiBattleTowerAppState *arg0);

s32 ov73_021E9144(WifiBattleTowerAppState *arg0) {
    s32 v0;
    u32 v1;

    if (ov72_02237B54() != 0) {
        v0 = ov72_02237B74();
        arg0->timeoutCounter = 0;
        v1 = v0 + 7;
        switch (v1) {
        case 6:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            break;
        case 5:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            break;
        case 4:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            break;
        case 1:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            break;
        case 0:
        case 3:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            break;
        case 2:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            sub_020399EC();
            break;
        default:
            if (v0 > 0) {
                arg0->matchCount = v0;
                arg0->state = arg0->nextState;
            }
            break;
        }
        ov73_021EA180(arg0);
    } else {
        arg0->timeoutCounter = (s32)(arg0->timeoutCounter + 1);
        if (arg0->timeoutCounter == 0xE10) {
            sub_020399EC();
        }
    }
    return 3;
}

s32 ov73_021E83EC(void *, s32, s32);

s32 ov73_021E9FF8(void *, MsgData *, s32, s32, s32);

s32 ov73_021E91F4(WifiBattleTowerAppState *arg0);

s32 ov73_021E91F4(WifiBattleTowerAppState *arg0) {
    ov73_021E9FF8(arg0, arg0->msgLoader1, 0, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x2F, 0xE);
    return 3;
}

s32 ov73_021EA0F0(void *, String *, s32, s32);

s32 ov73_021EA134(void *, s32, s32);

s32 ov73_021E9224(WifiBattleTowerAppState *arg0);

s32 ov73_021E9224(WifiBattleTowerAppState *arg0) {
    DrawFrameAndWindow1(&arg0->opponentWindow, 0, 0x1FU, 0xEU);
    ov73_021EA0F0(&arg0->opponentWindow, arg0->opponentIdxString, arg0->selectedOpponentIdx, 3);
    ov73_021EA134(arg0, 0, 1);
    CopyWindowToVram(&arg0->opponentWindow);
    arg0->state = NINTENDO_WFC_STATE_HANDLE_CHALLENGE_ROOM_INPUT;
    return 3;
}
