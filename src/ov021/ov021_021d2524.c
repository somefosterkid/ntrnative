// Black's code at ov021 0x021d2524-0x021d297c. No name is known: the module and address.


#include "types.h"
#include "field/field_script.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "field/city_state.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_player.h"
#include "field/field_player_core.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/key.h"
#include "gfl/random.h"
#include "pml/poke_graphic.h"
#include "save/adventure.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "struct_decls.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

typedef struct {
    BmpWin *window;
    ClActUnit *unit;
    ClActor *actor;
    u32 chars;
    u32 palette;
    u32 cellAnims;
} EventFieldPokeDisp;

GameEventReturnCode func_ov021_021d26d0(GameEvent *event, u32 *state, void *data);
void func_ov021_021d25a4(EventFieldPokeDisp *data);
void func_ov021_021d25f4(EventFieldPokeDisp *data, u16 species, u16 form, int sex, u32 rare, u32 personality);

u16 func_02013c60(u16 param_1);
void FieldPlayer_GetGPos(FieldPlayer *player, s16 *x, s16 *y, s16 *z);
void func_ov021_021a36bc(FieldPlayer *player, RailPosition *rail);
void *func_ov020_02186830(void *handle, BOOL flag, HeapID heapId);
u8 *func_ov020_02186844(void *handle, u32 index);
u8 *func_ov020_0218684c(void *handle, u32 index);
void func_ov020_0218683c(void *handle);


u32 func_ov021_021d2524(VM *param_1, FieldScriptEnv *param_2) {
    u16 result;
    u16 result2;
    u16 result3;
    u16 result4;
    GameSystem *gsys;
    ScriptWork *work;
    GameEvent *event;
    EventFieldPokeDisp *data;

    gsys = FieldScriptEnv_GetGameSystem(param_2);
    GSYS_GetField(gsys);
    result = ScriptReadAny(param_1, param_2);
    result2 = ScriptReadAny(param_1, param_2);
    result3 = ScriptReadAny(param_1, param_2);
    result4 = ScriptReadAny(param_1, param_2);
    work = FieldScriptEnv_GetScriptWork(param_2);
    event = GameEvent_Create(gsys, NULL, func_ov021_021d26d0, sizeof(EventFieldPokeDisp));
    data = GameEvent_GetData(event);
    func_ov021_021d25a4(data);
    func_ov021_021d25f4(data, result, result2, result3, result4, 0);
    ScriptWork_CallEvent(work, event);
    return 1;
}

void func_ov021_021d25a4(EventFieldPokeDisp *data) {
    BmpWin *window;
    GFLBitmap *bitmap;

    window = BmpWin_CreateDynamic(1, 0xa, 3, 0xc, 0xc, 0xd, 1);
    data->window = window;
    bitmap = BmpWin_GetBitmap(window);
    GFL_BitmapFill(bitmap, 0x11);
    BmpWin_FlushChar(data->window);
    BmpWin_FlushMap(data->window);
    BmpWin_DrawFrame(data->window, 1, 1, 0xd);
}

void func_ov021_021d25e8(EventFieldPokeDisp *data) {
    func_0201f7a8(data->window, 1);
}

void func_ov021_021d25f4(EventFieldPokeDisp *data, u16 species, u16 form, int sex, u32 rare, u32 personality) {
    ArcTool *arc;

    data->unit = func_0204b100(1, 0, 0x8015);
    arc = MakePokeGraArcHandle(0x8015);
    data->chars = func_0202d81c(arc, species, form, sex, rare, 0, 0, personality, 0, 0x8015);
    data->palette = func_0202d7d8(arc, species, form, sex, rare, 0, 0, 0, 0, 0x8015);
    GFL_ArcToolFree(arc);
    data->cellAnims = func_0202d898(species, form, sex, rare, 0, 0, 1, 0, 0x8015);
    {
        ClActorSetup setup = {0x80, 0x48, 0, 0, 0};
        data->actor = func_0204b294(data->unit, data->chars, data->palette, data->cellAnims, &setup, 0, 0x8015);
    }
}


void func_ov021_021d26ac(EventFieldPokeDisp *data) {
    func_0204b3b4(data->actor);
    func_0204a8d4(data->chars);
    func_0204ada4(data->palette);
    func_0204afd8(data->cellAnims);
    func_0204b1cc(data->unit);
}

GameEventReturnCode func_ov021_021d26d0(GameEvent *event, u32 *state, void *data) {
    EventFieldPokeDisp *disp = data;

    switch (*state) {
    case 0:
        if (GCTX_HIDGetPressedKeys() & 3) {
            func_ov021_021d26ac(disp);
            func_ov021_021d25e8(disp);
            (*state)++;
        }
        break;
    case 1:
        BmpWin_Free(disp->window);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


u32 func_ov021_021d2710(VM *param_1, FieldScriptEnv *param_2) {
    u16 flag;
    u16 *ptr;
    GameSystem *gsys;
    GameData *gameData;
    TrainerCardSave *trainerCard;
    PlayerInfo *playerInfo;
    BOOL BVar3;

    flag = ScriptReadAny(param_1, param_2);
    ptr = ScriptReadVar(param_1, param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    gameData = GSYS_GetGameData(gsys);
    trainerCard = getTrainerCardDataBlkAddress(gameData);
    playerInfo = GetGameDataPlayerInfo(gameData);
    BVar3 = isOneShotDRObtained(trainerCard, (u32)flag, playerInfo);
    *ptr = (u16)BVar3;
    return 0;
}

BOOL func_ov021_021d2750(GameSystem *gsys) {
    FieldActorConfig config;
    VecFx32 pos;
    RailPosition rail;
    s16 x;
    s16 y;
    s16 z;
    GameData *gameData;
    EventData *eventData;
    ZoneNPC *npcs;
    Field *field;
    MMSys *actorSystem;
    u32 zoneId;
    ZoneNPCGridPosition *grid;
    void *handle;
    s32 npcCount;
    FieldPlayer *player;
    s32 i;
    s32 j;
    ZoneNPC *npc;
    void *list;
    u8 *entry;
    u32 exState;
    BOOL flag;

    gameData = GSYS_GetGameData(gsys);
    eventData = GameData_GetEventData(gameData);
    npcs = GetZoneNPCs(eventData);
    npcCount = GetZoneNPCsCount(eventData);
    field = GSYS_GetField(gsys);
    actorSystem = Field_GetActorSystem(field);
    player = Field_GetPlayer(field);
    zoneId = Field_GetPlayerStateZoneID(field);
    if (func_02013c60(zoneId) == FALSE) {
        return FALSE;
    }
    exState = FieldPlayer_GetExState(player);
    if (exState == 2 || exState == 3) {
        return FALSE;
    }
    if ((npcs == NULL || npcCount == 0) && zoneId != 0 && zoneId != 0x1a8) {
        return TRUE;
    }
    for (i = 0; i < npcCount; i++) {
        npc = &npcs[i];
        if (npc->isRail == FALSE) {
            grid = &npc->pos.grid;
            GetNPCMdlInfoForOBJCODE(actorSystem, npc->modelId, &config);
            FieldPlayer_GetGPos(player, &x, &y, &z);
            if (npc->pos.grid.x <= x && x < npc->pos.grid.x + config.collWidth
                && grid->z - config.collHeight < z && z <= grid->z) {
                return FALSE;
            }
        } else if (npc->isRail == TRUE) {
            ZoneNPCRailPosition *npcRail = &npc->pos.rail;
            if (Field_GetResolvedControllerTypeID(field) == 1) {
                func_ov021_021a36bc(player, &rail);
                if (npcRail->railIndex == rail.componentId && npcRail->frontPos == rail.posFront
                    && npcRail->sidePos == rail.posSide) {
                    return FALSE;
                }
            }
        }
    }
    if (Field_GetResolvedControllerTypeID(field) == 0) {
        FieldActor *actor = FieldPlayer_GetActor(player);
        EventWork *eventWork = GameData_GetEventWork(gameData);
        CopyActorWPos(actor, &pos);
        if (FindCollidingZoneTriggerAtLocation(eventData, eventWork, &pos) != NULL) {
            return FALSE;
        }
    }
    if (zoneId == 0 || zoneId == 0x1a8) {
        FieldPlayer_GetGPos(player, &x, &y, &z);
        flag = FALSE;
        handle = func_ov020_021867c4(0, 0x8015);
        if (zoneId != 0) {
            flag = TRUE;
        }
        list = func_ov020_02186830(handle, flag, 0x8015);
        for (j = 0; j < 10; j++) {
            entry = func_ov020_02186844(list, j);
            if (entry[0] == x && entry[1] == z) {
                func_ov020_0218683c(list);
                func_ov020_02186804(handle);
                return FALSE;
            }
        }
        for (i = 0; i < 10; i++) {
            entry = func_ov020_0218684c(list, i);
            if (entry[0] == x && entry[1] == z) {
                func_ov020_0218683c(list);
                func_ov020_02186804(handle);
                return FALSE;
            }
        }
        func_ov020_0218683c(list);
        func_ov020_02186804(handle);
    }
    return TRUE;
}



