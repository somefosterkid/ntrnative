#include "global.h"
#include "gs/nonport.h"
#include "overlay_00_nintendo_wfcmanager.h"
#include "constants/heap.h"


extern void ov00_021E7234(int index);

void *ov00_021E65D4(DWCAllocType allocType, u32 size, int alignment);

extern NintendoWFCManager *_0221A680;

int NintendoWFC_Init(SaveData *saveData, enum HeapID heapID, int heapSize, int maxPlayers);

void ov00_021E727C(int index);

void ov00_021E727C(int index) {
    ov00_021E7234(index);

    if (_0221A680->receiveBuffers[index] == 0L) {
        if (_0221A680->secondaryHeapID == HEAP_ID_FIELD2) {
            _0221A680->receiveBuffers[index] = Heap_Alloc(_0221A680->secondaryHeapID, 4096 + 32);
            _0221A680->receiveBuffersOnGlobalHeap[index] = 1;
        } else {
            _0221A680->receiveBuffers[index] = ov00_021E65D4((DWCAllocType)(0), 4096, 32);
            _0221A680->receiveBuffersOnGlobalHeap[index] = 0;
        }

        DWC_SetRecvBuffer(index, ((void *)(((u32)(_0221A680->receiveBuffers[index]) + (32 - 1)) / (32) * (32))), 4096);
    }
}
