#include "global.h"

#include <vct.h>

#include "heap.h"
#include "overlay_00_thumb.h"

extern void *_0221A684;

void ov00_021E79CC(void);

void ov00_021E79CC(void) {
    VCTSession *v0;
    s32 v1;

    v0 = *(VCTSession **)((u8 *)(*(void **)((u8 *)(&_0221A684) + 4)) + 0x1A54);
    if ((v0 == NULL) || (v1 = *(s32 *)((u8 *)(*(void **)((u8 *)(&_0221A684) + 4)) + 0x19EC), (v1 == 0))) {
        ov00_021E6A4C();
        return;
    }
    if ((v1 == 1) && (VCT_Request(v0, VCT_REQUEST_CANCEL) != 0)) {
        ov00_021E6A4C();
        return;
    }
    if (VCT_Request(*(VCTSession **)((u8 *)(*(void **)((u8 *)(&_0221A684) + 4)) + 0x1A54), VCT_REQUEST_BYE) != 0) {
        ov00_021E6A4C();
    }
}

void ov00_021E7A1C(void *arg0);

void ov00_021E7A1C(void *arg0) {
    *(void **)((u8 *)(*(void **)((u8 *)(&_0221A684) + 4)) + 0x198C) = arg0;
}

void ov00_021E7A30(void);

void ov00_021E7A30(void) {
    s32 (*v0)();

    if (*(void **)((u8 *)(&_0221A684) + 4) != NULL) {
        v0 = *(s32(**)())((u8 *)(*(void **)((u8 *)(&_0221A684) + 4)) + 0x198C);
        MIC_StopAutoSampling();
        NNS_SndStrmStop(*(void **)((u8 *)(&_0221A684) + 4) + 0x19F8);
        NNS_SndStrmFreeChannel(*(void **)((u8 *)(&_0221A684) + 4) + 0x19F8);
        VCT_Cleanup();
        Heap_FreeExplicit(*(enum HeapID *)((u8 *)(*(void **)((u8 *)(&_0221A684) + 4)) + 0x19F4), *(void **)((u8 *)(*(void **)((u8 *)(&_0221A684) + 4)) + 0x888));
        Heap_FreeExplicit(*(enum HeapID *)((u8 *)(*(void **)((u8 *)(&_0221A684) + 4)) + 0x19F4), *(void **)((u8 *)(*(void **)((u8 *)(&_0221A684) + 4)) + 0x880));
        *(void **)((u8 *)(&_0221A684) + 4) = NULL;
        if (v0 != NULL) {
            v0();
        }
    }
}
