// A unit test of the launcher's save check for Platinum (port/src/launcher_core.c, LC_CheckSave) on synthetic saves, no game:
// port/tests/run_launcher_plat_save_test.sh builds it with the launcher core alone and runs it. A Platinum save is Gen 4's
// (pret's src/savedata.c): the general block (0xcf2c bytes) and the boxes (to 0x1f110) in the copy at 0 and in the one at
// 0x40000, a 20-byte footer ending each block: size at +8, signature 0x20060623 at +12, the block's number at +16.
#include <stdio.h>
#include <string.h>
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

// a block's footer in a copy: the block ends at `end`
static void Footer(unsigned copy, unsigned end, unsigned size, unsigned id) {
    u8 *f = sSave + copy + end - 20;
    Put32(f, 7);
    Put32(f + 4, 3);
    Put32(f + 8, size);
    Put32(f + 12, 0x20060623);
    f[16] = id;
}

// the game's footer of Black and White (0x23f9c) and of Black 2 and White 2 (0x25fa4), in the copy at 0 and at the backup's offset
static void GenFiveFooter(unsigned end, unsigned copy) {
    u8 *f = sSave + copy + end - 16;
    Put32(f + 4, end);
    Put32(f + 8, 0x31053527);
}

static void Check(const char *what, const char *code, int want, const char *wantMsg) {
    char err[200] = "";
    const LC_Game *g = LC_GameByCode(code);
    int r = g == NULL ? 99 : LC_CheckSave(g, sSave, sizeof(sSave), err, sizeof(err));
    int ok = r == want && (wantMsg == NULL || strstr(err, wantMsg) != NULL);
    printf("%s %s: %d %s\n", ok ? "ok  " : "FAIL", what, r, err);
    sFails += !ok;
}

static void Edition(const char *code, const char *want) {
    char line[96];
    const LC_Game *g = LC_GameByCode(code);
    int ok;

    if (g == NULL) {
        printf("FAIL %s is not in the table (build with -DBWPORT_PLAT -DBWPORT_HG)\n", code);
        sFails++;
        return;
    }
    LC_Edition(g, line, sizeof(line));
    ok = strcmp(line, want) == 0;
    printf("%s the edition line of %s: %s\n", ok ? "ok  " : "FAIL", code, line);
    sFails += !ok;
}

// every game of the table has its own look (the front-ends read it from the row, none keeps a table of codes), and a light or dark
// theme the showcase's text colour follows; the prompts name the games this build plays
static void Looks(void) {
    int n, i, j, ok = 1;
    const LC_Game *all = LC_Games(&n);
    char names[200];

    for (i = 0; i < n; i++) {
        int mine = all[i].accent != 0 && all[i].deep != 0 && all[i].glow != 0 && (all[i].dark == 0 || all[i].dark == 1);
        for (j = 0; j < i; j++) {
            mine &= all[i].accent != all[j].accent && all[i].glow != all[j].glow;
        }
        printf("%s %s: accent %06x deep %06x glow %06x %s\n", mine ? "ok  " : "FAIL", all[i].shortName, all[i].accent, all[i].deep,
               all[i].glow, all[i].dark ? "dark" : "light");
        ok &= mine;
    }
    sFails += !ok;
    LC_PlayableNames(names, sizeof(names));
    ok = strstr(names, "Platinum") != NULL && strstr(names, "SoulSilver") != NULL && strstr(names, "HeartGold and SoulSilver") != NULL;
    printf("%s the prompt's names: %s\n", ok ? "ok  " : "FAIL", names);
    sFails += !ok;
}

int main(void) {
    const LC_Game *g = LC_GameByCode("CPUE");
    if (g == NULL) {
        printf("FAIL Platinum is not in the table (build with -DBWPORT_PLAT)\n");
        return 1;
    }
    printf("ok   Platinum: saveEnd 0x%x saveCopy 0x%x saveBlock0 0x%x\n", g->saveEnd, g->saveCopy, g->saveBlock0);

    memset(sSave, 0xff, sizeof(sSave));
    Check("all 0xff: a new game", "CPUE", LC_SAVE_NEW, NULL);

    memset(sSave, 0, sizeof(sSave));
    Check("zeros: no save", "CPUE", -2, "holds no Pokemon Platinum save");

    Footer(0, 0xcf2c, 0xcf2c, 0);
    Footer(0, 0x1f110, 0x121e4, 1);
    Check("both blocks in the main copy", "CPUE", LC_SAVE_GOOD, NULL);

    memset(sSave, 0, sizeof(sSave));
    Footer(0x40000, 0xcf2c, 0xcf2c, 0);
    Footer(0x40000, 0x1f110, 0x121e4, 1);
    Check("both blocks in the backup copy", "CPUE", LC_SAVE_GOOD, NULL);

    memset(sSave, 0, sizeof(sSave));
    Footer(0x40000, 0xcf2c, 0xcf2c, 0);
    Footer(0, 0x1f110, 0x121e4, 1);
    Check("the blocks newest in different copies", "CPUE", LC_SAVE_GOOD, NULL);

    memset(sSave, 0, sizeof(sSave));
    Footer(0, 0xcf2c, 0xcf2c, 0);
    Check("the boxes' footer missing", "CPUE", -2, NULL);

    memset(sSave, 0, sizeof(sSave));
    Footer(0, 0xcf2c, 0xcf2c, 0);
    Footer(0, 0x1f110, 0x121e4, 0);
    Check("the boxes' block numbered 0", "CPUE", -2, NULL);

    memset(sSave, 0, sizeof(sSave));
    Footer(0, 0xcf2c, 0xcf2c, 0);
    Footer(0, 0x1f110, 0x121e0, 1);
    Check("the boxes' size wrong", "CPUE", -2, NULL);

    // a Black save is refused as Platinum's, by name, and Platinum's as Black's
    memset(sSave, 0, sizeof(sSave));
    GenFiveFooter(0x23f9c, 0);
    Check("a Black save as Platinum", "CPUE", -3, "Pokemon Black save, not a Pokemon Platinum");
    Check("a Black save as Black", "IRBO", LC_SAVE_GOOD, NULL);
    memset(sSave, 0, sizeof(sSave));
    GenFiveFooter(0x23f9c, 0x24000);
    Check("a Black save's backup copy as Black", "IRBO", LC_SAVE_GOOD, NULL);

    memset(sSave, 0, sizeof(sSave));
    Footer(0, 0xcf2c, 0xcf2c, 0);
    Footer(0, 0x1f110, 0x121e4, 1);
    Check("a Platinum save as Black", "IRBO", -3, "Pokemon Platinum save, not a Pokemon Black");

    // the edition line of each game's page comes from its No-Intro name (Platinum and the HG/SS pair are USA only, not NDSi Enhanced)
    Edition("IRBO", "USA / Europe  \xc2\xb7  NDSi Enhanced");
    Edition("CPUE", "USA  \xc2\xb7  Rev 1");
    Edition("IPKE", "USA");
    Edition("IPGE", "USA");
    Looks();

    printf(sFails == 0 ? "PASS\n" : "FAIL (%d)\n", sFails);
    return sFails != 0;
}
