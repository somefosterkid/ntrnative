// TwlSDK's private mi_ndma.h (libraries/mi/include/mi_ndma.h in libntr's reconstruction of TwlSDK 5): the address
// modes and kinds of an NDMA transfer, and the transfers under mi_ndma.c's and mi_ndma_gxcommand.c's functions
#ifndef BWPORT_TWL_MI_NDMA_H
#define BWPORT_TWL_MI_NDMA_H

#include "twl/mi.h"

#define MIi_SRC_INC (MI_NDMA_SRC_INC | MI_NDMA_SRC_RELOAD_DISABLE)
#define MIi_SRC_DEC (MI_NDMA_SRC_DEC | MI_NDMA_SRC_RELOAD_DISABLE)
#define MIi_SRC_FIX (MI_NDMA_SRC_FIX | MI_NDMA_SRC_RELOAD_DISABLE)
#define MIi_SRC_FILLDATA (MI_NDMA_SRC_FILLDATA | MI_NDMA_SRC_RELOAD_DISABLE)

#define MIi_DEST_INC (MI_NDMA_DEST_INC | MI_NDMA_DEST_RELOAD_DISABLE)
#define MIi_DEST_DEC (MI_NDMA_DEST_DEC | MI_NDMA_DEST_RELOAD_DISABLE)
#define MIi_DEST_FIX (MI_NDMA_DEST_FIX | MI_NDMA_DEST_RELOAD_DISABLE)
#define MIi_DEST_INC_RELOAD (MI_NDMA_SRC_INC | MI_NDMA_DEST_RELOAD_ENABLE)

#define MIi_IMM (MI_NDMA_IMM_MODE_ON)

#define MIi_CONT (MI_NDMA_CONTINUOUS_ON)

typedef enum {
    MIi_NDMA_TYPE_FILL = 0,
    MIi_NDMA_TYPE_COPY = 1,
    MIi_NDMA_TYPE_SEND = 2,
    MIi_NDMA_TYPE_RECV = 3,
    MIi_NDMA_TYPE_PIPE = 4,

    MIi_NDMA_TYPE_HBLANK = 5,
    MIi_NDMA_TYPE_HBLANK_IF = 6,
    MIi_NDMA_TYPE_MMCOPY = 7,
    MIi_NDMA_TYPE_GXCOPY = 8,
    MIi_NDMA_TYPE_GXCOPY_IF = 9,
    MIi_NDMA_TYPE_CAMERACONT = 10
} MIiNDmaType;

extern MINDmaConfig MIi_NDmaConfig[MI_NDMA_MAX_NUM + 1];

static inline void MIi_CallCallback(MINDmaCallback callback, void *arg) {
    if (callback) {
        (callback)(arg);
    }
}

void MIi_NDma_withConfig_Dev(MIiNDmaType ndmaType, u32 ndmaNo, const void *src, void *dest, u32 data, u32 size,
                             const MINDmaConfig *config, MINDmaDevice dev, u32 enable);

void MIi_NDmaAsync_withConfig_Dev(MIiNDmaType ndmaType, u32 ndmaNo, const void *src, void *dest, u32 data, u32 size,
                                  MINDmaCallback callback, void *arg, const MINDmaConfig *config, MINDmaDevice dev,
                                  u32 enable);

static inline void MIi_NDma(MIiNDmaType ndmaType, u32 ndmaNo, const void *src, void *dest, u32 data, u32 size,
                            u32 enable) {
    MIi_NDma_withConfig_Dev(ndmaType, ndmaNo, src, dest, data, size, &MIi_NDmaConfig[ndmaNo],
                            MIi_NDMA_TIMING_IMMIDIATE, enable);
}

static inline void MIi_NDmaAsync(MIiNDmaType ndmaType, u32 ndmaNo, const void *src, void *dest, u32 data, u32 size,
                                 MINDmaCallback callback, void *arg, u32 enable) {
    MIi_NDmaAsync_withConfig_Dev(ndmaType, ndmaNo, src, dest, data, size, callback, arg, &MIi_NDmaConfig[ndmaNo],
                                 MIi_NDMA_TIMING_IMMIDIATE, enable);
}

// TwlSDK's twl/mi/common/dma.h declares it (twl/mi.h leaves it out: camera_system.c has its own declaration)
void MI_Camera_NDmaRecvAsync(u32 ndmaNo, void *dest, u32 unit, u32 size, BOOL contSw, MINDmaCallback callback,
                             void *arg);

#endif // BWPORT_TWL_MI_NDMA_H
