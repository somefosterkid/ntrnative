#include "global.h"
#include "pokepic.h"
#include "unk_02026E30.h"

void ov71_0224B234(void *arg0);

extern s32 (*ov71_0224BF84[4])(void *, void *);

BOOL ov71_0224B1FC(void *param, s32 *state);

s32 ov71_022474CC();

BOOL ov71_0224B1FC(void *param, s32 *state) {
    s32 iVar1;

    if ((u32)*state < 4) {
        iVar1 = ov71_0224BF84[*state](param, (u8 *)param + 4);
        if (iVar1 != 0) {
            *state = *state + 1;
            *(u32 *)((u8 *)param + 4) = 0;
        }
        ov71_0224B234(param);
        return 0;
    }
    return 1;
}

void ov71_0224B234(void *arg0) {
    u32 args;

    G3X_Reset();
    NNS_G3dGeBufferOP_N(0x11U, NULL, 0U);
    NNS_G3dGeFlushBuffer();
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_HandleLoadImgAndOrPltt(*(PokepicManager **)((u8 *)arg0 + 0x10));
    PokepicManager_DrawAll(*(PokepicManager **)((u8 *)arg0 + 0x10));
    args = 1;
    NNS_G3dGeBufferOP_N(0x12U, &args, 1U);
    if (*(s32 *)((u8 *)arg0 + 0x74) != 0) {
        ov71_022474CC();
    }
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}
