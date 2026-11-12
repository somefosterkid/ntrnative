// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_arena.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py). TwlSDK 5 builds it as one object whose
// twl/ltdmain_begin.h block (OS_InitArenaEx's DSi part) goes to the LTD autoload's .ltdmain 0x027312b4-0x027312d0
// (ltd_autoload_0's delinks.txt names it os_arena@ltd.c: tools/two_modules.py).
#include <nitro.h>
#include "constants/version.h"

#define OSi_TRUNC(n, a)              (((u32) (n)) & ~((a) - 1))
#define OSi_ROUND(n, a)              (((u32) (n) + (a) - 1) & ~((a) - 1))

#define OS_ERR_GETARENAHI_INIT       "OS_GetArenaHi: init in advance"
#define OS_ERR_GETARENAHI_INVALID    "OS_GetArenaHi: invalid arena (hi<lo)"
#define OS_ERR_GETARENALO_INIT       "OS_GetArenaLo: init in advance"
#define OS_ERR_GETARENALO_INVALID    "OS_GetArenaLo: invalid arena (hi<lo)"

#define SDK_ARENAID_ASSERT(id)        SDK_ASSERT((u32)id < OS_ARENA_MAX)

#define OSi_GetArenaInfo()           (*(OSArenaInfo *)HW_ARENA_INFO_BUF)

extern void SDK_MAIN_ARENA_LO(void);

#define OSi_MAIN_ARENA_LO_DEFAULT               ((u32)SDK_MAIN_ARENA_LO)
#define OSi_MAIN_ARENA_HI_DEFAULT               HW_MAIN_MEM_MAIN_END

    extern void SDK_SECTION_ARENA_EX_START(void);

    #define OSi_MAINEX_ARENA_LO_DEFAULT             ((u32)SDK_SECTION_ARENA_EX_START)
    #define OSi_MAINEX_ARENA_HI_DEFAULT             HW_MAIN_MEM_DEBUGGER

    extern void SDK_SECTION_ARENA_DTCM_START(void);
    #define OSi_DTCM_ARENA_LO_DEFAULT               ((u32)SDK_SECTION_ARENA_DTCM_START)

    extern void SDK_SECTION_ARENA_ITCM_START(void);
    #define OSi_ITCM_ARENA_LO_DEFAULT               ((u32)SDK_SECTION_ARENA_ITCM_START)

#define OSi_WRAM_MAIN_ARENA_LO_DEFAULT          HW_WRAM
#define OSi_WRAM_MAIN_ARENA_HI_DEFAULT          HW_WRAM

extern void SDK_SYS_STACKSIZE(void);
#define OSi_SYS_STACKSIZE               ((int)SDK_SYS_STACKSIZE)

extern void SDK_IRQ_STACKSIZE(void);
#define OSi_IRQ_STACKSIZE               ((int)SDK_IRQ_STACKSIZE)

static BOOL OSi_Initialized = FALSE;

        BOOL OSi_MainExArenaEnabled = FALSE;

    void OS_InitArena (void)
    {
        if (OSi_Initialized) {
            return;
        }

        OSi_Initialized = TRUE;

        OS_InitArenaHiAndLo(OS_ARENA_MAIN);

        OS_SetArenaLo(OS_ARENA_MAINEX, (void *)0);
        OS_SetArenaHi(OS_ARENA_MAINEX, (void *)0);

        OS_InitArenaHiAndLo(OS_ARENA_ITCM);
        OS_InitArenaHiAndLo(OS_ARENA_DTCM);
        OS_InitArenaHiAndLo(OS_ARENA_SHARED);
        OS_InitArenaHiAndLo(OS_ARENA_WRAM_MAIN);
    }

#include <twl/ltdmain_begin.h>
// OS_InitArenaEx's part on a DSi, in the LTD autoload's .ltdmain: the cartridge's region (TwlSDK reads the console
// type here and drops it, as OS_InitArenaEx does)
void OSi_InitArenaExTwl(void)
{
    OS_GetConsoleType();
    OS_SetProtectionRegion2(3, HW_CTRDG_ROM, HW_C6_PR_64MB);
}
#include <twl/ltdmain_end.h>

void OS_InitArenaEx (void)
{
    OS_GetConsoleType();

    OS_SetArenaHi(OS_ARENA_MAINEX, OS_GetInitArenaHi(OS_ARENA_MAINEX));
    OS_SetArenaLo(OS_ARENA_MAINEX, OS_GetInitArenaLo(OS_ARENA_MAINEX));

    if (!OS_IsRunOnTwl()) {
        OS_SetProtectionRegion2(1, HW_MAIN_MEM_MAIN, HW_C6_PR_4MB);
        OS_SetProtectionRegion2(2, HW_MAIN_MEM_SHARED, HW_C6_PR_4KB);
    } else {
        OSi_InitArenaExTwl();
    }
}

void * OS_GetArenaHi (OSArenaId id)
{
    SDK_ASSERT(OSi_Initialized);
    SDK_ARENAID_ASSERT(id);
    SDK_ASSERTMSG((u32)OSi_GetArenaInfo().lo[id] != 0xffffffff, OS_ERR_GETARENALO_INIT);
    SDK_ASSERTMSG((u32)OSi_GetArenaInfo().lo[id] <= (u32)OSi_GetArenaInfo().hi[id], OS_ERR_GETARENALO_INVALID);

    return OSi_GetArenaInfo().hi[id];
}

void * OS_GetArenaLo (OSArenaId id)
{
    SDK_ASSERT(OSi_Initialized);
    SDK_ARENAID_ASSERT(id);
    SDK_ASSERTMSG((u32)OSi_GetArenaInfo().lo[id] != 0xffffffff, OS_ERR_GETARENALO_INIT);
    SDK_ASSERTMSG((u32)OSi_GetArenaInfo().lo[id] <= (u32)OSi_GetArenaInfo().hi[id], OS_ERR_GETARENALO_INVALID);

    return OSi_GetArenaInfo().lo[id];
}

void * OS_GetInitArenaHi (OSArenaId id)
{
    void * hi;

    switch (id) {
    case OS_ARENA_MAIN:
        if (OS_IsRunOnTwl()) {
            hi = (void *)0x2f80000;
        } else {
            hi = (void *)0x23e0000;
        }
        break;
    case OS_ARENA_MAINEX:
        if (OSi_MainExArenaEnabled == 0) {
            hi = NULL;
            break;
        }
        {
            u32 n = OS_GetConsoleType() & 0xf;

            if (OS_IsRunOnTwl()) {
                hi = (n == 4) ? NULL : (void *)0xdf00000;
            } else {
                hi = (n == 1) ? NULL : (void *)0x2700000;
            }
        }
        break;
    case OS_ARENA_ITCM:
        hi = (void *)0x2000000;
        break;
    case OS_ARENA_DTCM:
        {
            u32 irqStackLo = (u32)HW_DTCM_IRQ_STACK_END - OSi_IRQ_STACKSIZE;
            u32 sysStackLo;

            if (!OSi_SYS_STACKSIZE) {
                sysStackLo = HW_DTCM;
                if (sysStackLo < OSi_DTCM_ARENA_LO_DEFAULT) {
                    sysStackLo = OSi_DTCM_ARENA_LO_DEFAULT;
                }
            } else if (OSi_SYS_STACKSIZE < 0) {
                sysStackLo = OSi_DTCM_ARENA_LO_DEFAULT - OSi_SYS_STACKSIZE;
            } else {
                sysStackLo = irqStackLo - OSi_SYS_STACKSIZE;
            }
            hi = (void *)sysStackLo;
        }
        break;
    case OS_ARENA_SHARED:
        hi = (void *)0x2fff680;
        break;
    case OS_ARENA_WRAM_MAIN:
        hi = (void *)0x3040000;
        break;
    default:
        hi = NULL;
        break;
    }

    return hi;
}


void * OS_GetInitArenaLo (OSArenaId id)
{
    void * lo;

    switch (id) {
    case OS_ARENA_MAIN:
        if (OS_IsRunOnTwl()) {
            lo = (void *)0x2735b40;
        } else {
#if GAME_VERSION == VERSION_WHITE
            lo = (void *)0x2213d40;
#else
            lo = (void *)0x2213d20;
#endif
        }
        break;
    case OS_ARENA_MAINEX:
        if (OSi_MainExArenaEnabled == 0) {
            lo = NULL;
            break;
        }
        {
            u32 n = OS_GetConsoleType() & 0xf;

            if (OS_IsRunOnTwl()) {
                lo = NULL;
                if (n != 4) {
                    lo = (void *)OSi_MAINEX_ARENA_LO_DEFAULT;
                }
            } else {
                lo = NULL;
                if (n != 1) {
                    lo = (void *)0x2400060;
                }
            }
        }
        break;
    case OS_ARENA_ITCM:
        lo = (void *)0x1ff8820;
        break;
    case OS_ARENA_DTCM:
        lo = (void *)0x2fe00c0;
        break;
    case OS_ARENA_SHARED:
        lo = (void *)0x2fff000;
        break;
    case OS_ARENA_WRAM_MAIN:
        lo = (void *)0x3040000;
        break;
    default:
        lo = NULL;
        break;
    }

    return lo;
}



void OS_SetArenaHi (OSArenaId id, void * newHi)
{
    SDK_ASSERT(OSi_Initialized);
    SDK_ARENAID_ASSERT(id);

    OSi_GetArenaInfo().hi[id] = newHi;
}

void OS_SetArenaLo (OSArenaId id, void * newLo)
{
    SDK_ASSERT(OSi_Initialized);
    SDK_ARENAID_ASSERT(id);

    OSi_GetArenaInfo().lo[id] = newLo;
}

void * OS_AllocFromArenaLo (OSArenaId id, u32 size, u32 align)
{
    void * ptr;
    u8 * arenaLo;

    ptr = OS_GetArenaLo(id);
    if (!ptr) {
        return NULL;
    }

    arenaLo = ptr = (void *)OSi_ROUND(ptr, align);
    arenaLo += size;
    arenaLo = (u8 *)OSi_ROUND(arenaLo, align);

    if (arenaLo > OS_GetArenaHi(id)) {
        return NULL;
    }

    OS_SetArenaLo(id, arenaLo);

    return ptr;
}
