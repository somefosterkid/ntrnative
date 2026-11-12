// The encounter effects: EncEff_StartEvent starts an effect's event from enceff_data.c's table after loading the
// overlay it needs, and the BGM and effect a battle picks (pokebw2's enceff.c). Black's code at ov021
// 0x021cf45c-0x021cfa58. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/encounter_effect.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "struct_decls.h"
#include "field/field.h"
#include "gfl/heapsys.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "field/encounter_effect_model.h"
#include "field/encounter_effect_ripple.h"
#include "field/encounter_effect_spread.h"
#include "field/event_encounter_cutin.h"
#include "app/box2_obj.h"
#include "battle/trainer_data.h"
#include "field/encounter.h"
#include "field/field_map.h"
#include "field/field_player.h"
#include "field/field_script.h"
#include "field/fieldmap.h"
#include "field/zone.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "system/area_data.h"
#include "system/game_data.h"
#include "system/tr_tool.h"

#define ENCEFF_OVERLAYS 3
#define MAX_TRAINER_IDS 50
#define BOXES 24

// pokebw2's layout and names (Black's is the same)
struct EncEff {
    // The data of the effect's event
    void *eventData;
    void (*render)(EncEff *effect);
    Field *field;
    // The overlays EncEff_StartEvent loaded, unloaded by func_ov021_021cf5ac
    u32 overlays[ENCEFF_OVERLAYS];
    s32 overlayCount;
    void *workArea;
};

// The trainer IDs of the Pokemon that did not come from the player, without repeats
typedef struct TrainerIdList {
    s32 count;
    u32 ids[MAX_TRAINER_IDS];
} TrainerIdList;

// func_ov021_021cf7dc's event: counts them in the party, the boxes and the battle box into *result
typedef struct CountTrainersEvent {
    u32 playerId;
    s32 box;
    u16 *result;
    TrainerIdList ids;
} CountTrainersEvent;

extern u32 OVERLAY_75_ID[];
extern u32 OVERLAY_77_ID[];
void func_ov021_021cf54c(EncEff *encEff);
void func_ov021_021cf5ac(EncEff *enc);

void func_ov021_021cf564(EncEff *encEff, u32 overlayId);
GameEventReturnCode func_ov021_021cf844(GameEvent *param_1, u32 *param_2, void *param_3);
u32 func_ov021_021cfa04(u32 param_1, u32 param_2, TrainerIdList *param_3);
u32 func_ov021_021cfa1c(u32 param_1, TrainerIdList *param_2);
s32 func_ov021_021cf8e4(u32 param_1, GameData *param_2, TrainerIdList *param_3);
s32 func_ov021_021cf940(u32 param_1, GameData *param_2, u32 param_3, TrainerIdList *param_4);
s32 func_ov021_021cf9ac(u32 param_1, GameData *param_2, u32 param_3, TrainerIdList *param_4);

const u16 data_ov021_021db7e0[11] = {
    0x046d, 0x046d, 0x046c, 0x046f, 0x0470, 0x0471, 0x0471, 0x0472, 0x0473, 0x046e, 0x0479,
};

const u8 data_ov021_021db7f6[26] = {
    0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
    0x05, 0x1c, 0x1c, 0x1d, 0x1e, 0x1e, 0x05, 0x00,
};

EncEff *func_ov021_021cf45c(HeapID heapId, Field *field) {
    EncEff *encEff;

    encEff = GFL_HeapAllocate(heapId, sizeof(EncEff), 1, "enceff.c", 0x6c);
    encEff->field = field;
    encEff->overlayCount = 0;
    return encEff;
}

void func_ov021_021cf480(EncEff *encEff) {
    func_ov021_021cf54c(encEff);
    func_ov021_021cf5ac(encEff);
    GFL_HeapFree(encEff);
}

void EncEff_StartEvent(EncEff *encEff, GameEvent *event, u32 effect) {
    GameEvent *next;
    void *data;
    GameSystem *gsys;
    Field *field;

    field = encEff->field;
    gsys = Field_GetGameSystem(field);
    if ((s32)effect >= 37) {
        effect = 0;
    }
    if (GFL_HeapGetHighestAllocatableSize(0x15) < data_ov021_021db4fc[effect].heapSize) {
        effect = 7;
    }
    if (data_ov021_021db4fc[effect].overlayId != 0xffffffff) {
        func_ov021_021cf564(encEff, data_ov021_021db4fc[effect].overlayId);
    }
    next = data_ov021_021db4fc[effect].create(gsys, field, data_ov021_021db4fc[effect].param);
    GameEvent_ChainNext(event, next);
    data = GameEvent_GetData(next);
    encEff->eventData = data;
    encEff->render = data_ov021_021db4fc[effect].update;
}

void *EncEff_GetEventData(EncEff *effect) {
    return effect->eventData;
}

void func_ov021_021cf520(EncEff *encEff) {
    if (encEff->render != NULL) {
        encEff->render(encEff);
    }
}


void *EncEff_AllocWorkArea(EncEff *effect, u32 size, HeapID heapId) {
    void *ptr;

    ptr = GFL_HeapAllocate(heapId, size, 1, "enceff.c", 0xe3);
    effect->workArea = ptr;
    return ptr;
}

void func_ov021_021cf54c(EncEff *encEff) {
    if (encEff->workArea != NULL) {
        GFL_HeapFree(encEff->workArea);
        encEff->workArea = NULL;
    }
}

void *EncEff_GetWorkArea(EncEff *effect) {
    return effect->workArea;
}

void func_ov021_021cf564(EncEff *encEff, u32 overlayId) {
    s32 val;

    GFL_OvlLoad(overlayId);
    val = encEff->overlayCount;
    encEff->overlayCount = val + 1;
    encEff->overlays[val] = overlayId;
}

void func_ov021_021cf580(EncEff *effect) {
    func_ov021_021cf564(effect, OVERLAY_ID(75));
    func_ov021_021cf564(effect, OVERLAY_ID(77));
}



void func_ov021_021cf59c(EncEff *effect) {
    func_ov021_021cf564(effect, OVERLAY_ID(75));
}

void func_ov021_021cf5ac(EncEff *enc) {
    s32 i;
    if (enc->overlayCount > 0) {
        while (enc->overlayCount > 0) {
            i = enc->overlayCount - 1;
            if (enc->overlays[i] != 0) {
                GFL_OvlUnload(enc->overlays[i]);
            }
            enc->overlayCount--;
        }
    }
}


void EventBattleCall_DecideEnvWild(s32 species, s32 a2, BOOL a3, Field *field, u32 *encEffect, u16 *bgm) {
    u16 zoneId;
    FieldPlayer *player;
    u32 tileClass;
    u32 tileType;
    u32 encounter;
    u16 bgmId;

    zoneId = func_02013e40(Field_GetPlayerStateZoneID(field));
    switch (species) {
    case 0x23b:
        *bgm = 0x469;
        *encEffect = 0x23;
        return;
    case 0x283:
        *bgm = 0x474;
        *encEffect = 0x20;
        return;
    case 0x284:
        *bgm = 0x475;
        *encEffect = 0x20;
        return;
    case 0x286:
        *bgm = 0x476;
        *encEffect = 0x20;
        return;
    case 0x27d:
        *bgm = 0x468;
        *encEffect = 0x1f;
        return;
    case 0x1ee:
        *bgm = 0x478;
        *encEffect = 0x1f;
        return;
    case 0x27e:
    case 0x27f:
    case 0x280:
    case 0x287:
        *bgm = 0x478;
        *encEffect = 0x22;
        return;
    case 0x281:
    case 0x282:
    case 0x285:
        *bgm = 0x477;
        *encEffect = 0x21;
        return;
    }
    if (a2 == 1) {
        *bgm = 0x469;
        *encEffect = 0x1f;
        return;
    }
    if (a2 == 4) {
        if (zoneId == 0xd6) {
            *bgm = 0x469;
        } else {
            *bgm = 0x468;
        }
        *encEffect = 2;
        return;
    }
    player = Field_GetPlayer(field);
    tileType = FieldPlayer_GetTileTypeUnder(player);
    tileClass = GetTileClass(tileType);
    if (GetTileFlags(tileType) & 2) {
        bgmId = 0x468;
        encounter = 2;
    } else if (MapTile_IsTallGrass(tileClass)) {
        if (a3) {
            bgmId = 0x469;
            encounter = 1;
        } else {
            bgmId = 0x468;
            encounter = 0;
        }
    } else if (MapTile_IsSwamp(tileClass)) {
        bgmId = 0x468;
        encounter = 0;
    } else if (IsTileSand(tileClass)) {
        bgmId = 0x468;
        encounter = 4;
    } else {
        bgmId = 0x468;
        encounter = 3;
    }
    *encEffect = encounter;
    if (zoneId == 0xd6) {
        *bgm = 0x469;
    } else {
        *bgm = bgmId;
    }
}

void EventBattleCall_DecideEnvTrainer(u32 trainerId, Field *field, u32 *encEffect, u16 *bgm) {
    BOOL isExterior = TRUE;
    u32 trainerClass;
    u32 bgmGroup;
    u8 encounter;
    FieldPlayer *player;
    u32 tileType;
    u16 tileClass;
    AreaData *areaData;
    u32 effect;

    trainerClass = TrainerData_GetParam(trainerId, 1);
    bgmGroup = GetTrainerClassBGMGroupId(trainerClass);
    encounter = func_0202a3d8(trainerClass);
    if (bgmGroup < 0xb) {
        *bgm = data_ov021_021db7e0[bgmGroup];
        *encEffect = data_ov021_021db7f6[encounter];
        return;
    }
    player = Field_GetPlayer(field);
    tileType = FieldPlayer_GetTileTypeUnder(player);
    tileClass = GetTileClass(tileType);
    if (GetTileFlags(tileType) & 2) {
        effect = 6;
    } else if (IsTileSand(tileClass)) {
        effect = 8;
    } else {
        areaData = func_ov021_02188d58(field);
        if (!AreaData_IsExterior(areaData)) {
            isExterior = FALSE;
        }
        effect = isExterior ? 5 : 7;
    }
    *bgm = 0x46a;
    *encEffect = effect;
}

u32 func_ov021_021cf7dc(VM *param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    GameSystem *gsys;
    GameData *gameData;
    u16 *ptr;
    GameEvent *event;
    PlayerInfo *info;
    CountTrainersEvent *data;

    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    gameData = FieldScriptEnv_GetGameData(param_2);
    ptr = ScriptReadVar(param_1, param_2);
    event = GameEvent_Create(gsys, NULL, func_ov021_021cf844, sizeof(CountTrainersEvent));
    info = GetGameDataPlayerInfo(gameData);
    data = GameEvent_GetData(event);
    data->playerId = getIDAsUInt(info);
    data->ids.count = 0;
    data->box = 0;
    data->result = ptr;
    ScriptWork_CallEvent(work, event);
    return 1;
}

GameEventReturnCode func_ov021_021cf844(GameEvent *param_1, u32 *param_2, void *param_3) {
    CountTrainersEvent *count = param_3;
    BOOL done = FALSE;
    GameSystem *gsys;
    GameData *gameData;
    s32 base;
    s32 i;

    gsys = GameEvent_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    switch (*param_2) {
    case 0:
        func_ov021_021cf8e4(count->playerId, gameData, &count->ids);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        base = count->box;
        for (i = 0; i < 2; i++) {
            func_ov021_021cf940(count->playerId, gameData, base + i, &count->ids);
            count->box = count->box + 1;
            if (count->box >= BOXES) {
                *param_2 = *param_2 + 1;
                break;
            }
        }
        break;
    case 2:
        func_ov021_021cf9ac(count->playerId, gameData, 0, &count->ids);
        done = TRUE;
        break;
    }
    if (count->ids.count >= MAX_TRAINER_IDS) {
        done = TRUE;
    }
    if (done) {
        *count->result = (u16)count->ids.count;
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


s32 func_ov021_021cf8e4(u32 param_1, GameData *param_2, TrainerIdList *param_3) {
    PokeParty *party;
    s32 pkmCount;
    PartyPkm *pkm;
    u32 result;
    s32 i;
    u32 index;

    party = GameData_GetParty(param_2);
    pkmCount = PokeParty_GetPkmCount(party);
    i = 0;
    index = 0;
    if (0 < pkmCount) {
        do {
            pkm = PokeParty_GetPkm(party, index);
            result = PokeParty_GetParam(pkm, 0x4c, (void *)0x0);
            if (result == 0) {
                result = PokeParty_GetParam(pkm, 7, (void *)0x0);
                result = func_ov021_021cfa04(param_1, result, param_3);
                if (result != 0) {
                    i = i + 1;
                }
            }
            index = index + 1;
        } while ((s32)index < pkmCount);
    }
    return i;
}

s32 func_ov021_021cf940(u32 param_1, GameData *param_2, u32 param_3, TrainerIdList *param_4) {
    BoxSaveAccessor *boxes;
    u32 result;
    u32 pos;
    s32 i;

    boxes = GameData_GetBoxSaveAccessor(param_2);
    i = 0;
    pos = 0;
    do {
        result = BoxSaveAccessor_GetPkmParam(boxes, param_3, pos, 0xa9, (void *)0x0);
        if (result != 0) {
            result = BoxSaveAccessor_GetPkmParam(boxes, param_3, pos, 0x4c, (void *)0x0);
            if (result == 0) {
                result = BoxSaveAccessor_GetPkmParam(boxes, param_3, pos, 7, (void *)0x0);
                result = func_ov021_021cfa04(param_1, result, param_4);
                if (result != 0) {
                    i = i + 1;
                }
            }
        }
        pos = pos + 1;
    } while ((s32)pos < 0x1e);
    return i;
}

s32 func_ov021_021cf9ac(u32 param_1, GameData *param_2, u32 param_3, TrainerIdList *param_4) {
    SaveControl *save;
    BattleBoxSave *battleBox;
    BoxPkm *pkm;
    u32 result;
    u32 slot;
    s32 i;

    save = GameData_GetSaveControl(param_2);
    battleBox = getBattleBox(save);
    i = 0;
    slot = 0;
    do {
        pkm = getBoxSlotAddress(battleBox, param_3, slot);
        result = PML_PkmGetParam(pkm, 0xa9, (void *)0x0);
        if (result != 0) {
            result = PML_PkmGetParam(pkm, 7, (void *)0x0);
            result = func_ov021_021cfa04(param_1, result, param_4);
            if (result != 0) {
                i = i + 1;
            }
        }
        slot = slot + 1;
    } while ((s32)slot < 6);
    return i;
}

u32 func_ov021_021cfa04(u32 param_1, u32 param_2, TrainerIdList *param_3) {
    u32 result;

    result = 0;
    if (param_1 != param_2) {
        result = func_ov021_021cfa1c(param_2, param_3);
    }
    return result;
}

u32 func_ov021_021cfa1c(u32 param_1, TrainerIdList *param_2) {
    s32 count = param_2->count;
    s32 i;

    if (count >= MAX_TRAINER_IDS) {
        return 0;
    }
    for (i = 0; i < param_2->count; i++) {
        if (param_1 == param_2->ids[i]) {
            return 0;
        }
    }
    param_2->ids[i] = param_1;
    param_2->count = param_2->count + 1;
    return 1;
}
