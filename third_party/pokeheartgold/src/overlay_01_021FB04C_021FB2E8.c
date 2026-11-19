#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "heap.h"
#include "overlay_01_021FB04C.h"
#include "sys_task.h"
#include "sys_task_api.h"

enum BDHCSubTask {
    BDHC_LOADER_SUBTASK_PREPARE_FILE_LOAD = 0,
    BDHC_LOADER_SUBTASK_LOAD_FILE,
    BDHC_LOADER_SUBTASK_END_TASK,
};
typedef struct {
    int pointsCount;
    int normalsCount;
    int constantsCount;
    int platesCount;
    int stripsCount;
    int accessListCount;
} BDHCHeader;
typedef struct {
    FSFile dummy00;
    int dummyAC;
    BDHCHeader bdhcHeader;
    BOOL dummyC8;
    u8 currentSubTask;
    u8 *buffer;
    Bdhc *bdhc;
    BOOL killLoadTask;
    BOOL *loadTaskRunning;
    NARC *landDataNARC;
    int dummyE4;
    BOOL *mapModelLoadTaskRunning;
} BDHCLoaderTaskContext;
void ov01_021FB174(SysTask *sysTask, void *sysTaskParam);


void ov01_021FB2E8(Bdhc *bdhc) {
    if (bdhc != 0L) {
        Heap_Free(bdhc);
    }
}

void ov01_021FB2F4(Bdhc *bdhc) {
    if (bdhc == 0L) {
        return;
    }

    bdhc->loaded = 0;
    bdhc->points = 0L;
    bdhc->normals = 0L;
    bdhc->plates = 0L;
    bdhc->strips = 0L;
    bdhc->accessList = 0L;
}

SysTask *ov01_021FB308(NARC *landDataNARC, const int sLotoId, Bdhc *bdhc, BOOL *loadTaskRunning, u8 **buffer, BOOL *mapModelLoadTaskRunning) {
    BDHCLoaderTaskContext *ctx = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(BDHCLoaderTaskContext));

    ctx->currentSubTask = BDHC_LOADER_SUBTASK_PREPARE_FILE_LOAD;
    ctx->landDataNARC = landDataNARC;
    ctx->dummyE4 = sLotoId;
    ctx->bdhc = bdhc;
    ctx->loadTaskRunning = loadTaskRunning;
    ctx->killLoadTask = 0;
    ctx->dummyC8 = 0;
    ctx->dummyAC = 0;
    ctx->buffer = *buffer;
    ctx->mapModelLoadTaskRunning = mapModelLoadTaskRunning;

    return SysTask_CreateOnMainQueue(ov01_021FB174, ctx, 1);
}

void ov01_021FB354(SysTask *sysTask) {
    BDHCLoaderTaskContext *ctx = SysTask_GetData(sysTask);
    ctx->killLoadTask = 1;
}
