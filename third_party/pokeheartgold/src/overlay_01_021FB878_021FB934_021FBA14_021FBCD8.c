#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "sys_task.h"
#include "sys_task_api.h"

void ov01_021FBD8C(SysTask *task, void *data);

void Field3dModel_LoadFromFilesystem(Field3dModel *model, enum NarcId narcId, s32 fileId, enum HeapID heapID) {
    unsigned int new_var;
    NNSG3dResDict *temp_r2;
    NNSG3dResFileHeader *temp_r0;
    NNSG3dResMdlSet *temp_r0_2;
    NNSG3dResTex *temp_r0_3;
    s32 *var_r1;
    s8 *var_r0;
    temp_r0 = GfGfxLoader_LoadFromNarc(narcId, fileId, 0, heapID, 0);
    model->dataRaw = temp_r0;
    new_var = 0U;
    temp_r0_2 = NNS_G3dGetMdlSet(temp_r0);
    model->modelSet = temp_r0_2;
    if (temp_r0_2 != 0L) {
        temp_r2 = &temp_r0_2->dict;
        if ((temp_r2 != 0L) && (((u32)temp_r0_2->dict.numEntry) > new_var)) {
            var_r1 = (s32 *)((((u8 *)temp_r2) + temp_r0_2->dict.ofsEntry) + 4);
        } else {
            var_r1 = 0L;
        }
        if (var_r1 != 0L) {
            var_r0 = &temp_r0_2->header.chr[*var_r1];
        } else {
            goto block_7;
        }
    } else {
block_7:
        var_r0 = 0L;
    }
    model->model = (NNSG3dResMdl *)var_r0;
    temp_r0_3 = NNS_G3dGetTex((NNSG3dResFileHeader *)model->dataRaw);
    model->texture = temp_r0_3;
    if (temp_r0_3 != 0L) {
        SysTask_CreateOnVWaitQueue(ov01_021FBD8C, model, 0x400U);
    }
}
