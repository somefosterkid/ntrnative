// The route pilot's story readout (port/src/route_story.c; docs/port-worker.md, "Routes"): what the story wants next. The
// map dump's "story" section lists the event flags and work values that changed since the pilot's start, and for this
// zone's actors, triggers, BG events and scene scripts the flag or work value that gates each and whether it is met now;
// the pilot's `Ws:` token waits on one. Each game's game_event.c stand-in fills it (port/include/port_route_story_game.h:
// the game's EventWork and its zone's EventData). Test-only, as the rest of port_route.h.
//
// Gen 4 later (Platinum, HeartGold/SoulSilver, Diamond/Pearl; pret's pokeplatinum / pokeheartgold, names to check there):
// the same readout needs their flags-and-vars save block (pokeplatinum's VarsFlags, VarsFlags_CheckFlag and
// VarsFlags_GetVarAddress; vars from 0x4000, temp flags reset per map) and names from their flag and var constants; an
// actor's gate is its object event's hidden flag, a trigger's its coord event's var and value; the map's "on frame"
// init scripts (var == value -> script) are the twin of Gen 5's scene table. Fill a PortStory from those, nothing else.
#ifndef BWPORT_PORT_ROUTE_STORY_H
#define BWPORT_PORT_ROUTE_STORY_H

#include <stdio.h>

#define PORT_STORY_WORK0 0x4000   // the first work value's ID (works[0])
#define PORT_STORY_TEMP0 0x4000   // the first temporary flag (not saved: reset at each zone's load)
#define PORT_STORY_NTEMP 64

typedef struct {
    int ok;                       // filled (the game data is up)
    unsigned zone;
    const unsigned char *flags;   // the saved flags, a bit each: flag n is bit n % 8 of byte n / 8
    int nflags;                   // their count (bits)
    const unsigned short *works;  // the work values from PORT_STORY_WORK0
    int nworks;
    unsigned char temp[PORT_STORY_NTEMP / 8];   // the temporary flags now
    // the zone's events (only when the fill is asked for them)
    int nnpc, nlive, nbg, ntrig, nscene, ninit;
    struct {
        unsigned uid, flag, script, ev;   // ev: the actor's event type (trainer, item ...)
        int x, z, rail;
    } npc[64];
    unsigned live[64];            // the UIDs of the actors on the map now
    struct {
        unsigned script, cond, flag;      // cond 2: a hidden item, flag its "found" flag (0: none)
        int x, z, rail;
    } bg[64];
    struct {
        unsigned script, work, value, type;
        int x, z, w, h, rail;
    } trig[64];
    struct {                      // the scene table (init script type 1): checked every field frame, the first
        unsigned work, value, script;   // entry whose work has its value runs
    } scene[32];
    struct {
        unsigned type, script;    // the zone's other init scripts (2 zone init, 3 new load, 4 reload ...)
    } init[16];
} PortStory;

// the game's filler (set by the game_event.c stand-in each field frame); full: the zone's events too
extern void (*gPortRouteStoryFill)(PortStory *s, int full);

// each field frame (the game_event.c stand-in): the snapshot at the first, then each change's V-blank and zone
// (BWPORT_STORYLOG=1 also prints a "story: ..." line each)
void PortRouteStory_Frame(const char *game);
// the map dump's story section (port/src/route_map.c)
void PortRouteStory_Print(FILE *f, const char *game);
// the pilot's Ws:SPEC (route_test.c): 1 met, 0 not (yet). SPEC: a flag (name or number) set, !FLAG cleared,
// WORK=V a work value (name or number from 0x4000) equal to V
int PortRouteStory_Cond(const char *game, const char *spec);
// a flag's or work value's name from the decomp's constants (NULL: none)
const char *PortRouteStory_Name(const char *game, unsigned id);
int PortRouteStory_Lookup(const char *game, const char *name);   // -1: no such name

#endif
