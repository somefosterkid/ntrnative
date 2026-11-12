// Black's code at ov021 0x021c8984-0x021c8b9c: pokebw2's field_slope_slip.c (the slopes the player slides down).

#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "field/field_map.h"
#include "field/field_slope_slip.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "system/game_event.h"

// A slope tile: its class, the action command that moves the player, the direction the player ends up facing and
// how many frames pass between the player's turns
typedef struct SlopeSlipTileConfig {
    u16 tileClass;
    u16 acmd;
    u16 dir;
    u16 turnInterval;
} SlopeSlipTileConfig;

// The event's work (pokebw2's layout and names)
typedef struct SlopeSlipWork {
    GameSystem *gsys;
    Field *field;
    FieldPlayer *player;
    FieldActor *actor;
    const SlopeSlipTileConfig *config;
    u16 dir;
    u32 frames;
} SlopeSlipWork;

static const SlopeSlipTileConfig data_ov021_021dab24[4] = {
    { 119, 14, 2, 2 },
    { 118, 15, 3, 2 },
    { 120, 12, 0, 2 },
    { 121, 13, 1, 2 },
};

GameEventReturnCode func_ov021_021c89bc(GameEvent *event, u32 *state, void *data);
BOOL IsTileSlopeSlip_(u32 tileClass);
void func_ov021_021c8ad0(SlopeSlipWork *work);
u16 func_ov021_021c8aac(SlopeSlipWork *work);
void func_ov021_021c8b48(SlopeSlipWork *work);
u32 func_ov021_021c8b5c(SlopeSlipWork *work);
void func_ov021_021c8b14(SlopeSlipWork *work);
void func_ov021_021c8b78(SlopeSlipWork *work);

const SlopeSlipTileConfig *GetSlopeSlipTileConfig(u32 tileClass) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (tileClass == data_ov021_021dab24[i].tileClass) {
            return &data_ov021_021dab24[i];
        }
    }
    return NULL;
}

BOOL IsTileSlopeSlip_(u32 tileClass) {
    if (GetSlopeSlipTileConfig(tileClass) != NULL) {
        return TRUE;
    }
    return FALSE;
}

GameEventReturnCode func_ov021_021c89bc(GameEvent *event, u32 *state, void *data) {
    SlopeSlipWork *work = data;

    switch (*state) {
    case 0:
        if (IsTileSlopeSlip_(func_ov021_021c8aac(work)) == 0) {
            *state = 2;
        } else {
            *state = 1;
        }
        break;
    case 1:
        func_ov021_021c8ad0(work);
        if (func_ov010_0216d454(work->actor) != 0) {
            func_ov021_021c8b48(work);
            if (func_ov021_021c8b5c(work) != 0) {
                GFL_SndPlayerStop(GFL_SndSeqGetPlayerIndex(0x694));
                *state = 2;
            } else {
                func_ov021_021c8b14(work);
            }
        }
        break;
    case 2:
        func_ov021_021c8b78(work);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventSlopeSlip_Create(GameEvent *parent, GameSystem *gsys, Field *field) {
    GameEvent *event;
    SlopeSlipWork *work;
    FieldPlayer *player;
    FieldActor *actor;

    event = GameEvent_Create(gsys, parent, func_ov021_021c89bc, sizeof(SlopeSlipWork));
    work = GameEvent_GetData(event);
    work->gsys = gsys;
    work->field = field;
    player = Field_GetPlayer(field);
    work->player = player;
    actor = FieldPlayer_GetActor(player);
    work->actor = actor;
    work->config = NULL;
    work->dir = GetActorFaceDir(actor);
    work->frames = 0;
    return event;
}

BOOL IsTileSlopeSlip(u32 tileClass) {
    return IsTileSlopeSlip_(tileClass);
}

u16 func_ov021_021c8a7c(SlopeSlipWork *work) {
    if (work->config != NULL) {
        return work->config->acmd;
    }
    return 0;
}

u16 func_ov021_021c8a8c(SlopeSlipWork *work) {
    if (work->config != NULL) {
        return work->config->dir;
    }
    return 0;
}

u16 func_ov021_021c8a9c(SlopeSlipWork *work) {
    if (work->config != NULL) {
        return work->config->turnInterval;
    }
    return 0;
}

u16 func_ov021_021c8aac(SlopeSlipWork *work) {
    FieldG3DMapper *mapper;
    u32 tileType;
    VecFx32 pos;

    FieldPlayer_GetWPos(work->player, &pos);
    mapper = Field_GetG3DMapper(work->field);
    tileType = GetTileTypeAtPos(mapper, &pos);
    return GetTileClass(tileType);
}

void func_ov021_021c8ad0(SlopeSlipWork *work) {
    u16 interval;

    interval = func_ov021_021c8a9c(work);
    if (++work->frames % interval == 0) {
        work->dir = (work->dir + 1) % 4;
        ClearActorFlag(work->actor, 8);
        CheckSetActorFaceDir(work->actor, work->dir);
        SetActorFlag(work->actor, 8);
    }
}

void func_ov021_021c8b14(SlopeSlipWork *work) {
    u16 acmd;

    acmd = func_ov021_021c8a7c(work);
    FldAct_SetAcmd(work->actor, acmd);
    SetActorFlag(work->actor, 8);
    if (GFL_SndPlayerIsActive(GFL_SndSeqGetPlayerIndex(0x694)) == 0) {
        GFL_SndSEPlay(0x694);
    }
}

void func_ov021_021c8b48(SlopeSlipWork *work) {
    const SlopeSlipTileConfig *config;

    config = GetSlopeSlipTileConfig(func_ov021_021c8aac(work));
    if (config != NULL) {
        work->config = config;
    }
}

u32 func_ov021_021c8b5c(SlopeSlipWork *work) {
    u16 dir;

    dir = func_ov021_021c8a8c(work);
    if (ActorRouteCollCheckOneTileInDir(work->actor, dir) != 0) {
        return 1;
    }
    return 0;
}

void func_ov021_021c8b78(SlopeSlipWork *work) {
    u16 dir;

    ClearActorFlag(work->actor, 8);
    if (work->config != NULL) {
        dir = func_ov021_021c8a8c(work);
        CheckSetActorFaceDir(work->actor, dir);
    }
}
