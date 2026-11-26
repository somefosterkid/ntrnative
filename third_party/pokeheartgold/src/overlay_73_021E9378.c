#include "global.h"
struct WifiBattleTowerAppState;

#include "message_format.h"
#include "unk_0202D230.h"
#include "unk_02037C94.h"
#include "unk_02069528.h"
#include "bg_window.h"
#include "render_window.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
struct WifiBattleTowerDownloadData;

s32 ov73_021E83EC(void *, s32, s32);

s32 ov73_021E9F34(void *, MsgData *, s32, s32, s32);

s32 ov73_021E9378(WifiBattleTowerAppState *arg0);

s32 ov73_021E96EC(WifiBattleTowerAppState *arg0);

s32 ov73_021E9728(WifiBattleTowerAppState *arg0);
s32 ov73_021EA0F0(void *, String *, s32, s32);
s32 ov73_021EA134(void *, s32, s32);
s32 ov73_021E9758(WifiBattleTowerAppState *arg0);

s32 ov73_021E9378(WifiBattleTowerAppState *arg0) {
    BufferIntegerAsString(arg0->stringTemplate, 0U, arg0->selectedOpponentIdx, 3U, PRINTING_MODE_LEADING_ZEROS, 1);
    BufferIntegerAsString(arg0->stringTemplate, 1U, arg0->selectedRank, 2U, PRINTING_MODE_LEADING_ZEROS, 1);
    ov73_021E9F34(arg0, arg0->msgLoader1, 1, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x2F, 0x11);
    MIi_CpuClearFast(0U, &arg0->downloadBuffer, 0xA38U);
    return 3;
}

s32 ov73_021E93EC(WifiBattleTowerAppState *arg0);

s32 ov73_021E93EC(WifiBattleTowerAppState *arg0) {
    ov73_021E9F34(arg0, arg0->msgLoader1, 2, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x2F, 0xD);
    return 3;
}

s32 ov72_02237BD0(s32, s32, void *);

s32 ov73_021EA15C(void *);

s32 ov73_021E941C(WifiBattleTowerAppState *arg0);

s32 ov73_021E941C(WifiBattleTowerAppState *arg0) {
    ov72_02237BD0(arg0->playerRank, arg0->selectedOpponentIdx, &arg0->downloadBuffer);
    ov73_021EA15C(arg0);
    arg0->state = NINTENDO_WFC_STATE_WAIT_TRAINERS_DOWNLOAD;
    arg0->timeoutCounter = 0;
    return 3;
}

s32 ov72_02237B54(void);

s32 ov72_02237B74(void);

s32 ov73_021EA180(void *);

s32 sub_0202D678(void *, u8, u8, RTCDate *);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 sub_0202D7C0(struct WifiBattleTowerDownloadData *, void *, s8, s8);
#else
s32 sub_0202D7C0(struct WifiBattleTowerDownloadData *, void *, u8, u8);
#endif


s32 ov73_021E944C(WifiBattleTowerAppState *arg0);

s32 ov73_021E944C(WifiBattleTowerAppState *arg0) {
    RTCTime v0;
    RTCDate date;
    s32 v1;
    u32 v2;

    if (ov72_02237B54() != 0) {
        v1 = ov72_02237B74();
        arg0->timeoutCounter = 0;
        v2 = v1 + 7;
        switch (v2) {
        case 7:
            sub_0202D7C0(arg0->args->downloadData, &arg0->downloadBuffer, (u8) arg0->playerRank, (u8) arg0->selectedOpponentIdx);
            ov73_021E83EC(arg0, 0x27, 0x13);
            DWC_GetDateTime(&date, &v0);
            sub_0202D678(arg0->args->downloadData, (u8) arg0->selectedRank, (u8) arg0->selectedOpponentIdx, &date);
            break;
        case 6:
            arg0->asyncResult = v1;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            break;
        case 5:
            arg0->asyncResult = v1;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            break;
        case 3:
        case 4:
            arg0->asyncResult = v1;
            arg0->state = NINTENDO_WFC_STATE_REQUEST_ROOM_COUNT;
            ov73_021EA180(arg0);
            break;
        case 1:
            arg0->asyncResult = v1;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            break;
        case 0:
            arg0->asyncResult = v1;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            break;
        case 2:
            arg0->asyncResult = v1;
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

s32 ov73_021E955C(WifiBattleTowerAppState *arg0);

s32 ov73_021E955C(WifiBattleTowerAppState *arg0) {
    ov73_021E9FF8(arg0, arg0->msgLoader1, 3, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x2F, 0x2D);
    ov73_021EA180(arg0);
    arg0->args->unk20 = 0;
    return 3;
}

s32 ov73_021E9598(WifiBattleTowerAppState *arg0);

s32 ov73_021E9598(WifiBattleTowerAppState *arg0) {
    ov73_021E9FF8(arg0, arg0->msgLoader1, 7, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x2F, 0x1A);
    ov73_021EA15C(arg0);
    return 3;
}

s32 ov72_02237C30(s32, s32, s32, void *);


s32 sub_0202D7F0(struct WifiBattleTowerDownloadData *, s32 *);

s32 ov73_021E95CC(WifiBattleTowerAppState *arg0);

s32 ov73_021E95CC(WifiBattleTowerAppState *arg0) {
    s32 v0;
    s32 v1;

    v1 = sub_0202D568(*(void **)(arg0->args));
    sub_0202D7F0(arg0->args->downloadData, &v0);
    sub_02069528(arg0->args->saveData, 1, (void *)&arg0->wifiPlayerProfile);
    ov72_02237C30(*(u8 *)(&v0), *(u8 *)((u8 *)(&v0) + 1), v1, &arg0->wifiPlayerProfile);
    arg0->state = NINTENDO_WFC_STATE_WAIT_UPLOAD;
    arg0->timeoutCounter = 0;
    return 3;
}

s32 ov73_021E9618(WifiBattleTowerAppState *arg0);

s32 ov73_021E9618(WifiBattleTowerAppState *arg0) {
    s32 v0;
    u32 v1;

    if (ov72_02237B54() != 0) {
        v0 = ov72_02237B74();
        arg0->timeoutCounter = 0;
        v1 = v0 + 7;
        switch (v1) {
        case 7:
            sub_0202D5DC(**(FrontierData ***)arg0, 5U, 2U);
            ov73_021E83EC(arg0, 0x27, 0x1C);
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
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            break;
        case 1:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov73_021EA180(arg0);
            break;
        case 0:
        case 3:
            arg0->asyncResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
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

s32 ov73_021E96EC(WifiBattleTowerAppState *arg0) {
    ov73_021E9FF8(arg0, arg0->msgLoader1, 8, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x2F, 0x2D);
    ov73_021EA180(arg0);
    arg0->args->unk20 = 0;
    return 3;
}

s32 ov73_021E9728(WifiBattleTowerAppState *arg0) {
    ov73_021E9FF8(arg0, arg0->msgLoader1, 5, 1, 0xF0F);
    ov73_021E83EC(arg0, 0x2F, 0x1E);
    return 3;
}

s32 ov73_021E9758(WifiBattleTowerAppState *arg0) {
    arg0->selectedRank = 1;
    DrawFrameAndWindow1(&arg0->rankWindow, 0, 0x1FU, 0xEU);
    ov73_021EA0F0(&arg0->rankWindow, arg0->rankString, arg0->selectedRank, 2);
    ov73_021EA134(arg0, 1, 1);
    CopyWindowToVram(&arg0->rankWindow);
    arg0->state = NINTENDO_WFC_STATE_HANDLE_LEADERS_RANK_INPUT;
    return 3;
}
