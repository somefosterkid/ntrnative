// The route pilot's test cheats (tokens X:money:N, X:item:ID:N, X:mon:SPECIES:LEVEL, X:badge:N; port/src/route_test.c): the pilot
// queues one here and the games' field hook (port/include/port_route_story_game.h, PortStory_Cheat) runs it on its next field frame
// with the game's own functions (money, bag, party, trainer card), then sets done. Test-only, like the battle cheat.
#ifndef PORT_ROUTE_CHEAT_H
#define PORT_ROUTE_CHEAT_H

enum { PORT_CHEAT_MONEY = 1, PORT_CHEAT_ITEM, PORT_CHEAT_MON, PORT_CHEAT_BADGE, PORT_CHEAT_WORK, PORT_CHEAT_FLAG,
       PORT_CHEAT_SCRIPT, PORT_CHEAT_WARP, PORT_CHEAT_HEAL };

typedef struct {
    int kind;      // PORT_CHEAT_*
    int a, b;      // money N | item ID, count | species, level | badge N | work ID, value | flag ID, 0/1 | zone, script ID | zone, x
    int c, d;      // warp: z, direction (0 up 1 down 2 left 3 right)
    int after;     // warp: 1 the map change is queued, 2 it runs: when it ends the fade-in a script's warp is followed by runs
    int pending;   // queued by the pilot, not run yet
    int done;      // run by the hook (a script: its event is started; 2 = refused, wrong zone)
} PortRouteCheat;
extern PortRouteCheat gPortRouteCheat;

#endif
