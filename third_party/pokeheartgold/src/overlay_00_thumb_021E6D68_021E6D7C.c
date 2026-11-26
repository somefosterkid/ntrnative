#include "global.h"
#include "gs/nonport.h"
#include "overlay_00_nintendo_wfcmanager.h"
#include "constants/heap.h"
#include "save.h"

s32 ov00_021E7314(void);

void ov00_021E6428(int param_1, u8 param_2);

void ov00_021E6484(u8 aid, u8 *data, int size);

void ov00_021E6554(DWCError error, BOOL selfClosed, BOOL hostClosed, u8 aid, int closingFriendIdx, void *userParam);

void ov00_021E6240(u8 aid);

void ov00_021E7300(void);

void ov00_021E6ED8(DWCError error, BOOL cancelled, BOOL isThisConsole, BOOL hostCancelled, int hostFriendIdx, void *userParam);

void ov00_021E6FBC(DWCError error, BOOL cancelled, BOOL isThisConsole, BOOL hostCancelled, int hostFriendIdx, void *userParam);

void ov00_021E6FE0(int arg0, void *arg1);

extern NintendoWFCManager *_0221A680;

int ov00_021E5900(SaveData *saveData, enum HeapID heapID, int heapSize, int maxPlayers);

int ov00_021E6D7C(int hostFriendIdx, int maxParties, BOOL isTwoPlayersOnly);

int ov00_021E6D7C(int hostFriendIdx, int maxParties, BOOL isTwoPlayersOnly) {

    if (ov00_021E7314()) {
        return -4;
    }

    if (_0221A680->state != WFC_STATE_READY) {
        _0221A680->timer++;

        if (_0221A680->timer > 120) {
            return -3;
        }

        return -1;
    }

    ov00_021E7300();

    _0221A680->matchmakingAIDBitmap = 0;
    _0221A680->disconnectIfAlone = TRUE;
    _0221A680->hostFriendIdx = hostFriendIdx;
    _0221A680->maxPlayers = maxParties;

    if (isTwoPlayersOnly) {
        maxParties = 2;
    }

    _0221A680->unused_1094 = 1;

    BOOL success;
    if (hostFriendIdx < 0) {
        success = DWC_SetupGameServer((u8)maxParties, ov00_021E6ED8, NULL, ov00_021E6FE0, NULL);
        _0221A680->connectionRole = WFC_CONNECTION_HOST;
    } else {
        success = DWC_ConnectToGameServerAsync(hostFriendIdx, ov00_021E6FBC, NULL, ov00_021E6FE0, NULL);
        _0221A680->connectionRole = WFC_CONNECTION_CLIENT;
    }

    if (!success) {
        _0221A680->timer++;

        if (_0221A680->timer > 120) {
            return -3;
        }

        return -2;
    }

    _0221A680->timer = 0;
    _0221A680->state = WFC_STATE_WAIT_MATCHMAKING;

    DWC_SetUserSendCallback(ov00_021E6428);
    DWC_SetUserRecvCallback(ov00_021E6484);
    DWC_SetConnectionClosedCallback(ov00_021E6554, NULL);
    DWC_SetUserRecvTimeoutCallback(ov00_021E6240);

    _0221A680->dataSent = FALSE;

    return 0;
}
