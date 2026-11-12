// Black 2's and White 2's walk, run and Surf speed options (port/extern/src/ov021/field_player_grid.c's twin;
// port/src/move_speed.c): pokebw2's src/ov036/field_player_grid.c compiled through this file. Only the steps the player
// controller picks for the held direction change: a walk (WALK_8F, uphill WALK_UPHILL_8F) by the walk speed, a run (DASH or
// DASH_UPHILL) by the run speed, a Surf step (WALK_4F) by the Surf speed, each to the game's or the port's faster command
// (port/extern_b2w2/src/ov036/field_actor_acmd.c). The turn in place before a walk from standing (STAY_WALK_4F, 4 frames)
// by the walk speed, the Surf turn by the Surf speed. Brakes, jumps, forced and constant moves, the bike and every scripted
// move keep the game's commands. Speed 1 passes every command through unchanged. The DS build never sees this file.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "field/field_actor.h"   // its FldAct_SetAcmd, before the rename

void PortPlayer_SetAcmd(FieldActor *actor, u16 acmd, const char *caller);
#define FldAct_SetAcmd(actor, acmd) PortPlayer_SetAcmd(actor, acmd, __func__)

#include "../../../../third_party/pokebw2/src/ov036/field_player_grid.c"

#undef FldAct_SetAcmd

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
