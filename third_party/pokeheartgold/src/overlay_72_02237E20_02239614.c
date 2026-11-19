#include "global.h"

#include "overlay_72_wfc_connect.h"

#include "message_format.h"
#include "unk_02037C94.h"

s32 ov72_02237B80(s32);

s32 ov72_0223A420(NintendoWfcConnect *);

s32 ov72_02239614(NintendoWfcConnect *arg0);

s32 ov72_02239614(NintendoWfcConnect *arg0) {
    ov72_02237B80(arg0->rank);
    arg0->state = NINTENDO_WFC_STATE_WAIT_ROOM_COUNT;
    arg0->timeoutCounter = 0;
    ov72_0223A420(arg0);
    return 3;
}

s32 ov72_02237B54(void);

s32 ov72_02237B74(void);

s32 ov72_0223A444(void *);

s32 ov72_02239638(NintendoWfcConnect *arg0);

s32 ov72_02239638(NintendoWfcConnect *arg0) {
    s32 v0;
    u32 v1;

    if (ov72_02237B54() != 0) {
        v0 = ov72_02237B74();
        arg0->timeoutCounter = 0;
        v1 = v0 + 7;
        switch (v1) {
        case 6:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            break;
        case 5:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            break;
        case 4:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            break;
        case 1:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            break;
        case 0:
        case 3:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            break;
        case 2:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            sub_020399EC();
            break;
        default:
            if (v0 > 0) {
                arg0->roomCount = v0;
                arg0->state = arg0->nextState;
            }
            break;
        }
        ov72_0223A444(arg0);
    } else {
        arg0->timeoutCounter = (s32)(arg0->timeoutCounter + 1);
        if (arg0->timeoutCounter == 0xE10) {
            sub_020399EC();
        }
    }
    return 3;
}

s32 ov72_02238680(void *, s32, s32);

s32 ov72_0223A280(void *, MsgData *, s32, s32, s32);

s32 ov72_022396E8(NintendoWfcConnect *arg0);

s32 ov72_022396E8(NintendoWfcConnect *arg0) {
    s32 num;

    num = arg0->roomCount;
    if (num >= 0x64) {
        BufferIntegerAsString(arg0->msgFormat, 0U, num, 3U, PRINTING_MODE_LEADING_ZEROS, 1);
        ov72_0223A280(arg0, arg0->msgDataTower, 0xB, 1, 0xF0F);
    } else {
        BufferIntegerAsString(arg0->msgFormat, 0U, num, 2U, PRINTING_MODE_LEADING_ZEROS, 1);
        ov72_0223A280(arg0, arg0->msgDataTower, 0, 1, 0xF0F);
    }
    ov72_02238680(arg0, 0x2F, 0xE);
    return 3;
}

s32 ov72_0223A41C(void *, s32, s32);

s32 ov72_0223A520(NintendoWfcConnect *, s32, s32);

s32 ov72_02239760(NintendoWfcConnect *arg0);

s32 ov72_02239760(NintendoWfcConnect *arg0) {
    ov72_0223A520(arg0, 1, arg0->roomCount);
    arg0->state = NINTENDO_WFC_STATE_HANDLE_CHALLENGE_ROOM_INPUT;
    ov72_0223A41C(arg0, 0, 1);
    return 3;
}
