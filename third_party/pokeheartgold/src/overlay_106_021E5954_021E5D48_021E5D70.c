#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "gf_3d_render.h"
#include "gf_gfx_planes.h"
#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

void ov106_021E5D70(LegendaryCinematic *cinematic) {
    Camera *pCVar1;
    pCVar1 = Camera_New(HEAP_ID_153);
    cinematic->camera = pCVar1;
    GF3dRender_InitSimpleManager(HEAP_ID_153);
    reg_G3X_DISP3DCNT = reg_G3X_DISP3DCNT & 0xFFFFCFFF | 0x10;
    G3X_SetFog(0, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(0x1f, 0, 0x7fff, 0x3f, 0);
    NNS_G3dGlbMaterialColorDiffAmb(cinematic->args->unk4.diffuse, cinematic->args->unk4.ambient, cinematic->args->unk4.setDiffuseColorAsVertexColor);
    NNS_G3dGlbMaterialColorSpecEmi(cinematic->args->unk4.specular, cinematic->args->unk4.emission, cinematic->args->unk4.enableSpecularReflectShininessTable);
    NNS_G3dGlbPolygonAttr(cinematic->args->unk4.lightMask, cinematic->args->unk4.polygonMode, cinematic->args->unk4.cullMode, cinematic->args->unk4.id, cinematic->args->unk4.alpha, cinematic->args->unk4.miscFlags);
    SetBgPriority(0, 0);
    GfGfx_EngineATogglePlanes(1, 1);
    return;
}
