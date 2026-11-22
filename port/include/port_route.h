// Test-only hooks for the playthrough routes (docs/port-worker.md, "Routes"): all off unless their environment variable is
// set, none in the UI or launcher.ini. The game's own stand-ins (port/extern*/src/system/game_event.c, field_encount.c)
// call these; port/src/route_test.c has them.
#ifndef BWPORT_PORT_ROUTE_H
#define BWPORT_PORT_ROUTE_H

#if defined(PLATINUM)   // Gen 4: the modules compiled with pret's headers (port/game/route_gen4.c, to come: docs/platinum.md, docs/hgss.md)
#define PORT_ROUTE_GAME "PT"
#elif defined(HEARTGOLD)
#define PORT_ROUTE_GAME "HG"
#elif defined(SOULSILVER)
#define PORT_ROUTE_GAME "SS"
#elif defined(BLACK2)
#define PORT_ROUTE_GAME "B2"
#elif defined(WHITE2)
#define PORT_ROUTE_GAME "W2"
#elif defined(WHITE)
#define PORT_ROUTE_GAME "W"
#else
#define PORT_ROUTE_GAME "B"
#endif

typedef struct {
    const char *game;    // "B", "W", "B2", "W2", "PT", "HG", "SS"
    unsigned zone;       // the player's zone ID
    int gx, gz;          // the player's grid position (the world position / 16 tiles' fixed point)
    int event;           // a game event is running (a scene, a menu); route_test.c makes it 2 in a battle (A, not B)
    int save;            // the field menu save's state (EventSave_Callback's), -1 when no save runs
    int text;            // the text streams printing now (Black 2, White 2: their printsys.c stand-in), -1 not known
    int fx, fz;          // the world position in whole units (16 a tile): it changes every frame of a step
    int mud;             // Platinum: a deep-mud task runs (it ends after five turns): the key bits of a direction that turns the player, else 0
} PortRouteState;

// Black 2 / White 2 (port/extern_b2w2/src/ov036/field_menu.c): the touch screen's field menu, set at each of its updates
typedef struct {
    int input;           // it takes keys (FIELD_MENU_STATE_INPUT)
    int onSave;          // the cursor is on SAVE
    int grid;            // Black, White (port/extern/src/ov021/field_menu.c) also give the slots: the items' two columns (slot = x + 2 * y,
                         // 6 the bottom button) and the cursor's and SAVE's slot, so the pilot can go right and left too
    int slot, saveSlot;  // the cursor's slot, SAVE's (-1: the menu has none)
    int pokeSlot;        // the POKEMON entry's slot (Black, White; -1: none or not given): F:fly goes there, then A
    int bagSlot;         // the BAG entry's slot (Black, White; -1: none or not given): F:teach goes there, then A
    unsigned vblank;     // the V-blank of its last update (a closed menu stops updating)
    unsigned key;        // HeartGold / SoulSilver (port/game/route_gen4.c): the key that moves the cursor a step toward SAVE on
                         // the game's own move table (0: not given, the grid's way)
    unsigned pokeKey;    // HeartGold / SoulSilver: the same toward the POKEMON entry (F:fly)
} PortRouteMenu;
extern PortRouteMenu gPortRouteMenu;
// The battle's menus (port/extern/src/ov093/btlv_core.c, port/extern_b2w2/src/ov167/btlv_core.c: stand-ins of the action menu's poll
// and the Bag's update), for the pilot's C:ball (a wild catch): each is the V-blank of its last update (a closed menu stops updating)
typedef struct {
    unsigned menuVblank;  // the action menu (Fight, Bag, Pokemon, Run) is polled for its answer
    unsigned bagVblank;   // the Bag overlay runs (BtlvCore's subSeq 3 of the item select)
    int bagItem, bagPocket, bagDone, bagBallError;   // the Bag's outputs (BBagParam: item, pocket, done; ballError: why no ball goes)
    int ballRow, ballPage;   // the Poke Balls pocket's cursor memory (BBagParam rows[2], pages[2]: the slot of a page, the page)
} PortRouteBattle;
extern PortRouteBattle gPortRouteBattle;
// C:ball:ITEM: the balls (items 1 to 15) of the bag's Items pocket in bag order, 0 ends it: port_route_story_game.h's hook, each field frame
// while C:ball runs (the battle bag lists them in that order)
extern int gPortRouteBalls[17];
// the game's cached clock (RTC_GetCachedDateTime) and its season (Season_GetRealTime), each field frame: month, day, hour, season (X:time)
extern int gPortRouteClock[4];
// C:ball is running: the pilot catches a wild Pokemon (the games' stand-ins keep the one-shot off and throw a sure catch under the test cheat)
extern int gPortRouteCatch;
// the party's size (port_route_story_game.h's hook, each field frame): C:ball checks that it rose
extern int gPortRoutePartyCount;
extern int gPortRouteFlySlot;   // the first party slot that knows Fly, -1 for none (the field hook, each frame)
extern int gPortRouteFlyMove;   // the move slot (0-3) Fly is in there; the list's submenu item for it is 16 + this
// F:fly (route_fly.h): the Pokemon list and its submenu (Black, White: port/extern/src/ov091/plist_sys.c, plist_menu.c) and the Town Map
// (port/extern/src/ov072/townmap.c) publish their state at each update; vblank is the V-blank of the last one (a closed screen stops updating)
typedef struct {
    unsigned vblank;      // the list's per-frame update
    int state, subState, input, cursorPos, selectPos, mode;   // PokeListWork's (state 2 + subState 1: the keys move the cursor), the param's mode
    unsigned menuVblank;  // the submenu's update (PokeListMenu: Summary, Switch, the field moves ... in items, 16-19 = move slots 0-3)
    int menuCount, menuItems[8], menuCursor;
} PortRoutePList;
extern PortRoutePList gPortRoutePList;
// F:teach (route_teach.h): the list's question (state 9: 1 forget a move for the new one?, 2 give up learning it?, 0 another),
// whether its message waits for A (state 7), the param's result and move; set by the list's hook with the rest
typedef struct {
    int ask, msgWait, result, move;
} PortRoutePListTeach;
extern PortRoutePListTeach gPortRoutePListTeach;
// F:teach: the Bag (Black, White: port/extern/src/ov070/itemmenu.c wraps its proc's table), each frame after the game's main
typedef struct {
    unsigned vblank;      // the last frame the proc ran (0: not yet)
    int alive;            // 1 while it runs, 0 after its end
    int phase;            // the state: 1 the list takes keys, 2 an item's menu (Use, Give ...), 3 a message waits for A,
                          // 4 the TM's yes/no (cursor 0 = yes), 5 a message waits for A, then the list; 0 another
    int pocket, index, count;   // the pocket (2: TMs and HMs), the cursor's index in it, its item count
    int items[128];       // the pocket's items in list order (the first 128)
    int menuCursor, menuActions[4], menuLive;   // the item menu's cursor and its entries' actions (0 = Use); live: a menu is up
    int result;           // the bag's result (6: teach the TM, then the Pokemon list)
} PortRouteBag;
extern PortRouteBag gPortRouteBag;
// F:teach: the summary's move page in its forget mode (port/extern/src/ov131/p_sta_skill.c), each frame of the page
typedef struct {
    unsigned vblank;
    int cursor, confirming, exiting, moveCount, newMove, mode;   // cursor 0-3 the moves, 4 the new one; mode 2 = forget a move
    int moves[4];
} PortRouteSkill;
extern PortRouteSkill gPortRouteSkill;
// the party's species and moves, each field frame (port_route_story_game.h): F:teach finds its Pokemon and checks the move
extern int gPortRouteParty[6][5];
// the Town Map's proc (port/extern/src/ov072/townmap.c wraps its GameProcFunctions): each frame after the game's own main
typedef struct {
    unsigned vblank;      // the last frame the proc ran (0: not yet)
    int alive;            // 1 while the proc runs, 0 after its end
    int grid;             // Platinum: cx, cy and the places' x, y are cells of the map's grid (the cursor moves one a key; no scroll, no pixel slack)
    int mode, result, zoomed, moving;   // TOWNMAP_MODE_* (FLY = only the visited towns), TOWNMAP_RESULT_* (the pick: FLY), the zoom, the zoom's animation
    int zone, flyZone;    // the player's zone; the zone picked (when result is FLY)
    unsigned warpX, warpZ;
    int cx, cy, sx, sy;   // the cursor's position (screen), the places' scroll
    int hit, ring, selected, count;   // the place index under the cursor, the ring's, the selected one (-1 none); the places' count
    struct { int zone, x, y, canFly, hidden; } p[64];   // x, y: where the cursor goes for the place (add the scroll)
} PortRouteTownMap;
extern PortRouteTownMap gPortRouteTownMap;
// Black 2 / White 2 (port/extern_b2w2/src/system/printsys.c): the text streams printing now (the game's side calls it)
int PortRoute_TextPrinting(void);

// BWPORT_NOENCOUNTER=1: no wild encounters while walking or surfing (the one boot line says so)
int PortRoute_NoEncounter(const char *game);
// BWPORT_TESTCHEAT=win: the player's side moves first, one-shots and takes no damage (docs/port-worker.md, "Test cheat")
int PortRoute_CheatWin(const char *game);
// The battle pilot's move (the games' btl_client.c stand-ins, port/extern/src/ov093 and port/extern_b2w2/src/ov167): of n moves,
// slot keep (the move the A press chose; -1 none) when it is usable (PP left, not disabled) and can hurt the target, else the
// usable one with the best type effectiveness x base power against the target. eff is the game's effectiveness code (0 immune,
// 1 x1/4, 2 x1/2, 3 x1, 4 x2, 5 x4; a status move is passed as 0), a damaging move's power counts as 1 at least (fixed-damage
// moves). The slot, the lower one on a tie; -1 when no move hurts the target (the caller switches).
int PortRoute_BestMove(int n, const int *eff, const int *power, const int *usable, int keep);
// BWPORT_TESTCHEAT=win also: the party knows the field moves (Cut, Fly, Surf, Strength, Waterfall, Flash, Rock Smash, Dive);
// nonzero when the cheat is on and move is one of them
int PortRoute_CheatFieldMove(const char *game, unsigned move);
// Called once a frame with the player's state: BWPORT_POSLOG=1 prints "pos: GAME Z zone N x,z" when it changes,
// BWPORT_PILOT="tokens" presses the keys (port/src/route_test.c's header has the tokens)
void PortRoute_Frame(const PortRouteState *st);
void PortRoute_VBlank(void);   // port/src/main_threads.c: the pilot before the field's first frame
// port/src/title_fork.c's forked leg: the pilot goes on in the leg's own BWPORT_PILOT, at the same offset (the tokens before
// the fork point are the same in every leg that shares the snapshot; a leg whose differ ends with "rebase")
void PortRoute_PilotRebase(const char *full);

#endif
