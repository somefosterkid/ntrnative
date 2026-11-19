#include "global.h"

void DWC_CloseAllConnectionsHard();

extern u8 *_0221A680;

void ov00_021E6240(u8 aid);

void ov00_021E6240(u8 aid) {
    if (*(s32 *)(_0221A680 + 0x10a8) != 0) {
        DWC_CloseAllConnectionsHard();
        *(u32 *)(_0221A680 + 0x1098) = 0xffffffff;
        *(u32 *)(_0221A680 + 0x1070) = 0x12;
    }
    return;
}
