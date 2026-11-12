// The route pilot's story readout (port/include/port_route_story.h; docs/port-worker.md, "Routes"): the map dump's
// "story" section and the Ws: token's test. Test-only: nothing runs unless BWPORT_PILOT, BWPORT_MAPDUMP or
// BWPORT_STORYLOG is set.
//
// The snapshot is taken at the first field frame of the run (from a save: the save's flags; from NEW GAME: the first
// frame the field runs), then each field frame the flags and work values are compared with the frame before: each
// change keeps its V-blank and zone (BWPORT_STORYLOG=1 prints "story: flag 0x.. 0 -> 1 (V-blank N, zone Z) NAME" at
// once). The section, after the dump's zones:
//   story (GAME zone N; the snapshot at V-blank V in zone Z)
//   changed since the snapshot: each flag and work value now different, its value then, the last change's V-blank and
//     zone, its name when the decomp has one (port/src/route_story_names.c, tools/route_story_names.py)
//   temp flags set now (0x4000..0x403f: the zone's own, reset at each zone load)
//   scene table: the zone's scene scripts (checked every field frame: the first whose work has its value runs; DUE
//     marks the one that would run now)
//   init scripts: the zone's other init scripts by type (2 zone init, 3 new load, 4 reload ...)
//   actors: each NPC entry of the zone with its spawn flag: clear (or none) = created, set = hidden; and whether an
//     actor of that UID is on the map now (a script can add or remove one)
//   triggers: each trigger's work and value (work 0: always) and whether it is met now (LIVE)
//   bg events: a hidden item's found flag (found = gone), other conditions as numbers
// Reading it for the next story step: a DUE scene or a LIVE trigger is what runs next where the player stands or
// walks; a flag the last scene set (changed) is what the next gated actor or trigger waits on.
#include <nitro.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_route_story.h"

void (*gPortRouteStoryFill)(PortStory *s, int full);

typedef struct {
    unsigned short id;
    const char *name;
} PortStoryName;
// port/src/route_story_names.c (tools/route_story_names.py)
extern const PortStoryName gPortStoryNamesBW[], gPortStoryNamesB2W2[];
extern const int gPortStoryNamesBWCount, gPortStoryNamesB2W2Count;

#define MAXF 4096
#define MAXW 512

static int sOn = -1, sLog, sHave, sNF, sNW;
static unsigned sStartV, sStartZone;
static unsigned char sBaseF[MAXF / 8], sCurF[MAXF / 8], sCurT[PORT_STORY_NTEMP / 8];
static unsigned short sBaseW[MAXW], sCurW[MAXW];
static unsigned sWhenF[MAXF], sWhenW[MAXW];
static unsigned short sZoneF[MAXF], sZoneW[MAXW];
static PortStory sS;

static int EnvOn(const char *name) {
    const char *e = getenv(name);

    return e != NULL && *e != '\0' && *e != '0';
}

static void Names(const char *game, const PortStoryName **t, int *n) {
    if (game[0] != '\0' && game[1] == '2') {
        *t = gPortStoryNamesB2W2;
        *n = gPortStoryNamesB2W2Count;
    } else {
        *t = gPortStoryNamesBW;
        *n = gPortStoryNamesBWCount;
    }
}

const char *PortRouteStory_Name(const char *game, unsigned id) {
    const PortStoryName *t;
    int n, i;

    Names(game, &t, &n);
    for (i = 0; i < n; i++) {
        if (t[i].id == id) {
            return t[i].name;
        }
    }
    return NULL;
}

int PortRouteStory_Lookup(const char *game, const char *name) {
    const PortStoryName *t;
    int n, i;

    Names(game, &t, &n);
    for (i = 0; i < n; i++) {
        if (strcmp(t[i].name, name) == 0) {
            return t[i].id;
        }
    }
    return -1;
}

static const char *Nm(const char *game, unsigned id) {
    const char *s = PortRouteStory_Name(game, id);

    return s != NULL ? s : "";
}

static int FlagNow(const PortStory *s, unsigned flag) {
    if (flag == 0) {
        return 0;
    }
    if (flag >= PORT_STORY_TEMP0) {
        flag -= PORT_STORY_TEMP0;
        return flag < PORT_STORY_NTEMP && (s->temp[flag / 8] >> (flag % 8) & 1);
    }
    return (int)flag < s->nflags && (s->flags[flag / 8] >> (flag % 8) & 1);
}

static int WorkNow(const PortStory *s, unsigned work) {
    if (work < PORT_STORY_WORK0 || (int)(work - PORT_STORY_WORK0) >= s->nworks) {
        return -1;
    }
    return s->works[work - PORT_STORY_WORK0];
}

void PortRouteStory_Frame(const char *game) {
    unsigned now;
    int i, b;

    if (sOn < 0) {
        sLog = EnvOn("BWPORT_STORYLOG");
        sOn = sLog || getenv("BWPORT_PILOT") != NULL || EnvOn("BWPORT_MAPDUMP");
    }
    if (!sOn || gPortRouteStoryFill == NULL) {
        return;
    }
    gPortRouteStoryFill(&sS, 0);
    if (!sS.ok) {
        return;
    }
    now = (unsigned)OS_GetVBlankCount();
    if (!sHave) {
        sNF = sS.nflags < MAXF ? sS.nflags : MAXF;
        sNW = sS.nworks < MAXW ? sS.nworks : MAXW;
        memcpy(sBaseF, sS.flags, sNF / 8);
        memcpy(sCurF, sS.flags, sNF / 8);
        memcpy(sBaseW, sS.works, sNW * sizeof(unsigned short));
        memcpy(sCurW, sS.works, sNW * sizeof(unsigned short));
        memcpy(sCurT, sS.temp, sizeof sCurT);
        sStartV = now;
        sStartZone = sS.zone;
        sHave = 1;
        if (sLog) {
            fprintf(stderr, "story: %s snapshot at V-blank %u in zone %u (%d flags, %d work values)\n", game, now, sS.zone,
                    sNF, sNW);
        }
        return;
    }
    if (memcmp(sCurF, sS.flags, sNF / 8) != 0) {
        for (i = 0; i < sNF / 8; i++) {
            unsigned char d = sCurF[i] ^ sS.flags[i];

            for (b = 0; d != 0 && b < 8; b++) {
                if (d >> b & 1) {
                    sWhenF[i * 8 + b] = now;
                    sZoneF[i * 8 + b] = (unsigned short)sS.zone;
                    if (sLog) {
                        fprintf(stderr, "story: flag 0x%04x %d -> %d (V-blank %u, zone %u) %s\n", i * 8 + b,
                                sCurF[i] >> b & 1, sS.flags[i] >> b & 1, now, sS.zone, Nm(game, i * 8 + b));
                    }
                }
            }
            sCurF[i] = sS.flags[i];
        }
    }
    if (memcmp(sCurW, sS.works, sNW * sizeof(unsigned short)) != 0) {
        for (i = 0; i < sNW; i++) {
            if (sCurW[i] != sS.works[i]) {
                sWhenW[i] = now;
                sZoneW[i] = (unsigned short)sS.zone;
                if (sLog) {
                    fprintf(stderr, "story: work 0x%04x %u -> %u (V-blank %u, zone %u) %s\n", PORT_STORY_WORK0 + i,
                            sCurW[i], sS.works[i], now, sS.zone, Nm(game, PORT_STORY_WORK0 + i));
                }
                sCurW[i] = sS.works[i];
            }
        }
    }
    if (sLog && memcmp(sCurT, sS.temp, sizeof sCurT) != 0) {
        for (i = 0; i < PORT_STORY_NTEMP; i++) {
            if ((sCurT[i / 8] ^ sS.temp[i / 8]) >> (i % 8) & 1) {
                fprintf(stderr, "story: temp flag 0x%04x %d -> %d (V-blank %u, zone %u)\n", PORT_STORY_TEMP0 + i,
                        sCurT[i / 8] >> (i % 8) & 1, sS.temp[i / 8] >> (i % 8) & 1, now, sS.zone);
            }
        }
    }
    memcpy(sCurT, sS.temp, sizeof sCurT);
}

static void FlagState(FILE *f, const char *game, const PortStory *s, unsigned flag) {
    if (flag == 0) {
        fprintf(f, "no flag: always");
        return;
    }
    fprintf(f, "flag 0x%04x%s%s %s", flag, *Nm(game, flag) ? " " : "", Nm(game, flag), FlagNow(s, flag) ? "set" : "clear");
}

void PortRouteStory_Print(FILE *f, const char *game) {
    PortStory *s = &sS;
    int i, j, n;

    if (gPortRouteStoryFill == NULL) {
        return;
    }
    gPortRouteStoryFill(s, 1);
    if (!s->ok) {
        fprintf(f, "story: no game data\n");
        return;
    }
    if (sHave) {
        fprintf(f, "story (%s zone %u; the snapshot at V-blank %u in zone %u)\n", game, s->zone, sStartV, sStartZone);
    } else {
        fprintf(f, "story (%s zone %u; no snapshot: nothing changed is listed)\n", game, s->zone);
    }
    n = 0;
    for (i = 0; sHave && i < sNF; i++) {
        n += (s->flags[i / 8] ^ sBaseF[i / 8]) >> (i % 8) & 1;
    }
    for (i = 0; sHave && i < sNW; i++) {
        n += s->works[i] != sBaseW[i];
    }
    fprintf(f, "changed since the snapshot (%d):\n", n);
    for (i = 0; sHave && i < sNF; i++) {
        if ((s->flags[i / 8] ^ sBaseF[i / 8]) >> (i % 8) & 1) {
            fprintf(f, "  flag 0x%04x = %d (was %d; V-blank %u, zone %u) %s\n", i, s->flags[i / 8] >> (i % 8) & 1,
                    sBaseF[i / 8] >> (i % 8) & 1, sWhenF[i], sZoneF[i], Nm(game, i));
        }
    }
    for (i = 0; sHave && i < sNW; i++) {
        if (s->works[i] != sBaseW[i]) {
            fprintf(f, "  work 0x%04x = %u (was %u; V-blank %u, zone %u) %s\n", PORT_STORY_WORK0 + i, s->works[i],
                    sBaseW[i], sWhenW[i], sZoneW[i], Nm(game, PORT_STORY_WORK0 + i));
        }
    }
    fprintf(f, "temp flags set now:");
    for (i = j = 0; i < PORT_STORY_NTEMP; i++) {
        if (s->temp[i / 8] >> (i % 8) & 1) {
            fprintf(f, " 0x%04x", PORT_STORY_TEMP0 + i);
            j++;
        }
    }
    fprintf(f, j ? "\n" : " none\n");
    fprintf(f, "scene table (%d; each field frame the first whose work has its value runs):\n", s->nscene);
    for (i = 0, j = 0; i < s->nscene; i++) {
        int v = WorkNow(s, s->scene[i].work), due = !j && v == (int)s->scene[i].value;

        fprintf(f, "  work 0x%04x%s%s == %u -> script %u: now %d%s\n", s->scene[i].work, *Nm(game, s->scene[i].work) ? " " : "",
                Nm(game, s->scene[i].work), s->scene[i].value, s->scene[i].script, v, due ? " DUE" : "");
        j |= due;
    }
    fprintf(f, "init scripts (%d):", s->ninit);
    for (i = 0; i < s->ninit; i++) {
        fprintf(f, " type %u script %u%s", s->init[i].type, s->init[i].script, i + 1 < s->ninit ? ";" : "");
    }
    fprintf(f, "\n");
    fprintf(f, "actors (%d NPC entries; created while the spawn flag is clear):\n", s->nnpc);
    for (i = 0; i < s->nnpc; i++) {
        int live = 0;

        for (j = 0; j < s->nlive; j++) {
            live |= s->live[j] == s->npc[i].uid;
        }
        if (s->npc[i].rail) {
            fprintf(f, "  uid %u (rail) script %u ev %u ", s->npc[i].uid, s->npc[i].script, s->npc[i].ev);
        } else {
            fprintf(f, "  uid %u at %d,%d script %u ev %u ", s->npc[i].uid, s->npc[i].x, s->npc[i].z, s->npc[i].script,
                    s->npc[i].ev);
        }
        FlagState(f, game, s, s->npc[i].flag);
        fprintf(f, ": %s; %s\n", FlagNow(s, s->npc[i].flag) ? "hidden" : "shown", live ? "on the map" : "not on the map");
    }
    fprintf(f, "triggers (%d; run when the work has the value, work 0 always):\n", s->ntrig);
    for (i = 0; i < s->ntrig; i++) {
        int v = WorkNow(s, s->trig[i].work);

        if (s->trig[i].rail) {
            fprintf(f, "  T%d (rail) script %u ", i, s->trig[i].script);
        } else {
            fprintf(f, "  T%d %d,%d %dx%d script %u ", i, s->trig[i].x, s->trig[i].z, s->trig[i].w, s->trig[i].h,
                    s->trig[i].script);
        }
        if (s->trig[i].work == 0) {
            fprintf(f, "no work: LIVE\n");
        } else {
            fprintf(f, "work 0x%04x%s%s == %u: now %d%s\n", s->trig[i].work, *Nm(game, s->trig[i].work) ? " " : "",
                    Nm(game, s->trig[i].work), s->trig[i].value, v, v == (int)s->trig[i].value ? " LIVE" : "");
        }
    }
    fprintf(f, "bg events (%d):\n", s->nbg);
    for (i = 0; i < s->nbg; i++) {
        if (s->bg[i].rail) {
            fprintf(f, "  b%d (rail) script %u ", i, s->bg[i].script);
        } else {
            fprintf(f, "  b%d %d,%d script %u ", i, s->bg[i].x, s->bg[i].z, s->bg[i].script);
        }
        if (s->bg[i].cond == 2) {
            fprintf(f, "hidden item, ");
            FlagState(f, game, s, s->bg[i].flag);
            fprintf(f, ": %s\n", FlagNow(s, s->bg[i].flag) ? "found" : "there");
        } else {
            fprintf(f, "condition %u\n", s->bg[i].cond);
        }
    }
}

int PortRouteStory_Cond(const char *game, const char *spec) {
    static char sBad[64];
    char name[64];
    const char *p = spec, *e;
    int neg = 0, id, n;

    if (gPortRouteStoryFill == NULL) {
        return 0;
    }
    gPortRouteStoryFill(&sS, 0);
    if (!sS.ok) {
        return 0;
    }
    if (*p == '!') {
        neg = 1;
        p++;
    }
    for (n = 0; p[n] != '\0' && p[n] != ' ' && p[n] != '=' && n < (int)sizeof name - 1; n++) {
        name[n] = p[n];
    }
    name[n] = '\0';
    e = p + n;
    if (isdigit((unsigned char)name[0])) {
        id = (int)strtoul(name, NULL, 0);
    } else if ((id = PortRouteStory_Lookup(game, name)) < 0) {
        if (strcmp(sBad, name) != 0) {
            fprintf(stderr, "pilot: Ws: no flag or work value named %s (%s)\n", name, game);
            snprintf(sBad, sizeof sBad, "%s", name);
        }
        return 0;
    }
    if (*e == '=') {
        return WorkNow(&sS, (unsigned)id) == (int)strtoul(e + 1, NULL, 0);
    }
    return FlagNow(&sS, (unsigned)id) != neg;
}
