#include "global.h"

#include "unk_0203170C.h"
#include "unk_02037C94.h"
#include "overlay_72_wfc_connect.h"

s32 ov72_02237CB0(void);

s32 ov72_022393B0(NintendoWfcConnect *arg0);

s32 ov72_022393B0(NintendoWfcConnect *arg0) {
    ov72_02237CB0();
    arg0->state = NINTENDO_WFC_STATE_WAIT_INFO;
    arg0->timeoutCounter = 0;
    return 3;
}

s32 ov72_02237B54(void);

s32 ov72_02237B74(void);

s32 ov72_02238680(void *, s32, s32);

s32 ov72_0223A444(void *);

s32 ov72_022393CC(NintendoWfcConnect *arg0);

s32 ov72_022393CC(NintendoWfcConnect *arg0) {
    s32 v0;
    s32 v1;
    u32 v2;

    if (ov72_02237B54() != 0) {
        v0 = ov72_02237B74();
        arg0->timeoutCounter = 0;
        v2 = v0 + 7;
        switch (v2) {
        case 7:
            ov72_0223A444(arg0);
            v1 = arg0->args->mode;
            switch (v1) {
            case 0:
                ov72_02238680(arg0, 0xB, 0xD);
                break;
            case 1:
                arg0->state = NINTENDO_WFC_STATE_PRINT_SENDING_RECORD;
                break;
            case 2:
                arg0->state = NINTENDO_WFC_STATE_ASK_LEADERS_RANK;
                break;
            }
            break;
        case 8:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            break;
        case 6:
        case 9:
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
        case 1:
        case 5:
            arg0->httpResult = v0;
            arg0->state = NINTENDO_WFC_STATE_SHOW_HTTP_ERROR;
            ov72_0223A444(arg0);
            break;
        default:
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

s32 ov72_02237CF4(void *, void *);

s32 ov72_022394A4(NintendoWfcConnect *arg0);

s32 ov72_022394A4(NintendoWfcConnect *arg0) {
    sub_0203189C(arg0->args->saveData, (void *)&arg0->profile);
    ov72_02237CF4(&arg0->profile, &arg0->profileResponse);
    arg0->state = NINTENDO_WFC_STATE_WAIT_SET_PROFILE;
    arg0->timeoutCounter = 0;
    return 3;
}
