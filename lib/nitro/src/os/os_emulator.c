// TwlSDK's OS library, os_emulator.c: Black's main 0x02085cbc-0x02085d94, the console and device type. The file is
// libntr's (MIT, https://github.com/cybervisi0n/libntr) for SDK 5, as TwlSDK builds it for a final ROM that also runs
// on a DSi (SDK_FINALROM, SDK_TWLHYB): its os_arena.c calls OS_GetConsoleType where Black's calls the first function,
// and OSi_DetectDeviceType is kh-recoded-decomp's twin's name (MIT). Black's link drops OS_GetRunningConsoleType,
// OSi_DetectEmulator, OSi_DetectPlatform and OSi_IsNitroModeOnTwl (nothing calls them), but their statics stay in the
// file's .data and .bss: OSi_RunningConsoleTypeCache is the first word of .data (0x020a86a4, before the cache that
// OS_GetConsoleType reads at base + 4), and the two statics of each of the other three are .bss 0x02151244 and
// 0x0215124c-0x02151260, around OS_IsRunOnTwl's (base + 4 and + 0x1c, which its code reads). They are written as the SDK
// has them, so that the live functions' statics land where Black has them: without any one of them OS_IsRunOnTwl (and
// without the first OS_GetConsoleType) no longer matches. The SDK's OSi_DetectDebugger and
// OS_IsRunOnDebugger, which have no statics, are left out. Built with -ipa file, as the SDK was: the statics are
// reached from their section's base.

#include "types.h"
#include "nitro/os.h"

#define OSi_CONSOLE_NOT_DETECT 0xffffffff

#define OS_CONSOLE_TWL 0x01000000
#define OS_CONSOLE_TWLDEBUGGER 0x02000000
#define OS_CONSOLE_ISDEBUGGER 0x40000000
#define OS_CONSOLE_NITRO 0x80000000
#define OS_CONSOLE_BOARD_A9_A7 0x00100000
#define OS_CONSOLE_BOARD_A9 0x00200000
#define OS_CONSOLE_TWLTYPE_RETAIL 0x00800000
#define OS_CONSOLE_TWLTYPE_DEV 0x00400000
#define OS_CONSOLE_EVALUATE 0x00080000
#define OS_CONSOLE_DEV_CARD 0x00020000
#define OS_CONSOLE_DEV_DOWNLOAD 0x00004000
#define OS_CONSOLE_DEV_NAND 0x00040000
#define OS_CONSOLE_DEV_MEMORY 0x00008000
#define OS_CONSOLE_SIZE_MASK 0x0000000f
#define OS_CONSOLE_SIZE_4MB 0x00000001
#define OS_CONSOLE_SIZE_8MB 0x00000002
#define OS_CONSOLE_SIZE_16MB 0x00000004
#define OS_CONSOLE_SIZE_32MB 0x00000008

// TwlSDK's shared main memory (twl/hw/common/mmap_shared.h) and SCFG_A9ROM
#define HW_MMEMCHECKER_SUB 0x02fffffa
#define HW_SYS_CONF_BUF 0x02fffdf0
#define HW_CHECK_DEBUGGER_BUF2 0x027ffc14
#define reg_SCFG_A9ROM (*(vu8 *)0x04004000)
#define REG_SCFG_A9ROM_SEC_MASK 0x01
#define REG_SCFG_A9ROM_RSEL_MASK 0x02

#define OS_CHIPTYPE_DEBUGGER_ADDR (HW_SYS_CONF_BUF + 4)
#define OS_CHIPTYPE_DEBUGGER_MASK 0x3
#define OS_CHIPTYPE_JTAG_ADDR (HW_SYS_CONF_BUF + 5)
#define OS_CHIPTYPE_JTAG_MASK 0x2
#define OS_CHIPTYPE_SMX_ADDR (HW_MMEMCHECKER_SUB)
#define OS_CHIPTYPE_SMX_MASK 0x8000

enum {
    OS_CHIPTYPE_TWL = 0,
    OS_CHIPTYPE_DEBUGGER_1 = 1,
    OS_CHIPTYPE_DEBUGGER_2 = 2,
    OS_CHIPTYPE_EVALUATE = 3
};

#define memorySize() (*(u16 *)HW_MMEMCHECKER_SUB & OS_CONSOLE_SIZE_MASK)
#define soundMixFlag() (*(u16 *)OS_CHIPTYPE_SMX_ADDR & OS_CHIPTYPE_SMX_MASK)

static u32 OSi_DetectDeviceType(void);
static u32 OSi_DetectPlatform(void);
u32 OS_GetRunningConsoleType(void);
u32 OSi_DetectEmulator(void);
BOOL OSi_IsRunOnTwl(void);
BOOL OSi_IsNitroModeOnTwl(void);

u32 OS_GetConsoleType(void) {
    static u32 OSi_ConsoleTypeCache = OSi_CONSOLE_NOT_DETECT;

    if (OSi_ConsoleTypeCache == OSi_CONSOLE_NOT_DETECT) {
        // TwlSDK's OS_IsRunOnTwl (pokebw2's hw_isDSi)
        if (OS_IsRunOnTwl()) {
            OSi_ConsoleTypeCache = OS_CONSOLE_TWL | OS_CONSOLE_TWLTYPE_RETAIL | OS_CONSOLE_SIZE_16MB;
        } else {
            OSi_ConsoleTypeCache = OS_CONSOLE_NITRO | OS_CONSOLE_SIZE_4MB;
        }
        OSi_ConsoleTypeCache |= OSi_DetectDeviceType();
    }
    return OSi_ConsoleTypeCache;
}

static u32 OSi_RunningConsoleTypeCache = OSi_CONSOLE_NOT_DETECT;

// Black's link drops it
u32 OS_GetRunningConsoleType(void) {
    while (*(vu16 *)HW_MMEMCHECKER_SUB == 0) {
        SVC_WaitByLoop(0x100 / 4);
    }

    if (OSi_RunningConsoleTypeCache == OSi_CONSOLE_NOT_DETECT) {
        u32 emulator = OSi_DetectEmulator();

        OSi_RunningConsoleTypeCache = ((emulator) ? emulator : OSi_DetectPlatform()) | OSi_DetectDeviceType() | memorySize();
    }
    return OSi_RunningConsoleTypeCache;
}

static u32 OSi_DetectDeviceType(void) {
    static const u32 table[] = {
        0,                       // for OS_BOOTTYPE_ILLEGAL
        OS_CONSOLE_DEV_CARD,     // for OS_BOOTTYPE_ROM
        OS_CONSOLE_DEV_DOWNLOAD, // for OS_BOOTTYPE_DOWNLOAD_MB
        OS_CONSOLE_DEV_NAND,     // for OS_BOOTTYPE_NAND
        OS_CONSOLE_DEV_MEMORY,   // for OS_BOOTTYPE_MEMORY
    };

    return table[OS_GetBootType()];
}

// Black's link drops it
u32 OSi_DetectEmulator(void) {
    static u32 OSi_IsDetectedEmulator = FALSE;
    static u32 OSi_Emulator;

    if (!OSi_IsDetectedEmulator) {
        OSi_Emulator = 0;
        OSi_IsDetectedEmulator = TRUE;
    }
    return OSi_Emulator;
}

// Black's link drops it
static u32 OSi_DetectPlatform(void) {
    static u32 OSi_IsDetectedPlatform = FALSE;
    static u32 OSi_Platform;

    if (!OSi_IsDetectedPlatform) {
        switch (*(u8 *)(OS_CHIPTYPE_DEBUGGER_ADDR) & OS_CHIPTYPE_DEBUGGER_MASK) {
        case OS_CHIPTYPE_TWL: {
            switch (memorySize()) {
            case OS_CONSOLE_SIZE_4MB:
                OSi_Platform = OSi_IsNitroModeOnTwl() ? (OS_CONSOLE_TWL | OS_CONSOLE_TWLTYPE_RETAIL) : OS_CONSOLE_NITRO;
                break;
            case OS_CONSOLE_SIZE_8MB: {
                int isDebuggerFlag = *(u16 *)HW_CHECK_DEBUGGER_BUF2;
                if (isDebuggerFlag == 1) {
                    OSi_Platform = OS_CONSOLE_ISDEBUGGER;
                } else {
                    OSi_Platform = 0;
                }
            } break;
            case OS_CONSOLE_SIZE_16MB:
                OSi_Platform = OS_CONSOLE_TWL | OS_CONSOLE_TWLTYPE_RETAIL;
                break;
            case OS_CONSOLE_SIZE_32MB:
                OSi_Platform = 0;
                break;
            default:
                OSi_Platform = 0;
            }
        } break;
        case OS_CHIPTYPE_DEBUGGER_1:
            if (*(u8 *)(OS_CHIPTYPE_JTAG_ADDR) & OS_CHIPTYPE_JTAG_MASK) {
                OSi_Platform = OS_CONSOLE_TWLDEBUGGER | OS_CONSOLE_BOARD_A9_A7;
            } else {
                OSi_Platform = OS_CONSOLE_TWL | OS_CONSOLE_TWLTYPE_DEV;
            }
            break;
        case OS_CHIPTYPE_DEBUGGER_2:
            if (*(u8 *)(OS_CHIPTYPE_JTAG_ADDR) & OS_CHIPTYPE_JTAG_MASK) {
                OSi_Platform = OS_CONSOLE_TWLDEBUGGER | OS_CONSOLE_BOARD_A9;
            } else {
                OSi_Platform = OS_CONSOLE_TWL | OS_CONSOLE_TWLTYPE_DEV;
            }
            break;
        case OS_CHIPTYPE_EVALUATE:
            OSi_Platform = OS_CONSOLE_TWL | OS_CONSOLE_TWLTYPE_DEV | OS_CONSOLE_EVALUATE;
            break;
        }

        OSi_IsDetectedPlatform = TRUE;
    }
    return OSi_Platform;
}

BOOL OS_IsRunOnEmulator(void) {
    return (OS_GetConsoleType() & OS_CONSOLE_ENSATA) ? TRUE : FALSE;
}

// TwlSDK's OS_IsRunOnTwl
BOOL OS_IsRunOnTwl(void) {
    static BOOL OSi_IsDetectedTWL = FALSE;
    static BOOL OSi_IsRunOnTWL = FALSE;

    if (!OSi_IsDetectedTWL) {
        u8 rom9 = reg_SCFG_A9ROM;

        OSi_IsRunOnTWL = (rom9 & (REG_SCFG_A9ROM_SEC_MASK | REG_SCFG_A9ROM_RSEL_MASK)) == REG_SCFG_A9ROM_SEC_MASK;

        OSi_IsDetectedTWL = TRUE;
    }

    return OSi_IsRunOnTWL;
}

BOOL OSi_IsRunOnTwl(void) {
    return OS_IsRunOnTwl();
}

// Black's link drops it
BOOL OSi_IsNitroModeOnTwl(void) {
    static BOOL OSi_IsDetected = FALSE;
    static BOOL OSi_IsNitroModeOnTwl = FALSE;

    if (!OSi_IsDetected) {
        if (!OS_IsRunOnTwl() && soundMixFlag()) {
            OSi_IsNitroModeOnTwl = TRUE;
        }

        OSi_IsDetected = TRUE;
    }

    return OSi_IsNitroModeOnTwl;
}
