#include "global.h"
struct TradeTubePhase;

#include "pokepic.h"
#include "unk_02026E30.h"
#include "overlay_71_trade_tube_phase.h"

s32 ov71_022474CC(void *);

void ov71_022479C8(struct TradeTubePhase *arg0);

void ov71_022479C8(TradeTubePhase *arg0) {
    u32 args;

    G3X_Reset();
    NNS_G3dGeBufferOP_N(0x11U, NULL, 0U);
    NNS_G3dGeFlushBuffer();
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_HandleLoadImgAndOrPltt(*((PokepicManager * *)&arg0->animResources.cellBuffer));
    PokepicManager_DrawAll(*((PokepicManager * *)&arg0->animResources.cellBuffer));
    args = 1;
    NNS_G3dGeBufferOP_N(0x12U, &args, 1U);
    ov71_022474CC(*((void **)&arg0->sprites[10][1]));
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}
