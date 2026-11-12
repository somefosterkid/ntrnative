// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov034/union_oneself.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "field/union_oneself.h"
#include "field/union_actor.h"
#include "types.h"
#include "constants/sound.h"
#include "constants/version.h"
#include "constants/zones.h"
#include "app/pms_select.h"
#include "app/pokelist.h"
#include "app/pokemon_trade.h"
#include "battle/btl_setup.h"
#include "demo/shinka_demo.h"
#include "field/colosseum.h"
#include "field/field_actor.h"
#include "field/field_controller.h"
#include "field/field_internal.h"
#include "field/field_player.h"
#include "field/union_comm.h"
#include "field/union_eff.h"
#include "field/union_main.h"
#include "field/union_msg.h"
#include "battle/regulation.h"
#include "field/comm_player.h"
#include "field/trcard_sys.h"
#include "nitro/math.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "save/wifi_list.h"
#include "system/wordset.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/net.h"
#include "gfl/net_system.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "save/box.h"
#include "save/save_control.h"
#include "save/trainer_data_savedata.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/union_view.h"
#include "nitro/os.h"

// The Union Room's steps on the field, for this machine (its situ, UnionSelf); the actors of the players heard are
// union_actor.c's. Every step of the machine is a row of begin, run and end functions. Names are not swan's

// The keys that end a message: A, B and the Control Pad
#define UNION_PAD_ANY (PAD_BUTTON_A | PAD_BUTTON_B | PAD_PLUS_KEY_MASK)

// What overlay 34 allocates for the commands it starts, for the draw and the battle
typedef struct {
    u32 unk00;
    GameData *gameData;
} UnionDrawParam;

typedef struct {
    u32 unk00;
    GameData *gameData;
    PokeParty *party;
    SaveControl *save;
    GameRecords *records;
    u32 unk14;
} UnionBattleParam;

// A step of the machine
typedef struct {
    UnionSituFunc begin;
    UnionSituFunc run;
    UnionSituFunc end;
} UnionSituStep;

// BEGIN PROTOS
void func_ov019_0217b798(UnionSystem *unisys, UnionBeaconEntry *entry);
BOOL func_ov019_0217b8e4(UnionSystem *unisys, u32 step);
static BOOL func_ov019_0217b8f0(UnionSystem *unisys, u32 step);
static u32 func_ov019_0217b908(u32 activity);
static BOOL func_ov019_0217b974(u32 activity);
static BOOL func_ov019_0217b9bc(u32 activity);
PokeParty *func_ov019_0217b9cc(UnionSystem *unisys);
static void func_ov019_0217b9f4(PokeParty *party);
static void func_ov019_0217ba10(UnionSystem *unisys, u32 activity);
static u32 func_ov019_0217ba48(UnionSystem *unisys, u32 *result);
static u32 func_ov019_0217ba6c(UnionSystem *unisys, u32 *result);
static BOOL func_ov019_0217baa8(UnionSystem *unisys, u32 activity, u32 *partyResult, u32 *boxResult);
static BOOL func_ov019_0217bb04(UnionSystem *unisys);
static BOOL func_ov019_0217bb20(UnionSystem *unisys);
static BOOL func_ov019_0217bb3c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217bb70(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217bcdc(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217bd7c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217bd80(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217bd9c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217be44(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217be64(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217c03c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217c04c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217c074(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217c1f0(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217c200(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217c2ac(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217c444(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217c450(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217c518(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217c5a8(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217c888(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217c88c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217caac(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217ceb8(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static u32 UnionOneself_GetCardVersion(UnionSystem *unisys, BOOL remote);
BOOL func_ov019_0217cfa8(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217d18c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217d314(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217d51c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217d5b8(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217d7a4(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217d890(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217d9a8(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static void func_ov019_0217dc64(UnionSystem *unisys, PlayerInfo *info, const u8 *mac);
static void func_ov019_0217dc94(UnionSystem *unisys, PlayerInfo *info, const u8 *mac);
BOOL func_ov019_0217dca8(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217de88(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217dea0(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217dfb4(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217e0a4(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217e188(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217e378(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217e440(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217e5a0(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
static BOOL func_ov019_0217e838(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217e904(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
BOOL func_ov019_0217ead8(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq);
// END PROTOS

// pokebw2's sUnionSituSteps
static const UnionSituStep data_ov019_02180074[] = {
    { func_ov019_0217bb3c, func_ov019_0217bb70, NULL }, // UNION_STEP_IDLE
    { NULL, func_ov019_0217bcdc, NULL }, // UNION_STEP_RESET
    { NULL, func_ov019_0217bd7c, NULL }, // UNION_STEP_NONE
    { NULL, func_ov019_0217bd80, NULL }, // UNION_STEP_COUNTER
    { NULL, func_ov019_0217bd9c, NULL }, // UNION_STEP_GREETING
    { func_ov019_0217be44, func_ov019_0217be64, func_ov019_0217c03c }, // UNION_STEP_ASK
    { func_ov019_0217c04c, func_ov019_0217c074, func_ov019_0217c1f0 }, // UNION_STEP_ASKED
    { func_ov019_0217c200, func_ov019_0217c2ac, func_ov019_0217c444 }, // UNION_STEP_PARENT
    { NULL, func_ov019_0217c450, NULL }, // UNION_STEP_OFFER
    { func_ov019_0217c518, func_ov019_0217c5a8, func_ov019_0217c888 }, // UNION_STEP_OFFER_PARTY
    { NULL, func_ov019_0217c88c, NULL }, // UNION_STEP_RULE_SELECT
    { NULL, func_ov019_0217caac, NULL }, // UNION_STEP_ANSWER
    { NULL, func_ov019_0217ceb8, NULL }, // UNION_STEP_ANSWER_MEMBER
    { NULL, func_ov019_0217cfa8, NULL }, // UNION_STEP_CARD
    { NULL, func_ov019_0217d5b8, NULL }, // UNION_STEP_JOIN_DRAW
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_0
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_1
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_2
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_3
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_4
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_5
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_6
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_7
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_8
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_9
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_10
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_11
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_12
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_13
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_14
    { NULL, func_ov019_0217d7a4, NULL }, // UNION_STEP_JOIN_BATTLE_15
    { NULL, func_ov019_0217d890, NULL }, // UNION_STEP_JOIN_MULTI_0
    { NULL, func_ov019_0217d890, NULL }, // UNION_STEP_JOIN_MULTI_1
    { NULL, func_ov019_0217d890, NULL }, // UNION_STEP_JOIN_MULTI_2
    { NULL, func_ov019_0217d890, NULL }, // UNION_STEP_JOIN_MULTI_3
    { NULL, func_ov019_0217d18c, NULL }, // UNION_STEP_TRADE
    { NULL, func_ov019_0217d5b8, NULL }, // UNION_STEP_JOIN_EGG
    { NULL, func_ov019_0217d51c, NULL }, // UNION_STEP_LEAVE_2
    { NULL, func_ov019_0217d314, NULL }, // UNION_STEP_RECRUIT
    { NULL, func_ov019_0217d51c, NULL }, // UNION_STEP_LEAVE
    { NULL, func_ov019_0217d9a8, NULL }, // UNION_STEP_COLOSSEUM
    { NULL, func_ov019_0217dca8, NULL }, // UNION_STEP_COLOSSEUM_SEND
    { func_ov019_0217de88, func_ov019_0217dea0, NULL }, // UNION_STEP_COLOSSEUM_IDLE
    { NULL, func_ov019_0217dfb4, NULL }, // UNION_STEP_COLOSSEUM_SIT
    { NULL, func_ov019_0217e0a4, NULL }, // UNION_STEP_COLOSSEUM_STAND
    { NULL, func_ov019_0217e188, NULL }, // UNION_STEP_PARTY_SELECT
    { NULL, func_ov019_0217e378, NULL }, // UNION_STEP_PARTY_WAIT
    { NULL, func_ov019_0217e440, NULL }, // UNION_STEP_PARTY_SEND
    { NULL, func_ov019_0217e5a0, NULL }, // UNION_STEP_BATTLE_LIST
    { NULL, func_ov019_0217e838, NULL }, // UNION_STEP_QUIT
    { NULL, func_ov019_0217e904, NULL }, // UNION_STEP_COLOSSEUM_END
    { NULL, func_ov019_0217ead8, NULL }, // UNION_STEP_COLOSSEUM_CARD
    { NULL, NULL, NULL }, // UNION_STEP_END
};

// pokebw2's UnionOneself_CheckEntries
void func_ov019_0217b75c(UnionSystem *unisys) {
    int i;
    UnionBeaconEntry *entry = unisys->found;

    for (i = 0; i < UNION_BEACON_ENTRY_MAX; i++, entry++) {
        if (entry->beacon.valid == 1 && entry->status != 0) {
            func_ov019_0217b798(unisys, entry);
            entry->status = 0;
        }
    }
}

void func_ov019_0217b798(UnionSystem *unisys, UnionBeaconEntry *entry) {
    UnionSelf *situ = &unisys->situ;
    u8 ownMac[6];

    func_ov021_021bba70(unisys, entry, NULL, 0);
    if (situ->step == 0 && situ->nextStep == 0 && entry->beacon.unk0A == 5 && entry->timeout != 0) {
        OS_GetMacAddress(ownMac);
        if (GFL_STD_MemCmp(entry, ownMac, 6) == 0 && func_ov019_0217b8f0(unisys, 6) == TRUE) {
            func_ov012_021734a8(unisys, 2, (uintptr_t)entry);
            func_ov012_021734a8(unisys, 0, (uintptr_t)entry);
        }
    }
}

// pokebw2's UnionOneself_CreateCommandEvent
GameEvent *func_ov019_0217b80c(GameSystem *gsys, Field *field) {
    UnionSystem *unisys = func_02026474(GSYS_GetGameCommSystem(gsys));
    GameEvent *event = NULL;
    UnionSubproc *subproc;

    subproc = &unisys->subproc;
    if (subproc->id != UNION_SUBPROC_ID_NULL && subproc->active == FALSE) {
        event = func_ov012_0217413c(gsys, field, unisys);
    }
    return event;
}

// pokebw2's UnionOneself_ReturnFalse, which takes nothing: Black's caller passes what it passes
// func_ov019_0217b80c, and returns what it gives as the event
GameEvent *func_ov019_0217b83c(GameSystem *gsys, Field *field) {
    return NULL;
}

// pokebw2's UnionOneself_RunStep
u8 func_ov019_0217b840(UnionSystem *unisys, Field *field) {
    UnionSelf *situ = &unisys->situ;
    UnionSituFunc func;
    BOOL done = TRUE;

    switch (situ->phase) {
    case 0:
        func = data_ov019_02180074[situ->step].begin;
        break;
    case 1:
        func = data_ov019_02180074[situ->step].run;
        break;
    case 2:
        func = data_ov019_02180074[situ->step].end;
        break;
    }
    if (func != NULL) {
        done = func(unisys, situ, field, &situ->seq);
    }
    if (done == TRUE) {
        situ->seq = 0;
        situ->phase++;
        if (situ->phase > 2) {
            situ->phase = 0;
            situ->prevStep = situ->step;
            situ->step = situ->nextStep;
            situ->nextStep = 0;
        }
    }
    return situ->step;
}

// pokebw2's UnionOneself_SetNextStep
BOOL func_ov019_0217b8e4(UnionSystem *unisys, u32 step) {
    unisys->situ.nextStep = step;
    return TRUE;
}

// pokebw2's UnionOneself_SetNextStepIfRunning
static BOOL func_ov019_0217b8f0(UnionSystem *unisys, u32 step) {
    if (unisys->situ.phase == 1) {
        return func_ov019_0217b8e4(unisys, step);
    }
    return FALSE;
}

// The regulation of an activity, by its file
// pokebw2's UnionOneself_GetRegulationFile
static u32 func_ov019_0217b908(u32 activity) {
    switch (activity) {
    case 4:
    case 5:
        return 5;
    case 6:
    case 7:
        return 0x18;
    case 8:
    case 9:
        return 6;
    case 10:
    case 11:
        return 0x19;
    case 12:
    case 13:
        return 7;
    case 14:
    case 15:
        return 0x1a;
    case 16:
    case 17:
        return 8;
    case 18:
    case 19:
        return 0x1b;
    case 20:
    case 21:
        return 9;
    case 22:
    case 23:
        return 0x1c;
    default:
        return 5;
    }
}

// pokebw2's UnionOneself_IsDoubleActivity
static BOOL func_ov019_0217b974(u32 activity) {
    switch (activity) {
    case 4:
    case 6:
    case 8:
    case 10:
    case 12:
    case 14:
    case 16:
    case 18:
    case 20:
    case 22:
        return TRUE;
    default:
        return FALSE;
    }
}

// pokebw2's UnionOneself_IsBattleActivity
static BOOL func_ov019_0217b9bc(u32 activity) {
    if (activity < 4 || activity > 0x17) {
        return FALSE;
    }
    return TRUE;
}

PokeParty *func_ov019_0217b9cc(UnionSystem *unisys) {
    BattleBoxSave *battleBox = getBattleBox(GameData_GetSaveControl(unisys->param->gameData));

    if (func_0200bba8(battleBox) == FALSE) {
        return NULL;
    }
    return convertBoxedPokeSetToParty(battleBox, 0x41);
}

// pokebw2's UnionOneself_FreeBoxParty
static void func_ov019_0217b9f4(PokeParty *party) {
    GFL_ASSERT(party != NULL);
    GFL_HeapFree(party);
}

// pokebw2's UnionOneself_SetRegulation
static void func_ov019_0217ba10(UnionSystem *unisys, u32 activity) {
    func_0201a364(func_ov019_0217b908(activity), unisys->alloc.regulation);
    if (func_ov019_0217b974(activity) == TRUE) {
        func_0200aee8(unisys->alloc.regulation, 13, 1);
    } else {
        func_0200aee8(unisys->alloc.regulation, 13, 0);
    }
}

// pokebw2's UnionOneself_CheckPartyRegulation
static u32 func_ov019_0217ba48(UnionSystem *unisys, u32 *result) {
    *result = 0;
    return func_0201a090(unisys->alloc.regulation, GameData_GetParty(unisys->param->gameData), result);
}

// pokebw2's UnionOneself_CheckBoxRegulation
static u32 func_ov019_0217ba6c(UnionSystem *unisys, u32 *result) {
    PokeParty *party = func_ov019_0217b9cc(unisys);
    u32 ret;

    *result = 0;
    if (party != NULL) {
        ret = func_0201a090(unisys->alloc.regulation, party, result);
        func_ov019_0217b9f4(party);
    } else {
        *result = -1;
        ret = 5;
    }
    return ret;
}

// pokebw2's UnionOneself_CheckRegulation
static BOOL func_ov019_0217baa8(UnionSystem *unisys, u32 activity, u32 *partyResult, u32 *boxResult) {
    u32 partyRet;
    u32 boxRet;

    *partyResult = 0;
    *boxResult = 0;
    if (!func_ov019_0217b9bc(activity)) {
        return TRUE;
    }
    func_ov019_0217ba10(unisys, activity);
    partyRet = func_ov019_0217ba48(unisys, partyResult);
    boxRet = func_ov019_0217ba6c(unisys, boxResult);
    if (partyRet == 0 || boxRet == 0) {
        return TRUE;
    }
    return FALSE;
}

void func_ov019_0217baf0(UnionSystem *unisys, Field *field, u8 disable) {
    unisys->updateDisabled = disable;
    FieldmapCtrlGrid_SetUpdateDisable(field, disable);
}

// pokebw2's UnionOneself_CheckNetError
static BOOL func_ov019_0217bb04(UnionSystem *unisys) {
    if (func_0200fb24()) {
        func_ov019_0217b8e4(unisys, UNION_STEP_RESET);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionOneself_CheckNetErrorEnd
static BOOL func_ov019_0217bb20(UnionSystem *unisys) {
    if (func_ov019_0217bb04(unisys) == TRUE) {
        func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_END);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepIdleBegin
static BOOL func_ov019_0217bb3c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    func_ov012_02173524(unisys, &situ->mycomm);
    func_ov012_021734a8(unisys, 4, 0);
    func_ov019_0217f074(unisys);
    func_ov019_0217baf0(unisys, field, FALSE);
    unisys->unk282E = 0;
    return TRUE;
}

// pokebw2's UnionOneself_StepIdleRun
static BOOL func_ov019_0217bb70(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    FieldPlayer *player = Field_GetPlayer(field);
    MMSys *actorSystem = Field_GetActorSystem(field);
    s16 x, y, z;

    if (func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    if (func_ov012_02172d68(unisys) == TRUE) {
        return FALSE;
    }
    if (situ->nextStep != 0) {
        return TRUE;
    }
    if (func_0203d1b4() > 1) {
        func_ov012_02172d20(unisys);
        return FALSE;
    }
    if (GCTX_HIDGetPressedKeys() & 1) {
        FieldActor *actor;
        u16 id;
        u16 entryIndex;

        FieldPlayer_GetGPosInFront(player, &x, &y, &z);
        actor = GetFirstActorOnGPos(actorSystem, x, z, FALSE);
        if (actor == NULL) {
            return FALSE;
        }
        id = GetActorUID(actor);
        if (func_ov019_0217af38(id) == FALSE || func_ov019_0217af4c(unisys, id) == FALSE) {
            return FALSE;
        }
        situ->mycomm.unk44 = id;
        entryIndex = func_ov019_0217aefc(id);
        func_ov012_021734a8(unisys, 0, (uintptr_t)&unisys->found[entryIndex]);
        if (func_ov019_0217aee8(id) == FALSE) {
            if (unisys->found[entryIndex].beacon.activity == 0) {
                func_ov012_021734a8(unisys, 1, (uintptr_t)&unisys->found[entryIndex]);
                func_ov019_0217b8e4(unisys, UNION_STEP_ASK);
            } else {
                func_ov019_0217b8e4(unisys, UNION_STEP_ANSWER);
            }
        } else {
            func_ov019_0217b8e4(unisys, UNION_STEP_ANSWER_MEMBER);
        }
        func_ov019_0217baf0(unisys, field, TRUE);
        return TRUE;
    }
    if (unisys->unk282E == 1) {
        func_ov019_0217b8e4(unisys, UNION_STEP_GREETING);
        return TRUE;
    }
    if (func_ov019_0217f7e4(field) == TRUE) {
        if (unisys->unk3564 == 1) {
            func_ov019_0217baf0(unisys, field, TRUE);
            func_ov019_0217b8e4(unisys, UNION_STEP_COUNTER);
        }
    } else {
        unisys->unk3564 = 1;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepNetErrorRun
static BOOL func_ov019_0217bcdc(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    switch (*seq) {
    case 0:
        func_ov019_0217baf0(unisys, field, TRUE);
        (*seq)++;
        break;
    case 1:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            func_ov019_0217f074(unisys);
            (*seq)++;
        }
        break;
    case 2:
        if (func_0200fb24()) {
            func_02012124(unisys->param->gsys, 1);
            return FALSE;
        }
        func_ov012_02172d38(unisys);
        (*seq)++;
        break;
    case 3:
        if (!func_ov012_02172d68(unisys)) {
            (*seq)++;
        }
        break;
    case 4:
        func_ov012_02173420(unisys);
        func_ov012_02173524(unisys, &situ->mycomm);
        func_ov012_02172d50(unisys);
        (*seq)++;
        break;
    case 5:
        if (!func_ov012_02172d68(unisys)) {
            (*seq)++;
        }
        break;
    default:
        func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepNone
static BOOL func_ov019_0217bd7c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    return TRUE;
}

// pokebw2's UnionOneself_StepWaitCounter
static BOOL func_ov019_0217bd80(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    if (*seq == 0) {
        unisys->unk3568 = 1;
        (*seq)++;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepGreeting
static BOOL func_ov019_0217bd9c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    PMSSelectParam *param;
    PMSData *result;

    switch (*seq) {
    case 0:
        if (unisys->unk282E == 0) {
            return TRUE;
        }
        unisys->unk282E = 0;
        GFL_SndSEPlay(SEQ_SE_SELECT4);
        param = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(PMSSelectParam), TRUE, "union_oneself.c", 1050);
        param->save = GameData_GetSaveControl(unisys->param->gameData);
        unisys->cmdArg = param;
        func_ov012_0217422c(unisys, 29, param);
        (*seq)++;
        break;
    case 1:
        if (!func_ov012_02174268(unisys)) {
            (*seq)++;
        }
        break;
    case 2:
        param = unisys->cmdArg;
        result = param->result;
        if (param->cancel == 0) {
            func_ov012_021736a0(unisys, (const u16 *)result);
        }
        GFL_HeapFree(unisys->cmdArg);
        unisys->cmdArg = NULL;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepAskBegin
static BOOL func_ov019_0217be44(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    GFL_SndSEPlay(SEQ_SE_MESSAGE);
    func_ov019_0217f210(unisys, field, 0xe2);
    return TRUE;
}

BOOL func_ov019_0217be64(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    int i;

    if (func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    switch (*seq) {
    case 0:
        func_0203d14c(1, 1, NULL);
        situ->wait = 0;
        (*seq)++;
        break;
    case 1:
        if (func_ov019_0217f1ec(unisys)) {
            if (func_0203d1b4() > 1) {
                for (i = 2; i < 5; i++) {
                    if (func_0203d1c0((u8)i) == TRUE) {
                        break;
                    }
                }
                if (i < 5 || func_020392b8() > 2) {
                    func_ov019_0217f210(unisys, field, func_ov019_0217ec04(unisys));
                    func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
                    func_ov012_02172d20(unisys);
                    *seq = 4;
                } else {
                    func_0203d7bc(TRUE);
                    func_0203d1e8(func_020399a8(), 0);
                    *seq = 2;
                }
            } else {
                situ->wait++;
                if (situ->wait > 600 || (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B)) {
                    func_ov012_02172d20(unisys);
                    func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
                    func_ov012_021734a8(unisys, 1, 0);
                    if (situ->wait > 600) {
                        func_ov019_0217f210(unisys, field, func_ov019_0217ec04(unisys));
                    } else {
                        func_ov019_0217f210(unisys, field, 0xa);
                    }
                    *seq = 5;
                }
            }
        }
        break;
    case 2:
        if (situ->mycomm.unk10[6] == 1) {
            if (GFL_STD_MemCmp(situ->mycomm.unk10, situ->mycomm.unk4->mac, 6) != 0) {
                if (func_ov012_02173afc(2) == TRUE) {
                    func_0203d7bc(FALSE);
                    *seq = 6;
                }
            } else if (func_ov012_02173afc(1) == TRUE) {
                GFL_SndSEPlay(SEQ_SE_SYS_101);
                func_ov019_0217b8e4(unisys, UNION_STEP_PARENT);
                func_ov012_021734a8(unisys, 3, (uintptr_t)situ->mycomm.unk4);
                func_ov012_021734a8(unisys, 1, 0);
                *seq = 3;
            }
        }
        break;
    case 3:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            return TRUE;
        }
        break;
    case 4:
        if (func_ov019_0217f1ec(unisys) == TRUE && (GCTX_HIDGetPressedKeys() & UNION_PAD_ANY)) {
            return TRUE;
        }
        break;
    case 5:
        if (!func_ov012_02172d68(unisys)) {
            *seq = 4;
        }
        break;
    case 6:
        if (func_020392b8() <= 1) {
            func_ov012_02172d20(unisys);
            (*seq)++;
        }
        break;
    case 7:
        if (!func_ov012_02172d68(unisys)) {
            situ->mycomm.unk10[6] = 0;
            *seq = 0;
        }
        break;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepAskEnd
static BOOL func_ov019_0217c03c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    func_ov012_021734a8(unisys, 1, 0);
    return TRUE;
}

// pokebw2's UnionOneself_StepAskedBegin
static BOOL func_ov019_0217c04c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    GFL_SndSEPlay(SEQ_SE_SYS_101);
    func_ov019_0217f210(unisys, field, 7);
    func_ov019_0217baf0(unisys, field, TRUE);
    return TRUE;
}

BOOL func_ov019_0217c074(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    if (func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    switch (*seq) {
    case 0:
        GFL_ASSERT(situ->mycomm.answer_pc != NULL);
        func_0203d14c(2, 1, situ->mycomm.answer_pc->mac);
        situ->wait = 0;
        *seq = 1;
        break;
    case 1:
        if (func_0203d1b4() > 1) {
            func_0203d7bc(TRUE);
            *seq = 3;
        } else {
            situ->wait++;
            if (situ->wait > 600 || (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B)) {
                func_ov012_02172d20(unisys);
                func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
                func_ov012_021734a8(unisys, 2, 0);
                if (situ->wait > 600) {
                    func_ov019_0217f210(unisys, field, 8);
                } else {
                    func_ov019_0217f210(unisys, field, 0xa);
                }
                *seq = 2;
            }
        }
        break;
    case 2:
        if (!func_ov012_02172d68(unisys) && func_ov019_0217f1ec(unisys) == TRUE && (GCTX_HIDGetPressedKeys() & UNION_PAD_ANY)) {
            return TRUE;
        }
        break;
    case 3:
        if (func_ov012_02173a90() == TRUE) {
            *seq = 4;
        }
        break;
    case 4:
        switch (situ->mycomm.unk46_2) {
        case 1:
            func_ov012_021734a8(unisys, 3, (uintptr_t)situ->mycomm.answer_pc);
            func_ov012_021734a8(unisys, 2, 0);
            func_ov019_0217b8e4(unisys, UNION_STEP_OFFER_PARTY);
            return TRUE;
        case 2:
            func_ov012_021734a8(unisys, 2, 0);
            func_ov019_0217f210(unisys, field, 8);
            func_ov012_02172d20(unisys);
            *seq = 5;
            break;
        }
        break;
    case 5:
        if (!func_ov012_02172d68(unisys) && func_ov019_0217f1ec(unisys) == TRUE && (GCTX_HIDGetPressedKeys() & UNION_PAD_ANY)) {
            func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepAskedEnd
static BOOL func_ov019_0217c1f0(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    func_ov012_021734a8(unisys, 2, 0);
    return TRUE;
}

// pokebw2's UnionOneself_StepParentBegin
static BOOL func_ov019_0217c200(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    if (func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    if (situ->prevStep == UNION_STEP_RULE_SELECT) {
        situ->result = 1;
        return TRUE;
    }
    situ->result = 0;
    switch (*seq) {
    case 0:
        func_ov012_02173518(&situ->mycomm);
        func_ov012_021734a8(unisys, 4, 1);
        func_ov012_021735fc(&situ->mycomm, situ->mycomm.parent);
        func_02039c68(func_020399a8(), 2, 0x14);
        (*seq)++;
        break;
    case 1:
        if (func_02039cb4(func_020399a8(), 2, 0x14) == TRUE) {
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov012_02173b4c(unisys) == TRUE) {
            (*seq)++;
        }
        break;
    case 3:
        if (MATH_CountPopulation(situ->mycomm.unk47) > 1) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepParentRun
static BOOL func_ov019_0217c2ac(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    PokeParty *party;
    intptr_t choice;

    if (func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    switch (*seq) {
    case 0:
        if (situ->result == 1) {
            situ->result = 0;
        } else if (situ->mycomm.unk48 == 0) {
            func_ov019_0217f210(unisys, field, func_ov019_0217ec1c(situ->mycomm.target->beacon.gender));
            situ->mycomm.unk48++;
        } else {
            func_ov019_0217f210(unisys, field, func_ov019_0217ec88(situ->mycomm.target->beacon.gender));
        }
        (*seq)++;
        break;
    case 1:
        if (func_ov019_0217f1ec(unisys) != TRUE) {
            break;
        }
        (*seq)++;
        break;
    case 2:
        func_ov019_0217f360(unisys, field);
        (*seq)++;
        break;
    case 3:
        party = GameData_GetParty(unisys->param->gameData);
        choice = func_ov019_0217f380(unisys, field);
        if (choice == -1) {
            break;
        }
        func_ov019_0217f378(unisys);
        if (choice == -2 || choice == 0) {
            func_ov019_0217f210(unisys, field, func_ov019_0217ec28(situ->mycomm.target->beacon.gender));
            situ->mycomm.unk19 = 0;
            func_ov019_0217b8e4(unisys, UNION_STEP_OFFER);
            return TRUE;
        }
        if (choice == UNION_ACTIVITY_TRADE) {
            GSYS_GetGameData(unisys->param->gsys);
            if (howManyPartyPokesAreNotEggs(party) < 2) {
                func_ov019_0217f210(unisys, field, func_ov019_0217eca0(situ->mycomm.target->beacon.gender));
                *seq = 4;
                break;
            }
        } else if (choice == UNION_ACTIVITY_EGG_TRADE) {
            if (countSanityEggsInParty(party) > 0) {
                func_ov019_0217f210(unisys, field, func_ov019_0217ecb8(unisys));
                *seq = 4;
                break;
            } else if (countAllEggsInParty(party) == 0) {
                func_ov019_0217f210(unisys, field, func_ov019_0217ecac(situ->mycomm.target->beacon.gender));
                *seq = 4;
                break;
            }
        }
        if (choice == UNION_ACTIVITY_NONE) {
            func_ov019_0217b8e4(unisys, UNION_STEP_RULE_SELECT);
            return TRUE;
        }
        situ->mycomm.unk19 = choice;
        func_ov019_0217b8e4(unisys, UNION_STEP_OFFER);
        return TRUE;
    case 4:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            *seq = 0;
        }
        break;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepParentEnd
static BOOL func_ov019_0217c444(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    func_ov019_0217f378(unisys);
    return TRUE;
}

// pokebw2's UnionOneself_StepOfferRun
static BOOL func_ov019_0217c450(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    if (func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    switch (*seq) {
    case 0:
        if (func_ov012_02173a04(situ->mycomm.unk19) == TRUE) {
            if (situ->mycomm.unk19 == 0) {
                func_ov019_0217b8e4(unisys, UNION_STEP_LEAVE);
                return TRUE;
            }
            func_ov019_0217f210(unisys, field,
                                func_ov019_0217ec40(situ->mycomm.unk19, situ->mycomm.target->beacon.gender));
            (*seq)++;
        }
        break;
    case 1:
        if (func_ov019_0217f1ec(unisys)) {
            if (situ->mycomm.unk1A == 1) {
                func_ov019_0217b8e4(unisys, UNION_STEP_FOR_ACTIVITY(situ->mycomm.unk19));
                (*seq)++;
            } else if (situ->mycomm.unk1A == 0) {
                func_ov019_0217b8e4(unisys, UNION_STEP_LEAVE);
                func_ov019_0217f210(unisys, field,
                                    func_ov019_0217ec54(situ->mycomm.unk19, situ->mycomm.target->beacon.gender));
                return TRUE;
            }
        }
        break;
    case 2:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepOfferBegin
static BOOL func_ov019_0217c518(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    if (func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    switch (*seq) {
    case 0:
        func_ov012_02173518(&situ->mycomm);
        func_ov012_021734a8(unisys, 4, 1);
        func_ov012_021735fc(&situ->mycomm, situ->mycomm.parent);
        func_02039c68(func_020399a8(), 2, 0x14);
        (*seq)++;
        break;
    case 1:
        if (func_02039cb4(func_020399a8(), 2, 0x14) == TRUE) {
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov012_02173b4c(unisys) == TRUE) {
            (*seq)++;
        }
        break;
    case 3:
        if (MATH_CountPopulation(situ->mycomm.unk47) > 1) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov019_0217c5a8(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    PokeParty *party;
    BOOL yes;

    if (func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    if (func_ov019_0217f1ec(unisys) == FALSE) {
        return FALSE;
    }
    switch (*seq) {
    case 0:
        func_ov019_0217f210(unisys, field, 9);
        (*seq)++;
        break;
    case 1:
        if (situ->mycomm.unk19 == 0xff) {
            break;
        }
        if (situ->mycomm.unk19 == 0) {
            func_ov019_0217f210(unisys, field, func_ov019_0217ec34(situ->mycomm.target->beacon.gender));
            func_ov019_0217b8e4(unisys, UNION_STEP_LEAVE);
            return TRUE;
        }
        if (situ->mycomm.unk19 < 0x1b) {
            func_ov019_0217f210(unisys, field, func_ov019_0217ec68(situ->mycomm.unk19));
            if (func_ov019_0217b9bc(situ->mycomm.unk19) == TRUE) {
                func_ov019_0217ba10(unisys, situ->mycomm.unk19);
                func_ov019_0217f640(unisys, field, 0, func_ov019_0217b974(situ->mycomm.unk19), 0);
                *seq = 2;
            } else {
                *seq = 3;
            }
        } else if (func_ov012_02173a48(0) == TRUE) {
            func_ov019_0217f210(unisys, field, 0x13d);
            func_ov019_0217b8e4(unisys, UNION_STEP_LEAVE);
            return TRUE;
        }
        break;
    case 2:
        if (func_ov019_0217f794(unisys, field) == TRUE) {
            *seq = 3;
        }
        break;
    case 3:
        func_ov019_0217f7b8(unisys);
        func_ov019_0217f22c(unisys, field);
        (*seq)++;
        break;
    case 4:
        situ->result = 0;
        if (func_ov019_0217f268(unisys, &yes) == TRUE) {
            party = GameData_GetParty(unisys->param->gameData);
            func_ov019_0217f24c(unisys);
            if (func_ov019_0217baa8(unisys, situ->mycomm.unk19, &situ->partyCheck, &situ->boxCheck) == FALSE) {
                func_ov019_0217f210(unisys, field, 0x13e);
                yes = FALSE;
            } else if (situ->mycomm.unk19 == UNION_ACTIVITY_TRADE && yes == TRUE) {
                GSYS_GetGameData(unisys->param->gsys);
                if (howManyPartyPokesAreNotEggs(party) < 2) {
                    yes = FALSE;
                    situ->result = 1;
                }
            } else if (situ->mycomm.unk19 == UNION_ACTIVITY_EGG_TRADE && yes == TRUE) {
                if (countSanityEggsInParty(party) > 0) {
                    yes = FALSE;
                    situ->result = 3;
                } else if (countAllEggsInParty(party) == 0) {
                    yes = FALSE;
                    situ->result = 2;
                }
            }
            situ->mycomm.unk1A = yes;
            (*seq)++;
        }
        break;
    case 5:
        if (func_ov012_02173a48(situ->mycomm.unk1A) == TRUE) {
            if (situ->partyCheck != 0 && situ->boxCheck != 0) {
                *seq = 6;
            } else {
                *seq = 10;
            }
        }
        break;
    case 6:
        func_ov019_0217f640(unisys, field, situ->partyCheck, func_ov019_0217b974(situ->mycomm.unk19), 1);
        (*seq)++;
        break;
    case 7:
        if (func_ov019_0217f794(unisys, field) == TRUE && (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            func_ov019_0217f7b8(unisys);
            if (situ->boxCheck == -1) {
                *seq = 10;
            } else {
                (*seq)++;
            }
        }
        break;
    case 8:
        func_ov019_0217f640(unisys, field, situ->partyCheck, func_ov019_0217b974(situ->mycomm.unk19), 2);
        (*seq)++;
        break;
    case 9:
        if (func_ov019_0217f794(unisys, field) == TRUE && (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            func_ov019_0217f7b8(unisys);
            *seq = 10;
        }
        break;
    case 10:
        if (situ->mycomm.unk1A == 0) {
            if (situ->result == 1) {
                func_ov019_0217f210(unisys, field, 0x28);
            } else if (situ->result == 2) {
                func_ov019_0217f210(unisys, field, 0x2f);
            } else if (situ->result == 3) {
                func_ov019_0217f210(unisys, field, func_ov019_0217ecd0(unisys));
            } else {
                func_ov019_0217f210(unisys, field, func_ov019_0217ec78(situ->mycomm.unk19));
            }
            func_ov019_0217b8e4(unisys, UNION_STEP_LEAVE);
            return TRUE;
        }
        func_ov019_0217b8e4(unisys, UNION_STEP_FOR_ACTIVITY(situ->mycomm.unk19));
        *seq = 11;
        break;
    case 11:
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov019_0217c888(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    return TRUE;
}

BOOL func_ov019_0217c88c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    BOOL picked;
    BOOL isFinal;
    u32 choice;

    if (func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    if (func_ov019_0217f1ec(unisys) == FALSE) {
        return FALSE;
    }
    switch (*seq) {
    case 0:
        situ->result = 0;
        (*seq)++;
        break;
    case 1:
        if (situ->result == 5) {
            func_ov019_0217f5e0(unisys, field);
        } else if (situ->result == 0) {
            func_ov019_0217f624(unisys);
        }
        func_ov019_0217f388(unisys, field, situ->result, situ->ruleSel);
        (*seq)++;
        break;
    case 2:
        choice = func_ov019_0217f4b4(unisys, &picked, situ->ruleSel, &isFinal, field);
        if (picked == TRUE) {
            situ->result = choice;
            func_ov019_0217f4ac(unisys);
            (*seq)--;
        } else if (choice == -2 || choice == 0) {
            func_ov019_0217f4ac(unisys);
            if (situ->result > 0) {
                situ->result = 0;
                (*seq)--;
            } else {
                func_ov019_0217f624(unisys);
                func_ov019_0217b8e4(unisys, UNION_STEP_PARENT);
                return TRUE;
            }
        } else {
            if (isFinal == TRUE) {
                func_ov019_0217f624(unisys);
                func_ov019_0217f4ac(unisys);
                func_ov019_0217ba10(unisys, choice);
                situ->mycomm.unk19 = choice;
                *seq = 3;
            } else {
                if (choice == -1) {
                    break;
                }
                func_ov019_0217f624(unisys);
                func_ov019_0217f4ac(unisys);
                if (func_ov019_0217baa8(unisys, choice, &situ->partyCheck, &situ->boxCheck) == FALSE) {
                    func_ov019_0217f210(unisys, field, 0x13e);
                    *seq = 5;
                } else {
                    situ->mycomm.unk19 = choice;
                    func_ov019_0217b8e4(unisys, UNION_STEP_OFFER);
                    return TRUE;
                }
            }
            break;
        }
        break;
    case 3:
        func_ov019_0217f640(unisys, field, 0, func_ov019_0217b974(situ->mycomm.unk19), 0);
        (*seq)++;
        break;
    case 4:
        if (func_ov019_0217f794(unisys, field) == TRUE && (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            func_ov019_0217f7b8(unisys);
            *seq = 1;
        }
        break;
    case 5:
        func_ov019_0217f640(unisys, field, situ->partyCheck, func_ov019_0217b974(situ->mycomm.unk19), 1);
        (*seq)++;
        break;
    case 6:
        if (func_ov019_0217f794(unisys, field) == TRUE && (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            func_ov019_0217f7b8(unisys);
            if (situ->boxCheck == -1) {
                *seq = 9;
            } else {
                (*seq)++;
            }
        }
        break;
    case 7:
        func_ov019_0217f640(unisys, field, situ->partyCheck, func_ov019_0217b974(situ->mycomm.unk19), 2);
        (*seq)++;
        break;
    case 8:
        if (func_ov019_0217f794(unisys, field) == TRUE && (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            func_ov019_0217f7b8(unisys);
            *seq = 9;
        }
        break;
    case 9:
        func_ov019_0217f210(unisys, field, func_ov019_0217ec88(situ->mycomm.target->beacon.gender));
        *seq = 0;
        break;
    }
    return FALSE;
}

BOOL func_ov019_0217caac(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    int index = func_ov019_0217aefc(situ->mycomm.unk44);
    int activity = unisys->found[index].beacon.activity;
    PokeParty *party;

    switch (*seq) {
    case 0:
        situ->mycomm.unk19 = activity;
        switch (activity) {
        case 1:
            func_ov019_0217f210(unisys, field, func_ov019_0217ed04(situ->mycomm.target->beacon.gender));
            func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
            *seq = 2;
            break;
        case 2:
            func_ov019_0217f210(unisys, field, func_ov019_0217ed10(situ->mycomm.target->beacon.gender));
            func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
            *seq = 2;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
            func_ov019_0217f210(unisys, field, func_ov019_0217ece8(situ->mycomm.target->beacon.gender));
            func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
            *seq = 2;
            break;
        case 20:
        case 21:
        case 22:
        case 23:
            if (unisys->found[index].beacon.memberCount < func_ov019_0217f060(activity)) {
                func_ov019_0217f210(unisys, field, func_ov019_0217ed80(situ->mycomm.target->beacon.gender));
                func_ov019_0217ba10(unisys, activity);
                func_ov019_0217f640(unisys, field, 0, func_ov019_0217b974(activity), 0);
                *seq = 3;
                break;
            }
            func_ov019_0217f210(unisys, field, func_ov019_0217ece8(situ->mycomm.target->beacon.gender));
            func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
            *seq = 2;
            break;
        case 25:
            if (unisys->found[index].beacon.memberCount < func_ov019_0217f060(activity)
                && unisys->found[index].beacon.entryClosed == 0) {
                func_ov019_0217f210(unisys, field, func_ov019_0217ed8c(situ->mycomm.target->beacon.gender));
                *seq = 10;
                break;
            }
            func_ov019_0217f210(unisys, field, func_ov019_0217ed64(situ->mycomm.target->beacon.gender));
            func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
            *seq = 2;
            break;
        case 3:
            if (unisys->found[index].beacon.memberCount < func_ov019_0217f060(activity)) {
                func_ov019_0217f210(unisys, field, func_ov019_0217ed98(situ->mycomm.target->beacon.gender));
                *seq = 10;
                break;
            }
            func_ov019_0217f210(unisys, field, func_ov019_0217ed2c(situ->mycomm.target->beacon.gender));
            func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
            *seq = 2;
            break;
        case 24:
            func_ov019_0217f210(unisys, field, func_ov019_0217ed48(situ->mycomm.target->beacon.gender));
            func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
            *seq = 2;
            break;
        default:
            func_ov019_0217f210(unisys, field, 0x151);
            func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
            *seq = 2;
            break;
        }
        break;
    case 2:
        if (func_ov019_0217f1ec(unisys) == TRUE && (GCTX_HIDGetPressedKeys() & UNION_PAD_ANY)) {
            return TRUE;
        }
        break;
    case 1:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            return TRUE;
        }
        break;
    case 3:
        if (func_ov019_0217f794(unisys, field) == TRUE) {
            *seq = 4;
        }
        break;
    case 4:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            func_ov019_0217f7b8(unisys);
            func_ov019_0217f22c(unisys, field);
            (*seq)++;
        }
        break;
    case 5: {
        BOOL yes;

        if (func_ov019_0217f268(unisys, &yes) == TRUE) {
            func_ov019_0217f24c(unisys);
            if (yes == FALSE) {
                func_ov019_0217f210(unisys, field, func_ov019_0217eda4(situ->mycomm.target->beacon.gender));
                func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
                *seq = 2;
            } else if (func_ov019_0217baa8(unisys, situ->mycomm.unk19, &situ->partyCheck, &situ->boxCheck) == FALSE) {
                func_ov019_0217f210(unisys, field, 0x13e);
                func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
                *seq = 6;
            } else {
                situ->mycomm.unk1A = yes;
                func_ov019_0217b8e4(unisys, UNION_STEP_RECRUIT);
                func_ov019_0217f210(unisys, field, func_ov019_0217ee28(situ->mycomm.target->beacon.gender));
                *seq = 1;
            }
        }
        break;
    }
    case 6:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            return TRUE;
        }
        func_ov019_0217f640(unisys, field, situ->partyCheck, func_ov019_0217b974(situ->mycomm.unk19), 1);
        (*seq)++;
        break;
    case 7:
        if (func_ov019_0217f794(unisys, field) == TRUE && (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            func_ov019_0217f7b8(unisys);
            if (situ->boxCheck == -1) {
                *seq = 1;
            } else {
                (*seq)++;
            }
        }
        break;
    case 8:
        func_ov019_0217f640(unisys, field, situ->partyCheck, func_ov019_0217b974(situ->mycomm.unk19), 2);
        (*seq)++;
        break;
    case 9:
        if (func_ov019_0217f794(unisys, field) == TRUE && (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            func_ov019_0217f7b8(unisys);
            *seq = 1;
        }
        break;
    case 10:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            func_ov019_0217f22c(unisys, field);
            (*seq)++;
        }
        break;
    case 11: {
        BOOL yes;

        if (func_ov019_0217f268(unisys, &yes) == TRUE) {
            func_ov019_0217f24c(unisys);
            if (yes == FALSE) {
                switch (situ->mycomm.unk19) {
                case 3:
                    func_ov019_0217f210(unisys, field, func_ov019_0217edbc(situ->mycomm.target->beacon.gender));
                    break;
                case UNION_ACTIVITY_EGG_TRADE:
                    func_ov019_0217f210(unisys, field, func_ov019_0217edb0(situ->mycomm.target->beacon.gender));
                    break;
                }
                func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
                *seq = 2;
            } else {
                situ->mycomm.unk1A = yes;
                func_ov019_0217b8e4(unisys, UNION_STEP_RECRUIT);
                *seq = 1;
                switch (situ->mycomm.unk19) {
                case 3:
                    func_ov019_0217f210(unisys, field, func_ov019_0217ee04(situ->mycomm.target->beacon.gender));
                    break;
                case UNION_ACTIVITY_EGG_TRADE:
                    party = GameData_GetParty(unisys->param->gameData);
                    if (countSanityEggsInParty(party) > 0) {
                        func_ov019_0217f210(unisys, field, func_ov019_0217edd4(situ->mycomm.target->beacon.gender));
                        func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
                        *seq = 2;
                    } else if (countAllEggsInParty(party) == 0) {
                        func_ov019_0217f210(unisys, field, func_ov019_0217edc8(situ->mycomm.target->beacon.gender));
                        func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
                        *seq = 2;
                    } else {
                        func_ov019_0217f210(unisys, field, func_ov019_0217ede0(situ->mycomm.target->beacon.gender));
                    }
                    break;
                }
            }
        }
        break;
    }
    }
    return FALSE;
}

BOOL func_ov019_0217ceb8(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    u8 gender;

    switch (*seq) {
    case 0:
        gender = func_ov019_0217af04(unisys, situ->mycomm.unk44);
        switch (unisys->found[func_ov019_0217aefc(situ->mycomm.unk44)].beacon.activity) {
        case 1:
            func_ov019_0217f210(unisys, field, func_ov019_0217ed04(gender));
            break;
        case 2:
            func_ov019_0217f210(unisys, field, func_ov019_0217ed10(gender));
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
            func_ov019_0217f210(unisys, field, func_ov019_0217ece8(gender));
            break;
        case 25:
            func_ov019_0217f210(unisys, field, func_ov019_0217ed64(gender));
            break;
        case 3:
            func_ov019_0217f210(unisys, field, func_ov019_0217ed2c(gender));
            break;
        case 24:
            func_ov019_0217f210(unisys, field, func_ov019_0217ed48(gender));
            break;
        default:
            func_ov019_0217f210(unisys, field, 0x151);
            break;
        }
        func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
        (*seq)++;
        break;
    case 1:
        if (func_ov019_0217f1ec(unisys) == TRUE && (GCTX_HIDGetPressedKeys() & UNION_PAD_ANY)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov019_0217cfa8(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    if (func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    switch (*seq) {
    case 0:
        GFL_ASSERT((unisys->alloc.my_card == NULL) && (unisys->alloc.target_card == NULL));
        unisys->alloc.my_card = GFL_HeapAllocate(0x41, sizeof(TrainerCardData), TRUE, "union_oneself.c", 0x9b3);
        unisys->alloc.target_card = GFL_HeapAllocate(0x41, sizeof(TrainerCardData), TRUE, "union_oneself.c", 0x9b5);
        func_ov010_0216f40c(unisys->alloc.my_card, unisys->param->gameData, 1, 0, 0x41);
        MI_CpuCopy8(unisys->alloc.my_card, unisys->alloc.target_card, sizeof(TrainerCardData));
        func_02039c68(func_020399a8(), 3, 0x14);
        func_ov019_0217f210(unisys, field,
                            func_ov019_0217ee4c(unisys, situ->mycomm.target->beacon.country,
                                                situ->mycomm.target->beacon.province));
        (*seq)++;
        break;
    case 1:
        if (func_02039cb4(func_020399a8(), 3, 0x14) == TRUE) {
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov012_02173bb0(unisys) == TRUE) {
            (*seq)++;
        }
        break;
    case 3:
        if (situ->mycomm.recvFlag == 1 && func_ov019_0217f1ec(unisys) == TRUE) {
            unisys->alloc.target_card->unk04_5 = TRUE;
            unisys->alloc.cardParam = func_ov010_0216f7f4(unisys->param->gameData, unisys->alloc.target_card, 0x41);
            (*seq)++;
        }
        break;
    case 4:
        func_02039c68(func_020399a8(), 4, 0x14);
        (*seq)++;
        break;
    case 5:
        if (func_02039cb4(func_020399a8(), 4, 0x14) == TRUE) {
            (*seq)++;
        }
        break;
    case 6:
        func_ov012_0217422c(unisys, 1, unisys->alloc.cardParam);
        (*seq)++;
        break;
    case 7:
        if (func_ov012_02174268(unisys) == FALSE) {
            (*seq)++;
        }
        break;
    case 8:
        func_ov019_0217f210(unisys, field, 0x10);
        func_02039c68(func_020399a8(), 5, 0x14);
        (*seq)++;
        break;
    case 9:
        if (func_ov019_0217f1ec(unisys) && func_02039cb4(func_020399a8(), 5, 0x14) == TRUE) {
            GFL_HeapFree(unisys->alloc.my_card);
            GFL_HeapFree(unisys->alloc.target_card);
            unisys->alloc.cardParam = NULL;
            unisys->alloc.my_card = NULL;
            unisys->alloc.target_card = NULL;
            if (func_0203d378() == TRUE) {
                func_ov019_0217b8e4(unisys, UNION_STEP_PARENT);
            } else {
                func_ov019_0217b8e4(unisys, UNION_STEP_OFFER_PARTY);
            }
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov019_0217d18c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    if (*seq != 4 && func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    switch (*seq) {
    case 0:
        func_ov019_0217f210(unisys, field, 0x26);
        (*seq)++;
        break;
    case 1:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            func_02039c68(func_020399a8(), 6, 0x14);
            (*seq)++;
        }
        break;
    case 2:
        if (func_02039cb4(func_020399a8(), 6, 0x14) == TRUE) {
            GameData *gameData = GSYS_GetGameData(unisys->param->gsys);
            RivalDataSave *rival = getHollow_RivalData(GameData_GetSaveControl(gameData));

            func_0200ed54(rival, getIDAsUInt(func_02012950(gameData, func_0203d1a4(func_020399a8()) ^ 1)));
            (*seq)++;
        }
        break;
    case 3: {
        PokemonTradeParam *param = GFL_HeapAllocate(0x41, sizeof(PokemonTradeParam), TRUE, "union_oneself.c", 0xa54);
        ShinkaDemoParam *evolution;

        param->gameData = GSYS_GetGameData(unisys->param->gsys);
        param->party = PokeParty_Create(0x41);
        evolution = GFL_HeapAllocate(4, sizeof(ShinkaDemoParam), FALSE, "union_oneself.c", 0xa5b);
        evolution->gameData = param->gameData;
        evolution->party = param->party;
        evolution->species = 0;
        evolution->partyIndex = 0;
        evolution->method = 0;
        evolution->unkC = 1;
        evolution->canCancel = FALSE;
        param->evolution = evolution;
        param->next = 0;
        unisys->cmdArg = param;
        func_ov012_0217422c(unisys, 2, param);
        (*seq)++;
        break;
    }
    case 4:
        if (func_ov012_02174268(unisys) == FALSE) {
            PokemonTradeParam *param = unisys->cmdArg;

            GFL_HeapFree(param->evolution);
            GFL_HeapFree(param->party);
            GFL_HeapFree(unisys->cmdArg);
            unisys->cmdArg = NULL;
            (*seq)++;
        }
        break;
    case 5:
        func_ov019_0217f210(unisys, field, 0x10);
        func_02039c68(func_020399a8(), 7, 0x14);
        (*seq)++;
        break;
    case 6:
        if (func_ov019_0217f1ec(unisys) && func_02039cb4(func_020399a8(), 7, 0x14) == TRUE) {
            if (func_0203d378() == TRUE) {
                func_ov019_0217b8e4(unisys, UNION_STEP_PARENT);
            } else {
                func_ov019_0217b8e4(unisys, UNION_STEP_OFFER_PARTY);
            }
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov019_0217d314(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    int index = func_ov019_0217aefc(situ->mycomm.unk44);
    int max;

    if (func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    switch (*seq) {
    case 0:
        situ->mycomm.unk46_0 = 1;
        func_ov012_021736e0(situ->mycomm.unk19);
        func_0203d14c(2, 1, unisys->found[index].mac);
        situ->wait = 0;
        (*seq)++;
        break;
    case 1:
        if (func_ov019_0217f1ec(unisys) == FALSE) {
            break;
        }
        if (func_0203d1b4() > 1) {
            func_0203d7bc(TRUE);
            max = func_ov019_0217f060(situ->mycomm.unk19);
            if (max <= func_0203d1a4(func_020399a8())) {
                *seq = 4;
            } else {
                func_ov012_021734a8(unisys, 3, (uintptr_t)&unisys->found[index]);
                situ->mycomm.unk46_1 = 1;
                switch (situ->mycomm.unk19) {
                case 20:
                    func_ov019_0217b8e4(unisys, UNION_STEP_JOIN_MULTI_0);
                    break;
                case 21:
                    func_ov019_0217b8e4(unisys, UNION_STEP_JOIN_MULTI_1);
                    break;
                case 22:
                    func_ov019_0217b8e4(unisys, UNION_STEP_JOIN_MULTI_2);
                    break;
                case 23:
                    func_ov019_0217b8e4(unisys, UNION_STEP_JOIN_MULTI_3);
                    break;
                case 3:
                    func_ov019_0217b8e4(unisys, UNION_STEP_JOIN_DRAW);
                    break;
                case 25:
                    func_ov019_0217b8e4(unisys, UNION_STEP_JOIN_EGG);
                    break;
                default:
                    GFL_ASSERT(0);
                    break;
                }
                *seq = 3;
            }
        } else {
            situ->wait++;
            if (situ->wait > 600 || (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B)) {
                situ->mycomm.unk46_0 = 0;
                func_ov019_0217b8e4(unisys, UNION_STEP_IDLE);
                *seq = 4;
            }
        }
        break;
    case 2:
        switch (situ->mycomm.unk19) {
        case 20:
        case 21:
        case 22:
        case 23:
            func_ov019_0217f210(unisys, field, func_ov019_0217ee40(situ->mycomm.target->beacon.gender));
            break;
        case 3:
            func_ov019_0217f210(unisys, field, func_ov019_0217ee1c(situ->mycomm.target->beacon.gender));
            break;
        case 25:
            func_ov019_0217f210(unisys, field, func_ov019_0217edf8(situ->mycomm.target->beacon.gender));
            break;
        }
        *seq = 3;
        break;
    case 3:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            return TRUE;
        }
        break;
    case 4:
        func_ov012_02172d20(unisys);
        (*seq)++;
        break;
    case 5:
        if (func_ov012_02172d68(unisys) == FALSE) {
            *seq = 2;
        }
        break;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepLeaveRun
static BOOL func_ov019_0217d51c(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    if (*seq < 3 && func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    switch (*seq) {
    case 0:
        func_ov012_02172edc(unisys);
        (*seq)++;
        break;
    case 1:
        if (!func_ov012_02172d68(unisys) && func_0203cdbc() == TRUE) {
            if (situ->mycomm.parent != NULL) {
                func_ov012_02173650(&situ->mycomm, situ->mycomm.parent);
                func_ov012_021734a8(unisys, 3, 0);
            }
            func_ov012_02172d50(unisys);
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov012_02172d68(unisys)) {
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov019_0217f1ec(unisys) == TRUE && (GCTX_HIDGetPressedKeys() & UNION_PAD_ANY)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov019_0217d5b8(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    if (*seq != 4 && *seq != 5 && *seq != 8 && func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    switch (*seq) {
    case 0:
        if (situ->mycomm.unk46_1 == 1) {
            *seq = 6;
            break;
        }
        (*seq)++;
        break;
    case 1:
        func_02039c68(func_020399a8(), 8, 0x14);
        (*seq)++;
        break;
    case 2:
        if (func_02039cb4(func_020399a8(), 8, 0x14) == TRUE) {
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            if (situ->step == UNION_STEP_JOIN_DRAW) {
                UnionDrawParam *param = GFL_HeapAllocate(0x41, sizeof(UnionDrawParam), TRUE, "union_oneself.c", 0xb9a);

                param->gameData = unisys->param->gameData;
                unisys->cmdArg = param;
                func_ov012_0217422c(unisys, 3, param);
            } else {
                UnionBattleParam *param = GFL_HeapAllocate(0x41, sizeof(UnionBattleParam), TRUE, "union_oneself.c", 0xba2);

                param->gameData = unisys->param->gameData;
                param->party = GameData_GetParty(unisys->param->gameData);
                param->save = GameData_GetSaveControl(unisys->param->gameData);
                param->records = GameData_GetRecords(unisys->param->gameData);
                unisys->cmdArg = param;
                func_ov012_0217422c(unisys, 4, param);
            }
            (*seq)++;
        }
        break;
    case 4:
        if (!func_ov012_02174268(unisys)) {
            GFL_HeapFree(unisys->cmdArg);
            unisys->cmdArg = NULL;
            if (func_0200fb24()) {
                func_ov019_0217b8e4(unisys, UNION_STEP_RESET);
                return TRUE;
            }
            func_ov012_02172d50(unisys);
            (*seq)++;
        }
        break;
    case 5:
        if (!func_ov012_02172d68(unisys)) {
            return TRUE;
        }
        break;
    case 6:
        if (func_ov012_02173d38(unisys) == TRUE) {
            (*seq)++;
        }
        break;
    case 7:
        switch (unisys->entryReply) {
        case 1:
            if (situ->step == UNION_STEP_JOIN_DRAW) {
                func_ov019_0217f210(unisys, field, func_ov019_0217ee10(situ->mycomm.target->beacon.gender));
            } else {
                func_ov019_0217f210(unisys, field, func_ov019_0217edec(situ->mycomm.target->beacon.gender));
            }
            *seq = 3;
            break;
        case 2:
            if (situ->step == UNION_STEP_JOIN_DRAW) {
                func_ov019_0217f210(unisys, field, func_ov019_0217ee1c(situ->mycomm.target->beacon.gender));
            } else {
                func_ov019_0217f210(unisys, field, func_ov019_0217edf8(situ->mycomm.target->beacon.gender));
            }
            func_ov012_02172d20(unisys);
            *seq = 8;
            break;
        }
        break;
    case 8:
        if (func_ov019_0217f1ec(unisys) == TRUE && !func_ov012_02172d68(unisys)) {
            func_ov012_021734a8(unisys, 3, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov019_0217d7a4(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    if (*seq < 4 && func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    switch (*seq) {
    case 0:
        GFL_ASSERT(unisys->colosseum_sys == NULL);
        unisys->colosseum_sys = func_ov012_02174f4c(unisys->param->gameData, unisys->param->gsys,
                                                    unisys->param->playerInfo, situ->mycomm.unk46_1);
        func_ov019_0217f210(unisys, field, func_ov019_0217ec94(situ->mycomm.target->beacon.gender));
        (*seq)++;
        break;
    case 1:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            (*seq)++;
        }
        break;
    case 2:
        func_02039c68(func_020399a8(), 0xb, 0x14);
        (*seq)++;
        break;
    case 3:
        if (func_02039cb4(func_020399a8(), 0xb, 0x14) == TRUE) {
            (*seq)++;
        }
        break;
    case 4:
        func_ov012_0217422c(unisys, situ->mycomm.unk19 + 1, NULL);
        (*seq)++;
        break;
    case 5:
        if (!func_ov012_02174268(unisys)) {
            func_ov019_0217baf0(unisys, field, TRUE);
            func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov019_0217d890(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    if (*seq < 4 && func_ov019_0217bb04(unisys) == TRUE) {
        return TRUE;
    }
    switch (*seq) {
    case 0:
        if (situ->mycomm.unk46_1 == 0) {
            func_ov019_0217f210(unisys, field, func_ov019_0217ec94(situ->mycomm.target->beacon.gender));
        } else {
            func_ov019_0217f210(unisys, field, func_ov019_0217ee34(situ->mycomm.target->beacon.gender));
        }
        (*seq)++;
        break;
    case 1:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            (*seq)++;
        }
        break;
    case 2:
        if (situ->mycomm.unk46_1 == 0) {
            func_02039c68(func_020399a8(), 0xb, 0x14);
        }
        (*seq)++;
        break;
    case 3:
        if (situ->mycomm.unk46_1 == 0) {
            if (func_02039cb4(func_020399a8(), 0xb, 0x14) == TRUE) {
                (*seq)++;
            }
        } else {
            (*seq)++;
        }
        break;
    case 4:
        func_ov012_0217422c(unisys, situ->mycomm.unk19 + 1, NULL);
        (*seq)++;
        break;
    case 5:
        if (!func_ov012_02174268(unisys)) {
            GFL_ASSERT(unisys->colosseum_sys == NULL);
            unisys->colosseum_sys = func_ov012_02174f4c(unisys->param->gameData, unisys->param->gsys,
                                                        unisys->param->playerInfo, situ->mycomm.unk46_1);
            func_ov019_0217baf0(unisys, field, TRUE);
            func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov019_0217d9a8(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    UnionColosseum *colosseum = unisys->colosseum_sys;
    PlayerInfo *info;

    func_0203d1a4(func_020399a8());
    GFL_ASSERT(colosseum != NULL);
    if (func_ov019_0217bb20(unisys) == TRUE) {
        if (colosseum->unk168 != NULL) {
            func_ov021_021cd5a4(colosseum->unk168);
        }
        func_ov019_0217f074(unisys);
        return TRUE;
    }
    switch (*seq) {
    case 0:
        if (func_ov019_0217f060(situ->mycomm.unk19) == 2) {
            colosseum->unkDE = 1;
            if (func_0203d378() == TRUE) {
                if (func_ov012_02173cb4() == TRUE) {
                    *seq = 0x64;
                }
            } else {
                *seq = 0x64;
            }
            break;
        }
        func_0203d7bc(FALSE);
        if (func_0203d378() == TRUE) {
            info = GetGameDataPlayerInfo(unisys->param->gameData);
            colosseum->unk168 = func_ov021_021cd48c(info, field, func_ov019_0217f060(situ->mycomm.unk19),
                                                    func_ov019_0217f060(situ->mycomm.unk19), 0x41, 0, 0, 0);
            func_ov021_021cd60c(colosseum->unk168, unisys, func_ov019_0217dc64, func_ov019_0217dc94);
        } else if (situ->mycomm.unk46_1 == 0) {
            info = GetGameDataPlayerInfo(unisys->param->gameData);
            colosseum->unk168 = func_ov021_021cd48c(info, field, func_ov019_0217f060(situ->mycomm.unk19),
                                                    func_ov019_0217f060(situ->mycomm.unk19), 0x41, 2, 0, 0);
        } else {
            info = GetGameDataPlayerInfo(unisys->param->gameData);
            colosseum->unk168 = func_ov021_021cd48c(info, field, func_ov019_0217f060(situ->mycomm.unk19),
                                                    func_ov019_0217f060(situ->mycomm.unk19), 0x41, 3, 0, 0);
        }
        if (situ->mycomm.unk46_1 == 0) {
            func_ov012_021736e0(situ->activity);
            func_02039c68(func_020399a8(), 0xc, 0x14);
        }
        (*seq)++;
        break;
    case 1:
        if (situ->mycomm.unk46_1 == 0) {
            if (func_02039cb4(func_020399a8(), 0xc, 0x14) == TRUE) {
                func_0203d1e8(func_020399a8(), 1);
                (*seq)++;
            }
        } else {
            (*seq)++;
        }
        break;
    case 2:
        (*seq)++;
        break;
    case 4:
        break;
    case 3: {
        u32 result = func_ov021_021cd688(colosseum->unk168);

        if (result != 0) {
            colosseum->unk16C[2] = func_ov021_021cd5a4(colosseum->unk168);
            colosseum->unk168 = NULL;
            switch (result) {
            case 1:
                *seq = 0x64;
                func_0203d7bc(TRUE);
                break;
            case 2:
                *seq = 0xc8;
                break;
            case 3:
                *seq = 0xc8;
                break;
            case 4:
                *seq = 0xc8;
                break;
            default:
                GFL_ASSERT(0);
                break;
            }
        }
        break;
    }
    case 0x64:
        colosseum->unkDE = 1;
        func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_SEND);
        return TRUE;
    case 0xc8:
        func_ov019_0217f210(unisys, field, 0x14c);
        colosseum->unkDE = 2;
        (*seq)++;
        break;
    case 0xc9:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_END);
            func_ov019_0217baf0(unisys, field, TRUE);
            return TRUE;
        }
        break;
    case 0x12c:
        colosseum->unkDE = 2;
        func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_END);
        func_ov019_0217baf0(unisys, field, TRUE);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionOneself_AddColosseumMember
static void func_ov019_0217dc64(UnionSystem *unisys, PlayerInfo *info, const u8 *mac) {
    func_ov012_0217357c(&unisys->situ.mycomm, mac, func_02008554(info), getTrainerGender(info));
}

// pokebw2's UnionOneself_RemoveColosseumMember
static void func_ov019_0217dc94(UnionSystem *unisys, PlayerInfo *info, const u8 *mac) {
    func_ov012_02173614(&unisys->situ.mycomm, mac);
}

BOOL func_ov019_0217dca8(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    UnionColosseum *colosseum = unisys->colosseum_sys;
    u8 netId = func_0203d1a4(func_020399a8());

    GFL_ASSERT(colosseum != NULL);
    if (func_ov019_0217bb20(unisys) == TRUE) {
        func_ov019_0217f074(unisys);
        return TRUE;
    }
    switch (*seq) {
    case 0:
        func_02039c68(func_020399a8(), 0xd, 0x14);
        (*seq)++;
        break;
    case 1:
        if (func_02039cb4(func_020399a8(), 0xd, 0x14) == TRUE) {
            (*seq)++;
        }
        break;
    case 2:
        func_ov012_02174b40(unisys);
        func_02039c68(func_020399a8(), 0xe, 0x14);
        (*seq)++;
        break;
    case 3:
        if (func_02039cb4(func_020399a8(), 0xe, 0x14) == TRUE) {
            (*seq)++;
        }
        break;
    case 4:
        if (func_ov012_02174bb8(&colosseum->members[netId], FALSE) == TRUE) {
            (*seq)++;
        }
        break;
    case 5:
        if (func_ov019_0217fdc8(colosseum) == TRUE) {
            int count = func_0203d1b4();
            int i;

            for (i = 0; i < count; i++) {
                if (colosseum->members[i].unk26 == 1) {
                    func_ov012_0217357c(&situ->mycomm, colosseum->members[i].mac,
                                        func_02008554(&colosseum->members[i].info),
                                        getTrainerGender(&colosseum->members[i].info));
                }
            }
            (*seq)++;
        }
        break;
    case 6:
        func_02039c68(func_020399a8(), 0xf, 0x14);
        (*seq)++;
        break;
    case 7:
        if (func_02039cb4(func_020399a8(), 0xf, 0x14) == TRUE) {
            (*seq)++;
        }
        break;
    case 8:
        if (func_ov012_02174c34(colosseum->card[netId]) == TRUE) {
            (*seq)++;
        }
        break;
    case 9:
        if (func_ov019_0217fdf8(colosseum) == TRUE) {
            (*seq)++;
        }
        break;
    case 10: {
        RivalDataSave *rival = getHollow_RivalData(GameData_GetSaveControl(GSYS_GetGameData(unisys->param->gsys)));
        int count = func_0203d1b4();
        int i;

        for (i = 0; i < count; i++) {
            if (colosseum->members[i].unk26 == 1 && i != func_0203d1a4(func_020399a8())) {
                func_0200ed54(rival, getIDAsUInt(&colosseum->members[i].info));
            }
        }
        func_ov012_02175094(colosseum, TRUE);
        func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_IDLE);
        return TRUE;
    }
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepIdleColosseumBegin
static BOOL func_ov019_0217de88(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    func_ov019_0217f074(unisys);
    func_ov019_0217baf0(unisys, field, FALSE);
    return TRUE;
}

// pokebw2's UnionOneself_StepIdleColosseumRun
static BOOL func_ov019_0217dea0(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    UnionColosseum *clsys = unisys->colosseum_sys;
    FieldPlayer *player = Field_GetPlayer(field);
    s16 z, y, x;
    u32 dir = 0;
    u32 out;
    u32 pos;

    if (func_ov019_0217bb20(unisys) == TRUE) {
        func_ov019_0217f074(unisys);
        return TRUE;
    }
    if (*seq == 0) {
        if (func_ov019_0217fe44(field, func_ov019_0217f060(situ->mycomm.unk19), &pos) == TRUE) {
            func_ov012_021750b0(clsys, pos);
            func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_SIT);
            func_ov019_0217baf0(unisys, field, TRUE);
            return TRUE;
        }
        if (func_ov019_0217fef4(clsys) == TRUE) {
            func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_END);
            func_ov019_0217baf0(unisys, field, TRUE);
            return TRUE;
        }
        if (func_ov019_0217ff14(field) == TRUE) {
            func_ov019_0217b8e4(unisys, UNION_STEP_QUIT);
            func_ov019_0217baf0(unisys, field, TRUE);
            return TRUE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            u32 dir = 0;
            s16 x, y, z;

            FieldPlayer_GetGPosInFront(player, &x, &y, &z);
            if (func_ov010_021679a4(clsys->commPlayer, x, y, z, &out, &dir) == TRUE) {
                clsys->unk16C[0] = out;
                func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_CARD);
                func_ov019_0217baf0(unisys, field, TRUE);
                return TRUE;
            }
        }
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepColosseumSeatRun
static BOOL func_ov019_0217dfb4(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    UnionColosseum *clsys = unisys->colosseum_sys;
    FieldActor *actor;
    u32 pos;

    if (func_ov019_0217bb20(unisys) == TRUE) {
        func_ov019_0217f074(unisys);
        return TRUE;
    }
    switch (*seq) {
    case 0:
        (*seq)++;
        break;
    case 1:
        if (func_ov012_02174ce4(clsys) == TRUE) {
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov019_0217f1ec(unisys)) {
            u8 result = func_ov012_021750d0(clsys);

            if (result == 1) {
                (*seq)++;
            } else if (result == 0) {
                func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_STAND);
                return TRUE;
            }
        }
        break;
    case 3:
        actor = FieldPlayer_GetActor(Field_GetPlayer(field));
        if (IsAllActorAcmdFinished(actor) == TRUE) {
            func_ov019_0217fe44(field, func_ov019_0217f060(situ->mycomm.unk19), &pos);
            FldAct_SetAcmd(actor, (pos & 1) ? 2 : 3);
            (*seq)++;
        }
        break;
    case 4:
        actor = FieldPlayer_GetActor(Field_GetPlayer(field));
        if (func_ov010_0216d454(actor) == TRUE) {
            func_ov010_0216d480(actor);
            func_ov019_0217b8e4(unisys, UNION_STEP_PARTY_SELECT);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepColosseumLeaveRun
static BOOL func_ov019_0217e0a4(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    UnionColosseum *clsys = unisys->colosseum_sys;
    FieldActor *actor = FieldPlayer_GetActor(Field_GetPlayer(field));
    u32 pos;

    if (func_ov019_0217bb20(unisys) == TRUE) {
        func_ov019_0217f074(unisys);
        return TRUE;
    }
    switch (*seq) {
    case 0:
        func_ov012_021750b0(clsys, 0xff);
        (*seq)++;
        break;
    case 1:
        if (func_ov012_02174ce4(clsys) == TRUE) {
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov012_021750d0(clsys) != 0xff) {
            (*seq)++;
        }
        break;
    case 3:
        func_ov019_0217f074(unisys);
        (*seq)++;
        break;
    case 4:
        if (IsAllActorAcmdFinished(actor) == TRUE) {
            func_ov019_0217fe44(field, func_ov019_0217f060(situ->mycomm.unk19), &pos);
            FldAct_SetAcmd(actor, (pos & 1) ? 0xb : 0xa);
            (*seq)++;
        }
        break;
    case 5:
        if (func_ov010_0216d454(actor) == TRUE) {
            func_ov010_0216d480(actor);
            func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_IDLE);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepPartySelectRun
static BOOL func_ov019_0217e188(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    UnionColosseum *clsys = unisys->colosseum_sys;
    u32 partyResult;
    u32 boxResult;
    u32 selected;
    u32 result;
    u32 kind;
    u8 partyOk;
    u8 boxOk;

    if (!func_ov019_0217f1ec(unisys)) {
        return FALSE;
    }
    switch (*seq) {
    case 0:
        situ->result = 0;
        (*seq)++;
        // fallthrough
    case 1:
        if (func_0200bba8(getBattleBox(GameData_GetSaveControl(unisys->param->gameData))) == TRUE) {
            (*seq)++;
        } else {
            clsys->unk16C[1] = 0;
            situ->result++;
            *seq = 4;
        }
        break;
    case 2:
        func_ov019_0217f074(unisys);
        func_ov019_0217ba48(unisys, &partyResult);
        func_ov019_0217ba6c(unisys, &boxResult);
        partyOk = TRUE;
        if (partyResult == 0) {
            partyOk = FALSE;
        }
        boxOk = TRUE;
        if (boxResult == 0) {
            boxOk = FALSE;
        }
        unisys->alloc.boxParty = func_ov019_0217b9cc(unisys);
        unisys->alloc.partySelect = func_ov021_021cc720(field, GameData_GetParty(unisys->param->gameData),
                                                        unisys->alloc.boxParty, partyOk, boxOk, 0x15);
        (*seq)++;
        break;
    case 3:
        if (func_ov021_021cc810(unisys->alloc.partySelect) == TRUE) {
            result = func_ov021_021cc7b4(unisys->alloc.partySelect, &selected);
            unisys->alloc.partySelect = NULL;
            if (result == 2) {
                func_ov019_0217b9f4(unisys->alloc.boxParty);
                unisys->alloc.boxParty = NULL;
                func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_STAND);
                return TRUE;
            }
            switch (result) {
            case 0:
                clsys->unk16C[1] = 0;
                break;
            case 1:
                clsys->unk16C[1] = 1;
                break;
            }
            func_ov019_0217b9f4(unisys->alloc.boxParty);
            unisys->alloc.boxParty = NULL;
            (*seq)++;
        }
        break;
    case 4: {
        u32 count = 0;

        if (clsys->unk16C[1] == 0) {
            func_ov019_0217ba48(unisys, &count);
            kind = 1;
        } else {
            func_ov019_0217ba6c(unisys, &count);
            kind = 2;
        }
        if (count != 0) {
            func_ov019_0217f098(unisys, field);
            func_ov019_0217f640(unisys, field, count, func_ov019_0217b974(situ->activity), kind);
            *seq = 5;
        } else {
            func_ov019_0217b8e4(unisys, UNION_STEP_PARTY_WAIT);
            return TRUE;
        }
        break;
    }
    case 5:
        if (func_ov019_0217f794(unisys, field) == TRUE) {
            func_ov019_0217f210(unisys, field, 0x115);
            (*seq)++;
        }
        break;
    case 6:
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            func_ov019_0217f7b8(unisys);
            if (situ->result == 0) {
                *seq = 0;
            } else {
                func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_STAND);
                return TRUE;
            }
        }
        break;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepPartyWaitRun
static BOOL func_ov019_0217e378(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    UnionColosseum *clsys = unisys->colosseum_sys;

    if (func_ov019_0217bb20(unisys) == TRUE) {
        func_ov019_0217f074(unisys);
        return TRUE;
    }
    if (clsys->unkCB == 1) {
        func_ov019_0217b8e4(unisys, UNION_STEP_PARTY_SEND);
        clsys->unkCB = 0;
        clsys->unkDF = 0;
        return TRUE;
    }
    switch (*seq) {
    case 0:
        func_ov019_0217f210(unisys, field, 0x116);
        (*seq)++;
        break;
    case 1:
        if (func_ov012_02174e20() == TRUE) {
            clsys->unkDF = 1;
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov019_0217f1ec(unisys) == TRUE && (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B)) {
            *seq = 0x64;
        }
        break;
    case 0x64:
        if (func_ov012_02174e64() == TRUE) {
            (*seq)++;
        }
        break;
    case 0x65:
        if (clsys->unkDF == 0) {
            func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_STAND);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepPartySendRun
static BOOL func_ov019_0217e440(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    UnionColosseum *clsys = unisys->colosseum_sys;
    u8 netId = func_0203d1a4(func_020399a8());

    if (func_ov019_0217bb20(unisys) == TRUE) {
        func_ov019_0217f074(unisys);
        return TRUE;
    }
    switch (*seq) {
    case 0:
        switch (clsys->unk16C[1]) {
        case 0:
        default:
            PokeParty_Copy(GameData_GetParty(unisys->param->gameData), clsys->party[netId]);
            break;
        case 1: {
            PokeParty *boxParty = func_ov019_0217b9cc(unisys);

            PokeParty_Copy(boxParty, clsys->party[netId]);
            func_ov019_0217b9f4(boxParty);
            break;
        }
        }
        func_0201a2a8(unisys->alloc.regulation, clsys->party[netId]);
        (*seq)++;
        break;
    case 1:
        func_ov012_02175240(clsys, TRUE);
        func_ov019_0217fe38(clsys);
        func_ov012_02175208(clsys);
        func_02039c68(func_020399a8(), 0x14, 0x14);
        (*seq)++;
        break;
    case 2:
        if (func_02039cb4(func_020399a8(), 0x14, 0x14) == TRUE) {
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov012_02174d98(clsys->party[netId]) == TRUE) {
            (*seq)++;
        }
        break;
    case 4:
        if (func_ov012_02175214(clsys) == TRUE) {
            (*seq)++;
        }
        break;
    case 5:
        func_02039c68(func_020399a8(), 0x10, 0x14);
        (*seq)++;
        break;
    case 6:
        if (func_02039cb4(func_020399a8(), 0x10, 0x14) == TRUE) {
            (*seq)++;
        }
        break;
    case 7:
        if (func_0203d378() == TRUE) {
            if (func_ov012_02174df0(clsys->unkCC) == TRUE) {
                (*seq)++;
            }
        } else {
            (*seq)++;
        }
        break;
    case 8:
        if (func_ov019_0217fe24(clsys) == TRUE) {
            func_ov019_0217b8e4(unisys, UNION_STEP_BATTLE_LIST);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov019_0217e5a0(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    UnionColosseum *colosseum = unisys->colosseum_sys;
    UnionColosseumParam *param;
    int netId = func_0203d1a4(func_020399a8());

    if (*seq < 3 && func_ov019_0217bb20(unisys) == TRUE) {
        func_ov019_0217f074(unisys);
        return TRUE;
    }
    switch (*seq) {
    case 0:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            u32 msgId;

            switch (situ->mycomm.unk19) {
            default:
                msgId = 0x117;
                break;
            case 6:
            case 7:
            case 22:
            case 23:
                msgId = 0x118;
                break;
            case 20:
            case 21:
                msgId = 0x11b;
                break;
            case 10:
            case 11:
            case 18:
            case 19:
                msgId = 0x119;
                break;
            case 14:
            case 15:
                msgId = 0x11a;
                break;
            }
            func_ov019_0217f210(unisys, field, msgId);
            (*seq)++;
        }
        break;
    case 1:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            func_02039c68(func_020399a8(), 0x11, 0x14);
            (*seq)++;
        }
        break;
    case 2:
        if (func_02039cb4(func_020399a8(), 0x11, 0x14) == TRUE) {
            func_ov019_0217f074(unisys);
            (*seq)++;
        }
        break;
    case 3: {
        int i;
        int count;

        param = GFL_HeapAllocate(0x41, sizeof(UnionColosseumParam), TRUE, "union_oneself.c", 0x10c4);

        param->list.party = colosseum->party[netId];
        param->list.bag = GameData_GetBag(unisys->param->gameData);
        param->list.trainerData = getTrainerDataBlkAddress(GameData_GetSaveControl(unisys->param->gameData));
        param->list.mode = 0x1a;
        param->list.regulation = unisys->alloc.regulation;
        param->battle = GFL_HeapAllocate(0x41, sizeof(ColosseumBattleParam), TRUE, "union_oneself.c", 0x10ce);
        count = 0;
        for (i = 0; i < func_0203d1b4(); i++) {
            if (i != netId) {
                int slot;

                if ((colosseum->unk15C[netId] & 1) == (colosseum->unk15C[i] & 1)) {
                    slot = 0;
                } else {
                    slot = ++count;
                }
                param->list.partners[slot].party = colosseum->party[i];
                param->list.partners[slot].name = GetPlayerName(&colosseum->members[i].info);
                param->list.partners[slot].gender = getTrainerGender(&colosseum->members[i].info);
            }
        }
        switch (situ->mycomm.unk19) {
        case 20:
        case 21:
        case 22:
        case 23:
            param->list.partnerCount = 2;
            break;
        default:
            param->list.partnerCount = 1;
            break;
        }
        param->list.showPartners = TRUE;
        param->list.timerEnabled = FALSE;
        param->list.timeLeft = 0;
        unisys->cmdArg = param;
        func_ov012_0217422c(unisys, 0x1a, param);
        (*seq)++;
        break;
    }
    case 4:
        if (func_ov012_02174268(unisys) != TRUE) {
            param = unisys->cmdArg;
            GFL_HeapFree(param->battle);
            GFL_HeapFree(unisys->cmdArg);
            unisys->cmdArg = NULL;
            func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_STAND);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's UnionOneself_StepQuitRun
static BOOL func_ov019_0217e838(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    FieldActor *actor = FieldPlayer_GetActor(Field_GetPlayer(field));
    BOOL yes;

    if (func_ov019_0217bb20(unisys) == TRUE) {
        func_ov019_0217f074(unisys);
        return TRUE;
    }
    switch (*seq) {
    case 0:
        func_ov019_0217f210(unisys, field, 0x14e);
        (*seq)++;
        break;
    case 1:
        if (func_ov019_0217f1ec(unisys)) {
            func_ov019_0217f22c(unisys, field);
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov019_0217f268(unisys, &yes) == TRUE) {
            func_ov019_0217f24c(unisys);
            if (yes == 0) {
                *seq = 3;
            } else {
                func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_END);
                return TRUE;
            }
        }
        break;
    case 3:
        if (IsAllActorAcmdFinished(actor) == TRUE) {
            FldAct_SetAcmd(actor, 8);
            (*seq)++;
        }
        break;
    case 4:
        if (func_ov010_0216d454(actor) == TRUE) {
            func_ov010_0216d480(actor);
            func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_IDLE);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov019_0217e904(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    UnionColosseum *colosseum = unisys->colosseum_sys;
    int i;

    if (*seq < 7 && func_0200fb24() && func_ov019_0217f1ec(unisys) == TRUE) {
        func_ov019_0217baf0(unisys, field, TRUE);
        *seq = 7;
    }
    switch (*seq) {
    case 0:
        func_ov019_0217baf0(unisys, field, TRUE);
        if (colosseum->unkDE == 2) {
            if (func_ov019_0217f1ec(unisys) == TRUE) {
                *seq = 6;
            }
        } else {
            func_ov019_0217f210(unisys, field, 0x14c);
            (*seq)++;
        }
        break;
    case 1:
        if (func_ov012_02174f34() == TRUE) {
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov019_0217feb8(colosseum) == TRUE) {
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov019_0217f1ec(unisys) == TRUE) {
            (*seq)++;
        }
        break;
    case 4:
        func_02039c68(func_020399a8(), 0x13, 0x14);
        (*seq)++;
        break;
    case 5:
        if (func_02039cb4(func_020399a8(), 0x13, 0x14) == TRUE) {
            func_0203d7bc(FALSE);
            (*seq)++;
        }
        break;
    case 6:
        if (func_0203d378() == TRUE) {
            u8 mask = colosseum->unk16C[2];

            if (func_020392b8() <= 1) {
                (*seq)++;
            } else {
                for (i = 0; i < 5; i++) {
                    if (i != func_02039338() && (mask & (1 << i)) && func_0203d1c0((u8)i) == TRUE) {
                        break;
                    }
                }
                if (i == 5) {
                    (*seq)++;
                }
            }
        } else {
            (*seq)++;
        }
        break;
    case 7:
        if (colosseum->unkC8_1 == 1 && func_ov012_02172d68(unisys) == TRUE) {
            return FALSE;
        }
        if (func_0200fb24()) {
            func_ov019_0217f074(unisys);
            func_02012124(unisys->param->gsys, 1);
            return FALSE;
        }
        func_ov012_02172d38(unisys);
        (*seq)++;
        break;
    case 8:
        if (!func_ov012_02172d68(unisys)) {
            (*seq)++;
        }
        break;
    case 9:
        colosseum->unkC9 = 1;
        func_ov012_02173408(unisys);
        func_ov019_0217ae20(unisys);
        func_ov012_02173524(unisys, &situ->mycomm);
        func_ov012_0217422c(unisys, 0x19, NULL);
        (*seq)++;
        break;
    case 10:
        if (!func_ov012_02174268(unisys)) {
            if (unisys->colosseum_sys != NULL) {
                func_ov012_0217505c(unisys->colosseum_sys, unisys->param->gsys);
                unisys->colosseum_sys = NULL;
            }
            func_ov012_02172d50(unisys);
            (*seq)++;
        }
        break;
    case 11:
        if (!func_ov012_02172d68(unisys)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov019_0217ead8(UnionSystem *unisys, UnionSelf *situ, Field *field, u8 *seq) {
    UnionColosseum *colosseum = unisys->colosseum_sys;

    switch (*seq) {
    case 0: {
        u32 msgId;
        u32 version;

        func_ov019_0217baf0(unisys, field, TRUE);
        version = func_0200855c(&colosseum->members[colosseum->unk16C[0]].info);
        if (version - VERSION_WHITE <= 1) {
            int stars = colosseum->card[colosseum->unk16C[0]]->stars;

            if (stars < 5) {
                msgId = 0x156 + stars;
            } else if (stars == 5) {
                switch (version) {
                case VERSION_BLACK:
                    msgId = 0x15c;
                    break;
                case VERSION_WHITE:
                    msgId = 0x15b;
                    break;
                default:
                    msgId = 0x15d;
                    break;
                }
            } else {
                msgId = 0x15d;
            }
        } else {
            msgId = 0x15d;
        }
        func_ov019_0217f098(unisys, field);
        copyVarForText(unisys->wordset, 0, &colosseum->members[colosseum->unk16C[0]].info);
        func_ov019_0217f188(unisys, msgId);
        (*seq)++;
        break;
    }
    case 1:
        if (func_ov019_0217f1ec(unisys) == TRUE && (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            func_ov019_0217f074(unisys);
            colosseum->card[colosseum->unk16C[0]]->unk04_5 = TRUE;
            unisys->cmdArg = func_ov010_0216f7f4(unisys->param->gameData, colosseum->card[colosseum->unk16C[0]], 0x41);
            func_ov012_0217422c(unisys, 0x1c, unisys->cmdArg);
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov012_02174268(unisys)) {
            unisys->cmdArg = NULL;
            (*seq)++;
        }
        break;
    case 3:
        func_ov019_0217b8e4(unisys, UNION_STEP_COLOSSEUM_IDLE);
        return TRUE;
    }
    return FALSE;
}

