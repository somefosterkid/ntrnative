#include "global.h"
#include "constants/heap.h"
#include "sys_task.h"
#include "screen_scroll_manager.h"
#include "overlay_07_bg_scroll_context.h"

struct BufferManager;
struct ScreenScrollManager;
struct SysTask;
struct SysTaskQueue;
void sub_02014C60(s16 *dest, u32 entryCount, u16 angleIncrement, fx32 amplitude);
SysTask * SysTask_CreateOnMainQueue(SysTaskFunc func, void *data, u32 priority);
void sub_02014CBC(SysTask *task, void *param);
BufferManager *sub_02014A08(enum HeapID heapID, void *buffer1, void *buffer2);
ScreenScrollManager *ScreenScrollManager_New(enum HeapID heapID);
void *sub_02014A4C(const BufferManager *bufferManager);
void SysTask_Destroy(SysTask *task);

void sub_02014B08(ScreenScrollManager *screenScrollMgr, u8 start, u8 end, u16 angleIncrement, fx32 amplitude, s16 speed, void *bg, u32 initValue, u32 priority);
void sub_02014B9C(ScreenScrollManager *screenScrollMgr);

void sub_02014B08(ScreenScrollManager *screenScrollMgr, u8 start, u8 end, u16 angleIncrement, fx32 amplitude, s16 speed, void *bg, u32 initValue, u32 priority)
{
    ((screenScrollMgr) ? (void)0 : GF_AssertFail()) ;
    ((screenScrollMgr->scrollTask == 0L) ? (void)0 : GF_AssertFail()) ;

    screenScrollMgr->start = start;
    screenScrollMgr->end = end;
    screenScrollMgr->scrollPos = 0;
    screenScrollMgr->scrollSpeed = speed;
    screenScrollMgr->offsetRegisterAddr = bg;
    screenScrollMgr->initValue = initValue;

    sub_02014C60(screenScrollMgr->sineTable, 192 , angleIncrement, amplitude);

    screenScrollMgr->scrollTask = SysTask_CreateOnMainQueue(sub_02014CBC, screenScrollMgr, priority);

    MI_CpuFill32(screenScrollMgr->buffer1, screenScrollMgr->initValue, sizeof(u32) * 192 );
    MI_CpuFill32(screenScrollMgr->buffer2, screenScrollMgr->initValue, sizeof(u32) * 192 );
}

void sub_02014B9C(ScreenScrollManager *screenScrollMgr)
{
    ((screenScrollMgr) ? (void)0 : GF_AssertFail()) ;

    if (screenScrollMgr->scrollTask) {
        SysTask_Destroy(screenScrollMgr->scrollTask);
        screenScrollMgr->scrollTask = 0L ;

        void *writeBuffer = sub_02014A4C(screenScrollMgr->bufferManager);
        memset(writeBuffer, screenScrollMgr->initValue, sizeof(u32) * 192 );
    }
}
