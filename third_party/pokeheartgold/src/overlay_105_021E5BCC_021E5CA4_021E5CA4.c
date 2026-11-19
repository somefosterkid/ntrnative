#include "global.h"

#include "constants/heap.h"

#include "field/model_attributes.h"

#include "camera.h"
#include "filesystem.h"
#include "gf_3d_render.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_105.h"
#include "overlay_105_unk_struct_ov105_021e5900.h"
#include "overlay_manager.h"
#include "screen_fade.h"
#include "system.h"
#include "unk_02005D10.h"
#include "unk_02026E30.h"
#include "unk_0208805C.h"

struct Camera;

struct CameraAngle;

struct CameraHistory;

struct CameraLookAt;

struct CameraPerspective;

struct NARC;

struct UnkStruct_ov105_Ids;

typedef struct UnkStruct_ov105_Ids {
    u32 modelId;
    u16 animIds[4];
} UnkStruct_ov105_Ids;

extern const u32 _021E5DC4[];

extern const u32 ov105_021E5DC8[];

extern const UnkStruct_ov105_Ids _021E5E80;

extern const UnkStruct_ov105_Ids ov105_021E5E8C;

extern const UnkStruct_ov105_Ids ov105_021E5E98;

extern const UnkStruct_ov105_Ids ov105_021E5EA4;

void ov105_021E5CA4(UnkStruct_ov105_021E5900 *data);

void ov105_021E5CA4(UnkStruct_ov105_021E5900 *data) {
    NNSG3dResTex *tex;
    NARC *narc;
    u8 i;
    const UnkStruct_ov105_Ids *ids;
    void *anim;
    UnkStruct_ov105_Model *model;

    narc = NARC_New((NarcId)(0xf0), HEAP_ID_151);
    HeapExp_FndInitAllocator(&data->allocator, HEAP_ID_151, 4);
    if (data->unk9c == 0) {
        if (data->unk9d == 0) {
            ids = &ov105_021E5E8C;
        } else {
            ids = &_021E5E80;
        }
        data->unkA4 = ov105_021E5DC8;
    } else {
        if (data->unk9d == 0) {
            ids = &ov105_021E5EA4;
        } else {
            ids = &ov105_021E5E98;
        }
        data->unkA4 = _021E5DC4;
    }
    model = &data->models[0];
    model->header = NARC_AllocAndReadWholeMember(narc, ids->modelId, HEAP_ID_151);
    GF3dRender_InitObjFromHeader(&model->renderObj, &model->model, &model->header);
    tex = NNS_G3dGetTex(data->models[0].header);
    NNS_G3dMdlUseGlbDiff(model->model);
    NNS_G3dMdlUseGlbAmb(model->model);
    NNS_G3dMdlUseGlbSpec(model->model);
    NNS_G3dMdlUseGlbEmi(model->model);
    for (i = 0; i < 4; i++) {
        model->animFiles[i] = NARC_AllocAndReadWholeMember(narc, ids->animIds[i], HEAP_ID_151);
        anim = NNS_G3dGetAnmByIdx(model->animFiles[i], 0);
        model->animObjs[i] = NNS_G3dAllocAnmObj(&data->allocator, anim, model->model);
        NNS_G3dAnmObjInit(model->animObjs[i], anim, model->model, tex);
        NNS_G3dRenderObjAddAnmObj(&model->renderObj, model->animObjs[i]);
    }
    NARC_Delete(narc);
}
