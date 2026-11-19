#include "global.h"

s32 VCT_DeleteSession(s32);

s32 VCT_StopStreaming();

extern void *_0221A684;

void ov00_021E7638(s32 arg0);

void ov00_021E7638(s32 arg0) {
    VCT_StopStreaming();
    VCT_DeleteSession(arg0);
    *(s32 *)((u8 *)(*(void **)((u8 *)(&_0221A684) + 4)) + 0x1A54) = 0;
}
