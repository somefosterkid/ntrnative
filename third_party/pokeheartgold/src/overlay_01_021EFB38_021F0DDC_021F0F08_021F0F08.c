#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"

#include "camera.h"
#include "error_handling.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_01_021EFB38.h"
#include "overlay_01_unk_struct_ov5_02202120.h"
#include "unk_02014DA0.h"

u32 ov01_021F1224(u32 param0, BOOL param1);

u32 ov01_021F1250(u32 param0, BOOL param1);

void ov01_021F0F08(NARC *param0, u32 param1) {
    void *pvVar1;
    ParticleSystem *pSVar2;
    Camera *camera;

    if (ov01_02209B64 == NULL) {
        GF_AssertFail();
    }
    if (ov01_02209B64->unk_08 != NULL) {
        GF_AssertFail();
    }
    pvVar1 = Heap_Alloc(HEAP_ID_FIELD1, 0x4800);
    ov01_02209B64->unk_0C = pvVar1;
    pSVar2 = sub_02014DB4(ov01_021F1224, ov01_021F1250, ov01_02209B64->unk_0C, 0x4800, 1, HEAP_ID_FIELD1);
    ov01_02209B64->unk_08 = pSVar2;
    if (ov01_02209B64->unk_08 == NULL) {
        GF_AssertFail();
    }
    sub_02015528(ov01_02209B64->unk_08, 1);
    camera = sub_02015524(ov01_02209B64->unk_08);
    Camera_SetPerspectiveClippingPlane(FX32_ONE, FX32_CONST(900), camera);
    pvVar1 = GfGfxLoader_LoadFromOpenNarc(param0, param1, 0, HEAP_ID_FIELD1, 0);
    sub_0201526C(ov01_02209B64->unk_08, pvVar1, 0, 0);
    return;
}
