#include "global.h"

s32 VCT_CreateSession(u8);

void VCT_DeleteSession(s32);

s32 VCT_Request(s32, u32);

extern u8 _0221A684;

BOOL ov00_021E74A8(u8 id);

BOOL ov00_021E74A8(u8 id) {
    s32 iVar1;
    s32 iVar2;

    iVar2 = *(s32 *)(((u8 **)&_0221A684)[1] + 0x1a54);
    if (iVar2 == 0) {
        iVar2 = VCT_CreateSession(id);
        if (iVar2 == 0) {
            return 0;
        }
        iVar1 = VCT_Request(iVar2, 0);
    } else {
        iVar1 = VCT_Request(iVar2, 0);
    }
    if (iVar1 != 0) {
        VCT_DeleteSession(iVar2);
        *(u32 *)(((u8 **)&_0221A684)[1] + 0x1a54) = 0;
        return 0;
    }
    *(s32 *)(((u8 **)&_0221A684)[1] + 0x1a54) = iVar2;
    return 1;
}
