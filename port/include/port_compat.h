// pokebw2's SDK spellings (lib/nitro/include, lib/twl/include, lib/nnsys/include) that the game's C still uses and
// libntr or libntrsystem spell otherwise or lack: macros and declarations over libntr's names, each as our header has
// it. Only names: a definition with a body goes in port/src (sdk_*.c, stub_*.c), and a name port/src defines as a
// function must not be a macro here (it would rename that definition too).
#ifndef BWPORT_PORT_COMPAT_H
#define BWPORT_PORT_COMPAT_H

// nitro/gx.h
#define GX_PLANEMASK_ALL 0x1f
// registers and masks by pokebw2's names (libntr's PC registers are the s_reg_* variables)
#ifndef GX_FX16_FX10
#define GX_FX16_FX10(x) ((fx16)((x) >> 3))
#endif

// nitro/fx.h: FX_DEG_TO_IDX
#define DEG_TO_IDX(deg) ((u16)((deg) * 0x10000 / 360))

// nitro/os.h
#define OS_HEAP_INVALID (-1)

// nitro/mi.h: Black's own SDK addition (port/src/sdk_mi.c defines it; libntr declares it for ARM only)
void MIi_CpuClearFast(u32 value, void *dest, u32 size);

// nitro/mi.h: the DMA channels' registers as pokebw2's array (lib/gfl/src/dma_vblank.c). libntr's PC registers are
// the variables s_reg_MI_DMA0SAD, DMA0DAD, DMA0CNT ... DMA3CNT, defined one after another as the DS lays them out:
// its own MI_DMA_REGADDR (nitro/mi/dma.h) reads them as this array
typedef struct MIDmaRegs {
    vu32 src;
    vu32 dest;
    vu32 cnt;
} MIDmaRegs;
#define MI_DMA_REGS ((MIDmaRegs *)REG_DMA0SAD_ADDR)

// nitro/card.h
#ifndef CARD_BACKUP_TYPE_FLASH_4MBITS_EX
#define CARD_BACKUP_TYPE_FLASH_4MBITS_EX CARD_BACKUP_TYPE_DEFINE(FLASH, 19, 0xff)
#endif

// TwlSDK's MIC limited sampling: libntr defines it (spi/src/mic.c) and its headers don't declare it
MICResult MIC_StartLimitedSampling(const MICAutoParam *param);
MICResult MIC_StopLimitedSampling(void);
MICResult MIC_StartLimitedSamplingAsync(const MICAutoParam *param, MICCallback callback, void *arg);
MICResult MIC_StopLimitedSamplingAsync(MICCallback callback, void *arg);

// twl/scfg.h: TwlSDK's SCFG_SetCpuSpeed by its address name
#define func_0208c988(cpuSpeed) SCFG_SetCpuSpeed(cpuSpeed)
// twl/sndex.h: TwlSDK's SNDEX IIR filter (DSi only: src/system/snd_mic.c calls it when OS_IsRunOnTwl; port/src stubs it)
SNDEXResult func_027047c0(int target, const u16 *coefficients);


#endif // BWPORT_PORT_COMPAT_H
