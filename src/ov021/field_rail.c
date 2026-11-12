// Black's code at ov021 0x021b7884-0x021bb088. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_rail.h"
#include "field/field_actor_internal.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/math.h"
#include "nitro/mi.h"
#include "save/mystery_gift.h"
#include "struct_decls.h"
#include "field/field.h"
#include "gfl/bg_sys.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "nitro/fs.h"
#include "system/wordset.h"
#include "field/calender.h"
#include "field/city_state.h"
#include "field/encounter.h"
#include "field/event_data.h"
#include "field/field_map.h"
#include "field/field_status.h"
#include "field/pdw_postman.h"
#include "field/player_state.h"
#include "field/pleasure_boat.h"
#include "field/trial_house.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/config.h"
#include "save/dream_world.h"
#include "save/event_work.h"
#include "save/gimmick_savedata.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/player_savedata.h"
#include "save/playtime.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "system/game_data.h"
#include "system/shortcut_util.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "field/field_camera.h"
#include "gfl/calctool.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/key.h"
#include "gfl/sound.h"
#include "gfl/std.h"
// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they
// belong in the library's header
enum {
    RAIL_DIR_NONE,
    RAIL_DIR_UP,
    RAIL_DIR_RIGHT,
    RAIL_DIR_DOWN,
    RAIL_DIR_LEFT,
};

RailPoint *GetRailPoint(RailDataHandle *handle, u32 index);
RailLine *GetRailLine(RailDataHandle *handle, u32 index);
RailCamera *func_ov021_021b91c8(RailUnit *unit);
RailCamera *GetRailCamera(RailDataHandle *handle, u32 index);
void RailUnit_SetDataCore(RailUnit *unit, const RailDataHandle *data, FieldRailSystem *rail);
RailCurve *GetRailCurve(RailDataHandle *handle, u32 index);
void func_ov021_021b91f8(RailUnit *unit, VecFx32 *pos);
s32 func_ov021_021b97d0(RailDataHandle *handle, const RailPoint *point);
s32 func_ov021_021b97fc(const RailDataHandle *handle, const RailLine *line);
s32 GetLerpedLineWidthByLineYPos(const RailLine *line, u32 y, s32 length, const RailDataHandle *data);

const RailCamera data_ov021_021d9a0c = { 0xFFFF };

// pokebw2's sPendingSteps
static u8 data_ov021_021de298[5] = { 1, 2, 4, 8, 16 };
// pokebw2's sUp
static const VecFx32 data_ov021_021d9a00;
extern const RailCurve data_ov021_021d9a30;
void func_ov021_021b9464(RailDisabledLines *disabled);
void func_ov021_021b87c8(void *param_1, void *param_2);
void func_ov021_021b87bc(void *param_1);
u16 RailUnit_GetState(RailUnit *unit);
u32 func_ov021_021b7de0(RailUnit *unit, const RailPosition *pos);
u32 func_ov021_021b960c(RailPoint *point, RailLine *line, RailDataHandle *handle);
u32 func_ov021_021b93d4(u32 param_1);
void func_ov021_021ba2b4(FieldRailSystem *rail);
void func_ov021_021ba348(FieldRailSystem *rail);
void func_ov021_021bb014(FieldRailSystem *param_1, RailUnit *param_2, RailPoint *param_3);
void func_ov021_021bac04(FieldRailSystem *rail);
void RailUnit_SetData(RailUnit *unit, RailDataHandle *data);
s32 GetLineTileLengthWorldSpace(const RailLine *line, u32 tileWidth, const RailDataHandle *handle);
void RailUnit_SetLineInfo(RailUnit *unit, RailLine *line, u32 dir, u32 front, u32 side, u32 length, u32 width);
RailCameraFunc GetRailCameraCurveFunc(RailDataHandle *handle, u32 index);
RailLine *func_ov021_021b9510(RailDisabledLines *disabled, RailDataHandle *handle, u32 lineId);
BOOL func_ov021_021b9828(const VecFx32 *a, const VecFx32 *b, const VecFx32 *p3, const VecFx32 *p4, const VecFx32 *p5, VecFx32 *out);
WorldToRailFunc GetHandler2CurveFunc(RailDataHandle *handle, u32 index);
void func_ov021_021b9238(const RailUnit *unit, RailPosition *pos);
u32 func_ov021_021b9368(FieldRailSystem *, RailPosition *, u32, s32, VecFx32 *);

BOOL func_ov021_021b94d8(RailDisabledLines *disabled, const RailLine *line, const RailDataHandle *handle);
RailLine *GetLineContinueInDirection(const RailPoint *point, u32 dir, RailDataHandle *handle, RailDisabledLines *disabled, s32 checkDisabled);
void SetupLineWidthForLineChange(RailDataHandle *data, const RailLine *line, s32 *outNext, s32 *outCur, RailDisabledLines *disabled, u32 tileWidth);
void func_ov021_021b9a50(VecFx32 *out, const VecFx32 *a, const VecFx32 *b, fx32 t);
FieldCamera *FieldRailSystem_GetFieldCamera(FieldRailSystem *rail);
RailUnit *FieldRailSystem_GetCameraParentUnit(FieldRailSystem *rail);
void FieldRailSystem_GetCameraParentPos(FieldRailSystem *rail, VecFx32 *pos);

void func_ov021_021ba7e0(FieldRailSystem *rail, u16 angle, s32 dist, VecFx32 *target);
BOOL func_ov021_021b95c0(RailDisabledLines *disabled, RailLine *line, s32 front, s32 side, RailDataHandle *data, u32 tileWidth);
u32 func_ov021_021b963c(RailLine *line, RailDataHandle *handle);
u32 func_ov021_021b91ac(RailUnit *unit);
s32 func_ov021_021b88dc(RailUnit *m, u32 dir, s32 step);
void func_ov021_021b977c(RailUnit *unit);
u32 func_ov021_021b9434(u32 param_1);
u32 func_ov021_021b9404(u32 param_1);
u8 func_ov021_021b8048(RailUnit *unit);
void func_ov021_021b9530(RailDisabledLines *param_1, RailUnit *param_2, s32 param_3, s32 param_4, s32 param_5);
u32 func_ov021_021b96ac(RailPoint *param_1, RailDataHandle *param_2);
u32 func_ov021_021b9338(RailUnit *unit, RailLine *param_2, s32 param_3, u32 param_4);

FieldRailSystem *FieldRailSystem_Create(HeapID heapId, u32 unitCount, FieldCamera *camera) {
    u32 i;
    FieldRailSystem *pFVar1;
    RailUnit *pRVar2;

    pFVar1 = GFL_HeapAllocate(heapId, sizeof(FieldRailSystem), 1, "field_rail.c", 0x12e);
    pFVar1->heapId = heapId;
    i = 0;
    pFVar1->loaded = 0;
    pFVar1->cameraEnabled = 1;
    pFVar1->camera = camera;
    pFVar1->cameraParent = (RailUnit *)0x0;
    pRVar2 = GFL_HeapAllocate(heapId, unitCount * sizeof(RailUnit), 1, "field_rail.c", 0x137);
    pFVar1->units = pRVar2;
    pFVar1->unitCount = unitCount;
    while (i < unitCount) {
        RailUnit_SetDataCore(pFVar1->units + i, &pFVar1->data, pFVar1);
        i = i + 1;
    }
    pRVar2 = GFL_HeapAllocate(heapId, sizeof(RailUnit), 1, "field_rail.c", 0x13f);
    pFVar1->playerUnit = pRVar2;
    RailUnit_SetDataCore(pRVar2, &pFVar1->data, pFVar1);
    pFVar1->playerUnit->acquired = 1;
    return pFVar1;
}

void func_ov021_021b791c(FieldRailSystem *rail) {
    GFL_HeapFree(rail->playerUnit);
    GFL_HeapFree(rail->units);
    GFL_HeapFree(rail);
}

void FieldRailSystem_Load(FieldRailSystem *rail, RailDataHandle *data) {
    rail->loaded = 1;
    func_ov021_021b9464(&rail->disabledLines);
    func_ov021_021b87c8(&rail->data, data);
}

void func_ov021_021b7954(FieldRailSystem *rail) {
    rail->loaded = 0;
    func_ov021_021b87bc(&rail->data);
    func_ov021_021b9464(&rail->disabledLines);
}

RailUnit *FieldRailSystem_AcquireUnit(FieldRailSystem *rail) {
    s32 i;
    RailUnit *pRStack_18;

    pRStack_18 = (RailUnit *)0x0;
    i = 0;
    if (0 < (s32)rail->unitCount) {
        do {
            if (RailUnit_GetState(rail->units + i) == 0) {
                pRStack_18 = rail->units + i;
                break;
            }
            i = i + 1;
        } while (i < (s32)rail->unitCount);
    }
    pRStack_18->acquired = 1;
    return pRStack_18;
}

void FieldRailSystem_ReleaseUnit(FieldRailSystem *rail, RailUnit *unit) {
    RailUnit_SetDataCore(unit, &rail->data, rail);
    unit->acquired = 0;
}

void FieldRailSystem_SetCameraParent(FieldRailSystem *rail, RailUnit *unit) {
    rail->cameraParent = unit;
    rail->cameraDirty = 1;
}

void FieldRailSystem_ClearCameraParent(FieldRailSystem *rail) {
    rail->cameraParent = (RailUnit *)0x0;
}

void FieldRailSystem_GetCameraParentPos(FieldRailSystem *rail, VecFx32 *pos) {
    if (rail->cameraParent != NULL) {
        RailUnit_GetCalcPos(rail->cameraParent, pos);
    }
}

void func_ov021_021b79e4(FieldRailSystem *rail) {
    if (rail->loaded == 0) {
        return;
    }
    if (rail->cameraEnabled == 0) {
        return;
    }
}

BOOL FieldRailSystem_UpdateCamera(FieldRailSystem *rail, BOOL force) {
    RailUnit *unit = rail->cameraParent;
    RailCameraFunc func = NULL;
    u32 cameraId;

    if (rail->loaded == 0) {
        return FALSE;
    }
    if (rail->cameraEnabled == 0) {
        return FALSE;
    }
    if (unit == NULL) {
        return FALSE;
    }
    if (!func_ov021_021b91ac(unit)) {
        return FALSE;
    }
    if (!func_ov021_021b8048(unit) && rail->cameraDirty == 0 && !force) {
        return FALSE;
    }
    switch (unit->onLine) {
    case 0:
        cameraId = unit->component.point->cameraId;
        if (cameraId != 0xffff) {
            func = GetRailCameraCurveFunc(unit->handle, GetRailCamera(unit->handle, cameraId)->funcId);
        }
        break;
    case 1:
        cameraId = unit->component.line->cameraId;
        if (cameraId != 0xffff) {
            func = GetRailCameraCurveFunc(unit->handle, GetRailCamera(unit->handle, cameraId)->funcId);
        }
        break;
    }
    if (func != NULL) {
        func(rail);
    }
    rail->cameraDirty = 0;
    return TRUE;
}

static inline RailDisabledLines *FieldRailSystem_GetDisabledLines(FieldRailSystem *rail) {
    return &rail->disabledLines;
}

BOOL CalculateRailCurves(FieldRailSystem *rail, const VecFx32 *start, const VecFx32 *end, RailPosition *railPos,
                         VecFx32 *hit) {
    VecFx32 point;
    VecFx32 diff;
    VecFx32 cross;
    VecFx32 up = data_ov021_021d9a00;
    VecFx32 bestHit;
    VecFx32 startPos = *start;
    VecFx32 endPos = *end;
    RailPosition bestPos;
    s32 i;
    s32 bestDist;
    BOOL found = FALSE;

    for (i = 0; i < rail->data.lineCount; i++) {
        RailLine *link = func_ov021_021b9510(&rail->disabledLines, &rail->data, i);
        RailPoint *a;
        RailPoint *b;
        RailCurve *info;
        WorldToRailFunc callback;

        if (link == NULL) {
            continue;
        }
        a = GetRailPoint(&rail->data, link->pointA);
        b = GetRailPoint(&rail->data, link->pointB);
        info = GetRailCurve(&rail->data, link->curveId);
        VEC_Subtract(&a->pos, &b->pos, &diff);
        VEC_CrossProduct(&diff, &up, &cross);
        if (!func_ov021_021b9828(&diff, &cross, &b->pos, &startPos, &endPos, &point)) {
            continue;
        }
        callback = GetHandler2CurveFunc(&rail->data, info->handlerId);
        if (callback == NULL) {
            continue;
        }
        if (!callback(i, rail, &point, railPos, hit)) {
            continue;
        }
        if (!found) {
            bestPos = *railPos;
            bestHit = *hit;
            bestDist = VEC_Distance(&startPos, hit);
            found = TRUE;
        } else {
            s32 dist = VEC_Distance(&startPos, hit);

            if (dist < bestDist) {
                bestPos = *railPos;
                bestHit = *hit;
                bestDist = dist;
            }
        }
    }
    if (found) {
        *railPos = bestPos;
        *hit = bestHit;
    }
    return found;
}

u32 func_ov021_021b94c0(RailDisabledLines *disabled, u16 lineId);
void func_ov021_021b9478(RailDisabledLines *disabled, u16 lineId);
void func_ov021_021b949c(RailDisabledLines *disabled, u16 lineId);
void func_ov021_021b7c04(FieldRailSystem *rail, u32 lineId, BOOL enabled) {
    if (enabled == 0) {
        if (func_ov021_021b94c0(&rail->disabledLines, lineId) == 0) {
            func_ov021_021b9478(&rail->disabledLines, lineId);
        }
    } else {
        func_ov021_021b949c(&rail->disabledLines, lineId);
    }
}

fx32 func_ov021_021b7c38(FieldRailSystem *rail) {
    s64 value = (s64)(s32)rail->data.tileWidth << 16;
    return (value + 0x800) >> 12;
}

void FieldRailSystem_NormalizePos(FieldRailSystem *rail, RailDataHandle *data, u16 componentId, u16 front, u16 side, RailPosition *pos) {
    RailUnit *unit = rail->playerUnit;

    unit->handle = data;
    pos->componentId = componentId;
    pos->componentIsLine = TRUE;
    pos->posSide = 0;
    pos->posFront = front;
    RailUnit_SetRailPos(unit, pos);
    pos->posSide = side - (s32)unit->width / 16;
    unit->handle = &rail->data;
}

u32 func_ov021_021b7c90(FieldRailSystem *rail, RailPosition *pos, u32 dir, VecFx32 *out) {
    RailUnit_SetRailPos(rail->playerUnit, pos);
    func_ov021_021b80e0(rail->playerUnit, 4, dir);
    func_ov021_021b810c(rail->playerUnit);
    RailUnit_GetCalcPos(rail->playerUnit, out);
    if (GFL_STD_MemCmp(pos, &rail->playerUnit->railPos, 8) == 0) {
        return 0;
    }
    return func_ov021_021b7de0(rail->playerUnit, &rail->playerUnit->railPos);
}

BOOL func_ov021_021b7cd8(FieldRailSystem *rail, const RailPosition *railPos, u32 railDir, RailPosition *next) {
    RailPosition saved = *railPos;

    RailUnit_SetRailPos(rail->playerUnit, railPos);
    func_ov021_021b80e0(rail->playerUnit, 4, railDir);
    func_ov021_021b810c(rail->playerUnit);
    func_ov021_021b7dc0(rail->playerUnit, next);
    if (GFL_STD_MemCmp(&saved, next, 8) == 0) {
        return FALSE;
    }
    return func_ov021_021b7de0(rail->playerUnit, next);
}

void func_ov021_021b7d38(FieldRailSystem *rail, const RailPosition *railPos, VecFx32 *pos) {
    RailUnit_SetRailPos(rail->playerUnit, railPos);
    RailUnit_GetCalcPos(rail->playerUnit, pos);
}

s32 func_ov021_021b7d50(FieldRailSystem *rail, const RailPosition *pos) {
    u32 width;

    RailUnit_SetRailPos(rail->playerUnit, pos);
    width = rail->playerUnit->width;
    return (s32)(width + ((u32)((s32)width >> 3) >> 0x1c)) >> 4;
}

void FieldRailSystem_SetPlayerPos(FieldRailSystem *rail, const RailPosition *pos) {
    RailCameraFunc callback = NULL;
    RailUnit *prev;
    RailUnit *unit;
    u32 index;

    RailUnit_SetRailPos(rail->playerUnit, pos);
    prev = rail->cameraParent;
    unit = rail->playerUnit;
    rail->cameraParent = unit;
    switch (unit->onLine) {
    case 0:
        index = unit->component.point->cameraId;
        if (index != 0xFFFF) {
            callback = GetRailCameraCurveFunc(unit->handle, GetRailCamera(unit->handle, index)->funcId);
        }
        break;
    case 1:
        index = unit->component.line->cameraId;
        if (index != 0xFFFF) {
            callback = GetRailCameraCurveFunc(unit->handle, GetRailCamera(unit->handle, index)->funcId);
        }
        break;
    }
    if (callback != NULL) {
        callback(rail);
    }
    rail->cameraParent = prev;
}

void func_ov021_021b7dc0(const RailUnit *unit, RailPosition *pos) {
    *pos = unit->railPos;
}

u32 func_ov021_021b7de0(RailUnit *unit, const RailPosition *pos) {
    RailDataHandle *data;

    if (pos->componentIsLine == 0) {
        if (pos->componentId >= unit->handle->pointCount) {
            return 0;
        }
    } else {
        s32 front;
        s32 side;
        s32 span;
        s32 limit;
        RailLine *line;

        data = unit->handle;
        if (pos->componentId >= data->lineCount) {
            return 0;
        }
        line = &data->lines[pos->componentId];
        front = pos->posFront << 4;
        side = pos->posSide << 4;
        span = GetLineTileLengthWorldSpace(line, unit->tileWidth, data);
        limit = GetLerpedLineWidthByLineYPos(line, front, span, unit->handle);
        if (-limit > side || side > limit) {
            return 0;
        }
        if (front > span || front < 0) {
            return 0;
        }
    }
    return 1;
}

u8 func_ov021_021b7e60(RailUnit *unit, u32 dir, RailPosition *outPos, VecFx32 *outVec) {
    if (unit->nextCached == 0 || dir != unit->nextDir) {
        unit->nextOk = func_ov021_021b7cd8(unit->rail, &unit->railPos, dir, &unit->nextPos);
        func_ov021_021b7d38(unit->rail, &unit->nextPos, &unit->nextVec);
        unit->nextCached = 1;
        unit->nextDir = dir;
    }
    if (outPos != NULL) {
        MI_CpuCopy8(&unit->nextPos, outPos, 8);
    }
    if (outVec != NULL) {
        *outVec = unit->nextVec;
    }
    return unit->nextOk;
}

void func_ov021_021b7ed4(FieldActor *actor, VecFx32 *dest) {
    *dest = actor->wPos;
}

void RailUnit_SetRailPos(RailUnit *unit, const RailPosition *pos) {
    s32 i;
    RailLine *line = NULL;
    u32 front;
    s32 side;
    u32 dir;
    s32 result;
    s32 span;
    u16 v0, v1, v2, v3;

    RailUnit_SetData(unit, unit->handle);
    if (pos->componentIsLine == 0) {
        unit->onLine = 0;
        unit->component.point = unit->handle->points + pos->componentId;
        unit->width = unit->component.point->widths[0];
        for (i = 0; i < 4; i++) {
            s32 index = unit->component.point->lineIds[i];

            if (index != 0xFFFF && line == NULL) {
                line = &unit->handle->lines[index];
                if (line->pointA == pos->componentId) {
                    front = 0;
                    side = 0;
                    dir = unit->component.point->dirs[i];
                } else {
                    if (line->pointB != pos->componentId) {
                        continue;
                    }
                    front = GetLineTileLengthWorldSpace(line, unit->tileWidth, unit->handle);
                    side = 0;
                    dir = unit->component.point->dirs[i];
                }
            }
        }
    } else {
        u32 f;

        line = &unit->handle->lines[pos->componentId];
        f = pos->posFront;
        if (f >= line->length) {
            f = f - 1;
        }
        front = f << 4;
        side = pos->posSide << 4;
        dir = pos->railDirection;
    }
    span = GetLineTileLengthWorldSpace(line, unit->tileWidth, unit->handle);
    result = GetLerpedLineWidthByLineYPos(line, front, span, unit->handle);
    RailUnit_SetLineInfo(unit, line, dir, front, side, span, result);
    unit->unk_33 = 1;
    unit->unk_32 = dir;
    unit->active = 1;
    func_ov021_021b91f8(unit, &unit->calcPos);
    v0 = pos->componentId;
    v1 = pos->lineAndDirection;
    unit->railPosHalves[0] = v0;
    unit->railPosHalves[1] = v1;
    v2 = pos->posSide;
    unit->railPosHalves[2] = v2;
    v3 = pos->posFront;
    unit->railPosHalves[3] = v3;
    unit->prevRailPosHalves[0] = v0;
    unit->prevRailPosHalves[1] = v1;
    unit->prevRailPosHalves[2] = v2;
    unit->prevRailPosHalves[3] = v3;
    unit->prevCalcPos = unit->calcPos;
}

void RailUnit_GetCalcPos(RailUnit *unit, VecFx32 *dest) {
    *dest = unit->calcPos;
}

u8 func_ov021_021b8038(RailUnit *unit) {
    return unit->unk_32;
}

u8 func_ov021_021b8040(RailUnit *unit) {
    return unit->pending;
}

u8 func_ov021_021b8048(RailUnit *unit) {
    return unit->unk_34;
}

void func_ov021_021b8050(RailUnit *unit, s16 *out) {
    RailPosition tmp;
    VecFx32 start;
    VecFx32 next;
    u32 savedZ = unit->side;
    u32 base;
    u32 limit;
    u32 target;
    s32 x, y, z;

    unit->side = 0;
    func_ov021_021b91f8(unit, &start);
    base = unit->front;
    unit->side = savedZ;
    target = base + 0x10;
    limit = unit->length;
    if ((s32)target < (s32)limit) {
        unit->front = unit->front + 0x10;
        unit->side = 0;
        func_ov021_021b91f8(unit, &next);
        unit->side = savedZ;
        unit->front = base;
    } else {
        func_ov021_021b9238(unit, &tmp);
        tmp.posSide = 0;
        z = target;
        func_ov021_021b9368(unit->rail, &tmp, unit->component.line->dir, z - limit, &next);
    }
    VEC_Subtract(&next, &start, &next);
    VEC_Normalize(&next, &next);
    z = next.z;
    y = next.y;
    x = next.x;
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

u32 func_ov021_021b80d8(RailUnit *unit) {
    return unit->component.line->dir;
}

BOOL func_ov021_021b80e0(RailUnit *unit, u32 param_2, u32 param_3) {
    if (unit->pending == 0) {
        unit->pending = 1;
        unit->counter = 0;
        unit->pendingArgB = param_3;
        unit->pendingArgA = param_2;
        return TRUE;
    }
    return FALSE;
}

void func_ov021_021b810c(RailUnit *unit) {
    FieldRailSystem *rail;
    u16 onLine;
    u16 pendingArgB;
    u8 step;
    u32 moved;
    s32 rem;

    rail = unit->rail;
    if (rail->loaded == 0 || unit->active == 0 || !func_ov021_021b91ac(unit)) {
        return;
    }
    unit->unk_34 = 0;
    if (unit->pending == 0) {
        return;
    }
    onLine = unit->onLine;
    pendingArgB = unit->pendingArgB;
    step = data_ov021_021de298[unit->pendingArgA];
    unit->counter += (s8)step;
    if (onLine == 1) {
        moved = func_ov021_021b88dc(unit, pendingArgB, step);
    } else {
        moved = 0;
    }
    if (moved != 0) {
        unit->unk_32 = moved;
        unit->unk_33 = unit->pendingArgA;
        unit->unk_34 = 1;
        func_ov021_021b91f8(unit, &unit->calcPos);
    }
    if (onLine != unit->onLine) {
        func_ov021_021b977c(unit);
    }
    if (unit->counter < 16 && moved != 0) {
        return;
    }
    unit->pending = 0;
    unit->nextCached = 0;
    unit->prevRailPos = unit->railPos;
    func_ov021_021b9238(unit, &unit->railPos);
    unit->prevCalcPos = unit->calcPos;
    rem = (s32)unit->front % 16;
    if (rem == 0) {
        return;
    }
    if (rem > 8) {
        unit->front += 16 - rem;
    } else {
        unit->front -= rem;
    }
}

FieldCamera *FieldRailSystem_GetFieldCamera(FieldRailSystem *rail) {
    return rail->camera;
}

RailUnit *FieldRailSystem_GetCameraParentUnit(FieldRailSystem *rail) {
    return rail->cameraParent;
}

RailPoint *FieldRailSystem_GetPoint(FieldRailSystem *rail, u32 index) {
    return GetRailPoint(&rail->data, index);
}

RailLine *FieldRailSystem_GetLine(FieldRailSystem *rail, u32 index) {
    return GetRailLine(&rail->data, index);
}

RailCurve *GetRailHnd2Curve(FieldRailSystem *rail, u32 index) {
    return GetRailCurve(&rail->data, index);
}

fx32 FieldRailSystem_GetTileWidth(FieldRailSystem *rail) {
    return rail->data.tileWidth;
}

RailUnit *func_ov021_021b824c(FieldRailSystem *rail) {
    return rail->playerUnit;
}

void func_ov021_021b8250(const FieldRailSystem *rail, const RailPosition *in, RailPosition *out) {
    RailPosition pos = *in;
    RailLine *lines = rail->data.lines;
    RailLine *line;
    s32 front;
    s32 side;
    s32 length;
    s32 width;

    *out = pos;
    line = lines + pos.componentId;
    front = pos.posFront * 16;
    side = pos.posSide * 16;
    length = GetLineTileLengthWorldSpace(line, rail->data.tileWidth, &rail->data);
    width = GetLerpedLineWidthByLineYPos(line, front, length, &rail->data);
    if (front >= length) {
        out->posFront = (length - 1) / 16;
    }
    if ((side < 0 ? -side : side) > width) {
        if (side > 0) {
            out->posSide = width / 16;
        } else {
            out->posSide = -width / 16;
        }
    }
}

u16 RailUnit_IsOnLine(const RailUnit *unit) {
    return unit->onLine;
}

RailLine *GetCurrentLine(const RailUnit *unit) {
    return unit->component.line;
}

u32 GetCurrentLinePosV(const RailUnit *unit) {
    return unit->front;
}

u32 GetCurrentLineLength(const RailUnit *unit) {
    return unit->length;
}

u32 GetCurrentLinePosH(const RailUnit *unit) {
    return unit->side;
}

u32 func_ov021_021b8304(RailUnit *unit) {
    return unit->width;
}

u32 RailUnit_GetTileWidth(const RailUnit *unit) {
    return unit->tileWidth;
}

RailCamera *GetRailCameraByCamHandler(RailUnit *param_1) {
    return func_ov021_021b91c8(param_1);
}

void func_ov021_021b8314(RailUnit *unit, VecFx32 *param) {
    u32 unk_14 = unit->side;
    unit->side = 0;
    func_ov021_021b91f8(unit, param);
    unit->side = unk_14;
}

void func_ov021_021b8328(RailUnit *unit, u32 value, VecFx32 *param) {
    u32 unk_0C = unit->front;
    unit->front = value;
    func_ov021_021b91f8(unit, param);
    unit->front = unk_0C;
}

RailPoint *GetCurrentRailLinePoint1(RailUnit *unit) {
    return GetRailPoint(unit->handle, unit->component.line->pointA);
}

RailPoint *GetCurrentRailLinePoint2(RailUnit *unit) {
    return GetRailPoint(unit->handle, unit->component.line->pointB);
}

RailCurve *RailUnit_GetCurve(RailUnit *unit) {
    return GetRailCurve(unit->handle, unit->component.line->curveId);
}

RailCamera *GetRailCameraByPoint(RailUnit *unit, RailPoint *point) {
    return GetRailCamera(unit->handle, point->cameraId);
}

s32 func_ov021_021b8378(s32 value) {
    u32 abs = -value;

    if (value >= 0) {
        abs = value;
    }
    if ((abs & 0xF) >= 8) {
        if (value >= 0) {
            return value / 16 + 1;
        }
        return value / 16 - 1;
    }
    return value / 16;
}

BOOL func_ov021_021b83b0(const VecFx32 *a, const VecFx32 *b, fx32 dist) {
    s32 dx = (a->x >> 12) - (b->x >> 12);
    s32 dz = (a->z >> 12) - (b->z >> 12);
    s32 distSq = dx * dx + dz * dz;
    s32 dy = a->y - b->y;
    s32 radius;
    s32 radiusSq;

    if (dy < 0) {
        dy = -dy;
    }
    radius = (dist >> 12) * 2;
    radiusSq = radius * radius;
    if (dy < 0x8000 && distSq < radiusSq) {
        return TRUE;
    }
    return FALSE;
}

void SetWkToActorRailPos(RailUnit *unit, u16 *a1, u16 *a2, u16 *a3) {
    s32 len;
    s32 other;
    s32 id;
    s32 i;

    len = GetLerpedLineWidthByLineYPos(unit->component.line, 0, unit->length, unit->handle);
    other = GetLerpedLineWidthByLineYPos(unit->component.line, unit->length, unit->length, unit->handle);
    if (len < other) {
        len = other;
    }
    id = 0;
    for (i = 0; i < unit->handle->lineCount; i++) {
        if (unit->component.line == &unit->handle->lines[i]) {
            id = i;
        }
    }
    *a1 = id;
    *a2 = unit->front / 16;
    *a3 = unit->side / 16 + len / 16;
}

void func_ov021_021b8484(RailUnit *unit, u16 a1, u16 a2, u16 a3) {
    RailPosition pos;

    func_ov021_021b7dc0(unit, &pos);
    pos.componentId = a1;
    pos.posSide = 0;
    pos.posFront = a2;
    RailUnit_SetRailPos(unit, &pos);
    pos.posSide = a3 - (s32)unit->width / 16;
    func_ov021_021b91f8(unit, &unit->calcPos);
    unit->railPos = pos;
    unit->prevRailPos = pos;
    unit->prevCalcPos = unit->calcPos;
}

RailLine *GetLineContinueInDirection(const RailPoint *point, u32 dir, RailDataHandle *handle, RailDisabledLines *disabled, s32 checkDisabled) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (dir == point->dirs[i]) {
            if (checkDisabled) {
                return func_ov021_021b9510(disabled, handle, point->lineIds[i]);
            }
            return GetRailLine(handle, point->lineIds[i]);
        }
    }
    return NULL;
}

u32 func_ov021_021b854c(RailUnit *unit, RailLine *line, u32 dir, s32 front, s32 side, s32 length, s32 width) {
    RailLine *next;
    RailPoint *point;
    u32 lineDir;

    if (!func_ov021_021b95c0(unit->disabled, line, front, side, unit->handle, unit->tileWidth)) {
        return 0;
    }
    if ((side < 0 ? -side : side) > width) {
        return 0;
    }
    if (front >= length || front < 0) {
        if (front >= length) {
            point = GetRailPoint(unit->handle, line->pointB);
            lineDir = line->dir;
            next = GetLineContinueInDirection(point, lineDir, unit->handle, unit->disabled, TRUE);
        } else {
            point = GetRailPoint(unit->handle, line->pointA);
            lineDir = func_ov021_021b93d4(line->dir);
            next = GetLineContinueInDirection(point, lineDir, unit->handle, unit->disabled, TRUE);
        }
        if (next == NULL) {
            if (front > length) {
                return 0;
            }
        } else {
            line = next;
            if (front >= length) {
                front -= length;
                length = GetLineTileLengthWorldSpace(line, unit->tileWidth, unit->handle);
                width = GetLerpedLineWidthByLineYPos(line, front, length, unit->handle);
            } else if (front < 0) {
                length = GetLineTileLengthWorldSpace(line, unit->tileWidth, unit->handle);
                front += length;
                width = GetLerpedLineWidthByLineYPos(line, front, length, unit->handle);
            }
            if (front >= length) {
                return 0;
            }
            if ((side < 0 ? -side : side) > width) {
                return 0;
            }
        }
    }
    RailUnit_SetLineInfo(unit, line, dir, front, side, length, width);
    func_ov021_021b963c(line, unit->handle);
    return dir;
}

void RailUnit_SetLineInfo(RailUnit *unit, RailLine *line, u32 dir, u32 front, u32 side, u32 length, u32 width) {
    unit->onLine = 1;
    unit->component.line = line;
    unit->side = side;
    unit->length = length;
    unit->dir = dir;
    unit->front = front;
    unit->width = width;
}

BOOL func_ov021_021b8674(RailDataHandle *handle, RailLine *line, RailPoint *point) {
    RailPoint *result = GetRailPoint(handle, line->pointA);
    if (result != point) {
        return FALSE;
    }
    return TRUE;
}

u32 func_ov021_021b868c(RailDataHandle *handle, RailLine *line, RailPoint *point) {
    RailPoint *result;

    result = GetRailPoint(handle, line->pointB);
    if (result == point) {
        return 1;
    }
    return 0;
}

void SetupLineWidthForLineChange(RailDataHandle *data, const RailLine *line, s32 *outNext, s32 *outCur, RailDisabledLines *disabled, u32 tileWidth) {
    RailPoint *point;
    RailLine *l1;
    RailLine *l2;
    RailLine *l3;
    s32 nextWidth;
    s32 curWidth;
    s32 length;
    s32 len;
    s32 w;

    nextWidth = 0;
    point = GetRailPoint(data, line->pointA);
    l1 = GetLineContinueInDirection(point, func_ov021_021b93d4(line->dir), data, disabled, TRUE);
    l2 = GetLineContinueInDirection(point, func_ov021_021b9434(line->dir), data, disabled, TRUE);
    l3 = GetLineContinueInDirection(point, func_ov021_021b9404(line->dir), data, disabled, TRUE);
    length = GetLineTileLengthWorldSpace(line, tileWidth, data);
    curWidth = GetLerpedLineWidthByLineYPos(line, 0, length, data);

    if (l1 != NULL) {
        len = GetLineTileLengthWorldSpace(l1, tileWidth, data);
        w = GetLerpedLineWidthByLineYPos(l1, len, len, data);
        if (w > curWidth) {
            curWidth = w;
        }
    }
    if (l2 != NULL) {
        len = GetLineTileLengthWorldSpace(l2, tileWidth, data);
        if (func_ov021_021b8674(data, l2, point)) {
            w = GetLerpedLineWidthByLineYPos(l2, 0, len, data);
        } else {
            w = GetLerpedLineWidthByLineYPos(l2, len, len, data);
        }
        if (w > 0) {
            nextWidth = w;
        }
    }
    if (l3 != NULL) {
        len = GetLineTileLengthWorldSpace(l3, tileWidth, data);
        if (func_ov021_021b8674(data, l3, point)) {
            w = GetLerpedLineWidthByLineYPos(l3, 0, len, data);
        } else {
            w = GetLerpedLineWidthByLineYPos(l3, len, len, data);
        }
        if (w > nextWidth) {
            nextWidth = w;
        }
    }
    *outNext = nextWidth;
    *outCur = curWidth;
}

void func_ov021_021b87bc(void *param_1) {
    MI_CpuFill8(param_1, 0, 0x30);
}

void func_ov021_021b87c8(void *param_1, void *param_2) {
    MI_CpuCopy8(param_2, param_1, 0x30);
}

RailPoint *GetRailPoint(RailDataHandle *handle, u32 index) {
    if (handle->pointCount > index) {
        return &handle->points[index];
    }
    if (index == 0xFFFF) {
        return NULL;
    }
    return handle->points;
}

RailLine *GetRailLine(RailDataHandle *handle, u32 index) {
    if (handle->lineCount > index) {
        return &handle->lines[index];
    }
    if (index == 0xFFFF) {
        return NULL;
    }
    return handle->lines;
}

RailCamera *GetRailCamera(RailDataHandle *handle, u32 index) {
    if (handle->cameraCount > index) {
        return &handle->cameras[index];
    }
    if (index == 0xFFFF) {
        return NULL;
    }
    return &data_ov021_021d9a0c;
}

RailCurve *GetRailCurve(RailDataHandle *handle, u32 index) {
    if (handle->curveCount > index) {
        return &handle->curves[index];
    }
    if (index == 0xFFFF) {
        return NULL;
    }
    return &data_ov021_021d9a30;
}

RailCameraFunc GetRailCameraCurveFunc(RailDataHandle *handle, u32 index) {
    if (handle->camFuncCount > index) {
        return handle->camFuncs[index];
    }
    if (index == 0xFFFF) {
        return NULL;
    }
    return *handle->camFuncs;
}

RailToWorldFunc GetHandler1CurveFunc(RailDataHandle *handle, u32 index) {
    if (handle->railToWorldCount > index) {
        return handle->railToWorldFuncs[index];
    }
    if (index == 0xFFFF) {
        return NULL;
    }
    return *handle->railToWorldFuncs;
}

WorldToRailFunc GetHandler2CurveFunc(RailDataHandle *handle, u32 index) {
    if (handle->worldToRailCount > index) {
        return handle->worldToRailFuncs[index];
    }
    if (index == 0xFFFF) {
        return NULL;
    }
    return *handle->worldToRailFuncs;
}

s32 func_ov021_021b88dc(RailUnit *unit, u32 dir, s32 step) {
    RailLine *line = unit->component.line;
    s32 length = unit->length;
    s32 nextLength;

    if (dir == line->dir) {
        // Along the line toward point B, then onto the line ahead there or one to the left or right
        RailPoint *point;
        RailLine *ahead;
        RailLine *left;
        RailLine *right;
        BOOL outsideWidth;
        s32 width;
        u32 resultDir;

        point = GetRailPoint(unit->handle, line->pointB);
        outsideWidth = FALSE;
        ahead = GetLineContinueInDirection(point, dir, unit->handle, unit->disabled, FALSE);
        left = GetLineContinueInDirection(point, func_ov021_021b9434(dir), unit->handle, unit->disabled, FALSE);
        right = GetLineContinueInDirection(point, func_ov021_021b9404(dir), unit->handle, unit->disabled, FALSE);
        func_ov021_021b9530(unit->disabled, unit, step, 0, dir);
        if (unit->front < length) {
            width = GetLerpedLineWidthByLineYPos(line, unit->front, length, unit->handle);
            if (width >= MATH_ABS(unit->side)) {
                return dir;
            }
            outsideWidth = TRUE;
        }
        if (ahead != NULL && !outsideWidth) {
            func_ov021_021b96ac(point, unit->handle);
            nextLength = GetLineTileLengthWorldSpace(ahead, unit->tileWidth, unit->handle);
            width = GetLerpedLineWidthByLineYPos(ahead, unit->front - length, nextLength, unit->handle);
            resultDir = func_ov021_021b854c(unit, ahead, dir, unit->front - length, unit->side, nextLength, width);
            if (resultDir != RAIL_DIR_NONE) {
                return resultDir;
            }
        }
        if (left != NULL && unit->side <= 0) {
            if (func_ov021_021b8674(unit->handle, left, point)) {
                func_ov021_021b96ac(point, unit->handle);
                nextLength = GetLineTileLengthWorldSpace(left, unit->tileWidth, unit->handle);
                width = GetLerpedLineWidthByLineYPos(left, -unit->side, nextLength, unit->handle);
                resultDir =
                    func_ov021_021b854c(unit, left, dir, -unit->side, unit->front - length, nextLength, width);
                if (resultDir != RAIL_DIR_NONE) {
                    return resultDir;
                }
            } else if (func_ov021_021b868c(unit->handle, left, point)) {
                func_ov021_021b96ac(point, unit->handle);
                nextLength = GetLineTileLengthWorldSpace(left, unit->tileWidth, unit->handle);
                width = GetLerpedLineWidthByLineYPos(left, nextLength + unit->side, nextLength, unit->handle);
                resultDir = func_ov021_021b854c(unit, left, dir, nextLength + unit->side, -(unit->front - length),
                                                  nextLength, width);
                if (resultDir != RAIL_DIR_NONE) {
                    return resultDir;
                }
            }
        }
        if (right != NULL && unit->side >= 0) {
            if (func_ov021_021b8674(unit->handle, right, point)) {
                func_ov021_021b96ac(point, unit->handle);
                nextLength = GetLineTileLengthWorldSpace(right, unit->tileWidth, unit->handle);
                width = GetLerpedLineWidthByLineYPos(right, unit->side, nextLength, unit->handle);
                resultDir =
                    func_ov021_021b854c(unit, right, dir, unit->side, -(unit->front - length), nextLength, width);
                if (resultDir != RAIL_DIR_NONE) {
                    return resultDir;
                }
            } else if (func_ov021_021b868c(unit->handle, right, point)) {
                func_ov021_021b96ac(point, unit->handle);
                nextLength = GetLineTileLengthWorldSpace(right, unit->tileWidth, unit->handle);
                width = GetLerpedLineWidthByLineYPos(right, nextLength - unit->side, nextLength, unit->handle);
                resultDir = func_ov021_021b854c(unit, right, dir, nextLength - unit->side, unit->front - length,
                                                  nextLength, width);
                if (resultDir != RAIL_DIR_NONE) {
                    return resultDir;
                }
            }
        }
        if (outsideWidth) {
            return func_ov021_021b9338(unit, line, length, dir);
        }
        if (unit->front <= length) {
            return dir;
        }
        unit->front -= step;
        return RAIL_DIR_NONE;
    } else if (dir == func_ov021_021b93d4(line->dir)) {
        // Back along the line toward point A, the same way
        RailPoint *point;
        RailLine *right;
        RailLine *left;
        RailLine *ahead;
        BOOL outsideWidth;
        s32 width;
        u32 resultDir;

        point = GetRailPoint(unit->handle, line->pointA);
        right = GetLineContinueInDirection(point, func_ov021_021b9404(dir), unit->handle, unit->disabled, FALSE);
        outsideWidth = FALSE;
        left = GetLineContinueInDirection(point, func_ov021_021b9434(dir), unit->handle, unit->disabled, FALSE);
        ahead = GetLineContinueInDirection(point, dir, unit->handle, unit->disabled, FALSE);
        func_ov021_021b9530(unit->disabled, unit, -step, 0, dir);
        if (unit->front >= 0) {
            width = GetLerpedLineWidthByLineYPos(line, unit->front, length, unit->handle);
            if (width >= MATH_ABS(unit->side)) {
                return dir;
            }
            outsideWidth = TRUE;
        }
        if (ahead != NULL && !outsideWidth) {
            func_ov021_021b96ac(point, unit->handle);
            nextLength = GetLineTileLengthWorldSpace(ahead, unit->tileWidth, unit->handle);
            width = GetLerpedLineWidthByLineYPos(ahead, nextLength - MATH_ABS(unit->front), nextLength, unit->handle);
            resultDir = func_ov021_021b854c(unit, ahead, dir, nextLength - MATH_ABS(unit->front), unit->side,
                                              nextLength, width);
            if (resultDir != RAIL_DIR_NONE) {
                return resultDir;
            }
        }
        if (right != NULL && unit->side <= 0) {
            if (func_ov021_021b8674(unit->handle, right, point)) {
                func_ov021_021b96ac(point, unit->handle);
                nextLength = GetLineTileLengthWorldSpace(right, unit->tileWidth, unit->handle);
                width = GetLerpedLineWidthByLineYPos(right, -unit->side, nextLength, unit->handle);
                resultDir = func_ov021_021b854c(unit, right, dir, -unit->side, unit->front, nextLength, width);
                if (resultDir != RAIL_DIR_NONE) {
                    return resultDir;
                }
            } else if (func_ov021_021b868c(unit->handle, right, point)) {
                func_ov021_021b96ac(point, unit->handle);
                nextLength = GetLineTileLengthWorldSpace(right, unit->tileWidth, unit->handle);
                width = GetLerpedLineWidthByLineYPos(right, nextLength + unit->side, nextLength, unit->handle);
                resultDir =
                    func_ov021_021b854c(unit, right, dir, nextLength + unit->side, -unit->front, nextLength, width);
                if (resultDir != RAIL_DIR_NONE) {
                    return resultDir;
                }
            }
        }
        if (left != NULL && unit->side >= 0) {
            if (func_ov021_021b8674(unit->handle, left, point)) {
                func_ov021_021b96ac(point, unit->handle);
                nextLength = GetLineTileLengthWorldSpace(left, unit->tileWidth, unit->handle);
                width = GetLerpedLineWidthByLineYPos(left, unit->side, nextLength, unit->handle);
                resultDir = func_ov021_021b854c(unit, left, dir, unit->side, -unit->front, nextLength, width);
                if (resultDir != RAIL_DIR_NONE) {
                    return resultDir;
                }
            }
            if (func_ov021_021b868c(unit->handle, left, point)) {
                func_ov021_021b96ac(point, unit->handle);
                nextLength = GetLineTileLengthWorldSpace(left, unit->tileWidth, unit->handle);
                width = GetLerpedLineWidthByLineYPos(left, nextLength - unit->side, nextLength, unit->handle);
                resultDir =
                    func_ov021_021b854c(unit, left, dir, nextLength - unit->side, unit->front, nextLength, width);
                if (resultDir != RAIL_DIR_NONE) {
                    return resultDir;
                }
            }
        }
        if (outsideWidth) {
            return func_ov021_021b9338(unit, line, length, dir);
        }
        unit->front += step;
        return RAIL_DIR_NONE;
    } else if (dir == func_ov021_021b9404(line->dir)) {
        // To the right of the line: at each whole tile, onto a line that leaves point A or point B in dir
        RailPoint *pointA;
        RailPoint *pointB;
        RailLine *next;
        s32 width;
        u32 resultDir;

        func_ov021_021b9530(unit->disabled, unit, 0, step, dir);
        if (unit->side > 0 && unit->side % 16 == 0) {
            pointA = GetRailPoint(unit->handle, line->pointA);
            next = GetLineContinueInDirection(pointA, dir, unit->handle, unit->disabled, FALSE);
            if (next != NULL) {
                if (func_ov021_021b8674(unit->handle, next, pointA)) {
                    func_ov021_021b96ac(pointA, unit->handle);
                    nextLength = GetLineTileLengthWorldSpace(next, unit->tileWidth, unit->handle);
                    width = GetLerpedLineWidthByLineYPos(next, unit->side, nextLength, unit->handle);
                    resultDir = func_ov021_021b854c(unit, next, dir, unit->side, -unit->front, nextLength, width);
                    if (resultDir != RAIL_DIR_NONE) {
                        return resultDir;
                    }
                }
                if (func_ov021_021b868c(unit->handle, next, pointA)) {
                    func_ov021_021b96ac(pointA, unit->handle);
                    nextLength = GetLineTileLengthWorldSpace(next, unit->tileWidth, unit->handle);
                    width = GetLerpedLineWidthByLineYPos(next, nextLength - unit->side, nextLength, unit->handle);
                    resultDir =
                        func_ov021_021b854c(unit, next, dir, nextLength - unit->side, unit->front, nextLength, width);
                    if (resultDir != RAIL_DIR_NONE) {
                        return resultDir;
                    }
                }
            }
            pointB = GetRailPoint(unit->handle, line->pointB);
            next = GetLineContinueInDirection(pointB, dir, unit->handle, unit->disabled, FALSE);
            if (next != NULL) {
                if (func_ov021_021b8674(unit->handle, next, pointB)) {
                    func_ov021_021b96ac(pointB, unit->handle);
                    nextLength = GetLineTileLengthWorldSpace(next, unit->tileWidth, unit->handle);
                    width = GetLerpedLineWidthByLineYPos(next, unit->side, nextLength, unit->handle);
                    resultDir =
                        func_ov021_021b854c(unit, next, dir, unit->side, length - unit->front, nextLength, width);
                    if (resultDir != RAIL_DIR_NONE) {
                        return resultDir;
                    }
                }
                if (func_ov021_021b868c(unit->handle, next, pointB)) {
                    func_ov021_021b96ac(pointB, unit->handle);
                    nextLength = GetLineTileLengthWorldSpace(next, unit->tileWidth, unit->handle);
                    width = GetLerpedLineWidthByLineYPos(next, nextLength - unit->side, nextLength, unit->handle);
                    resultDir = func_ov021_021b854c(unit, next, dir, nextLength - unit->side, -(length - unit->front),
                                                      nextLength, width);
                    if (resultDir != RAIL_DIR_NONE) {
                        return resultDir;
                    }
                }
            }
        }
        if (unit->side > 0 && unit->side <= unit->width) {
            return func_ov021_021b9338(unit, line, length, dir);
        }
        return dir;
    } else if (dir == func_ov021_021b9434(line->dir)) {
        // To the left of the line, the same way
        RailPoint *point;
        RailLine *nextA;
        RailLine *nextB;
        s32 width;
        u32 resultDir;

        func_ov021_021b9530(unit->disabled, unit, 0, -step, dir);
        if (unit->side < 0 && MATH_ABS(unit->side) % 16 == 0) {
            point = GetRailPoint(unit->handle, line->pointA);
            nextA = GetLineContinueInDirection(point, dir, unit->handle, unit->disabled, FALSE);
            if (nextA != NULL) {
                if (func_ov021_021b8674(unit->handle, nextA, point)) {
                    func_ov021_021b96ac(point, unit->handle);
                    nextLength = GetLineTileLengthWorldSpace(nextA, unit->tileWidth, unit->handle);
                    width = GetLerpedLineWidthByLineYPos(nextA, MATH_ABS(unit->side), nextLength, unit->handle);
                    resultDir =
                        func_ov021_021b854c(unit, nextA, dir, MATH_ABS(unit->side), unit->front, nextLength, width);
                    if (resultDir != RAIL_DIR_NONE) {
                        return resultDir;
                    }
                }
                if (func_ov021_021b868c(unit->handle, nextA, point)) {
                    func_ov021_021b96ac(point, unit->handle);
                    nextLength = GetLineTileLengthWorldSpace(nextA, unit->tileWidth, unit->handle);
                    width = GetLerpedLineWidthByLineYPos(nextA, nextLength - MATH_ABS(unit->side), nextLength,
                                                         unit->handle);
                    resultDir = func_ov021_021b854c(unit, nextA, dir, nextLength - MATH_ABS(unit->side), -unit->front,
                                                      nextLength, width);
                    if (resultDir != RAIL_DIR_NONE) {
                        return resultDir;
                    }
                }
            }
            point = GetRailPoint(unit->handle, line->pointB);
            nextB = GetLineContinueInDirection(point, dir, unit->handle, unit->disabled, FALSE);
            if (nextB != NULL) {
                if (func_ov021_021b8674(unit->handle, nextB, point)) {
                    func_ov021_021b96ac(point, unit->handle);
                    nextLength = GetLineTileLengthWorldSpace(nextB, unit->tileWidth, unit->handle);
                    width = GetLerpedLineWidthByLineYPos(nextB, MATH_ABS(unit->side), nextLength, unit->handle);
                    resultDir = func_ov021_021b854c(unit, nextB, dir, MATH_ABS(unit->side), -(length - unit->front),
                                                      nextLength, width);
                    if (resultDir != RAIL_DIR_NONE) {
                        return resultDir;
                    }
                }
                if (func_ov021_021b868c(unit->handle, nextB, point)) {
                    func_ov021_021b96ac(point, unit->handle);
                    nextLength = GetLineTileLengthWorldSpace(nextB, unit->tileWidth, unit->handle);
                    width = GetLerpedLineWidthByLineYPos(nextB, nextLength - MATH_ABS(unit->side), nextLength,
                                                         unit->handle);
                    resultDir = func_ov021_021b854c(unit, nextB, dir, nextLength - MATH_ABS(unit->side),
                                                      length - unit->front, nextLength, width);
                    if (resultDir != RAIL_DIR_NONE) {
                        return resultDir;
                    }
                }
            }
        }
        if (unit->side < 0 && MATH_ABS(unit->side) <= unit->width) {
            return func_ov021_021b9338(unit, line, length, dir);
        }
        return dir;
    }
    return RAIL_DIR_NONE;
}

void RailUnit_SetDataCore(RailUnit *unit, const RailDataHandle *data, FieldRailSystem *rail) {
    unit->handle = data;
    unit->onLine = 2;
    unit->tileWidth = data->tileWidth;
    unit->unk_33 = 0;
    unit->unk_32 = 0;
    unit->pending = 0;
    unit->rail = rail;
    unit->component.line = NULL;
    unit->front = 0;
    unit->side = 0;
    unit->dir = 0;
    unit->disabled = &rail->disabledLines;
    unit->nextCached = 0;
    unit->active = 0;
}

void RailUnit_SetData(RailUnit *unit, RailDataHandle *data) {
    RailUnit_SetDataCore(unit, data, unit->rail);
}

u16 RailUnit_GetState(RailUnit *unit) {
    return unit->acquired;
}

u32 func_ov021_021b91ac(RailUnit *unit) {
    if (((unit->onLine < 2) && (unit->component.line != NULL)) && (unit->acquired != 0)) {
        return 1;
    }
    return 0;
}

RailCamera *func_ov021_021b91c8(RailUnit *unit) {
    switch (unit->onLine) {
    case 0:
        return GetRailCamera(unit->handle, unit->component.point->cameraId);
    case 1:
        return GetRailCamera(unit->handle, unit->component.line->cameraId);
    default:
        return &data_ov021_021d9a0c;
    }
}

void func_ov021_021b91f8(RailUnit *unit, VecFx32 *pos) {
    RailLine *link;
    RailCurve *curve;
    RailToWorldFunc func;

    if (unit->onLine == 0) {
        RailPoint *point = unit->component.point;
        *pos = point->pos;
    } else if (unit->onLine == 1) {
        link = unit->component.line;
        curve = GetRailCurve(unit->handle, link->curveId);
        func = GetHandler1CurveFunc(unit->handle, curve->handlerId);
        if (func != NULL) {
            func(unit, pos);
        }
    }
}

void func_ov021_021b9238(const RailUnit *unit, RailPosition *pos) {
    s32 result;

    pos->componentIsLine = unit->onLine;
    if (unit->onLine == 0) {
        result = func_ov021_021b97d0(unit->handle, unit->component.point);
    } else {
        result = func_ov021_021b97fc(unit->handle, unit->component.line);
    }
    pos->componentId = result;
    pos->posFront = (s32)unit->front / 16;
    pos->posSide = (s32)unit->side / 16;
    pos->railDirection = unit->dir;
}

s32 GetLineTileLengthWorldSpace(const RailLine *line, u32 tileWidth, const RailDataHandle *handle) {
    return line->length << 4;
}

s32 GetLerpedLineWidthByLineYPos(const RailLine *line, u32 y, s32 length, const RailDataHandle *data) {
    const RailLine *self = line;
    RailPoint *pointA = data->points + self->pointA;
    RailPoint *pointB = data->points + self->pointB;
    s32 widthA;
    s32 widthB = -1;
    s32 diff;
    s32 i;

    widthA = -1;
    for (i = 0; i < 4; i++) {
        if (pointA->lineIds[i] != 0xffff && self == data->lines + pointA->lineIds[i]) {
            widthA = pointA->widths[i];
        }
        if (pointB->lineIds[i] != 0xffff && self == data->lines + pointB->lineIds[i]) {
            widthB = pointB->widths[i];
        }
    }
    if (widthA < 0) {
        widthA = widthB;
    }
    if (widthB < 0) {
        widthB = widthA;
    }
    if (y > (u32)(length - 16)) {
        y = length - 16;
    }
    diff = widthB - widthA;
    length -= 16;
    if (length != 0) {
        diff = diff * (s32)y / length;
    }
    return diff + widthA;
}

u32 func_ov021_021b9338(RailUnit *unit, RailLine *param_2, s32 param_3, u32 param_4) {
    s32 value;
    s32 unk14;
    s32 abs;

    value = GetLerpedLineWidthByLineYPos(param_2, unit->front, param_3, unit->handle);
    unit->width = value;
    unk14 = unit->side;
    abs = -unk14;
    if (unk14 >= 0) {
        abs = unk14;
    }
    if (abs >= value) {
        if (unk14 > 0) {
            unit->side = value;
        } else {
            unit->side = -value;
        }
    }
    return param_4;
}

u32 func_ov021_021b9368(FieldRailSystem *param_1, RailPosition *param_2, u32 param_3, s32 param_4, VecFx32 *param_5) {
    s32 i;

    RailUnit_SetRailPos(param_1->playerUnit, param_2);
    func_ov021_021b80e0(param_1->playerUnit, 4, param_3);
    func_ov021_021b810c(param_1->playerUnit);
    i = 0;
    func_ov021_021b80e0(param_1->playerUnit, 0, param_3);
    do {
        if (param_4 == i) {
            RailUnit_GetCalcPos(param_1->playerUnit, param_5);
        }
        func_ov021_021b810c(param_1->playerUnit);
        i = i + 1;
    } while (i < 0x10);
    if (GFL_STD_MemCmp(param_2, &param_1->playerUnit->railPos, 8) == 0) {
        return 0;
    }
    return func_ov021_021b7de0(param_1->playerUnit, &param_1->playerUnit->railPos);
}

u32 func_ov021_021b93d4(u32 param_1) {
    switch (param_1) {
    case 1:
        return 3;
    case 3:
        return 1;
    case 4:
        return 2;
    case 2:
        return 4;
    }
    return 0;
}

u32 func_ov021_021b9404(u32 param_1) {
    switch (param_1) {
    case 1:
        return 2;
    case 3:
        return 4;
    case 4:
        return 1;
    case 2:
        return 3;
    }
    return 0;
}

u32 func_ov021_021b9434(u32 param_1) {
    switch (param_1) {
    case 1:
        return 4;
    case 3:
        return 2;
    case 4:
        return 3;
    case 2:
        return 1;
    }
    return 0;
}

void func_ov021_021b9464(RailDisabledLines *disabled) {
    s32 i;
    for (i = 0; i < 0x12; i++) {
        disabled->lineIds[i] = 0xFFFF;
    }
}

void func_ov021_021b9478(RailDisabledLines *disabled, u16 lineId) {
    s32 i;

    i = 0;
    do {
        if (disabled->lineIds[i] == 0xffff) {
            disabled->lineIds[i] = lineId;
            return;
        }
        i = i + 1;
    } while (i < 0x12);
}

void func_ov021_021b949c(RailDisabledLines *disabled, u16 lineId) {
    s32 i;

    i = 0;
    do {
        if (lineId == disabled->lineIds[i]) {
            disabled->lineIds[i] = 0xffff;
            return;
        }
        i = i + 1;
    } while (i < 0x12);
}

u32 func_ov021_021b94c0(RailDisabledLines *disabled, u16 lineId) {
    s32 i;

    i = 0;
    do {
        if (lineId == disabled->lineIds[i]) {
            return 1;
        }
        i = i + 1;
    } while (i < 0x12);
    return 0;
}

BOOL func_ov021_021b94d8(RailDisabledLines *disabled, const RailLine *line, const RailDataHandle *handle) {
    u32 index = 0;
    s32 i;

    for (i = 0; i < handle->lineCount; i++) {
        if (line == handle->lines + i) {
            index = i;
        }
    }
    return func_ov021_021b94c0(disabled, index);
}

RailLine *func_ov021_021b9510(RailDisabledLines *disabled, RailDataHandle *handle, u32 lineId) {
    if (func_ov021_021b94c0(disabled, lineId) == 0) {
        return GetRailLine(handle, lineId);
    }
    return NULL;
}

void func_ov021_021b9530(RailDisabledLines *param_1, RailUnit *param_2, s32 param_3, s32 param_4, s32 param_5) {
    RailPoint *point;
    RailLine *next;
    s32 uStack_18;
    s32 uStack_1c;
    s32 front;
    s32 side;

    if (func_ov021_021b94d8(param_1, param_2->component.line, param_2->handle) != FALSE) {
        point = GetRailPoint(param_2->handle, param_2->component.line->pointA);
        next = GetLineContinueInDirection(point, param_5, param_2->handle, param_1, 0);
        SetupLineWidthForLineChange(param_2->handle, param_2->component.line, &uStack_18, &uStack_1c, param_1, param_2->tileWidth);
        front = param_2->front + param_3;
        param_2->front = front;
        param_2->side = param_2->side + param_4;
        if (front > uStack_18) {
            param_2->front = uStack_18;
        }
        if (next == NULL) {
            side = param_2->side;
            front = -side;
            if (side >= 0) {
                front = side;
            }
            if (front > uStack_1c) {
                if (0 < side) {
                    param_2->side = uStack_1c;
                    return;
                }
                param_2->side = -uStack_1c;
                return;
            }
        }
    } else {
        param_2->front = param_2->front + param_3;
        param_2->side = param_2->side + param_4;
    }
}

BOOL func_ov021_021b95c0(RailDisabledLines *disabled, RailLine *line, s32 front, s32 side, RailDataHandle *data, u32 tileWidth) {
    s32 nextWidth;
    s32 curWidth;

    if (func_ov021_021b94d8(disabled, line, data) != FALSE) {
        SetupLineWidthForLineChange(data, line, &nextWidth, &curWidth, disabled, tileWidth);
        if (front <= nextWidth) {
            if (side < 0) {
                side = -side;
            }
            if (side <= curWidth) {
                return TRUE;
            }
        }
        return FALSE;
    }
    return TRUE;
}

u32 func_ov021_021b960c(RailPoint *point, RailLine *line, RailDataHandle *handle) {
    long i;

    for (i = 0; i < 4; i++) {
        if (GetRailLine(handle, point->lineIds[i]) == line) {
            return point->dirs[i];
        }
    }
    return 0;
}

u32 func_ov021_021b963c(RailLine *line, RailDataHandle *handle) {
    RailPoint *pointA;
    RailPoint *pointB;
    s32 result;
    u32 result2;

    if ((line->pointA == 0xffff) || (line->pointB == 0xffff)) {
        return 0;
    }
    pointA = GetRailPoint(handle, line->pointA);
    pointB = GetRailPoint(handle, line->pointB);
    result2 = func_ov021_021b960c(pointA, line, handle);
    if (result2 == 0) {
        return 0;
    }
    if (result2 != line->dir) {
        return 0;
    }
    result = func_ov021_021b960c(pointB, line, handle);
    if (result == 0) {
        return 0;
    }
    if (result == func_ov021_021b93d4(line->dir)) {
        return 1;
    }
    return 0;
}

u32 func_ov021_021b96ac(RailPoint *param_1, RailDataHandle *param_2) {
    s32 i;
    RailPoint *railPoint;
    RailPoint *railPoint2;
    RailLine *railLine;
    u32 lineId;
    s32 iStack_18;
    u32 *dirs;
    RailLine *railLine2;

    iStack_18 = 0;
    do {
        lineId = param_1->lineIds[iStack_18];
        if (param_1->dirs[iStack_18] == 0 != (param_1->lineIds[iStack_18] == 0xffff)) {
            return 0;
        }
        if (lineId != 0xffff) {
            railLine2 = GetRailLine(param_2, lineId);
            railPoint = GetRailPoint(param_2, railLine2->pointA);
            railPoint2 = GetRailPoint(param_2, railLine2->pointB);
            if (railPoint != param_1 && railPoint2 != param_1) {
                return 0;
            }
            i = iStack_18;
            if (param_1->dirs[iStack_18] != 0) {
                while (i = i + 1, i < 4) {
                    lineId = func_ov021_021b93d4(param_1->dirs[i]);
                    dirs = &param_1->dirs[iStack_18];
                    if (*dirs == lineId && ((railLine = GetRailLine(param_2, param_1->lineIds[i]),
                            railLine2->dir != railLine->dir))) {
                        return 0;
                    }
                }
            }
        }
        iStack_18 = iStack_18 + 1;
        if (iStack_18 >= 4) {
            return 1;
        }
    } while (TRUE);
}

void func_ov021_021b9768(void *param_1, VecFx32 *pos) {
}

void func_ov021_021b976c(void *param_1, RailPoint *point, void *param_3) {
    if (point != NULL) {
        func_ov021_021b9768(param_1, &point->pos);
    }
}

void func_ov021_021b977c(RailUnit *unit) {
    RailPoint *point;
    RailPoint *point2;

    if (unit->onLine == 0) {
        func_ov021_021b976c(0, unit->component.point, "\n");
        return;
    }
    if (unit->onLine == 1) {
        point = GetRailPoint(unit->handle, unit->component.line->pointA);
        point2 = GetRailPoint(unit->handle, unit->component.line->pointB);
        func_ov021_021b976c("start", point, 0);
        func_ov021_021b976c("-end", point2, "\n");
    }
}

s32 func_ov021_021b97d0(RailDataHandle *handle, const RailPoint *point) {
    s32 i;
    i = 0;
    while (i < (s32)(u32)handle->pointCount) {
        if (point == handle->points + i) {
            return i;
        }
        i = i + 1;
    }
    return 0;
}

s32 func_ov021_021b97fc(const RailDataHandle *handle, const RailLine *line) {
    s32 i;

    for (i = 0; i < handle->lineCount; i++) {
        if (line == handle->lines + i) {
            return i;
        }
    }
    return 0;
}

BOOL func_ov021_021b9828(const VecFx32 *a, const VecFx32 *b, const VecFx32 *p3, const VecFx32 *p4,
                                const VecFx32 *p5, VecFx32 *out) {
    VecFx32 n;
    VecFx32 dir;
    VecFx32 na;
    VecFx32 nb;
    fx32 len;
    fx32 num;
    fx32 denom;
    fx32 t;

    VEC_Subtract(p5, p4, &dir);
    len = VEC_Mag(&dir);
    VEC_Normalize(&dir, &dir);
    VEC_Normalize(a, &na);
    VEC_Normalize(b, &nb);
    VEC_CrossProduct(&na, &nb, &n);
    VEC_Normalize(&n, &n);
    num = (FX_Mul(n.x, p3->x) + FX_Mul(n.y, p3->y) + FX_Mul(n.z, p3->z)) -
          (FX_Mul(n.x, p4->x) + FX_Mul(n.y, p4->y) + FX_Mul(n.z, p4->z));
    denom = FX_Mul(n.x, dir.x) + FX_Mul(n.y, dir.y) + FX_Mul(n.z, dir.z);
    if (denom == 0) {
        return FALSE;
    }
    t = FX_Div(num, denom);
    if (t > len) {
        return FALSE;
    }
    if (t <= 0x1000) {
        return FALSE;
    }
    VEC_Set(out, p4->x + FX_Mul(dir.x, t), p4->y + FX_Mul(dir.y, t), p4->z + FX_Mul(dir.z, t));
    return TRUE;
}

void func_ov021_021b9a50(VecFx32 *out, const VecFx32 *a, const VecFx32 *b, fx32 t) {
    VecFx32 na;
    VecFx32 nb;
    s32 angle;
    u16 sinA;

    VEC_Normalize(a, &na);
    VEC_Normalize(b, &nb);
    angle = FX_AcosIdx(VEC_DotProduct(&na, &nb));
    sinA = FX_SinIdx(angle);
    if (sinA == 0) {
        *out = nb;
        return;
    }
    angle <<= 12;
    vecfx_mul(&na, FX_Div((u16)FX_SinIdx(FX_Mul(angle, 0x1000 - t) >> 12), sinA), &na);
    vecfx_mul(&nb, FX_Div((u16)FX_SinIdx(FX_Mul(angle, t) >> 12), sinA), &nb);
    VEC_Add(&na, &nb, out);
    VEC_Normalize(out, out);
}

void InterpolateRailPointsGlobal(VecFx32 *out, const VecFx32 *a, const VecFx32 *b, fx32 t) {
    VecFx32 zero = { 0, 0, 0 };
    fx32 lenA;
    fx32 lenB;

    func_ov021_021b9a50(out, a, b, t);
    lenA = VEC_Mag(a);
    lenB = VEC_Mag(b);
    VEC_MultAdd(lenA + FX_Mul(lenB - lenA, t), out, &zero, out);
}

void func_ov021_021b9b7c(RailUnit *unit, VecFx32 *out) {
    VecFx32 dir;
    RailPoint *p0;
    RailPoint *p1;
    VecFx32 *start;
    fx32 t;
    s32 posV;

    GetCurrentLine(unit);
    posV = GetCurrentLinePosV(unit);
    t = FX_Div(posV << 12, GetCurrentLineLength(unit) << 12);
    p0 = GetCurrentRailLinePoint1(unit);
    p1 = GetCurrentRailLinePoint2(unit);
    start = &p0->pos;
    VEC_Subtract(&p1->pos, start, &dir);
    VEC_MultAdd(t, &dir, start, out);
    {
        VecFx32 up = { 0, FX32_ONE, 0 };
        VecFx32 side;
        s32 posH = GetCurrentLinePosH(unit);
        s32 width = RailUnit_GetTileWidth(unit);

        VEC_CrossProduct(&dir, &up, &side);
        VEC_Normalize(&side, &side);
        VEC_MultAdd(FX_Mul(posH << 12, width), &side, out, out);
    }
}

const RailCurve data_ov021_021d9a30 = { 0xFFFF, { 0xFF, 0, 0 } };

void func_ov021_021b9c28(RailUnit *unit, VecFx32 *out) {
    u32 posV;
    u32 length;
    fx32 t;
    VecFx32 center;
    VecFx32 a;
    VecFx32 b;
    VecFx32 interp;
    VecFx32 dir;
    VecFx32 side;
    VecFx32 cross;
    RailPoint *p0;
    RailPoint *p1;
    VecFx32 *pos0;
    VecFx32 *pos1;
    RailCurve *curve;
    s32 posH;
    s32 width;
    fx32 offset;

    GetCurrentLine(unit);
    p0 = GetCurrentRailLinePoint1(unit);
    p1 = GetCurrentRailLinePoint2(unit);
    pos0 = &p0->pos;
    pos1 = &p1->pos;
    posV = GetCurrentLinePosV(unit);
    length = GetCurrentLineLength(unit);
    t = FX_Div(posV << 12, length << 12);
    out->y = pos1->y - pos0->y;
    out->y = FX_Div(FX_Mul(out->y, (fx32)posV << 12), (fx32)length << 12);
    out->y += pos0->y;
    curve = RailUnit_GetCurve(unit);
    center.x = curve->center.x;
    center.y = out->y;
    center.z = curve->center.z;
    VEC_Subtract(pos0, &center, &a);
    VEC_Subtract(pos1, &center, &b);
    a.y = 0;
    b.y = 0;
    VEC_Subtract(&b, &a, &dir);
    dir.y = 0;
    VEC_Normalize(&dir, &dir);
    InterpolateRailPointsGlobal(&interp, &a, &b, t);
    VEC_Add(&center, &interp, out);
    posH = GetCurrentLinePosH(unit);
    width = RailUnit_GetTileWidth(unit);
    offset = FX_Mul(posH << 12, width);
    VEC_Normalize(&interp, &side);
    VEC_CrossProduct(&side, &dir, &cross);
    if (cross.y < 0) {
        VEC_MultAdd(-offset, &side, out, out);
    } else {
        VEC_MultAdd(offset, &side, out, out);
    }
}

void func_ov021_021b9d60(RailUnit *unit, VecFx32 *out) {
    RailPoint *p0;
    RailPoint *p1;
    VecFx32 *pos0;
    VecFx32 *pos1;
    RailCurve *curve;
    VecFx32 center;
    VecFx32 a;
    VecFx32 b;
    VecFx32 interp;
    VecFx32 dir;
    VecFx32 side;
    VecFx32 cross;
    s32 posV;
    s32 length;
    fx32 t;
    s32 posH;
    s32 width;
    fx32 offset;

    GetCurrentLine(unit);
    p0 = GetCurrentRailLinePoint1(unit);
    p1 = GetCurrentRailLinePoint2(unit);
    pos0 = &p0->pos;
    pos1 = &p1->pos;
    posV = GetCurrentLinePosV(unit);
    length = GetCurrentLineLength(unit);
    t = FX_Div(posV << 12, length << 12);
    curve = RailUnit_GetCurve(unit);
    center.x = curve->center.x;
    center.y = curve->center.y;
    center.z = curve->center.z;
    VEC_Subtract(pos0, &center, &a);
    VEC_Subtract(pos1, &center, &b);
    VEC_Subtract(&b, &a, &dir);
    VEC_Normalize(&dir, &dir);
    InterpolateRailPointsGlobal(&interp, &a, &b, t);
    VEC_Add(&center, &interp, out);
    posH = GetCurrentLinePosH(unit);
    width = RailUnit_GetTileWidth(unit);
    offset = FX_Mul(posH << 12, width);
    if (interp.x != 0 && interp.z != 0) {
        interp.y = 0;
    } else {
        VecFx32 up = {0, FX32_ONE, 0};

        VEC_Subtract(pos1, pos0, &interp);
        VEC_CrossProduct(&interp, &up, &interp);
    }
    VEC_Normalize(&interp, &side);
    VEC_CrossProduct(&side, &dir, &cross);
    if (cross.y < 0) {
        VEC_MultAdd(-offset, &side, out, out);
    } else {
        VEC_MultAdd(offset, &side, out, out);
    }
}

static const VecFx32 data_ov021_021d9a00 = { 0, FX32_ONE, 0 };

fx32 FieldRailSystem_GetTileWidth(FieldRailSystem *rail);
fx32 func_ov021_021b8378(fx32 x);
BOOL func_ov021_021bae7c(RailPoint *p0, RailPoint *p1, const VecFx32 *pos, VecFx32 *hit, fx32 *along, VecFx32 *cross);
s32 GFL_STD_MemCmp(const void *a, const void *b, u32 size);
BOOL func_ov021_021b9e84(u32 lineId, FieldRailSystem *rail, const VecFx32 *pos, RailPosition *outPos, VecFx32 *outWorld) {
    fx32 tileWidth;
    RailLine *line;
    RailPoint *p0;
    RailPoint *p1;
    s32 w0;
    s32 w1;
    s32 length;
    u32 front;
    s32 base;
    VecFx32 hit;
    VecFx32 cross;
    VecFx32 a;
    VecFx32 b;
    fx32 along;
    RailPosition check;
    s32 i;

    line = FieldRailSystem_GetLine(rail, lineId);
    p0 = FieldRailSystem_GetPoint(rail, line->pointA);
    p1 = FieldRailSystem_GetPoint(rail, line->pointB);
    tileWidth = FieldRailSystem_GetTileWidth(rail);
    if (!func_ov021_021bae7c(p0, p1, pos, &hit, &along, &cross)) {
        return FALSE;
    }
    a.x = pos->x;
    a.z = pos->z;
    b.x = hit.x;
    b.z = hit.z;
    a.z = pos->z;
    a.y = 0;
    b.y = 0;
    front = FX_Div(VEC_Distance(&a, &b), tileWidth) >> 12;
    GetRailHnd2Curve(rail, line->curveId);
    length = line->length * 16;
    base = length - (FX_Div(along, tileWidth) >> 12);
    for (i = 0; i < 4; i++) {
        if (lineId == p0->lineIds[i]) {
            w0 = p0->widths[i];
        }
        if (lineId == p1->lineIds[i]) {
            w1 = p1->widths[i];
        }
    }
    if (front <= w0 + (u32)(base * (w1 - w0)) / (u32)length + 0x10) {
        outPos->componentId = lineId;
        outPos->componentIsLine = 1;
        outPos->railDirection = 3;
        if (cross.y >= 0) {
            outPos->posSide = func_ov021_021b8378(front);
        } else {
            outPos->posSide = -func_ov021_021b8378(front);
        }
        outPos->posFront = func_ov021_021b8378(base);
        func_ov021_021b8250(rail, outPos, &check);
        func_ov021_021b7d38(rail, &check, outWorld);
        if (GFL_STD_MemCmp(outPos, &check, sizeof(RailPosition)) == 0) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

BOOL func_ov021_021b9fc8(u32 lineId, FieldRailSystem *rail, const VecFx32 *pos, RailPosition *outPos, VecFx32 *outWorld) {
    RailLine *line;
    RailPoint *p0;
    RailPoint *p1;
    VecFx32 cur;
    VecFx32 prev;
    VecFx32 d;
    VecFx32 e;
    VecFx32 dn;
    VecFx32 hit;
    VecFx32 cross;
    VecFx32 b;
    VecFx32 a;
    VecFx32 n1;
    VecFx32 n2;
    fx32 along;
    RailPosition check;
    RailUnit *unit;
    u32 length;
    fx32 tileWidth;
    u32 step;

    line = FieldRailSystem_GetLine(rail, lineId);
    p0 = FieldRailSystem_GetPoint(rail, line->pointA);
    p1 = FieldRailSystem_GetPoint(rail, line->pointB);
    if (!func_ov021_021bae7c(p0, p1, pos, &hit, &along, &cross)) {
        return FALSE;
    }
    step = 1;
    outPos->componentIsLine = 1;
    outPos->railDirection = 3;
    outPos->componentId = lineId;
    outPos->posSide = 0;
    outPos->posFront = 0;
    unit = func_ov021_021b824c(rail);
    RailUnit_SetRailPos(unit, outPos);
    length = GetCurrentLineLength(unit);
    length >>= 4;
    RailUnit_GetCalcPos(unit, &cur);
    prev.x = cur.x;
    prev.y = cur.y;
    prev.z = cur.z;
    tileWidth = FieldRailSystem_GetTileWidth(rail);
    if (length > 1) {
        do {
            fx32 mag;
            s32 width;
            fx32 ratio;
            fx32 dot;
            u32 front;

            outPos->posFront = step;
            RailUnit_SetRailPos(unit, outPos);
            RailUnit_GetCalcPos(unit, &cur);
            width = func_ov021_021b8304(unit);
            VEC_Subtract(&prev, &cur, &d);
            VEC_Subtract(pos, &cur, &e);
            VEC_Normalize(&d, &dn);
            mag = VEC_Mag(&d);
            VEC_Normalize(&d, &n1);
            VEC_Normalize(&e, &n2);
            VEC_CrossProduct(&n1, &n2, &cross);
            d = dn;
            ratio = VEC_Mag(&e);
            VEC_Normalize(&e, &e);
            ratio = FX_Div(ratio, mag);
            e.x = FX_Mul(e.x, ratio);
            e.y = FX_Mul(e.y, ratio);
            e.z = FX_Mul(e.z, ratio);
            dot = VEC_DotProduct(&d, &e);
            if (dot <= 0x1000 && dot >= 0) {
                fx32 t = FX_Mul(dot, mag);

                hit.x = FX_Mul(dn.x, t);
                hit.y = FX_Mul(dn.y, t);
                hit.z = FX_Mul(dn.z, t);
                VEC_Add(&cur, &hit, &hit);
                VEC_Set(&b, pos->x, 0, pos->z);
                VEC_Set(&a, hit.x, 0, hit.z);
                front = FX_Div(VEC_Distance(&b, &a), tileWidth) >> 12;
                if (width + 0x10 >= front) {
                    if (cross.y >= 0) {
                        outPos->posSide = func_ov021_021b8378(front);
                    } else {
                        outPos->posSide = -func_ov021_021b8378(front);
                    }
                    func_ov021_021b8250(rail, outPos, &check);
                    func_ov021_021b7d38(rail, outPos, outWorld);
                    if (GFL_STD_MemCmp(outPos, &check, sizeof(RailPosition)) == 0) {
                        return TRUE;
                    }
                    return FALSE;
                }
            }
            prev.x = cur.x;
            prev.y = cur.y;
            prev.z = cur.z;
            step++;
        } while (step < length);
    }
    return FALSE;
}

void func_ov021_021ba25c(FieldRailSystem *param_1) {
    RailUnit *pRVar1;
    RailCamera *railCameraByCamHandler;
    FieldCamera *pFVar3;
    VecFx32 VStack_20;

    pRVar1 = FieldRailSystem_GetCameraParentUnit(param_1);
    railCameraByCamHandler = GetRailCameraByCamHandler(pRVar1);
    FieldRailSystem_GetCameraParentPos(param_1, &VStack_20);
    pFVar3 = FieldRailSystem_GetFieldCamera(param_1);
    FieldCamera_SetTransformType(pFVar3, 2);
    VStack_20.x = VStack_20.x + railCameraByCamHandler->unk_04;
    VStack_20.y = VStack_20.y + railCameraByCamHandler->unk_08;
    VStack_20.z = VStack_20.z + railCameraByCamHandler->unk_0C;
    pFVar3 = FieldRailSystem_GetFieldCamera(param_1);
    FieldCamera_CoordsSetEye(pFVar3, &VStack_20);
    pFVar3 = FieldRailSystem_GetFieldCamera(param_1);
    FieldCamera_ResetBind(pFVar3);
}

void func_ov021_021ba2b4(FieldRailSystem *rail) {
    FieldCamera *camera;
    RailCamera *cam;
    VecFx32 pos;
    u16 pitch;
    u16 yaw;
    s32 zoom;

    camera = FieldRailSystem_GetFieldCamera(rail);
    cam = GetRailCameraByCamHandler(FieldRailSystem_GetCameraParentUnit(rail));
    FieldCamera_SetTransformType(camera, 0);
    FieldCamera_ResetBind(FieldRailSystem_GetFieldCamera(rail));
    FieldRailSystem_GetCameraParentPos(rail, &pos);
    pitch = cam->unk_04;
    yaw = cam->unk_08;
    zoom = cam->unk_0C;
    FieldCamera_CoordsSetYaw(camera, yaw);
    FieldCamera_CoordsSetPitch(camera, pitch);
    FieldCamera_CoordsSetZoom(camera, zoom);
}

void func_ov021_021ba310(FieldRailSystem *param_1) {
    RailUnit *unit;
    FieldCamera *camera;
    VecFx32 VStack_20;

    unit = FieldRailSystem_GetCameraParentUnit(param_1);
    camera = FieldRailSystem_GetFieldCamera(param_1);
    func_ov021_021ba2b4(param_1);
    FieldCamera_ClearBind(camera);
    func_ov021_021b8314(unit, &VStack_20);
    FieldCamera_CoordsSetTarget(camera, &VStack_20);
}

void func_ov021_021ba348(FieldRailSystem *rail) {
    RailUnit *unit = FieldRailSystem_GetCameraParentUnit(rail);
    RailPoint *p0 = GetCurrentRailLinePoint1(unit);
    RailPoint *p1 = GetCurrentRailLinePoint2(unit);
    FieldCamera *camera = FieldRailSystem_GetFieldCamera(rail);
    RailCamera *c0;
    RailCamera *c1;
    s32 length;
    s32 posV;
    fx32 t;
    s32 pitch0;
    s32 yaw0;
    s32 zoom0;
    s32 yaw1;
    s32 zoom1;
    u16 pitch;
    s32 diff;

    FieldCamera_SetTransformType(camera, 0);
    FieldCamera_ResetBind(FieldRailSystem_GetFieldCamera(rail));
    c0 = GetRailCameraByPoint(unit, p0);
    c1 = GetRailCameraByPoint(unit, p1);
    length = GetCurrentLineLength(unit);
    posV = GetCurrentLinePosV(unit);
    t = FX_Div(posV << 12, length << 12);
    pitch0 = (u16)c0->unk_04;
    yaw0 = (u16)c0->unk_08;
    zoom0 = c0->unk_0C;
    yaw1 = (u16)c1->unk_08;
    zoom1 = c1->unk_0C;
    diff = (u16)c1->unk_04 - pitch0;
    if (diff >= 0x8000) {
        diff -= 0x10000;
    } else if (diff <= -0x8000) {
        diff += 0x10000;
    }
    pitch = pitch0 + (FX_Mul(diff << 12, t) >> 12);
    diff = yaw1 - yaw0;
    if (diff >= 0x8000) {
        diff -= 0x10000;
    } else if (diff <= -0x8000) {
        diff += 0x10000;
    }
    FieldCamera_CoordsSetYaw(camera, yaw0 + (FX_Mul(diff << 12, t) >> 12));
    FieldCamera_CoordsSetPitch(camera, pitch);
    FieldCamera_CoordsSetZoom(camera, zoom0 + FX_Mul(zoom1 - zoom0, t));
}

void func_ov021_021ba494(FieldRailSystem *param_1) {
    FieldCamera *camera;
    VecFx32 VStack_18;

    camera = FieldRailSystem_GetFieldCamera(param_1);
    func_ov021_021ba348(param_1);
    FieldCamera_ClearBind(camera);
    FieldRailSystem_GetCameraParentPos(param_1, &VStack_18);
    FieldCamera_CoordsSetTarget(camera, &VStack_18);
}

void func_ov021_021ba4c4(FieldRailSystem *param_1) {
    RailUnit *unit;
    FieldCamera *camera;
    VecFx32 VStack_20;

    unit = FieldRailSystem_GetCameraParentUnit(param_1);
    camera = FieldRailSystem_GetFieldCamera(param_1);
    func_ov021_021ba348(param_1);
    FieldCamera_ClearBind(camera);
    func_ov021_021b8314(unit, &VStack_20);
    FieldCamera_CoordsSetTarget(camera, &VStack_20);
}

void func_ov021_021ba4fc(FieldRailSystem *param_1) {
    RailUnit *pRVar1;
    RailCamera *railCameraByCamHandler;
    FieldCamera *camera;
    FieldCamera *camera_00;
    VecFx32 VStack_1c;
    VecFx32 VStack_28;

    pRVar1 = FieldRailSystem_GetCameraParentUnit(param_1);
    railCameraByCamHandler = GetRailCameraByCamHandler(pRVar1);
    camera = FieldRailSystem_GetFieldCamera(param_1);
    FieldCamera_SetTransformType(camera, 2);
    camera_00 = FieldRailSystem_GetFieldCamera(param_1);
    FieldCamera_ClearBind(camera_00);
    VStack_1c.x = railCameraByCamHandler->unk_04;
    VStack_1c.y = railCameraByCamHandler->unk_08;
    VStack_1c.z = railCameraByCamHandler->unk_0C;
    VStack_28.x = railCameraByCamHandler->unk_10;
    VStack_28.y = railCameraByCamHandler->unk_14;
    VStack_28.z = railCameraByCamHandler->unk_18;
    FieldCamera_CoordsSetEye(camera, &VStack_1c);
    FieldCamera_CoordsSetTarget(camera, &VStack_28);
}

void func_ov021_021ba550(FieldRailSystem *rail) {
    RailUnit *unit = FieldRailSystem_GetCameraParentUnit(rail);
    FieldCamera *camera = FieldRailSystem_GetFieldCamera(rail);
    RailPoint *p0 = GetCurrentRailLinePoint1(unit);
    RailPoint *p1 = GetCurrentRailLinePoint2(unit);
    RailCamera *c0;
    RailCamera *c1;
    s32 length;
    s32 posV;
    VecFx32 eye;
    VecFx32 target;

    c0 = GetRailCameraByPoint(unit, p0);
    c1 = GetRailCameraByPoint(unit, p1);
    FieldCamera_SetTransformType(camera, 2);
    FieldCamera_ClearBind(FieldRailSystem_GetFieldCamera(rail));
    length = GetCurrentLineLength(unit);
    posV = GetCurrentLinePosV(unit);
    eye.x = FX_Div(FX_Mul(c1->unk_04 - c0->unk_04, posV << 12), length << 12);
    eye.y = FX_Div(FX_Mul(c1->unk_08 - c0->unk_08, posV << 12), length << 12);
    eye.z = FX_Div(FX_Mul(c1->unk_0C - c0->unk_0C, posV << 12), length << 12);
    target.x = FX_Div(FX_Mul(c1->unk_10 - c0->unk_10, posV << 12), length << 12);
    target.y = FX_Div(FX_Mul(c1->unk_14 - c0->unk_14, posV << 12), length << 12);
    target.z = FX_Div(FX_Mul(c1->unk_18 - c0->unk_18, posV << 12), length << 12);
    eye.x += c0->unk_04;
    eye.y += c0->unk_08;
    eye.z += c0->unk_0C;
    target.x += c0->unk_10;
    target.y += c0->unk_14;
    target.z += c0->unk_18;
    FieldCamera_CoordsSetEye(camera, &eye);
    FieldCamera_CoordsSetTarget(camera, &target);
}

void func_ov021_021ba6e8(FieldRailSystem *rail) {
    RailUnit *unit = FieldRailSystem_GetCameraParentUnit(rail);
    RailCamera *c0;
    RailCamera *c1;
    s32 length;
    s32 posV;
    s32 pitchStep;
    u16 angle;
    s32 dist;
    VecFx32 target;

    FieldRailSystem_GetFieldCamera(rail);
    if (RailUnit_IsOnLine(unit) == 1) {
        RailPoint *p0 = GetCurrentRailLinePoint1(unit);
        RailPoint *p1 = GetCurrentRailLinePoint2(unit);

        c0 = GetRailCameraByPoint(unit, p0);
        c1 = GetRailCameraByPoint(unit, p1);
        length = GetCurrentLineLength(unit);
        posV = GetCurrentLinePosV(unit);
    } else {
        c0 = GetRailCameraByCamHandler(unit);
        c1 = GetRailCameraByCamHandler(unit);
        length = 1;
        posV = 0;
    }
    pitchStep = (s32)(c1->unk_04 - c0->unk_04) * posV / length;
    angle = c0->unk_04 + pitchStep;
    dist = c0->unk_08 + FX_Div(FX_Mul(c1->unk_08 - c0->unk_08, posV << 12), length << 12);
    target.x = c0->unk_0C;
    target.y = c0->unk_10;
    target.z = c0->unk_14;
    func_ov021_021ba7e0(rail, angle, dist, &target);
}

void func_ov021_021ba7b0(FieldRailSystem *rail) {
    RailCamera *data;
    VecFx32 pos;
    u16 id;
    u32 unk;

    data = GetRailCameraByCamHandler(FieldRailSystem_GetCameraParentUnit(rail));
    id = data->unk_04;
    unk = data->unk_08;
    pos.x = data->unk_0C;
    pos.y = data->unk_10;
    pos.z = data->unk_14;
    func_ov021_021ba7e0(rail, id, unk, &pos);
}

void func_ov021_021ba7e0(FieldRailSystem *rail, u16 angle, s32 dist, VecFx32 *target) {
    FieldCamera *camera = FieldRailSystem_GetFieldCamera(rail);
    VecFx32 diff;
    VecFx32 base;
    VecFx32 dir;
    VecFx32 eye;
    fx32 horiz;
    fx32 y;

    FieldCamera_SetTransformType(camera, 2);
    FieldCamera_ClearBind(camera);
    base = *target;
    y = base.y;
    base.y = 0;
    FieldRailSystem_GetCameraParentPos(rail, &diff);
    diff.y = 0;
    VEC_Subtract(&diff, &base, &diff);
    VEC_Normalize(&diff, &dir);
    eye.y = FX_Mul(FX_SinIdx(angle), dist);
    horiz = FX_Mul(FX_CosIdx(angle), dist);
    eye.x = FX_Mul(dir.x, horiz);
    eye.z = FX_Mul(dir.z, horiz);
    eye.x += base.x;
    eye.y += y;
    eye.z += base.z;
    FieldCamera_CoordsSetTarget(camera, target);
    FieldCamera_CoordsSetEye(camera, &eye);
}

void func_ov021_021ba8e0(FieldRailSystem *rail) {
    RailUnit *unit = FieldRailSystem_GetCameraParentUnit(rail);
    s32 length;
    s32 posV;
    RailCamera *cam;
    u16 angle;
    s32 dist;
    VecFx32 target;

    RailUnit_IsOnLine(unit);
    length = GetCurrentLineLength(unit);
    posV = GetCurrentLinePosV(unit);
    cam = GetRailCameraByCamHandler(unit);
    angle = cam->unk_04;
    dist = cam->unk_08;
    target.x = cam->unk_0C + FX_Div(FX_Mul(cam->unk_18 - cam->unk_0C, posV << 12), length << 12);
    target.y = cam->unk_10 + FX_Div(FX_Mul(cam->unk_1C - cam->unk_10, posV << 12), length << 12);
    target.z = cam->unk_14 + FX_Div(FX_Mul(cam->unk_20 - cam->unk_14, posV << 12), length << 12);
    func_ov021_021ba7e0(rail, angle, dist, &target);
}

void func_ov021_021ba9b4(FieldRailSystem *param_1) {
    u16 yaw;
    RailUnit *pRVar1;
    RailCamera *railCameraByCamHandler;
    FieldCamera *camera;
    FieldCamera *camera_00;
    VecFx32 VStack_24;
    VecFx32 VStack_30;

    pRVar1 = FieldRailSystem_GetCameraParentUnit(param_1);
    railCameraByCamHandler = GetRailCameraByCamHandler(pRVar1);
    camera = FieldRailSystem_GetFieldCamera(param_1);
    FieldCamera_SetTransformType(camera, 0);
    camera_00 = FieldRailSystem_GetFieldCamera(param_1);
    FieldCamera_ResetBind(camera_00);
    FieldRailSystem_GetCameraParentPos(param_1, &VStack_30);
    VStack_30.y = 0;
    VStack_24.x = railCameraByCamHandler->unk_0C;
    VStack_24.y = 0;
    VStack_24.z = railCameraByCamHandler->unk_10;
    VEC_Subtract(&VStack_30, &VStack_24, &VStack_30);
    VEC_Normalize(&VStack_30, &VStack_30);
    yaw = FX_Atan2Idx(VStack_30.x, VStack_30.z);
    FieldCamera_CoordsSetPitch(camera, (u16)railCameraByCamHandler->unk_04);
    FieldCamera_CoordsSetZoom(camera, railCameraByCamHandler->unk_08);
    FieldCamera_CoordsSetYaw(camera, yaw);
}

void func_ov021_021baa30(FieldRailSystem *param_1) {
    VecFx16 sp0;
    FieldCamera *camera;
    RailUnit *unit;
    u16 result;
    RailCamera *railCameraByCamHandler;

    unit = FieldRailSystem_GetCameraParentUnit(param_1);
    railCameraByCamHandler = GetRailCameraByCamHandler(unit);
    camera = FieldRailSystem_GetFieldCamera(param_1);
    FieldCamera_SetTransformType(camera, 0U);
    FieldCamera_ResetBind(FieldRailSystem_GetFieldCamera(param_1));
    func_ov021_021b8050(unit, &sp0.x);
    result = FX_Atan2Idx(-sp0.x, -sp0.z);
    FieldCamera_CoordsSetPitch(camera, (u16) railCameraByCamHandler->unk_08);
    FieldCamera_CoordsSetZoom(camera, railCameraByCamHandler->unk_0C);
    FieldCamera_CoordsSetYaw(camera, (u16) (result + railCameraByCamHandler->unk_04));
}

void func_ov021_021baa9c(FieldRailSystem *rail) {
    RailUnit *unit = FieldRailSystem_GetCameraParentUnit(rail);
    RailCamera *cam = GetRailCameraByCamHandler(unit);
    FieldCamera *camera = FieldRailSystem_GetFieldCamera(rail);
    VecFx16 dir;
    VecFx16 flat;
    s32 yaw;
    s32 pitch;

    FieldCamera_SetTransformType(camera, 0);
    FieldCamera_ResetBind(FieldRailSystem_GetFieldCamera(rail));
    func_ov021_021b8050(unit, &dir.x);
    yaw = FX_Atan2Idx(-dir.x, -dir.z);
    flat.x = dir.x;
    flat.z = dir.z;
    flat.y = 0;
    pitch = FX_Atan2Idx(-dir.y, VEC_Fx16Mag(&flat));
    FieldCamera_CoordsSetPitch(camera, pitch + cam->unk_08);
    FieldCamera_CoordsSetZoom(camera, cam->unk_0C);
    FieldCamera_CoordsSetYaw(camera, yaw + cam->unk_04);
}

void func_ov021_021bab30(FieldRailSystem *param_1) {
    VecFx32 spC;
    VecFx16 sp4;
    s32 sp0;
    FieldCamera *camera;
    RailUnit *unit;
    RailCamera *railCameraByCamHandler;

    unit = FieldRailSystem_GetCameraParentUnit(param_1);
    railCameraByCamHandler = GetRailCameraByCamHandler(unit);
    camera = FieldRailSystem_GetFieldCamera(param_1);
    FieldCamera_SetTransformType(camera, 0U);
    FieldCamera_ClearBind(FieldRailSystem_GetFieldCamera(param_1));
    func_ov021_021b8050(unit, &sp4.x);
    sp0 = (s32) FX_Atan2Idx(-sp4.x, -sp4.z);
    FieldCamera_CoordsSetPitch(camera, (u16) railCameraByCamHandler->unk_08);
    FieldCamera_CoordsSetZoom(camera, railCameraByCamHandler->unk_0C);
    FieldCamera_CoordsSetYaw(camera, (u16) (sp0 + railCameraByCamHandler->unk_04));
    func_ov021_021b8314(unit, &spC);
    FieldCamera_CoordsSetTarget(FieldRailSystem_GetFieldCamera(param_1), &spC);
}

void func_ov021_021babb4(FieldRailSystem *param_1) {
    u16 result;
    RailUnit *unit;
    RailPoint *pRVar2;

    unit = FieldRailSystem_GetCameraParentUnit(param_1);
    result = RailUnit_IsOnLine(unit);
    pRVar2 = GetCurrentRailLinePoint1(unit);
    if (result == 1) {
        func_ov021_021bb014(param_1, unit, pRVar2);
    }
}

void func_ov021_021babdc(FieldRailSystem *param_1) {
    u16 result;
    RailUnit *unit;
    RailPoint *pRVar2;

    unit = FieldRailSystem_GetCameraParentUnit(param_1);
    result = RailUnit_IsOnLine(unit);
    pRVar2 = GetCurrentRailLinePoint2(unit);
    if (result == 1) {
        func_ov021_021bb014(param_1, unit, pRVar2);
    }
}

void func_ov021_021bac04(FieldRailSystem *rail) {
    FieldCamera *camera;
    RailCamera *cam;
    VecFx32 pos;
    VecFx32 offset;
    u16 pitch;
    u16 yaw;
    s32 zoom;

    camera = FieldRailSystem_GetFieldCamera(rail);
    cam = GetRailCameraByCamHandler(FieldRailSystem_GetCameraParentUnit(rail));
    FieldCamera_SetTransformType(camera, 0);
    FieldCamera_ResetBind(FieldRailSystem_GetFieldCamera(rail));
    FieldRailSystem_GetCameraParentPos(rail, &pos);
    pitch = cam->unk_04;
    yaw = cam->unk_08;
    zoom = cam->unk_0C;
    FieldCamera_CoordsSetYaw(camera, yaw);
    FieldCamera_CoordsSetPitch(camera, pitch);
    FieldCamera_CoordsSetZoom(camera, zoom);
    offset.x = cam->unk_10;
    offset.y = cam->unk_14;
    offset.z = cam->unk_18;
    FieldCamera_CoordsSetTargetOffset(camera, &offset);
}

void func_ov021_021bac74(FieldRailSystem *param_1) {
    FieldCamera *camera;
    VecFx32 VStack_18;

    camera = FieldRailSystem_GetFieldCamera(param_1);
    func_ov021_021bac04(param_1);
    FieldCamera_ClearBind(camera);
    FieldRailSystem_GetCameraParentPos(param_1, &VStack_18);
    FieldCamera_CoordsSetTarget(camera, &VStack_18);
    FieldCamera_EnableDelay(camera);
}

static inline u16 LerpRailAngle(u16 from, u16 to, fx32 t) {
    s32 diff = to - from;

    if (diff >= 0x8000) {
        diff -= 0x10000;
    } else if (diff <= -0x8000) {
        diff += 0x10000;
    }
    return from + (FX_Mul(diff << 12, t) >> 12);
}
void func_ov021_021baca8(FieldRailSystem *rail) {
    RailUnit *unit = FieldRailSystem_GetCameraParentUnit(rail);
    RailPoint *p0 = GetCurrentRailLinePoint1(unit);
    RailPoint *p1 = GetCurrentRailLinePoint2(unit);
    FieldCamera *camera = FieldRailSystem_GetFieldCamera(rail);
    RailCamera *c1;
    RailCamera *c0;
    s32 length;
    s32 posV;
    fx32 t;
    s32 pitch0;
    s32 yaw0;
    s32 zoom0;
    s32 yaw1;
    s32 zoom1;
    u16 pitch;
    s32 diff;
    VecFx32 offset;

    FieldCamera_SetTransformType(camera, 0);
    FieldCamera_ResetBind(FieldRailSystem_GetFieldCamera(rail));
    c0 = GetRailCameraByPoint(unit, p0);
    c1 = GetRailCameraByPoint(unit, p1);
    length = GetCurrentLineLength(unit);
    posV = GetCurrentLinePosV(unit);
    t = FX_Div(posV << 12, length << 12);
    pitch0 = (u16)c0->unk_04;
    yaw0 = (u16)c0->unk_08;
    zoom0 = c0->unk_0C;
    yaw1 = (u16)c1->unk_08;
    zoom1 = c1->unk_0C;
    diff = (u16)c1->unk_04 - pitch0;
    if (diff >= 0x8000) {
        diff -= 0x10000;
    } else if (diff <= -0x8000) {
        diff += 0x10000;
    }
    pitch = pitch0 + (FX_Mul(diff << 12, t) >> 12);
    diff = yaw1 - yaw0;
    if (diff >= 0x8000) {
        diff -= 0x10000;
    } else if (diff <= -0x8000) {
        diff += 0x10000;
    }
    FieldCamera_CoordsSetYaw(camera, yaw0 + (FX_Mul(diff << 12, t) >> 12));
    FieldCamera_CoordsSetPitch(camera, pitch);
    FieldCamera_CoordsSetZoom(camera, zoom0 + FX_Mul(zoom1 - zoom0, t));
    offset.x = c0->unk_10 + FX_Mul(c1->unk_10 - c0->unk_10, t);
    offset.y = c0->unk_14 + FX_Mul(c1->unk_14 - c0->unk_14, t);
    offset.z = c0->unk_18 + FX_Mul(c1->unk_18 - c0->unk_18, t);
    FieldCamera_CoordsSetTargetOffset(camera, &offset);
}

BOOL func_ov021_021bae7c(RailPoint *p0, RailPoint *p1, const VecFx32 *pos, VecFx32 *out, fx32 *along,
                         VecFx32 *cross) {
    VecFx32 lineDir;
    VecFx32 toPos;
    VecFx32 ndir;
    VecFx32 a;
    VecFx32 b;
    fx32 len;
    fx32 mag;
    fx32 ratio;
    fx32 dot;
    fx32 dy;
    fx32 mult;
    fx32 x;
    fx32 y;
    fx32 z;

    VEC_Subtract(&p0->pos, &p1->pos, &lineDir);
    VEC_Subtract(pos, &p1->pos, &toPos);
    VEC_Normalize(&lineDir, &ndir);
    len = VEC_Mag(&lineDir);
    lineDir = ndir;
    mag = VEC_Mag(&toPos);
    VEC_Normalize(&toPos, &toPos);
    ratio = FX_Div(mag, len);
    toPos.x = FX_Mul(toPos.x, ratio);
    toPos.y = FX_Mul(toPos.y, ratio);
    toPos.z = FX_Mul(toPos.z, ratio);
    dot = VEC_DotProduct(&lineDir, &toPos);
    *along = dot;
    if (dot > 0x1000) {
        return FALSE;
    }
    if (dot < 0) {
        return FALSE;
    }
    *along = FX_Mul(dot, len);
    VEC_Normalize(&lineDir, &a);
    VEC_Normalize(&toPos, &b);
    VEC_CrossProduct(&a, &b, cross);
    mult = *along;
    z = FX_Mul(ndir.z, mult);
    y = FX_Mul(ndir.y, mult);
    x = FX_Mul(ndir.x, mult);
    out->x = x;
    out->y = y;
    out->z = z;
    VEC_Add(&p1->pos, out, out);
    if (VEC_Distance(pos, out) >= 0x100000) {
        return FALSE;
    }
    dy = pos->y - out->y;
    if (dy < 0) {
        dy = -dy;
    }
    if (dy > 0x8000) {
        return FALSE;
    }
    return TRUE;
}

void func_ov021_021bb014(FieldRailSystem *param_1, RailUnit *param_2, RailPoint *param_3) {
    RailPoint *pRVar1;
    RailCamera *railCameraByPoint;
    FieldCamera *camera;
    u32 value;
    VecFx32 VStack_20;

    pRVar1 = GetCurrentRailLinePoint1(param_2);
    if (pRVar1 == param_3) {
        value = 0;
    }
    else {
        value = GetCurrentLineLength(param_2);
    }
    func_ov021_021b8328(param_2, value, &VStack_20);
    railCameraByPoint = GetRailCameraByPoint(param_2, param_3);
    camera = FieldRailSystem_GetFieldCamera(param_1);
    FieldCamera_SetTransformType(camera, 0);
    FieldCamera_ClearBind(camera);
    FieldCamera_CoordsSetTarget(camera, &VStack_20);
    FieldCamera_CoordsSetPitch(camera, (u16)railCameraByPoint->unk_04);
    FieldCamera_CoordsSetYaw(camera, (u16)railCameraByPoint->unk_08);
    FieldCamera_CoordsSetZoom(camera, railCameraByPoint->unk_0C);
}
