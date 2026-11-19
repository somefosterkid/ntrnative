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
SysTask *sub_02014AA0(SysTaskFunc callback, void *param, u32 priority);

void sub_02014C60(s16 *dest, u32 entryCount, u16 angleIncrement, fx32 amplitude);
void sub_02014CBC(SysTask *task, void *param);
SysTask *sub_02014D68(SysTaskFunc callback, void *param, int priority);

void sub_02014C60(s16 *dest, u32 entryCount, u16 angleIncrement, fx32 amplitude)
{
    u16 angle = 0;
    for (int i = 0; i < entryCount; i++) {
        dest[i] = FX_MulInline(FX_SinIdx(angle), amplitude) >> 12 ;
        angle += angleIncrement;
    }
}

void sub_02014CBC(SysTask *task, void *param)
{
    ScreenScrollManager *screenScrollMgr = param;
    u32 *writeBuffer = sub_02014A4C(screenScrollMgr->bufferManager);
    u8 scrollPos = screenScrollMgr->scrollPos / 100 ;

    for (int i = screenScrollMgr->start; i <= screenScrollMgr->end; i++) {
        s16 xOff = writeBuffer[i] & 0xffff;
        u16 yOff = writeBuffer[i] >> 16;

        xOff += screenScrollMgr->sineTable[scrollPos];
        writeBuffer[i] = (yOff << 16) | (xOff & 0xffff);
        scrollPos = (scrollPos + 1) % 192 ;
    }

    screenScrollMgr->scrollPos += screenScrollMgr->scrollSpeed;

    if (screenScrollMgr->scrollPos >= (192 * 100) ) {
        screenScrollMgr->scrollPos %= (192 * 100) ;
    } else if (screenScrollMgr->scrollPos < 0) {
        screenScrollMgr->scrollPos += (192 * 100) ;
    }
}

SysTask *sub_02014D68(SysTaskFunc callback, void *param, int priority)
{
    return sub_02014AA0(callback, param, priority);
}
