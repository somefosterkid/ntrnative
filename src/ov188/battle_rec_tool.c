// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov273/battle_rec_tool.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// battle_rec_tool.c (overlay 273, the name its allocations pass): stores a battle's setup in the battle video that is
// loaded, or in Pokéstar Studios' block, and loads it back, converting the parties, the clients and the setup's
// parameters to the form they are kept in

#include "types.h"
#include "battle/battle_rec_tool.h"
#include "battle/btl_setup.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "pml/poke_party.h"
#include "save/config.h"
#include "save/player_info.h"
#include "constants/pokemon.h"
#include "save/pokewood.h"

void func_ov188_021ee920(PartyPkm *pkm, BattleRecPkm *rec);
static void func_ov188_021eeed8(PokeParty *party, BattleRecParty *rec);
static void func_ov188_021eef2c(BattleRecParty *rec, PokeParty *party, HeapID heapId);
static void func_ov188_021eefa4(const BtlSetup *setup, BattleRecBody *body);
static void func_ov188_021eefe4(BtlSetup *setup, BattleRecBody *body, HeapID heapId);
static void BattleRecTool_StoreClients(const BtlSetup *setup, BattleRecClient *clients, u32 count);
static void BattleRecTool_StorePokewoodClients(const BtlSetup *setup, BattleRecClient *clients, u32 count,
                                               u16 *trainerClass);
static void BattleRecTool_LoadClients(BtlSetup *setup, BattleRecClient *clients, u32 count);
static void BattleRecTool_LoadPokewoodClients(BtlSetup *setup, const BattleRecClient *clients, u32 count,
                                              u16 trainerClass);
void func_ov188_021ef0f4(const BtlSetupTrainer *trainer, BattleRecTrainer *rec);
static void func_ov188_021ef140(BtlSetupTrainer *trainer, const BattleRecTrainer *rec);
static BOOL BattleRecTool_StoreRecData(const BtlSetup *setup, BattleRecBody *body);
static BOOL BattleRecTool_LoadRecData(BtlSetup *setup, const BattleRecBody *body);
static BOOL BattleRecTool_StoreSetupParams(const BtlSetup *setup, BattleRecSetup *rec);
static BOOL BattleRecTool_LoadSetupParams(BtlSetup *setup, const BattleRecSetup *rec);
static void BattleRecTool_StorePokewoodParty(const BtlSetup *setup, PokewoodBlock *block);
static void BattleRecTool_LoadPokewoodParty(BtlSetup *setup, PokewoodBlock *block, HeapID heapId);
static BOOL BattleRecTool_StorePokewoodRecData(const BtlSetup *setup, PokewoodBlock *block);
static BOOL BattleRecTool_LoadPokewoodRecData(BtlSetup *setup, const PokewoodBlock *block);
static void BattleRecTool_StorePokewoodPartyCore(PokeParty *party, BattleRecParty *rec);
static void BattleRecTool_LoadPokewoodPartyCore(BattleRecParty *rec, PokeParty *party, HeapID heapId);
static void BattleRecTool_StorePokewoodInfo(const BtlSetup *setup, PokewoodBattleInfo *info);
static void BattleRecTool_LoadPokewoodInfo(BtlSetup *setup, const PokewoodBattleInfo *info);
static void BattleRecTool_StorePokewoodExtra(const BtlSetup *setup, PokewoodBlock *block);
static void BattleRecTool_LoadPokewoodExtra(BtlSetup *setup, const PokewoodBlock *block);

// The smaller of two numbers, a on a tie
static inline int BattleRecTool_Min(int a, int b) {
    return a <= b ? a : b;
}

// pokebw2's BattleRecTool_StoreParty

extern BattleRecVideo *data_0214616c;

void func_ov188_021ef014(BtlSetup *setup, BattleRecBody *body);
BOOL func_ov188_021ef1ac(const BtlSetup *setup, BattleRecBody *body);
BOOL func_ov188_021ef1f4(BtlSetup *setup, BattleRecBody *body);


void func_ov188_021ee920(PartyPkm *pkm, BattleRecPkm *rec) {
    PkmBlockA *blockA;
    PkmBlockB *blockB;
    PkmBlockC *blockC;
    PkmBlockD *blockD;
    int i;

    MI_CpuFill8(rec, 0, sizeof(BattleRecPkm));
    if (!pkm->base.partyDecrypted) {
        PML_CryptoRun(&pkm->statusCond, sizeof(PartyPkm) - offsetof(PartyPkm, statusCond), pkm->base.pid);
        PML_CryptoRun(&pkm->base.contentBuffer, sizeof(PkmBuffer), pkm->base.checksum);
    }
    blockA = PML_PkmGetParamBlockCore(&pkm->base, pkm->base.pid, 0);
    blockB = PML_PkmGetParamBlockCore(&pkm->base, pkm->base.pid, 1);
    blockC = PML_PkmGetParamBlockCore(&pkm->base, pkm->base.pid, 2);
    blockD = PML_PkmGetParamBlockCore(&pkm->base, pkm->base.pid, 3);

    rec->pid = pkm->base.pid;
    rec->partyDecrypted = FALSE;
    rec->boxDecrypted = FALSE;
    rec->badEgg = pkm->base.badEgg;
    rec->nature = blockB->nature;

    rec->species = blockA->species;
    rec->item = blockA->item;
    rec->id = blockA->id;
    rec->exp = blockA->exp;
    rec->friendship = blockA->friendship;
    rec->ability = blockA->ability;
    rec->hpEV = blockA->hpEV;
    rec->atkEV = blockA->atkEV;
    rec->defEV = blockA->defEV;
    rec->speEV = blockA->speEV;
    rec->spaEV = blockA->spaEV;
    rec->spdEV = blockA->spdEV;
    rec->language = blockA->language;

    for (i = 0; i < 4; i++) {
        rec->moves[i] = blockB->moves[i];
        rec->pp[i] = blockB->pp[i];
        rec->ppUps[i] = blockB->ppUps[i];
    }
    rec->hpIV = blockB->hpIV;
    rec->atkIV = blockB->atkIV;
    rec->defIV = blockB->defIV;
    rec->speIV = blockB->speIV;
    rec->spaIV = blockB->spaIV;
    rec->spdIV = blockB->spdIV;
    rec->isEgg = blockB->isEgg;
    rec->isNicknamed = blockB->isNicknamed;
    rec->fatefulEncounter = blockB->fatefulEncounter;
    rec->gender = blockB->gender;
    rec->form = blockB->form;

    for (i = 0; i < 11; i++) {
        rec->nickname[i] = blockC->nickname[i];
    }

    for (i = 0; i < 8; i++) {
        rec->otName[i] = blockD->otName[i];
    }
    rec->ball = blockD->ball;

    rec->statusCond = pkm->statusCond;
    rec->level = pkm->level;
    rec->pokestarFame = pkm->unk8D;
    rec->nowHP = pkm->nowHP;
    rec->maxHP = pkm->maxHP;
    rec->atk = pkm->atk;
    rec->def = pkm->def;
    rec->spe = pkm->spe;
    rec->spa = pkm->spa;
    rec->spd = pkm->spd;

    if (!pkm->base.partyDecrypted) {
        PML_CryptoRun(&pkm->statusCond, sizeof(PartyPkm) - offsetof(PartyPkm, statusCond), pkm->base.pid);
        PML_CryptoRun(&pkm->base.contentBuffer, sizeof(PkmBuffer), pkm->base.checksum);
    }
}

void func_ov188_021eebf0(BattleRecPkm *rec, PartyPkm *pkm) {
    PkmBlockA *blockA;
    PkmBlockB *blockB;
    PkmBlockC *blockC;
    PkmBlockD *blockD;
    int i;

    MI_CpuFill8(pkm, 0, sizeof(PartyPkm));
    blockA = PML_PkmGetParamBlockCore(&pkm->base, rec->pid, 0);
    blockB = PML_PkmGetParamBlockCore(&pkm->base, rec->pid, 1);
    blockC = PML_PkmGetParamBlockCore(&pkm->base, rec->pid, 2);
    blockD = PML_PkmGetParamBlockCore(&pkm->base, rec->pid, 3);

    pkm->base.pid = rec->pid;
    pkm->base.partyDecrypted = FALSE;
    pkm->base.boxDecrypted = FALSE;
    pkm->base.badEgg = rec->badEgg;

    blockA->species = rec->species;
    blockA->item = rec->item;
    blockA->id = rec->id;
    blockA->exp = rec->exp;
    blockA->friendship = rec->friendship;
    blockA->ability = rec->ability;
    blockA->hpEV = rec->hpEV;
    blockA->atkEV = rec->atkEV;
    blockA->defEV = rec->defEV;
    blockA->speEV = rec->speEV;
    blockA->spaEV = rec->spaEV;
    blockA->spdEV = rec->spdEV;
    blockA->language = rec->language;

    for (i = 0; i < 4; i++) {
        blockB->moves[i] = rec->moves[i];
        blockB->pp[i] = rec->pp[i];
        blockB->ppUps[i] = rec->ppUps[i];
    }
    blockB->hpIV = rec->hpIV;
    blockB->atkIV = rec->atkIV;
    blockB->defIV = rec->defIV;
    blockB->speIV = rec->speIV;
    blockB->spaIV = rec->spaIV;
    blockB->spdIV = rec->spdIV;
    blockB->isEgg = rec->isEgg;
    blockB->isNicknamed = rec->isNicknamed;
    blockB->fatefulEncounter = rec->fatefulEncounter;
    blockB->gender = rec->gender;
    blockB->form = rec->form;
    blockB->nature = rec->nature;

    for (i = 0; i < 11; i++) {
        blockC->nickname[i] = rec->nickname[i];
    }

    for (i = 0; i < 8; i++) {
        blockD->otName[i] = rec->otName[i];
    }
    blockD->ball = rec->ball;

    pkm->statusCond = rec->statusCond;
    pkm->level = rec->level;
    pkm->unk8D = rec->pokestarFame;
    pkm->nowHP = rec->nowHP;
    pkm->maxHP = rec->maxHP;
    pkm->atk = rec->atk;
    pkm->def = rec->def;
    pkm->spe = rec->spe;
    pkm->spa = rec->spa;
    pkm->spd = rec->spd;

    PML_CryptoRun(&pkm->statusCond, sizeof(PartyPkm) - offsetof(PartyPkm, statusCond), pkm->base.pid);
    pkm->base.checksum = PML_CryptoGenKey(&pkm->base.contentBuffer, sizeof(PkmBuffer));
    PML_CryptoRun(&pkm->base.contentBuffer, sizeof(PkmBuffer), pkm->base.checksum);
}

void func_ov188_021eee78(BtlSetup *setup) {
    BattleRecBody *body = &data_0214616c->body;

    func_ov188_021eefa4(setup, body);
    func_ov188_021ef014(setup, body);
    func_ov188_021ef1ac(setup, body);
    func_ov188_021ef1f4(setup, body);
}

void func_ov188_021ef090(BtlSetup *setup, BattleRecBody *body);
BOOL func_ov188_021ef1d8(BtlSetup *setup, BattleRecBody *body);
BOOL func_ov188_021ef2d4(BtlSetup *setup, const BattleRecBody *body);
void func_ov188_021eeea8(BtlSetup *setup, HeapID heapId) {
    BattleRecBody *body = &data_0214616c->body;

    func_ov188_021eefe4(setup, body, heapId);
    func_ov188_021ef090(setup, body);
    func_ov188_021ef1d8(setup, body);
    func_ov188_021ef2d4(setup, body);
}


static void func_ov188_021eeed8(PokeParty *party, BattleRecParty *rec) {
    int i;

    MI_CpuFill8(rec, 0, sizeof(BattleRecParty));
    if (PokeParty_GetPkmCount(party) != 0) {
        rec->capacity = PokeParty_GetCapacity(party);
        rec->count = PokeParty_GetPkmCount(party);
        for (i = 0; i < rec->count; i++) {
            func_ov188_021ee920(PokeParty_GetPkm(party, i), &rec->pkm[i]);
        }
    }
}

// pokebw2's BattleRecTool_LoadParty
static void func_ov188_021eef2c(BattleRecParty *rec, PokeParty *party, HeapID heapId) {
    int i;
    PartyPkm *pkm = GFL_HeapAllocate(HEAPID_TAIL(heapId), PokeParty_GetPkmRawSize(), TRUE, "battle_rec_tool.c", 173);

    PokeParty_InitCore(party, rec->capacity);
    for (i = 0; i < rec->count; i++) {
        func_ov188_021eebf0(&rec->pkm[i], pkm);
        PokeParty_SetParam(pkm, 0x9f, 0);
        PokeParty_AddPkm(party, pkm);
    }
    GFL_HeapFree(pkm);
}

// pokebw2's BattleRecTool_StoreParties
static void func_ov188_021eefa4(const BtlSetup *setup, BattleRecBody *body) {
    u32 i;

    MI_CpuFill8(body->parties, 0, sizeof(body->parties));
    for (i = 0; i < 4; i++) {
        if (setup->party[i] != NULL) {
            func_ov188_021eeed8(setup->party[i], &body->parties[i]);
        }
    }
}

// pokebw2's BattleRecTool_LoadParties
static void func_ov188_021eefe4(BtlSetup *setup, BattleRecBody *body, HeapID heapId) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (setup->party[i] != NULL) {
            func_ov188_021eef2c(&body->parties[i], setup->party[i], heapId);
        }
    }
}

void func_ov188_021ef014(BtlSetup *setup, BattleRecBody *body) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (setup->trainers[i] != NULL) {
            func_ov188_021ef0f4(setup->trainers[i], &body->clients[i].info.trainer);
            body->clients[i].type = 2;
        } else if (setup->unk34[i] != NULL) {
            func_02008494(setup->unk34[i], &body->clients[i].info.player);
            body->clients[i].type = 1;
        } else {
            body->clients[i].type = 0;
        }
        body->clients[i].unk2 = setup->unk44[i];
    }
}


void func_ov188_021ef090(BtlSetup *setup, BattleRecBody *body) {
    u32 i;

    for (i = 0; i < 4; i++) {
        switch (body->clients[i].type) {
        case 1:
            func_02008494(&body->clients[i].info.player, setup->unk34[i]);
            break;
        case 2:
            func_ov188_021ef140(setup->trainers[i], &body->clients[i].info.trainer);
            break;
        }
        setup->unk44[i] = body->clients[i].unk2;
    }
}


// pokebw2's BattleRecTool_StoreTrainer
void func_ov188_021ef0f4(const BtlSetupTrainer *trainer, BattleRecTrainer *rec) {
    u32 i;

    rec->trainerId = trainer->trainerId;
    rec->trainerClass = trainer->trainerClass;
    rec->aiFlags = trainer->aiFlags;
    rec->unk30 = trainer->unk18;
    rec->unk38 = trainer->unk20;
    for (i = 0; i < 4; i++) {
        rec->items[i] = trainer->items[i];
    }
    GFL_StrBufStoreString(trainer->name, rec->name, 16);
}

// pokebw2's BattleRecTool_LoadTrainer
static void func_ov188_021ef140(BtlSetupTrainer *trainer, const BattleRecTrainer *rec) {
    u32 i;
    const u16 *str;

    trainer->trainerId = rec->trainerId;
    trainer->trainerClass = rec->trainerClass;
    trainer->aiFlags = rec->aiFlags;
    trainer->unk18 = rec->unk30;
    trainer->unk20 = rec->unk38;
    for (i = 0; i < 4; i++) {
        trainer->items[i] = rec->items[i];
    }
    GFL_StrBufLoadString(trainer->name, rec->name);
    str = GFL_StrBufGetStringPtr(trainer->name);
    while (*str != GFL_StrBufGetTerminator()) {
        str++;
    }
}

BOOL func_ov188_021ef1ac(const BtlSetup *setup, BattleRecBody *body) {
    if (setup->unk94 != NULL && setup->unk98 < sizeof(body->data)) {
        body->dataSize = setup->unk98;
        MI_CpuCopy8(setup->unk94, body->data, setup->unk98);
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov188_021ef1d8(BtlSetup *setup, BattleRecBody *body) {
    u32 size = body->dataSize;

    setup->unk98 = size;
    MI_CpuCopy8(body->data, setup->unk94, size);
    return TRUE;
}

BOOL func_ov188_021ef1f4(BtlSetup *setup, BattleRecBody *body) {
    body->setup.env = setup->fieldSituation.env;
    body->setup.rand = setup->rand;
    body->setup.bgm = setup->fieldSituation.bgm;
    body->setup.unk2E = setup->fieldSituation.unk12;
    body->setup.battleType = setup->battleType;
    body->setup.battleStyle = setup->battleStyle;
    body->setup.unk33_4 = setup->fieldSituation.unk1a;
    body->setup.unk30 = setup->unk86;
    body->setup.unk32_5 = setup->fieldSituation.unk19;
    body->setup.unk33_7 = setup->unk7F != 0;
    initConfig(setup->config, (Config *)body->setup.config);
    return TRUE;
}

BOOL func_ov188_021ef2d4(BtlSetup *setup, const BattleRecBody *body) {
    setup->fieldSituation.env = body->setup.env;
    setup->rand = body->setup.rand;
    setup->fieldSituation.bgm = body->setup.bgm;
    setup->fieldSituation.unk12 = body->setup.unk2E;
    setup->battleType = body->setup.battleType;
    setup->battleStyle = body->setup.battleStyle;
    setup->fieldSituation.unk1a = body->setup.unk33_4;
    setup->unk86 = body->setup.unk30;
    setup->fieldSituation.unk19 = body->setup.unk32_5;
    setup->unk7F = body->setup.unk33_7;
    return TRUE;
}





