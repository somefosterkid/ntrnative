// Black's code at ov021 0x021b66b8-0x021b69b0. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "constants/species.h"
#include "constants/version.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "field/field_script.h"
#include "gfl/heap.h"
#include "field/calender.h"
#include "field/city_state.h"
#include "field/encounter.h"
#include "field/event_data.h"
#include "field/field_actor_internal.h"
#include "field/field_map.h"
#include "field/field_status.h"
#include "field/player_state.h"
#include "field/pleasure_boat.h"
#include "field/trial_house.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "nitro/mi.h"
#include "pml/item.h"
#include "pml/mail.h"
#include "save/bag.h"
#include "save/box.h"
#include "save/config.h"
#include "save/event_work.h"
#include "save/gimmick_savedata.h"
#include "save/high_link.h"
#include "save/player_savedata.h"
#include "save/playtime.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "system/shortcut_util.h"
#include "app/box2_obj.h"

// The legendary Pokemon the player's own version has the script look for (Reshiram in Black, Zekrom in White)
#if GAME_VERSION == VERSION_WHITE
#define OT_LEGENDARY_SPECIES SPECIES_ZEKROM
#else
#define OT_LEGENDARY_SPECIES SPECIES_RESHIRAM
#endif

u32 GetOTZekromPartyIdx(GameData *param_1);

u32 func_ov021_021b6850(GameData *gameData, u16 param_2, u16 *param_3, u16 *param_4);


void func_ov021_021b6904(FieldScriptEnv *env);
void func_ov021_021b6708(FieldScriptEnv *env, u32 value);

u32 func_ov021_021b66b8(VM *vm, FieldScriptEnv *env) {
    u16 type = ScriptReadAny(vm, env);
    u16 *var = ScriptReadVar(vm, env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);

    switch (type) {
    case 0:
        if ((s32)GetOTZekromPartyIdx(gameData) < 0) {
            *var = 0;
        } else {
            *var = 1;
        }
        break;
    case 1:
        func_ov021_021b6904(env);
        break;
    case 2:
        func_ov021_021b6708(env, *var);
        func_ov021_021b6904(env);
        break;
    }
    return 0;
}

void func_ov021_021b6708(FieldScriptEnv *param_1, u32 param_2) {
    GameData *gameData;
    PokeParty *party;
    u16 uStack_16;
    u16 uStack_18;
    HeapID heapId;
    BoxSaveAccessor *boxes;
    u32 size;
    PartyPkm *pkm;
    PartyPkm *pPVar3;
    BOOL BVar4;
    MailData *mails;
    HeapID heapId_00;
    MailData *mail;
    BoxPkm *pBVar5;

    gameData = FieldScriptEnv_GetGameData(param_1);
    party = GameData_GetParty(gameData);
    if (func_ov021_021b6850(gameData, OT_LEGENDARY_SPECIES, &uStack_16, &uStack_18) != 0) {
        heapId = FieldScriptEnv_GetHeapID(param_1);
        boxes = GameData_GetBoxSaveAccessor(gameData);
        size = PokeParty_GetPkmRawSize();
        pkm = GFL_HeapAllocate(heapId,size,1,"scrcmd_pokemon_fld.c",0x4c7);
        size = PokeParty_GetPkmRawSize();
        pPVar3 = PokeParty_GetPkm(party, param_2);
        MI_CpuCopy8(pPVar3, pkm, size);
        size = PokeParty_GetParam(pkm, 6, (void *)0x0);
        BVar4 = PML_ItemIsMail((u16)size);
        if (BVar4 == 1) {
            mails = func_02008fec(gameData);
            heapId_00 = FieldScriptEnv_GetHeapID(param_1);
            mail = CreateMailData(heapId_00);
            size = func_02009020(mails, 0);
            PokeParty_GetParam(pkm, 0xa7, mail);
            GetGameDataPlayerInfo(gameData);
            if ((s32)size >= 0) {
                func_0200903c(mails, 0, size, mail);
            }
            ResetMailData(mail);
            PokeParty_SetParam(pkm, 0xa7, (uintptr_t)mail);
            PokeParty_SetParam(pkm, 6, 0);
            GFL_HeapFree(mail);
        }
        size = PokeParty_GetParam(pkm, 0xab, (void *)0x0);
        if (size == 0x1ec) {
            PokeParty_ChangeForme(pkm, 0);
            PokeParty_RecalcStats(pkm);
        }
        pBVar5 = BoxSaveAccessor_GetPkm(boxes, (u32)uStack_16, (u32)uStack_18);
        pPVar3 = boxPkmRegenToPartyPkm(pBVar5, heapId);
        copyPkmIntoPartyBlk(party, param_2, pPVar3);
        GFL_HeapFree(pPVar3);
        pBVar5 = func_020185f0(pkm);
        BoxSaveAccessor_SetPkm(boxes, (u32)uStack_16, (u32)uStack_18, pBVar5);
        GFL_HeapFree(pkm);
    }
}

u32 func_ov021_021b6850(GameData *gameData, u16 param_2, u16 *param_3, u16 *param_4) {
    s32 lastBox;
    s32 i;
    s32 tray;
    int pos;
    u32 id;
    PlayerInfo *info;
    BoxSaveAccessor *boxes;

    GameData_GetParty(gameData);
    info = GetGameDataPlayerInfo(gameData);
    boxes = GameData_GetBoxSaveAccessor(gameData);
    lastBox = BoxSaveAccessor_GetLastOpenedBox(boxes);
    id = getIDAsUInt(info);

    for (i = 0; i < 24; i++) {
        tray = (i + lastBox) % 24;
        for (pos = 0; pos < 30; pos++) {
            if (BoxSaveAccessor_GetPkmParam(boxes, tray, pos, 0xa9, NULL) != 0) {
                if (BoxSaveAccessor_GetPkmParam(boxes, tray, pos, 7, NULL) == id) {
                    if (BoxSaveAccessor_GetPkmParam(boxes, tray, pos, 5, NULL) == param_2) {
                        if (BoxSaveAccessor_GetPkmParam(boxes, tray, pos, 0x4c, NULL) == 0) {
                            *param_3 = tray;
                            *param_4 = pos;
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}


void func_ov021_021b6904(FieldScriptEnv *param_1) {
    HeapID heapId;
    GameData *gameData;
    PokeParty *party;
    u32 indexA;
    gameData = FieldScriptEnv_GetGameData(param_1);
    party = GameData_GetParty(gameData);
    indexA = GetOTZekromPartyIdx(gameData);
    if ((s32)indexA >= 0) {
        heapId = FieldScriptEnv_GetHeapID(param_1);
        PokeParty_SwapPkms(party, indexA, 0, heapId);
    }
}

u32 GetOTZekromPartyIdx(GameData *param_1) {
    PokeParty *party;
    PlayerInfo *info;
    s32 pkmCount;
    u32 idAsUInt;
    PartyPkm *pkm;
    u32 result;
    u32 index;

    party = GameData_GetParty(param_1);
    info = GetGameDataPlayerInfo(param_1);
    pkmCount = PokeParty_GetPkmCount(party);
    idAsUInt = getIDAsUInt(info);
    index = 0;
    if (0 < pkmCount) {
        do {
            pkm = PokeParty_GetPkm(party, index);
            result = PokeParty_GetParam(pkm, 7, (void *)0x0);
            if (((idAsUInt == result) && (result = PokeParty_GetParam(pkm, 5, (void *)0x0), result == OT_LEGENDARY_SPECIES)) &&
                  (result = PokeParty_GetParam(pkm, 0x4c, (void *)0x0), result == 0)) {
                return index;
            }
            index = index + 1;
        } while ((s32)index < pkmCount);
    }
    return 0xffffffff;
}
