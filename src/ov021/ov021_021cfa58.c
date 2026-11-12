// Black's code at ov021 0x021cfa58-0x021cfc4c. No name is known: the module and address.

#include "types.h"
#include "app/box2_obj.h"
#include "field/encounter.h"
#include "field/field_async_proc.h"
#include "field/field_script.h"
#include "field/underwater_effect.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/player_savedata.h"
#include "save/save_control.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "gfl/overlay.h"
#include "field/fieldmap.h"
#include "system/tr_tool.h"
#include "gfl/str.h"
#include "system/wordset.h"
#include "battle/trainer_data.h"
#include "field/field_player.h"
#include "field/field_map.h"
#include "system/area_data.h"
#include "field/ov021_021cfa58.h"


GameEventReturnCode func_ov021_021cfba8(GameEvent *param_1, u32 *state, void *data);
void func_ov021_021cfb1c(HighLinkSave *param_1, s32 param_2, HighLinkEntry *param_3);

// The zone of each of the 18 places below; ov018's intrude_field.c reads it with data_ov021_021db834 (no code here does)
const u16 data_ov021_021db810[18] = {
#include "asset/ov021_021cfa58_data_ov021_021db810.inc"
};

// 18 places: the grid position (x, y, z) and a u16 that func_ov021_021cfac0 compares; ov018's intrude_field.c
// reads the positions too
const Ov021DataRec data_ov021_021db834[18] = {
#include "asset/ov021_021cfa58_data_ov021_021db834.inc"
};


u32 func_ov021_021cfa58(s32 param_1, FieldScriptEnv *param_2) {
    FieldScriptEnv_GetScriptWork(param_2);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(param_2);
    Field *field = GSYS_GetField(gsys);
    FieldAsyncProcManager *mgr = Field_GetAsyncProcMgr(field);
    FieldAsyncProcManager_AddProc(OVERLAY_ID(57), mgr, &data_ov057_021f3900);
    return 0;
}


u32 func_ov021_021cfa88(VM *param_1, FieldScriptEnv *param_2) {
    GameSystem *gsys;
    GameData *gameData;
    SaveControl *save;
    PlayerSave *playerSave;
    u16 *abyssalRuinsStepCounter;

    FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    gameData = GSYS_GetGameData(gsys);
    save = GameData_GetSaveControl(gameData);
    playerSave = SaveControl_GetPlayerSave(save);
    abyssalRuinsStepCounter = ScriptReadVar(param_1, param_2);
    *abyssalRuinsStepCounter = PlayerSave_GetAbyssalRuinsStepCounter(playerSave);
    return 0;
}

u32 func_ov021_021cfac0(HighLinkSave *param_1, u16 param_2, s16 param_3, s16 param_4, s16 param_5) {
    s32 i;
    HighLinkEntry *entry = param_1->unk10;

    for (i = 0; i < 0x14; i++, entry++) {
        const Ov021DataRec *rec;

        if (entry->unk10 == 0) {
            return 0xff;
        }
        rec = &data_ov021_021db834[entry->unk12];
        if (rec->unk06 == param_2 && rec->unk00 == param_3 && rec->unk04 == param_5
            && rec->unk02 - 1 <= param_4 && param_4 <= rec->unk02 + 1) {
            return i;
        }
    }
    return 0xff;
}


void func_ov021_021cfb1c(HighLinkSave *param_1, s32 param_2, HighLinkEntry *param_3) {
    *param_3 = param_1->unk10[param_2];
    for (param_2++; param_2 < 0x14; param_2++) {
        param_1->unk10[param_2 - 1] = param_1->unk10[param_2];
    }
    param_1->unk10[19].unk10 = 0;
}


const Ov021DataRec *func_ov021_021cfb68(HighLinkSave *param_1, s32 *param_2) {
    while (*param_2 < 0x14) {
        HighLinkEntry *entry = &param_1->unk10[*param_2];
        if (entry->unk10 != 0) {
            const Ov021DataRec *ret = &data_ov021_021db834[(&param_1->unk10[*param_2])->unk12];
            *param_2 = *param_2 + 1;
            return ret;
        }
        *param_2 = *param_2 + 1;
    }
    return NULL;
}

// The event's work is the entry func_ov021_021cfb1c took out of the save
GameEventReturnCode func_ov021_021cfba8(GameEvent *param_1, u32 *state, void *data) {
    HighLinkEntry entry;
    ScriptWork *work;
    WordSet *wordSet;
    StrBuf *strbuf;

    entry = *(HighLinkEntry *)data;
    work = EventScriptCall_Replace(param_1, 0xafc, NULL, 0x15);
    ScriptWork_SetParams(work, entry.unk10, 0, 0, 0);
    wordSet = ScriptWork_GetWordSet(work);
    strbuf = GFL_StrBufCreate(8, 0x15);
    GFL_StrBufLoadFixedString(strbuf, entry.words, 8);
    func_0201ee0c(wordSet, 3, strbuf, 1, 1, 2);
    GFL_StrBufFree(strbuf);
    return 0;
}



GameEvent *func_ov021_021cfc14(GameSystem *param_1, u32 param_2, s32 param_3) {
    GameEvent *event;
    HighLinkEntry *data;
    GameData *gameData;
    SaveControl *save;
    HighLinkSave *pHVar2;

    event = GameEvent_Create(param_1, (GameEvent *)0x0, func_ov021_021cfba8, sizeof(HighLinkEntry));
    data = GameEvent_GetData(event);
    gameData = GSYS_GetGameData(param_1);
    save = GameData_GetSaveControl(gameData);
    pHVar2 = getHighLinkBlockAddress(save);
    func_ov021_021cfb1c(pHVar2, param_3, data);
    return event;
}
