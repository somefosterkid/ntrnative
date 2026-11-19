#include "global.h"

#include "camera.h"
#include "gf_3d_render.h"
#include "unk_02026E30.h"
#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

extern const VecFx32 _021E6D70;
extern const MtxFx33 ov106_021E6D8C;

void ov106_021E5C30(LegendaryCinematic *cinematic) {
    MtxFx33 sp1C;
    VecFx32 sp10;
    VecFx32 sp4;
    LegendaryCinematic *sp0;
    u32 var_r7;
    LegendaryCinematicModel *var_r4_2;
    u32 var_r2_2;
    NNSG3dAnmObj *temp_r1_2;
    s32 temp_r0_2;
    sp0 = cinematic;
    sp1C = ov106_021E6D8C;
    sp10 = _021E6D70;
#if defined(SDK_PORT) || defined(NONMATCHING)
    sp4.x = 0;
    sp4.y = 0;
    sp4.z = 0;
#else
    temp_r1_2 = (NNSG3dAnmObj *)&sp4;
    ((VecFx32 *)temp_r1_2)->x = 0;
    ((VecFx32 *)temp_r1_2)->y = 0;
    ((VecFx32 *)temp_r1_2)->z = 0;
#endif
    Thunk_G3X_Reset();
    Camera_PushLookAtToNNSGlb();
    var_r7 = 0;
    if (var_r7 < sp0->numModels) {
        var_r4_2 = &sp0->models[0];
        do {
            var_r2_2 = 0;
            for (; var_r2_2 < 4U; var_r2_2++) {
                temp_r1_2 = var_r4_2->anmObj[var_r2_2];
                if (temp_r1_2 != NULL) {
                    temp_r0_2 = temp_r1_2->frame + 0x1000;
                    if (temp_r0_2 < NNS_G3dAnmObjGetNumFrame(temp_r1_2)) {
                        temp_r1_2->frame = temp_r0_2;
                    }
                }
            }
            GF3dRender_DrawModel(&var_r4_2->renderObj, &sp4, &sp1C, &sp10);
            var_r7 += 1;
            var_r4_2++;
        } while (var_r7 < sp0->numModels);
    }
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}
