// The PC build compiles src/ov021/field_rail_loader.c through this file: the billboard update keeps a 1-frame step's
// legs moving (Run 4x, and the walk 4x's WALK_1F: port/extern/src/ov021/fldmmdl_acmd.c).
// A 1-frame step sets the actor's animation state (10: the run) and sets it back to 0 in the same actor update, so the
// billboard update, which runs after it, saw a standing actor every frame: the player slid in one pose (Run 4x). In the
// frame of such a step the update reads the step's animation state and "moving", as the game's own steps of 2 frames or
// more show it, and the billboard animation moves on one picture each step: at 1x a run picture lasts one tile
// (4 frames), so the legs keep their pace per tile. Steps of 2 frames and more are untouched (1x and 2x as before).
#include "types.h"
#include "field/field_actor.h"
#include "gfl/blact.h"
#include "nitro/os.h"

u16 func_ov010_0216d678(FieldActor *actor);
u32 func_ov021_02197c9c(FieldActor *actor);
void func_020507f0(BlActSys *sys, u32 actor, BOOL animate);
void func_0205085c(BlActSys *sys, u32 actor, u16 step);
u16 func_02050880(BlActSys *sys, u32 actor);

extern FieldActor *gPortStep1FActor;
extern u16 gPortStep1FAnim;
extern u32 gPortStep1FVBlank;

static BOOL PortRail_Step1FNow(FieldActor *actor) {
    return actor == gPortStep1FActor && OS_GetVBlankCount() == gPortStep1FVBlank;
}

static u16 PortRail_AnimState(FieldActor *actor) {
    u16 state = func_ov010_0216d678(actor);

    if (state == 0 && PortRail_Step1FNow(actor)) {
        return gPortStep1FAnim;
    }
    return state;
}

static u32 PortRail_AnimHalted(FieldActor *actor) {
    if (PortRail_Step1FNow(actor)) {
        return 0;
    }
    return func_ov021_02197c9c(actor);
}

// func_ov021_021a000c's last word on the animation: in a 1-frame step's frame the next tick shows the next picture
// (func_0205085c at the current step clears the picture's wait)
static void PortRail_SetAnimate(FieldActor *actor, BlActSys *sys, u32 id, BOOL animate) {
    func_020507f0(sys, id, animate);
    if (animate && PortRail_Step1FNow(actor)) {
        func_0205085c(sys, id, func_02050880(sys, id));
    }
}

#define func_ov010_0216d678 PortRail_AnimState
#define func_ov021_02197c9c PortRail_AnimHalted
#define func_020507f0(sys, id, animate) PortRail_SetAnimate(param_1, sys, id, animate)
#include "../../../../src/ov021/field_rail_loader.c"
