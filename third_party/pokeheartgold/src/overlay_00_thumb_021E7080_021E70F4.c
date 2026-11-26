#include "global.h"
#include "gs/nonport.h"
#include "overlay_00_nintendo_wfcmanager.h"
#include "constants/heap.h"
#include "save.h"

extern NintendoWFCManager *_0221A680;

int ov00_021E5900(SaveData *saveData, enum HeapID heapID, int heapSize, int maxPlayers);

void ov00_021E70E0(void *arg0);

int ov00_021E70F4(void);

int ov00_021E70F4(void) {
    if (_0221A680->matchmakingCancelState == MATCHMAKING_NOT_CANCELED) {
        _0221A680->matchmakingCancelState = MATCHMAKING_CANCEL_REQUESTED;
        DWC_StopSCMatchingAsync(ov00_021E70E0, NULL);
    }

    return _0221A680->matchmakingCancelState == MATCHMAKING_CANCELED;
}
