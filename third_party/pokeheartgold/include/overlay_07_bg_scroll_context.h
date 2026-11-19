#ifndef POKEHEARTGOLD_OVERLAY_07_BG_SCROLL_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_BG_SCROLL_CONTEXT_H

#include "constants/heap.h"

#include "screen_scroll_manager.h"
#include "sys_task.h"

// Two-buffer scroll table owner for ScreenScrollManager / the custom HBlank scroll contexts.
typedef struct BufferManager {
    void *buffers[2];
    u8 writeBuffer;
    u8 mode;
    u8 padding[2];
} BufferManager;

enum BufferManagerDMAType {
    BUFFER_MANAGER_DMA_TYPE_16BIT = 0,
    BUFFER_MANAGER_DMA_TYPE_32BIT = 1,
};

typedef void (*VBlankDMAFunc)(void *);

// sizeof == 0x1C on the DS
typedef struct VBlankDMAController {
    SysTask *onVBlankTask;
    SysTask *postVBlankTask;
    BOOL doDMA;
    BOOL swapBuffers;
    VBlankDMAFunc dmaFunc;
    VBlankDMAFunc swapBufferFunc;
    void *param;
} VBlankDMAController;

// sizeof == 0x20 on the DS
typedef struct BgScrollContext {
    VBlankDMAController dmaController;
    ScreenScrollManager *screenScrollMgr;
} BgScrollContext;

// sizeof == 0x624 on the DS
typedef struct CustomBgScrollContext {
    VBlankDMAController dmaController;
    BufferManager *bufferManager;
    u32 buffer1[192];
    u32 buffer2[192];
    void *offsetReg;
} CustomBgScrollContext;

BgScrollContext *ov07_02222CCC(u8 startY, u8 endY, u16 angleIncrement, fx32 amplitude, s16 speed, u32 bgID, u32 priority, u32 initValue, enum HeapID heapID);
void ov07_02222D3C(BgScrollContext *ctx);

#endif // POKEHEARTGOLD_OVERLAY_07_BG_SCROLL_CONTEXT_H
