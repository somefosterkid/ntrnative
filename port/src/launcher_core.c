// The launcher's core: see launcher_core.h and docs/launcher.md (sections 4 to 6, "The maintainer's decisions").
//
// Platforms: plain C over stdio, dirent and the POSIX file calls. What each still needs:
// - macOS, iOS, Linux, Android: nothing (fsync, rename over a file, symlink and lstat are POSIX).
// - Windows (mingw): _commit for fsync and MoveFileEx for a rename over a file are here; paths are taken as UTF-8 bytes
//   by fopen, so a root with non-ASCII characters needs the _wfopen family (not done). No symlinks there (none needed:
//   root and work are the same folder).
#include "launcher_core.h"

#include <dirent.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#include <windows.h>
#define LC_MKDIR(p) _mkdir(p)
#else
#define LC_MKDIR(p) mkdir(p, 0755)
#endif

#ifndef _WIN32    // (win64: libntr's u8 and u32, port_prefix.h, are 8 and 32 bits too: unsigned char, unsigned long)
typedef uint8_t u8;
#ifndef SDK_PORT   /* the port build has libntr's (unsigned long on win64) */
typedef uint32_t u32;
#endif
#endif

static void Say(char *err, size_t n, const char *fmt, ...) {
    va_list ap;
    char buf[600];
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    fprintf(stderr, "bwport: launcher: %s\n", buf);
    if (err != NULL && n > 0) {
        snprintf(err, n, "%s", buf);
    }
}

// ---------------------------------------------------------------- SHA-1 (FIPS 180-1)

typedef struct {
    u32 h[5];
    unsigned long long len;
    u8 buf[64];
    u32 used;
} Sha1;

#define ROL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

static void Sha1Block(Sha1 *s, const u8 *p) {
    u32 w[80], a = s->h[0], b = s->h[1], c = s->h[2], d = s->h[3], e = s->h[4];
    int i;
    for (i = 0; i < 16; i++) {
        w[i] = (u32)p[i * 4] << 24 | (u32)p[i * 4 + 1] << 16 | (u32)p[i * 4 + 2] << 8 | p[i * 4 + 3];
    }
    for (; i < 80; i++) {
        w[i] = ROL(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    }
    for (i = 0; i < 80; i++) {
        u32 f, k, t;
        if (i < 20) {
            f = (b & c) | (~b & d), k = 0x5a827999;
        } else if (i < 40) {
            f = b ^ c ^ d, k = 0x6ed9eba1;
        } else if (i < 60) {
            f = (b & c) | (b & d) | (c & d), k = 0x8f1bbcdc;
        } else {
            f = b ^ c ^ d, k = 0xca62c1d6;
        }
        t = ROL(a, 5) + f + e + k + w[i];
        e = d, d = c, c = ROL(b, 30), b = a, a = t;
    }
    s->h[0] += a, s->h[1] += b, s->h[2] += c, s->h[3] += d, s->h[4] += e;
}

static void Sha1Init(Sha1 *s) {
    static const u32 h[5] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0};
    memcpy(s->h, h, sizeof(h));
    s->len = 0;
    s->used = 0;
}

static void Sha1Update(Sha1 *s, const u8 *p, size_t n) {
    s->len += n;
    while (n > 0) {
        size_t k = 64 - s->used < n ? 64 - s->used : n;
        memcpy(s->buf + s->used, p, k);
        s->used += (u32)k, p += k, n -= k;
        if (s->used == 64) {
            Sha1Block(s, s->buf);
            s->used = 0;
        }
    }
}

static void Sha1Hex(Sha1 *s, char out[41]) {
    unsigned long long bits = s->len * 8;
    u8 pad = 0x80, len[8];
    int i;
    Sha1Update(s, &pad, 1);
    pad = 0;
    while (s->used != 56) {
        Sha1Update(s, &pad, 1);
    }
    for (i = 0; i < 8; i++) {
        len[i] = (u8)(bits >> (56 - i * 8));
    }
    Sha1Update(s, len, 8);
    for (i = 0; i < 20; i++) {
        sprintf(out + i * 2, "%02x", (unsigned)(s->h[i / 4] >> (24 - (i % 4) * 8)) & 0xff);
    }
}

// ---------------------------------------------------------------- the table

static const LC_Game sGames[] = {
    {"IRBO", "Pokemon Black", "Black", "Pokemon Black", {"26ad0b9967aa279c4a266ee69f52b9b2332399a5", NULL},
     "Pokemon - Black Version (USA, Europe) (NDSi Enhanced)", 0x23f9c, 0x24000},
    // White plays when its C is in the build (port/meson.build: its ROM was there for its assets; the user's dump)
#ifdef BWPORT_WHITE
    {"IRAO", "Pokemon White", "White", "Pokemon White", {"bc696a0dfb448c7b3a8a206f0f8214411a039208", NULL},
#else
    {"IRAO", "Pokemon White", "White", "Pokemon White", {NULL, NULL},
#endif
     "Pokemon - White Version (USA, Europe) (NDSi Enhanced)", 0x23f9c, 0x24000},
    // Black 2 and White 2 play when pokebw2's C is in the build (port/meson.build: its ROM was there; the maintainers' notes)
#ifdef BWPORT_B2
    {"IREO", "Pokemon Black 2", "Black 2", "Pokemon Black 2", {"e51e6dfb8678a3d19dcd2a10691b96a569ca0abb", NULL},
     "Pokemon - Black Version 2 (USA, Europe) (NDSi Enhanced)", 0x25fa4, 0x26000},
#endif
#ifdef BWPORT_W2
    {"IRDO", "Pokemon White 2", "White 2", "Pokemon White 2", {"b5d7490be7b415b8f1e672a53e978a9cc667e56a", NULL},
     "Pokemon - White Version 2 (USA, Europe) (NDSi Enhanced)", 0x25fa4, 0x26000},
#endif
};
#define GAME_COUNT ((int)(sizeof(sGames) / sizeof(sGames[0])))

const LC_Game *LC_Games(int *count) {
    *count = GAME_COUNT;
    return sGames;
}

const LC_Game *LC_GameByCode(const char *code) {
    int i;
    for (i = 0; code != NULL && i < GAME_COUNT; i++) {
        if (strcmp(sGames[i].code, code) == 0) {
            return &sGames[i];
        }
    }
    return NULL;
}

// ---------------------------------------------------------------- paths and small file helpers

static char sRoot[LC_PATH] = ".", sWork[LC_PATH] = ".";
static time_t sClock;

void LC_Init(const char *root, const char *work) {
    snprintf(sRoot, sizeof(sRoot), "%s", root);
    snprintf(sWork, sizeof(sWork), "%s", work != NULL ? work : root);
}

void LC_SetClock(time_t now) {
    sClock = now;
}

static time_t Now(void) {
    return sClock != 0 ? sClock : time(NULL);
}

void LC_GameDir(const LC_Game *g, char *out, size_t n) {
    snprintf(out, n, "%s/%s", sRoot, g->folder);
}

void LC_GameWorkDir(const LC_Game *g, char *out, size_t n) {
    snprintf(out, n, "%s/%s", sWork, g->folder);
}

static int Exists(const char *path) {    // anything there, a dangling link too
    struct stat st;
#ifdef _WIN32
    return stat(path, &st) == 0;
#else
    return lstat(path, &st) == 0;
#endif
}

static int IsFile(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

static int IsDir(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

static int IsLink(const char *path) {
#ifdef _WIN32
    (void)path;
    return 0;
#else
    struct stat st;
    return lstat(path, &st) == 0 && S_ISLNK(st.st_mode);
#endif
}

static int SyncClose(FILE *f) {
    int bad = fflush(f) != 0;
#ifdef _WIN32
    bad |= _commit(_fileno(f)) != 0;
#else
    bad |= fsync(fileno(f)) != 0;
#endif
    bad |= fclose(f) != 0;
    return bad ? -1 : 0;
}

static int RenameOver(const char *from, const char *to) {    // replaces to when it exists
#ifdef _WIN32
    return MoveFileExA(from, to, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) ? 0 : -1;
#else
    return rename(from, to);
#endif
}

// a whole small file (a save, launcher.ini) into buf: its size, or -1; a file bigger than max reads as max + 1
static long ReadAll(const char *path, u8 *buf, long max) {
    FILE *f = fopen(path, "rb");
    long n;
    if (f == NULL) {
        return -1;
    }
    n = (long)fread(buf, 1, (size_t)max, f);
    if (n == max && fgetc(f) != EOF) {
        n = max + 1;
    }
    fclose(f);
    return n;
}

// path.tmp written, fsynced, read back and compared, then renamed over path (replace: 0 = never over a file)
static int WriteVerified(const char *path, const u8 *data, long size, int replace) {
    char tmp[LC_PATH + 8];
    static u8 back[LC_SAVE_SIZE + 1];
    u8 *check = size <= LC_SAVE_SIZE ? back : malloc((size_t)size + 1);
    FILE *f;
    int ok;

    snprintf(tmp, sizeof(tmp), "%s.tmp", path);
    if (check == NULL || (f = fopen(tmp, "wb")) == NULL) {
        if (check != back) {
            free(check);
        }
        return -1;
    }
    ok = fwrite(data, 1, (size_t)size, f) == (size_t)size;
    ok &= SyncClose(f) == 0;
    ok = ok && ReadAll(tmp, check, size) == size && memcmp(check, data, (size_t)size) == 0;
    if (check != back) {
        free(check);
    }
    if (ok && !replace && Exists(path)) {
        ok = 0;
    }
    if (!ok || (replace ? RenameOver(tmp, path) : rename(tmp, path)) != 0) {
        remove(tmp);
        return -1;
    }
    return 0;
}

// ---------------------------------------------------------------- the ROM's file list (its FNT and FAT)

static volatile u32 sExtractBytes, sRomSize, sCopyPercent;

int PORT_ExtractPercent(void) {    // the extraction's progress, for the UIs' bars (ios_launcher.m)
    return sRomSize != 0 ? (int)((unsigned long long)sExtractBytes * 100 / sRomSize) : 0;
}

int LC_CopyPercent(void) {
    return (int)sCopyPercent;
}

typedef struct Walk Walk;
// isDir: 0 a file (start, end its bytes), 1 a directory before its entries, 2 after; depth 0 = the root's entries.
// Returns 1 to skip a directory's entries, -1 to stop
typedef int (*WalkFn)(Walk *w, const char *rel, int isDir, u32 start, u32 end, int depth);
struct Walk {
    FILE *rom;
    WalkFn fn;
    const char *dir;    // the files' folder
    int count, failed;
};

static u32 Read32(FILE *f, u32 offset) {
    u8 b[4] = {0};
    fseek(f, (long)offset, SEEK_SET);
    if (fread(b, 1, 4, f) != 4) {
        return 0;
    }
    return (u32)b[0] | ((u32)b[1] << 8) | ((u32)b[2] << 16) | ((u32)b[3] << 24);
}

static int WalkDir(Walk *w, u32 fnt, u32 fat, u32 dirId, const char *prefix, int depth) {
    u32 sub = fnt + Read32(w->rom, fnt + (dirId & 0xfff) * 8);
    u32 fileId = Read32(w->rom, fnt + (dirId & 0xfff) * 8 + 4) & 0xffff;
    int guard = 0;

    while (guard++ < 65536) {
        u8 kind, name[128];
        char rel[512];
        u32 len;
        int r;

        fseek(w->rom, (long)sub, SEEK_SET);
        if (fread(&kind, 1, 1, w->rom) != 1 || kind == 0) {
            break;
        }
        len = kind & 0x7f;
        if (fread(name, 1, len, w->rom) != len) {
            break;
        }
        name[len] = '\0';
        snprintf(rel, sizeof(rel), "%s%s", prefix, (const char *)name);
        sub += 1 + len;
        if (kind & 0x80) {
            u8 id[2];
            char subPrefix[512];
            fseek(w->rom, (long)sub, SEEK_SET);
            if (fread(id, 1, 2, w->rom) != 2) {
                break;
            }
            sub += 2;
            if ((r = w->fn(w, rel, 1, 0, 0, depth)) < 0) {
                return -1;
            }
            if (r == 0 && depth < 16) {
                snprintf(subPrefix, sizeof(subPrefix), "%s/", rel);
                if (WalkDir(w, fnt, fat, (u32)id[0] | ((u32)id[1] << 8), subPrefix, depth + 1) < 0 ||
                    w->fn(w, rel, 2, 0, 0, depth) < 0) {
                    return -1;
                }
            }
        } else {
            u32 start = Read32(w->rom, fat + fileId * 8);
            u32 end = Read32(w->rom, fat + fileId * 8 + 4);
            if (w->fn(w, rel, 0, start, end, depth) < 0) {
                return -1;
            }
            fileId++;
        }
    }
    return 0;
}

static int Walk_(Walk *w, const char *rom) {
    int r;
    if ((w->rom = fopen(rom, "rb")) == NULL) {
        return -1;
    }
    r = WalkDir(w, Read32(w->rom, 0x40), Read32(w->rom, 0x48), 0xf000, "", 0);
    fclose(w->rom);
    w->rom = NULL;
    return r;
}

static unsigned sSlowUs;    // BWPORT_EXTRACT_SLOW_MS: a pause per file, to look at the iOS progress screen

static int CopyOut(Walk *w, const char *path, u32 start, u32 end) {
    static u8 buf[1 << 16];
    FILE *out = fopen(path, "wb");
    if (out == NULL) {
        return 0;
    }
    fseek(w->rom, (long)start, SEEK_SET);
    while (start < end) {
        u32 n = end - start > sizeof(buf) ? (u32)sizeof(buf) : end - start;
        if (fread(buf, 1, n, w->rom) != n || fwrite(buf, 1, n, out) != n) {
            fclose(out);
            return 0;
        }
        start += n;
        sExtractBytes += n;
    }
    fclose(out);
    if (sSlowUs != 0) {
        usleep(sSlowUs);
    }
    return 1;
}

static int ExtractFn(Walk *w, const char *rel, int isDir, u32 start, u32 end, int depth) {
    char path[LC_PATH + 512];
    (void)depth;
    snprintf(path, sizeof(path), "%s/%s", w->dir, rel);
    if (isDir == 1) {
        LC_MKDIR(path);
    } else if (isDir == 0) {
        long pos = ftell(w->rom);
        if (CopyOut(w, path, start, end)) {
            w->count++;
        } else {
            w->failed++;
        }
        fseek(w->rom, pos, SEEK_SET);
    }
    return 0;
}

int LC_ExtractRom(const char *rom, const char *dir) {
    Walk w = {0};
    char path[LC_PATH + 16];
    FILE *f = fopen(rom, "rb");

    if (f == NULL) {
        return 0;
    }
    fprintf(stderr, "bwport: extracting %s\n", rom);
    fseek(f, 0, SEEK_END);
    sRomSize = (u32)ftell(f);
    fclose(f);
    sExtractBytes = 0;
    sSlowUs = getenv("BWPORT_EXTRACT_SLOW_MS") != NULL ? (unsigned)atoi(getenv("BWPORT_EXTRACT_SLOW_MS")) * 1000 : 0;
    w.fn = ExtractFn;
    w.dir = dir;
    if (Walk_(&w, rom) == 0 && w.count > 0 && (w.rom = fopen(rom, "rb")) != NULL) {
        snprintf(path, sizeof(path), "%s/header.bin", dir);
        CopyOut(&w, path, 0, 0x200);    // libntr's main() reads its first 0x160 bytes; FS takes it as "extracted"
        fclose(w.rom);
    }
    fprintf(stderr, "bwport: %d files extracted\n", w.count);
    return w.count;
}

static int RemoveFn(Walk *w, const char *rel, int isDir, u32 start, u32 end, int depth) {
    char path[LC_PATH + 512];
    (void)start, (void)end, (void)depth;
    snprintf(path, sizeof(path), "%s/%s", w->dir, rel);
    if (isDir == 0) {
        if (remove(path) == 0) {
            w->count++;
        } else if (errno != ENOENT) {
            w->failed++;
        }
    } else if (isDir == 2) {
        rmdir(path);    // only when empty: anything not the ROM's stays
    }
    return 0;
}

// The ROM's files out of dir, by the ROM's list; header.bin and libntr's DbgLog.txt too (never a wildcard)
static int RemoveFiles(const char *rom, const char *dir) {
    Walk w = {0};
    char path[LC_PATH + 16];
    w.fn = RemoveFn;
    w.dir = dir;
    if (Walk_(&w, rom) != 0) {
        return -1;
    }
    snprintf(path, sizeof(path), "%s/header.bin", dir);
    remove(path);
    snprintf(path, sizeof(path), "%s/DbgLog.txt", dir);
    remove(path);
    return w.failed ? -1 : w.count;
}

// ---------------------------------------------------------------- ROM import and removal

int LC_HasRom(const LC_Game *g) {
    char path[LC_PATH * 2];
    snprintf(path, sizeof(path), "%s/%s/%s.nds", sRoot, g->folder, g->folder);
    return IsFile(path);
}

int LC_HasFiles(const LC_Game *g) {
    char a[LC_PATH * 2], b[LC_PATH * 2];
    snprintf(a, sizeof(a), "%s/%s/header.bin", sWork, g->folder);
    snprintf(b, sizeof(b), "%s/%s/a/0/0/0", sWork, g->folder);
    return IsFile(a) && IsFile(b);
}

int LC_HasSave(const LC_Game *g) {
    char path[LC_PATH * 2];
    snprintf(path, sizeof(path), "%s/%s/save.bin", sRoot, g->folder);
    return IsFile(path);
}

int LC_Scan(const LC_Game **out, int max) {
    int i, n = 0;
    for (i = 0; i < GAME_COUNT && n < max; i++) {
        char dir[LC_PATH * 2];
        LC_GameDir(&sGames[i], dir, sizeof(dir));
        if (IsDir(dir)) {
            out[n++] = &sGames[i];
        }
    }
    return n;
}

static u32 Crc16(const u8 *p, size_t n) {    // the header's CRC at 0x15E (CRC-16/MODBUS over 0x000-0x15D)
    u32 crc = 0xffff;
    size_t i;
    int b;
    for (i = 0; i < n; i++) {
        crc ^= p[i];
        for (b = 0; b < 8; b++) {
            crc = (crc & 1) ? (crc >> 1) ^ 0xa001 : crc >> 1;
        }
    }
    return crc;
}

static const char *RegionName(char c) {
    switch (c) {
    case 'O': return "USA/Europe";
    case 'E': return "USA";
    case 'P': return "Europe";
    case 'J': return "Japan";
    case 'K': return "Korea";
    case 'D': return "Germany";
    case 'F': return "France";
    case 'I': return "Italy";
    case 'S': return "Spain";
    default: return "another region";
    }
}

// The header: a DS ROM, a game in the table and a playable one. *chip gets the chip size the hash pads to
static const LC_Game *CheckHeader(FILE *f, long long size, long long *chip, char *err, size_t n) {
    u8 h[0x200];
    char code[5], title[13];
    const LC_Game *g;
    int i;

    fseek(f, 0, SEEK_SET);
    if (size < 0x200 || fread(h, 1, sizeof(h), f) != sizeof(h) ||
        Crc16(h, 0x15e) != ((u32)h[0x15e] | (u32)h[0x15f] << 8)) {
        Say(err, n, "This file is not a DS ROM (%lld bytes, no valid DS header).", size);
        return NULL;
    }
    memcpy(code, h + 0x0c, 4);
    code[4] = 0;
    memcpy(title, h, 12);
    title[12] = 0;
    *chip = 0x20000LL << (h[0x14] & 0x0f);
    if ((g = LC_GameByCode(code)) == NULL) {
        for (i = 0; i < GAME_COUNT; i++) {
            if (memcmp(sGames[i].code, code, 3) == 0) {
                Say(err, n, "This is %s (%s, %s). This build plays the %s dump (%s) only.", sGames[i].title,
                    RegionName(code[3]), code, RegionName(sGames[i].code[3]), sGames[i].code);
                return NULL;
            }
        }
        Say(err, n, "This ROM is \"%s\" (%s), a game this build doesn't play.", title, code);
        return NULL;
    }
    if (g->sha1[0] == NULL) {
        Say(err, n, "%s (%s) is not playable in this build yet.", g->title, code);
        return NULL;
    }
    if (size > *chip) {
        Say(err, n, "This %s file is bigger than its chip (%lld bytes): not a clean dump.", g->title, size);
        return NULL;
    }
    return g;
}

int LC_ImportRom(const char *path, const LC_Game **game, char *err, size_t n) {
    FILE *in = fopen(path, "rb"), *out = NULL;
    const LC_Game *g = NULL;
    char dir[LC_PATH * 2], work[LC_PATH * 2], dst[LC_PATH * 3], tmp[LC_PATH * 3], hex[41];
    long long size, chip, done = 0;
    int madeDir = 0, madeWork = 0, ok = 0, i, match = 0, hadFiles;
    size_t cap = 1 << 20, k;
    u8 *buf = NULL;
    Sha1 s;

    sCopyPercent = 0;
    if (in == NULL) {
        Say(err, n, "That file couldn't be read.");
        return -1;
    }
    fseek(in, 0, SEEK_END);
    size = ftell(in);
    if ((g = CheckHeader(in, size, &chip, err, n)) == NULL) {
        fclose(in);
        return -2;
    }
    LC_GameDir(g, dir, sizeof(dir));
    LC_GameWorkDir(g, work, sizeof(work));
    madeDir = !IsDir(dir) && LC_MKDIR(dir) == 0;
    madeWork = strcmp(dir, work) != 0 && !IsDir(work) && LC_MKDIR(work) == 0;
    snprintf(dst, sizeof(dst), "%s/%s.nds", dir, g->folder);
    snprintf(tmp, sizeof(tmp), "%s/import.tmp", dir);
    hadFiles = LC_HasFiles(g);
    if ((out = fopen(tmp, "wb")) == NULL || (buf = malloc(cap)) == NULL) {
        Say(err, n, "The ROM couldn't be written into %s.", dir);
        goto fail;
    }
    Sha1Init(&s);
    fseek(in, 0, SEEK_SET);
    while ((k = fread(buf, 1, cap, in)) > 0) {
        Sha1Update(&s, buf, k);
        if (fwrite(buf, 1, k, out) != k) {
            Say(err, n, "The ROM couldn't be copied (is the disk full?).");
            goto fail;
        }
        done += (long long)k;
        sCopyPercent = (u32)(done * 100 / size);
    }
    if (done != size || SyncClose(out) != 0) {
        out = NULL;
        Say(err, n, "The ROM couldn't be copied (is the disk full?).");
        goto fail;
    }
    out = NULL;
    memset(buf, 0xff, cap);    // a trimmed dump hashes as the full chip: the rest is 0xFF
    for (; done < chip; done += (long long)k) {
        k = chip - done > (long long)cap ? cap : (size_t)(chip - done);
        Sha1Update(&s, buf, k);
    }
    Sha1Hex(&s, hex);
    fprintf(stderr, "bwport: launcher: import %s: %lld bytes, sha1 (padded to %lld) %s\n", path, size, chip, hex);
    for (i = 0; i < 2 && g->sha1[i] != NULL; i++) {
        match |= strcmp(hex, g->sha1[i]) == 0;
    }
    if (!match) {
        Say(err, n, "This %s ROM is not the supported dump (sha1 %s): it may be a bad dump or another version.",
            g->title, hex);
        goto fail;
    }
    if (RenameOver(tmp, dst) != 0) {
        Say(err, n, "The ROM couldn't be put in place (%s).", strerror(errno));
        goto fail;
    }
    if (!hadFiles && LC_ExtractRom(dst, work) <= 0) {
        RemoveFiles(dst, work);
        remove(dst);
        Say(err, n, "The ROM's files couldn't be extracted (is the disk full?).");
        goto fail;
    }
    ok = 1;
fail:
    if (out != NULL) {
        fclose(out);
    }
    fclose(in);
    free(buf);
    if (!ok) {
        remove(tmp);
        if (madeWork) {
            rmdir(work);
        }
        if (madeDir) {
            rmdir(dir);    // only when empty
        }
        return -3;
    }
    if (game != NULL) {
        *game = g;
    }
    fprintf(stderr, "bwport: launcher: imported %s into %s\n", g->title, dir);
    return 0;
}

int LC_RemoveRom(const LC_Game *g, char *err, size_t n) {
    char dir[LC_PATH * 2], work[LC_PATH * 2], rom[LC_PATH * 3], path[LC_PATH * 3];
    int r;

    LC_GameDir(g, dir, sizeof(dir));
    LC_GameWorkDir(g, work, sizeof(work));
    snprintf(rom, sizeof(rom), "%s/%s.nds", dir, g->folder);
    if (!IsFile(rom)) {
        Say(err, n, "%s's ROM copy is not there, so its file list can't be read: nothing was removed.", g->title);
        return -1;
    }
    if ((r = RemoveFiles(rom, work)) < 0) {
        Say(err, n, "Some of %s's files couldn't be removed.", g->title);
        return -2;
    }
    if (remove(rom) != 0) {
        Say(err, n, "%s's ROM copy couldn't be removed (%s).", g->title, strerror(errno));
        return -3;
    }
    if (strcmp(dir, work) != 0) {    // iOS: the links main_start.c made in the working directory, then the folder if empty
        snprintf(path, sizeof(path), "%s/save.bin", work);
        if (IsLink(path)) {
            unlink(path);
        }
        snprintf(path, sizeof(path), "%s/sim_config.ini", work);
        if (IsLink(path)) {
            unlink(path);
        }
        rmdir(work);
    }
    fprintf(stderr, "bwport: launcher: removed %s: %d files and the ROM copy; save.bin and backups/ stay\n", g->title, r);
    return 0;
}

// ---------------------------------------------------------------- saves

static u32 Get32(const u8 *p) {
    return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
}

// The game's own footer test (lib/gfl/src/backup_system.c; src/system/save_table.c; pokebw2's the same): 16 bytes ending the
// save, with SAVE_MAGIC at +8 and the save's size, the footer's own end, at +4; in the main copy (0) or the backup. The
// size differs by generation (Black, White 0x23f9c; Black 2, White 2 0x25fa4), so a save of another game is told apart
static int FooterOk(const LC_Game *g, const u8 *d, size_t copy) {
    const u8 *f = d + copy + g->saveEnd - 16;
    return Get32(f + 8) == 0x31053527 && Get32(f + 4) == g->saveEnd;
}

int LC_CheckSave(const LC_Game *g, const u8 *d, size_t size, char *err, size_t n) {
    size_t i;
    const LC_Game *o;
    int count;
    if (size != LC_SAVE_SIZE) {
        Say(err, n, "A DS save is %d bytes; this file is %zu.", LC_SAVE_SIZE, size);
        return -1;
    }
    for (i = 0; i < size && d[i] == 0xff; i++) {
    }
    if (i == size) {
        return LC_SAVE_NEW;
    }
    if (FooterOk(g, d, 0) || FooterOk(g, d, g->saveCopy)) {
        return LC_SAVE_GOOD;
    }
    o = LC_Games(&count);
    for (i = 0; i < (size_t)count; i++) {
        if (&o[i] != g && (FooterOk(&o[i], d, 0) || FooterOk(&o[i], d, o[i].saveCopy))) {
            Say(err, n, "This is a %s save, not a %s save.", o[i].title, g->title);
            return -3;
        }
    }
    Say(err, n, "This file is the right size but holds no %s save.", g->title);
    return -2;
}

static long ReadSaveFile(const char *path, u8 *d, long max, char *err, size_t n);

// a save file (.sav, or a DeSmuME .dsv cut at its footer) against g's footer test: what the launchers ask before an import
int LC_CheckSaveFile(const LC_Game *g, const char *path, char *err, size_t n) {
    static u8 d[LC_SAVE_SIZE + 4096];
    long size = ReadSaveFile(path, d, sizeof(d) - 1, err, n);
    return size < 0 ? -1 : LC_CheckSave(g, d, (size_t)size, err, n);
}

static void SavePath(const LC_Game *g, char *out, size_t n) {
    snprintf(out, n, "%s/%s/save.bin", sRoot, g->folder);
}

int LC_ExportSave(const LC_Game *g, const char *dir, char *out, size_t outN, char *err, size_t n) {
    static u8 d[LC_SAVE_SIZE + 1];
    char src[LC_PATH * 2];
    long size;

    SavePath(g, src, sizeof(src));
    if ((size = ReadAll(src, d, LC_SAVE_SIZE)) != LC_SAVE_SIZE) {
        Say(err, n, "%s has no save to export.", g->title);
        return -1;
    }
    snprintf(out, outN, "%s/%s.sav", dir, g->title);
    if (WriteVerified(out, d, size, 1) != 0) {
        Say(err, n, "The save couldn't be written to %s.", out);
        return -2;
    }
    fprintf(stderr, "bwport: launcher: exported %s's save to %s\n", g->title, out);
    return 0;
}

static const char kDsvSnip[] = "|<--Snip above here to create a raw sav by excluding this DeSmuME savedata footer:";
static const char kDsvEnd[] = "|-DESMUME SAVE-|";

// a .sav as it is; a DeSmuME .dsv cut at its marker
static long ReadSaveFile(const char *path, u8 *d, long max, char *err, size_t n) {
    long size = ReadAll(path, d, max);
    size_t k = sizeof(kDsvEnd) - 1, m = sizeof(kDsvSnip) - 1;
    if (size < 0) {
        Say(err, n, "That file couldn't be read.");
        return -1;
    }
    if (size <= max && (size_t)size >= k && memcmp(d + size - k, kDsvEnd, k) == 0) {
        if (size < LC_SAVE_SIZE + (long)m || memcmp(d + LC_SAVE_SIZE, kDsvSnip, m) != 0) {
            Say(err, n, "This DeSmuME save doesn't have %d bytes before its footer.", LC_SAVE_SIZE);
            return -2;
        }
        fprintf(stderr, "bwport: launcher: a DeSmuME .dsv: cut at %d bytes\n", LC_SAVE_SIZE);
        return LC_SAVE_SIZE;
    }
    return size;
}

// The play time and the date of the last save, as the game keeps them (include/save/playtime.h: hours u16, minutes u8,
// seconds u8, then the save's date as year:7 month:4 day:5 hour:5 minute:6) in the trainer data block at 0x19400 + 0x24 of
// each copy (the main one at 0, the backup at g->saveCopy; PKHeX's SAV5BW agrees). Black 2 and White 2 keep the block at the
// same offset (checked on their saves; pokebw2's block table places it by the sizes before it, 256-aligned).
// The newer date of the two is the save's. g NULL: whichever game's save it is.
int LC_SaveFileInfo(const LC_Game *g, const char *path, long *seconds, time_t *saved) {
    static u8 d[LC_SAVE_SIZE + 4096];
    long size = IsFile(path) ? ReadSaveFile(path, d, sizeof(d) - 1, NULL, 0) : -1;
    u32 best = 0, copy;
    int i, count;
    const LC_Game *all = LC_Games(&count);

    if (size != LC_SAVE_SIZE) {
        return -1;
    }
    if (g == NULL) {
        for (i = 0; i < count && g == NULL; i++) {
            if (LC_CheckSave(&all[i], d, LC_SAVE_SIZE, NULL, 0) == LC_SAVE_GOOD) {
                g = &all[i];
            }
        }
    }
    if (g == NULL || LC_CheckSave(g, d, LC_SAVE_SIZE, NULL, 0) != LC_SAVE_GOOD) {
        return -1;
    }
    for (copy = 0; copy <= g->saveCopy; copy += g->saveCopy) {
        const u8 *p = d + copy + 0x19400 + 0x24;
        u32 when = Get32(p + 4);
        if (FooterOk(g, d, copy) && when != 0xffffffff && when >= best) {
            struct tm t = {0};
            best = when;
            *seconds = (long)(p[0] | p[1] << 8) * 3600 + p[2] * 60 + p[3];
            t.tm_year = 100 + (int)(when & 127);
            t.tm_mon = (int)(when >> 7 & 15) - 1;
            t.tm_mday = (int)(when >> 11 & 31);
            t.tm_hour = (int)(when >> 16 & 31);
            t.tm_min = (int)(when >> 21 & 63);
            t.tm_isdst = -1;
            *saved = mktime(&t);
        }
    }
    return best != 0 ? 0 : -1;
}

// before every import, restore or deletion: a copy of save.bin in backups/, read back; only the last 20 are kept
static void Prune(const LC_Game *g);

static int Backup(const LC_Game *g, const char *suffix, char *err, size_t n) {
    static u8 d[LC_SAVE_SIZE + 1];
    char src[LC_PATH * 2], dir[LC_PATH * 2], dst[LC_PATH * 3], stamp[32];
    time_t t = Now();
    long size;

    SavePath(g, src, sizeof(src));
    if ((size = ReadAll(src, d, LC_SAVE_SIZE)) < 0) {
        return 1;    // no save: nothing to keep
    }
    snprintf(dir, sizeof(dir), "%s/%s/backups", sRoot, g->folder);
    LC_MKDIR(dir);
    do {    // two backups in one second: the second's name a second on, so none replaces another
        strftime(stamp, sizeof(stamp), "%Y-%m-%d-%H%M%S", localtime(&t));
        snprintf(dst, sizeof(dst), "%s/save-%s%s.bin", dir, stamp, suffix);
        t++;
    } while (Exists(dst) && t < Now() + 100);
    if (WriteVerified(dst, d, size > LC_SAVE_SIZE ? LC_SAVE_SIZE : size, 0) != 0) {
        Say(err, n, "A backup of the save couldn't be written (%s): nothing was changed.", dst);
        return -1;
    }
    fprintf(stderr, "bwport: launcher: backup %s\n", dst);
    Prune(g);
    return 0;
}

int LC_BackupBefore(const LC_Game *g, char *err, size_t n) {
    return Backup(g, "-before", err, n) < 0 ? -1 : 0;
}

// the new save (in d) over save.bin: tmp, fsync, read back, rename
static int PutSave(const LC_Game *g, const u8 *d, char *err, size_t n) {
    char dir[LC_PATH * 2], path[LC_PATH * 2];
    LC_GameDir(g, dir, sizeof(dir));
    LC_MKDIR(dir);
    SavePath(g, path, sizeof(path));
    if (WriteVerified(path, d, LC_SAVE_SIZE, 1) != 0) {
        Say(err, n, "The save couldn't be written: the old one stays.");
        return -1;
    }
    return 0;
}

int LC_ImportSave(const LC_Game *g, const char *path, char *err, size_t n) {
    static u8 d[LC_SAVE_SIZE + 4096];
    long size = ReadSaveFile(path, d, sizeof(d) - 1, err, n);
    int kind;

    if (size < 0) {
        return -1;
    }
    if ((kind = LC_CheckSave(g, d, (size_t)size, err, n)) < 0) {
        return -2;
    }
    if (LC_BackupBefore(g, err, n) != 0 || PutSave(g, d, err, n) != 0) {
        return -3;
    }
    fprintf(stderr, "bwport: launcher: imported a save into %s (%s)\n", g->title, kind == LC_SAVE_NEW ? "a new game" : "checked");
    return 0;
}

int LC_DeleteSave(const LC_Game *g, char *err, size_t n) {
    char path[LC_PATH * 2];
    SavePath(g, path, sizeof(path));
    if (!IsFile(path)) {
        return 0;
    }
    if (LC_BackupBefore(g, err, n) != 0) {
        return -1;
    }
    if (remove(path) != 0) {
        Say(err, n, "The save couldn't be deleted (%s).", strerror(errno));
        return -2;
    }
    fprintf(stderr, "bwport: launcher: deleted %s's save (a backup was made)\n", g->title);
    return 0;
}

// ---------------------------------------------------------------- backups: listing, pruning, the tiered copies

static int ParseBackup(const char *name, LC_BackupInfo *b) {
    struct tm tm = {0};
    char rest[32];
    size_t len = strlen(name);
    if (strcmp(name, "save-before-launcher.bin") == 0) {
        snprintf(b->name, sizeof(b->name), "%s", name);
        b->when = 0;
        b->kind = LC_BACKUP_MIGRATION;
        return 1;
    }
    if (len >= sizeof(b->name) || sscanf(name, "save-%4d-%2d-%2d-%2d%2d%2d%31s", &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
                                         &tm.tm_hour, &tm.tm_min, &tm.tm_sec, rest) != 7) {
        return 0;
    }
    if (strcmp(rest, ".bin") == 0) {
        b->kind = LC_BACKUP_TIER;
    } else if (strcmp(rest, "-before.bin") == 0) {
        b->kind = LC_BACKUP_BEFORE;
    } else {
        return 0;
    }
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    tm.tm_isdst = -1;
    b->when = mktime(&tm);
    snprintf(b->name, sizeof(b->name), "%s", name);
    return 1;
}

static int NewerFirst(const void *a, const void *b) {
    const LC_BackupInfo *x = a, *y = b;
    if (x->when != y->when) {
        return x->when < y->when ? 1 : -1;
    }
    return strcmp(y->name, x->name);
}

#define MAX_BACKUPS 512

int LC_ListBackups(const LC_Game *g, LC_BackupInfo *out, int max) {
    char dir[LC_PATH * 2];
    DIR *d;
    struct dirent *e;
    int n = 0;

    snprintf(dir, sizeof(dir), "%s/%s/backups", sRoot, g->folder);
    if ((d = opendir(dir)) == NULL) {
        return 0;
    }
    while ((e = readdir(d)) != NULL && n < max) {
        n += ParseBackup(e->d_name, &out[n]);
    }
    closedir(d);
    qsort(out, (size_t)n, sizeof(out[0]), NewerFirst);
    return n;
}

static long DayOf(time_t t) {    // the local calendar day, counted from 1970
    struct tm tm = *localtime(&t);
    return (long)(tm.tm_year + 1900) * 366 + tm.tm_yday;    // ordered; only equality and order are used
}

static long MonthOf(time_t t) {
    struct tm tm = *localtime(&t);
    return (long)(tm.tm_year + 1900) * 12 + tm.tm_mon;
}

// The tiers (the maintainer's decision): the newest copy of each day for a week, of each week for a month, of each calendar month
// for a year; none older. Buckets are fixed on the calendar (a week is 7 days counted from 1970), so a copy kept as its
// week's newest is still its week's newest tomorrow: nothing kept flips to deleted as the days go on, apart from ageing out.
// The "before" copies: the newest 20. The migration's: always.
static void Prune(const LC_Game *g) {
    static LC_BackupInfo b[MAX_BACKUPS];
    char path[LC_PATH * 3];
    time_t now = Now();
    int n = LC_ListBackups(g, b, MAX_BACKUPS), i, j, before = 0;

    for (i = 0; i < n; i++) {    // newest first
        int keep = 0;
        if (b[i].kind == LC_BACKUP_MIGRATION) {
            keep = 1;
        } else if (b[i].kind == LC_BACKUP_BEFORE) {
            keep = ++before <= 20;
        } else {
            double age = difftime(now, b[i].when) / 86400.0;
            int newest = 1;
            for (j = 0; j < i && newest; j++) {    // a newer tiered copy in the same bucket
                if (b[j].kind != LC_BACKUP_TIER) {
                    continue;
                }
                if (age < 7) {
                    newest = DayOf(b[j].when) != DayOf(b[i].when);
                } else if (age < 31) {
                    newest = (long)(b[j].when / 86400 / 7) != (long)(b[i].when / 86400 / 7);
                } else {
                    newest = MonthOf(b[j].when) != MonthOf(b[i].when);
                }
            }
            keep = age < 366 && newest;
        }
        if (!keep) {
            snprintf(path, sizeof(path), "%s/%s/backups/%s", sRoot, g->folder, b[i].name);
            if (remove(path) == 0) {
                fprintf(stderr, "bwport: launcher: pruned backup %s\n", b[i].name);
            }
        }
    }
}

// A tiered copy of save.bin as the game left it, when it differs from the newest tiered copy. main_start.c takes one at
// each start, before the game runs: a start always happens (iOS ends an app without telling it, a crash or a kill -9 has
// no exit), no game is writing then, so the copy is never caught mid-save, and it is the save of the last session
int LC_TierBackup(const LC_Game *g) {
    static LC_BackupInfo b[MAX_BACKUPS];
    static u8 cur[LC_SAVE_SIZE + 1], last[LC_SAVE_SIZE + 1];
    char path[LC_PATH * 3];
    long size;
    int n, i;

    SavePath(g, path, sizeof(path));
    if ((size = ReadAll(path, cur, LC_SAVE_SIZE)) != LC_SAVE_SIZE) {
        return 0;
    }
    n = LC_ListBackups(g, b, MAX_BACKUPS);
    for (i = 0; i < n; i++) {
        if (b[i].kind == LC_BACKUP_TIER) {
            snprintf(path, sizeof(path), "%s/%s/backups/%s", sRoot, g->folder, b[i].name);
            if (ReadAll(path, last, LC_SAVE_SIZE) == size && memcmp(cur, last, (size_t)size) == 0) {
                Prune(g);
                return 0;
            }
            break;
        }
    }
    return Backup(g, "", NULL, 0) == 0 ? 1 : 0;
}

int LC_RestoreBackup(const LC_Game *g, const char *name, char *err, size_t n) {
    static u8 d[LC_SAVE_SIZE + 1];
    char path[LC_PATH * 3];
    LC_BackupInfo info;

    if (strchr(name, '/') != NULL || strchr(name, '\\') != NULL || !ParseBackup(name, &info)) {
        Say(err, n, "%s is not one of the backups.", name);
        return -1;
    }
    snprintf(path, sizeof(path), "%s/%s/backups/%s", sRoot, g->folder, name);
    if (ReadAll(path, d, LC_SAVE_SIZE) != LC_SAVE_SIZE || LC_CheckSave(g, d, LC_SAVE_SIZE, err, n) < 0) {
        Say(err, n, "The backup %s couldn't be read or is damaged.", name);
        return -2;
    }
    if (LC_BackupBefore(g, err, n) != 0 || PutSave(g, d, err, n) != 0) {
        return -3;
    }
    fprintf(stderr, "bwport: launcher: restored %s from %s\n", g->title, name);
    return 0;
}

// ---------------------------------------------------------------- launcher.ini

static char sLast[8];
static int sStart = LC_START_LAST, sLayout = LC_LAYOUT_LIBRARY, sCovers = 1, sSkipMenu, sIniRead;

static void ReadIni(void) {
    char path[LC_PATH + 16], line[128];
    FILE *f;
    if (sIniRead) {
        return;
    }
    sIniRead = 1;
    snprintf(path, sizeof(path), "%s/launcher.ini", sRoot);
    if ((f = fopen(path, "r")) == NULL) {
        return;
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        if (strncmp(line, "LastGame=", 9) == 0) {
            snprintf(sLast, sizeof(sLast), "%s", line + 9);
        } else if (strncmp(line, "Start=", 6) == 0) {
            sStart = strcmp(line + 6, "launcher") == 0 ? LC_START_LAUNCHER : LC_START_LAST;
        } else if (strncmp(line, "Layout=", 7) == 0) {
            sLayout = strcmp(line + 7, "tabs") == 0 ? LC_LAYOUT_TABS : LC_LAYOUT_LIBRARY;
        } else if (strncmp(line, "DownloadArtwork=", 16) == 0) {
            sCovers = strcmp(line + 16, "off") != 0;
        } else if (strncmp(line, "SkipMenu=", 9) == 0) {
            sSkipMenu = strcmp(line + 9, "on") == 0;
        }
    }
    fclose(f);
}

static int WriteIni(void) {
    char path[LC_PATH + 16], text[640];
    int len = snprintf(text, sizeof(text), "; bw-port's launcher (port/src/launcher_core.c). Start=last|launcher, Layout=library|tabs,\n"
                       "; DownloadArtwork=on|off (the games' covers from GameTDB or libretro-thumbnails),\n"
                       "; SkipMenu=on|off (a start goes into the save as CONTINUE does: no logos, title or menu)\n"
                       "LastGame=%s\nStart=%s\nLayout=%s\nDownloadArtwork=%s\nSkipMenu=%s\n", sLast,
                       sStart == LC_START_LAUNCHER ? "launcher" : "last", sLayout == LC_LAYOUT_TABS ? "tabs" : "library",
                       sCovers ? "on" : "off", sSkipMenu ? "on" : "off");
    snprintf(path, sizeof(path), "%s/launcher.ini", sRoot);
    return WriteVerified(path, (const u8 *)text, len, 1);
}

const LC_Game *LC_LastGame(void) {
    ReadIni();
    return LC_GameByCode(sLast);
}

void LC_SetLastGame(const LC_Game *g) {
    ReadIni();
    if (strcmp(sLast, g->code) != 0) {
        snprintf(sLast, sizeof(sLast), "%s", g->code);
        WriteIni();
    }
}

int LC_StartMode(void) {
    ReadIni();
    return sStart;
}

void LC_SetStartMode(int mode) {
    ReadIni();
    sStart = mode;
    WriteIni();
}

int LC_Layout(void) {
    ReadIni();
    return sLayout;
}

void LC_SetLayout(int layout) {
    ReadIni();
    sLayout = layout;
    WriteIni();
}

int LC_CoverDownloads(void) {
    ReadIni();
    return sCovers;
}

void LC_SetCoverDownloads(int on) {
    ReadIni();
    sCovers = on != 0;
    WriteIni();
}

int LC_SkipMenu(void) {
    ReadIni();
    return sSkipMenu;
}

void LC_SetSkipMenu(int on) {
    ReadIni();
    sSkipMenu = on != 0;
    WriteIni();
}

// ---------------------------------------------------------------- the migration (section 6)

int LC_IsFlat(void) {
    char a[LC_PATH + 16], b[LC_PATH + 16];
    const LC_Game *found[GAME_COUNT];
    int i;
    snprintf(a, sizeof(a), "%s/save.bin", sRoot);
    snprintf(b, sizeof(b), "%s/header.bin", sWork);
    if (!IsFile(a) && !IsFile(b)) {
        return 0;
    }
    for (i = 0; i < GAME_COUNT; i++) {
        char w[LC_PATH * 2];
        LC_GameWorkDir(&sGames[i], w, sizeof(w));
        if (IsDir(w)) {
            return 0;
        }
    }
    return LC_Scan(found, GAME_COUNT) == 0;
}

// the sha1 of a file as a dump of g (padded with 0xFF to its chip); 0 = it is one of g's
static int RomIs(const char *path, const LC_Game *g) {
    FILE *f = fopen(path, "rb");
    long long size, chip;
    const LC_Game *h;
    u8 *buf;
    size_t cap = 1 << 20, k;
    long long done = 0;
    char hex[41];
    Sha1 s;
    int i;

    if (f == NULL) {
        return -1;
    }
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    if ((h = CheckHeader(f, size, &chip, NULL, 0)) != g || (buf = malloc(cap)) == NULL) {
        fclose(f);
        return -1;
    }
    Sha1Init(&s);
    fseek(f, 0, SEEK_SET);
    while ((k = fread(buf, 1, cap, f)) > 0) {
        Sha1Update(&s, buf, k);
        done += (long long)k;
    }
    fclose(f);
    memset(buf, 0xff, cap);
    for (; done < chip; done += (long long)k) {
        k = chip - done > (long long)cap ? cap : (size_t)(chip - done);
        Sha1Update(&s, buf, k);
    }
    free(buf);
    Sha1Hex(&s, hex);
    for (i = 0; i < 2 && g->sha1[i] != NULL; i++) {
        if (strcmp(hex, g->sha1[i]) == 0) {
            return 0;
        }
    }
    return -1;
}

#define MAX_MOVES 64
typedef struct Move {
    char from[LC_PATH + 160], to[LC_PATH + 160];
} Move;

typedef struct {
    Move m[MAX_MOVES];
    int n;
} Moves;

static int sFailMove;    // tests: the Nth move fails (BWPORT_LCTEST failmove=N)

// a rename that never replaces anything, remembered for the undo
static int MoveOne(Moves *mv, const char *from, const char *to, char *err, size_t n) {
    if (sFailMove > 0 && --sFailMove == 0) {
        Say(err, n, "migration: test: the move of %s fails", from);
        return -1;
    }
    if (Exists(to)) {
        Say(err, n, "migration: %s already exists: nothing is overwritten", to);
        return -1;
    }
    if (mv->n == MAX_MOVES || rename(from, to) != 0) {
        Say(err, n, "migration: can't move %s to %s (%s)", from, to, strerror(errno));
        return -1;
    }
    snprintf(mv->m[mv->n].from, sizeof(mv->m[0].from), "%s", from);
    snprintf(mv->m[mv->n].to, sizeof(mv->m[0].to), "%s", to);
    mv->n++;
    return 0;
}

static void Undo(Moves *mv) {
    while (mv->n > 0) {
        mv->n--;
        if (rename(mv->m[mv->n].to, mv->m[mv->n].from) != 0) {
            fprintf(stderr, "bwport: launcher: migration: UNDO FAILED: %s stays at %s\n", mv->m[mv->n].from, mv->m[mv->n].to);
        }
    }
}

typedef struct {
    char name[MAX_MOVES][160];
    int n;
} RootList;

static RootList sRootList;

static int RootFn(Walk *w, const char *rel, int isDir, u32 start, u32 end, int depth) {
    (void)w, (void)start, (void)end;
    if (depth == 0 && isDir != 2 && sRootList.n < MAX_MOVES) {
        snprintf(sRootList.name[sRootList.n++], sizeof(sRootList.name[0]), "%s", rel);
    }
    return 1;    // the root's entries only
}

int LC_Migrate(char *err, size_t n) {
    static Moves mv;
    static const char *const extra[] = {"header.bin", "arm9.bin", "banner.bin", "overlay9", "overlay7"};
    const LC_Game *g = &sGames[0];    // Black: the only game the flat layout ever held
    char dir[LC_PATH * 2], work[LC_PATH * 2], bdir[LC_PATH * 2], rom[LC_PATH * 2], romDst[LC_PATH * 3];
    char from[LC_PATH + 160], to[LC_PATH * 2 + 160], backup[LC_PATH * 3];
    static u8 save[LC_SAVE_SIZE + 1];
    long saveSize;
    int romIsCopy = 0, madeWork = 0, needExtract = 0, i;
    Walk w = {0};
    DIR *d;
    struct dirent *e;

    if (getenv("BWPORT_DIR") != NULL || !LC_IsFlat()) {
        return 1;
    }
    fprintf(stderr, "bwport: launcher: migration: a flat layout at %s: moving %s's files into its folder\n", sRoot, g->title);
    // 1. Black's ROM: a .nds at the root with Black's sha1, else BWPORT_ROM's (copied, not moved: it is outside the root)
    rom[0] = 0;
    if ((d = opendir(sRoot)) != NULL) {
        while ((e = readdir(d)) != NULL && rom[0] == 0) {
            size_t len = strlen(e->d_name);
            if (len > 4 && (strcmp(e->d_name + len - 4, ".nds") == 0 || strcmp(e->d_name + len - 4, ".NDS") == 0)) {
                snprintf(rom, sizeof(rom), "%s/%s", sRoot, e->d_name);
                if (RomIs(rom, g) != 0) {
                    rom[0] = 0;
                }
            }
        }
        closedir(d);
    }
    if (rom[0] == 0 && getenv("BWPORT_ROM") != NULL && RomIs(getenv("BWPORT_ROM"), g) == 0) {
        snprintf(rom, sizeof(rom), "%s", getenv("BWPORT_ROM"));
        romIsCopy = 1;
    }
    if (rom[0] == 0) {
        Say(err, n, "migration: no %s ROM with the supported sha1 at %s or in BWPORT_ROM: the flat layout stays", g->title, sRoot);
        return -1;
    }
    // 2. the folders, and the save's copy: fsynced, read back, compared
    LC_GameDir(g, dir, sizeof(dir));
    LC_GameWorkDir(g, work, sizeof(work));
    snprintf(bdir, sizeof(bdir), "%s/backups", dir);
    if (LC_MKDIR(dir) != 0 || LC_MKDIR(bdir) != 0) {
        rmdir(dir);
        Say(err, n, "migration: can't make %s (%s): the flat layout stays", bdir, strerror(errno));
        return -2;
    }
    if (strcmp(dir, work) != 0) {
        if (LC_MKDIR(work) != 0) {
            Say(err, n, "migration: can't make %s (%s): the flat layout stays", work, strerror(errno));
            goto fail;
        }
        madeWork = 1;
    }
    snprintf(from, sizeof(from), "%s/save.bin", sRoot);
    snprintf(backup, sizeof(backup), "%s/save-before-launcher.bin", bdir);
    saveSize = ReadAll(from, save, LC_SAVE_SIZE);
    if (saveSize >= 0) {
        Sha1 s;
        char hex[41];
        if (saveSize > LC_SAVE_SIZE || WriteVerified(backup, save, saveSize, 0) != 0) {
            Say(err, n, "migration: the save's copy %s couldn't be written and verified: the flat layout stays", backup);
            goto fail;
        }
        Sha1Init(&s);
        Sha1Update(&s, save, (size_t)saveSize);
        Sha1Hex(&s, hex);
        fprintf(stderr, "bwport: launcher: migration: save.bin copied to %s and read back (sha1 %s)\n", backup, hex);
    }
    // 3. the moves (renames on the same volume: the bytes don't change), never over anything
    mv.n = 0;
    if (saveSize >= 0) {
        snprintf(to, sizeof(to), "%s/save.bin", dir);
        if (MoveOne(&mv, from, to, err, n) != 0) {
            goto fail;
        }
    }
    snprintf(romDst, sizeof(romDst), "%s/%s.nds", dir, g->folder);
    if (romIsCopy) {
        Say(NULL, 0, "migration: the ROM is BWPORT_ROM's (%s): it is copied in after the moves", rom);
    } else if (MoveOne(&mv, rom, romDst, err, n) != 0) {
        goto fail;
    }
    sRootList.n = 0;
    w.fn = RootFn;
    if (Walk_(&w, romIsCopy ? rom : romDst) != 0 || sRootList.n == 0) {
        Say(err, n, "migration: the ROM's file list couldn't be read");
        goto fail;
    }
    for (i = 0; i < sRootList.n + (int)(sizeof(extra) / sizeof(extra[0])); i++) {
        const char *name = i < sRootList.n ? sRootList.name[i] : extra[i - sRootList.n];
        snprintf(from, sizeof(from), "%s/%s", sWork, name);
        snprintf(to, sizeof(to), "%s/%s", work, name);
        if (!Exists(from)) {
            needExtract |= i < sRootList.n || strcmp(name, "header.bin") == 0;
            continue;
        }
        if (MoveOne(&mv, from, to, err, n) != 0) {
            goto fail;
        }
    }
    if (romIsCopy) {
        // the ROM copied in last: a failure here undoes the moves above
        FILE *in = fopen(rom, "rb"), *out = fopen(romDst, "wb");
        static u8 buf[1 << 16];
        size_t k;
        int ok = in != NULL && out != NULL;
        while (ok && (k = fread(buf, 1, sizeof(buf), in)) > 0) {
            ok = fwrite(buf, 1, k, out) == k;
        }
        if (in != NULL) {
            fclose(in);
        }
        ok = out != NULL && SyncClose(out) == 0 && ok;
        if (!ok || RomIs(romDst, g) != 0) {
            remove(romDst);
            Say(err, n, "migration: the ROM couldn't be copied to %s", romDst);
            goto fail;
        }
    }
    if (needExtract) {
        Say(NULL, 0, "migration: some extracted files were missing: extracting again");
        if (LC_ExtractRom(romDst, work) <= 0) {
            Undo(&mv);
            RemoveFiles(romIsCopy ? rom : romDst, work);
            if (romIsCopy) {
                remove(romDst);
            }
            Say(err, n, "migration: the extraction failed");
            goto failUndone;
        }
    }
    // the old links (iOS): links, not data
    if (strcmp(sRoot, sWork) != 0) {
        snprintf(from, sizeof(from), "%s/save.bin", sWork);
        if (IsLink(from)) {
            unlink(from);
        }
    }
    // 4. LastGame
    sIniRead = 1;
    snprintf(sLast, sizeof(sLast), "%s", g->code);
    if (WriteIni() != 0) {
        fprintf(stderr, "bwport: launcher: migration: launcher.ini couldn't be written (%s is chosen anyway)\n", g->title);
    }
    fprintf(stderr, "bwport: launcher: migration: done: %d moves into %s\n", mv.n, dir);
    return 0;

fail:
    // 5. any failure: everything back where it was, the copy and the new folders gone
    Undo(&mv);
failUndone:
    remove(backup);
    rmdir(bdir);
    rmdir(dir);
    if (madeWork) {
        rmdir(work);
    }
    fprintf(stderr, "bwport: launcher: migration: failed: everything stays where it was; %s runs from the flat layout\n", g->title);
    return -3;
}

// ---------------------------------------------------------------- the test hook

static time_t ParseDay(const char *s) {
    struct tm tm = {0};
    if (sscanf(s, "%4d-%2d-%2d-%2d%2d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min) < 3) {
        return 0;
    }
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    tm.tm_hour = tm.tm_hour ? tm.tm_hour : 12;
    tm.tm_isdst = -1;
    return mktime(&tm);
}

// BWPORT_LCTEST: steps split by ';', each printed with its result:
//   clock=YYYY-MM-DD[-HHMM]  days=N (the clock N days on)  game=CODE  importrom=PATH  removerom  exportsave=DIR
//   importsave=PATH  deletesave  restore=NAME  backup  tier  list  migrate  check=PATH (LC_CheckSave on a file)
//   saveinfo=PATH (LC_SaveFileInfo)
//   init=ROOT,WORK (another root and working directory, as iOS's Documents and Application Support)
//   covers=on|off  cover (LC_FetchCover)  cover=CODE (a code not in the table, into root/Unknown game)
//   failmove=N (the migration's Nth move fails)  daily=N (N days: the save changed, a tiered copy, the clock a day on)  befores=N (N backups, a minute apart)
// the test's "play": the save's last byte (past the game's data, 0x48000 on) changed, as a session would change the save
static int Played(const LC_Game *g, int day) {
    char path[LC_PATH * 2];
    FILE *f;
    SavePath(g, path, sizeof(path));
    if ((f = fopen(path, "r+b")) == NULL) {
        return -1;
    }
    fseek(f, LC_SAVE_SIZE - 4, SEEK_SET);
    fwrite(&day, sizeof(day), 1, f);
    fclose(f);
    return 0;
}

int LC_TestScript(const char *script) {
    static LC_BackupInfo b[MAX_BACKUPS];
    char buf[4096], err[600], out[LC_PATH * 2];
    const LC_Game *g = &sGames[0];
    char *step, *save = NULL;
    int fails = 0;

    snprintf(buf, sizeof(buf), "%s", script);
    for (step = strtok_r(buf, ";", &save); step != NULL; step = strtok_r(NULL, ";", &save)) {
        char *arg = strchr(step, '=');
        int r = 0, i, n;
        err[0] = 0;
        if (arg != NULL) {
            *arg++ = 0;
        }
        if (strcmp(step, "clock") == 0) {
            LC_SetClock(ParseDay(arg));
        } else if (strcmp(step, "days") == 0) {
            LC_SetClock(Now() + (time_t)atoi(arg) * 86400);
        } else if (strcmp(step, "game") == 0) {
            r = (g = LC_GameByCode(arg)) != NULL ? 0 : -1;
            g = g != NULL ? g : &sGames[0];
        } else if (strcmp(step, "importrom") == 0) {
            r = LC_ImportRom(arg, &g, err, sizeof(err));
            g = g != NULL ? g : &sGames[0];
        } else if (strcmp(step, "removerom") == 0) {
            r = LC_RemoveRom(g, err, sizeof(err));
        } else if (strcmp(step, "exportsave") == 0) {
            r = LC_ExportSave(g, arg, out, sizeof(out), err, sizeof(err));
        } else if (strcmp(step, "importsave") == 0) {
            r = LC_ImportSave(g, arg, err, sizeof(err));
        } else if (strcmp(step, "deletesave") == 0) {
            r = LC_DeleteSave(g, err, sizeof(err));
        } else if (strcmp(step, "restore") == 0) {
            r = LC_RestoreBackup(g, arg, err, sizeof(err));
        } else if (strcmp(step, "backup") == 0) {
            r = LC_BackupBefore(g, err, sizeof(err));
        } else if (strcmp(step, "tier") == 0) {
            r = LC_TierBackup(g);
        } else if (strcmp(step, "daily") == 0) {    // N days of play: the save changes, a start takes its tiered copy
            for (i = 0; i < atoi(arg) && r >= 0; i++) {
                r = Played(g, i);
                LC_TierBackup(g);
                LC_SetClock(Now() + 86400);
            }
        } else if (strcmp(step, "befores") == 0) {    // N backups before actions
            for (i = 0; i < atoi(arg) && r >= 0; i++) {
                r = LC_BackupBefore(g, err, sizeof(err));
                LC_SetClock(Now() + 60);
            }
        } else if (strcmp(step, "init") == 0 && arg != NULL && strchr(arg, ',') != NULL) {    // ROOT,WORK: iOS's split
            *strchr(arg, ',') = 0;
            LC_Init(arg, arg + strlen(arg) + 1);
            arg[strlen(arg)] = ',';
            sIniRead = 0;
        } else if (strcmp(step, "covers") == 0) {    // on|off: launcher.ini's DownloadArtwork
            LC_SetCoverDownloads(arg == NULL || strcmp(arg, "off") != 0);
        } else if (strcmp(step, "cover") == 0) {    // LC_FetchCover for the game, or (=CODE) for a game not in the table
            static LC_Game fake = {"", "Unknown game", "Unknown", "Unknown game", {NULL, NULL}, NULL};
            if (arg != NULL) {
                fake.code = arg;
                snprintf(out, sizeof(out), "%s/%s", sRoot, fake.folder);
                LC_MKDIR(out);
            }
            r = LC_FetchCover(arg != NULL ? &fake : g, err, sizeof(err));
        } else if (strcmp(step, "failmove") == 0) {
            sFailMove = atoi(arg);
        } else if (strcmp(step, "migrate") == 0) {
            r = LC_Migrate(err, sizeof(err));
        } else if (strcmp(step, "check") == 0) {
            static u8 d[LC_SAVE_SIZE + 4096];
            long size = ReadSaveFile(arg, d, sizeof(d) - 1, err, sizeof(err));
            r = size < 0 ? -1 : LC_CheckSave(g, d, (size_t)size, err, sizeof(err));
        } else if (strcmp(step, "saveinfo") == 0) {
            long secs = 0;
            time_t when = 0;
            r = LC_SaveFileInfo(g, arg, &secs, &when);
            fprintf(stderr, "lctest:   %ld:%02ld:%02ld played, saved %s", secs / 3600, secs / 60 % 60, secs % 60, ctime(&when));
        } else if (strcmp(step, "list") == 0) {
            n = LC_ListBackups(g, b, MAX_BACKUPS);
            for (i = 0; i < n; i++) {
                fprintf(stderr, "lctest:   %s\n", b[i].name);
            }
            r = n;
        } else {
            r = -99;
        }
        fprintf(stderr, "lctest: %s%s%s -> %d%s%s\n", step, arg ? "=" : "", arg ? arg : "", r, err[0] ? ": " : "", err);
        fails += r < 0;
    }
    return fails;
}
