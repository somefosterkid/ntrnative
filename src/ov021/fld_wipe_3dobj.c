// Black's code at ov021 0x021c8018-0x021c812c. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_flash.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx_vec.h"

const MtxFx43 *NNS_G3dGlbGetInvV(void);
void NNS_G3dMdlSetMdlLightEnableFlag(NNSG3dResMdl * pMdl, u32 matID, int light);
void NNS_G3dMdlSetMdlPolygonID(NNSG3dResMdl * pMdl, u32 matID, int polygonID);
void NNS_G3dMdlSetMdlCullMode(NNSG3dResMdl * pMdl, u32 matID, GXCull cullMode);
void NNS_G3dMdlSetMdlAlpha(NNSG3dResMdl * pMdl, u32 matID, int alpha);
void NNS_G3dMdlSetMdlPolygonMode(NNSG3dResMdl * pMdl, u32 matID, GXPolygonMode polyMode);

// pokebw2's FieldFlashG3D (field_flash.h names it): the darkness sphere's resource and actor, Black's layout the
// same. field_flash.c declares the create as returning a u32 *
struct FieldFlashG3D {
    void *resource;
    G3DActor *actor;
};

u32 *FieldFlashG3D_Create(HeapID param_1) {
    FieldFlashG3D *origResource;
    void *resource;
    G3DModel *model;
    G3DActor *pGVar2;

    origResource = GFL_HeapAllocate(param_1, sizeof(FieldFlashG3D), 1, "fld_wipe_3dobj.c", 0x24);
    resource = GFL_G3DSysReadArcSysResource(0x4a, 0);
    origResource->resource = resource;
    model = GFL_G3DMdlCreate(resource, 0, (void *)0x0);
    pGVar2 = GFL_G3DActorCreate(model, (void **)0x0, 0);
    origResource->actor = pGVar2;
    return (u32 *)origResource;
}

void FieldFlashG3D_Free(FieldFlashG3D *flash) {
    G3DModel *model;

    model = GFL_G3DActorGetMdl(flash->actor);
    GFL_G3DMdlFree(model);
    GFL_G3DActorFree(flash->actor);
    GFL_G3DResFree(flash->resource);
    GFL_HeapFree(flash);
}

void FieldFlashG3D_Draw(FieldFlashG3D *flash, fx32 scale, u32 alpha) {
    SRTMatrix transform;
    FxLookAt lookAt;
    VecFx32 dir;
    NNSG3dResMdl *mdl;
    fx32 height;

    if (scale != 0) {
        GFL_G3DSysMtxGetViewLookAt(&lookAt);
        transform.translation = lookAt.position;
        VEC_Subtract(&lookAt.target, &lookAt.position, &dir);
        VEC_Normalize(&dir, &dir);
        height = dir.y > 0 ? dir.y : -dir.y;
        transform.scale.x = scale;
        transform.scale.y = FX_Mul(scale, height);
        transform.scale.z = FX32_CONST(4);
        MI_Copy36B(NNS_G3dGlbGetInvV(), &transform.rotation);
        mdl = GFL_G3DMdlGetEngineModel(GFL_G3DActorGetMdl(flash->actor))->resMdl;
        NNS_G3dMdlSetMdlLightEnableFlag(mdl, 0, 0);
        NNS_G3dMdlSetMdlPolygonID(mdl, 0, 0);
        NNS_G3dMdlSetMdlCullMode(mdl, 0, 2);
        NNS_G3dMdlSetMdlAlpha(mdl, 0, alpha);
        NNS_G3dMdlSetMdlPolygonMode(mdl, 0, 0);
        GFL_G3DSysDrawObj(flash->actor, &transform);
    }
}

