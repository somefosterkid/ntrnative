#include "global.h"

#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "sys_task_api.h"
#include "field/overlay_01_021FB878.h"

s32 sub_02018324(void **);

void sub_02018030(void **arg0, NARC *arg1, s32 arg2, enum HeapID arg3);

void sub_02018030(void **arg0, NARC *narc, s32 fileId, enum HeapID heapID) {
    *arg0 = GfGfxLoader_LoadFromOpenNarc(narc, fileId, 0, heapID, 0);
    sub_02018324(arg0);
}

void sub_0201804C(SysTask *arg0, void *arg1);

void sub_0201804C(SysTask *task, void *arg1) {
    GF3dRender_AllocAndLoadTexResources(((Field3dModel *)arg1)->texture);
    GF3dRender_BindModelSet(*(NNSG3dResFileHeader **)arg1, ((Field3dModel *)arg1)->texture);
    SysTask_Destroy(task);
}
