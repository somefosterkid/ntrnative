#include "global.h"
struct UnkStruct_ov5_021ED0A4_t;

#include "error_handling.h"
#include "gf_3d_loader.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

s32 ov01_021F9D5C(void *, s32);


s32 ov01_021FA668(void *, s32);

void ov01_021FA61C(struct UnkStruct_ov5_021ED0A4_t *arg0, s32 arg1, void *arg2, s32 arg3);

void ov01_021FA61C(UnkStruct_ov5_021ED0A4_t *arg0, s32 id, void *resource, s32 arg3) {
    if (GF3dGfxRawResMan_AllocObjAndKeys(*((GF_3DGfxRawResMan * *)&arg0->unk_F0), resource, id, 1, HEAP_ID_FIELD1) == NULL) {
        GF_AssertFail();
    }
    ov01_021FA668(arg0, id);
    if (arg3 == 0) {
        ov01_021F9D5C(arg0, id);
        return;
    }
    ov01_021F9DD0(arg0, id);
}
