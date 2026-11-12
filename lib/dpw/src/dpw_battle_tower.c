// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dpw/src/dpw_battle_tower.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.

#include "dpw_internal.h"
#include "nitro/os.h"
#include "gfl/net.h"
#include "gfl/net_devwifi.h"
#include "dwc/dwc.h"
#include "dwc/gamestats.h"
#include "dpw/dpw_tr.h"

// The Battle Subway's client, see dpw_bt.h. It works like dpw_tr.c: the states are even while a request is out and the
// answer of the server decides the result, which is 0 or more when it went well and negative otherwise. The states
// end at 0xc, which is also where a request that failed stops

// The team that is uploaded, with what the server needs to know about it. It is built here and then copied to the
// request, the same bytes as the request holds
typedef struct DpwBtUpload {
    u8 team[0xf0];
    u8 kind;
    u8 room;
    u8 win;
    u8 unkF3[5];
    DpwFriendKey key;
    u8 sign[0x80];
    u32 id;
} DpwBtUpload;

typedef struct DpwBtWork {
    int state;
    int result;
    s32 pid;
    DpwFriendKey friendKey;
    u8 request[0x184];
    u8 code[2];
    void *resultData;
    u32 unk1A0;
} DpwBtWork;

int func_ov114_021c97e0(int group);

// pokebw2's sDpwBtWork
static DpwBtWork data_ov114_021d4084;
// pokebw2's sDpwBtUpload
static DpwBtUpload data_ov114_021d3f00;
// pokebw2's sDpwBtServerType
static int data_ov114_021d3efc;
// the answer codes of the server, written here by the cases that read them
static u8 data_ov114_021d3ef8[2];

// pokebw2's DpwBt_End

void func_ov114_021c8f2c(s32 pid, u64 friendKey, int server) {
    data_ov114_021d4084.state = 1;
    data_ov114_021d4084.result = 0;
    data_ov114_021d4084.pid = pid;
    data_ov114_021d4084.friendKey.low = (u32)friendKey;
    data_ov114_021d4084.friendKey.high = friendKey >> 32;
    switch (server) {
    case GAMESTATS_SERVER_LIVE:
        data_ov114_021d3efc = GAMESTATS_SERVER_LIVE;
        break;
    case GAMESTATS_SERVER_DEVELOPMENT:
        data_ov114_021d3efc = GAMESTATS_SERVER_DEVELOPMENT;
        break;
    case GAMESTATS_SERVER_TEST:
        data_ov114_021d3efc = GAMESTATS_SERVER_TEST;
        break;
    default:
        data_ov114_021d3efc = GAMESTATS_SERVER_DEVELOPMENT;
        break;
    }
}

void func_ov114_021c8fa8(void) {
    const u8 *answer;
    int size;
    int type;
    int group;

    switch (data_ov114_021d4084.state) {
    case 2:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d4084.state = 0xc;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d4084.result = func_ov114_021c97e0(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d4084.state = 0xc;
            answer = func_ov114_021ca228(&size);
            __memcpy(data_ov114_021d4084.code, answer, sizeof(data_ov114_021d4084.code));
            if (size != 0) {
                data_ov114_021d4084.result = data_ov114_021d4084.code[0];
            } else {
                data_ov114_021d4084.result = -2;
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 4:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d4084.state = 0xc;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d4084.result = func_ov114_021c97e0(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d4084.state = 0xc;
            answer = func_ov114_021ca228(&size);
            memcpy(data_ov114_021d4084.resultData, answer, 0xa8c);
            if (size == 0xa8c) {
                data_ov114_021d4084.result = 0;
            } else {
                switch (*(u8 *)data_ov114_021d4084.resultData) {
                case 2:
                    data_ov114_021d4084.result = -3;
                    break;
                case 5:
                    data_ov114_021d4084.result = -2;
                    break;
                default:
                    data_ov114_021d4084.result = -5;
                    break;
                }
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 6:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d4084.state = 0xc;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d4084.result = func_ov114_021c97e0(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d4084.state = 0xc;
            answer = func_ov114_021ca228(&size);
            __memcpy(data_ov114_021d4084.code, answer, sizeof(data_ov114_021d4084.code));
            switch (data_ov114_021d4084.code[0]) {
            case 1:
                data_ov114_021d4084.result = 0;
                break;
            case 2:
                data_ov114_021d4084.result = -3;
                break;
            case 4:
                data_ov114_021d4084.result = -1;
                break;
            case 5:
                data_ov114_021d4084.result = -2;
                break;
            default:
                data_ov114_021d4084.result = -5;
                break;
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 8:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d4084.state = 0xc;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d4084.result = func_ov114_021c97e0(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d4084.state = 0xc;
            answer = func_ov114_021ca228(&size);
            __memcpy(data_ov114_021d4084.code, answer, sizeof(data_ov114_021d4084.code));
            switch (data_ov114_021d4084.code[0]) {
            case 1:
                data_ov114_021d4084.result = 0;
                break;
            case 3:
                data_ov114_021d4084.result = 1;
                break;
            case 4:
                data_ov114_021d4084.result = 2;
                break;
            case 5:
                data_ov114_021d4084.result = -2;
                break;
            default:
                data_ov114_021d4084.result = -5;
                break;
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 10:
        switch (func_ov114_021ca0a0()) {
        case GAMESTATS_STATE_FAILED:
            data_ov114_021d4084.state = 0xc;
            DWC_GetLastErrorEx(&type, &group);
            data_ov114_021d4084.result = func_ov114_021c97e0(group);
            func_ov114_021ca1b4();
            break;
        case GAMESTATS_STATE_DONE:
            data_ov114_021d4084.state = 0xc;
            answer = func_ov114_021ca228(&size);
            __memcpy(data_ov114_021d4084.resultData, answer, 8);
            if (size == 8) {
                data_ov114_021d4084.result = 0;
            } else {
                switch (*(u8 *)data_ov114_021d4084.resultData) {
                case 1:
                    data_ov114_021d4084.result = 0;
                    break;
                case 3:
                    data_ov114_021d4084.result = 1;
                    break;
                case 4:
                    data_ov114_021d4084.result = 2;
                    break;
                case 5:
                    data_ov114_021d4084.result = -2;
                    break;
                default:
                    data_ov114_021d4084.result = -5;
                    break;
                }
            }
            func_ov114_021ca1b4();
            break;
        }
        break;
    case 3:
    case 5:
    case 7:
    case 9:
    case 11:
        data_ov114_021d4084.state = 0xc;
        data_ov114_021d4084.result = -4;
        func_ov114_021ca1b4();
        break;
    case 0:
    case 1:
    case 0xc:
        break;
    }
}

void func_ov114_021c93bc(void) {
    data_ov114_021d4084.state = 0;
}

// pokebw2's DpwBt_IsFinished
BOOL func_ov114_021c93d0(void) {
    switch (data_ov114_021d4084.state) {
    case 1:
        return TRUE;
    case 0xc:
        data_ov114_021d4084.state = 1;
        return TRUE;
    default:
        return FALSE;
    }
}

// pokebw2's DpwBt_GetResult
s32 func_ov114_021c940c(void) {
    return data_ov114_021d4084.result;
}

void func_ov114_021c941c(int kind) {
    data_ov114_021d4084.request[0] = kind - 1;
    func_ov114_021c9ee4(data_ov114_021d3efc, "HZEdGCzcGGLvguqUEKQN0001d93500002dd5000000082db842b2syachi2ds");
    if (func_ov114_021ca470("/web/battletower/roomnum.asp", data_ov114_021d4084.pid, data_ov114_021d4084.request, 1, NULL) == 0) {
        data_ov114_021d4084.state = 2;
        return;
    }
    data_ov114_021d4084.state = 0xc;
    data_ov114_021d4084.result = -5;
    func_ov114_021ca1b4();
}

void func_ov114_021c94a0(int kind, int room, void *result) {
    data_ov114_021d4084.resultData = result;
    data_ov114_021d4084.request[0] = kind - 1;
    data_ov114_021d4084.request[1] = room - 1;
    func_ov114_021c9ee4(data_ov114_021d3efc, "HZEdGCzcGGLvguqUEKQN0001d93500002dd5000000082db842b2syachi2ds");
    if (func_ov114_021ca470("/web/battletower/download.asp", data_ov114_021d4084.pid, data_ov114_021d4084.request, 2, NULL) == 0) {
        data_ov114_021d4084.state = 4;
        return;
    }
    data_ov114_021d4084.state = 0xc;
    data_ov114_021d4084.result = -5;
    func_ov114_021ca1b4();
}

void func_ov114_021c9530(int kind, int room, u8 win, const void *team, const void *sign, u32 id) {
    memcpy(data_ov114_021d3f00.team, team, 0xf0);
    data_ov114_021d3f00.kind = kind - 1;
    data_ov114_021d3f00.room = room - 1;
    data_ov114_021d3f00.win = win;
    __memcpy(&data_ov114_021d3f00.key, &data_ov114_021d4084.friendKey, sizeof(data_ov114_021d3f00.key));
    memcpy(data_ov114_021d3f00.sign, sign, 0x80);
    data_ov114_021d3f00.id = id;
    *(DpwBtUpload *)data_ov114_021d4084.request = data_ov114_021d3f00;
    func_ov114_021c9ee4(data_ov114_021d3efc, DPW_GAMESTATS_KEY);
    if (func_ov114_021ca470("/web/battletower/upload.asp", data_ov114_021d4084.pid, data_ov114_021d4084.request, 0x184, NULL) == 0) {
        data_ov114_021d4084.state = 6;
    } else {
        data_ov114_021d4084.state = 0xc;
        data_ov114_021d4084.result = -5;
        func_ov114_021ca1b4();
    }
}

void func_ov114_021c9634(void) {
    switch (data_ov114_021d4084.state) {
    case 2:
        DWCi_RankingSessionCancel();
        data_ov114_021d4084.state = 3;
        break;
    case 4:
        DWCi_RankingSessionCancel();
        data_ov114_021d4084.state = 5;
        break;
    case 6:
        DWCi_RankingSessionCancel();
        data_ov114_021d4084.state = 7;
        break;
    case 8:
        DWCi_RankingSessionCancel();
        data_ov114_021d4084.state = 9;
        break;
    case 10:
        DWCi_RankingSessionCancel();
        data_ov114_021d4084.state = 11;
        break;
    }
}





void func_ov114_021c96cc(void) {
    func_ov114_021c9ee4(data_ov114_021d3efc, "HZEdGCzcGGLvguqUEKQN0001d93500002dd5000000082db842b2syachi2ds");
    if (func_ov114_021ca470("/web/battletower/info.asp", data_ov114_021d4084.pid, data_ov114_021d4084.request, 0, NULL) == 0) {
        data_ov114_021d4084.state = 8;
        return;
    }
    data_ov114_021d4084.state = 0xc;
    data_ov114_021d4084.result = -5;
    func_ov114_021ca1b4();
}

void func_ov114_021c9744(Dpw_Common_Profile *profile, Dpw_Common_ProfileResult *result) {
    OS_GetMacAddress(profile->unk1C);
    memcpy(data_ov114_021d4084.request, profile, 0x64);
    data_ov114_021d4084.resultData = result;
    func_ov114_021c9ee4(data_ov114_021d3efc, "HZEdGCzcGGLvguqUEKQN0001d93500002dd5000000082db842b2syachi2ds");
    if (func_ov114_021ca470("/web/common/setProfile.asp", data_ov114_021d4084.pid, data_ov114_021d4084.request, 0x64, NULL) == 0) {
        data_ov114_021d4084.state = 10;
        return;
    }
    data_ov114_021d4084.state = 0xc;
    data_ov114_021d4084.result = -5;
    func_ov114_021ca1b4();
}



static int func_ov114_021c97e0(int group) {
    int result;

    switch (group) {
    case 0:
    case 1:
    case 2:
        result = -7;
        break;
    case 3:
        result = -6;
        break;
    case 5:
        result = -6;
        break;
    case 4:
        result = -7;
        break;
    case 6:
        result = -6;
        break;
    case 7:
        result = -5;
        break;
    default:
        result = -6;
        break;
    }
    if (result != -5) {
        if (func_0205d074()) {
            result = -6;
        }
        DWC_ClearError();
    }
    return result;
}


