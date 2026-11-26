#include "global.h"

#include "error_handling.h"
#include "gs/nonport.h"
#include "overlay_00_nintendo_wfcmanager.h"
#include "constants/heap.h"
#include "save.h"

void ov00_021E63CC(DWCError error, BOOL canceled, void *userParam);

void ov00_021E6428(int param_1, u8 param_2);

void ov00_021E6484(u8 aid, u8 *data, int size);

void ov00_021E6554(DWCError error, BOOL selfClosed, BOOL hostClosed, u8 aid, int closingFriendIdx, void *userParam);

int ov00_021E6424(int index, void *userParam);

void ov00_021E6240(u8 aid);

void ov00_021E7300(void);

extern NintendoWFCManager *_0221A680;

int ov00_021E5900(SaveData *saveData, enum HeapID heapID, int heapSize, int maxPlayers);

extern const char _02216034[];

BOOL ov00_021E5CEC(u8 *matchmakingString, int maxPlayers, BOOL addSecondKey, u32 timeout);

BOOL ov00_021E5CEC(u8 *matchmakingString, int maxPlayers, BOOL addSecondKey, u32 timeout) {
    ((_0221A680 != 0L) ? (void)0 : GF_AssertFail());

    if (_0221A680->state != WFC_STATE_READY) {
        return 0;
    }

    ov00_021E7300();

    int result;
    DWCMatchOptMinComplete matchOpts = { .valid = 1, .minEntry = 2, .timeout = 0 };

    matchOpts.timeout = timeout * 1000;
    result = DWC_SetMatchingOption(DWC_MATCH_OPTION_MIN_COMPLETE, &matchOpts, sizeof(matchOpts));

    ((result == DWC_SET_MATCH_OPT_RESULT_SUCCESS) ? (void)0 : GF_AssertFail());

    ((DWC_AddMatchKeyString(0, "ppw_lobby", (const char *)matchmakingString) != 0) ? (void)0 : GF_AssertFail());

    MI_CpuClear8(_0221A680->ppwMatchKey, 128);
    sprintf((char *)_0221A680->ppwMatchKey, _02216034, "ppw_lobby", matchmakingString);
    ((strlen((const char *)_0221A680->ppwMatchKey) < 128) ? (void)0 : GF_AssertFail());

    if (addSecondKey) {
        DWC_AddMatchKeyString(1, (const char *)_0221A680->ppwMatchKey, (const char *)_0221A680->ppwMatchKey);
    }

    _0221A680->state = WFC_STATE_WAIT_MATCHMAKING;
    _0221A680->maxPlayers = maxPlayers;
    DWC_ConnectToAnybodyAsync(maxPlayers, (const char *)_0221A680->ppwMatchKey, ov00_021E63CC, 0L, ov00_021E6424, 0L);
    _0221A680->connectionRole = 0;

    DWC_SetUserSendCallback(ov00_021E6428);
    DWC_SetUserRecvCallback(ov00_021E6484);
    DWC_SetConnectionClosedCallback(ov00_021E6554, 0L);
    DWC_SetUserRecvTimeoutCallback(ov00_021E6240);

    _0221A680->dataSent = 0;
    _0221A680->disconnectIfAlone = 1;

    return 1;
}
