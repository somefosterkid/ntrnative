#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "overlay_01_021FB04C.h"
#include "sys_task_api.h"

typedef struct BDHCPlate {
    u16 firstPointIndex;
    u16 secondPointIndex;
    u16 normalIndex;
    u16 constantIndex;
} BDHCPlate;

typedef struct BDHCPoint {
    fx32 x;
    fx32 z;
} BDHCPoint;

typedef struct BDHCStrip {
    fx32 scanline;
    u16 accessListElementCount;
    u16 accessListStartIndex;
} BDHCStrip;

typedef struct BDHC {
    BDHCPlate *plates;
    fx32 *constants;
    BDHCStrip *strips;
    u16 *accessList;
    BDHCPoint *points;
    VecFx32 *normals;
    BOOL loaded;
    int stripsCount;
} BDHC;

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
    BDHC *bdhc;
    BOOL killLoadTask;
    BOOL *loadTaskRunning;
    NARC *landDataNARC;
    int dummyE4;
    BOOL *mapModelLoadTaskRunning;
} BDHCLoaderTaskContext;

void ov01_021FB0B0(const BDHCHeader *bdhcHeader, BDHC *bdhc, void **buffer);

void ov01_021FB110(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader);

void ov01_021FB120(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader);

void ov01_021FB134(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader);

void ov01_021FB144(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader);

void ov01_021FB154(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader);

void ov01_021FB164(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader);

void ov01_021FB04C(NARC *narc, BDHCHeader *bdhcHeader);

void ov01_021FB174(SysTask *sysTask, void *sysTaskParam) {
    BOOL subTaskCompleted;
    BDHCLoaderTaskContext *ctx = (BDHCLoaderTaskContext *)sysTaskParam;

    if (ctx->killLoadTask == TRUE) {
        ctx->currentSubTask = BDHC_LOADER_SUBTASK_END_TASK;
    }

    switch (ctx->currentSubTask) {
    case BDHC_LOADER_SUBTASK_PREPARE_FILE_LOAD:
        if (*ctx->mapModelLoadTaskRunning) {
            subTaskCompleted = FALSE;
            break;
        }

        ov01_021FB04C(ctx->landDataNARC, &ctx->bdhcHeader);
        ctx->bdhc->stripsCount = ctx->bdhcHeader.stripsCount;
        ov01_021FB0B0(&ctx->bdhcHeader, ctx->bdhc, (void **)&ctx->buffer);

        subTaskCompleted = TRUE;
        break;

    case BDHC_LOADER_SUBTASK_LOAD_FILE:
        ov01_021FB110(ctx->landDataNARC, ctx->bdhc, &ctx->bdhcHeader);
        ov01_021FB120(ctx->landDataNARC, ctx->bdhc, &ctx->bdhcHeader);
        ov01_021FB134(ctx->landDataNARC, ctx->bdhc, &ctx->bdhcHeader);
        ov01_021FB144(ctx->landDataNARC, ctx->bdhc, &ctx->bdhcHeader);
        ov01_021FB154(ctx->landDataNARC, ctx->bdhc, &ctx->bdhcHeader);
        ov01_021FB164(ctx->landDataNARC, ctx->bdhc, &ctx->bdhcHeader);

        subTaskCompleted = TRUE;
        break;

    case BDHC_LOADER_SUBTASK_END_TASK:
        *ctx->loadTaskRunning = FALSE;

        Heap_Free((void *)sysTaskParam);
        SysTask_Destroy(sysTask);

        return;
    }

    if (subTaskCompleted == TRUE) {
        ctx->currentSubTask++;

        if (ctx->currentSubTask == BDHC_LOADER_SUBTASK_END_TASK) {
            ctx->bdhc->loaded = TRUE;
        }
    }
}
