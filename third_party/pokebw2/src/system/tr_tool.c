#include "types.h"
#include "battle/btl_setup.h"
#include "battle/trainer_data.h"
#include "constants/arc.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "constants/trainer_back_sprites.h"
#include "constants/trainer_classes.h"
#include "constants/trainer_messages.h"
#include "constants/trainer_sprites.h"
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
static void TrainerData_ReadParty(int trainerId, void *party);
static void TrainerUtil_CalcBasePID(u32 species, u32 form, u8 genderAbility, u32 *pidBase, HeapID heapId);
static void TrainerUtil_SetupPkm(int trainerId, PartyPkm *pkm, u32 form, u8 genderAbility);
static u8 TrainerUtil_IsIDBattleInst(int trainerId);

static const TrClassBattleBG TR_CLASS_BATTLE_BG_OVERRIDES[] = {
    { TRAINER_CLASS_TEAM_PLASMA_GHETSIS, 20 },
};

static const SpecialTrainerClass SPECIAL_TRAINER_CLASSES[TRAINER_CLASS_SPECIAL_COUNT] = {
#include "asset/tr_tool_SPECIAL_TRAINER_CLASSES.inc"
};

static const TrClassBackSprite TRAINER_CLASS_BACK_SPRITES[] = {
    { TRAINER_CLASS_PKMN_TRAINER_NATE, TRAINER_BACK_SPRITE_PKMN_TRAINER_NATE },
    { TRAINER_CLASS_PKMN_TRAINER_ROSA, TRAINER_BACK_SPRITE_PKMN_TRAINER_ROSA },
    { TRAINER_CLASS_PKMN_TRAINER_CHILI, TRAINER_BACK_SPRITE_PKMN_TRAINER_CHILI },
    { TRAINER_CLASS_PKMN_TRAINER_CILAN, TRAINER_BACK_SPRITE_PKMN_TRAINER_CILAN },
    { TRAINER_CLASS_PKMN_TRAINER_CRESS, TRAINER_BACK_SPRITE_PKMN_TRAINER_CRESS },
    { TRAINER_CLASS_NO_DATA_M_37, TRAINER_BACK_SPRITE_NO_DATA_M },
    { TRAINER_CLASS_PKMN_TRAINER_BIANCA, TRAINER_BACK_SPRITE_PKMN_TRAINER_BIANCA },
    { TRAINER_CLASS_DOCTOR, TRAINER_BACK_SPRITE_DOCTOR },
    { TRAINER_CLASS_PKMN_TRAINER_RIVAL, TRAINER_BACK_SPRITE_PKMN_TRAINER_RIVAL },
    { TRAINER_CLASS_PKMN_TRAINER_M_180, TRAINER_BACK_SPRITE_PKMN_TRAINER_M_180 },
    { TRAINER_CLASS_PKMN_TRAINER_F_181, TRAINER_BACK_SPRITE_PKMN_TRAINER_F_181 },
    { TRAINER_CLASS_PKMN_TRAINER_M_182, TRAINER_BACK_SPRITE_PKMN_TRAINER_M_182 },
    { TRAINER_CLASS_PKMN_TRAINER_F_183, TRAINER_BACK_SPRITE_PKMN_TRAINER_F_183 },
    { TRAINER_CLASS_PKMN_TRAINER_M_184, TRAINER_BACK_SPRITE_PKMN_TRAINER_M_184 },
    { TRAINER_CLASS_PKMN_TRAINER_F_185, TRAINER_BACK_SPRITE_PKMN_TRAINER_F_185 },
    { TRAINER_CLASS_CHAMPION, TRAINER_BACK_SPRITE_CHAMPION },
    { TRAINER_CLASS_PKMN_TRAINER_CHEREN, TRAINER_BACK_SPRITE_PKMN_TRAINER_CHEREN },
    { TRAINER_CLASS_PKMN_TRAINER_F_198, TRAINER_BACK_SPRITE_CHAMPION },
    { TRAINER_CLASS_HERO, TRAINER_BACK_SPRITE_HERO },
    { TRAINER_CLASS_HEROINE, TRAINER_BACK_SPRITE_HEROINE },
    { TRAINER_CLASS_NINJA_M, TRAINER_BACK_SPRITE_NINJA_M },
    { TRAINER_CLASS_NINJA_F, TRAINER_BACK_SPRITE_NINJA_F },
    { TRAINER_CLASS_UDF_TROOPER_M, TRAINER_BACK_SPRITE_UDF_TROOPER_M },
    { TRAINER_CLASS_UDF_TROOPER_F, TRAINER_BACK_SPRITE_UDF_TROOPER_F },
    { TRAINER_CLASS_SCIENTIST_M_205, TRAINER_BACK_SPRITE_SCIENTIST_M },
    { TRAINER_CLASS_SCIENTIST_F_206, TRAINER_BACK_SPRITE_SCIENTIST_F },
    { TRAINER_CLASS_GUY, TRAINER_BACK_SPRITE_GUY },
    { TRAINER_CLASS_GIRL, TRAINER_BACK_SPRITE_GIRL },
    { TRAINER_CLASS_PRINCE, TRAINER_BACK_SPRITE_PRINCE },
    { TRAINER_CLASS_PRINCESS, TRAINER_BACK_SPRITE_PRINCESS },
    { TRAINER_CLASS_GUY_IN_SUIT_225, TRAINER_BACK_SPRITE_GUY_IN_SUIT },
    { TRAINER_CLASS_GIRL_IN_SUIT_226, TRAINER_BACK_SPRITE_GIRL_IN_SUIT },
};

static const TrClassResource TRAINER_CLASS_RESOURCES[TRAINER_CLASS_COUNT] = {
#include "asset/tr_tool_TRAINER_CLASS_RESOURCES.inc"
};

u32 TrainerData_GetParam(int trainerId, TrainerDataParam param) {
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

BOOL TrainerMsg_CheckExists(int trainerId, u32 msgId, HeapID heapId) {
    ArcTool *handle;
    u32 size;
    u16 offset;
    u16 pair[2];
    BOOL exists = FALSE;

    if (TrainerUtil_IsIDBattleInst(trainerId) == FALSE) {
        size = GFL_ArcSysGetDataLength(ARCID_TRTBL, 0);
        GFL_ArcSysReadRange(&offset, ARCID_TRTBLOFS, 0, trainerId * 2, sizeof(offset));
        handle = GFL_ArcSysCreateFileHandle(ARCID_TRTBL, heapId);
        while (offset != size) {
            GFL_ArcToolReadRange(handle, 0, offset, sizeof(pair), pair);
            if (pair[0] == trainerId && pair[1] == msgId) {
                exists = TRUE;
                break;
            }
            if (pair[0] != trainerId) {
                break;
            }
            offset += sizeof(pair);
        }
        GFL_ArcToolFree(handle);
    } else if (msgId == TRMSG_LOSE || msgId == TRMSG_WIN) {
        exists = TRUE;
    }
    return exists;
}

void TrainerMsg_Load(int trainerId, u32 msgId, StrBuf *strbuf, HeapID heapId) {
    ArcTool *handle;
    MsgData *msgData;
    u32 size;
    int offsetsSize;
    u16 offset;
    u16 pair[2];
    u8 battleInst = TrainerUtil_IsIDBattleInst(trainerId);

    if (battleInst == FALSE) {
        msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TRMSG_FILE, heapId);
        size = GFL_ArcSysGetDataLength(ARCID_TRTBL, 0);
        offsetsSize = GFL_ArcSysGetDataLength(ARCID_TRTBLOFS, 0);
        if (offsetsSize < trainerId * 2) {
            GFL_StrBufClear(strbuf);
            GFL_MsgDataFree(msgData);
            return;
        }
        GFL_ArcSysReadRange(&offset, ARCID_TRTBLOFS, 0, trainerId * 2, sizeof(offset));
        handle = GFL_ArcSysCreateFileHandle(ARCID_TRTBL, heapId);
        while (offset != size) {
            GFL_ArcToolReadRange(handle, 0, offset, sizeof(pair), pair);
            if (pair[0] == trainerId && pair[1] == msgId) {
                GFL_MsgDataLoadStrbuf(msgData, offset / sizeof(pair), strbuf);
                break;
            }
            offset += sizeof(pair);
        }
        GFL_ArcToolFree(handle);
        GFL_MsgDataFree(msgData);
        if (offset == size) {
            GFL_StrBufClear(strbuf);
        }
    } else if (battleInst == TRUE) {
        msgData = NULL;
        switch (msgId) {
        case TRMSG_START:
            msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TRMSG_BATTLE_INST_START_FILE, heapId);
            break;
        case TRMSG_LOSE:
            msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TRMSG_BATTLE_INST_LOSE_FILE, heapId);
            break;
        }
        if (msgData != NULL) {
            GFL_MsgDataLoadStrbuf(msgData, trainerId - TRAINER_ID_BATTLE_INST_START, strbuf);
            GFL_MsgDataFree(msgData);
        }
    }
}

static void TrainerData_ReadTrainerData(int trainerId, TrainerData *data) {
    if (trainerId < TRAINER_ID_BATTLE_INST_START) {
        GFL_ArcSysRead(data, ARCID_TRDATA, trainerId);
    } else {
        int id = trainerId - TRAINER_ID_BATTLE_INST_START;

        if (id >= TRDATA_BATTLE_INST_GROUP_START &&
            id < TRDATA_BATTLE_INST_GROUP_START + TRDATA_BATTLE_INST_GROUP_COUNT) {
            GFL_ArcSysRead(data, ARCID_TRDATA, TRDATA_BATTLE_INST_GROUP_FILE);
        } else {
            GFL_ArcSysRead(data, ARCID_TRDATA, TRDATA_BATTLE_INST_FILE);
        }
    }
}

static void TrainerData_ReadParty(int trainerId, void *party) {
    GFL_ArcSysRead(party, ARCID_TRPOKE, trainerId);
}

u8 TrainerClass_GetSex(u32 trainerClass) {
    return TRAINER_CLASS_RESOURCES[trainerClass].sex;
}

u8 GetTrSpriteBaseDatID(u32 trainerClass, u32 flags) {
    u32 i;

    if (flags & TRAINER_SPRITE_BACK) {
        for (i = 0; i < NELEMS(TRAINER_CLASS_BACK_SPRITES); i++) {
            if (trainerClass == TRAINER_CLASS_BACK_SPRITES[i].trainerClass) {
                return TRAINER_CLASS_BACK_SPRITES[i].sprite;
            }
        }
        // Any other class has the player's back: a female class Nate's and a male class Rosa's
        return TrainerClass_GetSex(trainerClass) == 1 ? TRAINER_BACK_SPRITE_PKMN_TRAINER_NATE
                                                      : TRAINER_BACK_SPRITE_PKMN_TRAINER_ROSA;
    }
    if (trainerClass < TRAINER_CLASS_COUNT) {
        return TRAINER_CLASS_RESOURCES[trainerClass].sprite;
    }
    return 0;
}

u16 GetTrainerClassEyeBGMID(u32 trainerClass) {
    return TRAINER_CLASS_RESOURCES[trainerClass].eyeBGM;
}

u8 GetTrainerClassSpecialId(u32 trainerClass) {
    int i;

    for (i = 0; i < TRAINER_CLASS_SPECIAL_COUNT; i++) {
        if (trainerClass == SPECIAL_TRAINER_CLASSES[i].trainerClass) {
            return i;
        }
    }
    return TRAINER_CLASS_SPECIAL_COUNT;
}

u32 GetTrainerClassBGMGroupId(u32 trainerClass) {
    u8 id = GetTrainerClassSpecialId(trainerClass);

    if (id >= TRAINER_CLASS_SPECIAL_COUNT) {
        return 15;
    }
    return SPECIAL_TRAINER_CLASSES[id].bgmGroup;
}

u32 GetTrainerClassBattlePedestal(u32 trainerClass) {
    u8 id = GetTrainerClassSpecialId(trainerClass);

    if (id >= TRAINER_CLASS_SPECIAL_COUNT) {
        return TRAINER_PEDESTAL_NONE;
    }
    return SPECIAL_TRAINER_CLASSES[id].pedestal;
}

u32 CheckOverridenTrainerBattleBG(u32 trainerClass, u32 bg) {
    u32 i;

    for (i = 0; i < NELEMS(TR_CLASS_BATTLE_BG_OVERRIDES); i++) {
        if (trainerClass == TR_CLASS_BATTLE_BG_OVERRIDES[i].trainerClass) {
            return TR_CLASS_BATTLE_BG_OVERRIDES[i].bg;
        }
    }
    return bg;
}

void TrainerUtil_LoadTrainer(GameData *gameData, int trainerId, BtlSetupTrainer *trainer, HeapID heapId) {
    TrainerData *data = GFL_HeapAllocate(heapId, sizeof(TrainerData), FALSE, "tr_tool.c", 523);

    TrainerData_ReadTrainerData(trainerId, data);
    trainer->trainerId = trainerId;
    trainer->trainerClass = data->trainerClass;
    trainer->aiFlags = data->aiFlags;
    if (trainer->trainerClass == TRAINER_CLASS_PKMN_TRAINER_RIVAL) {
        GFL_StrBufLoadString(trainer->name, getPtrToRivalName(getHollow_RivalData(GameData_GetSaveControl(gameData))));
    } else {
        MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TRNAME_FILE, heapId);

        GFL_MsgDataLoadStrbuf(msgData, trainerId, trainer->name);
        GFL_MsgDataFree(msgData);
    }
    MIi_CpuCopy16(data->items, trainer->items, sizeof(trainer->items));
    PMSData_Clear(&trainer->winWords);
    PMSData_Clear(&trainer->loseWords);
    GFL_HeapFree(data);
}

void TrainerUtil_LoadParty(int trainerId, PokeParty *party, HeapID heapId) {
    TrainerData *data;
    void *pokes;
    PartyPkm *pkm;
    u32 pidBase;
#ifdef BUGFIX
    u32 trainerPidBase;
#endif
    MATHRandContext32 rand;
    u32 rnd;
    u8 iv;
    int i;
    int j;

    PokeParty_InitCore(party, 6);
    data = GFL_HeapAllocate(HEAPID_TAIL(heapId), sizeof(TrainerData), FALSE, "tr_tool.c", 573);
    pokes = GFL_HeapAllocate(HEAPID_TAIL(heapId), sizeof(TrainerPokeItemMoves) * 6, FALSE, "tr_tool.c", 574);
    pkm = GFL_HeapAllocate(HEAPID_TAIL(heapId), PokeParty_GetPkmRawSize(), FALSE, "tr_tool.c", 575);
    TrainerData_ReadTrainerData(trainerId, data);
    TrainerData_ReadParty(trainerId, pokes);
    // BUG: TrainerUtil_CalcBasePID only changes the PID base for a Pokémon that asks for a sex or an ability, and the
    // base is never reset, so a later Pokémon that asks for neither keeps the one before it. Grimsley's Liepard asks to
    // be female, and his Scrafty, Krookodile and Bisharp, which ask only for an ability, come out female too.
    pidBase = TrainerClass_GetSex(data->trainerClass) == 1 ? PID_BASE_FEMALE_TRAINER : PID_BASE_MALE_TRAINER;
#ifdef BUGFIX
    trainerPidBase = pidBase;
#endif

    switch (data->partyKind) {
    case 0: {
        TrainerPoke *poke = pokes;

        for (i = 0; i < data->pokeCount; i++) {
#ifdef BUGFIX
            pidBase = trainerPidBase;
#endif
            TrainerUtil_CalcBasePID(poke[i].species, poke[i].form, poke[i].genderAbility, &pidBase, heapId);
            rnd = poke[i].difficulty + poke[i].level + poke[i].species + trainerId;
            MATH_InitRand32(&rand, rnd);
            for (j = 0; j < data->trainerClass; j++) {
                rnd = MATH_Rand32(&rand, 0x10000);
            }
            rnd = (rnd << 8) + pidBase;
            iv = poke[i].difficulty * 31 / 255;
            PokeParty_CreatePkm(pkm, poke[i].species, poke[i].level, PKM_ID_NOT_SHINY, PACK_IVS(iv, iv, iv, iv, iv, iv),
                                rnd);
            TrainerUtil_SetupPkm(trainerId, pkm, poke[i].form, poke[i].genderAbility);
            PokeParty_AddPkm(party, pkm);
        }
        break;
    }
    case PARTY_MOVES: {
        TrainerPokeMoves *poke = pokes;

        for (i = 0; i < data->pokeCount; i++) {
#ifdef BUGFIX
            pidBase = trainerPidBase;
#endif
            TrainerUtil_CalcBasePID(poke[i].base.species, poke[i].base.form, poke[i].base.genderAbility, &pidBase,
                                    heapId);
            rnd = poke[i].base.difficulty + poke[i].base.level + poke[i].base.species + trainerId;
            MATH_InitRand32(&rand, rnd);
            for (j = 0; j < data->trainerClass; j++) {
                rnd = MATH_Rand32(&rand, 0x10000);
            }
            rnd = (rnd << 8) + pidBase;
            iv = poke[i].base.difficulty * 31 / 255;
            PokeParty_CreatePkm(pkm, poke[i].base.species, poke[i].base.level, PKM_ID_NOT_SHINY,
                                PACK_IVS(iv, iv, iv, iv, iv, iv), rnd);
            for (j = 0; j < 4; j++) {
                PokeParty_SetMove(pkm, poke[i].moves[j], j);
            }
            TrainerUtil_SetupPkm(trainerId, pkm, poke[i].base.form, poke[i].base.genderAbility);
            PokeParty_AddPkm(party, pkm);
        }
        break;
    }
    case PARTY_ITEMS: {
        TrainerPokeItem *poke = pokes;

        for (i = 0; i < data->pokeCount; i++) {
#ifdef BUGFIX
            pidBase = trainerPidBase;
#endif
            TrainerUtil_CalcBasePID(poke[i].base.species, poke[i].base.form, poke[i].base.genderAbility, &pidBase,
                                    heapId);
            rnd = poke[i].base.difficulty + poke[i].base.level + poke[i].base.species + trainerId;
            MATH_InitRand32(&rand, rnd);
            for (j = 0; j < data->trainerClass; j++) {
                rnd = MATH_Rand32(&rand, 0x10000);
            }
            rnd = (rnd << 8) + pidBase;
            iv = poke[i].base.difficulty * 31 / 255;
            PokeParty_CreatePkm(pkm, poke[i].base.species, poke[i].base.level, PKM_ID_NOT_SHINY,
                                PACK_IVS(iv, iv, iv, iv, iv, iv), rnd);
            PokeParty_SetParam(pkm, PKM_PARAM_ITEM, poke[i].item);
            TrainerUtil_SetupPkm(trainerId, pkm, poke[i].base.form, poke[i].base.genderAbility);
            PokeParty_AddPkm(party, pkm);
        }
        break;
    }
    case PARTY_MOVES | PARTY_ITEMS: {
        TrainerPokeItemMoves *poke = pokes;

        for (i = 0; i < data->pokeCount; i++) {
#ifdef BUGFIX
            pidBase = trainerPidBase;
#endif
            TrainerUtil_CalcBasePID(poke[i].base.species, poke[i].base.form, poke[i].base.genderAbility, &pidBase,
                                    heapId);
            rnd = poke[i].base.difficulty + poke[i].base.level + poke[i].base.species + trainerId;
            MATH_InitRand32(&rand, rnd);
            for (j = 0; j < data->trainerClass; j++) {
                rnd = MATH_Rand32(&rand, 0x10000);
            }
            rnd = (rnd << 8) + pidBase;
            iv = poke[i].base.difficulty * 31 / 255;
            PokeParty_CreatePkm(pkm, poke[i].base.species, poke[i].base.level, PKM_ID_NOT_SHINY,
                                PACK_IVS(iv, iv, iv, iv, iv, iv), rnd);
            PokeParty_SetParam(pkm, PKM_PARAM_ITEM, poke[i].item);
            for (j = 0; j < 4; j++) {
                PokeParty_SetMove(pkm, poke[i].moves[j], j);
            }
            TrainerUtil_SetupPkm(trainerId, pkm, poke[i].base.form, poke[i].base.genderAbility);
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
        PersonalData *personal = PML_PersonalLoad(species, form, heapId);

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

// Sets what CreatePkm doesn't: the friendship, 0 if the Pokémon knows Frustration, the form and the ability
static void TrainerUtil_SetupPkm(int trainerId, PartyPkm *pkm, u32 form, u8 genderAbility) {
    u16 species;
    u8 happiness = 255;
    int i;
    u32 ability;

    species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    for (i = 0; i < 4; i++) {
        if (PokeParty_GetParam(pkm, PKM_PARAM_MOVE1 + i, NULL) == MOVE_FRUSTRATION) {
            happiness = 0;
        }
    }
    PokeParty_SetParam(pkm, PKM_PARAM_HAPPINESS, happiness);
    PokeParty_SetParam(pkm, PKM_PARAM_FORM, form);

    ability = genderAbility & 0xf0;
    if (ability == 0x30) {
        PokeParty_SetHiddenAbil(pkm, species, form);
    } else if (ability != 0) {
        u32 param = PERSONAL_ABILITY_1;

        if (PML_PersonalGetParamSingle(species, form, PERSONAL_ABILITY_2) != 0 && ability == 0x20) {
            param = PERSONAL_ABILITY_2;
        }
        PokeParty_SetParam(pkm, PKM_PARAM_ABILITY, PML_PersonalGetParamSingle(species, form, param));
    }
    PokeParty_SetNature(pkm, (u8)(((int)PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL) >> 8) % 25));
}

static u8 TrainerUtil_IsIDBattleInst(int trainerId) {
    return trainerId >= TRAINER_ID_BATTLE_INST_START;
}
