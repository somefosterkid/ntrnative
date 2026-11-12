// Test hooks for the playthrough routes (docs/port-worker.md, "Routes"; tools/route.py). Everything here is off unless its
// environment variable is set; none of it is in the UI or launcher.ini.
//
// BWPORT_NOENCOUNTER=1  no wild encounters while walking or surfing, as a Repel would (the stand-ins of the games'
//                       EventWildBattleCall_CreateRandom); trainers, scripts and static encounters still happen.
// BWPORT_TESTCHEAT=win  battles play in full, but the player's side moves first, its attacks one-shot and it takes no
//                       damage (the stand-ins of the games' battle_event.c).
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
//                         Wb        a battle starts and ends, then the field is free (30 frames); presses through
//                         Wz:N      the player is in zone N and the field is free (8 frames); presses through
//                         Ws:FLAG   a story beat: the event flag FLAG is set (a name or a number), then the field is
//                                   free (8 frames); Ws:!FLAG cleared, Ws:WORK=V a work value equal to V; presses
//                                   through (port/src/route_story.c; the map dump's story section lists them)
//                         Mn        press the pulse key every 4th step for n steps, whatever runs (a title, an intro)
//                         Pa Pb     the pulse key during events: A, or B (the default)
//                         Py Pn     a question in the field (a script's yes/no list, not a battle's): press the pulse key
//                                   through the event until the yes/no list is up, then answer Yes (A; up first when the
//                                   cursor starts on No) or No (B), and go on once the list has the answer; no list in
//                                   BWPORT_PILOT_HANG frames (3000): "pilot: hang: Py: no yes/no list" and the end
//                         S         the field menu's SAVE: Black 2, White 2 wait for the field, X, the menu's input, down
//                                   until the cursor is on SAVE, A, A at the save's questions and B until the field is
//                                   free again; Black, White: X, down twice, A, then A, fixed frames
//                         C         end the run here: print the position, quit (the script's end does too); before the
//                                   field is up (a leg's start) it waits for it, pressing the pulse key
//                         Md        write a map dump now (port/src/route_map.c: the walkable grid, warps, triggers, actors);
//                                   it waits for the field's own frame (a leg's start, the Bag, a scene's own proc)
//                         G:X,Z     walk to grid X,Z on a planned path (A* on the game's own terrain and collision, actors
//                                   in the way, ledges one way; port/src/route_map.c), across zone edges where the map
//                                   goes on (the tiles not loaded yet are tried), re-planned at each corner and when a
//                                   step does not happen; "pilot: no path: ..." and the end when there is none
//                         G:warp:N  the same to a warp leading to zone N (onto it, or next to it and into it), or where
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
//                         G:bg:N    next to BG event N (the dump's bN), facing it, A, the event's end; G:bg:N:nota
//                       A target token that cannot be done (no such actor, no path, no turn, no event after A three
//                       times) prints "pilot: no path: WHY (G token ...)", writes a map dump and ends the run.
//                       In a battle every token presses A (the first move), except at the forget-a-move and the "switch
//                       Pokemon?" questions: B, No ("pilot: battle yes/no question: answering No"; the games' btlv_core.c
//                       stand-ins set gPortRouteYesNo).
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
                            // White 2): the field's yes/no list is up (a script's question): 1 + its cursor's row (0 Yes, 1 No)
PortRouteMenu gPortRouteMenu;

int PortRoute_NoEncounter(const char *game) {
    static int sOn = -1;

    if (sOn < 0) {
        sOn = Env("BWPORT_NOENCOUNTER");
        if (sOn) {
            fprintf(stderr, "bwport: NOENCOUNTER on: no wild encounters (%s)\n", game);
        }
    }
    return sOn;
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

// game key bits (PAD_BUTTON_*) to the key register: KEYINPUT bits 0-9 (a b select start right left up down r l), active low;
// X and Y in HW_BUTTON_XY_BUF (bits 10, 11, active low; the game ORs the two words)
static void SetKeys(unsigned bits) {
    vu16 *xy = (vu16 *)HW_BUTTON_XY_BUF;

    s_reg_PAD_KEYINPUT = (u16)((s_reg_PAD_KEYINPUT & ~0x3ff) | (~bits & 0x3ff));
    *xy = (u16)((*xy & ~0xc00) | (~bits & 0xc00));
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

static void End(const PortRouteState *st, const char *why) {
    if (sInFrame && Env("BWPORT_MAPDUMP")) {
        PortRouteMap_Dump(st->game, strncmp(why, "stuck", 5) == 0 ? "stuck" : strncmp(why, "timeout", 7) == 0 ? "timeout" : "end",
                          (unsigned)OS_GetVBlankCount());
    }
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

// the no-progress check: called once a pilot step while it presses through an event (any token but M and K)
static void HangCheck(const PortRouteState *st, const char *tok) {
    static const char *sTok;
    static int sN, sZone, sGx, sGz, sEvent;
    int sLimit = HangLimit();

    if (!st->event || tok != sTok || (int)st->zone != sZone || st->gx != sGx || st->gz != sGz || st->event != sEvent) {
        sTok = tok;
        sZone = (int)st->zone;
        sGx = st->gx;
        sGz = st->gz;
        sEvent = st->event;
        sN = 0;
        return;
    }
    if (++sN < (st->event == 2 ? 4 * sLimit : sLimit)) {
        return;
    }
    fprintf(stderr, "pilot: hang: %.*s, event %s, zone %u %d,%d, no progress in %d frames at V-blank %u\n",
            (int)(NextTok(tok) - tok), tok, st->event == 2 ? "battle" : "on", st->zone, st->gx, st->gz, sN,
            (unsigned)OS_GetVBlankCount());
    if (sInFrame && st->zone != 0xffff) {
        PortRouteMap_Dump(st->game, "hang", (unsigned)OS_GetVBlankCount());
    }
    SetKeys(0);
    End(st, "hang");
}

static int TokenTimeout(void) {
    static int sT = -1;

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
                gPortRouteFieldYesNo == 1 ? "Yes" : "No", (unsigned)OS_GetVBlankCount(), yes ? "Yes" : "No");
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

// the event waits Wf Wt Wb Wz (age: the frames this token has run, 1 on its first): 1 when it is done
static int WaitStep(const PortRouteState *st, const char *tok, int age) {
    static int sN, sBattle;
    int n;

    if (age == 1) {
        sN = sBattle = 0;
    }
    switch (tok[1]) {
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

// Black 2 / White 2's S: the field menu's SAVE, each step on the game's state (the field menu's input and cursor, the
// save event's state); 1 when it is done
static int SaveStep(const PortRouteState *st, int age) {
    static int sPhase, sWait, sTries, sSaved;
    unsigned now = OS_GetVBlankCount();
    int input = gPortRouteMenu.vblank != 0 && gPortRouteMenu.vblank + 3 >= now && gPortRouteMenu.input;

    if (age == 1) {
        sPhase = sWait = sTries = sSaved = 0;
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
    case 2:   // down until the cursor is on SAVE (a press each 6 frames: 2 down, 4 up), then A
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
        SetKeys(sWait % 6 == 1 || sWait % 6 == 2 ? 0x80u : 0);
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

// a target token that cannot be done: the pilot line, a map dump (always: BWPORT_MAPDUMP's End makes one too), the end
static void GoFail(const PortRouteState *st, const char *tok, const char *why) {
    fprintf(stderr, "pilot: no path: %s (G token %.*s)\n", why, (int)(NextTok(tok) - tok), tok);
    if (Env("BWPORT_MAPDUMP")) {
        SIM_ShotRequest("mapstuck");
    } else if (sInFrame) {
        PortRouteMap_Dump(st->game, "nopath", (unsigned)OS_GetVBlankCount());
    }
    End(st, "stuck: no path");
}

static const unsigned kDirKey[4] = {0x40u, 0x80u, 0x20u, 0x10u};   // up down left right

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
            PressEvent(st);
            return 0;
        }
        SetKeys(0);
        if (sSeen) {
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

// an event that starts now is the G target's own: the player is on the target tile (G:X,Z) or on the trigger's tiles
// (G:trig:N): its scene, a battle it starts
static int TargetEvent(const PortRouteState *st, const PortRouteGoal *g) {
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
    return st->gx >= t->x && st->gx < t->x + (t->w ? t->w : 1) && st->gz >= t->z && st->gz < t->z + (t->h ? t->h : 1);
}

static int GoStep(const PortRouteState *st, const char *tok, int age) {
    static int sWx, sWz, sHave, sNoMove, sReplans, sPress, sPressN, sLastGx, sLastGz, sTalk, sEvLog, sEvZone, sTargetEv;
    static PortRouteGoal sGoal;
    static const char *sFor;   // the token sGoal is for (its first frames can come before the field: no GoStep then)
    PortRoutePlan plan;
    int r;

    if (tok != sFor || age == 1) {
        char why[160];

        sFor = tok;

        if (!PortRoute_ParseGoal(tok, &sGoal, why, sizeof why)) {
            fprintf(stderr, "pilot: bad token: %s\n", why);
            End(st, "stuck: bad token");
        }
        sHave = sNoMove = sReplans = sPressN = sTalk = sEvLog = sTargetEv = 0;
        sPress = -1;
        sLastGx = st->gx;
        sLastGz = st->gz;
        PortRoutePlan_Reset();
    }
    if (sGoal.kind == GOAL_WARP ? (int)st->zone == sGoal.n
                                : sGoal.kind == GOAL_TILE && st->gx == sGoal.x && st->gz == sGoal.z) {
        fprintf(stderr, "pilot: %.*s done at %d,%d zone %u (V-blank %u)\n", (int)(NextTok(tok) - tok), tok, st->gx, st->gz,
                st->zone, (unsigned)OS_GetVBlankCount());
        SetKeys(0);
        return 1;
    }
    if (sTalk) {   // at the target: facing it, A, its event
        if (TalkStep(st, tok, sPress, sGoal.talk, 0)) {
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
            if (!sTargetEv && TargetEvent(st, &sGoal)) {
                sTargetEv = 1;
            }
            fprintf(stderr, "pilot: event during G at V-blank %u (zone %u, %d,%d)%s\n", (unsigned)OS_GetVBlankCount(),
                    st->zone, st->gx, st->gz, sTargetEv ? ": the target's own" : "");
        }
        PressEvent(st);
        sHave = 0;
        sPressN = 0;
        return 0;
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
    if (st->gx != sLastGx || st->gz != sLastGz) {
        sLastGx = st->gx;
        sLastGz = st->gz;
        sNoMove = 0;
    } else if (++sNoMove == 150) {   // no step for 150 frames: the tile ahead is in the way (an actor, a wrong guess)
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
    if (sPressN > 0) {   // into a warp
        SetKeys(sPress == 0 ? 0x40u : sPress == 1 ? 0x80u : sPress == 2 ? 0x20u : 0x10u);
        if (--sPressN == 0) {   // the zone did not change: try another way
            PortRoutePlan_Block(st->zone, st->gx, st->gz);
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
            if (sGoal.kind == GOAL_NPC || sGoal.kind == GOAL_ITEM || sGoal.kind == GOAL_BG) {
                fprintf(stderr, "pilot: G: at %d,%d, facing the target (dir %d)\n", st->gx, st->gz, plan.press);
                sPress = plan.press;
                sTalk = 1;
                TalkStep(st, tok, sPress, sGoal.talk, 1);
                SetKeys(0);
                return 0;
            }
            fprintf(stderr, "pilot: G: at %d,%d, into the warp (dir %d)\n", st->gx, st->gz, plan.press);
            sPress = plan.press;
            sPressN = 90;
            return 0;
        }
        sWx = plan.x;
        sWz = plan.z;
        sHave = 1;
        fprintf(stderr, "pilot: G: %d,%d -> waypoint %d,%d (%d steps to go)\n", st->gx, st->gz, sWx, sWz, plan.steps);
        WalkTo(st, sWx, sWz, 1);
    }
    r = WalkTo(st, sWx, sWz, 0);
    if (r != 0) {   // at the waypoint (1) or the walk's own stuck (2): plan again
        sHave = 0;
        SetKeys(0);
    }
    return 0;
}

static void Pilot(const PortRouteState *st) {
    static const char *sTok, *sAgeTok;
    static int sInit, sHold, sStuck, sIdle, sLastX, sLastZ, sTotalStuck, sZone = -1, sAge;
    static unsigned sHoldKeys;
    static int sMash;
    static const char *sRet, *sZoneTok;
    // Black, White: X, then down to SAVE (POKEDEX, POKEMON, BAG, ... with the Pokedex handed over: 3 downs)
    static const char sSave[] = "Kx:2 K0:40 KD:2 K0:20 KD:2 K0:20 KD:2 K0:20 Ka:2 K0:200 Ka:2 K0:700 Ka:2 K0:60 ";
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
    if (*sTok == '\0') {
        if (sRet != NULL) {
            sTok = sRet;
            sRet = NULL;
            return;
        }
        SetKeys(0);
        End(st, "script done");
    }
    if (sMash > 0) {   // an M token: the pulse key every 4th step, whatever runs (a title, a new game's intro)
        SetKeys((++sCnt & 3) == 0 ? sPulse : 0);
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
    HangCheck(st, sTok);
    if (*sTok == 'W' && (sTok[1] == 'f' || sTok[1] == 't' || sTok[1] == 'b' || sTok[1] == 'z' || sTok[1] == 's')) {
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
    if (*sTok == 'S' && st->game[1] == '2') {
        if (SaveStep(st, sAge)) {
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
            PortRouteMap_Dump(st->game, "md", (unsigned)OS_GetVBlankCount());
            sTok = NextTok(sTok);
            return;
        }
        sMash = atoi(sTok + 1);
        if (sMash <= 0) {
            sTok = NextTok(sTok);
        }
        return;
    case 'P':
        sPulse = sTok[1] == 'a' ? 1 : 2;
        sTok = NextTok(sTok);
        return;
    case 'S':
        sRet = sTok + 1;
        sTok = sSave;
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
        SetKeys(d > 3 * v ? k : 0);
    } else if (d > 16) {
        SetKeys(k);
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

        st.event = 1;
        Pilot(&st);
    }
    sCalled = 0;
}
