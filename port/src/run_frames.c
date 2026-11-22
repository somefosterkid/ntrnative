// tools/port_run.py's frame counter: the address of the V-blank count (OS_GetVBlankCount's word, which libntr's
// frame adds 1 to after the V-blank handler). The harness reads the count through /proc/<pid>/mem while the game runs,
// with no breakpoint and no code in the frame. Nothing in the game uses it.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const volatile u32 *const bwport_vblank_count = (const volatile u32 *)HW_VBLANK_COUNT_BUF;

// BWPORT_EXIT_FRAME=N (tools/port_run.py --frames N): the run ends, exit status 0, at the first V-blank wait where the
// game's own V-blank count is N: a run is cut at a game frame, never at a wall time (a loaded host reached fewer frames
// before the wall cap and read as worse). Called first in each V-blank wait (main_threads.c: PORT_WaitIrq,
// PortPlat_WaitIrq: every game), so the frames before N have all run and the game's own per-frame logs (BWPORT_PLATTRACE,
// BWPORT_HGTRACE: flushed per line) end the same on every run. _exit, as a route leg's (route_test.c) and the lockstep
// dump's (lstep_dump.c Finish): the sound captures are closed by hand, stdio flushed.
extern void acapture_close(void);   // acapture.c
extern void PORT_HeapCheckFrame(void);   // nnsys_heapcheck.c (BWPORT_HEAPCHECK): every heap checked once per V-blank wait
extern int FS_PortOpenFiles(int *peak);  // libntr fs_file.c (libntr-openfiles.patch): host files open now, the most at once
extern void ab_wav_close(void);     // port/bridge/acore_bridge.c

extern void PortOvl_LoadedList(char *buf, size_t n);                           // ovl.c
// the game's part of the line: port/game/route_gen4.c (gen 4) sets it at its first frame (a function in the game module is not
// visible to this file's link, so it registers itself)
void (*PORT_StateHook)(char *buf, size_t n);

// One `bwport: state: frame=N ovl=1,5,12 field=up task=+0xOFF script=ID ...` line when the exe ends itself at the frame cap:
// where a run that is stuck (frames advance, nothing happens) was; tools/sweep_wall.py stuck_top groups the sweeps' rows by it.
// An address is an offset in the image: `addr2line -f -e build/port/linux-hgss/bwport 0xOFF`.
static void PORT_StateLine(void) {
    char ovl[160], game[160];

    game[0] = 0;
    PortOvl_LoadedList(ovl, sizeof ovl);
    if (PORT_StateHook != NULL) {
        PORT_StateHook(game, sizeof game);
    }
    fprintf(stderr, "bwport: state: frame=%u ovl=%s%s\n", (unsigned)OS_GetVBlankCount(), ovl, game);
}

// The open-file guard (fl1, 2026-11-17: HG's GetNarcMemberSizeByIdPair and Platinum's NARC_GetMemberSizeByIndexPair never closed
// their file: ~33 host files per 1000 frames, the save failed at EMFILE after ~8 minutes of play; win64's CRT stops at 512). The
// games keep ~120 NARC files open at once (docs/port-runtime.md "Open files"); once more than PORT_OPEN_FILES_BOUND were open at
// once, `openfiles: leak: ...` is said (once), and with BWPORT_HEAPCHECK on the exe stops there (abort, as the heap checker's).
// PORT_OpenFilesLine is the run's end: `openfiles: now N peak P` (tools/route.py's chain report prints the peak).
#define PORT_OPEN_FILES_BOUND 256

void PORT_OpenFilesLine(void) {
    int peak, now = FS_PortOpenFiles(&peak);

    fprintf(stderr, "openfiles: now %d peak %d\n", now, peak);
}

static void PORT_OpenFilesFrame(void) {
    static int sSaid;
    int peak, now;

    if (sSaid) {
        return;
    }
    now = FS_PortOpenFiles(&peak);
    if (peak > PORT_OPEN_FILES_BOUND) {
        const char *e = getenv("BWPORT_HEAPCHECK");

        sSaid = 1;
        fprintf(stderr, "openfiles: leak: %d host files open at frame %u (peak %d, bound %d): a game opens files and never closes them\n",
                now, (unsigned)OS_GetVBlankCount(), peak, PORT_OPEN_FILES_BOUND);
        if (e != NULL && e[0] != '\0' && e[0] != '0') {
            fflush(NULL);
            abort();
        }
    }
}

void PORT_ForkFrame(void);   // title_fork.c (BWPORT_FORK_AT)

void PORT_ExitFrame(void) {
    static int sState;   // 0: not read yet, 1: on, -1: off
    static u32 sFrame;

    PORT_ForkFrame();
    PORT_HeapCheckFrame();   // (a heap hit since the last wait is said at this V-blank, the exit frame's too)
    PORT_OpenFilesFrame();
    if (sState == 0) {
        const char *e = getenv("BWPORT_EXIT_FRAME");

        sFrame = e != NULL ? (u32)strtoul(e, NULL, 10) : 0;
        sState = sFrame > 0 ? 1 : -1;
    }
    if (sState > 0 && OS_GetVBlankCount() >= sFrame) {
        PORT_StateLine();
        PORT_OpenFilesLine();
        fprintf(stderr, "bwport: exit-frame %u\n", (unsigned)OS_GetVBlankCount());
        acapture_close();
        ab_wav_close();
        fflush(NULL);
        _exit(0);
    }
}
