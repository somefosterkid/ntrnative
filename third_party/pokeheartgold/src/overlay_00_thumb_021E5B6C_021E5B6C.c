#include "global.h"
#include "constants/heap.h"
#include "overlay_00_nintendo_wfcmanager.h"

enum NintendoWFCResult {
    NINTENDO_WFC_RESULT_MATCHMAKING_SUCCESS = DWC_ERROR_NUM,
    NINTENDO_WFC_RESULT_CONN_RESET,
    NINTENDO_WFC_RESULT_CONN_RESET_AFTER_HOST_LEFT,
    NINTENDO_WFC_RESULT_RECEIVE_TIMED_OUT,
    NINTENDO_WFC_RESULT_CONNECTION_CLOSED,
    NINTENDO_WFC_RESULT_UNUSED_5,
    NINTENDO_WFC_RESULT_DUMMY_6,
    NINTENDO_WFC_RESULT_CONNECTED_TO_SERVER,
};

int ov00_021E6690(void);

void ov00_021E61A8(DWCError error, int profileID, void *userParam);

void *ov00_021E65D4(DWCAllocType allocType, u32 size, int alignment);

void ov00_021E6638(DWCAllocType allocType, void *ptr, u32 size);

extern NintendoWFCManager *_0221A680;

int ov00_021E5900(SaveData *saveData, enum HeapID heapID, int heapSize, int maxPlayers);

int ov00_021E5B6C(void);

extern const char _02216400[];

int ov00_021E5B6C(void) {
    switch (_0221A680->state) {
    case WFC_STATE_INIT: {
        DWC_SetMemFunc(ov00_021E65D4, ov00_021E6638);
        DWC_InitInetEx(&_0221A680->inetCtl, 2, 1, 20);
        DWC_SetAuthServer(DWC_CONNECTINET_AUTH_RELEASE);
        DWC_ConnectInetAsync();

        _0221A680->state = WFC_STATE_WAIT_CONNECTION;
        _0221A680->voiceChatActive = 0;
    }
    case WFC_STATE_WAIT_CONNECTION: {
        if (DWC_CheckInet()) {
            if (DWC_GetInetStatus() == DWC_CONNECTINET_STATE_CONNECTED) {
                _0221A680->state = WFC_STATE_INIT_LOGIN;
            } else {
                _0221A680->state = WFC_STATE_CONNECTION_ERROR;
            }
        } else {
            DWC_ProcessInet();
            break;
        }
    }
    case WFC_STATE_INIT_LOGIN: {
        int error = ov00_021E6690();

        if (error != DWC_ERROR_NONE) {
            return error;
        }
    }

        DWC_InitFriendsMatch(&(_0221A680->friendsMatchCtl), _0221A680->userData, 10727, _02216400, "1vTlwb", 0, 0, _0221A680->friends, 32);

        OSOwnerInfo ownerInfo;
        OS_GetOwnerInfo(&ownerInfo);
        DWC_LoginAsync(ownerInfo.nickName, 0L, ov00_021E61A8, 0L);

        _0221A680->state = WFC_STATE_WAIT_FOR_LOGIN;

    case WFC_STATE_WAIT_FOR_LOGIN:
        DWC_ProcessFriendsMatch();
        break;
    case WFC_STATE_READY:
        DWC_ProcessFriendsMatch();
        return NINTENDO_WFC_RESULT_CONNECTED_TO_SERVER;
    default:
        break;
    }

    return ov00_021E6690();
}
