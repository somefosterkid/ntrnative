// Unit tests of the game data fill (port/src/asset_load.c, PORT_AssetsLoad) on synthetic ROMs and a synthetic .bwfill table,
// no ROM and no game: port/tests/run_asset_load_test.sh builds it with asset_load.c included (its static parts reachable) and
// runs it. Each case runs in a child process (the fill's state is per process, and a refused ROM ends the process):
// - the assets.tsv rows of one blank both HeartGold and SoulSilver register (third_party/pokeheartgold/port/asmdata/
//   hg_rom_assets.c), filled for HeartGold, then SoulSilver, then HeartGold again: each time the running game's bytes
// - the image's copies (tools/port_romfill.py's table, encoded here as its encode() does): a game's own copies and the
//   shared ones (done for the first game only), a ROM of another revision (the hash) and one without the module's bytes
//   refused, and a game the table has no entry for while it has entries for others refused
// A check of an item of the rev1 review that is not fixed yet (sOpen) prints "xfail" and passes; once fixed it prints XPASS
// and fails: then remove the item from sOpen.
#include "../src/asset_load.c"

#include <stdarg.h>
#include <sys/wait.h>

typedef unsigned char u8;

static const int sOpen[] = {0};      // the rev1 review's items still open: none (items 2 and 3 fixed by lcfix1)
static int sFails;
static char sDir[256];
static unsigned char sDst[64] __attribute__((aligned(16)));   // the image's region the table's copies go to (writable data)
static unsigned char sBlank[16];                               // a table both games register (rows)

void PortOvl_Keep(const void *p, size_t n) {
    (void)p, (void)n;
}
void PORT_MachineKeep(const void *p, size_t n) {
    (void)p, (void)n;
}

static const BwAssetRow sRowsHg[] = {{"blank", -1, 0x02000010, 0x02000020, "u8"}};
static const BwAssetRow sRowsSs[] = {{"blank", -1, 0x02000030, 0x02000040, "u8"}};    // (another offset, as 71 real rows)
// fnv: the rows' FNV-1a 64 (asset_load.c's RowsHash) of Rom("IPKE", 3)'s and Rom("IPGE", 5)'s bytes, the ROMs the cases load
const BwAssetGame bwasset_games[] = {{"IPKE", sRowsHg, 1, 0x03a7cbe16264e845ULL}, {"IPGE", sRowsSs, 1, 0x185649b9ced735a5ULL},
                                     {NULL, NULL, 0, 0}};

static void Verdict(int ok, int item, const char *fmt, ...) {
    int open = 0;
    size_t i;
    va_list ap;

    for (i = 0; item != 0 && i < sizeof(sOpen) / sizeof(sOpen[0]); i++) {
        open |= sOpen[i] == item;
    }
    printf("%s ", open ? (ok ? "XPASS" : "xfail") : ok ? "ok  " : "FAIL");
    if (item != 0) {
        printf("(rev1 item %d%s) ", item, open && ok ? ": fixed, remove it from sOpen" : "");
    }
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    fflush(stdout);
    sFails += open ? ok : !ok;
}

// ---------------------------------------------------------------- synthetic ROMs: a header and an ARM9 module of 0x100 bytes
#define MOD_BASE 0x02000000u
#define MOD_LEN 0x100u

static u8 ModByte(int seed, unsigned i) {
    return (u8)(i * (unsigned)seed + 17u * (unsigned)seed + 1u);
}

static void Le32(u8 *p, unsigned v) {
    p[0] = v;
    p[1] = v >> 8;
    p[2] = v >> 16;
    p[3] = v >> 24;
}

static const char *Rom(const char *code, int seed) {
    static char paths[8][300];
    static int next;
    char *path = paths[next++ & 7];
    u8 img[0x200 + MOD_LEN];
    unsigned i;
    FILE *f;

    memset(img, 0, sizeof(img));
    memcpy(img + 0x0c, code, 4);
    Le32(img + 0x20, 0x200);
    Le32(img + 0x28, MOD_BASE);
    Le32(img + 0x2c, MOD_LEN);
    for (i = 0; i < MOD_LEN; i++) {
        img[0x200 + i] = ModByte(seed, i);
    }
    snprintf(path, 300, "%s/%s_%d.nds", sDir, code, seed);
    f = fopen(path, "wb");
    fwrite(img, 1, sizeof(img), f);
    fclose(f);
    return path;
}

static int Same(const u8 *p, int seed, unsigned off, unsigned n) {
    unsigned i;
    for (i = 0; i < n; i++) {
        if (p[i] != ModByte(seed, off + i)) {
            return 0;
        }
    }
    return 1;
}

// ---------------------------------------------------------------- the .bwfill table, as tools/port_romfill.py's encode()
typedef struct {
    unsigned dst, off, len, shared;    // dst: an offset in sDst; off: the ARM9 module's byte
} FillCopy;

typedef struct {
    const char *code;
    int seed;    // the ROM the hash is taken over
    const FillCopy *copies;
    int n;
} FillGameSpec;

static u8 *Var(u8 *p, unsigned long long v) {
    do {
        *p++ = (u8)((v & 0x7f) | (v > 0x7f ? 0x80 : 0));
        v >>= 7;
    } while (v != 0);
    return p;
}

static unsigned long long Zig(long long v) {
    return v < 0 ? ((unsigned long long)~v << 1) | 1 : (unsigned long long)v << 1;
}

static void Table(const FillGameSpec *games, int ngames) {
    u8 *tab = (u8 *)(uintptr_t)bwfill_table, *s;
    long page = sysconf(_SC_PAGESIZE);
    uintptr_t a = (uintptr_t)tab & ~(uintptr_t)(page - 1);
    long long rel = (long long)((uintptr_t)sDst - (uintptr_t)tab);
    unsigned streamAt;
    int i, k;

    if (mprotect((void *)a, (uintptr_t)tab + BWFILL_SIZE - a, PROT_READ | PROT_WRITE) != 0) {
        Verdict(0, 0, "the test can't write the .bwfill table");
        exit(sFails);
    }
    memset(tab, 0, BWFILL_SIZE);
    memcpy(tab, "BWFL", 4);
    Le32(tab + 4, 1);
    Le32(tab + 8, 1);    // one region: sDst, writable data (PF_R | PF_W)
    Le32(tab + 12, (unsigned)ngames);
    Le32(tab + 16, (unsigned)rel);
    Le32(tab + 20, (unsigned)((unsigned long long)rel >> 32));
    Le32(tab + 24, sizeof(sDst));
    Le32(tab + 28, 6);
    streamAt = 16 + 16 + 32 * (unsigned)ngames;
    for (i = 0; i < ngames; i++) {
        u8 *gh = tab + 32 + 32 * i;
        unsigned long long h = 0xCBF29CE484222325ULL;
        uintptr_t prevDst = (uintptr_t)tab;
        long long prevSrc = 0;
        unsigned bytes = 0, b;

        s = tab + streamAt;
        for (k = 0; k < games[i].n; k++) {
            const FillCopy *c = &games[i].copies[k];
            uintptr_t dst = (uintptr_t)sDst + c->dst;
            s = Var(s, Zig((long long)(dst - prevDst)));
            s = Var(s, c->len);
            s = Var(s, 0u << 1 | c->shared);    // module 0: the ARM9 static module
            s = Var(s, Zig((long long)c->off - prevSrc));
            prevDst = dst + c->len;
            prevSrc = (long long)(c->off + c->len);
            for (b = 0; b < c->len; b++) {
                h = (h ^ ModByte(games[i].seed, c->off + b)) * 0x100000001B3ULL;
            }
            bytes += c->len;
        }
        memcpy(gh, games[i].code, 4);
        Le32(gh + 4, streamAt);
        Le32(gh + 8, (unsigned)(s - (tab + streamAt)));
        Le32(gh + 12, (unsigned)games[i].n);
        Le32(gh + 16, bytes);
        Le32(gh + 20, (unsigned)h);
        Le32(gh + 24, (unsigned)(h >> 32));
        streamAt = (unsigned)(s - tab);
    }
}

// ---------------------------------------------------------------- the cases (each the body of a child process)
#define LOADED 42    // a child's exit: the load came back (a case that wants it refused)

static void RowsHgSsHg(void) {
    PORT_AssetAdd("IPKE", "blank", sBlank, sizeof(sBlank), NULL);
    PORT_AssetAdd("IPGE", "blank", sBlank, sizeof(sBlank), NULL);
    PORT_AssetSetRom("IPKE", Rom("IPKE", 3));
    PORT_AssetSetRom("IPGE", Rom("IPGE", 5));
    PORT_AssetsLoad("IPKE");
    Verdict(Same(sBlank, 3, 0x10, 16), 0, "rows: HeartGold first: HeartGold's bytes");
    PORT_AssetsLoad("IPGE");
    Verdict(Same(sBlank, 5, 0x30, 16), 0, "rows: then SoulSilver: SoulSilver's bytes");
    PORT_AssetsLoad("IPKE");
    Verdict(Same(sBlank, 3, 0x10, 16), 3, "rows: then HeartGold again: HeartGold's bytes, not SoulSilver's");
}

static const FillCopy sCopies[] = {{0, 0x20, 8, 0}, {16, 0x40, 8, 1}};

static void FillTwoGames(void) {
    const FillGameSpec games[] = {{"IPKE", 3, sCopies, 2}, {"IPGE", 5, sCopies, 2}};
    Table(games, 2);
    PORT_AssetSetRom("IPKE", Rom("IPKE", 3));
    PORT_AssetSetRom("IPGE", Rom("IPGE", 5));
    Verdict(PORT_AssetsLoad("IPKE") == 1 && Same(sDst, 3, 0x20, 8) && Same(sDst + 16, 3, 0x40, 8), 0,
            "fill: HeartGold's own and shared copies from its ROM");
    Verdict(PORT_AssetsLoad("IPGE") == 1 && Same(sDst, 5, 0x20, 8) && Same(sDst + 16, 3, 0x40, 8), 0,
            "fill: then SoulSilver's own copies, the shared ones left as the first game filled them");
}

static void FillOtherRevision(void) {
    const FillGameSpec games[] = {{"IPKE", 3, sCopies, 2}};
    Table(games, 1);
    PORT_AssetSetRom("IPKE", Rom("IPKE", 7));
    PORT_AssetsLoad("IPKE");
    exit(LOADED);
}

static void FillNoModuleBytes(void) {
    static const FillCopy past[] = {{0, MOD_LEN - 4, 8, 0}};
    const FillGameSpec games[] = {{"IPKE", 3, past, 1}};
    Table(games, 1);
    PORT_AssetSetRom("IPKE", Rom("IPKE", 3));
    PORT_AssetsLoad("IPKE");
    exit(LOADED);
}

static void FillGameMissing(void) {
    const FillGameSpec games[] = {{"IPKE", 3, sCopies, 2}};
    Table(games, 1);
    PORT_AssetAdd("IPGE", "blank", sBlank, sizeof(sBlank), NULL);
    PORT_AssetSetRom("IPGE", Rom("IPGE", 5));
    exit(PORT_AssetsLoad("IPGE") == 1 ? LOADED : 43);
}

// runs body in a child: its exit status (-1: no normal exit), its output in out
static int Child(void (*body)(void), char *out, size_t n) {
    int fd[2], status = 0;
    size_t k = 0;
    ssize_t r;
    pid_t pid;

    fflush(stdout);
    if (pipe(fd) != 0 || (pid = fork()) < 0) {
        return -1;
    }
    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], 1);
        sFails = 0;
        body();
        fflush(stdout);
        _exit(sFails);
    }
    close(fd[1]);
    while (k + 1 < n && (r = read(fd[0], out + k, n - 1 - k)) > 0) {
        k += (size_t)r;
    }
    out[k] = 0;
    close(fd[0]);
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

// a case whose child prints its own verdicts: its failures count
static void Verdicts(void (*body)(void)) {
    char out[8192];
    int st = Child(body, out, sizeof(out)), i;
    char *line;

    for (line = strtok(out, "\n"); line != NULL; line = strtok(NULL, "\n")) {
        if (strncmp(line, "assets:", 7) != 0) {
            printf("%s\n", line);
        }
    }
    i = st < 0 ? 1 : st;
    if (st < 0) {
        printf("FAIL a case's process ended abnormally\n");
    }
    sFails += i;
}

// a case that wants the load refused: the process ends with exit 1 and a message naming the cause
static void Refused(void (*body)(void), const char *what, const char *wantMsg, int item) {
    char out[8192];
    int st = Child(body, out, sizeof(out));
    char *msg = strstr(out, "bwport: ");

    if (msg != NULL) {
        msg[strcspn(msg, "\n")] = 0;
    }
    Verdict(st == 1 && (wantMsg == NULL || (msg != NULL && strstr(msg, wantMsg) != NULL)), item, "%s: refused (exit %d%s%s)",
            what, st, msg ? ", " : ", the load came back", msg ? msg : "");
}

int main(void) {
    char cmd[400];

    snprintf(sDir, sizeof(sDir), "%s/asset_test.XXXXXX", getenv("TMPDIR") ? getenv("TMPDIR") : "/tmp");
    if (mkdtemp(sDir) == NULL) {
        printf("FAIL no temporary folder\n");
        return 1;
    }
    Verdicts(RowsHgSsHg);
    Verdicts(FillTwoGames);
    Refused(FillOtherRevision, "fill: a ROM of another revision (other bytes)", "another revision", 0);
    Refused(FillNoModuleBytes, "fill: a copy past the ROM module's end", "has no module 0", 0);
    Refused(FillGameMissing, "fill: a table with entries for other games but none for SoulSilver", NULL, 2);
    snprintf(cmd, sizeof(cmd), "rm -rf '%s'", sDir);
    if (system(cmd) != 0) {
        printf("note: %s not removed\n", sDir);
    }
    printf(sFails == 0 ? "PASS\n" : "FAIL (%d)\n", sFails);
    return sFails != 0;
}
