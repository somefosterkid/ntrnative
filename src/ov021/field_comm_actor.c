// Black's field_comm_actor.c (pokebw2's name for its twin): ov021 0x02194284-0x021945a4.

#include "types.h"
#include "field/field_comm_actor.h"
#include "field/field_actor.h"

// The comm actor's move code work is its FldCommActLink
WORK_AREA_CHECK(FldCommActLink, FIELD_ACTOR_MOVE_WORK_SIZE);

void func_ov021_02194588(FieldActor *actor, const u16 *dir, const VecFx32 *pos, const u32 *hidden, FldCommActEntry *entry);
u16 func_ov021_021945a0(u16 param_1);
FieldActor *FieldCommActor_Add(FldCommActSys *sys, u16 modelId, const u16 *dir, const VecFx32 *pos, const u32 *hidden, FldCommActEntry *entry);
void FieldCommActor_Reset(FldCommActEntry *entry);

FldCommActSys *FldCommActSys_Create(u8 a0, MMSys *actorSystem, HeapID heapId, int a3) {
    FldCommActSys *sys = GFL_HeapAllocate(heapId, sizeof(FldCommActSys), TRUE, "field_comm_actor.c", 93);

    sys->count = a0;
    sys->unk2 = a3;
    sys->actorSystem = actorSystem;
    sys->entries = GFL_HeapAllocate(heapId, sizeof(FldCommActEntry) * a0, TRUE, "field_comm_actor.c", 99);
    return sys;
}

void func_ov021_021942c8(FldCommActSys *sys) {
    u16 i;
    FldCommActEntry *entry;

    entry = sys->entries;
    for (i = 0; i < sys->count; entry++, i++) {
        if (entry->actor != NULL) {
            FieldCommActor_Reset(entry);
        }
    }
    GFL_HeapFree(sys->entries);
    GFL_HeapFree(sys);
}

void FldCommActSys_CreateActor(FldCommActSys *sys, u32 netId, u16 a2, const u16 *dir, const VecFx32 *pos, const u32 *a5) {
    FldCommActEntry *entry;
    u16 i;

    entry = sys->entries;
    func_ov021_021945a0(*dir);
    for (i = 0; i < sys->count; i++) {
        if (entry->actor == NULL) {
            entry->actor = FieldCommActor_Add(sys, a2, dir, pos, a5, entry);
            entry->netId = netId;
            entry->unk6 = sys->unk2;
            return;
        }
        entry++;
    }
}

void FldCommActSys_DeleteActor(FldCommActSys *sys, u32 netId) {
    FldCommActEntry *entry;
    u16 i;

    entry = sys->entries;
    for (i = 0; i < sys->count; i++) {
        if (entry->actor != NULL && entry->netId == netId) {
            FieldCommActor_Reset(entry);
            return;
        }
        entry++;
    }
}

u32 FldCommActSys_FindActor(FldCommActSys *sys, s16 a1, s32 a2, s16 a3, u32 *a4, u32 *a5) {
    FldCommActEntry *entry;
    u16 count;
    s16 diff;

    count = sys->count;
    if (*a5 < count) {
        entry = &sys->entries[*a5];
        do {
            (*a5)++;
            if (entry->actor != NULL) {
                diff = (s16)(FldAct_GetGPosY(entry->actor) - a2);
                if (diff < 0) {
                    diff = -diff;
                }
                if (diff < 1 && CheckActorVolumeOverGPos(entry->actor, a1, a3, TRUE)) {
                    *a4 = entry->netId;
                    return 1;
                }
                entry++;
            }
        } while (*a5 < count);
    }
    return 0;
}

FieldActor *func_ov021_021943ec(FldCommActSys *sys, u32 netId) {
    u16 i;
    FldCommActEntry *entry;

    entry = sys->entries;
    for (i = 0; i < sys->count; i++) {
        if (entry->actor != NULL && entry->netId == netId) {
            return entry->actor;
        }
        entry++;
    }
    return NULL;
}

void FieldCommActor_Reset(FldCommActEntry *entry) {
    if (entry->actorDeleted == 0) {
        DeleteActor(entry->actor);
    }
    MI_CpuFill8(entry, 0, sizeof(FldCommActEntry));
}

static const ZoneNPC data_ov021_021d3f68 = { 0xfc, 0, 0x44, 0, 0, 0x7d0, 0, 0, 0, 0, 4, 4 };

FieldActor *FieldCommActor_Add(FldCommActSys *sys, u16 modelId, const u16 *dir, const VecFx32 *pos, const u32 *hidden, FldCommActEntry *entry) {
    ZoneNPC npc;
    FieldActor *actor;
    MMSys *mmSys = sys->actorSystem;

    npc = data_ov021_021d3f68;
    npc.modelId = modelId;
    actor = CreateNewActorByEntityNoWKOBJCODE(mmSys, &npc, 0);
    func_ov021_02194588(actor, dir, pos, hidden, entry);
    SetActorWPosAll(actor, pos, func_ov021_021945a0(*dir));
    func_ov010_0216db4c(actor, TRUE);
    func_ov010_0216dc98(actor, TRUE);
    SetActorFlag32(actor, TRUE);
    func_ov010_0216dae0(actor, FALSE);
    func_ov010_0216dc64(actor, TRUE);
    func_ov010_0216dccc(actor, TRUE);
    return actor;
}

void func_ov021_021944bc(FieldActor *actor) {
    ClearActorPositionBlock(actor, sizeof(FldCommActLink));
}

void func_ov021_021944c8(FieldActor *actor) {
    FldCommActLink *link = (FldCommActLink *)GetNPCRailPosPtrAddr(actor);

    link->entry->actorDeleted = 1;
}

void func_ov021_021944d8(FieldActor *actor) {
    FldCommActLink *work;
    VecFx32 pos;
    u32 value;
    s32 dist;
    s32 d;

    work = (FldCommActLink *)GetNPCRailPosPtrAddr(actor);
    CheckSetActorFaceDir(actor, func_ov021_021945a0(*work->dir));
    value = 0;
    CopyActorWPos(actor, &pos);
    if (pos.x != work->pos->x || pos.y != work->pos->y || pos.z != work->pos->z) {
        SetActorWPosAll(actor, work->pos, GetActorFaceDir(actor));
        value = 3;
        if (work->entry->unk6 != 0) {
            u16 code = FldAct_GetObjCode(actor);
            if (code == 1 || code == 4) {
                dist = 0;
                d = pos.x - work->pos->x;
                if (d < 0) {
                    d = -d;
                }
                dist += d;
                d = pos.z - work->pos->z;
                if (d < 0) {
                    d = -d;
                }
                dist += d;
                if (dist >= 0x4000) {
                    value = 10;
                }
            }
        }
    }
    func_ov010_0216d674(actor, value);
    if (work->hidden != NULL) {
        BOOL hidden = FALSE;
        if (*work->hidden == 1) {
            hidden = TRUE;
        }
        SetActorHidden(actor, hidden);
    }
}

void func_ov021_02194588(FieldActor *actor, const u16 *dir, const VecFx32 *pos, const u32 *hidden, FldCommActEntry *entry) {
    FldCommActLink *link = (FldCommActLink *)GetNPCRailPosPtrAddr(actor);

    link->dir = dir;
    link->pos = pos;
    link->hidden = hidden;
    link->entry = entry;
}

u16 func_ov021_021945a0(u16 param_1) {
    return param_1;
}
