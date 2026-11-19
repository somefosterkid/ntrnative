// HG/SS function coverage for the crash sweep (meson -Dhgss_cov=true; tools/hg_sweep.py, docs/hgss.md "Crash sweep"): the module's
// units are compiled with -fsanitize-coverage=func,inline-bool-flag,pc-table (a flag per function, set inline: no call, so no
// argument register is touched; trace-pc-guard's call at each entry clobbered the registers a missing argument still held on the
// plain build, PlaySE's GF_GetPlayerNoBySeq()), this file is not. HgCov_Dump (the sweep's field hook calls it every 256 V-blanks
// and at its exit) writes BWPORT_HG_COV=FILE: this file's own function address (8 bytes, so a reader maps the rest to the exe's
// symbols whatever the load address), then the address of each function whose flag is set (8 bytes each). Linked into every HG
// build: without the option the init calls never come and HgCov_Dump does nothing.
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

void __sanitizer_cov_bool_flag_init(char *start, char *stop);
void __sanitizer_cov_pcs_init(const uintptr_t *start, const uintptr_t *stop);
void HgCov_Dump(void);

static char *sFlags, *sFlagsEnd;
static const uintptr_t *sPcs, *sPcsEnd;

void __sanitizer_cov_bool_flag_init(char *start, char *stop) {
    if (sFlags == NULL && start != stop) {   // (every unit's constructor passes the whole section)
        sFlags = start;
        sFlagsEnd = stop;
    }
}

void __sanitizer_cov_pcs_init(const uintptr_t *start, const uintptr_t *stop) {
    if (sPcs == NULL && start != stop) {
        sPcs = start;
        sPcsEnd = stop;
    }
}

void HgCov_Dump(void) {
    static uint64_t buf[1 << 16];
    const char *f = getenv("BWPORT_HG_COV");
    size_t n = 1, i, flags = (size_t)(sFlagsEnd - sFlags), pcs = (size_t)(sPcsEnd - sPcs) / 2;
    int fd;

    if (f == NULL || sFlags == NULL || sPcs == NULL) {
        return;
    }
    buf[0] = (uint64_t)(uintptr_t)&HgCov_Dump;
    for (i = 0; i < flags && i < pcs && n < sizeof buf / sizeof buf[0]; i++) {
        if (sFlags[i]) {
            buf[n++] = (uint64_t)sPcs[2 * i];   // the table's pairs: the PC, the flags
        }
    }
    fd = open(f, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (write(fd, buf, n * sizeof buf[0]) < 0) {
            n = 0;
        }
        close(fd);
    }
}
