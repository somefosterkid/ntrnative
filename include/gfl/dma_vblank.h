// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/dma_vblank.h.
#ifndef BWPORT_GFL_DMA_VBLANK_H
#define BWPORT_GFL_DMA_VBLANK_H

#include "types.h"

// Writes the DMA channel settings that were requested, which GFL_VBlankUpdate calls. The table is never set up
void func_02050a70(void);

#endif // BWPORT_GFL_DMA_VBLANK_H
