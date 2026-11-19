#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "overlay_01_021FB878.h"
#include "sys_task.h"
#include "sys_task_api.h"

BOOL GF3dRender_BindModelSet(void *resource, NNSG3dResTex *texture);
BOOL GF3dRender_AllocAndLoadTexResources(NNSG3dResTex *texture);

void ov01_021FBD8C(SysTask *task, void *param) {
    Field3dModel *model = param;

    GF3dRender_AllocAndLoadTexResources(model->texture);
    GF3dRender_BindModelSet(model->dataRaw, model->texture);
    SysTask_Destroy(task);
}

void Field3dModel_Unload(Field3dModel *model) {
    NNSG3dTexKey texKey;
    NNSG3dTexKey tex4x4Key;
    NNSG3dPlttKey paletteKey;

    if (model->texture) {
        NNS_G3dTexReleaseTexKey(model->texture, &texKey, &tex4x4Key);
        NNS_GfdFreeTexVram(texKey);
        NNS_GfdFreeTexVram(tex4x4Key);

        paletteKey = NNS_G3dPlttReleasePlttKey(model->texture);
        NNS_GfdFreePlttVram(paletteKey);
    }

    if (model->dataRaw) {
        Heap_Free(model->dataRaw);
    }

    memset(model, 0, sizeof(Field3dModel));
}
