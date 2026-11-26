#include "global.h"

#include "error_handling.h"
#include "gs/nonport.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern NintendoWFCManager *_0221A680;

int ov00_021E5900(SaveData *saveData, enum HeapID heapID, int heapSize, int maxPlayers);

void ov00_021E62D8(void);

void ov00_021E62D8(void) {
    MI_CpuClear8(_0221A680->clientUpdateTimers, sizeof(_0221A680->clientUpdateTimers));
}



static void ov00_021E62F4(void);

static void ov00_021E62F4(void) {
    for (int i = 0; i < 4; i++) {
        DWC_SetRecvTimeoutTime(i, 0);
    }

    if (DWC_GetMyAID() == 0) {
        for (int i = 0; i < _0221A680->maxPlayers; i++) {
            if (DWC_GetMyAID() != i) {
                if (DWC_GetAIDBitmap() & (0x1 << i)) {
                    GF_ASSERT(DWC_SetRecvTimeoutTime(i, 10 * 1000));
                }
            }
        }
    } else {
        GF_ASSERT(DWC_SetRecvTimeoutTime(0, 10 * 1000));
    }

    _0221A680->enableTimeout = 1;
    ov00_021E62D8();
}

void ov00_021E727C(int index);

void ov00_021E6388(int unused);

void ov00_021E6388(int unused) {
    _0221A680->state = WFC_STATE_MATCHMAKING_SUCCESS;

    for (int i = 0; i < _0221A680->maxPlayers; i++) {
        if (DWC_GetMyAID() != i) {
            ov00_021E727C(i);
        }
    }

    ov00_021E62F4();
}

void ov00_021E5E34(void);

void ov00_021E63CC(DWCError error, BOOL canceled, void *userParam);

void ov00_021E63CC(DWCError error, BOOL canceled, void *userParam) {
    if (error == DWC_ERROR_NONE) {
        if (!canceled) {
            ov00_021E6388(1 - DWC_GetMyAID());
        } else {
            ov00_021E5E34();
        }
    } else {
        _0221A680->state = WFC_STATE_CONNECTION_ERROR;
    }

    if (_0221A680->newClientCallback) {
        _0221A680->newClientCallback(DWC_GetMyAID(), _0221A680->newClientCallbackArg);
    }
}
