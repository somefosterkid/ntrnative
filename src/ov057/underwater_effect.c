// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov126/underwater_effect.c, the same code in
// Black 2/White 2's overlay 126 but for the archive, which is 0xbe in Black. The names are Black's dsd names and swan's;
// pokebw2's guesses are in the comments.
#include "types.h"
#include "field/field.h"
#include "field/underwater_effect.h"
#include "gfl/arc.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"

struct UnderwaterEffectWork {
    void *resources[7];
    G3DModel *models[3];
    void *animations[4];
    G3DActor *actors[3];
};

static const u32 sModelResourceIds[3] = { 0, 1, 2 };
static const u32 sAnimationModelIds[4] = { 0, 1, 2, 0 };
static const u32 sAnimationResourceIds[4] = { 3, 4, 5, 6 };
static const u32 sArchiveFileIds[7] = {
#include "asset/underwater_effect_sArchiveFileIds.inc"
};
static const SRTMatrix sUnderwaterMatrix = {
#include "asset/underwater_effect_sUnderwaterMatrix.inc"
};

static void func_ov057_021f36c0(FieldAsyncProc *proc, Field *field, void *data);
static void func_ov057_021f37bc(FieldAsyncProc *proc, Field *field, void *data);
static void func_ov057_021f381c(FieldAsyncProc *proc, Field *field, void *data);
static void func_ov057_021f3858(FieldAsyncProc *proc, Field *field, void *data);

// pokebw2's UNDERWATER_EFFECT_PROC
const FieldAsyncProcDef data_ov057_021f3900 = {
    0x80,
    sizeof(UnderwaterEffectWork),
    func_ov057_021f36c0,
    func_ov057_021f37bc,
    func_ov057_021f381c,
    func_ov057_021f3858,
};

// pokebw2's UnderwaterEffect_Init
static void func_ov057_021f36c0(FieldAsyncProc *proc, Field *field, void *data) {
    UnderwaterEffectWork *work = data;
    HeapID heapId = Field_GetHeapID(field);
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0xbe, HEAPID_TAIL(heapId));
    int i;

    for (i = 0; i < 7; i++) {
        work->resources[i] = GFL_G3DSysReadArcToolResource(arc, sArchiveFileIds[i]);
        if (GFL_G3DResCheckType(work->resources[i], G3D_RES_CHECK_TEX)) {
            GFL_G3DResUploadTexData(work->resources[i]);
        }
    }
    for (i = 0; i < 3; i++) {
        u32 index = sModelResourceIds[i];
        if (GFL_G3DResCheckType(work->resources[index], G3D_RES_CHECK_TEX)) {
            work->models[i] = GFL_G3DMdlCreate(work->resources[index], 0, work->resources[index]);
        } else {
            work->models[i] = GFL_G3DMdlCreate(work->resources[index], 0, NULL);
        }
    }
    for (i = 0; i < 4; i++) {
        work->animations[i] =
            GFL_G3DAnmCreate(work->models[sAnimationModelIds[i]], work->resources[sAnimationResourceIds[i]], 0);
    }
    for (i = 0; i < 3; i++) {
        work->actors[i] = GFL_G3DActorCreate(work->models[i], work->animations, 4);
    }
    GFL_G3DActorBindAnm(work->actors[0], 0);
    GFL_G3DActorBindAnm(work->actors[0], 3);
    GFL_G3DActorBindAnm(work->actors[1], 1);
    GFL_G3DActorBindAnm(work->actors[2], 2);
    GFL_G3DSysSetSwapBufferParams(1, 1);
    GFL_ArcToolFree(arc);
}

// pokebw2's UnderwaterEffect_Free
static void func_ov057_021f37bc(FieldAsyncProc *proc, Field *field, void *data) {
    UnderwaterEffectWork *work = data;
    int i;
    for (i = 0; i < 3; i++) {
        GFL_G3DActorFree(work->actors[i]);
    }
    for (i = 0; i < 4; i++) {
        GFL_G3DAnmFree(work->animations[i]);
    }
    for (i = 0; i < 3; i++) {
        GFL_G3DMdlFree(work->models[i]);
    }
    for (i = 0; i < 7; i++) {
        if (GFL_G3DResCheckType(work->resources[i], G3D_RES_CHECK_TEX)) {
            GFL_G3DResFreeTexData(work->resources[i]);
        }
        GFL_G3DResFree(work->resources[i]);
    }
}

// pokebw2's UnderwaterEffect_Update
static void func_ov057_021f381c(FieldAsyncProc *proc, Field *field, void *data) {
    UnderwaterEffectWork *work = data;
    GFL_G3DActorStepAnmFrameLoop(work->actors[0], 0, 0x1000);
    GFL_G3DActorStepAnmFrameLoop(work->actors[0], 3, 0x1000);
    GFL_G3DActorStepAnmFrameLoop(work->actors[1], 1, 0x1000);
    GFL_G3DActorStepAnmFrameLoop(work->actors[2], 2, 0x1000);
    GFL_G3DSysSetSwapBufferParams(1, 1);
}

// pokebw2's UnderwaterEffect_Draw
static void func_ov057_021f3858(FieldAsyncProc *proc, Field *field, void *data) {
    UnderwaterEffectWork *work = data;
    GFL_G3DSysDrawObj(work->actors[0], &sUnderwaterMatrix);
    GFL_G3DSysDrawObj(work->actors[1], &sUnderwaterMatrix);
    GFL_G3DSysDrawObj(work->actors[2], &sUnderwaterMatrix);
}
