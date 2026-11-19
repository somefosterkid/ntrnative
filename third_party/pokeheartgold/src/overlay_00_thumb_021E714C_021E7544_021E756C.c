#include "global.h"

#include <vct.h>

#include "overlay_00_thumb.h"

void ov00_021E7638(VCTSession *);

extern u8 _0221A684;

void ov00_021E756C(u32 param_1, u32 param_2, VCTSession *param_3);

void ov00_021E756C(u32 param_1, u32 param_2, VCTSession *param_3) {
    s32 iVar1;

    switch (param_2) {
    case 1:
        if (*(s32 *)(((u8 **)&_0221A684)[1] + 0x1a54) != 0) {
            VCT_Response(param_3, VCT_RESPONSE_BUSY_HERE);
            VCT_DeleteSession(param_3);
            return;
        }
        *(VCTSession **)(((u8 **)&_0221A684)[1] + 0x1a54) = param_3;
        return;
    case 8:
        VCT_Response(param_3, VCT_RESPONSE_OK);
        ov00_021E7638(param_3);
        ov00_021E6A4C();
        return;
    case 9:
        ov00_021E7638(param_3);
        ov00_021E6A4C();
        return;
    case 4:
        VCT_Response(param_3, VCT_RESPONSE_TERMINATED);
        ov00_021E7638(param_3);
        return;
    case 7:
        if (param_3->mode != *(s32 *)(((u8 **)&_0221A684)[1] + 0x19e8)) {
            ov00_021E7638(param_3);
            return;
        }
        iVar1 = VCT_StartStreaming(param_3);
        if (iVar1 != 0) {
            *(u32 *)(((u8 **)&_0221A684)[1] + 0x19ec) = 2;
            return;
        }
        break;
    case 2:
        ov00_021E7638(param_3);
        return;
    case 3:
    case 0xb:
    case 0xc:
        ov00_021E7638(param_3);
        break;
    }
    return;
}
