// Black's code at ov021 0x021cd1f0-0x021cd48c. No name is known: the module and address.

#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_map.h"
#include "gfl/arc.h"
#include "app/name_entry.h"
#include "field/beacon_view.h"
#include "field/field_event.h"
#include "field/game_beacon_set.h"
#include "gfl/heap.h"
#include "gfl/str.h"
#include "system/game_beacon.h"
#include "system/game_event.h"
#include "system/trainer_game_info_savedata.h"
#include "struct_decls.h"
#include "gfl/overlay.h"
#include "system/game_system.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "save/player_info.h"
#include "nitro/fx/fx_vec.h"

// pokebw2's EventPhraseInputData, without the C-Gear name's save block that Black 2 has after trainerInfo
typedef struct EventPhraseInputData {
    GameSystem *gsys;
    GameData *gameData;
    TrainerGameInfoSave *trainerInfo;
    PlayerInfo *playerInfo;
    Field *field;
    BeaconStatus *status;
    u16 *result;
    NameEntryParam nameEntry;
    HeapID heapId;
    u16 unk4A;
    u32 mode;
} EventPhraseInputData;

const u8 data_ov021_021db44c[4] = { 0x08, 0x08, 0x08, 0x00 };

u16 func_ov021_021cd400(u32 param_1, u32 param_2, u32 param_3);

const u16 data_ov021_021db450[4][4] = {
    { 0x00f0, 0x0320, 0x0113, 0x00f1 },
    { 0x00f0, 0x0311, 0x0121, 0x00f2 },
    { 0x00f0, 0x032f, 0x0121, 0x00f3 },
    { 0x00f0, 0x0320, 0x0133, 0x00f4 },
};

void func_ov021_021cd3a4(EventPhraseInputData *data, NameEntryParam *entry);
GameEventReturnCode func_ov021_021cd348(GameEvent *event, u32 *state, void *eventData);

void FldActMatColorPreset_Free(FldActMatColorPreset *preset) {
    GFL_ArcToolFree(preset->arcTool);
    GFL_HeapFree(preset);
}

void FldActMatColorPreset_Load(FldActMatColorPreset *preset, u32 presetId) {
    GFL_ArcToolRead(preset->arcTool, presetId, preset->normal);
}

void FldActMatColorPreset_Apply(FldActMatColorPreset *preset, BlActScene *scene) {
    VecFx16 normal;
    u16 diffuse;
    u16 ambient;
    u16 specular;
    u16 emission;

    normal.x = preset->normal[0];
    normal.y = preset->normal[1];
    normal.z = preset->normal[2];
    VEC_Fx16Normalize(&normal, &normal);
    BlActScene_SetCustomNormal(scene, &normal);
    diffuse = preset->diffuse[2] << 10 | (preset->diffuse[0] | preset->diffuse[1] << 5);
    BlActScene_SetColDiffuse(scene, &diffuse);
    ambient = preset->ambient[2] << 10 | (preset->ambient[0] | preset->ambient[1] << 5);
    BlActScene_SetColAmbient(scene, &ambient);
    specular = preset->specular[2] << 10 | (preset->specular[0] | preset->specular[1] << 5);
    BlActScene_SetColSpecular(scene, &specular);
    emission = preset->emissive[2] << 10 | (preset->emissive[0] | preset->emissive[1] << 5);
    BlActScene_SetColEmissive(scene, &emission);
}

GameEvent *EventPhraseInput_Create(GameSystem *gsys, Field *field, GameEvent *parent, u32 mode, u16 *result) {
    GameEvent *event;
    EventPhraseInputData *data;

    event = GameEvent_Create(gsys, parent, func_ov021_021cd348, sizeof(EventPhraseInputData));
    data = GameEvent_GetData(event);
    data->gsys = gsys;
    data->gameData = GSYS_GetGameData(gsys);
    data->field = field;
    data->result = result;
    data->mode = mode;
    data->heapId = 4;
    data->trainerInfo = getTrainerGameInfoAddress(GameData_GetSaveControl(data->gameData));
    data->playerInfo = GetGameDataPlayerInfo(data->gameData);
    data->status = func_02012a54(data->gameData);
    data->nameEntry.mode = mode;
    data->nameEntry.gender = getTrainerGender(data->playerInfo);
    data->nameEntry.gameInfo = data->trainerInfo;
    data->nameEntry.maxLength = data_ov021_021db44c[mode - 5];
    data->nameEntry.name = GFL_StrBufCreate(data->nameEntry.maxLength + 1, data->heapId);
    switch (mode) {
    case 5:
        GFL_StrBufLoadString(data->nameEntry.name, func_0200c1f4(data->trainerInfo));
        break;
    case 6:
        GFL_StrBufLoadString(data->nameEntry.name, func_0200c20c(data->trainerInfo));
        break;
    case 7:
        break;
    }
    return event;
}



GameEventReturnCode func_ov021_021cd348(GameEvent *event, u32 *state, void *eventData) {
    EventPhraseInputData *data = eventData;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, EventFieldSubprocessTransition_Create(data->gsys, data->field, OVERLAY_ID(194), &data_ov194_021f5580, &data->nameEntry));
        (*state)++;
        break;
    case 1:
        func_ov021_021cd3a4(data, &data->nameEntry);
        GFL_StrBufFree(data->nameEntry.name);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


void func_ov021_021cd3a4(EventPhraseInputData *data, NameEntryParam *entry) {
    if (data->result != NULL) {
        *data->result = (entry->unk1C == 0);
    }
    if (entry->unk1C != 1) {
        switch (data->mode) {
        case 5:
            func_0200c1f8(data->trainerInfo, entry->name);
            func_02027198();
            break;
        case 6:
            func_0200c210(data->trainerInfo, entry->name);
            break;
        case 7:
            GFL_StrBufCopy(func_020275c4(data->status), data->nameEntry.name);
            func_ov010_02165844(entry->name);
            break;
        }
    }
}


u16 func_ov021_021cd400(u32 param_1, u32 param_2, u32 param_3) {
    u32 i;
    const u16 *entry;
    entry = data_ov021_021db450[0];
    for (i = 0; i < 4; i++) {
        if (entry[0] == param_1) {
            if (entry[1] <= param_2 && (int)param_2 < entry[1] + 3) {
                if (entry[2] <= param_3 && (int)param_3 < entry[2] + 3) {
                    return entry[3];
                }
            }
        }
        entry += 4;
    }
    return 0x1ab;
}


u16 GetAbyssalRuinsDiveZoneID(Field *field, u16 *zoneId) {
    u16 playerStateZoneID;
    u16 gPosX;
    u16 gPosZ;
    FieldPlayer *player;
    FieldActor *actor;

    playerStateZoneID = Field_GetPlayerStateZoneID(field);
    player = Field_GetPlayer(field);
    actor = FieldPlayer_GetActor(player);
    gPosX = GetGPosX(actor);
    gPosZ = GetGPosZ(actor);
    gPosX = func_ov021_021cd400((u32)playerStateZoneID, (u32)gPosX, (u32)gPosZ);
    *zoneId = gPosX;
    if (*zoneId != 0x1ab) {
        return 1;
    }
    *zoneId = playerStateZoneID;
    return 0;
}
