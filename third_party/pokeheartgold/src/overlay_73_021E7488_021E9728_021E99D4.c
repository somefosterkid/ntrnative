#include "global.h"
struct WifiBattleTowerAppState;

#include "message_format.h"
#include "save.h"
#include "unk_02037C94.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

s32 ov73_021E83EC(void *, s32, s32);

s32 ov73_021E9F34(void *, MsgData *, s32, s32, s32);

s32 ov73_021E99D4(WifiBattleTowerAppState *arg0);

s32 ov73_021E99D4(WifiBattleTowerAppState *arg0) {
    BufferIntegerAsString(arg0->stringTemplate, 0U, arg0->selectedOpponentIdx, 3U, PRINTING_MODE_LEADING_ZEROS, 1);
    BufferIntegerAsString(arg0->stringTemplate, 1U, arg0->selectedRank, 2U, PRINTING_MODE_LEADING_ZEROS, 1);
    ov73_021E9F34(arg0, arg0->msgLoader1, 1, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x2F, 0x24);
    arg0->state = NINTENDO_WFC_STATE_REQUEST_LEADERS_DOWNLOAD;
    return 3;
}

s32 ov72_02237BD0(s32, s32, void *);

s32 ov73_021EA15C(void *);

s32 ov73_021E9A3C(WifiBattleTowerAppState *arg0);

s32 ov73_021E9A3C(WifiBattleTowerAppState *arg0) {
    ov72_02237BD0(arg0->selectedRank, arg0->selectedOpponentIdx, &arg0->downloadBuffer);
    arg0->state = NINTENDO_WFC_STATE_WAIT_LEADERS_DOWNLOAD;
    arg0->timeoutCounter = 0;
    ov73_021EA15C(arg0);
    return 3;
}

s32 ov72_02237B54(void);

s32 ov72_02237B74(void);

s32 ov73_021EA180(void *);

s32 sub_0202D8A4(void *, void *, u8, u8);

s32 ov73_021E9A6C(WifiBattleTowerAppState *arg0);

s32 ov73_021E9A6C(WifiBattleTowerAppState *arg0) {
    s32 v0;
    u32 v1;

    if (ov72_02237B54() != 0) {
        v0 = ov72_02237B74();
        arg0->timeoutCounter = 0;
        v1 = v0 + 7;
        switch (v1) {
        case 7:
            sub_0202D8A4(arg0->args->downloadData, arg0->downloadBuffer.matchCandidates, (u8) arg0->selectedRank, (u8) arg0->selectedOpponentIdx);
            ov73_021E83EC(arg0, 0x27, 0x26);
            break;
        case 6:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            break;
        case 5:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            break;
        case 4:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_REQUEST_ROOM_COUNT;
            ov73_021EA180(arg0);
            break;
        case 1:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            break;
        case 0:
        case 3:
            arg0->state = NINTENDO_WFC_STATE_REQUEST_ROOM_COUNT;
            arg0->asyncResult = v0;
            ov73_021EA180(arg0);
            break;
        case 2:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            sub_020399EC();
            break;
        }
    } else {
        arg0->timeoutCounter = (s32)(arg0->timeoutCounter + 1);
        if (arg0->timeoutCounter == 0xE10) {
            sub_020399EC();
        }
    }
    return 3;
}

s32 ov73_021E9FF8(void *, MsgData *, s32, s32, s32);

s32 ov73_021E9B58(WifiBattleTowerAppState *arg0);

s32 ov73_021E9B58(WifiBattleTowerAppState *arg0) {
    ov73_021E9FF8(arg0, arg0->msgLoader1, 3, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x2F, 0x2D);
    ov73_021EA180(arg0);
    arg0->args->unk20 = 0;
    return 3;
}

s32 ov73_021E9B94(WifiBattleTowerAppState *arg0);

s32 ov73_021E9B94(WifiBattleTowerAppState *arg0) {
    arg0->state = NINTENDO_WFC_STATE_PRINT_DISCONNECTING;
    return 3;
}

s32 ov73_021E9B9C(WifiBattleTowerAppState *arg0);

s32 ov73_021E9B9C(WifiBattleTowerAppState *arg0) {
    Save_PrepareForAsyncWrite(arg0->args->saveData, 2);
    arg0->state = NINTENDO_WFC_STATE_SAVE;
    return 3;
}

s32 ov73_021E9BB4(WifiBattleTowerAppState *arg0);

s32 ov73_021E9BB4(WifiBattleTowerAppState *arg0) {
    if (Save_WriteFileAsync(arg0->args->saveData) == 2) {
        arg0->state = arg0->nextState;
        ov73_021EA180(arg0);
    }
    return 3;
}

s32 ov73_021E9BD4(WifiBattleTowerAppState *arg0);

s32 ov73_021E9BD4(WifiBattleTowerAppState *arg0) {
    ov73_021E9FF8(arg0, arg0->msgLoader1, 4, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x30, 0x16);
    return 3;
}

s32 ov73_021E83D4(YesNoPrompt *);

s32 ov73_021E9C04(WifiBattleTowerAppState *arg0);

s32 ov73_021E9C04(WifiBattleTowerAppState *arg0) {
    s32 v0;

    v0 = ov73_021E83D4(arg0->yesNoPrompt);
    if (v0 != 0) {
        if (v0 == 2) {
            arg0->state = NINTENDO_WFC_STATE_ASK_CHALLENGE_ROOM;
        } else {
            arg0->state = NINTENDO_WFC_STATE_PRINT_DISCONNECTING;
            arg0->args->unk20 = 1;
        }
    }
    return 3;
}

s32 ov73_021E9C30(WifiBattleTowerAppState *arg0);

s32 ov73_021E9C30(WifiBattleTowerAppState *arg0) {
    ov73_021E9FF8(arg0, arg0->msgLoader1, 9, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x30, 0x18);
    return 3;
}

s32 ov73_021E9C60(WifiBattleTowerAppState *arg0);

s32 ov73_021E9C60(WifiBattleTowerAppState *arg0) {
    s32 v0;

    v0 = ov73_021E83D4(arg0->yesNoPrompt);
    if (v0 != 0) {
        if (v0 == 2) {
            arg0->state = NINTENDO_WFC_STATE_ASK_LEADERS_RANK;
        } else {
            arg0->state = NINTENDO_WFC_STATE_PRINT_DISCONNECTING;
            arg0->args->unk20 = 1;
        }
    }
    return 3;
}

s32 ov73_021E9C8C(WifiBattleTowerAppState *arg0);

s32 ov73_021E9C8C(WifiBattleTowerAppState *arg0) {
    ov73_021E9FF8(arg0, arg0->msgLoader3, 0x1A, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x2F, 0x2A);
    return 3;
}
