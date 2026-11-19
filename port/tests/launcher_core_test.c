// Unit tests of the launcher core (port/src/launcher_core.c) on synthetic files, no ROM and no game. port/tests/
// run_launcher_core_test.sh builds it three times (every game in the table; Black and White; Black alone) and runs each:
// - ROM identification (LC_ImportRom): each game of the table by its header's code; another region and an unknown code are
//   refused by name, a header with a bad CRC, a short file and one bigger than its chip as no clean DS ROM; a file that is not
//   the supported dump (another revision: the header's byte 0x1E, or any other byte) by its sha1, and nothing is left behind;
//   a trimmed dump hashes as the full chip padded with 0xFF.
// - save validation per family (LC_CheckSave): Black, White and Black 2, White 2 (one 16-byte footer ending each copy: the
//   save's end at +4, the magic 0x31053527 at +8; src/system/save_table.c), Platinum (pret's 20-byte SaveBlockFooter ending
//   each of its two blocks), HeartGold and SoulSilver (pret's SaveChunkFooter, third_party/pokeheartgold/include/save.h and
//   src/save.c: 16 bytes ending each of two chunks: the save counter, the chunk's size at +4, the magic 0x20060623 at +8, the
//   slot at +12; the general chunk 0xf628 bytes at 0, the boxes 0x12310 bytes at 0xf700 (rounded up to 0x100), the two
//   copies 0x40000 apart).
// - LC_PlayableNames and LC_Edition, into small buffers too.
// A check of an item of the rev1 review that is not fixed yet (sOpen) prints "xfail" and passes; once fixed it prints XPASS
// and fails: then remove the item from sOpen.
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "launcher_core.h"

typedef unsigned char u8;

static const int sOpen[] = {0};    // the rev1 review's items still open: none (item 1 fixed by lcfix1)
static u8 sSave[LC_SAVE_SIZE];
static int sFails;
static char sRoot[256];

// the core's test script can fetch covers (port/src/launcher_cover.c, the network): not linked here
int LC_FetchCover(const LC_Game *g, char *err, size_t n) {
    return -1;
}

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
    sFails += open ? ok : !ok;
}

static void Put32(u8 *p, unsigned v) {
    p[0] = v;
    p[1] = v >> 8;
    p[2] = v >> 16;
    p[3] = v >> 24;
}

// ---------------------------------------------------------------- saves

// Black, White, Black 2, White 2: the footer ending the copy at `copy`, the save's end `end` (0x23f9c; 0x25fa4)
static void Gen5(unsigned copy, unsigned end) {
    u8 *f = sSave + copy + end - 16;
    Put32(f + 4, end);
    Put32(f + 8, 0x31053527);
}

// Platinum: a block's 20-byte footer, the block ending at `end` in the copy
static void Plat(unsigned copy, unsigned end, unsigned size, unsigned id) {
    u8 *f = sSave + copy + end - 20;
    Put32(f, 7);
    Put32(f + 4, 3);
    Put32(f + 8, size);
    Put32(f + 12, 0x20060623);
    f[16] = id;
}

// HeartGold, SoulSilver: a chunk's 16-byte footer, the chunk at `start` of `size` bytes in the copy
static void Hgss(unsigned copy, unsigned start, unsigned size, unsigned slot) {
    u8 *f = sSave + copy + start + size - 16;
    Put32(f, 9);
    Put32(f + 4, size);
    Put32(f + 8, 0x20060623);
    f[12] = slot;
    f[13] = 0;
}

static void HgssSave(unsigned generalCopy, unsigned boxesCopy) {
    memset(sSave, 0, sizeof(sSave));
    Hgss(generalCopy, 0, 0xf628, 0);
    Hgss(boxesCopy, 0xf700, 0x12310, 1);
}

static void Check(const char *what, const char *code, int want, const char *wantMsg, int item) {
    char err[200] = "";
    const LC_Game *g = LC_GameByCode(code);
    int r;

    if (g == NULL) {
        return;    // not in this build's table
    }
    r = LC_CheckSave(g, sSave, sizeof(sSave), err, sizeof(err));
    Verdict(r == want && (wantMsg == NULL || strstr(err, wantMsg) != NULL), item, "%s as %s: %d %s (want %d%s%s)", what, code,
            r, err, want, wantMsg ? " " : "", wantMsg ? wantMsg : "");
}

// the save in sSave is game `from`'s: checked as game `code` it is refused by name (when both are in this build's table)
static void Cross(const char *from, const char *code, int item) {
    const LC_Game *f = LC_GameByCode(from), *g = LC_GameByCode(code);
    char what[80], msg[120];

    if (f != NULL && g != NULL) {
        snprintf(what, sizeof(what), "a %s save", f->shortName);
        snprintf(msg, sizeof(msg), "This is a %s save, not a %s save.", f->title, g->title);
        Check(what, code, -3, msg, item);
    }
}

static void Saves(void) {
    static const char *const codes[] = {"IRBO", "IRAO", "IREO", "IRDO", "CPUE", "IPKE", "IPGE"};
    int i;
    char err[200];

    for (i = 0; i < 7; i++) {
        memset(sSave, 0xff, sizeof(sSave));
        Check("all 0xff (a new game)", codes[i], LC_SAVE_NEW, NULL, 0);
        memset(sSave, 0, sizeof(sSave));
        Check("all zeros", codes[i], -2, "holds no Pokemon", 0);
    }
    Verdict(LC_CheckSave(LC_GameByCode("IRBO"), sSave, 1000, err, sizeof(err)) == -1 && strstr(err, "524288") != NULL, 0,
            "a file of the wrong size: %s", err);

    // Black and White: one layout, so each takes the other's save
    memset(sSave, 0, sizeof(sSave));
    Gen5(0, 0x23f9c);
    Check("a Black save (main copy)", "IRBO", LC_SAVE_GOOD, NULL, 0);
    Check("a Black save (main copy)", "IRAO", LC_SAVE_GOOD, NULL, 0);
    Cross("IRBO", "IREO", 0);
    Cross("IRBO", "CPUE", 0);
    Cross("IRBO", "IPKE", 0);
    memset(sSave, 0, sizeof(sSave));
    Gen5(0x24000, 0x23f9c);
    Check("a Black save (backup copy)", "IRBO", LC_SAVE_GOOD, NULL, 0);
    memset(sSave, 0, sizeof(sSave));
    Gen5(0, 0x23f9c);
    Put32(sSave + 0x23f9c - 16 + 4, 0x23f98);
    Check("a Black footer stating another end", "IRBO", -2, NULL, 0);
    memset(sSave, 0, sizeof(sSave));
    Gen5(0, 0x23f9c);
    Put32(sSave + 0x23f9c - 16 + 8, 0x31053528);
    Check("a Black footer with another magic", "IRBO", -2, NULL, 0);

    // Black 2 and White 2: the same footer, another end and backup offset
    memset(sSave, 0, sizeof(sSave));
    Gen5(0, 0x25fa4);
    Check("a Black 2 save (main copy)", "IREO", LC_SAVE_GOOD, NULL, 0);
    Check("a Black 2 save (main copy)", "IRDO", LC_SAVE_GOOD, NULL, 0);
    Cross("IREO", "IRBO", 0);
    memset(sSave, 0, sizeof(sSave));
    Gen5(0x26000, 0x25fa4);
    Check("a Black 2 save (backup copy)", "IRDO", LC_SAVE_GOOD, NULL, 0);
    memset(sSave, 0, sizeof(sSave));
    Gen5(0x24000, 0x25fa4);
    Check("a Black 2 footer at Black's backup offset", "IREO", -2, NULL, 0);

    // Platinum: both blocks' footers, each in either copy
    memset(sSave, 0, sizeof(sSave));
    Plat(0, 0xcf2c, 0xcf2c, 0);
    Plat(0x40000, 0x1f110, 0x121e4, 1);
    Check("a Platinum save (blocks in different copies)", "CPUE", LC_SAVE_GOOD, NULL, 0);
    Cross("CPUE", "IPKE", 0);
    Cross("CPUE", "IRBO", 0);
    memset(sSave, 0, sizeof(sSave));
    Plat(0, 0xcf2c, 0xcf2c, 0);
    Check("a Platinum save without the boxes' footer", "CPUE", -2, NULL, 0);

    // HeartGold and SoulSilver: one layout (the same module), a footer ending each chunk, each chunk in either copy
    HgssSave(0, 0);
    Check("a HeartGold save (main copy)", "IPKE", LC_SAVE_GOOD, NULL, 1);
    Check("a HeartGold save (main copy)", "IPGE", LC_SAVE_GOOD, NULL, 1);
    Cross("IPKE", "CPUE", 1);
    Cross("IPKE", "IRBO", 1);
    HgssSave(0x40000, 0x40000);
    Check("a HeartGold save (backup copy)", "IPKE", LC_SAVE_GOOD, NULL, 1);
    HgssSave(0x40000, 0);
    Check("a SoulSilver save (chunks in different copies)", "IPGE", LC_SAVE_GOOD, NULL, 1);
    memset(sSave, 0, sizeof(sSave));
    Hgss(0, 0, 0xf628, 0);
    Check("a HeartGold save without the boxes' footer", "IPKE", -2, NULL, 0);
    HgssSave(0, 0);
    sSave[0xf700 + 0x12310 - 16 + 12] = 0;
    Check("a HeartGold save, the boxes' chunk in slot 0", "IPKE", -2, NULL, 0);
    HgssSave(0, 0);
    Put32(sSave + 0xf700 + 0x12310 - 16 + 4, 0x12300);
    Check("a HeartGold save, the boxes' size wrong", "IPKE", -2, NULL, 0);
    memset(sSave, 0, sizeof(sSave));
    Hgss(0, 0, 0xf628, 0);
    Hgss(0, 0xf628, 0x12310, 1);
    Check("a HeartGold save, the boxes right after the general chunk (not at 0xf700)", "IPKE", -2, NULL, 0);
}

// ---------------------------------------------------------------- ROMs

static unsigned Crc16(const u8 *p, size_t n) {    // CRC-16/MODBUS, the header's at 0x15E
    unsigned crc = 0xffff;
    int b;

    while (n-- > 0) {
        crc ^= *p++;
        for (b = 0; b < 8; b++) {
            crc = crc & 1 ? (crc >> 1) ^ 0xa001 : crc >> 1;
        }
    }
    return crc;
}

// a synthetic ROM: a header with the code, revision and chip size 0x20000 << chipShift, then `size` bytes in all, `fill` after it
static const char *Rom(const char *name, const char *code, int rev, long size, int fill, int badCrc) {
    static char path[512];
    u8 h[0x200];
    unsigned crc;
    FILE *f;
    long i;

    memset(h, 0, sizeof(h));
    memcpy(h, "SYNTHETIC", 9);
    memcpy(h + 0x0c, code, 4);
    h[0x14] = 0;
    h[0x1e] = rev;
    crc = Crc16(h, 0x15e) ^ (badCrc ? 1 : 0);
    h[0x15e] = crc;
    h[0x15f] = crc >> 8;
    snprintf(path, sizeof(path), "%s/%s", sRoot, name);
    f = fopen(path, "wb");
    fwrite(h, 1, size < (long)sizeof(h) ? (size_t)size : sizeof(h), f);
    for (i = sizeof(h); i < size; i++) {
        fputc(fill, f);
    }
    fclose(f);
    return path;
}

static int Exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}

static int Import(const char *path, char *err, size_t n) {
    const LC_Game *g = NULL;
    err[0] = 0;
    return LC_ImportRom(path, &g, err, n);
}

static void Sha1Of(const char *err, char out[41]) {
    const char *p = strstr(err, "(sha1 ");
    snprintf(out, 41, "%s", p != NULL ? p + 6 : "");
}

static void Roms(void) {
    int count, i, r;
    const LC_Game *all = LC_Games(&count);
    char err[400], want[200], dir[600], code[5], a[41], b[41], c[41];

    for (i = 0; i < count; i++) {
        const LC_Game *g = &all[i];
        int rev;
        for (rev = 0; rev < 2; rev++) {
            r = Import(Rom("x.nds", g->code, rev, 0x200, 0xff, 0), err, sizeof(err));
            snprintf(dir, sizeof(dir), "%s/%s", sRoot, g->folder);
            if (g->sha1[0] == NULL) {
                snprintf(want, sizeof(want), "%s (%s) is not playable in this build yet", g->title, g->code);
                Verdict(r == -2 && strstr(err, want) != NULL, 0, "%s rev %d, not playable here: %d %s", g->code, rev, r, err);
            } else {
                snprintf(want, sizeof(want), "This %s ROM is not the supported dump", g->title);
                Verdict(r == -3 && strstr(err, want) != NULL && !Exists(dir) && !LC_HasRom(g), 0,
                        "%s rev %d, not the supported dump: refused by sha1, nothing left: %d %s", g->code, rev, r, err);
            }
        }
        memcpy(code, g->code, 4);
        code[3] = 'J';
        code[4] = 0;
        r = Import(Rom("x.nds", code, 0, 0x200, 0xff, 0), err, sizeof(err));
        snprintf(want, sizeof(want), "This is %s (Japan, %s)", g->title, code);
        Verdict(r == -2 && strstr(err, want) != NULL, 0, "%s's Japanese code: %d %s", g->code, r, err);
    }
    r = Import(Rom("x.nds", "ABCD", 0, 0x200, 0xff, 0), err, sizeof(err));
    Verdict(r == -2 && strstr(err, "(ABCD), a game this build doesn't play") != NULL, 0, "an unknown code: %d %s", r, err);
    r = Import(Rom("x.nds", "IRBO", 0, 0x200, 0xff, 1), err, sizeof(err));
    Verdict(r == -2 && strstr(err, "not a DS ROM") != NULL, 0, "a header with a bad CRC: %d %s", r, err);
    r = Import(Rom("x.nds", "IRBO", 0, 0x100, 0xff, 0), err, sizeof(err));
    Verdict(r == -2 && strstr(err, "not a DS ROM") != NULL, 0, "a file shorter than a header: %d %s", r, err);
    r = Import(Rom("x.nds", "IRBO", 0, 0x20001, 0xff, 0), err, sizeof(err));
    Verdict(r == -2 && strstr(err, "bigger than its chip") != NULL, 0, "a file bigger than its chip: %d %s", r, err);
    snprintf(dir, sizeof(dir), "%s/missing.nds", sRoot);
    r = Import(dir, err, sizeof(err));
    Verdict(r == -1, 0, "a missing file: %d %s", r, err);

    // a trimmed dump hashes as the chip padded with 0xFF: the same sha1 as the full file; another byte gives another one
    Import(Rom("x.nds", "IRBO", 0, 0x200, 0xff, 0), err, sizeof(err));
    Sha1Of(err, a);
    Import(Rom("x.nds", "IRBO", 0, 0x20000, 0xff, 0), err, sizeof(err));
    Sha1Of(err, b);
    Import(Rom("x.nds", "IRBO", 0, 0x20000, 0x00, 0), err, sizeof(err));
    Sha1Of(err, c);
    Verdict(strlen(a) == 40 && strcmp(a, b) == 0 && strcmp(a, c) != 0, 0, "a trimmed dump's sha1 is the full chip's: %s %s %s",
            a, b, c);
}

// ---------------------------------------------------------------- names

static void Names(void) {
    static const struct {
        const char *code, *edition;
    } eds[] = {
        {"IRBO", "USA / Europe  \xc2\xb7  NDSi Enhanced"}, {"IRAO", "USA / Europe  \xc2\xb7  NDSi Enhanced"},
        {"IREO", "USA / Europe  \xc2\xb7  NDSi Enhanced"}, {"IRDO", "USA / Europe  \xc2\xb7  NDSi Enhanced"},
        {"CPUE", "USA  \xc2\xb7  Rev 1"}, {"IPKE", "USA"}, {"IPGE", "USA"},
    };
#if defined(BWPORT_WHITE) && defined(BWPORT_B2) && defined(BWPORT_W2) && defined(BWPORT_PLAT) && defined(BWPORT_HG)
    const char *names = "Black, White, Black 2, White 2, Platinum, HeartGold and SoulSilver";
#elif defined(BWPORT_WHITE)
    const char *names = "Black and White";
#else
    const char *names = "Black";
#endif
    char line[200], small[16];
    size_t i;
    int k, ok;

    for (i = 0; i < sizeof(eds) / sizeof(eds[0]); i++) {
        const LC_Game *g = LC_GameByCode(eds[i].code);
        if (g != NULL) {
            LC_Edition(g, line, sizeof(line));
            Verdict(strcmp(line, eds[i].edition) == 0, 0, "the edition line of %s: %s", eds[i].code, line);
        }
    }
    LC_PlayableNames(line, sizeof(line));
    Verdict(strcmp(line, names) == 0, 0, "the playable names: %s", line);

    // a small buffer: cut, terminated, nothing written past it
    for (k = 1; k <= 8; k += 7) {
        memset(small, '#', sizeof(small));
        LC_PlayableNames(small, (size_t)k);
        ok = memchr(small, 0, (size_t)k) != NULL && strncmp(small, names, strlen(small)) == 0 && small[k] == '#';
        memset(small, '#', sizeof(small));
        LC_Edition(LC_GameByCode("IRBO"), small, (size_t)k);
        ok &= memchr(small, 0, (size_t)k) != NULL && strncmp(small, eds[0].edition, strlen(small)) == 0 && small[k] == '#';
        Verdict(ok, 0, "the names and the edition into %d bytes", k);
    }
}

int main(void) {
    snprintf(sRoot, sizeof(sRoot), "%s/lc_test.XXXXXX", getenv("TMPDIR") ? getenv("TMPDIR") : "/tmp");
    if (mkdtemp(sRoot) == NULL) {
        printf("FAIL no temporary folder\n");
        return 1;
    }
    LC_Init(sRoot, sRoot);    // (root and work one folder, as on Windows)
    Saves();
    Roms();
    Names();
    {
        char cmd[400];
        snprintf(cmd, sizeof(cmd), "rm -rf '%s'", sRoot);
        if (system(cmd) != 0) {
            printf("note: %s not removed\n", sRoot);
        }
    }
    printf(sFails == 0 ? "PASS\n" : "FAIL (%d)\n", sFails);
    return sFails != 0;
}
