// Black 2's and White 2's walk, run and Surf speed options on the rails (port/extern/src/ov021/field_player_nogrid.c's twin;
// port/src/move_speed.c): pokebw2's src/ov036/field_player_nogrid.c compiled through this file, as
// port/extern_b2w2/src/ov036/field_player_grid.c does on the grid. Only the steps of FieldPlayerRail_StartWalkStep (on foot:
// walk 8F by the walk speed, the dash by the run speed) and FieldPlayerRail_StartRailStep (Surf: walk 4F by the Surf speed)
// change, to the game's rail WALK_4F / WALK_2F or the port's rail DASH_2F (PortAcmd_FasterRail). The rail's step takes a
// frames-to-speed index with nothing under 2 frames, so the rail run and the rail Surf stop at 2 frames a tile (their 2x) at
// 4x too. Turns, brakes, jumps, forced steps and the bike keep the game's commands. Speed 1 passes every command through
// unchanged. The DS build never sees this file.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "field/field_actor.h"   // its FldAct_SetAcmd, before the rename

void PortPlayerRail_SetAcmd(FieldActor *actor, u16 acmd, const char *caller);
#define FldAct_SetAcmd(actor, acmd) PortPlayerRail_SetAcmd(actor, acmd, __func__)

#include "../../../../third_party/pokebw2/src/ov036/field_player_nogrid.c"

#undef FldAct_SetAcmd

int PortMoveSpeed_Walk(void);
int PortMoveSpeed_Run(void);
int PortMoveSpeed_Surf(void);
u16 PortAcmd_FasterRail(u16 acmd, int k);

void PortPlayerRail_SetAcmd(FieldActor *actor, u16 acmd, const char *caller) {
    if (strcmp(caller, "FieldPlayerRail_StartWalkStep") == 0) {
        if ((acmd & ~3) == 0x0c) {
            acmd = PortAcmd_FasterRail(acmd, PortMoveSpeed_Walk());
        } else if ((acmd & ~3) == 0x58) {
            acmd = PortAcmd_FasterRail(acmd, PortMoveSpeed_Run());
        }
    } else if (strcmp(caller, "FieldPlayerRail_StartRailStep") == 0) {
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
