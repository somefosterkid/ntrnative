#include "global.h"

#include "constants/heap.h"

#include "screen_fade.h"
#include "sound_02004A44.h"
#include "sound_chatot.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"




MotionBlur *ov106_021E601C(int coeffA, int coeffB);




void sub_02006118(u16, u16);

u32 ov106_021E62F4(LegendaryCinematic *param_1);

u32 ov106_021E62F4(LegendaryCinematic *param_1) {
    s32 iVar1;
    BOOL BVar2;
    MotionBlur *pFVar3;
    u32 uVar4;

    iVar1 = ov106_021E5D08(param_1);
    if ((iVar1 == 0) && (BVar2 = IsPaletteFadeFinished(), BVar2 == 1)) {
        param_1->frame = 0;
        StopSE(0x868, 0);
        ov106_021E6064(param_1->capture);
        return 1;
    }
    iVar1 = param_1->frame;
    if (iVar1 == 0) {
        sub_02006118(0x868, 0);
    } else if (iVar1 == 0x73) {
        PlaySE(0x931);
    } else if (iVar1 == 0x91) {
        PlayCry(0xf9, 0);
        pFVar3 = ov106_021E601C(5, 0xd);
        param_1->capture = pFVar3;
    } else if (iVar1 == 0xe8) {
        ov106_021E597C(param_1, 6, 1, 1);
    }
    uVar4 = param_1->frame;
    if (uVar4 != 0) {
        if (uVar4 == 10) {
            GF_SetVolumeBySeqNo(0x868, 0x1e);
        } else if (uVar4 < 10) {
            GF_SetVolumeBySeqNo(0x868, uVar4 * 0x300 >> 8);
        }
    }
    param_1->frame = param_1->frame + 1;
    return 3;
}
