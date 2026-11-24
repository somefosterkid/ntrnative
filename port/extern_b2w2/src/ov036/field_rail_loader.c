// The PC build compiles pokebw2's src/ov036/field_rail_loader.c through this file: the billboard update keeps a 1-frame
// step's legs moving (Run 4x, and the walk 4x's WALK_1F: port/extern_b2w2/src/ov036/field_actor_acmd.c). Black's twin,
// port/extern/src/ov021/field_rail_loader.c, says why.
#include "types.h"
#include "field/field_actor.h"
#include "gfl/billboard_actor.h"
#include "nitro/os.h"

extern FieldActor *gPortStep1FActor;
extern u16 gPortStep1FAnim;
extern u32 gPortStep1FVBlank;

static BOOL PortRail_Step1FNow(FieldActor *actor) {
    return actor == gPortStep1FActor && OS_GetVBlankCount() == gPortStep1FVBlank;
}

static u16 PortRail_DrawStatus(FieldActor *actor) {
    u16 state = FldAct_GetDrawStatus(actor);

    if (state == 0 && PortRail_Step1FNow(actor)) {
        return gPortStep1FAnim;
    }
    return state;
}

static BOOL PortRail_AnimePaused(FieldActor *actor) {
    if (PortRail_Step1FNow(actor)) {
        return FALSE;
    }
    return FieldAcmdTCB_CheckAnimePaused(actor);
}

// RailActor_UpdateVisibilityAndAnimation's last word on the animation: in a 1-frame step's frame the next tick shows
// the next picture (setting the current step clears the picture's wait)
static void PortRail_SetAnimating(FieldActor *actor, BillboardActorSys *sys, u32 index, BOOL animate) {
    BillboardActorSys_SetActorAnimating(sys, index, animate);
    if (animate && PortRail_Step1FNow(actor)) {
        BillboardActorSys_SetActorStep(sys, index, BillboardActorSys_GetActorStep(sys, index));
    }
}

#define FldAct_GetDrawStatus PortRail_DrawStatus
#define FieldAcmdTCB_CheckAnimePaused PortRail_AnimePaused
#define BillboardActorSys_SetActorAnimating(sys, index, animate) PortRail_SetAnimating(actor, sys, index, animate)
#include "../../../../third_party/pokebw2/src/ov036/field_rail_loader.c"
