// Black's code at main 0x020821c0-0x02082a48. No name is known: the module and address.

#include "types.h"
#include "nitro/os.h"
#include "system/main.h"
#include "nitro/mi.h"
#include "nitro/gx.h"
#include "nitro/card.h"
// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they
// belong in the library's header
#define REG_DMA0SAD_ADDR 0x040000b0
#define MI_DMA_REGS ((MIDmaRegs *)REG_DMA0SAD_ADDR)
#define MI_DMA_TIMING_MASK 0x38000000
#define MI_DMA_CONTINUOUS_ON 0x02000000
#define MI_DMA_ENABLE 0x80000000
#define MI_DMA_DEST_FIX 0x00400000
#define MI_DMA_SRC_FIX 0x01000000
#define MI_DMA0_DUMMY_CNT (MI_DMA_ENABLE | MI_DMA_SRC_FIX | MI_DMA_DEST_FIX | 1)


typedef struct dma dma;

typedef struct MIiGXDmaParams {
    vu32 isBusy;
    u32 dmaNo;
    uintptr_t src;
    u32 length;
    MIDmaCallback callback;
    void *arg;
    GXFifoIntrCond fifoCond;
    void (*fifoFunc)(void);
    u32 unk20[5];
} MIiGXDmaParams;

static MIiGXDmaParams data_02150e6c;
void func_02082900(void *arg);
void func_0208284c(void);

#pragma thumb off

void func_02082a1c(void *unused);

void MIi_DmaFill32(u32 dmaNo, void *dest, u32 data, u32 size, BOOL dmaEnable) {
    vu32 *dmaCntp;

    if (size == 0) {
        return;
    }
    dmaCntp = &((vu32 *)REG_DMA0SAD_ADDR)[dmaNo * 3 + 2];
    while (*dmaCntp & MI_DMA_ENABLE) {
    }
    if (dmaEnable) {
        MIi_DmaSetParameters(dmaNo, data, (uintptr_t)dest, MI_DMA_ENABLE | MI_DMA_32BIT_BUS | MI_DMA_SRC_FIX | (size >> 2),
                             MIi_DMA_MODE_WAIT | MIi_DMA_MODE_SRC32);
    } else {
        MIi_DmaSetParameters(dmaNo, data, (uintptr_t)dest, MI_DMA_32BIT_BUS | MI_DMA_SRC_FIX | (size >> 2),
                             MIi_DMA_MODE_WAIT | MIi_DMA_MODE_NOCLEAR | MIi_DMA_MODE_SRC32);
    }
    while (*dmaCntp & MI_DMA_ENABLE) {
    }
}

void MIi_DmaCopy32(u32 dmaNo, const void *src, void *dest, u32 size, BOOL dmaEnable) {
    vu32 *dmaCntp;

    if (size == 0) {
        return;
    }
    MIi_CheckDma0SourceAddress(dmaNo, (uintptr_t)src, size, MI_DMA_SRC_INC);
    dmaCntp = &((vu32 *)REG_DMA0SAD_ADDR)[dmaNo * 3 + 2];
    while (*dmaCntp & MI_DMA_ENABLE) {
    }
    if (dmaEnable) {
        MIi_DmaSetParameters(dmaNo, (uintptr_t)src, (uintptr_t)dest, MI_DMA_ENABLE | MI_DMA_32BIT_BUS | (size >> 2),
                             MIi_DMA_MODE_WAIT);
    } else {
        MIi_DmaSetParameters(dmaNo, (uintptr_t)src, (uintptr_t)dest, MI_DMA_32BIT_BUS | (size >> 2),
                             MIi_DMA_MODE_WAIT | MIi_DMA_MODE_NOCLEAR);
    }
    while (*dmaCntp & MI_DMA_ENABLE) {
    }
}

void MIi_DmaCopy16(u32 dmaNo, const void *src, void *dest, u32 size, BOOL dmaEnable) {
    vu32 *dmaCntp;

    if (size == 0) {
        return;
    }
    MIi_CheckDma0SourceAddress(dmaNo, (uintptr_t)src, size, MI_DMA_SRC_INC);
    dmaCntp = &((vu32 *)REG_DMA0SAD_ADDR)[dmaNo * 3 + 2];
    while (*dmaCntp & MI_DMA_ENABLE) {
    }
    if (dmaEnable) {
        MIi_DmaSetParameters(dmaNo, (uintptr_t)src, (uintptr_t)dest, MI_DMA_ENABLE | (size >> 1), MIi_DMA_MODE_WAIT);
    } else {
        MIi_DmaSetParameters(dmaNo, (uintptr_t)src, (uintptr_t)dest, size >> 1, MIi_DMA_MODE_WAIT | MIi_DMA_MODE_NOCLEAR);
    }
    while (*dmaCntp & MI_DMA_ENABLE) {
    }
}



void MIi_DmaFill32Async(u32 dmaNo, void *dest, u32 data, u32 size, MIDmaCallback callback, void *arg, BOOL dmaEnable) {
    if (size == 0) {
        if (callback != NULL) {
            callback(arg);
        }
        return;
    }
    MI_WaitDma(dmaNo);
    if (callback != NULL) {
        OSi_EnterDmaCallback(dmaNo, callback, arg);
        if (dmaEnable) {
            MIi_DmaSetParameters(dmaNo, data, (uintptr_t)dest,
                                 MI_DMA_ENABLE | MI_DMA_IF_ENABLE | MI_DMA_32BIT_BUS | MI_DMA_SRC_FIX | (size >> 2),
                                 MIi_DMA_MODE_SRC32);
        } else {
            MIi_DmaSetParameters(dmaNo, data, (uintptr_t)dest,
                                 MI_DMA_IF_ENABLE | MI_DMA_32BIT_BUS | MI_DMA_SRC_FIX | (size >> 2),
                                 MIi_DMA_MODE_NOCLEAR | MIi_DMA_MODE_SRC32);
        }
    } else {
        if (dmaEnable) {
            MIi_DmaSetParameters(dmaNo, data, (uintptr_t)dest,
                                 MI_DMA_ENABLE | MI_DMA_32BIT_BUS | MI_DMA_SRC_FIX | (size >> 2), MIi_DMA_MODE_SRC32);
        } else {
            MIi_DmaSetParameters(dmaNo, data, (uintptr_t)dest, MI_DMA_32BIT_BUS | MI_DMA_SRC_FIX | (size >> 2),
                                 MIi_DMA_MODE_NOCLEAR | MIi_DMA_MODE_SRC32);
        }
    }
}
#pragma thumb on

#pragma thumb off
void MIi_DmaCopy32Async(u32 dmaNo, const void *src, void *dest, u32 size, MIDmaCallback callback, void *arg,
                        BOOL dmaEnable) {
    MIi_CheckDma0SourceAddress(dmaNo, (uintptr_t)src, size, MI_DMA_SRC_INC);
    if (size == 0) {
        if (callback != NULL) {
            callback(arg);
        }
        return;
    }
    MI_WaitDma(dmaNo);
    if (callback != NULL) {
        OSi_EnterDmaCallback(dmaNo, callback, arg);
        if (dmaEnable) {
            MIi_DmaSetParameters(dmaNo, (uintptr_t)src, (uintptr_t)dest,
                                 MI_DMA_ENABLE | MI_DMA_IF_ENABLE | MI_DMA_32BIT_BUS | (size >> 2), 0);
        } else {
            MIi_DmaSetParameters(dmaNo, (uintptr_t)src, (uintptr_t)dest, MI_DMA_IF_ENABLE | MI_DMA_32BIT_BUS | (size >> 2),
                                 MIi_DMA_MODE_NOCLEAR);
        }
    } else {
        if (dmaEnable) {
            MIi_DmaSetParameters(dmaNo, (uintptr_t)src, (uintptr_t)dest, MI_DMA_ENABLE | MI_DMA_32BIT_BUS | (size >> 2), 0);
        } else {
            MIi_DmaSetParameters(dmaNo, (uintptr_t)src, (uintptr_t)dest, MI_DMA_32BIT_BUS | (size >> 2), MIi_DMA_MODE_NOCLEAR);
        }
    }
}
#pragma thumb on

#pragma thumb off
void MI_WaitDma(u32 dma) {
    OSIntrMode enabled = OS_DisableInterrupts();

    while (MI_DMA_REGS[dma].cnt & MI_DMA_ENABLE) {
    }
    if (dma == 0) {
        MIDmaRegs *regs = &MI_DMA_REGS[dma];
        regs->src = 0;
        regs->dest = 0;
        regs->cnt = MI_DMA0_DUMMY_CNT;
    }
    OS_RestoreInterrupts(enabled);
}

void MI_StopDma(u32 dmaNo) {
    u32 enabled = OS_DisableInterrupts();
    vu32 *dmaCntp = &MI_DMA_REGS[dmaNo].cnt;

    *dmaCntp &= ~(MI_DMA_TIMING_MASK | MI_DMA_CONTINUOUS_ON);
    *dmaCntp &= ~MI_DMA_ENABLE;
    // The ARM9 waits two cycles for the channel to stop
    (void)*dmaCntp;
    (void)*dmaCntp;
    if (dmaNo == 0) {
        MIDmaRegs *regs = &MI_DMA_REGS[dmaNo];
        regs->src = 0;
        regs->dest = 0;
        regs->cnt = MI_DMA0_DUMMY_CNT;
    }
    OS_RestoreInterrupts(enabled);
}

#pragma thumb on

#pragma thumb off
void MI_StopAllDma(void) {
    MI_StopDma(0);
    MI_StopDma(1);
    MI_StopDma(2);
    MI_StopDma(3);
}
#pragma thumb on

#pragma thumb off
void MIi_CheckAnotherAutoDMA(u32 dmaNo, u32 dmaType) {
    int i;

    for (i = 0; i < 3; i++) {
        u32 cnt;
        u32 type;

        if (i == dmaNo) {
            continue;
        }
        cnt = MI_DMA_REGS[i].cnt;
        if (!(cnt & MI_DMA_ENABLE)) {
            continue;
        }
        type = cnt & MI_DMA_TIMING_MASK;
        if (type == dmaType) {
            continue;
        }
        if (type == MI_DMA_TIMING_V_BLANK && dmaType == MI_DMA_TIMING_H_BLANK) {
            continue;
        }
        if (type == MI_DMA_TIMING_H_BLANK && dmaType == MI_DMA_TIMING_V_BLANK) {
            continue;
        }
        if (type == MI_DMA_TIMING_DISP || type == MI_DMA_TIMING_DISP_MMEM || type == MI_DMA_TIMING_CARD ||
            type == MI_DMA_TIMING_CARTRIDGE || type == MI_DMA_TIMING_GXFIFO || type == MI_DMA_TIMING_V_BLANK ||
            type == MI_DMA_TIMING_H_BLANK) {
            OS_Terminate();
        }
    }
}
#pragma thumb on

#pragma thumb off
void MIi_CheckDma0SourceAddress(u32 dmaNo, uintptr_t src, u32 size, u32 dir) {
    if (dmaNo == 0) {
        // The address's area: its top byte
        uintptr_t areaStart = src & 0xff000000;
        uintptr_t areaEnd = src;

        switch (dir) {
        case MI_DMA_SRC_INC:
            areaEnd = src + size;
            break;
        case MI_DMA_SRC_DEC:
            areaEnd = src - size;
            break;
        }
        areaEnd &= 0xff000000;
        // The I/O registers (0x04000000) and the cartridge (0x08000000 on)
        if (areaStart == 0x04000000 || areaStart >= 0x08000000 || areaEnd == 0x04000000 || areaEnd >= 0x08000000) {
            OS_Terminate();
        }
    }
}

void MI_SendGXCommandAsync(u32 dmaNo, const void *src, u32 length, MIDmaCallback callback, void *arg) {
    OSIntrMode enabled;

    if (length == 0) {
        if (callback != NULL) {
            callback(arg);
        }
        return;
    }
    while (data_02150e6c.isBusy) {
    }
    while (!(G3X_GetCommandFifoStatus() & GX_FIFOSTAT_UNDERHALF)) {
    }
    data_02150e6c.isBusy = TRUE;
    data_02150e6c.dmaNo = dmaNo;
    data_02150e6c.src = (uintptr_t)src;
    data_02150e6c.length = length;
    data_02150e6c.callback = callback;
    data_02150e6c.arg = arg;
    MIi_CheckDma0SourceAddress(dmaNo, (uintptr_t)src, length, MI_DMA_SRC_INC);
    MI_WaitDma(dmaNo);
    enabled = OS_DisableInterrupts();
    data_02150e6c.fifoCond = (GXFifoIntrCond)((reg_G3X_GXSTAT & REG_G3X_GXSTAT_FI_MASK) >> REG_G3X_GXSTAT_FI_SHIFT);
    data_02150e6c.fifoFunc = OS_GetIrqFunction(OS_IE_GXFIFO);
    G3X_SetFifoIntrCond(GX_FIFOINTR_COND_UNDERHALF);
    OS_SetIrqFunction(OS_IE_GXFIFO, func_0208284c);
    OS_EnableIrqMask(OS_IE_GXFIFO);
    func_0208284c();
    OS_RestoreInterrupts(enabled);
}

#pragma thumb on

#pragma thumb off
void func_0208284c(void) {
    u32 length;
    uintptr_t src;

    if (data_02150e6c.length == 0) {
        return;
    }
    length = data_02150e6c.length >= 0x1d8 ? 0x1d8 : data_02150e6c.length;
    src = data_02150e6c.src;
    data_02150e6c.length -= length;
    data_02150e6c.src += length;
    if (data_02150e6c.length == 0) {
        OSi_EnterDmaCallback(data_02150e6c.dmaNo, func_02082900, NULL);
        MIi_DmaSetParameters(data_02150e6c.dmaNo, src, REG_GXFIFO_ADDR, 0xc4400000 | (length >> 2), 0);
        OS_ResetRequestIrqMask(OS_IE_GXFIFO);
        return;
    }
    MIi_DmaSetParameters(data_02150e6c.dmaNo, src, REG_GXFIFO_ADDR, 0x84400000 | (length >> 2), 0);
    OS_ResetRequestIrqMask(OS_IE_GXFIFO);
}

void func_02082900(void *arg) {
    MIDmaCallback callback;
    void *callbackArg;

    OS_DisableIrqMask(OS_IE_GXFIFO);
    G3X_SetFifoIntrCond(data_02150e6c.fifoCond);
    OS_SetIrqFunction(OS_IE_GXFIFO, data_02150e6c.fifoFunc);
    data_02150e6c.isBusy = FALSE;
    callback = data_02150e6c.callback;
    callbackArg = data_02150e6c.arg;
    if (callback != NULL) {
        callback(callbackArg);
    }
}

void MI_SendGXCommandAsyncFast(u32 dmaNo, const void *src, u32 length, MIDmaCallback callback, void *arg) {
    if (length == 0) {
        if (callback != NULL) {
            callback(arg);
        }
        return;
    }
    while (data_02150e6c.isBusy) {
    }
    data_02150e6c.isBusy = TRUE;
    data_02150e6c.dmaNo = dmaNo;
    data_02150e6c.callback = callback;
    data_02150e6c.arg = arg;
    MIi_CheckAnotherAutoDMA(dmaNo, MI_DMA_TIMING_GXFIFO);
    MIi_CheckDma0SourceAddress(dmaNo, (uintptr_t)src, length, MI_DMA_SRC_INC);
    MI_WaitDma(dmaNo);
    OSi_EnterDmaCallback(dmaNo, func_02082a1c, NULL);
    MIi_DmaSetParameters(dmaNo, (uintptr_t)src, REG_GXFIFO_ADDR,
                         MI_DMA_ENABLE | MI_DMA_IF_ENABLE | MI_DMA_TIMING_GXFIFO | MI_DMA_32BIT_BUS | MI_DMA_DEST_FIX |
                             (length >> 2),
                         0);
}


#pragma thumb on

#pragma thumb off
void func_02082a1c(void *unused) {
    void *callbackArg;
    MIDmaCallback callback;

    data_02150e6c.isBusy = FALSE;
    callbackArg = data_02150e6c.arg;
    callback = data_02150e6c.callback;
    if (callback != NULL) {
        callback(callbackArg);
    }
}
#pragma thumb on
