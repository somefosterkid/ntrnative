// Test hooks for the playthrough routes (docs/port-worker.md, "Routes"; tools/route.py). Everything here is off unless its
// environment variable is set; none of it is in the UI or launcher.ini.
//
// BWPORT_NOENCOUNTER=1  no wild encounters while walking or surfing, as a Repel would (the stand-ins of the games'
//                       EventWildBattleCall_CreateRandom); trainers, scripts and static encounters still happen.
// BWPORT_TESTCHEAT=win  battles play in full, but the player's side moves first, its attacks one-shot and it takes no
//                       damage (the stand-ins of the games' battle_event.c). The party also knows every field move
//                       (HMs): the script command PokePartyHasMoveAny and the field's Surf / Waterfall checks answer as if
//                       party slot 0 had it (the stand-ins of scrcmd_pokemon.c and fieldmap_event.c).
// BWPORT_POSLOG=1       "pos: GAME Z zone N x,z" each time the player's zone or grid position changes; the last one is
//                       where a run ended (the route check). A pilot's end prints one more.
// BWPORT_PILOT="..."    the player's keys from the game thread, once a frame, so the same script always plays the same
//                       frames (the key script of BWPORT_KEYS runs in its own thread and jitters by a frame or two). Tokens,
//                       separated by spaces, run in order:
//                         X,Z       walk to grid X,Z (x first, then z); while an event runs (a scene, text) press the pulse
//                                   key (B) every 4th frame; blocked 240 frames: try the other axis first
//                         zX,Z      the same, z first
//                         X,Z!      the same, and stop at once when the zone changes (a door or gate on the way)
//                         Kkeys:N   hold keys N frames whatever runs (a b e=select s=start R L U D = the d-pad, r l = the
//                                   shoulders, x y); K0:N holds nothing
//                         Wn        wait until no event has run for n frames (pressing the pulse key while one does)
//                       The event waits (they wait on the game's state, not on a count of frames, so a leg plays the
//                       same with the speed options on: Instant text, fast walking, fast battles):
//                         Wf[:N]    the field is free: no event for N frames (default 8)
//                         Wt        no text is printing (Black 2, White 2: the text streams; Black, White: Wf:2); presses
//                                   nothing while it waits
//                         Wr        the game resets itself (the in-process restart, machine.c: the end of Platinum's credits);
//                                   presses through until then; follow it with `Mz:ZONE` (the title and CONTINUE of the new boot)
//                         Wb        a battle starts and ends, then the field is free (30 frames); presses through
//                         Wz:N      the player is in zone N and the field is free (8 frames); presses through
//                         Ws:FLAG   a story beat: the event flag FLAG is set (a name or a number), then the field is
//                                   free (8 frames); Ws:!FLAG cleared, Ws:WORK=V a work value equal to V; presses
//                                   through (port/src/route_story.c; the map dump's story section lists them)
//                         To:N      the next token may run N frames before "timeout" (default BWPORT_PILOT_TIMEOUT, 20000):
//                                   one long walk need not be split (`To:60000 G:npc:11`); a G:zone's hops get it only on
//                                   the first (route.py expands G:zone before the run)
//                         Mn        press the pulse key every 4th step for n steps, whatever runs (a title, an intro)
//                         Mz:N      the same until the field is up in zone N with no event for 30 frames (NEW GAME's title, intro
//                                   and the room's scene: `Pa Mz:391`), at most BWPORT_PILOT_TIMEOUT steps ("timeout")
//                         Pa Pb     the pulse key during events: A, or B (the default)
//                         Py Pn     a question in the field (a script's yes/no list, not a battle's): press the pulse key
//                                   through the event until the yes/no list is up, then answer Yes (A; up first when the
//                                   cursor starts on No) or No (B), and go on once the list has the answer; no list in
//                                   BWPORT_PILOT_HANG frames (3000): "pilot: hang: Py: no yes/no list" and the end.
//                                   Also a two-item list (Black 2, White 2: a script's list menu such as the C-Gear's
//                                   on/off question, which B does not cancel; Yes is the first row): up or down
//                                   onto the answer, then A
//                         Pl:N      a script's list menu (not a yes/no): press through the event until it is up, move the cursor
//                                   onto row N (1-based, the order the script adds them) with up / down, then A; Pl:0 is B (the list's
//                                   cancel; some lists take none). The answer's value is logged ("pilot: Pl:2 done ...: the answer is
//                                   1"); N past the list's rows: "hang" and the end; no list in BWPORT_PILOT_HANG frames: "hang" too.
//                                   A menu of columns (HeartGold's floor menus: gPortRouteListHigh rows a column, the rows counted
//                                   down each column in turn) is crossed left / right to the row's column first
//                         Pk        the naming screen (a Pokemon's nickname, the player's name): press through the event until it is
//                                   up, wait 120 frames, then the keys A, Start (the cursor onto OK), A, one every 8th frame, again
//                                   until the screen has closed: the name is "A" at most one letter (the player's name must not be
//                                   empty). The nickname question before it is a yes/no: Py first (Pa alone mashes A through the
//                                   screen: seven As)
//                         Pk:pw     White 2's Plasma Frigate terminal (zone 563 BG 0, after its yes/no: `G:bg:0 Py Pk:pw`): as Pk, but
//                                   the naming screen's stand-in (port/extern_b2w2/src/ov280/name_input.c) types the password the
//                                   game asked for (the message its GetPasswordIndex picked, work 0x400a) into the name box, and the
//                                   keys are Start (the cursor onto OK) and A only
//                         S         the field menu's SAVE: wait for the field, X, the menu's input, down until the cursor is on
//                                   SAVE (the menu's real state: its entries differ before the first Pokemon and before the
//                                   Pokedex), A, A at the save's questions and B until the field is free again
//                         E:on E:off  wild encounters on / off from here on (the legs start with them off: NOENCOUNTER)
//                         F:cut:UID F:smash:UID F:strength:UID[:DIR]  F:push:UID:DIR[:N]  F:stone:X,Z:DIR  F:surf:X,Z:DIR   field moves used as the
//                                   player does (A on the target, Yes; the move comes from the test cheat): the header of
//                                   port/src/route_fieldmove.h says what each does (Strength pushes a boulder, Surf starts surfing)
//                         F:fly:ZONE  Fly to the Town Map place of zone ZONE through the menus (Black, White; port/src/route_fly.h):
//                                   a party member must know Fly (move 19: X:mon:SPECIES:LEVEL:LEAD:19), the town visited
//                         F:teach:ITEM:SPECIES[:FORGET]  teach the TM/HM ITEM from the Bag to the first party member of SPECIES
//                                   through the menus (Black, White; port/src/route_teach.h); FORGET (a move id) when it knows 4 moves
//                         X:money:N X:item:ID:N X:mon:SPECIES:LEVEL[:1[:MOVE]] X:badge:N   test cheats (numbers: item, species): set the money,
//                                   add N of an item to the bag, add a party member (:1 = as the lead: the battle's first move is
//                                   its first move), add a badge; X:heal heals the party as a Pokemon Center does (HP, status,
//                                   PP: a lead whose first move ran dry before the battle cheat kept PP); run on the next field frame by
//                                   the game's own functions (port_route_story_game.h, port_route_cheat.h), one "cheat:" log line
//                         X:work:ID:VAL X:flag:ID:0|1   set an event work value / clear or set an event flag (ID as story.py prints it)
//                         X:warp:ZONE:X:Z[:DIR]   (a test save only) the game's own map change (EventMapChange_CreateGridDefault, the way a
//                                   scripted warp does it) to a grid zone's tile, facing DIR (0 up 1 down 2 left 3 right, default down),
//                                   once no event runs; follow it with `Wf:60`. Rail zones (the city plazas) are not supported
//                         X:time:MONTH:DAY:HOUR   move the game's clock: BWPORT_BENCH_CLOCK (route.py pins it) is set again, libntr's
//                                   getPCNow reads it on every call (libntr-ios-benchpin.patch), the game's RTC cache follows within 11
//                                   frames; done when it reads the new time ("pilot: X:time: the game reads M/D H:00, season S"; the
//                                   season is (month - 1) % 4). The year stays the pinned one
//                         R:script:ZONE:ID   run the zone's script ID the way the field starts a trigger's (EventScriptCall_Create, no
//                                   actor) once no event runs, then press through its event until the field is free for 8 frames
//                                   ("script zone" end when the player is not in ZONE): the fallback for a puzzle, say so in the leg.
//                                   Before Py, Pn, Pl:N or Pk it only starts the script and goes on (those wait for the event's own
//                                   question), so `R:script:10:6 Pl:2` answers the script's list
//                         C:ball[:ITEM]  catch a wild Pokemon with the first Poke Ball, or with the ball ITEM (a number, as X:item:
//                                   takes it; it must be in the bag's Items pocket and on the first page of the Bag's balls: the
//                                   pilot keys to its slot, "pilot: C:ball: item N is slot S" says where, and the leg fails when
//                                   another ball goes): encounters on, one step left and right
//                                   (grass) until a wild battle starts, then in it: A through the text until the action menu
//                                   waits, Bag, the Poke Balls pocket, the first ball, Use (again while the menu comes back);
//                                   done when the battle is over and the field is free, with the party's count logged
//                                   ("pilot: C:ball: party N -> M"; "catch failed" and the end when it did not rise). The
//                                   test cheat makes the throw a sure catch (port/extern/src/ov093/btl_server_flow_sub.c)
//                         Cc        before a G:npc / G:item / G:bg: the battle that talk starts (a scripted wild battle, a
//                                   legendary) is a catch, not a knock-out: CatchStep runs in it (the Bag's first ball, no
//                                   attack: the test cheat's one-shot is off) and the nickname question after it is No (Pn)
//                         C         end the run here: print the position, quit (the script's end does too); before the
//                                   field is up (a leg's start) it waits for it, pressing the pulse key
//                         Bk        Platinum: put the bicycle (the bag must hold it: the story gives it) on the Y button as the bag's
//                                   Register does; `Bk Ky:4 Wf:60` then mounts it (Ky:4 again dismounts), as the player's Y does
//                         Md        write a map dump now (port/src/route_map.c: the walkable grid, warps, triggers, actors);
//                                   it waits for the field's own frame (a leg's start, the Bag, a scene's own proc)
//                         T:X,Y[:N] tap the DS touch screen at pixel X,Y (0..255, 0..191) for N frames (default 4), then 2 frames
//                                   up; T:X,Y>X2,Y2[:N] a drag from the first point to the second over N frames. Presses
//                                   nothing else, whatever runs (the stylus-only buttons: libntr-touchforce.patch)
//                         G:X,Z     walk to grid X,Z on a planned path (A* on the game's own terrain and collision, actors
//                                   in the way, ledges one way; port/src/route_map.c), across zone edges where the map
//                                   goes on (the tiles not loaded yet are tried), re-planned at each corner and when a
//                                   step does not happen; "pilot: no path: ..." and the end when there is none.
//                                   Platinum: a bike slope is climbed by the bicycle (Bk Ky:4 first): the planner has an edge for it, and
//                                   the walk presses B once for the fast gear; a trainer who turns at random is waited out at the edge
//                                   of his line of sight (the next tile on his line, a frame count, 600 at most): docs/platinum.md
//                         G:warp:N  the same to a warp leading to zone N (onto it, or next to it and into it, from the
//                                   side its direction needs: a door or walk-in warp), or where
//                                   no warp of this zone leads there, to the nearest tile of zone N on the map matrix;
//                                   done when the player is in zone N
//                         G:npc:N   walk next to the actor of uid N (the dump's "N uid N" lines; the UID the zone's
//                                   scripts use, not the list's order), or across a counter from it, turn to face it,
//                                   press A and wait for the event it starts to end (pressing the pulse key: Pa / Pb)
//                                   until the field is free for 8 frames (Wf). G:npc:N:nota walks there and faces it only
//                         G:item:N  the same for an item ball (its actor's uid N); G:item:N:nota
//                         G:trig:N  walk onto trigger N (the dump's TN: the index in the zone's list); done on its tile,
//                                   or when the scene it started (an event that began on its tiles) has ended, even if
//                                   the scene moved the player off them (a scene that turns the player back)
//                         G:bg:N    next to BG event N (the dump's bN), on the side its direction reads it from (a hidden
//                                   item from any), facing it, A, the event's end; G:bg:N:nota
//                         G:wall:N  through the pass-through actor of uid N (the dump's "N uid N" line; the game's adapter says which
//                                   actors: Castelia Gym's honey walls): walk to the cell before it, on the player's side, hold the
//                                   key toward it until the player is on the tile past it (Black, White: 2 past the wall's cell; a
//                                   longer hold walks on into the next trigger), or "wall N closed" when no step is taken for
//                                   120 frames (its switches are not all pressed yet). Follow it with Wf
//                         G:ride:N  ride N of the game's ride table (the adapter's: Black 2 and White 2's Castelia Gym cocoons,
//                                   N the index in gym_insect.c's sRides): walk to its start tile on its floor (another floor:
//                                   "no path" at once), hold the key into it until the ride's event starts ("ride N did not
//                                   start" after 120 frames), press through the ride and what it starts until the field is
//                                   free for 8 frames; "pilot: G:ride:N landed at X,Z y Y zone Z" (the next G plans there).
//                                   Nimbasa's roller coaster (Black, White, Black 2, White 2): N the spot 0..8 (zone 63 / 585): stand
//                                   on it facing up, wait (2400 frames) for the car, ride to the next station and hold Down there;
//                                   "switch T<n> wrong for spot N" when the spot is off the car's path (G:trig:T throws the switch)
//                       A target token that cannot be done (no such actor, no path, no turn, no event after A three
//                       times) prints "pilot: no path: WHY (G token ...)", writes a map dump and ends the run.
//                       In a battle every token presses A, except at the forget-a-move and the "switch
//                       Pokemon?" questions: B, No ("pilot: battle yes/no question: answering No"; the games' btlv_core.c
//                       stand-ins set gPortRouteYesNo). The A's move (the move menu's cursor) is kept while it can hurt the
//                       target; only a move that cannot (immune, no PP, disabled, a status move) is replaced by the usable one
//                       that hurts it most, and with no such move the lead switches to the first party member that has one
//                       (PortRoute_BestMove; the games' btl_client.c stand-ins: "pilot: battle move: ..." / "pilot: battle
//                       switch: ..."). Battles play as they did before the stand-ins, so the saved legs keep their frames.
// BWPORT_MAPDUMP=1 (or =DIR): a map dump at the run's end (stuck, timeout, the script's end: with a screenshot when
// stuck) and at each Md, as DIR/mapdump-NN-WHY.txt (DIR: BWPORT_SHOTS's, else the working dir).
// A walk stuck for 2000 frames, a token not done in BWPORT_PILOT_TIMEOUT frames (default 20000: a looping scene, a walk
// that overshoots), or a script that ends, prints "pilot: ..." and "pos: ..." and quits at once. The no-progress check:
// the same token, zone, tile and event state for BWPORT_PILOT_HANG frames (default 3000; 4x that in a battle) while the
// pilot presses through an event prints "pilot: hang: TOKEN, event E, zone Z x,z, no progress in N frames", writes a map
// dump and ends (a scene that loops, a text box the pulse key does not close). A G walk logs each event it presses
// through: "pilot: event during G at V-blank N (zone Z, x,z)".
#include <nitro.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "port_game.h"
#include "port_ovl.h"
#include "port_route.h"
#include "port_route_cheat.h"
#include "port_route_cheat.h"
#include "port_route_map.h"
#include "port_route_plan.h"
#include "port_route_story.h"

static int Env(const char *name) {
    const char *e = getenv(name);

    return e != NULL && *e != '\0' && *e != '0';
}

int gPortRouteYesNo;   // port/extern/src/ov093/btlv_core.c (Black, White), port/extern_b2w2/src/ov167/btlv_core.c (Black 2, White 2):
                       // a battle's yes/no is up: 1 answer A, 2 answer B
int gPortRouteFieldYesNo;   // port/extern/src/ov021/field_msgbg.c (Black, White), port/extern_b2w2/src/ov036/field_msgbg.c (Black 2,
                            // White 2): the field's yes/no list is up (a script's question): 1 + its cursor's row (0 Yes, 1 No); 3 + the row:
// a two-item list menu (Black 2, White 2: a script's list such as the C-Gear's question), its row live, B no answer
int gPortRouteList;         // port/extern/src/ov021/field_msgbg.c, port/extern_b2w2/src/ov036/field_msgbg.c: a script's list menu (Pl:N) is up:
int gPortRouteListCount;    // 1 while it has no answer; its row count, its cursor's row (0-based, live), and the last answer's value
int gPortRouteListRow;      // (BMPMENULIST_CANCEL, -2, for B)
int gPortRouteListResult;
int gPortRouteListHigh;     // the list's rows a column when it has columns (HeartGold's 2D field menus, row = column * high + its
                            // place in it), 0 one column (port/game/route_gen4.c PortRoute4_FieldMenu)
int gPortRouteNameIn;       // port/extern/src/ov194/namein.c (Black, White), port/extern_b2w2/src/ov280/name_input.c (Black 2, White 2): 1 while the
                            // naming screen's proc runs, from its init to its exit (Pk)
int gPortRouteNamePassword; // 1 while Pk:pw runs: port/extern_b2w2/src/ov280/name_input.c types the Plasma Frigate's password
PortRouteMenu gPortRouteMenu;
PortRoutePList gPortRoutePList;     // F:fly: the Pokemon list (Black, White: port/extern/src/ov091) ...
PortRouteTownMap gPortRouteTownMap; // ... and the Town Map (port/extern/src/ov072/townmap.c) tell the pilot their state each frame
PortRouteBattle gPortRouteBattle;   // the battle's action menu and Bag (port/extern/src/ov093/btlv_core.c): C:ball
int gPortRouteCatch;                // C:ball runs (battle_event.c keeps the one-shot off; the pilot does not attack)
int gPortRoutePartyCount;           // the party's size, each field frame (port_route_story_game.h)
int gPortRouteFlySlot = -1;         // the first party slot that knows Fly (move 19), -1 none (port_route_story_game.h)
PortRoutePListTeach gPortRoutePListTeach;   // F:teach (route_teach.h): the list's question and message (port/extern/src/ov091)
PortRouteBag gPortRouteBag;                 // ... the Bag (port/extern/src/ov070/itemmenu.c)
PortRouteSkill gPortRouteSkill;             // ... the forget-a-move page (port/extern/src/ov131/p_sta_skill.c)
int gPortRouteParty[6][5];                  // ... the party: species, moves (port_route_story_game.h, each field frame)
int gPortRouteFlyMove = -1;         // its move slot (0-3): the list's submenu shows it as item 16 + move slot
int gPortRouteClock[4];             // the game's cached RTC: month, day, hour, Season_GetRealTime (port_route_story_game.h, each frame): X:time waits on it
int gPortRouteBalls[17];            // C:ball:ITEM: the balls of the bag's Items pocket in bag order, 0 ends it (port_route_story_game.h, while C:ball runs)

static int sEncOverride = -1;   // E:on 0 (encounters), E:off 1 (none), -1 the environment's (BWPORT_NOENCOUNTER)

int PortRoute_NoEncounter(const char *game) {
    static int sOn = -1;

    if (sOn < 0) {
        sOn = Env("BWPORT_NOENCOUNTER");
        if (sOn) {
            fprintf(stderr, "bwport: NOENCOUNTER on: no wild encounters (%s)\n", game);
        }
    }
    return sEncOverride >= 0 ? sEncOverride : sOn;
}

int PortRoute_BestMove(int n, const int *eff, const int *power, const int *usable, int keep) {
    int best = -1, bestScore = 0, i;

    // the move the A press chose stays while it can hurt the target (with the damage cheat any damage wins: the most
    // damaging move would change battles for nothing)
    if (keep >= 0 && keep < n && usable[keep] && eff[keep] > 0) {
        return keep;
    }
    for (i = 0; i < n; i++) {
        // eff 0 (immune) scores 0; 1..5 are x1/4 .. x4, so 1 << (eff - 1) is the factor in quarters
        int score = usable[i] && eff[i] > 0 ? (1 << (eff[i] - 1)) * (power[i] > 1 ? power[i] : 1) : 0;

        if (score > bestScore) {
            best = i;
            bestScore = score;
        }
    }
    return best;
}

int PortRoute_CheatWin(const char *game) {
    static int sOn = -1;

    if (sOn < 0) {
        const char *e = getenv("BWPORT_TESTCHEAT");

        sOn = e != NULL && strcmp(e, "win") == 0;
        if (sOn) {
            fprintf(stderr, "bwport: TESTCHEAT win on: player moves first, one-shots, takes no damage (%s)\n", game);
        }
    }
    return sOn;
}

// The field moves the scripts and the field's checks ask for (Cut, Fly, Surf, Strength, Waterfall, Flash, Rock Smash, Dive):
// with the test cheat on the party "knows" them (see the header)
int PortRoute_CheatFieldMove(const char *game, unsigned move) {
    switch (move) {
    case 15:
    case 19:
    case 57:
    case 70:
    case 127:
    case 148:
    case 249:
    case 291:
        return PortRoute_CheatWin(game);
    default:
        return 0;
    }
}

// game key bits (PAD_BUTTON_*) to the key register: KEYINPUT bits 0-9 (a b select start right left up down r l), active low;
// X and Y in HW_BUTTON_XY_BUF (bits 10, 11, active low; the game ORs the two words)
static void SetKeys(unsigned bits) {
    vu16 *xy = (vu16 *)HW_BUTTON_XY_BUF;

    s_reg_PAD_KEYINPUT = (u16)((s_reg_PAD_KEYINPUT & ~0x3ff) | (~bits & 0x3ff));
    *xy = (u16)((*xy & ~0xc00) | (~bits & 0xc00));
}

// the interaction sweep's presses (route_gen4.c Pr4_Int*): the same register write, any bits (0: all keys up)
void PortRoute_SetKeysExternal(unsigned bits) {
    SetKeys(bits);
}

static unsigned KeyBits(const char *k, const char **end) {
    unsigned b = 0;

    for (; *k != '\0' && *k != ':'; k++) {
        switch (*k) {
        case 'a': b |= 1; break;
        case 'b': b |= 2; break;
        case 'e': b |= 4; break;
        case 's': b |= 8; break;
        case 'R': b |= 0x10; break;
        case 'L': b |= 0x20; break;
        case 'U': b |= 0x40; break;
        case 'D': b |= 0x80; break;
        case 'r': b |= 0x100; break;
        case 'l': b |= 0x200; break;
        case 'x': b |= 0x400; break;
        case 'y': b |= 0x800; break;
        }
    }
    *end = k;
    return b;
}

extern void SIM_ShotRequest(const char *name);
static int sInFrame;   // the pilot runs in PortRoute_Frame (the field's own frame: the map can be read)

static const char *sRest;   // the tokens from the one the pilot is on: the map dump's targets are the G tokens in them

static void DumpMap(const PortRouteState *st, const char *why) {
    PortRouteMap_SetTargets(sRest);
    PortRouteMap_Dump(st->game, why, (unsigned)OS_GetVBlankCount());
}

extern void PORT_OpenFilesLine(void);   // port/src/run_frames.c: `openfiles: now N peak P`, tools/route.py reads it

static void End(const PortRouteState *st, const char *why) {
    if (sInFrame && Env("BWPORT_MAPDUMP")) {
        DumpMap(st, strncmp(why, "stuck", 5) == 0 ? "stuck" : strncmp(why, "timeout", 7) == 0 ? "timeout" : "end");
    }
    PORT_OpenFilesLine();
    fprintf(stderr, "pilot: %s at V-blank %u\n", why, (unsigned)OS_GetVBlankCount());
    fprintf(stderr, "pos: %s Z zone %u %d,%d\n", st->game, st->zone, st->gx, st->gz);
    fflush(stderr);
    _exit(0);
}

static const char *NextTok(const char *t) {
    while (*t != '\0' && *t != ' ') {
        t++;
    }
    return t;
}

static int sValid;   // the field has run: PortRoute_Frame drives the pilot
static int sCalled;  // PortRoute_Frame ran since the last V-blank
static PortRouteState sLast;

static unsigned sPulse = 2, sCnt;

// one frame of pressing through an event: a scene or text (the pulse key every 4th frame), a battle (A)
static void PressEvent(const PortRouteState *st) {
    static int sNo;   // a No question is up (logged once)

    if (st->event == 2 && gPortRouteYesNo == 2) {   // the forget-a-move or switch question (all four games): No, so the
        if (!sNo) {                                   // first move (a damaging one) stays first
            sNo = 1;
            fprintf(stderr, "pilot: battle yes/no question: answering No at V-blank %u\n", (unsigned)OS_GetVBlankCount());
        }
        SetKeys((++sCnt & 3) == 0 ? 2u : 0);
        return;
    }
    sNo = 0;
    if (st->mud) {   // the deep-mud task (Platinum): the key of a turn, held; the frame hook picks the direction from the facing
        SetKeys((unsigned)st->mud);
        return;
    }
    SetKeys((++sCnt & 3) == 0 ? (st->event == 2 ? 1u : sPulse) : 0);
}

static int HangLimit(void) {   // BWPORT_PILOT_HANG, default 3000 frames
    static int sLimit = -1;

    if (sLimit < 0) {
        const char *e = getenv("BWPORT_PILOT_HANG");

        sLimit = e != NULL && atoi(e) > 0 ? atoi(e) : 3000;
    }
    return sLimit;
}

static int sTimeoutNext;   // To:N: the next token's timeout (frames), 0 none
static int sTimeoutTok;    // the running token's, from the To:N before it; 0: BWPORT_PILOT_TIMEOUT

// the no-progress check: called once a pilot step while it presses through an event (any token but M and K)
static void HangCheck(const PortRouteState *st, const char *tok) {
    static const char *sTok;
    static int sN, sZone, sGx, sGz, sEvent, sTick;
    int sLimit = HangLimit();

    if (sTimeoutTok > sLimit) {
        sLimit = sTimeoutTok;   // a To:N token (the credits roll with the player standing) is told to run that long
    }
    if (!st->event || tok != sTok || (int)st->zone != sZone || st->gx != sGx || st->gz != sGz || st->event != sEvent) {
        sTok = tok;
        sZone = (int)st->zone;
        sGx = st->gx;
        sGz = st->gz;
        sEvent = st->event;
        sN = 0;
        return;
    }
    sN++;
    if ((++sTick & 15) == 0 && st->event != 2 && (!sInFrame || !PortRouteMap_Get()->ok)) {
        sN -= 12;   // no field (sampled each 16 frames): a demo plays, a quarter counts (Black 2's frozen Opelucid: ~5000 frames)
    }
    if (sN < (st->event == 2 ? 4 * sLimit : sLimit)) {
        return;
    }
    fprintf(stderr, "pilot: hang: %.*s, event %s, zone %u %d,%d, no progress in %d frames%s at V-blank %u\n",
            (int)(NextTok(tok) - tok), tok, st->event == 2 ? "battle" : "on", st->zone, st->gx, st->gz, sN,
            st->event != 2 && gPortRouteFieldYesNo != 0 ? ", a yes/no list is up (Py or Pn answers it)"
            : st->event != 2 && gPortRouteList != 0 ? ", a list menu is up (Pl:N answers it)"
            : gPortRouteNameIn != 0 ? ", the naming screen is up (Pk answers it)" : "",
            (unsigned)OS_GetVBlankCount());
    if (sInFrame && st->zone != 0xffff) {
        DumpMap(st, "hang");
    }
    SetKeys(0);
    End(st, "hang");
}

static int TokenTimeout(void) {
    static int sT = -1;

    if (sTimeoutTok > 0) {
        return sTimeoutTok;
    }
    if (sT < 0) {
        const char *e = getenv("BWPORT_PILOT_TIMEOUT");

        sT = e != NULL && atoi(e) > 0 ? atoi(e) : 20000;
    }
    return sT;
}

// Py / Pn (age: the frames this token has run, 1 on its first): press through the event until the field's yes/no list is up,
// then answer it (Yes: A, with up first when the cursor starts on No; No: B, the list's cancel); 1 once the list has the answer
static int YesNoStep(const PortRouteState *st, const char *tok, int age) {
    static int sUp, sUpAge, sUpPressed;
    int yes = tok[1] == 'y';

    if (age == 1) {
        sUp = sUpPressed = 0;
    }
    if (!sUp) {
        if (gPortRouteFieldYesNo == 0) {
            if (age >= HangLimit()) {
                fprintf(stderr, "pilot: hang: %.2s: no yes/no list in %d frames, zone %u %d,%d at V-blank %u\n", tok, age - 1,
                        st->zone, st->gx, st->gz, (unsigned)OS_GetVBlankCount());
                SetKeys(0);
                End(st, "hang");
            }
            if (st->event) {
                PressEvent(st);
            } else {
                SetKeys(0);
            }
            return 0;
        }
        sUp = 1;
        sUpAge = age;
        sCnt = 0;
        fprintf(stderr, "pilot: %.2s: yes/no list up (cursor on %s) at V-blank %u: answering %s\n", tok,
                (gPortRouteFieldYesNo - 1) & 1 ? "No" : "Yes", (unsigned)OS_GetVBlankCount(), yes ? "Yes" : "No");
    }
    if (gPortRouteFieldYesNo == 0) {   // answered
        fprintf(stderr, "pilot: %.2s done at V-blank %u\n", tok, (unsigned)OS_GetVBlankCount());
        SetKeys(0);
        return 1;
    }
    if (age - sUpAge > 600) {
        fprintf(stderr, "pilot: hang: %.2s: the yes/no list takes no answer in 600 frames at V-blank %u\n", tok,
                (unsigned)OS_GetVBlankCount());
        SetKeys(0);
        End(st, "hang");
    }
    if ((++sCnt & 7) != 0) {   // a key every 8th frame, released between (the list reads new presses)
        SetKeys(0);
    } else if (gPortRouteFieldYesNo >= 3) {   // a two-item list: onto the answer (up Yes, down No), then A
        SetKeys(((gPortRouteFieldYesNo - 3) != 0) == !yes ? 1u : yes ? 0x40u : 0x80u);
    } else if (!yes) {
        SetKeys(2u);   // B: the list's cancel, No
    } else if (gPortRouteFieldYesNo == 2 && !sUpPressed) {
        sUpPressed = 1;
        SetKeys(0x40u);   // up: the cursor onto Yes
    } else {
        SetKeys(1u);
    }
    return 0;
}

// Py Pn Pl:N Pk: the tokens that wait for a question (a list, the naming screen) of the event that is running or about to start
static int WaitsOnEvent(const char *t) {
    return t[0] == 'P' && (t[1] == 'y' || t[1] == 'n' || t[1] == 'k' || (t[1] == 'l' && t[2] == ':'));
}

// Pl:N (age: the frames this token has run, 1 on its first): press through the event until a script's list menu is up, move
// its cursor onto row N (1-based; tap up or down every 8th frame), then A; Pl:0 is B (the list's cancel); 1 once the list
// has its answer (the answer's value is logged)
static int ListStep(const PortRouteState *st, const char *tok, int age) {
    static int sUp, sUpAge, sKey;
    int want = atoi(tok + 3);

    if (age == 1) {
        sUp = 0;
    }
    if (!sUp) {
        if (gPortRouteList == 0) {
            if (age >= HangLimit()) {
                fprintf(stderr, "pilot: hang: %.*s: no list menu in %d frames, zone %u %d,%d at V-blank %u\n",
                        (int)(NextTok(tok) - tok), tok, age - 1, st->zone, st->gx, st->gz, (unsigned)OS_GetVBlankCount());
                SetKeys(0);
                End(st, "hang");
            }
            if (st->event) {
                PressEvent(st);
            } else {
                SetKeys(0);
            }
            return 0;
        }
        sUp = 1;
        sUpAge = age;
        sKey = 0;
        fprintf(stderr, "pilot: %.*s: list up (%d rows, cursor on row %d) at V-blank %u\n", (int)(NextTok(tok) - tok), tok,
                gPortRouteListCount, gPortRouteListRow + 1, (unsigned)OS_GetVBlankCount());
        if (want > gPortRouteListCount) {
            fprintf(stderr, "pilot: hang: %.*s: the list has only %d rows\n", (int)(NextTok(tok) - tok), tok,
                    gPortRouteListCount);
            SetKeys(0);
            End(st, "hang");
        }
    }
    if (gPortRouteList == 0) {   // answered
        fprintf(stderr, "pilot: %.*s done at V-blank %u: the answer is %d\n", (int)(NextTok(tok) - tok), tok,
                (unsigned)OS_GetVBlankCount(), gPortRouteListResult);
        SetKeys(0);
        return 1;
    }
    if (age - sUpAge > 600) {
        fprintf(stderr, "pilot: hang: %.*s: the list takes no answer in 600 frames at V-blank %u\n",
                (int)(NextTok(tok) - tok), tok, (unsigned)OS_GetVBlankCount());
        SetKeys(0);
        End(st, "hang");
    }
    if ((++sKey & 7) != 0) {   // a key every 8th frame, released between (the list reads new presses)
        SetKeys(0);
    } else if (want == 0) {
        SetKeys(2u);
    } else if (gPortRouteListHigh > 0 && gPortRouteListRow / gPortRouteListHigh != (want - 1) / gPortRouteListHigh) {
        SetKeys(gPortRouteListRow / gPortRouteListHigh < (want - 1) / gPortRouteListHigh ? 0x10u : 0x20u);   // right / left: its column
    } else if (gPortRouteListRow + 1 < want) {
        SetKeys(0x80u);   // down
    } else if (gPortRouteListRow + 1 > want) {
        SetKeys(0x40u);   // up
    } else {
        SetKeys(1u);
    }
    return 0;
}

// Pk (age: the frames this token has run, 1 on its first): press through the event until the naming screen is up, give it 120
// frames to fade in, then Start (the cursor onto the OK key) and A (OK) every 8th frame: the name stays as the screen offers it
// (empty: the default); 1 once the screen has closed
// The naming-screen tokens: 0 not one, 1 Pk, 2 Pk:pw (tools/route_tests.py compiles this function on its own: keep it plain C with
// no other names than strncmp)
static int PkTokenMode(const char *tok) {
    if (tok[0] != 'P' || tok[1] != 'k') {
        return 0;
    }
    if (tok[2] == '\0' || tok[2] == ' ') {
        return 1;
    }
    if (strncmp(tok + 2, ":pw", 3) == 0 && (tok[5] == '\0' || tok[5] == ' ')) {
        return 2;
    }
    return 0;
}

static int NameStep(const PortRouteState *st, int age, int password) {
    static int sUp, sUpAge;

    if (age == 1) {
        sUp = 0;
    }
    gPortRouteNamePassword = password;
    if (!sUp) {
        if (gPortRouteNameIn == 0) {
            if (age >= HangLimit()) {
                fprintf(stderr, "pilot: hang: Pk: no naming screen in %d frames, zone %u %d,%d at V-blank %u\n", age - 1, st->zone,
                        st->gx, st->gz, (unsigned)OS_GetVBlankCount());
                SetKeys(0);
                End(st, "hang");
            }
            if (st->event || st->zone == 0xffff) {
                PressEvent(st);
            } else {
                SetKeys(0);
            }
            return 0;
        }
        sUp = 1;
        sUpAge = age;
        fprintf(stderr, "pilot: Pk: naming screen up at V-blank %u\n", (unsigned)OS_GetVBlankCount());
    }
    if (gPortRouteNameIn == 0) {
        fprintf(stderr, "pilot: Pk done at V-blank %u\n", (unsigned)OS_GetVBlankCount());
        SetKeys(0);
        gPortRouteNamePassword = 0;
        return 1;
    }
    if (age - sUpAge > 1200) {
        fprintf(stderr, "pilot: hang: Pk: the naming screen takes no OK in 1200 frames at V-blank %u\n", (unsigned)OS_GetVBlankCount());
        SetKeys(0);
        End(st, "hang");
    }
    if (age - sUpAge < 120 || ((age - sUpAge) & 7) != 0) {
        SetKeys(0);
    } else if (password) {
        // Pk:pw: the name box already holds the password; Start first (the cursor onto OK), then A: an A before the Start would type
        // a letter after the password (the first press comes at age - sUpAge == 120, so the phase counts from there)
        SetKeys((((age - sUpAge - 120) >> 3) & 1) == 0 ? 8u : 1u);
    } else {
        // A, Start, A: the first A of a touch-mode keyboard only shows the cursor, else it types the first key's letter (the
        // player's name needs one: an empty name is refused with a beep), Start puts the cursor on OK, the last A presses it
        SetKeys((((age - sUpAge) >> 3) % 3) == 1 ? 8u : 1u);
    }
    return 0;
}

// the event waits Wf Wt Wb Wz (age: the frames this token has run, 1 on its first): 1 when it is done
static int WaitStep(const PortRouteState *st, const char *tok, int age) {
    static int sN, sBattle;
    int n;

    if (age == 1) {
        sN = sBattle = 0;
    }
    switch (tok[1]) {
    case 'r': {   // the game reset itself (the machine's restart count grew since the token began); presses through until then
        extern int PORT_MachineResets(void);   // port/src/machine.c
        static int sR0;

        if (age == 1) {
            sR0 = PORT_MachineResets();
        }
        if (PORT_MachineResets() > sR0) {
            return 1;
        }
        if (st->event) {
            PressEvent(st);
        } else {
            SetKeys(0);
        }
        return 0;
    }
    case 't':   // no text printing (Black, White: the field free); presses nothing
        SetKeys(0);
        sN = (st->text >= 0 ? st->text == 0 : !st->event) ? sN + 1 : 0;
        return sN >= 2;
    case 'b':   // a battle starts and ends, then the field is free
        sBattle |= st->event == 2;
        break;
    case 's':   // a story flag or work value (port/src/route_story.c), then the field free
    case 'z':   // in zone N, the field free
        if (tok[1] == 's' ? !PortRouteStory_Cond(st->game, tok + 3) : (int)st->zone != atoi(tok + 3)) {
            sN = 0;
            if (st->event) {
                PressEvent(st);
            } else {
                SetKeys(0);
            }
            return 0;
        }
        break;
    }
    if (st->event) {
        sN = 0;
        PressEvent(st);
        return 0;
    }
    SetKeys(0);
    n = tok[1] == 'b' ? 30 : tok[1] == 'f' && tok[2] == ':' ? atoi(tok + 3) : 8;
    return (tok[1] != 'b' || sBattle) && ++sN >= n;
}

// S: the key that moves the field menu's cursor toward SAVE, called at each press (dir: the key it pressed last, 0 at the first
// press). Black 2, White 2 know no slots: down. Black, White: the slots' columns (x = slot % 2) and rows (y = slot / 2): toward
// SAVE's row first, then its column; a press that left the cursor where it was (the menu has no such step: the cursor skips
// from the first column's second row to its third) is not repeated from that slot, the other axis goes instead
static int sMenuSlot = -1;   // the cursor's slot at the last press
static unsigned sMenuBlocked;   // the keys that did not move it from there

static unsigned MenuDir(const PortRouteMenu *m, unsigned dir) {
    unsigned want[6], key[4] = {0x80u, 0x10u, 0x40u, 0x20u};   // down, right, up, left
    int n = 0, i, cx = m->slot % 2, cy = m->slot / 2, sx = m->saveSlot % 2, sy = m->saveSlot / 2;

    if (m->key != 0) {   // the game's own step toward SAVE (HeartGold / SoulSilver: the menu's moves skip and wrap)
        return m->key;
    }
    if (!m->grid || m->saveSlot < 0) {
        return 0x80u;
    }
    if (m->slot == sMenuSlot) {   // the last press did not move it
        sMenuBlocked |= dir;
    } else {
        sMenuBlocked = 0;
    }
    sMenuSlot = m->slot;
    if (cy != sy) {
        want[n++] = cy < sy ? key[0] : key[2];
    }
    if (cx != sx) {
        want[n++] = cx < sx ? key[1] : key[3];
    }
    for (i = 0; i < 4; i++) {   // the other ways, in case those are blocked
        want[n++] = key[i];
    }
    for (i = 0; i < n; i++) {
        if (!(sMenuBlocked & want[i])) {
            return want[i];
        }
    }
    return 0x80u;
}

// S: the field menu's SAVE, each step on the game's state (the field menu's input and cursor, the save event's state:
// port/extern*/src/ov021|ov036/field_menu.c, system/game_event.c); 1 when it is done
static int SaveStep(const PortRouteState *st, int age) {
    static int sPhase, sWait, sTries, sSaved;
    static unsigned sDir;
    unsigned now = OS_GetVBlankCount();
    int input = gPortRouteMenu.vblank != 0 && gPortRouteMenu.vblank + 3 >= now && gPortRouteMenu.input;

    if (age == 1) {
        sPhase = sWait = sTries = sSaved = 0;
        sMenuSlot = -1;
        sMenuBlocked = 0;
        sDir = 0;
    }
    sWait++;
    switch (sPhase) {
    case 0:   // the field free for 8 frames, then X
        if (st->event) {
            sWait = 0;
            PressEvent(st);
            return 0;
        }
        SetKeys(sWait >= 8 ? 0x400u : 0);
        if (sWait >= 8) {
            fprintf(stderr, "pilot: S: X at V-blank %u\n", now);
            sPhase = 1;
            sWait = 0;
        }
        return 0;
    case 1:   // X held a second frame, then the menu takes input
        SetKeys(sWait == 1 ? 0x400u : 0);
        if (input) {
            sPhase = 2;
            sWait = 0;
        } else if (sWait > 300) {   // no menu (X fell into a frame that did not take it): again
            if (++sTries > 3) {
                End(st, "save: the field menu does not open");
            }
            sPhase = 0;
            sWait = 0;
        }
        return 0;
    case 2:   // to SAVE (a press each 6 frames: 2 pressed, 4 not), then A
        if (!input) {
            SetKeys(0);
            if (sWait > 300) {
                End(st, "save: the field menu stopped taking input");
            }
            return 0;
        }
        if (sWait % 6 == 1 && gPortRouteMenu.onSave) {
            fprintf(stderr, "pilot: S: SAVE chosen at V-blank %u\n", now);
            SetKeys(1);
            sPhase = 3;
            sWait = 0;
            return 0;
        }
        if (sWait > 6 * 12) {
            End(st, "save: the cursor does not reach SAVE");
        }
        if (sWait % 6 == 1) {
            sDir = MenuDir(&gPortRouteMenu, sDir);
        }
        SetKeys(sWait % 6 == 1 || sWait % 6 == 2 ? sDir : 0);
        return 0;
    case 3:   // A held a second frame; then the save: A at its questions and at "saved", nothing while it writes
        if (sWait == 1) {
            SetKeys(1);
            return 0;
        }
        if (st->save < 0) {
            SetKeys(0);
            if (sSaved) {
                fprintf(stderr, "pilot: S: done at V-blank %u\n", now);
                sPhase = 4;
                sWait = 0;
            } else if (sWait > 600) {
                End(st, "save: the save does not start");
            }
            return 0;
        }
        sSaved |= st->save >= 8;   // REPORT_STATE_SAVE: the write
        SetKeys((st->save < 5 || st->save > 10) && (++sCnt & 3) == 0 ? 1u : 0);
        return 0;
    default:   // B until the field is free for 8 frames (the menu closed)
        if (st->event || input) {
            sWait = 0;
            SetKeys((++sCnt & 3) == 0 ? 2u : 0);
            return 0;
        }
        SetKeys(0);
        return sWait >= 8;
    }
}


// the G tokens: G:X,Z and G:warp:N, a path planned on the map (port/src/route_map.c) walked waypoint by waypoint with the
// X,Z walk; 1 when done
static int WalkTo(const PortRouteState *st, int tx, int tz, int reset);
static int WatchWait(const PortRouteState *st, int tx, int tz);

// the rider is on a floor that pushes it when no key is held (the Cycling Road's bridge, a bicycle: player_move.c): a frame without a
// key at a corner sends it back one tile, so the walk keeps a key down there (WalkTo, GoStep's corner)
static int OnPush(void) {
    const PortRouteAdapter *ad = PortRouteMap_Adapter();

    return ad != NULL && ad->pushed != NULL && ad->pushed();
}
static int sWatchSig = -1, sWatchTurned, sWatchN;   // WatchWait's episode at the edge of a trainer's line of sight

// a target token that cannot be done: the pilot line, a map dump (always: BWPORT_MAPDUMP's End makes one too), the end
static void GoFail(const PortRouteState *st, const char *tok, const char *why) {
    fprintf(stderr, "pilot: no path: %s (G token %.*s)\n", why, (int)(NextTok(tok) - tok), tok);
    if (Env("BWPORT_MAPDUMP")) {
        SIM_ShotRequest("mapstuck");
    } else if (sInFrame) {
        DumpMap(st, "nopath");
    }
    End(st, "stuck: no path");
}

static const unsigned kDirKey[4] = {0x40u, 0x80u, 0x20u, 0x10u};   // up down left right

static int CatchStep(const PortRouteState *st, int age, const char *tok);
static int sCatchTalk;   // Cc: 1 = the next talk's battle is a catch (a scripted wild battle), 2 = that battle has started

// the event of a talk armed by Cc (sCatchTalk 1 armed, 2 in the battle, 3 after it): the first battle (event 2) is a catch (CatchStep in its talk mode: the Bag's first ball, no attack),
// the nickname question after it is answered No (a Pn token's way); other text and questions: A as always
static void CatchTalk(const PortRouteState *st) {
    static int sAge, sYesNo, sYesNoAge;

    if (st->event == 2) {
        if (sCatchTalk == 3) {   // a later battle of the same event (N's, after the legendary): fought as always
            PressEvent(st);
            return;
        }
        if (sCatchTalk == 1) {
            sCatchTalk = 2;
            sAge = sYesNo = 0;
        }
        CatchStep(st, ++sAge, "C:talk");
        return;
    }
    if (sCatchTalk == 2) {   // the catch's battle is over: the one-shot cheat is back for the next one
        sCatchTalk = 3;
        gPortRouteCatch = 0;
    }
    if (sCatchTalk == 3 && (gPortRouteFieldYesNo != 0 || sYesNo)) {
        sYesNo = YesNoStep(st, "Pn", sYesNo ? ++sYesNoAge : (sYesNoAge = 1)) ? 0 : 1;
        return;
    }
    PressEvent(st);
}

// G:npc / G:item / G:bg at the goal tile: turn to face the target (a one-frame tap turns without a step), A, then the
// event it starts until the field is free (the Wf wait). 1 done
static int TalkStep(const PortRouteState *st, const char *tok, int dir, int talk, int reset) {
    static int sPhase, sWait, sTries, sSeen;
    const PortMap *m;

    if (reset) {
        sPhase = sWait = sTries = sSeen = 0;
        return 0;
    }
    sWait++;
    switch (sPhase) {
    case 0:   // face it
        m = PortRouteMap_Get();
        if (m->ok && m->pdir == dir) {
            SetKeys(0);
            if (!talk) {
                return 1;
            }
            sPhase = 1;
            sWait = 0;
            return 0;
        }
        if (sWait > 60) {
            if (m->ok && m->pdir >= 0) {
                char why[96];

                snprintf(why, sizeof why, "at %d,%d the player does not turn to face %d (faces %d)", st->gx, st->gz, dir,
                         m->pdir);
                GoFail(st, tok, why);
            }
            sPhase = talk;   // the facing is not known: tapped enough
            sWait = 0;
            return !talk;
        }
        SetKeys((sWait & 3) == 1 ? kDirKey[dir] : 0);
        return 0;
    case 1:   // A, two frames (as Ka:2)
        SetKeys(1);
        if (sWait >= 2) {
            fprintf(stderr, "pilot: G: A facing %d at %d,%d (V-blank %u)\n", dir, st->gx, st->gz, (unsigned)OS_GetVBlankCount());
            sPhase = 2;
            sWait = 0;
            sSeen = 0;
        }
        return 0;
    default:   // the event it started, until the field is free for 8 frames (Wf)
        if (st->event) {
            sSeen = 1;
            sWait = 0;
            if (sCatchTalk) {
                CatchTalk(st);
                return 0;
            }
            PressEvent(st);
            return 0;
        }
        SetKeys(0);
        if (sSeen) {
            if (sWait >= 8 && sCatchTalk) {   // the talk is over: the Cc arm ends with it
                sCatchTalk = 0;
                gPortRouteCatch = 0;
            }
            return sWait >= 8;
        }
        if (sWait > 60) {   // nothing started: A again, three times
            if (++sTries >= 3) {
                char why[96];

                snprintf(why, sizeof why, "A facing %d at %d,%d started no event (3 tries)", dir, st->gx, st->gz);
                GoFail(st, tok, why);
            }
            sPhase = 0;
            sWait = 0;
        }
        return 0;
    }
}

// G:wall:N at the cell before the wall: the key held toward it through the pass (the game's wall event ends when the key
// is let go; the pulse key is not pressed) until the player is on the tile past it (tx,tz). No step for 120 frames: the
// wall is closed (its switches are not all pressed, or it is not on this line). 1 done
static int WallStep(const PortRouteState *st, const char *tok, int uid, int dir, int tx, int tz, int reset) {
    static int sStill, sGx, sGz;

    if (reset) {
        sStill = 0;
        sGx = st->gx;
        sGz = st->gz;
        return 0;
    }
    if (st->gx == tx && st->gz == tz) {
        SetKeys(0);
        return 1;
    }
    if (st->gx != sGx || st->gz != sGz) {
        sGx = st->gx;
        sGz = st->gz;
        sStill = 0;
    } else if (++sStill >= 120) {
        char why[96];

        snprintf(why, sizeof why, "wall %d closed (no step in 120 frames at %d,%d, key %d)", uid, st->gx, st->gz, dir);
        GoFail(st, tok, why);
    }
    SetKeys(kDirKey[dir]);
    return 0;
}

// G:ride at the ride's start tile: the key toward its entry until the ride's event starts, then the pulse key through it
// (the ride, a scene at its end) until the field is free for 8 frames; 1 then, with the landing logged
static int RideStep(const PortRouteState *st, const char *tok, int n, int dir, int reset) {
    static int sAge, sOn, sFree, sMoved, sRode;
    const PortRouteAdapter *ad = PortRouteMap_Adapter();
    int rs = ad != NULL && ad->rideState != NULL && sInFrame ? ad->rideState(st->zone, n) : -1;   // a car on its own schedule

    if (reset) {
        sAge = sOn = sFree = sMoved = sRode = 0;
        return 0;
    }
    if (rs >= 0 && (rs & 1)) {   // riding: the car left its first station (moved), then stands at one: Down gets off
        sRode = 1;
        sMoved |= !(rs & 2);
    }
    if (st->event) {
        sOn = 1;
        sFree = 0;
        PressEvent(st);
        if (rs >= 0 && (rs & 3) == 3 && sMoved) {
            SetKeys(((sCnt & 3) == 0 ? sPulse : 0) | kDirKey[1]);
        }
        return 0;
    }
    if (sOn && rs >= 0 && !sRode) {   // a first boarding is an intro scene that rides nowhere: board again when the car is back
        sOn = sFree = 0;
        sAge = 0;
    }
    if (!sOn) {
        if (++sAge > (rs >= 0 ? 2400 : 120)) {
            char why[96];

            snprintf(why, sizeof why, "ride %d did not start (key %d held 120 frames at %d,%d)", n, dir, st->gx, st->gz);
            GoFail(st, tok, why);
        }
        SetKeys(kDirKey[dir]);
        return 0;
    }
    SetKeys(0);
    if (++sFree < 8) {
        return 0;
    }
    fprintf(stderr, "pilot: G:ride:%d landed at %d,%d y %d zone %u (V-blank %u)\n", n, st->gx, st->gz,
            sInFrame ? PortRouteMap_Get()->py : -1, st->zone, (unsigned)OS_GetVBlankCount());
    return 1;
}

// an event that starts now is the G target's own: the player is on the target tile (G:X,Z) or on the trigger's tiles
// (G:trig:N): its scene, a battle it starts. A trigger whose scene is already on when the player is still one tile
// before it (the walk's waypoint is the trigger's tile: Opelucid Gym's statue triggers lift the player away) counts too,
// but as 2, not 1: GoStep first walks on to the trigger tile after such a scene (what every older leg was made with) and
// ends where the scene left the player only when there is no way on
static int TargetEvent(const PortRouteState *st, const PortRouteGoal *g, int have, int wx, int wz) {
    const PortMap *m;
    const PortMapTrig *t;

    if (g->kind == GOAL_TILE) {
        return st->gx == g->x && st->gz == g->z;
    }
    if (g->kind != GOAL_TRIG || !sInFrame) {
        return 0;
    }
    m = PortRouteMap_Get();
    if (!m->ok || m->zone != st->zone || g->n >= m->ntrig) {
        return 0;
    }
    t = &m->trig[g->n];
    if (t->rail) {
        return m->rail && m->rpos.line == t->line && m->rpos.front >= t->x && m->rpos.front < t->x + (t->w ? t->w : 1) &&
               m->rpos.side >= t->z && m->rpos.side < t->z + (t->h ? t->h : 1);
    }
    if (st->gx >= t->x && st->gx < t->x + (t->w ? t->w : 1) && st->gz >= t->z && st->gz < t->z + (t->h ? t->h : 1)) {
        return 1;
    }
    return (have && wx >= t->x && wx < t->x + (t->w ? t->w : 1) && wz >= t->z && wz < t->z + (t->h ? t->h : 1) &&
            abs(wx - st->gx) + abs(wz - st->gz) == 1) ? 2 : 0;
}

// G:door:I into a panel of the zone itself (a warp whose destination is this map: Galactic HQ's warp panels, Gen 4): the zone does
// not change, the token is done when the player stands on the panel's destination tile
static int DoorPadDone(const PortRouteState *st, const PortRouteGoal *g) {
    const PortMap *pm = PortRouteMap_Get();
    const PortMapWarp *w, *d;

    if (g->kind != GOAL_WARP || !g->door || !pm->ok || g->n < 0 || g->n >= pm->nwarp) {
        return 0;
    }
    w = &pm->warp[g->n];
    if (w->rail || w->zone != st->zone || (int)w->dest >= pm->nwarp) {
        return 0;
    }
    d = &pm->warp[w->dest];
    return st->gx == d->x && st->gz == d->z && (st->gx != w->x || st->gz != w->z);
}

static int GoStep(const PortRouteState *st, const char *tok, int age) {
    static int sWx, sWz, sHave, sNoMove, sReplans, sPress, sPressN, sLastGx, sLastGz, sTalk, sEvLog, sEvZone, sTargetEv, sWall, sWallX, sWallZ, sNearTried, sNearEnd;
    static PortRouteGoal sGoal;
    static const char *sFor;   // the token sGoal is for (its first frames can come before the field: no GoStep then)
    static PortRailPos sRLast;   // on a rail: the place the walk was last at, the direction it holds
    static int sRHave, sRDir, sPrevRail;   // sPrevRail: on a rail at the last step (-1 not known): a hand-off flips it
    static int sSettle, sSetFx, sSetFz, sSetSame;   // a rail -> grid hand-off: no key while the step to its place runs
    static int sRide;   // G:ride at its start tile: RideStep runs
    static unsigned sDoorZone;   // G:door: the zone it started in (done when the zone changes)
    static int sGearWait, sGearTry;   // the bicycle's gear press (plan.gear): frames to wait after it, presses made on this token
    static int sDownT, sDownTry;   // getting off the bicycle (plan.dismount): frames since the Y press (0 none), presses made on this token
    static int sDownFx, sDownFz, sDownSame;   // ... and the player's fine position: Y is pressed only once he has stood still
    static int sNoPathWait;   // frames waited on "no path" for a walking actor to step out of the way
    static int sWindWait;   // waiting in a shelter for the wind's gust (the plan's wait)
    const PortRouteAdapter *ad = PortRouteMap_Adapter();
    PortRailPos rp;
    PortRoutePlan plan;
    int r, onRail;

    if (tok != sFor || age == 1) {
        char why[160];

        sFor = tok;

        if (!PortRoute_ParseGoal(tok, &sGoal, why, sizeof why)) {
            fprintf(stderr, "pilot: bad token: %s\n", why);
            End(st, "stuck: bad token");
        }
        sHave = sNoMove = sReplans = sPressN = sTalk = sEvLog = sTargetEv = sRHave = sWall = sNearTried = sNearEnd = 0;
        sPress = sRDir = sPrevRail = -1;
        sRide = sNoPathWait = sSettle = sGearWait = sGearTry = sDownT = sDownTry = sDownSame = 0;
        sWatchSig = -1;
        sWatchTurned = sWatchN = 0;
        sDoorZone = st->zone;
        sLastGx = st->gx;
        sLastGz = st->gz;
        PortRoutePlan_Reset();
    }
    if (sGoal.kind == GOAL_WARP ? (sGoal.door ? st->zone != sDoorZone || DoorPadDone(st, &sGoal) : (int)st->zone == sGoal.n)
                                : sGoal.kind == GOAL_TILE && st->gx == sGoal.x && st->gz == sGoal.z) {
        fprintf(stderr, "pilot: %.*s done at %d,%d zone %u (V-blank %u)\n", (int)(NextTok(tok) - tok), tok, st->gx, st->gz,
                st->zone, (unsigned)OS_GetVBlankCount());
        SetKeys(0);
        return 1;
    }
    if (sRide) {   // G:ride at its start tile: the key into it, the ride, its end
        if (RideStep(st, tok, sGoal.n, sPress, 0)) {
            SetKeys(0);
            return 1;
        }
        return 0;
    }
    if (sTalk || sWall) {   // at the target: facing it, A, its event; G:wall at the cell before it: the key held through the pass
        if (sWall ? WallStep(st, tok, sGoal.n, sPress, sWallX, sWallZ, 0) : TalkStep(st, tok, sPress, sGoal.talk, 0)) {
            fprintf(stderr, "pilot: %.*s done at %d,%d zone %u (V-blank %u)\n", (int)(NextTok(tok) - tok), tok, st->gx,
                    st->gz, st->zone, (unsigned)OS_GetVBlankCount());
            SetKeys(0);
            return 1;
        }
        return 0;
    }
    if (st->event) {   // a scene: press through it, then plan again
        if (!sEvLog) {   // one line an event (a trainer that saw the player, a scene on the way)
            sEvLog = 1;
            sEvZone = (int)st->zone;
            if (!sTargetEv) {
                sTargetEv = TargetEvent(st, &sGoal, sHave, sWx, sWz);
                if (sTargetEv == 2 && sNearTried) {   // the walk on to the trigger tile did not help: the scene is the trigger's own
                    sTargetEv = 1;
                }
            }
            fprintf(stderr, "pilot: event during G at V-blank %u (zone %u, %d,%d)%s\n", (unsigned)OS_GetVBlankCount(),
                    st->zone, st->gx, st->gz, sTargetEv == 1 ? ": the target's own" : sTargetEv ? ": one tile before the trigger" : "");
        }
        PressEvent(st);
        sHave = 0;
        sPressN = 0;
        return 0;
    }
    if (sEvLog && sTargetEv == 2 && (int)st->zone == sEvZone) {
        // a scene that started one tile before the trigger has ended: walk on to the trigger tile, as the pilot did
        // before (a scene that only happened to start there leaves the player somewhere else, and its legs end on the
        // trigger); when no plan reaches the tile (a lift moved the player onto a platform) the token ends here
        {
            const PortMap *pm = PortRouteMap_Get();
            int pad = 0, i;

            // only a warp pad (a lever with s -2 on the trigger's tile: the White 2 Frigate's ActorSetGPos triggers) is
            // done where it left the player; any other scene that moves the player (Route 5, Chargestone, N's Castle ...)
            // walks on to the trigger tile as before, and those legs end there
            for (i = 0; sGoal.kind == GOAL_TRIG && pm != NULL && pm->ok && sGoal.n < pm->ntrig && i < pm->nlever &&
                        i < PORT_MAP_LEVERS; i++) {
                pad |= pm->lever[i].s == -2 && pm->lever[i].x == pm->trig[sGoal.n].x && pm->lever[i].z == pm->trig[sGoal.n].z;
            }
            if (pad && abs(pm->trig[sGoal.n].x - (int)st->gx) + abs(pm->trig[sGoal.n].z - (int)st->gz) > 4) {
                // the pad moved the player far from the trigger: the token is done where it left the player
                fprintf(stderr, "pilot: %.*s done at %d,%d zone %u (V-blank %u): the scene moved the player off the trigger (a pad)\n",
                        (int)(NextTok(tok) - tok), tok, st->gx, st->gz, st->zone, (unsigned)OS_GetVBlankCount());
                SetKeys(0);
                return 1;
            }
        }
        fprintf(stderr, "pilot: scene one tile before the trigger has ended at %d,%d: on to the trigger tile\n", st->gx, st->gz);
        sTargetEv = 0;
        sNearTried = sNearEnd = 1;
        sHave = 0;
    }
    if (sEvLog && (sGoal.kind == GOAL_TRIG || sGoal.kind == GOAL_TILE) && (sTargetEv || (int)st->zone != sEvZone)) {
        // the event the target started has ended (or an event took the player to another zone): done, wherever the
        // scene left the player (a trigger's scene that turns the player back would start again on each step onto it)
        fprintf(stderr, "pilot: %.*s done at %d,%d zone %u (V-blank %u): %s\n", (int)(NextTok(tok) - tok), tok, st->gx,
                st->gz, st->zone, (unsigned)OS_GetVBlankCount(),
                sTargetEv ? "the scene it started has ended" : "an event moved the player to another zone");
        SetKeys(0);
        return 1;
    }
    sEvLog = 0;
    onRail = ad != NULL && ad->railAt != NULL && ad->railStep != NULL && ad->railAt(&rp);
    if (sPrevRail >= 0 && onRail != sPrevRail) {   // a hybrid zone's hand-off (rail <-> grid) happened: plan from there
        if (onRail) {
            fprintf(stderr, "pilot: G: onto the rail at rail[%d,%d,%d]\n", rp.line, rp.front, rp.side);
        } else {
            fprintf(stderr, "pilot: G: off the rail onto the grid at %d,%d\n", st->gx, st->gz);
        }
        sHave = sPressN = sRHave = sNoMove = 0;
        sLastGx = st->gx;
        sLastGz = st->gz;
    }
    sPrevRail = onRail;
    if (onRail) {   // on a rail (a no-grid map): a plan at each new rail place, its first step's key held
        if (sGoal.kind == GOAL_RAIL && rp.line == sGoal.n && rp.front == sGoal.x && rp.side == sGoal.z) {
            fprintf(stderr, "pilot: %.*s done at rail[%d,%d,%d] zone %u (V-blank %u)\n", (int)(NextTok(tok) - tok), tok,
                    rp.line, rp.front, rp.side, st->zone, (unsigned)OS_GetVBlankCount());
            SetKeys(0);
            return 1;
        }
        if (!sRHave || rp.line != sRLast.line || rp.isLine != sRLast.isLine || rp.front != sRLast.front ||
            rp.side != sRLast.side) {
            sRLast = rp;
            sRHave = 1;
            sNoMove = 0;
            if (sPressN == 0) {
                sHave = 0;
            }
        } else if (++sNoMove == 60) {   // no step for 60 frames: the place ahead is in the way (an actor, a wrong guess)
            if (sHave && sRDir >= 0) {
                PortRailPos to;

                ad->railStep(&rp, sRDir, &to);
                PortRoutePlan_BlockRail(st->zone, &to);
            }
            fprintf(stderr, "pilot: G: no step at rail[%d,%d,%d]: re-planning\n", rp.line, rp.front, rp.side);
            sHave = 0;
            sPressN = 0;
            sNoMove = 0;
            if (++sReplans > 12) {
                fprintf(stderr, "pilot: stuck at rail[%d,%d,%d] (G: %d re-plans without a step)\n", rp.line, rp.front,
                        rp.side, sReplans - 1);
                End(st, "stuck");
            }
        }
    } else if (st->gx != sLastGx || st->gz != sLastGz) {
        sLastGx = st->gx;
        sLastGz = st->gz;
        sNoMove = 0;
    } else if (++sNoMove == 150) {   // no step for 120 frames: the tile ahead is in the way (an actor, a wrong guess)
        if (sHave) {   // the tile ahead toward the waypoint
            PortRoutePlan_Block(st->zone, st->gx + (sWx > st->gx) - (sWx < st->gx), st->gz + (sWz > st->gz) - (sWz < st->gz));
        }
        fprintf(stderr, "pilot: G: no step at %d,%d: re-planning\n", st->gx, st->gz);
        sHave = 0;
        sPressN = 0;
        sNoMove = 0;
        if (++sReplans > 12) {
            fprintf(stderr, "pilot: stuck at %d,%d (G: %d re-plans without a step)\n", st->gx, st->gz, sReplans - 1);
            End(st, "stuck");
        }
    }
    if (sPressN > 0 && sSettle > 0) {   // the rail step to the hand-off's place is still running: no key until it stops
        if (st->fx == sSetFx && st->fz == sSetFz) {
            sSettle = ++sSetSame >= 2 ? 0 : sSettle - 1;   // (two frames without a move: stopped)
        } else {
            sSetSame = 0;
            sSetFx = st->fx;
            sSetFz = st->fz;
            sSettle--;
        }
        if (sSettle > 0) {
            SetKeys(0);
            return 0;
        }
    }
    if (sPressN > 0) {   // into a warp
        SetKeys(sPress == 0 ? 0x40u : sPress == 1 ? 0x80u : sPress == 2 ? 0x20u : 0x10u);
        if (--sPressN == 0) {   // the zone did not change: try another way
            if (onRail) {
                PortRoutePlan_BlockRail(st->zone, &rp);
            } else {
                PortRoutePlan_Block(st->zone, st->gx, st->gz);
            }
            sHave = 0;
            if (++sReplans > 12) {
                fprintf(stderr, "pilot: stuck at %d,%d (G: the warp does not take)\n", st->gx, st->gz);
                End(st, "stuck");
            }
        }
        return 0;
    }
    if (!sHave) {
        r = PortRoutePlan_Next(st, &sGoal, &plan);
        if (r == PLAN_NONE && sNearEnd) {   // no way on to the trigger tile after its scene: done where the scene left the player
            fprintf(stderr, "pilot: %.*s done at %d,%d zone %u (V-blank %u): the scene one tile before the trigger moved the player off its way (%s)\n",
                    (int)(NextTok(tok) - tok), tok, st->gx, st->gz, st->zone, (unsigned)OS_GetVBlankCount(), plan.why);
            SetKeys(0);
            return 1;
        }
        sNearEnd = 0;
        if (r == PLAN_NONE && sNoPathWait < 240) {   // a walking actor in a one-tile lane (Twist Mountain 202's x 15): wait 4 s
            const PortMap *m = PortRouteMap_Get();
            int i;

            for (i = 0; i < m->nnpc && (m->npc[i].move == 0 || m->npc[i].rail); i++) {
            }
            if (i < m->nnpc) {
                if (sNoPathWait++ == 0) {
                    fprintf(stderr, "pilot: no path (%s): waiting for the walking actors to move\n", plan.why);
                }
                SetKeys(0);
                return 0;
            }
        }
        if (r == PLAN_NONE) {
            GoFail(st, tok, plan.why);
        }
        if (r == PLAN_THERE) {
            if (plan.press < 0) {   // on the goal and nothing to press: done (a G:X,Z end is caught above; G:trig here)
                SetKeys(0);
                if (sGoal.kind == GOAL_TRIG) {
                    fprintf(stderr, "pilot: %.*s done at %d,%d zone %u (V-blank %u)\n", (int)(NextTok(tok) - tok), tok,
                            st->gx, st->gz, st->zone, (unsigned)OS_GetVBlankCount());
                }
                return sGoal.kind != GOAL_WARP;
            }
            if (sGoal.kind == GOAL_WALL) {   // the key toward the wall, held until the player is past it
                fprintf(stderr, "pilot: G: at %d,%d, through the wall (dir %d) to %d,%d\n", st->gx, st->gz, plan.press,
                        plan.passX, plan.passZ);
                sPress = plan.press;
                sWallX = plan.passX;
                sWallZ = plan.passZ;
                sWall = 1;
                WallStep(st, tok, sGoal.n, sPress, sWallX, sWallZ, 1);
                return 0;
            }
            if (sGoal.kind == GOAL_RIDE) {
                fprintf(stderr, "pilot: G: at %d,%d, into ride %d (dir %d)\n", st->gx, st->gz, sGoal.n, plan.press);
                sPress = plan.press;
                sRide = 1;
                RideStep(st, tok, sGoal.n, sPress, 1);
                return 0;
            }
            if (plan.handoff) {   // the key held until the game swaps the player onto the other side (above)
                if (onRail) {
                    fprintf(stderr, "pilot: G: at rail[%d,%d,%d], the hand-off to the grid (dir %d)\n", rp.line, rp.front,
                            rp.side, plan.press);
                    // the place reads the new one while the step to it still runs, and where the rail goes on (a switch
                    // place inside a rail: Black 2's Opelucid Gym corridor at 15,46) a key held then walks the player on
                    // along it: no key until the player has stopped (20 frames at most)
                    sSettle = 20;
                    sSetFx = st->fx;
                    sSetFz = st->fz;
                    sSetSame = 0;
                    SetKeys(0);   // (this frame too: the step's key held one frame more starts the next step)
                } else {
                    fprintf(stderr, "pilot: G: at %d,%d, the hand-off to a rail (dir %d)\n", st->gx, st->gz, plan.press);
                }
                sPress = plan.press;
                sPressN = 90;
                return 0;
            }
            if (sGoal.kind == GOAL_NPC || sGoal.kind == GOAL_ITEM || sGoal.kind == GOAL_BG) {
                fprintf(stderr, "pilot: G: at %d,%d, facing the target (dir %d)\n", st->gx, st->gz, plan.press);
                sPress = plan.press;
                sTalk = 1;
                TalkStep(st, tok, sPress, sGoal.talk, 1);
                SetKeys(0);
                return 0;
            }
            if (onRail) {
                fprintf(stderr, "pilot: G: at rail[%d,%d,%d], into the warp (dir %d)\n", rp.line, rp.front, rp.side, plan.press);
            } else {
                fprintf(stderr, "pilot: G: at %d,%d, into the warp (dir %d)\n", st->gx, st->gz, plan.press);
            }
            sPress = plan.press;
            sPressN = 90;
            return 0;
        }
        if (plan.wait) {   // the wind (route_slide.c's PortRouteWind_Hop): stand still until the gust has passed, then plan again
            if (!sWindWait) {
                sWindWait = 1;
                fprintf(stderr, "pilot: G: at %d,%d, %s\n", st->gx, st->gz, plan.why);
            }
            sNoMove = 0;
            SetKeys(0);
            return 0;
        }
        sWindWait = 0;
        if (plan.dismount || sDownT > 0) {   // the rider's next step is onto mud, snow or very tall grass, which the bicycle is refused
                                             // (a step that never happens): get off as the player does (Y, the registered bicycle), wait
                                             // for the state to change and settle, then walk (the plan after this is on foot)
            sNoMove = 0;   // (standing still here is the wait, not a blocked step)
            if (sDownT == 0) {
                const PortRouteAdapter *bad = PortRouteMap_Adapter();

                // a Y while the last step is still running is lost (the walk's hop ends as the step starts): stand still first
                if (st->fx != sDownFx || st->fz != sDownFz) {
                    sDownFx = st->fx;
                    sDownFz = st->fz;
                    sDownSame = 0;
                }
                if (++sDownSame < 4) {
                    SetKeys(0);
                    return 0;
                }
                sDownSame = 0;
                if (++sDownTry > 3) {
                    fprintf(stderr, "pilot: G: the rider did not get off after %d presses of Y\n", sDownTry - 1);
                    GoFail(st, tok, "the rider could not get off the bicycle (Y: is it registered? Bk)");
                }
                if (sDownTry > 1 && bad != NULL && bad->bike != NULL) {
                    bad->bike();   // (the bicycle may not be on the Y button)
                }
                fprintf(stderr, "pilot: G: at %d,%d, the next step is a tile the bicycle is refused: Y to get off\n", st->gx, st->gz);
            }
            sDownT++;
            if (sDownT <= 5) {   // four frames of the key, as Ky:4
                SetKeys(0x800);
                return 0;
            }
            SetKeys(0);
            if (!plan.dismount && sDownT >= 30) {   // on foot, and the get-off has settled: walk on from this plan
                fprintf(stderr, "pilot: G: off the bicycle after %d frames\n", sDownT);
                sDownT = sDownTry = 0;
            } else {
                if (sDownT > 90) {   // still on it: press again (sDownTry bounds the presses)
                    sDownT = 0;
                }
                return 0;
            }
        }
        if (plan.gear) {   // the first run climbs a bike slope (route_slide.c PortRouteSlide_BikeRun) and the bicycle is in its slow gear,
                           // which never builds the speed the slope needs (player_move.c): B toggles the gear (the walk plans again after)
            if (sGearWait > 0) {
                sGearWait--;
                SetKeys(0);
                return 0;
            }
            if (++sGearTry > 2) {
                fprintf(stderr, "pilot: G: the bicycle's gear did not change after %d presses of B\n", sGearTry - 1);
                GoFail(st, tok, "the bicycle's fast gear did not come on (B)");
            }
            fprintf(stderr, "pilot: G: a bike slope on the way: B for the fast gear\n");
            SetKeys(0x2);
            sGearWait = 8;
            return 0;
        }
        if (onRail) {
            sRDir = plan.dir;
            sHave = 1;
            fprintf(stderr, "pilot: G: rail[%d,%d,%d] -> dir %d to rail[%d,%d,%d] (%d steps to go)\n", rp.line, rp.front,
                    rp.side, sRDir, plan.rnext.line, plan.rnext.front, plan.rnext.side, plan.steps);
        } else {
            sWx = plan.x;
            sWz = plan.z;
            sHave = 1;
            fprintf(stderr, "pilot: G: %d,%d -> waypoint %d,%d (%d steps to go)%s%s\n", st->gx, st->gz, sWx, sWz, plan.steps,
                    plan.why[0] ? "; " : "", plan.why);
            WalkTo(st, sWx, sWz, 1);
        }
    }
    if (onRail) {
        SetKeys(kDirKey[sRDir & 3]);
        return 0;
    }
    if (WatchWait(st, sWx, sWz)) {   // the next tile is on a trainer's line of sight: stand until he has turned (a frame count, bounded)
        sNoMove = 0;
        SetKeys(0);
        return 0;
    }
    r = WalkTo(st, sWx, sWz, 0);
    if (r != 0) {   // at the waypoint (1) or the walk's own stuck (2): plan again
        static int sCorner;

        sHave = 0;
        if (r == 1 && !sCorner && OnPush()) {   // on a pushing floor (the Cycling Road going up) the next waypoint's key is set in
                                                // this frame: one frame with no key at the corner sends the rider back down
            sCorner = 1;
            r = GoStep(st, tok, age);
            sCorner = 0;
            return r;
        }
        SetKeys(0);
    }
    return 0;
}

static int WalkTo(const PortRouteState *st, int tx, int tz, int reset);

// the walk's next tile (WalkTo's axis order) is on a line of sight of a trainer who has not been beaten and the player's own tile is on
// none: stand at its edge until a trainer has turned (a trainer who turns at random is freshly turned, so the crossing has the
// longest time before the next turn) and none looks along the line to the tile; 600 frames at most, then go on. 1: wait
static int WatchWait(const PortRouteState *st, int tx, int tz) {
    const PortRouteAdapter *ad = PortRouteMap_Adapter();
    int nx, nz, w;

    if (ad == NULL || ad->watched == NULL || (st->gx == tx && st->gz == tz)) {
        return 0;
    }
    nx = st->gx != tx ? st->gx + (st->gx < tx ? 1 : -1) : st->gx;
    nz = st->gx != tx ? st->gz : st->gz + (st->gz < tz ? 1 : -1);
    w = ad->watched(nx, nz);
    if (w < 0 || ad->watched(st->gx, st->gz) >= 0) {   // not at the edge of a line: the next episode starts afresh
        sWatchSig = -1;
        sWatchTurned = sWatchN = 0;
        return 0;
    }
    if (sWatchSig == -2) {   // gave up at this edge
        return 0;
    }
    if (sWatchSig == -1) {
        sWatchSig = w >> 1;
        sWatchTurned = 0;
        sWatchN = 0;
        fprintf(stderr, "pilot: G: at %d,%d, waiting: the next tile is on a trainer's line of sight (%s)\n", st->gx, st->gz,
                (w & 1) ? "he looks along it" : "he looks elsewhere");
    }
    if ((w & 1) || (w >> 1) != sWatchSig) {
        sWatchTurned = 1;
    }
    if (sWatchTurned && !(w & 1)) {
        fprintf(stderr, "pilot: G: at %d,%d, the trainer has turned away after %d frames: on\n", st->gx, st->gz, sWatchN);
        sWatchSig = -2;   // (this edge is done: the walk goes on, and the next edge starts afresh)
        return 0;
    }
    if (++sWatchN > 600) {
        fprintf(stderr, "pilot: G: at %d,%d, no turn in 600 frames: on\n", st->gx, st->gz);
        sWatchSig = -2;
        return 0;
    }
    return 1;
}

// C:ball's key steps: a comma list of key letters (KeyBits: "D,R,a"), one at a time: down for 2 pilot steps, up for `gap`;
// 1 when the list is done. BWPORT_CATCH_SHOTS=DIR takes a picture after each step (the layout probe)
typedef struct {
    int idx, n;
} TapState;

static int sShotN;

static void CatchShot(const char *why) {
    char name[60];

    if (Env("BWPORT_CATCH_SHOTS")) {   // a relative name: SIM_ShotRequest writes DIR/NAME.png, DIR = BWPORT_SHOTS (the run's shots dir)
        snprintf(name, sizeof name, "c%02d-%s", sShotN++, why);
        SIM_ShotRequest(name);
    }
}

static int TapList(TapState *t, const char *list, int gap) {
    const char *p = list;
    const char *e;
    char tok[16];
    int i, len;

    for (i = 0; i < t->idx && *p != '\0'; i++) {
        while (*p != '\0' && *p != ',') {
            p++;
        }
        if (*p == ',') {
            p++;
        }
    }
    if (*p == '\0') {
        SetKeys(0);
        return 1;
    }
    for (len = 0; p[len] != '\0' && p[len] != ',' && len < 15; len++) {
        tok[len] = p[len];
    }
    tok[len] = '\0';
    SetKeys(t->n < 2 ? KeyBits(tok, &e) : 0);
    if (++t->n == 2 + gap - 1) {
        CatchShot(tok);
    }
    if (t->n >= 2 + gap) {
        t->n = 0;
        t->idx++;
    }
    return 0;
}

static char sBallKeys[48];   // C:ball:ITEM's keys in the Bag, made when the Bag is up (BallKeys)

// C:ball:ITEM's keys in the Bag: R (the Poke Balls pocket) a (open it), then from the slot the cursor starts on (the Bag remembers the
// last one: gPortRouteBattle.ballRow) to the ball's: a page's 6 slots are 2 columns of 3 (slot s: column s % 2, row s / 2), L / R and
// U / D between them; then a (the ball) a (Use). The ball's slot is its place among the bag's balls (gPortRouteBalls). 0 when the bag
// does not list it or it is on another page (the page arrows take the stylus)
static int BallKeys(int item) {
    int t, s, r, dc, dr;
    char *p = sBallKeys;

    for (t = 0; gPortRouteBalls[t] != 0 && gPortRouteBalls[t] != item; t++) {
    }
    if (gPortRouteBalls[t] == 0 || t / 6 != gPortRouteBattle.ballPage) {
        fprintf(stderr, "pilot: C:ball: item %d is not on the Bag's page %d (balls in the bag: %d)\n", item, gPortRouteBattle.ballPage, t);
        return 0;
    }
    s = t % 6;
    r = gPortRouteBattle.ballRow;
    dc = s % 2 - r % 2;
    dr = s / 2 - r / 2;
    p += sprintf(p, "R,a");
    if (dc != 0) {
        p += sprintf(p, dc > 0 ? ",R" : ",L");
    }
    for (; dr != 0; dr += dr > 0 ? -1 : 1) {
        p += sprintf(p, dr > 0 ? ",D" : ",U");
    }
    sprintf(p, ",a,a");
    fprintf(stderr, "pilot: C:ball: item %d is slot %d (the cursor starts on %d): keys %s\n", item, s, r, sBallKeys);
    return 1;
}

// the keys of C:ball, hard-coded from the shots of the layout probe (BWPORT_CATCH_KEYS0 / 1 replace them: at the action menu,
// in the Bag)
static const char *CatchKeys(int step) {
    // menu: D,R,U,L,a ended on BAG and opened it (probe 11-12: the first key press moves the cursor; FIGHT on top, BAG bottom left,
    // RUN bottom middle, POKEMON bottom right: the shortest way to BAG is not known yet). Bag, the pocket grid (HP/PP RESTORE, POKE BALLS
    // / STATUS RESTORE, BATTLE ITEMS; the cursor starts on the first): R = POKE BALLS, a opens it; what follows (the first ball, Use)
    // is still to be probed: "R,a,a,a" and look at the shots (BWPORT_CATCH_SHOTS=1 BWPORT_SHOTS=DIR)
    static const char *kDefault[2] = {"D,R,U,L,a", "R,a,a,a"};
    const char *e = getenv(step == 0 ? "BWPORT_CATCH_KEYS0" : "BWPORT_CATCH_KEYS1");

    if (e == NULL || *e == '\0') {
        return step == 1 && sBallKeys[0] != '\0' ? sBallKeys : kDefault[step];
    }
    return e;
}

// C:ball[:ITEM] (age: the frames this token has run): catch a wild Pokemon with the first Poke Ball or with ITEM. Phase 0 paces one step left and
// right with the encounters on until a wild battle starts; phase 1 plays the battle (A through the text, then at the action menu the
// keys for Bag, the Poke Balls pocket, the first ball; again when the menu comes back after a ball that missed); phase 2 waits for
// the field and checks that the party grew. 1 when done
static int CatchStep(const PortRouteState *st, int age, const char *tok) {
    static int sPhase, sTx, sX0, sDir, sParty, sSeq, sFree, sBagFrames, sTries, sTextFrames, sKeysDone, sYesNo, sYesNoAge, sBall;
    static unsigned sStartV;
    static TapState sTap;
    unsigned now = OS_GetVBlankCount();
    int inBattle = PortOvl_IsLoaded(PORT_Game->ovlBattle);
    int talk = strncmp(tok, "C:talk", 6) == 0;   // Cc's talk (CatchTalk): the battle is already up, called in it only
    int menu, bag, r;

    if (age == 1) {
        if (!talk) {
            sEncOverride = 0;
        }
        gPortRouteCatch = 1;
        sPhase = sSeq = sFree = sTries = sTextFrames = sYesNo = 0;
        sBall = 0;
        sscanf(tok, "C:ball:%d", &sBall);
        sBallKeys[0] = '\0';
        sStartV = now;
        sParty = gPortRoutePartyCount;
        sX0 = st->gx;
        sDir = 1;
        sTx = sX0 + sDir;
        if (talk) {
            sPhase = 1;
            sBagFrames = 0;
            sTap.idx = sTap.n = 0;
            memset(&gPortRouteBattle, 0, sizeof gPortRouteBattle);
            fprintf(stderr, "pilot: Cc: the scripted battle is a catch, party %d (V-blank %u)\n", sParty, now);
        } else {
            WalkTo(st, 0, 0, 1);
            fprintf(stderr, "pilot: C:ball: encounters on at %d,%d, party %d (V-blank %u)\n", st->gx, st->gz, sParty, now);
        }
    }
    if (now - sStartV > (unsigned)TokenTimeout()) {   // V-blanks, not pilot steps: the Bag and the battle's own procs run unseen
        SetKeys(0);
        sEncOverride = 1;
        gPortRouteCatch = 0;
        fprintf(stderr, "pilot: timeout: C:ball not done in %u V-blanks (phase %d, tries %d)\n", now - sStartV, sPhase, sTries);
        End(st, "timeout");
        return 0;
    }
    if (sPhase == 0) {
        if (inBattle) {
            sPhase = 1;
            sSeq = sBagFrames = 0;
            sTap.idx = sTap.n = 0;
            memset(&gPortRouteBattle, 0, sizeof gPortRouteBattle);
            fprintf(stderr, "pilot: C:ball: a wild battle at V-blank %u\n", now);
        } else if (st->event) {   // the encounter's transition, or a scene
            PressEvent(st);
            return 0;
        } else {
            r = WalkTo(st, sTx, st->gz, 0);   // a step along the row
            if (r == 1) {
                sTx = sTx == sX0 ? sX0 + sDir : sX0;
                WalkTo(st, 0, 0, 1);
            } else if (r == 2) {   // blocked: the other side
                sDir = -sDir;
                sTx = sX0 + sDir;
                WalkTo(st, 0, 0, 1);
            }
            return 0;
        }
    }
    if (sPhase == 1) {
        if (!inBattle) {
            sPhase = 2;
            sFree = sTextFrames = 0;
            SetKeys(0);
            fprintf(stderr, "pilot: C:ball: the battle is over at V-blank %u\n", now);
            return 0;
        }
        menu = gPortRouteBattle.menuVblank != 0 && gPortRouteBattle.menuVblank + 3 >= now;
        bag = gPortRouteBattle.bagVblank != 0 && gPortRouteBattle.bagVblank + 3 >= now;
        if (bag) {
            if (sSeq != 2) {
                sSeq = 2;
                sTap.idx = sTap.n = 0;
                sBagFrames = 0;
                fprintf(stderr, "pilot: C:ball: the Bag is up at V-blank %u\n", now);
                CatchShot("bag");
                if (sBall != 0 && !BallKeys(sBall)) {
                    SetKeys(0);
                    sEncOverride = 1;
                    gPortRouteCatch = 0;
                    End(st, "catch failed: ball not in the Bag");
                    return 0;
                }
            }
            if (++sBagFrames < 40) {   // the overlay's own start
                SetKeys(0);
            } else if (!TapList(&sTap, CatchKeys(1), 20)) {
                sKeysDone = 0;
            } else if (++sKeysDone > 300) {   // the keys ended and the Bag is still up (no ball went): B backs out to the action menu
                SetKeys((sKeysDone & 3) < 2 ? 2u : 0);
            }
            return 0;
        }
        if (menu && sSeq != 1) {
            sSeq = 1;
            sTap.idx = sTap.n = 0;
            fprintf(stderr, "pilot: C:ball: the action menu is up at V-blank %u\n", now);
            CatchShot("menu");
        }
        if (menu) {
            TapList(&sTap, CatchKeys(0), 20);
            return 0;
        }
        if (sSeq == 2) {   // the Bag closed
            fprintf(stderr, "pilot: C:ball: the Bag closed: item %d pocket %d done %d ballError %d (V-blank %u)\n",
                    gPortRouteBattle.bagItem, gPortRouteBattle.bagPocket, gPortRouteBattle.bagDone, gPortRouteBattle.bagBallError,
                    now);
            sSeq = 3;
            sTextFrames = 0;
            if (sBall != 0 && gPortRouteBattle.bagItem != 0 && gPortRouteBattle.bagItem != sBall) {
                fprintf(stderr, "pilot: C:ball: item %d went, %d was asked for\n", gPortRouteBattle.bagItem, sBall);
                SetKeys(0);
                sEncOverride = 1;
                gPortRouteCatch = 0;
                End(st, "catch failed: another ball");
                return 0;
            }
            if (gPortRouteBattle.bagItem == 0 || gPortRouteBattle.bagBallError != 0) {   // closed without a ball: the menu comes back
                fprintf(stderr, "pilot: C:ball: no ball was thrown (try %d)\n", sTries + 1);
                if (++sTries >= 3) {
                    SetKeys(0);
                    sEncOverride = 1;
                    gPortRouteCatch = 0;
                    End(st, "catch failed: no ball thrown");
                    return 0;
                }
            }
        }
        if (Env("BWPORT_CATCH_SHOTS") && sTextFrames % 60 == 59 && sShotN < 60) {
            CatchShot("text");
        }
        sTextFrames++;
        PressEvent(st);   // text: A through it (the battle's events press A every 4th frame)
        return 0;
    }
    // phase 2: the battle is over: the field free for 30 frames, then the party must have grown. The nickname question
    // (the nickname question, a yes/no after the Pokedex page) is answered No (B), as a Pn token does
    if (gPortRouteFieldYesNo != 0 || sYesNo) {
        sYesNo = YesNoStep(st, "Pn", sYesNo ? ++sYesNoAge : (sYesNoAge = 1)) ? 0 : 1;
        sFree = 0;
        return 0;
    }
    if (st->event) {
        sFree = 0;
        if (Env("BWPORT_CATCH_SHOTS") && sTextFrames % 30 == 29 && sShotN < 80) {
            CatchShot("field");
        }
        sTextFrames++;
        PressEvent(st);
        return 0;
    }
    SetKeys(0);
    if (++sFree < 30) {
        return 0;
    }
    fprintf(stderr, "pilot: C:ball: party %d -> %d\n", sParty, gPortRoutePartyCount);
    sEncOverride = 1;
    gPortRouteCatch = 0;
    if (gPortRoutePartyCount <= sParty) {
        End(st, "catch failed");
    }
    return 1;
}

PortRouteCheat gPortRouteCheat;   // the X: cheats' queue (port_route_cheat.h): the games' field hook runs it

// X:money:N (set the money) X:item:ID:N (add N of an item) X:mon:SPECIES:LEVEL (add a party Pokemon) X:badge:N: queue it, then
// wait until the field hook has run it (the hook logs one "cheat:" line); 1 when done
static int CheatStep(const char *tok) {
    static const char *sQueued;
    PortRouteCheat *c = &gPortRouteCheat;

    if (sQueued != tok) {
        memset(c, 0, sizeof(*c));
        if (sscanf(tok, "X:money:%d", &c->a) == 1) {
            c->kind = PORT_CHEAT_MONEY;
        } else if (sscanf(tok, "X:item:%d:%d", &c->a, &c->b) == 2) {
            c->kind = PORT_CHEAT_ITEM;
        } else if (sscanf(tok, "X:mon:%d:%d:%d:%d", &c->a, &c->b, &c->c, &c->d) >= 2) {   // c = 1: the new Pokemon leads the party; d = move 1
            c->kind = PORT_CHEAT_MON;
        } else if (sscanf(tok, "X:badge:%d", &c->a) == 1) {
            c->kind = PORT_CHEAT_BADGE;
        } else if (strncmp(tok, "X:heal", 6) == 0 && (tok[6] == '\0' || tok[6] == ' ')) {
            c->kind = PORT_CHEAT_HEAL;
        } else if (strncmp(tok, "X:work:", 7) == 0) {   // X:work:ID:VAL (ID as story.py prints it: 0x4088; VAL decimal or 0x)
            char *e;

            c->kind = PORT_CHEAT_WORK;
            c->a = (int)strtol(tok + 7, &e, 0);
            c->b = *e == ':' ? (int)strtol(e + 1, NULL, 0) : 0;
        } else if (strncmp(tok, "X:flag:", 7) == 0) {   // X:flag:ID:0|1
            char *e;

            c->kind = PORT_CHEAT_FLAG;
            c->a = (int)strtol(tok + 7, &e, 0);
            c->b = *e == ':' ? (int)strtol(e + 1, NULL, 0) != 0 : 1;
        } else if (strncmp(tok, "R:script:", 9) == 0) {   // R:script:ZONE:ID
            char *e;

            c->kind = PORT_CHEAT_SCRIPT;
            c->a = (int)strtol(tok + 9, &e, 0);
            c->b = *e == ':' ? (int)strtol(e + 1, NULL, 0) : 0;
        } else if (strncmp(tok, "X:warp:", 7) == 0) {   // X:warp:ZONE:X:Z[:DIR]
            char *e;

            c->kind = PORT_CHEAT_WARP;
            c->a = (int)strtol(tok + 7, &e, 0);
            c->b = *e == ':' ? (int)strtol(e + 1, &e, 0) : 0;
            c->c = *e == ':' ? (int)strtol(e + 1, &e, 0) : 0;
            c->d = *e == ':' ? (int)strtol(e + 1, &e, 0) : 1;
        } else {
            fprintf(stderr, "pilot: X: unknown cheat %.*s\n", (int)(NextTok(tok) - tok), tok);
            _exit(2);
        }
        sQueued = tok;
        c->pending = 1;
        return 0;
    }
    return c->done;
}

// T:X,Y[>X2,Y2][:N] (the header): the stylus for N frames through libntr's bwport_touch_force (libntr-touchforce.patch), then up
// for 2 frames so the game sees the lift. Returns 1 when the token is done.
extern volatile unsigned int bwport_touch_force;

static int TouchStep(const char *tok) {
    static int sAge;
    char *p;
    int x0, y0, x1, y1, n = 4, x, y;

    x0 = (int)strtol(tok + 2, &p, 10);
    y0 = *p == ',' ? (int)strtol(p + 1, &p, 10) : 0;
    x1 = x0;
    y1 = y0;
    if (*p == '>') {
        x1 = (int)strtol(p + 1, &p, 10);
        y1 = *p == ',' ? (int)strtol(p + 1, &p, 10) : 0;
    }
    if (*p == ':') {
        n = atoi(p + 1);
    }
    if (n < 1) {
        n = 1;
    }
    sAge++;
    if (sAge <= n) {
        x = n > 1 ? x0 + (x1 - x0) * (sAge - 1) / (n - 1) : x0;
        y = n > 1 ? y0 + (y1 - y0) * (sAge - 1) / (n - 1) : y0;
        x = x < 0 ? 0 : x > 255 ? 255 : x;
        y = y < 0 ? 0 : y > 191 ? 191 : y;
        if (sAge == 1) {
            fprintf(stderr, "pilot: touch %d,%d%s for %d frames at V-blank %u\n", x0, y0, x1 != x0 || y1 != y0 ? " (drag)" : "", n,
                    (unsigned)OS_GetVBlankCount());
        }
        bwport_touch_force = 0x80000000u | (unsigned)y << 8 | (unsigned)x;
        return 0;
    }
    bwport_touch_force = 0;
    if (sAge <= n + 2) {
        return 0;
    }
    sAge = 0;
    return 1;
}
#include "route_fly.h"         // F:fly:ZONE (FlyStep, called by FieldMoveStep)
#include "route_teach.h"       // F:teach:ITEM:SPECIES[:FORGET] (TeachStep, called by FieldMoveStep)
#include "route_fieldmove.h"   // the F: tokens (FieldMoveStep)

static void Pilot(const PortRouteState *st) {
    static const char *sTok, *sAgeTok;
    static int sInit, sHold, sStuck, sIdle, sLastX, sLastZ, sTotalStuck, sZone = -1, sAge;
    static unsigned sHoldKeys;
    static int sMash, sMashZone = -1, sMashQuiet;
    static const char *sZoneTok;
    static int sPrevEvent;
    static int sPrevFx, sPrevFz, sStill, sBest = -1;
    const char *e;
    int tx, tz, zfirst, vx, vz, dist, xaxis, d, v;
    unsigned k;

    if (st->event != sPrevEvent && st->zone != 0xffff) {
        fprintf(stderr, "pilot: event %s at V-blank %u\n", st->event ? "on" : "off", (unsigned)OS_GetVBlankCount());
    }
    sPrevEvent = st->event;
    if (!sInit) {
        sInit = 1;
        sTok = getenv("BWPORT_PILOT");
        if (sTok != NULL && *sTok == '\0') {
            sTok = NULL;
        }
    }
    if (sTok == NULL) {
        return;
    }
    while (*sTok == ' ') {
        sTok++;
    }
    sRest = sTok;
    if (*sTok == '\0') {
        SetKeys(0);
        End(st, "script done");
    }
    if (sTok[0] == 'T' && sTok[1] == 'o' && sTok[2] == ':' && sMash == 0 && sHold == 0) {   // To:N: the next token's timeout
        sTimeoutNext = atoi(sTok + 3);
        fprintf(stderr, "pilot: %.*s: the next token may run %d frames\n", (int)(NextTok(sTok) - sTok), sTok, sTimeoutNext);
        sTok = NextTok(sTok);
        return;
    }
    if (sMash > 0) {   // an M token: the pulse key every 4th step, whatever runs (a title, a new game's intro)
        SetKeys((++sCnt & 3) == 0 ? sPulse : 0);
        if (sMashZone >= 0) {   // Mz:N: until the field has been up and free in zone N for 30 frames
            sMashQuiet = (int)st->zone == sMashZone && !st->event ? sMashQuiet + 1 : 0;
            if (sMashQuiet >= 30) {
                sMash = 0;
                SetKeys(0);
                sTok = NextTok(sTok);
            } else if (--sMash == 0) {
                fprintf(stderr, "pilot: timeout: %.*s not done in %d frames\n", (int)(NextTok(sTok) - sTok), sTok, TokenTimeout());
                End(st, "timeout");
            }
            return;
        }
        if (--sMash == 0) {
            sTok = NextTok(sTok);
        }
        return;
    }
    if (sHold > 0) {   // a held key token
        SetKeys(sHoldKeys);
        if (--sHold == 0) {
            sTok = NextTok(sTok);
        }
        return;
    }
    if (sTok != sAgeTok) {
        sAgeTok = sTok;
        sAge = 0;
        sTimeoutTok = sTimeoutNext;   // a To:N just before it, else none
        sTimeoutNext = 0;
    }
    if (++sAge > TokenTimeout()) {
        fprintf(stderr, "pilot: timeout: %.*s not done in %d frames\n", (int)(NextTok(sTok) - sTok), sTok, sAge - 1);
        End(st, "timeout");
    }
    if (*sTok == 'P' && (sTok[1] == 'y' || sTok[1] == 'n')) {   // a field question: its own no-list check, not HangCheck's
        if (YesNoStep(st, sTok, sAge)) {
            sTok = NextTok(sTok);
        }
        return;
    }
    if (PkTokenMode(sTok) != 0) {   // the naming screen: its own no-screen check too
        if (NameStep(st, sAge, PkTokenMode(sTok) == 2)) {
            sTok = NextTok(sTok);
        }
        return;
    }
    if (strncmp(sTok, "Pl:", 3) == 0) {   // a script's list menu: its own no-list check too
        if (ListStep(st, sTok, sAge)) {
            sTok = NextTok(sTok);
        }
        return;
    }
    HangCheck(st, sTok);
    if (*sTok == 'W' && (sTok[1] == 'f' || sTok[1] == 't' || sTok[1] == 'b' || sTok[1] == 'z' || sTok[1] == 's' || sTok[1] == 'r')) {
        if (WaitStep(st, sTok, sAge)) {
            fprintf(stderr, "pilot: %.*s done at V-blank %u\n", (int)(NextTok(sTok) - sTok), sTok,
                    (unsigned)OS_GetVBlankCount());
            sTok = NextTok(sTok);
        }
        return;
    }
    if (*sTok == 'S' && st->zone == 0xffff) {   // no field yet: the save waits for it
        PressEvent(st);
        return;
    }
    if (*sTok == 'S') {
        if (SaveStep(st, sAge)) {
            sTok = NextTok(sTok);
        }
        return;
    }
    if (*sTok == 'E' && sTok[1] == ':') {   // E:on / E:off: wild encounters
        sEncOverride = strncmp(sTok + 2, "on", 2) == 0 ? 0 : 1;
        fprintf(stderr, "pilot: encounters %s at V-blank %u\n", sEncOverride ? "off" : "on", (unsigned)OS_GetVBlankCount());
        sTok = NextTok(sTok);
        return;
    }
    if (strncmp(sTok, "X:time:", 7) == 0) {   // X:time:MONTH:DAY:HOUR: the RTC (libntr's pinned clock, BWPORT_BENCH_CLOCK) moves there;
        static int sM, sD, sH;                // the game's cache follows within 11 frames: done when it reads it (60 frames at most)
        char clock[40];
        int year = 2026;

        if (sAge == 1) {
            sscanf(sTok, "X:time:%d:%d:%d", &sM, &sD, &sH);
            if (getenv("BWPORT_BENCH_CLOCK") != NULL) {
                sscanf(getenv("BWPORT_BENCH_CLOCK"), "%d", &year);
            }
            snprintf(clock, sizeof clock, "%d-%02d-%02dT%02d:00:00", year, sM, sD, sH);
#ifdef _WIN32
            _putenv_s("BWPORT_BENCH_CLOCK", clock);
#else
            setenv("BWPORT_BENCH_CLOCK", clock, 1);
#endif
            fprintf(stderr, "pilot: X:time: clock set to %s (game reads %d/%d %d:00, season %d)\n", clock, gPortRouteClock[0],
                    gPortRouteClock[1], gPortRouteClock[2], gPortRouteClock[3]);
        }
        if (sAge >= 60 || (gPortRouteClock[0] == sM && gPortRouteClock[1] == sD && gPortRouteClock[2] == sH)) {
            fprintf(stderr, "pilot: X:time: the game reads %d/%d %d:00, season %d after %d frames%s\n", gPortRouteClock[0],
                    gPortRouteClock[1], gPortRouteClock[2], gPortRouteClock[3], sAge, sAge >= 60 ? " (NOT the clock set)" : "");
            sTok = NextTok(sTok);
        }
        return;
    }
    if (*sTok == 'X' && sTok[1] == ':') {   // X:money:N, X:item:ID:N, X:mon:SPECIES:LEVEL, X:badge:N: test cheats
        if (st->zone == 0xffff) {   // no field yet: wait for it
            PressEvent(st);
        } else if (CheatStep(sTok)) {
            sTok = NextTok(sTok);
        }
        return;
    }
    if (*sTok == 'T' && sTok[1] == ':') {   // T:X,Y[:N]: the stylus, whatever runs
        if (TouchStep(sTok)) {
            sTok = NextTok(sTok);
        }
        return;
    }
    if (*sTok == 'R' && sTok[1] == ':') {   // R:script:ZONE:ID: run a zone script (a repeatable fallback for a puzzle), then press
        static int sQuiet;                  // through its event until the field is free for 8 frames
        if (st->zone == 0xffff) {
            PressEvent(st);
        } else if (!CheatStep(sTok)) {
            sQuiet = 0;
            if (st->event) {   // another event is running: press through it, the hook waits for a free field
                PressEvent(st);
            } else {
                SetKeys(0);
            }
        } else if (gPortRouteCheat.done == 2) {
            End(st, "script zone");
        } else if (WaitsOnEvent(NextTok(sTok) + (*NextTok(sTok) == ' '))) {   // the next token answers the script's own question: hand over at once
            fprintf(stderr, "pilot: %.*s started at V-blank %u\n", (int)(NextTok(sTok) - sTok), sTok, (unsigned)OS_GetVBlankCount());
            sTok = NextTok(sTok);
        } else if (st->event) {
            sQuiet = 0;
            PressEvent(st);
        } else {
            SetKeys(0);
            if (++sQuiet >= 8) {
                sQuiet = 0;
                fprintf(stderr, "pilot: %.*s done at V-blank %u\n", (int)(NextTok(sTok) - sTok), sTok, (unsigned)OS_GetVBlankCount());
                sTok = NextTok(sTok);
            }
        }
        return;
    }
    if (*sTok == 'C' && sTok[1] == 'c') {   // Cc: the battle of the next talk (G:npc ...) is a catch (a scripted wild battle)
        sCatchTalk = 1;
        fprintf(stderr, "pilot: Cc: the next talk's battle is a catch\n");
        sTok = NextTok(sTok);
        return;
    }
    if (*sTok == 'C' && sTok[1] == ':') {   // C:ball: catch a wild Pokemon
        if (st->zone == 0xffff) {   // no field yet (a leg's start): wait for it, as G does
            PressEvent(st);
        } else if (CatchStep(st, sAge, sTok)) {
            sTok = NextTok(sTok);
        }
        return;
    }
    switch (*sTok) {
    case 'K':
        sHoldKeys = KeyBits(sTok + 1, &e);
        sHold = atoi(e + 1);
        SetKeys(sHoldKeys);
        if (sHold <= 1) {
            sHold = 0;
            sTok = NextTok(sTok);
        }
        return;
    case 'M':
        if (sTok[1] == 'd') {
            if (st->zone == 0xffff || !sInFrame || !PortRouteMap_Get()->ok) {   // no field yet, not its frame: wait
                PressEvent(st);
                return;
            }
            DumpMap(st, "md");
            sTok = NextTok(sTok);
            return;
        }
        sMashZone = sTok[1] == 'z' && sTok[2] == ':' ? atoi(sTok + 3) : -1;
        sMashQuiet = 0;
        sMash = sMashZone >= 0 ? TokenTimeout() : atoi(sTok + 1);
        if (sMash <= 0) {
            sTok = NextTok(sTok);
        }
        return;
    case 'B':
        if (sTok[1] == 'k') {   // Bk: the bicycle on the Y button (Gen 4: the bag's Register); Ky:4 then mounts it
            const PortRouteAdapter *ad = PortRouteMap_Adapter();

            if (st->zone == 0xffff || !sInFrame || ad == NULL) {   // no field yet, not its frame: wait
                PressEvent(st);
                return;
            }
            if (ad->bike == NULL || !ad->bike()) {
                fprintf(stderr, "pilot: Bk: the bag holds no bicycle (or this game has no hook for it)\n");
                End(st, "no bicycle");
                return;
            }
            sTok = NextTok(sTok);
            return;
        }
        break;
    case 'P':
        sPulse = sTok[1] == 'a' ? 1 : 2;
        sTok = NextTok(sTok);
        return;
    case 'C':
        if (st->zone == 0xffff) {   // no field yet (a leg's start): wait for it, as G does
            PressEvent(st);
            return;
        }
        End(st, "end token");
    }
    if (*sTok == 'G' && sTok[1] == ':') {
        if (st->zone != 0xffff && GoStep(st, sTok, sAge)) {
            sTok = NextTok(sTok);
        } else if (st->zone == 0xffff) {
            PressEvent(st);
        }
        return;
    }
    if (*sTok == 'F' && sTok[1] == ':') {   // a field move (route_fieldmove.h)
        if (st->zone != 0xffff && FieldMoveStep(st, sTok, sAge)) {
            sTok = NextTok(sTok);
        } else if (st->zone == 0xffff) {
            PressEvent(st);
        }
        return;
    }
    if (st->event) {   // a scene, text or a battle: press through it
        sIdle = 0;
        PressEvent(st);
        return;
    }
    if (*sTok == 'W') {
        if (++sIdle >= atoi(sTok + 1)) {
            sIdle = 0;
            sTok = NextTok(sTok);
        }
        SetKeys(0);
        return;
    }
    // a walk: [z]X,Z[!]
    zfirst = *sTok == 'z';
    e = sTok + zfirst;
    tx = atoi(e);
    while (*e != '\0' && *e != ',') {
        e++;
    }
    tz = atoi(e + 1);
    if (sZoneTok != sTok) {
        sZoneTok = sTok;
        sZone = (int)st->zone;
        sBest = -1;
    }
    if ((int)st->zone != sZone) {
        const char *end = NextTok(sTok);

        if (end > sTok && end[-1] == '!') {   // a zone change ends this walk
            fprintf(stderr, "pilot: zone %d -> %u at %d,%d (V-blank %u)\n", sZone, st->zone, st->gx, st->gz,
                    (unsigned)OS_GetVBlankCount());
            sStuck = 0;
            sTok = end;
            SetKeys(0);
            return;
        }
    }
    // the walk steers by the fine position (st->fx, fz: 16 a tile, a tile's center at +8). The game decides to go on to
    // the next tile a frame or two before a step ends, so a key let go when the grid position reaches the target is too
    // late at the fast walk (4x: a step is 2 frames): it took one more step, turned back, and swung around the target for
    // good (the old 107,662 hang of aspc-alder). So the key is let go 3 frames' movement before the target's center, and
    // the last step from standing still is a one-frame tap (a held key would start a second step).
    vx = abs(st->fx - sPrevFx);
    vz = abs(st->fz - sPrevFz);
    sStill = vx != 0 || vz != 0 ? 0 : sStill + 1;
    sPrevFx = st->fx;
    sPrevFz = st->fz;
    // there by the grid (a step onto the target may start a scene at once: the walk must be done before it)
    if (st->gx == tx && st->gz == tz) {
        fprintf(stderr, "pilot: at %d,%d (V-blank %u)\n", st->gx, st->gz, (unsigned)OS_GetVBlankCount());
        sStuck = 0;
        sTotalStuck = 0;
        sBest = -1;
        sTok = NextTok(sTok);
        SetKeys(0);
        return;
    }
    dist = abs(st->gx - tx) + abs(st->gz - tz);
    if (st->gx != sLastX || st->gz != sLastZ) {   // a step: the blocked-axis count starts again
        sLastX = st->gx;
        sLastZ = st->gz;
        sStuck = 0;
    }
    if (sBest < 0 || dist < sBest) {   // nearer than ever: not stuck (a walk that swings around its target is)
        sBest = dist;
        sTotalStuck = 0;
    }
    sStuck++;
    if (++sTotalStuck == 2000) {
        fprintf(stderr, "pilot: stuck at %d,%d going to %d,%d\n", st->gx, st->gz, tx, tz);
        End(st, "stuck");
    }
    xaxis = zfirst ? st->gz == tz : st->gx != tx;
    if (sStuck > 240 && st->gx != tx && st->gz != tz) {   // blocked on one axis: try the other
        xaxis = !xaxis;
    }
    if (xaxis) {
        k = st->gx < tx ? 0x10 : st->gx > tx ? 0x20 : st->fx < tx * 16 + 8 ? 0x10 : 0x20;
        d = abs(tx * 16 + 8 - st->fx);
        v = vx;
    } else {
        k = st->gz < tz ? 0x80 : st->gz > tz ? 0x40 : st->fz < tz * 16 + 8 ? 0x80 : 0x40;
        d = abs(tz * 16 + 8 - st->fz);
        v = vz;
    }
    if (v != 0) {   // moving on this axis: hold on until 3 frames' movement before the center
        SetKeys(d > 3 * v ? k : 0);
    } else if (d > 16) {   // more than a step to go: hold (it is let go in time once moving)
        SetKeys(k);
    } else {   // a step or less to go: a one-frame tap once still for 4 frames (a turn first takes one too)
        SetKeys(sStill >= 4 && (sStill & 3) == 0 ? k : 0);
    }
}


// one waypoint of a G walk, straight on one axis (route_map.c's waypoints are): the X,Z walk's steering. 1 there, 2 stuck
static int WalkTo(const PortRouteState *st, int tx, int tz, int reset) {
    static int sPrevFx, sPrevFz, sStill, sStuck;
    int vx, vz, d, v, xaxis;
    unsigned k;

    if (reset) {
        sStuck = 0;
        return 0;
    }
    vx = abs(st->fx - sPrevFx);
    vz = abs(st->fz - sPrevFz);
    sStill = vx != 0 || vz != 0 ? 0 : sStill + 1;
    sPrevFx = st->fx;
    sPrevFz = st->fz;
    if (st->gx == tx && st->gz == tz) {
        SetKeys(0);
        return 1;
    }
    if (++sStuck > 600) {
        return 2;
    }
    xaxis = st->gx != tx;
    if (xaxis) {
        k = st->gx < tx ? 0x10 : st->gx > tx ? 0x20 : st->fx < tx * 16 + 8 ? 0x10 : 0x20;
        d = abs(tx * 16 + 8 - st->fx);
        v = vx;
    } else {
        k = st->gz < tz ? 0x80 : st->gz > tz ? 0x40 : st->fz < tz * 16 + 8 ? 0x80 : 0x40;
        d = abs(tz * 16 + 8 - st->fz);
        v = vz;
    }
    if (v != 0) {
        SetKeys(d > 3 * v || OnPush() ? k : 0);   // (on a pushing floor the key stays down: the corner sets the next one)
    } else if (d > 16 || OnPush()) {
        SetKeys(k);   // (on a pushing floor, the Cycling Road's bridge, a tap only turns: hold the key)
    } else {
        SetKeys(sStill >= 4 && (sStill & 3) == 0 ? k : 0);
    }
    return 0;
}

// BWPORT_PILOT_TRACE=1: one line a field frame (V-blank, zone, grid and fine position, event, save, text, the menu's
// input and cursor, the keys the pilot holds): to see what a leg waits on
static void Trace(const PortRouteState *st) {
    static int sOn = -1;

    if (sOn < 0) {
        sOn = Env("BWPORT_PILOT_TRACE");
    }
    if (sOn) {
        unsigned keys = (~s_reg_PAD_KEYINPUT & 0x3ffu) | (~*(vu16 *)HW_BUTTON_XY_BUF & 0xc00u);

        fprintf(stderr, "trace: v%u z%u %d,%d f%d,%d e%d s%d t%d m%d%d k%03x\n", (unsigned)OS_GetVBlankCount(), st->zone,
                st->gx, st->gz, st->fx, st->fz, st->event, st->save, st->text, gPortRouteMenu.input,
                gPortRouteMenu.onSave, keys);
    }
}

void PortRoute_Frame(const PortRouteState *st) {
    static int sLog = -1;
    static unsigned sLastZone;
    static int sLastX = -1, sLastZ = -1;

    if (sLog < 0) {
        sLog = Env("BWPORT_POSLOG");
    }
    if (sLog && (st->zone != sLastZone || st->gx != sLastX || st->gz != sLastZ)) {
        fprintf(stderr, "pos: %s Z zone %u %d,%d (V-blank %u)\n", st->game, st->zone, st->gx, st->gz,
                (unsigned)OS_GetVBlankCount());
        sLastZone = st->zone;
        sLastX = st->gx;
        sLastZ = st->gz;
    }
    sValid = 1;
    sCalled = 1;
    sLast = *st;
    if (PortOvl_IsLoaded(PORT_Game->ovlBattle)) {   // a battle runs under the field's game system: an event too
        PortRouteState ev = *st;

        ev.event = 2;
        sInFrame = 1;
        Pilot(&ev);
        sInFrame = 0;
        Trace(&ev);
        return;
    }
    sInFrame = 1;
    Pilot(st);
    sInFrame = 0;
    Trace(st);
}

// Called at each V-blank wait (main_threads.c's PORT_WaitIrq, the game lock held). Until the field's first frame the pilot
// runs from here with no player (every walk or wait token is "an event runs": it presses the pulse key), so a script can
// press through a title, a menu and a new game's intro. Later the pilot runs in PortRoute_Frame, except in a frame it was
// not called for 6 V-blanks: the field's game system is not updating (the Bag, a scene's own proc), which is an event too
void PortRoute_VBlank(void) {
    static const PortRouteState sNone = {"?", 0xffff, -1, -1, 1, -1, -1, 0, 0};
    static int sOn = -1, sMissed;

    if (sOn < 0) {
        sOn = getenv("BWPORT_PILOT") != NULL;
    }
    if (!sOn) {
        return;
    }
    if (!sValid) {
        Pilot(&sNone);
    } else if (sCalled) {
        sMissed = 0;
    } else if (++sMissed >= 6) {   // (the game system runs every 2nd V-blank in places: a missed frame or two is not it)
        PortRouteState st = sLast;

        // a battle too (the same test as PortRoute_Frame's): Platinum's field system does not update while its battle
        // runs, so this is the only frame the pilot gets there, and event 2 is what presses A in the action menu
        st.event = PortOvl_IsLoaded(PORT_Game->ovlBattle) ? 2 : 1;
        Pilot(&st);
    }
    sCalled = 0;
}
