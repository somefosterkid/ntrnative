// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/include/twl/mi.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_TWL_MI_H
#define BWPORT_TWL_MI_H

#include "types.h"

typedef enum {
    MI_WRAM_A,
    MI_WRAM_B,
    MI_WRAM_C,
} MIWram;

typedef enum {
    MI_WRAM_ARM9,
    MI_WRAM_ARM7,
    MI_WRAM_DSP,
    MI_WRAM_FREE, // a bank that no processor has (TwlSDK's sharedWram.h)
} MIWramProc;

// From TwlSDK's twl/mi/common/sharedWram.h: whether a bank is enabled, a size in WRAM B's 32 KB slots, and the banks
// of each WRAM and their sizes
typedef enum {
    MI_WRAM_ENABLE = TRUE,
    MI_WRAM_DISABLE = FALSE,
} MIWramEnable;

typedef enum {
    MI_WRAM_SIZE_0KB,
    MI_WRAM_SIZE_32KB,
    MI_WRAM_SIZE_64KB,
    MI_WRAM_SIZE_96KB,
    MI_WRAM_SIZE_128KB,
    MI_WRAM_SIZE_160KB,
    MI_WRAM_SIZE_192KB,
    MI_WRAM_SIZE_224KB,
    MI_WRAM_SIZE_256KB,
} MIWramSize;

#define MI_WRAM_A_MAX_NUM 4
#define MI_WRAM_B_MAX_NUM 8
#define MI_WRAM_C_MAX_NUM 8

#define MI_WRAM_A_SLOT_SIZE 0x10000
#define MI_WRAM_B_SLOT_SIZE 0x8000
#define MI_WRAM_C_SLOT_SIZE 0x8000

// TwlSDK's twl/hw/ARM9/ioreg_MI.h: a bank's control byte (its master, and the enable bit), and where each WRAM is
// mapped in the ARM9's memory (the start in units of a slot)
#define REG_MBK_A0_ADDR 0x04004040
#define REG_MBK_B0_ADDR 0x04004044
#define REG_MBK_C0_ADDR 0x0400404c
#define REG_MI_MBK_A0_M_MASK 0x01
#define REG_MI_MBK_B0_M_MASK 0x03
#define REG_MI_MBK_C0_M_MASK 0x03
#define REG_MI_MBK_A0_E_MASK 0x80
#define REG_MI_MBK_B0_E_MASK 0x80
#define REG_MI_MBK_C0_E_MASK 0x80
#define MI_WRAM_MASTER_MASK_A REG_MI_MBK_A0_M_MASK
#define MI_WRAM_MASTER_MASK_B REG_MI_MBK_B0_M_MASK
#define MI_WRAM_MASTER_MASK_C REG_MI_MBK_C0_M_MASK
#define MI_WRAM_ENABLE_MASK_A REG_MI_MBK_A0_E_MASK
#define MI_WRAM_ENABLE_MASK_B REG_MI_MBK_B0_E_MASK
#define MI_WRAM_ENABLE_MASK_C REG_MI_MBK_C0_E_MASK

#define reg_MI_MBK6 (*(vu32 *)0x04004054)
#define reg_MI_MBK7 (*(vu32 *)0x04004058)
#define reg_MI_MBK8 (*(vu32 *)0x0400405c)
#define REG_MI_MBK6_WA_SADDR_SHIFT 4
#define REG_MI_MBK6_WA_SADDR_MASK 0x00000ff0
#define REG_MI_MBK7_WB_SADDR_SHIFT 3
#define REG_MI_MBK7_WB_SADDR_MASK 0x00000ff8
#define REG_MI_MBK8_WC_SADDR_SHIFT 3
#define REG_MI_MBK8_WC_SADDR_MASK 0x00000ff8

typedef void (*MINDmaCallback)(void *arg);

// The banks of the shared WRAM B and C go from one processor to another by their slots. One byte per bank of WRAM B:
// its master, and the slot it has in the master's memory. WRAM C's follow
#define reg_MBK_B ((vu8 *)0x04004044)

#define reg_MBK_C ((vu8 *)0x0400404c)

int MI_FreeWram(MIWram wram, MIWramProc proc);
// Gives count free banks from bank to proc
int MI_AllocWramSlot(MIWram wram, int bank, int count, MIWramProc proc);
// Takes the banks back from proc
int MI_FreeWramSlot(MIWram wram, int bank, int count, MIWramProc proc);
// Whether the bank is given to a processor
BOOL MIi_IsWramSlotUsed(MIWram wram, int bank);
// The processor that the hardware gives the bank to
MIWramProc MI_GetWramBankMaster(MIWram wram, int bank);
// Where WRAM B is in the ARM9's memory, WRAM C, and either of them
void *MI_GetWramMapStart_B(void);
void *MI_GetWramMapStart_C(void);
void *MI_GetWramMapStart(MIWram wram);

// The setup of the shared WRAM's manager, which NitroSDK's MI_Init (mi_sharedWram.c's MI_InitWramManager) calls on a
// DSi
void MIi_InitWramManagerCore(void);

// TwlSDK's twl/mi/common/dma.h (libntr's reconstruction) and the registers of twl/hw/ARM9/ioreg_MI.h it uses: the
// DSi's four new DMA (NDMA) channels, their settings and the functions of mi_ndma.c and mi_ndma_gxcommand.c
#define REG_NDMAGCNT_ADDR 0x04004100
#define reg_MI_NDMAGCNT (*(vu32 *)REG_NDMAGCNT_ADDR)
#define REG_NDMA0SAD_ADDR 0x04004104

#define REG_MI_NDMAGCNT_ARBITER_SHIFT 31
#define REG_MI_NDMAGCNT_ARBITER_MASK 0x80000000
#define REG_MI_NDMAGCNT_CPUCYCLE_SHIFT 16
#define REG_MI_NDMAGCNT_CPUCYCLE_MASK 0x000f0000
#define REG_MI_NDMA0BCNT_PS_SHIFT 16
#define REG_MI_NDMA0CNT_E_SHIFT 31
#define REG_MI_NDMA0CNT_E_MASK 0x80000000
#define REG_MI_NDMA0CNT_I_SHIFT 30
#define REG_MI_NDMA0CNT_CM_SHIFT 29
#define REG_MI_NDMA0CNT_IM_SHIFT 28
#define REG_MI_NDMA0CNT_MODE_SHIFT 24
#define REG_MI_NDMA0CNT_WORDCNT_SHIFT 16
#define REG_MI_NDMA0CNT_SRL_SHIFT 15
#define REG_MI_NDMA0CNT_SAR_SHIFT 13
#define REG_MI_NDMA0CNT_DRL_SHIFT 12
#define REG_MI_NDMA0CNT_DAR_SHIFT 10

#define MI_NDMA_MAX_NUM 3

#define MI_NDMA_ARBITRAMENT_FIX (0UL << REG_MI_NDMAGCNT_ARBITER_SHIFT)
#define MI_NDMA_ARBITRAMENT_ROUND (1UL << REG_MI_NDMAGCNT_ARBITER_SHIFT)
#define MI_NDMA_RCYCLE_MASK (REG_MI_NDMAGCNT_CPUCYCLE_MASK)
#define MI_NDMA_RCYCLE_32 (6UL << REG_MI_NDMAGCNT_CPUCYCLE_SHIFT)
#define MI_NDMA_RCYCLE_DEFAULT MI_NDMA_RCYCLE_32

#define MI_NDMA_ENABLE_MASK (1UL << REG_MI_NDMA0CNT_E_SHIFT)
#define MI_NDMA_ENABLE (1UL << REG_MI_NDMA0CNT_E_SHIFT)
#define MI_NDMA_DISABLE (0UL << REG_MI_NDMA0CNT_E_SHIFT)
#define MI_NDMA_IF_ENABLE (1UL << REG_MI_NDMA0CNT_I_SHIFT)
#define MI_NDMA_IF_DISABLE (0UL << REG_MI_NDMA0CNT_I_SHIFT)
#define MI_NDMA_CONTINUOUS_ON (1UL << REG_MI_NDMA0CNT_CM_SHIFT)
#define MI_NDMA_CONTINUOUS_OFF (0UL << REG_MI_NDMA0CNT_CM_SHIFT)
#define MI_NDMA_IMM_MODE_ON (1UL << REG_MI_NDMA0CNT_IM_SHIFT)
#define MI_NDMA_IMM_MODE_OFF (0UL << REG_MI_NDMA0CNT_IM_SHIFT)

#define MI_NDMA_TIMING_CARD (4UL << REG_MI_NDMA0CNT_MODE_SHIFT)
#define MI_NDMA_TIMING_V_BLANK (6UL << REG_MI_NDMA0CNT_MODE_SHIFT)
#define MI_NDMA_TIMING_H_BLANK (7UL << REG_MI_NDMA0CNT_MODE_SHIFT)
#define MI_NDMA_TIMING_GXFIFO (10UL << REG_MI_NDMA0CNT_MODE_SHIFT)
#define MI_NDMA_TIMING_CAMERA (11UL << REG_MI_NDMA0CNT_MODE_SHIFT)
#define MIi_NDMA_TIMING_IMMIDIATE 0xffffffff

#define MI_NDMA_BWORD_16 (4UL << REG_MI_NDMA0CNT_WORDCNT_SHIFT)

#define MI_NDMA_SRC_RELOAD_ENABLE (1UL << REG_MI_NDMA0CNT_SRL_SHIFT)
#define MI_NDMA_SRC_RELOAD_DISABLE (0UL << REG_MI_NDMA0CNT_SRL_SHIFT)
#define MI_NDMA_SRC_INC (0UL << REG_MI_NDMA0CNT_SAR_SHIFT)
#define MI_NDMA_SRC_DEC (1UL << REG_MI_NDMA0CNT_SAR_SHIFT)
#define MI_NDMA_SRC_FIX (2UL << REG_MI_NDMA0CNT_SAR_SHIFT)
#define MI_NDMA_SRC_FILLDATA (3UL << REG_MI_NDMA0CNT_SAR_SHIFT)
#define MI_NDMA_DEST_RELOAD_ENABLE (1UL << REG_MI_NDMA0CNT_DRL_SHIFT)
#define MI_NDMA_DEST_RELOAD_DISABLE (0UL << REG_MI_NDMA0CNT_DRL_SHIFT)
#define MI_NDMA_DEST_INC (0UL << REG_MI_NDMA0CNT_DAR_SHIFT)
#define MI_NDMA_DEST_DEC (1UL << REG_MI_NDMA0CNT_DAR_SHIFT)
#define MI_NDMA_DEST_FIX (2UL << REG_MI_NDMA0CNT_DAR_SHIFT)

#define MI_NDMA_INTERVAL_PS_1 (0UL << REG_MI_NDMA0BCNT_PS_SHIFT)

// A channel's registers, as words from its source address. The address is a byte sum, as pokebw2 has it: Black
// multiplies the channel by 0x1c (a mul), where libntr's word sum (MI_NDMA_NUM_WOFFSET added to a vu32 pointer)
// compiles to a shift and a subtract
#define MI_NDMA_NUM_WOFFSET(ndmaNo) (ndmaNo * 7)
#define MI_NDMA_REG_SAD_WOFFSET 0
#define MI_NDMA_REG_DAD_WOFFSET 1
#define MI_NDMA_REG_TCNT_WOFFSET 2
#define MI_NDMA_REG_WCNT_WOFFSET 3
#define MI_NDMA_REG_BCNT_WOFFSET 4
#define MI_NDMA_REG_FDATA_WOFFSET 5
#define MI_NDMA_REG_CNT_WOFFSET 6
#define MI_NDMA_REGADDR(ndmaNo, reg) ((vu32 *)(REG_NDMA0SAD_ADDR + (ndmaNo) * 0x1c + (reg) * 4))
#define MI_NDMA_REG(ndmaNo, reg) (*(MI_NDMA_REGADDR(ndmaNo, reg)))

#define MI_NDMA_NO_INTERVAL 0
#define MI_NDMA_AT_A_TIME 0xffffffff

typedef struct _MINDmaConfig {
    u32 intervalTimer;
    u32 prescaler;
    u32 blockWord;
    u32 wordCount;
} MINDmaConfig;

typedef u32 MINDmaDevice;

void MI_InitNDma(void);
void MI_NDmaFill(u32 ndmaNo, void *dest, u32 data, u32 size);
void MI_NDmaCopy(u32 ndmaNo, const void *src, void *dest, u32 size);
void MI_NDmaFillAsync(u32 ndmaNo, void *dest, u32 data, u32 size, MINDmaCallback callback, void *arg);
void MI_NDmaCopyAsync(u32 ndmaNo, const void *src, void *dest, u32 size, MINDmaCallback callback, void *arg);
BOOL MI_IsNDmaBusy(u32 ndmaNo);
void MI_WaitNDma(u32 ndmaNo);
void MI_StopNDma(u32 ndmaNo);
void MI_StopAllNDma(void);
void MI_SetNDmaArbitrament(u32 mode, u32 cycle);
void MI_InitNDmaConfig(void);
void MI_GetNDmaConfig(u32 ndmaNo, MINDmaConfig *config);
void MI_Card_NDmaCopy(u32 ndmaNo, const void *src, void *dest, u32 size);
void MI_Camera_NDmaRecvAsync(u32 ndmaNo, void *dest, u32 unit, u32 size, BOOL contSw, MINDmaCallback callback,
                             void *arg);
void MI_SendNDmaGXCommandAsync(u32 ndmaNo, const void *src, u32 commandLength, MINDmaCallback callback, void *arg);
void MI_SendNDmaGXCommandAsyncFast(u32 ndmaNo, const void *src, u32 commandLength, MINDmaCallback callback,
                                   void *arg);

// The rest of TwlSDK's mi_sharedWram.c that Black's link keeps, under Black's names (the SDK's in the comments)
// MIi_GetWramBank_A/B/C: a bank's control byte
vu8 func_02730210(int num);
vu8 func_02730220(int num);
vu8 func_02730230(int num);
// MIi_SetWramBankMaster_A/B/C and MIi_SetWramBankMaster
void func_02730240(int num, MIWramProc proc);
void func_0273025c(int num, MIWramProc proc);
void func_02730278(int num, MIWramProc proc);
void func_02730294(MIWram wram, int num, MIWramProc proc);
// MIi_SetWramBankEnable_A/B/C and MIi_SetWramBankEnable
void func_027302b4(int num, MIWramEnable enable);
void func_027302dc(int num, MIWramEnable enable);
void func_02730304(int num, MIWramEnable enable);
void func_0273032c(MIWram wram, int num, MIWramEnable enable);
// MI_GetWramBankMaster_A/B/C
MIWramProc func_0273034c(int num);
MIWramProc func_02730360(int num);
MIWramProc func_02730374(int num);
// MI_GetWramMapStart_A
void *func_027303a4(void);
// MI_CancelWram: cancels every reservation of proc's
int func_02730fbc(MIWram wram, MIWramProc proc);
// MI_LockWramManager and MI_UnlockWramManager: the lock word the two processors share for the manager
void func_027311e8(u16 *lockid);
void func_02731210(u16 lockid);

// TwlSDK's timing mask and the display timings of a new DMA channel
#define REG_MI_NDMA0CNT_MODE_MASK 0x0f000000
#define MI_NDMA_TIMING_MASK (REG_MI_NDMA0CNT_MODE_MASK)
#define MI_NDMA_TIMING_DISP (8UL << REG_MI_NDMA0CNT_MODE_SHIFT)
#define MI_NDMA_TIMING_DISP_MMEM (9UL << REG_MI_NDMA0CNT_MODE_SHIFT)

#endif // BWPORT_TWL_MI_H
