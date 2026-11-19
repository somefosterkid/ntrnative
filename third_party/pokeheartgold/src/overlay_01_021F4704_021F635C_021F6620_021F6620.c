#include "global.h"

#include "field/overlay_01_02204004.h"

#include "error_handling.h"
#include "filesystem.h"
#include "gf_3d_render.h"
#include "heap.h"
#include "overlay_01_021F4704.h"
#include "sys_task.h"
#include "sys_task_api.h"

s32 sub_02054E20(s32);

void ov01_021F6620(SysTask *sysTask, void *sysTaskParam) {
    MapModelLoadTask *task = sysTaskParam;
    u32 idx = 0;
    s32 var_r6;
    s32 var_r5;
    s32 var_r5_2;
    s8 *var_r5_3;
    s32 temp_r2;
    NNSG3dResDict *temp_r1;
    u8 var_r0;
    u8 temp_r0;
    s32 *var_r0_2;
    NNSG3dResTex *temp_r0_2;
    NNSG3dResMdlSet *temp_r0_3;

    if (task->cancel == 1) {
        task->state = 5U;
    }
    temp_r0 = task->state;
    switch ((u32)temp_r0) {
    case 0:
        task->bytesRead = 0;
        var_r5 = task->modelSize;
        if (var_r5 <= 0xE000) {
            var_r0 = 2;
        } else {
            var_r5 = 0xE000;
            var_r0 = 1;
        }
        task->state = var_r0;
        NARC_ReadFile(task->narc, (u32)var_r5, (u8 *)*task->modelFile + task->bytesRead);
        task->bytesRead = (s32)(task->bytesRead + var_r5);
        return;
    case 1:
        temp_r2 = task->bytesRead;
        var_r5_2 = task->modelSize - temp_r2;
        if (var_r5_2 > 0xE000) {
            var_r5_2 = 0xE000;
            var_r6 = 0;
        } else {
            var_r6 = 1;
        }
        NARC_ReadFile(task->narc, (u32)var_r5_2, (u8 *)*task->modelFile + temp_r2);
        if (var_r6 != 0) {
            task->state = 2U;
            return;
        }
        task->bytesRead = (s32)(task->bytesRead + var_r5_2);
        return;
    case 2:
        temp_r0_2 = task->texture;
        if ((temp_r0_2 != NULL) && (GF3dRender_ResTexIsLoaded(temp_r0_2) == 1)) {
            GF3dRender_BindModelSet(*task->modelFile, task->texture);
        }
        task->state = 3U;
        return;
    case 3:
        temp_r0_3 = NNS_G3dGetMdlSet(*task->modelFile);
        if (temp_r0_3->dict.numEntry != 1) {
            GF_AssertFail();
        }
        if (temp_r0_3 != NULL) {
            temp_r1 = &temp_r0_3->dict;
            if ((temp_r1 != NULL) && ((u32)temp_r0_3->dict.numEntry > idx)) {
                var_r0_2 = (s32 *)((u8 *)temp_r1 + temp_r0_3->dict.ofsEntry + 4);
            } else {
                var_r0_2 = NULL;
            }
            if (var_r0_2 != NULL) {
                var_r5_3 = &temp_r0_3->header.chr[*var_r0_2];
            } else {
                goto block_28;
            }
        } else {
block_28:
            var_r5_3 = NULL;
        }
        if (var_r5_3 == NULL) {
            GF_AssertFail();
        }
        NNS_G3dRenderObjInit(task->renderObj, (NNSG3dResMdl *)var_r5_3);
        if (task->renderObj->anmMat != NULL) {
            GF_AssertFail();
        }
        task->renderObj->anmMat = NULL;
        if ((ov01_02204698(task->areaMapAnimation) != 0) && (sub_02054E20(task->modelNo) == 0)) {
            ov01_02204678(task->areaMapAnimation, task->renderObj);
        }
        *task->modelLoaded = 1;
        task->state = 5U;
        return;
    case 5:
        *task->loading = 0;
        Heap_Free(task);
        SysTask_Destroy(sysTask);
        return;
    }
}
