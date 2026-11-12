// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dpw/src/dpw_tr.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "dpw/dpw_tr.h"

#include "dpw_internal.h"
#include "dwc/gamestats.h"
#include "nitro/os.h"
#include "gfl/net.h"
#include "gfl/net_devwifi.h"
#include "dwc/dwc.h"
#include "dwc/ext.h"

// The Global Trade Station's client, see dpw_tr.h. Every call fills the request that the game statistics client sends to
// one page of the server, and DpwTr's process runs it a frame at a time. The state is even while a call waits for its
// answer, and the answer of the server (a code, and for some calls the Pokémon) ends the call with a result: 0 or more
// when it went well, and a negative number for the ways it failed. A call that could not start ends in state 0x18 with
// the result -13, and the other states after an answer or a failure of the game statistics client are 0x18 too, until
// the game asks for the end (func_ov114_021c866c)

// The two codes the server's answer starts with
typedef struct DpwTrAnswerHeader {
    u8 code;
    u8 unk1;
} DpwTrAnswerHeader;

// The state of the library: where the call is, what it ended with, the player's ID and friend key, and the request
// that is sent: the Pokémon, the signature that goes with it and the size of the signature. The answer of the server
// starts with two codes
typedef struct DpwTrWork {
    int state;
    int result;
    s32 pid;
    DpwFriendKey friendKey;
    u8 request[0x128];
    u32 requestId;
    u8 signature[0x80];
    u32 signatureSize;
    u8 code[2];
    void *resultData;
} DpwTrWork;

static int func_ov114_021c8eb4(int group);

// pokebw2's sDpwTrWork
static DpwTrWork data_ov114_021d3d2c;
// pokebw2's sDpwTrServerType
static int data_ov114_021d3d28;

// pokebw2's DpwTr_Init
void func_ov114_021c7ba0(s32 pid, u64 friendKey, int server) {
    data_ov114_021d3d2c.state = 1;
    data_ov114_021d3d2c.result = 0;
    data_ov114_021d3d2c.pid = pid;
    data_ov114_021d3d2c.friendKey.low = (u32)friendKey;
    data_ov114_021d3d2c.friendKey.high = friendKey >> 32;
    switch (server) {
    case GAMESTATS_SERVER_LIVE:
        data_ov114_021d3d28 = GAMESTATS_SERVER_LIVE;
        break;
    case GAMESTATS_SERVER_DEVELOPMENT:
        data_ov114_021d3d28 = GAMESTATS_SERVER_DEVELOPMENT;
        break;
    case GAMESTATS_SERVER_TEST:
        data_ov114_021d3d28 = GAMESTATS_SERVER_TEST;
        break;
    default:
        data_ov114_021d3d28 = GAMESTATS_SERVER_DEVELOPMENT;
        break;
    }
}

// pokebw2's DpwTr_Process
void func_ov114_021c7c1c(void) {
    const u8 *answer;
    int size;
    int type;
    int group;

    switch (data_ov114_021d3d2c.state) {
    case 2:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d3d2c.state = 0x18;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d3d2c.result = func_ov114_021c8eb4(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d3d2c.state = 0x18;
            answer = func_ov114_021ca228(&size);
            __memcpy(data_ov114_021d3d2c.code, answer, sizeof(data_ov114_021d3d2c.code));
            switch (data_ov114_021d3d2c.code[0]) {
            case 1:
                data_ov114_021d3d2c.result = 0;
                break;
            case 2:
                data_ov114_021d3d2c.result = -5;
                break;
            case 3:
                data_ov114_021d3d2c.result = -4;
                break;
            case 7:
                data_ov114_021d3d2c.result = -1;
                break;
            case 12:
                data_ov114_021d3d2c.result = -6;
                break;
            case 13:
                data_ov114_021d3d2c.result = -7;
                break;
            case 8:
                data_ov114_021d3d2c.result = -8;
                break;
            case 9:
                data_ov114_021d3d2c.result = -9;
                break;
            case 10:
                data_ov114_021d3d2c.result = -10;
                break;
            case 11:
                data_ov114_021d3d2c.result = -11;
                break;
            case 14:
                data_ov114_021d3d2c.result = -2;
                break;
            default:
                data_ov114_021d3d2c.result = -13;
                break;
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 4:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d3d2c.state = 0x18;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d3d2c.result = func_ov114_021c8eb4(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d3d2c.state = 0x18;
            answer = func_ov114_021ca228(&size);
            __memcpy(data_ov114_021d3d2c.code, answer, sizeof(data_ov114_021d3d2c.code));
            switch (data_ov114_021d3d2c.code[0]) {
            case 1:
                data_ov114_021d3d2c.result = 0;
                break;
            case 2:
                data_ov114_021d3d2c.result = -5;
                break;
            case 3:
                data_ov114_021d3d2c.result = -4;
                break;
            case 5:
                data_ov114_021d3d2c.result = -3;
                break;
            case 14:
                data_ov114_021d3d2c.result = -2;
                break;
            default:
                data_ov114_021d3d2c.result = -13;
                break;
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 6:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d3d2c.state = 0x18;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d3d2c.result = func_ov114_021c8eb4(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d3d2c.state = 0x18;
            answer = func_ov114_021ca228(&size);
            memcpy(data_ov114_021d3d2c.resultData, answer, 0x128);
            if (size == 0x128) {
                data_ov114_021d3d2c.result = 0;
            } else {
                switch (*(u8 *)data_ov114_021d3d2c.resultData) {
                case 5:
                    data_ov114_021d3d2c.result = -3;
                    break;
                case 3:
                    data_ov114_021d3d2c.result = -4;
                    break;
                case 14:
                    data_ov114_021d3d2c.result = -2;
                    break;
                default:
                    data_ov114_021d3d2c.result = -13;
                    break;
                }
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 8:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d3d2c.state = 0x18;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d3d2c.result = func_ov114_021c8eb4(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d3d2c.state = 0x18;
            answer = func_ov114_021ca228(&size);
            memcpy(data_ov114_021d3d2c.resultData, answer, 0x128);
            if (size == 0x128) {
                data_ov114_021d3d2c.result = 1;
            } else {
                switch (*(u8 *)data_ov114_021d3d2c.resultData) {
                case 5:
                    data_ov114_021d3d2c.result = -3;
                    break;
                case 3:
                    data_ov114_021d3d2c.result = -4;
                    break;
                case 4:
                    data_ov114_021d3d2c.result = 0;
                    break;
                case 14:
                    data_ov114_021d3d2c.result = -2;
                    break;
                default:
                    data_ov114_021d3d2c.result = -13;
                    break;
                }
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 10:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d3d2c.state = 0x18;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d3d2c.result = func_ov114_021c8eb4(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d3d2c.state = 0x18;
            answer = func_ov114_021ca228(&size);
            __memcpy(data_ov114_021d3d2c.code, answer, sizeof(data_ov114_021d3d2c.code));
            switch (data_ov114_021d3d2c.code[0]) {
            case 1:
                data_ov114_021d3d2c.result = 0;
                break;
            case 5:
                data_ov114_021d3d2c.result = -3;
                break;
            case 3:
                data_ov114_021d3d2c.result = -4;
                break;
            case 14:
                data_ov114_021d3d2c.result = -2;
                break;
            default:
                data_ov114_021d3d2c.result = -13;
                break;
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 12:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d3d2c.state = 0x18;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d3d2c.result = func_ov114_021c8eb4(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d3d2c.state = 0x18;
            answer = func_ov114_021ca228(&size);
            __memcpy(data_ov114_021d3d2c.code, answer, sizeof(data_ov114_021d3d2c.code));
            switch (data_ov114_021d3d2c.code[0]) {
            case 1:
                data_ov114_021d3d2c.result = 0;
                break;
            case 5:
                data_ov114_021d3d2c.result = -3;
                break;
            case 2:
                data_ov114_021d3d2c.result = -5;
                break;
            case 3:
                data_ov114_021d3d2c.result = -4;
                break;
            case 14:
                data_ov114_021d3d2c.result = -2;
                break;
            default:
                data_ov114_021d3d2c.result = -13;
                break;
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 14:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d3d2c.state = 0x18;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d3d2c.result = func_ov114_021c8eb4(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d3d2c.state = 0x18;
            answer = func_ov114_021ca228(&size);
            __memcpy(data_ov114_021d3d2c.resultData, answer, 2);
            switch (*(u8 *)data_ov114_021d3d2c.resultData) {
            case 1:
                if ((u32)(size - 2) >= 0x128) {
                    data_ov114_021d3d2c.result = (u32)(size - 2) / 0x128;
                    memcpy(data_ov114_021d3d2c.resultData, answer + 2, size - 2);
                } else if (size == 2) {
                    data_ov114_021d3d2c.result = 0;
                }
                break;
            case 14:
                data_ov114_021d3d2c.result = -2;
                break;
            default:
                data_ov114_021d3d2c.result = -13;
                break;
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 16:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d3d2c.state = 0x18;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d3d2c.result = func_ov114_021c8eb4(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d3d2c.state = 0x18;
            answer = func_ov114_021ca228(&size);
            memcpy(data_ov114_021d3d2c.resultData, answer, 0x128);
            if (size == 0x128) {
                data_ov114_021d3d2c.result = 0;
            } else {
                switch (*(u8 *)data_ov114_021d3d2c.resultData) {
                case 2:
                    data_ov114_021d3d2c.result = -5;
                    break;
                case 12:
                    data_ov114_021d3d2c.result = -6;
                    break;
                case 13:
                    data_ov114_021d3d2c.result = -7;
                    break;
                case 8:
                    data_ov114_021d3d2c.result = -8;
                    break;
                case 9:
                    data_ov114_021d3d2c.result = -9;
                    break;
                case 10:
                    data_ov114_021d3d2c.result = -10;
                    break;
                case 11:
                    data_ov114_021d3d2c.result = -11;
                    break;
                case 14:
                    data_ov114_021d3d2c.result = -2;
                    break;
                default:
                    data_ov114_021d3d2c.result = -13;
                    break;
                }
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 18:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d3d2c.state = 0x18;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d3d2c.result = func_ov114_021c8eb4(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d3d2c.state = 0x18;
            answer = func_ov114_021ca228(&size);
            __memcpy(data_ov114_021d3d2c.code, answer, sizeof(data_ov114_021d3d2c.code));
            switch (data_ov114_021d3d2c.code[0]) {
            case 1:
                data_ov114_021d3d2c.result = 0;
                break;
            case 2:
                data_ov114_021d3d2c.result = -5;
                break;
            case 14:
                data_ov114_021d3d2c.result = -2;
                break;
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 20:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d3d2c.state = 0x18;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d3d2c.result = func_ov114_021c8eb4(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d3d2c.state = 0x18;
            answer = func_ov114_021ca228(&size);
            __memcpy(data_ov114_021d3d2c.code, answer, sizeof(data_ov114_021d3d2c.code));
            switch (data_ov114_021d3d2c.code[0]) {
            case 1:
                data_ov114_021d3d2c.result = 0;
                break;
            case 6:
                data_ov114_021d3d2c.result = 1;
                break;
            case 7:
                data_ov114_021d3d2c.result = 2;
                break;
            case 14:
                data_ov114_021d3d2c.result = -2;
                break;
            default:
                data_ov114_021d3d2c.result = -13;
                break;
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 22:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d3d2c.state = 0x18;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d3d2c.result = func_ov114_021c8eb4(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d3d2c.state = 0x18;
            answer = func_ov114_021ca228(&size);
            __memcpy(data_ov114_021d3d2c.resultData, answer, 2);
            if (size == 8) {
                data_ov114_021d3d2c.result = 0;
            } else {
                switch (*(u8 *)data_ov114_021d3d2c.resultData) {
                case 1:
                    data_ov114_021d3d2c.result = 0;
                    break;
                case 6:
                    data_ov114_021d3d2c.result = 1;
                    break;
                case 7:
                    data_ov114_021d3d2c.result = 2;
                    break;
                case 14:
                    data_ov114_021d3d2c.result = -2;
                    break;
                default:
                    data_ov114_021d3d2c.result = -13;
                    break;
                }
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 0:
    case 1:
    case 0x18:
        break;
    case 3:
    case 5:
    case 7:
    case 9:
    case 11:
    case 13:
    case 15:
    case 17:
    case 19:
    case 21:
    case 23:
        data_ov114_021d3d2c.state = 0x18;
        data_ov114_021d3d2c.result = -12;
        func_ov114_021ca1b4();
        break;
    }
}

// pokebw2's DpwTr_End
void func_ov114_021c8658(void) {
    data_ov114_021d3d2c.state = 0;
}

// pokebw2's DpwTr_IsFinished
BOOL func_ov114_021c866c(void) {
    switch (data_ov114_021d3d2c.state) {
    case 1:
        return TRUE;
    case 0x18:
        data_ov114_021d3d2c.state = 1;
        return TRUE;
    default:
        return FALSE;
    }
}

// pokebw2's DpwTr_GetResult
s32 func_ov114_021c86a8(void) {
    return data_ov114_021d3d2c.result;
}

// pokebw2's DpwTr_Upload
void func_ov114_021c86b8(Dpw_Tr_Data *data, const void *sign, u32 size) {
    memcpy(data_ov114_021d3d2c.request, data, 0x128);
    data_ov114_021d3d2c.requestId = 0;
    memcpy(data_ov114_021d3d2c.signature, sign, size);
    data_ov114_021d3d2c.signatureSize = size;
    func_ov114_021c9ee4(data_ov114_021d3d28, DPW_GAMESTATS_KEY);
    if (func_ov114_021ca470("/web/worldexchange/post.asp", data_ov114_021d3d2c.pid, data_ov114_021d3d2c.request, 0x1b0, NULL) == 0) {
        data_ov114_021d3d2c.state = 2;
    } else {
        data_ov114_021d3d2c.state = 0x18;
        data_ov114_021d3d2c.result = -13;
        func_ov114_021ca1b4();
    }
}

// pokebw2's DpwTr_UploadFinish
void func_ov114_021c8770(void) {
    DpwFriendKey *key = &data_ov114_021d3d2c.friendKey;
    DpwFriendKey *request = (DpwFriendKey *)data_ov114_021d3d2c.request;
    request->low = key->low;
    request->high = key->high;
    func_ov114_021c9ee4(data_ov114_021d3d28, DPW_GAMESTATS_KEY);
    if (func_ov114_021ca470("/web/worldexchange/post_finish.asp", data_ov114_021d3d2c.pid, request, 8, NULL) == 0) {
        data_ov114_021d3d2c.state = 4;
    } else {
        data_ov114_021d3d2c.state = 0x18;
        data_ov114_021d3d2c.result = -13;
        func_ov114_021ca1b4();
    }
}

// pokebw2's DpwTr_Download
void func_ov114_021c87fc(Dpw_Tr_Data *result) {
    data_ov114_021d3d2c.resultData = result;
    func_ov114_021c9ee4(data_ov114_021d3d28, DPW_GAMESTATS_KEY);
    if (func_ov114_021ca470("/web/worldexchange/get.asp", data_ov114_021d3d2c.pid, data_ov114_021d3d2c.request, 0x1b0, NULL) == 0) {
        data_ov114_021d3d2c.state = 6;
    } else {
        data_ov114_021d3d2c.state = 0x18;
        data_ov114_021d3d2c.result = -13;
        func_ov114_021ca1b4();
    }
}

// pokebw2's DpwTr_GetTradeResult
void func_ov114_021c887c(Dpw_Tr_Data *result) {
    data_ov114_021d3d2c.resultData = result;
    func_ov114_021c9ee4(data_ov114_021d3d28, DPW_GAMESTATS_KEY);
    if (func_ov114_021ca470("/web/worldexchange/result.asp", data_ov114_021d3d2c.pid, data_ov114_021d3d2c.request, 0x1b0, NULL) == 0) {
        data_ov114_021d3d2c.state = 8;
    } else {
        data_ov114_021d3d2c.state = 0x18;
        data_ov114_021d3d2c.result = -13;
        func_ov114_021ca1b4();
    }
}

// pokebw2's DpwTr_DeleteFinish
void func_ov114_021c88fc(void) {
    func_ov114_021c9ee4(data_ov114_021d3d28, DPW_GAMESTATS_KEY);
    if (func_ov114_021ca470("/web/worldexchange/delete.asp", data_ov114_021d3d2c.pid, data_ov114_021d3d2c.request, 0x1b0, NULL) == 0) {
        data_ov114_021d3d2c.state = 10;
    } else {
        data_ov114_021d3d2c.state = 0x18;
        data_ov114_021d3d2c.result = -13;
        func_ov114_021ca1b4();
    }
}

// pokebw2's DpwTr_ReturnFinish
void func_ov114_021c8978(void) {
    func_ov114_021c9ee4(data_ov114_021d3d28, DPW_GAMESTATS_KEY);
    if (func_ov114_021ca470("/web/worldexchange/return.asp", data_ov114_021d3d2c.pid, data_ov114_021d3d2c.request, 0, NULL) == 0) {
        data_ov114_021d3d2c.state = 12;
    } else {
        data_ov114_021d3d2c.state = 0x18;
        data_ov114_021d3d2c.result = -13;
        func_ov114_021ca1b4();
    }
}

// Cancels the call that is running, and waits for the next state
// pokebw2's DpwTr_Cancel
void func_ov114_021c89f0(void) {
    switch (data_ov114_021d3d2c.state) {
    case 2:
        DWCi_RankingSessionCancel();
        data_ov114_021d3d2c.state = 3;
        break;
    case 4:
        DWCi_RankingSessionCancel();
        data_ov114_021d3d2c.state = 5;
        break;
    case 6:
        DWCi_RankingSessionCancel();
        data_ov114_021d3d2c.state = 7;
        break;
    case 8:
        DWCi_RankingSessionCancel();
        data_ov114_021d3d2c.state = 9;
        break;
    case 10:
        DWCi_RankingSessionCancel();
        data_ov114_021d3d2c.state = 11;
        break;
    case 12:
        DWCi_RankingSessionCancel();
        data_ov114_021d3d2c.state = 13;
        break;
    case 14:
        DWCi_RankingSessionCancel();
        data_ov114_021d3d2c.state = 15;
        break;
    case 16:
        DWCi_RankingSessionCancel();
        data_ov114_021d3d2c.state = 17;
        break;
    case 18:
        DWCi_RankingSessionCancel();
        data_ov114_021d3d2c.state = 19;
        break;
    case 20:
        DWCi_RankingSessionCancel();
        data_ov114_021d3d2c.state = 21;
        break;
    case 22:
        DWCi_RankingSessionCancel();
        data_ov114_021d3d2c.state = 23;
        break;
    }
}

// pokebw2's DpwTr_Search
void func_ov114_021c8b18(const Dpw_Tr_PokemonSearchData *search, s32 maxNum, Dpw_Tr_Data *result) {
    const u8 *src = (const u8 *)search;
    u8 *request = data_ov114_021d3d2c.request;
    data_ov114_021d3d2c.resultData = result;
    request[0] = src[0];
    request[1] = src[1];
    request[2] = src[2];
    request[3] = src[3];
    request[4] = src[4];
    request[5] = src[5];
    data_ov114_021d3d2c.request[6] = maxNum;
    func_ov114_021c9ee4(data_ov114_021d3d28, DPW_GAMESTATS_KEY);
    if (func_ov114_021ca470("/web/worldexchange/search.asp", data_ov114_021d3d2c.pid, request, 7, NULL) == 0) {
        data_ov114_021d3d2c.state = 14;
    } else {
        data_ov114_021d3d2c.state = 0x18;
        data_ov114_021d3d2c.result = -13;
        func_ov114_021ca1b4();
    }
}

// pokebw2's DpwTr_SearchEx
// The request of func_ov114_021c8bc4: the search, as the bytes the server reads (copied a byte at a time)
typedef struct {
    u8 bytes[8];
} DpwTrSearchExRequest;

void func_ov114_021c8bc4(const Dpw_Tr_PokemonSearchDataEx *search, Dpw_Tr_Data *result) {
    data_ov114_021d3d2c.resultData = result;
    *(DpwTrSearchExRequest *)data_ov114_021d3d2c.request = *(const DpwTrSearchExRequest *)search;
    func_ov114_021c9ee4(data_ov114_021d3d28, DPW_GAMESTATS_KEY);
    if (func_ov114_021ca470("/web/worldexchange/search.asp", data_ov114_021d3d2c.pid, data_ov114_021d3d2c.request, 8, NULL) == 0) {
        data_ov114_021d3d2c.state = 14;
    } else {
        data_ov114_021d3d2c.state = 0x18;
        data_ov114_021d3d2c.result = -13;
        func_ov114_021ca1b4();
    }
}

// pokebw2's DpwTr_Exchange
void func_ov114_021c8c68(s32 id, Dpw_Tr_Data *upload, Dpw_Tr_Data *result, const void *sign, u32 size) {
    data_ov114_021d3d2c.resultData = result;
    memcpy(data_ov114_021d3d2c.request, upload, 0x128);
    data_ov114_021d3d2c.requestId = id;
    memcpy(data_ov114_021d3d2c.signature, sign, size);
    data_ov114_021d3d2c.signatureSize = size;
    func_ov114_021c9ee4(data_ov114_021d3d28, DPW_GAMESTATS_KEY);
    if (func_ov114_021ca470("/web/worldexchange/exchange.asp", data_ov114_021d3d2c.pid, data_ov114_021d3d2c.request, 0x1b0, NULL) == 0) {
        data_ov114_021d3d2c.state = 16;
    } else {
        data_ov114_021d3d2c.state = 0x18;
        data_ov114_021d3d2c.result = -13;
        func_ov114_021ca1b4();
    }
}

// pokebw2's DpwTr_ExchangeFinish
void func_ov114_021c8d14(void) {
    DpwFriendKey *key = &data_ov114_021d3d2c.friendKey;
    DpwFriendKey *request = (DpwFriendKey *)data_ov114_021d3d2c.request;
    request->low = key->low;
    request->high = key->high;
    func_ov114_021c9ee4(data_ov114_021d3d28, DPW_GAMESTATS_KEY);
    if (func_ov114_021ca470("/web/worldexchange/exchange_finish.asp", data_ov114_021d3d2c.pid, request, 8, NULL) == 0) {
        data_ov114_021d3d2c.state = 0x12;
    } else {
        data_ov114_021d3d2c.state = 0x18;
        data_ov114_021d3d2c.result = -13;
        func_ov114_021ca1b4();
    }
}

// pokebw2's DpwTr_GetServerState
void func_ov114_021c8da0(void) {
    func_ov114_021c9ee4(data_ov114_021d3d28, DPW_GAMESTATS_KEY);
    if (func_ov114_021ca470("/web/worldexchange/info.asp", data_ov114_021d3d2c.pid, data_ov114_021d3d2c.request, 0, NULL) == 0) {
        data_ov114_021d3d2c.state = 20;
    } else {
        data_ov114_021d3d2c.state = 0x18;
        data_ov114_021d3d2c.result = -13;
        func_ov114_021ca1b4();
    }
}

// pokebw2's DpwTr_SetProfile
void func_ov114_021c8e18(Dpw_Common_Profile *profile, Dpw_Common_ProfileResult *result) {
    OS_GetMacAddress(profile->unk1C);
    memcpy(data_ov114_021d3d2c.request, profile, 0x64);
    data_ov114_021d3d2c.resultData = result;
    func_ov114_021c9ee4(data_ov114_021d3d28, DPW_GAMESTATS_KEY);
    if (func_ov114_021ca470("/web/common/setProfile.asp", data_ov114_021d3d2c.pid, data_ov114_021d3d2c.request, 0x64, NULL) == 0) {
        data_ov114_021d3d2c.state = 22;
    } else {
        data_ov114_021d3d2c.state = 0x18;
        data_ov114_021d3d2c.result = -13;
        func_ov114_021ca1b4();
    }
}

// The result of a failure of the game statistics client, from its group of errors
// pokebw2's DpwTrMapGamestatsError
static int func_ov114_021c8eb4(int group) {
    int result;

    switch (group) {
    case 0:
    case 1:
    case 2:
        result = -15;
        break;
    case 3:
        result = -14;
        break;
    case 5:
        result = -14;
        break;
    case 4:
        result = -15;
        break;
    case 6:
        result = -14;
        break;
    case 7:
        result = -13;
        break;
    default:
        result = -14;
        break;
    }
    if (result != -13) {
        if (func_0205d074()) {
            result = -14;
        }
        DWC_ClearError();
    }
    return result;
}
