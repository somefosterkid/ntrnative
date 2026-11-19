// The route planner's slides (port/src/route_map.c's A*): the floors that move the player by themselves, as the game
// moves it (port_route_map.h's PortMapCell.slide, the adapter's from the tile class).
//   Ice (map_attr.c's IsTileSlidingIce; field_player_grid.c): on an ice tile the player goes on one more tile the way it
//   moves (ConstMoveUpdate_STRAIGHT) until that tile is in the way (PlayerMoveCollCheck: a wall, an actor, a height) and
//   stops there (the end spot: the next step is the keys' again), or until it is on a tile that is not ice.
//   A slope slip (field_slope_slip.c, an event that starts on the tile): the player moves the way the tile says, then the
//   way of each slope tile it crosses, on past the slope, until the next tile that way is in the way.
// Test-only, as route_map.c.
#include <stdlib.h>
#include "port_route_map.h"
#include "port_route_plan.h"

// Black / White's Mistralton Gym cannons (ov041 func_ov041_021f37e0 and the event func_ov041_021f3884): the player on the tile two
// before a cannon, pressing toward it (the tile between is a wall), is set on the shot's landing tile when the scene ends
int PortRouteSlide_Cannon(const PortMap *m, const unsigned char *block, int cx, int cz, int d, int *x, int *z) {
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    int s = m->cell[cz][cx].slide - PORT_MAP_CANNON, n = (s >> 2) + 1, nx = cx + dx[d] * n, nz = cz + dz[d] * n;
    char k;

    if (m->cell[cz][cx].slide < PORT_MAP_CANNON || d != (s & 3) || nx < 0 || nz < 0 || nx >= PORT_MAP_N || nz >= PORT_MAP_N ||
        block[nz * PORT_MAP_N + nx]) {
        return 0;
    }
    k = m->cell[nz][nx].kind;
    if (k != '.' && k != ',' && k != '?') {   // ('?': the floor there is not loaded yet; the plan is made again as it loads)
        return 0;
    }
    *x = nx;
    *z = nz;
    return 1;
}

// The wind (Black 2 / White 2's Mistralton Gym, ov094 gym_mistralton.c): a gust blows the player south off any tile the
// adapter has not marked PORT_MAP_SHELTER, and the calm between gusts is shorter than the way across the gym. So the walk
// hops: to the shelter within the steps left that is fewest steps from the goal, waits there for the gust (it leaves the
// player standing; the next calm is a whole one), and hops on. Steps on walk tiles only ('.', ','), the pass bits, no
// actor's tile (block)
// A bike slope (Gen 4's muddy slope, player_move.c PlayerAvatar_TileMove_BikeSlope): on a slope tile, the player who moved north and
// holds north is carried up if he is on the bicycle at speed 3, else he is slid back. In the fast gear (PlayerAvatar_SetMovement_
// BikeFourthGear) PlayerAvatar_AccelerateBike adds one speed to each step made in a line, so the third step from rest ends at speed 3:
// the slope's first tile must be PORT_MAP_BIKESLOPE_RUN steps up the column from where the run starts (a start tile one step nearer
// gets there at speed 2: measured, it slides back). The tiles between are plain floor, each step open (pass bit up), and the slope
// goes on up the column to a plain tile.
int PortRouteSlide_BikeRun(const PortMap *m, const unsigned char *block, int cx, int cz, int *x, int *z, int *cost) {
    int i, s = 0, n = PORT_MAP_BIKESLOPE_RUN;
    char k;

    if (!m->cycle || cz - n - 1 < 0) {
        return 0;
    }
    for (i = 1; i < n; i++) {   // the run-up tiles: plain, open, no actor
        k = m->cell[cz - i][cx].kind;
        if ((k != '.' && k != ',') || block[(cz - i) * PORT_MAP_N + cx] || !(m->cell[cz - i + 1][cx].pass & 1)) {
            return 0;
        }
    }
    while (cz - n - s >= 0 && s < 16 && (m->cell[cz - n - s][cx].flags & PORT_MAP_BIKESLOPE)) {
        s++;
    }
    if (s == 0 || cz - n - s < 0) {
        return 0;
    }
    k = m->cell[cz - n - s][cx].kind;
    if ((k != '.' && k != ',') || block[(cz - n - s) * PORT_MAP_N + cx]) {
        return 0;
    }
    *x = cx;
    *z = cz - n - s;
    *cost = 2 * (n + s) + 2;
    return 1;
}

int PortRouteWind_Hop(const PortMap *m, const unsigned char *block, const unsigned char *goal, int start, int *path, int *n) {
    enum { N = PORT_MAP_N };
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    static int ds[N * N], dg[N * N], from[N * N], q[N * N];
    int budget = (m->gust - PORT_GUST_MARGIN) / PORT_GUST_STEP, head = 0, tail = 0, best = -1, i, c, d;

    if (budget < 0) {
        budget = 0;
    }
    for (i = 0; i < N * N; i++) {   // the steps from each tile to a goal (back along the steps the game takes)
        ds[i] = -1;
        dg[i] = goal[i] ? 0 : -1;
        if (goal[i]) {
            q[tail++] = i;
        }
    }
    while (head < tail) {
        c = q[head++];
        for (d = 0; d < 4; d++) {
            int px = c % N - dx[d], pz = c / N - dz[d], p = pz * N + px;   // p's step d lands on c
            char k;

            if (px < 0 || pz < 0 || px >= N || pz >= N || dg[p] >= 0 || block[p] || !(m->cell[pz][px].pass & (1 << d))) {
                continue;
            }
            k = m->cell[pz][px].kind;
            if (k == '.' || k == ',') {
                dg[p] = dg[c] + 1;
                q[tail++] = p;
            }
        }
    }
    head = tail = 0;   // the tiles within budget steps from start
    ds[start] = 0;
    from[start] = -1;
    q[tail++] = start;
    while (head < tail) {
        c = q[head++];
        if (goal[c]) {
            return 0;
        }
        if ((m->cell[c / N][c % N].flags & PORT_MAP_SHELTER) && dg[c] >= 0 && (best < 0 || dg[c] < dg[best])) {
            best = c;
        }
        if (ds[c] >= budget) {
            continue;
        }
        for (d = 0; d < 4; d++) {
            int nx = c % N + dx[d], nz = c / N + dz[d], nc = nz * N + nx;
            char k;

            if (nx < 0 || nz < 0 || nx >= N || nz >= N || ds[nc] >= 0 || block[nc] || !(m->cell[c / N][c % N].pass & (1 << d))) {
                continue;
            }
            k = m->cell[nz][nx].kind;
            if (k == '.' || k == ',' || goal[nc]) {
                ds[nc] = ds[c] + 1;
                from[nc] = c;
                q[tail++] = nc;
            }
        }
    }
    if (best < 0) {
        return 0;
    }
    if (best == start) {   // wait for the gust there; not where the count stands still (it would never come)
        return m->windHold ? 0 : 2;
    }
    *n = ds[best] + 1;
    for (c = best, i = *n - 1; c != -1; c = from[c]) {
        path[i--] = c;
    }
    return 1;
}

int PortRouteSlide_End(const PortMap *m, const unsigned char *block, int d, int *x, int *z) {
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    int slope = 0, i, speed = 0;

    if (m->cell[*z][*x].slide == 0 || m->cell[*z][*x].slide >= PORT_MAP_CANNON) {   // (a cannon's tile: the shot is from it)
        return 0;
    }
    if (m->cell[*z][*x].slide >= PORT_MAP_PAD) {   // a Driftveil Gym pad (ov096 gym_driftveil.c: DriftveilGym_CheckPadEvent): a step
        static const int kPadTiles[4] = {7, 4, 18, 14};   // onto it the way it faces jumps one tile, slides that many, jumps one
        int s = m->cell[*z][*x].slide - PORT_MAP_PAD, n = kPadTiles[s >> 2] + 2;

        if (d != (s & 3) || *x + dx[d] * n < 0 || *z + dz[d] * n < 0 || *x + dx[d] * n >= PORT_MAP_N || *z + dz[d] * n >= PORT_MAP_N) {
            return 0;
        }
        *x += dx[d] * n;
        *z += dz[d] * n;
        return 1;
    }
    for (i = 0; i < 4 * PORT_MAP_N; i++) {   // (a ring of slopes would go on for ever: the game's too)
        const PortMapCell *c = &m->cell[*z][*x];
        int nx, nz, smash = -1;
        char k;

        // a breakable actor (block 2: Gen 4's snowball) on the next tile is deleted between two steps of a slide with speed
        // (ov5_021DFB54.c ov5_021E06A8, run while the forced move is on, before the tile's own step decision: so also on the
        // first tile off the ice, where the slide then stops); the slide goes on through its tile. The grid is the adapter's
        // again at the next plan (the game has deleted it)
        if (speed >= 1 && *x + dx[d] >= 0 && *z + dz[d] >= 0 && *x + dx[d] < PORT_MAP_N && *z + dz[d] < PORT_MAP_N &&
            block[(*z + dz[d]) * PORT_MAP_N + *x + dx[d]] == 2) {
            smash = (*z + dz[d]) * PORT_MAP_N + *x + dx[d];
        }
        if (c->slide >= PORT_MAP_SLOPE) {
            slope = 1;
            d = c->slide - PORT_MAP_SLOPE;
        } else if (c->slide != PORT_MAP_ICE && !slope) {
            break;   // off the ice
        }
        nx = *x + dx[d];
        nz = *z + dz[d];
        {   // Gen 4's ice with a height (Snowpoint's gym, player_move.c PlayerAvatar_TileMove_Ice): the avatar's speed starts at rest, one
            // more at each tile where the floor falls a quarter tile on (to 3), one less where it rises; an incline it has no speed for,
            // or one with a wall at once beyond, turns it round: it walks one tile back (the way it came) and rests there
            int hc = c->slide == PORT_MAP_ICE && !slope ? (c->ihc >> (2 * d)) & 3 : 0;

            if (hc != 0) {
                int open = nx >= 0 && nz >= 0 && nx < PORT_MAP_N && nz < PORT_MAP_N && (c->pass & (1 << d)) &&
                           (!block[nz * PORT_MAP_N + nx] || nz * PORT_MAP_N + nx == smash) &&
                           (m->cell[nz][nx].kind == '.' || m->cell[nz][nx].kind == ',');

                if (hc == 2 && open && speed < 3) {
                    speed++;
                } else if (hc == 1 && (!open || speed == 0)) {
                    int bx = *x - dx[d], bz = *z - dz[d];

                    if (bx >= 0 && bz >= 0 && bx < PORT_MAP_N && bz < PORT_MAP_N && (c->pass & (1 << (d ^ 1))) && !block[bz * PORT_MAP_N + bx]) {
                        *x = bx;
                        *z = bz;
                    }
                    return 1;
                } else if (hc == 1) {
                    speed--;
                }
            }
        }
        if (nx < 0 || nz < 0 || nx >= PORT_MAP_N || nz >= PORT_MAP_N || !(c->pass & (1 << d)) ||
            (block[nz * PORT_MAP_N + nx] && nz * PORT_MAP_N + nx != smash)) {
            break;
        }
        k = m->cell[nz][nx].kind;
        if (k != '.' && k != ',') {   // ('?': not loaded, stop there; the plan is made again as it loads)
            break;
        }
        *x = nx;
        *z = nz;
    }
    return 1;
}
