// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/src/photo_sign_util.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "nitro/os.h"
#include "nitro/pxi.h"
#include "photo_sign_private.h"

// What the photo signature's other files use: sending words to the ARM7 over the FIFO, the lock of its commands, and
// the cache maintenance of the buffers it reads and writes. ARM code in the DSi-only LTD autoload. The ROM names none
// of it; the file's name and those of its functions and data are descriptive guesses, and so is the boundary: its
// .bss is reached from its own base, so the lock was a file of its own, but whether the FIFO and cache helpers were
// in the same file the code doesn't show.

// Below these sizes the caches are flushed or invalidated by range, above them whole
#define DC_RANGE_MAX 0x2400
#define IC_RANGE_MAX 0x400
#define CACHE_LINE_MASK 0x1f

// A lock that the thread holding it may take again. The ARM7's result releases it in the FIFO's interrupt, so the
// owner is volatile: func_02707b0c reads it again after testing it
// Black 2's, not checked against Black
typedef struct {
    OSThread *volatile owner;
    s32 count;
    OSThreadQueue queue;
} PhotoSignLock;

static void func_027079d0(u32 fifoTag, u32 data);
static void func_027079fc(u32 fifoTag, const u8 *data, u32 size);

// pokebw2's sLock
static PhotoSignLock data_02735b20;

// pokebw2's PhotoSign_SendHeader
void func_02707994(u32 fifoTag, u16 command, u16 data) {
    while (PXI_SendWordByFifo(fifoTag,
                              ((command << PHOTO_SIGN_FIFO_COMMAND_SHIFT) & PHOTO_SIGN_FIFO_COMMAND_MASK) |
                                  PHOTO_SIGN_FIFO_START_BIT | (data & PHOTO_SIGN_FIFO_DATA_MASK),
                              FALSE) < 0) {
    }
}

// Sends a word of payload
// pokebw2's PhotoSign_SendWord
static void func_027079d0(u32 fifoTag, u32 data) {
    while (PXI_SendWordByFifo(fifoTag, data & PHOTO_SIGN_FIFO_PAYLOAD_MASK, FALSE) < 0) {
    }
}

// Sends data 3 bytes to a word, the first in the high byte
// pokebw2's PhotoSign_SendData
static void func_027079fc(u32 fifoTag, const u8 *data, u32 size) {
    u32 i;

    for (i = 0; size - i >= 3; i += 3) {
        func_027079d0(fifoTag, (data[i] << 16) | (data[i + 1] << 8) | data[i + 2]);
    }
    if (size - i == 2) {
        func_027079d0(fifoTag, (data[i] << 16) | (data[i + 1] << 8));
    } else if (size - i == 1) {
        func_027079d0(fifoTag, data[i] << 16);
    }
}

// pokebw2's PhotoSign_SendCommand
void func_02707a94(u32 command, const void *data, u32 size) {
    func_02707994(PHOTO_SIGN_FIFO_TAG, command, 0);
    func_027079fc(PHOTO_SIGN_FIFO_TAG, data, size);
}

// pokebw2's PhotoSign_SendCommandData
void func_02707acc(u32 command, u32 data) {
    func_02707994(PHOTO_SIGN_FIFO_TAG, command, data);
}

// pokebw2's PhotoSign_InitLock
void func_02707aec(void) {
    OS_InitThreadQueue(&data_02735b20.queue);
    data_02735b20.owner = NULL;
    data_02735b20.count = 0;
}

// pokebw2's PhotoSign_TryLock
BOOL func_02707b0c(void) {
    u32 enabled = OS_DisableInterrupts();
    OSThread *current = OSi_ThreadInfo.current;
    BOOL result;

    if (data_02735b20.owner == NULL) {
        data_02735b20.owner = current;
        data_02735b20.count = 1;
        result = TRUE;
    } else if (data_02735b20.owner == current) {
        data_02735b20.count++;
        result = TRUE;
    } else {
        result = FALSE;
    }
    OS_RestoreInterrupts(enabled);
    return result;
}

// pokebw2's PhotoSign_Lock
void func_02707b68(void) {
    u32 enabled = OS_DisableInterrupts();
    OSThread *current = OSi_ThreadInfo.current;

    while (TRUE) {
        OSThread *owner = data_02735b20.owner;

        if (owner == NULL) {
            data_02735b20.owner = current;
            data_02735b20.count = 1;
            break;
        } else if (owner == current) {
            data_02735b20.count++;
            break;
        }
        OS_SleepThread(&data_02735b20.queue);
    }
    OS_RestoreInterrupts(enabled);
}



// pokebw2's PhotoSign_Unlock
void func_02707bdc(void) {
    u32 enabled = OS_DisableInterrupts();

    if (data_02735b20.owner != NULL && --data_02735b20.count == 0) {
        data_02735b20.owner = NULL;
        OS_WakeupThread(&data_02735b20.queue);
    }
    OS_RestoreInterrupts(enabled);
}

// pokebw2's PhotoSign_FlushCache
void func_02707c2c(const void *addr, u32 size) {
    if (size < DC_RANGE_MAX) {
        DC_FlushRange(addr, size);
    } else {
        DC_FlushAll();
    }
}

// Writes back the lines the buffer shares at its ends before discarding it
// pokebw2's PhotoSign_InvalidateCache
void func_02707c48(void *addr, u32 size) {
    u32 end;

    if (size < IC_RANGE_MAX) {
        IC_InvalidateRange(addr, size);
    } else {
        IC_InvalidateAll();
    }
    if (size < DC_RANGE_MAX) {
        end = (u32)addr + size;
        if ((u32)addr & CACHE_LINE_MASK) {
            DC_StoreRange(addr, 1);
        }
        if (end & CACHE_LINE_MASK) {
            DC_StoreRange((void *)(end - 1), 1);
        }
        DC_InvalidateRange(addr, size);
    } else {
        DC_FlushAll();
    }
}
