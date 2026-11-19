// A unit test of the route pilot's planner (port/src/route_map.c, PortRoutePlan_Next) on synthetic grids, no game:
// port/tests/run_route_plan_test.sh builds it with gcc and runs it. Each case draws a map ('.' walk, '#' wall, '@' the
// player, 'v' a ledge jumped down, digits: tiles of the zone of that number, 'W' a warp's tile, 'N' an actor (uid 5),
// 'C' a counter, 'E' an electric rock (a wall the dump shows as E), 'B' a BG event on a wall, 'T' a trigger's tile, 'A' a live
// trigger the adapter wants walked round, PortMapTrig.avoid) and checks
// the plan; the rail planner on a made-up rail
// (RailFill: two lines, RailStep: the game's step check's stand-in); and the G tokens' parser.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "port_route_map.h"
#include "port_route_plan.h"

static const char *const *sPic;
static int sRows, sWarpZone;
static int sAtX = -1, sAtZ;   // where Walk puts the player (instead of the '@')
static int sWarpType, sWarpDir = 2, sBgDir, sBgCond;   // what the W and B tiles carry (Walk's cases set them)
static int sWalkX, sWalkZ;   // where Walk's last waypoint put the player (a walk that ends in no path: the nearest it got)
static int sSurfOn;   // the player is surfing (m->surf): the '@' tile is water, '~' tiles are walkable and carry pass bits
static int sHybRail;  // the player is on a rail (HybridCases): at sHybPos, the grid tile under it sAtX,sAtZ
static PortRailPos sHybPos;
static int sGust = -1;  // >= 0: the wind is on (WindCases), the gust in sGust event checks; 'H' a shelter tile
static int sWindHold;   // the gust's count does not run on the player's tile (m->windHold, Mistralton Gym's platforms)
static PortMapWarp sHybWarp;   // a rail warp (HybridCases), none when its zone is 0

static char KindAt(int x, int z) {
    if (z < 0 || z >= sRows || x < 0 || x >= (int)strlen(sPic[z])) {
        return '#';
    }
    switch (sPic[z][x]) {
    case '#':
    case 'C':
    case 'E':
    case '=':   // (HybridCases: a rail under it)
    case 'B': return '#';
    case '~': return '~';
    case 'e': return 'e';
    case '?': return '?';   // terrain not loaded yet
    case '@': return sSurfOn ? '~' : '.';
    case 'v': return 'v';
    default: return '.';
    }
}

static int sBridge;   // BridgeCases: 'U' an upper floor (6 tiles up), 'X' a path tile with that floor over it (PORT_MAP_BRIDGE)

// BridgeCases' heights and the pass bits the game's filler makes with them (port_route_map_game.h's PortMap_Pass): no step
// to a floor 20 or more units away; a neighbour with two floors is taken at the nearer
static void BridgeHeights(PortMap *m) {
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    int x, z, d;

    for (z = 0; z < sRows; z++) {
        for (x = 0; sPic[z][x] != '\0'; x++) {
            PortMapCell *c = &m->cell[2000 + z - m->z0][1000 + x - m->x0];

            c->yf = sPic[z][x] == 'U' ? 96 : 0;
            if (sPic[z][x] == 'X') {
                c->flags |= PORT_MAP_BRIDGE;
                c->yb = 96;
            }
        }
    }
    for (z = 0; z < sRows; z++) {
        for (x = 0; sPic[z][x] != '\0'; x++) {
            PortMapCell *c = &m->cell[2000 + z - m->z0][1000 + x - m->x0];

            for (d = 0; d < 4; d++) {
                int nx = x + dx[d], nz = z + dz[d], nh;
                const PortMapCell *n;

                if (nz < 0 || nz >= sRows || nx < 0 || nx >= (int)strlen(sPic[nz])) {
                    continue;
                }
                n = &m->cell[2000 + nz - m->z0][1000 + nx - m->x0];
                nh = (n->flags & PORT_MAP_BRIDGE) && abs(n->yb - c->yf) < abs(n->yf - c->yf) ? n->yb : n->yf;
                if (abs(nh - c->yf) >= 20) {
                    c->pass &= (unsigned char)~(1 << d);
                }
                if ((c->flags & PORT_MAP_BRIDGE) && KindAt(nx, nz) == '.' &&
                    abs(((n->flags & PORT_MAP_BRIDGE) ? n->yb : n->yf) - c->yb) < 20) {
                    c->passb |= 1 << d;
                }
            }
        }
    }
}

// the mapper's chunks (ChunkCases): with sBoxR > 0 the filler loads only the tiles within sBoxR of the player, the rest '?'
// (and the planner keeps what it saw: route_map.c's Recall); 0: the whole picture, nothing remembered between maps
static int sBoxR;
// sRom (pz19): the tiles beyond sBoxR are the ROM's terrain (the adapter's PortMap_RomTerrain, a fake loader here: the picture
// itself, flagged PORT_MAP_ROM) instead of '?', and nothing is remembered between plans (each plan has only what it is given)
static int sRom;

static int Unloaded(int x, int z, int px, int pz) {
    return sBoxR > 0 && (abs(x - px) > sBoxR || abs(z - pz) > sBoxR);
}

static char SeenAt(int x, int z, int px, int pz) {
    return Unloaded(x, z, px, pz) && !sRom ? '?' : KindAt(x, z);
}

// the picture at grid 1000,2000 (so the window's offsets are tested too); pass bits as the game's filler makes them: a
// step onto a walkable tile (ledges are jumped by the planner itself)
static void Fill(PortMap *m) {
    int x, z, d, px = 0, pz = 0;
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};

    if (sBoxR == 0 || sRom) {
        PortMap_Forget();
    }
    memset(m, 0, sizeof *m);
    for (z = 0; z < sRows; z++) {
        const char *p = strchr(sPic[z], '@');

        if (p != NULL) {
            px = (int)(p - sPic[z]);
            pz = z;
        }
    }
    if (sAtX >= 0) {
        px = sAtX;
        pz = sAtZ;
    }
    m->px = 1000 + px;
    m->pz = 2000 + pz;
    m->x0 = m->px - PORT_MAP_N / 2;
    m->z0 = m->pz - PORT_MAP_N / 2;
    m->zone = 1;
    m->surf = (unsigned char)sSurfOn;
    m->rail = sHybRail;
    m->wind = sGust >= 0;
    m->gust = sGust;
    m->windHold = sWindHold;
    m->rpos = sHybPos;
    m->nwarp = sHybWarp.zone != 0;
    m->warp[0] = sHybWarp;
    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            int gx = m->x0 + x - 1000, gz = m->z0 + z - 2000;
            PortMapCell *c = &m->cell[z][x];
            char k = SeenAt(gx, gz, px, pz), s = gz >= 0 && gz < sRows && gx >= 0 && gx < (int)strlen(sPic[gz]) ? sPic[gz][gx] : '#';

            c->kind = k;
            c->flags = s == 'C' ? PORT_MAP_COUNTER : s == 'E' ? PORT_MAP_ELECTRIC : s == 'S' ? PORT_MAP_SWITCH : s == 'H' ? PORT_MAP_SHELTER : 0;
            c->slide = s == 'I'   ? PORT_MAP_ICE   // ice; the slopes push u d l r (up down left right)
                       : s == 'u' ? PORT_MAP_SLOPE
                       : s == 'd' ? PORT_MAP_SLOPE + 1
                       : s == 'l' ? PORT_MAP_SLOPE + 2
                       : s == 'r' ? PORT_MAP_SLOPE + 3
                       : s == 'P' ? PORT_MAP_PAD + 4 + 1   // a Driftveil Gym pad facing down, length class 1 (4 tiles: it lands 6 on)
                       : s == 'K' ? PORT_MAP_CANNON + 3 * 4   // a Mistralton Gym cannon's tile, shooting up, 4 tiles
                                  : 0;
            c->zone = s >= '2' && s <= '9' ? (unsigned short)(s - '0') : 1;
            if (sRom && Unloaded(gx, gz, px, pz)) {
                c->flags |= PORT_MAP_ROM;
            }
            if (k == '?') {   // unloaded: any way out (the adapter's)
                c->pass = 15;
                c->slide = 0;
            }
            if (k != '.' && !(k == '~' && sSurfOn)) {
                continue;
            }
            for (d = 0; d < 4; d++) {
                char nk = SeenAt(gx + dx[d], gz + dz[d], px, pz);

                if (nk == '.' || (nk == '~' && k == '~')) {   // never land to water: the F:surf token starts a surf
                    c->pass |= 1 << d;
                }
                if (nk == 'e' && k == '~' && KindAt(gx + 2 * dx[d], gz + 2 * dz[d]) == '.') {   // the hop over an edge tile
                    c->pass |= 1 << d;
                }
            }
        }
    }
    if (sBridge) {
        BridgeHeights(m);
    }
    PortMap_OpenUnloaded(m);   // toward '?' tiles: the core's rule, as the game's adapter
    for (z = 0; z < sRows; z++) {
        for (x = 0; sPic[z][x] != '\0'; x++) {
            if (sPic[z][x] == 'W' && m->nwarp < 64) {
                PortMapWarp *w = &m->warp[m->nwarp++];

                w->x = 1000 + x;
                w->z = 2000 + z;
                w->w = w->h = 1;
                w->zone = (unsigned)sWarpZone;
                w->dir = sWarpDir;   // arrives facing down (DIR + 1)
                w->type = sWarpType;
            }
            if ((sPic[z][x] == 'N' || sPic[z][x] == 'n' || sPic[z][x] == 'M' || sPic[z][x] == 'Q' || sPic[z][x] == 'q') &&
                m->nnpc < 64) {
                PortMapNpc *n = &m->npc[m->nnpc++];

                n->id = 5;
                n->x = n->x0 = 1000 + x;
                n->z = n->z0 = 2000 + z;
                if (sPic[z][x] == 'Q' || sPic[z][x] == 'q') {   // a 2x2 actor (Dragonspiral Tower's boulder): this tile, the
                    n->w = n->h = 2;                             // one east, the two north
                }
                n->nocoll = sPic[z][x] == 'q';   // 'q': a 2x2 boulder in its Strength hole (collision off): not in the way
                // 'n': an actor with a height, on another layer than the cells (grid height 0): not in the way; 'M': with a
                // height, on the cells' layer: in the way; 'N': no height (an adapter without layers): in the way
                n->layer = sPic[z][x] != 'N';
                n->y = n->y0 = sPic[z][x] == 'n' ? 3 : 0;
            }
            if (sPic[z][x] == 'B' && m->nbg < 64) {
                m->bg[m->nbg].x = 1000 + x;
                m->bg[m->nbg].dir = (unsigned)sBgDir;
                m->bg[m->nbg].cond = (unsigned)sBgCond;
                m->bg[m->nbg++].z = 2000 + z;
            }
            if ((sPic[z][x] == 'T' || sPic[z][x] == 'A') && m->ntrig < 64) {
                PortMapTrig *t = &m->trig[m->ntrig++];

                t->x = 1000 + x;
                t->z = 2000 + z;
                t->w = t->h = 1;
                t->avoid = sPic[z][x] == 'A';
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
static int ZoneOne(int x, int z) {   // the matrix: zone 1 everywhere east of x 0 (the warp hints' source zone is 1)
    (void)z;
    return x >= 0 ? 1 : -1;
}
static const PortRouteAdapter kHint = {"test", Fill, ZoneOne};
static int PassActor(unsigned zone, const PortMapNpc *n) {   // the 'N' actor (uid 5) is a wall: the pass ends 2 past its cell
    return zone == 1 && n->id == 5 ? 2 : 0;
}
static const PortRouteAdapter kWall = {"test", Fill, NULL, NULL, NULL, NULL, NULL, PassActor};
// rides: 0 starts at 3,4 facing up on the player's floor (y 0), 1 on floor y 10, 2 is closed, the rest none
static int Ride(unsigned zone, int n, int *x, int *z, int *y, int *dir) {
    if (zone != 1 || n > 2) {
        return 0;
    }
    *x = 1003;
    *z = 2004;
    *y = n == 1 ? 10 : 0;
    *dir = 0;
    return n == 2 ? -1 : 1;
}
static const PortRouteAdapter kRide = {"test", Fill, NULL, NULL, NULL, NULL, NULL, NULL, Ride};
static const char *RideWhy(unsigned zone, int n) {
    (void)zone;
    return n == 2 ? "switch T9 wrong for spot 2" : NULL;
}
static const PortRouteAdapter kRideWhy = {"test", Fill, NULL, NULL, NULL, NULL, NULL, NULL, Ride, NULL, RideWhy};
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

// follows a G token's plan to its end: the waypoints one after the other (the player put on each, sAtX/Z), then where it
// stands and the press there; 0 if it ends in no path
static int Walk(const char *const *pic, int rows, const char *tok, int *ex, int *ez, int *press) {
    PortRoutePlan p;
    int i, n, x = -1, z = -1, ok = 0;

    for (i = 0; i < rows; i++) {
        const char *at = strchr(pic[i], '@');

        if (at != NULL) {
            x = (int)(at - pic[i]);
            z = i;
        }
    }
    for (n = 0; n < 12 && !ok; n++) {
        int r;

        sAtX = x;
        sAtZ = z;
        r = PlanTok(pic, rows, tok, &p);
        if (r == PLAN_THERE || (r == PLAN_OK && p.final)) {
            *ex = r == PLAN_THERE ? x : p.x - 1000;
            *ez = r == PLAN_THERE ? z : p.z - 2000;
            *press = p.press;
            ok = 1;
        } else if (r != PLAN_OK) {
            break;
        }
        x = p.x - 1000;
        z = p.z - 2000;
    }
    sAtX = -1;
    sWalkX = x;
    sWalkZ = z;
    return ok;
}

static char PicAt(int x, int z) {
    return z >= 0 && z < sRows && x >= 0 && x < (int)strlen(sPic[z]) ? sPic[z][x] : '#';
}

// follows a G token's plan as the pilot and the game do (the test's own model of the slides, not route_slide.c's): the
// key toward the waypoint (x first, as route_test.c's WalkTo), a step at a time; a step onto ice goes on the way it moves
// until the next tile is in the way or the tile is not ice (field_player_grid.c), a slope's the way of each slope tile
// until the next tile is in the way (field_slope_slip.c's event: the pilot plans again where it stops). Plans again at
// each waypoint. 0: no path, or a step into a wall
static int SlideWalk(const char *const *pic, int rows, const char *tok, int *ex, int *ez) {
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    PortRoutePlan p;
    int i, n, x = -1, z = -1, steps = 0;

    for (i = 0; i < rows; i++) {
        const char *at = strchr(pic[i], '@');

        if (at != NULL) {
            x = (int)(at - pic[i]);
            z = i;
        }
    }
    for (n = 0; n < 20; n++) {
        int r;

        sAtX = x;
        sAtZ = z;
        r = PlanTok(pic, rows, tok, &p);
        if (r != PLAN_OK) {
            *ex = x;
            *ez = z;
            sAtX = -1;
            return r == PLAN_THERE;
        }
        while ((x != p.x - 1000 || z != p.z - 2000) && steps++ < 200) {
            int d = x != p.x - 1000 ? (x < p.x - 1000 ? 3 : 2) : (z < p.z - 2000 ? 1 : 0), slope = 0;

            if (KindAt(x + dx[d], z + dz[d]) != '.') {
                sAtX = -1;
                return 0;
            }
            x += dx[d];
            z += dz[d];
            for (;;) {
                char c = PicAt(x, z);

                if (c == 'u' || c == 'd' || c == 'l' || c == 'r') {
                    slope = 1;
                    d = c == 'u' ? 0 : c == 'd' ? 1 : c == 'l' ? 2 : 3;
                } else if (c != 'I' && !slope) {
                    break;
                }
                if (KindAt(x + dx[d], z + dz[d]) != '.') {
                    break;
                }
                x += dx[d];
                z += dz[d];
            }
            if (slope) {
                break;
            }
        }
    }
    sAtX = -1;
    return 0;
}

static void CheckSlide(const char *name, const char *const *pic, int rows, const char *tok, int x, int z) {
    int ex = -1, ez = -1, ok = SlideWalk(pic, rows, tok, &ex, &ez);

    printf("%s %s (ends %d,%d)\n", ok && ex == x && ez == z ? "ok  " : "FAIL", name, ex, ez);
    sFails += !(ok && ex == x && ez == z);
}

static void SlideCases(void) {
    PortRoutePlan p;
    int r;
    static const char *const line[] = {
        "#######",
        "#@III.#",
        "#######",
    };
    static const char *const room[] = {   // right to the wall, then down off the ice
        "########",
        "#@IIIII#",
        "#IIII#I#",
        "#I#IIII#",
        "#IIIIII#",
        "######.#",
        "########",
    };
    static const char *const endspot[] = {   // a stop on the ice at a wall, a step on from there
        "#########",
        "#@III#..#",
        "#IIIIIII#",
        "####I####",
        "####.####",
        "#########",
    };
    static const char *const around[] = {   // the slope would carry the player down: around it
        "#######",
        "#@.d..#",
        "#.....#",
        "#.....#",
        "#######",
    };
    static const char *const down[] = {   // the slope is the only way: the step right, the slide down
        "#####",
        "#@d##",
        "##.##",
        "##..#",
        "#####",
    };

    PortRouteMap_SetAdapter(&kTest);
    r = Plan(line, 3, 5, 1, -1, 0, &p);
    Check("ice: one step slides to the floor past it", r == PLAN_OK && p.x == 1005 && p.z == 2001 && p.final, &p);
    r = Plan(line, 3, 3, 1, -1, 0, &p);
    Check("ice: no stop in the middle of the ice", r == PLAN_NONE, &p);
    r = Plan(room, 7, 6, 5, -1, 0, &p);
    Check("ice: the first slide is the waypoint", r == PLAN_OK && p.x == 1006 && p.z == 2001 && !p.final, &p);
    CheckSlide("ice: two slides", room, 7, "G:1006,2005", 6, 5);
    CheckSlide("ice: a stop at a wall on the ice, a step on", endspot, 6, "G:1004,2004", 4, 4);
    CheckSlide("slope: around it", around, 5, "G:1005,2001", 5, 1);
    r = Plan(down, 5, 3, 3, -1, 0, &p);
    Check("slope: the step onto it is the waypoint", r == PLAN_OK && p.x == 1002 && p.z == 2001 && !p.final, &p);
    CheckSlide("slope: the step, the slide, on", down, 5, "G:1003,2003", 3, 3);
    {
        static const char *const pad[] = {"#@#", "#P#", "###", "###", "###", "###", "###", "#.#", "###"};

        r = Plan(pad, 9, 1, 7, -1, 0, &p);   // the gap is walls: the only way is the pad's jump (a step down onto it)
        Check("pad: one step down onto it lands past the gap", r == PLAN_OK && p.x == 1001 && p.z == 2007 && p.final, &p);
        r = Plan(pad, 9, 1, 3, -1, 0, &p);
        Check("pad: no stop inside the gap", r == PLAN_NONE, &p);
    }
    {
        static const char *const cannon[] = {"#.#", "###", "###", "###", "#K#", "#@#"};
        static const char *const none[] = {"#.#", "###", "###", "###", "#.#", "#@#"};

        r = Plan(cannon, 6, 1, 0, -1, 0, &p);   // the landing is behind walls: the way is the cannon's shot from its tile, up
        Check("cannon: the way is the shot up from its tile", r == PLAN_OK && p.x == 1001 && p.z == 2000 && p.final, &p);
        r = Plan(none, 6, 1, 0, -1, 0, &p);
        Check("cannon: no cannon, no way", r == PLAN_NONE, &p);
    }
}

// a walk as the pilot makes it on the chunked map (sBoxR): plan, one step toward the waypoint (and on over the ice, the
// picture's), plan again. 1: on the goal within 120 plans
static int ChunkWalk(const char *const *pic, int rows, const char *tok, int *ex, int *ez) {
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    PortRoutePlan p;
    int i, n, x = -1, z = -1;

    for (i = 0; i < rows; i++) {
        const char *at = strchr(pic[i], '@');

        if (at != NULL) {
            x = (int)(at - pic[i]);
            z = i;
        }
    }
    PortMap_Forget();
    for (n = 0; n < 120; n++) {
        int r, d;

        sAtX = x;
        sAtZ = z;
        r = PlanTok(pic, rows, tok, &p);
        if (r != PLAN_OK) {
            *ex = x;
            *ez = z;
            sAtX = -1;
            return r == PLAN_THERE;
        }
        d = x != p.x - 1000 ? (x < p.x - 1000 ? 3 : 2) : (z < p.z - 2000 ? 1 : 0);
        if (KindAt(x + dx[d], z + dz[d]) != '.') {
            break;
        }
        x += dx[d];
        z += dz[d];
        while (PicAt(x, z) == 'I' && KindAt(x + dx[d], z + dz[d]) == '.') {
            x += dx[d];
            z += dz[d];
        }
    }
    *ex = x;
    *ez = z;
    sAtX = -1;
    return 0;
}

// Black 2's Giant Chasm (zone 233, pz17): an ice column between a dead-end pocket (top) and a room walled off from the goal
// (bottom); the way is west along the top and down. Only the tiles within 10 of the player are loaded: from the bottom the
// guess is up the ice and through the unloaded north-west, from the top (before the planner kept what it saw) down the ice
// and through the unloaded south, which the bottom had seen walled: it slid up and down for ever
static void ChunkCases(void) {
    static const char *const chasm[] = {
        "########################",
        "#...........I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.##########I###########",
        "#.....T.#.......@...####",
        "#.......#...........####",
        "########################",
    };
    int ex = -1, ez = -1, ok;

    sBoxR = 10;
    ok = ChunkWalk(chasm, 24, "G:trig:0", &ex, &ez) && ex == 6 && ez == 21;
    sBoxR = 0;
    printf("%s chunks: a way guessed through unloaded tiles seen walled before is not taken again (ends %d,%d)\n",
           ok ? "ok  " : "FAIL", ex, ez);
    sFails += !ok;
    sBoxR = 10;   // the unloaded chunks from the ROM, nothing remembered: the first plan already goes up the ice and west
    sRom = 1;
    ok = ChunkWalk(chasm, 24, "G:trig:0", &ex, &ez) && ex == 6 && ez == 21;
    sRom = 0;
    sBoxR = 0;
    printf("%s chunks: the ROM's terrain of unloaded chunks (no memory) walks the way at once (ends %d,%d)\n", ok ? "ok  " : "FAIL",
           ex, ez);
    sFails += !ok;
}

// surfing (F:surf's end, route_map.c's Walkable): the water is walked, an edge tile hopped onto the land beyond; never from
// land into the water, and a walker on the land does not plan over water
static void CheckWalk(const char *name, const char *const *pic, int rows, const char *tok, int x, int z, int press);
// a goal tile that cannot be reached: no path at once (a walk that goes nearer is for a goal that is no tile, a wall)
static void CheckNear(const char *name, const char *const *pic, int rows, const char *tok, int x, int z) {
    int ex, ez, ep, ok = Walk(pic, rows, tok, &ex, &ez, &ep);

    printf("%s %s (stops at %d,%d)\n", !ok && sWalkX == x && sWalkZ == z ? "ok  " : "FAIL", name, sWalkX, sWalkZ);
    sFails += !(!ok && sWalkX == x && sWalkZ == z);
}

// the wind (Black 2 / White 2's Mistralton Gym; route_slide.c's PortRouteWind_Hop): a corridor 20 tiles up, shelters 7 and 13 up
static void WindCases(void) {
    static const char *const hall[] = {"#.#", "#.#", "#.#", "#.#", "#.#", "#.#", "#.#", "#H#", "#.#", "#.#", "#.#",
                                       "#.#", "#.#", "#H#", "#.#", "#.#", "#.#", "#.#", "#.#", "#.#", "#@#"};
    PortRoutePlan p;
    int r;

    sGust = PORT_GUST_MARGIN + 8 * PORT_GUST_STEP;   // 8 steps: the goal is not in reach, the shelter 7 up is
    r = Plan(hall, 21, 1, 0, -1, 0, &p);
    Check("wind: a hop to the shelter in reach", r == PLAN_OK && p.x == 1001 && p.z == 2013 && !p.final && !p.wait, &p);
    sAtX = 1;   // in that shelter, 3 steps: the next one is 6 up, wait for the gust
    sAtZ = 13;
    sGust = PORT_GUST_MARGIN + 3 * PORT_GUST_STEP;
    r = Plan(hall, 21, 1, 0, -1, 0, &p);
    Check("wind: wait in the shelter", r == PLAN_OK && p.wait && p.x == 1001 && p.z == 2013, &p);
    sWindHold = 1;   // the shelter is a platform: the count stands still there, no gust comes: walk on (b2 gym6-out)
    r = Plan(hall, 21, 1, 0, -1, 0, &p);
    Check("wind: no wait where the count stands still", r == PLAN_OK && !p.wait && p.z < 2013, &p);
    sWindHold = 0;
    sGust = PORT_GUST_MARGIN + 12 * PORT_GUST_STEP;   // after the gust, a longer calm: the next shelter, then the goal
    r = Plan(hall, 21, 1, 0, -1, 0, &p);
    Check("wind: a whole calm, the next shelter", r == PLAN_OK && p.x == 1001 && p.z == 2007 && !p.wait, &p);
    sAtX = 1;
    sAtZ = 7;
    r = Plan(hall, 21, 1, 0, -1, 0, &p);
    Check("wind: the goal in reach, the plain plan", r == PLAN_OK && p.x == 1001 && p.z == 2000 && p.final, &p);
    sAtX = -1;
    sGust = -1;
    r = Plan(hall, 21, 1, 0, -1, 0, &p);
    Check("wind: off, the plain plan", r == PLAN_OK && p.z == 2000 && p.final && !p.wait, &p);
}

// the way round goes beyond the window (fx4): two corridors joined only at row 142, past the window's last row from the
// player at row 10 (the window is PORT_MAP_N tiles round him, all of it known, as the ROM's terrain makes it), the goal in
// the east one at row 60 (b back-mist: Mistralton's zone in the window, the way into it just past its edge): the plan goes
// south toward the window's edge (not "unreachable"), then plans again. A goal back north (5,0) is not looked for that way
// (no walking out of a dead end the way the player came: "G:npc:N beyond the window and walled off")
static void EdgeCases(void) {
    static char rows[150][8];
    static const char *pic[150];
    PortRoutePlan p;
    int r, z;

    for (z = 0; z < 150; z++) {
        strcpy(rows[z], z >= 140 && z < 145 ? "#.....#" : "#.###.#");
        pic[z] = rows[z];
    }
    rows[10][1] = '@';
    r = Plan(pic, 150, 5, 60, -1, 0, &p);
    Check("edge: a way round beyond the window", r == PLAN_OK && !p.final && p.x == 1001 && p.z > 2010, &p);
    r = Plan(pic, 150, 5, 0, -1, 0, &p);
    Check("edge: not away from the goal", r == PLAN_NONE, &p);
}

static void SurfCases(void) {
    static const char *const lake[] = {
        "##########",
        "#..e~@~e.#",
        "##########",
    };
    static const char *const two[] = {   // two lakes with a land strip between them: the walk stops on the strip
        "#############",
        "#e~@~e.e~~e.#",
        "#############",
    };
    static const char *const dry[] = {   // on the land, the water is in the way
        "#########",
        "#@..~~~.#",
        "#########",
    };
    static const char *const bay[] = {   // the shore tile is next to the water with no edge tile
        "#######",
        "#.~~~@#",
        "#######",
    };

    PortRouteMap_SetAdapter(&kTest);
    sSurfOn = 1;
    CheckWalk("surf: over the water and the edge tile to the land", lake, 3, "G:1008,2001", 8, 1, -1);
    CheckWalk("surf: the other way", lake, 3, "G:1001,2001", 1, 1, -1);
    CheckWalk("surf: a step on the water", lake, 3, "G:1004,2001", 4, 1, -1);
    CheckWalk("surf: onto a shore with no edge tile", bay, 3, "G:1001,2001", 1, 1, -1);
    CheckWalk("surf: onto the strip between two lakes", two, 3, "G:1006,2001", 6, 1, -1);
    CheckNear("surf: no re-entering the water from the land (no path to the far shore)", two, 3, "G:1011,2001", 3, 1);
    {   // Seaside Cave (Black 2 zone 515): the lake's channel runs north into the unloaded chunk, the goal is up there
        static const char *const chan[] = {"#????#", "#????#", "##~###", "##~###", "#.~@~#", "######"};
        PortRoutePlan p;
        int r = Plan(chan, 6, 3, 0, -1, 0, &p);

        Check("surf: up a channel into the unloaded chunk", r == PLAN_OK && p.x == 1002 && p.z == 2004, &p);
        sSurfOn = 0;
        r = Plan(chan, 6, 3, 0, -1, 0, &p);
        Check("no surf: the channel is no way into the unloaded chunk", r != PLAN_OK, &p);
    }
    sSurfOn = 0;
    CheckNear("no surf: the water is a wall (no path across)", dry, 3, "G:1007,2001", 1, 1);
}

static void CheckWalk(const char *name, const char *const *pic, int rows, const char *tok, int x, int z, int press) {
    int ex = -1, ez = -1, ep = -9, ok = Walk(pic, rows, tok, &ex, &ez, &ep);

    printf("%s %s (ends %d,%d press %d)\n", ok && ex == x && ez == z && ep == press ? "ok  " : "FAIL", name, ex, ez, ep);
    sFails += !(ok && ex == x && ez == z && ep == press);
}

// PortRouteMap_PushFrom for the picture's first actor pushed in dir: the tile and whether one is reachable
static void CheckPushFrom(const char *name, const char *const *pic, int rows, int dir, int x, int z, int ok) {
    const PortMap *m;
    int ex = -1, ez = -1, r = -1;

    sPic = pic;
    sRows = rows;
    sAtX = -1;
    m = PortRouteMap_Get();
    if (m->ok && m->nnpc > 0) {
        r = PortRouteMap_PushFrom(m, 0, dir, &ex, &ez);
        ex -= 1000;
        ez -= 2000;
    }
    printf("%s %s (%d at %d,%d)\n", r == ok && ex == x && ez == z ? "ok  " : "FAIL", name, r, ex, ez);
    sFails += !(r == ok && ex == x && ez == z);
}

static void CheckParse(const char *tok, int ok, int kind, int a, int b, int talk) {
    PortRouteGoal g;
    char why[160] = "";
    int r = PortRoute_ParseGoal(tok, &g, why, sizeof why), good;

    good = ok ? r && g.kind == kind && (kind == GOAL_TILE ? g.x == a && g.z == b : g.n == a) && g.talk == talk : !r;
    printf("%s parse \"%s\"%s%s\n", good ? "ok  " : "FAIL", tok, why[0] ? ": " : "", why);
    sFails += !good;
}

// ---- a made-up rail: line 0 (front 0..9) runs on into line 1 (front 0..4), both sides -2..2; up side + 1, down side - 1,
// left front + 1, right front - 1 (as Castelia's streets); a step off the sides is not taken (a wall: a warp there)
static PortRailPos sRStart;
static PortMapWarp sRWarp;
static PortMapTrig sRTrig;
static PortMapNpc sRNpc;
static int sRHaveWarp, sRHaveTrig, sRHaveNpc;
static int sRLedge;   // line 1 front 2 is a ledge jumped going left: from front 1 the step lands on front 3, one way
static int sRSlide;   // a cliff slide: Down at rail[1,4,-2] (off the side: blocked) lands on rail[0,0,2]

static void RailFill(PortMap *m) {
    memset(m, 0, sizeof *m);
    m->ok = 1;
    m->zone = 1;
    m->rail = 1;
    m->rpos = sRStart;
    m->nwarp = sRHaveWarp;
    m->warp[0] = sRWarp;
    m->ntrig = sRHaveTrig;
    m->trig[0] = sRTrig;
    m->nnpc = sRHaveNpc;
    m->npc[0] = sRNpc;
}

static int RailAt(PortRailPos *p) {
    *p = sRStart;
    return 1;
}

static int RailStep(const PortRailPos *from, int dir, PortRailPos *to) {
    static const int kF[4] = {0, 0, 1, -1}, kS[4] = {1, -1, 0, 0};

    if (sRSlide && dir == 1 && from->line == 1 && from->front == 4 && from->side == -2) {   // PortRail_Slide's landing
        *to = *from;
        to->line = 0;
        to->front = 0;
        to->side = 2;
        return 1;
    }
    *to = *from;
    to->front += kF[dir];
    to->side += kS[dir];
    if (sRLedge && to->line == 1 && to->front == 2) {   // the adapter's railStep over a ledge (PortRail_Step: two steps)
        if (dir != 2) {
            return 0;
        }
        to->front = 3;
    }
    if (to->line == 0 && to->front > 9) {
        to->line = 1;
        to->front = 0;
    } else if (to->line == 1 && to->front < 0) {
        to->line = 0;
        to->front = 9;
    }
    return to->front >= 0 && to->front <= (to->line == 0 ? 9 : 4) && to->side >= -2 && to->side <= 2;
}

// a hybrid zone's hand-off: from rail[0,9,0] pressing left (front + 1, past the line's end) the player steps onto the grid
static int RailToGrid(const PortRailPos *p, int dir) {
    return p->line == 0 && p->front == 9 && p->side == 0 && dir == 2;
}

static const PortRouteAdapter kRail = {"test-rail", RailFill, NULL, RailAt, RailStep, RailToGrid, NULL};

static int RailPlanTok(int line, int front, int side, const char *tok, PortRoutePlan *p) {
    PortRouteState st;
    PortRouteGoal g;
    char why[160];

    sRStart.line = line;
    sRStart.isLine = 1;
    sRStart.front = front;
    sRStart.side = side;
    memset(&st, 0, sizeof st);
    st.zone = 1;
    memset(p, 0, sizeof *p);
    if (!PortRoute_ParseGoal(tok, &g, why, sizeof why)) {
        snprintf(p->why, sizeof p->why, "%s", why);
        return -1;
    }
    return PortRoutePlan_Next(&st, &g, p);
}

static void RailCases(void) {
    PortRoutePlan p;
    int r;
    PortRailPos b = {0, 1, 1, 0, 0};

    PortRouteMap_SetAdapter(&kRail);
    PortRoutePlan_Reset();
    r = RailPlanTok(0, 0, 0, "G:rail:1,2,0", &p);
    Check("G:rail onto the next line", r == PLAN_OK && p.dir == 2 && p.steps == 12 && p.rnext.front == 1, &p);
    r = RailPlanTok(0, 0, 0, "G:rail:0,0,0", &p);
    Check("G:rail on the place", r == PLAN_THERE, &p);
    r = RailPlanTok(0, 0, 0, "G:3,4", &p);
    Check("G:X,Z on a hybrid zone's rail: to the hand-off", r == PLAN_OK && p.handoff && p.steps == 9 && p.dir == 2, &p);
    r = RailPlanTok(0, 9, 0, "G:3,4", &p);
    Check("G:X,Z at the hand-off: press", r == PLAN_THERE && p.handoff && p.press == 2, &p);
    sRWarp.rail = 1;   // a wall's warp beyond side 2, front 3..4, arrived at facing down: walked into facing up
    sRWarp.line = 0;
    sRWarp.x = 3;
    sRWarp.z = 3;
    sRWarp.w = 2;
    sRWarp.h = 1;
    sRWarp.zone = 7;
    sRWarp.type = 7;
    sRWarp.dir = 2;
    sRHaveWarp = 1;
    r = RailPlanTok(0, 0, 0, "G:warp:7", &p);
    Check("G:warp:N into a rail wall's warp", r == PLAN_OK && p.steps == 5 && p.press == 0, &p);
    r = RailPlanTok(0, 4, 2, "G:warp:7", &p);
    Check("G:warp:N at the rail warp: press", r == PLAN_THERE && p.press == 0, &p);
    r = RailPlanTok(0, 0, 0, "G:warp:9", &p);
    Check("G:warp:N no rail warp: names the zones", r == PLAN_NONE && strstr(p.why, "lead to: 7") != NULL, &p);
    sRHaveWarp = 0;
    sRNpc.rail = 1;   // an actor at rail[0,1,0]: around it, or next to it
    sRNpc.id = 5;
    sRNpc.line = 0;
    sRNpc.x = 1;
    sRNpc.z = 0;
    sRHaveNpc = 1;
    r = RailPlanTok(0, 0, 0, "G:rail:0,2,0", &p);
    Check("G:rail around an actor", r == PLAN_OK && p.steps == 4, &p);
    r = RailPlanTok(0, 3, 0, "G:npc:5", &p);
    Check("G:npc:N on a rail: next to it, facing it", r == PLAN_OK && p.steps == 1 && p.dir == 3 && p.press == 3, &p);
    sRHaveNpc = 0;
    PortRoutePlan_BlockRail(1, &b);
    r = RailPlanTok(0, 0, 0, "G:rail:0,2,0", &p);
    Check("G:rail around a blocked place", r == PLAN_OK && p.steps == 4, &p);
    PortRoutePlan_Reset();
    sRTrig.rail = 1;   // a trigger across the rail at front 5
    sRTrig.line = 0;
    sRTrig.x = 5;
    sRTrig.z = -2;
    sRTrig.w = 1;
    sRTrig.h = 5;
    sRHaveTrig = 1;
    r = RailPlanTok(0, 0, 1, "G:trig:0", &p);
    Check("G:trig:N onto a rail trigger", r == PLAN_OK && p.steps == 5 && p.press == -1, &p);
    sRHaveTrig = 0;
    sRLedge = 1;   // a rail ledge (Dragonspiral Tower's rail floors): the jump is one step of the search, back is no path
    r = RailPlanTok(1, 1, 0, "G:rail:1,4,0", &p);
    Check("G:rail over a rail ledge", r == PLAN_OK && p.steps == 2 && p.dir == 2 && p.rnext.front == 3, &p);
    r = RailPlanTok(1, 4, 0, "G:rail:1,0,0", &p);
    Check("G:rail back up a rail ledge: no path", r == PLAN_NONE, &p);
    sRSlide = 1;   // a cliff slide (Victory Road, zone 214: PortRail_Step's slide tile, Down blocked): one step to the rail below
    r = RailPlanTok(1, 4, 0, "G:rail:0,0,0", &p);
    Check("G:rail down a cliff slide", r == PLAN_OK && p.steps == 5 && p.dir == 1, &p);
    r = RailPlanTok(1, 4, -2, "G:rail:0,0,0", &p);
    Check("G:rail at the slide: Down", r == PLAN_OK && p.dir == 1 && p.rnext.line == 0 && p.rnext.front == 0 && p.rnext.side == 2, &p);
    sRSlide = 0;
    sRLedge = 0;
}

// a walkway over a path (Opelucid Gym's upper floor over its corridors: port_route_map_game.h's PortMap_Reach gives a tile
// the flood reaches on two floors PORT_MAP_BRIDGE): the walkway crosses the bridge tile straight, the path below too, and
// neither turns onto the other there
static void BridgeCases(void) {
    static const char *const pic[] = {
        "#####.#####",
        "#####.#####",
        "UUUUUXUUUUU",
        "#####.#####",
        "#####.#####",
    };
    PortRoutePlan p;
    int r;

    sBridge = 1;
    sAtX = 0;
    sAtZ = 2;
    r = Plan(pic, 5, 10, 2, -1, 0, &p);
    Check("bridge: the walkway across it", r == PLAN_OK && p.x == 1010 && p.z == 2002, &p);
    r = Plan(pic, 5, 5, 0, -1, 0, &p);
    Check("bridge: no turn from the walkway onto the path below", r == PLAN_NONE, &p);
    sAtX = 5;
    sAtZ = 0;
    r = Plan(pic, 5, 5, 4, -1, 0, &p);
    Check("bridge: the path below through it", r == PLAN_OK && p.x == 1005 && p.z == 2004, &p);
    r = Plan(pic, 5, 0, 2, -1, 0, &p);
    Check("bridge: no turn from the path onto the walkway", r == PLAN_NONE, &p);
    sAtX = -1;
    sBridge = 0;
}

// ---- a hybrid zone (a grid and rails in one zone: Icirrus and Opelucid Gyms). A row's run of '=' is a rail line (in reading
// order: line 0, 1, ...), front 0 at its left end, a wall for the grid; 'S' is a switch tile: the key right (left) held at an
// 'S' with a run on its right (left) steps onto the rail's first (last) place, and at the run's right (left) end the key right
// (left) steps off onto the grid tile after the run
static int HybRun(int line, int *x0, int *z0, int *len) {
    int z, x, n = 0;

    for (z = 0; z < sRows; z++) {
        for (x = 0; sPic[z][x] != '\0'; x++) {
            if (sPic[z][x] == '=' && (x == 0 || sPic[z][x - 1] != '=') && n++ == line) {
                *x0 = x;
                *z0 = z;
                for (*len = 0; sPic[z][x + *len] == '='; (*len)++) {
                }
                return 1;
            }
        }
    }
    return 0;
}

static int HybStep(const PortRailPos *from, int dir, PortRailPos *to) {
    int rx, rz, len, f = from->front + (dir == 3) - (dir == 2);

    *to = *from;
    if (dir < 2 || !HybRun(from->line, &rx, &rz, &len)) {
        return 0;
    }
    to->front = f;
    return f >= 0 && f < len;
}

static int HybToGrid(const PortRailPos *p, int dir) {
    int rx, rz, len;

    if (!HybRun(p->line, &rx, &rz, &len)) {
        return 0;
    }
    return (dir == 3 && p->front == len - 1 && KindAt(rx + len, rz) == '.') || (dir == 2 && p->front == 0 && KindAt(rx - 1, rz) == '.');
}

static int HybLand(const PortRailPos *p, int dir, int *x, int *z) {
    int rx, rz, len;

    if (!HybRun(p->line, &rx, &rz, &len)) {
        return 0;
    }
    *x = 1000 + (dir == 3 ? rx + len : rx - 1);
    *z = 2000 + rz;
    return 1;
}

static int HybToRail(int x, int z, int dir, PortRailPos *to) {
    int px = x - 1000, pz = z - 2000, tx = px + (dir == 3) - (dir == 2), line, rx, rz, len;

    if (dir < 2 || px < 0 || pz < 0 || pz >= sRows || sPic[pz][px] != 'S' || tx < 0 || sPic[pz][tx] != '=') {
        return 0;
    }
    for (line = 0; HybRun(line, &rx, &rz, &len); line++) {
        if (rz == pz && tx >= rx && tx < rx + len) {
            to->line = line;
            to->isLine = 1;
            to->front = dir == 3 ? 0 : len - 1;
            to->side = 0;
            to->dir = 0;
            return 1;
        }
    }
    return 0;
}

static const PortRouteAdapter kHyb = {"test-hyb", Fill, NULL, NULL, HybStep, HybToGrid, HybToRail, NULL, NULL, NULL, NULL, HybLand};

static void HybridCases(void) {
    static const char *const one[] = {"#@.S===S.N#"};   // the wall between the rooms is a rail
    static const char *const two[] = {"#@.S===S.S===S.N#"};   // two rails, three rooms
    PortRoutePlan p;
    int r;

    PortRouteMap_SetAdapter(&kHyb);
    sAtX = 1;
    sAtZ = 0;
    r = PlanTok(one, 1, "G:npc:5", &p);
    Check("hybrid: no way on the grid: to the switch tile", r == PLAN_OK && p.x == 1003 && !p.final, &p);
    sAtX = 3;
    r = PlanTok(one, 1, "G:npc:5", &p);
    Check("hybrid: on the switch tile: press onto the rail", r == PLAN_THERE && p.handoff && p.press == 3, &p);
    sAtX = 1;
    r = PlanTok(two, 1, "G:npc:5", &p);
    Check("hybrid: grid, rail, grid, rail, grid: the first switch tile", r == PLAN_OK && p.x == 1003 && !p.final, &p);
    sAtX = 8;
    r = PlanTok(two, 1, "G:npc:5", &p);
    Check("hybrid: the second room's switch tile", r == PLAN_OK && p.x == 1009 && !p.final, &p);
    sHybWarp.rail = 1;   // a door on rail 0 at front 1: the switch tile that leads onto rail 0, not the nearest one (the League hall)
    sHybWarp.line = 0;
    sHybWarp.x = 1;
    sHybWarp.w = 1;
    sHybWarp.h = 1;
    sHybWarp.zone = 7;
    sHybWarp.type = 7;
    sHybWarp.dir = 2;
    sAtX = 9;   // standing on the switch tile of rail 1
    r = PlanTok(two, 1, "G:warp:7", &p);
    Check("hybrid: a rail warp: not the nearest switch tile (rail 1), the one onto its rail", r == PLAN_OK && p.x == 1007 && !p.handoff, &p);
    sHybWarp.line = 1;
    sAtX = 8;   // between the switch tile of rail 0 (7) and rail 1 (9)
    r = PlanTok(two, 1, "G:warp:7", &p);
    Check("hybrid: a rail warp on rail 1: its switch tile", r == PLAN_OK && p.x == 1009, &p);
    sHybWarp.line = 5;   // on no rail
    r = PlanTok(two, 1, "G:warp:7", &p);
    Check("hybrid: a rail warp no switch tile leads to", r == PLAN_NONE && strstr(p.why, "a rail that has it") != NULL, &p);
    memset(&sHybWarp, 0, sizeof sHybWarp);
    sAtX = 1;
    sHybRail = 1;   // on the rail at its first place (the player's grid tile there is under the wall)
    sHybPos.line = 0;
    sHybPos.isLine = 1;
    sHybPos.front = 0;
    sHybPos.side = 0;
    sAtX = 4;
    r = PlanTok(one, 1, "G:npc:5", &p);   // the nearest hand-off (front 0, left) leads back; the way is the far end
    Check("hybrid: on the rail: to the hand-off that leads to the goal", r == PLAN_OK && p.dir == 3 && p.steps == 2 && p.rnext.front == 1, &p);
    sHybPos.front = 2;
    sAtX = 6;
    r = PlanTok(one, 1, "G:npc:5", &p);
    Check("hybrid: at that hand-off: press onto the grid", r == PLAN_THERE && p.handoff && p.press == 3, &p);
    sHybRail = 0;
    sAtX = -1;
    PortRouteMap_SetAdapter(&kTest);
}

// ---- a hybrid zone's switches (Icirrus Gym): the lever 'L' flips switch 0; walked onto rightwards, the place 'P' hands the
// player onto the rail '===' (line 0) when switch 0 is flipped; an arm actor (uid 6) stands on the way to P (row 0) until
// the switch is flipped, then in the wall below it
static unsigned sSwState;

static void SwFill(PortMap *m) {
    PortMapNpc *a;
    int x, z;

    Fill(m);
    m->sw = sSwState;
    for (z = 0; z < sRows; z++) {
        for (x = 0; sPic[z][x] != '\0'; x++) {
            if (sPic[z][x] == 'L') {
                m->lever[0].x = 1000 + x;
                m->lever[0].z = 2000 + z;
                m->lever[0].s = 0;
                m->lever[0].x1 = m->lever[0].x;
                m->lever[0].z1 = m->lever[0].z;
                m->nlever = 1;
            } else if (sPic[z][x] == 'P') {
                m->place[0].x = 1000 + x;
                m->place[0].z = 2000 + z;
                m->place[0].dir = 3;
                m->place[0].s = 0;
                m->place[0].want = 1;
                m->place[0].to.line = 0;
                m->place[0].to.isLine = 1;
                m->place[0].to.front = 0;
                m->nplace = 1;
            }
        }
    }
    a = &m->npc[m->nnpc];
    memset(a, 0, sizeof *a);
    a->id = 6;
    a->x = a->x0 = 1003;
    a->z = a->z0 = 2000 + (sSwState & 1);
    m->arm[0].npc = m->nnpc++;
    m->arm[0].x1 = 1003;
    m->arm[0].z1 = 2000 + !(sSwState & 1);
    m->arm[0].s = 0;
    m->narm = 1;
}

static const PortRouteAdapter kSw = {"test-sw", SwFill, NULL, NULL, HybStep, HybToGrid, HybToRail, NULL, NULL, NULL, NULL, HybLand};

static void SwitchCases(void) {
    static const char *const pic[] = {"#@..P===.N#", "##L########"};
    PortRoutePlan p;
    int r;

    PortRouteMap_SetAdapter(&kSw);
    sSwState = 0;   // the arm in the way, the place off: the lever first
    sAtX = 1;
    sAtZ = 0;
    r = PlanTok(pic, 2, "G:npc:5", &p);
    Check("switches: the lever first (not final)", r == PLAN_OK && p.x == 1002 && p.z == 2000 && !p.final, &p);
    sAtX = 2;
    r = PlanTok(pic, 2, "G:npc:5", &p);
    Check("switches: onto the lever", r == PLAN_OK && p.x == 1002 && p.z == 2001 && !p.final, &p);
    sSwState = 1;   // flipped (the player on the lever): the arm moved, the place on
    sAtX = 2;
    sAtZ = 1;
    r = PlanTok(pic, 2, "G:npc:5", &p);
    Check("switches: flipped: toward the place", r == PLAN_OK && p.x == 1002 && p.z == 2000 && !p.final, &p);
    sAtX = 3;
    sAtZ = 0;
    r = PlanTok(pic, 2, "G:npc:5", &p);
    Check("switches: walk onto the place: the hand-off", r == PLAN_THERE && p.handoff && p.press == 3, &p);
    sAtX = 4;
    r = PlanTok(pic, 2, "G:npc:5", &p);
    Check("switches: on the place: its key is the hand-off", r == PLAN_THERE && p.handoff && p.press == 3, &p);
    sSwState = 0;   // no lever in the window: the arm stays in the way
    sAtX = 2;
    r = PlanTok(pic, 1, "G:npc:5", &p);
    Check("switches: no lever: no path", r == PLAN_NONE, &p);
    sAtX = -1;
    PortRouteMap_SetAdapter(&kTest);
}

// ---- lifts (Black 2's Opelucid Gym): levers without a switch (s -1) that move the player to another platform, each live
// in one state (sw) and leaving the next. 'a' lifts to 'b' (state 0 -> 1), 'c' to 'f' (1 -> 2: the goal's platform), 'g'
// (the nearest) to 'h' (0 -> 3), a platform with no way on; 'x' is a lift of state 3, walked over in state 0
static int sLiftsOn = 1;
static unsigned sLiftState;

static void LiftFill(PortMap *m) {
    static const char kFrom[4] = {'a', 'c', 'g', 'x'}, kTo[4] = {'b', 'f', 'h', 'x'};
    static const int kOn[4] = {0, 1, 0, 3}, kTo2[4] = {1, 2, 3, 0};
    int x, z, i, fx[4] = {0}, fz[4] = {0}, tx[4] = {0}, tz[4] = {0};

    Fill(m);
    for (z = 0; z < sRows; z++) {
        for (x = 0; sPic[z][x] != '\0'; x++) {
            for (i = 0; i < 4; i++) {
                if (sPic[z][x] == kFrom[i]) {
                    fx[i] = x;
                    fz[i] = z;
                } else if (sPic[z][x] == kTo[i]) {
                    tx[i] = x;
                    tz[i] = z;
                }
            }
        }
    }
    m->sw = sLiftState;
    for (i = 0; i < 4 && sLiftsOn; i++) {
        m->lever[m->nlever].x = 1000 + fx[i];
        m->lever[m->nlever].z = 2000 + fz[i];
        m->lever[m->nlever].s = -1;
        m->lever[m->nlever].x1 = 1000 + tx[i];
        m->lever[m->nlever].z1 = 2000 + tz[i];
        m->lever[m->nlever].on = kOn[i];
        m->lever[m->nlever].to = kTo2[i];
        m->nlever++;
    }
}

static const PortRouteAdapter kLift = {"test-lift", LiftFill, NULL, NULL, HybStep, HybToGrid, HybToRail, NULL, NULL, NULL, NULL, HybLand};

// ---- warp pads into the zone itself (Black's Opelucid Gym: the pad behind the Leader back to the entrance): 'p' a pad
// whose arrival is 'a' (warp index 1 after it), the goal 'W' a warp to zone 7
static void PadFill(PortMap *m) {
    int x, z, pi = -1, ai = -1;

    Fill(m);
    for (z = 0; z < sRows; z++) {
        for (x = 0; sPic[z][x] != '\0'; x++) {
            if ((sPic[z][x] == 'p' || sPic[z][x] == 'a') && m->nwarp < 64) {
                PortMapWarp *w = &m->warp[m->nwarp];

                w->x = 1000 + x;
                w->z = 2000 + z;
                w->w = w->h = 1;
                w->type = 6;   // a warp-pad (fires on the tile)
                w->zone = sPic[z][x] == 'p' ? m->zone : 65535;
                w->dest = 65535;
                *(sPic[z][x] == 'p' ? &pi : &ai) = m->nwarp++;
            }
        }
    }
    if (pi >= 0 && ai >= 0) {
        m->warp[pi].dest = (unsigned)ai;
    }
}

static const PortRouteAdapter kPad = {"test-pad", PadFill, NULL};

static void PadCases(void) {
    static const char *const pic[] = {"#####", "#.p.#", "#.@.#", "#####", "#.a.#", "#...#", "#.W.#", "#####"};
    static const char *const shut[] = {"#####", "#.p.#", "#.@.#", "#####", "#.a.#", "#####", "#.W.#", "#####"};
    static const char *const nopad[] = {"#####", "#...#", "#.@.#", "#####", "#.a.#", "#...#", "#.W.#", "#####"};
    PortRoutePlan p;
    int r;

    PortRouteMap_SetAdapter(&kPad);
    r = Plan(pic, 8, 0, 0, 7, 7, &p);
    Check("pads: walled off from the goal: onto the pad whose arrival reaches it (not final)",
          r == PLAN_OK && p.x == 1002 && p.z == 2001 && !p.final, &p);
    sAtX = 2;
    sAtZ = 4;
    r = Plan(pic, 8, 0, 0, 7, 7, &p);
    Check("pads: arrived: on to the goal", r == PLAN_OK && p.x == 1002 && p.z == 2005 && p.final, &p);
    sAtX = -1;
    r = Plan(shut, 8, 0, 0, 7, 7, &p);
    Check("pads: the arrival does not reach the goal either: no path", r == PLAN_NONE, &p);
    r = Plan(nopad, 8, 0, 0, 7, 7, &p);
    Check("pads: none: no path", r == PLAN_NONE, &p);
    PortRouteMap_SetAdapter(&kTest);
}

static void LiftCases(void) {
    static const char *const pic[] = {"#.g@..a#b.c#f.N#", "################", "#h##############"};
    static const char *const over[] = {"#@.x..N#"};
    PortRoutePlan p;
    int r;

    PortRouteMap_SetAdapter(&kLift);
    sLiftState = 0;
    sAtX = 3;
    sAtZ = 0;
    r = PlanTok(pic, 3, "G:npc:5", &p);
    Check("lifts: the one whose chain reaches the goal, not the nearest (not final)", r == PLAN_OK && p.x == 1006 && p.z == 2000 && !p.final, &p);
    sLiftState = 1;
    sAtX = 8;
    r = PlanTok(pic, 3, "G:npc:5", &p);
    Check("lifts: landed: on to the next lift", r == PLAN_OK && p.x == 1010 && p.z == 2000 && !p.final, &p);
    sLiftState = 0;
    r = PlanTok(pic, 3, "G:npc:5", &p);
    Check("lifts: the next lift is another state's: no path", r == PLAN_NONE, &p);
    sLiftState = 2;
    sAtX = 12;
    r = PlanTok(pic, 3, "G:npc:5", &p);
    Check("lifts: on the goal's platform: the walk to it", r == PLAN_OK && p.x == 1013 && p.final, &p);
    sLiftState = 0;
    sAtX = 1;
    r = PlanTok(over, 1, "G:npc:5", &p);
    Check("lifts: another state's lift tile is walked over", r == PLAN_OK && p.x == 1005 && p.final, &p);
    sLiftsOn = 0;
    sAtX = 3;
    r = PlanTok(pic, 3, "G:npc:5", &p);
    Check("lifts: none: no path", r == PLAN_NONE, &p);
    sLiftsOn = 1;
    sAtX = -1;
    PortRouteMap_SetAdapter(&kTest);
}

// ---- switches that turn rail lines on and off (Opelucid Gym's dragons): the lever 'L' flips switch 4; the rail '===' (line
// 0) is on only while switch 4 is flipped (the adapter's railSwitches sets that for the search and puts it back)
static unsigned sRsOn;   // the rail line's switch state the adapter answers for now
static int sRsSets;      // railSwitches(1, ...) calls

static int RsStep(const PortRailPos *from, int dir, PortRailPos *to) {
    return (sRsOn >> 4) & 1 ? HybStep(from, dir, to) : 0;
}

static int RsToRail(int x, int z, int dir, PortRailPos *to) {
    return (sRsOn >> 4) & 1 ? HybToRail(x, z, dir, to) : 0;
}

static void RsFill(PortMap *m) {
    int x, z;

    Fill(m);
    m->sw = sSwState;
    sRsOn = sSwState;
    for (z = 0; z < sRows; z++) {
        for (x = 0; sPic[z][x] != '\0'; x++) {
            if (sPic[z][x] == 'L') {
                m->lever[0].x = 1000 + x;
                m->lever[0].z = 2000 + z;
                m->lever[0].s = 4;
                m->lever[0].x1 = m->lever[0].x;
                m->lever[0].z1 = m->lever[0].z;
                m->nlever = 1;
            }
        }
    }
}

static void RsSet(int set, unsigned sw) {
    sRsSets += set;
    sRsOn = set ? sw : sSwState;
}

static const PortRouteAdapter kRs = {"test-rs", RsFill, NULL, NULL, RsStep, HybToGrid, RsToRail, NULL, NULL, NULL, NULL, HybLand, RsSet};

static void RailSwitchCases(void) {
    static const char *const pic[] = {"#@.S===S.N#", "#L#########"};
    PortRoutePlan p;
    int r;

    PortRouteMap_SetAdapter(&kRs);
    sSwState = 0;   // the rail off: the lever first
    sAtX = 1;
    sAtZ = 0;
    sRsSets = 0;
    r = PlanTok(pic, 2, "G:npc:5", &p);
    Check("rail switches: the lever first (not final)", r == PLAN_OK && p.x == 1001 && p.z == 2001 && !p.final, &p);
    Check("rail switches: searched with the rail on, put back after", sRsSets > 0 && sRsOn == 0, &p);
    sSwState = 0x10;   // flipped (the player on the lever): the rail on
    sAtX = 1;
    sAtZ = 1;
    r = PlanTok(pic, 2, "G:npc:5", &p);
    Check("rail switches: flipped: toward the switch tile", r == PLAN_OK && p.x == 1001 && p.z == 2000 && !p.final, &p);
    sSwState = 0;   // no lever in the window: no way
    sAtX = 1;
    sAtZ = 0;
    r = PlanTok(pic, 1, "G:npc:5", &p);
    Check("rail switches: no lever: no path", r == PLAN_NONE, &p);
    sAtX = -1;
    PortRouteMap_SetAdapter(&kTest);
}

// ---- switches that change the grid (Black 2 / White 2's Plasma Frigate floor panels): the levers '0'..'3' press switches
// 0..3; the zone's picture with switches sw flipped from now is sPipePics[sPipeReal ^ sw] (the adapter's gridSwitches
// refills the map from it); the switch pressed last (sPipeLast) does nothing until another is pressed
static const char *const *const *sPipePics;
static unsigned sPipeReal, sPipeCur;
static int sPipeLast = -1, sPipeSets;

static void PipeFill(PortMap *m) {
    int x, z;

    sPic = sPipePics[sPipeCur];
    Fill(m);
    m->leverOnce = 1;
    m->leverLast = sPipeLast;
    for (z = 0; z < sRows; z++) {
        for (x = 0; sPic[z][x] != '\0'; x++) {
            if (sPic[z][x] >= '0' && sPic[z][x] <= '3' && m->nlever < PORT_MAP_LEVERS) {
                m->lever[m->nlever].x = m->lever[m->nlever].x1 = 1000 + x;
                m->lever[m->nlever].z = m->lever[m->nlever].z1 = 2000 + z;
                m->lever[m->nlever].s = sPic[z][x] - '0';
                m->nlever++;
            }
        }
    }
}

static void PipeSet(PortMap *m, int set, unsigned sw) {
    sPipeSets += set;
    sPipeCur = set ? sPipeReal ^ sw : sPipeReal;
    PipeFill(m);
}

static const PortRouteAdapter kPipe = {"test-pipe", PipeFill, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                       PipeSet};

static int PipeTok(const char *const *const *pics, int rows, const char *tok, PortRoutePlan *p) {
    sPipePics = pics;
    sPipeCur = sPipeReal;
    return PlanTok(pics[sPipeReal], rows, tok, p);
}

static void PipeCases(void) {
    // the wall at x 7 opens while switch 1 is flipped; switch 0 (nearer, in the niche below) does nothing
    static const char *const shut[] = {"#@...1.#.N#", "##0########"};
    static const char *const open[] = {"#@...1...N#", "##0########"};
    static const char *const *const byB[4] = {shut, shut, open, open};
    // the same wall opened by switch 0 alone
    static const char *const *const byA[4] = {shut, open, shut, open};
    // a way past a switch that is not needed: the straight one over it, and one around it
    static const char *const around[] = {"#@.0.N#", "#.###.#", "#.....#"};
    static const char *const *const still[4] = {around, around, around, around};
    PortRoutePlan p;
    int r;

    PortRouteMap_SetAdapter(&kPipe);
    sAtX = 1;
    sAtZ = 0;
    sPipeReal = 0;
    sPipeLast = -1;
    sPipeSets = 0;
    r = PipeTok(byB, 2, "G:npc:5", &p);
    Check("grid switches: the far switch that opens the way, not the near one", r == PLAN_OK && p.x == 1005 && p.z == 2000 && !p.final,
          &p);
    Check("grid switches: searched with the grid switched, put back after", sPipeSets > 0 && sPipeCur == 0, &p);
    sPipeReal = 2;   // pressed (the player on it): it is the last, the way open
    sPipeLast = 1;
    sAtX = 5;
    r = PipeTok(byB, 2, "G:npc:5", &p);
    Check("grid switches: pressed: on to the goal", r == PLAN_OK && p.x == 1008 && p.z == 2000, &p);
    sPipeReal = 0;
    sPipeLast = -1;
    sAtX = 1;
    r = PipeTok(still, 3, "G:npc:5", &p);
    Check("grid switches: around a switch the way does not need", r == PLAN_OK && p.x == 1001 && p.z == 2002, &p);
    sPipeLast = 0;   // pressed last: dead, walked over
    r = PipeTok(still, 3, "G:npc:5", &p);
    Check("grid switches: over the switch pressed last", r == PLAN_THERE || (r == PLAN_OK && p.x == 1004 && p.z == 2000), &p);
    sPipeLast = -1;
    r = PipeTok(byA, 2, "G:npc:5", &p);
    Check("grid switches: the switch in the niche opens it", r == PLAN_OK && p.x == 1002 && p.z == 2000 && !p.final, &p);
    sPipeLast = 0;   // that switch pressed last: the other one first, then it
    r = PipeTok(byA, 2, "G:npc:5", &p);
    Check("grid switches: the switch pressed last only after another", r == PLAN_OK && p.x == 1005 && p.z == 2000 && !p.final, &p);
    sPipeLast = -1;
    sAtX = -1;
    PortRouteMap_SetAdapter(&kTest);
}

// the map dump: the electric rock is an E in the grid, and a full dump lists every actor (the short one stops at 12)
static void DumpCases(void) {
    static const char *const pic[] = {
        "###########",
        "#@..E.....#",
        "#NNNNNNNNNNN#",
        "#NNNNNNNNNN.#",
        "###########",
    };
    char dir[] = "/tmp/route_plan_dump.XXXXXX", path[256], line[256];
    int sawE = 0, actors = 0, header = 0;
    FILE *f;

    if (mkdtemp(dir) == NULL) {
        printf("FAIL dump: no temp dir\n");
        sFails++;
        return;
    }
    sPic = pic;
    sRows = 5;
    sAtX = -1;
    PortRouteMap_SetAdapter(&kTest);
    setenv("BWPORT_MAPDUMP", dir, 1);
    setenv("BWPORT_MAPDUMP_FULL", "1", 1);
    PortRouteMap_Dump("T", "end", 1);
    snprintf(path, sizeof path, "%s/mapdump-01-end.txt", dir);
    f = fopen(path, "r");
    while (f != NULL && fgets(line, sizeof line, f) != NULL) {
        sawE |= line[0] == ' ' && strstr(line, "@..E") != NULL;
        actors += strncmp(line, "  N uid 5 at ", 13) == 0;
        header |= strstr(line, "actors (21):") != NULL;
    }
    if (f != NULL) {
        fclose(f);
    }
    unlink(path);
    rmdir(dir);
    unsetenv("BWPORT_MAPDUMP");
    unsetenv("BWPORT_MAPDUMP_FULL");
    printf("%s dump: E for an electric rock, all %d actors listed in a full dump (E %d, header %d)\n",
           sawE && header && actors == 21 ? "ok  " : "FAIL", actors, sawE, header);
    sFails += !(sawE && header && actors == 21);
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
    {   // a warp tile that is not the goal is no way across (Sprout Tower's stairs in a corridor, a mat in a doorway)
        static const char *const around[] = {"#######", "#@W...#", "#.....#", "#######"};
        static const char *const only[] = {"#######", "#@W..##", "#######"};

        r = Plan(around, 4, 4, 1, -1, 7, &p);   // the way round: down first (the straight way crosses the warp)
        Check("a non-goal warp: the path goes round it", r == PLAN_OK && p.x == 1001 && p.z == 2002, &p);
        r = Plan(only, 3, 4, 1, -1, 7, &p);   // the one way: across it
        Check("a non-goal warp: crossed when it is the one way", r == PLAN_OK && p.x == 1004 && p.z == 2001, &p);
    }
    {   // a key warp (Gen 4's mats, ladders, stairs): the player stands on it holding the key into the wall: a step in from
        // beside it stands on the mat without firing, so only the step in from behind it and its own tile are goals
        static const char *const mat[] = {"#####", "#...#", "#.@.#", "#.W.#", "#####"};
        static const char *const matSide[] = {"#####", "#...#", "#...#", "#@W.#", "#####"};

        sWarpDir = 1;   // arrives facing up: the key is down
        r = Plan(matSide, 5, 0, 0, 7, 7, &p);
        Check("a mat: from beside it, press into it (any side)", r == PLAN_THERE && p.press == 3, &p);
        sWarpType = 7;
        r = Plan(matSide, 5, 0, 0, 7, 7, &p);
        Check("a key mat: from beside it, onto the mat first", r == PLAN_OK && p.x == 1002 && p.z == 2003, &p);
        r = Plan(mat, 5, 0, 0, 7, 7, &p);
        Check("a key mat: from behind it, press down into it", r == PLAN_THERE && p.press == 1, &p);
        sWarpType = 0;
        sWarpDir = 2;
    }
    {   // a warp tile the search only touches must not reorder the equal-cost goals: Platinum's contest hall lobby, the player
        // two tiles above the door mat (a key mat), the actor to be faced at 9,3: its west side (8,3) and its south side (9,4)
        // are both 7 steps; the way to the south side is the one the search took before the warp cost (lobbyfan's end tile)
        static const char *const lobby[] = {"..................#", "..................#", ".................##",
                                            ".........N.......##", "..........##......#", "...@......##.....##",
                                            ".................##", "...W..............#", "###################"};
        PortRoutePlan at;
        int ex, ez, press;

        sWarpType = 7;
        sWarpDir = 1;
        memset(&at, 0, sizeof at);
        Walk(lobby, 9, "G:npc:5", &ex, &ez, &press);
        at.x = 1000 + ex;
        at.z = 2000 + ez;
        Check("a warp the search touches: the goals pop in their old order", ex == 9 && ez == 4, &at);
        sWarpType = 0;
        sWarpDir = 2;
    }
    r = Plan(edge, 4, 0, 0, 2, 7, &p);   // no warp to zone 2: its tiles where the map goes on
    Check("G:warp:N to a zone's edge", r == PLAN_OK && p.final && p.x == 1001 && p.z == 2002 && p.steps == 4, &p);
    {   // a warp 150 tiles east, beyond the window: BWPORT_WARP_HINTS (Z=S:x,z) is the far target, only where the matrix says S
        static char top[210], mid[210];
        const char *const corr[] = {top, mid, top};

        memset(top, 35, 200);   // '#'
        memset(mid, 46, 200);   // '.'
        mid[0] = mid[199] = '#';
        mid[1] = '@';
        PortRouteMap_SetAdapter(&kHint);
        setenv("BWPORT_WARP_HINTS", "9=1:1150,2001;9=5:1010,2001;8=1:1005,2001", 1);   // (zone 5's is not on the matrix)
        r = Plan(corr, 3, 0, 0, 9, 9, &p);
        Check("G:warp:N beyond the window: the hint", r == PLAN_OK && !p.final && p.x == 1064 && p.z == 2001 && p.steps == 63, &p);
        setenv("BWPORT_WARP_HINTS", "9=5:1010,2001;8=1:1150,2001", 1);
        r = Plan(corr, 3, 0, 0, 9, 9, &p);
        Check("G:warp:N beyond the window: no usable hint", r == PLAN_NONE && strstr(p.why, "no hint") != NULL, &p);
        unsetenv("BWPORT_WARP_HINTS");
        r = Plan(corr, 3, 0, 0, 9, 9, &p);
        Check("G:warp:N beyond the window: no hint at all", r == PLAN_NONE && strstr(p.why, "no hint") != NULL, &p);
        PortRouteMap_SetAdapter(&kTest);
    }
    {   // a G:npc / G:trig target 190 tiles along a corridor, beyond the window: hops toward it, the window moves with the player
        static char top[210], mid[210], mid2[210];
        const char *const corr[] = {top, mid, top};
        const char *const corr2[] = {top, mid2, top};

        memset(top, 35, 200);   // '#'
        memset(mid, 46, 200);   // '.'
        mid[0] = mid[199] = '#';
        mid[1] = '@';
        mid[190] = 'N';
        CheckWalk("G:npc:N 190 tiles away: hops, ends facing it", corr, 3, "G:npc:5", 189, 1, 3);
        mid[190] = 'T';
        CheckWalk("G:trig:N 190 tiles away: hops onto it", corr, 3, "G:trig:0", 190, 1, -1);
        {   // a live trigger the adapter marks (hgpil1: HeartGold's gym pits): round it when there is a way round, else across;
            // a goal on it as any tile
            static const char *const loop[] = {"#######", "#@.A..#", "#.###.#", "#.....#", "#######"};
            static const char *const line[] = {"#######", "#@.A..#", "#######"};
            PortRoutePlan p;

            r = PlanTok(loop, 5, "G:1005,2001", &p);
            Check("G:X,Z past a live trigger: the way round it first (down)", r == PLAN_OK && !p.final && p.x == 1001 && p.z == 2003, &p);
            CheckWalk("G:X,Z past a live trigger: round it to the tile", loop, 5, "G:1005,2001", 5, 1, -1);
            CheckWalk("G:X,Z past a live trigger with no way round: across it", line, 3, "G:1005,2001", 5, 1, -1);
            CheckWalk("G:trig:0 onto a live trigger", loop, 5, "G:trig:0", 3, 1, -1);
            CheckWalk("G:X,Z onto a live trigger's tile", loop, 5, "G:1003,2001", 3, 1, -1);
        }
        memcpy(mid2, mid, 210);
        mid2[190] = '.';
        mid2[100] = '#';   // walled off 90 tiles on: the nearest tile it gets to is the one before the wall
        mid2[150] = 'N';
        CheckNear("G:npc:N beyond the window and walled off: ends at the wall", corr2, 3, "G:npc:5", 99, 1);
        {   // the target is in the window but the terrain round it is not loaded (a gym's far chunk): toward it, not "0 goal tiles"
            const char *const unl[] = {"#######", "#@..?N#", "#######"};
            PortRoutePlan p;

            r = PlanTok(unl, 3, "G:npc:5", &p);
            Check("G:npc:N with unloaded terrain round it: a hop toward it", r == PLAN_OK && !p.final && p.x == 1004 && p.z == 2001, &p);
        }
    }
    {
        static const char *const big[] = {"#########", "#.......#", "#.......#", "#...Q..@#", "#########"};
        static const char *const bigN[] = {"#########", "#.......#", "#.......#", "#...Q...#", "#....@..#", "#########"};

        CheckWalk("G:npc:N a 2x2 actor: next to its footprint, not its tile", big, 5, "G:npc:5", 6, 3, 2);
        CheckWalk("G:npc:N a 2x2 actor from below its east half", bigN, 6, "G:npc:5", 5, 4, 0);
    }
    {   // F:strength:UID:DIR's far side (pz20: Black 2's Victory Road, zone 587/590): any tile next to the footprint there
        static const char *const rock[] = {"#######", "#.....#", "#.....#", "#..Q..#", "#..#..#", "#....@#", "#######"};
        static const char *const shut[] = {"#######", "#.....#", "#..Q..#", "#..##.#", "###..@#", "#######"};
        static const char *const hole[] = {"#######", "#.....#", "#@.q..#", "#######"};
        static const char *const rockIn[] = {"#######", "#.....#", "#@.Q..#", "#######"};
        int ex, ez, ep;

        CheckPushFrom("push up a 2x2 boulder: under its east half (the west one is a wall)", rock, 7, 0, 4, 4, 1);
        CheckPushFrom("push left a 2x2 boulder: the nearer of its two east tiles", rock, 7, 2, 5, 3, 1);
        CheckPushFrom("push right a 2x2 boulder: west of it, the lower tile first", rock, 7, 3, 2, 3, 1);
        CheckPushFrom("push down a 2x2 boulder: above it, its east tile nearer", rock, 7, 1, 4, 1, 1);
        CheckPushFrom("push up, the far side walled off: none (the first tile for the walk's no path)", shut, 6, 0, 3, 3, 0);
        CheckWalk("a 2x2 boulder in its Strength hole (collision off) is walked over", hole, 4, "G:1005,2002", 5, 2, -1);
        r = Walk(rockIn, 4, "G:1005,2002", &ex, &ez, &ep);
        printf("%s a 2x2 boulder out of a hole is in the way (no path)\n", r ? "FAIL" : "ok  ");
        sFails += r != 0;
    }
    // the parser
    CheckParse("G:12,-3", 1, GOAL_TILE, 12, -3, 0);
    CheckParse("G:12,3 W30", 1, GOAL_TILE, 12, 3, 0);   // the token ends at a space
    CheckParse("G:warp:427", 1, GOAL_WARP, 427, 0, 0);
    CheckParse("G:door:3", 1, GOAL_WARP, 3, 0, 0);
    CheckParse("G:door:3:nota", 0, 0, 0, 0, 0);
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
    // facing the way the event needs (the BG event's direction; a walk-in warp's)
    {
        static const char *const shelf[] = {   // a BG event at 4,2; the player starts west of it (2,2)
            "#########",
            "#.......#",
            "#.@.B...#",
            "#.......#",
            "#########",
        };
        static const char *const above[] = {   // the same, the player on the tile north of it
            "#########",
            "#...@...#",
            "#...B...#",
            "#.......#",
            "#########",
        };
        static const char *const below[] = {   // the player south of it, a wall east of it
            "#########",
            "#.......#",
            "#...B#..#",
            "#...@...#",
            "#########",
        };
        static const char *const wallS[] = {   // the player west of it, a wall south of it
            "#########",
            "#.......#",
            "#.@.B...#",
            "#...#...#",
            "#########",
        };
        static const char *const gateW[] = {   // a warp at 3,2; the player starts west of it
            "#######",
            "#.....#",
            "#@.W..#",
            "#.....#",
            "#######",
        };
        static const char *const gateE[] = {   // the player east of it
            "#######",
            "#.....#",
            "#..W.@#",
            "#.....#",
            "#######",
        };
        static const char *const nook[] = {   // a warp at 2,2 with a wall on its west
            "#######",
            "#.....#",
            "##W..@#",
            "#.....#",
            "#######",
        };

        sBgDir = 0;   // read facing up: from the south
        CheckWalk("G:bg:N dir 0 from the south, facing up", shelf, 5, "G:bg:0", 4, 3, 0);
        sBgDir = 1;   // facing right: from the west
        CheckWalk("G:bg:N dir 1 from the west, facing right", above, 5, "G:bg:0", 3, 2, 3);
        sBgDir = 2;   // facing left: from the east
        CheckWalk("G:bg:N dir 2 from the east, facing left", shelf, 5, "G:bg:0", 5, 2, 2);
        sBgDir = 3;   // facing down: from the north
        CheckWalk("G:bg:N dir 3 from the north, facing down", shelf, 5, "G:bg:0", 4, 1, 1);
        sBgDir = 4;   // any: where the player is
        CheckWalk("G:bg:N dir 4 from any side", above, 5, "G:bg:0", 4, 1, 1);
        sBgDir = 5;   // left or right: not from the south
        CheckWalk("G:bg:N dir 5 from a side", below, 5, "G:bg:0", 3, 2, 3);
        sBgDir = 6;   // up or down: not from the sides
        CheckWalk("G:bg:N dir 6 from above or below", wallS, 5, "G:bg:0", 4, 1, 1);
        sBgDir = 0;
        sBgCond = 2;   // a hidden item: from any side, whatever its direction
        CheckWalk("G:bg:N hidden item from the near side", shelf, 5, "G:bg:0", 3, 2, 3);
        sBgCond = 0;
        sWarpType = 2;   // walk-in-2, arrival facing west (dir 3): walked into facing east, from the west
        sWarpDir = 3;
        CheckWalk("G:warp:N walk-in from the west", gateE, 5, "G:warp:0", 2, 2, 3);
        sWarpDir = 4;    // arrival facing east (dir 4): walked into facing west, from the east
        CheckWalk("G:warp:N walk-in from the east", gateW, 5, "G:warp:0", 4, 2, 2);
        sWarpType = 3;   // a door, arrival facing down (dir 2): walked into facing up, from the south
        sWarpDir = 2;
        CheckWalk("G:warp:N door from the south", gateE, 5, "G:warp:0", 3, 3, 0);
        sWarpType = 1;   // a mat: its own tile, pressing into the wall: from any side
        CheckWalk("G:warp:N mat from the near side", gateE, 5, "G:warp:0", 4, 2, 2);
        sWarpType = 5;   // a warp pad fires on the tile: any side
        sWarpDir = 3;
        CheckWalk("G:warp:N warp pad from any side", gateW, 5, "G:warp:0", 2, 2, 3);
        sWarpType = 2;   // the side it needs has a wall: any side
        CheckWalk("G:warp:N walk-in with a wall on its side", nook, 5, "G:warp:0", 3, 2, 2);
        sWarpType = 0;
        sWarpDir = 2;
        {
            static const char *const twoDoors[] = {   // D0 at 2,2 (near) and D1 at 8,2, both to zone 0 (a cave's rooms)
                "###########",
                "#.........#",
                "#.W..@...W#",
                "#.........#",
                "###########",
            };
            PortRoutePlan p;

            CheckWalk("G:warp:N two doors to the zone: the nearest", twoDoors, 5, "G:warp:0", 3, 2, 2);
            CheckWalk("G:door:I the door asked for, not the nearest", twoDoors, 5, "G:door:1", 8, 2, 3);
            CheckWalk("G:door:I door 0", twoDoors, 5, "G:door:0", 3, 2, 2);
            Check("G:door:I no such door", PlanTok(twoDoors, 5, "G:door:2", &p) == PLAN_NONE && strstr(p.why, "no grid warp D2") != NULL,
                  &p);
        }
    }
    {   // G:wall:N: the cell before the pass-through actor N, pressing toward it
        static const char *const wall[] = {   // the wall N in a one-tile gap between two rooms
            "#######",
            "#.....#",
            "#.@...#",
            "###N###",
            "#.....#",
            "#.....#",
            "#######",
        };
        static const char *const wallS[] = {   // the player south of it
            "#######",
            "#.....#",
            "#.....#",
            "###N###",
            "#.....#",
            "#@....#",
            "#######",
        };

        PortRouteMap_SetAdapter(&kWall);
        CheckWalk("G:wall:N from the north room", wall, 7, "G:wall:5", 3, 2, 1);
        CheckWalk("G:wall:N from the south room", wallS, 7, "G:wall:5", 3, 4, 0);
        sAtX = 3;   // on the cell before it: nothing to walk, press down, the pass ends 2 past the wall's cell (3,5)
        sAtZ = 2;
        r = PlanTok(wall, 7, "G:wall:5", &p);
        sAtX = -1;
        Check("G:wall:N on the cell before it", r == PLAN_THERE && p.press == 1 && p.passX == 1003 && p.passZ == 2005, &p);
        r = PlanTok(wall, 7, "G:wall:9", &p);
        Check("G:wall:N no such actor", r == PLAN_NONE && strstr(p.why, "no actor of uid 9") != NULL, &p);
        PortRouteMap_SetAdapter(&kTest);
        r = PlanTok(wall, 7, "G:wall:5", &p);
        Check("G:wall:N the game names no pass-through actor", r == PLAN_NONE && strstr(p.why, "not a pass-through") != NULL, &p);
        PortRouteMap_SetAdapter(&kTest);
    }
    {
        static const char *const room[] = {
            "#######",
            "#.....#",
            "###.###",
            "#.....#",
            "#.....#",
            "#@....#",
            "#######",
        };

        PortRouteMap_SetAdapter(&kRide);
        CheckWalk("G:ride:N to its start tile, the key toward its entry", room, 7, "G:ride:0", 3, 4, 0);
        r = PlanTok(room, 7, "G:ride:1", &p);
        Check("G:ride:N on another floor", r == PLAN_NONE && strstr(p.why, "floor y 10") != NULL, &p);
        r = PlanTok(room, 7, "G:ride:2", &p);
        Check("G:ride:N closed", r == PLAN_NONE && strstr(p.why, "closed") != NULL, &p);
        PortRouteMap_SetAdapter(&kRideWhy);
        r = PlanTok(room, 7, "G:ride:2", &p);
        Check("G:ride:N closed, the adapter's own reason", r == PLAN_NONE && strstr(p.why, "switch T9 wrong for spot 2") != NULL, &p);
        PortRouteMap_SetAdapter(&kRide);
        r = PlanTok(room, 7, "G:ride:7", &p);
        Check("G:ride:N no such ride", r == PLAN_NONE && strstr(p.why, "no such ride") != NULL, &p);
        PortRouteMap_SetAdapter(&kTest);
        r = PlanTok(room, 7, "G:ride:0", &p);
        Check("G:ride:N the game has no rides", r == PLAN_NONE && strstr(p.why, "no such ride") != NULL, &p);
    }
    CheckParse("G:ride:4", 1, GOAL_RIDE, 4, 0, 0);
    CheckParse("G:ride:", 0, 0, 0, 0, 0);
    CheckParse("G:wall:3", 1, GOAL_WALL, 3, 0, 0);
    CheckParse("G:wall:", 0, 0, 0, 0, 0);
    CheckParse("G:wall:3:nota", 0, 0, 0, 0, 0);
    RailCases();
    SlideCases();
    ChunkCases();
    WindCases();
    EdgeCases();
    HybridCases();
    BridgeCases();
    SwitchCases();
    LiftCases();
    PadCases();
    RailSwitchCases();
    PipeCases();
    SurfCases();
    DumpCases();
    {   // actors block only at their own grid height (Driftveil Gym: a girder's actors are not in the way on the one above)
        static const char *const other[] = {"######", "#@.n.#", "######"};
        static const char *const same[] = {"######", "#@.M.#", "######"};
        static const char *const nolayer[] = {"######", "#@.N.#", "######"};

        r = Plan(other, 3, 4, 1, -1, 0, &p);
        Check("G:X,Z past an actor on another layer", r == PLAN_OK && p.x == 1004 && p.z == 2001 && p.steps == 3, &p);
        r = Plan(same, 3, 4, 1, -1, 0, &p);
        Check("G:X,Z an actor on the player's layer blocks", r == PLAN_NONE, &p);
        r = Plan(nolayer, 3, 4, 1, -1, 0, &p);
        Check("G:X,Z an actor without a height blocks", r == PLAN_NONE, &p);
    }
    CheckParse("G:rail:1,2,-3", 1, GOAL_RAIL, 1, 0, 0);
    CheckParse("G:rail:1,2", 0, 0, 0, 0, 0);
    printf("%s\n", sFails ? "FAIL" : "PASS");
    return sFails != 0;
}
