#include "global.h"

#include "message_format.h"
#include "save.h"
#include "unk_0202D230.h"
#include "unk_02037C94.h"
#include "unk_02069528.h"
#include "overlay_72_wfc_connect.h"
struct WifiBattleTowerDownloadData;

s32 ov72_02238680(void *, s32, s32);

s32 ov72_0223A280(void *, MsgData *, s32, s32, s32);

s32 ov72_022397E8(NintendoWfcConnect *arg0);

s32 ov72_022397E8(NintendoWfcConnect *arg0) {
    BufferIntegerAsString(arg0->msgFormat, 0U, arg0->room, 3U, PRINTING_MODE_LEADING_ZEROS, 1);
    BufferIntegerAsString(arg0->msgFormat, 1U, arg0->rank, 2U, PRINTING_MODE_LEADING_ZEROS, 1);
    ov72_0223A280(arg0, arg0->msgDataTower, 1, 1, 0xF0F);
    ov72_02238680(arg0, 0x2F, 0x11);
    MIi_CpuClearFast(0U, &arg0->downloadBuffer, 0xA38U);
    return 3;
}

s32 ov72_0223985C(NintendoWfcConnect *arg0);

s32 ov72_0223985C(NintendoWfcConnect *arg0) {
    ov72_0223A280(arg0, arg0->msgDataTower, 2, 1, 0xF0F);
    ov72_02238680(arg0, 0x2F, 0xD);
    return 3;
}

s32 ov72_02237BD0(s32, s32, void *);

s32 ov72_0223A420(NintendoWfcConnect *);

s32 ov72_0223988C(NintendoWfcConnect *arg0);

s32 ov72_0223988C(NintendoWfcConnect *arg0) {
    ov72_02237BD0(arg0->ownRank, arg0->room, &arg0->downloadBuffer);
    ov72_0223A420(arg0);
    arg0->state = NINTENDO_WFC_STATE_WAIT_TRAINERS_DOWNLOAD;
    arg0->timeoutCounter = 0;
    return 3;
}

s32 ov72_02237B54(void);

s32 ov72_02237B74(void);

s32 ov72_0223A444(void *);

s32 sub_0202D678(void *, u8, u8, RTCDate *);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 sub_0202D7C0(struct WifiBattleTowerDownloadData *, void *, s8, s8);
#else
s32 sub_0202D7C0(struct WifiBattleTowerDownloadData *, void *, u8, u8);
#endif


s32 ov72_022398BC(NintendoWfcConnect *arg0);

s32 ov72_022398BC(NintendoWfcConnect *arg0) {
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
            sub_0202D7C0(arg0->args->downloadData, &arg0->downloadBuffer, (u8) arg0->ownRank, (u8) arg0->room);
            ov72_02238680(arg0, 0x27, 0x13);
            DWC_GetDateTime(&date, &v0);
            sub_0202D678(arg0->args->downloadData, (u8) arg0->rank, (u8) arg0->room, &date);
            break;
        case 6:
            arg0->httpResult = v1;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            break;
        case 5:
            arg0->httpResult = v1;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            break;
        case 3:
        case 4:
            arg0->httpResult = v1;
            arg0->state = NINTENDO_WFC_STATE_REQUEST_ROOM_COUNT;
            ov72_0223A444(arg0);
            break;
        case 1:
            arg0->httpResult = v1;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            break;
        case 0:
            arg0->httpResult = v1;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            break;
        case 2:
            arg0->httpResult = v1;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
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

s32 ov72_0223A350(void *, MsgData *, s32, s32, s32);

s32 ov72_022399CC(NintendoWfcConnect *arg0);

s32 ov72_022399CC(NintendoWfcConnect *arg0) {
    ov72_0223A350(arg0, arg0->msgDataTower, 3, 1, 0xF0F);
    ov72_02238680(arg0, 0x2F, 0x2D);
    ov72_0223A444(arg0);
    arg0->args->unk20 = 0;
    return 3;
}

s32 ov72_02239A08(NintendoWfcConnect *arg0);

s32 ov72_02239A08(NintendoWfcConnect *arg0) {
    ov72_0223A350(arg0, arg0->msgDataTower, 7, 1, 0xF0F);
    ov72_02238680(arg0, 0x2F, 0x1A);
    ov72_0223A420(arg0);
    return 3;
}

s32 ov72_02237C30(s32, s32, s32, void *);


s32 sub_0202D7F0(struct WifiBattleTowerDownloadData *, s32 *);

s32 ov72_02239A3C(NintendoWfcConnect *arg0);

s32 ov72_02239A3C(NintendoWfcConnect *arg0) {
    s32 v0;
    s32 v1;

    v1 = sub_0202D568(arg0->args->frontierData);
    sub_0202D7F0(arg0->args->downloadData, &v0);
    sub_02069528(arg0->args->saveData, 1, (void *)&arg0->uploadRecord);
    ov72_02237C30(*(u8 *)(&v0), *(u8 *)((u8 *)(&v0) + 1), v1, &arg0->uploadRecord);
    arg0->state = NINTENDO_WFC_STATE_WAIT_UPLOAD;
    arg0->timeoutCounter = 0;
    return 3;
}

s32 ov72_02239A88(NintendoWfcConnect *arg0);

s32 ov72_02239A88(NintendoWfcConnect *arg0) {
    s32 v0;
    u32 v1;

    if (ov72_02237B54() != 0) {
        v0 = ov72_02237B74();
        arg0->timeoutCounter = 0;
        v1 = v0 + 7;
        switch (v1) {
        case 7:
            sub_0202D5DC(**(FrontierData ***)arg0, 5U, 2U);
            ov72_02238680(arg0, 0x27, 0x1C);
            break;
        case 6:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            break;
        case 5:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            break;
        case 4:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            break;
        case 1:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            break;
        case 0:
        case 3:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            break;
        case 2:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
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

s32 ov72_02239B5C(NintendoWfcConnect *arg0);

s32 ov72_02239B5C(NintendoWfcConnect *arg0) {
    ov72_0223A350(arg0, arg0->msgDataTower, 8, 1, 0xF0F);
    ov72_02238680(arg0, 0x2F, 0x2D);
    ov72_0223A444(arg0);
    arg0->args->unk20 = 0;
    return 3;
}

s32 ov72_02239B98(NintendoWfcConnect *arg0);

s32 ov72_02239B98(NintendoWfcConnect *arg0) {
    ov72_0223A350(arg0, arg0->msgDataTower, 5, 1, 0xF0F);
    ov72_02238680(arg0, 0x2F, 0x1E);
    return 3;
}

s32 ov72_0223A41C(void *, s32, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov72_0223A520(NintendoWfcConnect *, s8, s8);
#else
s32 ov72_0223A520(NintendoWfcConnect *, s32, s32);
#endif

s32 ov72_02239BC8(NintendoWfcConnect *arg0);

s32 ov72_02239BC8(NintendoWfcConnect *arg0) {
    arg0->rank = 1;
    ov72_0223A41C(arg0, 1, 1);
    ov72_0223A520(arg0, 1, 0xA);
    arg0->state = NINTENDO_WFC_STATE_HANDLE_LEADERS_RANK_INPUT;
    return 3;
}

s32 ov72_0223A588(NintendoWfcConnect *wfc);

s32 ov72_02239BEC(NintendoWfcConnect *arg0);

s32 ov72_02239BEC(NintendoWfcConnect *arg0) {
    s32 v0;

    v0 = ov72_0223A588(arg0);
    if (v0 != 0) {
        if (v0 > 0) {
            arg0->rank = v0;
            ov72_02238680(arg0, 0xB, 0x20);
        } else {
            arg0->state = NINTENDO_WFC_STATE_ASK_CANCEL_DOWNLOAD;
        }
    }
    return 3;
}

s32 ov72_02239C14(NintendoWfcConnect *arg0);

s32 ov72_02239C14(NintendoWfcConnect *arg0) {
    s32 num;

    num = arg0->roomCount;
    if (num >= 0x64) {
        BufferIntegerAsString(arg0->msgFormat, 0U, num, 3U, PRINTING_MODE_LEADING_ZEROS, 1);
        ov72_0223A280(arg0, arg0->msgDataTower, 0xA, 1, 0xF0F);
    } else {
        BufferIntegerAsString(arg0->msgFormat, 0U, num, 2U, PRINTING_MODE_LEADING_ZEROS, 1);
        ov72_0223A280(arg0, arg0->msgDataTower, 6, 1, 0xF0F);
    }
    ov72_02238680(arg0, 0x2F, 0x21);
    return 3;
}

s32 ov72_02239C8C(NintendoWfcConnect *arg0);

s32 ov72_02239C8C(NintendoWfcConnect *arg0) {
    arg0->room = 1;
    ov72_0223A520(arg0, 1, arg0->roomCount);
    ov72_0223A41C(arg0, 0, 1);
    arg0->state = NINTENDO_WFC_STATE_HANDLE_LEADERS_ROOM_INPUT;
    return 3;
}

s32 ov72_02239CB4(NintendoWfcConnect *arg0);

s32 ov72_02239CB4(NintendoWfcConnect *arg0) {
    s32 v0;

    v0 = ov72_0223A588(arg0);
    if (v0 != 0) {
        if (v0 > 0) {
            arg0->room = v0;
            arg0->state = NINTENDO_WFC_STATE_PRINT_RECEIVING_LEADERS;
        } else {
            arg0->state = NINTENDO_WFC_STATE_ASK_LEADERS_RANK;
        }
    }
    return 3;
}

s32 ov72_02239CD8(NintendoWfcConnect *arg0);

s32 ov72_02239CD8(NintendoWfcConnect *arg0) {
    BufferIntegerAsString(arg0->msgFormat, 0U, arg0->room, 3U, PRINTING_MODE_LEADING_ZEROS, 1);
    BufferIntegerAsString(arg0->msgFormat, 1U, arg0->rank, 2U, PRINTING_MODE_LEADING_ZEROS, 1);
    ov72_0223A280(arg0, arg0->msgDataTower, 1, 1, 0xF0F);
    ov72_02238680(arg0, 0x2F, 0x24);
    return 3;
}

s32 ov72_02239D3C(NintendoWfcConnect *arg0);

s32 ov72_02239D3C(NintendoWfcConnect *arg0) {
    ov72_02237BD0(arg0->rank, arg0->room, &arg0->downloadBuffer);
    arg0->state = NINTENDO_WFC_STATE_WAIT_LEADERS_DOWNLOAD;
    arg0->timeoutCounter = 0;
    ov72_0223A420(arg0);
    return 3;
}

s32 sub_0202D8A4(void *, void *, u8, u8);

s32 ov72_02239D6C(NintendoWfcConnect *arg0);

s32 ov72_02239D6C(NintendoWfcConnect *arg0) {
    s32 v0;
    u32 v1;

    if (ov72_02237B54() != 0) {
        v0 = ov72_02237B74();
        arg0->timeoutCounter = 0;
        v1 = v0 + 7;
        switch (v1) {
        case 7:
            sub_0202D8A4(arg0->args->downloadData, arg0->downloadBuffer.leaders, (u8) arg0->rank, (u8) arg0->room);
            ov72_02238680(arg0, 0x27, 0x26);
            break;
        case 6:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            break;
        case 5:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            break;
        case 4:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_REQUEST_ROOM_COUNT;
            ov72_0223A444(arg0);
            break;
        case 1:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            break;
        case 0:
        case 3:
            arg0->state = NINTENDO_WFC_STATE_REQUEST_ROOM_COUNT;
            arg0->httpResult = v0;
            ov72_0223A444(arg0);
            break;
        case 2:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
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

s32 ov72_02239E58(NintendoWfcConnect *arg0);

s32 ov72_02239E58(NintendoWfcConnect *arg0) {
    ov72_0223A350(arg0, arg0->msgDataTower, 3, 1, 0xF0F);
    ov72_02238680(arg0, 0x2F, 0x2D);
    ov72_0223A444(arg0);
    arg0->args->unk20 = 0;
    return 3;
}

s32 ov72_02239E94(NintendoWfcConnect *arg0);

s32 ov72_02239E94(NintendoWfcConnect *arg0) {
    arg0->state = NINTENDO_WFC_STATE_PRINT_DISCONNECTING;
    return 3;
}

s32 ov72_02239E9C(NintendoWfcConnect *arg0);

s32 ov72_02239E9C(NintendoWfcConnect *arg0) {
    Save_PrepareForAsyncWrite(arg0->args->saveData, 2);
    arg0->state = NINTENDO_WFC_STATE_SAVE;
    return 3;
}

s32 ov72_02239EB4(NintendoWfcConnect *arg0);

s32 ov72_02239EB4(NintendoWfcConnect *arg0) {
    if (Save_WriteFileAsync(arg0->args->saveData) == 2) {
        arg0->state = arg0->nextState;
        ov72_0223A444(arg0);
    }
    return 3;
}

s32 ov72_02239ED4(NintendoWfcConnect *arg0);

s32 ov72_02239ED4(NintendoWfcConnect *arg0) {
    ov72_0223A350(arg0, arg0->msgDataTower, 4, 1, 0xF0F);
    ov72_02238680(arg0, 0x30, 0x16);
    return 3;
}
