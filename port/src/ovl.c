// The PC build's overlays. On the DS an overlay is loaded into memory it shares with other overlays, and each
// FS_LoadOverlay brings its .data back to the ROM's values and zeroes its .bss. The PC build links every overlay into
// the one exe, where their code and data sit side by side and stay; what a load still has to do is that reset.
// tools/port_overlays.py writes a linker script that gathers each overlay's .data and .bss (COMMON
// with them) into two output sections in overlay order, .ovldata and .ovlbss, and a table (.ovltab) of each overlay's
// offsets into them and its DS layout. Here: a copy of .ovldata taken before the game starts, and libntr's
// FS_LoadOverlayInfo, FS_LoadOverlay and FS_UnloadOverlay over it (libntr's assume a ROM image and 32-bit addresses
// on PC: its FS_LoadOverlay is empty with SDK 5). With all three here, libntr's fs_overlay.c object stays out of the
// link (its FSi_InitOverlay, which FS_Init calls, is port/subprojects/packagefiles/libntr-sdk-gaps.patch's).
//
// FS_LoadOverlayInfo gives the DS's numbers (address, image and bss size): lib/gfl/src/gf_overlay.c checks with them
// that no two loaded overlays share memory, as on the DS. The address is the DS's: nothing on PC may read through it.
//
// macOS has no linker script (ld64): with BWPORT_OVL_MACHO the overlay files are an archive of their own, in overlay
// order, with a marker object before each overlay (and one at the end) in the sections __DATA,__ovldata and __ovlbss
// that the overlay files' data is compiled into (port/machine/macos_ovl_section.h); the generated ovltab.c lists the
// markers' addresses and the DS layout, and PortOvl_Init makes the same table from them (port/meson.build).
//
// Without either (BWPORT_OVL_SCRIPT undefined) the loads reset nothing and each overlay gets a made-up DS range
// of its own, so the game still runs. Environment: BWPORT_OVL_CHECK=1 prints after each load whether .data is the
// snapshot and .bss zero (and how much of each was dirty before the reset); BWPORT_OVL_LOG=1 prints each load and unload to stderr;
// BWPORT_OVL_POISON=1 fills an overlay's .data and .bss with 0xa5 when it is unloaded, which finds reads of an
// overlay's data after its unload (the DS keeps the bytes until another overlay is loaded over them).
#include <nitro.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "port_game.h"
#include "port_ovl.h"

#define BWOVL_MAGIC 0x564F5742 // "BWOV", tools/port_overlays.py
#define BWOVL_VERSION 1
#define BWOVL_MAX 512   // Black's 237 overlays, Black 2's 344

// The battle's overlay (src/ov093/btl_main.c's GameProc, data_ov093_021efdac): in from the battle's start (after the field's
// encounter effect) to its end (before the field comes back). The encounter effect before it (src/ov021/enceff.c's
// overlays 76-79: encounter_effect_model, _grid, _ripple, _cells) is the battle intro. libntr's Battle speed
// (libntr-ios-battlespeed.patch) runs the game faster while either is in (the maintainers' notes, 6.), except while the
// battle's Bag (overlay 197, src/ov197/b_bag_main.c; src/ov093/btlv_core.c loads it for the Bag screen only) or its party
// screen (overlay 198, also where a Bag item's target is picked) is open: the maintainer wants items and Pokemon picked at the
// normal speed.
// The numbers are the running game's (port_game.h: Black's 93, 76-79, 197, 198; Black 2's 167, 148-154, 286, 287).
void SIM_SetBattle(int on);

// One overlay in .ovltab (tools/port_overlays.py writes them as LONGs)
typedef struct {
    u32 id;
    u32 dsRam;       // the DS's load address
    u32 dsImageSize; // text, rodata and data: FSOverlayInfoHeader.ram_size
    u32 dsBssSize;
    u32 dataOffset; // into .ovldata
    u32 dataSize;
    u32 bssOffset; // into .ovlbss
    u32 bssSize;
} PortOvlEntry;

typedef struct {
    u32 magic;
    u32 version;
    u32 count;
    u32 dataTotal;
    u32 bssTotal;
    u32 reserved[3];
    PortOvlEntry entries[];
} PortOvlTable;

// Black 2 and White 2 on the PC (GNU ld, lld, PE): their overlays' data is outside the script's table (port/machine/gnu_ovl_section.h,
// tools/port_overlays.py puts it at the end of .ovldata and .ovlbss), so they have a table of their own, made as the Mach-O build
// makes each game's from the markers in PortGame; Black and White (ovlCount NULL) keep the script's.
#if defined(BWPORT_OVL_SCRIPT) && !defined(BWPORT_OVL_MACHO) && (defined(BWPORT_B2) || defined(BWPORT_W2))
#define BWPORT_OVL_GAMES
#endif

#if defined(BWPORT_OVL_MACHO) || defined(BWPORT_OVL_GAMES)
// ld64: port/meson.build links the overlay files in overlay order with a marker object before each overlay's (and one
// at the end) in __DATA,__ovldata and __DATA,__ovlbss; the generated ovltab.c lists the markers and the DS layout.
// The table is made from them at startup.
// Each game has its own (port/include/port_game.h); PortOvl_SetGame switches to the one PORT_EnterGame chose.
static PortOvlTable *sMachoTab;
static u8 *sMachoData, *sMachoBss;
#define __bwovl_tab (*sMachoTab)
#define __bwovl_data sMachoData
#define __bwovl_bss sMachoBss
#ifdef BWPORT_OVL_GAMES
extern const PortOvlTable sScriptTab __asm("__bwovl_tab");
extern u8 sScriptData[] __asm("__bwovl_data");
extern u8 sScriptBss[] __asm("__bwovl_bss");
static const PortOvlTable *sScriptTable;
static u8 *sScriptSnapshot;
#endif
#elif defined(BWPORT_OVL_SCRIPT)
extern const PortOvlTable __bwovl_tab;
extern u8 __bwovl_data[];
extern u8 __bwovl_bss[];
#endif

#if defined(BWPORT_OVL_SCRIPT) && !defined(BWPORT_OVL_MACHO)
// The script's table section takes it after the table: read-only data's flags for the section (ELF and PE)
__attribute__((section(".ovltab"), used)) static const u32 sPortOvlTabAnchor = BWOVL_MAGIC;
#endif

static const PortOvlTable *sTable;
static u8 *sSnapshot;
static u8 sLoaded[BWOVL_MAX];
static BOOL sInit;
static BOOL sLog;
static BOOL sPoison;
// wall milliseconds, for the log lines that carry a frame count (the speed options' timings need both)
static unsigned PortOvl_Ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
static BOOL sCheck;
static void PortOvl_Load(void);

static BOOL PortOvl_EnvOn(const char *name) {
    const char *v = getenv(name);
    return v != NULL && v[0] != '\0' && strcmp(v, "0") != 0;
}

// To stderr, flushed: on Windows a redirected stderr is buffered, and a killed or crashed game loses what it held
static void PortOvl_Print(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fflush(stderr);
}

void PortOvl_Init(void) {
    if (sInit) {
        return;
    }
    sInit = TRUE;
    sLog = PortOvl_EnvOn("BWPORT_OVL_LOG");
    sPoison = PortOvl_EnvOn("BWPORT_OVL_POISON");
    sCheck = PortOvl_EnvOn("BWPORT_OVL_CHECK");
#ifdef BWPORT_OVL_GAMES
    sMachoTab = (PortOvlTable *)&sScriptTab;
    sMachoData = sScriptData;
    sMachoBss = sScriptBss;
    PortOvl_Load();
    sScriptTable = sTable;
    sScriptSnapshot = sSnapshot;
#endif
#if defined(BWPORT_OVL_MACHO) || defined(BWPORT_OVL_GAMES)
    PortOvl_SetGame(PORT_Game);
#else
    PortOvl_Load();
#endif
}

#if defined(BWPORT_OVL_MACHO) || defined(BWPORT_OVL_GAMES)
// A game's table and snapshot, made the first time it is chosen (its overlays' data is still as linked then: a game
// runs only once chosen, and machine.c puts every game's back at a reset)
static struct {
    const PortGame *game;
    PortOvlTable *tab;
    u8 *snapshot;
} sGames[4];

void PortOvl_SetGame(const PortGame *g) {
    u32 i, k, n;

#ifdef BWPORT_OVL_GAMES
    if (g->ovlCount == NULL) {   // Black, White: the script's table
        sMachoTab = (PortOvlTable *)&sScriptTab;
        sMachoData = sScriptData;
        sMachoBss = sScriptBss;
        sTable = sScriptTable;
        sSnapshot = sScriptSnapshot;
        memset(sLoaded, 0, sizeof(sLoaded));
        return;
    }
#endif
    n = *g->ovlCount;
    for (k = 0; k < 4 && sGames[k].game != NULL && sGames[k].game != g; k++) {
    }
    if (k == 4) {
        return;
    }
    if (sGames[k].game == NULL) {
        PortOvlTable *t = calloc(1, sizeof(PortOvlTable) + n * sizeof(PortOvlEntry));

        if (t == NULL) {
            return;
        }
        *t = (PortOvlTable){BWOVL_MAGIC, BWOVL_VERSION, n, (u32)(g->ovlData[n] - g->ovlData[0]),
                (u32)(g->ovlBss[n] - g->ovlBss[0]), {0, 0, 0}};
        for (i = 0; i < n; i++) {
            PortOvlEntry *e = &t->entries[i];

            e->id = g->ovlDs[i][0];
            e->dsRam = g->ovlDs[i][1];
            e->dsImageSize = g->ovlDs[i][2];
            e->dsBssSize = g->ovlDs[i][3];
            e->dataOffset = (u32)(g->ovlData[i] - g->ovlData[0]);
            e->dataSize = (u32)(g->ovlData[i + 1] - g->ovlData[i]);
            e->bssOffset = (u32)(g->ovlBss[i] - g->ovlBss[0]);
            e->bssSize = (u32)(g->ovlBss[i + 1] - g->ovlBss[i]);
        }
        sMachoTab = t;
        sMachoData = g->ovlData[0];
        sMachoBss = g->ovlBss[0];
        sTable = NULL;
        sSnapshot = NULL;
        PortOvl_Load();
        sGames[k].game = g;
        sGames[k].tab = t;
        sGames[k].snapshot = sSnapshot;
    }
    sMachoTab = sGames[k].tab;
    sMachoData = g->ovlData[0];
    sMachoBss = g->ovlBss[0];
    sTable = sMachoTab;
    sSnapshot = sGames[k].snapshot;
    memset(sLoaded, 0, sizeof(sLoaded));
}
#else
void PortOvl_SetGame(const PortGame *g) {   // one game: the linker script's table
    (void)g;
}
#endif

// The table checked and its data's snapshot taken
static void PortOvl_Load(void) {
#ifdef BWPORT_OVL_SCRIPT
    if (__bwovl_tab.magic != BWOVL_MAGIC || __bwovl_tab.version != BWOVL_VERSION || __bwovl_tab.count > BWOVL_MAX) {
        PortOvl_Print("ovl: .ovltab is not tools/port_overlays.py's (magic %08x version %u count %u): no resets\n",
                (unsigned)__bwovl_tab.magic, (unsigned)__bwovl_tab.version, (unsigned)__bwovl_tab.count);
        return;
    }
    sTable = &__bwovl_tab;
    if (sTable->dataTotal != 0) {
        sSnapshot = malloc(sTable->dataTotal);
        if (sSnapshot == NULL) {
            PortOvl_Print("ovl: no memory for the %u-byte .data snapshot: no resets\n", (unsigned)sTable->dataTotal);
            sTable = NULL;
            return;
        }
        memcpy(sSnapshot, __bwovl_data, sTable->dataTotal);
    }
    if (sLog) {
        PortOvl_Print("ovl: %u overlays, %u bytes of .data (snapshot), %u of .bss\n", (unsigned)sTable->count,
                (unsigned)sTable->dataTotal, (unsigned)sTable->bssTotal);
    }
#else
    if (sLog) {
        PortOvl_Print("ovl: built without tools/port_overlays.py's script: loads reset nothing\n");
    }
#endif
}

// asset_load.c: a table filled from the ROM after the snapshot was taken; its bytes go into the snapshot too, so each load of
// its overlay puts the ROM's values back (not the placeholders the exe was linked with)
void PortOvl_Keep(const void *p, size_t n) {
#if defined(BWPORT_OVL_SCRIPT) && !defined(BWPORT_OVL_MACHO)
    const u8 *b = p;

    if (sSnapshot != NULL && sTable != NULL && b >= __bwovl_data && b + n <= __bwovl_data + sTable->dataTotal) {
        memcpy(sSnapshot + (b - __bwovl_data), b, n);
    }
#else
    (void)p;
    (void)n;
#endif
}

// Before NitroMain and anything that writes an overlay's data: the snapshot is the values the exe was linked with
__attribute__((constructor)) static void PortOvl_Constructor(void) {
    PortOvl_Init();
}

static const PortOvlEntry *PortOvl_Entry(MIProcessor target, FSOverlayID id) {
    if (target != MI_PROCESSOR_ARM9 || sTable == NULL || id >= sTable->count) {
        return NULL;
    }
    return &sTable->entries[id];
}

static BOOL PortOvl_Valid(MIProcessor target, FSOverlayID id) {
    PortOvl_Init();
#ifdef BWPORT_OVL_SCRIPT
    return PortOvl_Entry(target, id) != NULL;
#else
    return target == MI_PROCESSOR_ARM9 && id < BWOVL_MAX;
#endif
}

BOOL FS_LoadOverlayInfo(FSOverlayInfo *p_ovi, MIProcessor target, FSOverlayID id) {
    const PortOvlEntry *e;

    if (!PortOvl_Valid(target, id)) {
        return FALSE;
    }
    e = PortOvl_Entry(target, id);
    memset(p_ovi, 0, sizeof(*p_ovi));
    p_ovi->header.id = id;
    if (e != NULL) {
        p_ovi->header.ram_address = (u8 *)(uintptr_t)e->dsRam;
        p_ovi->header.ram_size = e->dsImageSize;
        p_ovi->header.bss_size = e->dsBssSize;
    } else {
        // No table: a range of its own for each, in main memory's numbers but past it (no two collide)
        p_ovi->header.ram_address = (u8 *)(uintptr_t)(0x10000000u + id * 0x100000u);
        p_ovi->header.ram_size = 0x1000;
    }
    p_ovi->header.file_id = id;
    p_ovi->target = target;
    return TRUE;
}

// The battle waits at its command menu with the idle camera on (port/extern/src/ov094/btlv_effect.c)
static BOOL sBattleIdle;

// The battle or its intro is in, and neither the battle's Bag nor its party screen is, nor its command menu
static BOOL PortOvl_BattleIn(void) {
    u32 id;

    if (sLoaded[PORT_Game->ovlBag] || sLoaded[PORT_Game->ovlParty] || sBattleIdle) {
        return FALSE;
    }
    for (id = PORT_Game->ovlEncFirst; id <= PORT_Game->ovlEncLast; id++) {
        if (sLoaded[id]) {
            return TRUE;
        }
    }
    return sLoaded[PORT_Game->ovlBattle];
}

// An in-process restart (machine.c put the overlays' data back as linked): nothing is loaded, no battle
void PortOvl_Reset(void) {
    memset(sLoaded, 0, sizeof(sLoaded));
    sBattleIdle = FALSE;
    SIM_SetBattle(0);
}

// Each battle tick (BtlvEffect_Main): the battle's idle camera is on, i.e. the game waits for the player's command
void PortOvl_SetBattleIdle(BOOL on) {
    if (on != sBattleIdle) {
        sBattleIdle = on;
        if (sLog) {
            PortOvl_Print("ovl: [f%u t%u] battle command menu %s\n", (unsigned)OS_GetVBlankCount(), PortOvl_Ms(), on ? "up" : "down");
        }
        SIM_SetBattle(PortOvl_BattleIn());
    }
}

BOOL FS_LoadOverlay(MIProcessor target, FSOverlayID id) {
    const PortOvlEntry *e;

    if (!PortOvl_Valid(target, id)) {
        PortOvl_Print("ovl: FS_LoadOverlay(%d, %u): no such overlay\n", (int)target, (unsigned)id);
        return FALSE;
    }
    e = PortOvl_Entry(target, id);
#ifdef BWPORT_OVL_SCRIPT
    u32 dirtyData = 0, dirtyBss = 0, i;

    if (sCheck && e != NULL) {
        // what the overlay's statics held before this load (nonzero: the reset below is doing something)
        for (i = 0; i < e->dataSize; i++) {
            dirtyData += __bwovl_data[e->dataOffset + i] != sSnapshot[e->dataOffset + i];
        }
        for (i = 0; i < e->bssSize; i++) {
            dirtyBss += __bwovl_bss[e->bssOffset + i] != 0;
        }
    }
    memcpy(__bwovl_data + e->dataOffset, sSnapshot + e->dataOffset, e->dataSize);
    memset(__bwovl_bss + e->bssOffset, 0, e->bssSize);
    if (sCheck) {
        // right after the load: .data is the start-of-game snapshot and .bss is zero
        u32 badData = memcmp(__bwovl_data + e->dataOffset, sSnapshot + e->dataOffset, e->dataSize) != 0;
        u32 badBss = 0;

        for (i = 0; i < e->bssSize; i++) {
            badBss += __bwovl_bss[e->bssOffset + i] != 0;
        }
        PortOvl_Print("ovl-check: ov%03u%s data %u bytes (%u were dirty) bss %u bytes (%u were dirty): %s\n",
                (unsigned)id, sLoaded[id] ? " (again)" : "", (unsigned)e->dataSize, (unsigned)dirtyData,
                (unsigned)e->bssSize, (unsigned)dirtyBss, badData || badBss ? "FAIL" : "ok");
    }
#endif
    if (sLog) {
        PortOvl_Print("ovl: [f%u t%u] load ov%03u%s (data %u, bss %u bytes)\n", (unsigned)OS_GetVBlankCount(), PortOvl_Ms(), (unsigned)id, sLoaded[id] ? " (again)" : "",
                e ? (unsigned)e->dataSize : 0u, e ? (unsigned)e->bssSize : 0u);
    }
    sLoaded[id] = TRUE;
    SIM_SetBattle(PortOvl_BattleIn());
    return TRUE;
}

BOOL FS_UnloadOverlay(MIProcessor target, FSOverlayID id) {
    const PortOvlEntry *e;

    if (!PortOvl_Valid(target, id)) {
        return FALSE;
    }
    e = PortOvl_Entry(target, id);
#ifdef BWPORT_OVL_SCRIPT
    if (sPoison) {
        memset(__bwovl_data + e->dataOffset, 0xa5, e->dataSize);
        memset(__bwovl_bss + e->bssOffset, 0xa5, e->bssSize);
    }
#endif
    if (sLog) {
        PortOvl_Print("ovl: [f%u t%u] unload ov%03u%s\n", (unsigned)OS_GetVBlankCount(), PortOvl_Ms(), (unsigned)id, sLoaded[id] ? "" : " (wasn't loaded)");
    }
    sLoaded[id] = FALSE;
    if (id == PORT_Game->ovlBattle) {
        sBattleIdle = FALSE;
    }
    SIM_SetBattle(PortOvl_BattleIn());
    return TRUE;
}

BOOL PortOvl_IsLoaded(u32 id) {
    return id < BWOVL_MAX && sLoaded[id];
}

int PortOvl_Owner(const void *p) {
#ifdef BWPORT_OVL_SCRIPT
    const u8 *a = p;
    u32 i;

    PortOvl_Init();
    for (i = 0; sTable != NULL && i < sTable->count; i++) {
        const PortOvlEntry *e = &sTable->entries[i];

        if ((a >= __bwovl_data + e->dataOffset && a < __bwovl_data + e->dataOffset + e->dataSize)
            || (a >= __bwovl_bss + e->bssOffset && a < __bwovl_bss + e->bssOffset + e->bssSize)) {
            return (int)e->id;
        }
    }
#else
    (void)p;
#endif
    return -1;
}

u32 PortOvl_Count(void) {
    PortOvl_Init();
    return sTable != NULL ? sTable->count : 0;
}
