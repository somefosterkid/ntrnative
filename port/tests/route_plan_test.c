// A unit test of the route pilot's planner (port/src/route_map.c, PortRoutePlan_Next) on synthetic grids, no game:
// port/tests/run_route_plan_test.sh builds it with gcc and runs it. Each case draws a map ('.' walk, '#' wall, '@' the
// player, 'v' a ledge jumped down, digits: tiles of the zone of that number, 'W' a warp's tile, 'N' an actor (uid 5),
// 'C' a counter, 'B' a BG event on a wall, 'T' a trigger's tile) and checks the plan; and the G tokens' parser.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_route_map.h"
#include "port_route_plan.h"

static const char *const *sPic;
static int sRows, sWarpZone;

static char KindAt(int x, int z) {
    if (z < 0 || z >= sRows || x < 0 || x >= (int)strlen(sPic[z])) {
        return '#';
    }
    switch (sPic[z][x]) {
    case '#':
    case 'C':
    case 'B': return '#';
    case 'v': return 'v';
    default: return '.';
    }
}

// the picture at grid 1000,2000 (so the window's offsets are tested too); pass bits as the game's filler makes them: a
// step onto a walkable tile (ledges are jumped by the planner itself)
static void Fill(PortMap *m) {
    int x, z, d, px = 0, pz = 0;
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};

    memset(m, 0, sizeof *m);
    for (z = 0; z < sRows; z++) {
        const char *p = strchr(sPic[z], '@');

        if (p != NULL) {
            px = (int)(p - sPic[z]);
            pz = z;
        }
    }
    m->px = 1000 + px;
    m->pz = 2000 + pz;
    m->x0 = m->px - PORT_MAP_N / 2;
    m->z0 = m->pz - PORT_MAP_N / 2;
    m->zone = 1;
    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            int gx = m->x0 + x - 1000, gz = m->z0 + z - 2000;
            PortMapCell *c = &m->cell[z][x];
            char k = KindAt(gx, gz), s = gz >= 0 && gz < sRows && gx >= 0 && gx < (int)strlen(sPic[gz]) ? sPic[gz][gx] : '#';

            c->kind = k;
            c->flags = s == 'C' ? PORT_MAP_COUNTER : 0;
            c->zone = s >= '2' && s <= '9' ? (unsigned short)(s - '0') : 1;
            if (k != '.') {
                continue;
            }
            for (d = 0; d < 4; d++) {
                if (KindAt(gx + dx[d], gz + dz[d]) == '.') {
                    c->pass |= 1 << d;
                }
            }
        }
    }
    for (z = 0; z < sRows; z++) {
        for (x = 0; sPic[z][x] != '\0'; x++) {
            if (sPic[z][x] == 'W' && m->nwarp < 64) {
                PortMapWarp *w = &m->warp[m->nwarp++];

                w->x = 1000 + x;
                w->z = 2000 + z;
                w->w = w->h = 1;
                w->zone = (unsigned)sWarpZone;
                w->dir = 2;   // arrives facing down (DIR + 1)
            }
            if (sPic[z][x] == 'N' && m->nnpc < 64) {
                PortMapNpc *n = &m->npc[m->nnpc++];

                n->id = 5;
                n->x = n->x0 = 1000 + x;
                n->z = n->z0 = 2000 + z;
            }
            if (sPic[z][x] == 'B' && m->nbg < 64) {
                m->bg[m->nbg].x = 1000 + x;
                m->bg[m->nbg++].z = 2000 + z;
            }
            if (sPic[z][x] == 'T' && m->ntrig < 64) {
                PortMapTrig *t = &m->trig[m->ntrig++];

                t->x = 1000 + x;
                t->z = 2000 + z;
                t->w = t->h = 1;
            }
        }
    }
    m->ok = 1;
}

// route_map.c's dump calls the story section (port/src/route_story.c, which needs the game's headers): not part of this test
void PortRouteStory_Print(FILE *f, const char *game) {
    (void)f;
    (void)game;
}

static const PortRouteAdapter kTest = {"test", Fill, NULL};
static int sFails;

static void Check(const char *name, int ok, const PortRoutePlan *p) {
    printf("%s %s (waypoint %d,%d, %d steps, press %d%s%s)\n", ok ? "ok  " : "FAIL", name, p->x - 1000, p->z - 2000, p->steps,
           p->press, p->why[0] ? ": " : "", p->why);
    sFails += !ok;
}

static int PlanGoal(const char *const *pic, int rows, const PortRouteGoal *g, int warpTo, PortRoutePlan *p) {
    PortRouteState st;

    sPic = pic;
    sRows = rows;
    sWarpZone = warpTo;
    memset(&st, 0, sizeof st);
    st.zone = 1;
    PortRoutePlan_Reset();
    return PortRoutePlan_Next(&st, g, p);
}

static int Plan(const char *const *pic, int rows, int tx, int tz, int warpZone, int warpTo, PortRoutePlan *p) {
    PortRouteGoal g;

    memset(&g, 0, sizeof g);
    g.kind = warpZone < 0 ? GOAL_TILE : GOAL_WARP;
    g.x = 1000 + tx;
    g.z = 2000 + tz;
    g.n = warpZone;
    return PlanGoal(pic, rows, &g, warpTo, p);
}

// a G token through the parser and the planner
static int PlanTok(const char *const *pic, int rows, const char *tok, PortRoutePlan *p) {
    PortRouteGoal g;
    char why[160];

    memset(p, 0, sizeof *p);
    if (!PortRoute_ParseGoal(tok, &g, why, sizeof why)) {
        snprintf(p->why, sizeof p->why, "parse: %s", why);
        return -1;
    }
    return PlanGoal(pic, rows, &g, 0, p);
}

static void CheckParse(const char *tok, int ok, int kind, int a, int b, int talk) {
    PortRouteGoal g;
    char why[160] = "";
    int r = PortRoute_ParseGoal(tok, &g, why, sizeof why), good;

    good = ok ? r && g.kind == kind && (kind == GOAL_TILE ? g.x == a && g.z == b : g.n == a) && g.talk == talk : !r;
    printf("%s parse \"%s\"%s%s\n", good ? "ok  " : "FAIL", tok, why[0] ? ": " : "", why);
    sFails += !good;
}

int main(void) {
    PortRoutePlan p;
    int r;
    static const char *const around[] = {
        "#########",
        "#@..#...#",
        "#.#.#.#.#",
        "#.#...#.#",
        "#########",
    };
    static const char *const ledge[] = {
        "#####",
        "#.@.#",
        "#vvv#",
        "#...#",
        "#####",
    };
    static const char *const walled[] = {   // zone 2 behind a wall; the warp W leads elsewhere (zone 7)
        "#########",
        "#22#....#",
        "#22#.@..#",
        "#22#W#..#",
        "#########",
    };
    static const char *const edge[] = {   // zone 2 straight on to the west
        "#########",
        "22......#",
        "22...@..#",
        "#########",
    };

    PortRouteMap_SetAdapter(&kTest);
    r = Plan(around, 5, 7, 1, -1, 0, &p);   // around the walls: right 2, down 2, right 2, up 2, right 2
    Check("G:X,Z around two walls", r == PLAN_OK && p.x == 1003 && p.z == 2001 && p.steps == 10, &p);
    r = Plan(around, 5, 1, 1, -1, 0, &p);
    Check("G:X,Z on the target", r == PLAN_THERE, &p);
    r = Plan(around, 5, 2, 2, -1, 0, &p);
    Check("G:X,Z into a wall: no path", r == PLAN_NONE, &p);
    r = Plan(ledge, 5, 2, 3, -1, 0, &p);   // down over the ledge: one jump of two tiles
    Check("G:X,Z jumps a ledge", r == PLAN_OK && p.x == 1002 && p.z == 2003 && p.steps == 1 && p.final, &p);
    {
        static const char *const below[] = {"#####", "#...#", "#vvv#", "#.@.#", "#####"};

        r = Plan(below, 5, 2, 1, -1, 0, &p);
        Check("G:X,Z up a ledge: no path", r == PLAN_NONE, &p);
    }
    r = Plan(walled, 5, 0, 0, 7, 7, &p);   // to the warp's zone: next to it, then press down into it
    Check("G:warp:N to a warp", r == PLAN_OK && p.final && p.press == 1 && p.x == 1004 && p.z == 2002, &p);
    r = Plan(walled, 5, 0, 0, 2, 7, &p);   // zone 2's tiles are walled off: no path, the reached warp named
    Check("G:warp:N walled off: names the warps reached", r == PLAN_NONE && strstr(p.why, "zone 7 (1004,2003)") != NULL,
          &p);
    r = Plan(edge, 4, 0, 0, 2, 7, &p);   // no warp to zone 2: its tiles where the map goes on
    Check("G:warp:N to a zone's edge", r == PLAN_OK && p.final && p.x == 1001 && p.z == 2002 && p.steps == 4, &p);
    // the parser
    CheckParse("G:12,-3", 1, GOAL_TILE, 12, -3, 0);
    CheckParse("G:12,3 W30", 1, GOAL_TILE, 12, 3, 0);   // the token ends at a space
    CheckParse("G:warp:427", 1, GOAL_WARP, 427, 0, 0);
    CheckParse("G:npc:3", 1, GOAL_NPC, 3, 0, 1);
    CheckParse("G:npc:3:nota", 1, GOAL_NPC, 3, 0, 0);
    CheckParse("G:item:12", 1, GOAL_ITEM, 12, 0, 1);
    CheckParse("G:item:12:nota", 1, GOAL_ITEM, 12, 0, 0);
    CheckParse("G:trig:0", 1, GOAL_TRIG, 0, 0, 0);
    CheckParse("G:bg:2", 1, GOAL_BG, 2, 0, 1);
    CheckParse("G:bg:2:nota", 1, GOAL_BG, 2, 0, 0);
    CheckParse("G:trig:1:nota", 0, 0, 0, 0, 0);   // :nota only after a talking target
    CheckParse("G:npc:", 0, 0, 0, 0, 0);
    CheckParse("G:npc:x", 0, 0, 0, 0, 0);
    CheckParse("G:npc:3:a", 0, 0, 0, 0, 0);
    CheckParse("G:foo:3", 0, 0, 0, 0, 0);
    CheckParse("G:12", 0, 0, 0, 0, 0);
    CheckParse("G:12,", 0, 0, 0, 0, 0);
    CheckParse("G:1,2x", 0, 0, 0, 0, 0);
    // the target tokens
    {
        static const char *const room[] = {   // an actor at 6,1; a BG event on the wall at 3,0; a trigger at 1,3
            "###B####",
            "#@....N#",
            "#......#",
            "#T######",
            "########",
        };
        static const char *const center[] = {   // the nurse behind the counter: talked to from 3,3, facing up
            "#######",
            "###N###",
            "#CCCCC#",
            "#.....#",
            "#..@..#",
            "#######",
        };

        r = PlanTok(room, 5, "G:npc:5", &p);   // to 5,1 (or 6,2: 5,1 is nearer), then face right
        Check("G:npc:N next to the actor", r == PLAN_OK && p.final && p.press == 3 && p.x == 1005 && p.z == 2001, &p);
        r = PlanTok(room, 5, "G:npc:9", &p);
        Check("G:npc:N no such uid: names the uids", r == PLAN_NONE && strstr(p.why, "uids: 5") != NULL, &p);
        r = PlanTok(room, 5, "G:bg:0", &p);   // under the BG event at 3,1, facing up
        Check("G:bg:N facing the BG event", r == PLAN_OK && p.final && p.press == 0 && p.x == 1003 && p.z == 2001, &p);
        r = PlanTok(room, 5, "G:bg:1", &p);
        Check("G:bg:N out of range", r == PLAN_NONE && strstr(p.why, "1 BG events") != NULL, &p);
        r = PlanTok(room, 5, "G:trig:0", &p);   // down to 1,3: the trigger's tile, nothing to press
        Check("G:trig:N onto the trigger", r == PLAN_OK && p.final && p.press == -1 && p.x == 1001 && p.z == 2003, &p);
        r = PlanTok(center, 6, "G:npc:5", &p);   // up one: 3,3 faces the nurse across the counter
        Check("G:npc:N across a counter", r == PLAN_OK && p.final && p.press == 0 && p.x == 1003 && p.z == 2003, &p);
        {
            static const char *const by[] = {"#####", "#.N.#", "#.@.#", "#####"};

            r = PlanTok(by, 4, "G:npc:5:nota", &p);   // already below it: there, press up (face it)
            Check("G:npc:N already next to it", r == PLAN_THERE && p.press == 0, &p);
        }
    }
    printf("%s\n", sFails ? "FAIL" : "PASS");
    return sFails != 0;
}
