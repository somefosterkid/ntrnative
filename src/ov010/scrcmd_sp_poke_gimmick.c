// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/scrcmd_sp_poke_gimmick.c, ported by tools/port_file.py.
// Script commands of the special Pokémon: overlay 129's events for them (sp_poke_gimmick.c), finding one given at an
// event in the party, and the roaming Pokémon. The name is a guess after overlay 129's file. s020D_PokePartyFindEx is
// swan's name (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "field/field.h"
#include "field/field_script.h"
#include "field/sp_poke_event.h"
#include "field/scrcmd_sp_poke_gimmick.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "pml/met_data.h"
#include "pml/poke_party.h"
#include "save/encounter.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/vm.h"

// Any form
#define FORM_ANY 0xff

// The center of a grid square, in fx32, and a height in half squares
#define GRID_CENTER(n) ((n) * FX32_CONST(16) + FX32_CONST(8))
#define GRID_HEIGHT(n) ((n) * FX32_CONST(8))

BOOL func_ov010_0216be20(PokeParty *party, u16 species, u16 move, u32 kind, u16 *slot);
BOOL func_0202f488(PartyPkm *pkm, u32 kind);
void func_0202f630(PartyPkm *pkm, u32 kind);

BOOL func_ov010_0216bbd4(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    Field *field = GSYS_GetField(gsys);
    u16 mode = ScriptReadAny(vm, env);
    u16 x = ScriptReadAny(vm, env);
    u16 y = ScriptReadAny(vm, env);
    u16 z = ScriptReadAny(vm, env);
    u16 height = ScriptReadAny(vm, env);
    u16 a5 = ScriptReadAny(vm, env);
    VecFx32 playerPos;
    VecFx32 pos;
    VecFx32 *start;
    VecFx32 *end;
    GameEvent *event;

    FieldPlayer_GetWPos(Field_GetPlayer(field), &playerPos);
    pos.x = GRID_CENTER(x);
    pos.y = GRID_HEIGHT(y);
    pos.z = GRID_CENTER(z);
    if (mode == 0) {
        start = &playerPos;
        end = &pos;
    } else {
        start = &pos;
        end = &playerPos;
    }
    event = func_ov059_021f39b8(gsys, mode, start, end, GRID_HEIGHT(height), a5);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL func_ov010_0216bc88(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u16 a1 = ScriptReadAny(vm, env);
    u16 x = ScriptReadAny(vm, env);
    u16 y = ScriptReadAny(vm, env);
    u16 z = ScriptReadAny(vm, env);
    VecFx32 pos;

    pos.x = GRID_CENTER(x);
    pos.y = GRID_HEIGHT(y);
    pos.z = GRID_CENTER(z);
    func_ov059_021f3aa0(gsys, a1, &pos);
    return FALSE;
}

BOOL func_ov010_0216bce4(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameEvent *event = func_ov059_021f3b68(gsys, ScriptReadAny(vm, env));

    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL func_ov010_0216bd1c(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameEvent *event = func_ov059_021f3b40(gsys, ScriptReadAny(vm, env));

    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL s020D_PokePartyFindEx(VM *vm, FieldScriptEnv *env) {
    PokeParty *party = GameData_GetParty(FieldScriptEnv_GetGameData(env));
    u16 species = ScriptReadAny(vm, env);
    u16 checkMove = ScriptReadAny(vm, env);
    u16 *slot = ScriptReadVar(vm, env);
    u16 *found = ScriptReadVar(vm, env);
    u32 kind;
    u16 move;

    switch (species) {
    case 0x287:
        kind = 4;
        move = 0x224;
        break;
    case 0x288:
        kind = 5;
        move = 0x223;
        break;
    case 0xfb:
        kind = 0;
        move = 0;
        break;
    case 0xf3:
    case 0xf4:
    case 0xf5:
        kind = 2;
        move = 0;
        break;
    case 0x289:
        kind = 6;
        move = 0;
        break;
    case 0x1ec:
        kind = 7;
        move = 0;
        break;
    default:
        *found = 0;
        return FALSE;
    }
    if (checkMove == 0) {
        move = 0;
    }
    *found = func_ov010_0216be20(party, species, move, kind, slot);
    return FALSE;
}

BOOL func_ov010_0216be20(PokeParty *party, u16 species, u16 move, u32 kind, u16 *slot) {
    int i;
    BOOL found;
    int count;

    count = PokeParty_GetPkmCount(party);
    found = FALSE;
    *slot = 0;
    for (i = 0; i < count; i++) {
        PartyPkm *pkm = PokeParty_GetPkm(party, i);
        if (func_0202f488(pkm, kind) != 0 && PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL) == species) {
            if (move == 0 || (move != PokeParty_GetParam(pkm, PKM_PARAM_MOVE1, NULL)
                && move != PokeParty_GetParam(pkm, PKM_PARAM_MOVE1 + 1, NULL)
                && move != PokeParty_GetParam(pkm, PKM_PARAM_MOVE1 + 2, NULL)
                && move != PokeParty_GetParam(pkm, PKM_PARAM_MOVE1 + 3, NULL))) {
                found = TRUE;
                *slot = i;
                break;
            }
        }
    }
    return found;
}


BOOL func_ov010_0216bec0(VM *vm, FieldScriptEnv *env) {
    PokeParty *party = GameData_GetParty(FieldScriptEnv_GetGameData(env));
    u16 species = ScriptReadAny(vm, env);
    u16 slot = ScriptReadAny(vm, env);
    u32 kind;
    PartyPkm *pkm;

    switch (species) {
    case SPECIES_CELEBI:
        kind = 0;
        break;
    case SPECIES_RAIKOU:
    case SPECIES_ENTEI:
    case SPECIES_SUICUNE:
        kind = 2;
        break;
    default:
        return FALSE;
    }
    pkm = PokeParty_GetPkm(party, slot);
    if (func_0202f488(pkm, kind) == 0) {
        return FALSE;
    }
    func_0202f630(pkm, kind);
    return FALSE;
}


BOOL func_ov010_0216bf38(VM *vm, FieldScriptEnv *env) {
    EncountSave *encountSave = SaveControl_GetEncountSave(GameData_GetSaveControl(FieldScriptEnv_GetGameData(env)));
    u8 index = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);

    *result = func_0200d468(encountSave, index);
    return FALSE;
}
