// The route pilot's map (docs/port-worker.md, "Routes"; port/include/port_route_map.h): the dump (BWPORT_MAPDUMP, the
// pilot's Md token, a stuck walk), the G tokens' parser and their planner (A* on the game's own walkability, port/src/
// route_test.c walks the plan). Game-neutral: the game is the registered PortRouteAdapter's fill and zoneAt. Test-only:
// nothing runs unless the pilot or BWPORT_MAPDUMP asks.
//
// The dump, a text file in the run's out/ dir (BWPORT_MAPDUMP=DIR, or 1: BWPORT_SHOTS's dir, else the working dir),
// mapdump-NN-WHY.txt: the grid window (short: the player, the G targets and what is near them, unloaded rows one line; with
// BWPORT_MAPDUMP_FULL=1 the 64 x 64 around the player), a row a line with its z first and the x on top every 8th tile:
//   .  walk          #  blocked       ~  water (surf)    ,  grass         ^ v < >  a ledge, jumped that way
//   e  a surf edge tile (blocked for walking; surfing, a step onto it hops over it)   F  a waterfall tile (blocked)
//   W  a whirlpool tile (HeartGold / SoulSilver; blocked: F:whirlpool crosses it)
//   L  a magma tile off the platforms (HeartGold / SoulSilver, Blackthorn Gym; blocked: the gym's own collision answers per tile)
//   E  an electric rock (Chargestone Cave; blocked): a pushed crystal slides until it stops next to one
//   O  a Strength hole (class 0x1d; blocked)   o  a Strength hole a boulder fills (walk: the game's saved boulder there)
//   ?  no terrain (a chunk the mapper has not loaded and the ROM's chunk file gives none: the planner tries it)   (space)  off
//      the map. A chunk not loaded is the ROM's terrain (the adapter's PORT_MAP_ROM cells, pz19), printed as any cell
//   @  the player    N  an actor (NPC, item ball, ...)    D  a warp        T  a trigger (coordinate event)   b  a BG event
//   n  an actor at another grid height than the cell's layer (a girder above or below): not in the way
//   =  a hybrid zone's switch tile (walk; a key held from it toward a wall may step onto a rail: the hand-offs list)
//   i  ice           s  a slope slip  (walk: a step onto it slides on, port/src/route_slide.c; the planner plans the slides)
//   p  a Driftveil Gym pad (Black 2 / White 2)   c  a Mistralton Gym cannon's tile (Black / White: the key toward the wall there shoots)
// then the lists: warps (x,z w x h -> zone, warp, type, dir), triggers (x,z w x h, script, its work and value), BG
// events (x,z script), actors (every live one: uid at x,z, its grid height y, from x,z, object code, move code; "other
// layer" when the cell's layer is at another height), and the map matrix's zones around (a 32 x 32 chunk each).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_route.h"
#include "port_route_map.h"
#include "port_route_plan.h"
#include "port_route_story.h"

static const PortRouteAdapter *sAdapter;   // the game's (port_route_map.h)
static PortMap sMap;

// ---- the terrain the planner has seen (pz17, 2026-11-13) --------------------------------------------------------------------
// The mapper keeps only the chunks near the player loaded, and the planner takes '?' as open floor. A way guessed through '?'
// that the game then loads as walls, walked away from, is unloaded and guessed again: Black 2's G:trig:0 from zone 233's Frigate
// door slid the ice column x 60 up and down for 60000 frames (at its north end the south chunks were '?', a short way; at its
// south end those were walls and the west chunk '?', the long way north). So every cell once loaded is remembered (until the
// zone changes) and a '?' cell is the remembered one; the steps between a remembered cell and its neighbours are the ones the
// game gave when both were loaded, else open (as toward '?'). Since pz19 the adapter fills an unloaded chunk from the ROM's
// chunk files (PORT_MAP_ROM): static terrain, so a cell the game showed loaded (a switch's panel, a moved floor) is put over it
// the same way, and ROM cells are never remembered as seen.
#define MEM_CHUNKS 64   // 32 x 32 tiles each (the map matrix's chunks), reused oldest first
typedef struct {
    int cx, cz, used;
    PortMapCell cell[32][32];
    unsigned char known[32][32];   // 16: seen; bit d (0..3): its pass bit d was set with the neighbour that way loaded
} MemChunk;
static MemChunk sMem[MEM_CHUNKS];
static MemChunk *sMemLast;
static int sMemNext;
static unsigned sMemZone = 0xffffffffu;
static unsigned char sRecalled[PORT_MAP_N][PORT_MAP_N];

void PortMap_Forget(void) {
    int i;

    for (i = 0; i < MEM_CHUNKS; i++) {
        sMem[i].used = 0;
    }
    sMemLast = NULL;
    sMemNext = 0;
    sMemZone = 0xffffffffu;
}

static MemChunk *MemFind(int x, int z, int make) {
    int cx = x >> 5, cz = z >> 5, i;
    MemChunk *c;

    if (sMemLast != NULL && sMemLast->cx == cx && sMemLast->cz == cz) {
        return sMemLast;
    }
    for (i = 0; i < MEM_CHUNKS; i++) {
        if (sMem[i].used && sMem[i].cx == cx && sMem[i].cz == cz) {
            return sMemLast = &sMem[i];
        }
    }
    if (!make) {
        return NULL;
    }
    c = &sMem[sMemNext];
    sMemNext = (sMemNext + 1) % MEM_CHUNKS;
    c->used = 1;
    c->cx = cx;
    c->cz = cz;
    memset(c->known, 0, sizeof c->known);
    return sMemLast = c;
}

static int OpenKind(const PortMap *m, char k) {
    return k == '.' || k == ',' || (k == '~' && m->surf);
}

// remember the loaded cells, fill the '?' ones from what was seen before
static void Recall(PortMap *m) {
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    int x, z, d;

    if (m->zone != sMemZone) {
        PortMap_Forget();
        sMemZone = m->zone;
    }
    memset(sRecalled, 0, sizeof sRecalled);
    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            const PortMapCell *c = &m->cell[z][x];
            int wx = m->x0 + x, wz = m->z0 + z;
            unsigned char known = 16, old;
            MemChunk *mc;
            PortMapCell *mcell;

            if (c->kind == '?' || c->kind == ' ' || (c->flags & PORT_MAP_ROM)) {
                continue;
            }
            for (d = 0; d < 4; d++) {
                int nx = x + dx[d], nz = z + dz[d];

                if (nx >= 0 && nz >= 0 && nx < PORT_MAP_N && nz < PORT_MAP_N && m->cell[nz][nx].kind != '?' &&
                    !(m->cell[nz][nx].flags & PORT_MAP_ROM)) {
                    known |= (unsigned char)(1 << d);
                }
            }
            mc = MemFind(wx, wz, 1);
            mcell = &mc->cell[wz & 31][wx & 31];
            old = mc->known[wz & 31][wx & 31];
            {
                unsigned char keep = (old & 16) && mcell->kind == c->kind ? (unsigned char)(old & ~known & 15) : 0;
                unsigned char pass = (unsigned char)((c->pass & ~keep) | (mcell->pass & keep));

                *mcell = *c;
                mcell->pass = pass;
                mc->known[wz & 31][wx & 31] = (unsigned char)(known | keep);
            }
        }
    }
    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            int wx = m->x0 + x, wz = m->z0 + z;
            MemChunk *mc;

            if ((m->cell[z][x].kind != '?' && !(m->cell[z][x].flags & PORT_MAP_ROM)) || (mc = MemFind(wx, wz, 0)) == NULL ||
                !(mc->known[wz & 31][wx & 31] & 16)) {
                continue;
            }
            m->cell[z][x] = mc->cell[wz & 31][wx & 31];
            sRecalled[z][x] = 1;
        }
    }
    for (z = 0; z < PORT_MAP_N; z++) {   // the steps across a recalled cell's sides
        for (x = 0; x < PORT_MAP_N; x++) {
            PortMapCell *c = &m->cell[z][x];
            int wx = m->x0 + x, wz = m->z0 + z;
            MemChunk *mc;
            unsigned char known;

            if (!OpenKind(m, c->kind) || (mc = MemFind(wx, wz, 0)) == NULL) {
                continue;
            }
            known = mc->known[wz & 31][wx & 31];
            for (d = 0; d < 4; d++) {
                int nx = x + dx[d], nz = z + dz[d];

                if (nx < 0 || nz < 0 || nx >= PORT_MAP_N || nz >= PORT_MAP_N || (!sRecalled[z][x] && !sRecalled[nz][nx])) {
                    continue;
                }
                if (known & (1 << d)) {
                    c->pass = (unsigned char)((c->pass & ~(1 << d)) | (mc->cell[wz & 31][wx & 31].pass & (1 << d)));
                } else if (OpenKind(m, m->cell[nz][nx].kind) || m->cell[nz][nx].kind == '?') {
                    c->pass |= (unsigned char)(1 << d);
                }
            }
        }
    }
}

void PortMap_OpenUnloaded(PortMap *m) {
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    int x, z, d;

    Recall(m);
    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            PortMapCell *c = &m->cell[z][x];

            if (c->kind != '.' && c->kind != ',' && !(c->kind == '~' && m->surf)) {
                continue;
            }
            for (d = 0; d < 4; d++) {
                int nx = x + dx[d], nz = z + dz[d];

                if (nx >= 0 && nz >= 0 && nx < PORT_MAP_N && nz < PORT_MAP_N && m->cell[nz][nx].kind == '?') {
                    c->pass |= 1 << d;
                }
            }
        }
    }
}

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

// Whether actor n standing at height y is in the way on grid tile gx,gz: the game's actor collision
// (field_actor_tool.c func_ov010_02163af0) blocks only when the tile's layer is at the actor's grid height. An actor
// without heights (layer 0) and a tile with no terrain (no layer to compare) block always.
static int NpcBlocks(const PortMap *m, const PortMapNpc *n, int gx, int gz, int y) {
    int x = gx - m->x0, z = gz - m->z0;
    const PortMapCell *c;

    if (n->nocoll) {
        return 0;   // a boulder in its hole: the game's collision is off for it
    }
    if (!n->layer || x < 0 || z < 0 || x >= PORT_MAP_N || z >= PORT_MAP_N) {
        return 1;
    }
    c = &m->cell[z][x];
    return c->kind == ' ' || c->kind == '?' || c->gy == y;
}

static int NpcW(const PortMapNpc *n) {
    return n->w > 1 ? n->w : 1;
}

static int NpcH(const PortMapNpc *n) {
    return n->h > 1 ? n->h : 1;
}

static const char *NpcSize(const PortMapNpc *n) {
    static char buf[40];

    if (NpcW(n) == 1 && NpcH(n) == 1) {
        return "";
    }
    snprintf(buf, sizeof buf, " size %dx%d", NpcW(n), NpcH(n));
    return buf;
}

// grid tile gx,gz is under actor n standing at ax,az (its collision footprint: field_actor_tool.c's CheckAreaOverActor)
static int NpcOver(const PortMapNpc *n, int ax, int az, int gx, int gz) {
    return gx >= ax && gx <= ax + (n->w > 1 ? n->w - 1 : 0) && gz <= az && gz >= az - (n->h > 1 ? n->h - 1 : 0);
}

int PortRouteMap_PushFrom(const PortMap *m, int i, int dir, int *ox, int *oz) {
    static const int kDx[4] = {0, 0, -1, 1}, kDz[4] = {-1, 1, 0, 0};
    static unsigned short sQ[PORT_MAP_N * PORT_MAP_N];
    static unsigned char sSeen[PORT_MAP_N][PORT_MAP_N];
    const PortMapNpc *n = &m->npc[i];
    int w = NpcW(n), h = NpcH(n), head = 0, tail = 0, d, j;
    int fx = kDx[dir] < 0 ? n->x + w : kDx[dir] > 0 ? n->x - 1 : n->x;   // the far side's first tile
    int fz = kDz[dir] > 0 ? n->z - h : kDz[dir] < 0 ? n->z + 1 : n->z;
    int sx = kDx[dir] != 0 ? 0 : 1, sz = kDx[dir] != 0 ? -1 : 0;           // along the far side: x .. x + w - 1, z .. z - h + 1
    int len = kDx[dir] != 0 ? h : w;

    *ox = fx;
    *oz = fz;
    if (m->px - m->x0 < 0 || m->pz - m->z0 < 0 || m->px - m->x0 >= PORT_MAP_N || m->pz - m->z0 >= PORT_MAP_N) {
        return 0;
    }
    memset(sSeen, 0, sizeof sSeen);
    sQ[tail++] = (unsigned short)((m->pz - m->z0) * PORT_MAP_N + (m->px - m->x0));
    sSeen[m->pz - m->z0][m->px - m->x0] = 1;
    while (head < tail) {
        int cx = sQ[head] % PORT_MAP_N, cz = sQ[head] / PORT_MAP_N;

        head++;
        for (j = 0; j < len; j++) {
            if (m->x0 + cx == fx + sx * j && m->z0 + cz == fz + sz * j) {
                *ox = m->x0 + cx;
                *oz = m->z0 + cz;
                return 1;
            }
        }
        for (d = 0; d < 4; d++) {
            int nx = cx + kDx[d], nz = cz + kDz[d], k, under = 0;

            if (nx < 0 || nz < 0 || nx >= PORT_MAP_N || nz >= PORT_MAP_N || sSeen[nz][nx] || !(m->cell[cz][cx].pass & (1 << d))) {
                continue;
            }
            for (k = 0; k < m->nnpc && !under; k++) {
                under = !m->npc[k].rail && NpcOver(&m->npc[k], m->npc[k].x, m->npc[k].z, m->x0 + nx, m->z0 + nz) &&
                        NpcBlocks(m, &m->npc[k], m->x0 + nx, m->z0 + nz, m->npc[k].y);
            }
            if (!under) {
                sSeen[nz][nx] = 1;
                sQ[tail++] = (unsigned short)(nz * PORT_MAP_N + nx);
            }
        }
    }
    return 0;
}

static char Overlay(const PortMap *m, int gx, int gz, int *npcId) {
    int i, other = -1;

    if (gx == m->px && gz == m->pz) {
        return '@';
    }
    for (i = 0; i < m->nnpc; i++) {
        if (!m->npc[i].rail && NpcOver(&m->npc[i], m->npc[i].x, m->npc[i].z, gx, gz)) {
            if (!NpcBlocks(m, &m->npc[i], gx, gz, m->npc[i].y)) {
                other = i;
                continue;
            }
            *npcId = m->npc[i].id;
            return 'N';
        }
    }
    if (other >= 0) {
        *npcId = m->npc[other].id;
        return 'n';
    }
    for (i = 0; i < m->nwarp; i++) {
        const PortMapWarp *w = &m->warp[i];

        if (!w->rail && gx >= w->x && gx < w->x + (w->w ? w->w : 1) && gz >= w->z && gz < w->z + (w->h ? w->h : 1)) {
            return 'D';
        }
    }
    for (i = 0; i < m->ntrig; i++) {
        const PortMapTrig *t = &m->trig[i];

        if (!t->rail && gx >= t->x && gx < t->x + t->w && gz >= t->z && gz < t->z + t->h) {
            return 'T';
        }
    }
    for (i = 0; i < m->nbg; i++) {
        if (!m->bg[i].rail && m->bg[i].x == gx && m->bg[i].z == gz) {
            return 'b';
        }
    }
    return 0;
}

static const char *WarpType(int t) {
    static const char *names[] = {"none", "mat", "walk-in-2", "door", "walk-in", "warp-pad", "gate", "mat-key"};

    return t >= 0 && t < 8 ? names[t] : "camera";
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

// The short dump (the default; BWPORT_MAPDUMP_FULL=1 writes the full 64 x 64 and every entry): the grid window that covers the
// player, the targets (the G tokens still to run: set_targets from the pilot) and the warps, triggers, BG events and actors
// within SHORT_NEAR tiles of them, padded by SHORT_PAD; rows with no terrain loaded are one line; a list shows the entries in
// the window (rail ones: on the lines near the player or a target) and the others on a line of ids.
static const char *sTargets;

void PortRouteMap_SetTargets(const char *rest) {
    sTargets = rest;
}

#define SHORT_NEAR 8
#define SHORT_PAD 3
#define SHORT_LIST 12
#define SHORT_TARGETS 6

typedef struct {
    int full;                 // the 64 x 64 and every entry
    int x0, z0, x1, z1;       // the grid window (tiles, inclusive)
    int cx, cz;               // the player's tile (on a rail: where the rail place is in the world)
    int nt;
    struct {
        int x0, z0, x1, z1;
    } t[SHORT_TARGETS * 8];   // the targets' tiles
    unsigned char line[128];  // the rail lines of the player and the targets
} Win;

static void AddT(Win *w, int x, int z, int bw, int bh) {
    if (w->nt < (int)(sizeof w->t / sizeof w->t[0])) {
        w->t[w->nt].x0 = x;
        w->t[w->nt].z0 = z;
        w->t[w->nt].x1 = x + (bw > 1 ? bw : 1) - 1;
        w->t[w->nt].z1 = z + (bh > 1 ? bh : 1) - 1;
        w->nt++;
    }
}

static void AddLine(Win *w, int line) {
    if (line >= 0 && line < 128) {
        w->line[line] = 1;
    }
}

// warp i is the goal's: G:warp:N any warp to zone N, G:door:I warp I of the zone
static int WarpWanted(const PortMap *m, const PortRouteGoal *g, int i) {
    return g->door ? i == g->n : (int)m->warp[i].zone == g->n;
}

// the G tokens in rest, as tiles (or rail lines) in the window's target list
static void FindTargets(const PortMap *m, Win *w, const char *rest) {
    int n = 0, i;

    while (rest != NULL && n < SHORT_TARGETS) {
        PortRouteGoal g;
        char why[32];
        const char *e;

        while (*rest == ' ') {
            rest++;
        }
        if (*rest == '\0') {
            break;
        }
        e = strchr(rest, ' ');
        if (rest[0] == 'G' && PortRoute_ParseGoal(rest, &g, why, sizeof why)) {
            n++;
            switch (g.kind) {
            case GOAL_TILE:
                AddT(w, g.x, g.z, 1, 1);
                break;
            case GOAL_WARP:
                for (i = 0; i < m->nwarp; i++) {
                    if (WarpWanted(m, &g, i)) {
                        if (m->warp[i].rail) {
                            AddLine(w, m->warp[i].line);
                        } else {
                            AddT(w, m->warp[i].x, m->warp[i].z, m->warp[i].w, m->warp[i].h);
                        }
                    }
                }
                break;
            case GOAL_NPC:
            case GOAL_ITEM:
            case GOAL_WALL:
                for (i = 0; i < m->nnpc; i++) {
                    if (m->npc[i].id == g.n) {
                        if (m->npc[i].rail) {
                            AddLine(w, m->npc[i].line);
                        } else {
                            AddT(w, m->npc[i].x, m->npc[i].z - NpcH(&m->npc[i]) + 1, NpcW(&m->npc[i]), NpcH(&m->npc[i]));
                        }
                    }
                }
                break;
            case GOAL_TRIG:
                if (g.n >= 0 && g.n < m->ntrig) {
                    if (m->trig[g.n].rail) {
                        AddLine(w, m->trig[g.n].line);
                    } else {
                        AddT(w, m->trig[g.n].x, m->trig[g.n].z, m->trig[g.n].w, m->trig[g.n].h);
                    }
                }
                break;
            case GOAL_BG:
                if (g.n >= 0 && g.n < m->nbg) {
                    if (m->bg[g.n].rail) {
                        AddLine(w, m->bg[g.n].line);
                    } else {
                        AddT(w, m->bg[g.n].x, m->bg[g.n].z, 1, 1);
                    }
                }
                break;
            case GOAL_RAIL:
                AddLine(w, g.n);
                break;
            }
        }
        rest = e != NULL ? e : rest + strlen(rest);
    }
}

static int Gap(int a0, int a1, int b0, int b1) {
    return a0 > b1 ? a0 - b1 : b0 > a1 ? b0 - a1 : 0;
}

static int BoxEnd(int x, int span) {
    return x + (span > 1 ? span : 1) - 1;
}

// a box within SHORT_NEAR tiles of the player or a target
static int NearBox(const Win *w, int x, int z, int bw, int bh) {
    int i;

    if (Gap(x, BoxEnd(x, bw), w->cx, w->cx) <= SHORT_NEAR && Gap(z, BoxEnd(z, bh), w->cz, w->cz) <= SHORT_NEAR) {
        return 1;
    }
    for (i = 0; i < w->nt; i++) {
        if (Gap(x, BoxEnd(x, bw), w->t[i].x0, w->t[i].x1) <= SHORT_NEAR &&
            Gap(z, BoxEnd(z, bh), w->t[i].z0, w->t[i].z1) <= SHORT_NEAR) {
            return 1;
        }
    }
    return 0;
}

static void Grow(Win *w, int x, int z, int bw, int bh) {
    w->x0 = x < w->x0 ? x : w->x0;
    w->z0 = z < w->z0 ? z : w->z0;
    w->x1 = BoxEnd(x, bw) > w->x1 ? BoxEnd(x, bw) : w->x1;
    w->z1 = BoxEnd(z, bh) > w->z1 ? BoxEnd(z, bh) : w->z1;
}

static int Loaded(const PortMapCell *c) {
    return c->kind != '?' && c->kind != ' ';
}

static void MakeWin(const PortMap *m, Win *w, int full) {
    int i, x, z, lx0 = 1 << 30, lz0 = 1 << 30, lx1 = -(1 << 30), lz1 = -(1 << 30);

    memset(w, 0, sizeof *w);
    w->full = full;
    w->cx = m->px;
    w->cz = m->pz;
    if (m->rail && m->wx >= m->x0 && m->wx < m->x0 + PORT_MAP_N && m->wz >= m->z0 && m->wz < m->z0 + PORT_MAP_N) {
        w->cx = m->wx;   // the grid position the actor keeps is stale while it is on a rail
        w->cz = m->wz;
    }
    if (m->rail) {
        AddLine(w, m->rpos.line);
    }
    if (full) {
        w->x0 = w->cx - 32;
        w->z0 = w->cz - 32;
        w->x1 = w->x0 + 63;
        w->z1 = w->z0 + 63;
        return;
    }
    FindTargets(m, w, sTargets);
    w->x0 = w->x1 = w->cx;
    w->z0 = w->z1 = w->cz;
    for (i = 0; i < w->nt; i++) {
        Grow(w, w->t[i].x0, w->t[i].z0, w->t[i].x1 - w->t[i].x0 + 1, w->t[i].z1 - w->t[i].z0 + 1);
    }
    for (i = 0; i < m->nwarp; i++) {
        if (!m->warp[i].rail && NearBox(w, m->warp[i].x, m->warp[i].z, m->warp[i].w, m->warp[i].h)) {
            Grow(w, m->warp[i].x, m->warp[i].z, m->warp[i].w, m->warp[i].h);
        }
    }
    for (i = 0; i < m->ntrig; i++) {
        if (!m->trig[i].rail && NearBox(w, m->trig[i].x, m->trig[i].z, m->trig[i].w, m->trig[i].h)) {
            Grow(w, m->trig[i].x, m->trig[i].z, m->trig[i].w, m->trig[i].h);
        }
    }
    for (i = 0; i < m->nbg; i++) {
        if (!m->bg[i].rail && NearBox(w, m->bg[i].x, m->bg[i].z, 1, 1)) {
            Grow(w, m->bg[i].x, m->bg[i].z, 1, 1);
        }
    }
    for (i = 0; i < m->nnpc; i++) {
        const PortMapNpc *n = &m->npc[i];

        if (!n->rail && NearBox(w, n->x, n->z - NpcH(n) + 1, NpcW(n), NpcH(n))) {
            Grow(w, n->x, n->z - NpcH(n) + 1, NpcW(n), NpcH(n));
        }
    }
    w->x0 -= SHORT_PAD;
    w->z0 -= SHORT_PAD;
    w->x1 += SHORT_PAD;
    w->z1 += SHORT_PAD;
    for (z = 0; z < PORT_MAP_N; z++) {   // clipped to the terrain that is loaded
        for (x = 0; x < PORT_MAP_N; x++) {
            if (Loaded(&m->cell[z][x])) {
                lx0 = m->x0 + x < lx0 ? m->x0 + x : lx0;
                lx1 = m->x0 + x > lx1 ? m->x0 + x : lx1;
                lz0 = m->z0 + z < lz0 ? m->z0 + z : lz0;
                lz1 = m->z0 + z > lz1 ? m->z0 + z : lz1;
            }
        }
    }
    if (lx0 <= lx1) {
        if (w->cx < lx0 || w->cx > lx1 || w->cz < lz0 || w->cz > lz1) {   // the player is off the terrain: all of it
            w->x0 = lx0;
            w->x1 = lx1;
            w->z0 = lz0;
            w->z1 = lz1;
        }
        w->x0 = w->x0 < lx0 ? lx0 : w->x0;
        w->x1 = w->x1 > lx1 ? lx1 : w->x1;
        w->z0 = w->z0 < lz0 ? lz0 : w->z0;
        w->z1 = w->z1 > lz1 ? lz1 : w->z1;
    }
    if (w->x1 - w->x0 > 63) {   // never wider than the full dump: around the player
        w->x0 = w->cx - 32;
        w->x1 = w->x0 + 63;
    }
    if (w->z1 - w->z0 > 63) {
        w->z0 = w->cz - 32;
        w->z1 = w->z0 + 63;
    }
}

// is an entry of a list in the window (a rail one: on a line near the player or a target)
static int Shown(const Win *w, int rail, int line, int x, int z, int bw, int bh) {
    if (w->full) {
        return 1;
    }
    if (rail) {
        return line >= 0 && line < 128 && w->line[line];
    }
    return BoxEnd(x, bw) >= w->x0 && x <= w->x1 && BoxEnd(z, bh) >= w->z0 && z <= w->z1;
}

// the first SHORT_LIST of the entries in the window are shown (sh[i] 1); returns how many
static int Mark(int full, unsigned char *sh, int n) {
    int i, k = 0;

    for (i = 0; i < n; i++) {
        if (sh[i] && !full && k >= SHORT_LIST) {
            sh[i] = 0;
        }
        k += sh[i];
    }
    return k;
}

static void ListHead(FILE *f, const char *name, int n, int shown) {
    if (shown < n) {
        fprintf(f, "%s (%d, %d shown):\n", name, n, shown);
    } else {
        fprintf(f, "%s (%d):\n", name, n);
    }
}

// the entries a short list leaves out, on one line: fmt gets the index (or v[i]) and w[i]
static void Others(FILE *f, int n, int shown, const unsigned char *sh, const char *fmt, const int *v, const int *w) {
    int i, k = 0;

    if (shown >= n) {
        return;
    }
    fprintf(f, "  not shown:");
    for (i = 0; i < n; i++) {
        if (!sh[i]) {
            if (++k > 12) {
                fprintf(f, " ... (BWPORT_MAPDUMP_FULL=1 lists all)");
                break;
            }
            fprintf(f, fmt, v != NULL ? v[i] : i, w != NULL ? w[i] : 0);
        }
    }
    fprintf(f, "\n");
}

// the grid: the window's rows, the x on top at every 8th tile; the short dump's rows with no terrain loaded are one line
static void PrintGrid(FILE *f, const PortMap *m, const Win *w) {
    char lab[256], row[256];
    int x, z, run = -1, n = w->x1 - w->x0 + 1, labelled = 0;

    memset(lab, ' ', sizeof lab);
    for (x = w->x0; x <= w->x1; x++) {
        char t[16];
        int k = snprintf(t, sizeof t, "%d", x);

        if (x % 8 == 0 && x - w->x0 + k < (int)sizeof lab - 1) {
            memcpy(lab + (x - w->x0), t, (size_t)k);
        }
    }
    lab[n < (int)sizeof lab - 1 ? n + 4 : (int)sizeof lab - 1] = '\0';
    while (lab[0] != '\0' && lab[strlen(lab) - 1] == ' ') {
        lab[strlen(lab) - 1] = '\0';
    }
    for (z = w->z0; z <= w->z1 + 1; z++) {
        int any = 0;

        if (z <= w->z1) {
            for (x = w->x0; x <= w->x1 && x - w->x0 < (int)sizeof row - 1; x++) {
                int cx = x - m->x0, cz = z - m->z0, id = -1, in = cx >= 0 && cz >= 0 && cx < PORT_MAP_N && cz < PORT_MAP_N;
                char o = Overlay(m, x, z, &id), k = in ? m->cell[cz][cx].kind : ' ';

                row[x - w->x0] = o                                                                    ? o
                                 : in && (m->cell[cz][cx].flags & PORT_MAP_BIKESLOPE)            ? 'M'
                                 : in && (m->cell[cz][cx].flags & (PORT_MAP_CLIMBNS | PORT_MAP_CLIMBEW)) ? 'C'   // a Rock Climb wall (F:climb)
                                 : in && (m->cell[cz][cx].flags & PORT_MAP_ELECTRIC) && k == '#' ? 'E'
                                 : in && (m->cell[cz][cx].flags & PORT_MAP_HOLE) && k == '#'     ? 'O'
                                 : in && (m->cell[cz][cx].flags & PORT_MAP_HOLE)                 ? 'o'
                                 : in && (m->cell[cz][cx].flags & PORT_MAP_SWITCH) && k == '.' ? '='
                                 : in && m->cell[cz][cx].slide && (k == '.' || k == ',')
                                     ? (m->cell[cz][cx].slide == PORT_MAP_ICE ? 'i' : m->cell[cz][cx].slide >= PORT_MAP_CANNON ? 'c'
                                        : m->cell[cz][cx].slide >= PORT_MAP_PAD ? 'p' : 's')
                                     : k;
                any |= in && Loaded(&m->cell[cz][cx]);
            }
            row[n < (int)sizeof row - 1 ? n : (int)sizeof row - 1] = '\0';
        }
        if (!w->full && z <= w->z1 && !any) {   // not loaded: collapsed into a run
            run = run < 0 ? z : run;
            continue;
        }
        if (!labelled && (run >= 0 || z <= w->z1)) {
            fprintf(f, "      %s\n", lab);
            labelled = 1;
        }
        if (run >= 0) {
            if (run == z - 1) {
                fprintf(f, "%5d row not loaded\n", run);
            } else {
                fprintf(f, "%5d..%d rows not loaded\n", run, z - 1);
            }
            run = -1;
        }
        if (z <= w->z1) {
            fprintf(f, "%5d %s\n", z, row);
        }
    }
}

int PortRouteMap_Dump(const char *game, const char *why, unsigned vblank) {
    static int sCount;
    const PortMap *m = PortRouteMap_Get();
    const char *fe = getenv("BWPORT_MAPDUMP_FULL");
    char dir[512], path[700];
    FILE *f;
    Win win;
    unsigned char sh[64];
    int dz[64];
    int x, z, i, k, n;

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
    MakeWin(m, &win, fe != NULL && *fe != '\0' && *fe != '0');
    fprintf(f, "map: %s zone %u player %d,%d y %d facing %d (V-blank %u, %s) matrix %dx%d chunks; tiles %d..%d x %d..%d (%s)",
            game, m->zone, m->px, m->pz, m->py, m->pdir, vblank, why, m->mw, m->mh, win.x0, win.x1, win.z0, win.z1,
            sAdapter->engine);
    if (m->rail && (win.cx != m->px || win.cz != m->pz)) {
        fprintf(f, ", on a rail: at world tile %d,%d", win.cx, win.cz);
    }
    fprintf(f, "%s\n", win.full ? "" : " [short: BWPORT_MAPDUMP_FULL=1 for the 64 x 64 and every entry]");
    PrintGrid(f, m, &win);
    if (m->rail) {
        fprintf(f, "rail: the player is at rail[%d,%d,%d] (%s, rail dir %d)\n", m->rpos.line, m->rpos.front, m->rpos.side,
                m->rpos.isLine ? "a line" : "a point", m->rpos.dir);
        for (i = 0; i < 4 && sAdapter->railToGrid != NULL; i++) {
            int h = sAdapter->railToGrid(&m->rpos, i);

            if (h) {
                fprintf(f, "  hand-off: dir %d here steps onto the grid%s\n", i, h == 2 ? " (maybe: the grid is not loaded)" : "");
            }
        }
    }
    if (!m->rail && sAdapter->gridToRail != NULL) {   // a hybrid zone's ways onto its rails from the '=' tiles
        n = 0;
        for (z = win.z0; z <= win.z1 && (win.full || n < 12); z++) {
            for (x = win.x0; x <= win.x1 && (win.full || n < 12); x++) {
                int cx = x - m->x0, cz = z - m->z0, d;
                PortRailPos to;

                if (cx < 0 || cz < 0 || cx >= PORT_MAP_N || cz >= PORT_MAP_N) {
                    continue;
                }
                for (d = 0; d < 4 && (m->cell[cz][cx].flags & PORT_MAP_SWITCH); d++) {
                    if (sAdapter->gridToRail(x, z, d, &to)) {
                        fprintf(f, "%s %d,%d dir %d -> rail[%d,%d,%d]", n % 4 == 0 ? (n ? "\n " : "hand-offs (grid -> rail):\n ") : "",
                                x, z, d, to.line, to.front, to.side);
                        n++;
                    }
                }
            }
        }
        if (m->nlever > 0 || m->nplace > 0) {   // the levers and the scripted hand-offs (the planner flips the levers it needs)
            fprintf(f, "%sswitches now 0x%x; levers:", n ? "\n" : "", m->sw);
            for (i = 0; i < m->nlever; i++) {
                fprintf(f, " %d,%d s%d", m->lever[i].x, m->lever[i].z, m->lever[i].s);
            }
            fprintf(f, "; places (onto a rail walking in dir when switch s is want):");
            for (i = 0; i < m->nplace; i++) {
                fprintf(f, " %d,%d dir %d s%d=%d -> rail[%d,%d,%d]", m->place[i].x, m->place[i].z, m->place[i].dir, m->place[i].s,
                        m->place[i].want, m->place[i].to.line, m->place[i].to.front, m->place[i].to.side);
            }
            n = 1;
        }
        if (n) {
            fprintf(f, "%s\n", !win.full && n >= 12 ? "\n  (the first 12 in the window; BWPORT_MAPDUMP_FULL=1 lists all)" : "");
        }
    }
    if (m->nline > 0) {   // which line joins which: the lines that share a point
        n = win.full || m->nline <= 24 ? m->nline : 24;
        fprintf(f, "rail lines (%d; line: point - point, length in tiles):\n", m->nline);
        for (i = 0; i < n; i++) {
            fprintf(f, "%s L%d: %d-%d %d", i % 6 == 0 ? " " : "", i, m->line[i].a, m->line[i].b, m->line[i].len);
            if (i % 6 == 5 || i == n - 1) {
                fprintf(f, "\n");
            }
        }
        if (n < m->nline) {
            fprintf(f, "  (%d more: BWPORT_MAPDUMP_FULL=1)\n", m->nline - n);
        }
    }
    for (i = 0; i < m->nwarp; i++) {
        const PortMapWarp *w = &m->warp[i];

        sh[i] = (unsigned char)Shown(&win, w->rail, w->line, w->x, w->z, w->w, w->h);
        dz[i] = (int)w->zone;
    }
    k = Mark(win.full, sh, m->nwarp);
    ListHead(f, "warps", m->nwarp, k);
    for (i = 0; i < m->nwarp; i++) {
        const PortMapWarp *w = &m->warp[i];

        if (!sh[i]) {
            continue;
        }
        if (w->rail) {
            fprintf(f, "  D%d rail[%d,%d,%d] %dx%d -> zone %u warp %u (%s, arrive facing %d)\n", i, w->line, w->x, w->z, w->w,
                    w->h, w->zone, w->dest, WarpType(w->type), w->dir - 1);
            continue;
        }
        fprintf(f, "  D%d %d,%d %dx%d -> zone %u warp %u (%s, arrive facing %d)\n", i, w->x, w->z, w->w, w->h, w->zone,
                w->dest, WarpType(w->type), w->dir - 1);
    }
    Others(f, m->nwarp, k, sh, " D%d>z%d", NULL, dz);
    for (i = 0; i < m->ntrig; i++) {
        const PortMapTrig *t = &m->trig[i];

        sh[i] = (unsigned char)Shown(&win, t->rail, t->line, t->x, t->z, t->w, t->h);
    }
    k = Mark(win.full, sh, m->ntrig);
    ListHead(f, "triggers", m->ntrig, k);
    for (i = 0; i < m->ntrig; i++) {
        const PortMapTrig *t = &m->trig[i];

        if (!sh[i]) {
            continue;
        }
        if (t->rail) {
            fprintf(f, "  T%d rail[%d,%d,%d] %dx%d script %u (work 0x%x = %u, type %u)\n", i, t->line, t->x, t->z, t->w, t->h,
                    t->script, t->workId, t->workValue, t->type);
            continue;
        }
        fprintf(f, "  T%d %d,%d %dx%d script %u (work 0x%x = %u, type %u)%s\n", i, t->x, t->z, t->w, t->h, t->script,
                t->workId, t->workValue, t->type, t->avoid ? " live: walked round" : "");
    }
    Others(f, m->ntrig, k, sh, " T%d", NULL, NULL);
    for (i = 0; i < m->nbg; i++) {
        sh[i] = (unsigned char)Shown(&win, m->bg[i].rail, m->bg[i].line, m->bg[i].x, m->bg[i].z, 1, 1);
    }
    k = Mark(win.full, sh, m->nbg);
    ListHead(f, "bg events", m->nbg, k);
    for (i = 0; i < m->nbg; i++) {
        if (!sh[i]) {
            continue;
        }
        if (m->bg[i].rail) {
            fprintf(f, "  b%d rail[%d,%d,%d] script %u dir %u\n", i, m->bg[i].line, m->bg[i].x, m->bg[i].z, m->bg[i].script,
                    m->bg[i].dir);
            continue;
        }
        fprintf(f, "  b%d %d,%d script %u dir %u\n", i, m->bg[i].x, m->bg[i].z, m->bg[i].script, m->bg[i].dir);
    }
    Others(f, m->nbg, k, sh, " b%d", NULL, NULL);
    for (i = 0; i < m->nnpc; i++) {   // every live actor (pz1: a gym's actors on other girders were cut off at 12)
        sh[i] = 1;
        dz[i] = m->npc[i].id;
    }
    k = m->nnpc;
    ListHead(f, "actors", m->nnpc, k);
    for (i = 0; i < m->nnpc; i++) {
        const PortMapNpc *n2 = &m->npc[i];

        if (!sh[i]) {
            continue;
        }
        if (n2->rail) {
            fprintf(f, "  N uid %d at rail[%d,%d,%d] code %u move %u\n", n2->id, n2->line, n2->x, n2->z, n2->code, n2->move);
            continue;
        }
        if (n2->layer) {
            fprintf(f, "  N uid %d at %d,%d y %d (from %d,%d) code %u move %u%s%s\n", n2->id, n2->x, n2->z, n2->y, n2->x0,
                    n2->z0, n2->code, n2->move, NpcSize(n2), NpcBlocks(m, n2, n2->x, n2->z, n2->y) ? "" : " other layer");
            continue;
        }
        fprintf(f, "  N uid %d at %d,%d (from %d,%d) code %u move %u%s", n2->id, n2->x, n2->z, n2->x0, n2->z0, n2->code,
                n2->move, NpcSize(n2));
        if (n2->look != 0) {   // a trainer who sees: its facing and sight range (route_test.c waits at the edge of a watched line)
            fprintf(f, " sees %s %d tiles", n2->look == 15 ? "all ways" : n2->face == 0 ? "up" : n2->face == 1 ? "down" : n2->face == 2 ? "left" : "right", n2->sight);
        }
        fprintf(f, "\n");
    }
    Others(f, m->nnpc, k, sh, " uid %d", dz, NULL);
    if (sAdapter->zoneAt != NULL) {
        int cx = win.cx / 32, cz = win.cz / 32, zr = win.full ? 3 : 1;

        fprintf(f, "zones (the matrix's chunks of 32x32 tiles; the player's in []):\n");
        for (z = cz - zr; z <= cz + zr; z++) {
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
    {
        // BWPORT_MAPDUMP_H=x0,z0,x1,z1: a "heights" section, the height in tiles of each cell in that window (what the pass bits
        // compare: a step of 20 or more is blocked; a raised ledge or a log shows here when the grid shows only '.')
        const char *hw = getenv("BWPORT_MAPDUMP_H");
        int hx0, hz0, hx1, hz1;

        if (hw != NULL && sscanf(hw, "%d,%d,%d,%d", &hx0, &hz0, &hx1, &hz1) == 4) {
            fprintf(f, "heights (x %d..%d, z %d..%d; the height in 1/16 tiles, then the cell's kind):\n      ", hx0, hx1, hz0, hz1);
            for (x = hx0; x <= hx1; x++) {
                fprintf(f, "%5d", x);
            }
            fprintf(f, "\n");
            for (z = hz0; z <= hz1; z++) {
                fprintf(f, "%5d ", z);
                for (x = hx0; x <= hx1; x++) {
                    int cx = x - m->x0, cz = z - m->z0;

                    if (cx < 0 || cz < 0 || cx >= PORT_MAP_N || cz >= PORT_MAP_N || m->cell[cz][cx].cls == 0xff) {
                        fprintf(f, "    ?");
                    } else {
                        fprintf(f, "%4d%c", m->cell[cz][cx].yf, m->cell[cz][cx].kind);
                    }
                }
                fprintf(f, "\n");
            }
            fprintf(f, "classes (tile class hex, pass bits u d l r as a digit 0-15):\n      ");
            for (x = hx0; x <= hx1; x++) {
                fprintf(f, "%7d", x);
            }
            fprintf(f, "\n");
            for (z = hz0; z <= hz1; z++) {
                fprintf(f, "%5d ", z);
                for (x = hx0; x <= hx1; x++) {
                    int cx = x - m->x0, cz = z - m->z0;

                    if (cx < 0 || cz < 0 || cx >= PORT_MAP_N || cz >= PORT_MAP_N || m->cell[cz][cx].cls == 0xff) {
                        fprintf(f, "      ?");
                    } else {
                        fprintf(f, " %04x/%x%c", m->cell[cz][cx].cls, m->cell[cz][cx].pass & 15, m->cell[cz][cx].kind);
                    }
                }
                fprintf(f, "\n");
            }
        }
    }
    PortRouteStory_Print(f, game, !win.full);   // the story section (port/src/route_story.c)
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
        int kind, talk, door;
    } kWords[] = {{"warp:", GOAL_WARP, 0}, {"npc:", GOAL_NPC, 1}, {"item:", GOAL_ITEM, 1}, {"trig:", GOAL_TRIG, 0},
                  {"bg:", GOAL_BG, 1}, {"wall:", GOAL_WALL, 0}, {"ride:", GOAL_RIDE, 0}, {"door:", GOAL_WARP, 0, 1}};
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
    if (end - p > 5 && strncmp(p, "rail:", 5) == 0) {   // G:rail:L,F,S
        p += 5;
        g->kind = GOAL_RAIL;
        if (!Num(&p, end, &g->n) || g->n < 0 || p >= end || *p != ',' || (p++, !Num(&p, end, &g->x)) || p >= end ||
            *p != ',' || (p++, !Num(&p, end, &g->z)) || p != end) {
            snprintf(why, (size_t)whyN, "%.*s: want G:rail:LINE,FRONT,SIDE", (int)(end - tok), tok);
            return 0;
        }
        return 1;
    }
    for (i = 0; i < sizeof kWords / sizeof kWords[0]; i++) {
        size_t n = strlen(kWords[i].word);

        if ((size_t)(end - p) >= n && strncmp(p, kWords[i].word, n) == 0) {
            p += n;
            g->kind = kWords[i].kind;
            g->talk = kWords[i].talk;
            g->door = kWords[i].door;
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
        snprintf(why, (size_t)whyN, "%.*s: want G:X,Z, G:warp:N, G:door:I, G:npc:N[:nota], G:item:N[:nota], G:trig:N, G:bg:N[:nota], G:wall:N or G:rail:L,F,S",
                 (int)(end - tok), tok);
        return 0;
    }
    return 1;
}

// ---- the planner -----------------------------------------------------------------------------------------------------

static const int kDx[4] = {0, 0, -1, 1}, kDz[4] = {-1, 1, 0, 0};

static int sSurf;   // the player is surfing (m->surf, set by PortRoutePlan_Next): the water is walkable

static int Walkable(char k) {
    return k == '.' || k == ',' || k == '?' || (k == '~' && sSurf);
}

// a hybrid zone (a grid and rails in one zone): the side the goal is on, 1 a rail, 0 the grid, -1 either or not known (a
// warp to the zone from both or neither, an actor not there)
static int GoalSide(const PortMap *m, const PortRouteGoal *g) {
    int i, grid = 0, rail = 0;

    switch (g->kind) {
    case GOAL_TILE:
        return 0;
    case GOAL_RAIL:
        return 1;
    case GOAL_TRIG:
        return g->n < m->ntrig ? m->trig[g->n].rail : -1;
    case GOAL_BG:
        return g->n < m->nbg ? m->bg[g->n].rail : -1;
    case GOAL_NPC:
    case GOAL_ITEM:
    case GOAL_WALL:
        for (i = 0; i < m->nnpc; i++) {
            if (m->npc[i].id == g->n) {
                return m->npc[i].rail;
            }
        }
        return -1;
    case GOAL_WARP:
        for (i = 0; i < m->nwarp; i++) {
            if (WarpWanted(m, g, i)) {
                rail |= m->warp[i].rail;
                grid |= !m->warp[i].rail;
            }
        }
        return rail == grid ? -1 : rail;
    default:
        return -1;
    }
}

static int LedgeDir(char k) {
    return k == '^' ? 0 : k == 'v' ? 1 : k == '<' ? 2 : k == '>' ? 3 : -1;
}

#define N PORT_MAP_N
static int sG[N * N], sF[N * N], sFrom[N * N];
static unsigned char sClosed[N * N], sBlock[N * N], sGoal[N * N];
static unsigned char sLever[N * N], sPlace[N * N];   // a lever's tile; a place's tile (its index + 1): PortMap's switches
static unsigned sSw;                                  // the switches' states the search plans with (PortMap's sw, or flipped)
static unsigned char sSlopeRun[N * N];   // the tile was reached by a bike slope run (PortRouteSlide_BikeRun): the fast gear's climb
static unsigned char sStep[N * N];   // the direction of the step that reached the tile, + 4 when it slid on (route_slide.c)
static int sHeap[N * N * 4], sHeapN;
// the tiles of the grid warps (a step onto one changes the zone, or presses nothing and the player stands on a stair: either
// way never a way across): the search costs a step onto one that is not a goal WARP_COST, so a path round it always wins and
// a warp is crossed only when it is the one way. The cost is carried by the tiles beyond the warp, not by the warp tile's own
// queue key: a warp tile the search only touches (a mat two tiles from the start, never popped before the goal) must queue as
// it did before, or equal-cost goals pop in another order (Platinum's lobbyfan ended a tile over).
// A live trigger the adapter marks (PortMapTrig.avoid: HeartGold's Ecruteak gym pits, Victory Road's holes) costs the same:
// walked round when there is a way round, crossed when it is the one way, a goal (G:trig:N, G:X,Z on it) as any tile.
#define WARP_COST 1000
static unsigned char sWarpT[N * N];

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
static int sRTmpN;   // the rail planner's (below)

void PortRoutePlan_Block(unsigned zone, int x, int z) {
    if (zone != sTmpZone) {
        sTmpN = 0;
        sRTmpN = 0;
        sTmpZone = zone;
    }
    sTmpX[sTmpN % 64] = x;
    sTmpZ[sTmpN % 64] = z;
    sTmpN++;
}

static int sPadFrom = -1;   // PadPlan's (below)

void PortRoutePlan_Reset(void) {
    sTmpN = 0;
    sRTmpN = 0;
    sPadFrom = -1;
}

// the player facings (bit d: kDx/kDz's d, the game's DIR_UP/DOWN/LEFT/RIGHT) that read BG event b: the field code's check
// (Black func_ov010_02162970, Black 2 CheckProxyEntityEvent): a hidden item from any side, else the event's direction 0 facing
// up, 1 right, 2 left, 3 down, 4 any, 5 left or right, 6 up or down (7 and on never fire: any side then, a try is all there is)
static int BgFacings(const PortMapBg *b) {
    static const unsigned char kFacing[7] = {1 << 0, 1 << 3, 1 << 2, 1 << 1, 15, 4 | 8, 1 | 2};

    return b->cond == 2 || b->dir >= 7 ? 15 : kFacing[b->dir];
}

// the goal tiles that face grid tile tx,tz (an actor, a BG event): next to it, or across a counter (the player talks over
// it); sGoal 3 + the direction that faces it. facings: the directions allowed (bit d). The number of goal tiles
static int FacingGoals(const PortMap *m, int tx, int tz, int facings) {
    int d, k, n = 0;

    for (d = 0; d < 4; d++) {
        if (!(facings & (1 << d))) {
            continue;
        }
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

// the tiles an actor is faced from: those next to its footprint (a 2x2 boulder: 8), none under it
static int NpcFacingGoals(const PortMap *m, const PortMapNpc *n) {
    int fx, fz, x, z, k = 0;

    for (fz = 0; fz < NpcH(n); fz++) {
        for (fx = 0; fx < NpcW(n); fx++) {
            FacingGoals(m, n->x + fx, n->z - fz, 15);
        }
    }
    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            if (sGoal[z * PORT_MAP_N + x] >= 3 && NpcOver(n, n->x, n->z, x + m->x0, z + m->z0)) {
                sGoal[z * PORT_MAP_N + x] = 0;
            }
            k += sGoal[z * PORT_MAP_N + x] >= 3;
        }
    }
    return k;
}

// the direction a walk into warp w must have (the field code's CheckWarpDirectionMatch, stepping onto its tile or pressing into
// it against a wall; the warp's dir is the facing arriving there, + 1), or -1 for any: a none, mat (the goal is the mat
// itself, pressing into the wall), warp-pad or gate warp (they fire on the tile), a warp with no direction
static int WarpWalkIn(const PortMapWarp *w) {
    return w->type == 0 || w->type == 1 || w->type == 5 || w->type == 6 || w->dir < 1 || w->dir > 4 ? -1 : (w->dir - 1) ^ 1;
}

// the goal tiles of warp w (every tile of it): on it (sGoal 2) or the tile from which a step in d enters it (3 + d); only:
// just that d (and not on it), or -1 for any. The number of goal tiles
static int WarpGoals(const PortMap *m, const PortMapWarp *w, int only) {
    int wx, wz, d, n = 0;

    for (wz = w->z; wz < w->z + (w->h ? w->h : 1); wz++) {
        for (wx = w->x; wx < w->x + (w->w ? w->w : 1); wx++) {
            int x = wx - m->x0, z = wz - m->z0;

            if (x < 0 || z < 0 || x >= N || z >= N) {
                continue;
            }
            if (only < 0 && Walkable(m->cell[z][x].kind)) {
                sGoal[z * N + x] = 2;   // on it
                n++;
            }
            for (d = 0; d < 4; d++) {
                int nx = x - kDx[d], nz = z - kDz[d];   // the tile from which a step in d enters the warp

                if ((only < 0 || only == d) && nx >= 0 && nz >= 0 && nx < N && nz < N && Walkable(m->cell[nz][nx].kind) &&
                    !sGoal[nz * N + nx]) {
                    sGoal[nz * N + nx] = (unsigned char)(3 + d);
                    n++;
                }
            }
        }
    }
    return n;
}

// ---- the rail planner (Gen 5's no-grid maps) --------------------------------------------------------------------------

// rail places the pilot found blocked (a step that did not happen, a warp that did not take): kept until the zone changes
static PortRailPos sRTmp[64];

void PortRoutePlan_BlockRail(unsigned zone, const PortRailPos *p) {
    if (zone != sTmpZone) {
        sTmpN = 0;
        sRTmpN = 0;
        sTmpZone = zone;
    }
    sRTmp[sRTmpN % 64] = *p;
    sRTmpN++;
}

// the same place (the game's direction value and line-or-point aside: the game's event checks compare the component only)
static int RailSame(const PortRailPos *a, int line, int front, int side) {
    return a->line == line && a->front == front && a->side == side;
}

// p inside an entry's span (eventdata_system.c's CheckWarpPositionMatchRail)
static int RailIn(const PortRailPos *p, int line, int x, int z, int w, int h) {
    return p->line == line && p->front >= x && p->front < x + (w ? w : 1) && p->side >= z && p->side < z + (h ? h : 1);
}

static int RailBlocked(const PortMap *m, const PortRailPos *p) {
    int i;

    for (i = 0; i < m->nnpc; i++) {
        if (m->npc[i].rail && RailSame(p, m->npc[i].line, m->npc[i].x, m->npc[i].z)) {
            return 1;
        }
    }
    for (i = 0; i < sRTmpN && i < 64; i++) {
        if (RailSame(p, sRTmp[i].line, sRTmp[i].front, sRTmp[i].side)) {
            return 1;
        }
    }
    return 0;
}

#define RN 8192
#define RH 16384
static PortRailPos sRPos[RN];
static short sRFrom[RN], sRDepth[RN];
static signed char sRStep[RN];   // the step's direction from sRFrom
static int sRHash[RH];   // a place's node + 1, 0 none

static unsigned RailKey(const PortRailPos *p) {
    return ((unsigned)p->line * 2u + (unsigned)p->isLine) * 40503u + (unsigned)p->front * 977u + (unsigned)(p->side + 64);
}

// the node of place p, added when new (*isNew); -1 when the table is full
static int RailNode(const PortRailPos *p, int from, int *isNew) {
    unsigned h = RailKey(p) % RH;
    static int sN;

    if (from < 0) {
        sN = 0;
        memset(sRHash, 0, sizeof sRHash);
    }
    *isNew = 0;
    for (;;) {
        int k = sRHash[h] - 1;

        if (k < 0) {
            break;
        }
        if (sRPos[k].line == p->line && sRPos[k].isLine == p->isLine && sRPos[k].front == p->front && sRPos[k].side == p->side) {
            return k;
        }
        h = (h + 1) % RH;
    }
    if (sN >= RN) {
        return -1;
    }
    sRPos[sN] = *p;
    sRFrom[sN] = (short)from;
    sRDepth[sN] = from < 0 ? 0 : (short)(sRDepth[from] + 1);
    sRHash[h] = sN + 1;
    *isNew = 1;
    return sN++;
}

// a goal at place p (dir -1) or by the step from p in dir that leads to *to: the direction to press there (dir, or for a
// warp under the player the way it was arrived facing, reversed), -1 nothing to press, -2 not a goal. strict: a warp only
// from the side it must be walked into (as the grid's WarpWalkIn)
static int RailGoal(const PortMap *m, const PortRouteGoal *g, const PortRailPos *p, int dir, const PortRailPos *to, int strict) {
    int i;

    switch (g->kind) {
    case GOAL_RAIL:
        return dir < 0 && RailSame(p, g->n, g->x, g->z) ? -1 : -2;
    case GOAL_TRIG:
        return dir < 0 && RailIn(p, m->trig[g->n].line, m->trig[g->n].x, m->trig[g->n].z, m->trig[g->n].w, m->trig[g->n].h)
                   ? -1
                   : -2;
    case GOAL_BG:
        return dir >= 0 && RailSame(to, m->bg[g->n].line, m->bg[g->n].x, m->bg[g->n].z) ? dir : -2;
    case GOAL_NPC:
    case GOAL_ITEM:
        for (i = 0; i < m->nnpc; i++) {
            if (m->npc[i].id == g->n && m->npc[i].rail) {
                return dir >= 0 && RailSame(to, m->npc[i].line, m->npc[i].x, m->npc[i].z) ? dir : -2;
            }
        }
        return -2;
    case GOAL_WARP:
        for (i = 0; i < m->nwarp; i++) {
            const PortMapWarp *w = &m->warp[i];
            int in = WarpWalkIn(w);

            if (!w->rail || !WarpWanted(m, g, i)) {
                continue;
            }
            if (dir < 0 && RailIn(p, w->line, w->x, w->z, w->w, w->h)) {   // on it: walk on the other way it was arrived facing
                return w->dir > 0 ? (w->dir - 1) ^ 1 : 1;
            }
            if (dir >= 0 && (!strict || in < 0 || in == dir) && RailIn(to, w->line, w->x, w->z, w->w, w->h)) {
                return dir;   // the step into it (the wall's warp: CheckRailWallWarpEvent)
            }
        }
        return -2;
    default:
        return -2;
    }
}

// the rail warps a step from one of the first `count` places of RailNode's table walks into (a search that found no goal:
// which doors it could take instead, " D2 zone 224 D8 zone 218"), " none" when none
static void RailDoorsReached(const PortMap *m, int count, char *buf, size_t size) {
    size_t n = 0;
    int i, c, d;

    buf[0] = '\0';
    for (i = 0; i < m->nwarp; i++) {
        PortRouteGoal g;
        int hit = 0;

        if (!m->warp[i].rail) {
            continue;
        }
        memset(&g, 0, sizeof g);
        g.kind = GOAL_WARP;
        g.door = 1;
        g.n = i;
        for (c = 0; c < count && !hit; c++) {
            PortRailPos p = sRPos[c], to;

            if (RailBlocked(m, &p)) {
                continue;
            }
            for (d = -1; d < 4 && !hit; d++) {
                to = p;
                if (d >= 0) {
                    sAdapter->railStep(&p, d, &to);
                }
                hit = RailGoal(m, &g, &p, d, &to, 0) != -2;
            }
        }
        if (hit && n + 16 < size) {
            n += (size_t)snprintf(buf + n, size - n, " D%d zone %u", i, m->warp[i].zone);
        }
    }
    if (n == 0) {
        snprintf(buf, size, " none");
    }
}

// PortRoutePlan_Next on a rail: a breadth-first search over the rail places by the game's own step check (the adapter's
// railStep); out->dir the first step's direction
static int RailPlan(const PortMap *m, const PortRouteGoal *g, PortRoutePlan *out, const char *gname, const PortRailPos *want,
                    int wantDir) {
    // a hand-off to the grid first: any (the nearest), or the one at place want pressing wantDir (the grid search's choice)
    int pass, i, isNew, toGrid = sAdapter->railToGrid != NULL && (want != NULL || GoalSide(m, g) == 0);

    if (sAdapter->railStep == NULL) {
        snprintf(out->why, sizeof out->why, "%s: on a rail and the game has no rail step check", gname);
        return PLAN_NONE;
    }
    if (toGrid) {   // the goal's own checks are the grid planner's, once there
    } else if (g->kind == GOAL_TILE) {
        snprintf(out->why, sizeof out->why, "%s: the player is on a rail at rail[%d,%d,%d]: G:rail:L,F,S, not a tile", gname,
                 m->rpos.line, m->rpos.front, m->rpos.side);
        return PLAN_NONE;
    } else if (g->kind == GOAL_TRIG && !m->trig[g->n].rail) {
        snprintf(out->why, sizeof out->why, "%s: the player is on a rail, the trigger is not", gname);
        return PLAN_NONE;
    } else if (g->kind == GOAL_BG && !m->bg[g->n].rail) {
        snprintf(out->why, sizeof out->why, "%s: the player is on a rail, the BG event is not", gname);
        return PLAN_NONE;
    } else if (g->kind == GOAL_NPC || g->kind == GOAL_ITEM) {
        for (i = 0; i < m->nnpc && m->npc[i].id != g->n; i++) {
        }
        if (!m->npc[i].rail) {
            snprintf(out->why, sizeof out->why, "%s: the player is on a rail, the actor is not", gname);
            return PLAN_NONE;
        }
    } else if (g->kind == GOAL_WARP) {
        char zones[160];
        size_t n = 0;

        zones[0] = '\0';
        for (i = 0; i < m->nwarp && !(m->warp[i].rail && WarpWanted(m, g, i)); i++) {
            if (m->warp[i].rail && n + 8 < sizeof zones) {
                n += (size_t)snprintf(zones + n, sizeof zones - n, " %u", m->warp[i].zone);
            }
        }
        if (i == m->nwarp) {
            snprintf(out->why, sizeof out->why, "%s: no rail warp %s %d in zone %u (its rail warps lead to:%s)", gname,
                     g->door ? "D" : "to zone", g->n, m->zone, n ? zones : " none");
            return PLAN_NONE;
        }
    }
    out->handoff = toGrid;
    for (pass = g->kind == GOAL_WARP && !toGrid ? 0 : 1; pass < 2; pass++) {   // a warp: from its walk-in side first, then any
        int head = 0, best = -1, press = -2, start = RailNode(&m->rpos, -1, &isNew), count = 1;

        while (head < count && best < 0) {
            int c = head++, d, r;
            PortRailPos p = sRPos[c], to;

            if (c != start && RailBlocked(m, &p)) {
                continue;
            }
            if ((c != start || !RailBlocked(m, &p)) && !toGrid && (r = RailGoal(m, g, &p, -1, &p, pass == 0)) != -2) {
                best = c;
                press = r;
                break;
            }
            for (d = 0; d < 4; d++) {
                int ok = sAdapter->railStep(&p, d, &to), k;

                if (toGrid) {   // the step that hands off to the grid (a switch place, the rail ahead blocked)
                    r = sAdapter->railToGrid(&p, d) && (want == NULL || (d == wantDir && RailSame(&p, want->line, want->front, want->side)))
                            ? d
                            : -2;
                } else {
                    r = RailGoal(m, g, &p, d, &to, pass == 0);
                }
                if (r != -2 && !RailBlocked(m, &p)) {
                    best = c;
                    press = r;
                    break;
                }
                if (!ok || RailBlocked(m, &to)) {
                    continue;
                }
                k = RailNode(&to, c, &isNew);
                if (k >= 0 && isNew) {
                    sRStep[k] = (signed char)d;
                    count = k + 1;
                }
            }
        }
        if (best < 0) {
            if (pass == 1) {
                char doors[120];

                RailDoorsReached(m, count, doors, sizeof doors);
                snprintf(out->why, sizeof out->why, "no rail path from rail[%d,%d,%d] to %s%s (%d places reached; rail warps reached lead to:%s)",
                         m->rpos.line, m->rpos.front, m->rpos.side, gname,
                         toGrid ? " (it is on the grid: no hand-off to the grid reached)" : "", count, doors);
                return PLAN_NONE;
            }
            continue;
        }
        out->press = press;
        out->x = m->px;
        out->z = m->pz;
        if (best == start) {
            return PLAN_THERE;
        }
        out->steps = sRDepth[best];
        out->final = sRDepth[best] == 1;
        for (i = best; sRFrom[i] != start; i = sRFrom[i]) {
        }
        out->rnext = sRPos[i];
        out->dir = sRStep[i];
        return PLAN_OK;
    }
    return PLAN_NONE;
}
#undef RN
#undef RH

// G:ride:N's goal: the start tile of the adapter's ride N, marked with the key to hold (sGoal 3 + dir). The planner's grid
// is the player's floor (the fill floods from the player's height), so a ride on another floor is "no path" at once
static int RideGoal(const PortMap *m, const PortRouteGoal *g, const char *gname, PortRoutePlan *out) {
    int x, z, y = m->py, dir, r = sAdapter->ride != NULL ? sAdapter->ride(m->zone, g->n, &x, &z, &y, &dir) : 0;

    if (r <= 0) {
        const char *w = r < 0 && sAdapter->rideWhy != NULL ? sAdapter->rideWhy(m->zone, g->n) : NULL;

        snprintf(out->why, sizeof out->why, "%s: %s", gname,
                 w != NULL ? w : r < 0 ? "the ride is closed (its switch is not done yet)" : "no such ride here (the adapter's ride table)");
        return 0;
    }
    if (m->py != y) {
        snprintf(out->why, sizeof out->why, "%s: the ride starts at %d,%d on floor y %d, the player is at y %d (another ride first)",
                 gname, x, z, y, m->py);
        return 0;
    }
    x -= m->x0;
    z -= m->z0;
    if (x < 0 || z < 0 || x >= N || z >= N) {
        snprintf(out->why, sizeof out->why, "%s: its start tile is outside the map window", gname);
        return 0;
    }
    sGoal[z * N + x] = (unsigned char)(3 + dir);
    sBlock[z * N + x] = 0;
    return 1;
}

static const char *GoalName(const PortRouteGoal *g, char *buf, size_t n) {
    static const char *const kNames[] = {"", "warp", "npc", "item", "trig", "bg", "rail", "wall", "ride"};

    if (g->kind == GOAL_TILE) {
        snprintf(buf, n, "%d,%d", g->x, g->z);
    } else if (g->kind == GOAL_RAIL) {
        snprintf(buf, n, "rail:%d,%d,%d", g->n, g->x, g->z);
    } else {
        snprintf(buf, n, "%s:%d", g->door ? "door" : kNames[g->kind], g->n);
    }
    return buf;
}

// BWPORT_WARP_HINTS (tools/route.py, from tools/atlas.py): "Z=S:x,z;..." the grid tile x,z of a warp into zone Z that the
// event data of zone S holds (outdoors the warps are in world tiles). Sets *hx, *hz to the one nearest the player (Manhattan)
// whose tile the map matrix says is zone S (a tile of S's own interior is not): the far target of a G:warp:Z whose warp is
// not in the window. 1 when there is one.
static int WarpHint(int zone, const PortMap *m, int *hx, int *hz) {
    const char *p = getenv("BWPORT_WARP_HINTS");
    int bd = 1 << 30;

    while (p != NULL && *p != '\0') {
        char *e;
        long z = strtol(p, &e, 10), s, x, y;

        if (*e != '=') {
            break;
        }
        s = strtol(e + 1, &e, 10);
        if (*e != ':') {
            break;
        }
        x = strtol(e + 1, &e, 10);
        if (*e != ',') {
            break;
        }
        y = strtol(e + 1, &e, 10);
        if (z == zone && ZoneAt((int)x, (int)y) == (int)s) {
            int dd = abs((int)x - m->px) + abs((int)y - m->pz);

            if (dd < bd) {
                bd = dd;
                *hx = (int)x;
                *hz = (int)y;
            }
        }
        p = *e == ';' ? e + 1 : e;
    }
    return bd < 1 << 30;
}

// The tile of an actor / item / BG event / trigger goal when it lies beyond the window (or on its edge row: the tiles it is
// faced from are not in it): 1 with *fx, *fz, the far target of the walk (the window moves with the player: a long gym,
// Driftveil's, is crossed in hops). 0 when the goal is in the window or has no tile of its own.
static int FarGoal(const PortMap *m, const PortRouteGoal *goal, int *fx, int *fz) {
    int i, have = 0;

    if (goal->kind == GOAL_NPC || goal->kind == GOAL_ITEM) {
        for (i = 0; i < m->nnpc && !have; i++) {
            if (m->npc[i].id == goal->n && !m->npc[i].rail) {
                *fx = m->npc[i].x;
                *fz = m->npc[i].z;
                have = 1;
            }
        }
    } else if (goal->kind == GOAL_BG && goal->n < m->nbg && !m->bg[goal->n].rail) {
        *fx = m->bg[goal->n].x;
        *fz = m->bg[goal->n].z;
        have = 1;
    } else if (goal->kind == GOAL_TRIG && goal->n < m->ntrig && !m->trig[goal->n].rail) {
        *fx = m->trig[goal->n].x;
        *fz = m->trig[goal->n].z;
        have = 1;
    }
    if (have && (*fx - m->x0 < 1 || *fx - m->x0 >= PORT_MAP_N - 1 || *fz - m->z0 < 1 || *fz - m->z0 >= PORT_MAP_N - 1)) {
        return 1;
    }
    for (i = 0; have && i < 4; i++) {   // terrain around it not loaded yet (a chunk the mapper loads as the player comes near)
        if (m->cell[*fz - m->z0 + kDz[i]][*fx - m->x0 + kDx[i]].kind == '?') {
            return 1;
        }
    }
    return 0;
}

// ---- hand-offs in the grid search (a hybrid zone: grid -> rail -> grid chains, Icirrus and Opelucid Gyms) --------------------

#define HAND 8   // sStep bit: the tile was reached by a hand-off (the key pressed at the switch tile in the low bits), not a step
static int sHeurOn, sHeurX, sHeurZ;   // the search is toward a far target (the heuristic), not a goal tile
static int sSeedNode[N * N];          // a seed tile (a search that starts on a rail): its rail place's node (RailReach's) and key
static unsigned char sSeedDir[N * N];

static int Heur(const PortMap *m, int nx, int nz) {
    return sHeurOn ? 2 * (abs(m->x0 + nx - sHeurX) + abs(m->z0 + nz - sHeurZ)) : 0;
}

// the game has the hand-offs a chain needs (port_route_map.h: railToGrid, gridToRail, railLand, railStep)
static int Hybrid(void) {
    return sAdapter->gridToRail != NULL && sAdapter->railToGrid != NULL && sAdapter->railLand != NULL && sAdapter->railStep != NULL;
}

// the rail places reachable from *from by the adapter's railStep, breadth-first, in RailNode's table (sRPos / sRDepth; the
// first is *from, the actors and blocked places in the way): how many
static int RailReach(const PortMap *m, const PortRailPos *from) {
    int head = 0, count = 1, d, isNew;

    RailNode(from, -1, &isNew);
    while (head < count) {
        int c = head++;
        PortRailPos p = sRPos[c], to;

        for (d = 0; d < 4; d++) {
            int k;

            if (!sAdapter->railStep(&p, d, &to) || RailBlocked(m, &to)) {
                continue;
            }
            k = RailNode(&to, c, &isNew);
            if (k >= 0 && isNew) {
                count = k + 1;
            }
        }
    }
    return count;
}

// the rail places reachable from *from (RailReach's search) have the goal: a place of it, or a step into it (RailPlan's checks).
// A hybrid zone's switch tiles lead to different rails (the League hall's four lanes, one door each)
static int RailHasGoal(const PortMap *m, const PortRouteGoal *g, const PortRailPos *from) {
    int head = 0, count = 1, d, isNew;

    RailNode(from, -1, &isNew);
    while (head < count) {
        int c = head++;
        PortRailPos p = sRPos[c], to;

        if (!RailBlocked(m, &p) && RailGoal(m, g, &p, -1, &p, 0) != -2) {
            return 1;
        }
        for (d = 0; d < 4; d++) {
            int k, ok = sAdapter->railStep(&p, d, &to);

            if (!RailBlocked(m, &p) && RailGoal(m, g, &p, d, &to, 0) != -2) {
                return 1;
            }
            if (!ok || RailBlocked(m, &to)) {
                continue;
            }
            k = RailNode(&to, c, &isNew);
            if (k >= 0 && isNew) {
                count = k + 1;
            }
        }
    }
    return 0;
}

// the grid tiles the rail places of RailReach hand off onto (railToGrid: the key held toward the grid at a switch place;
// railLand: the tile), relaxed in the search at g0 + the rail steps: from grid tile `from` pressing pd there (a hand-off
// onto the rail and off it), or from = -1: the search starts on the rail (the seeds)
static void RailLandings(const PortMap *m, int count, int g0, int from, int pd) {
    int c, d;

    for (c = 0; c < count; c++) {
        PortRailPos p = sRPos[c];

        if (RailBlocked(m, &p)) {
            continue;
        }
        for (d = 0; d < 4; d++) {
            int lx, lz, nx, nz, cost, n;

            if (!sAdapter->railToGrid(&p, d) || !sAdapter->railLand(&p, d, &lx, &lz)) {
                continue;
            }
            nx = lx - m->x0;
            nz = lz - m->z0;
            if (nx < 0 || nz < 0 || nx >= N || nz >= N || sBlock[nz * N + nx] || !Walkable(m->cell[nz][nx].kind)) {
                continue;
            }
            cost = g0 + 2 * sRDepth[c] + 8;
            if (PortRouteSlide_End(m, sBlock, d, &nx, &nz)) {   // (ice where it lands: where it stops)
                cost += abs(nx - (lx - m->x0)) + abs(nz - (lz - m->z0));
            }
            n = nz * N + nx;
            if (sClosed[n] || cost >= sG[n]) {
                continue;
            }
            sG[n] = cost;
            sFrom[n] = from;
            sStep[n] = (unsigned char)((pd < 0 ? d : pd) | HAND);
            sSlopeRun[n] = 0;
            sF[n] = cost + Heur(m, nx, nz);
            if (from < 0) {
                sSeedNode[n] = c;
                sSeedDir[n] = (unsigned char)d;
            }
            if (sHeapN < N * N * 4) {
                HeapPush(n);
            }
        }
    }
}

// a step from cx,cz in d onto a PORT_MAP_BRIDGE cell whose second floor (yb) is the one at cx,cz's height: that floor is
// crossed straight (it is not the cell's own: no stop, no turn on it). 1: *nx,*nz the tile beyond the bridge cells; 0 not
// such a step (one onto the cell's own floor); -1 the crossing is blocked
static int BridgeCross(const PortMap *m, int cx, int cz, int d, int *nx, int *nz) {
    int x = cx + kDx[d], z = cz + kDz[d], h = m->cell[cz][cx].yf;

    if (!(m->cell[z][x].flags & PORT_MAP_BRIDGE) || abs(m->cell[z][x].yf - h) < 20 || abs(m->cell[z][x].yb - h) >= 20) {
        return 0;
    }
    while (x >= 0 && z >= 0 && x < N && z < N && (m->cell[z][x].flags & PORT_MAP_BRIDGE) && abs(m->cell[z][x].yb - h) < 20 &&
           abs(m->cell[z][x].yf - h) >= 20) {
        if (!(m->cell[z][x].passb & (1 << d))) {
            return -1;
        }
        h = m->cell[z][x].yb;
        x += kDx[d];
        z += kDz[d];
    }
    if (x < 0 || z < 0 || x >= N || z >= N || !Walkable(m->cell[z][x].kind) || abs(m->cell[z][x].yf - h) >= 20) {
        return -1;
    }
    *nx = x;
    *nz = z;
    return 1;
}

static int Search(const PortMap *m, int start, int hand, int *bestHOut);

// The way may leave the window (fx4, 2026-11-13). Until pz19 the chunks the mapper had not loaded were '?' (open), so the
// window's far parts were guessed open and a way round outside it was walked into; with the ROM's terrain the whole window is
// known and a goal whose way goes round beyond its edge (Route 6's south end into Mistralton's zone just past the window's
// last row) was "unreachable". After a search that found no goal (its sClosed: the reachable tiles), the reachable tile on
// the window's edge whose step out of it lands on the map matrix with the least g + 2 x (the distance from there to the
// nearest goal tile, 0 when that tile is in the wanted zone), and nearer it than the player (no walking back out of a dead end
// the way he came): *fx, *fz in grid tiles. 0: none (no way known).
static int EdgeTarget(const PortMap *m, int warpZone, int *fx, int *fz) {
    static int gx[512], gz[512];
    int ng = 0, i, x, z, d, bestScore = 1 << 30, here = 1 << 20;

    for (i = 0; i < N * N && ng < 512; i++) {   // the goal tiles (the first 512 of them: a zone's own tiles can be many)
        if (sGoal[i]) {
            gx[ng] = m->x0 + i % N;
            gz[ng] = m->z0 + i / N;
            here = abs(gx[ng] - m->px) + abs(gz[ng] - m->pz) < here ? abs(gx[ng] - m->px) + abs(gz[ng] - m->pz) : here;
            ng++;
        }
    }
    for (z = 0; z < N; z++) {
        for (x = 0; x < N; x++) {
            int c = z * N + x;

            if ((x > 0 && z > 0 && x < N - 1 && z < N - 1) || !sClosed[c] || !Walkable(m->cell[z][x].kind) ||
                sG[c] >= 1 << 29) {
                continue;
            }
            for (d = 0; d < 4; d++) {
                int ox = x + kDx[d], oz = z + kDz[d], wx = m->x0 + ox, wz = m->z0 + oz, dist = 1 << 20, k;

                if ((ox >= 0 && oz >= 0 && ox < N && oz < N) || wx < 0 || wz < 0 || (m->mw > 0 && wx >= m->mw * 32) ||
                    (m->mh > 0 && wz >= m->mh * 32)) {
                    continue;   // inside the window, or off the map matrix
                }
                if (warpZone >= 0 && ZoneAt(wx, wz) == warpZone) {
                    dist = 0;
                }
                for (k = 0; k < ng && dist > 0; k++) {
                    int dd = abs(gx[k] - wx) + abs(gz[k] - wz);

                    dist = dd < dist ? dd : dist;
                }
                if (dist < here && sG[c] + 2 * dist < bestScore) {   // only a way out nearer the goal than the player is
                    bestScore = sG[c] + 2 * dist;
                    *fx = m->x0 + x;
                    *fz = m->z0 + z;
                }
            }
        }
    }
    return bestScore < 1 << 30;
}

// the search (A*): steps cost 2, '?' 6, a ledge jump 4, a slide its length, a cannon's shot 12; h = 2 x the Manhattan distance
// to a far target (none, a goal in the window: h = 0, Dijkstra). start < 0: the player is on a rail, the search starts at the
// grid tiles its hand-offs lead to. hand: a switch tile's hand-off onto a rail, along it and off it onto the grid again is an
// edge too. Returns the goal tile it reached, or toward a far target the tile nearest it (*bestHOut: its distance), -1 none
static int Search(const PortMap *m, int start, int hand, int *bestHOut) {
    int i, best = -1, bestH = 1 << 30;

    for (i = 0; i < N * N; i++) {
        sG[i] = 1 << 30;
        sClosed[i] = 0;
        sSlopeRun[i] = 0;
        sWarpT[i] = 0;
    }
    for (i = 0; i < m->nwarp; i++) {
        const PortMapWarp *w = &m->warp[i];
        int x, z;

        for (z = w->z - m->z0; !w->rail && z < w->z - m->z0 + (w->h ? w->h : 1); z++) {
            for (x = w->x - m->x0; x < w->x - m->x0 + (w->w ? w->w : 1); x++) {
                if (x >= 0 && z >= 0 && x < N && z < N) {
                    sWarpT[z * N + x] = 1;
                }
            }
        }
    }
    for (i = 0; i < m->ntrig; i++) {
        const PortMapTrig *t = &m->trig[i];
        int x, z;

        for (z = t->z - m->z0; t->avoid && !t->rail && z < t->z - m->z0 + t->h; z++) {
            for (x = t->x - m->x0; x < t->x - m->x0 + t->w; x++) {
                if (x >= 0 && z >= 0 && x < N && z < N) {
                    sWarpT[z * N + x] = 1;
                }
            }
        }
    }
    sHeapN = 0;
    if (start >= 0) {
        sG[start] = 0;
        sF[start] = 0;
        sFrom[start] = -1;
        HeapPush(start);
    } else {
        RailLandings(m, RailReach(m, &m->rpos), 0, -1, -1);
    }
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
        if (sLever[c] && c != start) {   // stepping on a lever flips its switch: the way ends there (LeverPlan goes on)
            continue;
        }
        if (sHeurOn) {
            int h = abs(m->x0 + cx - sHeurX) + abs(m->z0 + cz - sHeurZ);

            if (h < bestH || (h == bestH && best >= 0 && sG[c] < sG[best])) {
                bestH = h;
                best = c;
            }
        }
        if (hand && sPlace[c] &&   // a place: its way's key pressed there runs its hand-off (its switch in the state it wants)
            ((sSw >> m->place[sPlace[c] - 1].s) & 1) == (unsigned)m->place[sPlace[c] - 1].want) {
            RailLandings(m, RailReach(m, &m->place[sPlace[c] - 1].to), sG[c] + 2, c, m->place[sPlace[c] - 1].dir);
        }
        if (hand && (m->cell[cz][cx].flags & PORT_MAP_SWITCH)) {   // the hand-off onto a rail from this switch tile
            for (d = 0; d < 4; d++) {
                PortRailPos to;

                if (sAdapter->gridToRail(m->x0 + cx, m->z0 + cz, d, &to)) {
                    RailLandings(m, RailReach(m, &to), sG[c], c, d);
                }
            }
        }
        for (d = 0; d < 4; d++) {
            int nx = cx + kDx[d], nz = cz + kDz[d], n, cost, slid = 0, pen = 0;
            char nk;

            if (nx < 0 || nz < 0 || nx >= N || nz >= N) {
                continue;
            }
            nk = m->cell[nz][nx].kind;
            if (sSurf && nk == '~' && (ck == '.' || ck == ',')) {
                continue;   // a surfing player who steps onto land stops surfing: no walking from land back into water (F:surf starts again)
            }
            if (sPlace[c] && m->place[sPlace[c] - 1].dir == d &&   // its way's key on an active place: the hand-off (above)
                ((sSw >> m->place[sPlace[c] - 1].s) & 1) == (unsigned)m->place[sPlace[c] - 1].want) {
                continue;
            }
            if (sPlace[nz * N + nx] && m->place[sPlace[nz * N + nx] - 1].dir == d) {   // a place walked onto its way: the same
                const int pi = sPlace[nz * N + nx] - 1;

                if (((sSw >> m->place[pi].s) & 1) == (unsigned)m->place[pi].want) {   // its script hands off onto the rail
                    if (hand && !sBlock[nz * N + nx] && (m->cell[cz][cx].pass & (1 << d)) && Walkable(nk)) {
                        RailLandings(m, RailReach(m, &m->place[pi].to), sG[c] + 2, c, d);
                    }
                    continue;
                }
            }
            // a ledge the step jumps, or an edge tile the surfing player hops (the adapter's pass bit: land beyond it): lands
            // a tile beyond
            if (m->cell[cz][cx].slide >= PORT_MAP_CANNON && PortRouteSlide_Cannon(m, sBlock, cx, cz, d, &nx, &nz)) {
                cost = 12;   // a cannon's shot (route_slide.c): the key toward it from its tile, the scene, the landing
                slid = 4;
            } else if ((LedgeDir(nk) == d && ck != '?') || (nk == 'e' && ck == '~' && (m->cell[cz][cx].pass & (1 << d)))) {
                const int hop = LedgeDir(nk) == d && (m->cell[nz][nx].flags & PORT_MAP_FARJUMP) ? 2 : 1;   // a far jump: 3 tiles

                nx += hop * kDx[d];
                nz += hop * kDz[d];
                if (nx < 0 || nz < 0 || nx >= N || nz >= N || !Walkable(m->cell[nz][nx].kind)) {
                    continue;
                }
                cost = 2 + 2 * hop;
            } else {
                int br;

                if (!(m->cell[cz][cx].pass & (1 << d)) || !Walkable(nk) || (br = BridgeCross(m, cx, cz, d, &nx, &nz)) < 0) {
                    continue;
                }
                cost = br > 0 ? 2 * (abs(nx - cx) + abs(nz - cz)) : nk == '?' || ck == '?' ? 6 : 2;
                if (!sBlock[nz * N + nx] && PortRouteSlide_End(m, sBlock, d, &nx, &nz)) {   // ice, a slope: where it stops
                    cost = 2 + abs(nx - cx) + abs(nz - cz);
                    slid = 4;
                }
            }
            n = nz * N + nx;
            if (sWarpT[n] && !sGoal[n]) {
                cost += WARP_COST;
                pen = WARP_COST;
            }
            if (sBlock[n] || sClosed[n] || sG[c] + cost >= sG[n] || ((m->cell[nz][nx].flags & PORT_MAP_STEPON) && !sGoal[n])) {
                continue;
            }
            sG[n] = sG[c] + cost;
            sFrom[n] = c;
            sStep[n] = (unsigned char)(d + slid);
            sF[n] = sG[n] - pen + Heur(m, nx, nz);
            sSlopeRun[n] = 0;
            if (sHeapN < N * N * 4) {
                HeapPush(n);
            }
        }
        {   // a bike slope up the column (the bicycle only): the run-up from this tile, past the slope, in one edge
            int sx, sz, sc;

            if (!m->rail && PortRouteSlide_BikeRun(m, sBlock, cx, cz, &sx, &sz, &sc)) {
                int n = sz * N + sx;

                if (!sClosed[n] && sG[c] + sc < sG[n]) {
                    sG[n] = sG[c] + sc;
                    sFrom[n] = c;
                    sStep[n] = 0;
                    sSlopeRun[n] = 1;
                    sF[n] = sG[n] + Heur(m, sx, sz);
                    if (sHeapN < N * N * 4) {
                        HeapPush(n);
                    }
                }
            }
        }
    }
    *bestHOut = bestH;
    return best;
}

// a switch tile in the window (a hybrid zone's hand-off: PORT_MAP_SWITCH)
static int HasSwitch(const PortMap *m) {
    int x, z;

    if (m->nplace > 0) {
        return 1;
    }
    for (z = 0; z < N; z++) {
        for (x = 0; x < N; x++) {
            if (m->cell[z][x].flags & PORT_MAP_SWITCH) {
                return 1;
            }
        }
    }
    return 0;
}

// sBlock for the switches' states sw: an arm whose switch sw has flipped (from the map's m->sw) blocks its other cell
static void ArmsApply(const PortMap *m, unsigned sw) {
    int i;

    for (i = 0; i < m->narm; i++) {
        const PortMapNpc *a = &m->npc[m->arm[i].npc];
        int moved = (int)(((sw ^ m->sw) >> m->arm[i].s) & 1), x = a->x - m->x0, z = a->z - m->z0;

        if (x >= 0 && z >= 0 && x < N && z < N) {
            sBlock[z * N + x] = (unsigned char)!moved;
        }
        x = m->arm[i].x1 - m->x0;
        z = m->arm[i].z1 - m->z0;
        if (x >= 0 && z >= 0 && x < N && z < N) {
            sBlock[z * N + x] = (unsigned char)moved;
        }
    }
}

// sLever for the switches' states sw: every lever's tile, a lift's (s -1) only while sw is the state it runs in; with
// leverOnce, not the switch pressed last (last)
static int LeverLive(const PortMap *m, int i, unsigned sw, int last) {
    if (m->leverOnce && m->lever[i].s >= 0 && m->lever[i].s == last) {
        return 0;
    }
    return m->lever[i].s >= 0 || m->lever[i].s == -2 || (unsigned)m->lever[i].on == sw;   // (s -2: a warp pad, always)
}

static void LeversMark(const PortMap *m, unsigned sw, int last) {
    int i, x, z;

    memset(sLever, 0, sizeof sLever);
    for (i = 0; i < m->nlever && i < PORT_MAP_LEVERS; i++) {
        x = m->lever[i].x - m->x0;
        z = m->lever[i].z - m->z0;
        if (x >= 0 && z >= 0 && x < N && z < N && LeverLive(m, i, sw, last)) {
            sLever[z * N + x] = 1;
        }
    }
}

// a zone with levers (PortMap's switches: Icirrus Gym's levers, Opelucid Gym's dragon triggers): breadth-first over the
// switches' states, each lever a search reaches flipping its switch and starting the next search where it leaves the player
// (its tile, or x1,z1; the adapter's railSwitches puts the rail lines in that state for it, its gridSwitches refills the
// grid so). The lever tile to walk onto first on the chain with the fewest
// levers that reaches a goal (toward a far target: the chain that gets nearest it, nearer than bestH0); -1 none
static int LeverPlan(const PortMap *m, int start, int bestH0) {
    static struct {
        unsigned sw;
        int at, first, last;
    } q[512];
    static unsigned char seen[64][PORT_MAP_LEVERS];
    int qn = 1, qi, res = -1, resH = bestH0, i;

    memset(seen, 0, sizeof seen);
    q[0].sw = m->sw;
    q[0].at = start;
    q[0].first = -1;
    q[0].last = m->leverLast;
    for (qi = 0; qi < qn; qi++) {
        int h = 1 << 30, r;

        sSw = q[qi].sw;
        if (sAdapter->gridSwitches != NULL) {   // (first: a refill sets the lists again)
            sAdapter->gridSwitches((PortMap *)m, sSw != m->sw, sSw);
        }
        ArmsApply(m, sSw);
        LeversMark(m, sSw, q[qi].last);
        if (sAdapter->railSwitches != NULL) {
            sAdapter->railSwitches(sSw != m->sw, sSw);
        }
        r = Search(m, q[qi].at, Hybrid(), &h);
        if (qi > 0 && r >= 0 && (sHeurOn ? h < resH : sGoal[r] != 0)) {
            res = q[qi].first;
            resH = h;
            if (!sHeurOn) {
                break;
            }
        }
        for (i = 0; i < m->nlever && i < PORT_MAP_LEVERS && qn < 512; i++) {   // the levers it reached: flip one, search on
            int x = m->lever[i].x - m->x0, z = m->lever[i].z - m->z0, t = z * N + x;
            int x1 = m->lever[i].x1 - m->x0, z1 = m->lever[i].z1 - m->z0;
            unsigned ns = m->lever[i].s == -2 ? q[qi].sw : m->lever[i].s < 0 ? (unsigned)m->lever[i].to : q[qi].sw ^ (1u << m->lever[i].s);   // (a lift: its state; a pad: the same)

            if (!LeverLive(m, i, q[qi].sw, q[qi].last) || x < 0 || z < 0 || x >= N || z >= N || x1 < 0 || z1 < 0 || x1 >= N || z1 >= N || t == q[qi].at ||
                !sClosed[t] || ns >= 64 || seen[ns][i]) {
                continue;
            }
            seen[ns][i] = 1;
            q[qn].sw = ns;
            q[qn].at = z1 * N + x1;   // (where the lever leaves the player)
            q[qn].first = q[qi].first < 0 ? t : q[qi].first;
            q[qn].last = m->lever[i].s == -2 ? q[qi].last : m->lever[i].s;
            qn++;
        }
    }
    sSw = m->sw;
    if (sAdapter->gridSwitches != NULL) {
        sAdapter->gridSwitches((PortMap *)m, 0, m->sw);
    }
    ArmsApply(m, m->sw);
    LeversMark(m, m->sw, m->leverLast);
    if (sAdapter->railSwitches != NULL) {
        sAdapter->railSwitches(0, m->sw);
    }
    return res;
}

// a warp pad into the zone itself (a warp whose zone is this one: Black and White's Opelucid Gym, the pad behind the
// Leader back to the entrance; the Leader's tier has no other way down): when the search reaches no goal, the pad tile
// the search reached whose arrival reaches one (with levers too), or whose arrival is out of the window; never the pad
// back to the one the last pad came from (sPadFrom: this G token's last pad, in this zone). The pad's tile, or -1
static unsigned sPadZone;

static int PadPlan(const PortMap *m, int start, int hand, int *padWarp) {
    int cand[8], cw[8], nc = 0, i, k, h;

    for (i = 0; i < m->nwarp && nc < 8; i++) {
        const PortMapWarp *w = &m->warp[i];
        int x = w->x - m->x0, z = w->z - m->z0;

        if (w->rail || w->zone != m->zone || (int)w->dest == i || (int)w->dest >= m->nwarp || m->warp[w->dest].rail ||
            (sPadFrom >= 0 && (int)w->dest == sPadFrom) || x < 0 || z < 0 || x >= N || z >= N || z * N + x == start ||
            !sClosed[z * N + x]) {
            continue;
        }
        cand[nc] = z * N + x;
        cw[nc++] = i;
    }
    for (k = 0; k < nc; k++) {
        const PortMapWarp *d = &m->warp[m->warp[cw[k]].dest];
        int x = d->x - m->x0, z = d->z - m->z0, r;

        if (x < 0 || z < 0 || x >= N || z >= N) {   // its arrival out of the window: plan again there
            *padWarp = cw[k];
            return cand[k];
        }
        r = Search(m, z * N + x, hand, &h);
        if ((r >= 0 && sGoal[r]) || (m->nlever > 0 && hand && LeverPlan(m, z * N + x, 1 << 30) >= 0)) {
            *padWarp = cw[k];
            return cand[k];
        }
    }
    return -1;
}

// a G token's goal: the next waypoint toward it. Returns PLAN_OK with the waypoint (the end of the path's first straight
// run), PLAN_THERE (on a goal tile; *press: the direction to press into a warp or that faces the target, or -1), or
// PLAN_NONE with why. seeds: the player on a rail of a hybrid zone, the goal on its grid: the grid search starts at the
// hand-offs of the rail (the rail planner's nearest hand-off may lead into the wrong room)
static int PlanNext(const PortRouteState *st, const PortRouteGoal *goal, PortRoutePlan *out, int seeds) {
    const PortMap *m = PortRouteMap_Get();
    int tx = goal->kind == GOAL_TILE ? goal->x : 0, tz = goal->kind == GOAL_TILE ? goal->z : 0;
    int warpZone = goal->kind == GOAL_WARP ? goal->n : -1;
    int i, x, z, start, best = -1, bestH = 1 << 30, goals = 0, farX = tx, farZ = tz, far = goal->kind == GOAL_TILE,
                     goalWarps = 0, toRail, wall = -1, past = 0, hand, leverLeg = 0, lastHand = 0;
    char gname[32];

    memset(out, 0, sizeof *out);
    out->press = -1;
    if (!m->ok) {
        snprintf(out->why, sizeof out->why, "no field");
        return PLAN_NONE;
    }
    sSurf = m->surf;
    GoalName(goal, gname, sizeof gname);
    if (st->zone != sTmpZone) {
        sTmpN = 0;
        sRTmpN = 0;
        sTmpZone = st->zone;
    }
    if (goal->kind == GOAL_BG && goal->n >= m->nbg) {
        snprintf(out->why, sizeof out->why, "%s: zone %u has %d BG events (b0..b%d)", gname, m->zone, m->nbg, m->nbg - 1);
        return PLAN_NONE;
    }
    if (goal->kind == GOAL_TRIG && goal->n >= m->ntrig) {
        snprintf(out->why, sizeof out->why, "%s: zone %u has %d triggers (T0..T%d)", gname, m->zone, m->ntrig, m->ntrig - 1);
        return PLAN_NONE;
    }
    if ((goal->kind == GOAL_NPC || goal->kind == GOAL_ITEM) && m->rail) {
        for (i = 0; i < m->nnpc && m->npc[i].id != goal->n; i++) {
        }
        if (i == m->nnpc) {
            snprintf(out->why, sizeof out->why, "%s: no actor of uid %d in zone %u", gname, goal->n, m->zone);
            return PLAN_NONE;
        }
    }
    if (m->rail) {
        if (goal->kind == GOAL_WALL || goal->kind == GOAL_RIDE) {
            snprintf(out->why, sizeof out->why, "%s: the player is on a rail (G:wall is for a grid map)", gname);
            return PLAN_NONE;
        }
        if (!seeds || !Hybrid() || GoalSide(m, goal) != 0) {
            return RailPlan(m, goal, out, gname, NULL, 0);
        }
    }
    toRail = sAdapter->gridToRail != NULL && GoalSide(m, goal) == 1;   // a hand-off onto a rail first
    if (goal->kind == GOAL_RAIL && !toRail) {
        snprintf(out->why, sizeof out->why, "%s: the player is not on a rail", gname);
        return PLAN_NONE;
    }
    memset(sBlock, 0, sizeof sBlock);
    memset(sGoal, 0, sizeof sGoal);
    for (i = 0; i < m->nnpc; i++) {   // actors are in the way, where they stand and where they step from, at their height
        int ax[2] = {m->npc[i].x, m->npc[i].x0}, az[2] = {m->npc[i].z, m->npc[i].z0}, ay[2] = {m->npc[i].y, m->npc[i].y0}, j;
        int fx, fz;   // each tile of its footprint (a 2x2 boulder)

        for (j = 0; j < 2 && !m->npc[i].rail; j++) {
            for (fz = 0; fz < NpcH(&m->npc[i]); fz++) {
                for (fx = 0; fx < NpcW(&m->npc[i]); fx++) {
                    x = ax[j] + fx - m->x0;
                    z = az[j] - fz - m->z0;
                    if (x >= 0 && z >= 0 && x < N && z < N && NpcBlocks(m, &m->npc[i], ax[j] + fx, az[j] - fz, ay[j])) {
                        sBlock[z * N + x] = m->npc[i].breaks && sBlock[z * N + x] != 1 ? 2 : 1;   // (2: a slide may smash it)
                    }
                }
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
    memset(sPlace, 0, sizeof sPlace);   // the switches: levers and places in the window
    sSw = m->sw;
    LeversMark(m, m->sw, m->leverLast);
    for (i = 0; i < m->nplace && i < 8; i++) {
        x = m->place[i].x - m->x0;
        z = m->place[i].z - m->z0;
        if (x >= 0 && z >= 0 && x < N && z < N) {
            sPlace[z * N + x] = (unsigned char)(i + 1);
        }
    }
    // the goal tiles
    if (toRail) {   // a switch tile, pressing the way that steps onto a rail (not a tile that failed)
        for (z = 0; z < N; z++) {
            for (x = 0; x < N; x++) {
                PortRailPos to;
                int d;

                for (d = 0; d < 4 && (m->cell[z][x].flags & PORT_MAP_SWITCH) && Walkable(m->cell[z][x].kind) &&
                            !sBlock[z * N + x];
                     d++) {
                    if (sAdapter->gridToRail(m->x0 + x, m->z0 + z, d, &to) && RailHasGoal(m, goal, &to)) {
                        sGoal[z * N + x] = (unsigned char)(3 + d);
                        goals++;
                        break;
                    }
                }
            }
        }
        if (goals == 0) {
            snprintf(out->why, sizeof out->why, "%s: it is on a rail and no switch tile here hands off to a rail that has it", gname);
            return PLAN_NONE;
        }
    } else if (goal->kind == GOAL_NPC || goal->kind == GOAL_ITEM) {   // next to the actor, facing it
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
        if (m->npc[i].rail) {
            snprintf(out->why, sizeof out->why, "%s: the actor is on a rail, the player is not", gname);
            return PLAN_NONE;
        }
        goals = NpcFacingGoals(m, &m->npc[i]);
    } else if (goal->kind == GOAL_WALL) {   // the cell before the pass-through actor, on the side the search reaches first
        for (i = 0; i < m->nnpc && m->npc[i].id != goal->n; i++) {
        }
        if (i == m->nnpc) {
            snprintf(out->why, sizeof out->why, "%s: no actor of uid %d in zone %u", gname, goal->n, m->zone);
            return PLAN_NONE;
        }
        past = sAdapter->passActor != NULL ? sAdapter->passActor(m->zone, &m->npc[i]) : 0;
        if (past <= 0 || m->npc[i].rail) {
            snprintf(out->why, sizeof out->why, "%s: the actor of uid %d in zone %u is not a pass-through actor", gname, goal->n,
                     m->zone);
            return PLAN_NONE;
        }
        wall = i;
        for (int d = 0; d < 4; d++) {   // the cell before it for a step in d
            x = m->npc[i].x - kDx[d] - m->x0;
            z = m->npc[i].z - kDz[d] - m->z0;
            if (x >= 0 && z >= 0 && x < N && z < N && Walkable(m->cell[z][x].kind) && !sBlock[z * N + x]) {
                sGoal[z * N + x] = (unsigned char)(3 + d);
                goals++;
            }
        }
    } else if (goal->kind == GOAL_RIDE) {   // the ride's start tile, facing its entry
        if (!RideGoal(m, goal, gname, out)) {
            return PLAN_NONE;
        }
        goals = 1;
    } else if (goal->kind == GOAL_BG) {   // next to the BG event, facing it
        if (goal->n >= m->nbg) {
            snprintf(out->why, sizeof out->why, "%s: zone %u has %d BG events (b0..b%d)", gname, m->zone, m->nbg, m->nbg - 1);
            return PLAN_NONE;
        }
        if (m->bg[goal->n].rail) {
            snprintf(out->why, sizeof out->why, "%s: the BG event is on a rail, the player is not", gname);
            return PLAN_NONE;
        }
        goals = FacingGoals(m, m->bg[goal->n].x, m->bg[goal->n].z, BgFacings(&m->bg[goal->n]));
    } else if (goal->kind == GOAL_TRIG) {   // onto the trigger
        const PortMapTrig *t;
        int gx, gz;

        if (goal->n >= m->ntrig) {
            snprintf(out->why, sizeof out->why, "%s: zone %u has %d triggers (T0..T%d)", gname, m->zone, m->ntrig, m->ntrig - 1);
            return PLAN_NONE;
        }
        t = &m->trig[goal->n];
        if (t->rail) {
            snprintf(out->why, sizeof out->why, "%s: the trigger is on a rail, the player is not", gname);
            return PLAN_NONE;
        }
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
            int in = WarpWalkIn(w), n;

            if (!WarpWanted(m, goal, i) || w->rail) {
                continue;
            }
            n = in >= 0 ? WarpGoals(m, w, in) : 0;   // from the side it must be walked into; any when there is none to stand on
            if (w->type == 7 && in >= 0) {   // Gen 4 mat or stairs (route_gen4.c): the key is held on it, so standing on it is a goal too
                int wx, wz;

                for (wz = w->z; wz < w->z + (w->h ? w->h : 1); wz++) {
                    for (wx = w->x; wx < w->x + (w->w ? w->w : 1); wx++) {
                        int gx = wx - m->x0, gz = wz - m->z0;

                        if (gx >= 0 && gz >= 0 && gx < N && gz < N && Walkable(m->cell[gz][gx].kind) && !sGoal[gz * N + gx]) {
                            sGoal[gz * N + gx] = 2;
                            n++;
                        }
                    }
                }
            }
            goals += n > 0 ? n : WarpGoals(m, w, -1);
        }
        goalWarps = goals > 0;
        if (goal->door && goals == 0) {   // G:door: that warp only, never the zone's edge or a hint
            snprintf(out->why, sizeof out->why, "no grid warp D%d in zone %u (it has %d)", goal->n, m->zone, m->nwarp);
            return PLAN_NONE;
        }
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
            if (bd == 1 << 30 && WarpHint(warpZone, m, &farX, &farZ)) {   // an interior zone: the warps into it, from the atlas
                bd = 0;
            }
            if (bd == 1 << 30) {
                snprintf(out->why, sizeof out->why, "no warp to zone %d here, no chunk of it on the map matrix and no hint (BWPORT_WARP_HINTS)", warpZone);
                return PLAN_NONE;
            }
            far = 1;
        }
    }
    if (goals == 0 && !far && !toRail && FarGoal(m, goal, &farX, &farZ)) {   // beyond the window: toward it, then plan again
        far = 1;
    }
    x = m->px - m->x0;
    z = m->pz - m->z0;
    start = z * N + x;
    if (!m->rail && sGoal[start]) {
        out->x = m->px;
        out->z = m->pz;
        out->press = sGoal[start] >= 3 ? sGoal[start] - 3 : -1;
        out->handoff = toRail;
        if (wall >= 0 && out->press >= 0) {   // the pass ends past the wall's cell, on the line of the step
            out->passX = m->npc[wall].x + past * kDx[out->press];
            out->passZ = m->npc[wall].z + past * kDz[out->press];
        }
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
    sHeurOn = far && goals == 0;
    sHeurX = farX;
    sHeurZ = farZ;
    hand = !toRail && Hybrid();
    if (m->rail) {   // from the rail: the seeds are the grid tiles its hand-offs lead to
        start = -1;
        lastHand = 1;
        best = Search(m, -1, 1, &bestH);
    } else {
        best = Search(m, start, 0, &bestH);
        if (hand && (sHeurOn ? bestH > 0 : (best < 0 || best == start)) && HasSwitch(m)) {   // none on the grid alone: by the rails
            int h2, b2 = Search(m, start, 1, &h2);

            if (b2 >= 0 && b2 != start && (!sHeurOn || h2 < bestH)) {
                best = b2;
                lastHand = 1;
                bestH = h2;
            } else {
                best = Search(m, start, 0, &bestH);   // (the arrays the failure's reasons read are the grid alone's)
            }
        }
    }
    // levers to flip on the way (a hybrid zone's, or a grid zone's: the Plasma Frigate's floor switches)
    if (m->nlever > 0 && (hand || (!toRail && !m->rail)) && (sHeurOn ? bestH > 0 : (best < 0 || best == start))) {
        int lv = LeverPlan(m, start, sHeurOn ? bestH : 1 << 30);

        if (lv >= 0) {   // the way to the first lever (the plan goes on from there with its switch flipped)
            memset(sGoal, 0, sizeof sGoal);
            sGoal[lv] = 1;
            sHeurOn = 0;
            best = Search(m, start, Hybrid(), &bestH);
            leverLeg = 1;
        } else {   // (the search without levers again: its arrays)
            best = Search(m, start, lastHand, &bestH);
        }
    }
    if (st->zone != sPadZone) {
        sPadFrom = -1;
        sPadZone = st->zone;
    }
    if ((best < 0 || best == start) && !m->rail && !sHeurOn) {   // no way: a warp pad into this zone whose arrival has one
        int pw = -1, pad = PadPlan(m, start, hand, &pw);

        if (pad >= 0) {   // the way onto the pad (the plan goes on where it puts the player)
            memset(sGoal, 0, sizeof sGoal);
            sGoal[pad] = 1;
            best = Search(m, start, lastHand, &bestH);
            if (best >= 0 && best != start) {
                leverLeg = 1;
                sPadFrom = pw;
            }
        } else {   // (the failed search again: its arrays)
            best = Search(m, start, lastHand, &bestH);
        }
    }
    if ((best < 0 || best == start) && !m->rail && !sHeurOn && goals > 0 && EdgeTarget(m, warpZone, &farX, &farZ)) {
        // no way inside the window: one may leave it (pz19's ROM terrain fills the whole window, so a way round outside it
        // is no longer guessed through '?' cells): toward the edge tile that EdgeTarget picked, then plan again there
        sHeurOn = 1;
        sHeurX = farX;
        sHeurZ = farZ;
        best = Search(m, start, 0, &bestH);
        if (best >= 0 && best != start) {
            leverLeg = 1;   // (not the goal: plan again on the way)
        } else {
            sHeurOn = 0;
            best = Search(m, start, lastHand, &bestH);   // (the failed search again: its arrays)
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
                rn += (size_t)snprintf(reach + rn, sizeof reach - rn, " D%d zone %u (%d,%d)", i, w->zone, w->x, w->z);
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
        if (!m->cycle) {   // a bike slope in the window: a walker cannot climb it (the bicycle in its fast gear can: Bk Ky:4)
            int bx, bz;

            for (bz = 0; bz < N; bz++) {
                for (bx = 0; bx < N && !(m->cell[bz][bx].flags & PORT_MAP_BIKESLOPE); bx++) {
                }
                if (bx < N) {
                    rn = strlen(out->why);
                    snprintf(out->why + rn, sizeof out->why - rn, "; a bike slope at %d,%d (a walker cannot climb it: Bk Ky:4 mounts the bicycle)",
                             m->x0 + bx, m->z0 + bz);
                    break;
                }
            }
        }
        return PLAN_NONE;
    }
    if (m->wind && !m->rail && !sHeurOn && goals > 0) {   // the wind's gusts (route_slide.c): a hop to a shelter, or wait there
        static int wpath[N * N];
        int wn = 0, k = 1, w = PortRouteWind_Hop(m, sBlock, sGoal, start, wpath, &wn);

        if (w == 2) {
            out->x = out->nextX = m->px;
            out->z = out->nextZ = m->pz;
            out->wait = 1;
            snprintf(out->why, sizeof out->why, "wind: in the shelter, the gust in %d", m->gust);
            return PLAN_OK;
        }
        if (w == 1) {
            while (k + 1 < wn && wpath[k + 1] - wpath[k] == wpath[1] - wpath[0]) {
                k++;
            }
            out->x = m->x0 + wpath[k] % N;
            out->z = m->z0 + wpath[k] / N;
            out->nextX = m->x0 + wpath[1] % N;
            out->nextZ = m->z0 + wpath[1] / N;
            out->steps = wn - 1;
            snprintf(out->why, sizeof out->why, "wind: to the shelter %d,%d, the gust in %d", m->x0 + wpath[wn - 1] % N,
                     m->z0 + wpath[wn - 1] / N, m->gust);
            return PLAN_OK;
        }
    }
    // the path back from best; the waypoint is the end of its first straight run (a ledge jump stays on its line)
    {
        static int path[N * N];
        int n = 0, c, k, dx0, dz0;

        for (c = best; c != -1; c = sFrom[c]) {
            path[n++] = c;
        }
        // path[n-1] is the start
        if (m->rail) {   // from a rail: its way to the hand-off that lands on the path's first tile
            PortRailPos want;

            RailReach(m, &m->rpos);
            want = sRPos[sSeedNode[path[n - 1]]];
            return RailPlan(m, goal, out, gname, &want, sSeedDir[path[n - 1]]);
        }
        if (sStep[path[n - 2]] & HAND) {   // the first step is the hand-off from this switch tile
            out->x = m->px;
            out->z = m->pz;
            out->press = sStep[path[n - 2]] & 3;
            out->handoff = 1;
            return PLAN_THERE;
        }
        dx0 = path[n - 2] % N - path[n - 1] % N;
        dz0 = path[n - 2] / N - path[n - 1] / N;
        dx0 = dx0 > 0 ? 1 : dx0 < 0 ? -1 : 0;
        dz0 = dz0 > 0 ? 1 : dz0 < 0 ? -1 : 0;
        k = n - 2;
        while (k > 0) {
            int ddx = path[k - 1] % N - path[k] % N, ddz = path[k - 1] / N - path[k] / N;

            ddx = ddx > 0 ? 1 : ddx < 0 ? -1 : 0;
            ddz = ddz > 0 ? 1 : ddz < 0 ? -1 : 0;
            if (ddx != dx0 || ddz != dz0 || (sStep[path[k - 1]] & 3) != (sStep[path[n - 2]] & 3) || (sStep[path[k - 1]] & HAND)) {
                break;   // (a slide on that line from a step another way: a slope's: a run of its own)
            }
            k--;
        }
        if (m->cycle) {   // a rider is refused some tiles (mud, snow, very tall grass): the run stops before the first, and a run
                          // whose first step is onto one starts with the pilot getting off
            for (i = n - 2; i >= k && !(m->cell[path[i] / N][path[i] % N].flags & PORT_MAP_NOBIKE); i--) {
            }
            if (i >= k && i == n - 2) {
                out->dismount = 1;
            } else if (i >= k) {
                k = i + 1;
            }
        }
        out->x = m->x0 + path[k] % N;
        out->z = m->z0 + path[k] / N;
        if ((sStep[path[n - 2]] & 4) && (dx0 != kDx[sStep[path[n - 2]] & 3] || dz0 != kDz[sStep[path[n - 2]] & 3])) {
            // a slope's slide off the step's line first: the waypoint is the step's tile (the slide is the game's event:
            // the pilot presses through it and plans again where it stops)
            out->x = m->px + kDx[sStep[path[n - 2]] & 3];
            out->z = m->pz + kDz[sStep[path[n - 2]] & 3];
            k = n - 1;   // (not the goal)
        }
        for (i = k; i <= n - 2; i++) {   // the first run climbs a bike slope: the bicycle needs its fast gear (the pilot presses B)
            if (sSlopeRun[path[i]] && m->cycle == 1) {
                out->gear = 1;
            }
        }
        if (k < n - 1 && sSlopeRun[path[k]]) {   // the run ends on a slope's landing: the walk lets go of the key within a few frames of its
            // waypoint, and a rider still on the slope then slides back (measured at the slope's top): the waypoint is two tiles on
            int lx = path[k] % N, lz = path[k] / N, e;

            for (e = 0; e < 2 && lz > 0 && Walkable(m->cell[lz - 1][lx].kind) && !sBlock[(lz - 1) * N + lx] && (m->cell[lz][lx].pass & 1); e++) {
                lz--;
            }
            if (e > 0) {
                out->x = m->x0 + lx;
                out->z = m->z0 + lz;
                k = n - 1;   // (not the goal: the next plan walks back from there)
            }
        }
        out->nextX = m->x0 + path[n - 2] % N;
        out->nextZ = m->z0 + path[n - 2] / N;
        out->steps = n - 1;
        out->final = k == 0 && !(far && goals == 0) && !leverLeg;
        out->press = out->final && sGoal[best] >= 3 ? sGoal[best] - 3 : -1;
    }
    return PLAN_OK;
}

int PortRoutePlan_Next(const PortRouteState *st, const PortRouteGoal *goal, PortRoutePlan *out) {
    int r = PlanNext(st, goal, out, 1);

    if (r == PLAN_NONE && PortRouteMap_Get()->rail) {   // no way by the rails' hand-offs: the rail planner's nearest hand-off
        r = PlanNext(st, goal, out, 0);
    }
    return r;
}
#undef N
