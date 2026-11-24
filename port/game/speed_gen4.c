// The speed options' Gen 4 side: Platinum, HeartGold and SoulSilver (docs/speed-options.md "Platinum and HeartGold/SoulSilver").
// Each game's patch (port/subprojects/pokeplatinum-speed.patch, pokeheartgold-speed.patch) calls these at its hook points; they
// read the same settings Black's and Black 2's stand-ins read (SIM_UISettingGet: sim_config.ini, BWPORT_*, the Settings rows),
// so one setting does the same thing in every game. Every function answers "the game's own" (0) at the default setting.
// Compiled into each Gen 4 module (port/meson.build's main_files), so each game has its own copy.

extern int SIM_UISettingGet(const char *key);

// ---- Fades and the transition window (spd2, 2026-11-19; the Transitions row, `fadespeed` 0 = the game's, 1 = 2x, 2 = 4x,
// 3 = Instant, as Black's port/extern/lib/gfl/src/fade.c and port/extern/src/system/game_event.c) ----
#include <stdio.h>
#include <stdlib.h>

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

// Movement log: BWPORT_MOVELOG4=1 prints one line a tile the player's grid position changes (`move4:` the V-blank, the tile, the
// V-blanks since the last change, the follower's tile and follow state, -1 = none, and `ends`: how many field frames so far saw the
// player's step end, PLAYER_MOVE_STATE_END, the frames wild encounters and step counters run on) and one `move4f:` line a tile the
// follower's changes; =2 also one line per V-blank the pixel position moves (the step's px). The route pilot's frame hook
// (port/game/route_gen4.c, after the field's frame) calls it once a field frame; tools/speed4_measure.sh reads it.
void PortSpeed4_MoveLog(int gx, int gz, int px, int pz, int fgx, int fgz, int fstate, int moveState) {
    static int mode = -1, lgx = -1, lgz = -1, lpx, lpz, lfgx = -1, lfgz = -1, ends;
    static unsigned lvb;
    unsigned vb = OS_GetVBlankCount();

    if (mode < 0) {
        const char *e = getenv("BWPORT_MOVELOG4");

        mode = e != NULL ? atoi(e) : 0;
    }
    if (mode == 0) {
        return;
    }
    if (mode >= 2 && (px != lpx || pz != lpz)) {
        fprintf(stderr, "move4px: vb=%u px=%d,%d\n", vb, px, pz);
    }
    lpx = px;
    lpz = pz;
    ends += moveState == 3;
    if (gx != lgx || gz != lgz) {
        fprintf(stderr, "move4: vb=%u tile=%d,%d dt=%u follower=%d,%d state=%d ends=%d\n", vb, gx, gz, vb - lvb, fgx, fgz, fstate, ends);
        lgx = gx;
        lgz = gz;
        lvb = vb;
    }
    if (fgx != lfgx || fgz != lfgz) {
        fprintf(stderr, "move4f: vb=%u follower=%d,%d player=%d,%d state=%d\n", vb, fgx, fgz, gx, gz, fstate);
        lfgx = fgx;
        lfgz = fgz;
    }
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

// HG/SS's following Pokemon: its follow code (sub_02065A4C, followMon.unk1C 0 idle / 1 asked / 2 asked while stepping / 3 stepping)
// takes one request per player step and holds ONE pending; the game's code waits a frame in state 1 and another after each of its
// steps, so a player step of 2 frames dropped requests and the Pokemon stayed tiles behind for good (hg cherry at Walk 4x: 3 tiles).
// The speed hunks used to hold Walk to 2x and Run to the game's while it followed (Walk 4x was 2x, Run 2x and 4x the game's).
// Now the follow code itself keeps up while a faster Walk or Run is set (PortSpeed4_FollowFast, pokeheartgold-speed-spd4.patch:
// no wait frames, and a step that ended away from the player's last tile steps again). The step pick still reports the follower
// each step (PortSpeed4_SetFollower); Platinum has none.
static int sFollowerActive;

void PortSpeed4_SetFollower(int active) {
    sFollowerActive = active != 0;
}

int PortSpeed4_WalkSteps(void) {
    return PortSpeed4_Steps("walkspeed", 2);
}

int PortSpeed4_FollowFast(void) {
    return sFollowerActive && (SIM_UISettingGet("walkspeed") >= 2 || SIM_UISettingGet("runspeed") >= 2);
}

int PortSpeed4_SurfSteps(void) {
    return PortSpeed4_Steps("surfspeed", 1);   // 4x = 2x: a 1-frame step (WALK_FASTEST) ends no step the game's logic can see (Surf legs pt ver58, ver62)
}

// Run: the same count for the RunSpeed setting; the run action itself (4 px a frame for 4 frames, the run animation) is remade by
// the game's own Step0 for the player's object only (Platinum unk_020655F4.c, HG/SS unk_02062108_020626AC.c): 2x 8 px for 2
// frames, 4x 16 px for 1 (the 1-frame step reports its end and keeps its animation: PortSpeed4_NoteStep1F below).
int PortSpeed4_RunSteps(void) {
    return PortSpeed4_Steps("runspeed", 2);
}

// What a following object (Platinum's unk_02069BE0.c, HG/SS's follower) adds to the walk action 0x10 (WALK_FAST: what it copies the
// player's run as) to keep the player's pace: 0x14 (WALK_FASTER, +4) at Run 2x, 0x54 (the 1-frame walk, +0x44) at Run 4x.
int PortSpeed4_FollowRunAdd(void) {
    switch (PortSpeed4_RunSteps()) {
    case 1:
        return 4;
    case 2:
        return 0x44;
    }
    return 0;
}

// The player's 1-frame step (Run 4x). It starts and ends inside one map-object update, so (1) the player's move-state observer
// (Platinum player_move.c PlayerAvatar_UpdatePlayerMoveState, HG/SS PlayerAvatar_UpdateMovement) only sees it finished and never
// reports its end, which wild encounters, step counters and the follower wait for, and (2) its animation is set and reset to 0 in
// the same update, so the sprite would be drawn standing. The run's Step0 notes it here; the observer takes the flag
// (PortSpeed4_TakeStep1F: report END), and the map object's animation getter (Platinum map_object.c sub_02062A14, HG/SS
// sub_0205F330) returns the step's animation while the V-blank is that of the step or the one after (PortSpeed4_Step1FAnim; the
// field runs at 30 fps, so a step every field frame keeps it). Black's pattern: port/extern/src/ov021/field_player_core.c and
// field_rail_loader.c.
static int sStep1F;
static const void *sStep1FObj;
static u32 sStep1FAnim, sStep1FVBlank;

void PortSpeed4_NoteStep1F(void *mapObj, u32 anim) {
    sStep1F = 1;
    sStep1FObj = mapObj;
    sStep1FAnim = anim;
    sStep1FVBlank = OS_GetVBlankCount();
}

int PortSpeed4_TakeStep1F(void) {
    int step = sStep1F;

    sStep1F = 0;
    return step;
}

u32 PortSpeed4_Step1FAnim(const void *mapObj, u32 anim) {
    return anim == 0 && mapObj == sStep1FObj && OS_GetVBlankCount() - sStep1FVBlank <= 1 ? sStep1FAnim : anim;
}
