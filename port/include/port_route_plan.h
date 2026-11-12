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
};

typedef struct {
    int kind;    // GOAL_*
    int x, z;    // GOAL_TILE
    int n;       // the zone (GOAL_WARP), the actor's id (GOAL_NPC, GOAL_ITEM), the index (GOAL_TRIG, GOAL_BG)
    int talk;    // GOAL_NPC, GOAL_ITEM, GOAL_BG: press A facing it and wait for the event (0: ":nota")
} PortRouteGoal;

typedef struct {
    int x, z;           // the waypoint: the end of the path's first straight run
    int nextX, nextZ;   // the path's first tile
    int steps;          // the path's length in tiles
    int final;          // the waypoint is the goal
    int press;          // at the goal: the direction (0 up 1 down 2 left 3 right) that walks into the warp, or (npc, item,
                        // bg) that faces the target; -1 none
    char why[320];      // PLAN_NONE: why
} PortRoutePlan;

const PortMap *PortRouteMap_Get(void);   // fills the map now (m->ok 0: no field)
int PortRouteMap_Dump(const char *game, const char *why, unsigned vblank);
// a G token (the text up to its end: a space or the string's end) to a goal: 1, or 0 with why (a bad token)
int PortRoute_ParseGoal(const char *tok, PortRouteGoal *g, char *why, int whyN);
int PortRoutePlan_Next(const PortRouteState *st, const PortRouteGoal *goal, PortRoutePlan *out);
void PortRoutePlan_Block(unsigned zone, int x, int z);
void PortRoutePlan_Reset(void);

#endif
