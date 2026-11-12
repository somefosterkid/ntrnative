// The PC build compiles src/ov021/field_player_nogrid.c through this file: the walk, run and Surf speed options
// (port/src/move_speed.c) on the rails (some bridges and routes), as port/extern/src/ov021/field_player_grid.c does on
// the grid. Only the steps of func_ov021_021a5edc (on foot: walk 8F by the walk speed, the dash by the run speed) and
// func_ov021_021a6428 (Surf: walk 4F by the Surf speed) change, to the game's rail WALK_4F / WALK_2F or the port's
// rail DASH_2F (port/extern/src/ov021/fldmmdl_acmd.c, PortAcmd_FasterRail). The rail's step takes a frames-to-speed
// index with nothing under 2 frames, so the rail run and the rail Surf stop at 2 frames a tile (their 2x) at 4x too.
// Turns (already the 2-frame STAY_WALK here), brakes, jumps, forced steps and the bike keep the game's commands.
// Speed 1 passes every command through unchanged.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "field/field_actor.h"   // its FldAct_SetAcmd, before the rename

void PortPlayerRail_SetAcmd(FieldActor *actor, u16 acmd, const char *caller);
#pragma push_macro("FldAct_SetAcmd")
#define FldAct_SetAcmd(actor, acmd) PortPlayerRail_SetAcmd(actor, acmd, __func__)

#include "../../../../src/ov021/field_player_nogrid.c"

#pragma pop_macro("FldAct_SetAcmd")

int PortMoveSpeed_Walk(void);
int PortMoveSpeed_Run(void);
int PortMoveSpeed_Surf(void);
u16 PortAcmd_FasterRail(u16 acmd, int k);

void PortPlayerRail_SetAcmd(FieldActor *actor, u16 acmd, const char *caller) {
    if (strcmp(caller, "func_ov021_021a5edc") == 0) {
        if ((acmd & ~3) == 0x0c) {
            acmd = PortAcmd_FasterRail(acmd, PortMoveSpeed_Walk());
        } else if ((acmd & ~3) == 0x58) {
            acmd = PortAcmd_FasterRail(acmd, PortMoveSpeed_Run());
        }
    } else if (strcmp(caller, "func_ov021_021a6428") == 0) {
        if ((acmd & ~3) == 0x10) {
            acmd = PortAcmd_FasterRail(acmd, PortMoveSpeed_Surf());
        }
    }
    // BWPORT_MOVELOG=1: each command the rail controller gives, with the frame (tests)
    if (getenv("BWPORT_MOVELOG") != NULL) {
        fprintf(stderr, "bwport: move f%u rail %s 0x%02x\n", (unsigned)OS_GetVBlankCount(), caller, acmd);
    }
    FldAct_SetAcmd(actor, acmd);
}
