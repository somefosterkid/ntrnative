#include "global.h"
#include "camera.h"
#include "overlay_68_move_relearner.h"
#include "pokepic.h"
#include "unk_02026E30.h"

void ov68_021E734C(MoveRelearnerMonPic *monPic);

void ov68_021E734C(MoveRelearnerMonPic *monPic) {
    Thunk_G3X_Reset();
    Camera_PushLookAtToNNSGlb();

    G3_MtxMode(GX_MTXMODE_PROJECTION);
    G3_Identity();
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_Identity();

    NNS_G3dGlbFlush();
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_DrawAll(monPic->pokepicManager);

    G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}
