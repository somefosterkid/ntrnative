// Test-only hooks for the playthrough routes (docs/port-worker.md, "Routes"): all off unless their environment variable is
// set, none in the UI or launcher.ini. The game's own stand-ins (port/extern*/src/system/game_event.c, field_encount.c)
// call these; port/src/route_test.c has them.
#ifndef BWPORT_PORT_ROUTE_H
#define BWPORT_PORT_ROUTE_H

#if defined(BLACK2)
#define PORT_ROUTE_GAME "B2"
#elif defined(WHITE2)
#define PORT_ROUTE_GAME "W2"
#elif defined(WHITE)
#define PORT_ROUTE_GAME "W"
#else
#define PORT_ROUTE_GAME "B"
#endif

typedef struct {
    const char *game;    // "B", "W", "B2", "W2"
    unsigned zone;       // the player's zone ID
    int gx, gz;          // the player's grid position (the world position / 16 tiles' fixed point)
    int event;           // a game event is running (a scene, a menu); route_test.c makes it 2 in a battle (A, not B)
    int save;            // the field menu save's state (EventSave_Callback's), -1 when no save runs
    int text;            // the text streams printing now (Black 2, White 2: their printsys.c stand-in), -1 not known
    int fx, fz;          // the world position in whole units (16 a tile): it changes every frame of a step
} PortRouteState;

// Black 2 / White 2 (port/extern_b2w2/src/ov036/field_menu.c): the touch screen's field menu, set at each of its updates
typedef struct {
    int input;           // it takes keys (FIELD_MENU_STATE_INPUT)
    int onSave;          // the cursor is on SAVE
    unsigned vblank;     // the V-blank of its last update (a closed menu stops updating)
} PortRouteMenu;
extern PortRouteMenu gPortRouteMenu;
// Black 2 / White 2 (port/extern_b2w2/src/system/printsys.c): the text streams printing now (the game's side calls it)
int PortRoute_TextPrinting(void);

// BWPORT_NOENCOUNTER=1: no wild encounters while walking or surfing (the one boot line says so)
int PortRoute_NoEncounter(const char *game);
// BWPORT_TESTCHEAT=win: the player's side moves first, one-shots and takes no damage (docs/port-worker.md, "Test cheat")
int PortRoute_CheatWin(const char *game);
// Called once a frame with the player's state: BWPORT_POSLOG=1 prints "pos: GAME Z zone N x,z" when it changes,
// BWPORT_PILOT="tokens" presses the keys (port/src/route_test.c's header has the tokens)
void PortRoute_Frame(const PortRouteState *st);
void PortRoute_VBlank(void);   // port/src/main_threads.c: the pilot before the field's first frame

#endif
