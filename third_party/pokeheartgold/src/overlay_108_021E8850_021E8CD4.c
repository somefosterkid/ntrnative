#include "global.h"

#include "camera.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"
#include "unk_02026E30.h"

void ov108_021E8CD4(SafariDecoration3dView *view, u8 timeOfDay, enum HeapID heapID) {
    u32 v0;
    u32 v1;

    MI_CpuFill8(view, 0U, sizeof(SafariDecoration3dView));
    NNS_G3dInit();
    G3X_Init();
    G3X_InitMtxStack();
    reg_G3X_DISP3DCNT &= 0xFFFFCFFD;
    reg_G3X_DISP3DCNT &= 0xCFFB;
    reg_G3X_DISP3DCNT = 8 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    reg_G3X_DISP3DCNT = 0x10 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    reg_G3X_DISP3DCNT = 0x20 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    G3X_SetFog(0, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(0U, 0, 0x7FFF, 0x3F, 0);
    reg_G3_SWAP_BUFFERS = 2;
    reg_G3_VIEWPORT = 0xBFFF0000;
    view->heapID = heapID;
    view->unk_F1 = timeOfDay;
    v0 = NNS_GfdGetLnkTexVramManagerWorkSize(0x80U);
    view->texVramMan = Heap_Alloc(view->heapID, v0);
    GF_3DVramMan_InitLinkedListTexVramManager(0x20000U, 0U, view->texVramMan, v0, 1);
    v1 = NNS_GfdGetLnkPlttVramManagerWorkSize(0x100U);
    view->plttVramMan = Heap_Alloc(view->heapID, v1);
    GF_3DVramMan_InitLinkedListPlttVramManager(0x4000U, view->plttVramMan, v1, 1);
    HeapExp_FndInitAllocator(&view->allocator, heapID, 0x20);
    view->narcModels = NARC_New(NARC_a_0_4_0, heapID);
    view->narcAnimTable = NARC_New(NARC_a_1_0_7, heapID);
    view->narcAnims = NARC_New(NARC_a_1_0_6, heapID);
    ov108_021E9230(view, heapID);
    GfGfx_EngineATogglePlanes(1U, 1U);
    reg_G2_BG0CNT = 1 | (reg_G2_BG0CNT & ~3);
}

void ov108_021E8E10(SafariDecoration3dView *view) {
    Thunk_G3X_Reset();
    Camera_PushLookAtToNNSGlb();
    NNS_G3dGeFlushBuffer();
    reg_G3_MTX_MODE = 0;
    reg_G3_MTX_IDENTITY = 0;
    reg_G3_MTX_MODE = 2;
    reg_G3_MTX_IDENTITY = 0;
    if ((view->enabled != 0) && (view->model.loaded != 0)) {
        ov108_021E9144(&view->model);
    }
    reg_G3_SWAP_BUFFERS = 1;
}

void ov108_021E8E60(SafariDecoration3dView *view) {
    if (view->model.loaded != 0) {
        ov108_021E90C4(view, &view->model);
    }
    Camera_Delete(view->camera);
    Heap_Free(view->plttVramMan);
    Heap_Free(view->texVramMan);
    NARC_Delete(view->narcAnims);
    NARC_Delete(view->narcAnimTable);
    NARC_Delete(view->narcModels);
}

void ov108_021E8EA4(SafariDecoration3dView *view, s32 id, const SafariObjectConfig *config) {
    if (view->model.loaded != 0) {
        ov108_021E90C4(view, &view->model);
    }
    view->enabled = 1;
    if (config != NULL) {
        ov108_021E8F00(view, id, config);
    }
}

void ov108_021E8ED8(SafariDecoration3dView *view, u8 enabled) {
    view->enabled = enabled;
    if (view->enabled == 0) {
        Thunk_G3X_Reset();
        Camera_PushLookAtToNNSGlb();
        NNS_G3dGeFlushBuffer();
        reg_G3_SWAP_BUFFERS = 1;
    }
}
