// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov033/event_fishing.c, Black 2/White 2's
// overlay 33's. Black's has fewer steps: it checks no height for surfing and no tile for the rod's animation, and
// records neither the Pokémon hooked nor a battle's start. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "battle/btl_setup.h"
#include "field/encounter.h"
#include "field/event_battle.h"
#include "field/event_fishing.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "field/field_g3d_mapper.h"
#include "field/field_map.h"
#include "field/field_player.h"
#include "field/field_script.h"
#include "gfl/key.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "save/records.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

BOOL CreateSurfPos(void *context, Field *field, VecFx32 *position) {
    VecFx32 targetPosition;
    MapTerrainBuf terrain;
    FieldG3DMapper *mapper;
    FieldPlayer *player;
    FieldActor *actor;
    u8 direction;

    mapper = Field_GetG3DMapper(field);
    player = Field_GetPlayer(field);
    actor = FieldPlayer_GetActor(player);
    direction = GetActorFaceDir(actor);
    FieldPlayer_GetWPosInDir(player, direction, &targetPosition);
    if (Field_GetResolvedControllerTypeID(field) == 1) {
        return FALSE;
    }
    if (FieldG3DMapper_GetTerrain(mapper, &targetPosition, &terrain) == FALSE) {
        return FALSE;
    }
    if (MapTile_IsSurfEdge(GetTileClass(terrain.tileType)) == TRUE) {
        ExpandVecInGridDir(direction, &targetPosition, 1 << 16);
        if (FieldG3DMapper_GetTerrain(mapper, &targetPosition, &terrain) == FALSE) {
            return FALSE;
        }
    }
    if (IsTileSurfWater(GetTileClass(terrain.tileType)) == FALSE) {
        return FALSE;
    }
    if (position != NULL) {
        position->x = targetPosition.x;
        position->y = terrain.height;
        position->z = targetPosition.z;
    }
    return TRUE;
}

GameEvent *EventFieldFishing_Create(Field *field, GameSystem *gsys) {
    GameEvent *event;
    FishingEventWork *work;
    u32 flag;

    event = GameEvent_Create(gsys, NULL, EventFieldFishing_Callback, sizeof(FishingEventWork));
    work = GameEvent_GetData(event);
    MI_CpuFill8(work, 0, sizeof(FishingEventWork));
    work->gsys = gsys;
    work->field = field;
    work->gameData = GSYS_GetGameData(gsys);
    work->encountSystem = Field_GetEncountSystem(work->field);
    work->records = GameData_GetRecords(work->gameData);
    work->actorSystem = Field_GetActorSystem(work->field);
    work->player = Field_GetPlayer(field);
    work->actor = FieldPlayer_GetActor(work->player);
    work->faceDirection = GetActorFaceDir(work->actor);
    FieldPlayer_GetWPos(work->player, &work->playerPos);
    work->playerExState = FieldPlayer_DeriveExState(work->player);
    if (!CreateSurfPos(work->gameData, work->field, &work->fishingPos)) {
        work->noFishing = 1;
    }
    work->gridX = (work->fishingPos.x >> 4) / 4096;
    work->gridY = (work->fishingPos.y >> 4) / 4096;
    work->gridZ = (work->fishingPos.z >> 4) / 4096;
    if (EncountState_CheckSpecialEncountPos(work->encountSystem, &work->gridX)) {
        work->isPhenomenon = flag = TRUE;
    } else {
        work->isPhenomenon = flag = FALSE;
    }
    work->battleSetup = BtlSetup_CreateFishing(work->encountSystem, flag);
    DisableAllActorsMovement(work->actorSystem);
    return event;
}

GameEventReturnCode EventFieldFishing_Callback(GameEvent *event, u32 *state, void *data) {
    FishingEventWork *work;
    GameEvent *next;
    u32 result;
    u8 battleMode;

    work = data;
    switch (*state) {
    case 0:
        if (work->noFishing != 0) {
            *state = 8;
            break;
        }
        FieldPlayer_SetSpecialSeq(work->player, 0x200);
        ++*state;
    case 1:
        if (func_ov021_021a2f54(work->player) == 1) {
            func_ov010_0216d674(work->actor, 0);
            GFL_SndSEPlay(0x684);
            ++*state;
        }
        break;
    case 2:
        if (func_ov021_021c5fa8(work, 15) != 0) {
            func_ov021_021c602c(work);
            if (work->battleSetup == NULL) {
                *state = 5;
            } else {
                work->timer = GFL_RandomLCAlt(90) + 30;
                ++*state;
            }
        }
        break;
    case 3:
        result = func_ov021_021c5fc0(work, work->timer);
        if (result == 1) {
            *state = 6;
        } else if (result == 2) {
            func_ov021_021c5fec(work);
            ++*state;
        }
        break;
    case 4:
        result = func_ov021_021c5fc0(work, 30);
        if (result != 0) {
            func_ov021_021c6018(work);
            if (result == 1) {
                func_ov021_021c6054(work);
                func_ov010_0216d674(work->actor, 3);
                EventScriptCall_Start(event, 0x2796, NULL, 0, HEAPID_FIELDMAP);
                *state = 10;
            } else if (result == 2) {
                RecordAddOne(work->records, 0x50);
                *state = 7;
            }
        }
        break;
    case 5:
        result = func_ov021_021c5fc0(work, 120);
        if (result != 2) {
            if (result == 1) {
                *state = 6;
            }
            break;
        }
    case 6:
    case 7:
        func_ov021_021c6054(work);
        if (work->battleSetup != NULL) {
            if (work->isPhenomenon == 1) {
                EncountSystem_CancelPhenomenon(work->encountSystem);
            }
            BtlSetup_Free(work->battleSetup);
        }
        func_ov010_0216d674(work->actor, 1);
        EventScriptCall_Start(event, 0x2792 + *state, NULL, 0, 0x15);
        *state = 8;
        break;
    case 8:
        FieldPlayer_SetSpecialSeq(work->player, 8);
        *state = 9;
    case 9:
        if (func_ov021_021a2f54(work->player) != 1) {
            break;
        }
        return GAMEEVENT_DONE;
    case 10:
        RecordAddOne(work->records, 8);
        battleMode = work->isPhenomenon == 1 ? 1 : 4;
        next = EventWildBattleCall_Create(work->gsys, work->field, work->battleSetup, 0, battleMode);
        GameEvent_Replace(event, next);
        return GAMEEVENT_CONTINUE;
    }
    return GAMEEVENT_CONTINUE;
}

u32 func_ov021_021c5fa8(FishingEventWork *work, u32 value) {
    u32 elapsed;

    elapsed = work->elapsed++;
    if (elapsed < value) {
        return 0;
    }
    work->elapsed = 0;
    return 1;
}

u32 func_ov021_021c5fc0(FishingEventWork *work, u32 value) {
    u32 elapsed;

    elapsed = work->elapsed++;
    if (elapsed < value) {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            work->elapsed = 0;
            return 1;
        }
    } else {
        work->elapsed = 0;
        return 2;
    }
    return 0;
}

void func_ov021_021c5fec(FishingEventWork *work) {
    void *effects;

    effects = Field_GetFieldEffects(work->field);
    func_ov010_0216d674(work->actor, 2);
    work->effect2C = func_ov021_021bb528(effects, work->actor, 0, 1);
    func_ov021_021ae01c(work->effect30, 1);
}

void func_ov021_021c6018(FishingEventWork *work) {
    if (work->effect2C != NULL) {
        func_ov021_021ac154(work->effect2C);
        work->effect2C = NULL;
    }
}

void func_ov021_021c602c(FishingEventWork *work) {
    void *effects;
    u32 sameHeight;

    effects = Field_GetFieldEffects(work->field);
    sameHeight = 1;
    if (work->fishingPos.y != work->playerPos.y) {
        sameHeight = 0;
    }
    work->effect30 = func_ov021_021adf94(effects, &work->fishingPos, work->faceDirection, sameHeight);
}

void func_ov021_021c6054(FishingEventWork *work) {
    if (work->effect30 != NULL) {
        func_ov021_021ac154(work->effect30);
        work->effect30 = NULL;
    }
}
