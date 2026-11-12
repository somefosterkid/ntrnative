// Black's code at ov021 0x021c8b9c-0x021c8e68: pokebw2's field_water_current.c (the currents that carry the player).

#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "field/field_map.h"
#include "field/field_player.h"
#include "field/field_water_current.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "system/game_event.h"

// A current tile's class, the action command that carries the player along it, and one more (unused) halfword
typedef struct WaterCurrentTile {
    u16 tileClass;
    u16 acmd;
    u16 unused;
} WaterCurrentTile;

// A tile that bumps the player off the current: its class and action command
typedef struct WaterCurrentTileAcmd {
    u16 tileClass;
    u16 acmd;
} WaterCurrentTileAcmd;

// The event's work (pokebw2's layout and names)
typedef struct EventWaterCurrentMove {
    GameSystem *gsys;
    Field *field;
    FieldPlayer *player;
    FieldActor *actor;
    const WaterCurrentTile *tile;
    const WaterCurrentTileAcmd *flowTile;
    BOOL flowing;
    s32 flowPlayer;
    s32 endPlayer;
    s32 loopPlayer;
} EventWaterCurrentMove;

static const WaterCurrentTileAcmd data_ov021_021dab44[4] = {
    { 114, 120 },
    { 115, 119 },
    { 116, 117 },
    { 117, 118 },
};
static const WaterCurrentTile WATER_CURRENT_TILES[4] = {
    { 149, 82, 2 },
    { 148, 83, 3 },
    { 150, 80, 0 },
    { 151, 81, 1 },
};

GameEventReturnCode func_ov021_021c8bfc(GameEvent *event, u32 *state, void *data);
void func_ov021_021c8df4(EventWaterCurrentMove *work);
void func_ov021_021c8e14(EventWaterCurrentMove *work);
const WaterCurrentTile *GetWaterCurrentTileConfig(u32 tileClass);
u16 func_ov021_021c8d18(EventWaterCurrentMove *work);
void func_ov021_021c8e30(EventWaterCurrentMove *work);
void func_ov021_021c8d74(EventWaterCurrentMove *work);
void func_ov021_021c8dc4(EventWaterCurrentMove *work);
void func_ov021_021c8dd4(EventWaterCurrentMove *work);
void func_ov021_021c8e5c(EventWaterCurrentMove *work);
void func_ov021_021c8d3c(EventWaterCurrentMove *work);
void func_ov021_021c8de4(EventWaterCurrentMove *work);
BOOL IsTileWaterCurrent_(u32 tileClass);

const WaterCurrentTile *GetWaterCurrentTileConfig(u32 tileClass) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (tileClass == WATER_CURRENT_TILES[i].tileClass) {
            return &WATER_CURRENT_TILES[i];
        }
    }
    return NULL;
}

BOOL IsTileWaterCurrent_(u32 tileClass) {
    if (GetWaterCurrentTileConfig(tileClass) != NULL) {
        return TRUE;
    }
    return FALSE;
}

const WaterCurrentTileAcmd *func_ov021_021c8bd8(u32 tileClass) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (tileClass == data_ov021_021dab44[i].tileClass) {
            return &data_ov021_021dab44[i];
        }
    }
    return NULL;
}

GameEventReturnCode func_ov021_021c8bfc(GameEvent *event, u32 *state, void *data) {
    EventWaterCurrentMove *work = data;

    switch (*state) {
    case 0:
        if (IsTileWaterCurrent_(func_ov021_021c8d18(work)) == 0) {
            *state = 2;
        } else {
            func_ov021_021acbc8(func_ov021_021a2f98(work->player), FALSE);
            func_ov021_021c8e30(work);
            *state = 1;
        }
        break;
    case 1:
        func_ov021_021c8e30(work);
        if (func_ov010_0216d454(work->actor) != 0) {
            func_ov021_021c8d74(work);
            func_ov021_021c8dc4(work);
            func_ov021_021c8dd4(work);
            if (work->tile == NULL && work->flowTile == NULL) {
                func_ov021_021c8e5c(work);
                *state = 2;
            } else {
                func_ov021_021c8d3c(work);
            }
        }
        break;
    case 2:
        func_ov021_021c8de4(work);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventWaterCurrentMove_Create(GameEvent *parent, GameSystem *gsys, Field *field) {
    FieldPlayer *player;
    GameEvent *event;
    EventWaterCurrentMove *work;

    player = Field_GetPlayer(field);
    event = GameEvent_Create(gsys, parent, func_ov021_021c8bfc, sizeof(EventWaterCurrentMove));
    work = GameEvent_GetData(event);
    work->gsys = gsys;
    work->field = field;
    work->player = player;
    work->actor = FieldPlayer_GetActor(player);
    work->tile = NULL;
    work->flowTile = NULL;
    work->flowing = FALSE;
    work->flowPlayer = GFL_SndSeqGetPlayerIndex(0x55e);
    work->endPlayer = GFL_SndSeqGetPlayerIndex(0x6e2);
    work->loopPlayer = GFL_SndSeqGetPlayerIndex(0x769);
    return event;
}

BOOL IsTileWaterCurrent(u32 tileClass) {
    return IsTileWaterCurrent_(tileClass);
}

u16 func_ov021_021c8cf8(EventWaterCurrentMove *work) {
    if (work->tile != NULL) {
        return work->tile->acmd;
    }
    return 0;
}

u16 func_ov021_021c8d08(EventWaterCurrentMove *work) {
    if (work->flowTile != NULL) {
        return work->flowTile->acmd;
    }
    return 0;
}

u16 func_ov021_021c8d18(EventWaterCurrentMove *work) {
    FieldG3DMapper *mapper;
    u32 tileType;
    VecFx32 pos;

    FieldPlayer_GetWPos(work->player, &pos);
    mapper = Field_GetG3DMapper(work->field);
    tileType = GetTileTypeAtPos(mapper, &pos);
    return GetTileClass(tileType);
}

void func_ov021_021c8d3c(EventWaterCurrentMove *work) {
    u16 acmd;

    if (work->flowTile != NULL) {
        acmd = func_ov021_021c8d08(work);
        FldAct_SetAcmd(work->actor, acmd);
        func_ov021_021c8df4(work);
        work->flowing = TRUE;
        return;
    }
    if (work->tile != NULL) {
        acmd = func_ov021_021c8cf8(work);
        FldAct_SetAcmd(work->actor, acmd);
    }
}

void func_ov021_021c8d74(EventWaterCurrentMove *work) {
    FieldEffects *effects;
    u32 effectId;
    VecFx32 pos;

    if (work->flowTile != NULL) {
        effects = Field_GetFieldEffects(work->field);
        effectId = 9;
        if (FieldEffects_IsLoaded(effects, 9) == 0) {
            FieldEffects_Load(effects, &effectId, 1);
        }
        FieldPlayer_GetWPos(work->player, &pos);
        func_ov021_021ad598(effects, 0, &pos);
        func_ov021_021c8e14(work);
        work->flowTile = NULL;
        work->flowing = FALSE;
    }
}

void func_ov021_021c8dc4(EventWaterCurrentMove *work) {
    work->tile = GetWaterCurrentTileConfig(func_ov021_021c8d18(work));
}

void func_ov021_021c8dd4(EventWaterCurrentMove *work) {
    work->flowTile = func_ov021_021c8bd8(func_ov021_021c8d18(work));
}

void func_ov021_021c8de4(EventWaterCurrentMove *work) {
    func_ov021_021acbc8(func_ov021_021a2f98(work->player), TRUE);
}

void func_ov021_021c8df4(EventWaterCurrentMove *work) {
    if (func_02005c64() == 0) {
        GFL_SndPlayerStop(work->loopPlayer);
        GFL_SEPlayKeepVol(0x55e, work->flowPlayer);
    }
}

void func_ov021_021c8e14(EventWaterCurrentMove *work) {
    if (func_02005c64() == 0) {
        GFL_SEPlayKeepVol(0x6e2, work->endPlayer);
    }
}

void func_ov021_021c8e30(EventWaterCurrentMove *work) {
    if (func_02005c64() == 0 && work->flowing == 0 && GFL_SndPlayerIsActive(work->loopPlayer) == 0) {
        GFL_SEPlayKeepVol(0x769, work->loopPlayer);
    }
}

void func_ov021_021c8e5c(EventWaterCurrentMove *work) {
    GFL_SndPlayerStop(work->loopPlayer);
}
