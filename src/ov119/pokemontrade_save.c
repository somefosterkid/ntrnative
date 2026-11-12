// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov194/pokemontrade_save.c, ported by tools/port_file.py.
#include "types.h"
#include "app/pokemon_trade_local.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "field/unity_tower.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "pml/evolution.h"
#include "pml/item.h"
#include "pml/mail.h"
#include "pml/met_data.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/chatter.h"
#include "save/medal_box.h"
#include "save/player_info.h"
#include "save/pokedex.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "system/bmp_winframe.h"
#include "system/country_region.h"
#include "system/game_data.h"
#include "system/net_save.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "save/playtime.h"

// Writing the trade to the save: the copies of what it changes, kept to restore if the save fails, the trade itself,
// the evolution it may start and the saving between the two machines

static void func_ov119_021dfd30(PokemonTradeWork *wk, int timer);
void func_ov119_021dfd3c(PokemonTradeWork *wk);
void func_ov119_021dfd64(PokemonTradeWork *wk);
void func_ov119_021dfd7c(PokemonTradeWork *wk);
static void func_ov119_021dfe48(PokemonTradeWork *wk);
static void func_ov119_021dff40(PokemonTradeWork *wk);
static void func_ov119_021dffe0(PokemonTradeWork *wk);
static void func_ov119_021e0008(PokemonTradeWork *wk);
void func_ov119_021e0030(PokemonTradeWork *wk);
void func_ov119_021e0054(PokemonTradeWork *wk);
void *func_ov119_021e0138(GameData *gameData, HeapID heapId);
static void func_ov119_021e017c(GameData *gameData, void *copy);
void *func_ov119_021e01a0(GameData *gameData, HeapID heapId);
static void func_ov119_021e01e0(GameData *gameData, void *copy);
void *func_ov119_021e0200(GameData *gameData, HeapID heapId);
static void func_ov119_021e0240(GameData *gameData, void *copy);
void func_ov119_021e0260(PokemonTradeWork *wk);
void func_ov119_021e02e8(PokemonTradeWork *wk);
BOOL func_ov119_021e0410(PokemonTradeWork *wk, PokeParty *party, int slot);
static void func_ov119_021e049c(PokemonTradeWork *wk);
static void func_ov119_021e08e8(PokemonTradeWork *wk);
static void func_ov119_021e0958(PokemonTradeWork *wk);
static void func_ov119_021e0980(PokemonTradeWork *wk);
static void func_ov119_021e0a64(PokemonTradeWork *wk);
static void func_ov119_021e0ba4(PokemonTradeWork *wk);
static void func_ov119_021e0bc4(PokemonTradeWork *wk);
void func_ov119_021e0be8(PokemonTradeWork *wk);
void func_ov119_021e0c44(PokemonTradeWork *wk);
static void func_ov119_021e0c78(PokemonTradeWork *wk);
static void func_ov119_021e0cc0(PokemonTradeWork *wk);


static void func_ov119_021dfd30(PokemonTradeWork *wk, int timer) {
    wk->timer = timer;
}

void func_ov119_021dfd3c(PokemonTradeWork *wk) {
    if (wk->unk11F8 != 0) {
        GFL_SndBGMPop();
        wk->unk11F8--;
        GFL_SndBGMSetPaused(0);
        GFL_SndBGMFadeIn(0x3c);
    }
}

void func_ov119_021dfd64(PokemonTradeWork *wk) {
    GFL_SndBGMPop();
    wk->unk11F8--;
}

void func_ov119_021dfd7c(PokemonTradeWork *wk) {
    GFL_SndBGMPop();
    wk->unk11F8--;
    GFL_SndBGMSetPaused(0);
    GFL_SndBGMFadeIn(0x18);
}

void func_ov119_021dfda0(PokemonTradeWork *wk) {
    ClActUnit *pCVar1;

    func_0204b1cc(wk->clactUnit);
    func_0204a5ac();
    func_ov119_021e3e60(wk);
    pCVar1 = func_0204b100(0x154, 0, wk->heapId);
    wk->clactUnit = pCVar1;
    if (wk->type == 5) {
        func_ov119_021dfd3c(wk);
        wk->param->next = 3;
        func_ov119_021d8e84(wk, (void *)func_ov119_021d9434);
        return;
    }
    if (wk->type == 6) {
        func_ov119_021dfd3c(wk);
        wk->param->next = 3;
        func_ov119_021d8e84(wk, (void *)0x0);
        return;
    }
    func_ov119_021e10d8(wk);
    GFL_SndBGMSetPaused(1);
    GFL_SndBGMPush();
    wk->unk11F8 = wk->unk11F8 + 1;
    GFL_SndBGMFadeOut(6);
    wk->timer = 0;
    func_ov119_021d8e84(wk, (void *)func_ov119_021dfe48);
}

static void func_ov119_021dfe48(PokemonTradeWork *wk) {
    PartyPkm *pkm;
    if (wk->timer >= 6) {
        GFL_SndBGMPlay(SEQ_ME_POKEGET, 0xffff);
        GFL_SndBGMFadeIn(6);
        if (wk->type == 7) {
            GFL_MsgDataLoadStrbuf(wk->msgData, 153, wk->strbufTemplate);
        } else if (wk->type == 8) {
            GFL_MsgDataLoadStrbuf(wk->msgData, 156, wk->strbufTemplate);
        } else if (wk->unk11EE != 0) {
            GFL_MsgDataLoadStrbuf(wk->msgData, 131, wk->strbufTemplate);
        } else {
            GFL_MsgDataLoadStrbuf(wk->msgData, 48, wk->strbufTemplate);
        }
        if (func_ov119_021d8aa8(wk)) {
            pkm = func_ov119_021d8fa4(wk, 0);
        } else {
            pkm = func_ov119_021d8fa4(wk, 1);
        }
        loadPokemonNicknameToStrbuf(wk->wordSet, 1, pkm);
        copyVarForText(wk->wordSet, 0, wk->partnerInfo);
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, wk->strbufTemplate);
        wk->cursorImage = LoadCursorImageEndOfHeap(6, 15, 0, wk->heapId);
        func_ov119_021e0ff4(wk, TRUE, 0);
        func_ov119_021dfd30(wk, 0);
        GFL_BGSysSetBGEnabled(6, TRUE);
        wk->timer = 0;
        func_ov119_021d8e84(wk, func_ov119_021dff40);
    }
}

static void func_ov119_021dff40(PokemonTradeWork *wk) {
    PartyPkm *pkm;
    if (wk->timer >= 200) {
        if (func_ov119_021d8aa8(wk)) {
            pkm = func_ov119_021d8fa4(wk, 0);
        } else {
            pkm = func_ov119_021d8fa4(wk, 1);
        }
        loadPokemonNicknameToStrbuf(wk->wordSet, 1, pkm);
        copyVarForText(wk->wordSet, 0, wk->partnerInfo);
        if (wk->type != 7 && wk->type != 8) {
            if (wk->unk11EE != 0) {
                GFL_MsgDataLoadStrbuf(wk->msgData, 132, wk->strbufTemplate);
                GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, wk->strbufTemplate);
                func_ov119_021e0ff4(wk, TRUE, 0);
            } else {
                GFL_MsgDataLoadStrbuf(wk->msgData, 49, wk->strbufTemplate);
                GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, wk->strbufTemplate);
                func_ov119_021e0ff4(wk, TRUE, 0);
            }
        }
        func_ov119_021d8e84(wk, func_ov119_021dffe0);
    }
}

static void func_ov119_021dffe0(PokemonTradeWork *wk) {
    if (wk->timer >= 270) {
        func_ov119_021dfd7c(wk);
        func_ov119_021d8e84(wk, func_ov119_021e0008);
    }
}

static void func_ov119_021e0008(PokemonTradeWork *wk) {
    if (wk->timer >= 420) {
        func_ov119_021dfd30(wk, 0);
        func_ov119_021d8e84(wk, func_ov119_021e08e8);
    }
}

void func_ov119_021e0030(PokemonTradeWork *wk) {
    NetHandle *handle;
    BOOL BVar1;

    handle = func_020399a8();
    BVar1 = func_02039cb4(handle, 0x17, 8);
    if (BVar1 != 0) {
        func_ov119_021d8e84(wk, (void *)func_ov119_021e0980);
    }
}

void func_ov119_021e0054(PokemonTradeWork *wk) {
    UnityTowerVisitor profile;
    PartyPkm *sent;
    PartyPkm *received;
    SaveControl *saveControl;
    UnityTowerSurveySave *survey;
    NetHandle *netHandle;

    saveControl = GameData_GetSaveControl(wk->gameData);
    survey = getUnityTower_SurveySaveBlkAddrress(saveControl);
    if (func_ov119_021d8aa8(wk)) {
        sent = func_ov119_021d8fa4(wk, 0);
        received = func_ov119_021d8fa4(wk, 1);
    } else {
        sent = func_ov119_021d8fa4(wk, 1);
        received = func_ov119_021d8fa4(wk, 0);
    }
    func_02008494(GetGameDataPlayerInfo(wk->gameData), &profile.info);
    profile.sentSpecies = PokeParty_GetParam(sent, 5, NULL);
    profile.receivedSpecies = PokeParty_GetParam(received, 5, NULL);
    profile.hobby = getPlayerSurveys(survey);
    profile.countryCount = func_02009584(survey);
    profile.unk26_0 = func_0200960c(survey);
    if (func_ov119_021d8a24(wk)) {
        netHandle = func_020399a8();
        if (func_0203d3a8(netHandle, 0xc12, 0x28, &profile)) {
            netHandle = func_020399a8();
            func_02039c68(netHandle, 0x17, 8);
            func_ov119_021d8e84(wk, func_ov119_021e0030);
        }
    } else {
        func_ov119_021d8e84(wk, func_ov119_021e0c78);
    }
}


inline void *BackupMail(GameData *gameData, HeapID heapId) {
    void *copy = GFL_HeapAllocate(heapId, func_02008ffc(), FALSE, "pokemontrade_save.c", 355);
    u32 size = func_02008ffc();
    MI_CpuCopy8(func_02008fec(gameData), copy, size);
    return copy;
}

inline void *BackupRecords(GameData *gameData, HeapID heapId) {
    void *copy = GFL_HeapAllocate(heapId, func_02008c20(), FALSE, "pokemontrade_save.c", 386);
    u32 size = func_02008c20();
    MI_CpuCopy8(GameData_GetRecords(gameData), copy, size);
    return copy;
}

void *func_ov119_021e0138(GameData *gameData, HeapID heapId) {
    SaveControl *save;
    u32 size;
    void *dest;
    UnityTowerSurveySave *src;

    save = GameData_GetSaveControl(gameData);
    size = func_02009454();
    dest = GFL_HeapAllocate(heapId, size, 0, "pokemontrade_save.c", 0x1a0);
    size = func_02009454();
    src = getUnityTower_SurveySaveBlkAddrress(save);
    MI_CpuCopy8(src, dest, size);
    return dest;
}

static void func_ov119_021e017c(GameData *gameData, void *copy) {
    SaveControl *save;
    UnityTowerSurveySave *dest;
    u32 size;

    save = GameData_GetSaveControl(gameData);
    dest = getUnityTower_SurveySaveBlkAddrress(save);
    size = func_02009454();
    MI_CpuCopy8(copy, dest, size);
    GFL_HeapFree(copy);
}

void *func_ov119_021e01a0(GameData *gameData, HeapID heapId) {
    u32 size;
    void *dest;
    PokeDexSave *src;

    size = getSizeofPokedexData();
    dest = GFL_HeapAllocate(heapId, size, 0, "pokemontrade_save.c", 0x1bd);
    size = getSizeofPokedexData();
    src = GameData_GetPokedex(gameData);
    MI_CpuCopy8(src, dest, size);
    return dest;
}

static void func_ov119_021e01e0(GameData *gameData, void *copy) {
    PokeDexSave *dest;
    u32 size;

    dest = GameData_GetPokedex(gameData);
    size = getSizeofPokedexData();
    MI_CpuCopy8(copy, dest, size);
    GFL_HeapFree(copy);
}

void *func_ov119_021e0200(GameData *gameData, HeapID heapId) {
    u32 size;
    void *dest;
    PalPadFriendList *src;

    size = PalPadFriendList_GetSize();
    dest = GFL_HeapAllocate(heapId, size, 0, "pokemontrade_save.c", 0x1da);
    size = PalPadFriendList_GetSize();
    src = func_02012f14(gameData);
    MI_CpuCopy8(src, dest, size);
    return dest;
}

static void func_ov119_021e0240(GameData *gameData, void *copy) {
    PalPadFriendList *dest;
    u32 size;

    dest = func_02012f14(gameData);
    size = PalPadFriendList_GetSize();
    MI_CpuCopy8(copy, dest, size);
    GFL_HeapFree(copy);
}

void func_ov119_021e0260(PokemonTradeWork *wk) {
    if (wk->backup.mail != NULL) {
        GFL_HeapFree(wk->backup.mail);
    }
    if (wk->backup.records != NULL) {
        GFL_HeapFree(wk->backup.records);
    }
    if (wk->backup.survey != NULL) {
        GFL_HeapFree(wk->backup.survey);
    }
    if (wk->backup.pokedex != NULL) {
        GFL_HeapFree(wk->backup.pokedex);
    }
    if (wk->backup.playersMet != NULL) {
        GFL_HeapFree(wk->backup.playersMet);
    }
    if (wk->backup.party != NULL) {
        GFL_HeapFree(wk->backup.party);
    }
    if (wk->backup.box != NULL) {
        GFL_HeapFree(wk->backup.box);
    }
    MI_CpuFill8(&wk->backup, 0, sizeof(TradeBackup));
    wk->unk11F7 = 0;
}

void func_ov119_021e02e8(PokemonTradeWork *wk) {
    void *dest;
    u32 size;
    GameRecords *dest_00;
    PlayTime *time;
    PerapVoice *chatter;
    void *mail;
    GameData *gameData;

    mail = wk->backup.mail;
    if (mail != (void *)0x0) {
        gameData = wk->gameData;
        GameData_GetSaveControl(gameData);
        dest = func_02008fec(gameData);
        size = func_02008ffc();
        MI_CpuCopy8(mail, dest, size);
        GFL_HeapFree(mail);
    }
    mail = wk->backup.records;
    if (mail != (void *)0x0) {
        dest_00 = GameData_GetRecords(wk->gameData);
        size = func_02008c20();
        MI_CpuCopy8(mail, dest_00, size);
        GFL_HeapFree(mail);
    }
    if (wk->backup.survey != NULL) {
        func_ov119_021e017c(wk->gameData, wk->backup.survey);
    }
    if (wk->backup.pokedex != NULL) {
        func_ov119_021e01e0(wk->gameData, wk->backup.pokedex);
    }
    if (wk->backup.playersMet != NULL) {
        func_ov119_021e0240(wk->gameData, wk->backup.playersMet);
    }
    if (wk->backup.party != NULL) {
        PokeParty_Copy(wk->backup.party, wk->party);
        time = func_02012fcc(wk->gameData);
        func_020086f8(time, &wk->backup.unk1C);
        GFL_HeapFree(wk->backup.party);
        wk->backup.party = NULL;
    }
    if (wk->backup.box != NULL) {
        size = getSizeofBox();
        mail = BoxSaveAccessor_GetBox(wk->boxes, (u32)wk->param->box);
        MI_CpuCopy8(wk->backup.box, mail, size);
        GFL_HeapFree(wk->backup.box);
    }
    if (wk->backup.chatot) {
        chatter = getChatterDataAddress(wk->gameData);
        setChatotPresent(chatter);
    }
    MI_CpuFill8(&wk->backup, 0, sizeof(TradeBackup));
}

// Forgets the Chatot's cry if the only Chatot of the party is traded away
BOOL func_ov119_021e0410(PokemonTradeWork *wk, PokeParty *party, int slot) {
    PerapVoice *chatter;
    BOOL BVar1;
    s32 pkmCount;
    PartyPkm *pPVar3;
    s32 i;
    u32 index;

    i = 0;
    chatter = getChatterDataAddress(wk->gameData);
    BVar1 = doesChatotExist(chatter);
    if (BVar1 == 0) {
        return 0;
    }
    pkmCount = PokeParty_GetPkmCount(party);
    index = 0;
    if (0 < pkmCount) {
        do {
            pPVar3 = PokeParty_GetPkm(party, index);
            if (PokeParty_GetParam(pPVar3, 5, (void *)0x0) == 0x1b9) {
                i = i + 1;
            }
            index = index + 1;
        } while ((s32)index < pkmCount);
    }
    if (i != 1) {
        return 0;
    }
    pPVar3 = PokeParty_GetPkm(party, slot);
    index = PokeParty_GetParam(pPVar3, 5, (void *)0x0);
    if (index == 0x1b9) {
        setChatotNotPresent(chatter);
        return 1;
    }
    return 0;
}

static void func_ov119_021e049c(PokemonTradeWork *wk) {
    PartyPkm *pkm;
    GameRecords *records;
    if (!func_ov119_021d8aa8(wk)) {
        wk->backup.mail = BackupMail(wk->gameData, wk->heapId);
        wk->backup.records = BackupRecords(wk->gameData, wk->heapId);
        wk->backup.survey = func_ov119_021e0138(wk->gameData, wk->heapId);
        wk->backup.pokedex = func_ov119_021e01a0(wk->gameData, wk->heapId);
        wk->backup.playersMet = func_ov119_021e0200(wk->gameData, wk->heapId);
        func_020086f0(func_02012fcc(wk->gameData), &wk->backup.unk1C);
        if (wk->param->box == BoxSaveAccessor_GetAvailableBoxCount(wk->boxes)) {
            wk->backup.party = PokeParty_Create(wk->heapId);
            PokeParty_Copy(wk->party, wk->backup.party);
            wk->backup.chatot = func_ov119_021e0410(wk, wk->party, wk->param->slot);
        } else {
            u32 size;
            void *dest;
            void *src;
            wk->backup.box = GFL_HeapAllocate(wk->heapId, getSizeofPokeBox(), FALSE, "pokemontrade_save.c", 625);
            size = getSizeofPokeBox();
            dest = wk->backup.box;
            src = BoxSaveAccessor_GetBox(wk->boxes, wk->param->box);
            MI_CpuCopy8(src, dest, size);
        }
    } else {
        PokemonTradeParam *param = wk->param;
        if (param->box == BoxSaveAccessor_GetAvailableBoxCount(wk->boxes)) {
            func_ov119_021e0410(wk, wk->party, param->slot);
        }
        wk->unk11F7 = 1;
    }
    {
        PokemonTradeParam *param = wk->param;
        if (param->box != BoxSaveAccessor_GetAvailableBoxCount(wk->boxes)) {
            pkm = PokeParty_GetPkm(param->party, 0);
            if (PML_ItemIsMail(PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL))) {
                MailData *block = func_02008fec(wk->gameData);
                int mailSlot = func_02009020(block, 0);
                if (mailSlot != -1) {
                    MailData *mail = CreateMailData(wk->heapId);
                    PokeParty_GetParam(pkm, PKM_PARAM_MAIL, mail);
                    func_0200903c(block, 0, mailSlot, mail);
                    ResetMailData(mail);
                    PokeParty_SetParam(pkm, PKM_PARAM_MAIL, (uintptr_t)mail);
                    PokeParty_SetParam(pkm, PKM_PARAM_ITEM, 0);
                    GFL_HeapFree(mail);
                }
            }
        }
    }
    {
        u8 country = UnityTowerVisitor_GetCountry(wk->partnerInfo);
        u8 province = UnityTowerVisitor_GetProvince(wk->partnerInfo);
        u32 validCountry = func_02025c1c(country, province, TrainerInfo_GetRegion(wk->partnerInfo));
        u32 validProvince = func_02025c30(country, province, TrainerInfo_GetRegion(wk->partnerInfo));
        if (country != validCountry || province != validProvince) {
            func_02008574(wk->partnerInfo, 0, 0);
        }
    }
    func_02009de8(func_02012f14(wk->gameData), wk->partnerInfo);
    {
        int i;
        UnityTowerSurveySave *survey = getUnityTower_SurveySaveBlkAddrress(GameData_GetSaveControl(wk->gameData));
        PlayerInfo *infos[2];
        infos[0] = wk->myInfo;
        infos[1] = wk->partnerInfo;
        for (i = 0; i < 2; i++) {
            PlayerInfo *info = infos[i];
            if (info != NULL) {
                u8 country = UnityTowerVisitor_GetCountry(info);
                u8 province = UnityTowerVisitor_GetProvince(info);
                TrainerInfo_GetRegion(info);
                if (!func_0200949c(survey, country, province)) {
                    func_020094d8(survey, country, province, TRUE);
                }
            }
        }
    }
    pkm = PokeParty_GetPkm(wk->param->party, 0);
    records = GameData_GetRecords(wk->gameData);
    PokeParty_Recover(pkm);
    if (!PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL)) {
        PokeParty_SetParam(pkm, PKM_PARAM_HAPPINESS, 70);
    }
    if (PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL)) {
        PokeParty_SetupMetData(pkm, 6, wk->myInfo, 30003, wk->heapId);
    }
    switch (wk->type) {
    case 2:
    case 3:
        RecordAddOne(records, 16);
        break;
    case 1:
        RecordAddOne(records, 12);
        break;
    case 0:
        RecordAddOne(records, 36);
        break;
    }
    addPkmToDex(GameData_GetPokedex(wk->gameData), pkm);
    if (wk->type == 3) {
        func_02009eb0(func_02012f14(wk->gameData));
    }
    if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL) == SPECIES_SHAYMIN &&
        PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL) == 1) {
        PokeParty_ChangeForme(pkm, 0);
        addPkmToDex(GameData_GetPokedex(wk->gameData), pkm);
    }
    if (wk->param->box == BoxSaveAccessor_GetAvailableBoxCount(wk->boxes)) {
        copyPkmIntoPartyBlk(wk->party, wk->param->slot, pkm);
    } else {
        BoxSaveAccessor_SetPkm(wk->boxes, wk->param->box, wk->param->slot, func_0201d624(pkm));
    }
    switch (wk->type) {
    case 2:
        func_02009bb8(func_0201283c(wk->gameData), wk->param->friendIndex - 1, 0, 0, TRUE);
        break;
    case 0:
    case 1: {
        u32 index;
        if (func_02009d1c(func_0201283c(wk->gameData), wk->partnerInfo, &index)) {
            func_02009bb8(func_0201283c(wk->gameData), index, 0, 0, TRUE);
        }
        break;
    }
    }
}


static void func_ov119_021e08e8(PokemonTradeWork *wk) {
    func_ov119_021e36d8(wk, 0);
    func_ov119_021e36d8(wk, 1);
    func_ov119_021e36d8(wk, 2);
    func_ov119_021e36d8(wk, 3);
    if (wk->type == 4 || wk->type == 7 || wk->type == 8) {
        wk->param->next = 3;
        func_ov119_021dfd3c(wk);
        func_ov119_021d8e84(wk, func_ov119_021d9434);
    } else if (wk->type == 1 || wk->type == 2 || wk->type == 3) {
        func_ov119_021d8e84(wk, func_ov119_021e0054);
    } else {
        func_ov119_021d8e84(wk, func_ov119_021e0980);
    }
}

static void func_ov119_021e0958(PokemonTradeWork *wk) {
    if (func_ov119_021e12ac(wk) && wk->timer > 180) {
        func_ov119_021d8e84(wk, func_ov119_021e0a64);
    }
}

// Puts the received Pokémon in the parameter's party, for the event
static void func_ov119_021e0980(PokemonTradeWork *wk) {
    PartyPkm *pkm;
    int slot;
    if (func_ov119_021d8aa8(wk)) {
        int index = wk->unkFA0 % 3;
        pkm = func_ov119_021d8fa4(wk, 0);
        wk->param->box = wk->negoBox[0][index];
        slot = wk->negoSlot[0][index];
    } else {
        pkm = func_ov119_021d8fa4(wk, 1);
        wk->param->box = wk->selectBox;
        slot = wk->selectSlot;
    }
    wk->param->slot = slot;
    PokeParty_InitCore(wk->param->party, 6);
    PokeParty_AddPkm(wk->param->party, pkm);
    addPkmToDex(GameData_GetPokedex(wk->gameData), pkm);
    if (wk->param->box != BoxSaveAccessor_GetAvailableBoxCount(wk->boxes) &&
        PML_ItemIsMail(PokeParty_GetParam(PokeParty_GetPkm(wk->param->party, 0), PKM_PARAM_ITEM, NULL))) {
        GFL_MsgDataLoadStrbuf(wk->msgData, 106, wk->strbuf);
        func_ov119_021e1024(wk);
        func_ov119_021d8e84(wk, func_ov119_021e0958);
        wk->timer = 0;
        return;
    }
    func_ov119_021d8e84(wk, func_ov119_021e0a64);
}

// Starts the received Pokémon's evolution if it evolves by trade
static void func_ov119_021e0a64(PokemonTradeWork *wk) {
    if (wk->gameData == NULL) {
        func_ov119_021d8e84(wk, func_ov119_021e0c78);
    } else {
        if (wk->param != NULL) {
            PartyPkm *pkm;
            PartyPkm *other;
            u32 species;
            u32 method;
            if (func_ov119_021d8aa8(wk)) {
                pkm = func_ov119_021d8fa4(wk, 0);
                other = func_ov119_021d8fa4(wk, 1);
            } else {
                pkm = func_ov119_021d8fa4(wk, 1);
                other = func_ov119_021d8fa4(wk, 0);
            }
            species =
                CheckEvolveSpecies(NULL, pkm, 1, (uintptr_t)other, GameData_GetSeason(wk->gameData), &method, wk->heapId);
            if (species != 0) {
                wk->param->next = TRADE_NEXT_EVOLVE;
                wk->param->evolveSpecies = species;
                wk->param->evolveMethod = method;
                func_ov119_021d8e84(wk, func_ov119_021d9434);
                func_ov119_021dfd64(wk);
                return;
            }
        }
        GFL_MsgDataLoadStrbuf(wk->msgData, 50, wk->strbuf);
        func_ov119_021e1024(wk);
        func_ov119_021e1030(wk);
        func_ov119_021d8e84(wk, func_ov119_021e0ba4);
    }
    func_ov119_021dfd3c(wk);
}

void func_ov119_021e0b34(PokemonTradeWork *wk) {
    func_ov119_021e10d8(wk);
    GFL_MsgDataLoadStrbuf(wk->msgData, 50, wk->strbuf);
    wk->cursorImage = LoadCursorImageEndOfHeap(6, 15, 0, wk->heapId);
    func_ov119_021e1024(wk);
    func_ov119_021e1030(wk);
    func_ov119_021dfd30(wk, 0);
    GFL_BGSysSetBGEnabled(6, TRUE);
    func_0203d34c(TRUE, wk->heapId);
    GFL_FadeSet(3, 16, 0, 2);
    func_ov119_021d8e84(wk, func_ov119_021e0ba4);
}

static void func_ov119_021e0ba4(PokemonTradeWork *wk) {
    func_02039c68(func_020399a8(), 0x18, 8);
    func_ov119_021d8e84(wk, func_ov119_021e0bc4);
}

static void func_ov119_021e0bc4(PokemonTradeWork *wk) {
    if (func_02039cb4(func_020399a8(), 0x18, 8)) {
        func_ov119_021d8e84(wk, func_ov119_021e0be8);
    }
}

void func_ov119_021e0be8(PokemonTradeWork *wk) {
    if (func_ov119_021e12ac(wk)) {
        if (func_ov119_021d8a24(wk)) {
            func_ov119_021e049c(wk);
            wk->unkEF8 = func_020108c4(wk->heapId, wk->gameData, (void (*)(void *))func_ov119_021e0260, wk);
            func_ov119_021d8e84(wk, func_ov119_021e0c44);
        } else {
            func_ov119_021d8e84(wk, func_ov119_021e0c78);
        }
    }
}


void func_ov119_021e0c44(PokemonTradeWork *wk) {
    BOOL BVar1;

    BVar1 = func_02010904(wk->unkEF8);
    if (BVar1 != 0) {
        func_ov119_021e02e8(wk);
        func_02010934(wk->unkEF8);
        wk->unkEF8 = (void *)0x0;
        func_ov119_021d8e84(wk, (void *)func_ov119_021e0c78);
    }
}

static void func_ov119_021e0c78(PokemonTradeWork *wk) {
    GFL_WipeSet(0, 0, 0, 0, 6, 1, wk->heapId);
    wk->timer = 0;
    wk->unk11F7 = 0;
    func_ov119_021d8e84(wk, func_ov119_021e0cc0);
}

static void func_ov119_021e0cc0(PokemonTradeWork *wk) {
    if (wk->timer >= 6 && GFL_WipeIsFinished()) {
        if (wk->cursorImage != 0) {
            GFL_BGSysFreeCharMemory(6, (u16)wk->cursorImage, (u16)(wk->cursorImage >> 16));
        }
        wk->cursorImage = 0;
        GFL_BGSysSetEnabledBGsA(0);
        GFL_BGSysSetEnabledBGsB(0);
        func_0204b1cc(wk->clactUnit);
        wk->clactUnit = func_0204b100(340, 0, wk->heapId);
        func_ov119_021e106c(wk);
        func_ov119_021e57a4(wk);
        func_ov119_021e53cc(wk);
        func_ov119_021e57e8(0);
        func_ov119_021e5680(wk);
        func_ov119_021e58a0(wk);
        func_ov119_021e467c(wk);
        func_ov119_021e4420(wk);
        func_ov119_021e3e80(wk);
        func_ov119_021e3fe4(wk);
        func_ov119_021e3f74(wk);
        func_ov119_021e3208(wk, 0);
        GFL_BGSysSetEnabledBGsA(0x1f);
        GFL_BGSysSetEnabledBGsB(0x1e);
        func_ov119_021e3c20(wk);
        func_0203d34c(TRUE, wk->heapId);
        wk->unk11FA = 0;
        func_ov119_021d8e84(wk, func_ov119_021dace4);
    }
}


