// port/src/route_map.c's dump, G-token parser and planner, for port/src/route_test.c (the pilot's Md and G tokens).
// Game-neutral: it sees the game only through port_route_map.h's PortMap and adapter.
#ifndef BWPORT_PORT_ROUTE_PLAN_H
#define BWPORT_PORT_ROUTE_PLAN_H

#include "port_route.h"
#include "port_route_map.h"

enum { PLAN_NONE, PLAN_OK, PLAN_THERE };

// a G token's target (docs/port-worker.md "Routes"; route_test.c's header)
enum {
    GOAL_TILE,   // G:X,Z     onto the tile
    GOAL_WARP,   // G:warp:N  onto / into a warp to zone N, or where the map goes on into zone N; done in zone N
    GOAL_NPC,    // G:npc:N   next to the actor of id (UID) N, facing it; then A and the event's end (":nota": no A)
    GOAL_ITEM,   // G:item:N  the same for an item ball's actor (id N)
    GOAL_TRIG,   // G:trig:N  onto trigger N (the dump's TN: its index in the zone's list)
    GOAL_BG,     // G:bg:N    next to BG event N (the dump's bN), facing it; then A and the event's end (":nota": no A)
    GOAL_RAIL,   // G:rail:L,F,S  onto the rail place rail[L,F,S] (line, front, side: the dump's)
    GOAL_WALL,   // G:wall:N  through the pass-through actor of id N (the adapter's passActor): the cell before it, the key held toward it
    GOAL_RIDE,   // G:ride:N  ride N of the adapter's ride table: its start tile (on its floor), the key held into it, the ride's end
};

typedef struct {
    int kind;    // GOAL_*
    int x, z;    // GOAL_TILE
    int n;       // the zone (GOAL_WARP), the actor's id (GOAL_NPC, GOAL_ITEM), the index (GOAL_TRIG, GOAL_BG), the line
                 // (GOAL_RAIL: x front, z side)
    int talk;    // GOAL_NPC, GOAL_ITEM, GOAL_BG: press A facing it and wait for the event (0: ":nota")
    int door;    // GOAL_WARP from G:door:I: n is the warp's index in this zone's list (the dump's DI), done when the zone
                 // changes (a multi-room cave: G:warp:N takes the nearest of all the doors to N)
} PortRouteGoal;

typedef struct {
    int x, z;           // the waypoint: the end of the path's first straight run
    int nextX, nextZ;   // the path's first tile
    int steps;          // the path's length in tiles
    int final;          // the waypoint is the goal
    int press;          // at the goal: the direction (0 up 1 down 2 left 3 right) that walks into the warp, or (npc, item,
                        // bg) that faces the target; -1 none
    int dir;            // on a rail (PortMap's rail): the first step's direction (0 up 1 down 2 left 3 right) ...
    PortRailPos rnext;  // ... and the place it leads to; the waypoint fields are the player's grid tile then
    int passX, passZ;   // G:wall at the cell before the wall (PLAN_THERE): the tile the pass ends on
    int gear;           // the first straight run climbs a bike slope and the bicycle is in its slow gear (PortMap.cycle 1): the pilot
                        // presses B once (the gear toggles), then plans again
    int dismount;       // the bicycle's next step is onto a PORT_MAP_NOBIKE tile (a rider is refused it): the pilot gets off (the
                        // registered bicycle's key, the game's own way), then plans again on foot
    int wait;           // PortMap's wind: stand here until the gust has passed, then plan again (why says so)
    int handoff;        // the goal is a hybrid zone's hand-off (the target is on the other side, rail or grid): hold press
                        // until the player is on the other side, then plan again
    char why[320];      // PLAN_NONE: why
} PortRoutePlan;

const PortMap *PortRouteMap_Get(void);   // fills the map now (m->ok 0: no field)
// F:strength:UID:DIR / F:push: the tile to push actor m->npc[i] from in dir (0 up 1 down 2 left 3 right): next to its
// footprint on the far side, any of its w (or h) tiles there (the game pushes from any: FieldPlayerGrid_FindFrontActor),
// the one nearest the player by steps over the pass bits (other actors' tiles not crossed). 1 and *x,*z; 0 none
// reachable (*x,*z then the footprint's first tile there, for the walk's own "no path")
int PortRouteMap_PushFrom(const PortMap *m, int i, int dir, int *x, int *z);
int PortRouteMap_Dump(const char *game, const char *why, unsigned vblank);
// the pilot's remaining tokens, for the next dump: the G tokens in them are the targets its window covers
void PortRouteMap_SetTargets(const char *rest);
// a G token (the text up to its end: a space or the string's end) to a goal: 1, or 0 with why (a bad token)
int PortRoute_ParseGoal(const char *tok, PortRouteGoal *g, char *why, int whyN);
int PortRoutePlan_Next(const PortRouteState *st, const PortRouteGoal *goal, PortRoutePlan *out);
void PortRoutePlan_Block(unsigned zone, int x, int z);
void PortRoutePlan_BlockRail(unsigned zone, const PortRailPos *p);
void PortRoutePlan_Reset(void);
// port/src/route_slide.c: a step in d onto window tile (*x, *z) that starts a slide: 1 and the tile it stops on in *x, *z
int PortRouteSlide_End(const PortMap *m, const unsigned char *block, int d, int *x, int *z);
// a cannon tile (cx, cz) shoots the player that way when it holds d: 1 and the landing in *x, *z (a free walk tile in the window)
int PortRouteSlide_Cannon(const PortMap *m, const unsigned char *block, int cx, int cz, int d, int *x, int *z);
// a bike slope run (route_slide.c): the bicycle in its fast gear gains one speed per step in a line, and the slope carries it up only
// at speed 3: so from window tile (cx, cz) the first slope tile of the column must be PORT_MAP_BIKESLOPE_RUN steps up (plain floor
// between) and the slope goes on up to a plain landing tile. 1 and the landing in *x, *z, the cost (the planner's units, 2 a tile)
#define PORT_MAP_BIKESLOPE_RUN 3
int PortRouteSlide_BikeRun(const PortMap *m, const unsigned char *block, int cx, int cz, int *x, int *z, int *cost);
// PortMap's wind: the steps a walk has before the gust (the gust's event checks per step, less a margin)
#define PORT_GUST_STEP 4
#define PORT_GUST_MARGIN 8
// with the wind on and no goal tile (goal[c]) within the steps before the gust: 1 a hop to the shelter within those steps
// that is fewest steps from a goal (path[0 .. *n - 1]: start .. it), 2 wait at start (no shelter in reach is nearer), 0
// the plain plan (a goal is in reach, or no shelter is: the gust blows the player back and the pilot plans again)
int PortRouteWind_Hop(const PortMap *m, const unsigned char *block, const unsigned char *goal, int start, int *path, int *n);

#endif
