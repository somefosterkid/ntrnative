// The PC build compiles src/ov021/field_player_grid.c through this file: the player's walk, run and Surf speed
// options (port/src/move_speed.c). Only the steps the player controller picks for the held direction change: a walk
// (WALK_8F, uphill WALK_UPHILL_8F) by the walk speed, a run (DASH or DASH_UPHILL, or WALK_4F / WALK_UPHILL_4F with B
// held where running is not allowed) by the run speed, a Surf step (WALK_4F) by the Surf speed, each to the game's or
// the port's faster command (port/extern/src/ov021/fldmmdl_acmd.c). The turn in place before a walk from standing
// (STAY_WALK_2F, 4 frames: a turn from standing is always followed by a walk, as with B held and running allowed the
// player runs off without turning) by the walk speed, the Surf turn by the Surf speed. Downhill steps are the flat
// commands. Brakes, jumps, forced and constant moves (currents, ice), the bike and every scripted move keep the
// game's commands. Speed 1 passes every command through unchanged.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "field/field_actor.h"   // its FldAct_SetAcmd, before the rename

void PortPlayer_SetAcmd(FieldActor *actor, u16 acmd, const char *caller);
#pragma push_macro("FldAct_SetAcmd")
#define FldAct_SetAcmd(actor, acmd) PortPlayer_SetAcmd(actor, acmd, __func__)

#include "../../../../src/ov021/field_player_grid.c"

#pragma pop_macro("FldAct_SetAcmd")

int PortMoveSpeed_Walk(void);
int PortMoveSpeed_Run(void);
int PortMoveSpeed_Surf(void);
u16 PortAcmd_Faster(u16 acmd, int k);

void PortPlayer_SetAcmd(FieldActor *actor, u16 acmd, const char *caller) {
    if (strcmp(caller, "FieldPlayerGrid_UpdateNormalMove") == 0) {
        if ((acmd & ~3) == ACMD_WALK_8F || (acmd >= ACMD_WALK_UPHILL_8F && acmd < ACMD_WALK_UPHILL_8F + 4)) {
            acmd = PortAcmd_Faster(acmd, PortMoveSpeed_Walk());
        } else if ((acmd & ~3) == ACMD_DASH || (acmd & ~3) == ACMD_WALK_4F || (acmd & ~3) == ACMD_WALK_UPHILL_4F ||
                   (acmd >= ACMD_DASH_UPHILL && acmd < ACMD_DASH_UPHILL + 4)) {
            acmd = PortAcmd_Faster(acmd, PortMoveSpeed_Run());
        }
    } else if (strcmp(caller, "FieldPlayerGrid_UpdateNormalTurn") == 0) {
        acmd = PortAcmd_Faster(acmd, PortMoveSpeed_Walk());
    } else if (strcmp(caller, "FieldPlayerGrid_UpdateSurfMove") == 0) {
        if ((acmd & ~3) == ACMD_WALK_4F) {
            acmd = PortAcmd_Faster(acmd, PortMoveSpeed_Surf());
        }
    } else if (strcmp(caller, "FieldPlayerGrid_UpdateSurfTurn") == 0) {
        acmd = PortAcmd_Faster(acmd, PortMoveSpeed_Surf());
    }
    // BWPORT_MOVELOG=1: each command the controller gives, with the frame, the tile the player stands on and the
    // height in px (tests)
    if (getenv("BWPORT_MOVELOG") != NULL) {
        VecFx32 pos;

        CopyActorWPos(actor, &pos);
        fprintf(stderr, "bwport: move f%u %s 0x%02x gpos %d,%d y %d\n", (unsigned)OS_GetVBlankCount(), caller, acmd,
               GetGPosX(actor), GetGPosZ(actor), (int)(pos.y >> FX32_SHIFT));
    }
    FldAct_SetAcmd(actor, acmd);
}
