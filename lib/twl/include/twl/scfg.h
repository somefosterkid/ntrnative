// TwlSDK's SCFG, the DSi's system configuration: the prototypes of Black's functions.
#ifndef BWPORT_TWL_SCFG_H
#define BWPORT_TWL_SCFG_H

#include "types.h"

// SCFG_CLK, the DSi's clock register: bit 0 doubles the ARM9's clock, bit 1 runs the DSP, bit 2 the camera's interface and
// bit 8 the camera
#define reg_SCFG_CLK (*(vu16 *)0x04004004)
#define SCFG_CLK_CPU_2X 0x0001
#define SCFG_CLK_DSP 0x0002
#define SCFG_CLK_CAMERA_INTERFACE 0x0004
#define SCFG_CLK_CAMERA 0x0100

// SCFG_RST: bit 0 takes the DSP out of its reset, which it is held in while the bit is low
#define reg_SCFG_RST (*(vu16 *)0x04004006)

#define SCFG_RST_DSP 0x0001

typedef enum SCFGCpuSpeed {
    SCFG_CPU_SPEED_1X = 0,
    SCFG_CPU_SPEED_2X = 1,
} SCFGCpuSpeed;

// The words of the SCFG's PXI service: the command in bits 20 to 25, the ordinal of a part in bits 16 to 19 and 16 bits
// of data (libntr's twl/scfg/common/scfg.h)
#define SCFG_PXI_COMMAND_MASK 0x03f00000
#define SCFG_PXI_COMMAND_SHIFT 20
#define SCFG_PXI_ORDINAL_MASK 0x000f0000
#define SCFG_PXI_ORDINAL_SHIFT 16
#define SCFG_PXI_DATA_MASK 0x0000ffff
#define SCFG_PXI_DATA_SHIFT 0

typedef enum {
    SCFGi_PXI_COMMAND_READ = 1,
    SCFGi_PXI_COMMAND_READ_OP = 2
} SCFGPxiCommand;

#define SCFG_CPU_SPEED_MASK SCFG_CLK_CPU_2X

static inline SCFGCpuSpeed SCFG_GetCpuSpeed(void) {
    return (SCFGCpuSpeed)(reg_SCFG_CLK & SCFG_CPU_SPEED_MASK);
}

// Called with the ARM7's answer to the request that registered it: the 64 bits of data that it sent in four words, or
// a single 16-bit value in the low bits
typedef void (*SCFGCallback)(u64 response, void *arg);

// scfg_proc.c: SCFG_Init, which sets the PXI callback of the SCFG's tag
void func_0208c960(void);
// SCFG_SetCpuSpeed: 1 doubles the DSi's ARM9 clock (and OS_GetTick's rate)
void func_0208c988(SCFGCpuSpeed cpuSpeed);
// scfg_proc_itcm.c: SCFGi_SwitchCpuSpeed, in the ITCM
void func_01ff87d4(SCFGCpuSpeed speed);

#endif // BWPORT_TWL_SCFG_H
