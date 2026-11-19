// A unit test of the launcher's save check for HeartGold and SoulSilver (port/src/launcher_core.c, LC_CheckSave, LC_ImportSave)
// on synthetic saves, and on real ones when given: port/tests/run_launcher_hgss_save_test.sh builds it with the launcher core
// alone and runs it (its arguments: HeartGold or SoulSilver saves, each checked as both games, as Platinum's and Black's, then
// imported, imported again and its backup restored). A HeartGold save is Gen 4's (pokeheartgold's src/save.c): the general block (to 0xf628) and the boxes (0xf700
// to 0x21a10) in the copy at 0 and in the one at 0x40000, a 16-byte footer ending each block (include/save.h's
// SaveChunkFooter): the block's size at +4, the magic 0x20060623 at +8, the block's number (u16) at +12.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "launcher_core.h"

typedef unsigned char u8;
static u8 sSave[LC_SAVE_SIZE];

// the core's test script can fetch covers (port/src/launcher_cover.c, the network): not linked here
int LC_FetchCover(const LC_Game *g, char *err, size_t n) {
    return -1;
}
static int sFails;

static void Put32(u8 *p, unsigned v) {
    p[0] = v;
    p[1] = v >> 8;
    p[2] = v >> 16;
    p[3] = v >> 24;
}

// a HeartGold block's footer in a copy: the block ends at `end`
static void Chunk(unsigned copy, unsigned end, unsigned size, unsigned id) {
    u8 *f = sSave + copy + end - 16;
    Put32(f, 7);
    Put32(f + 4, size);
    Put32(f + 8, 0x20060623);
    f[12] = id;
    f[13] = id >> 8;
}

// Platinum's (20 bytes: size +8, magic +12, number +16)
static void PlatFooter(unsigned copy, unsigned end, unsigned size, unsigned id) {
    u8 *f = sSave + copy + end - 20;
    Put32(f + 8, size);
    Put32(f + 12, 0x20060623);
    f[16] = id;
}

static void Expect(const char *what, int r, const char *err, int want, const char *wantMsg) {
    int ok = r == want && (wantMsg == NULL || strstr(err, wantMsg) != NULL);
    printf("%s %s: %d %s\n", ok ? "ok  " : "FAIL", what, r, err);
    sFails += !ok;
}

static void Check(const char *what, const char *code, int want, const char *wantMsg) {
    char err[200] = "";
    const LC_Game *g = LC_GameByCode(code);
    int r = g == NULL ? 99 : LC_CheckSave(g, sSave, sizeof(sSave), err, sizeof(err));
    Expect(what, r, err, want, wantMsg);
}

static void Good(unsigned general, unsigned boxes) {
    memset(sSave, 0, sizeof(sSave));
    Chunk(general, 0xf628, 0xf628, 0);
    Chunk(boxes, 0x21a10, 0x12310, 1);
}

// a real save file: good for both games, refused by name as another game's, imported into a scratch library
static void Real(const char *path, const char *root) {
    char err[600] = "", what[1200], dir[1200];
    long secs;
    time_t when;
    struct stat st;
    const char *codes[] = {"IPKE", "IPGE"};
    int i;

    for (i = 0; i < 2; i++) {
        const LC_Game *g = LC_GameByCode(codes[i]);
        err[0] = 0;
        snprintf(what, sizeof(what), "%s as %s", path, g->shortName);
        Expect(what, LC_CheckSaveFile(g, path, err, sizeof(err)), err, LC_SAVE_GOOD, NULL);
        snprintf(dir, sizeof(dir), "%s/%s", root, g->folder);
        mkdir(dir, 0777);
        err[0] = 0;
        snprintf(what, sizeof(what), "%s imported into %s", path, g->shortName);
        Expect(what, LC_ImportSave(g, path, err, sizeof(err)), err, 0, NULL);
        snprintf(dir, sizeof(dir), "%s/%s/save.bin", root, g->folder);
        Expect("  its save.bin", stat(dir, &st) == 0 ? (int)st.st_size : -1, "", LC_SAVE_SIZE, NULL);
        // the save holds no date (Gen 4): no info, as Platinum's; the front-ends' import asks LC_CheckSaveFile then
        Expect("  its info (none: Gen 4 saves hold no date)", LC_SaveFileInfo(g, path, &secs, &when), "", -1, NULL);
        // a second import backs the first up (backups/save-...-before.bin); that backup restores (LC_RestoreBackup checks it
        // as an import does)
        {
            LC_BackupInfo b[32];
            int nb;
            LC_SetClock(1700000000 + i * 100);
            err[0] = 0;
            Expect("  imported again", LC_ImportSave(g, path, err, sizeof(err)), err, 0, NULL);
            nb = LC_ListBackups(g, b, 32);
            Expect("  a backup of it", nb > 0, "", 1, NULL);
            if (nb > 0) {
                err[0] = 0;
                Expect("  the backup restored", LC_RestoreBackup(g, b[0].name, err, sizeof(err)), err, 0, NULL);
            }
            LC_SetClock(0);
        }
    }
    err[0] = 0;
    snprintf(what, sizeof(what), "%s as Platinum", path);
    Expect(what, LC_CheckSaveFile(LC_GameByCode("CPUE"), path, err, sizeof(err)), err, -3, "Pokemon HeartGold save, not a Pokemon Platinum");
    err[0] = 0;
    snprintf(what, sizeof(what), "%s as Black", path);
    Expect(what, LC_CheckSaveFile(LC_GameByCode("IRBO"), path, err, sizeof(err)), err, -3, "Pokemon HeartGold save, not a Pokemon Black");
}

int main(int argc, char **argv) {
    const LC_Game *g = LC_GameByCode("IPKE"), *s = LC_GameByCode("IPGE");
    char root[] = "/tmp/lc_hgss_XXXXXX";
    int i;

    if (g == NULL || s == NULL || LC_GameByCode("CPUE") == NULL) {
        printf("FAIL HeartGold, SoulSilver or Platinum is not in the table (build with -DBWPORT_PLAT -DBWPORT_HG)\n");
        return 1;
    }
    printf("ok   HeartGold: saveEnd 0x%x saveCopy 0x%x saveBlock0 0x%x saveBoxes 0x%x footer %u\n", g->saveEnd, g->saveCopy,
           g->saveBlock0, g->saveBoxes, g->saveFooter);

    memset(sSave, 0xff, sizeof(sSave));
    Check("all 0xff: a new game", "IPKE", LC_SAVE_NEW, NULL);

    memset(sSave, 0, sizeof(sSave));
    Check("zeros: no save", "IPKE", -2, "holds no Pokemon HeartGold save");

    Good(0, 0);
    Check("both blocks in the main copy", "IPKE", LC_SAVE_GOOD, NULL);
    Check("the same as SoulSilver (one layout)", "IPGE", LC_SAVE_GOOD, NULL);
    Good(0x40000, 0x40000);
    Check("both blocks in the backup copy", "IPKE", LC_SAVE_GOOD, NULL);
    Good(0x40000, 0);
    Check("the blocks newest in different copies", "IPGE", LC_SAVE_GOOD, NULL);
    Good(0, 0x40000);
    Check("the blocks newest in different copies, the other way", "IPKE", LC_SAVE_GOOD, NULL);

    memset(sSave, 0, sizeof(sSave));
    Chunk(0, 0xf628, 0xf628, 0);
    Check("the boxes' footer missing", "IPKE", -2, NULL);

    memset(sSave, 0, sizeof(sSave));
    Chunk(0, 0x21a10, 0x12310, 1);
    Check("the general block's footer missing", "IPKE", -2, NULL);

    memset(sSave, 0, sizeof(sSave));
    Chunk(0, 0xf628, 0xf628, 0);
    Chunk(0, 0x21a10, 0x12310, 0);
    Check("the boxes' block numbered 0", "IPKE", -2, NULL);

    memset(sSave, 0, sizeof(sSave));
    Chunk(0, 0xf628, 0xf628, 0);
    Chunk(0, 0x21a10, 0x21a10, 1);
    Check("the boxes' size the save's end (the old Gen 5-style test)", "IPKE", -2, NULL);

    // Platinum's and HeartGold's saves are told apart by name (the same magic, other footers and offsets)
    Good(0, 0);
    Check("a HeartGold save as Platinum", "CPUE", -3, "Pokemon HeartGold save, not a Pokemon Platinum");
    Check("a HeartGold save as Black", "IRBO", -3, "Pokemon HeartGold save, not a Pokemon Black");
    memset(sSave, 0, sizeof(sSave));
    PlatFooter(0, 0xcf2c, 0xcf2c, 0);
    PlatFooter(0, 0x1f110, 0x121e4, 1);
    Check("a Platinum save as Platinum", "CPUE", LC_SAVE_GOOD, NULL);
    Check("a Platinum save as HeartGold", "IPKE", -3, "Pokemon Platinum save, not a Pokemon HeartGold");
    Check("a Platinum save as SoulSilver", "IPGE", -3, "Pokemon Platinum save, not a Pokemon SoulSilver");

    if (mkdtemp(root) == NULL) {
        printf("FAIL no scratch folder\n");
        return 1;
    }
    // the extraction's mark (LC_HasFilesIn): Platinum's file system has no a/ tree, so header.bin with a/0/0/0 never
    // said "extracted" for it
    {
        char p[200];
        FILE *f;
        snprintf(p, sizeof(p), "%s/header.bin", root);
        fclose(fopen(p, "w"));
        Expect("header.bin alone: not extracted", LC_HasFilesIn(root), "", 0, NULL);
        snprintf(p, sizeof(p), "%s/" LC_EXTRACTED_MARK, root);
        f = fopen(p, "w");
        fclose(f);
        Expect("header.bin and the mark: extracted", LC_HasFilesIn(root), "", 1, NULL);
        remove(p);
        snprintf(p, sizeof(p), "%s/a", root);
        mkdir(p, 0777);
        snprintf(p, sizeof(p), "%s/a/0", root);
        mkdir(p, 0777);
        snprintf(p, sizeof(p), "%s/a/0/0", root);
        mkdir(p, 0777);
        snprintf(p, sizeof(p), "%s/a/0/0/0", root);
        fclose(fopen(p, "w"));
        Expect("header.bin and a/0/0/0 (a Gen 5 extraction before the mark): extracted", LC_HasFilesIn(root), "", 1, NULL);
    }
    if (argc > 1) {
        LC_Init(root, root);
        for (i = 1; i < argc; i++) {
            Real(argv[i], root);
        }
    }
    {
        char rm[64];
        snprintf(rm, sizeof(rm), "rm -rf %s", root);
        if (system(rm) != 0) {
            printf("note: %s not removed\n", root);
        }
    }

    printf(sFails == 0 ? "PASS\n" : "FAIL (%d)\n", sFails);
    return sFails != 0;
}
