// The route pilot's map (docs/port-worker.md, "Routes"; port/include/port_route_map.h): the dump (BWPORT_MAPDUMP, the
// pilot's Md token, a stuck walk), the G tokens' parser and their planner (A* on the game's own walkability, port/src/
// route_test.c walks the plan). Game-neutral: the game is the registered PortRouteAdapter's fill and zoneAt. Test-only:
// nothing runs unless the pilot or BWPORT_MAPDUMP asks.
//
// The dump, a text file in the run's out/ dir (BWPORT_MAPDUMP=DIR, or 1: BWPORT_SHOTS's dir, else the working dir),
// mapdump-NN-WHY.txt: the 64 x 64 tiles around the player, a row a line with its z first and the x every 8 columns on top:
//   .  walk          #  blocked       ~  water (surf)    ,  grass         ^ v < >  a ledge, jumped that way
//   ?  not loaded (a chunk the mapper keeps no terrain of yet: the planner tries it)   (space)  off the map
//   @  the player    N  an actor (NPC, item ball, ...)    D  a warp        T  a trigger (coordinate event)   b  a BG event
// then the lists: warps (x,z w x h -> zone, warp, type, dir), triggers (x,z w x h, script, its work and value), BG
// events (x,z script), actors (uid at x,z, from x,z, object code, move code), and the map matrix's zones around (a 32 x 32
// chunk each).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_route.h"
#include "port_route_map.h"
#include "port_route_plan.h"
#include "port_route_story.h"

static const PortRouteAdapter *sAdapter;   // the game's (port_route_map.h)
static PortMap sMap;

void PortRouteMap_SetAdapter(const PortRouteAdapter *a) {
    sAdapter = a;
}

const PortRouteAdapter *PortRouteMap_Adapter(void) {
    return sAdapter;
}

static int ZoneAt(int x, int z) {
    return sAdapter != NULL && sAdapter->zoneAt != NULL ? sAdapter->zoneAt(x, z) : -1;
}

const PortMap *PortRouteMap_Get(void) {
    if (sAdapter == NULL || sAdapter->fill == NULL) {
        sMap.ok = 0;
        return &sMap;
    }
    sMap.pdir = -1;
    sAdapter->fill(&sMap);
    return &sMap;
}

static char Overlay(const PortMap *m, int gx, int gz, int *npcId) {
    int i;

    if (gx == m->px && gz == m->pz) {
        return '@';
    }
    for (i = 0; i < m->nnpc; i++) {
        if (m->npc[i].x == gx && m->npc[i].z == gz) {
            *npcId = m->npc[i].id;
            return 'N';
        }
    }
    for (i = 0; i < m->nwarp; i++) {
        const PortMapWarp *w = &m->warp[i];

        if (gx >= w->x && gx < w->x + (w->w ? w->w : 1) && gz >= w->z && gz < w->z + (w->h ? w->h : 1)) {
            return 'D';
        }
    }
    for (i = 0; i < m->ntrig; i++) {
        const PortMapTrig *t = &m->trig[i];

        if (gx >= t->x && gx < t->x + t->w && gz >= t->z && gz < t->z + t->h) {
            return 'T';
        }
    }
    for (i = 0; i < m->nbg; i++) {
        if (m->bg[i].x == gx && m->bg[i].z == gz) {
            return 'b';
        }
    }
    return 0;
}

static const char *WarpType(int t) {
    static const char *names[] = {"none", "mat", "walk-in-2", "door", "walk-in", "warp-pad", "gate"};

    return t >= 0 && t < 7 ? names[t] : "camera";
}

static void DumpDir(char *dir, size_t n) {
    const char *e = getenv("BWPORT_MAPDUMP");

    if (e != NULL && strchr(e, '/') != NULL) {
        snprintf(dir, n, "%s", e);
        return;
    }
    e = getenv("BWPORT_SHOTS");
    if (e != NULL && *e != '\0') {
        size_t i;

        snprintf(dir, n, "%s", e);
        for (i = 0; dir[i] != '\0'; i++) {
            if (dir[i] == ',') {
                dir[i] = '\0';
                break;
            }
        }
        return;
    }
    snprintf(dir, n, ".");
}

int PortRouteMap_Dump(const char *game, const char *why, unsigned vblank) {
    static int sCount;
    const PortMap *m = PortRouteMap_Get();
    char dir[512], path[700];
    FILE *f;
    int x, z, i, x0, z0;

    if (!m->ok) {
        fprintf(stderr, "pilot: map dump (%s): no field\n", why);
        return 0;
    }
    DumpDir(dir, sizeof dir);
    snprintf(path, sizeof path, "%s/mapdump-%02d-%s.txt", dir, ++sCount, why);
    f = fopen(path, "w");
    if (f == NULL) {
        fprintf(stderr, "pilot: map dump: cannot write %s\n", path);
        return 0;
    }
    x0 = m->px - 32;
    z0 = m->pz - 32;
    fprintf(f, "map: %s zone %u player %d,%d y %d facing %d (V-blank %u, %s) matrix %dx%d chunks; tiles %d..%d x %d..%d (%s)\n",
            game, m->zone, m->px, m->pz, m->py, m->pdir, vblank, why, m->mw, m->mh, x0, x0 + 63, z0, z0 + 63,
            sAdapter->engine);
    fprintf(f, "      ");
    for (x = 0; x < 64; x += 8) {
        fprintf(f, "%-8d", x0 + x);
    }
    fprintf(f, "\n");
    for (z = 0; z < 64; z++) {
        char row[65];

        for (x = 0; x < 64; x++) {
            int cx = x0 + x - m->x0, cz = z0 + z - m->z0, id = -1;
            char o = Overlay(m, x0 + x, z0 + z, &id);

            row[x] = o ? o : m->cell[cz][cx].kind;
        }
        row[64] = '\0';
        fprintf(f, "%5d %s\n", z0 + z, row);
    }
    fprintf(f, "warps (%d):\n", m->nwarp);
    for (i = 0; i < m->nwarp; i++) {
        const PortMapWarp *w = &m->warp[i];

        fprintf(f, "  D%d %d,%d %dx%d -> zone %u warp %u (%s, arrive facing %d)\n", i, w->x, w->z, w->w, w->h, w->zone,
                w->dest, WarpType(w->type), w->dir - 1);
    }
    fprintf(f, "triggers (%d):\n", m->ntrig);
    for (i = 0; i < m->ntrig; i++) {
        const PortMapTrig *t = &m->trig[i];

        fprintf(f, "  T%d %d,%d %dx%d script %u (work 0x%x = %u, type %u)\n", i, t->x, t->z, t->w, t->h, t->script,
                t->workId, t->workValue, t->type);
    }
    fprintf(f, "bg events (%d):\n", m->nbg);
    for (i = 0; i < m->nbg; i++) {
        fprintf(f, "  b%d %d,%d script %u dir %u\n", i, m->bg[i].x, m->bg[i].z, m->bg[i].script, m->bg[i].dir);
    }
    fprintf(f, "actors (%d):\n", m->nnpc);
    for (i = 0; i < m->nnpc; i++) {
        const PortMapNpc *n = &m->npc[i];

        fprintf(f, "  N uid %d at %d,%d (from %d,%d) code %u move %u\n", n->id, n->x, n->z, n->x0, n->z0, n->code, n->move);
    }
    if (sAdapter->zoneAt != NULL) {
        int cx = m->px / 32, cz = m->pz / 32;

        fprintf(f, "zones (the matrix's chunks of 32x32 tiles; the player's in []):\n");
        for (z = cz - 3; z <= cz + 3; z++) {
            if (z < 0 || z >= m->mh) {
                continue;
            }
            fprintf(f, "  z %4d:", z * 32);
            for (x = cx - 3; x <= cx + 3; x++) {
                int zone = x < 0 || x >= m->mw ? -1 : ZoneAt(x * 32, z * 32);

                if (x == cx && z == cz) {
                    fprintf(f, " [%5d]", zone);
                } else if (zone < 0 || zone == 0xffff) {
                    fprintf(f, "       -");
                } else {
                    fprintf(f, "  %5d ", zone);
                }
            }
            fprintf(f, "\n");
        }
        fprintf(f, "  (columns from x %d)\n", (cx - 3) * 32);
    }
    PortRouteStory_Print(f, game);   // the story section (port/src/route_story.c)
    fclose(f);
    fprintf(stderr, "pilot: map dump %s\n", path);
    return 1;
}

// ---- the G tokens ----------------------------------------------------------------------------------------------------

// a decimal number at *p (a sign allowed), moved past it: 1, or 0 when there is none
static int Num(const char **p, const char *end, int *v) {
    const char *s = *p;
    int neg = 0, n = 0;

    if (s < end && *s == '-') {
        neg = 1;
        s++;
    }
    if (s >= end || *s < '0' || *s > '9') {
        return 0;
    }
    while (s < end && *s >= '0' && *s <= '9') {
        n = n * 10 + (*s++ - '0');
    }
    *v = neg ? -n : n;
    *p = s;
    return 1;
}

int PortRoute_ParseGoal(const char *tok, PortRouteGoal *g, char *why, int whyN) {
    static const struct {
        const char *word;
        int kind, talk;
    } kWords[] = {{"warp:", GOAL_WARP, 0}, {"npc:", GOAL_NPC, 1}, {"item:", GOAL_ITEM, 1}, {"trig:", GOAL_TRIG, 0},
                  {"bg:", GOAL_BG, 1}};
    const char *end = tok, *p;
    size_t i;

    while (*end != '\0' && *end != ' ') {
        end++;
    }
    memset(g, 0, sizeof *g);
    if (end - tok < 3 || tok[0] != 'G' || tok[1] != ':') {
        snprintf(why, (size_t)whyN, "not a G token");
        return 0;
    }
    p = tok + 2;
    for (i = 0; i < sizeof kWords / sizeof kWords[0]; i++) {
        size_t n = strlen(kWords[i].word);

        if ((size_t)(end - p) >= n && strncmp(p, kWords[i].word, n) == 0) {
            p += n;
            g->kind = kWords[i].kind;
            g->talk = kWords[i].talk;
            if (!Num(&p, end, &g->n) || g->n < 0) {
                snprintf(why, (size_t)whyN, "%.*s: a number after %s", (int)(end - tok), tok, kWords[i].word);
                return 0;
            }
            if (p < end && g->talk && (size_t)(end - p) == 5 && strncmp(p, ":nota", 5) == 0) {
                g->talk = 0;
                p = end;
            }
            if (p != end) {
                snprintf(why, (size_t)whyN, "%.*s: '%.*s' after the number%s", (int)(end - tok), tok, (int)(end - p), p,
                         kWords[i].talk ? " (only :nota may follow)" : "");
                return 0;
            }
            return 1;
        }
    }
    g->kind = GOAL_TILE;
    if (!Num(&p, end, &g->x) || p >= end || *p != ',' || (p++, !Num(&p, end, &g->z)) || p != end) {
        snprintf(why, (size_t)whyN, "%.*s: want G:X,Z, G:warp:N, G:npc:N[:nota], G:item:N[:nota], G:trig:N or G:bg:N[:nota]",
                 (int)(end - tok), tok);
        return 0;
    }
    return 1;
}

// ---- the planner -----------------------------------------------------------------------------------------------------

static const int kDx[4] = {0, 0, -1, 1}, kDz[4] = {-1, 1, 0, 0};

static int Walkable(char k) {
    return k == '.' || k == ',' || k == '?';
}

static int LedgeDir(char k) {
    return k == '^' ? 0 : k == 'v' ? 1 : k == '<' ? 2 : k == '>' ? 3 : -1;
}

#define N PORT_MAP_N
static int sG[N * N], sF[N * N], sFrom[N * N];
static unsigned char sClosed[N * N], sBlock[N * N], sGoal[N * N];
static int sHeap[N * N * 4], sHeapN;

static void HeapPush(int c) {
    int i = sHeapN++;

    while (i > 0 && sF[sHeap[(i - 1) / 2]] > sF[c]) {
        sHeap[i] = sHeap[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    sHeap[i] = c;
}

static int HeapPop(void) {
    int top = sHeap[0], last = sHeap[--sHeapN], i = 0;

    for (;;) {
        int l = 2 * i + 1, r = l + 1, m = i;

        if (l < sHeapN && sF[sHeap[l]] < sF[last]) {
            m = l;
        }
        if (r < sHeapN && sF[sHeap[r]] < (m == i ? sF[last] : sF[sHeap[l]])) {
            m = r;
        }
        if (m == i) {
            break;
        }
        sHeap[i] = sHeap[m];
        i = m;
    }
    sHeap[i] = last;
    return top;
}

// tiles the pilot found blocked (an actor stood there, a step that did not happen): kept until the zone changes
static int sTmpX[64], sTmpZ[64], sTmpN;
static unsigned sTmpZone;

void PortRoutePlan_Block(unsigned zone, int x, int z) {
    if (zone != sTmpZone) {
        sTmpN = 0;
        sTmpZone = zone;
    }
    sTmpX[sTmpN % 64] = x;
    sTmpZ[sTmpN % 64] = z;
    sTmpN++;
}

void PortRoutePlan_Reset(void) {
    sTmpN = 0;
}

// the goal tiles that face grid tile tx,tz (an actor, a BG event): next to it, or across a counter (the player talks over
// it); sGoal 3 + the direction that faces it. The number of goal tiles
static int FacingGoals(const PortMap *m, int tx, int tz) {
    int d, k, n = 0;

    for (d = 0; d < 4; d++) {
        for (k = 1; k <= 2; k++) {   // k 2: across the counter between
            int x = tx - k * kDx[d] - m->x0, z = tz - k * kDz[d] - m->z0;

            if (x < 0 || z < 0 || x >= PORT_MAP_N || z >= PORT_MAP_N) {
                break;
            }
            if (k == 1 && (m->cell[z][x].flags & PORT_MAP_COUNTER)) {
                continue;
            }
            if (Walkable(m->cell[z][x].kind) && m->cell[z][x].kind != '?' && !sGoal[z * PORT_MAP_N + x]) {
                sGoal[z * PORT_MAP_N + x] = (unsigned char)(3 + d);
                n++;
            }
            break;
        }
    }
    return n;
}

static const char *GoalName(const PortRouteGoal *g, char *buf, size_t n) {
    static const char *const kNames[] = {"", "warp", "npc", "item", "trig", "bg"};

    if (g->kind == GOAL_TILE) {
        snprintf(buf, n, "%d,%d", g->x, g->z);
    } else {
        snprintf(buf, n, "%s:%d", kNames[g->kind], g->n);
    }
    return buf;
}

// a G token's goal: the next waypoint toward it. Returns PLAN_OK with the waypoint (the end of the path's first straight
// run), PLAN_THERE (on a goal tile; *press: the direction to press into a warp or that faces the target, or -1), or
// PLAN_NONE with why
int PortRoutePlan_Next(const PortRouteState *st, const PortRouteGoal *goal, PortRoutePlan *out) {
    const PortMap *m = PortRouteMap_Get();
    int tx = goal->kind == GOAL_TILE ? goal->x : 0, tz = goal->kind == GOAL_TILE ? goal->z : 0;
    int warpZone = goal->kind == GOAL_WARP ? goal->n : -1;
    int i, x, z, start, best = -1, bestH = 1 << 30, goals = 0, farX = tx, farZ = tz, far = goal->kind == GOAL_TILE,
                     goalWarps = 0;
    char gname[32];

    memset(out, 0, sizeof *out);
    out->press = -1;
    if (!m->ok) {
        snprintf(out->why, sizeof out->why, "no field");
        return PLAN_NONE;
    }
    GoalName(goal, gname, sizeof gname);
    if (st->zone != sTmpZone) {
        sTmpN = 0;
        sTmpZone = st->zone;
    }
    memset(sBlock, 0, sizeof sBlock);
    memset(sGoal, 0, sizeof sGoal);
    for (i = 0; i < m->nnpc; i++) {   // actors are in the way, where they stand and where they step from
        int ax[2] = {m->npc[i].x, m->npc[i].x0}, az[2] = {m->npc[i].z, m->npc[i].z0}, j;

        for (j = 0; j < 2; j++) {
            x = ax[j] - m->x0;
            z = az[j] - m->z0;
            if (x >= 0 && z >= 0 && x < N && z < N) {
                sBlock[z * N + x] = 1;
            }
        }
    }
    for (i = 0; i < sTmpN && i < 64; i++) {
        x = sTmpX[i] - m->x0;
        z = sTmpZ[i] - m->z0;
        if (x >= 0 && z >= 0 && x < N && z < N) {
            sBlock[z * N + x] = 1;
        }
    }
    // the goal tiles
    if (goal->kind == GOAL_NPC || goal->kind == GOAL_ITEM) {   // next to the actor, facing it
        char ids[160];
        size_t n = 0;

        ids[0] = '\0';
        for (i = 0; i < m->nnpc && m->npc[i].id != goal->n; i++) {
            if (n + 8 < sizeof ids) {
                n += (size_t)snprintf(ids + n, sizeof ids - n, " %d", m->npc[i].id);
            }
        }
        if (i == m->nnpc) {
            snprintf(out->why, sizeof out->why, "%s: no actor of uid %d in zone %u (the actors' uids:%s)", gname, goal->n,
                     m->zone, n ? ids : " none");
            return PLAN_NONE;
        }
        goals = FacingGoals(m, m->npc[i].x, m->npc[i].z);
    } else if (goal->kind == GOAL_BG) {   // next to the BG event, facing it
        if (goal->n >= m->nbg) {
            snprintf(out->why, sizeof out->why, "%s: zone %u has %d BG events (b0..b%d)", gname, m->zone, m->nbg, m->nbg - 1);
            return PLAN_NONE;
        }
        goals = FacingGoals(m, m->bg[goal->n].x, m->bg[goal->n].z);
    } else if (goal->kind == GOAL_TRIG) {   // onto the trigger
        const PortMapTrig *t;
        int gx, gz;

        if (goal->n >= m->ntrig) {
            snprintf(out->why, sizeof out->why, "%s: zone %u has %d triggers (T0..T%d)", gname, m->zone, m->ntrig, m->ntrig - 1);
            return PLAN_NONE;
        }
        t = &m->trig[goal->n];
        // its free tiles first (no actor on it, no step that failed there); only when there are none all of them
        for (int pass = 0; pass < 2 && goals == 0; pass++) {
            for (gz = t->z; gz < t->z + (t->h ? t->h : 1); gz++) {
                for (gx = t->x; gx < t->x + (t->w ? t->w : 1); gx++) {
                    x = gx - m->x0;
                    z = gz - m->z0;
                    if (x >= 0 && z >= 0 && x < N && z < N && Walkable(m->cell[z][x].kind) &&
                        (pass == 1 || !sBlock[z * N + x])) {
                        sGoal[z * N + x] = 1;
                        sBlock[z * N + x] = 0;
                        goals++;
                    }
                }
            }
        }
    } else if (warpZone < 0) {
        x = tx - m->x0;
        z = tz - m->z0;
        if (x >= 0 && z >= 0 && x < N && z < N) {
            sGoal[z * N + x] = 1;
            sBlock[z * N + x] = 0;
            goals++;
        }
    } else {
        for (i = 0; i < m->nwarp; i++) {   // a warp to the zone: stand on it, or next to it and press into it
            const PortMapWarp *w = &m->warp[i];
            int wx, wz, d;

            if ((int)w->zone != warpZone) {
                continue;
            }
            for (wz = w->z; wz < w->z + (w->h ? w->h : 1); wz++) {
                for (wx = w->x; wx < w->x + (w->w ? w->w : 1); wx++) {
                    x = wx - m->x0;
                    z = wz - m->z0;
                    if (x < 0 || z < 0 || x >= N || z >= N) {
                        continue;
                    }
                    if (Walkable(m->cell[z][x].kind)) {
                        sGoal[z * N + x] = 2;   // on it
                        goals++;
                    }
                    for (d = 0; d < 4; d++) {
                        int nx = x - kDx[d], nz = z - kDz[d];   // the tile from which a step in d enters the warp

                        if (nx >= 0 && nz >= 0 && nx < N && nz < N && Walkable(m->cell[nz][nx].kind) &&
                            !sGoal[nz * N + nx]) {
                            sGoal[nz * N + nx] = (unsigned char)(3 + d);
                            goals++;
                        }
                    }
                }
            }
        }
        goalWarps = goals > 0;
        if (goals == 0) {   // no warp: the zone's own tiles where the map goes on into it (a town's edge to a route)
            for (z = 0; z < N; z++) {
                for (x = 0; x < N; x++) {
                    if (m->cell[z][x].zone == (unsigned)warpZone && Walkable(m->cell[z][x].kind) && m->cell[z][x].kind != '?') {
                        sGoal[z * N + x] = 1;
                        goals++;
                    }
                }
            }
        }
        if (goals == 0 && sAdapter->zoneAt != NULL) {   // not in the window: the nearest chunk of it on the matrix
            int bd = 1 << 30, cx, cz;

            for (cz = 0; cz < m->mh; cz++) {
                for (cx = 0; cx < m->mw; cx++) {
                    if (ZoneAt(cx * 32, cz * 32) == warpZone) {
                        int gx = cx * 32 + 16, gz = cz * 32 + 16, dd = abs(gx - m->px) + abs(gz - m->pz);

                        if (dd < bd) {
                            bd = dd;
                            farX = gx;
                            farZ = gz;
                        }
                    }
                }
            }
            if (bd == 1 << 30) {
                snprintf(out->why, sizeof out->why, "no warp to zone %d here and no chunk of it on the map matrix", warpZone);
                return PLAN_NONE;
            }
            far = 1;
        }
    }
    x = m->px - m->x0;
    z = m->pz - m->z0;
    start = z * N + x;
    if (sGoal[start]) {
        out->x = m->px;
        out->z = m->pz;
        out->press = sGoal[start] >= 3 ? sGoal[start] - 3 : -1;
        if (sGoal[start] == 2) {   // on a warp tile: walk on the other way it was arrived facing (or try)
            for (i = 0; i < m->nwarp; i++) {
                const PortMapWarp *w = &m->warp[i];

                if (m->px >= w->x && m->px < w->x + (w->w ? w->w : 1) && m->pz >= w->z && m->pz < w->z + (w->h ? w->h : 1)) {
                    out->press = w->dir > 0 ? (w->dir - 1) ^ 1 : 1;
                }
            }
        }
        return PLAN_THERE;
    }
    // A*: steps cost 2, '?' 6, a ledge jump 4; h = 2 x the Manhattan distance to the target (the far one or the nearest goal
    // isn't known: with goals in the window, h = 0, Dijkstra)
    for (i = 0; i < N * N; i++) {
        sG[i] = 1 << 30;
        sClosed[i] = 0;
    }
    sHeapN = 0;
    sG[start] = 0;
    sF[start] = 0;
    sFrom[start] = -1;
    HeapPush(start);
    while (sHeapN > 0) {
        int c = HeapPop(), cx = c % N, cz = c / N, d;
        char ck = m->cell[cz][cx].kind;

        if (sClosed[c]) {
            continue;
        }
        sClosed[c] = 1;
        if (sGoal[c]) {
            best = c;
            break;
        }
        if (far && goals == 0) {
            int h = abs(m->x0 + cx - farX) + abs(m->z0 + cz - farZ);

            if (h < bestH || (h == bestH && best >= 0 && sG[c] < sG[best])) {
                bestH = h;
                best = c;
            }
        }
        for (d = 0; d < 4; d++) {
            int nx = cx + kDx[d], nz = cz + kDz[d], n, cost;
            char nk;

            if (nx < 0 || nz < 0 || nx >= N || nz >= N) {
                continue;
            }
            nk = m->cell[nz][nx].kind;
            if (LedgeDir(nk) == d && ck != '?') {   // a ledge the step jumps: lands a tile beyond
                nx += kDx[d];
                nz += kDz[d];
                if (nx < 0 || nz < 0 || nx >= N || nz >= N || !Walkable(m->cell[nz][nx].kind)) {
                    continue;
                }
                cost = 4;
            } else {
                if (!(m->cell[cz][cx].pass & (1 << d)) || !Walkable(nk)) {
                    continue;
                }
                cost = nk == '?' || ck == '?' ? 6 : 2;
            }
            n = nz * N + nx;
            if (sBlock[n] || sClosed[n] || sG[c] + cost >= sG[n]) {
                continue;
            }
            sG[n] = sG[c] + cost;
            sFrom[n] = c;
            sF[n] = sG[n] + (far && goals == 0 ? 2 * (abs(m->x0 + nx - farX) + abs(m->z0 + nz - farZ)) : 0);
            if (sHeapN < N * N * 4) {
                HeapPush(n);
            }
        }
    }
    if (best < 0 || best == start) {
        char reach[160];
        size_t rn = 0;

        // the warps the search did reach (on one, or next to it): the way on is often through one of them (Accumula's west
        // edge to Route 2 is a wall; the way is the walk-in warp to the gate, zone 320, and the gate's to Route 2)
        reach[0] = '\0';
        for (i = 0; i < m->nwarp && rn + 24 < sizeof reach; i++) {
            const PortMapWarp *w = &m->warp[i];
            int wx, wz, d, hit = 0;

            for (wz = w->z; wz < w->z + (w->h ? w->h : 1) && !hit; wz++) {
                for (wx = w->x; wx < w->x + (w->w ? w->w : 1) && !hit; wx++) {
                    for (d = -1; d < 4 && !hit; d++) {
                        x = wx - m->x0 + (d < 0 ? 0 : kDx[d]);
                        z = wz - m->z0 + (d < 0 ? 0 : kDz[d]);
                        hit = x >= 0 && z >= 0 && x < N && z < N && sClosed[z * N + x];
                    }
                }
            }
            if (hit) {
                rn += (size_t)snprintf(reach + rn, sizeof reach - rn, " zone %u (%d,%d)", w->zone, w->x, w->z);
            }
        }
        if (far && goals == 0) {
            snprintf(out->why, sizeof out->why, "no path from %d,%d toward %d,%d (no tile nearer reachable)", m->px, m->pz,
                     farX, farZ);
        } else {
            snprintf(out->why, sizeof out->why, "no path from %d,%d to %s (%s: %d goal tiles, none reachable)", m->px,
                     m->pz, gname,
                     goal->kind == GOAL_TILE   ? "the target"
                     : goal->kind != GOAL_WARP ? (goal->kind == GOAL_TRIG ? "its tiles" : "the tiles facing it")
                     : goalWarps               ? "the zone's warps"
                                               : "the zone's own tiles",
                     goals);
        }
        if (rn > 0) {
            rn = strlen(out->why);
            snprintf(out->why + rn, sizeof out->why - rn, "; warps reached lead to:%s", reach);
        }
        return PLAN_NONE;
    }
    // the path back from best; the waypoint is the end of its first straight run (a ledge jump stays on its line)
    {
        static int path[N * N];
        int n = 0, c, k, dx0, dz0;

        for (c = best; c != -1; c = sFrom[c]) {
            path[n++] = c;
        }
        // path[n-1] is the start
        dx0 = path[n - 2] % N - path[n - 1] % N;
        dz0 = path[n - 2] / N - path[n - 1] / N;
        dx0 = dx0 > 0 ? 1 : dx0 < 0 ? -1 : 0;
        dz0 = dz0 > 0 ? 1 : dz0 < 0 ? -1 : 0;
        k = n - 2;
        while (k > 0) {
            int ddx = path[k - 1] % N - path[k] % N, ddz = path[k - 1] / N - path[k] / N;

            ddx = ddx > 0 ? 1 : ddx < 0 ? -1 : 0;
            ddz = ddz > 0 ? 1 : ddz < 0 ? -1 : 0;
            if (ddx != dx0 || ddz != dz0) {
                break;
            }
            k--;
        }
        out->x = m->x0 + path[k] % N;
        out->z = m->z0 + path[k] / N;
        out->nextX = m->x0 + path[n - 2] % N;
        out->nextZ = m->z0 + path[n - 2] / N;
        out->steps = n - 1;
        out->final = k == 0 && !(far && goals == 0);
        out->press = out->final && sGoal[best] >= 3 ? sGoal[best] - 3 : -1;
    }
    return PLAN_OK;
}
#undef N
