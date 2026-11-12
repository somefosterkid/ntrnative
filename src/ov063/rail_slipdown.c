// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov131/rail_slipdown.c, which is the same code in
// Black 2/White 2's overlay 131. The names are Black's dsd names and swan's; pokebw2's guesses are in the comments.
#include "constants/sound.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_camera.h"
#include "field/field_effect.h"
#include "field/field_map.h"
#include "field/field_nogrid_mapper.h"
#include "field/field_player.h"
#include "field/field_rail.h"
#include "field/rail_slipdown.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"

enum {
    SLIPDOWN_STATE_INIT = 0,
    SLIPDOWN_STATE_WAIT_CAMERA = 1,
    SLIPDOWN_STATE_START = 4,
    SLIPDOWN_STATE_SLIDE = 5,
    SLIPDOWN_STATE_END = 8,
    SLIPDOWN_STATE_DONE = 9,
};

// How far ahead of the actor the slide starts, the height it slides down to, and how far it slides per frame
#define SLIPDOWN_START_DIST FX32_CONST(16)
#define SLIPDOWN_END_Y (-FX32_CONST(10))
#define SLIPDOWN_SPEED FX32_CONST(8)

typedef struct {
    u32 state;
    NoGridMapper *mapper;
    RailUnit *railUnit;
    // NULL when the slide moves another actor than the player
    FieldPlayer *player;
    FieldActor *actor;
    FieldEffects *effects;
    FieldCamera *camera;
    u32 unused1C;
    VecFx32 start;
    VecFx32 end;
    VecFx16 dir;
    int frame;
    u32 frames;
    // The camera at the start of the slide, and how far it moves by the end
    VecFx32 eye;
    VecFx32 target;
    VecFx32 eyeDelta;
    VecFx32 targetDelta;
    // Where the slide lands on the rail
    RailPosition railPos;
    TCB *task;
} RailSlipdown;

static void func_ov063_021f377c(TCB *tcb, void *data);

// pokebw2's RailSlipdown_Create
void *func_ov063_021f36c0(GameSystem *gsys, Field *field, FieldActor *actor, BOOL stopPlayer) {
    RailSlipdown *work;

    work = GFL_HeapAllocate(Field_GetHeapID(field), sizeof(RailSlipdown), TRUE, "rail_slipdown.c", 128);
    work->mapper = Field_GetNoGridMapper(field);
    work->actor = actor;
    work->railUnit = FldAct_GetRailUnit(actor);
    work->effects = Field_GetFieldEffects(field);
    work->camera = Field_GetCameraSystem(field);
    if (stopPlayer) {
        work->player = Field_GetPlayer(field);
        FieldPlayer_ForceBrake(work->player);
    } else {
        work->player = NULL;
    }
    func_ov010_0216db4c(work->actor, TRUE);
    func_ov010_0216dc98(work->actor, TRUE);
    func_ov010_0216dae0(work->actor, FALSE);
    DisableActorMovement(work->actor);
    work->task = GFL_TCBMgrAddTask(Field_GetTCBMgr(field), func_ov063_021f377c, work, 0);
    return work;
}

// pokebw2's RailSlipdown_Delete
void func_ov063_021f3758(void *data) {
    RailSlipdown *work = data;

    GFL_TCBRemove(work->task);
    GFL_HeapFree(work);
}

// pokebw2's RailSlipdown_IsDone
BOOL func_ov063_021f376c(void *data) {
    RailSlipdown *work = data;

    if (work->state == SLIPDOWN_STATE_DONE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's RailSlipdown_Task
static void func_ov063_021f377c(TCB *tcb, void *data) {
    RailSlipdown *work = data;

    switch (work->state) {
    case SLIPDOWN_STATE_INIT: {
        VecFx16 dir;
        VecFx32 pos;
        VecFx32 diff;
        FieldRailSystem *rail;
        RailPosition railPos;
        fx32 height;
        fx32 len;
        fx32 dist;
        s32 maxSide;
        BOOL found;
        u16 count;
        int i;

        func_ov021_0219e764(work->actor, &dir);
        found = FALSE;
        dir.y = 0;
        VEC_Fx16Normalize(&dir, &dir);
        RailUnit_GetCalcPos(work->railUnit, &pos);
        work->start.x = pos.x + FX_Mul(dir.x, SLIPDOWN_START_DIST);
        work->start.y = pos.y + FX_Mul(dir.y, SLIPDOWN_START_DIST);
        work->start.z = pos.z + FX_Mul(dir.z, SLIPDOWN_START_DIST);
        height = work->start.y - SLIPDOWN_END_Y;
        work->end.y = SLIPDOWN_END_Y;
        // The slope's length for its height, about 1.155 (2 / sqrt(3)) times it
        len = FX_Mul(height, 0x127b);
        work->end.x = work->start.x + FX_Mul(dir.x, len);
        work->end.z = work->start.z + FX_Mul(dir.z, len);

        rail = FieldNoGridMapper_GetRailSystem(work->mapper);
        if (!CalculateRailCurves(rail, &work->start, &work->end, &work->railPos, &work->end)) {
            work->state = SLIPDOWN_STATE_DONE;
            return;
        }

        // From two steps to the side, step across the rail and land on the last free tile before a blocked one
        railPos = work->railPos;
        maxSide = func_ov021_021b7d50(rail, &work->railPos);
        if (!(GetTileFlags(FieldNoGridMapper_GetTileAtPos(work->mapper, &railPos)) & 1)) {
            found = TRUE;
        }
        railPos.posSide += 2;
        if (railPos.posSide > maxSide) {
            railPos.posSide = maxSide;
        }
        count = maxSide + railPos.posSide;
        for (i = 0; i < count; i++) {
            railPos.posSide--;
            if (GetTileFlags(FieldNoGridMapper_GetTileAtPos(work->mapper, &railPos)) & 1) {
                break;
            }
            work->railPos = railPos;
            found = TRUE;
        }
        if (!found) {
            work->state = SLIPDOWN_STATE_DONE;
            return;
        }

        func_ov021_021b7d38(rail, &work->railPos, &work->end);
        VEC_Subtract(&work->end, &work->start, &diff);
        dist = VEC_Mag(&diff);
        VEC_Normalize(&diff, &diff);
        VEC_Fx16Set(&work->dir, diff.x, diff.y, diff.z);
        dist = FX_Div(dist, SLIPDOWN_SPEED);
        if (FX_Modf(dist, &dist) == 0) {
            work->frames = FX_Whole(dist);
        } else {
            work->frames = FX_Whole(dist) + 1;
        }
        work->frame = 0;
        if (work->player != NULL) {
            FieldCamera_FinishDelay(work->camera);
            work->state = SLIPDOWN_STATE_WAIT_CAMERA;
        } else {
            work->state = SLIPDOWN_STATE_START;
        }
        break;
    }
    case SLIPDOWN_STATE_WAIT_CAMERA:
        if (FieldCamera_IsDelayActive(work->camera)) {
            break;
        }
        FieldCamera_ClearBind(work->camera);
        FieldCamera_ChangeTransformType(work->camera, 2);
        FieldCamera_CoordsGetEye(work->camera, &work->eye);
        FieldCamera_CoordsGetTarget(work->camera, &work->target);
        FieldNoGridMapper_SetPlayerPos(work->mapper, &work->railPos);
        if (FieldCamera_GetBind(work->camera) != NULL) {
            FieldCamera_CoordsSetTarget(work->camera, &work->end);
            FieldCamera_ClearBind(work->camera);
        }
        FieldCamera_ChangeTransformType(work->camera, 2);
        FieldCamera_CoordsGetEye(work->camera, &work->eyeDelta);
        FieldCamera_CoordsGetTarget(work->camera, &work->targetDelta);
        VEC_Subtract(&work->eyeDelta, &work->eye, &work->eyeDelta);
        VEC_Subtract(&work->targetDelta, &work->target, &work->targetDelta);
        FieldNoGridMapper_SetCameraAreaEnabled(work->mapper, FALSE);
        FieldCamera_SetTransformType(work->camera, 2);
        FieldCamera_ClearBind(work->camera);
        FieldCamera_CoordsSetTarget(work->camera, &work->target);
        FieldCamera_CoordsSetEye(work->camera, &work->eye);
        work->state = SLIPDOWN_STATE_START;
        break;
    case SLIPDOWN_STATE_START:
        GFL_SndSEPlay(SEQ_SE_FLD_92);
        work->state = SLIPDOWN_STATE_SLIDE;
        SetActorMovementFlag(work->actor, 0x8000);
        // fallthrough
    case SLIPDOWN_STATE_SLIDE: {
        VecFx32 pos;
        VecFx32 eye;
        VecFx32 target;
        fx32 dist;

        if (++work->frame >= work->frames) {
            VEC_Set(&pos, work->end.x, work->end.y, work->end.z);
            work->state = SLIPDOWN_STATE_END;
        } else {
            dist = FX_Mul(FX32_CONST(work->frame), SLIPDOWN_SPEED);
            VEC_Set(&pos, work->start.x + FX_Mul(work->dir.x, dist), work->start.y + FX_Mul(work->dir.y, dist),
                    work->start.z + FX_Mul(work->dir.z, dist));
            // Dust every other frame
            if (work->frame % 2 == 0) {
                func_ov021_021ac528(work->actor, work->effects);
            }
        }
        if (work->player != NULL) {
            FieldPlayer_SetWPos(work->player, &pos);
        } else {
            SetActorWPosValue(work->actor, &pos);
        }
        if (work->player == NULL) {
            break;
        }
        VEC_Set(&eye,
                work->eye.x + FX_Div(FX_Mul(work->eyeDelta.x, work->frame * FX32_ONE), work->frames * FX32_ONE),
                work->eye.y + FX_Div(FX_Mul(work->eyeDelta.y, work->frame * FX32_ONE), work->frames * FX32_ONE),
                work->eye.z + FX_Div(FX_Mul(work->eyeDelta.z, work->frame * FX32_ONE), work->frames * FX32_ONE));
        VEC_Set(&target,
                work->target.x
                    + FX_Div(FX_Mul(work->targetDelta.x, work->frame * FX32_ONE), work->frames * FX32_ONE),
                work->target.y
                    + FX_Div(FX_Mul(work->targetDelta.y, work->frame * FX32_ONE), work->frames * FX32_ONE),
                work->target.z
                    + FX_Div(FX_Mul(work->targetDelta.z, work->frame * FX32_ONE), work->frames * FX32_ONE));
        FieldCamera_CoordsSetTarget(work->camera, &target);
        FieldCamera_CoordsSetEye(work->camera, &eye);
        break;
    }
    case SLIPDOWN_STATE_END: {
        VecFx32 pos;

        GFL_SndPlayerStop(GFL_SndSeqGetPlayerIndex(SEQ_SE_FLD_92));
        if (work->player != NULL) {
            FieldPlayer_SetRailPos(work->player, &work->railPos);
            FieldPlayer_GetRailWorldPos(work->player, &pos);
            FieldPlayer_SetWPos(work->player, &pos);
            FieldNoGridMapper_SetCameraAreaEnabled(work->mapper, TRUE);
            FieldNoGridMapper_ForceUpdateCamera(work->mapper);
        } else {
            SetActorInitedPositionRail(work->actor, &work->railPos);
        }
        ClearActorMovementFlag(work->actor, 0x8000);
        func_ov010_0216db4c(work->actor, FALSE);
        func_ov010_0216dc98(work->actor, FALSE);
        func_ov010_0216dae0(work->actor, TRUE);
        EnableActorMovement(work->actor);
        work->state = SLIPDOWN_STATE_DONE;
        break;
    }
    case SLIPDOWN_STATE_DONE:
        break;
    }
}
