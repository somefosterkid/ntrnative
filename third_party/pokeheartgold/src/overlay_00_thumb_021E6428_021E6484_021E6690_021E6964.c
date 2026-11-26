#include "global.h"
#include "gs/nonport.h"
#include "overlay_00_nintendo_wfcmanager.h"
#include "constants/heap.h"
#include "save.h"

extern NintendoWFCManager *_0221A680;

int ov00_021E5900(SaveData *saveData, enum HeapID heapID, int heapSize, int maxPlayers);

int ov00_021E6964(void);

int ov00_021E6964(void) {
    if (_0221A680) {
        if (_0221A680->state == WFC_STATE_MATCHMAKING_SUCCESS
            || _0221A680->state == WFC_STATE_WAIT_AFTER_MATCHMAKING_SUCCESS
            || _0221A680->state == WFC_STATE_CLOSE_CONNECTION) {
            return DWC_GetMyAID();
        }
    }

    return -1;
}

void ov00_021E6994(void);

void ov00_021E6994(void) {
    *((s32 *)&_0221A680->voiceChatActive) = 0;
}
