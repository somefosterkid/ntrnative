#include "global.h"

#include "sys_task_api.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "filesystem.h"
#include "field/overlay_01_021FB878.h"

struct Field3DModelAnimation;

struct Field3dModel;

struct NARC;

struct SysTask;

struct SysTaskQueue;

void ov01_021FBD8C(SysTask *task, void *param);


void sub_020180BC(Field3DModelAnimation *anim, const Field3dModel *model, NARC *narc, u32 memberIndex, enum HeapID heapID, NNSFndAllocator *allocator);

void ov01_021FBD38(Field3dModel *model, void *data);

void ov01_021FBD38(Field3dModel *model, void *data) {
    short new_var;
    model->dataRaw = data;
    model->modelSet = NNS_G3dGetMdlSet(data);
    model->model = NNS_G3dGetMdlByIdx(model->modelSet, 0);
    model->texture = NNS_G3dGetTex(model->dataRaw);
    if (model->texture) {
        new_var = 1024;
        SysTask_CreateOnVWaitQueue(ov01_021FBD8C, model, new_var);
    }
}
