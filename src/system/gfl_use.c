// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/gfl_use.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/arc.h"
#include "gfl/backup_system.h"
#include "gfl/bg_sys.h"
#include "gfl/dma_vblank.h"
#include "gfl/fade.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/rtc_cache.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/textprint.h"
#include "gfl/ui.h"
#include "nitro/math.h"
#include "nitro/os.h"
#include "nnsys/snd.h"
#include "system/dsi.h"
#include "system/gfl_use.h"
#include "system/net_define.h"
#include "system/save_error.h"
#include "work_area.h"

// The game's setup of the GFL library, which the ROM names: the heaps, the archive paths, the tasks that run in the
// HBlank and after the VBlank, the VBlank callback and the linear congruential random numbers. Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except those of the types and the statics

#define ARC_PATH_COUNT 235
#define HBLANK_TCB_COUNT 4
#define VBLANK_TCB_COUNT 16
// The size of heap 9, which a DS takes from the main arena's OS heap and a DSi from heap 1
#define EXTRA_HEAP_SIZE 0x3f00

// The bits of the console type (func_0207ac24) that have a failed assert show its message
#define CONSOLE_TYPE_DEBUG_MASK 0x81000000

void finishFunc(void);

// The file system paths of the archives, by archive ID
const char *NARC_PATH_LIST[ARC_PATH_COUNT] = {
// clang-format off
    "a/0/0/0", "a/0/0/1", "a/0/0/2", "a/0/0/3", "a/0/0/4", "a/0/0/5", "a/0/0/6", "a/0/0/7", "a/0/0/8", "a/0/0/9",
    "a/0/1/0", "a/0/1/1", "a/0/1/2", "a/0/1/3", "a/0/1/4", "a/0/1/5", "a/0/1/6", "a/0/1/7", "a/0/1/8", "a/0/1/9",
    "a/0/2/0", "a/0/2/1", "a/0/2/2", "a/0/2/3", "a/0/2/4", "a/0/2/5", "a/0/2/6", "a/0/2/7", "a/0/2/8", "a/0/2/9",
    "a/0/3/0", "a/0/3/1", "a/0/3/2", "a/0/3/3", "a/0/3/4", "a/0/3/5", "a/0/3/6", "a/0/3/7", "a/0/3/8", "a/0/3/9",
    "a/0/4/0", "a/0/4/1", "a/0/4/2", "a/0/4/3", "a/0/4/4", "a/0/4/5", "a/0/4/6", "a/0/4/7", "a/0/4/8", "a/0/4/9",
    "a/0/5/0", "a/0/5/1", "a/0/5/2", "a/0/5/3", "a/0/5/4", "a/0/5/5", "a/0/5/6", "a/0/5/7", "a/0/5/8", "a/0/5/9",
    "a/0/6/0", "a/0/6/1", "a/0/6/2", "a/0/6/3", "a/0/6/4", "a/0/6/5", "a/0/6/6", "a/0/6/7", "a/0/6/8", "a/0/6/9",
    "a/0/7/0", "a/0/7/1", "a/0/7/2", "a/0/7/3", "a/0/7/4", "a/0/7/5", "a/0/7/6", "a/0/7/7", "a/0/7/8", "a/0/7/9",
    "a/0/8/0", "a/0/8/1", "a/0/8/2", "a/0/8/3", "a/0/8/4", "a/0/8/5", "a/0/8/6", "a/0/8/7", "a/0/8/8", "a/0/8/9",
    "a/0/9/0", "a/0/9/1", "a/0/9/2", "a/0/9/3", "a/0/9/4", "a/0/9/5", "a/0/9/6", "a/0/9/7", "a/0/9/8", "a/0/9/9",
    "a/1/0/0", "a/1/0/1", "a/1/0/2", "a/1/0/3", "a/1/0/4", "a/1/0/5", "a/1/0/6", "a/1/0/7", "a/1/0/8", "a/1/0/9",
    "a/1/1/0", "a/1/1/1", "a/1/1/2", "a/1/1/3", "a/1/1/4", "a/1/1/5", "a/1/1/6", "a/1/1/7", "a/1/1/8", "a/1/1/9",
    "a/1/2/0", "a/1/2/1", "a/1/2/2", "a/1/2/3", "a/1/2/4", "a/1/2/5", "a/1/2/6", "a/1/2/7", "a/1/2/8", "a/1/2/9",
    "a/1/3/0", "a/1/3/1", "a/1/3/2", "a/1/3/3", "a/1/3/4", "a/1/3/5", "a/1/3/6", "a/1/3/7", "a/1/3/8", "a/1/3/9",
    "a/1/4/0", "a/1/4/1", "a/1/4/2", "a/1/4/3", "a/1/4/4", "a/1/4/5", "a/1/4/6", "a/1/4/7", "a/1/4/8", "a/1/4/9",
    "a/1/5/0", "a/1/5/1", "a/1/5/2", "a/1/5/3", "a/1/5/4", "a/1/5/5", "a/1/5/6", "a/1/5/7", "a/1/5/8", "a/1/5/9",
    "a/1/6/0", "a/1/6/1", "a/1/6/2", "a/1/6/3", "a/1/6/4", "a/1/6/5", "a/1/6/6", "a/1/6/7", "a/1/6/8", "a/1/6/9",
    "a/1/7/0", "a/1/7/1", "a/1/7/2", "a/1/7/3", "a/1/7/4", "a/1/7/5", "a/1/7/6", "a/1/7/7", "a/1/7/8", "a/1/7/9",
    "a/1/8/0", "a/1/8/1", "a/1/8/2", "a/1/8/3", "a/1/8/4", "a/1/8/5", "a/1/8/6", "a/1/8/7", "a/1/8/8", "a/1/8/9",
    "a/1/9/0", "a/1/9/1", "a/1/9/2", "a/1/9/3", "a/1/9/4", "a/1/9/5", "a/1/9/6", "a/1/9/7", "a/1/9/8", "a/1/9/9",
    "a/2/0/0", "a/2/0/1", "a/2/0/2", "a/2/0/3", "a/2/0/4", "a/2/0/5", "a/2/0/6", "a/2/0/7", "a/2/0/8", "a/2/0/9",
    "a/2/1/0", "a/2/1/1", "a/2/1/2", "a/2/1/3", "a/2/1/4", "a/2/1/5", "a/2/1/6", "a/2/1/7", "a/2/1/8", "a/2/1/9",
    "a/2/2/0", "a/2/2/1", "a/2/2/2", "a/2/2/3", "a/2/2/4", "a/2/2/5", "a/2/2/6", "a/2/2/7", "a/2/2/8", "a/2/2/9",
    "a/2/3/0", "a/2/3/1", "a/2/3/2", "a/2/3/3", "a/2/3/4",
// clang-format on
};

// The root heaps 0 and 1, of a DS and of a DSi
// pokebw2's HEAP_SETUP_NTR
static const HeapDef data_0209d8b0[] = {
    { 0x6000, 0 },
    { 0x199500, 0 },
};

// pokebw2's HEAP_SETUP_TWL
static const HeapDef data_0209d8a0[] = {
    { 0x6000, 0 },
    { 0x486000, 0 },
};

// Six statics, defined without initializers, so MWCC lists them at the end of its sorted data, the last one first: that
// order gives the 235 path strings Black's order, and these their addresses (data_020aa194..data_020aa1a8)
// pokebw2's sVBlankCount
static u32 data_020aa1a0;
// pokebw2's sVBlankCallback
static VBlankCallback data_020aa19c;
// pokebw2's sVBlankCallbackData
static void *data_020aa194;
// pokebw2's sOSHeap: the OS heap of the main arena, made on a DS
static int data_020aa198;
// pokebw2's sExtraHeapMemory: the memory of heap 9, taken from the OS heap on a DS
static void *data_020aa1a8;
// pokebw2's sWork
static GFLUseWork *data_020aa1a4;

// pokebw2's GFLAppInit
void GFLAppInit(void) {
    void *start;
    void *end;
    int heap;

    if (!OS_IsRunOnTwl()) {
        end = OS_GetArenaHi(OS_ARENA_MAIN);
        start = OS_GetArenaLo(OS_ARENA_MAIN);
        start = OS_InitAlloc(OS_ARENA_MAIN, start, end, 1);
        OS_SetArenaLo(OS_ARENA_MAIN, start);
        heap = OS_CreateExtraHeap(OS_ARENA_MAIN);
        data_020aa198 = heap;
        data_020aa1a8 = OS_AllocFromHeap(OS_ARENA_MAIN, heap, WORK_AREA_SIZE(EXTRA_HEAP_SIZE));
    }

    if (OS_IsRunOnTwl()) {
        GFL_MemInit(data_0209d8a0, 2, 0xad, 0);
    } else {
        GFL_MemInit(data_0209d8b0, 2, 0xad, 0);
    }
    GFL_HeapDTCMInit(0x2000);
    GFL_HeapCreateChild(1, 7, 0x25000);
    if (!OS_IsRunOnTwl()) {
        GFL_HeapCreateRoot(data_020aa1a8, WORK_AREA_SIZE(EXTRA_HEAP_SIZE), 9);
    } else {
        GFL_HeapCreateChild(1, 9, EXTRA_HEAP_SIZE);
    }

    initTableArea(0);
    GFL_ArcSysInit(NARC_PATH_LIST, 235);
    GFL_HIDInit(0, 0);
    if (OS_GetConsoleType() & CONSOLE_TYPE_DEBUG_MASK) {
        GFL_DebugSetVerboseAssertHandlers();
    } else {
        GFL_DebugSetAssertHandlers(NULL, NULL, finishFunc);
    }
    GFL_OvlManagerInit(0, 0x10, 4, 4);
    GCTX_ProcMgrInit(0);

    data_020aa1a4 = GFL_HeapAllocate(0, sizeof(GFLUseWork), FALSE, "gfl_use.c", 0x117);
    data_020aa1a4->hblankTCBWork = GFL_HeapAllocate(0, GFL_TCBMgrCalcAllocSize(4), FALSE, "gfl_use.c", 0x119);
    data_020aa1a4->hblankTCBMgr = GFL_TCBMgrCreate(4, data_020aa1a4->hblankTCBWork);
    data_020aa1a4->vblankTCBWork = GFL_HeapAllocate(0, GFL_TCBMgrCalcAllocSize(0x10), FALSE, "gfl_use.c", 0x11c);
    data_020aa1a4->vblankTCBMgr = GFL_TCBMgrCreate(0x10, data_020aa1a4->vblankTCBWork);

    func_020056d0();
    GFL_FadeCreate(0);
    mainBackupSystemInit(7, showSavegameMainError, showSavegameAltError);
    GFL_TextPrintInit(NULL);
    func_0203f3b4();
    data_020aa1a0 = 0;
    GFL_VBlankResetCallback();
    GFL_SndSetVolumeControlCallbacks();
    GFL_FadeSetUpdateFreq(2);
}

void GFLAppUpdate(void) {
    GCTX_HIDUpdate();
    func_0200f2d4();
    GCTX_ProcMgrUpdate();
    func_0203f3f0();
}

void func_0200560c(void) {
}

void func_02005610(void) {
    GFL_TCBMgrUpdate(data_020aa1a4->hblankTCBMgr);
}


TCB *GFL_HBlankTCBAdd(TCBFunc func, void *data, u32 priority) {
    return GFL_TCBMgrAddTask(data_020aa1a4->hblankTCBMgr, func, data, priority);
}

void GFL_VBlankExecCallback(void) {
    if (data_020aa19c != NULL) {
        data_020aa19c(data_020aa194);
    }
}


BOOL GFL_VBlankSetCallback(VBlankCallback callback, void *data) {
    if (data_020aa19c != NULL) {
        return FALSE;
    }
    data_020aa19c = callback;
    data_020aa194 = data;
    return TRUE;
}

void GFL_VBlankResetCallback(void) {
    data_020aa19c = NULL;
    data_020aa194 = NULL;
}

void GFL_VBlankUpdate(void) {
    GFL_TCBMgrUpdate(data_020aa1a4->vblankTCBMgr);
    GFL_BGSysUpdate();
    func_02050a70();
    GFL_FadeUpdate();
    data_020aa1a0++;
}


TCB *GFL_VBlankTCBAdd(TCBFunc func, void *data, u32 priority) {
    return GFL_TCBMgrAddTask(data_020aa1a4->vblankTCBMgr, func, data, priority);
}

TCBManager *GFL_VBlankGetTCBMgr(void) {
    return data_020aa1a4->vblankTCBMgr;
}

void func_020056c8(MATHRandContext32 *context) {
    buildSeed(context);
}

void func_020056d0(void) {
    u32 seed;

    func_020056c8(&data_020aa1a4->rand);
    seed = GFL_RandomLC(0);
    GFL_RandomUpdateMT(seed);
}



u32 GFL_RandomLC(u32 max) {
    return MATH_Rand32(&data_020aa1a4->rand, max);
}

u32 GFL_RandomLCAlt(u32 max) {
    MATHRandContext32 *context = &data_020aa1a4->rand;

    context->x = context->mul * context->x + context->add;
    return (u32)(((context->x >> 32) * max) >> 32);
}

const char *GFL_ArcSysGetResourcePath(u32 arcId) {
    return NARC_PATH_LIST[arcId];
}

// Waits for L, R, X and Y to be held together, after a failed assert's message
void finishFunc(void) {
    while (TRUE) {
        u32 keys;

        GCTX_HIDUpdate();
        keys = GCTX_HIDGetHeldKeys();
        if ((keys & PAD_BUTTON_L) && (keys & PAD_BUTTON_R) && (keys & PAD_BUTTON_X) && (keys & PAD_BUTTON_Y)) {
            break;
        }
    }
}

void GFL_SndMute(void *work) {
    NNS_SndSetMasterVolume(0);
}

void GFL_SndSetMaxVol(void *work) {
    NNS_SndSetMasterVolume(127);
}

void GFL_SndSetVolumeControlCallbacks(void) {
    func_0203508c(GFL_SndMute, GFL_SndSetMaxVol, NULL);
}

BOOL GFL_SndIsVolumeControlCallbackSet(void) {
    if (func_02035108() == GFL_SndMute) {
        return TRUE;
    }
    return FALSE;
}
