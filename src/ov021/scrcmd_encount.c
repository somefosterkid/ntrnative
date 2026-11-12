// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/scrcmd_encount.c, ported by tools/port_file.py.
// The script commands of wild encounters: static and scripted wild battles, the capture demo, the phenomena's items,
// roaming Pokémon, the fishing challenge's random Pokémon and the Repel's rearming. The name is the ROM's own, from
// GFL_HeapAllocate's file argument. Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "constants/items.h"
#include "constants/sound.h"
#include "field/enc_pokeset.h"
#include "field/encounter.h"
#include "field/event_battle.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/field_script.h"
#include "field/scrcmd_encount.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "pml/item.h"
#include "save/bag.h"
#include "save/encounter.h"
#include "save/save_control.h"
#include "system/game_beacon.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/vm.h"
#include "field/move_pokemon.h"
#include "battle/btl_result.h"
#include "field/event_game_manual.h"

#define FISHING_CHALLENGE_ZONE_COUNT 6
#define FISHING_SLOT_COUNT 5

// The zones of the fishing challenge, whose fishing Pokémon it picks from
static const u16 CHALLENGE_FISHING_ZONES[FISHING_CHALLENGE_ZONE_COUNT] = { 255, 317, 321, 324, 370, 423 };

BOOL s0174_CallWildBattle(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    u16 *result = ScriptReadVar(vm, env);

    ScriptWork_CallEvent(work, func_ov021_021d30b8(gsys, result));
    return TRUE;
}

BOOL s0297_CallWildBattleEx(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    u16 species = ScriptReadAny(vm, env);
    u16 level = ScriptReadAny(vm, env);
    u16 flags = ScriptReadAny(vm, env);
    ScriptFieldWork *fieldWork;

    ScriptWork_GetGameSystem(work);
    fieldWork = ScriptWork_GetFieldWork(work);
    ScriptWork_CallEvent(work, func_ov021_021aa0a0(Field_GetEncountSystem(fieldWork->field), species, level, flags));
    return TRUE;
}

BOOL s0175_CallWildBattleEnd(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    ScriptWork_CallEvent(work, CallFieldMapEntranceInTransition(gsys, GSYS_GetField(gsys), 0, 0, 1, 0, 0));
    return TRUE;
}

BOOL func_ov021_021b50d0(VM *vm, FieldScriptEnv *env) {
    u32 result = GameData_GetLastBtlResult(FieldScriptEnv_GetGameData(env));
    u16 *dest = ScriptReadVar(vm, env);

    *dest = GetWildBattleResultByCombined(result);
    return FALSE;
}

BOOL s0179_CallCaptureDemo(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    ScriptWork_CallEvent(work, EventCaptureDemo_Create(gsys, GSYS_GetField(gsys), 4));
    return TRUE;
}

BOOL s00BC_PhenomenonGetItemID(VM *vm, FieldScriptEnv *env) {
    Field *field = GSYS_GetField(FieldScriptEnv_GetGameSystem(env));
    u16 *result = ScriptReadVar(vm, env);

    *result = func_ov021_021aab48(Field_GetEncountSystem(field));
    return FALSE;
}

BOOL func_ov021_021b5154(VM *vm, FieldScriptEnv *env) {
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    EncountSave *save = SaveControl_GetEncountSave(GameData_GetSaveControl(gameData));
    u8 slot = ScriptReadAny(vm, env);

    if (EncountSave_GetRoamingPkmStatus(save, slot) == FALSE) {
        func_ov010_0215e440(gameData, slot);
    }
    return FALSE;
}

// Add the species to the list unless it is in it already; the new count
u16 LoadEventEncountPokemonToBuffer(u16 *list, u16 count, u16 species) {
    int i;

    for (i = 0; i < count; i++) {
        if (species == list[i]) {
            return count;
        }
    }
    list[count] = species;
    return count + 1;
}

BOOL func_ov021_021b51bc(VM *vm, FieldScriptEnv *env) {
    EncData encData;
    u8 season;
    u16 i;
    u16 j;
    u16 count;
    GameData *gameData;
    ArcTool *arc;
    u16 *result;
    u16 *list;
    count = 0;
    gameData = FieldScriptEnv_GetGameData(env);
    result = ScriptReadVar(vm, env);
    season = GameData_GetSeason(gameData);
    arc = GFL_ArcSysCreateFileHandle(0x7e, HEAPID_TAIL(HEAPID_FIELDMAP));
    list = GFL_HeapAllocate(HEAPID_TAIL(HEAPID_FIELDMAP), 0x78, TRUE, "scrcmd_encount.c", 248);

    for (i = 0; i < FISHING_CHALLENGE_ZONE_COUNT; i++) {
        if (EncData_Load(&encData, arc, CHALLENGE_FISHING_ZONES[i], season)) {
            if (encData.userData[5] != 0) {
                for (j = 0; j < FISHING_SLOT_COUNT; j++) {
                    count = LoadEventEncountPokemonToBuffer(list, count, encData.fishing[j].species);
                }
            }
            if (encData.userData[6] != 0) {
                for (j = 0; j < FISHING_SLOT_COUNT; j++) {
                    count = LoadEventEncountPokemonToBuffer(list, count, encData.fishingRare[j].species);
                }
            }
        }
    }
    *result = list[GFL_RandomLCAlt(count)];
    GFL_HeapFree(list);
    GFL_ArcToolFree(arc);
    return FALSE;
}
