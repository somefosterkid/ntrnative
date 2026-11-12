// Black 2's and White 2's action command dispatch with the port's faster steps (port/extern/src/ov021/fldmmdl_acmd.c's twin):
// pokebw2's src/ov036/field_actor_acmd.c compiled through this file. The dispatch reads port tables, the game's rows and
// faster steps after them, which only the player's walk, run and Surf speed options use
// (port/extern_b2w2/src/ov036/field_player_grid.c and field_player_nogrid.c, port/src/move_speed.c).
// - Flat steps: the game's step (init FieldAcmdTCB_WalkInit, step FieldAcmdTCB_WalkMove, end FieldAcmdTCB_SetAcmdEnd) with
//   a higher speed and fewer frames, so a tile stays exactly 16 px: DASH_2F and DASH_1F (the run's animation), WALK_1F
//   (the walk 2F's animation). The faster walks are the game's own WALK_4F and WALK_2F (the bike's step).
// - Uphill steps: the game's uphill step (FieldAcmdTCB_WalkTableInit and _WalkTableMove: it moves by a table of distances, one
//   a frame, and takes the height from the map each frame) with shorter tables of the same 16 px: WALK_UPHILL_3F and _2F
//   (the B-held uphill walk's animation), DASH_UPHILL_3F and _2F (the uphill run's). The game's own WALK_UPHILL_4F (6
//   frames) is the 2x uphill walk.
// - On rails (an actor with ACTOR_FLAG_RAIL reads the rail table): RAIL_DASH_2F at the port's DASH_2F code, the rail's WALK_2F
//   row (field_actor_g3dobj.c's static RailActorAcmd_InitMove, reached through the row's init) with the run's animation.
// A 1-frame step starts and ends inside one actor update, so the player's action observer never sees it moving and the
// step's events (wild encounters, step triggers, Repel's count) would never run. The 1-frame inits raise a flag that
// port/extern_b2w2/src/ov036/field_player_core.c's observer turns into the step's end. The DS build never sees this file.
// Black 2 has 193 commands (0-0xc0): the port's codes start at 0xc4.
#include "field/field_actor_internal.h"   // its extern of the game's tables, before the rename

#define PORT_ACMD_DASH_2F 0xc4     // past the game's last command (0xc0; 0xfd-0xff are special), 4 directions each
#define PORT_ACMD_DASH_1F 0xc8
#define PORT_ACMD_WALK_1F 0xcc
#define PORT_ACMD_WALK_UPHILL_3F 0xd0
#define PORT_ACMD_WALK_UPHILL_2F 0xd4
#define PORT_ACMD_DASH_UPHILL_3F 0xd8
#define PORT_ACMD_DASH_UPHILL_2F 0xdc
#define PORT_ACMD_END 0xe0
#define GAME_ACMD_ROWS 193         // field_actor_movedata.c's ACTOR_ACTION_COMMAND_TABLES and _RAIL

static const ActorAcmdFunc *const *PortAcmdTables(void);
static const ActorAcmdFunc *const *PortAcmdTablesRail(void);
#define ACTOR_ACTION_COMMAND_TABLES (PortAcmdTables())
#define ACTOR_ACTION_COMMAND_TABLES_RAIL (PortAcmdTablesRail())

#include "../../../../third_party/pokebw2/src/ov036/field_actor_acmd.c"

#undef ACTOR_ACTION_COMMAND_TABLES
#undef ACTOR_ACTION_COMMAND_TABLES_RAIL

// A 1-frame step began this frame (and has ended with it); PortAcmd_TakeStep1F reads and clears it
static BOOL sPortStep1F;

#define PORT_STEP(name, dir, speed, frames, anim)                       \
    static BOOL name(FieldActor *actor) {                                \
        FieldAcmdTCB_WalkInit(actor, dir, speed, frames, anim);           \
        sPortStep1F = (frames == 1);                                    \
        return 1;                                                       \
    }                                                                   \
    static const ActorAcmdFunc name##_row[] = { name, FieldAcmdTCB_WalkMove, FieldAcmdTCB_SetAcmdEnd };

PORT_STEP(PortDash2F_Up, 0, 0x8000, 2, 10)
PORT_STEP(PortDash2F_Down, 1, 0x8000, 2, 10)
PORT_STEP(PortDash2F_Left, 2, 0x8000, 2, 10)
PORT_STEP(PortDash2F_Right, 3, 0x8000, 2, 10)
PORT_STEP(PortDash1F_Up, 0, 0x10000, 1, 10)
PORT_STEP(PortDash1F_Down, 1, 0x10000, 1, 10)
PORT_STEP(PortDash1F_Left, 2, 0x10000, 1, 10)
PORT_STEP(PortDash1F_Right, 3, 0x10000, 1, 10)
PORT_STEP(PortWalk1F_Up, 0, 0x10000, 1, 5)
PORT_STEP(PortWalk1F_Down, 1, 0x10000, 1, 5)
PORT_STEP(PortWalk1F_Left, 2, 0x10000, 1, 5)
PORT_STEP(PortWalk1F_Right, 3, 0x10000, 1, 5)

// The uphill tables: 16 px in 3 and in 2 frames (the 12-frame walk's table summed in fours, the 6-frame one's in pairs)
static const s32 sPortUphill3F[] = { 0x5000, 0x5000, 0x6000 };
static const s32 sPortUphill2F[] = { 0x8000, 0x8000 };

static BOOL PortUphill3F_Step(FieldActor *actor) {
    return FieldAcmdTCB_WalkTableMove(actor, (fx32 *)sPortUphill3F) == 1;
}

static BOOL PortUphill2F_Step(FieldActor *actor) {
    return FieldAcmdTCB_WalkTableMove(actor, (fx32 *)sPortUphill2F) == 1;
}

#define PORT_UPHILL(name, dir, frames, anim, step)                      \
    static BOOL name(FieldActor *actor) {                                \
        FieldAcmdTCB_WalkTableInit(actor, dir, frames, anim);                  \
        return 1;                                                       \
    }                                                                   \
    static const ActorAcmdFunc name##_row[] = { name, step, FieldAcmdTCB_SetAcmdEnd };

PORT_UPHILL(PortWalkUphill3F_Up, 0, 3, 6, PortUphill3F_Step)
PORT_UPHILL(PortWalkUphill3F_Down, 1, 3, 6, PortUphill3F_Step)
PORT_UPHILL(PortWalkUphill3F_Left, 2, 3, 6, PortUphill3F_Step)
PORT_UPHILL(PortWalkUphill3F_Right, 3, 3, 6, PortUphill3F_Step)
PORT_UPHILL(PortWalkUphill2F_Up, 0, 2, 6, PortUphill2F_Step)
PORT_UPHILL(PortWalkUphill2F_Down, 1, 2, 6, PortUphill2F_Step)
PORT_UPHILL(PortWalkUphill2F_Left, 2, 2, 6, PortUphill2F_Step)
PORT_UPHILL(PortWalkUphill2F_Right, 3, 2, 6, PortUphill2F_Step)
PORT_UPHILL(PortDashUphill3F_Up, 0, 3, 0xb, PortUphill3F_Step)
PORT_UPHILL(PortDashUphill3F_Down, 1, 3, 0xb, PortUphill3F_Step)
PORT_UPHILL(PortDashUphill3F_Left, 2, 3, 0xb, PortUphill3F_Step)
PORT_UPHILL(PortDashUphill3F_Right, 3, 3, 0xb, PortUphill3F_Step)
PORT_UPHILL(PortDashUphill2F_Up, 0, 2, 0xb, PortUphill2F_Step)
PORT_UPHILL(PortDashUphill2F_Down, 1, 2, 0xb, PortUphill2F_Step)
PORT_UPHILL(PortDashUphill2F_Left, 2, 2, 0xb, PortUphill2F_Step)
PORT_UPHILL(PortDashUphill2F_Right, 3, 2, 0xb, PortUphill2F_Step)

static const ActorAcmdFunc *const PORT_ACMD_ROWS[PORT_ACMD_END - PORT_ACMD_DASH_2F] = {
    PortDash2F_Up_row, PortDash2F_Down_row, PortDash2F_Left_row, PortDash2F_Right_row,
    PortDash1F_Up_row, PortDash1F_Down_row, PortDash1F_Left_row, PortDash1F_Right_row,
    PortWalk1F_Up_row, PortWalk1F_Down_row, PortWalk1F_Left_row, PortWalk1F_Right_row,
    PortWalkUphill3F_Up_row, PortWalkUphill3F_Down_row, PortWalkUphill3F_Left_row, PortWalkUphill3F_Right_row,
    PortWalkUphill2F_Up_row, PortWalkUphill2F_Down_row, PortWalkUphill2F_Left_row, PortWalkUphill2F_Right_row,
    PortDashUphill3F_Up_row, PortDashUphill3F_Down_row, PortDashUphill3F_Left_row, PortDashUphill3F_Right_row,
    PortDashUphill2F_Up_row, PortDashUphill2F_Down_row, PortDashUphill2F_Left_row, PortDashUphill2F_Right_row,
};

// The rail run at 2x and 4x: the rail's WALK_2F row (2 frames a tile; its init is field_actor_g3dobj.c's static
// RailActorAcmd_InitMove(actor, dir, 2, DRAW_STATUS_WALK_2F)) with the run's animation, which the init's draw status is
// changed to afterwards (the rail's frames-to-speed table has no 1-frame entry, so the rail run stops at 2x)
#define PORT_RAIL_STEP(name, dir)                                       \
    static BOOL name##_init(FieldActor *actor) {                        \
        ACTOR_ACTION_COMMAND_TABLES_RAIL[ACMD_WALK_2F + dir][0](actor); \
        FldAct_SetDrawStatus(actor, DRAW_STATUS_ANIME_10);              \
        return TRUE;                                                    \
    }                                                                   \
    static const ActorAcmdFunc name##_row[] = { name##_init, RailActor_AcWalk1, RailActor_AcWalkEnd };

PORT_RAIL_STEP(PortRailDash2F_Up, 0)
PORT_RAIL_STEP(PortRailDash2F_Down, 1)
PORT_RAIL_STEP(PortRailDash2F_Left, 2)
PORT_RAIL_STEP(PortRailDash2F_Right, 3)

static const ActorAcmdFunc *const PORT_RAIL_DASH_2F_ROWS[4] = {
    PortRailDash2F_Up_row, PortRailDash2F_Down_row, PortRailDash2F_Left_row, PortRailDash2F_Right_row,
};

// Filled at the first command (and again after the overlay's data is reset at a reload)
static const ActorAcmdFunc *sPortAcmdTables[PORT_ACMD_END];
static const ActorAcmdFunc *sPortAcmdTablesRail[PORT_ACMD_END];

static const ActorAcmdFunc *const *PortAcmdTables(void) {
    int i;

    if (sPortAcmdTables[0] == NULL) {
        for (i = 0; i < GAME_ACMD_ROWS; i++) {
            sPortAcmdTables[i] = ACTOR_ACTION_COMMAND_TABLES[i];
        }
        for (i = PORT_ACMD_DASH_2F; i < PORT_ACMD_END; i++) {
            sPortAcmdTables[i] = PORT_ACMD_ROWS[i - PORT_ACMD_DASH_2F];
        }
    }
    return sPortAcmdTables;
}

// The rail table: the game's rows and the rail run at the port's DASH_2F code. The other port codes, which the rail
// controller never gives, play as the rail's nearest step: the dashes as the rail run, WALK_1F as the rail's WALK_2F,
// the uphill walks as its WALK_4F.
static const ActorAcmdFunc *const *PortAcmdTablesRail(void) {
    int i;

    if (sPortAcmdTablesRail[0] == NULL) {
        for (i = 0; i < GAME_ACMD_ROWS; i++) {
            sPortAcmdTablesRail[i] = ACTOR_ACTION_COMMAND_TABLES_RAIL[i];
        }
        for (i = PORT_ACMD_DASH_2F; i < PORT_ACMD_END; i++) {
            if (i < PORT_ACMD_WALK_1F || i >= PORT_ACMD_DASH_UPHILL_3F) {
                sPortAcmdTablesRail[i] = PORT_RAIL_DASH_2F_ROWS[i & 3];
            } else if (i < PORT_ACMD_WALK_UPHILL_3F) {
                sPortAcmdTablesRail[i] = ACTOR_ACTION_COMMAND_TABLES_RAIL[0x14 + (i & 3)];
            } else {
                sPortAcmdTablesRail[i] = ACTOR_ACTION_COMMAND_TABLES_RAIL[0x10 + (i & 3)];
            }
        }
    }
    return sPortAcmdTablesRail;
}

// TRUE once after a 1-frame step of the actor began (and ended) since the last call
BOOL PortAcmd_TakeStep1F(FieldActor *actor) {
    BOOL step = sPortStep1F;
    u16 acmd = FldAct_GetAcmd(actor);

    sPortStep1F = FALSE;
    return step && acmd >= PORT_ACMD_DASH_1F && acmd < PORT_ACMD_WALK_UPHILL_3F;
}

// The command k (2 or 4) times faster than the game's acmd, any direction: a walk 8F, walk 4F or dash; an uphill walk
// (WALK_UPHILL_8F, 12 frames), B-held uphill walk (WALK_UPHILL_4F, 6 frames) or uphill dash (6 frames); or the turn
// (STAY_WALK_2F, 4 frames). acmd itself for any other command or k.
// - A 6-frame uphill step at 4x is 2 frames (1.5 rounded up).
// - The turn at 2x and 4x is the game's 2-frame STAY_WALK. The turn's length is also the game's tap-to-turn window (a
//   direction let go before the turn ends only turns the player; one still held walks), and a 1-frame turn would
//   make nearly every tap a step, so the 4x turn stays at 2 frames.
u16 PortAcmd_Faster(u16 acmd, int k) {
    u16 dir = acmd & 3;

    if (k != 2 && k != 4) {
        return acmd;
    }
    if (acmd >= ACMD_DASH_UPHILL && acmd <= ACMD_DASH_UPHILL + 3) {   // dash uphill
        return (k == 2 ? PORT_ACMD_DASH_UPHILL_3F : PORT_ACMD_DASH_UPHILL_2F) + (acmd - 0xa3);
    }
    if (acmd >= 0xa7 && acmd <= 0xaa) {   // walk uphill, 12 frames
        return (k == 2 ? 0x4c : PORT_ACMD_WALK_UPHILL_3F) + (acmd - 0xa7);   // 2x: the game's 6-frame uphill walk
    }
    switch (acmd & ~3) {
    case 0x0c:   // walk 8F
        return (k == 2 ? 0x10 : 0x14) + dir;   // the game's walk 4F, walk 2F
    case 0x10:   // walk 4F
        return (k == 2 ? 0x14 : PORT_ACMD_WALK_1F) + dir;
    case 0x58:   // dash
        return (k == 2 ? PORT_ACMD_DASH_2F : PORT_ACMD_DASH_1F) + dir;
    case 0x4c:   // walk uphill, 6 frames
        return (k == 2 ? PORT_ACMD_WALK_UPHILL_3F : PORT_ACMD_WALK_UPHILL_2F) + dir;
    case 0x24:   // the turn (STAY_WALK, 4 frames)
        return 0x28 + dir;   // the game's STAY_WALK of 2 frames
    }
    return acmd;
}

// The rail command k (2 or 4) times faster than the game's rail walk 8F, walk 4F or dash (the rail controller's
// steps, port/extern/src/ov021/field_player_nogrid.c): the walks are the game's rail WALK_4F and WALK_2F, the dash
// the port's rail DASH_2F at 2x and 4x alike, the walk 4F (Surf on a rail) the rail WALK_2F at 2x and 4x alike.
u16 PortAcmd_FasterRail(u16 acmd, int k) {
    u16 dir = acmd & 3;

    if (k != 2 && k != 4) {
        return acmd;
    }
    switch (acmd & ~3) {
    case 0x0c:   // walk 8F
        return (k == 2 ? 0x10 : 0x14) + dir;
    case 0x10:   // walk 4F
        return 0x14 + dir;
    case 0x58:   // dash
        return PORT_ACMD_DASH_2F + dir;
    }
    return acmd;
}

// The game command a port step stands for, for the game's checks of the player's command (is the player running:
// fldmmdl_movecode.c, fieldmap_event.c; their stand-ins call this for FldAct_GetAcmd)
u16 PortAcmd_GetGameAcmd(FieldActor *actor) {
    u16 acmd = FldAct_GetAcmd(actor);

    if (acmd >= PORT_ACMD_DASH_2F && acmd < PORT_ACMD_WALK_1F) {
        return 0x58 + (acmd & 3);   // ACMD_DASH
    }
    if (acmd >= PORT_ACMD_WALK_1F && acmd < PORT_ACMD_WALK_UPHILL_3F) {
        return 0x14 + (acmd & 3);   // ACMD_WALK_2F
    }
    if (acmd >= PORT_ACMD_WALK_UPHILL_3F && acmd < PORT_ACMD_DASH_UPHILL_3F) {
        return 0x4c + (acmd & 3);   // ACMD_WALK_UPHILL_4F
    }
    if (acmd >= PORT_ACMD_DASH_UPHILL_3F && acmd < PORT_ACMD_END) {
        return 0xa3 + (acmd & 3);   // ACMD_DASH_UPHILL: still running
    }
    return acmd;
}
