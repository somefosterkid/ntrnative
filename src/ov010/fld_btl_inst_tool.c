// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/fld_btl_inst_tool.c, ported by tools/port_file.py.
// The battle facilities' tools: their trainers, Pokémon and battles, for the Battle Subway and the Trial House.
// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "constants/arc.h"
#include "battle/btl_setup.h"
#include "constants/pokemon.h"
#include "constants/text_banks.h"
#include "field/battle_facility.h"
#include "field/bsubway_scr.h"
#include "field/field.h"
#include "field/field_event.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net_handle.h"
#include "gfl/net_system.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/math.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "system/game_data.h"
#include "system/game_system.h"

#define MOVE_FRUSTRATION 218

// The clients of a battle setup: the player, the trainer, the partner and the second trainer
typedef enum {
    CLIENT_PLAYER,
    CLIENT_ENEMY,
    CLIENT_PARTNER,
    CLIENT_ENEMY_2,
} SetupClient;

void genSubwayBtlInstitutePoke(const BSubwayPokemon *src, PartyPkm *pkm);
static void func_ov010_02168800(BSubwayPokemonData *data, u32 file);

static u16 func_ov010_02168824(void);
u8 func_ov010_0216883c(u32 trainerId);
BOOL func_ov010_02168ac8(const BSubwayTrainer *trainer);

// The items of the rental Pokémon
static const union { u8 raw[8]; u16 t[4]; } data_ov010_02172084_raw = { {
#include "asset/fld_btl_inst_tool_data_ov010_02172084.inc"
} };
typedef char data_ov010_02172084_fits[sizeof(data_ov010_02172084_raw.t) <= sizeof(data_ov010_02172084_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout
#define data_ov010_02172084 (data_ov010_02172084_raw.t)

// What each trainer class gives
static const struct {
    u16 trainerClass;
    u16 value;
} data_ov010_0217208c[] = {
#include "asset/fld_btl_inst_tool_data_ov010_0217208c.inc"
};

BtlSetup *SetupTrialHouseBattle(GameSystem *gsys, PokeParty *party, u32 mode, BSubwayTrainer *trainers,
                                BSubwayTrainer *partner, int count) {
    u16 type;
    BtlSetup *setup;
    GameData *gameData;
    BtlFieldStatus status;
    HeapID heapId;
    NetHandle *handle;
    int netId;
    SetupClient client;
    PlayerInfo *info;
    PokeParty *playerParty;
    PokeParty *enemyParty;
    PokeParty *enemy2Party;
    PokeParty *partnerParty;
    PartyPkm *pkm;
    PartyPkm *srcPkm;
    BtlSetupTrainer *trainer;
    BSubwayTrainer *src;
    int i;

    gameData = GSYS_GetGameData(gsys);
    type = mode;
    SaveBtlFieldStatus(&status, gameData, GSYS_GetField(gsys));
    heapId = 4;
    setup = BtlSetup_Create(4);
    switch (type) {
    case 0:
    case 4:
    case 5:
        BtlSetup_SetTrainer1v1Single(setup, gameData, &status, 0, heapId);
        break;
    case 1:
    case 6:
        BtlSetup_SetTrainer1v1Double(setup, gameData, &status, 0, heapId);
        break;
    case 2:
    case 7:
        BtlSetup_SetTrainer2v2(setup, gameData, &status, 0, 0, 0, heapId);
        break;
    case 3:
    case 8:
        handle = func_020399a8();
        netId = 0;
        if (func_02039338()) {
            netId = 2;
        }
        BtlSetup_SetNetMultiVsAI(setup, gameData, handle, 1, netId, 0, 0, 4);
        break;
    }
    func_02013980(setup, 4);

    client = CLIENT_PLAYER;
    info = GetGameDataPlayerInfo(gameData);
    setup->unk34[client] = info;
    playerParty = setup->party[client];
    PokeParty_InitCore(playerParty, 6);
    pkm = GFL_HeapAllocate(HEAPID_TAIL(4), PokeParty_GetPkmRawSize(), FALSE, "fld_btl_inst_tool.c", 595);
    PokeParty_ClearPkm(pkm);
    for (i = 0; i < count; i++) {
        srcPkm = PokeParty_GetPkm(party, i);
        copyPartyPkm(srcPkm, pkm);
        if (PokeParty_GetParam(srcPkm, PKM_PARAM_LEVEL, NULL) != 50) {
            setLevel(pkm, 50);
        }
        PokeParty_AddPkm(playerParty, pkm);
    }
    GFL_HeapFree(pkm);

    client = CLIENT_ENEMY;
    src = trainers;
    trainer = setup->trainers[client];
    trainer->trainerId = src->unk00;
    trainer->trainerClass = src->trainerId;
    trainer->aiFlags = 0x87;
    GFL_StrBufLoadString(trainer->name, src->name);
    func_0202428c(&trainer->unk18);
    func_0202428c(&trainer->unk20);
    if (type == 4) {
        trainer->unk18 = *(const PMSData *)src->winWords;
        trainer->unk20 = *(const PMSData *)src->loseWords;
    } else if (func_ov010_02168ac8(src)) {
        trainer->unk18 = *(const PMSData *)src->winWords;
        trainer->unk20 = *(const PMSData *)src->loseWords;
    }
    enemyParty = setup->party[client];
    PokeParty_InitCore(enemyParty, 6);
    pkm = GFL_HeapAllocate(HEAPID_TAIL(4), PokeParty_GetPkmRawSize(), FALSE, "fld_btl_inst_tool.c", 670);
    for (i = 0; i < count; i++) {
        genSubwayBtlInstitutePoke(&src->pokemon[i], pkm);
        PokeParty_AddPkm(enemyParty, pkm);
    }
    GFL_HeapFree(pkm);

    if (setup->fieldSituation.unk1a != 0) {
        client = CLIENT_ENEMY_2;
        trainer = setup->trainers[client];
        src = &trainers[1];
        trainer->trainerId = src->unk00;
        trainer->trainerClass = src->trainerId;
        trainer->aiFlags = 0x87;
        GFL_StrBufLoadString(trainer->name, src->name);
        enemy2Party = setup->party[client];
        PokeParty_InitCore(enemy2Party, 6);
        pkm = GFL_HeapAllocate(HEAPID_TAIL(4), PokeParty_GetPkmRawSize(), FALSE, "fld_btl_inst_tool.c", 708);
        for (i = 0; i < count; i++) {
            genSubwayBtlInstitutePoke(&src->pokemon[i], pkm);
            PokeParty_AddPkm(enemy2Party, pkm);
        }
        GFL_HeapFree(pkm);
    }

    if (setup->fieldSituation.unk1a == 3) {
        client = CLIENT_PARTNER;
        src = partner;
        trainer = setup->trainers[client];
        trainer->trainerId = src->unk00;
        trainer->trainerClass = src->trainerId;
        trainer->aiFlags = 0x87;
        GFL_StrBufLoadString(trainer->name, src->name);
        func_0202428c(&trainer->unk18);
        func_0202428c(&trainer->unk20);
        partnerParty = setup->party[client];
        PokeParty_InitCore(partnerParty, 6);
        pkm = GFL_HeapAllocate(HEAPID_TAIL(4), PokeParty_GetPkmRawSize(), FALSE, "fld_btl_inst_tool.c", 755);
        for (i = 0; i < count; i++) {
            genSubwayBtlInstitutePoke(&src->pokemon[i], pkm);
            PokeParty_AddPkm(partnerParty, pkm);
        }
        GFL_HeapFree(pkm);
    }
    func_0201394c(setup);
    return setup;
}

u32 func_ov010_021684e8(BSubwayPokemon *pkm, u16 file, u32 id, u32 pid, u8 iv, u8 index, BOOL rentalItem,
                        HeapID heapId) {
    BSubwayPokemonData data;
    int i;
    int count;
    int ev;
    u8 happiness;
    u32 ability;
    MsgData *msgData;

    MI_CpuFill8(pkm, 0, sizeof(BSubwayPokemon));
    func_ov010_02168800(&data, file);
    pkm->species = data.species;
    pkm->form = data.form;
    if (rentalItem) {
        pkm->item = data_ov010_02172084[index];
    } else {
        pkm->item = data.item;
    }
    happiness = 0xff;
    for (i = 0; i < 4; i++) {
        pkm->moves[i] = data.moves[i];
        if (data.moves[i] == MOVE_FRUSTRATION) {
            happiness = 0;
        }
    }
    pkm->id = id;
    if (pid == 0) {
        pid = PML_GenPID(id, data.species, data.form, 2, 2, 0);
        pkm->personality = pid;
    } else {
        pkm->personality = pid;
    }
    pkm->nature = data.nature;
    pkm->ivs.stat.hp = iv;
    pkm->ivs.stat.attack = iv;
    pkm->ivs.stat.defense = iv;
    pkm->ivs.stat.speed = iv;
    pkm->ivs.stat.spAttack = iv;
    pkm->ivs.stat.spDefense = iv;
    count = 0;
    for (i = 0; i < 6; i++) {
        if (data.evFlags & (1 << i)) {
            count++;
        }
    }
    ev = 510 / count;
    if (ev > 255) {
        ev = 255;
    }
    for (i = 0; i < 6; i++) {
        if (data.evFlags & (1 << i)) {
            pkm->evs[i] = ev;
        }
    }
    pkm->ppUps = 0;
    pkm->region = 0;
    ability = PML_PersonalGetParamSingle(pkm->species, 0, 0x1b);
    if (ability != 0) {
        if (pkm->personality & 1) {
            pkm->ability = ability;
        } else {
            pkm->ability = PML_PersonalGetParamSingle(pkm->species, 0, 0x1a);
        }
    } else {
        pkm->ability = PML_PersonalGetParamSingle(pkm->species, 0, 0x1a);
    }
    pkm->happiness = happiness;
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_SPECIES_NAMES, heapId);
    GFL_MsgDataLoadRawStr(msgData, pkm->species, pkm->nickname, NELEMS(pkm->nickname));
    GFL_MsgDataFree(msgData);
    return pid;
}

void genSubwayBtlInstitutePoke(const BSubwayPokemon *src, PartyPkm *pkm) {
    StrBuf *strbuf;
    const u16 *name;
    u16 nickname[11];
    int i;
    u8 pp;
    u16 terminator;

    PokeParty_ClearPkm(pkm);
    PokeParty_CreatePkm(pkm, src->species, 50, PKM_ID_RANDOM, src->ivs.all & 0x3fffffff, src->personality);
    PokeParty_SetParam(pkm, PKM_PARAM_FORM, (u8)src->form);
    PokeParty_SetParam(pkm, PKM_PARAM_ITEM, src->item);
    for (i = 0; i < 4; i++) {
        PokeParty_SetParam(pkm, PKM_PARAM_MOVE1 + i, src->moves[i]);
        PokeParty_SetParam(pkm, PKM_PARAM_MOVE1_PP_UP + i, (u8)((src->ppUps >> (i * 2)) & 3));
        pp = PokeParty_GetParam(pkm, PKM_PARAM_MOVE1_MAX_PP + i, NULL);
        PokeParty_SetParam(pkm, PKM_PARAM_MOVE1_PP + i, pp);
    }
    PokeParty_SetParam(pkm, PKM_PARAM_ID, src->id);
    PokeParty_SetParam(pkm, PKM_PARAM_EV_HP, src->evs[0]);
    PokeParty_SetParam(pkm, PKM_PARAM_EV_HP + 1, src->evs[1]);
    PokeParty_SetParam(pkm, PKM_PARAM_EV_HP + 2, src->evs[2]);
    PokeParty_SetParam(pkm, PKM_PARAM_EV_HP + 3, src->evs[3]);
    PokeParty_SetParam(pkm, PKM_PARAM_EV_HP + 4, src->evs[4]);
    PokeParty_SetParam(pkm, PKM_PARAM_EV_HP + 5, src->evs[5]);
    PokeParty_SetParam(pkm, PKM_PARAM_ABILITY, src->ability);
    PokeParty_SetParam(pkm, PKM_PARAM_HAPPINESS, src->happiness);
    PokeParty_SetNature(pkm, src->nature);
    strbuf = GFL_StrBufCreate(NELEMS(nickname), HEAPID_GAMEEVENT);
    terminator = GFL_StrBufGetTerminator();
    name = src->nickname;
    for (i = 0; i < 11; i++) {
        nickname[i] = name[i];
    }
    nickname[i - 1] = terminator;
    GFL_StrBufLoadString(strbuf, nickname);
    PokeParty_SetParam(pkm, PKM_PARAM_NICKNAME, (uintptr_t)strbuf);
    GFL_StrBufFree(strbuf);
    PokeParty_SetParam(pkm, PKM_PARAM_REGION, src->region);
    PokeParty_SetParam(pkm, PKM_PARAM_LEVEL, PokeParty_GetLevel(pkm));
}

static void func_ov010_02168800(BSubwayPokemonData *data, u32 file) {
    GFL_ArcSysRead(data, 0xd6, file);
}

u16 *func_ov010_0216880c(u16 trainerId, HeapID heapId) {
    return GFL_ArcSysReadHeapNewLZ(0xd7, (u16)(trainerId + 1), FALSE, heapId);
}

static u16 func_ov010_02168824(void) {
    return GFL_RandomLC(0xffffffff) / (0xffffffff / 0x10000);
}

u8 func_ov010_0216883c(u32 trainerId) {
    if (trainerId < 50) {
        return 3;
    }
    if (trainerId < 70) {
        return 6;
    }
    if (trainerId < 90) {
        return 9;
    }
    if (trainerId < 110) {
        return 12;
    }
    if (trainerId < 160) {
        return 15;
    }
    if (trainerId < 180) {
        return 18;
    }
    if (trainerId < 200) {
        return 21;
    }
    return 31;
}

// pokebw2's func_ov012_02162864
BOOL func_ov010_02168878(BSubwayTrainer *trainer, u16 trainerId, u32 count, const u16 *species, const u16 *items,
                         BSubwayTeamConfig *config, HeapID heapId) {
    void *trainerData = func_ov010_021688bc(trainer, trainerId, heapId);
    BOOL result = func_ov010_02168924(trainerData, trainerId, trainer->pokemon, count, species, items, config, heapId);

    GFL_HeapFree(trainerData);
    return result;
}

void *func_ov010_021688bc(BSubwayTrainer *trainer, u16 trainerId, HeapID heapId) {
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, 0xb, heapId);
    u16 *trainerData;
    StrBuf *name;

    MI_CpuFill8(trainer, 0, sizeof(BSubwayTrainer));
    trainerData = func_ov010_0216880c(trainerId, heapId);
    trainer->unk00 = trainerId + 1;
    trainer->message.type = 0xffff;
    trainer->message.id = trainerId * 3;
    trainer->trainerId = trainerData[0];
    name = GFL_MsgDataLoadStrbufNew(msgData, trainerId);
    GFL_StrBufStoreString(name, trainer->name, sizeof(trainer->name));
    GFL_StrBufFree(name);
    GFL_MsgDataFree(msgData);
    return trainerData;
}

BOOL func_ov010_02168924(const u16 *trainerData, u16 trainerId, BSubwayPokemon *pkms, u8 count, const u16 *species,
                         const u16 *items, BSubwayTeamConfig *config, HeapID heapId) {
    u32 files[4];
    u32 pids[4];
    BSubwayPokemonData other;
    BSubwayPokemonData data;
    u8 natures[4];
    u8 iv;
    u32 seed;
    u32 file;
    u8 index;
    int i;
    int n;
    int retries;
    BOOL failed;

    failed = FALSE;
    retries = 0;
    n = 0;
    while (n != count) {
        index = func_ov010_02168824() % trainerData[1];
        file = trainerData[2 + index];
        func_ov010_02168800(&data, file);
        for (i = 0; i < n; i++) {
            func_ov010_02168800(&other, files[i]);
            if (other.species == data.species) {
                break;
            }
        }
        if (i != n) {
            continue;
        }
        if (species != NULL) {
            for (i = 0; i < count; i++) {
                if (data.species == species[i]) {
                    break;
                }
            }
            if (i != count) {
                continue;
            }
        }
        if (retries < 50) {
            for (i = 0; i < n; i++) {
                func_ov010_02168800(&other, files[i]);
                if (other.item != 0 && other.item == data.item) {
                    break;
                }
            }
            if (i != n) {
                retries++;
                continue;
            }
            if (items != NULL) {
                for (i = 0; i < count; i++) {
                    if (data.item == items[i] && items[i] != 0) {
                        break;
                    }
                }
                if (i != count) {
                    retries++;
                    continue;
                }
            }
        }
        files[n] = file;
        natures[n] = data.nature;
        n++;
    }
    iv = func_ov010_0216883c(trainerId);
    seed = func_ov010_02168824() | (func_ov010_02168824() << 16);
    if (retries >= 50) {
        failed = TRUE;
    }
    for (i = 0; i < n; i++) {
        pids[i] = func_ov010_021684e8(&pkms[i], files[i], seed, 0, iv, i, failed, heapId);
    }
    if (config == NULL) {
        return failed;
    }
    config->id = seed;
    for (i = 0; i < 2; i++) {
        config->files[i] = files[i];
        config->pids[i] = pids[i];
        config->natures[i] = natures[i];
    }
    return failed;
}

BOOL func_ov010_02168ac8(const BSubwayTrainer *trainer) {
    int i;

    for (i = 0; i < 4; i++) {
        if (trainer->winWords[i] != 0) {
            return TRUE;
        }
        if (trainer->loseWords[i] != 0) {
            return TRUE;
        }
    }
    return FALSE;
}

u32 func_ov010_02168aec(u32 trainerClass) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov010_0217208c); i++) {
        if (trainerClass == data_ov010_0217208c[i].trainerClass) {
            return data_ov010_0217208c[i].value;
        }
    }
    return 10;
}
