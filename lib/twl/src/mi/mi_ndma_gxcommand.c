// TwlSDK 5's mi_ndma_gxcommand.c (ARM9), in the LTD autoload's .ltdmain: geometry commands sent to the GX FIFO by a
// new DMA channel, as libntr's reconstruction has it (pokebw2's lib/twl/src/mi/mi_ndma_gxcommand.c is the same,
// compiled as Thumb). Black's link keeps only the asynchronous functions and their callbacks
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "twl/mi.h"
#include "mi_ndma.h"

#define MIi_GX_LENGTH_ONCE (118 * sizeof(u32)) //(byte)

typedef struct {
    volatile BOOL isBusy;

    u32 ndmaNo;
    u32 src;
    u32 length;
    MINDmaCallback callback;
    void *arg;

    GXFifoIntrCond fifoCond;
    void (*fifoFunc)(void);
} MIiGXNDmaParams;

// TwlSDK defines it after twl/ltdmain_begin.h, which sends it to .ltdmain.bss; this tree's link script places no
// .ltdmain.bss, so it comes before the header here (in .bss, at Black's 0x02732f3c)
static MIiGXNDmaParams MIi_GXNDmaParams = {FALSE};

#include "twl/ltdmain_begin.h"

static void MIi_FIFOCallback(void);
static void MIi_NDMACallback(void *);
static void MIi_NDMAFastCallback(void *);

void MI_SendNDmaGXCommandAsync(u32 ndmaNo, const void *src, u32 commandLength, MINDmaCallback callback, void *arg) {
    if (commandLength == 0) {
        MIi_CallCallback(callback, arg);
        return;
    }

    while (MIi_GXNDmaParams.isBusy) {
    }

    while (!(G3X_GetCommandFifoStatus() & GX_FIFOSTAT_UNDERHALF)) {
    }

    MIi_GXNDmaParams.isBusy = TRUE;
    MIi_GXNDmaParams.ndmaNo = ndmaNo;
    MIi_GXNDmaParams.src = (u32)src;
    MIi_GXNDmaParams.length = commandLength;
    MIi_GXNDmaParams.callback = callback;
    MIi_GXNDmaParams.arg = arg;

    MI_WaitNDma(ndmaNo);
    {
        OSIntrMode enabled = OS_DisableInterrupts();

        MIi_GXNDmaParams.fifoCond = (GXFifoIntrCond)((reg_G3X_GXSTAT & REG_G3X_GXSTAT_FI_MASK) >> REG_G3X_GXSTAT_FI_SHIFT);
        MIi_GXNDmaParams.fifoFunc = OS_GetIrqFunction(OS_IE_GXFIFO);

        G3X_SetFifoIntrCond(GX_FIFOINTR_COND_UNDERHALF);
        OS_SetIrqFunction(OS_IE_GXFIFO, MIi_FIFOCallback);
        (void)OS_EnableIrqMask(OS_IE_GXFIFO);

        MIi_FIFOCallback();

        (void)OS_RestoreInterrupts(enabled);
    }
}

static void MIi_FIFOCallback(void) {
    u32 length;
    u32 src;

    if (MIi_GXNDmaParams.length == 0) {
        return;
    }

    length = (MIi_GXNDmaParams.length >= MIi_GX_LENGTH_ONCE) ? MIi_GX_LENGTH_ONCE : MIi_GXNDmaParams.length;
    src = MIi_GXNDmaParams.src;

    MIi_GXNDmaParams.length -= length;
    MIi_GXNDmaParams.src += length;

    if (MIi_GXNDmaParams.length == 0) {
        (void)OS_DisableIrqMask(OS_IE_GXFIFO);
    }
    (void)OS_ResetRequestIrqMask(OS_IE_GXFIFO);

    MIi_NDmaAsync((MIi_GXNDmaParams.length == 0) ? MIi_NDMA_TYPE_GXCOPY_IF : MIi_NDMA_TYPE_GXCOPY,
                  MIi_GXNDmaParams.ndmaNo, (const void *)src, (void *)REG_GXFIFO_ADDR, 0 /*not used*/, length,
                  (MIi_GXNDmaParams.length == 0) ? MIi_NDMACallback : NULL, NULL, MI_NDMA_ENABLE);
}

static void MIi_NDMACallback(void *) {
    (void)OS_DisableIrqMask(OS_IE_GXFIFO);

    G3X_SetFifoIntrCond(MIi_GXNDmaParams.fifoCond);
    OS_SetIrqFunction(OS_IE_GXFIFO, MIi_GXNDmaParams.fifoFunc);

    MIi_GXNDmaParams.isBusy = FALSE;

    MIi_CallCallback(MIi_GXNDmaParams.callback, MIi_GXNDmaParams.arg);
}

void MI_SendNDmaGXCommandAsyncFast(u32 ndmaNo, const void *src, u32 commandLength, MINDmaCallback callback,
                                   void *arg) {
    if (commandLength == 0) {
        MIi_CallCallback(callback, arg);
        return;
    }

    while (MIi_GXNDmaParams.isBusy /*volatile valiable */) {
    }

    MIi_GXNDmaParams.isBusy = TRUE;
    MIi_GXNDmaParams.ndmaNo = ndmaNo;
    MIi_GXNDmaParams.callback = callback;
    MIi_GXNDmaParams.arg = arg;

    MI_WaitNDma(ndmaNo);

    MIi_NDmaAsync(MIi_NDMA_TYPE_GXCOPY_IF, ndmaNo, (const void *)src, (void *)REG_GXFIFO_ADDR, 0 /*not used*/,
                  commandLength, MIi_NDMAFastCallback, NULL, MI_NDMA_ENABLE);
}

static void MIi_NDMAFastCallback(void *) {
    MIi_GXNDmaParams.isBusy = FALSE;

    MIi_CallCallback(MIi_GXNDmaParams.callback, MIi_GXNDmaParams.arg);
}

#include "twl/ltdmain_end.h"
