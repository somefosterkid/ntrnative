#include "global.h"

extern u8 _0221A684;

void ov00_021E79B4(void);

void ov00_021E79B4(void) {
    (*(void (*)()) & NNS_SndStrmStart)(((s32 *)&_0221A684)[1] + 0x19f8);
    return;
}
