#include "global.h"
#include "gs/nonport.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_00_nintendo_wfcmanager.h"

BOOL ov00_021E77A4(int id, void *buffer, int size);

void *ov00_021E65D4(DWCAllocType allocType, u32 size, int alignment);

void ov00_021E6638(DWCAllocType allocType, void *ptr, u32 size);

extern NintendoWFCManager *_0221A680;

int NintendoWFC_Init(SaveData *saveData, enum HeapID heapID, int heapSize, int maxPlayers);

void ov00_021E644C(u32 header);

void ov00_021E6484(u8 aid, u8 *data, int size);

void ov00_021E6484(u8 aid, u8 *data, int size) {
    u32 headerU32 = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];

    _0221A680->enableTimeout = 1;

    if ((headerU32 & 0xff) == 0x1) {
        ov00_021E644C(headerU32);
        _0221A680->unused_10D1 = data[2];
    } else {
        if (ov00_021E77A4(aid, data, size)) {
            return;
        }

        ov00_021E644C(headerU32);
        return;
    }

    void *buffer = ov00_021E65D4((DWCAllocType)(0L), size - 4, 4);
    if (buffer == 0L) {
        return;
    }

    MI_CpuCopy8(data + 4, buffer, size - 4);

    if (DWC_GetMyAID() == 0) {
        if (_0221A680->dataTransferServerCallback != 0L) {
            _0221A680->dataTransferServerCallback(aid, buffer, size - 4);
        }
    } else {
        if (_0221A680->dataTransferClientCallback != 0L) {
            _0221A680->dataTransferClientCallback(aid, buffer, size - 4);
        }
    }

    ov00_021E6638((DWCAllocType)(0L), buffer, size - 4);
}
