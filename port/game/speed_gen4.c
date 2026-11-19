// The speed options' Gen 4 side: Platinum, HeartGold and SoulSilver (docs/speed-options.md "Platinum and HeartGold/SoulSilver").
// Each game's patch (port/subprojects/pokeplatinum-speed.patch, pokeheartgold-speed.patch) calls these at its hook points; they
// read the same settings Black's and Black 2's stand-ins read (SIM_UISettingGet: sim_config.ini, BWPORT_*, the Settings rows),
// so one setting does the same thing in every game. Every function answers "the game's own" (0) at the default setting.
// Compiled into each Gen 4 module (port/meson.build's main_files), so each game has its own copy.

extern int SIM_UISettingGet(const char *key);

// ---- Fades and the transition window (spd2, 2026-11-19; the Transitions row, `fadespeed` 0 = the game's, 1 = 2x, 2 = 4x,
// 3 = Instant, as Black's port/extern/lib/gfl/src/fade.c and port/extern/src/system/game_event.c) ----
#include <stdio.h>

#include <nitro.h>   // OS_GetVBlankCount (its u32 is the DS's type on every host)

extern void SIM_SetTransition(int speed);

// A screen fade's time: Platinum's StartScreenFade and HG/SS's BeginNormalPaletteFade (screen_fade.c, unk_0200FA24_0200FA24.c)
// make a fade of `steps` steps of `framesPerStep` frames each; both are asserted non-zero. Each halving takes the frames of a
// step first, then the steps themselves (a rounded-up half, so a fade never goes below one step of one frame): 2x halves once,
// 4x twice (6 steps of 1 frame: 3, then 2), Instant is one step of one frame. The default leaves both alone. The fade
// functions take both as plain inputs (a step's size is range / steps), so any count from 1 up is a valid fade.
void PortSpeed4_Fade(int *steps, int *framesPerStep) {
    int fade = SIM_UISettingGet("fadespeed");
    int halvings = fade == 1 ? 1 : fade == 2 ? 2 : 0;

    if (fade == 3) {
        *steps = 1;
        *framesPerStep = 1;
        return;
    }
    for (; halvings > 0; halvings--) {
        if (*framesPerStep > 1) {
            *framesPerStep = (*framesPerStep + 1) / 2;
        } else if (*steps > 1) {
            *steps = (*steps + 1) / 2;
        }
    }
}

// The transition window: `on` is "a map change (door, warp, fly, Dig, the Waterfall climb, the Pokemon Center heal) is in the
// field task chain this frame" (each game's field frame asks its chain: the patch's PortSpeed4_TransitionFrame). It speeds the
// whole game up through libntr's SIM_SetTransition (a pacing window like Battle speed's: the game's frames are the same, they
// run sooner): Transitions 2x -> 2x, 4x and Instant -> 4x, the game's own -> off. Called once a field frame; the call with
// `off` is what closes the window, so the speed is only ever the one the chain asked for this frame.
void PortSpeed4_TransitionWindow(int on) {
    static int sOn;
    int fade = SIM_UISettingGet("fadespeed");
    int speed = !on || fade < 1 ? 0 : fade == 1 ? 2 : 4;

    if ((on != 0) != sOn) {
        fprintf(stderr, "bwport: transition %s (frame %u, speed %d)\n", on ? "start" : "end", (unsigned)OS_GetVBlankCount(),
                speed ? speed : 1);
        sOn = on != 0;
    }
    SIM_SetTransition(speed);
}

// Text speed: characters a text printer draws in one frame, 0 = the game's own speed (the player's Options speed, a frame
// delay between characters). TextSpeed 1 = Faster (4 a frame), 2 = Fastest (8), 3 = Instant (1024: the game's own cap for
// an instant printer, text.c's Text_AddPrinter, so a whole window a frame), as Black's port/extern/src/system/text_speed.c.
int PortSpeed4_TextChars(void) {
    static const int chars[4] = { 0, 4, 8, 1024 };
    int i = SIM_UISettingGet("textspeed");

    return i >= 1 && i <= 3 ? chars[i] : 0;
}

// ---- Walk, Surf and Run speed (spd4) ----
// How many Gen 4 step actions faster the player's step is than the game's own: 0 = the game's (the default), 1 = the next faster
// action (the setting's 2x), 2 = the one after (4x). The settings are Black's: WalkSpeed / SurfSpeed / RunSpeed hold 1, 2 or 4
// (sim_config.ini, BWPORT_WALKSPEED / SURFSPEED / RUNSPEED, cfgscript walkspeed= / surfspeed= / runspeed=). Walk: WALK_NORMAL (8
// frames a tile), WALK_FAST (4), WALK_FASTER (2); Surf (WALK_FAST, 4) and Run (4 frames) stop at 2 frames: a 1-frame step (WALK_FASTEST,
// or the run remade at 16 px) leaves the game's step-end logic nothing to see and stops the Platinum Surf legs ver58 and ver62, so
// their 4x is their 2x. The player's step pick asks once a step (Platinum player_move.c, HG/SS
// unk_0205CB48_0205D450_0205D4B4_0205D4B4.c) and swaps the action itself; only the player's own step is touched.
static int PortSpeed4_Steps(const char *key, int max) {
    int v = SIM_UISettingGet(key);
    int steps = v >= 4 ? 2 : v >= 2 ? 1 : 0;

    return steps > max ? max : steps;
}

// HG/SS's following Pokemon: its follow code takes one request per player step and holds ONE pending, so a player step shorter than
// the follower's own (it cannot finish a step in under 3 frames: the run's 4 is the shortest the game ever asks of it) drops
// requests and the Pokemon stays tiles behind for good (route hg cherry at Walk 4x: 3 tiles, never closing). While it follows the
// player's step pick calls PortSpeed4_SetFollower(1) each step: Walk is held to 2x (4 frames, the game's own run pace) and Run to
// the game's. Platinum has no follower: it never calls it.
static int sFollowerActive;

void PortSpeed4_SetFollower(int active) {
    sFollowerActive = active != 0;
}

int PortSpeed4_WalkSteps(void) {
    int steps = PortSpeed4_Steps("walkspeed", 2);

    return sFollowerActive && steps > 1 ? 1 : steps;
}

int PortSpeed4_SurfSteps(void) {
    return PortSpeed4_Steps("surfspeed", 1);   // 4x = 2x: a 1-frame step (WALK_FASTEST) ends no step the game's logic can see (Surf legs pt ver58, ver62)
}

// Run: the same count for the RunSpeed setting; the run action itself (4 px a frame for 4 frames, the run animation) is remade by
// the game's own Step0 for the player's object only (Platinum unk_020655F4.c, HG/SS unk_02062108_020626AC.c): 2x 8 px for 2
// frames, 4x 16 px for 1.
int PortSpeed4_RunSteps(void) {
    return sFollowerActive ? 0 : PortSpeed4_Steps("runspeed", 1);   // 4x = 2x, as Surf's
}

// What a following object (Platinum's unk_02069BE0.c, HG/SS's follower) adds to the walk action 0x10 (WALK_FAST: what it copies the
// player's run as) to keep the player's pace: 0x14 (WALK_FASTER, +4) at Run 2x and 4x.
int PortSpeed4_FollowRunAdd(void) {
    return PortSpeed4_RunSteps() != 0 ? 4 : 0;
}
