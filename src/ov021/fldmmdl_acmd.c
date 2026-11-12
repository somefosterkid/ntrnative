// Black's fldmmdl_acmd.c, as pokebw2 names it: the actors' action commands (the task that runs an action list and
// the commands' functions) and their tables of functions, which fldmmdl_movedata.c's ACTOR_ACTION_COMMAND_TABLES
// list. ov021 .text 0x02197c4c-0x0219a098, .rodata 0x021d4160-0x021d4d04, .data 0x021ddbd4-0x021ddbf0.
// The command tables are defined at the end in pokebw2's order (the walk tables by speed, then the list's), each
// Black table taking the place of pokebw2's of the same command index: that order lays them out as Black's.

#include "types.h"
#include "field/field_actor.h"
#include "field/fldmmdl_blact.h"
#include "struct_decls.h"
#include "field/field_acmd.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "field/field.h"
#include "field/field_actor_internal.h"
#include "field/field_effect.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "constants/sound.h"
#include "field/event_trainer_eye.h"
#include "gfl/blact.h"
#include "gfl/key.h"
#include "gfl/random.h"
#include "field/ov021_021942c8.h"
#include "field/ov021_021d046c.h"
#include "field/field_map.h"
#include "field/fldmmdl_movecode_pair.h"

static const s32 data_ov021_021d4a44[];
static const s32 data_ov021_021d4a68[];
static const s32 data_ov021_021d4a80[];
static const s32 data_ov021_021d4a9c[];
static const fx32 *data_ov021_021ddbd4[];

void func_ov021_0219c5d0(MMSys *param_1);
void func_ov021_0219d488(MMSys *param_1);

void func_ov021_02197c4c(MMSys *system) {
    void *fieldBlAct;

    fieldBlAct = func_ov010_0216d520(system);
    if (fieldBlAct != (void *)0x0) {
        func_ov021_0219c5d0(system);
    }
    fieldBlAct = FldActSys_GetFieldBlAct(system);
    if (fieldBlAct != (void *)0x0) {
        func_ov021_0219d488(system);
    }
    func_ov010_0216d95c(system, 0);
}

void func_ov021_02197c78(FieldActor *actor) {
    MMSys *system;
    BOOL BVar2;

    system = GetActorMModelSystem(actor);
    if ((CheckMMSysFlag(system, 4) == 0) && (BVar2 = func_ov010_0216daa0(actor), BVar2 != 0)) {
        FldAct_InvokeUpdateCallback(actor);
    }
}

u32 func_ov021_02197c9c(FieldActor *param_1) {
    BOOL BVar1;

    BVar1 = func_ov010_0216db10(param_1);
    if ((BVar1 != 0) && (BVar1 = func_ov010_0216dc4c(param_1), BVar1 == 0)) {
        return 1;
    }
    BVar1 = CheckActorFlag(param_1, 0x10);
    if (BVar1 != 0) {
        return 1;
    }
    return 0;
}

void CopyActorPosAllAdd(FieldActor *actor, VecFx32 *dest) {
    VecFx32 pos;
    VecFx32 d8b0;
    VecFx32 d8d0;
    VecFx32 d8f0;
    VecFx32 offset;

    CopyActorWPos(actor, &pos);
    func_ov010_0216d8b0(actor, &d8b0);
    func_ov010_0216d8d0(actor, &d8d0);
    func_ov010_0216d8f0(actor, &d8f0);
    CopyActorPosOffset(actor, &offset);

    dest->x = pos.x + d8b0.x + d8d0.x + d8f0.x + offset.x;
    dest->y = pos.y + d8b0.y + d8d0.y + d8f0.y + offset.y;
    dest->z = pos.z + d8b0.z + d8d0.z + d8f0.z + offset.z;
}

struct TCB {
    TCBManager *manager;
    TCB *prev;
    TCB *next;
    u32 priority;
    void *data;
    TCBFunc func;
    BOOL addedDuringUpdate;
};
struct TCBManager {
    u32 count;
    u32 used;
    TCB head;
    TCB **ready;
    TCB *tasks;
    BOOL adding;
    TCB *current;
    TCB *next;
};

// One entry of an action list: the command and how many times it runs (pokebw2's)
typedef struct {
    u16 code;
    u16 count;
} FieldAcmdEntry;

// The work of an action list's task (pokebw2's FieldAcmdWork, without Black 2's queue)
typedef struct {
    u32 state;
    BOOL ended;
    s32 repeat;
    FieldActor *actor;
    const FieldAcmdEntry *cursor;
} FieldAcmdWork;

extern u32 (*const ACMD_TCB_FUNCS[])(FieldAcmdWork *work);
void func_ov021_02197dcc(TCB *tcb, void *data);

FieldAcmdTCB *FieldAcmdTCB_Create(FieldActor *actor, const u32 *action) {
    MMSys *sp4;
    MMSys *mmsys;
    TCB *tcb;
    u32 priority;
    FieldAcmdWork *work;

    mmsys = GetActorMModelSystem(actor);
    sp4 = mmsys;
    work = GFL_HeapAllocate((u16) (0x8000 | (FldActSys_GetFieldHeapID(mmsys) & 0x7FFF)), sizeof(FieldAcmdWork), 1, "fldmmdl_acmd.c", 0x115U);
    priority = func_ov010_0216d4f4(GetActorMModelSystem(actor)) - 2;
    tcb = GFL_TCBMgrAddTask(GetMMSysTCBMgr(mmsys), func_ov021_02197dcc, work, priority);
    work->actor = actor;
    work->cursor = (const FieldAcmdEntry *)action;
    return (FieldAcmdTCB *) tcb;
}

// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they
// belong in the library's header
typedef struct {
    fx32 speed;
    fx32 progress;
    u16 heightStep;
    u16 height;
    s8 dir;
    s8 frames;
    u8 state;
    s8 heightTable;
} FieldAcmdJumpWork;
#define SEQ_SE_FLD_10 0x67b


void func_ov021_02197f6c(FieldActor *param_1, u32 param_2);
void func_ov021_02198318(FieldActor *param_1, u32 param_2, u16 param_3, u32 param_4);
void func_ov021_021984c0(FieldActor *param_1, int param_2, s32 param_3, u8 param_4, u16 param_5, s16 param_6, u16 param_7, u32 param_8);
void func_ov021_02198c08(FieldActor *param_1, u16 param_2, u16 param_3, u32 param_4);

// pokebw2's AcmdDirStepsWork, with Black's types
typedef struct {
    u16 dir;
    u16 state;
    s16 steps;
    s16 index;
} AcmdDirStepsWork;

// The commands' works pokebw2 names: what func_ov010_0216d6bc clears in the actor and func_ov010_0216d6d4 returns
typedef struct {
    s32 pos;
    s32 speed;
} AcmdPosSpeedWork;

typedef struct {
    s32 kind;
    FieldEffectTask *task;
} AcmdEffectWork;

WORK_AREA_CHECK(AcmdEffectWork, FIELD_ACTOR_ACMD_WORK_SIZE);

typedef struct {
    s32 frames;
    VecFx32 delta;
} AcmdShiftWork;

typedef struct {
    u16 count;
    u16 limit;
} AcmdWaitWork;

// The work of a move that shifts the model while a sound plays
typedef struct {
    u16 time;
    u8 lastFrame;
} ShiftWork;


u32 FldAct_ExecAcmd(FieldActor *param_1, s32 param_2, s32 param_3);

BOOL FieldAcmdTCB_CheckEnded(FieldAcmdTCB *task) {
    FieldAcmdWork *work = GFL_TCBGetData((TCB *)task);

    return work->ended;
}

void FieldAcmdTCB_Remove(FieldAcmdTCB *task) {
    FieldAcmdWork *work;

    work = GFL_TCBGetData((TCB *)task);
    func_ov010_0216d480(work->actor);
    GFL_HeapFree(work);
    GFL_TCBRemove((TCB *)task);
}

void func_ov021_02197dcc(TCB *tcb, void *data) {
    FieldAcmdWork *work = data;
    u32 result;

    do {
        result = ACMD_TCB_FUNCS[work->state](work);
    } while (result == 1);
}


u32 func_ov021_02197de8(FieldAcmdWork *work) {
    work->repeat = 0;
    work->state = 1;
    return 1;
}

u32 func_ov021_02197df4(FieldAcmdWork *work) {
    BOOL BVar1;

    BVar1 = IsAllActorAcmdFinished(work->actor);
    if (BVar1 == 0) {
        return 0;
    }
    work->state = 2;
    return 1;
}

u32 func_ov021_02197e10(FieldAcmdWork *work) {
    FldAct_SetAcmd(work->actor, work->cursor->code);
    work->state = 3;
    return 0;
}

u32 func_ov021_02197e28(FieldAcmdWork *work) {
    BOOL BVar1;

    BVar1 = func_ov010_0216d454(work->actor);
    if (BVar1 == 0) {
        return 0;
    }
    work->state = 4;
    return 1;
}

u32 func_ov021_02197e44(FieldAcmdWork *work) {
    const FieldAcmdEntry *entry;
    s32 index;

    entry = work->cursor;
    index = work->repeat;
    work->repeat = index + 1;
    if (index + 1 < entry->count) {
        work->state = 1;
        return 1;
    }
    work->cursor = entry + 1;
    if (entry[1].code != 0xfe) {
        work->state = 0;
        return 1;
    }
    work->ended = 1;
    work->state = 5;
    return 0;
}


u32 func_ov021_02197e78(FieldAcmdWork *work) {
    return 0;
}

u32 (*const ACMD_TCB_FUNCS[])(FieldAcmdWork *work) = { func_ov021_02197de8, func_ov021_02197df4, func_ov021_02197e10, func_ov021_02197e28, func_ov021_02197e44, func_ov021_02197e78 };

u16 GetAcmdForDir(u16 dir, u16 acmd) {
    s32 i;
    const u32 *const *table;
    const u32 *row;

    table = ACMD_DIR_LUT;
    do {
        row = *table;
        for (i = 0; i < 4; i++) {
            if (acmd == row[i]) {
                return row[dir];
            }
        }
        table++;
    } while (*table != NULL);
    return acmd;
}


void func_ov021_02197eb4(FieldActor *param_1) {
    u16 acmd;
    u16 acmdState;

    while (1) {
        acmd = FldAct_GetAcmd(param_1);
        if (acmd == 0xff) {
            break;
        }
        acmdState = FldAct_GetAcmdState(param_1);
        if (FldAct_ExecAcmd(param_1, acmd, acmdState) == 0) {
            break;
        }
    }
}


u32 func_ov021_02197edc(FieldActor *param_1) {
    BOOL BVar2;

    if (FldAct_GetAcmd(param_1) != 0xff) {
        func_ov021_02197eb4(param_1);
        BVar2 = CheckActorMovementFlag(param_1, 0x400);
        if (BVar2 == 0) {
            return 0;
        }
    }
    ClearActorMovementFlag(param_1, 0x400);
    SetNextActorAcmd(param_1, 0xff);
    func_ov010_0216d7c8(param_1, 0);
    return 1;
}

u32 FldAct_ExecAcmd(FieldActor *param_1, s32 param_2, s32 param_3) {
    if (CheckActorFlag(param_1, 0x2000)) {
        return ACTOR_ACTION_COMMAND_TABLES_RAIL[param_2][param_3](param_1);
    }
    return ACTOR_ACTION_COMMAND_TABLES[param_2][param_3](param_1);
}


u32 func_ov021_02197f5c(FieldActor *param_1) {
    SetActorMovementFlag(param_1, 0x400);
    return 0;
}

void func_ov021_02197f6c(FieldActor *param_1, u32 param_2) {
    CheckSetActorFaceDir(param_1, param_2);
    func_ov010_0216d674(param_1, 0);
    SetActorInitialGPosToNowGPos(param_1);
    FieldActor_NextAcmdState(param_1);
}


u32 func_ov021_02197f90(FieldActor *param_1) {
    func_ov021_02197f6c(param_1, 0);
    return 1;
}

u32 func_ov021_02197f9c(FieldActor *param_1) {
    func_ov021_02197f6c(param_1, 1);
    return 1;
}

u32 func_ov021_02197fa8(FieldActor *param_1) {
    func_ov021_02197f6c(param_1, 2);
    return 1;
}

u32 func_ov021_02197fb4(FieldActor *param_1) {
    func_ov021_02197f6c(param_1, 3);
    return 1;
}

// The work of the moves that last some frames (pokebw2's)
typedef struct {
    u16 state;
    s16 frames;
    u32 dir;
    fx32 speed;
} FieldAcmdWalkWork;

void func_ov021_02197fc0(FieldActor *param_1, u32 param_2, u32 param_3, u16 param_4, u16 param_5) {
    FieldAcmdWalkWork *ptr;

    ptr = func_ov010_0216d6bc(param_1, sizeof(FieldAcmdWalkWork));
    ptr->state = param_5;
    ptr->frames = param_4;
    ptr->dir = param_2;
    ptr->speed = param_3;
    func_ov010_02163e7c(param_1, (u16)param_2);
    ChangeActorDirection(param_1, (u16)param_2);
    func_ov010_0216d674(param_1, (u32)param_5);
    SetActorMovementFlag(param_1, 0x20);
    FieldActor_NextAcmdState(param_1);
}

u32 func_ov021_0219800c(FieldActor *param_1) {
    FieldAcmdWalkWork *ptr;

    ptr = func_ov010_0216d6d4(param_1);
    func_ov010_02163fc0(param_1, (u16)ptr->dir, ptr->speed);
    CheckRecalcActorY(param_1);
    ptr->frames = ptr->frames + -1;
    if (0 < ptr->frames) {
        return 0;
    }
    SetActorMovementFlag(param_1, 0x440);
    SetActorInitialGPosToNowGPos(param_1);
    func_ov010_0216d674(param_1, 0);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

u32 func_ov021_02198060(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 0, 0x800, 0x20, 1);
    return 1;
}

u32 func_ov021_02198074(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 1, 0x800, 0x20, 1);
    return 1;
}

u32 func_ov021_02198088(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 2, 0x800, 0x20, 1);
    return 1;
}

u32 func_ov021_0219809c(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 3, 0x800, 0x20, 1);
    return 1;
}

u32 func_ov021_021980b0(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 0, 0x1000, 0x10, 2);
    return 1;
}

u32 func_ov021_021980c4(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 1, 0x1000, 0x10, 2);
    return 1;
}

u32 func_ov021_021980dc(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 2, 0x1000, 0x10, 2);
    return 1;
}

u32 func_ov021_021980f0(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 3, 0x1000, 0x10, 2);
    return 1;
}

u32 func_ov021_02198104(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 0, 0x2000, 8, 3);
    return 1;
}

u32 func_ov021_0219811c(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 1, 0x2000, 8, 3);
    return 1;
}

u32 func_ov021_02198134(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 2, 0x2000, 8, 3);
    return 1;
}

u32 func_ov021_02198148(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 3, 0x2000, 8, 3);
    return 1;
}

u32 func_ov021_0219815c(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 0, 0x4000, 4, 4);
    return 1;
}

u32 func_ov021_02198170(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 1, 0x4000, 4, 4);
    return 1;
}

u32 func_ov021_02198184(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 2, 0x4000, 4, 4);
    return 1;
}

u32 func_ov021_02198198(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 3, 0x4000, 4, 4);
    return 1;
}

u32 func_ov021_021981ac(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 0, 0x8000, 2, 5);
    return 1;
}

u32 func_ov021_021981c4(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 1, 0x8000, 2, 5);
    return 1;
}

u32 func_ov021_021981dc(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 2, 0x8000, 2, 5);
    return 1;
}

u32 func_ov021_021981f0(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 3, 0x8000, 2, 5);
    return 1;
}

u32 func_ov021_02198208(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 0, 0x10000, 1, 0);
    return 1;
}

u32 func_ov021_0219821c(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 1, 0x10000, 1, 0);
    return 1;
}

u32 func_ov021_02198234(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 2, 0x10000, 1, 0);
    return 1;
}

u32 func_ov021_02198248(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 3, 0x10000, 1, 0);
    return 1;
}

u32 func_ov021_02198260(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 0, 0x4000, 4, 10);
    return 1;
}

u32 func_ov021_02198278(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 1, 0x4000, 4, 10);
    return 1;
}

u32 func_ov021_02198290(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 2, 0x4000, 4, 10);
    return 1;
}

u32 func_ov021_021982a4(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 3, 0x4000, 4, 10);
    return 1;
}

u32 func_ov021_021982bc(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 0, 0x1000, 0x10, 5);
    return 1;
}

u32 func_ov021_021982d4(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 1, 0x1000, 0x10, 5);
    return 1;
}

u32 func_ov021_021982ec(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 2, 0x1000, 0x10, 5);
    return 1;
}

u32 func_ov021_02198300(FieldActor *param_1) {
    func_ov021_02197fc0(param_1, 3, 0x1000, 0x10, 5);
    return 1;
}

void func_ov021_02198318(FieldActor *param_1, u32 param_2, u16 param_3, u32 param_4) {
    FieldAcmdWalkWork *data;

    data = func_ov010_0216d6bc(param_1, sizeof(FieldAcmdWalkWork));
    data->state = param_4;
    data->frames = param_3;
    CheckSetActorFaceDir(param_1, param_2);
    func_ov010_0216d674(param_1, param_4);
    SetActorInitialGPosToNowGPos(param_1);
    FieldActor_NextAcmdState(param_1);
}


u32 func_ov021_0219834c(FieldActor *param_1) {
    FieldAcmdWalkWork *ptr;

    ptr = func_ov010_0216d6d4(param_1);
    ptr->frames = ptr->frames + -1;
    if (0 < ptr->frames) {
        return 0;
    }
    SetActorMovementFlag(param_1, 0x400);
    func_ov010_0216d674(param_1, 0);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

u32 func_ov021_02198380(FieldActor *param_1) {
    func_ov021_02198318(param_1, 0, 0x20, 1);
    return 1;
}

u32 func_ov021_02198390(FieldActor *param_1) {
    func_ov021_02198318(param_1, 1, 0x20, 1);
    return 1;
}

u32 func_ov021_021983a0(FieldActor *param_1) {
    func_ov021_02198318(param_1, 2, 0x20, 1);
    return 1;
}

u32 func_ov021_021983b0(FieldActor *param_1) {
    func_ov021_02198318(param_1, 3, 0x20, 1);
    return 1;
}

u32 func_ov021_021983c0(FieldActor *param_1) {
    func_ov021_02198318(param_1, 0, 0x10, 2);
    return 1;
}

u32 func_ov021_021983d0(FieldActor *param_1) {
    func_ov021_02198318(param_1, 1, 0x10, 2);
    return 1;
}

u32 func_ov021_021983e0(FieldActor *param_1) {
    func_ov021_02198318(param_1, 2, 0x10, 2);
    return 1;
}

u32 func_ov021_021983f0(FieldActor *param_1) {
    func_ov021_02198318(param_1, 3, 0x10, 2);
    return 1;
}

u32 func_ov021_02198400(FieldActor *param_1) {
    func_ov021_02198318(param_1, 0, 8, 3);
    return 1;
}

u32 func_ov021_02198410(FieldActor *param_1) {
    func_ov021_02198318(param_1, 1, 8, 3);
    return 1;
}

u32 func_ov021_02198420(FieldActor *param_1) {
    func_ov021_02198318(param_1, 2, 8, 3);
    return 1;
}

u32 func_ov021_02198430(FieldActor *param_1) {
    func_ov021_02198318(param_1, 3, 8, 3);
    return 1;
}

u32 func_ov021_02198440(FieldActor *param_1) {
    func_ov021_02198318(param_1, 0, 4, 4);
    return 1;
}

u32 func_ov021_02198450(FieldActor *param_1) {
    func_ov021_02198318(param_1, 1, 4, 4);
    return 1;
}

u32 func_ov021_02198460(FieldActor *param_1) {
    func_ov021_02198318(param_1, 2, 4, 4);
    return 1;
}

u32 func_ov021_02198470(FieldActor *param_1) {
    func_ov021_02198318(param_1, 3, 4, 4);
    return 1;
}

u32 func_ov021_02198480(FieldActor *param_1) {
    func_ov021_02198318(param_1, 0, 2, 5);
    return 1;
}

u32 func_ov021_02198490(FieldActor *param_1) {
    func_ov021_02198318(param_1, 1, 2, 5);
    return 1;
}

u32 func_ov021_021984a0(FieldActor *param_1) {
    func_ov021_02198318(param_1, 2, 2, 5);
    return 1;
}

u32 func_ov021_021984b0(FieldActor *param_1) {
    func_ov021_02198318(param_1, 3, 2, 5);
    return 1;
}

void func_ov021_021984c0(FieldActor *param_1, int param_2, s32 param_3, u8 param_4, u16 param_5, s16 param_6, u16 param_7, u32 param_8) {
    FieldAcmdJumpWork *work;

    work = func_ov010_0216d6bc(param_1, sizeof(FieldAcmdJumpWork));
    work->dir = param_2;
    work->speed = param_3;
    work->frames = param_4;
    work->state = param_5;
    work->heightTable = param_6;
    work->heightStep = param_7;
    if (param_3 == 0) {
        SetActorInitialGPosToNowGPos(param_1);
    } else {
        func_ov010_02163e7c(param_1, param_2);
    }
    SetActorMovementFlag(param_1, 0xa0);
    ChangeActorDirection(param_1, param_2);
    func_ov010_0216d674(param_1, param_5);
    FieldActor_NextAcmdState(param_1);
    if (param_8 != 0) {
        GFL_SndSEPlay(param_8);
    }
}


void func_ov021_0219852c(FieldActor *param_1, u16 param_2, s32 param_3, u8 param_4, u16 param_5, s16 param_6, u16 param_7) {
    func_ov021_021984c0(param_1, param_2, param_3, param_4, param_5, param_6, param_7, 0x55e);
}

BOOL func_ov021_02198550(FieldActor *actor) {
    FieldAcmdJumpWork *work = func_ov010_0216d6d4(actor);
    VecFx32 offset;
    fx32 speed;
    const fx32 *row;
    u16 index;

    if (work->speed != 0) {
        func_ov010_02163fc0(actor, work->dir, work->speed);
        CheckRecalcActorY(actor);
        if (work->progress >= 0x10000) {
            work->progress = 0;
            func_ov010_02163e7c(actor, work->dir);
            SetActorMovementFlag(actor, 0x20);
        }
        speed = work->speed;
        if (speed < 0) {
            speed = -speed;
        }
        work->progress += speed;
    }
    work->height += work->heightStep;
    if (work->height > 0xf00) {
        work->height = 0xf00;
    }
    index = work->height / 256;
    row = data_ov021_021ddbd4[work->heightTable];
    offset.x = 0;
    offset.y = row[index];
    offset.z = 0;
    SetActorWPosOffset(actor, &offset);
    work->frames--;
    if (work->frames > 0) {
        return FALSE;
    }
    VecFx32 zero = { 0, 0, 0 };
    SetActorWPosOffset(actor, &zero);
    SetActorMovementFlag(actor, 0x540);
    SetActorInitialGPosToNowGPos(actor);
    func_ov010_0216d674(actor, 0);
    FieldActor_NextAcmdState(actor);
    GFL_SndSEPlay(SEQ_SE_FLD_10);
    return TRUE;
}


u32 func_ov021_0219862c(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 0, 0, 0x10, 2, 1, 0x100);
    return 1;
}


u32 func_ov021_02198650(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 1, 0, 0x10, 2, 1, 0x100);
    return 1;
}

u32 func_ov021_02198674(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 2, 0, 0x10, 2, 1, 0x100);
    return 1;
}

u32 func_ov021_02198694(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 3, 0, 0x10, 2, 1, 0x100);
    return 1;
}

u32 func_ov021_021986b8(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 0, 0, 8, 3, 1, 0x200);
    return 1;
}

u32 func_ov021_021986d8(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 1, 0, 8, 3, 1, 0x200);
    return 1;
}

u32 func_ov021_021986f8(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 2, 0, 8, 3, 1, 0x200);
    return 1;
}


u32 func_ov021_02198718(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 3, 0, 8, 3, 1, 0x200);
    return 1;
}

u32 func_ov021_02198738(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 0, 0x2000, 8, 3, 0, 0x200);
    return 1;
}


u32 func_ov021_02198758(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 1, 0x2000, 8, 3, 0, 0x200);
    return 1;
}

u32 func_ov021_0219877c(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 2, 0x2000, 8, 3, 0, 0x200);
    return 1;
}

u32 func_ov021_021987a0(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 3, 0x2000, 8, 3, 0, 0x200);
    return 1;
}

u32 func_ov021_021987c0(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 0, 0x2000, 0x10, 3, 0, 0x100);
    return 1;
}

u32 func_ov021_021987e0(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 1, 0x2000, 0x10, 3, 0, 0x100);
    return 1;
}

u32 func_ov021_02198804(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 2, 0x2000, 0x10, 3, 0, 0x100);
    return 1;
}

u32 func_ov021_02198824(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 3, 0x2000, 0x10, 3, 0, 0x100);
    return 1;
}


u32 func_ov021_02198848(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 2, 0x1000, 0x10, 2, 0, 0xf0);
    return 1;
}

u32 func_ov021_02198868(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 3, 0x1000, 0x10, 2, 0, 0xf0);
    return 1;
}

u32 func_ov021_02198888(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 2, 0x4000, 0xc, 3, 0, 0x140);
    return 1;
}

u32 func_ov021_021988ac(FieldActor *param_1) {
    func_ov021_0219852c(param_1, 3, 0x4000, 0xc, 3, 0, 0x140);
    return 1;
}


u32 func_ov021_021988d0(FieldActor *param_1) {
    func_ov021_021984c0(param_1, 0, 0x2000, 0x18, 3, 0, 0xa0, 0);
    return 1;
}

u32 func_ov021_021988f4(FieldActor *param_1) {
    func_ov021_021984c0(param_1, 1, 0x2000, 0x18, 3, 0, 0xa0, 0);
    return 1;
}

u32 func_ov021_02198918(FieldActor *param_1) {
    func_ov021_021984c0(param_1, 2, 0x2000, 0x18, 3, 0, 0xa0, 0);
    return 1;
}

u32 func_ov021_0219893c(FieldActor *param_1) {
    func_ov021_021984c0(param_1, 3, 0x2000, 0x18, 3, 0, 0xa0, 0);
    return 1;
}

void func_ov021_02198960(FieldActor *param_1, u32 param_2) {
    u32 *ptr;

    ptr = func_ov010_0216d6bc(param_1, 4);
    *ptr = param_2;
    FieldActor_NextAcmdState(param_1);
}

u32 func_ov021_02198978(FieldActor *param_1) {
    s32 *ptr;

    ptr = func_ov010_0216d6d4(param_1);
    if (*ptr != 0) {
        *ptr = *ptr + -1;
        return 0;
    }
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02198998(FieldActor *param_1) {
    func_ov021_02198960(param_1, 1);
    return 1;
}

u32 func_ov021_021989a4(FieldActor *param_1) {
    func_ov021_02198960(param_1, 2);
    return 1;
}

u32 func_ov021_021989b0(FieldActor *param_1) {
    func_ov021_02198960(param_1, 4);
    return 1;
}

u32 func_ov021_021989bc(FieldActor *param_1) {
    func_ov021_02198960(param_1, 8);
    return 1;
}

u32 func_ov021_021989c8(FieldActor *param_1) {
    func_ov021_02198960(param_1, 0xf);
    return 1;
}

u32 func_ov021_021989d4(FieldActor *param_1) {
    func_ov021_02198960(param_1, 0x10);
    return 1;
}

u32 func_ov021_021989e0(FieldActor *param_1) {
    func_ov021_02198960(param_1, 0x20);
    return 1;
}

u32 func_ov021_021989ec(FieldActor *param_1) {
    AcmdPosSpeedWork *work = func_ov010_0216d6bc(param_1, sizeof(AcmdPosSpeedWork));

    work->speed = 0x10000;
    func_ov010_0216d674(param_1, 0);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02198a10(FieldActor *param_1) {
    AcmdPosSpeedWork *work;

    work = func_ov010_0216d6d4(param_1);
    work->pos += work->speed;
    {
        VecFx32 offset = { 0, work->pos, 0 };
        SetActorWPosOffset(param_1, &offset);
    }
    if (work->pos / 0x8000 < 0x28) {
        return 0;
    }
    FieldActor_NextAcmdState(param_1);
    return 1;
}


u32 func_ov021_02198a58(FieldActor *param_1) {
    AcmdPosSpeedWork *ptr;

    ptr = func_ov010_0216d6bc(param_1, sizeof(AcmdPosSpeedWork));
    ptr->pos = 0x140000;
    ptr->speed = 0xffff0000;
    GFL_SndSEPlay(0x676);
    func_ov010_0216d674(param_1, 0);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02198a8c(FieldActor *param_1) {
    AcmdPosSpeedWork *work;

    work = func_ov010_0216d6d4(param_1);
    work->pos += work->speed;
    if (work->pos < 0) {
        work->pos = 0;
    }
    {
        VecFx32 offset = { 0, work->pos, 0 };
        SetActorWPosOffset(param_1, &offset);
    }
    if (work->pos > 0) {
        return 0;
    }
    FieldActor_NextAcmdState(param_1);
    return 1;
}


u32 func_ov021_02198ad4(FieldActor *param_1) {
    SetActorFlag(param_1, 4);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02198ae8(FieldActor *param_1) {
    ClearActorFlag(param_1, 4);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02198afc(FieldActor *param_1) {
    SetActorFlag(param_1, 8);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02198b10(FieldActor *param_1) {
    ClearActorFlag(param_1, 8);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02198b24(FieldActor *param_1) {
    SetActorFlag(param_1, 0x10);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02198b38(FieldActor *param_1) {
    ClearActorFlag(param_1, 0x10);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

void func_ov021_02198b4c(FieldActor *param_1, u32 param_2, u32 param_3) {
    AcmdEffectWork *ptr;
    MMSys *mmSys;
    Field *field;
    FieldEffects *effects;
    FieldEffectTask *pFVar2;

    ptr = func_ov010_0216d6bc(param_1, sizeof(AcmdEffectWork));
    ptr->kind = param_2;
    mmSys = GetActorMModelSystem(param_1);
    field = GetMMSysField(mmSys);
    effects = Field_GetFieldEffects(field);
    pFVar2 = func_ov021_021bb528(effects, param_1, param_2, 1);
    ptr->task = pFVar2;
    FieldActor_NextAcmdState(param_1);
}

u32 func_ov021_02198b80(FieldActor *param_1) {
    AcmdEffectWork *ptr;
    BOOL BVar2;

    ptr = func_ov010_0216d6d4(param_1);
    BVar2 = func_ov021_021bb5c8(ptr->task);
    if (BVar2 == 1) {
        func_ov021_021ac154(ptr->task);
        FieldActor_NextAcmdState(param_1);
        return 1;
    }
    return 0;
}

u32 func_ov021_02198ba8(FieldActor *param_1) {
    func_ov021_02198b4c(param_1, 0, 0);
    return 0;
}

u32 func_ov021_02198bb8(FieldActor *param_1) {
    func_ov021_02198b4c(param_1, 0, 0);
    return 0;
}

u32 func_ov021_02198bc8(FieldActor *param_1) {
    func_ov021_02198b4c(param_1, 0, 0);
    return 0;
}

u32 func_ov021_02198bd8(FieldActor *param_1) {
    func_ov021_02198b4c(param_1, 1, 0);
    return 0;
}

u32 func_ov021_02198be8(FieldActor *param_1) {
    func_ov021_02198b4c(param_1, 2, 0);
    return 0;
}

u32 func_ov021_02198bf8(FieldActor *param_1) {
    func_ov021_02198b4c(param_1, 3, 0);
    return 0;
}

void func_ov021_02198c08(FieldActor *param_1, u16 param_2, u16 param_3, u32 param_4) {
    AcmdDirStepsWork *work;
    s64 dir;

    work = func_ov010_0216d6bc(param_1, sizeof(AcmdDirStepsWork));
    work->dir = param_2;
    dir = param_2;
    work->state = param_4;
    work->steps = param_3;
    func_ov010_02163e7c(param_1, dir);
    ChangeActorDirection(param_1, dir);
    func_ov010_0216d674(param_1, param_4);
    SetActorMovementFlag(param_1, 0x20);
    FieldActor_NextAcmdState(param_1);
}

u32 func_ov021_02198c4c(FieldActor *param_1, fx32 *param_2) {
    AcmdDirStepsWork *work;

    work = func_ov010_0216d6d4(param_1);
    func_ov010_02163fc0(param_1, work->dir, param_2[work->index]);
    CheckRecalcActorY(param_1);
    work->index = work->index + 1;
    if (work->index < work->steps) {
        return 0;
    }
    SetActorMovementFlag(param_1, 0x440);
    SetActorInitialGPosToNowGPos(param_1);
    func_ov010_0216d674(param_1, 0);
    FieldActor_NextAcmdState(param_1);
    return 1;
}


u32 func_ov021_02198ca4(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 0, 6, 6);
    return 1;
}

u32 func_ov021_02198cb4(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 1, 6, 6);
    return 1;
}

u32 func_ov021_02198cc4(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 2, 6, 6);
    return 1;
}

u32 func_ov021_02198cd4(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 3, 6, 6);
    return 1;
}

u32 func_ov021_02198ce4(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 0, 6, 0xb);
    return 1;
}

u32 func_ov021_02198cf4(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 1, 6, 0xb);
    return 1;
}

u32 func_ov021_02198d04(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 2, 6, 0xb);
    return 1;
}

u32 func_ov021_02198d14(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 3, 6, 0xb);
    return 1;
}

u32 func_ov021_02198c4c(FieldActor *param_1, fx32 *param_2);

u32 func_ov021_02198d24(FieldActor *param_1) {
    if (func_ov021_02198c4c(param_1, (fx32 *)data_ov021_021d4a68) == 1) {
        return 1;
    }
    return 0;
}

u32 func_ov021_02198d3c(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 0, 3, 7);
    return 1;
}

u32 func_ov021_02198d4c(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 1, 3, 7);
    return 1;
}

u32 func_ov021_02198d5c(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 2, 3, 7);
    return 1;
}

u32 func_ov021_02198d6c(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 3, 3, 7);
    return 1;
}


u32 func_ov021_02198d7c(FieldActor *param_1) {
    u32 result;

    result = func_ov021_02198c4c(param_1, (fx32 *)data_ov021_021d4a44);
    if (result == 1) {
        return 1;
    }
    return 0;
}

void func_ov021_02199888(FieldActor *param_1, s32 param_2, s32 param_3, s32 param_4, u8 param_5, u8 param_6, u8 param_7, u8 param_8, u8 param_9);
void func_ov021_0219fe64(FieldActor *param_1);
u32 func_ov021_0219fe20(FieldActor *param_1, u32 param_2, u32 param_3, u32 param_4);

u32 func_ov021_0219fcc0(FieldActor *, u16 *, u16 *);
u32 func_ov021_0219fc68(FieldActor *, u16 *, u16 *);

u32 func_ov021_02198d94(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 0, 7, 8);
    return 1;
}

u32 func_ov021_02198da4(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 1, 7, 8);
    return 1;
}

u32 func_ov021_02198db4(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 2, 7, 8);
    return 1;
}

u32 func_ov021_02198dc4(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 3, 7, 8);
    return 1;
}

u32 func_ov021_02198dd4(FieldActor *param_1) {
    if (func_ov021_02198c4c(param_1, (fx32 *)data_ov021_021d4a80) == 1) {
        return 1;
    }
    return 0;
}

u32 func_ov021_02198dec(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 0, 0xc, 9);
    return 1;
}

u32 func_ov021_02198dfc(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 1, 0xc, 9);
    return 1;
}

u32 func_ov021_02198e0c(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 2, 0xc, 9);
    return 1;
}

u32 func_ov021_02198e1c(FieldActor *param_1) {
    func_ov021_02198c08(param_1, 3, 0xc, 9);
    return 1;
}

u32 func_ov021_02198e2c(FieldActor *param_1) {
    if (func_ov021_02198c4c(param_1, (fx32 *)data_ov021_021d4a9c) == 1) {
        return 1;
    }
    return 0;
}

u32 func_ov021_02198e44(FieldActor *param_1) {
    func_ov010_0216d6bc(param_1, 4);
    func_ov010_0216d674(param_1, 10);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

u32 func_ov021_02198e60(FieldActor *param_1) {
    func_ov010_0216d6bc(param_1, 4);
    func_ov010_0216d674(param_1, 0xb);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

u32 func_ov021_02198e7c(FieldActor *param_1) {
    func_ov010_0216d6bc(param_1, 4);
    func_ov010_0216d674(param_1, 0xc);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

void func_ov021_02198e98(FieldActor *param_1) {
    GFL_SndSEPlay(0x7b7);
    func_ov021_02198e44(param_1);
}

void func_ov021_02198eb0(FieldActor *param_1) {
    GFL_SndSEPlay(0x7b7);
    func_ov021_02198e60(param_1);
}

u32 func_ov021_02198ec8(FieldActor *param_1) {
    u32 blActIdx;
    MMSys *system;
    BlActSys *sys;
    BOOL BVar3;
    u16 auStack_18 [2];

    func_ov010_0216d6d4(param_1);
    blActIdx = FldAct_GetBlActIdx(param_1, 0);
    if (blActIdx != 0xffff) {
        system = GetActorMModelSystem(param_1);
        sys = FieldBlAct_GetBlActSys(func_ov010_0216d520(system));
        BVar3 = func_020508ec(sys, blActIdx & 0xffff, auStack_18);
        if (BVar3 == 1) {
            func_ov010_0216d674(param_1, 0);
            FieldActor_NextAcmdState(param_1);
            return 1;
        }
    }
    return 0;
}

u32 func_ov021_02198f20(FieldActor *param_1, u32 param_2, u32 param_3, u32 param_4) {
    u16 sp0;
    u32 actor;

    func_ov010_0216d6d4(param_1);
    actor = FldAct_GetBlActIdx(param_1, 0U);
    if ((actor != 0xFFFF) && (func_020508ec(FieldBlAct_GetBlActSys(func_ov010_0216d520(GetActorMModelSystem(param_1))), (u32) (u16) actor, &sp0) == 1)) {
        FieldActor_NextAcmdState(param_1);
        return 1U;
    }
    return 0U;
}

u32 func_ov021_02198f68(FieldActor *param_1) {
    FieldEffectTask *task;

    func_ov010_0216d6bc(param_1, 4);
    task = func_ov021_0219bf48(param_1);
    if (task != NULL) {
        func_ov021_021d04a4(task);
    }
    {
        VecFx32 vec = {0, 0, 0};
        SetActorWPosOffset(param_1, &vec);
    }
    SetActorMovementFlag(param_1, 0xa0);
    ClearActorMovementFlag(param_1, 0x8000);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

u32 func_ov021_02198fb0(FieldActor *param_1) {
    u32 *work = func_ov010_0216d6d4(param_1);
    FieldEffectTask *task;
    u16 state;
    u32 frame;
    VecFx32 vec = {0, 0, 0};

    vec.y = data_ov021_021ddbd4[0][*work];
    SetActorWPosOffset(param_1, &vec);
    task = func_ov021_0219bf48(param_1);
    if (task != NULL) {
        state = func_ov021_021d04c4(task);
        if (state == 1) {
            func_ov021_021ac154(task);
            func_ov021_0219bf3c(param_1, NULL);
        }
    }
    frame = *work;
    *work = frame + 2;
    if (frame + 2 < 0x10) {
        return 0;
    }
    vec.y = 0;
    SetActorWPosOffset(param_1, &vec);
    SetActorMovementFlag(param_1, 0x540);
    func_ov021_0219bf54(param_1);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

void func_ov021_02199034(FieldActor *param_1, u32 param_2, u16 param_3) {
    AcmdWaitWork *work = func_ov010_0216d6bc(param_1, sizeof(AcmdWaitWork));

    work->limit = param_3;
    func_ov010_0216d674(param_1, param_2);
}

u32 func_ov021_02199050(FieldActor *param_1) {
    AcmdWaitWork *work = func_ov010_0216d6d4(param_1);
    work->count = work->count + 1;
    if (work->count >= work->limit) {
        FieldActor_NextAcmdState(param_1);
    }
    return 0;
}

u32 func_ov021_02199070(FieldActor *param_1) {
    func_ov021_02199034(param_1, 1, 0x28);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

u32 func_ov021_02199088(FieldActor *param_1) {
    func_ov021_02199034(param_1, 2, 0x28);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

u32 func_ov021_021990a0(FieldActor *param_1) {
    func_ov021_02199034(param_1, 0, 0xc);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

u32 func_ov021_021990b8(FieldActor *param_1) {
    func_ov010_0216d6bc(param_1, 4);
    func_ov010_0216d674(param_1, 1);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_021990d4(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, 0 };
    fx32 offsets[10] = { 0, 0, 0, 0, 0, 0x4000, 0x4000, 0x3000, 0x1000, 0 };
    s32 *ptr;
    s32 val;

    ptr = func_ov010_0216d6d4(param_1);
    val = *ptr;
    if (val >= 10) {
        val = 9;
    }
    vec.y = offsets[val];
    SetActorWPosOffset(param_1, &vec);
    val = *ptr;
    *ptr = val + 1;
    if (val + 1 >= 0x14) {
        FieldActor_NextAcmdState(param_1);
    }
    return 0;
}

void func_ov021_0219912c(FieldActor *param_1, const VecFx32 *param_2, u32 param_3, u32 param_4, u32 param_5, u8 param_6) {
    AcmdShiftWork *ptr;

    ptr = func_ov010_0216d6bc(param_1, sizeof(AcmdShiftWork));
    ptr->frames = param_5;
    ptr->delta = *param_2;
    CheckSetActorFaceDir(param_1, param_3);
    SetActorMotionDir(param_1, param_4);
    func_ov010_0216d674(param_1, param_6);
    func_ov010_0216da70(param_1);
    SetActorInitialGPosX(param_1, GetGPosX(param_1));
    SetActorInitialGPosY(param_1, FldAct_GetGPosY(param_1));
    SetActorInitialGPosZ(param_1, GetGPosZ(param_1));
    if (param_2->x < 0) {
        adjustXPos(param_1, -1);
    } else if (param_2->x > 0) {
        adjustXPos(param_1, 1);
    }
    if (param_2->y < 0) {
        adjustZPos(param_1, -1);
    } else if (param_2->y > 0) {
        adjustZPos(param_1, 1);
    }
    if (param_2->z < 0) {
        adjustYPos(param_1, -1);
    } else if (param_2->z > 0) {
        adjustYPos(param_1, 1);
    }
    FieldActor_NextAcmdState(param_1);
}

u32 func_ov021_021991f0(FieldActor *param_1) {
    AcmdShiftWork *ptr;
    s32 val;

    ptr = func_ov010_0216d6d4(param_1);
    func_ov010_02163f8c(param_1, &ptr->delta);
    val = ptr->frames;
    ptr->frames = val + -1;
    if (0 < val + -1) {
        return 0;
    }
    SetActorMovementFlag(param_1, 0x440);
    SetActorInitialGPosToNowGPos(param_1);
    func_ov010_0216d674(param_1, 0);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02199234(FieldActor *param_1) {
    VecFx32 vec = { 0, 0x2000, 0 };

    func_ov021_0219912c(param_1, &vec, 2, 0, 8, 3);
    return 1;
}

u32 func_ov021_02199268(FieldActor *param_1) {
    VecFx32 vec = { 0, -0x2000, 0 };

    func_ov021_0219912c(param_1, &vec, 3, 1, 8, 3);
    return 1;
}

u32 func_ov021_02199298(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, 0x2000 };

    func_ov021_0219912c(param_1, &vec, 1, 2, 8, 3);
    return 1;
}

u32 func_ov021_021992cc(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, -0x2000 };

    func_ov021_0219912c(param_1, &vec, 0, 3, 8, 3);
    return 1;
}

u32 func_ov021_021992fc(FieldActor *param_1) {
    VecFx32 vec = { 0, 0x2000, 0 };

    func_ov021_0219912c(param_1, &vec, 3, 0, 8, 3);
    return 1;
}

u32 func_ov021_0219932c(FieldActor *param_1) {
    VecFx32 vec = { 0, -0x2000, 0 };

    func_ov021_0219912c(param_1, &vec, 2, 1, 8, 3);
    return 1;
}

u32 func_ov021_02199360(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, -0x2000 };

    func_ov021_0219912c(param_1, &vec, 0, 2, 8, 3);
    return 1;
}

u32 func_ov021_02199394(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, 0x2000 };

    func_ov021_0219912c(param_1, &vec, 1, 2, 8, 3);
    return 1;
}

u32 func_ov021_021993c8(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, 0x2000 };

    func_ov021_0219912c(param_1, &vec, 1, 0, 8, 3);
    return 1;
}

u32 func_ov021_021993fc(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, -0x2000 };

    func_ov021_0219912c(param_1, &vec, 0, 1, 8, 3);
    return 1;
}

u32 func_ov021_02199430(FieldActor *param_1) {
    VecFx32 vec = { -0x2000, 0, 0 };

    func_ov021_0219912c(param_1, &vec, 3, 2, 8, 3);
    return 1;
}

u32 func_ov021_02199460(FieldActor *param_1) {
    VecFx32 vec = { 0x2000, 0, 0 };

    func_ov021_0219912c(param_1, &vec, 2, 3, 8, 3);
    return 1;
}

u32 func_ov021_02199490(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, 0x4000 };

    func_ov021_0219912c(param_1, &vec, 1, 0, 4, 4);
    return 1;
}

u32 func_ov021_021994c0(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, -0x4000 };

    func_ov021_0219912c(param_1, &vec, 0, 1, 4, 4);
    return 1;
}

u32 func_ov021_021994f0(FieldActor *param_1) {
    VecFx32 vec = { -0x4000, 0, 0 };

    func_ov021_0219912c(param_1, &vec, 3, 2, 4, 4);
    return 1;
}

u32 func_ov021_02199520(FieldActor *param_1) {
    VecFx32 vec = { 0x4000, 0, 0 };

    func_ov021_0219912c(param_1, &vec, 2, 3, 4, 4);
    return 1;
}

u32 func_ov021_02199550(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, 0x8000 };

    func_ov021_0219912c(param_1, &vec, 1, 0, 2, 5);
    return 1;
}

u32 func_ov021_02199584(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, -0x8000 };

    func_ov021_0219912c(param_1, &vec, 0, 1, 2, 5);
    return 1;
}

u32 func_ov021_021995b8(FieldActor *param_1) {
    VecFx32 vec = { -0x8000, 0, 0 };

    func_ov021_0219912c(param_1, &vec, 3, 2, 2, 5);
    return 1;
}

u32 func_ov021_021995e8(FieldActor *param_1) {
    VecFx32 vec = { 0x8000, 0, 0 };

    func_ov021_0219912c(param_1, &vec, 2, 3, 2, 5);
    return 1;
}

u32 func_ov021_02199618(FieldActor *param_1) {
    VecFx32 vec = { 0, 0x4000, 0 };

    func_ov021_0219912c(param_1, &vec, 2, 0, 4, 10);
    return 1;
}

u32 func_ov021_0219964c(FieldActor *param_1) {
    VecFx32 vec = { 0, -0x4000, 0 };

    func_ov021_0219912c(param_1, &vec, 3, 1, 4, 10);
    return 1;
}

u32 func_ov021_02199680(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, 0x4000 };

    func_ov021_0219912c(param_1, &vec, 1, 2, 4, 10);
    return 1;
}

u32 func_ov021_021996b4(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, -0x4000 };

    func_ov021_0219912c(param_1, &vec, 0, 3, 4, 10);
    return 1;
}

u32 func_ov021_021996e8(FieldActor *param_1) {
    VecFx32 vec = { 0, 0x4000, 0 };

    func_ov021_0219912c(param_1, &vec, 3, 0, 4, 10);
    return 1;
}

u32 func_ov021_0219971c(FieldActor *param_1) {
    VecFx32 vec = { 0, -0x4000, 0 };

    func_ov021_0219912c(param_1, &vec, 2, 1, 4, 10);
    return 1;
}

u32 func_ov021_02199750(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, -0x4000 };

    func_ov021_0219912c(param_1, &vec, 0, 2, 4, 10);
    return 1;
}

u32 func_ov021_02199784(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, 0x4000 };

    func_ov021_0219912c(param_1, &vec, 1, 2, 4, 10);
    return 1;
}

u32 func_ov021_021997b8(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, 0x4000 };

    func_ov021_0219912c(param_1, &vec, 1, 0, 4, 10);
    return 1;
}

u32 func_ov021_021997ec(FieldActor *param_1) {
    VecFx32 vec = { 0, 0, -0x4000 };

    func_ov021_0219912c(param_1, &vec, 0, 1, 4, 10);
    return 1;
}

u32 func_ov021_02199820(FieldActor *param_1) {
    VecFx32 vec = { -0x4000, 0, 0 };

    func_ov021_0219912c(param_1, &vec, 3, 2, 4, 10);
    return 1;
}

u32 func_ov021_02199854(FieldActor *param_1) {
    VecFx32 vec = { 0x4000, 0, 0 };

    func_ov021_0219912c(param_1, &vec, 2, 3, 4, 10);
    return 1;
}

// The work of the slide of an actor, one tile in some steps: how long it goes on, the axis it moves along and the one it bobs on
typedef struct ActorSlideWork {
    u8 steps;
    u8 bobAxis;
    u8 axis;
    u8 bobNegative;
    s32 speed;
    s32 progress;
    u16 phase;
    u16 phaseSpeed;
} ActorSlideWork;

void func_ov021_02199888(FieldActor *param_1, s32 param_2, s32 param_3, s32 param_4, u8 param_5, u8 param_6, u8 param_7, u8 param_8, u8 param_9) {
    ActorSlideWork *work;
    s32 dir = 1;

    work = func_ov010_0216d6bc(param_1, sizeof(ActorSlideWork));
    work->steps = param_5;
    work->speed = param_2;
    work->axis = param_7;
    work->bobAxis = param_8;
    work->bobNegative = param_9;
    work->phaseSpeed = 0x1000 / work->steps;
    CheckSetActorFaceDir(param_1, param_3);
    SetActorMotionDir(param_1, param_4);
    func_ov010_0216d674(param_1, param_6);
    func_ov010_0216da70(param_1);
    SetActorInitialGPosX(param_1, GetGPosX(param_1));
    SetActorInitialGPosY(param_1, FldAct_GetGPosY(param_1));
    SetActorInitialGPosZ(param_1, GetGPosZ(param_1));
    if (param_2 != 0) {
        switch (param_7) {
        case 0:
            if (param_2 < 0) {
                dir = -1;
            }
            adjustXPos(param_1, dir);
            break;
        case 1:
            if (param_2 < 0) {
                dir = -1;
            }
            adjustZPos(param_1, dir * 2);
            break;
        case 2:
            if (param_2 < 0) {
                dir = -1;
            }
            adjustYPos(param_1, dir);
            break;
        }
    }
    FieldActor_NextAcmdState(param_1);
}

u32 func_ov021_02199970(FieldActor *param_1) {
    ActorSlideWork *p;
    VecFx32 wpos;
    s32 s;

    p = func_ov010_0216d6d4(param_1);
    CopyActorWPos(param_1, &wpos);
    switch (p->axis) {
    case 0:
        wpos.x += p->speed;
        break;
    case 1:
        wpos.y += p->speed;
        break;
    case 2:
        wpos.z += p->speed;
        break;
    }
    SetActorWPosValue(param_1, &wpos);
    s = p->speed;
    if (s < 0) {
        s = -s;
    }
    p->progress += s;
    p->phase = p->phase + p->phaseSpeed;
    if (p->phase > 0xf00) {
        p->phase = 0xf00;
    }
    {
        VecFx32 offs = {0, 0, 0};
        u16 idx = p->phase / 256;
        fx32 v = data_ov021_021ddbd4[1][idx];
        if (p->bobNegative == 1) {
            v = -v;
        }
        switch (p->bobAxis) {
        case 0:
            offs.x = v;
            break;
        case 1:
            offs.y = v;
            break;
        case 2:
            offs.z = v;
            break;
        }
        SetActorWPosOffset(param_1, &offs);
    }
    p->steps = p->steps - 1;
    if (p->progress >= 0x10000 && p->steps != 0) {
        s32 dir;
        s32 speed;
        p->progress = p->progress - 0x10000;
        dir = 1;
        speed = p->speed;
        SetActorInitialGPosX(param_1, GetGPosX(param_1));
        SetActorInitialGPosY(param_1, FldAct_GetGPosY(param_1));
        SetActorInitialGPosZ(param_1, GetGPosZ(param_1));
        switch (p->axis) {
        case 0:
            if (speed < 0) {
                dir -= 2;
            }
            adjustXPos(param_1, dir);
            break;
        case 1:
            if (speed < 0) {
                dir -= 2;
            }
            adjustZPos(param_1, dir * 2);
            break;
        case 2:
            if (speed < 0) {
                dir -= 2;
            }
            adjustYPos(param_1, dir);
            break;
        }
    }
    if (p->steps != 0) {
        return 0;
    }
    {
        VecFx32 zero = {0, 0, 0};
        SetActorWPosOffset(param_1, &zero);
    }
    SetActorMovementFlag(param_1, 0x540);
    SetActorInitialGPosToNowGPos(param_1);
    func_ov010_0216d674(param_1, 0);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02199aec(FieldActor *param_1) {
    func_ov021_02199888(param_1, 0x2000, 2, 0, 8, 3, 1, 1, 0);
    return 1;
}

u32 func_ov021_02199b10(FieldActor *param_1) {
    func_ov021_02199888(param_1, -0x2000, 3, 1, 8, 3, 1, 1, 0);
    return 1;
}

u32 func_ov021_02199b38(FieldActor *param_1) {
    func_ov021_02199888(param_1, 0x2000, 1, 2, 8, 3, 2, 1, 0);
    return 1;
}

u32 func_ov021_02199b60(FieldActor *param_1) {
    func_ov021_02199888(param_1, -0x2000, 0, 3, 8, 3, 2, 1, 0);
    return 1;
}

u32 func_ov021_02199b88(FieldActor *param_1) {
    func_ov021_02199888(param_1, 0x2000, 3, 0, 8, 3, 1, 1, 0);
    return 1;
}

u32 func_ov021_02199bac(FieldActor *param_1) {
    func_ov021_02199888(param_1, -0x2000, 2, 1, 8, 3, 1, 1, 0);
    return 1;
}

u32 func_ov021_02199bd8(FieldActor *param_1) {
    func_ov021_02199888(param_1, -0x2000, 0, 2, 8, 3, 2, 1, 0);
    return 1;
}

u32 func_ov021_02199c00(FieldActor *param_1) {
    func_ov021_02199888(param_1, 0x2000, 1, 2, 8, 3, 2, 1, 0);
    return 1;
}

u32 func_ov021_02199c28(FieldActor *param_1) {
    func_ov021_02199888(param_1, 0x2000, 1, 0, 8, 3, 2, 1, 0);
    return 1;
}

u32 func_ov021_02199c50(FieldActor *param_1) {
    func_ov021_02199888(param_1, -0x2000, 0, 1, 8, 3, 2, 1, 0);
    return 1;
}

u32 func_ov021_02199c7c(FieldActor *param_1) {
    func_ov021_02199888(param_1, -0x2000, 3, 2, 8, 3, 0, 1, 0);
    return 1;
}

u32 func_ov021_02199ca4(FieldActor *param_1) {
    func_ov021_02199888(param_1, 0x2000, 2, 3, 8, 3, 0, 1, 0);
    return 1;
}

u32 func_ov021_02199cc8(FieldActor *param_1) {
    func_ov010_0216d6bc(param_1, 4);
    func_ov010_0216d674(param_1, 1);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

u32 func_ov021_02199ce4(FieldActor *param_1) {
    s32 *p;

    p = func_ov010_0216d6d4(param_1);
    *p = *p + 1;
    if (*p >= 0x16) {
        FieldActor_NextAcmdState(param_1);
    }
    return 0;
}

u32 func_ov021_02199d00(FieldActor *param_1) {
    func_ov010_0216d674(param_1, 10);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

// The work of acmd 0xc's hop: count steps, direction and speed
typedef struct {
    u16 acmd;
    s16 count;
    u32 dir;
    fx32 speed;
} Ov021HopWork;

u32 func_ov021_02199d14(FieldActor *param_1) {
    Ov021HopWork *work;

    work = func_ov010_0216d6bc(param_1, sizeof(Ov021HopWork));
    work->acmd = 0xc;
    work->count = 2;
    work->dir = 2;
    work->speed = work->dir << 14;
    func_ov010_02163e7c(param_1, work->dir);
    func_ov010_0216d674(param_1, work->acmd);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

typedef struct Ov021_02198d94Work5 {
    u8 unk_00[2];
    s16 unk_02;
    u32 unk_04;
    fx32 unk_08;
} Ov021_02198d94Work5;

u32 func_ov021_02199d4c(FieldActor *param_1) {
    Ov021_02198d94Work5 *ptr;

    ptr = func_ov010_0216d6d4(param_1);
    func_ov010_02163fc0(param_1, (u16)ptr->unk_04, ptr->unk_08);
    CheckRecalcActorY(param_1);
    ptr->unk_02 = ptr->unk_02 + -1;
    if (0 < ptr->unk_02) {
        return 0;
    }
    SetActorInitialGPosToNowGPos(param_1);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

typedef struct Ov021_02198d94Work6 {
    fx32 unk_00;
    u32 unk_04;
    u16 unk_08;
    u8 unk_0a;
} Ov021_02198d94Work6;

u32 func_ov021_02199d8c(FieldActor *param_1) {
    Ov021_02198d94Work6 *ptr;

    ptr = func_ov010_0216d6bc(param_1, 0x10);
    ptr->unk_04 = 0x2000;
    ptr->unk_08 = 0;
    ptr->unk_0a = 0;
    func_ov010_0216d674(param_1, 0xb);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02199db4(FieldActor *param_1) {
    Ov021_02198d94Work6 *work = func_ov010_0216d6d4(param_1);
    u16 anim;
    u16 step;
    if (func_ov021_0219fc68(param_1, &anim, &step) != TRUE) {
        return FALSE;
    }
    work->unk_08++;
    step--;
    if (FldAct_GetObjCode(param_1) == 0xd3) {
        if (anim == 2 && work->unk_0a != step) {
            work->unk_0a = step;
            switch (step) {
            case 7:
            case 12:
            case 17:
                GFL_SndSEPlay(0x7e5);
                break;
            case 10:
            case 15:
            case 20:
                GFL_SndSEPlay(0x7e6);
                break;
            }
        }
    } else if (work->unk_08 == 0x5f) {
        GFL_SndSEPlay(0x7e7);
    }
    if (work->unk_08 < 0x5f) {
        return FALSE;
    }
    work->unk_00 += work->unk_04;
    {
        VecFx32 offset = { 0, 0, 0 };
        offset.y = work->unk_00;
        SetActorWPosOffset(param_1, &offset);
    }
    if (work->unk_00 / 0x8000 < 16) {
        return FALSE;
    }
    func_ov010_0216d674(param_1, 0);
    FieldActor_NextAcmdState(param_1);
    return TRUE;
}

u32 func_ov021_02199eac(FieldActor *param_1) {
    func_ov010_0216d674(param_1, 10);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02199ec0(FieldActor *param_1) {
    func_ov010_0216d674(param_1, 0xb);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

u32 func_ov021_02199ed4(FieldActor *param_1) {
    MMSys *system;
    BlActSys *sys;
    u32 blActIdx;
    BOOL BVar3;
    u16 auStack_18 [2];

    system = GetActorMModelSystem(param_1);
    sys = FieldBlAct_GetBlActSys(func_ov010_0216d520(system));
    blActIdx = FldAct_GetBlActIdx(param_1, 0);
    if ((blActIdx != 0xffff) && (BVar3 = func_020508ec(sys, blActIdx & 0xffff, auStack_18), BVar3 == 0)) {
        return 0;
    }
    CheckSetActorFaceDir(param_1, 1);
    func_ov010_0216d674(param_1, 0);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02199f30(FieldActor *param_1) {
    VecFx32 vec = {0, 0, 0};

    SetActorWPosOffset(param_1, &vec);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02199f50(FieldActor *param_1) {
    VecFx32 vec = { 0, 0x8000, 0 };
    ShiftWork *work = func_ov010_0216d6bc(param_1, 6);

    work->time = 0;
    work->lastFrame = 0;
    SetActorWPosOffset(param_1, &vec);
    func_ov010_0216d674(param_1, 0xa);
    FieldActor_NextAcmdState(param_1);
    return 1;
}

u32 func_ov021_02199f94(FieldActor *param_1) {
    ShiftWork *p;
    u16 other;
    u16 cur;
    u8 *b;
    u32 ret;

    p = func_ov010_0216d6d4(param_1);
    if (func_ov021_0219fcc0(param_1, &other, &cur) != 1) {
        ret = 0;
    } else {
        p->time = p->time + 1;
        cur = cur - 1;
        if (FldAct_GetObjCode(param_1) == 0xdd && other == 0) {
            b = &p->lastFrame;
            if (*b != cur) {
                *b = cur;
                switch (cur) {
                case 0:
                    GFL_SndSEPlay(0x7e5);
                    break;
                case 3:
                    GFL_SndSEPlay(0x7e6);
                    break;
                }
            }
        }
        if (p->time < 0x5f) {
            ret = 0;
        } else {
            FieldActor_NextAcmdState(param_1);
            ret = 1;
        }
    }
    return ret;
}

u32 func_ov021_0219a014(FieldActor *param_1) {
    func_ov010_0216d674(param_1, 10);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

u32 func_ov021_0219a028(FieldActor *param_1) {
    func_ov010_0216d674(param_1, 0xb);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

u32 func_ov021_0219a03c(FieldActor *param_1) {
    func_ov010_0216d674(param_1, 0xc);
    func_ov021_0219fe64(param_1);
    FieldActor_NextAcmdState(param_1);
    return 0;
}

u32 func_ov021_0219a058(FieldActor *param_1, u32 param_2, u32 param_3, u32 param_4) {
    if (func_ov021_0219fe20(param_1, param_2, param_3, param_4) == 1) {
        func_ov010_0216d674(param_1, 0);
        FieldActor_NextAcmdState(param_1);
        return 1;
    }
    return 0;
}

u32 func_ov021_0219a07c(FieldActor *param_1, u32 param_2, u32 param_3, u32 param_4) {
    if (func_ov021_0219fe20(param_1, param_2, param_3, param_4) == 1) {
        FieldActor_NextAcmdState(param_1);
        return 1;
    }
    return 0;
}

const FieldActorFunc data_ov021_021d4180[] = { func_ov021_02197f90, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d41c0[] = { func_ov021_02197f9c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4170[] = { func_ov021_02197fa8, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4198[] = { func_ov021_02197fb4, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4a14[] = { func_ov021_02198060, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4a08[] = { func_ov021_02198074, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d49fc[] = { func_ov021_02198088, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d49f0[] = { func_ov021_0219809c, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d49e4[] = { func_ov021_021980b0, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d49d8[] = { func_ov021_021980c4, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d49cc[] = { func_ov021_021980dc, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d49c0[] = { func_ov021_021980f0, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d49b4[] = { func_ov021_02198104, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d49a8[] = { func_ov021_0219811c, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d499c[] = { func_ov021_02198134, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4990[] = { func_ov021_02198148, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4984[] = { func_ov021_0219815c, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4978[] = { func_ov021_02198170, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d496c[] = { func_ov021_02198184, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4960[] = { func_ov021_02198198, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4954[] = { func_ov021_021981ac, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4948[] = { func_ov021_021981c4, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d493c[] = { func_ov021_021981dc, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4930[] = { func_ov021_021981f0, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4924[] = { func_ov021_02198208, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4918[] = { func_ov021_0219821c, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d490c[] = { func_ov021_02198234, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4900[] = { func_ov021_02198248, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d48f4[] = { func_ov021_02198380, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d48e8[] = { func_ov021_02198390, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d48dc[] = { func_ov021_021983a0, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d48d0[] = { func_ov021_021983b0, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d48c4[] = { func_ov021_021983c0, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d48b8[] = { func_ov021_021983d0, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d48ac[] = { func_ov021_021983e0, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d48a0[] = { func_ov021_021983f0, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4894[] = { func_ov021_02198400, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4888[] = { func_ov021_02198410, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d487c[] = { func_ov021_02198420, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4870[] = { func_ov021_02198430, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4864[] = { func_ov021_02198440, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4858[] = { func_ov021_02198450, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d484c[] = { func_ov021_02198460, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4840[] = { func_ov021_02198470, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4834[] = { func_ov021_02198480, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4828[] = { func_ov021_02198490, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d481c[] = { func_ov021_021984a0, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4810[] = { func_ov021_021984b0, func_ov021_0219834c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4804[] = { func_ov021_0219862c, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d47f8[] = { func_ov021_02198650, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d47ec[] = { func_ov021_02198674, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d47e0[] = { func_ov021_02198694, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d47d4[] = { func_ov021_021986b8, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4468[] = { func_ov021_021986d8, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d47bc[] = { func_ov021_021986f8, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d47b0[] = { func_ov021_02198718, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d47a4[] = { func_ov021_02198738, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4798[] = { func_ov021_02198758, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d478c[] = { func_ov021_0219877c, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4780[] = { func_ov021_021987a0, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4774[] = { func_ov021_021987c0, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4768[] = { func_ov021_021987e0, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d475c[] = { func_ov021_02198804, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4750[] = { func_ov021_02198824, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4744[] = { func_ov021_02198848, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4738[] = { func_ov021_02198868, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4708[] = { func_ov021_02198888, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4714[] = { func_ov021_021988ac, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d472c[] = { func_ov021_02198998, func_ov021_02198978, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d47c8[] = { func_ov021_021989a4, func_ov021_02198978, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d46fc[] = { func_ov021_021989b0, func_ov021_02198978, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d46f0[] = { func_ov021_021989bc, func_ov021_02198978, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d46e4[] = { func_ov021_021989c8, func_ov021_02198978, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d46d8[] = { func_ov021_021989d4, func_ov021_02198978, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d46cc[] = { func_ov021_021989e0, func_ov021_02198978, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d46c0[] = { func_ov021_021989ec, func_ov021_02198a10, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d43d8[] = { func_ov021_02198a58, func_ov021_02198a8c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d41b0[] = { func_ov021_02198ad4, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4160[] = { func_ov021_02198ae8, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4190[] = { func_ov021_02198afc, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4168[] = { func_ov021_02198b10, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d41a0[] = { func_ov021_02198b24, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4188[] = { func_ov021_02198b38, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4660[] = { func_ov021_02198ba8, func_ov021_02198b80, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4654[] = { func_ov021_02198bb8, func_ov021_02198b80, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d43a8[] = { func_ov021_02198bd8, func_ov021_02198b80, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d463c[] = { func_ov021_02198be8, func_ov021_02198b80, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4630[] = { func_ov021_02198bf8, func_ov021_02198b80, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4624[] = { func_ov021_02198ca4, func_ov021_02198d24, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4618[] = { func_ov021_02198cb4, func_ov021_02198d24, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d460c[] = { func_ov021_02198cc4, func_ov021_02198d24, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4600[] = { func_ov021_02198cd4, func_ov021_02198d24, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d45f4[] = { func_ov021_02198ce4, func_ov021_02198d24, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4378[] = { func_ov021_02198cf4, func_ov021_02198d24, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d45dc[] = { func_ov021_02198d04, func_ov021_02198d24, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d45d0[] = { func_ov021_02198d14, func_ov021_02198d24, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d45c4[] = { func_ov021_02198d3c, func_ov021_02198d7c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d45b8[] = { func_ov021_02198d4c, func_ov021_02198d7c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d45ac[] = { func_ov021_02198d5c, func_ov021_02198d7c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d45a0[] = { func_ov021_02198d6c, func_ov021_02198d7c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4594[] = { func_ov021_02198260, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4588[] = { func_ov021_02198278, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d457c[] = { func_ov021_02198290, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4570[] = { func_ov021_021982a4, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4564[] = { func_ov021_02198d94, func_ov021_02198dd4, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4558[] = { func_ov021_02198da4, func_ov021_02198dd4, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d454c[] = { func_ov021_02198db4, func_ov021_02198dd4, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4540[] = { func_ov021_02198dc4, func_ov021_02198dd4, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4534[] = { func_ov021_02198dec, func_ov021_02198e2c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4528[] = { func_ov021_02198dfc, func_ov021_02198e2c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d451c[] = { func_ov021_02198e0c, func_ov021_02198e2c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4510[] = { func_ov021_02198e1c, func_ov021_02198e2c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4504[] = { func_ov021_02198e44, func_ov021_02198ec8, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d44f8[] = { func_ov021_02198e44, (FieldActorFunc)func_ov021_02198f20, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d44ec[] = { func_ov021_02198e60, func_ov021_02198ec8, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d44e0[] = { func_ov021_02198e60, (FieldActorFunc)func_ov021_02198f20, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d44d4[] = { func_ov021_02198e7c, func_ov021_02198ec8, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d44c8[] = { func_ov021_02198e7c, (FieldActorFunc)func_ov021_02198f20, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d44bc[] = { (FieldActorFunc)func_ov021_02198e98, func_ov021_02198ec8, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d44b0[] = { (FieldActorFunc)func_ov021_02198eb0, func_ov021_02198ec8, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d44a4[] = { func_ov021_02198f68, func_ov021_02198fb0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4498[] = { func_ov021_02199070, func_ov021_02199050, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d448c[] = { func_ov021_02199088, func_ov021_02199050, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4480[] = { func_ov021_021990a0, func_ov021_02199050, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4474[] = { func_ov021_02199234, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4414[] = { func_ov021_02199268, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4438[] = { func_ov021_02199298, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4444[] = { func_ov021_021992cc, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d445c[] = { func_ov021_021992fc, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4678[] = { func_ov021_0219932c, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d469c[] = { func_ov021_02199360, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d46a8[] = { func_ov021_02199394, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4720[] = { func_ov021_021993c8, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4a20[] = { func_ov021_021993fc, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d43fc[] = { func_ov021_02199430, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d43f0[] = { func_ov021_02199460, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d43e4[] = { func_ov021_02199490, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d427c[] = { func_ov021_021994c0, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d43cc[] = { func_ov021_021994f0, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d43c0[] = { func_ov021_02199520, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d466c[] = { func_ov021_02199550, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4264[] = { func_ov021_02199584, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d439c[] = { func_ov021_021995b8, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4390[] = { func_ov021_021995e8, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4384[] = { func_ov021_021988d0, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d424c[] = { func_ov021_021988f4, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d436c[] = { func_ov021_02198918, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4360[] = { func_ov021_0219893c, (FieldActorFunc)func_ov021_02198550, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4354[] = { func_ov021_02199618, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4348[] = { func_ov021_0219964c, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d433c[] = { func_ov021_02199680, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4330[] = { func_ov021_021996b4, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4324[] = { func_ov021_021996e8, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4318[] = { func_ov021_0219971c, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d430c[] = { func_ov021_02199750, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4300[] = { func_ov021_02199784, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d42f4[] = { func_ov021_021997b8, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d42e8[] = { func_ov021_021997ec, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d42c4[] = { func_ov021_02199820, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d42d0[] = { func_ov021_02199854, func_ov021_021991f0, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4408[] = { func_ov021_02199aec, func_ov021_02199970, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4420[] = { func_ov021_02199b10, func_ov021_02199970, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4450[] = { func_ov021_02199b38, func_ov021_02199970, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4684[] = { func_ov021_02199b60, func_ov021_02199970, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d46b4[] = { func_ov021_02199b88, func_ov021_02199970, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4a2c[] = { func_ov021_02199bac, func_ov021_02199970, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4288[] = { func_ov021_02199bd8, func_ov021_02199970, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d41c8[] = { func_ov021_02199c00, func_ov021_02199970, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4270[] = { func_ov021_02199c28, func_ov021_02199970, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d43b4[] = { func_ov021_02199c50, func_ov021_02199970, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4258[] = { func_ov021_02199c7c, func_ov021_02199970, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d41ec[] = { func_ov021_02199ca4, func_ov021_02199970, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4240[] = { func_ov021_02198bc8, func_ov021_02198b80, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4234[] = { func_ov021_021990b8, func_ov021_021990d4, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d421c[] = { func_ov021_021982bc, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4228[] = { func_ov021_021982d4, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d42a0[] = { func_ov021_021982ec, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d42ac[] = { func_ov021_02198300, func_ov021_0219800c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d42dc[] = { func_ov021_02199cc8, func_ov021_02199ce4, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d41a8[] = { func_ov021_02199d00, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4690[] = { func_ov021_02199d14, func_ov021_02199d4c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4a38[] = { func_ov021_02199d8c, func_ov021_02199db4, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d41b8[] = { func_ov021_02199eac, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d41d4[] = { func_ov021_02199ec0, func_ov021_02199ed4, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4178[] = { func_ov021_02199f30, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d41f8[] = { func_ov021_02199f50, func_ov021_02199f94, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d4294[] = { func_ov021_0219a014, (FieldActorFunc)func_ov021_0219a058, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d42b8[] = { func_ov021_0219a028, (FieldActorFunc)func_ov021_0219a07c, func_ov021_02197f5c };
const FieldActorFunc data_ov021_021d442c[] = { func_ov021_0219a03c, (FieldActorFunc)func_ov021_0219a07c, func_ov021_02197f5c };
static const s32 data_ov021_021d4a44[] = { 0x5000, 0x6000, 0x5000 };
static const fx32 data_ov021_021d4c44[] = {
#include "asset/fldmmdl_acmd_data_ov021_021d4c44.inc"
};
static const s32 data_ov021_021d4a68[] = {
#include "asset/fldmmdl_acmd_data_ov021_021d4a68.inc"
};
static const s32 data_ov021_021d4a9c[] = {
#include "asset/fldmmdl_acmd_data_ov021_021d4a9c.inc"
};
static const s32 data_ov021_021d4a80[] = {
#include "asset/fldmmdl_acmd_data_ov021_021d4a80.inc"
};
static const fx32 data_ov021_021d4c84[] = {
#include "asset/fldmmdl_acmd_data_ov021_021d4c84.inc"
};
static const fx32 data_ov021_021d4cc4[] = {
#include "asset/fldmmdl_acmd_data_ov021_021d4cc4.inc"
};
static const fx32 *data_ov021_021ddbd4[] = { data_ov021_021d4cc4, data_ov021_021d4c44, data_ov021_021d4c84 };
