// Black's code at ov021 0x021c0ca4-0x021c12c4. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_move_tcb.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "field/field_player_core.h"
#include "field/field_player_grid.h"
#include "struct_decls.h"
#include "field/ov021_021c12c4.h"
#include "system/game_event.h"
#include "nitro/mi.h"
#include "field/field_actor.h"
#include "field/player_action.h"
#include "field/field.h"
#include "field/field_player.h"
#include "field/field_map.h"
#include "field/encounter.h"
#include "gfl/sound.h"

GameEvent *FieldPlayerGrid_CheckObjContactEventCore(FieldPlayerGrid *grid, u16 dir, u32 flags);
GameEvent *FieldPlayerGrid_CheckStrengthRockPushEvent(FieldPlayerGrid *grid, u16 dir, s32 flags);
u32 StrengthRockSpaceCheck(FieldActor *actor, u16 dir, FieldG3DMapper *mapper);
GameEvent *EventStrengthRockPush_Create(FieldPlayerGrid *grid, u16 dir, FieldActor *actor1, int unk6);
GameEventReturnCode EventStrengthRockPush_Callback(GameEvent *event, u32 *state, void *work);
void FieldSurfTCB_Callback(TCB *tcb, void *work);

GameEvent *FieldPlayerGrid_CheckObjContactEvent(FieldPlayerGrid *grid, u32 pressedKeys, u32 heldKeys, u32 flags) {
    u16 dir;
    FieldPlayerCore *core;
    BOOL BVar1;
    GameEvent *pGVar2;

    core = FieldPlayerGrid_GetPlayerCore(grid);
    FieldPlayerGrid_GetPlayerActor(grid);
    dir = FieldPlayerCore_GetMoveDirByKey(core, heldKeys);
    BVar1 = FieldPlayerGrid_IsAcmdFinished(grid, dir);
    if (BVar1 == 0) {
        return (GameEvent *)0x0;
    }
    pGVar2 = FieldPlayerGrid_CheckObjContactEventCore(grid, dir, flags);
    if (pGVar2 == (GameEvent *)0x0) {
        pGVar2 = (GameEvent *)0x0;
    }
    return pGVar2;
}

GameEvent *FieldPlayerGrid_CheckObjContactEventCore(FieldPlayerGrid *grid, u16 dir, u32 flags) {
    GameEvent *event = NULL;
    u32 contType = FieldPlayerGrid_GetMoveDirContType(grid, dir);
    if (flags & 1) {
        event = FieldPlayerGrid_CheckStrengthRockPushEvent(grid, dir, contType);
    }
    return event;
}

u32 StrengthRockSpaceCheck(FieldActor *rock, u16 dir, FieldG3DMapper *mapper) {
    VecFx32 pos;
    VecFx32 tilePos;
    u32 tileType;
    u32 tileClass;
    u8 width;
    u8 j;
    BOOL hasFree;
    BOOL hasHole;
    u8 i;
    u8 height;

    CopyActorWPos(rock, &pos);
    ExpandVecInGridDir(dir, &pos, FX32_CONST(16));
    width = GetActorCollWidth(rock);
    height = GetActorCollHeight(rock);
    hasFree = FALSE;
    hasHole = FALSE;
    for (i = 0; i < height; i++, pos.z -= FX32_CONST(16)) {
        tilePos = pos;
        for (j = 0; j < width; j++, tilePos.x += FX32_CONST(16)) {
            tileType = GetTileTypeAtPos(mapper, &tilePos);
            tileClass = GetTileClass(tileType);
            if (MapTile_BlocksCollision(tileType)) {
                if (!MapTile_IsStrengthHole(tileClass)) {
                    return 1;
                }
                hasHole = TRUE;
            } else {
                hasFree = TRUE;
            }
        }
    }
    if (hasHole == TRUE) {
        if (hasFree == FALSE) {
            return 3;
        } else {
            return 2;
        }
    }
    return 0;
}


GameEvent *FieldPlayerGrid_CheckStrengthRockPushEvent(FieldPlayerGrid *grid, u16 dir, s32 flags) {
    FieldActor *actor;
    s32 result;
    s32 flag;
    u32 result2;

    if (flags == 1) {
        actor = FieldPlayerGrid_FindFrontActor(grid, dir);
        if ((actor != NULL) && (IsNPCStrengthRock(FldAct_GetObjCode(actor)) == 1)) {
            result2 = StrengthRockSpaceCheck(actor, dir, Field_GetG3DMapper(FieldPlayerCore_GetField(FieldPlayerGrid_GetPlayerCore(grid))));
            result = ActorRouteCollCheckOneTileInDir(actor, dir) & ~1;
            if ((2 & result) && ((u32) (result2 - 2) <= 1U)) {
                result &= ~2;
            }
            if (result == 0) {
                flag = 0;
                if (result2 == 3) {
                    flag = 1;
                }
                return EventStrengthRockPush_Create(grid, dir, actor, flag);
            }
        }
    }
    return NULL;
}


typedef struct FieldPlayerGridEvent {
    FieldPlayerGrid *grid;
    u16 dir;
    u16 unk_6;
    s16 counter;
    u16 unk_a;
    s32 height;
    VecFx32 pos;
    FieldActor *actor1;
    FieldActor *actor2;
} FieldPlayerGridEvent;

GameEvent *EventStrengthRockPush_Create(FieldPlayerGrid *grid, u16 dir, FieldActor *actor1, int unk6) {
    FieldPlayerCore *core;
    GameEvent *event;
    FieldPlayerGridEvent *data;

    core = FieldPlayerGrid_GetPlayerCore(grid);
    event = GameEvent_Create(FieldPlayerCore_GetGameSystem(core), NULL, EventStrengthRockPush_Callback, sizeof(FieldPlayerGridEvent));
    data = GameEvent_GetData(event);
    MI_CpuFill8(data, 0, sizeof(FieldPlayerGridEvent));
    data->grid = grid;
    data->dir = dir;
    data->unk_6 = unk6;
    data->actor1 = actor1;
    data->actor2 = FieldPlayerCore_GetActor(core);
    return event;
}

GameEventReturnCode EventStrengthRockPush_Callback(GameEvent *event, u32 *state, void *work) {
    FieldPlayerGridEvent *data = work;

    switch (*state) {
    case 0:
        if (IsAllActorAcmdFinished(data->actor1) == FALSE) {
            break;
        }
        if (IsAllActorAcmdFinished(data->actor2) == FALSE && func_ov021_021a5408(data->grid) == FALSE) {
            break;
        }
        FldAct_SetAcmd(data->actor1, GetAcmdForDir(data->dir, 0x8));
        FldAct_SetAcmd(data->actor2, GetAcmdForDir(data->dir, 0x20));
        GFL_SndSEPlay(0x674);
        (*state)++;
        break;
    case 1: {
        VecFx32 tmp;
        if (func_ov010_0216d454(data->actor1) == FALSE) {
            break;
        }
        if (func_ov010_0216d454(data->actor2) == FALSE) {
            break;
        }
        if (data->unk_6 == 0) {
            *state = 4;
            break;
        }
        CopyActorWPos(data->actor1, &tmp);
        GetHeightFromMap(data->actor1, &tmp, &data->height);
        if (FldAct_GetObjCode(data->actor1) == 0x6d) {
            data->height -= 0xe << 12;
        } else {
            data->height -= 0x1a << 12;
        }
        func_ov010_0216db4c(data->actor1, TRUE);
        func_ov010_0216dae0(data->actor1, FALSE);
        GFL_SndSEPlay(0x675);
        (*state)++;
        break;
    }
    case 2: {
        VecFx32 pos;
        CopyActorWPos(data->actor1, &pos);
        if (data->height < pos.y) {
            pos.y -= 0x4000;
            if (data->height > pos.y) {
                pos.y = data->height;
            }
        } else if (data->height > pos.y) {
            pos.y += 0x4000;
            if (data->height < pos.y) {
                pos.y = data->height;
            }
        }
        SetActorWPosValue(data->actor1, &pos);
        if (data->height == pos.y) {
            data->pos.y = 0x2000;
            (*state)++;
        }
        break;
    }
    case 3: {
        VecFx32 tmp;
        func_ov010_0216d8e0(data->actor1, &data->pos);
        data->pos.y = -data->pos.y;
        data->counter++;
        if (data->counter & 4) {
            if (data->pos.y < 0) {
                data->pos.y += 0x1000;
            } else if (data->pos.y > 0) {
                data->pos.y -= 0x1000;
            }
        }
        if (data->pos.y == 0) {
            CopyActorWPos(data->actor1, &tmp);
            SetActorWPosAll(data->actor1, &tmp, data->dir);
            func_ov010_0216d8e0(data->actor1, &data->pos);
            func_ov010_0216e57c(data->actor1);
            (*state)++;
        }
        break;
    }
    case 4:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}



typedef struct FieldSurfTCB {
    u16 state;
    s16 counter;
    u16 finished;
    u16 dir;
    u16 surfEdge;
    u16 unk_a;
    Field *field;
    FieldPlayer *player;
    FieldPlayerGrid *unk_14;
} FieldSurfTCB;

void *FieldSurfTCB_Create(FieldPlayer *player, u32 direction, u32 tileType, HeapID heapId) {
    FieldSurfTCB *data;

    data = GFL_HeapAllocate(heapId & 0x7fff | 0x8000, sizeof(FieldSurfTCB), TRUE, "field_player_grid_event.c", 0x1e6);
    data->field = FieldPlayer_GetField(player);
    data->dir = direction;
    data->player = player;
    data->unk_14 = func_ov021_021a36e0(player);
    if (MapTile_IsSurfEdge(GetTileClass(tileType)) == TRUE) {
        data->surfEdge = 1;
    }
    return GFL_TCBMgrAddTask(Field_GetTCBMgr(data->field), FieldSurfTCB_Callback, data, 0);
}




BOOL FieldSurfTCB_CheckEnd(void *tcb) {
    FieldSurfTCB *data = GFL_TCBGetData(tcb);

    return data->finished;
}

void FieldSurfTCB_Free(void *tcb) {
    void *ptr;

    ptr = GFL_TCBGetData(tcb);
    GFL_HeapFree(ptr);
    GFL_TCBRemove(tcb);
}

void FieldSurfTCB_Callback(TCB *tcb, void *work) {
    FieldSurfTCB *data = work;
    FieldActor *actor;
    u16 dir;
    s16 x;
    s16 z;
    VecFx32 pos;
    fx32 height;
    fx32 distance;
    FieldEffectTask *task;
    EncountSystem *encount;
    s16 gridPos[3];
    u16 acmd;

    actor = FieldPlayer_GetActor(data->player);
    switch (data->state) {
    case 0:
        dir = GetActorFaceDir(actor);
        x = GetGPosX(actor);
        z = GetGPosZ(actor);
        CopyActorWPos(actor, &pos);
        ConvGXZToVector(x, z, &pos);
        distance = 0x10000;
        if (data->surfEdge == 1) {
            distance = 0x20000;
        }
        ExpandVecInGridDir(dir, &pos, distance);
        height = 0;
        GetHeightFromMap(actor, &pos, &height);
        pos.y = height;
        task = func_ov021_021acb38(Field_GetFieldEffects(data->field), dir, &pos, actor, 0);
        func_ov021_021a2f8c(data->player, task);
        encount = Field_GetEncountSystem(data->field);
        gridPos[0] = (pos.x >> 4) / 0x1000;
        gridPos[1] = (pos.y >> 4) / 0x1000;
        gridPos[2] = (pos.z >> 4) / 0x1000;
        if (EncountState_CheckSpecialEncountPos(encount, gridPos)) {
            func_ov021_021aab1c(encount, 1);
        }
        data->state++;
        break;
    case 1:
        data->counter++;
        if (data->counter > 15) {
            data->state++;
        }
        break;
    case 2:
        dir = GetActorFaceDir(actor);
        acmd = GetAcmdForDir(dir, 0x34);
        if (data->surfEdge == 1) {
            acmd = GetAcmdForDir(dir, 0x38);
        }
        FldAct_SetAcmd(actor, acmd);
        data->state++;
        break;
    case 3:
        if (func_ov010_0216d454(actor) == 0) {
            break;
        }
        func_ov010_0216d480(actor);
        task = func_ov021_021a2f98(data->player);
        func_ov021_021acbb8(task, 1);
        FieldPlayer_SetSpecialSeq(data->player, 4);
        data->state++;
        /* fall through */
    case 4:
        if (func_ov021_021a2f54(data->player) == 1) {
            func_ov021_021a35a0(data->player, 1);
            data->finished = 1;
            data->state++;
        }
        break;
    case 5:
        break;
    }
}


typedef struct FieldWaterfallTCB {
    u16 flag;
    u16 unk_2;
    u16 dir;
    u32 unk_8[13];
    FieldPlayer *player;
    Field *field;
    FieldPlayerGrid *unk_44;
    u32 unk_48;
} FieldWaterfallTCB;
void *FieldWaterfallTCB_Create(FieldPlayer *player, u32 direction, u32 param, HeapID heapId) {
    FieldWaterfallTCB *data;
    FieldActor *actor;
    VecFx32 pos;
    VecFx32 expanded;
    fx32 height;

    data = GFL_HeapAllocate(heapId & 0x7fff | 0x8000, sizeof(FieldWaterfallTCB), TRUE, "field_player_grid_event.c", 0x2b3);
    data->dir = direction;
    data->player = player;
    data->field = FieldPlayer_GetField(player);
    actor = FieldPlayer_GetActor(player);
    CopyActorWPos(actor, &pos);
    expanded = pos;
    ExpandVecInGridDir(data->dir, &expanded, 0x20000);
    GetHeightFromMap(actor, &expanded, &height);
    if (height > pos.y) {
        data->flag = 1;
    }
    data->unk_44 = func_ov021_021a36e0(data->player);
    return GFL_TCBMgrAddTask(Field_GetTCBMgr(data->field), FieldWaterfallTCB_Callback, data, 0);
}

