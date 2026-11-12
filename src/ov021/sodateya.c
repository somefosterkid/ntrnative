// Black's code at ov021 0x021c2f84-0x021c457c. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field.h"
#include "field/field_daycare.h"
#include "field/field_event.h"
#include "gfl/heap.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "pml/species_names.h"
#include "save/box.h"
#include "save/chatter.h"
#include "save/event_work.h"
#include "save/player_info.h"
#include "save/pokedex.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "field/game_beacon_set.h"
#include "gfl/arc.h"
#include "constants/species.h"
#include "pml/item.h"
#include "gfl/str.h"
#include "gfl/msg.h"
#include "pml/met_data.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "system/wordset.h"
#include "system/str_tool.h"
#include "system/gf_font.h"
#include "gfl/arc_util.h"
#include "gfl/graphics.h"
#include "gfl/sound.h"

// Default parameter tables copied into the local work arrays used by move and IV inheritance (pokebw2's)
typedef struct {
    u32 values[4];
} DayCareMoveParamTable;
typedef struct {
    u32 values[6];
} DayCareStatParamTable;
typedef struct {
    u32 values[12][3];
} DayCareIncenseTable;

// pokebw2's sOpenSlotMoveParams
static const DayCareMoveParamTable data_ov021_021da8c8;
// pokebw2's sExistingMoveParams
static const DayCareMoveParamTable data_ov021_021da8d8;
// pokebw2's sIncenses
static const DayCareIncenseTable data_ov021_021da960;
// pokebw2's sPowerItems
static const DayCareStatParamTable data_ov021_021da930;
// pokebw2's sInheritedIVParams
static const DayCareStatParamTable data_ov021_021da918;
// pokebw2's sEggMoveParams
static const DayCareMoveParamTable data_ov021_021da908;
// pokebw2's sLevelMoveParams
static const DayCareMoveParamTable data_ov021_021da8f8;
// pokebw2's sTmMoveParams
static const DayCareMoveParamTable data_ov021_021da8e8;
// pokebw2's sEggIVParams
static const DayCareStatParamTable data_ov021_021da948;


// The two parents (pokebw2's EggParents)
typedef struct EggParents {
    PartyPkm *parents[2];
} EggParents;

// The egg's data as it is worked out (pokebw2's EggData, and the ability at 0x10 that Black 2's lacks)
typedef struct EggData {
    u32 species;
    u32 form;
    u32 level;
    u32 nature;
    u32 ability;
    u32 happiness;
    u32 ivs[6];
    u32 trainerId;
    u32 pid;
    u32 moveCount;
    u32 moves[4];
    u32 abilitySlot;
    u32 natureFromParent1;
    u32 natureFromParent2;
    u32 ivFromParent1[6];
    u32 ivFromParent2[6];
    u32 inheritedIvCount;
} EggData;

void func_ov021_021c38f4(HeapID heapId, PlayerInfo *info, u16 place, PartyPkm *pkm1, PartyPkm *pkm2, PartyPkm *out);
void swapDaycarePKMPointers(EggParents *parents);
PartyPkm *checkSecondPokemonDitto(EggParents *parents);
u32 validateSpeciesAndGetOffspring(u32 param_1);
PartyPkm *checkSecondPokemonDitto2(EggParents *parents);
void PushEggPkmMove(EggData *egg, s32 move);
void setPartyFullToConst(Field *param_1);
u32 func_ov021_021c36ec(PartyPkm *param_1, PartyPkm *param_2);
void DayCare_RecoverPkm(PartyPkm *param_1);
void shayminTransform(PartyPkm *param_1);
void updateChatotPartyStatus(Field *param_1, PokeParty *param_2);
void DayCare_CommitPkmGrowth(PartyPkm *param_1, u32 param_2);
void DayCare_SyncPokedex(DayCare *dayCare, PartyPkm *param_2);
void DayCareSave_ShiftPkms(DayCareSave *param_1);
void DayCare_CreateChildPkm(DayCare *param_1, PartyPkm *param_2);
u32 DayCare_CalcNewExp(u32 param_1, u32 param_2);
u32 func_ov021_021c4078(void);
u32 DayCare_CheckEggSpawn(DayCare *param_1);
void setPartyHasSpaceToConst(Field *param_1);
BOOL func_ov021_021c3670(PartyPkm *param_1, u32 param_2);
u32 func_ov021_021c447c(HeapID heapId, u16 species, u16 form, u16 tm);
u8 func_ov021_021c452c(u16 species, u16 form, u16 value, HeapID heapId);
void writeLvUpMoveOpenSlot(PartyPkm *param_1, u32 param_2);
u32 DayCare_CalcBaseEggSpawnChanceCore(PartyPkm *param_1, PartyPkm *param_2);
void func_ov021_021c3a00(EggParents *parents);
void func_ov021_021c3a44(EggParents *parents);
void func_ov021_021c3a7c(EggData *egg);
void func_ov021_021c3a90(EggParents *parents, EggData *egg);
void func_ov021_021c3aac(EggParents *parents, EggData *egg);
void func_ov021_021c3adc(EggParents *parents, EggData *egg);
void func_ov021_021c3b0c(EggParents *parents, EggData *egg);
void func_ov021_021c3b2c(EggParents *parents, EggData *egg);
void func_ov021_021c3bcc(EggParents *parents, EggData *egg);
void func_ov021_021c3bd4(EggParents *parents, EggData *egg);
void func_ov021_021c3c04(EggParents *parents, EggData *egg);
void func_ov021_021c3c30(EggParents *parents, EggData *egg);
void func_ov021_021c3c78(EggParents *parents, EggData *egg);
void func_ov021_021c3c88(EggParents *parents, EggData *egg);
void func_ov021_021c3d1c(const EggParents *parents, EggData *egg, HeapID heapId);
void func_ov021_021c3da8(EggParents *parents, EggData *egg);
void func_ov021_021c3de4(EggParents *parents, EggData *egg, HeapID heapId);
void func_ov021_021c3e38(EggData *egg, HeapID heapId);
void func_ov021_021c3e64(EggParents *parents, EggData *egg);
void func_ov021_021c3f60(EggParents *parents, EggData *egg);
void func_ov021_021c3fc0(EggParents *parents, EggData *egg);
void func_ov021_021c4018(PlayerInfo *info, EggData *egg);
void func_ov021_021c4024(EggParents *parents, EggData *egg);
void func_ov021_021c4030(EggParents *parents, EggData *egg);
void func_ov021_021c4084(EggData *egg);
void func_ov021_021c40f8(EggParents *parents, EggData *egg, HeapID heapId);
void func_ov021_021c4160(EggParents *parents, EggData *egg);
void func_ov021_021c4238(EggParents *parents, EggData *egg, HeapID heapId);
void func_ov021_021c42bc(EggParents *parents, EggData *egg);
void func_ov021_021c4340(PartyPkm *pkm, const EggData *egg, PlayerInfo *info, u32 place, u16 heapId);


DayCare *func_ov021_021c31d4(HeapID heapId, Field *field, DayCareSave *save) {
    DayCare *dayCare;

    dayCare = GFL_HeapAllocate(heapId, sizeof(DayCare), 0, "sodateya.c", 0x6a);
    dayCare->heapId = heapId;
    dayCare->field = field;
    dayCare->save = save;
    dayCare->unkC = 0;
    return dayCare;
}

void func_ov021_021c31fc(DayCare *dayCare) {
    GFL_HeapFree(dayCare);
}

void DayCare_AddPkm(DayCare *dayCare, PokeParty *party, u32 slot) {
    u32 count = DayCareSave_GetPkmCount(dayCare->save);
    PartyPkm *pkm = PokeParty_GetPkm(party, slot);

    if ((s32)count < 2) {
        DayCare_RecoverPkm(pkm);
        shayminTransform(pkm);
        DayCareSave_AddPkm(dayCare->save, count, pkm);
        PokeParty_RemovePkm(party, slot);
        updateChatotPartyStatus(dayCare->field, party);
    }
}


void DayCare_RecoverPkm(PartyPkm *param_1) {
    PokeParty_Recover(param_1);
}



void shayminTransform(PartyPkm *param_1) {
    if (PokeParty_GetParam(param_1, 5, (void *)0x0) == 0x1ec) {
        PokeParty_ChangeForme(param_1, 0);
    }
}

void DayCare_RemovePkm(DayCare *dayCare, u32 slot, PokeParty *party) {
    PartyPkm *pkm;

    if (DayCareSave_GetPkmStatus(dayCare->save, slot) == TRUE) {
        pkm = DayCareSave_GetPkm(dayCare->save, slot);
        DayCare_CommitPkmGrowth(pkm, DayCareSave_GetPkmStepCounter(dayCare->save, slot));
        DayCare_SyncPokedex(dayCare, pkm);
        PokeParty_AddPkm(party, pkm);
        DayCareSave_DisablePkmStatus(dayCare->save, slot);
        DayCareSave_ShiftPkms(dayCare->save);
    }
}


void DayCare_SyncPokedex(DayCare *dayCare, PartyPkm *param_2) {
    GameSystem *gsys;
    GameData *gameData;
    PokeDexSave *pokedex;
    BOOL BVar2;

    gsys = Field_GetGameSystem(dayCare->field);
    gameData = GSYS_GetGameData(gsys);
    pokedex = GameData_GetPokedex(gameData);
    BVar2 = PokeDex_IsCaught(pokedex, (u16)PokeParty_GetParam(param_2, 5, (void *)0x0));
    if (BVar2 == 0) {
        addPkmToDex(pokedex, param_2);
    }
}

void DayCare_Breed(DayCare *dayCare, PokeParty *party) {
    PartyPkm *pkm;
    GameSystem *gsys;
    GameData *gameData;
    u8 season;

    pkm = PokeParty_NewTempPkm(1, 1, 1, dayCare->heapId);
    if (DayCareSave_CheckSpawnFlag(dayCare->save) == TRUE) {
        func_ov010_0216556c();
        setPartyFullToConst(dayCare->field);
        DayCare_CreateChildPkm(dayCare, pkm);
        PokeParty_AddPkm(party, pkm);
        DayCareSave_ResetSeed(dayCare->save);
        GFL_HeapFree(pkm);
        gsys = Field_GetGameSystem(dayCare->field);
        gameData = GSYS_GetGameData(gsys);
        season = GameData_GetSeason(gameData);
        TransformVsPokePartyBySeason(gameData, party, season);
    }
}


void DayCare_CheckResetSeed(DayCare *dayCare) {
    if (DayCareSave_CheckSpawnFlag(dayCare->save) == TRUE) {
        DayCareSave_ResetSeed(dayCare->save);
        setPartyFullToConst(dayCare->field);
    }
}

u32 DayCare_Update(DayCare *param_1) {
    DayCareSave_UpdateStepCount(param_1->save, 1);
    if (DayCare_CheckEggSpawn(param_1) == 1) {
        DayCareSave_SetSpawnFlag(param_1->save);
        setPartyHasSpaceToConst(param_1->field);
        return 1;
    }
    return 0;
}



void setPartyHasSpaceToConst(Field *param_1) {
    GameSystem *gsys;
    GameData *gameData;
    EventWork *eventWork;
    PokeParty *party;
    u16 *wkPtr;

    gsys = Field_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    eventWork = GameData_GetEventWork(gameData);
    party = GameData_GetParty(gameData);
    if (PokeParty_GetPkmCount(party) < PokeParty_GetCapacity(party)) {
        wkPtr = EventWork_GetWkPtr(eventWork, 0x4139);
        *wkPtr = 1;
    }
}

void setPartyFullToConst(Field *param_1) {
    GameSystem *gsys;
    GameData *gameData;
    EventWork *eventWork;
    u16 *wkPtr;

    gsys = Field_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    eventWork = GameData_GetEventWork(gameData);
    wkPtr = EventWork_GetWkPtr(eventWork, 0x4139);
    *wkPtr = 0;
}

BOOL DayCare_CheckSpawnFlag(DayCare *dayCare) {
    return DayCareSave_CheckSpawnFlag(dayCare->save);
}

u8 DayCare_GetPkmCount(DayCare *dayCare) {
    return DayCareSave_GetPkmCount(dayCare->save);
}

u32 DayCare_CalcNewLevel(DayCare *dayCare, u32 slot) {
    PartyPkm *pkm;
    u32 species;
    u32 form;
    u32 exp;
    u32 gain;

    if (DayCareSave_GetPkmStatus(dayCare->save, slot) != TRUE) {
        return 1;
    }
    pkm = DayCareSave_GetPkm(dayCare->save, slot);
    species = PokeParty_GetParam(pkm, 5, NULL);
    form = PokeParty_GetParam(pkm, 0x6f, NULL);
    exp = PokeParty_GetParam(pkm, 8, NULL);
    PokeParty_GetParam(pkm, 0x9e, NULL);
    gain = DayCare_CalcNewExp(exp, DayCareSave_GetPkmStepCounter(dayCare->save, slot));
    return CalcLevelByExp(species, form, gain);
}


u32 DayCare_CalcLevelGain(DayCare *dayCare, u32 slot) {
    PartyPkm *pkm = DayCareSave_GetPkm(dayCare->save, slot);
    u32 level = PokeParty_GetParam(pkm, 0x9e, NULL);
    return DayCare_CalcNewLevel(dayCare, slot) - level;
}




s32 func_ov021_021c34b4(DayCare *param_1, u32 param_2) {
    return (DayCare_CalcLevelGain(param_1, param_2) + 1) * 100;
}

u8 DayCare_CalcEggSpawnChance(DayCare *dayCare) {
    PartyPkm *pkm1;
    PartyPkm *pkm2;

    if ((s32)DayCareSave_GetPkmCount(dayCare->save) < 2) {
        return 0;
    }
    pkm1 = DayCareSave_GetPkm(dayCare->save, 0);
    pkm2 = DayCareSave_GetPkm(dayCare->save, 1);
    return func_ov021_021c36ec(pkm1, pkm2);
}


PartyPkm *DayCare_GetPkm(DayCare *dayCare, u32 slot) {
    DayCareSave_GetPkmStatus(dayCare->save, slot);
    return DayCareSave_GetPkm(dayCare->save, slot);
}

u32 DayCare_CalcNewExp(u32 param_1, u32 param_2) {
    u64 sum = (u64)param_1 + param_2;
    if (sum > 0xFFFFFFFF) {
        return 0xFFFFFFFF;
    }
    return (u32)sum;
}

typedef struct LvUpMove {
    u16 move;
    u16 level;
} LvUpMove;

static inline BOOL LvUpMove_IsEnd(LvUpMove m) {
    return m.move == 0xffff && m.level == 0xffff;
}

static inline LvUpMove LvUpMove_Get(LvUpMove *list, s32 i) {
    return list[i];
}

void DayCare_CommitPkmGrowth(PartyPkm *param_1, u32 param_2) {
    LvUpMove learn[26];
    LvUpMove cur;
    LvUpMove t;
    LvUpMove m;
    u32 move;
    int level;
    u32 species;
    u32 form;
    u32 exp;
    s32 oldLevel;
    s32 newLevel;
    u32 cap;
    s32 k;

    species = PokeParty_GetParam(param_1, 5, NULL);
    form = PokeParty_GetParam(param_1, 0x6f, NULL);
    exp = PML_UtilGetPkmLvExp(species, form, 100);
    oldLevel = PokeParty_GetParam(param_1, 0x9e, NULL);
    cap = DayCare_CalcNewExp(PokeParty_GetParam(param_1, 8, NULL), param_2);
    if (exp < cap) {
        cap = exp;
    }
    PokeParty_SetParam(param_1, 8, cap);
    PokeParty_RecalcStats(param_1);
    newLevel = PokeParty_GetParam(param_1, 0x9e, NULL);
    PML_LearnsetLvUpLoad(species, form, learn);
    for (k = 0; cur = LvUpMove_Get(learn, k), !LvUpMove_IsEnd(cur); k++) {
        t = cur;
        level = t.level;
        m = cur;
        move = m.move;
        if (newLevel < t.level) {
            break;
        }
        if (oldLevel < level) {
            writeLvUpMoveOpenSlot(param_1, move);
        }
    }
}


void writeLvUpMoveOpenSlot(PartyPkm *param_1, u32 param_2) {
    DayCareMoveParamTable params = data_ov021_021da8c8;
    s32 i;

    if (param_2 == 0) {
        return;
    }
    if (func_ov021_021c3670(param_1, param_2)) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (PokeParty_GetParam(param_1, params.values[i], NULL) == 0) {
            PokeParty_SetMove(param_1, param_2, i);
            return;
        }
    }
    PokeParty_SetLastMove(param_1, param_2);
}


BOOL func_ov021_021c3670(PartyPkm *param_1, u32 param_2) {
    DayCareMoveParamTable params = data_ov021_021da8d8;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (param_2 == PokeParty_GetParam(param_1, params.values[i], NULL)) {
            return TRUE;
        }
    }
    return FALSE;
}


void DayCareSave_ShiftPkms(DayCareSave *param_1) {
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        if (DayCareSave_GetPkmStatus(param_1, i) != TRUE) {
            for (j = i + 1; j < 2; j++) {
                if (DayCareSave_GetPkmStatus(param_1, j) == TRUE) {
                    func_0200bcec(param_1, j, j - 1);
                }
            }
        }
    }
}

u32 func_ov021_021c36ec(PartyPkm *param_1, PartyPkm *param_2) {
    u32 species[2];
    u32 form[2];
    u32 unk6e[2];
    u32 p18[2];
    u32 p19[2];
    PartyPkm *pkms[2] = { param_1, param_2 };
    s32 i;

    for (i = 0; i < 2; i++) {
        species[i] = PokeParty_GetParam(pkms[i], 5, NULL);
        PokeParty_GetParam(pkms[i], 7, NULL);
        form[i] = PokeParty_GetParam(pkms[i], 0x6f, NULL);
        unk6e[i] = PokeParty_GetParam(pkms[i], 0x6e, NULL);
        p18[i] = PML_PersonalGetParamSingle(species[i], form[i], 0x18);
        p19[i] = PML_PersonalGetParamSingle(species[i], form[i], 0x19);
    }
    if (p18[0] == 0xf || p18[1] == 0xf || p19[0] == 0xf || p19[1] == 0xf) {
        return 0;
    }
    if (species[0] == 0x84 && species[1] == 0x84) {
        return 0;
    }
    if (species[0] == 0x84 || species[1] == 0x84) {
        return DayCare_CalcBaseEggSpawnChanceCore(param_1, param_2);
    }
    if (unk6e[0] == unk6e[1]) {
        return 0;
    }
    if (unk6e[0] == 2 || unk6e[1] == 2) {
        return 0;
    }
    if (p18[0] != p18[1] && p18[0] != p19[1] && p19[0] != p18[1] && p19[0] != p19[1]) {
        return 0;
    }
    return DayCare_CalcBaseEggSpawnChanceCore(param_1, param_2);
}


u32 DayCare_CalcBaseEggSpawnChanceCore(PartyPkm *param_1, PartyPkm *param_2) {
    u32 species[2];
    u32 unk7[2];
    PartyPkm *pkms[2];
    s32 i;

    pkms[0] = param_1;
    pkms[1] = param_2;
    for (i = 0; i < 2; i++) {
        species[i] = PokeParty_GetParam(pkms[i], 5, NULL);
        unk7[i] = PokeParty_GetParam(pkms[i], 7, NULL);
    }
    if (species[0] != species[1]) {
        if (unk7[0] == unk7[1]) {
            return 0x14;
        }
        return 0x32;
    }
    if (unk7[0] == unk7[1]) {
        return 0x32;
    }
    return 0x46;
}



u32 DayCare_CheckEggSpawn(DayCare *param_1) {
    u16 old = param_1->unkC;

    param_1->unkC = old + 0x10;
    if (old > 0xff) {
        param_1->unkC = 0;
        if ((s32)DayCareSave_GetPkmCount(param_1->save) >= 2) {
            u32 roll = GFL_RandomLCAlt(0x640);
            if (roll < DayCare_CalcEggSpawnChance(param_1)) {
                return 1;
            }
        }
    }
    return 0;
}




void DayCare_CreateChildPkm(DayCare *param_1, PartyPkm *param_2) {
    GameSystem *gsys;
    GameData *gameData;
    PlayerInfo *pPVar1;
    PartyPkm *pPVar2;
    PartyPkm *pPVar3;

    gsys = Field_GetGameSystem(param_1->field);
    gameData = GSYS_GetGameData(gsys);
    pPVar1 = GetGameDataPlayerInfo(gameData);
    pPVar2 = DayCareSave_GetPkm(param_1->save, 0);
    pPVar3 = DayCareSave_GetPkm(param_1->save, 1);
    func_ov021_021c38f4(param_1->heapId, pPVar1, 0xea62, pPVar2, pPVar3, param_2);
}

void updateChatotPartyStatus(Field *param_1, PokeParty *param_2) {
    GameSystem *gsys;
    GameData *gameData;
    PerapVoice *chatter;

    gsys = Field_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    chatter = getChatterDataAddress(gameData);
    checkChatotInParty(chatter, param_2);
}

void func_ov021_021c38f4(HeapID heapId, PlayerInfo *info, u16 place, PartyPkm *pkm1, PartyPkm *pkm2, PartyPkm *out) {
    EggParents parents;
    EggData egg;

    parents.parents[0] = pkm1;
    parents.parents[1] = pkm2;
    func_ov021_021c3a00(&parents);
    func_ov021_021c3a44(&parents);
    func_ov021_021c3a7c(&egg);
    func_ov021_021c3a90(&parents, &egg);
    func_ov021_021c3aac(&parents, &egg);
    func_ov021_021c3adc(&parents, &egg);
    func_ov021_021c3b0c(&parents, &egg);
    func_ov021_021c3b2c(&parents, &egg);
    func_ov021_021c3bcc(&parents, &egg);
    func_ov021_021c3bd4(&parents, &egg);
    func_ov021_021c3c04(&parents, &egg);
    func_ov021_021c3c30(&parents, &egg);
    func_ov021_021c3c78(&parents, &egg);
    func_ov021_021c3c88(&parents, &egg);
    func_ov021_021c3d1c(&parents, &egg, heapId);
    func_ov021_021c3da8(&parents, &egg);
    func_ov021_021c3de4(&parents, &egg, heapId);
    func_ov021_021c3e38(&egg, heapId);
    func_ov021_021c3e64(&parents, &egg);
    func_ov021_021c3f60(&parents, &egg);
    func_ov021_021c3fc0(&parents, &egg);
    func_ov021_021c4018(info, &egg);
    func_ov021_021c4024(&parents, &egg);
    func_ov021_021c4030(&parents, &egg);
    func_ov021_021c4084(&egg);
    func_ov021_021c40f8(&parents, &egg, heapId);
    func_ov021_021c4160(&parents, &egg);
    func_ov021_021c4238(&parents, &egg, heapId);
    func_ov021_021c42bc(&parents, &egg);
    func_ov021_021c4340(out, &egg, info, place, heapId);
}


void func_ov021_021c3a00(EggParents *parents) {
    u32 result;

    result = PokeParty_GetParam(parents->parents[0], 0x6e, (void *)0x0);
    PokeParty_GetParam(parents->parents[1], 0x6e, (void *)0x0);
    if (result == 1) {
        swapDaycarePKMPointers(parents);
    }
    PokeParty_GetParam(parents->parents[0], 0x6e, (void *)0x0);
    result = PokeParty_GetParam(parents->parents[1], 0x6e, (void *)0x0);
    if (result == 0) {
        swapDaycarePKMPointers(parents);
    }
}

void func_ov021_021c3a44(EggParents *parents) {
    u32 result;
    u32 result2;

    result = PokeParty_GetParam(parents->parents[0], 5, (void *)0x0);
    result2 = PokeParty_GetParam(parents->parents[1], 0x6e, (void *)0x0);
    if ((result == 0x84) && (result2 == 2)) {
        swapDaycarePKMPointers(parents);
    }
}

void swapDaycarePKMPointers(EggParents *parents) {
    PartyPkm *pkm;

    pkm = parents->parents[0];
    parents->parents[0] = parents->parents[1];
    parents->parents[1] = pkm;
}

void func_ov021_021c3a7c(EggData *egg) {
    MI_CpuFill8(egg, 0, sizeof(EggData));
    egg->level = 1;
}

void func_ov021_021c3a90(EggParents *parents, EggData *egg) {
    PartyPkm *pkm;

    pkm = checkSecondPokemonDitto(parents);
    egg->species = validateSpeciesAndGetOffspring(PokeParty_GetParam(pkm, 5, (void *)0x0) & 0xffff);
}

void func_ov021_021c3aac(EggParents *parents, EggData *egg) {
    PartyPkm *pkm;
    u32 result;
    pkm = checkSecondPokemonDitto(parents);
    result = PokeParty_GetParam(pkm, 5, (void *)0x0);
    if ((result == 0x1d) || ((result == 0x20 || result == 0x21 || result == 0x22))) {
        result = GFL_RandomLCAlt(2);
        if (result == 0) {
            egg->species = 0x1d;
            return;
        }
        egg->species = 0x20;
    }
}

void func_ov021_021c3adc(EggParents *parents, EggData *egg) {
    PartyPkm *pkm;
    u32 result;
    pkm = checkSecondPokemonDitto(parents);
    result = PokeParty_GetParam(pkm, 5, (void *)0x0);
    if ((result == 0x139 || result == 0x13a)) {
        result = GFL_RandomLCAlt(2);
        if (result == 0) {
            egg->species = 0x139;
            return;
        }
        egg->species = 0x13a;
    }
}

void func_ov021_021c3b0c(EggParents *parents, EggData *egg) {
    PartyPkm *pkm;

    pkm = checkSecondPokemonDitto(parents);
    if (PokeParty_GetParam(pkm, 5, (void *)0x0) == 0x1ea) {
        egg->species = 0x1e9;
    }
}

void func_ov021_021c3b2c(EggParents *parents, EggData *egg) {
    DayCareIncenseTable table = data_ov021_021da960;
    PartyPkm *pkm;
    int a;
    u32 b;
    s32 i;

    pkm = checkSecondPokemonDitto(parents);
    PokeParty_GetParam(pkm, 5, NULL);
    PokeParty_GetParam(pkm, 6, NULL);
    a = PokeParty_GetParam(parents->parents[0], 6, NULL);
    b = PokeParty_GetParam(parents->parents[1], 6, NULL);
    for (i = 0; i < 12; i++) {
        if (egg->species == table.values[i][0] && a != table.values[i][1] && b != table.values[i][1]) {
            egg->species = table.values[i][2];
            return;
        }
    }
}


PartyPkm *checkSecondPokemonDitto(EggParents *parents) {
    if (PokeParty_GetParam(parents->parents[1], 5, NULL) == SPECIES_DITTO) {
        return parents->parents[0];
    }
    return parents->parents[1];
}


void func_ov021_021c3bcc(EggParents *parents, EggData *egg) {
    egg->form = 0;
}

void func_ov021_021c3bd4(EggParents *parents, EggData *egg) {
    PartyPkm *pkm;
    u32 result;
    pkm = checkSecondPokemonDitto2(parents);
    result = PokeParty_GetParam(pkm, 5, (void *)0x0);
    if ((result == 0x1a6 || result == 0x1a7)) {
        result = PokeParty_GetParam(pkm, 0x6f, (void *)0x0);
        egg->form = result;
    }
}

void func_ov021_021c3c04(EggParents *parents, EggData *egg) {
    PartyPkm *pkm;
    u32 result;
    pkm = checkSecondPokemonDitto2(parents);
    result = PokeParty_GetParam(pkm, 5, (void *)0x0);
    if ((result == 0x19c || result == 0x19d)) {
        result = PokeParty_GetParam(pkm, 0x6f, (void *)0x0);
        egg->form = result;
    }
}

void func_ov021_021c3c30(EggParents *parents, EggData *egg) {
    PartyPkm *pkm;
    u32 result;

    pkm = checkSecondPokemonDitto2(parents);
    result = PokeParty_GetParam(pkm, 5, (void *)0x0);
    if (result == 0x226) {
        result = PokeParty_GetParam(pkm, 0x6f, (void *)0x0);
        egg->form = result;
    }
}

PartyPkm *checkSecondPokemonDitto2(EggParents *parents) {
    if (PokeParty_GetParam(parents->parents[1], 5, NULL) == SPECIES_DITTO) {
        return parents->parents[0];
    }
    return parents->parents[1];
}


void func_ov021_021c3c78(EggParents *parents, EggData *egg) {
    egg->nature = GFL_RandomLCAlt(0x19);
}

void func_ov021_021c3c88(EggParents *parents, EggData *egg) {
    s32 f7 = 0;
    u32 v0;
    u32 v5;
    s32 f6;

    if (PokeParty_GetParam(parents->parents[0], 6, NULL) == 0xe5) {
        f7 = 1;
    }
    f6 = 0;
    if (PokeParty_GetParam(parents->parents[1], 6, NULL) == 0xe5) {
        f6 = 1;
    }
    v0 = PokeParty_GetParam(parents->parents[0], 0x70, NULL);
    v5 = PokeParty_GetParam(parents->parents[1], 0x70, NULL);
    if (f7 || f6) {
        if (GFL_RandomLCAlt(2) == 0) {
            return;
        }
    }
    if (f7 && f6) {
        if (GFL_RandomLCAlt(2) == 0) {
            egg->nature = v0;
            egg->natureFromParent1 = 1;
        } else {
            egg->nature = v5;
            egg->natureFromParent2 = 1;
        }
    } else if (f7) {
        egg->nature = v0;
        egg->natureFromParent1 = 1;
    } else if (f6) {
        egg->nature = v5;
        egg->natureFromParent2 = 1;
    }
}


void func_ov021_021c3d1c(const EggParents *parents, EggData *egg, HeapID heapId) {
    u32 species;
    u32 form;
    u32 value;
    u32 kind;
    u32 rnd;

    species = PokeParty_GetParam(parents->parents[1], 5, NULL);
    form = PokeParty_GetParam(parents->parents[1], 0x6f, NULL);
    value = PokeParty_GetParam(parents->parents[1], 0xa, NULL);
    kind = func_ov021_021c452c(species, form, value, heapId);
    rnd = GFL_RandomLCAlt(100);
    switch (kind) {
    case 0:
    default:
        if (rnd < 0x50) {
            egg->abilitySlot = 0;
            return;
        }
        egg->abilitySlot = 1;
        return;
    case 1:
        if (rnd < 0x14) {
            egg->abilitySlot = 0;
            return;
        }
        egg->abilitySlot = 1;
        return;
    case 2:
        if (rnd < 0x14) {
            egg->abilitySlot = 0;
            return;
        }
        if (rnd < 0x28) {
            egg->abilitySlot = 1;
            return;
        }
        egg->abilitySlot = 2;
        return;
    }
}


void func_ov021_021c3da8(EggParents *parents, EggData *egg) {
    u32 result;
    u32 result2;

    result = PokeParty_GetParam(parents->parents[0], 5, (void *)0x0);
    result2 = PokeParty_GetParam(parents->parents[1], 5, (void *)0x0);
    if ((result == 0x84) || (result2 == 0x84)) {
        result = GFL_RandomLCAlt(2);
        if (result == 0) {
            egg->abilitySlot = 0;
            return;
        }
        egg->abilitySlot = 1;
    }
}

void func_ov021_021c3de4(EggParents *parents, EggData *egg, HeapID heapId) {
    void *personal;
    u32 val;
    u32 auStack_18 [3];

    personal = PML_PersonalLoad((u16)egg->species, (u16)egg->form, heapId);
    auStack_18[0] = PML_PersonalGetParam(personal, 0x1a);
    auStack_18[1] = PML_PersonalGetParam(personal, 0x1b);
    auStack_18[2] = PML_PersonalGetParam(personal, 0x1c);
    PML_PersonalFree(personal);
    if ((egg->abilitySlot < 2) && (val = auStack_18[egg->abilitySlot], egg->ability = val, val == 0)) {
        egg->ability = auStack_18[0];
    }
}

void func_ov021_021c3e38(EggData *egg, HeapID heapId) {
    void *personal;
    u32 result;

    personal = PML_PersonalLoad((u16)egg->species, (u16)egg->form, heapId);
    result = PML_PersonalGetParam(personal, 0x15);
    PML_PersonalFree(personal);
    egg->happiness = result;
}

void func_ov021_021c3e64(EggParents *parents, EggData *egg) {
    s32 found1 = 0;
    s32 found2 = 0;
    s32 idx1;
    s32 idx2;
    u32 a;
    u32 b;
    s32 i;

    a = PokeParty_GetParam(parents->parents[0], 6, NULL);
    b = PokeParty_GetParam(parents->parents[1], 6, NULL);
    {
        DayCareStatParamTable table = data_ov021_021da930;

        for (i = 0; i < 6; i++) {
            if (a == table.values[i]) {
                found1 = 1;
                idx1 = i;
                break;
            }
        }
        for (i = 0; i < 6; i++) {
            if (b == table.values[i]) {
                found2 = 1;
                idx2 = i;
                break;
            }
        }
    }
    if (found1 && found2) {
        if (GFL_RandomLCAlt(2) == 0) {
            egg->ivFromParent1[idx1] = 1;
            egg->inheritedIvCount++;
            return;
        }
        egg->ivFromParent2[idx2] = 1;
        egg->inheritedIvCount++;
        return;
    }
    if (found1) {
        egg->ivFromParent1[idx1] = 1;
        egg->inheritedIvCount++;
        return;
    }
    if (found2) {
        egg->ivFromParent2[idx2] = 1;
        egg->inheritedIvCount++;
    }
}


void func_ov021_021c3f60(EggParents *parents, EggData *egg) {
    u32 idx;

    while (egg->inheritedIvCount < 3) {
        idx = GFL_RandomLCAlt(6);
        if (GFL_RandomLCAlt(2) == 0) {
            if (egg->ivFromParent1[idx] == 0 && egg->ivFromParent2[idx] == 0) {
                egg->ivFromParent1[idx] = 1;
                egg->inheritedIvCount++;
            }
        } else {
            if (egg->ivFromParent1[idx] == 0 && egg->ivFromParent2[idx] == 0) {
                egg->ivFromParent2[idx] = 1;
                egg->inheritedIvCount++;
            }
        }
    }
}

void func_ov021_021c3fc0(EggParents *parents, EggData *egg) {
    DayCareStatParamTable ids = data_ov021_021da918;
    s32 i;

    for (i = 0; i < 6; i++) {
        if (egg->ivFromParent1[i] != 0) {
            egg->ivs[i] = PokeParty_GetParam(parents->parents[0], ids.values[i], NULL);
        }
        if (egg->ivFromParent2[i] != 0) {
            egg->ivs[i] = PokeParty_GetParam(parents->parents[1], ids.values[i], NULL);
        }
    }
}



void func_ov021_021c4018(PlayerInfo *info, EggData *egg) {
    egg->trainerId = getIDAsUInt(info);
}

void func_ov021_021c4024(EggParents *parents, EggData *egg) {
    egg->pid = func_ov021_021c4078();
}

void func_ov021_021c4030(EggParents *parents, EggData *egg) {
    u32 species0;
    u32 species1;
    s32 i;

    species0 = PokeParty_GetParam(parents->parents[0], 0xc, NULL);
    species1 = PokeParty_GetParam(parents->parents[1], 0xc, NULL);
    if (species0 != species1) {
        if (PML_UtilPIDIsRare(egg->trainerId, egg->pid) == 0) {
            for (i = 0; i < 5; i++) {
                egg->pid = func_ov021_021c4078();
                if (PML_UtilPIDIsRare(egg->trainerId, egg->pid) != 0) {
                    break;
                }
            }
        }
    }
}



u32 func_ov021_021c4078(void) {
    return GFL_RandomLCAlt(0xffffffff);
}

void func_ov021_021c4084(EggData *egg) {
    LvUpMove learn[26];
    LvUpMove cur;
    LvUpMove t;
    LvUpMove m;
    u32 move;
    s32 k;

    PML_LearnsetLvUpLoad(egg->species, egg->form, learn);
    for (k = 0; cur = LvUpMove_Get(learn, k), !LvUpMove_IsEnd(cur); k++) {
        t = cur;
        m = cur;
        move = m.move;
        if (egg->level < t.level) {
            break;
        }
        PushEggPkmMove(egg, move);
    }
}

void func_ov021_021c40f8(EggParents *parents, EggData *egg, HeapID heapId) {
    DayCareMoveParamTable params = data_ov021_021da908;
    u16 *data;
    int count;
    u32 value;
    s32 i;
    s32 j;

    data = GFL_ArcSysReadHeapNew(0x7b, egg->species, heapId);
    count = data[0];
    for (i = 0; i < 4; i++) {
        value = PokeParty_GetParam(parents->parents[0], params.values[i], NULL);
        j = 1;
        if (count >= 1) {
            do {
                if (value == data[j]) {
                    PushEggPkmMove(egg, value);
                    break;
                }
                j++;
            } while (j <= count);
        }
    }
    GFL_HeapFree(data);
}

void func_ov021_021c4160(EggParents *parents, EggData *egg) {
    DayCareMoveParamTable params = data_ov021_021da8f8;
    u32 result;
    LvUpMove learn[26];
    LvUpMove cur;
    LvUpMove m;
    u32 result2;
    s32 k;
    s32 found1;
    s32 found2;
    int move;
    s32 i;

    PML_LearnsetLvUpLoad(egg->species, egg->form, learn);
    for (k = 0; cur = LvUpMove_Get(learn, k), !LvUpMove_IsEnd(cur); k++) {
        m = cur;
        move = m.move;
        found1 = 0;
        found2 = 0;
        for (i = 0; i < 4; i++) {
            result2 = PokeParty_GetParam(parents->parents[0], params.values[i], NULL);
            if (result2 == move) {
                found1 = 1;
                break;
            }
        }
        for (i = 0; i < 4; i++) {
            result = PokeParty_GetParam(parents->parents[1], params.values[i], NULL);
            if (result == move) {
                found2 = 1;
                break;
            }
        }
        if (found1 && found2) {
            PushEggPkmMove(egg, move);
        }
    }
}


void func_ov021_021c4238(EggParents *parents, EggData *egg, HeapID heapId) {
    DayCareMoveParamTable params = data_ov021_021da8e8;
    u32 value;
    s32 tm;
    s32 i;

    for (i = 0; i < 4; i++) {
        value = PokeParty_GetParam(parents->parents[0], params.values[i], NULL);
        for (tm = 0x148; tm <= 0x272; tm++) {
            if (value == PML_ItemGetTMWazaID(tm)) {
                if (func_ov021_021c447c(heapId, egg->species, egg->form, tm)) {
                    PushEggPkmMove(egg, value);
                    break;
                }
            }
        }
    }
}




void func_ov021_021c42bc(EggParents *parents, EggData *egg) {
    if (((s32)egg->species == 0xac) && ((PokeParty_GetParam(parents->parents[0], 6, (void *)0x0) == 0xec || (PokeParty_GetParam(parents->parents[1], 6, (void *)0x0) == 0xec)))) {
        PushEggPkmMove(egg, 0x158);
    }
}

void PushEggPkmMove(EggData *egg, s32 move) {
    u32 i;
    u32 count;
    s32 j;

    if (move != 0) {
        count = egg->moveCount;
        for (i = 0; i < count; i++) {
            if (move == egg->moves[i]) {
                return;
            }
        }
        if (count < 4) {
            egg->moves[count] = move;
            egg->moveCount++;
            return;
        }
        for (j = 0; j < 3; j++) {
            egg->moves[j] = egg->moves[j + 1];
        }
        egg->moves[j] = move;
    }
}

void func_ov021_021c4340(PartyPkm *pkm, const EggData *egg, PlayerInfo *info, u32 place, u16 heapId) {
    DayCareStatParamTable params = data_ov021_021da948;
    StrBuf *strbuf;
    s32 i;

    PokeParty_CreatePkm(pkm, (u16)egg->species, (u16)egg->level, (u64)egg->trainerId, -1, (u64)egg->pid);
    PokeParty_SetParam(pkm, 0x6f, egg->form);
    PokeParty_SetParam(pkm, 0x70, egg->nature);
    PokeParty_SetParam(pkm, 9, egg->happiness);
    if (egg->abilitySlot == 2) {
        PokeParty_SetHiddenAbil(pkm, egg->species, egg->form);
    } else {
        PokeParty_SetParam(pkm, 0xb2, egg->ability);
    }
    for (i = 0; i < 6; i++) {
        if (egg->ivFromParent1[i] != 0 || egg->ivFromParent2[i] != 0) {
            PokeParty_SetParam(pkm, params.values[i], egg->ivs[i]);
        }
    }
    PokeParty_SetParam(pkm, 0x36, egg->moves[0]);
    PokeParty_SetParam(pkm, 0x37, egg->moves[1]);
    PokeParty_SetParam(pkm, 0x38, egg->moves[2]);
    PokeParty_SetParam(pkm, 0x39, egg->moves[3]);
    strbuf = copyTrainerNameToNewStrbuf(info->name, heapId);
    PokeParty_SetParam(pkm, 0x8d, (uintptr_t)strbuf);
    GFL_StrBufFree(strbuf);
    strbuf = GFL_MsgDataLoadStrbufNew(g_PMLSpeciesNamesResident, SPECIES_EGG);
    PokeParty_SetParam(pkm, 0x73, (uintptr_t)strbuf);
    GFL_StrBufFree(strbuf);
    PokeParty_SetupMetData(pkm, 5, info, place, heapId);
    PokeParty_SetParam(pkm, 0x4c, 1);
    PokeParty_PPRecover(pkm);
    PokeParty_RecalcStats(pkm);
    PokeParty_Recover(pkm);
}


u32 func_ov021_021c447c(HeapID heapId, u16 species, u16 form, u16 tm) {
    u32 bit;
    void *personal;
    u32 mask;

    if (species >= 0x289) {
        return 0;
    }
    if (PML_ItemIsTMHM(tm) == 0) {
        return 0;
    }
    personal = PML_PersonalLoad(species, form, heapId);
    bit = PML_ItemGetTMBitMask(tm);
    if (bit < 0x20) {
        mask = PML_PersonalGetParam(personal, 0x27);
    } else if (bit < 0x40) {
        mask = PML_PersonalGetParam(personal, 0x28);
        bit -= 0x20;
    } else if (bit < 0x60) {
        mask = PML_PersonalGetParam(personal, 0x29);
        bit -= 0x40;
    } else {
        mask = PML_PersonalGetParam(personal, 0x2a);
        bit -= 0x60;
    }
    PML_PersonalFree(personal);
    return (1 << bit) & mask;
}


u32 validateSpeciesAndGetOffspring(u32 param_1) {
    u16 data;

    if (param_1 < 0x289) {
        GFL_ArcSysReadRange(&data, 0x14, param_1, 0, 2);
        return data;
    }
    return param_1;
}

u8 func_ov021_021c452c(u16 species, u16 form, u16 value, HeapID heapId) {
    void *personal;
    u32 values[3];
    u32 i;

    personal = PML_PersonalLoad(species, form, heapId);
    values[0] = PML_PersonalGetParam(personal, 0x1a);
    values[1] = PML_PersonalGetParam(personal, 0x1b);
    values[2] = PML_PersonalGetParam(personal, 0x1c);
    PML_PersonalFree(personal);
    for (i = 0; i < 3; i++) {
        if (value == values[i]) {
            break;
        }
    }
    if (i >= 3) {
        i = 0;
    }
    return (u8)i;
}

static const DayCareMoveParamTable data_ov021_021da8c8 = { { PKM_PARAM_MOVE1, PKM_PARAM_MOVE2, PKM_PARAM_MOVE3, PKM_PARAM_MOVE4 } };

static const DayCareMoveParamTable data_ov021_021da8d8 = { { PKM_PARAM_MOVE1, PKM_PARAM_MOVE2, PKM_PARAM_MOVE3, PKM_PARAM_MOVE4 } };

static const DayCareIncenseTable data_ov021_021da960 = {
#include "asset/sodateya_data_ov021_021da960.inc"
};

static const DayCareStatParamTable data_ov021_021da930 = { {
    ITEM_POWER_WEIGHT, ITEM_POWER_BRACER, ITEM_POWER_BELT, ITEM_POWER_LENS, ITEM_POWER_BAND, ITEM_POWER_ANKLET
} };

static const DayCareMoveParamTable data_ov021_021da8f8 = { { PKM_PARAM_MOVE1, PKM_PARAM_MOVE2, PKM_PARAM_MOVE3, PKM_PARAM_MOVE4 } };

static const DayCareMoveParamTable data_ov021_021da8e8 = { { PKM_PARAM_MOVE1, PKM_PARAM_MOVE2, PKM_PARAM_MOVE3, PKM_PARAM_MOVE4 } };

static const DayCareStatParamTable data_ov021_021da918 = { {
    PKM_PARAM_IV_HP, PKM_PARAM_IV_ATTACK, PKM_PARAM_IV_DEFENSE, PKM_PARAM_IV_SP_ATTACK, PKM_PARAM_IV_SP_DEFENSE, PKM_PARAM_IV_SPEED
} };

static const DayCareStatParamTable data_ov021_021da948 = { {
    PKM_PARAM_IV_HP, PKM_PARAM_IV_ATTACK, PKM_PARAM_IV_DEFENSE, PKM_PARAM_IV_SP_ATTACK, PKM_PARAM_IV_SP_DEFENSE, PKM_PARAM_IV_SPEED
} };

static const DayCareMoveParamTable data_ov021_021da908 = { { PKM_PARAM_MOVE1, PKM_PARAM_MOVE2, PKM_PARAM_MOVE3, PKM_PARAM_MOVE4 } };
