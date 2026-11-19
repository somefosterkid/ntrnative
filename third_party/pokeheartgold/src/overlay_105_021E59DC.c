#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "gf_3d_render.h"
#include "overlay_manager.h"
#include "screen_fade.h"
#include "unk_02005D10.h"
#include "unk_02026E30.h"
#include "unk_0208805C.h"
#include "overlay_105_unk_struct_ov105_021e5900.h"

s32 ov105_021E5BCC(void *);

extern const VecFx32 ov105_021E5DD4;

extern const MtxFx33 ov105_021E5E08;

s32 ov105_021E59DC(OverlayManager *man, s32 *state);

s32 ov105_021E59DC(OverlayManager *man, s32 *state) {

    UnkStruct_ov105_Model *temp_r4;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 var_r7;
    u8 var_r2;
    NNSG3dAnmObj *temp_r1;
    NNSG3dAnmObj *temp_r1_2;
    UnkStruct_ov105_021E5900 *temp_r5;

    temp_r5 = OverlayManager_GetData(man);
    temp_r0 = *state;
    var_r7 = 0;
    temp_r4 = &temp_r5->models[temp_r5->unk9f];
    switch (temp_r0) {
    case 0:
        temp_r1 = temp_r4->animObjs[0];
        if ((temp_r1->frame + 0x1000) == (((NNSG3dResAnmCommon *)temp_r1->resAnm)->numFrame << 0xC)) {
            App_StartScreenFade(1U, HEAP_ID_151);
            *state += 1;
        }
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            temp_r5->unk9f = (u8)(temp_r5->unk9f + 1);
            temp_r5->unkA0 = (u8)(temp_r5->unkA0 + 1);
            if ((u32) temp_r5->unkA0 >= 1U) {
                var_r7 = 1;
            } else {
                ov105_021E5BCC(temp_r5);
                App_StartScreenFade(0U, HEAP_ID_151);
                *state = 0;
            }
        }
        break;
    }
    temp_r5->unkA1 = (u8)(temp_r5->unkA1 + 1);
    if (temp_r5->unkA1 == 0x1E) {
        PlaySE(temp_r5->unkA4[temp_r5->unk9f]);
    }
    var_r2 = 0;
    do {
        temp_r1_2 = temp_r4->animObjs[var_r2];
        temp_r0_2 = temp_r1_2->frame + 0x1000;
        if (temp_r0_2 < (s32)(((NNSG3dResAnmCommon *)temp_r1_2->resAnm)->numFrame << 0xC)) {
            temp_r1_2->frame = temp_r0_2;
        }
        var_r2 += 1;
    } while ((u32)var_r2 < 4U);
    {
        MtxFx33 sp18 = ov105_021E5E08;
        VecFx32 spC = ov105_021E5DD4;
        VecFx32 sp0 = { 0, 0, 0 };

        Thunk_G3X_Reset();
        Camera_PushLookAtToNNSGlb();
        GF3dRender_DrawModel(&temp_r4->renderObj, &sp0, &sp18, &spC);
        RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_W);
    }
    return var_r7;
}
