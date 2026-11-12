// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov156/field_pass_power.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "field/field_pass_power.h"
#include "constants/items.h"
#include "constants/script_text_banks.h"
#include "constants/sound.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_script.h"
#include "field/field_script_event.h"
#include "field/game_beacon_set.h"
#include "gfl/fade.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/high_link.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/str_tool.h"
#include "system/wordset.h"
#include "text/script/global_10435.h"

// The pass power whose script gets 16 in place of its effect
#define PASS_POWER_027 0x27
// The effects of the pass powers that act at once
#define PASS_POWER_EFFECT_HP_RESTORE 5
#define PASS_POWER_EFFECT_PP_RESTORE 6
// The pass powers in use have one of these effects each
#define PASS_POWER_EFFECT_COUNT 16

typedef struct {
    u32 unk00;
    GameSystem *gsys;
    GameData *gameData;
} PassPowerFlashWork;

typedef struct {
    u16 unk00;
    HeapID heapId;
    HeapID tailHeapId;
    GameSystem *gsys;
    GameData *gameData;
    Field *field;
    void *msgBGSys;
    void *listWindow;
    MMSys *actorSystem;
    void *msgWindow;
    u32 unk24;
    MsgData *msgData;
    WordSet *wordSet;
    void *passPowerData;
    StrBuf *strbuf;
    StrBuf *formatted;
} PassPowerListWork;

static void func_ov083_021f5390(PassPowerListWork *work, GameSystem *gsys, Field *field);
static void func_ov083_021f5400(PassPowerListWork *work);
static GameEventReturnCode func_ov083_021f542c(GameEvent *event, u32 *state, void *data);
void func_ov083_021f5488(GameSystem *gsys, WordSet *wordSet, u32 effect);
static GameEventReturnCode func_ov083_021f54cc(GameEvent *event, u32 *state, void *data);
static void func_ov083_021f556c(PassPowerListWork *work, StrBuf *format, u8 passPower, u16 seconds, u8 row);
static void func_ov083_021f55d4(PassPowerListWork *work);

// pokebw2's EventPassPowerDepleted_Create

void func_02010e14(int passPower, void *data, u32 paid);
void func_ov010_021652c4(u32 passPower);
void func_ov010_021657b0(u32 passPower);
void func_ov083_021f5488(GameSystem *gsys, WordSet *wordSet, u32 effect);
void func_ov010_021652c4(u32 passPower);
void func_ov010_021657b0(u32 passPower);

void func_ov083_021f5488(GameSystem *gsys, WordSet *wordSet, u32 effect);
void func_ov010_021652c4(u32 passPower);
void func_ov010_021657b0(u32 passPower);

void func_ov083_021f5488(GameSystem *gsys, WordSet *wordSet, u32 effect);
void func_ov010_021652c4(u32 passPower);
void func_ov010_021657b0(u32 passPower);

void func_ov083_021f5488(GameSystem *gsys, WordSet *wordSet, u32 effect);
void func_ov010_021652c4(u32 passPower);
void func_ov010_021657b0(u32 passPower);

void func_ov083_021f5488(GameSystem *gsys, WordSet *wordSet, u32 effect);
void func_ov010_021652c4(u32 passPower);
void func_ov010_021657b0(u32 passPower);

GameEvent *func_ov083_021f5260(GameSystem *gsys, void *args) {
    u32 *params = args;
    GameEvent *event;
    ScriptWork *work;
    GameData *gameData;
    void *data;
    s32 passPower;
    u32 effect;
    u16 price;

    gameData = GSYS_GetGameData(gsys);
    data = PassPowerData_Create(HEAPID_TAIL(HEAPID_FIELDMAP));
    passPower = params[0];
    event = EventScriptCall_Create(gsys, 0x28b9, NULL, HEAPID_FIELDMAP);
    work = EventScriptCall_GetWork(event);
    if (passPower == 0x30 || passPower > 0x2d) {
        passPower = 0;
    }
    effect = PassPowerData_GetEffect(data, passPower);
    func_02010e14(passPower, data, params[1]);
    if (passPower == PASS_POWER_027) {
        ScriptWork_SetParams(work, passPower, 0xb, params[1], 0);
    } else {
        ScriptWork_SetParams(work, passPower, (u16)effect, params[1], 0);
    }
    func_ov083_021f5488(gsys, ScriptWork_GetWordSet(work), effect);
    if (params[1] != 0) {
        price = PassPowerData_GetPrice(data, passPower);
        BagSave_SubItem(GameData_GetBag(gameData), ITEM_PASS_ORB, price, HEAPID_TAIL(HEAPID_FIELDMAP));
        func_ov010_021652c4(passPower);
    } else {
        func_ov010_021657b0(passPower);
    }
    PassPowerData_Free(data);
    return event;
}

GameEvent *func_ov083_021f532c(GameSystem *gsys, void *args) {
    return EventScriptCall_Create(gsys, 0x28ba, NULL, HEAPID_FIELDMAP);
}

// pokebw2's EventPassPowerFlash_Create
GameEvent *func_ov083_021f5340(GameSystem *gsys) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov083_021f542c, sizeof(PassPowerFlashWork));
    PassPowerFlashWork *work = GameEvent_GetData(event);

    work->gameData = GSYS_GetGameData(gsys);
    work->gsys = gsys;
    return event;
}

// pokebw2's EventPassPowerList_Create
GameEvent *func_ov083_021f5368(GameSystem *gsys, void *args) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov083_021f54cc, sizeof(PassPowerListWork));

    func_ov083_021f5390(GameEvent_GetData(event), gsys, args);
    return event;
}

// pokebw2's PassPowerList_Init
static void func_ov083_021f5390(PassPowerListWork *work, GameSystem *gsys, Field *field) {
    work->heapId = HEAPID_FIELDMAP;
    work->tailHeapId = HEAPID_TAIL(work->heapId);
    work->field = field;
    work->gsys = gsys;
    work->gameData = GSYS_GetGameData(gsys);
    work->msgBGSys = Field_GetMsgBGSys(work->field);
    work->actorSystem = Field_GetActorSystem(work->field);
    work->passPowerData = PassPowerData_Create(work->heapId);
    work->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SCRIPT_MESSAGE, SCRIPT_TEXT_GLOBAL_10435, work->heapId);
    work->wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
    work->strbuf = GFL_StrBufCreate(129, work->heapId);
    work->formatted = GFL_StrBufCreate(129, work->heapId);
    DisableAllActorsMovement(work->actorSystem);
}

// pokebw2's PassPowerList_Exit
static void func_ov083_021f5400(PassPowerListWork *work) {
    GFL_StrBufFree(work->formatted);
    GFL_StrBufFree(work->strbuf);
    GFL_WordSetSystemFree(work->wordSet);
    GFL_MsgDataFree(work->msgData);
    PassPowerData_Free(work->passPowerData);
    EnableAllActorsMovement(work->actorSystem);
}

// pokebw2's EventPassPowerFlash_Callback
static GameEventReturnCode func_ov083_021f542c(GameEvent *event, u32 *state, void *data) {
    switch (*state) {
    case 0:
        GFL_FadeSet(FADE_ENGINE_A_WHITE, 10, 16, 4);
        GFL_SndSEPlay(SEQ_SE_W295_01);
        (*state)++;
        break;
    case 1:
        if (GFL_FadeIsRunning()) {
            break;
        }
        GFL_FadeSet(FADE_ENGINE_A_WHITE, 16, 0, 8);
        (*state)++;
        // fallthrough
    case 2:
        if (GFL_FadeIsRunning()) {
            break;
        }
        // fallthrough
    default:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

// Puts the first Pokémon able to battle in the word set, and applies the pass powers that act at once
// pokebw2's PassPower_ApplyInstant
void func_ov083_021f5488(GameSystem *gsys, WordSet *wordSet, u32 effect) {
    PokeParty *party = GameData_GetParty(GSYS_GetGameData(gsys));
    u8 slot = PokeParty_GetFirstBattleReady(party);

    loadPokemonNicknameToStrbuf(wordSet, 1, PokeParty_GetPkm(party, slot));
    switch (effect) {
    case PASS_POWER_EFFECT_HP_RESTORE:
        PassPower_ApplyHPRestore(party);
        break;
    case PASS_POWER_EFFECT_PP_RESTORE:
        PassPower_ApplyPPRestore(party);
        break;
    }
}

// pokebw2's EventPassPowerList_Callback
static GameEventReturnCode func_ov083_021f54cc(GameEvent *event, u32 *state, void *data) {
    PassPowerListWork *work = data;

    switch (*state) {
    case 0:
        work->msgWindow = func_ov021_02190fa0(work->msgBGSys, work->msgData, 19);
        func_ov021_0219103c(work->msgWindow, 0, 0, 3);
        (*state)++;
        break;
    case 1:
        if (func_ov021_021910c0(work->msgWindow)) {
            func_ov021_02191008(work->msgWindow);
            (*state)++;
        }
        break;
    case 2:
        work->listWindow = func_ov021_021907c4(work->msgBGSys, work->msgData, 4, 1, 24, 22);
        func_ov021_02190854(work->listWindow);
        func_ov083_021f55d4(work);
        (*state)++;
        break;
    case 3:
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            (*state)++;
        }
        break;
    default:
        func_ov021_02190854(work->listWindow);
        func_ov021_0219082c(work->listWindow);
        func_ov083_021f5400(work);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

// Prints a pass power's name and the minutes and seconds it has left on a row of the list
// pokebw2's PassPowerList_PrintRow
static void func_ov083_021f556c(PassPowerListWork *work, StrBuf *format, u8 passPower, u16 seconds, u8 row) {
    loadPassPowerToStrbuf(work->wordSet, 0, passPower);
    WordSetNumber(work->wordSet, 1, seconds / 60, 2, NUM_PAD_SPACE, TRUE);
    WordSetNumber(work->wordSet, 2, seconds % 60, 2, NUM_PAD_ZERO, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->formatted, format);
    func_ov021_02190844(work->listWindow, 8, row * 16, work->formatted);
}

// Prints the pass powers in use, the one with the least time left first
// pokebw2's PassPowerList_Print



static void func_ov083_021f55d4(PassPowerListWork *work) {
    HighLinkSave *save;
    u8 passPower;
    u8 passPowers[13];
    u8 order[13];
    u16 seconds[13];
    u8 effect;
    u8 count;
    u8 i;
    u8 j;
    u8 tmp;

    save = getHighLinkBlockAddress(GameData_GetSaveControl(work->gameData));
    GFL_MsgDataLoadStrbuf(work->msgData, 4, work->strbuf);
    func_ov021_02190844(work->listWindow, 0, 0, work->strbuf);
    GFL_MsgDataLoadStrbuf(work->msgData, 6, work->strbuf);
    passPower = func_0200bf10(save);
    if (passPower != HIGH_LINK_POWER_NONE) {
        func_ov083_021f556c(work, work->strbuf, passPower, 0, 1);
    }
    GFL_MsgDataLoadStrbuf(work->msgData, 5, work->strbuf);
    func_ov021_02190844(work->listWindow, 0, HIGH_LINK_POWER_NONE, work->strbuf);
    GFL_MsgDataLoadStrbuf(work->msgData, 7, work->strbuf);

    for (effect = 0, count = 0; effect < 13; effect++) {
        order[effect] = effect;
        passPower = PassPower_GetUsedIDByEffect(effect);
        if (passPower != HIGH_LINK_POWER_NONE) {
            seconds[count] = PassPower_GetRemainingSeconds(effect);
            passPowers[count] = passPower;
            count++;
        }
    }

    for (i = 0; i < count - 1; i++) {
        for (j = count - 1; j > i; j--) {
            if (seconds[order[j - 1]] > seconds[order[j]]) {
                tmp = order[j];
                order[j] = order[j - 1];
                order[j - 1] = tmp;
            }
        }
    }

    for (i = 0; i < count; i++) {
        func_ov083_021f556c(work, work->strbuf, passPowers[order[i]], seconds[order[i]], i + 4);
    }
}
