// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/dma_vblank.c, which is the same code in
// Black 2/White 2.
#include "types.h"
#include "gfl/dma_vblank.h"
#include "nitro/mi.h"
#include "nitro/os.h"

// Settings of the four DMA channels, written to their control registers at the vertical blank when requested. The
// game never sets up the table, so this does nothing. The file's name is a guess: there is no name for it in the ROM

#define DMA_CHANNEL_COUNT 4

typedef struct {
    const void *src;
    void *dst;
    u32 : 1;
    u32 irq : 1;
    u32 : 3;
    u32 is32bit : 1;
    u32 : 1;
    u32 srcCtrl : 2;
    u32 dstCtrl : 2;
    u32 count : 21;
    u32 update : 1;
    u32 active : 1;
    u32 stop : 1;
} DMAVBlankChannel;

static DMAVBlankChannel *sDMAChannels;

static void func_02050b14(int ch);

void func_02050a70(void) {
    int ch;

    if (sDMAChannels == NULL) {
        return;
    }
    for (ch = 0; ch < DMA_CHANNEL_COUNT; ch++) {
        if (sDMAChannels[ch].update) {
            sDMAChannels[ch].update = FALSE;
            if (sDMAChannels[ch].count != 0) {
                func_02050b14(ch);
                sDMAChannels[ch].active = TRUE;
            }
        }
        if (sDMAChannels[ch].stop) {
            sDMAChannels[ch].stop = FALSE;
            func_02050b14(ch);
        }
    }
}

static void func_02050b14(int ch) {
    u32 mode = OS_DisableInterrupts();
    DMAVBlankChannel *channel = &sDMAChannels[ch];

    MI_DMA_REGS[ch].cnt = (channel->irq << 30) | (channel->is32bit << 26) | (channel->srcCtrl << 23) |
                          (channel->dstCtrl << 21) | channel->count;
    OS_RestoreInterrupts(mode);
}
