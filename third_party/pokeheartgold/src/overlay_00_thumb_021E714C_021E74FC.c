#include "global.h"

s32 VCT_Response(void *, s32);

s32 VCT_StartStreaming(void *);

extern void *_0221A684;

s32 ov00_021E74FC(void);

s32 ov00_021E74FC(void) {
    void *v0;

    v0 = *(void **)((u8 *)(*(void **)((u8 *)(&_0221A684) + 4)) + 0x1A54);
    if ((v0 != NULL) && (*(s32 *)((u8 *)v0 + 0xC) == 4)) {
        if (VCT_Response(v0, 0) != 0) {
            return 0;
        }
        if (VCT_StartStreaming(*(void **)((u8 *)(*(void **)((u8 *)(&_0221A684) + 4)) + 0x1A54)) != 0) {
            return 1;
        }
        return 0;
    }
    return 0;
}
