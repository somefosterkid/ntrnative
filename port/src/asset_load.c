// The game's data tables from the player's ROM (docs/port-build.md, "Game data from the ROM"). The PC build ships, so the
// rows of config/<version>/assets.tsv are not compiled in: tools/port_assets.py gives the C placeholders of the same shape
// and registers each table here (PORT_AssetAdd, from a constructor of its file), and before the game boots
// (machine.c's SIM_RunGame, after the launcher chose it) PORT_AssetsLoad reads them from the running game's ROM: the
// ARM9 static module and the overlays from the ROM's header, FAT and overlay table, BLZ-decompressed as the DS's crt0
// and FS_LoadOverlay do, each row's DS range at the address the ROM build reads it from (tools/asset.py). Black's
// rows come from Black's ROM, White's from White's (their own rows, config/w_us/assets.tsv).
//
// The tables are writable data now, in the overlay and machine ranges whose snapshots ovl.c and machine.c take (ovl.c's at
// start, before any ROM is known): each filled range is copied into them too (PortOvl_Keep, PORT_MachineKeep), so an
// overlay load or an in-process restart puts the ROM's values back, not the placeholders.
//
// The ROM: the one PORT_AssetSetRom named for the game (main_start.c's PORT_EnterGame: the launcher's copy), else
// BWPORT_ROM, else a .nds in the working directory, each only if its header has the game's code. Without it the game
// cannot run (its tables would be placeholders): a message and exit. BWPORT_ASSET_LOG=1 prints each table.
#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "asset_rows.h"

#if defined(__has_include)
#if __has_include(<SDL.h>) && !defined(BWASSET_TEST)
#include <SDL.h>
#define BWASSET_HAVE_SDL 1
#endif
#endif

void PortOvl_Keep(const void *p, size_t n);        // ovl.c
void PORT_MachineKeep(const void *p, size_t n);    // machine.c

#define REG_MAX 2048
#define GAMES_MAX 4

typedef struct {
    const char *game;
    const char *name;
    void *p;
    unsigned long size;
    const char *layout;   // the C type's fields when its host layout is not the DS's (tools/port_assets.py TYPE_LAYOUT)
} BwAssetReg;

static BwAssetReg sReg[REG_MAX];
static int sNReg;
static char sRomFor[GAMES_MAX][5];
static char sRomPath[GAMES_MAX][1024];
static char sLoaded[GAMES_MAX][5];

// From each rewritten file's constructor (tools/port_assets.py): nothing else runs yet, so only the table is written
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout) {
    if (sNReg < REG_MAX) {
        sReg[sNReg++] = (BwAssetReg){game, name, p, size, layout};
    } else {
        sNReg = REG_MAX + 1;   // reported by PORT_AssetsLoad
    }
}

// The launcher's ROM copy of a game (main_start.c's PORT_EnterGame)
void PORT_AssetSetRom(const char *code, const char *path) {
    int i;

    for (i = 0; i < GAMES_MAX; i++) {
        if (sRomFor[i][0] == 0 || memcmp(sRomFor[i], code, 4) == 0) {
            memcpy(sRomFor[i], code, 4);
            snprintf(sRomPath[i], sizeof(sRomPath[i]), "%s", path);
            return;
        }
    }
}

static unsigned Rd32(const unsigned char *p) {
    return (unsigned)p[0] | (unsigned)p[1] << 8 | (unsigned)p[2] << 16 | (unsigned)p[3] << 24;
}

static int RomHasCode(const char *path, const char *code) {
    unsigned char h[16];
    FILE *f = fopen(path, "rb");
    int ok = 0;

    if (f != NULL) {
        ok = fread(h, 1, sizeof(h), f) == sizeof(h) && memcmp(h + 12, code, 4) == 0;
        fclose(f);
    }
    return ok;
}

static int FindRom(const char *code, char *out, size_t n) {
    const char *env = getenv("BWPORT_ROM");
    DIR *dir;
    struct dirent *e;
    int i;

    for (i = 0; i < GAMES_MAX; i++) {
        if (memcmp(sRomFor[i], code, 4) == 0 && RomHasCode(sRomPath[i], code)) {
            snprintf(out, n, "%s", sRomPath[i]);
            return 1;
        }
    }
    if (env != NULL && *env && RomHasCode(env, code)) {
        snprintf(out, n, "%s", env);
        return 1;
    }
    if ((dir = opendir(".")) != NULL) {
        while ((e = readdir(dir)) != NULL) {
            size_t len = strlen(e->d_name);

            if (len > 4 && (strcmp(e->d_name + len - 4, ".nds") == 0 || strcmp(e->d_name + len - 4, ".NDS") == 0) &&
                    RomHasCode(e->d_name, code)) {
                snprintf(out, n, "%s", e->d_name);
                closedir(dir);
                return 1;
            }
        }
        closedir(dir);
    }
    return 0;
}

// The DS's backward LZ (crt0's MIi_UncompressBackward, the ARM9 static module and the overlays): in is the module as
// stored, its last 8 bytes the footer; *outLen the decompressed size. NULL if malformed.
unsigned char *BwAsset_Unblz(const unsigned char *in, unsigned len, unsigned *outLen) {
    unsigned inc, hdr, enc, dec, pak, raw, i;
    unsigned char *out, *o, *oend;
    const unsigned char *p, *pend;
    unsigned char flags = 0, mask = 0;
    unsigned char *tmp;

    if (len < 8) {
        return NULL;
    }
    inc = Rd32(in + len - 4);
    if (inc == 0) {
        out = malloc(len ? len : 1);
        if (out != NULL) {
            memcpy(out, in, len);
            *outLen = len;
        }
        return out;
    }
    hdr = in[len - 5];
    enc = Rd32(in + len - 8) & 0xFFFFFF;
    if (hdr < 8 || hdr > 11 || enc > len || enc < hdr) {
        return NULL;
    }
    dec = len - enc;       // the uncompressed head
    pak = enc - hdr;       // the compressed bytes
    raw = dec + enc + inc; // the module's size
    out = malloc(raw);
    tmp = malloc(pak ? pak : 1);
    if (out == NULL || tmp == NULL) {
        free(out);
        free(tmp);
        return NULL;
    }
    memcpy(out, in, dec);
    for (i = 0; i < pak; i++) {   // the compressed bytes read backwards
        tmp[i] = in[dec + pak - 1 - i];
    }
    p = tmp;
    pend = tmp + pak;
    o = out + dec;
    oend = out + raw;
    while (o < oend) {
        if ((mask >>= 1) == 0) {
            if (p == pend) {
                break;
            }
            flags = *p++;
            mask = 0x80;
        }
        if (!(flags & mask)) {
            if (p == pend) {
                break;
            }
            *o++ = *p++;
        } else {
            unsigned pos, n;

            if (p + 1 >= pend) {
                break;
            }
            pos = (unsigned)p[0] << 8 | p[1];
            p += 2;
            n = (pos >> 12) + 3;
            pos = (pos & 0xFFF) + 3;
            if (n > (unsigned)(oend - o)) {
                n = (unsigned)(oend - o);
            }
            if (pos > (unsigned)(o - out)) {
                free(out);
                free(tmp);
                return NULL;
            }
            while (n--) {
                *o = *(o - pos);
                o++;
            }
        }
    }
    free(tmp);
    // the decompressed tail was written forwards: turn it round
    for (i = 0; i < (raw - dec) / 2; i++) {
        unsigned char t = out[dec + i];
        out[dec + i] = out[raw - 1 - i];
        out[raw - 1 - i] = t;
    }
    *outLen = raw;
    return out;
}

typedef struct {
    FILE *f;
    unsigned char head[0x200];
    unsigned char *mod[2 + 512];   // [0] the ARM9 static module, [1] the ARM7's, [2 + N] overlay N
    unsigned modLen[2 + 512];
    unsigned modBase[2 + 512];
    char tried[2 + 512];
} BwAssetRom;

static unsigned char *ReadAt(FILE *f, unsigned off, unsigned len) {
    unsigned char *b = malloc(len ? len : 1);

    if (b != NULL && (fseek(f, (long)off, SEEK_SET) != 0 || fread(b, 1, len, f) != len)) {
        free(b);
        b = NULL;
    }
    return b;
}

// A module's image at its DS address (NULL: not in this ROM): -1 the ARM9 static module, -2 the ARM7's, N overlay N
static const unsigned char *Module(BwAssetRom *r, int module, unsigned *base, unsigned *len) {
    int k = module == -1 ? 0 : module == -2 ? 1 : 2 + module;

    if (module < -2 || module >= 512) {
        return NULL;
    }
    if (!r->tried[k]) {
        unsigned char *raw;

        r->tried[k] = 1;
        if (module == -2) {   // the ARM7 (header 0x30/0x38/0x3C), stored as it runs
            unsigned off = Rd32(r->head + 0x30), ram = Rd32(r->head + 0x38), size = Rd32(r->head + 0x3C);

            if ((raw = ReadAt(r->f, off, size)) == NULL) {
                return NULL;
            }
            r->mod[k] = raw;
            r->modLen[k] = size;
            r->modBase[k] = ram;
        } else if (module < 0) {
            unsigned off = Rd32(r->head + 0x20), ram = Rd32(r->head + 0x28), size = Rd32(r->head + 0x2C), i, cend = 0;

            raw = ReadAt(r->f, off, size);
            if (raw == NULL) {
                return NULL;
            }
            // the module params (crt0's _start_ModuleParams): the nitro code pair at +0x1C, the compressed end at +0x14
            for (i = 0; i + 8 <= size && i < 0x8000; i += 4) {
                if (Rd32(raw + i) == 0xDEC00621 && Rd32(raw + i + 4) == 0x2106C0DE && i >= 0x1C) {
                    cend = Rd32(raw + i - 0x1C + 0x14);
                    break;
                }
            }
            if (cend != 0 && cend > ram && cend - ram <= size) {
                unsigned n, tail = size - (cend - ram);
                unsigned char *u = BwAsset_Unblz(raw, cend - ram, &n), *all;

                // the part after the compressed end is stored as it is (tools/rom_modules.py's arm9)
                all = u != NULL ? realloc(u, n + tail + 1) : NULL;
                if (all == NULL) {
                    free(u);
                    free(raw);
                    return NULL;
                }
                memcpy(all + n, raw + (cend - ram), tail);
                free(raw);
                raw = all;
                size = n + tail;
            }
            r->mod[k] = raw;
            r->modLen[k] = size;
            r->modBase[k] = ram;
        } else {
            unsigned ovt = Rd32(r->head + 0x50), ovtSize = Rd32(r->head + 0x54), fat = Rd32(r->head + 0x48);
            unsigned char e[32], fe[8];
            unsigned start, end, flags;

            if ((unsigned)module * 32 + 32 > ovtSize || fseek(r->f, (long)(ovt + module * 32), SEEK_SET) != 0 ||
                    fread(e, 1, 32, r->f) != 32 || Rd32(e) != (unsigned)module ||
                    fseek(r->f, (long)(fat + Rd32(e + 0x18) * 8), SEEK_SET) != 0 || fread(fe, 1, 8, r->f) != 8) {
                return NULL;
            }
            start = Rd32(fe);
            end = Rd32(fe + 4);
            flags = Rd32(e + 0x1C);
            if (end < start || (raw = ReadAt(r->f, start, end - start)) == NULL) {
                return NULL;
            }
            r->modLen[k] = end - start;
            if (flags >> 24 & 1) {
                unsigned n, clen = flags & 0xFFFFFF;
                unsigned char *u = BwAsset_Unblz(raw, clen != 0 && clen <= end - start ? clen : end - start, &n);

                free(raw);
                if (u == NULL) {
                    return NULL;
                }
                raw = u;
                r->modLen[k] = n;
            }
            r->mod[k] = raw;
            r->modBase[k] = Rd32(e + 4);
        }
    }
    *base = r->modBase[k];
    *len = r->modLen[k];
    return r->mod[k];
}

// One record layout's fields: (DS size, host size) each, natural alignment; ptr/ref fields are not the ROM's (host pointers,
// left as compiled), w32 a DS word in a host pointer's slot (its low half: both little-endian)
typedef struct {
    unsigned char ds, host, ptr;
} BwAssetField;

static int Layout(const char *s, BwAssetField *f, int max) {
    int n = 0;

    while (*s) {
        unsigned sz, count = 1, isPtr = 0;

        if (strncmp(s, "u8", 2) == 0 || strncmp(s, "s8", 2) == 0) {
            sz = 1;
            s += 2;
        } else if (strncmp(s, "u16", 3) == 0 || strncmp(s, "s16", 3) == 0) {
            sz = 2;
            s += 3;
        } else if (strncmp(s, "u32", 3) == 0 || strncmp(s, "s32", 3) == 0) {
            sz = 4;
            s += 3;
        } else if (strncmp(s, "fx16", 4) == 0) {
            sz = 2;
            s += 4;
        } else if (strncmp(s, "fx32", 4) == 0) {
            sz = 4;
            s += 4;
        } else if (strncmp(s, "ptr", 3) == 0 || strncmp(s, "ref", 3) == 0) {
            sz = 4;
            isPtr = 1;
            s += 3;
        } else if (strncmp(s, "w32", 3) == 0) {   // a 4-byte value in a pointer-sized slot (a union with a pointer)
            sz = 4;
            isPtr = 2;
            s += 3;
        } else {
            return -1;
        }
        if (*s == '[') {   // value names: the .inc's text only
            while (*s && *s != ']') {
                s++;
            }
            if (*s == ']') {
                s++;
            }
        }
        if (*s == '*') {
            count = (unsigned)strtoul(s + 1, (char **)&s, 10);
        }
        while (count--) {
            if (n == max) {
                return -1;
            }
            f[n++] = (BwAssetField){(unsigned char)sz, (unsigned char)(isPtr ? sizeof(void *) : sz), (unsigned char)(isPtr == 1)};
        }
        if (*s == ',') {
            s++;
        } else if (*s) {
            return -1;
        }
    }
    return n;
}

// The row's bytes into the table: one copy when it has no pointer fields (then the host's layout is the DS's); else
// field by field, the pointers left as compiled. FALSE if the sizes disagree.
static int Fill(const BwAssetRow *row, const char *layout, const unsigned char *src, void *dst, unsigned long size) {
    BwAssetField f[256];
    int n = Layout(layout, f, 256), i, wide = 0;
    unsigned dsOff[256], hostOff[256], dsAlign = 1, hostAlign = 1, ds = 0, host = 0, dsStride, hostStride, rec, nrec;

    if (n <= 0) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        ds = (ds + f[i].ds - 1) / f[i].ds * f[i].ds;
        host = (host + f[i].host - 1) / f[i].host * f[i].host;
        dsOff[i] = ds;
        hostOff[i] = host;
        ds += f[i].ds;
        host += f[i].host;
        dsAlign = f[i].ds > dsAlign ? f[i].ds : dsAlign;
        hostAlign = f[i].host > hostAlign ? f[i].host : hostAlign;
        wide |= f[i].ptr || f[i].host != f[i].ds;
    }
    dsStride = (ds + dsAlign - 1) / dsAlign * dsAlign;
    hostStride = (host + hostAlign - 1) / hostAlign * hostAlign;
    if ((row->end - row->start) % dsStride != 0) {
        return 0;
    }
    nrec = (row->end - row->start) / dsStride;
    if (!wide) {   // the host's layout is the DS's: one copy (into the start of a bigger table: the rest is zeros)
        if (size < (unsigned long)(row->end - row->start)) {
            return 0;
        }
        memcpy(dst, src, row->end - row->start);
        return 1;
    }
    if (size != (unsigned long)nrec * hostStride) {
        return 0;
    }
    for (rec = 0; rec < nrec; rec++) {
        for (i = 0; i < n; i++) {
            if (!f[i].ptr) {
                memcpy((unsigned char *)dst + rec * hostStride + hostOff[i], src + rec * dsStride + dsOff[i], f[i].ds);
            }
        }
    }
    return 1;
}

static void Fail(const char *msg) {
    fprintf(stderr, "bwport: %s\n", msg);
    printf("bwport: %s\n", msg);
    fflush(stdout);
#ifdef BWASSET_HAVE_SDL
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "bwport", msg, NULL);
#endif
    exit(1);
}

// ---- The whole image's game data (tools/port_romfill.py): every table, string and constant of the game's C that the
// DS build puts into the ROM is zero in the linked image; the step after the link wrote here, per game, where each run
// of those bytes is in the ROM's modules (as the DS loads them). The pointer slots between the runs are the image's own
// relocations and are never touched. Layout (little endian): "BWFL", version, region count, game count; per region
// (an output section with copies: offset from this table, size, the page protection it runs with, PF_ bits R=4 W=2
// X=1); per game its code, the offset and size of its copy stream, copies, bytes, the FNV-1a hash of the bytes; the
// streams: per copy zigzag(destination - previous copy's end), length, module << 1 | shared (module 0 = ARM9, 1 =
// ARM7, 2 + N = overlay N), zigzag(source offset - previous copy's source end). A shared copy (C both games use) is
// done for the first game that starts. Unfilled (a link without the step): no regions, no games.
#if !defined(__APPLE__) && !defined(BWASSET_TEST)
#define BWFILL_SIZE (512 * 1024)
__attribute__((section(".bwfill"), used, aligned(16))) const unsigned char bwfill_table[BWFILL_SIZE] = {'B', 'W', 'F', 'L', 1};
#define BWFILL_HAVE 1
#endif

#ifdef BWFILL_HAVE
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

static int sFillShared;

static unsigned long long FillVar(const unsigned char **p, const unsigned char *end, int *bad) {
    unsigned long long v = 0;
    int shift = 0;

    while (*p < end && shift < 64) {
        unsigned char b = *(*p)++;

        v |= (unsigned long long)(b & 0x7F) << shift;
        if (!(b & 0x80)) {
            return v;
        }
        shift += 7;
    }
    *bad = 1;
    return 0;
}

static long long FillZig(unsigned long long u) {
    return (long long)(u >> 1) ^ -(long long)(u & 1);
}

// pages of [p, p + n) writable (on) or back as they run (off: prot, PF_ bits); *old holds Windows' own value between
typedef struct {
    unsigned char *p;
    size_t n;
    unsigned prot;
    unsigned long old;
} FillRegion;

static int FillProtect(FillRegion *g, int writable) {
    size_t page;
    uintptr_t a, b;

#ifdef _WIN32
    SYSTEM_INFO si;
    DWORD o;

    GetSystemInfo(&si);
    page = si.dwPageSize;
#else
    page = (size_t)sysconf(_SC_PAGESIZE);
#endif
    a = (uintptr_t)g->p & ~(uintptr_t)(page - 1);
    b = ((uintptr_t)g->p + g->n + page - 1) & ~(uintptr_t)(page - 1);
#ifdef _WIN32
    if (!VirtualProtect((void *)a, b - a, writable ? PAGE_READWRITE : (DWORD)g->old, &o)) {
        return 0;
    }
    if (writable) {
        g->old = o;
    }
    return 1;
#else
    {
        int prot = writable ? PROT_READ | PROT_WRITE
                            : (g->prot & 4 ? PROT_READ : 0) | (g->prot & 2 ? PROT_WRITE : 0) | (g->prot & 1 ? PROT_EXEC : 0);

        return mprotect((void *)a, b - a, prot) == 0;
    }
#endif
}

static unsigned FillRd32(const unsigned char *p) {
    return (unsigned)p[0] | (unsigned)p[1] << 8 | (unsigned)p[2] << 16 | (unsigned)p[3] << 24;
}

// Has the image copies for this game? (0: a link without tools/port_romfill.py, or not this game)
static const unsigned char *FillGame(const char *code) {
    const unsigned char *volatile vtab = bwfill_table;   // (volatile: the initialiser is not what the image holds)
    const unsigned char *tab = vtab;
    unsigned nreg, ngames, i;

    if (memcmp((const void *)tab, "BWFL", 4) != 0) {
        return NULL;
    }
    nreg = FillRd32(tab + 8);
    ngames = FillRd32(tab + 12);
    for (i = 0; i < ngames; i++) {
        const unsigned char *g = tab + 16 + nreg * 16 + i * 32;

        if (memcmp(g, code, 4) == 0) {
            return g;
        }
    }
    return NULL;
}

// Copies the game's runs from its ROM into the image; FALSE (with msg) if the ROM lacks a module or its bytes differ
static int FillImage(BwAssetRom *r, const char *code, unsigned long *bytes, unsigned *copies, char *msg, size_t msgLen) {
    const unsigned char *volatile vtab = bwfill_table;   // (volatile: the initialiser is not what the image holds)
    const unsigned char *tab = vtab;
    const unsigned char *g = FillGame(code), *p, *end;
    unsigned nreg, i, n, k;
    unsigned long long h = 0xCBF29CE484222325ULL, want;
    unsigned long total = 0;
    FillRegion regs[16];
    unsigned char *prevDst;
    long long prevSrc = 0;
    int bad = 0, ok = 1, doShared = !sFillShared;

    if (g == NULL) {
        return 1;
    }
    nreg = FillRd32(tab + 8);
    if (nreg > 16) {
        snprintf(msg, msgLen, "rom fill: %u regions", nreg);
        return 0;
    }
    for (i = 0; i < nreg; i++) {
        const unsigned char *e = tab + 16 + i * 16;
        long long rel = (long long)((unsigned long long)FillRd32(e) | (unsigned long long)FillRd32(e + 4) << 32);

        regs[i] = (FillRegion){(unsigned char *)(uintptr_t)tab + rel, FillRd32(e + 8), FillRd32(e + 12), 0};
        if (!(regs[i].prot & 2) && !FillProtect(&regs[i], 1)) {
            snprintf(msg, msgLen, "rom fill: can't make %zu bytes at %p writable", regs[i].n, (void *)regs[i].p);
            return 0;
        }
    }
    p = tab + FillRd32(g + 4);
    end = p + FillRd32(g + 8);
    n = FillRd32(g + 12);
    want = (unsigned long long)FillRd32(g + 20) | (unsigned long long)FillRd32(g + 24) << 32;
    prevDst = (unsigned char *)(uintptr_t)tab;
    for (k = 0; k < n && ok; k++) {
        unsigned char *dst = prevDst + FillZig(FillVar(&p, end, &bad));
        unsigned long long len = FillVar(&p, end, &bad), ms = FillVar(&p, end, &bad);
        long long src = prevSrc + FillZig(FillVar(&p, end, &bad));
        int mid = (int)(ms >> 1), shared = (int)(ms & 1);
        unsigned base, mlen, b;
        const unsigned char *img;
        int inReg = -1;

        if (bad) {
            snprintf(msg, msgLen, "rom fill: the table of %.4s is cut short", code);
            ok = 0;
            break;
        }
        img = Module(r, mid == 0 ? -1 : mid == 1 ? -2 : mid - 2, &base, &mlen);
        if (img == NULL || src < 0 || (unsigned long long)src + len > mlen) {
            snprintf(msg, msgLen, "the ROM has no module %d bytes %llx+%llx (a ROM of another revision?)", mid,
                     (unsigned long long)src, len);
            ok = 0;
            break;
        }
        for (i = 0; i < nreg; i++) {
            if (dst >= regs[i].p && dst + len <= regs[i].p + regs[i].n) {
                inReg = (int)i;
            }
        }
        if (inReg < 0) {
            snprintf(msg, msgLen, "rom fill: a copy to %p is outside its regions", (void *)dst);
            ok = 0;
            break;
        }
        for (b = 0; b < len; b++) {
            h = (h ^ img[src + b]) * 0x100000001B3ULL;
        }
        if (!shared || doShared) {
            memcpy(dst, img + src, (size_t)len);
            if (regs[inReg].prot & 2) {   // writable data: the overlay and restart snapshots get the ROM's values too
                PortOvl_Keep(dst, (size_t)len);
                PORT_MachineKeep(dst, (size_t)len);
            }
        }
        total += (unsigned long)len;
        prevDst = dst + len;
        prevSrc = src + (long long)len;
    }
    for (i = 0; i < nreg; i++) {
        if (!(regs[i].prot & 2) && !FillProtect(&regs[i], 0) && ok) {
            snprintf(msg, msgLen, "rom fill: can't restore the protection of %p", (void *)regs[i].p);
            ok = 0;
        }
    }
    if (ok && h != want) {
        snprintf(msg, msgLen, "the ROM's data is not the data this build was made for (a ROM of another revision?)");
        ok = 0;
    }
    if (ok) {
        sFillShared = 1;
    }
    *bytes = total;
    *copies = n;
    return ok;
}
#else
static const unsigned char *FillGame(const char *code) {
    (void)code;
    return NULL;
}

static int FillImage(BwAssetRom *r, const char *code, unsigned long *bytes, unsigned *copies, char *msg, size_t msgLen) {
    (void)r, (void)code, (void)msg, (void)msgLen;
    *bytes = 0;
    *copies = 0;
    return 1;
}
#endif

// Fills the game's tables (code: its ROM header's game code) from its ROM, once per process; 0 when the build registered
// none (macOS and iOS for now: their C still includes the ROM build's values)
int PORT_AssetsLoad(const char *code) {
    const BwAssetGame *g = NULL;
    BwAssetRom *r;
    char path[1024], msg[1400];
    struct timespec t0, t1;
    int i, k, filled = 0, log = getenv("BWPORT_ASSET_LOG") != NULL;
    unsigned long bytes = 0, fillBytes = 0;
    unsigned fillCopies = 0;

    if (code == NULL) {
        return 0;
    }
    if (sNReg > REG_MAX) {
        Fail("asset_load.c: more tables than REG_MAX");
    }
    for (k = 0; k < GAMES_MAX && sLoaded[k][0] != 0; k++) {
        if (memcmp(sLoaded[k], code, 4) == 0) {
            return 1;
        }
    }
    for (i = 0; i < sNReg && strncmp(sReg[i].game, code, 4) != 0; i++) {
    }
    if (i == sNReg && FillGame(code) == NULL) {
        return 0;
    }
    if (i < sNReg) {
        for (g = bwasset_games; g->code != NULL && strncmp(g->code, code, 4) != 0; g++) {
        }
    }
    if (i < sNReg && g->code == NULL) {
        snprintf(msg, sizeof(msg), "asset_load.c: no rows for game %.4s", code);
        Fail(msg);
    }
    clock_gettime(CLOCK_MONOTONIC, &t0);
    if (!FindRom(code, path, sizeof(path))) {
        snprintf(msg, sizeof(msg), "the game's data is read from its ROM at start, and no ROM with the game code %.4s was "
                 "found (the launcher's copy, BWPORT_ROM or a .nds in the working directory)", code);
        Fail(msg);
    }
    r = calloc(1, sizeof(BwAssetRom));
    if (r == NULL || (r->f = fopen(path, "rb")) == NULL || fread(r->head, 1, sizeof(r->head), r->f) != sizeof(r->head)) {
        snprintf(msg, sizeof(msg), "%s: can't read its header", path);
        Fail(msg);
    }
    for (i = 0; i < sNReg; i++) {
        const BwAssetReg *e = &sReg[i];
        const BwAssetRow *row = NULL;
        const unsigned char *img;
        unsigned base = 0, len = 0;
        int j;

        if (strncmp(e->game, code, 4) != 0) {
            continue;
        }
        for (j = 0; j < g->count; j++) {
            if (strcmp(g->rows[j].name, e->name) == 0) {
                row = &g->rows[j];
                break;
            }
        }
        if (row == NULL) {
            snprintf(msg, sizeof(msg), "asset %s: not in %.4s's rows", e->name, code);
            Fail(msg);
        }
        img = Module(r, row->module, &base, &len);
        if (img == NULL || row->start < base || row->end > base + len || row->end < row->start ||
                !Fill(row, e->layout != NULL ? e->layout : row->layout, img + (row->start - base), e->p, e->size)) {
            snprintf(msg, sizeof(msg), "asset %s: %s has no module %d bytes %08x-%08x of the table's %lu (a ROM of "
                     "another revision?)", e->name, path, row->module, row->start, row->end, e->size);
            Fail(msg);
        }
        PortOvl_Keep(e->p, e->size);
        PORT_MachineKeep(e->p, e->size);
        if (log) {
            printf("asset: %s module %d %08x-%08x -> %p (%lu bytes)\n", e->name, row->module, row->start, row->end,
                   e->p, e->size);
        }
        if (getenv("BWPORT_ASSET_DUMP") != NULL) {   // a test's: name, layout and the table's bytes as filled, one line each
            FILE *d = fopen(getenv("BWPORT_ASSET_DUMP"), "a");
            unsigned long b;

            if (d != NULL) {
                fprintf(d, "%.4s %s %s ", code, e->name, e->layout != NULL ? e->layout : row->layout);
                for (b = 0; b < e->size; b++) {
                    fprintf(d, "%02x", ((const unsigned char *)e->p)[b]);
                }
                fprintf(d, "\n");
                fclose(d);
            }
        }
        filled++;
        bytes += e->size;
    }
    if (!FillImage(r, code, &fillBytes, &fillCopies, msg, sizeof(msg))) {   // the whole image's game data
        char full[1600];

        snprintf(full, sizeof(full), "%s: %s", path, msg);
        Fail(full);
    }
    for (i = 0; i < 2 + 512; i++) {
        free(r->mod[i]);
    }
    fclose(r->f);
    free(r);
    for (k = 0; k < GAMES_MAX && sLoaded[k][0] != 0; k++) {
    }
    if (k < GAMES_MAX) {
        memcpy(sLoaded[k], code, 4);
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    printf("assets: %.4s: %d tables, %lu bytes, the image's data %lu bytes in %u copies, from %s in %.1f ms\n", code,
           filled, bytes, fillBytes, fillCopies, path,
           (double)(t1.tv_sec - t0.tv_sec) * 1e3 + (double)(t1.tv_nsec - t0.tv_nsec) / 1e6);
    fflush(stdout);
    return 1;
}

#ifdef BWASSET_TEST
// cc -DBWASSET_TEST -Iport/include port/src/asset_load.c -o t && ./t ROM OUTDIR: the ARM9 module and each overlay as
// asset_load.c reads them (compare with dsd's extract: arm9/arm9.bin's head, arm9_overlays/ovNNN.bin)
const BwAssetGame bwasset_games[] = {{0, 0, 0}};
void PortOvl_Keep(const void *p, size_t n) { (void)p; (void)n; }
void PORT_MachineKeep(const void *p, size_t n) { (void)p; (void)n; }
int main(int argc, char **argv) {
    BwAssetRom r = {0};
    int m;
    char out[1100];

    if (argc != 3 || (r.f = fopen(argv[1], "rb")) == NULL || fread(r.head, 1, 0x200, r.f) != 0x200) {
        return 2;
    }
    for (m = -1; m < 512; m++) {
        unsigned base, len;
        const unsigned char *img = Module(&r, m, &base, &len);
        FILE *f;

        if (img == NULL) {
            continue;
        }
        snprintf(out, sizeof(out), m < 0 ? "%s/arm9.bin" : "%s/ov%03d.bin", argv[2], m);
        if ((f = fopen(out, "wb")) != NULL) {
            fwrite(img, 1, len, f);
            fclose(f);
        }
        printf("%d %08x %u\n", m, base, len);
    }
    return 0;
}
#endif
