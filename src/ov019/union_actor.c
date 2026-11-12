// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov034/union_oneself.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The actors of the Union Room's players heard: for every member of a beacon's entry an actor on the field, made when
// the member is new, run by its step (a row of begin, run and end functions) and freed when the entry times out.
// pokebw2 has them in union_oneself.c (and now in a union_actor.c of its own); in Black they are an object of their own,
// before union_oneself.c, whose "" assert string its .data holds a second time. The file's name is pokebw2's guess
#include "field/union_actor.h"
#include "field/union_oneself.h"
#include "types.h"
#include "constants/zones.h"
#include "field/field_actor.h"
#include "field/field_internal.h"
#include "field/union_comm.h"
#include "field/union_eff.h"
#include "field/union_msg.h"
#include "gfl/std.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/union_view.h"
#include "nitro/os.h"

#define UNION_ACTOR_ID_MEMBER_BASE 0x20

// A step of an actor
typedef struct {
    UnionActorFunc begin;
    UnionActorFunc run;
    UnionActorFunc end;
} UnionActorStep;

typedef struct {
    s16 x;
    s16 z;
} UnionActorOffset;

static BOOL func_ov019_0217ae38(UnionActorSlot *slot, u8 step);
static FieldActor *func_ov019_0217ae40(UnionSystem *unisys, GameData *gameData, u8 look, u16 id);
static u16 func_ov019_0217aed4(UnionBeaconEntry *entry, UnionActorSlot *slot);
static FieldActor *func_ov019_0217afb0(UnionSystem *unisys, UnionBeaconEntry *entry, UnionActorSlot *slot);
static UnionActorSlot *func_ov019_0217afe4(UnionSystem *unisys, UnionBeaconEntry *entry, UnionMember *member, int memberIndex, BOOL visible, FieldActor **outActor);
static void func_ov019_0217b054(UnionSystem *unisys, UnionBeaconEntry *entry, UnionActorSlot *slot);
static void func_ov019_0217b098(UnionSystem *unisys, RivalDataSave *rival, WifiList *wifiList, void *effWork, UnionBeaconEntry *entry);
static BOOL func_ov019_0217b158(UnionSystem *unisys, UnionBeaconEntry *entry, const u8 *mac);
static BOOL func_ov019_0217b1d0(UnionSystem *unisys, UnionBeaconEntry *entry);
static BOOL func_ov019_0217b370(UnionSystem *unisys, UnionActorSlot *slot, Field *field);
static BOOL func_ov019_0217b3ec(UnionSystem *unisys, UnionBeaconEntry *entry);
static BOOL func_ov019_0217b4c4(UnionSystem *unisys, UnionActorSlot *slot, Field *field, u8 *seq);
static BOOL func_ov019_0217b66c(UnionSystem *unisys, UnionActorSlot *slot, Field *field, u8 *seq);
static BOOL func_ov019_0217b6b4(UnionSystem *unisys, UnionActorSlot *slot, Field *field, u8 *seq);

// pokebw2's sUnionMemberOffsets
static const UnionActorOffset data_ov019_0217ffe4[UNION_MEMBER_MAX] = {
#include "asset/union_actor_data_ov019_0217ffe4.inc"
};

// pokebw2's sUnionActorNpc
static const ZoneNPC data_ov019_0217fff8 = {
    0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, { { 0, 0, 0 } },
};

// pokebw2's sUnionEntryPositions
static const UnionActorOffset data_ov019_0218001c[UNION_BEACON_ENTRY_MAX] = {
#include "asset/union_actor_data_ov019_0218001c.inc"
};

// pokebw2's sUnionActorSteps
static const UnionActorStep data_ov019_02180044[4] = {
    { NULL, func_ov019_0217b4c4, NULL },
    { NULL, func_ov019_0217b5e0, NULL },
    { NULL, func_ov019_0217b66c, NULL },
    { NULL, func_ov019_0217b6b4, NULL },
};

// pokebw2's UnionOneself_ClearActors
void func_ov019_0217ae20(UnionSystem *unisys) {
    MI_CpuFill8(unisys->actors, 0, sizeof(unisys->actors));
}

// pokebw2's UnionOneself_SetActorNextStep
static BOOL func_ov019_0217ae38(UnionActorSlot *slot, u8 step) {
    slot->nextStep = step;
    return TRUE;
}

// pokebw2's UnionOneself_CreateActor
static FieldActor *func_ov019_0217ae40(UnionSystem *unisys, GameData *gameData, u8 look, u16 id) {
    MMSys *mmsys = GameData_GetMMSys(gameData);
    u16 entryIndex = id >> 8;
    u16 member = (u8)id - UNION_ACTOR_ID_MEMBER_BASE;
    ZoneNPC npc = data_ov019_0217fff8;

    npc.uid = id;
    npc.modelId = func_02025c74(look);
    if (member != 0) {
        npc.moveCode = 0xf;
    }
    func_ov010_0216e618(&npc, data_ov019_0218001c[entryIndex].x + data_ov019_0217ffe4[member].x,
                        data_ov019_0218001c[entryIndex].z + data_ov019_0217ffe4[member].z, 0);
    return CreateNewActorByEntityNoWKOBJCODE(mmsys, &npc, ZONE_UNION_ROOM);
}

// pokebw2's UnionOneself_GetActorId
static u16 func_ov019_0217aed4(UnionBeaconEntry *entry, UnionActorSlot *slot) {
    return (entry->index << 8) | (slot->member + UNION_ACTOR_ID_MEMBER_BASE);
}

// pokebw2's UnionOneself_IsMemberId
BOOL func_ov019_0217aee8(u16 id) {
    u16 member = (id & 0xff) - UNION_ACTOR_ID_MEMBER_BASE;

    switch (member) {
    case 0:
        return FALSE;
    default:
        return TRUE;
    }
}

// pokebw2's UnionOneself_GetEntryIndex
u16 func_ov019_0217aefc(u16 id) {
    return id >> 8;
}

// pokebw2's UnionOneself_GetActorGender
u8 func_ov019_0217af04(UnionSystem *unisys, u16 id) {
    int i;
    UnionActorSlot *slot = unisys->actors;

    for (i = 0; i < UNION_ACTOR_MAX; i++, slot++) {
        if (slot->used == 1 && slot->entry != NULL && id == func_ov019_0217aed4(slot->entry, slot)) {
            return slot->gender;
        }
    }
    return 0;
}

// pokebw2's UnionActor_IsId
BOOL func_ov019_0217af38(u16 id) {
    u16 low = id & 0xff;

    if (low < UNION_ACTOR_ID_MEMBER_BASE) {
        return FALSE;
    }
    return TRUE;
}

// pokebw2's UnionOneself_CanTalkTo
BOOL func_ov019_0217af4c(UnionSystem *unisys, u16 id) {
    UnionActorSlot *slot = unisys->actors;
    int i;

    for (i = 0; i < UNION_ACTOR_MAX; i++, slot++) {
        if (slot->used == 1 && slot->entry != NULL && slot->entry->timeout != 0
            && id == (slot->entry->index << 8 | (slot->member + UNION_ACTOR_ID_MEMBER_BASE))) {
            if (slot->step == 2 || slot->step == 1 || slot->nextStep == 1 || slot->nextStep == 2) {
                return FALSE;
            }
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's UnionOneself_FindActor
static FieldActor *func_ov019_0217afb0(UnionSystem *unisys, UnionBeaconEntry *entry, UnionActorSlot *slot) {
    u16 id = func_ov019_0217aed4(entry, slot);
    FieldActor *mmdl = FindFieldActor(GameData_GetMMSys(unisys->param->gameData), id);

    GFL_ASSERT(mmdl != NULL);
    return mmdl;
}

static UnionActorSlot *func_ov019_0217afe4(UnionSystem *unisys, UnionBeaconEntry *entry, UnionMember *member,
                                                    int memberIndex, BOOL visible, FieldActor **outActor) {
    UnionActorSlot *slot = unisys->actors;
    FieldActor *actor;
    int i;

    for (i = 0; i < UNION_ACTOR_MAX; i++, slot++) {
        if (slot->used == 0) {
            slot->entry = entry;
            slot->look = member->look;
            slot->gender = member->gender;
            slot->member = memberIndex;
            slot->used = 1;
            actor = func_ov019_0217ae40(unisys, unisys->param->gameData, member->look,
                                             func_ov019_0217aed4(entry, slot));
            if (!visible) {
                SetActorHidden(actor, TRUE);
            }
            *outActor = actor;
            return slot;
        }
    }
    GFL_ASSERT(0);
    return NULL;
}

// pokebw2's UnionOneself_FreeActorSlot
static void func_ov019_0217b054(UnionSystem *unisys, UnionBeaconEntry *entry, UnionActorSlot *slot) {
    Field *field = GSYS_GetField(unisys->param->gsys);
    FieldActor *actor = func_ov019_0217afb0(unisys, entry, slot);

    if (field != NULL && actor != NULL) {
        func_ov019_0217f958(func_ov021_02188ce0(field), actor);
    }
    DeleteActor(actor);
    MI_CpuFill8(slot, 0, sizeof(UnionActorSlot));
}

static void func_ov019_0217b098(UnionSystem *unisys, RivalDataSave *rival, WifiList *wifiList, void *effWork,
                                           UnionBeaconEntry *entry) {
    FieldActor *actor;
    u8 ownMac[6];
    int i;

    if (entry->timeout == 0) {
        return;
    }
    OS_GetMacAddress(ownMac);
    for (i = 0; i < UNION_MEMBER_MAX; i++) {
        if (entry->actor[i] == NULL && entry->beacon.members[i].used == 1
            && GFL_STD_MemCmp(ownMac, entry->beacon.members[i].mac, 6) != 0
            && func_ov019_0217b158(unisys, entry, entry->beacon.members[i].mac)) {
            entry->actor[i] = func_ov019_0217afe4(unisys, entry, &entry->beacon.members[i], i, FALSE, &actor);
            entry->actor[i]->step = 1;
            func_ov019_0217ae38(entry->actor[i], 0);
            if (i == 0) {
                if (func_ov019_0217f894(wifiList, &entry->beacon) == TRUE) {
                    func_ov019_0217f948(effWork, actor, 0);
                } else if (func_0200edc4(rival, entry->beacon.trainerId) == TRUE) {
                    func_ov019_0217f948(effWork, actor, 1);
                }
            }
        }
    }
}

// pokebw2's UnionOneself_IsMemberNew
static BOOL func_ov019_0217b158(UnionSystem *unisys, UnionBeaconEntry *entry, const u8 *mac) {
    UnionBeaconEntry *other = unisys->found;
    int i;

    for (i = 0; i < UNION_BEACON_ENTRY_MAX; i++, other++) {
        if (other != entry && other->beacon.valid == 1) {
            int j;

            for (j = 0; j < UNION_MEMBER_MAX; j++) {
                if (other->beacon.members[j].used == 1 && GFL_STD_MemCmp(mac, other->beacon.members[j].mac, 6) == 0) {
                    if (other->timeout != 0) {
                        other->timeout = 1;
                    }
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

// pokebw2's UnionOneself_ShowFocus
static BOOL func_ov019_0217b1d0(UnionSystem *unisys, UnionBeaconEntry *entry) {
    int i;

    for (i = 0; i < UNION_MEMBER_MAX; i++) {
        UnionActorSlot *slot = entry->actor[i];

        if (slot != NULL && entry->beacon.members[i].used == 1
            && GFL_STD_MemCmp(entry->beacon.members[i].mac, unisys->situ.mac78, 6) == 0) {
            if (GSYS_CheckField(unisys->param->gsys) == TRUE) {
                void *effWork = func_ov021_02188ce0(GSYS_GetField(unisys->param->gsys));

                if (effWork != NULL) {
                    u16 id = func_ov019_0217aed4(entry, slot);
                    FieldActor *actor = FindFieldActor(GameData_GetMMSys(unisys->param->gameData), id);

                    if (actor != NULL) {
                        func_ov019_0217f93c(effWork, actor);
                    }
                }
            }
            MI_CpuFill8(unisys->situ.mac78, 0, 6);
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's UnionOneself_UpdateActors
void func_ov019_0217b264(UnionSystem *unisys, GameData *gameData, Field *field) {
    u8 zero[6] = { 0, 0, 0, 0, 0, 0 };
    int i;
    BOOL focus = FALSE;
    RivalDataSave *rival;
    WifiList *wifiList;
    void *effWork;
    MMSys *mmsys;
    UnionBeaconEntry *entry;

    rival = getHollow_RivalData(GameData_GetSaveControl(gameData));
    wifiList = func_0201283c(gameData);
    effWork = func_ov021_02188ce0(field);
    mmsys = GameData_GetMMSys(gameData);
    if (GFL_STD_MemCmp(zero, unisys->situ.mac78, 6) != 0) {
        focus = TRUE;
    }
    entry = unisys->found;
    for (i = 0; i < UNION_BEACON_ENTRY_MAX; i++, entry++) {
        if (entry->beacon.valid == 1) {
            int emptyCount;
            int j;

            func_ov019_0217b3ec(unisys, entry);
            func_ov019_0217b098(unisys, rival, wifiList, effWork, entry);
            if (focus == TRUE && func_ov019_0217b1d0(unisys, entry) == TRUE) {
                focus = FALSE;
            }
            emptyCount = 0;
            for (j = 0; j < UNION_MEMBER_MAX; j++) {
                UnionActorSlot *slot = entry->actor[j];

                if (slot != NULL) {
                    if (func_ov019_0217b370(unisys, slot, field) == FALSE) {
                        func_ov019_0217b054(unisys, entry, slot);
                        entry->actor[j] = NULL;
                        emptyCount++;
                    }
                } else {
                    emptyCount++;
                }
            }
            if (emptyCount >= UNION_MEMBER_MAX && entry->timeout == 0) {
                MI_CpuFill8(entry, 0, sizeof(UnionBeaconEntry));
            }
        }
    }
    MI_CpuFill8(unisys->situ.mac78, 0, 6);
}

// pokebw2's UnionOneself_RunActor
static BOOL func_ov019_0217b370(UnionSystem *unisys, UnionActorSlot *slot, Field *field) {
    UnionActorFunc func;
    BOOL done = TRUE;

    switch (slot->phase) {
    case 0:
        func = data_ov019_02180044[slot->step].begin;
        break;
    case 1:
        func = data_ov019_02180044[slot->step].run;
        break;
    case 2:
        func = data_ov019_02180044[slot->step].end;
        break;
    }
    if (func != NULL) {
        done = func(unisys, slot, field, &slot->seq);
    }
    if (done == TRUE) {
        slot->seq = 0;
        slot->phase++;
        if (slot->phase > 2) {
            if (slot->step == 2) {
                return FALSE;
            }
            slot->phase = 0;
            slot->step = slot->nextStep;
            slot->nextStep = 0;
        }
    }
    return TRUE;
}

// pokebw2's UnionOneself_UpdateTimeout
static BOOL func_ov019_0217b3ec(UnionSystem *unisys, UnionBeaconEntry *entry) {
    UnionSelf *situ = &unisys->situ;

    if (situ->mycomm.target == entry || situ->mycomm.unk4 == entry || situ->mycomm.answer_pc == entry
        || situ->mycomm.parent == entry) {
        if (entry->timeout == 0) {
            entry->timeout = 1;
        }
        return TRUE;
    }
    if (entry->timeout == 0) {
        return FALSE;
    }
    entry->timeout--;
    return TRUE;
}

// pokebw2's UnionOneself_UpdateMarks
void func_ov019_0217b440(UnionSystem *unisys, GameData *gameData, Field *field) {
    RivalDataSave *rival = getHollow_RivalData(GameData_GetSaveControl(gameData));
    WifiList *wifiList = func_0201283c(gameData);
    void *effWork = func_ov021_02188ce0(field);
    UnionBeaconEntry *entry = unisys->found;
    int i;

    for (i = 0; i < UNION_BEACON_ENTRY_MAX; i++, entry++) {
        if (entry->beacon.valid == 1 && entry->actor[0] != NULL) {
            FieldActor *actor = func_ov019_0217afb0(unisys, entry->actor[0]->entry, entry->actor[0]);

            if (actor != NULL) {
                if (func_ov019_0217f894(wifiList, &entry->beacon) == TRUE) {
                    func_ov019_0217f948(effWork, actor, 0);
                } else if (func_0200edc4(rival, entry->beacon.trainerId) == TRUE) {
                    func_ov019_0217f948(effWork, actor, 1);
                }
            }
        }
    }
}

// pokebw2's UnionOneself_ActorIdle
static BOOL func_ov019_0217b4c4(UnionSystem *unisys, UnionActorSlot *slot, Field *field, u8 *seq) {
    UnionBeaconEntry *entry = slot->entry;
    UnionSelf *situ = &unisys->situ;
    u32 beaconAct;

    if (entry->timeout == 0) {
        func_ov019_0217ae38(slot, 2);
        return TRUE;
    }
    if (entry->beacon.members[slot->member].used == 0 || slot->look != entry->beacon.members[slot->member].look) {
        func_ov019_0217ae38(slot, 2);
        return TRUE;
    }
    beaconAct = func_ov019_0217f858(entry->beacon.activity);
    if (situ->mycomm.unk44 == func_ov019_0217aed4(slot->entry, slot)) {
        func_ov019_0217ae38(slot, 3);
        return TRUE;
    }
    if (situ->unk02 != 0 && (situ->unk02 == beaconAct || (beaconAct == 0 && situ->unk02 == entry->beacon.unk07_5))
        && slot->member == 0 && func_ov019_0217f060(entry->beacon.activity) > entry->beacon.memberCount) {
        FieldActor *actor = func_ov019_0217afb0(unisys, slot->entry, slot);

        if (IsAllActorAcmdFinished(actor) == TRUE) {
            FldAct_SetAcmd(actor, 0x31);
        }
    } else if (slot->member == 0 && entry->beacon.memberCount > 1) {
        FieldActor *actor = func_ov019_0217afb0(unisys, slot->entry, slot);

        if (GetActorMoveCode(actor) == 2) {
            ChangeActorMoveCodeSeq(actor, 0xf);
        }
    } else if (slot->member == 0 && entry->beacon.memberCount <= 1) {
        FieldActor *actor = func_ov019_0217afb0(unisys, slot->entry, slot);

        if (GetActorMoveCode(actor) == 0xf) {
            ChangeActorMoveCodeSeq(actor, 2);
        }
    }
    return FALSE;
}

// pokebw2's UnionActor_StepAppear
BOOL func_ov019_0217b5e0(UnionSystem *unisys, UnionActorSlot *slot, Field *field, u8 *seq) {
    GridPos actorPos;
    GridPos playerPos;
    FieldActor *actor = func_ov019_0217afb0(unisys, slot->entry, slot);

    switch (*seq) {
    case 0:
        if (IsAllActorAcmdFinished(actor) == TRUE) {
            FldAct_GetGPos(FieldPlayer_GetActor(Field_GetPlayer(field)), &playerPos);
            FldAct_GetGPos(actor, &actorPos);
            if (actorPos.x != playerPos.x || actorPos.z != playerPos.z) {
                FldAct_SetAcmd(actor, 0x44);
                SetActorHidden(actor, FALSE);
                (*seq)++;
            }
        }
        break;
    case 1:
        if (func_ov010_0216d454(actor) == TRUE) {
            func_ov010_0216d480(actor);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's UnionOneself_ActorLeave
static BOOL func_ov019_0217b66c(UnionSystem *unisys, UnionActorSlot *slot, Field *field, u8 *seq) {
    FieldActor *actor = func_ov019_0217afb0(unisys, slot->entry, slot);

    switch (*seq) {
    case 0:
        if (IsAllActorAcmdFinished(actor) == TRUE) {
            FldAct_SetAcmd(actor, 0x43);
            (*seq)++;
        }
        break;
    case 1:
        if (func_ov010_0216d454(actor) == TRUE) {
            func_ov010_0216d480(actor);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's UnionOneself_ActorTalk
static BOOL func_ov019_0217b6b4(UnionSystem *unisys, UnionActorSlot *slot, Field *field, u8 *seq) {
    UnionSelf *situ = &unisys->situ;
    FieldActor *actor = func_ov019_0217afb0(unisys, slot->entry, slot);

    switch (*seq) {
    case 0:
        if (IsAllActorAcmdFinished(actor) == TRUE) {
            u16 acmd;

            switch (GetActorFaceDir(FieldPlayer_GetActor(Field_GetPlayer(field)))) {
            case 0:
                acmd = 1;
                break;
            case 1:
                acmd = 0;
                break;
            case 2:
                acmd = 3;
                break;
            case 3:
            default:
                acmd = 2;
                break;
            }
            FldAct_SetAcmd(actor, acmd);
            (*seq)++;
        }
        break;
    case 1:
        if (func_ov010_0216d480(actor) == TRUE) {
            SetActorFlag(actor, 8);
            (*seq)++;
        }
        break;
    case 2:
        if (situ->mycomm.unk44 != func_ov019_0217aed4(slot->entry, slot)) {
            ClearActorFlag(actor, 8);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
