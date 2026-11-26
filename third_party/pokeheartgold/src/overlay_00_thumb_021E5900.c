#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "unk_0202C034.h"
#include "overlay_00_nintendo_wfcmanager.h"

void ov00_021E700C(void);

extern NintendoWFCManager *_0221A680;

int ov00_021E5900(SaveData *saveData, enum HeapID heapID, int heapSize, int maxPlayers);

int ov00_021E5900(SaveData *saveData, enum HeapID heapID, int heapSize, int maxPlayers) {
    ((_0221A680 == 0L) ? (void)0 : GF_AssertFail());

    void *wfcManAllocPtr = Heap_Alloc(heapID, sizeof(NintendoWFCManager) + 32);
    MI_CpuClear8(wfcManAllocPtr, sizeof(NintendoWFCManager) + 32);

    _0221A680 = (NintendoWFCManager *)((void *)(((u32)(wfcManAllocPtr) + (32 - 1)) / (32) * (32)));
    _0221A680->allocStartPtr = wfcManAllocPtr;
    _0221A680->saveData = saveData;
    _0221A680->dataTransferServerCallback = 0L;
    _0221A680->dataTransferClientCallback = 0L;
    _0221A680->fatalErrorCallback = 0L;
    _0221A680->state = WFC_STATE_INIT;
    _0221A680->primaryHeapID = heapID;
    _0221A680->secondaryHeapID = heapID;
    _0221A680->heapSize = heapSize;
    _0221A680->heap1AllocPtr = Heap_Alloc(heapID, heapSize + (4096 * 3) + 32);
    _0221A680->heap2AllocPtr = 0L;
    _0221A680->heap1 = NNS_FndCreateExpHeapEx(((void *)(((u32)(_0221A680->heap1AllocPtr) + (32 - 1)) / (32) * (32))), heapSize, 0);
    _0221A680->heap2 = 0L;
    _0221A680->voiceChatCodec = 0;
    _0221A680->hostFriendIdx = -1;
    _0221A680->nextFriendIdx = 0;
    _0221A680->maxPlayers = maxPlayers;
    _0221A680->voiceConnectedIDs = 0;
    _0221A680->matchmakingAIDBitmap = 0;
    _0221A680->latestNewClientFriendIdx = -1;
    _0221A680->voiceChatEnabled = 1;
    _0221A680->dummy_10D2 = 0;
    _0221A680->timer = 0;
    _0221A680->packetsSent = 0;
    _0221A680->unused_10D1 = 0;
    _0221A680->inVoiceChatLobby = 1;
    _0221A680->otherPlayersEnabledVoiceChat = 1;
    _0221A680->voiceChatEnabled_Battles = 1;

    if (saveData != 0L) {
        _0221A680->userData = sub_0202C08C(sub_0202C6F4(_0221A680->saveData));
        _0221A680->friends = sub_0202C23C(sub_0202C6F4(_0221A680->saveData), 0);
    }

    _0221A680->disconnectIfAlone = 1;
    _0221A680->friendStatusesBuffer = 0L;

    for (int i = 0; i < 32; i++) {
        _0221A680->friendConnStatus[i] = DWC_STATUS_OFFLINE;
    }

    ov00_021E700C();

    if (!DWC_CheckHasProfile(_0221A680->userData)) {
        return 1;
    }

    if (!DWC_CheckValidConsole(_0221A680->userData)) {
        return 2;
    }

    return 0;
}
