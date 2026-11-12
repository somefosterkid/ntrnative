// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/tr_tool.c, ported by tools/port_file.py.
#include "types.h"
#include "battle/btl_setup.h"
#include "battle/trainer_data.h"
#include "constants/arc.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "constants/trainer_classes.h"
#include "constants/trainer_messages.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/math.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/pms_data.h"
#include "nitro/mi.h"
#include "constants/species.h"

// The trainers' records, parties and messages, and the trainer classes' tables. Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except the structs, the constants and the fields

#define TRAINER_CLASS_COUNT 236

// The system message files of the trainers' messages and names, and of the Battle Institute trainers' messages
#define TRMSG_FILE TEXT_BANK_TRAINER_MESSAGES
#define TRNAME_FILE TEXT_BANK_TRAINER_NAMES
#define TRMSG_BATTLE_INST_START_FILE TEXT_BANK_BATTLE_INST_START_MESSAGES
#define TRMSG_BATTLE_INST_LOSE_FILE TEXT_BANK_BATTLE_INST_LOSE_MESSAGES


// The Battle Institute's trainers' records: the trainers from 620 to 639 share one, and all the others another
#define TRDATA_BATTLE_INST_GROUP_START 620
#define TRDATA_BATTLE_INST_GROUP_COUNT 20
#define TRDATA_BATTLE_INST_GROUP_FILE 661
#define TRDATA_BATTLE_INST_FILE 660

// The low byte of a trainer Pokémon's PID, by the trainer's sex: it makes the Pokémon of an even sex ratio female for
// a female trainer and male for a male one
#define PID_BASE_FEMALE_TRAINER 0x78
#define PID_BASE_MALE_TRAINER 0x88

#define PACK_IVS(hp, atk, def, spe, spa, spd)                                                                          \
    (((hp) & 0x1f) | (((atk) & 0x1f) << 5) | (((def) & 0x1f) << 10) | (((spe) & 0x1f) << 15) |                         \
     (((spa) & 0x1f) << 20) | (((spd) & 0x1f) << 25))

typedef struct {
    u16 trainerClass;
    u16 bg;
} TrClassBattleBG;

typedef struct {
    u8 trainerClass;
    u8 bgmGroup;
    u8 pedestal;
} SpecialTrainerClass;

typedef struct {
    u16 trainerClass;
    u16 sprite;
} TrClassBackSprite;

typedef struct {
    u8 sprite;
    u8 sex;
    u16 eyeBGM;
} TrClassResource;

static void TrainerData_ReadTrainerData(int trainerId, TrainerData *data);
void func_0202a3bc(u32 fileId, void *dst);
u8 func_0202a3cc(u32 index);
u8 func_0202a3d8(u32 trainerClass);
void func_0202a9f4(int trainerId, PartyPkm *pkm, u32 form, u8 genderAbility);
static void TrainerUtil_CalcBasePID(u32 species, u32 form, u8 genderAbility, u32 *pidBase, HeapID heapId);

u32 TrainerData_GetParam(int trainerId, u32 param) {
    TrainerData data;
    u32 value;

    TrainerData_ReadTrainerData(trainerId, &data);
    switch (param) {
    case TRAINER_PARAM_PARTY_KIND:
        value = data.partyKind;
        break;
    case TRAINER_PARAM_CLASS:
        value = data.trainerClass;
        break;
    case TRAINER_PARAM_BATTLE_STYLE:
        value = data.battleStyle;
        break;
    case TRAINER_PARAM_POKE_COUNT:
        value = data.pokeCount;
        break;
    case TRAINER_PARAM_ITEM_1:
    case TRAINER_PARAM_ITEM_2:
    case TRAINER_PARAM_ITEM_3:
    case TRAINER_PARAM_ITEM_4:
        value = data.items[param - TRAINER_PARAM_ITEM_1];
        break;
    case TRAINER_PARAM_AI_FLAGS:
        value = data.aiFlags;
        break;
    case TRAINER_PARAM_HEALS:
        value = data.heals;
        break;
    case TRAINER_PARAM_MONEY:
        value = data.money;
        break;
    case TRAINER_PARAM_REWARD_ITEM:
        value = data.rewardItem;
        break;
    }
    return value;
}

u32 func_0202a298(u32 id, u32 trainerClass, HeapID heapId)
{
    u32 totalLen;
    ArcTool *handle;
    u16 entry[3];
    u32 found;

    found = 0;
    totalLen = GFL_ArcSysGetDataLength(0x5a, 0);
    GFL_ArcSysReadRange(&entry[0], 0x5b, 0, id << 1, 2);
    handle = GFL_ArcSysCreateFileHandle(0x5a, heapId);

    while (entry[0] != totalLen)
    {
        GFL_ArcToolReadRange(handle, 0, entry[0], 4, &entry[1]);
        if (entry[1] == id && entry[2] == trainerClass)
        {
            found = 1;
            break;
        }
        if (entry[1] != id)
            break;
        entry[0] += 4;
    }

    GFL_ArcToolFree(handle);
    return found;
}

void func_0202a310(u16 trainerId, u16 trainerClass, StrBuf *strBuf, HeapID heapId)
{
    ArcTool *handle;
    MsgData *msgData;
    u16 offset;
    int dataLength;
    u16 record[2];

    msgData = GFL_MsgSysLoadData(0, 2, 0xbd, heapId);
    dataLength = GFL_ArcSysGetDataLength(0x5a, 0);
    GFL_ArcSysReadRange(&offset, 0x5b, 0, trainerId << 1, 2);
    handle = GFL_ArcSysCreateFileHandle(0x5a, heapId);
    while (offset != dataLength) {
        GFL_ArcToolReadRange(handle, 0, offset, 4, record);
        if (record[0] == trainerId && record[1] == trainerClass) {
            GFL_MsgDataLoadStrbuf(msgData, (u32)offset >> 2, strBuf);
            break;
        }
        offset += 4;
    }
    GFL_ArcToolFree(handle);
    GFL_MsgDataFree(msgData);
    if (offset == dataLength)
        GFL_StrBufClear(strBuf);
}





static void TrainerData_ReadTrainerData(int trainerId, TrainerData *data) {
    GFL_ArcSysRead(data, 0x5c, trainerId);
}

void func_0202a3bc(u32 fileId, void *dst)
{
    GFL_ArcSysRead(dst, 0x5d, fileId);
}

const u8 data_0209ff1c[] = {
#include "asset/tr_tool_data_0209ff1c.inc"
};

u8 func_0202a3cc(u32 index)
{
    return data_0209ff1c[index];
}

typedef struct {
    u16 unk_0 : 7;
    u16 bgmGroup : 4;
    u16 unk_11 : 5;
} TrainerClassBGMGroup;

const TrainerClassBGMGroup data_0209fee4[28] = {
    { 37, 0, 17 },
    { 38, 1, 17 },
    { 10, 2, 17 },
    { 12, 2, 17 },
    { 11, 2, 17 },
    { 19, 2, 17 },
    { 20, 2, 17 },
    { 21, 2, 17 },
    { 22, 2, 17 },
    { 23, 2, 17 },
    { 54, 2, 17 },
    { 55, 2, 17 },
    { 56, 2, 17 },
    { 78, 3, 17 },
    { 80, 3, 17 },
    { 79, 3, 17 },
    { 81, 3, 17 },
    { 89, 4, 17 },
    { 40, 5, 17 },
    { 47, 6, 17 },
    { 101, 7, 17 },
    { 82, 8, 16 },
    { 39, 9, 17 },
    { 77, 9, 17 },
    { 100, 10, 17 },
    { 88, 11, 17 },
    { 102, 11, 17 },
    { 0, 0, 0 },
};
u8 func_0202a3d8(u32 trainerClass)
{
    int i;
    for (i = 0; i < 28; i++)
    {
        if (trainerClass == data_0209fee4[i].unk_0)
            return i;
    }
    return 28;
}









u8 GetTrainerClassBGMGroupId(u32 trainerClass) {
    u8 id = func_0202a3d8(trainerClass);

    if (id >= 0x1c) {
        return 12;
    }
    return data_0209fee4[id].bgmGroup;
}

u32 func_0202a420(u32 trainerClass)
{
    u8 id = func_0202a3d8(trainerClass);
    if (id >= 0x1c)
        return 0x11;
    return data_0209fee4[id].unk_11;
}

void TrainerUtil_LoadTrainer(int trainerId, BtlSetupTrainer *trainer, HeapID heapId) {
    TrainerData *data = GFL_HeapAllocate(heapId, sizeof(TrainerData), FALSE, "tr_tool.c", 345);

    TrainerData_ReadTrainerData(trainerId, data);
    trainer->trainerId = trainerId;
    trainer->trainerClass = data->trainerClass;
    trainer->aiFlags = data->aiFlags;

    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TRNAME_FILE, heapId);

    GFL_MsgDataLoadStrbuf(msgData, trainerId, trainer->name);
    GFL_MsgDataFree(msgData);

    MIi_CpuCopy16(data->items, trainer->items, sizeof(trainer->items));
    func_0202428c(&trainer->unk18);
    func_0202428c(&trainer->unk20);
    GFL_HeapFree(data);
}





void TrainerUtil_LoadParty(int trainerId, PokeParty *party, HeapID heapId) {
    TrainerData *data;
    void *pokes;
    PartyPkm *pkm;
    u32 pidBase;
    MATHRandContext32 rand;
    u32 rnd;
    u8 iv;
    int i;
    int j;

    PokeParty_InitCore(party, 6);
    data = GFL_HeapAllocate(HEAPID_TAIL(heapId), sizeof(TrainerData), FALSE, "tr_tool.c", 387);
    pokes = GFL_HeapAllocate(HEAPID_TAIL(heapId), sizeof(TrainerPokeItemMoves) * 6, FALSE, "tr_tool.c", 388);
    pkm = GFL_HeapAllocate(HEAPID_TAIL(heapId), PokeParty_GetPkmRawSize(), FALSE, "tr_tool.c", 389);
    TrainerData_ReadTrainerData(trainerId, data);
    func_0202a3bc(trainerId, pokes);
    pidBase = func_0202a3cc(data->trainerClass) == 1 ? PID_BASE_FEMALE_TRAINER : PID_BASE_MALE_TRAINER;

    switch (data->partyKind) {
    case 0: {
        TrainerPoke *poke = pokes;

        for (i = 0; i < data->pokeCount; i++) {
            TrainerUtil_CalcBasePID(poke[i].species, poke[i].form, poke[i].genderAbility, &pidBase, heapId);
            rnd = poke[i].difficulty + poke[i].level + poke[i].species + trainerId;
            MATH_InitRand32(&rand, rnd);
            for (j = 0; j < data->trainerClass; j++) {
                rnd = MATH_Rand32(&rand, 0x10000);
            }
            rnd = (rnd << 8) + pidBase;
            iv = poke[i].difficulty * 31 / 255;
            PokeParty_CreatePkm(pkm, poke[i].species, poke[i].level, PKM_ID_NOT_SHINY, PACK_IVS(iv, iv, iv, iv, iv, iv), rnd);
            func_0202a9f4(trainerId, pkm, poke[i].form, poke[i].genderAbility);
            PokeParty_AddPkm(party, pkm);
        }
        break;
    }
    case PARTY_MOVES: {
        TrainerPokeMoves *poke = pokes;

        for (i = 0; i < data->pokeCount; i++) {
            TrainerUtil_CalcBasePID(poke[i].base.species, poke[i].base.form, poke[i].base.genderAbility, &pidBase,
                                    heapId);
            rnd = poke[i].base.difficulty + poke[i].base.level + poke[i].base.species + trainerId;
            MATH_InitRand32(&rand, rnd);
            for (j = 0; j < data->trainerClass; j++) {
                rnd = MATH_Rand32(&rand, 0x10000);
            }
            rnd = (rnd << 8) + pidBase;
            iv = poke[i].base.difficulty * 31 / 255;
            PokeParty_CreatePkm(pkm, poke[i].base.species, poke[i].base.level, PKM_ID_NOT_SHINY, PACK_IVS(iv, iv, iv, iv, iv, iv), rnd);
            for (j = 0; j < 4; j++) {
                PokeParty_SetMove(pkm, poke[i].moves[j], j);
            }
            func_0202a9f4(trainerId, pkm, poke[i].base.form, poke[i].base.genderAbility);
            PokeParty_AddPkm(party, pkm);
        }
        break;
    }
    case PARTY_ITEMS: {
        TrainerPokeItem *poke = pokes;

        for (i = 0; i < data->pokeCount; i++) {
            TrainerUtil_CalcBasePID(poke[i].base.species, poke[i].base.form, poke[i].base.genderAbility, &pidBase,
                                    heapId);
            rnd = poke[i].base.difficulty + poke[i].base.level + poke[i].base.species + trainerId;
            MATH_InitRand32(&rand, rnd);
            for (j = 0; j < data->trainerClass; j++) {
                rnd = MATH_Rand32(&rand, 0x10000);
            }
            rnd = (rnd << 8) + pidBase;
            iv = poke[i].base.difficulty * 31 / 255;
            PokeParty_CreatePkm(pkm, poke[i].base.species, poke[i].base.level, PKM_ID_NOT_SHINY, PACK_IVS(iv, iv, iv, iv, iv, iv), rnd);
            PokeParty_SetParam(pkm, PKM_PARAM_ITEM, poke[i].item);
            func_0202a9f4(trainerId, pkm, poke[i].base.form, poke[i].base.genderAbility);
            PokeParty_AddPkm(party, pkm);
        }
        break;
    }
    case PARTY_MOVES | PARTY_ITEMS: {
        TrainerPokeItemMoves *poke = pokes;

        for (i = 0; i < data->pokeCount; i++) {
            TrainerUtil_CalcBasePID(poke[i].base.species, poke[i].base.form, poke[i].base.genderAbility, &pidBase,
                                    heapId);
            rnd = poke[i].base.difficulty + poke[i].base.level + poke[i].base.species + trainerId;
            MATH_InitRand32(&rand, rnd);
            for (j = 0; j < data->trainerClass; j++) {
                rnd = MATH_Rand32(&rand, 0x10000);
            }
            rnd = (rnd << 8) + pidBase;
            iv = poke[i].base.difficulty * 31 / 255;
            PokeParty_CreatePkm(pkm, poke[i].base.species, poke[i].base.level, PKM_ID_NOT_SHINY, PACK_IVS(iv, iv, iv, iv, iv, iv), rnd);
            PokeParty_SetParam(pkm, PKM_PARAM_ITEM, poke[i].item);
            for (j = 0; j < 4; j++) {
                PokeParty_SetMove(pkm, poke[i].moves[j], j);
            }
            func_0202a9f4(trainerId, pkm, poke[i].base.form, poke[i].base.genderAbility);
            PokeParty_AddPkm(party, pkm);
        }
        break;
    }
    }
    GFL_HeapFree(data);
    GFL_HeapFree(pokes);
    GFL_HeapFree(pkm);
}

// Sets the low byte of the PID so that it gives the Pokémon the sex and ability of genderAbility
static void TrainerUtil_CalcBasePID(u32 species, u32 form, u8 genderAbility, u32 *pidBase, HeapID heapId) {
    int sex = genderAbility & 0xf;
    int ability = (genderAbility & 0xf0) >> 4;

    if (genderAbility != 0) {
        void *personal = PML_PersonalLoad(species, form, heapId);

        if (sex != 0) {
            *pidBase = PML_PersonalGetParam(personal, PERSONAL_SEX_RATIO);
            if (sex == 1) {
                *pidBase += 2;
            } else {
                *pidBase -= 2;
            }
        }
        if (ability == 1) {
            *pidBase &= ~1;
        } else if (ability == 2) {
            *pidBase |= 1;
        }
        PML_PersonalFree(personal);
    }
}

// Sets what CreatePkm doesn't: the friendship, 0 if the Pokémon knows Frustration, the form, the ability and the
// nature (fixed for two trainers' Reshiram and Zekrom)
void func_0202a9f4(int trainerId, PartyPkm *pkm, u32 form, u8 genderAbility) {
    u16 species;
    int i;
    u8 happiness = 255;
    u8 param;

    species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    for (i = 0; i < 4; i++) {
        if (PokeParty_GetParam(pkm, PKM_PARAM_MOVE1 + i, NULL) == MOVE_FRUSTRATION) {
            happiness = 0;
        }
    }
    PokeParty_SetParam(pkm, PKM_PARAM_HAPPINESS, happiness);
    PokeParty_SetParam(pkm, PKM_PARAM_FORM, form);

    if ((genderAbility & 0xF0) == 0x30) {
        PokeParty_SetHiddenAbil(pkm, species, form);
    } else if (genderAbility & 0xF0) {
        param = PERSONAL_ABILITY_1;
        if (PML_PersonalGetParamSingle(species, form, PERSONAL_ABILITY_2) != 0 && (genderAbility & 0xF0) == 0x20) {
            param = PERSONAL_ABILITY_2;
        }
        PokeParty_SetParam(pkm, PKM_PARAM_ABILITY, PML_PersonalGetParamSingle(species, form, param));
    }

    if (trainerId == 0x24A && species == SPECIES_RESHIRAM) {
        PokeParty_SetNature(pkm, 13);
    } else if (trainerId == 0x24B && species == SPECIES_ZEKROM) {
        PokeParty_SetNature(pkm, 10);
    } else {
        PokeParty_SetNature(pkm, (u8)(((int)PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL) >> 8) % 25));
    }
}



