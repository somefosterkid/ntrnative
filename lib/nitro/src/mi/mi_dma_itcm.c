// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/src/mi/mi_dma_itcm.c, ported by tools/port_file.py.
// The ITCM part of NitroSDK's mi_dma.c: set up a DMA channel and wait for its register writes to settle.
#include "nitro/mi.h"
#include "nitro/os.h"

#define MIi_DMA_CLEAR_DATA_BUF 0x040000e0
#define MIi_DUMMY_CNT (MI_DMA_ENABLE | MI_DMA_SRC_FIX | MI_DMA_DEST_FIX | 1)

void MIi_DmaSetParameters(u32 dmaNo, uintptr_t src, uintptr_t dest, u32 ctrl, u32 flags) {
    OSIntrMode interrupts;
    MIDmaRegs *dma;

    if (!(flags & MIi_DMA_MODE_NOINT)) {
        interrupts = OS_DisableInterrupts();
    }

    dma = &MI_DMA_REGS[dmaNo];
    if (flags & MIi_DMA_MODE_SRC32) {
        vu32 *fill = (vu32 *)(MIi_DMA_CLEAR_DATA_BUF + dmaNo * sizeof(u32));
        *fill = src;
        src = (uintptr_t)fill;
    } else if (flags & MIi_DMA_MODE_SRC16) {
        vu16 *fill = (vu16 *)(MIi_DMA_CLEAR_DATA_BUF + dmaNo * sizeof(u32));
        *fill = src;
        src = (uintptr_t)fill;
    }

    dma->src = src;
    dma->dest = dest;
    dma->cnt = ctrl;

    if (flags & MIi_DMA_MODE_WAIT) {
        (void)MI_DMA_REGS[0].src;
        (void)MI_DMA_REGS[0].src;
        if (!(flags & MIi_DMA_MODE_NOCLEAR) && dmaNo == 0) {
            dma->src = 0;
            dma->dest = 0;
            dma->cnt = MIi_DUMMY_CNT;
        }
    }

    if (!(flags & MIi_DMA_MODE_NOINT)) {
        OS_RestoreInterrupts(interrupts);
    }
    if (flags & MIi_DMA_MODE_WAIT) {
        (void)MI_DMA_REGS[0].src;
        (void)MI_DMA_REGS[0].src;
    }
}
