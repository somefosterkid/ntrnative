#include "global.h"
#include "overlay_01_021F1348.h"

#include "error_handling.h"
#include "gf_3d_loader.h"

enum HeapID ov01_021F1478(FieldEffectManager *fieldEffMan);
void *ov01_021F14B4(FieldEffectManager *fieldEffMan, u32 fileId, s32 allocAtEnd);

void ov01_021F1930(FieldEffectManager *fieldEffMan, u32 id, u32 fileId, BOOL shouldCopyWithoutTex);

void ov01_021F1930(FieldEffectManager *fieldEffMan, u32 id, u32 fileId, BOOL shouldCopyWithoutTex) {
    void *resource;
    struct UnkStruct_ov5_021DF8FC_t *v0;

    v0 = fieldEffMan->unk_20;
    resource = ov01_021F14B4(fieldEffMan, fileId, 1);
    if (GF3dGfxRawResMan_AllocObjAndKeys(v0->texResMan, resource, id, shouldCopyWithoutTex, ov01_021F1478(fieldEffMan)) == NULL) {
        GF_AssertFail();
    }
    ov01_021F197C(fieldEffMan, id, v0->texResMan);
}
