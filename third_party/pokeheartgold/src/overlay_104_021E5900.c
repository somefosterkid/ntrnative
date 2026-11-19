#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "heap.h"
#include "overlay_manager.h"
#include "screen_fade.h"
#include "overlay_104_ov104_app.h"
#include "overlay_104_105_args.h"

s32 ov104_021E5B88(void);

s32 ov104_021E5BEC(void *);

s32 ov104_021E5CC8(void *);

s32 ov104_021E5900(OverlayManager *man, s32 *state);

s32 ov104_021E5900(OverlayManager *man, s32 *state) {
    s32 var_r4;
    void *temp_r0;
    Ov104AppArgs *args;

    Heap_Create(HEAP_ID_3, HEAP_ID_149, 0x31000U);
    temp_r0 = OverlayManager_CreateAndGetData(man, sizeof(ov104_App), HEAP_ID_149);
    memset(temp_r0, 0, sizeof(ov104_App));
    args = OverlayManager_GetArgs(man);
    ((ov104_App *)temp_r0)->flagA = args->mode;
    ((ov104_App *)temp_r0)->flagB = args->gender;
    ((ov104_App *)temp_r0)->flagC = args->timeOfDay;
    *((s8 *)&((ov104_App *)temp_r0)->pad_c) = 0;
    *(Camera **)temp_r0 = Camera_New(HEAP_ID_149);
    ov104_021E5B88();
    ov104_021E5CC8(temp_r0);
    ov104_021E5BEC(temp_r0);
    var_r4 = GX_LIGHTID_0;
    do {
        NNS_G3dGlbLightVector((GXLightId)(var_r4), args->lights->lightVectors[var_r4].x, args->lights->lightVectors[var_r4].y, args->lights->lightVectors[var_r4].z);
        NNS_G3dGlbLightColor((GXLightId)(var_r4), args->lights->lightColors[var_r4]);
        var_r4 = (s32)(u8)(var_r4 + 1);
    } while ((u32)var_r4 < 4U);
    NNS_G3dGlbMaterialColorDiffAmb(args->lights->diffuse, args->lights->ambient, args->lights->setDiffuseColorAsVertexColor);
    NNS_G3dGlbMaterialColorSpecEmi(args->lights->specular, args->lights->emission, args->lights->enableSpecularReflectShininessTable);
    NNS_G3dGlbPolygonAttr(args->lights->lightMask, args->lights->polygonMode, args->lights->cullMode, args->lights->id, args->lights->alpha, args->lights->miscFlags);
    BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_149);
    return 1;
}
