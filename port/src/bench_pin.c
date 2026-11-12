// A repeatable game for bench runs (the maintainers' notes, "The repeatable bench"): BWPORT_BENCH_SEED=FRAME reseeds the
// game's generators at V-blank FRAME as the lockstep anchor does (docs/lockstep.md: the LC's x = 0x0123456789ABCDEF, the
// Mersenne Twister GFL_RandomUpdateMT(0x01234567)), so two runs of the same save and keys walk the same NPCs;
// BWPORT_BENCH_CLOCK=YYYY-MM-DDTHH:MM:SS (libntr-ios-benchpin.patch) freezes the RTC and the boot entropy beside it.
// The lockstep tool does this on Linux with link-time wraps and the ELF symbol table; Apple's linker has no wraps, and
// the LC's holder (gfl_use.c's static data_020aa1a4) is found in the executable's own Mach-O symbol table (its local
// symbols stay in the app: the build does not strip them). Off by default: nothing runs without the variable.
#include <nitro.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"
#include "system/gfl_use.h"
#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#endif

#include "port_game.h"
#define GFL_RandomUpdateMT (PORT_Game->randomUpdateMT)   // the running game's

// The address of a symbol of the main executable, locals included; NULL if not found (or not on Apple)
static void *LocalSymbol(const char *name) {
#ifdef __APPLE__
    const struct mach_header_64 *mh = (const struct mach_header_64 *)_dyld_get_image_header(0);
    intptr_t slide = _dyld_get_image_vmaddr_slide(0);
    const struct load_command *lc = (const struct load_command *)(mh + 1);
    const struct symtab_command *st = NULL;
    const struct segment_command_64 *linkedit = NULL;
    const struct nlist_64 *syms;
    const char *strs;
    uintptr_t base;
    u32 i;

    for (i = 0; i < mh->ncmds; i++, lc = (const struct load_command *)((const char *)lc + lc->cmdsize)) {
        if (lc->cmd == LC_SYMTAB) {
            st = (const struct symtab_command *)lc;
        } else if (lc->cmd == LC_SEGMENT_64 && strcmp(((const struct segment_command_64 *)lc)->segname, "__LINKEDIT") == 0) {
            linkedit = (const struct segment_command_64 *)lc;
        }
    }
    if (st == NULL || linkedit == NULL) {
        return NULL;
    }
    base = (uintptr_t)slide + linkedit->vmaddr - linkedit->fileoff;   // the symbol and string tables are mapped there
    syms = (const struct nlist_64 *)(base + st->symoff);
    strs = (const char *)(base + st->stroff);
    for (i = 0; i < st->nsyms; i++) {
        // Mach-O names carry a leading underscore; a section symbol (N_SECT), not a debug stab
        if ((syms[i].n_type & N_STAB) == 0 && (syms[i].n_type & N_TYPE) == N_SECT &&
            strs[syms[i].n_un.n_strx] == '_' && strcmp(strs + syms[i].n_un.n_strx + 1, name) == 0) {
            return (void *)(syms[i].n_value + (uintptr_t)slide);
        }
    }
#else
    (void)name;
#endif
    return NULL;
}

static int sFrame = -2;   // -2: not read yet, -1: off or done, else the frame to reseed at

// An in-process restart (machine.c): the new boot is reseeded at the same frame
void PORT_BenchReset(void) {
    sFrame = -2;
}

// Called at each V-blank wait with the game lock held (main_threads.c's PORT_WaitIrq)
void PORT_BenchFrame(void) {

    if (sFrame == -2) {
        const char *e = getenv("BWPORT_BENCH_SEED");
        sFrame = e != NULL && *e ? atoi(e) : -1;
    }
    if (sFrame >= 0 && OS_GetVBlankCount() >= (u32)sFrame) {
        GFLUseWork **work = PORT_Game->pinLC == NULL ? LocalSymbol("data_020aa1a4") : NULL;

        if (PORT_Game->pinLC != NULL) {   // each game's own (port/extern*/src/system/gfl_use.c)
            PORT_Game->pinLC();
        } else if (work != NULL && *work != NULL) {
            (*work)->rand.x = 0x0123456789ABCDEFULL;
        }
        GFL_RandomUpdateMT(0x01234567);
        printf("benchpin: the generators reseeded at V-blank %u (the LC %s, the MT)\n", (unsigned)OS_GetVBlankCount(),
               PORT_Game->pinLC != NULL || (work != NULL && *work != NULL) ? "too" : "not found");
        sFrame = -1;
    }
}
