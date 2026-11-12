// TwlSDK 5's mi_ndma.c (ARM9), in the LTD autoload's .ltdmain: the DSi's four new DMA channels, as libntr's
// reconstruction has it (pokebw2's lib/twl/src/mi/mi_ndma.c is the same, compiled as Thumb). Black's link keeps only the
// functions below. func_020848ec is main's OSi_EnterNDmaCallback
#include "nitro/mi.h"
#include "nitro/os.h"
#include "twl/mi.h"
#include "mi_ndma.h"

// TwlSDK defines it after twl/ltdmain_begin.h, which sends uninitialised data to .ltdmain.bss; this tree's link
// script places no .ltdmain.bss, so it comes before the header here (in .bss, at Black's 0x02732efc)
MINDmaConfig MIi_NDmaConfig[MI_NDMA_MAX_NUM + 1];

#include "twl/ltdmain_begin.h"

#define REG_DAT_ADDR 0x04004204

static void MIi_Wait(u32 ndmaNo);

static inline void MIi_SetSrc(u32 ndmaNo, u32 src) {
    MI_NDMA_REG(ndmaNo, MI_NDMA_REG_SAD_WOFFSET) = src;
}
static inline void MIi_SetDest(u32 ndmaNo, u32 dest) {
    MI_NDMA_REG(ndmaNo, MI_NDMA_REG_DAD_WOFFSET) = dest;
}
static inline void MIi_SetTotalWordCount(u32 ndmaNo, u32 size) {
    MI_NDMA_REG(ndmaNo, MI_NDMA_REG_TCNT_WOFFSET) = size;
}
static inline void MIi_SetWordCount(u32 ndmaNo, u32 size) {
    MI_NDMA_REG(ndmaNo, MI_NDMA_REG_WCNT_WOFFSET) = size;
}
static inline void MIi_SetInterval(u32 ndmaNo, u32 intervalTimer, u32 prescaler) {
    MI_NDMA_REG(ndmaNo, MI_NDMA_REG_BCNT_WOFFSET) = intervalTimer | prescaler;
}
static inline void MIi_SetFillData(u32 ndmaNo, u32 data) {
    MI_NDMA_REG(ndmaNo, MI_NDMA_REG_FDATA_WOFFSET) = data;
}
static inline void MIi_SetControl(u32 ndmaNo, u32 contData) {
    MI_NDMA_REG(ndmaNo, MI_NDMA_REG_CNT_WOFFSET) = contData;
}

static u32 MIi_GetControlData(MIiNDmaType ndmaType) {
    u32 contData;

    switch (ndmaType) {
    case MIi_NDMA_TYPE_FILL:
        contData = MIi_SRC_FILLDATA | MIi_DEST_INC;
        break;
    case MIi_NDMA_TYPE_COPY:
        contData = MI_NDMA_SRC_INC | MIi_DEST_INC;
        break;
    case MIi_NDMA_TYPE_SEND:
    case MIi_NDMA_TYPE_GXCOPY:
        contData = MI_NDMA_SRC_INC | MIi_DEST_FIX;
        break;
    case MIi_NDMA_TYPE_GXCOPY_IF:
        contData = MI_NDMA_SRC_INC | MIi_DEST_FIX | MI_NDMA_IF_ENABLE;
        break;
    case MIi_NDMA_TYPE_RECV:
        contData = MI_NDMA_SRC_FIX | MIi_DEST_INC;
        break;
    case MIi_NDMA_TYPE_PIPE:
        contData = MI_NDMA_SRC_FIX | MIi_DEST_FIX;
        break;
    case MIi_NDMA_TYPE_CAMERACONT:
        contData = MI_NDMA_SRC_FIX | MIi_DEST_INC_RELOAD | MIi_CONT | MI_NDMA_IF_ENABLE;
        break;
    case MIi_NDMA_TYPE_HBLANK:
        contData = MI_NDMA_SRC_INC | MIi_DEST_INC_RELOAD | MIi_CONT;
        break;
    case MIi_NDMA_TYPE_HBLANK_IF:
        contData = MI_NDMA_SRC_INC | MIi_DEST_INC_RELOAD | MIi_CONT | MI_NDMA_IF_ENABLE;
        break;
    case MIi_NDMA_TYPE_MMCOPY:
        contData = MI_NDMA_SRC_INC | MIi_DEST_FIX | MIi_CONT;
        break;
    default:
        contData = 0;
        break;
    }

    return contData;
}

void MIi_NDma_withConfig_Dev(MIiNDmaType ndmaType, u32 ndmaNo, const void *src, void *dest, u32 data, u32 size,
                             const MINDmaConfig *config, MINDmaDevice dev, u32 enable) {
    if (size > 0) {
        u32 contData;
        OSIntrMode enabled = OS_DisableInterrupts();

        MIi_Wait(ndmaNo);

        if (ndmaType != MIi_NDMA_TYPE_FILL) {
            MIi_SetSrc(ndmaNo, (u32)src);
        }
        MIi_SetDest(ndmaNo, (u32)dest);
        MIi_SetInterval(ndmaNo, config->intervalTimer, config->prescaler);
        if (ndmaType == MIi_NDMA_TYPE_FILL) {
            MIi_SetFillData(ndmaNo, data);
        }
        if (dev == MIi_NDMA_TIMING_IMMIDIATE) {
            MIi_SetWordCount(ndmaNo, size / 4);
        } else {
            MIi_SetTotalWordCount(ndmaNo, size / 4);
            MIi_SetWordCount(ndmaNo, (config->wordCount == MI_NDMA_AT_A_TIME) ? size / 4 : config->wordCount);
        }

        contData = config->blockWord | (enable & MI_NDMA_ENABLE_MASK);
        contData |= MIi_GetControlData(ndmaType);
        contData |= (dev == MIi_NDMA_TIMING_IMMIDIATE) ? MIi_IMM : dev;

        MIi_SetControl(ndmaNo, contData);

        MIi_Wait(ndmaNo);

        (void)OS_RestoreInterrupts(enabled);
    }
}

void MIi_NDmaAsync_withConfig_Dev(MIiNDmaType ndmaType, u32 ndmaNo, const void *src, void *dest, u32 data, u32 size,
                                  MINDmaCallback callback, void *arg, const MINDmaConfig *config, MINDmaDevice dev,
                                  u32 enable) {
    if (size == 0) {
        MIi_CallCallback(callback, arg);
    } else {
        u32 contData;
        OSIntrMode enabled = OS_DisableInterrupts();

        MIi_Wait(ndmaNo);

        if (callback) {
            func_020848ec(ndmaNo, callback, arg);
        }

        if (ndmaType != MIi_NDMA_TYPE_FILL) {
            MIi_SetSrc(ndmaNo, (u32)src);
        }
        MIi_SetDest(ndmaNo, (u32)dest);
        MIi_SetInterval(ndmaNo, config->intervalTimer, config->prescaler);
        if (ndmaType == MIi_NDMA_TYPE_FILL) {
            MIi_SetFillData(ndmaNo, data);
        }
        if (dev == MIi_NDMA_TIMING_IMMIDIATE) {
            MIi_SetWordCount(ndmaNo, size / 4);
        } else {
            MIi_SetTotalWordCount(ndmaNo, size / 4);
            MIi_SetWordCount(ndmaNo, (config->wordCount == MI_NDMA_AT_A_TIME) ? size / 4 : config->wordCount);
        }

        contData = config->blockWord | (enable & MI_NDMA_ENABLE_MASK);
        contData |= MIi_GetControlData(ndmaType);
        contData |= (dev == MIi_NDMA_TIMING_IMMIDIATE) ? MIi_IMM : dev;

        if (callback) {
            contData |= MI_NDMA_IF_ENABLE;
        }

        MIi_SetControl(ndmaNo, contData);

        (void)OS_RestoreInterrupts(enabled);
    }
}

void MI_InitNDma(void) {
    MI_SetNDmaArbitrament(MI_NDMA_ARBITRAMENT_ROUND, MI_NDMA_RCYCLE_DEFAULT);
    MI_InitNDmaConfig();
}

void MI_NDmaFill(u32 ndmaNo, void *dest, u32 data, u32 size) {
    MIi_NDma(MIi_NDMA_TYPE_FILL, ndmaNo, 0, /*Not used*/
             dest, data, size, MI_NDMA_ENABLE);
}

void MI_NDmaCopy(u32 ndmaNo, const void *src, void *dest, u32 size) {
    MIi_NDma(MIi_NDMA_TYPE_COPY, ndmaNo, src, dest, 0, /*Not used*/
             size, MI_NDMA_ENABLE);
}

void MI_NDmaFillAsync(u32 ndmaNo, void *dest, u32 data, u32 size, MINDmaCallback callback, void *arg) {
    MIi_NDmaAsync(MIi_NDMA_TYPE_FILL, ndmaNo, 0, /*Not used*/
                  dest, data, size, callback, arg, MI_NDMA_ENABLE);
}

void MI_NDmaCopyAsync(u32 ndmaNo, const void *src, void *dest, u32 size, MINDmaCallback callback, void *arg) {
    MIi_NDmaAsync(MIi_NDMA_TYPE_COPY, ndmaNo, src, dest, 0, /*Not used*/
                  size, callback, arg, MI_NDMA_ENABLE);
}

BOOL MI_IsNDmaBusy(u32 ndmaNo) {
    {
        vu32 *dmaCntp = MI_NDMA_REGADDR(ndmaNo, MI_NDMA_REG_CNT_WOFFSET);
        return (BOOL)((*dmaCntp & REG_MI_NDMA0CNT_E_MASK) >> REG_MI_NDMA0CNT_E_SHIFT);
    }
}

void MI_WaitNDma(u32 ndmaNo) {
    {
        OSIntrMode enabled = OS_DisableInterrupts();
        vu32 *dmaCntp = MI_NDMA_REGADDR(ndmaNo, MI_NDMA_REG_CNT_WOFFSET);
        while (*dmaCntp & REG_MI_DMA0CNT_E_MASK) {
        }
        (void)OS_RestoreInterrupts(enabled);
    }
}

void MI_StopNDma(u32 ndmaNo) {
    OSIntrMode enabled = OS_DisableInterrupts();

    vu32 *regCont = MI_NDMA_REGADDR(ndmaNo, MI_NDMA_REG_CNT_WOFFSET);

    *regCont = *regCont & (~MI_NDMA_ENABLE);

    {
        u32 dummy = *regCont;
    }
    {
        u32 dummy = *regCont;
    }

    (void)OS_RestoreInterrupts(enabled);
}

void MI_StopAllNDma(void) {
    MI_StopNDma(0);
    MI_StopNDma(1);
    MI_StopNDma(2);
    MI_StopNDma(3);
}

static void MIi_Wait(u32 ndmaNo) {
    while (MI_IsNDmaBusy(ndmaNo) == TRUE) {
    }
}

void MI_SetNDmaArbitrament(u32 mode, u32 cycle) {
    if (mode == MI_NDMA_ARBITRAMENT_ROUND) {
        reg_MI_NDMAGCNT = MI_NDMA_ARBITRAMENT_ROUND | cycle;
    } else {
        reg_MI_NDMAGCNT = MI_NDMA_ARBITRAMENT_FIX;
    }
}

void MI_InitNDmaConfig(void) {
    MINDmaConfig *p = &MIi_NDmaConfig[0];

    int n;
    for (n = 0; n <= MI_NDMA_MAX_NUM; n++) {
        p->intervalTimer = 1;
        p->prescaler = MI_NDMA_INTERVAL_PS_1;
        p->blockWord = MI_NDMA_BWORD_16;
        p->wordCount = MI_NDMA_AT_A_TIME;
        p++;
    }
}

void MI_GetNDmaConfig(u32 ndmaNo, MINDmaConfig *config) {
    MI_CpuCopy32(&MIi_NDmaConfig[ndmaNo], config, sizeof(MINDmaConfig));
}

static void MIi_CardNDmaRecv32(u32 ndmaNo, const void *src, void *dest) {
    {
        const MINDmaConfig *config = &MIi_NDmaConfig[ndmaNo];
        const u32 size = sizeof(u32);
        OSIntrMode enabled = OS_DisableInterrupts();
        u32 contData;
        MIi_Wait(ndmaNo);
        MIi_SetSrc(ndmaNo, (u32)src);
        MIi_SetDest(ndmaNo, (u32)dest);
        MIi_SetTotalWordCount(ndmaNo, size / sizeof(u32));
        MIi_SetWordCount(ndmaNo, size / sizeof(u32));
        contData = (config->blockWord | MI_NDMA_TIMING_CARD | MI_NDMA_SRC_FIX | MI_NDMA_DEST_INC |
                    MI_NDMA_DEST_RELOAD_DISABLE | MI_NDMA_ENABLE | MI_NDMA_CONTINUOUS_ON);
        MIi_SetControl(ndmaNo, contData);

        (void)OS_RestoreInterrupts(enabled);
    }
}

void MI_Card_NDmaCopy(u32 ndmaNo, const void *src, void *dest, u32 size) {
    (void)size;
    MIi_CardNDmaRecv32(ndmaNo, src, dest);
}

#define CAMERA_DMA_BLOCK_SIZE MI_NDMA_BWORD_16
#define CAMERA_DMA_INTERVAL 2
#define CAMERA_DMA_PRESCALER MI_NDMA_INTERVAL_PS_1

void MI_Camera_NDmaRecvAsync(u32 ndmaNo, void *dest, u32 unit, u32 size, BOOL contSw, MINDmaCallback callback,
                             void *arg) {
    MINDmaConfig config;

    MIiNDmaType type = (contSw) ? MIi_NDMA_TYPE_CAMERACONT : MIi_NDMA_TYPE_RECV;

    MI_GetNDmaConfig(ndmaNo, &config);
    config.intervalTimer = CAMERA_DMA_INTERVAL;
    config.prescaler = CAMERA_DMA_PRESCALER;
    config.blockWord = CAMERA_DMA_BLOCK_SIZE;
    config.wordCount = unit;

    MIi_NDmaAsync_withConfig_Dev(type, ndmaNo, (void *)REG_DAT_ADDR, dest, 0 /*Not used*/, size, callback, arg,
                                 &config, MI_NDMA_TIMING_CAMERA, MI_NDMA_ENABLE);
}

#include "twl/ltdmain_end.h"
