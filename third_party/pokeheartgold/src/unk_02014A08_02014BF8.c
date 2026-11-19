#include "global.h"
#include "constants/heap.h"
#include "sys_task.h"
#include "screen_scroll_manager.h"
#include "overlay_07_bg_scroll_context.h"

struct BufferManager;
struct ScreenScrollManager;
struct SysTask;
struct SysTaskQueue;
void *sub_02014A4C(const BufferManager *bufferManager);
BufferManager *sub_02014A08(enum HeapID heapID, void *buffer1, void *buffer2);
ScreenScrollManager *ScreenScrollManager_New(enum HeapID heapID);
void sub_02014A8C(BufferManager *bufferManager);

void *sub_02014BF8(ScreenScrollManager *screenScrollMgr);
void sub_02014C08(ScreenScrollManager *screenScrollMgr);

void *sub_02014BF8(ScreenScrollManager *screenScrollMgr)
{
    return sub_02014A4C(screenScrollMgr->bufferManager);
}

void sub_02014C08(ScreenScrollManager *screenScrollMgr)
{
    if (screenScrollMgr == 0L ) {
        return;
    }

    if (screenScrollMgr->scrollTask) {
        sub_02014A8C(screenScrollMgr->bufferManager);

        void *writeBuffer = sub_02014A4C(screenScrollMgr->bufferManager);
        MI_CpuFill32(writeBuffer, screenScrollMgr->initValue, sizeof(u32) * 192 );
    }
}
