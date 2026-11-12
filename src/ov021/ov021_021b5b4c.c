// Black's code at ov021 0x021b5b4c-0x021b66b8. No name is known: the module and address.

#include "types.h"
#include "field/event_field_trade.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/field_script.h"
#include "field/fld_trade.h"
#include "gfl/random.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/save_control.h"
#include "save/traded_pokemon.h"
#include "save/trainer_game_info.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/wordset.h"
#include "field/scrcmd_pokemon.h"
#include "save/pokedex.h"
#include "field/event_egg_demo.h"
#include "field/event_poke_status.h"
#include "gfl/str.h"

u32 CheckPartyAllMoveTutor(VM *param_1, FieldScriptEnv *param_2, u32 param_3, u8 *param_4, s32 param_5);

u32 func_ov021_021b5b4c(VM *param_1, FieldScriptEnv *param_2) {
    u16 *ptr;
    u16 *ptr2;
    ScriptWork *work;
    GameSystem *pGVar3;
    GameEvent *event;

    ptr = ScriptReadVar(param_1, param_2);
    ptr2 = ScriptReadVar(param_1, param_2);
    ScriptReadAny(param_1, param_2);
    work = FieldScriptEnv_GetScriptWork(param_2);
    pGVar3 = FieldScriptEnv_GetGameSystem(param_2);
    event = func_ov010_0215a07c(pGVar3, ptr, ptr2);
    ScriptWork_CallEvent(work, event);
    return 1;
}

u32 func_ov021_021b5b8c(VM *param_1, FieldScriptEnv *param_2) {
    PartyPkm *pkm;
    GameData *gameData;
    TrainerCardSave *info;
    u16 species;
    u16 form;
    u16 unk4c;
    u16 unk6e;

    if (CheckGetPartyPokemon(param_2, ScriptReadAny(param_1, param_2), &pkm) == 1) {
        gameData = FieldScriptEnv_GetGameData(param_2);
        info = getTrainerCardDataBlkAddress(gameData);
        species = PokeParty_GetParam(pkm, 5, NULL);
        form = PokeParty_GetParam(pkm, 0x6f, NULL);
        unk4c = PokeParty_GetParam(pkm, 0x4c, NULL);
        unk6e = PokeParty_GetParam(pkm, 0x6e, NULL);
        SetFavePokemonData(info, species, form, unk4c, unk6e);
    }
    return 0;
}

u32 func_ov021_021b5bfc(VM *param_1, FieldScriptEnv *param_2) {
    GameData *gameData;
    PokeParty *party;
    u16 species[6];
    u32 gym;
    s32 i, j, slot;
    PartyPkm *pkm;

    gameData = FieldScriptEnv_GetGameData(param_2);
    party = GameData_GetParty(gameData);
    gym = ScriptReadAny(param_1, param_2);
    for (j = 0; j < 6; j++) {
        species[j] = 0;
    }
    for (i = 0, slot = 0; i < PokeParty_GetPkmCount(party); i++) {
        pkm = PokeParty_GetPkm(party, i);
        if (PokeParty_GetParam(pkm, 0x4c, NULL) == 0) {
            species[slot++] = PokeParty_GetParam(pkm, 5, NULL);
        }
    }
    SetGymDefeatPartyData(getTrainerCardDataBlkAddress(gameData), gym, species);
    return 0;
}



u32 func_ov021_021b5c80(VM *param_1, FieldScriptEnv *param_2) {
    u16 gym;
    GameData *gameData;
    ScriptWork *work;
    WordSet *wordSet;
    u16 *origIndex;
    TrainerCardSave *info;
    u32 species;
    s32 i;
    u32 index;
    u16 auStack_20 [6];

    gameData = FieldScriptEnv_GetGameData(param_2);
    work = FieldScriptEnv_GetScriptWork(param_2);
    wordSet = ScriptWork_GetWordSet(work);
    gym = ScriptReadAny(param_1, param_2);
    origIndex = ScriptReadVar(param_1, param_2);
    info = getTrainerCardDataBlkAddress(gameData);
    GetGymDefeatPartyData(info, (u32)gym, auStack_20);
    i = 0;
    index = 0;
    do {
        species = (u32)auStack_20[i];
        if (species != 0) {
            WordSet_LoadSpeciesName(wordSet, index, species);
            index = index + 1;
        }
        i = i + 1;
    } while (i < 6);
    *origIndex = (u16)index;
    return 0;
}

u32 func_ov021_021b5ce8(VM *param_1, FieldScriptEnv *param_2) {
    u32 index;
    u32 type;
    u16 *out;
    PartyPkm *pkm;
    u16 stats[6];
    u16 sum;
    u16 max;
    u32 i;

    index = ScriptReadAny(param_1, param_2);
    type = ScriptReadAny(param_1, param_2);
    out = ScriptReadVar(param_1, param_2);
    if (CheckGetPartyPokemon(param_2, index, &pkm) == 0) {
        *out = 0;
        return 0;
    }
    stats[0] = PokeParty_GetParam(pkm, 0x46, NULL);
    stats[1] = PokeParty_GetParam(pkm, 0x47, NULL);
    stats[2] = PokeParty_GetParam(pkm, 0x48, NULL);
    stats[3] = PokeParty_GetParam(pkm, 0x49, NULL);
    stats[4] = PokeParty_GetParam(pkm, 0x4a, NULL);
    stats[5] = PokeParty_GetParam(pkm, 0x4b, NULL);
    sum = 0;
    max = 0;
    for (i = 0; i < 6; i++) {
        sum += stats[i];
        if (max < stats[i]) {
            max = stats[i];
        }
    }
    switch (type) {
    case 0:
        *out = sum;
        break;
    case 1:
        *out = max;
        break;
    case 2:
        *out = stats[0];
        break;
    case 3:
        *out = stats[1];
        break;
    case 4:
        *out = stats[2];
        break;
    case 5:
        *out = stats[3];
        break;
    case 6:
        *out = stats[4];
        break;
    case 7:
        *out = stats[5];
        break;
    }
    return 0;
}


u16 func_ov021_021b5dd0(const u16 *param_1, s32 param_2) {
    long i;

    for (i = 0; ; i++) {
        if (param_1[i * 2] == 0) {
            return param_1[i * 2 + 1];
        }
        if (param_2 <= param_1[i * 2]) {
            return param_1[i * 2 + 1];
        }
    }
}


const u16 data_ov021_021d9904[] = {
#include "asset/ov021_021b5b4c_data_ov021_021d9904.inc"
};

const u16 data_ov021_021d98dc[] = {
#include "asset/ov021_021b5b4c_data_ov021_021d98dc.inc"
};

const u16 data_ov021_021d992c[] = {
#include "asset/ov021_021b5b4c_data_ov021_021d992c.inc"
};

u32 func_ov021_021b5df8(VM *param_1, FieldScriptEnv *param_2) {
    u32 n;
    u16 *dest;
    u16 *nDest;
    HeapID heapId;
    PokeDexSave *pokedex;

    n = ScriptReadAny(param_1, param_2);
    dest = ScriptReadVar(param_1, param_2);
    nDest = ScriptReadVar(param_1, param_2);
    heapId = FieldScriptEnv_GetHeapID(param_2);
    pokedex = GameData_GetPokedex(FieldScriptEnv_GetGameData(param_2));
    PokeDex_IsNationalObtained(pokedex);
    *dest = 5;
    *nDest = 0;
    switch (n) {
    case 3:
        n = func_0200cf54(pokedex, heapId);
        if (PokeDex_IsCompleteUnova(pokedex, heapId) == 1) {
            *dest = 0x1e;
        } else {
            *dest = func_ov021_021b5dd0(data_ov021_021d98dc, n);
        }
        break;
    case 2:
        n = func_0200d0f8(pokedex, heapId);
        if (PokeDex_IsSeenAllUnova(pokedex, heapId) == 1) {
            *dest = 0xd;
        } else {
            *dest = func_ov021_021b5dd0(data_ov021_021d9904, n);
        }
        break;
    case 1:
        n = func_0200cf08(pokedex, heapId);
        if (PokeDex_IsCompleteNational(pokedex) == 1) {
            *dest = 0x32;
        } else {
            *dest = func_ov021_021b5dd0(data_ov021_021d992c, n);
        }
        break;
    default:
        return 0;
    }
    *nDest = n;
    return 0;
}

u32 func_ov021_021b5eb8(VM *param_1, FieldScriptEnv *param_2) {
    PokeDexSave *pokedex;
    Field *field;
    u16 *out;
    u32 type;

    pokedex = GameData_GetPokedex(FieldScriptEnv_GetGameData(param_2));
    field = GSYS_GetField(FieldScriptEnv_GetGameSystem(param_2));
    out = ScriptReadVar(param_1, param_2);
    type = ScriptReadAny(param_1, param_2);
    switch (type) {
    case 1:
        *out = PokeDex_IsCompleteNational(pokedex);
        break;
    case 3:
        *out = PokeDex_IsCompleteUnova(pokedex, Field_GetHeapID(field));
        break;
    case 2:
        *out = PokeDex_IsSeenAllUnova(pokedex, Field_GetHeapID(field));
        break;
    }
    return 0;
}

u16 PokeParty_GetTutorMoveID(PartyPkm *param_1, u32 param_2) {
    switch (param_2) {
    case 1:
        if (PokeParty_CheckMoveTutorNormal(param_1, 3) != FALSE) {
            return 0x152;
        }
        if (PokeParty_CheckMoveTutorNormal(param_1, 4) != FALSE) {
            return 0x133;
        }
        if (PokeParty_CheckMoveTutorNormal(param_1, 5) != FALSE) {
            return 0x134;
        }
        break;
    case 0:
        return 0x1b2;
    case 2:
        if (PokeParty_CheckMoveTutorNormal(param_1, 0) != FALSE) {
            return 0x208;
        }
        if (PokeParty_CheckMoveTutorNormal(param_1, 1) != FALSE) {
            return 0x207;
        }
        if (PokeParty_CheckMoveTutorNormal(param_1, 2) != FALSE) {
            return 0x206;
        }
        break;
    }
    return 0x133;
}





u32 PML_PkmGetHapinessMax(u32 param_1) {
    return 0xff;
}

BOOL PokeParty_CheckTutorMoveCompat(PartyPkm *param_1, u32 param_2) {
    switch (param_2) {
    case 1:
        if (PokeParty_CheckMoveTutorNormal(param_1, 3) != FALSE) {
            return TRUE;
        }
        if (PokeParty_CheckMoveTutorNormal(param_1, 4) != FALSE) {
            return TRUE;
        }
        if (PokeParty_CheckMoveTutorNormal(param_1, 5) != FALSE) {
            return TRUE;
        }
        return FALSE;
    case 0:
        if (PokeParty_CheckMoveTutorNormal(param_1, 6) != FALSE) {
            return TRUE;
        }
        return FALSE;
    case 2:
        if (PokeParty_CheckMoveTutorNormal(param_1, 0) != FALSE) {
            return TRUE;
        }
        if (PokeParty_CheckMoveTutorNormal(param_1, 1) != FALSE) {
            return TRUE;
        }
        if (PokeParty_CheckMoveTutorNormal(param_1, 2) == FALSE) {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

u32 CheckPartyAllMoveTutor(VM *param_1, FieldScriptEnv *param_2, u32 param_3, u8 *param_4, s32 param_5) {
    GameData *gameData;
    PokeParty *party;
    PartyPkm *pkm;
    s32 count;
    s32 i;
    s32 found;
    s32 ok;

    gameData = FieldScriptEnv_GetGameData(param_2);
    party = GameData_GetParty(gameData);
    count = PokeParty_GetPkmCount(party);
    found = 0;
    ok = 0;
    *param_4 = 0;
    for (i = 0; i < count; i++) {
        pkm = PokeParty_GetPkm(party, i);
        if (PokeParty_GetParam(pkm, 0x4c, NULL) == 1) {
            continue;
        }
        if (PokeParty_CheckTutorMoveCompat(pkm, param_3) == FALSE) {
            continue;
        }
        found = 1;
        if (param_5 != 0) {
            if (PokeParty_GetParam(pkm, 9, NULL) < PML_PkmGetHapinessMax(param_3)) {
                continue;
            }
        }
        ok = 1;
        *param_4 |= 1 << i;
    }
    if (found == 0) {
        return 1;
    }
    if (ok == 0) {
        return 2;
    }
    return 0;
}



u32 func_ov021_021b60f4(VM *param_1, FieldScriptEnv *param_2) {
    u16 result;
    u16 *ptr;
    u8 abStack_1c [4];

    result = ScriptReadAny(param_1, param_2);
    ptr = ScriptReadVar(param_1, param_2);
    *ptr = (u16)CheckPartyAllMoveTutor(param_1, param_2, (u32)result, abStack_1c, 1);
    return 0;
}

u32 func_ov021_021b6124(VM *param_1, FieldScriptEnv *param_2) {
    PartyPkm *pkm;
    u32 type;
    u32 index;
    u16 *out;
    u32 value;

    type = ScriptReadAny(param_1, param_2);
    index = ScriptReadAny(param_1, param_2);
    out = ScriptReadVar(param_1, param_2);
    if (CheckGetPartyPokemon(param_2, index, &pkm) == TRUE) {
        value = PokeParty_GetTutorMoveID(pkm, type);
    } else {
        value = 0;
    }
    *out = value;
    return 0;
}

u32 func_ov021_021b6164(VM *param_1, FieldScriptEnv *param_2) {
    PartyPkm *pkm;
    u32 type;
    u32 index;
    u16 *out;
    u16 move;

    type = ScriptReadAny(param_1, param_2);
    index = ScriptReadAny(param_1, param_2);
    out = ScriptReadVar(param_1, param_2);
    *out = 1;
    if (CheckGetPartyPokemon(param_2, index, &pkm) == FALSE) {
        return 0;
    }
    if (PokeParty_CheckTutorMoveCompat(pkm, type) == FALSE) {
        *out = 1;
    } else {
        move = PokeParty_GetTutorMoveID(pkm, type);
        if (CheckPokeMoveLearned_NonEgg(pkm, move) == TRUE) {
            *out = 3;
        } else if (PokeParty_GetParam(pkm, 9, NULL) < PML_PkmGetHapinessMax(type)) {
            *out = 2;
        } else {
            *out = 0;
        }
    }
    return 0;
}

u32 func_ov021_021b61e8(VM *param_1, FieldScriptEnv *param_2) {
    u32 type;
    u16 *resultVar;
    u16 *argVar;
    ScriptWork *work;
    GameSystem *gsys;
    GameEvent *event;
    u8 bits;
    u16 eventId;

    type = ScriptReadAny(param_1, param_2);
    resultVar = ScriptReadVar(param_1, param_2);
    argVar = ScriptReadVar(param_1, param_2);
    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    if (CheckPartyAllMoveTutor(param_1, param_2, type, &bits, 0) != 0) {
        *resultVar = 0;
    } else {
        switch (type) {
        case 1:
            eventId = 0xfffe;
            break;
        case 2:
            eventId = 0xfffd;
            break;
        case 0:
            eventId = 0x1b2;
            break;
        default:
            eventId = 0xa5;
            break;
        }
        event = func_ov010_0215a0e8(gsys, resultVar, argVar, bits, eventId);
        ScriptWork_CallEvent(work, event);
    }
    return 1;
}

u32 func_ov021_021b627c(VM *param_1, FieldScriptEnv *param_2) {
    u16 *out;
    GameData *gameData;
    PokeParty *party;
    PartyPkm *pkm;
    u32 count;
    s32 i;

    out = ScriptReadVar(param_1, param_2);
    *out = 0;
    gameData = FieldScriptEnv_GetGameData(param_2);
    party = GameData_GetParty(gameData);
    count = PokeParty_GetPkmCount(party);
    for (i = 0; i < count; i++) {
        pkm = PokeParty_GetPkm(party, i);
        if (PokeParty_GetParam(pkm, 0x97, NULL) != 0) {
            *out = 1;
            break;
        }
    }
    return 0;
}

u32 func_ov021_021b62c8(VM *param_1, FieldScriptEnv *param_2) {
    u16 *out;
    u32 type;
    u8 tm;
    PartyPkm *pkm;

    out = ScriptReadVar(param_1, param_2);
    type = ScriptReadAny(param_1, param_2);
    tm = PML_ItemGetTMBitMask(0x151);
    if (CheckGetPartyPokemon(param_2, type, &pkm) == FALSE) {
        *out = 0xffff;
    } else if (canPkmLearnTM_Wrapper(pkm, tm) == FALSE) {
        *out = 0xffff;
    } else {
        *out = getHiddenPowerType(pkm);
    }
    return 0;
}






u32 func_ov021_021b631c(VM *param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    GameSystem *pGVar3;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(param_2);
    pGVar3 = FieldScriptEnv_GetGameSystem(param_2);
    event = prepareForPokemonNameEntryData(pGVar3, ScriptReadVar(param_1, param_2), ScriptReadAny(param_1, param_2),
                                           ScriptReadAny(param_1, param_2));
    ScriptWork_CallEvent(work, event);
    return 1;
}

u32 func_ov021_021b636c(VM *param_1, FieldScriptEnv *param_2) {
    u16 *outA;
    u16 typeA;
    u16 *outB;
    u16 typeB;
    ScriptWork *work;
    GameSystem *gsys;
    Field *field;
    GameEvent *event;
    s32 flag;

    outA = ScriptReadVar(param_1, param_2);
    outB = ScriptReadVar(param_1, param_2);
    typeA = ScriptReadAny(param_1, param_2);
    typeB = ScriptReadAny(param_1, param_2);
    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    field = GSYS_GetField(gsys);
    flag = (typeB == 0) ? TRUE : FALSE;
    event = func_ov010_0215a2e4(gsys, field, typeA, outA, outB, flag, typeB);
    ScriptWork_CallEvent(work, event);
    return 1;
}


u32 func_ov021_021b63d4(VM *param_1, FieldScriptEnv *param_2) {
    u16 *origIndex;
    ScriptWork *work;
    GameSystem *gsys;
    Field *field;
    GameData *gameData;
    PokeParty *party;
    s32 pkmCount;
    PartyPkm *pkm;
    u32 result;
    u32 result2;
    u32 result3;
    GameEvent *event;
    u32 index;

    origIndex = ScriptReadVar(param_1, param_2);
    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    field = GSYS_GetField(gsys);
    Field_GetHeapID(field);
    gameData = GSYS_GetGameData(gsys);
    GetGameDataPlayerInfo(gameData);
    party = GameData_GetParty(gameData);
    pkmCount = PokeParty_GetPkmCount(party);
    index = 0;
    if (0 < pkmCount) {
        do {
            pkm = PokeParty_GetPkm(party, index);
            result = PokeParty_GetParam(pkm, 0x4c, (void *)0x0);
            result2 = PokeParty_GetParam(pkm, 3, (void *)0x0);
            result3 = PokeParty_GetParam(pkm, 9, (void *)0x0);
            if (((result != 0) && (result2 == 0)) && (result3 == 0)) {
                event = func_ov010_02166c4c(gsys, field, pkm);
                ScriptWork_CallEvent(work, event);
                *origIndex = (u16)index;
                return 1;
            }
            index = index + 1;
        } while ((s32)index < pkmCount);
    }
    return 0;
}

u32 func_ov021_021b647c(VM *param_1, FieldScriptEnv *param_2) {
    u16 heapId;
    u16 offerIndex;
    u16 index;
    GameSystem *gsys;
    GameData *gameData;
    Field *field;
    u16 *ptr;
    PokeParty *party;
    PartyPkm *pkm;
    FieldTradeInput *input;
    u32 wantedSex;
    u32 wantedSpecies;

    FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    gameData = GSYS_GetGameData(gsys);
    field = GSYS_GetField(gsys);
    heapId = Field_GetHeapID(field);
    ptr = ScriptReadVar(param_1, param_2);
    offerIndex = ScriptReadAny(param_1, param_2);
    index = ScriptReadAny(param_1, param_2);
    party = GameData_GetParty(gameData);
    pkm = PokeParty_GetPkm(party, (u32)index);
    input = FieldTradeInput_Create((u32)heapId, (u32)offerIndex);
    *ptr = 0;
    if ((*ptr == 0) && (wantedSex = PokeParty_GetParam(pkm, 0x4c, (void *)0x0), wantedSex == 1)) {
        *ptr = 1;
    }
    if (*ptr == 0) {
        wantedSex = PokeParty_GetParam(pkm, 5, (void *)0x0);
        wantedSpecies = FieldTradeInput_GetWantedSpecies(input);
        if (wantedSex != wantedSpecies) {
            *ptr = 2;
        }
    }
    if ((*ptr == 0) && (wantedSex = FieldTradeInput_GetWantedSex(input), wantedSex != 2)) {
        wantedSex = PokeParty_GetParam(pkm, 0x6e, (void *)0x0);
        wantedSpecies = FieldTradeInput_GetWantedSex(input);
        if (wantedSex != wantedSpecies) {
            *ptr = 3;
        }
    }
    FieldTradeInput_Free(input);
    return 0;
}

u32 func_ov021_021b654c(VM *param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    GameSystem *gsys;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    event = EventFieldTrade_Create(gsys, (u8)ScriptReadAny(param_1, param_2), (u8)ScriptReadAny(param_1, param_2));
    ScriptWork_CallEvent(work, event);
    return 1;
}

u32 func_ov021_021b6590(VM *param_1, FieldScriptEnv *param_2) {
    u16 result;
    u16 *ptr;
    GameData *gameData;
    BagSave *bag;
    u32 max;
    u32 index;
    BagItem *pBVar4;

    ptr = ScriptReadVar(param_1, param_2);
    gameData = FieldScriptEnv_GetGameData(param_2);
    bag = GameData_GetBag(gameData);
    max = BagSave_GetPocketItemCount(bag, 2);
    index = GFL_RandomLC(max);
    if (max == 0) {
        result = 0;
    }
    else {
        pBVar4 = BagSave_GetItemIndexHandle(bag, 2, (u16)index);
        result = PML_ItemGetTMWazaID(pBVar4->item);
    }
    *ptr = result;
    return 0;
}

u32 func_ov021_021b65d8(VM *param_1, FieldScriptEnv *param_2) {
    u16 index;
    u16 index2;
    GameSystem *gsys;
    GameData *gameData;
    TradedPokemonSave *block;
    PokeParty *party;
    PartyPkm *pkm;

    FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    gameData = GSYS_GetGameData(gsys);
    block = GetTradedPokemonBlock(gameData);
    party = GameData_GetParty(gameData);
    index = ScriptReadAny(param_1, param_2);
    index2 = ScriptReadAny(param_1, param_2);
    pkm = PokeParty_GetPkm(party, (u32)index);
    SetTradedPokemonData(block, (u32)index2, pkm);
    return 0;
}

u32 func_ov021_021b6628(VM *param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    GameSystem *gsys;
    GameData *gameData;
    TradedPokemonSave *block;
    StrBuf *strbuf;
    WordSet *wordSet;
    u16 *src;
    s16 buf;
    u32 index;
    u32 word;

    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    gameData = GSYS_GetGameData(gsys);
    block = GetTradedPokemonBlock(gameData);
    index = ScriptReadAny(param_1, param_2);
    word = ScriptReadAny(param_1, param_2);
    strbuf = ScriptWork_GetAltStrBuf(work);
    wordSet = ScriptWork_GetWordSet(work);
    if (func_0200e740(block, index) != FALSE) {
        GFL_StrBufLoadString(strbuf, func_0200e7ac(block, index));
    } else {
        src = &buf;
        *src = 0;
        buf = GFL_StrBufGetTerminator();
        GFL_StrBufLoadString(strbuf, src);
    }
    func_0201ee0c(wordSet, word, strbuf, 0, 1, 2);
    return 0;
}

