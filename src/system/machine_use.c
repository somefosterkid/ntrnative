// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/machine_use.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/arc.h"
#include "gfl/backup_system.h"
#include "gfl/fade.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/rtc_cache.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/textprint.h"
#include "gfl/ui.h"
#include "nitro/fs.h"
#include "nitro/fx/fx.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "nitro/mi.h"
#include "nitro/spi/ARM9/mic.h"
#include "nitro/os.h"
#include "nitro/rtc.h"
#include "nnsys/gfd.h"
#include "system/dsi.h"
#include "system/gfl_use.h"
#include "system/machine_use.h"
#include "system/main.h"
#include "system/save_error.h"
#include "nitro/spi/ARM9/pm.h"
#include "twl/scfg.h"

// The DSi's power management functions in the LTD autoload, which the SDK headers don't declare
void func_027314f0(void);                     // PM_ReadyToExit
void func_0273154c(PMExitCallbackInfo *info); // PM_AppendPreExitCallback
void func_027315c0(BOOL enabled);             // PM_SetAutoExit
BOOL func_027315d0(void);                     // PM_GetAutoExit

// The game's setup of the console and the SDK, with what the main loop does for them each frame. The file name is a
// guess after gfl_use.c, which follows it, as the ROM does not embed it; its statics have a base of their own, so it
// is not main's file. SystemInit and SystemUpdateVBlank are names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0).
//
// The calls to the DSi's power management go to the LTD autoload, out of reach of a branch, so the linker adds a
// veneer after each calling function: the `bx pc` stubs and ARM jumps after SystemInit, func_020053ec and
// func_02005440 (swan's dsi276952D, dsi2769595, dsi27694DD and GFLAppInit) are those, not functions of this file

// The DSi's system configuration: the extensions it enables for the game, and the copy that the boot code keeps
#define reg_SCFG_EXT (*(vu32 *)0x04004008)
#define SCFG_EXT_COPY (*(vu32 *)0x02fffdf0)
#define SCFG_EXT_VRAM 0x2000

// The bits of the console type (OS_GetConsoleType) that SystemInit checks
#define CONSOLE_TYPE_DEV_MASK 0x3000000
// The bits of the console type that have a failed assert show its message
#define CONSOLE_TYPE_DEBUG_MASK 0x81000000

// The size of heap 9, which a DS takes from the main arena's OS heap and a DSi from heap 1
#define EXTRA_HEAP_SIZE 0x3f00

#define VRAM_TRANSFER_TASK_COUNT 48

static void func_02005440(void *arg);

// pokebw2's sExitRequested
BOOL data_020a9e80;
// pokebw2's sExitCallbackInfo
static PMExitCallbackInfo data_020a9e84;
// pokebw2's sVramTransferTasks
static NNSGfdVramTransferTask data_020a9e94[VRAM_TRANSFER_TASK_COUNT];

void SystemInit(void) {
    u32 tableSize;
    void *table;
    int amp;

    OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    RTC_Init();
    GX_Init();
    FX_Init();

    if (OS_GetConsoleType() & CONSOLE_TYPE_DEV_MASK) {
        func_0208c988(1);
        reg_SCFG_EXT &= ~SCFG_EXT_VRAM;
        SCFG_EXT_COPY &= ~SCFG_EXT_VRAM;
    }

    (void)OS_EnableIrq();
    FS_Init(1);
    tableSize = FS_TryLoadTable(NULL, 0);
    table = OS_AllocFromArenaLo(OS_ARENA_MAIN, tableSize, 4);
    FS_TryLoadTable(table, tableSize);

    NNS_GfdInitVramTransferManager(data_020a9e94, VRAM_TRANSFER_TASK_COUNT);

    if (func_0202eb0c()) {
        data_020a9e80 = FALSE;
        PM_SetExitCallbackInfo(&data_020a9e84, func_02005440, NULL);
        func_0273154c(&data_020a9e84);
    }

    PM_GetAmp(&amp);
    if (amp == PM_AMP_ON) {
        PM_SetAmp(PM_AMP_OFF);
    }
}

void func_020053ec(void) {
    if (data_020a9e80 && !func_020351d0(2) && !func_027315d0()) {
        MI_SetMainMemoryPriority(MI_PROCESSOR_ARM7);
        func_027314f0();
    }
}

void SystemUpdateVBlank(void) {
    NNS_GfdDoVramTransfer();
}

void func_02005438(void) {
    func_020911a0();
}


// Called when the DSi is about to exit the game: keeps it from exiting until func_020053ec lets it
// pokebw2's OnExitRequest
static void func_02005440(void *arg) {
    func_027315c0(FALSE);
    data_020a9e80 = TRUE;
}


