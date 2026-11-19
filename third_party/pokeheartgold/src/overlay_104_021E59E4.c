#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "gf_3d_render.h"
#include "overlay_manager.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "unk_02005D10.h"
#include "unk_02026E30.h"
#include "unk_0208805C.h"
#include "overlay_104_ov104_app.h"

s32 ov104_021E5BEC(void *);

s32 ov104_021E5E78(NNSG3dRenderObj *);

s32 ov104_021E5EB0(s32, s32);

extern VecFx32 ov104_021E5EFC;

extern MtxFx33 ov104_021E5F3C;

s32 ov104_021E59E4(OverlayManager *man, s32 *state);

s32 ov104_021E59E4(OverlayManager *man, s32 *state) {
    MtxFx33 sp18;
    VecFx32 spC;
    VecFx32 sp0;
    NNSG3dRenderObj *temp_r4;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 var_r7;
    u8 var_r2;
    void *temp_r1;
    void *temp_r5;

    temp_r5 = OverlayManager_GetData(man);
    temp_r4 = temp_r5 + 4 + (0x6C * *((u8 *)&((ov104_App *)temp_r5)->pad_d[0]));
    temp_r0 = *state;
    var_r7 = 0;
    switch (temp_r0) {
    case 0:
        if (ov104_021E5E78(temp_r4) != 0) {
            ov104_021E5EB0(*((u8 *)&((ov104_App *)temp_r5)->pad_d[0]), 0x95);
            *state += 1;
        }
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            *((u8 *)&((ov104_App *)temp_r5)->pad_d[0]) = (u8)(*((u8 *)&((ov104_App *)temp_r5)->pad_d[0]) + 1);
            *((u8 *)&((ov104_App *)temp_r5)->pad_d[1]) = (u8)(*((u8 *)&((ov104_App *)temp_r5)->pad_d[1]) + 1);
            if ((u32) *((u8 *)&((ov104_App *)temp_r5)->pad_d[1]) >= 3U) {
                var_r7 = 1;
            } else {
                ov104_021E5BEC(temp_r5);
                App_StartScreenFade(0U, HEAP_ID_149);
                *state = 0;
            }
        }
        break;
    }
    if (**(s32 **)((u8 *)temp_r4 + 0x64) == 0) {
        PlaySE((u16) * (s32 *)(*((u8 **)&((ov104_App *)temp_r5)->table) + (*((u8 *)&((ov104_App *)temp_r5)->pad_d[0]) * 4)));
        if (*((u8 *)&((ov104_App *)temp_r5)->pad_d[0]) == 2) {
            Sound_SetSceneAndPlayBGM(0x44U, 0x447U, 1);
        }
    }
    var_r2 = 0;
    do {
        temp_r1 = *(void **)((u8 *)(((u8 *)temp_r4) + (var_r2 * 4)) + 0x64);
        temp_r0_2 = *(s32 *)temp_r1 + 0x1000;
        if (temp_r0_2 < (s32)(*(u16 *)((u8 *)(*(void **)((u8 *)temp_r1 + 8)) + 4) << 0xC)) {
            *(s32 *)temp_r1 = temp_r0_2;
        }
        var_r2 += 1;
    } while ((u32)var_r2 < 2U);
    sp18 = ov104_021E5F3C;
    spC = ov104_021E5EFC;
    temp_r1 = &sp0;
    ((VecFx32 *)temp_r1)->x = 0;
    ((VecFx32 *)temp_r1)->y = 0;
    ((VecFx32 *)temp_r1)->z = 0;
    Thunk_G3X_Reset();
    Camera_PushLookAtToNNSGlb();
    GF3dRender_DrawModel(temp_r4, &sp0, &sp18, &spC);
    RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_W);
    return var_r7;
}
