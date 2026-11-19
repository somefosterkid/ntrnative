#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "filesystem.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "options.h"
#include "pm_string.h"
#include "pokepic.h"
#include "unk_0201F990.h"

struct Camera;

struct CameraAngle;

struct UnkOv71Model;

struct UnkOv71Rotation;

struct UnkOv71Scene;

typedef struct UnkOv71Rotation {
    u16 x;
    u16 y;
    u16 z;
} UnkOv71Rotation;

typedef struct UnkOv71Model {
    void *unk0;
    NNSG3dRenderObj unk4;
    NNSG3dResMdlSet *unk58;
    NNSG3dResMdl *unk5C;
    NNSG3dResTex *unk60;
    BOOL unk64;
    VecFx32 unk68;
    VecFx32 unk74;
    UnkOv71Rotation unk80;
    int unk88;
} UnkOv71Model;

typedef struct UnkOv71Scene {
    Camera *unk0;
    VecFx32 unk4;
    CameraAngle unk10;
    UnkOv71Model *unk18;
    u32 unk1C;
} UnkOv71Scene;

UnkOv71Model *ov71_02247610(UnkOv71Scene *scene, int index, NarcId narcId, u32 memberIndex, fx32 x, fx32 y, fx32 z, BOOL enabled);

UnkOv71Model *ov71_02247610(UnkOv71Scene *scene, int index, NarcId narcId, u32 memberIndex, fx32 x, fx32 y, fx32 z, BOOL enabled) {
    UnkOv71Model *model = &scene->unk18[index];
    NNSG3dResMdlSet *modelSet;
    NNSG3dResTex *texture;

    model->unk0 = GfGfxLoader_LoadFromNarc(narcId, memberIndex, FALSE, HEAP_ID_57, TRUE);
    DC_FlushRange(model->unk0, GetNarcMemberSizeByIdPair(narcId, memberIndex));
    if (model->unk0 != NULL) {
        modelSet = NNS_G3dGetMdlSet(model->unk0);
        model->unk58 = modelSet;
        model->unk5C = NNS_G3dGetMdlByIdx(modelSet, 0);
        texture = NNS_G3dGetTex(model->unk0);
        model->unk60 = texture;
        GF3dRender_AllocAndLoadTexResources(texture);
        GF3dRender_BindModelSet(model->unk0, model->unk60);
        NNS_G3dRenderObjInit(&model->unk4, model->unk5C);
        model->unk68.x = x;
        model->unk68.y = y;
        model->unk68.z = z;
        model->unk88 = 31;
        model->unk64 = enabled;
    }
    return model;
}
