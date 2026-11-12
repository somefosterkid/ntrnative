// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/field_actor_tool.c, ported by tools/port_file.py.
#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_actor_internal.h"
#include "field/field_effect.h"
#include "field/field_g3d_mapper.h"
#include "field/field_map.h"
#include "field/zone.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/math.h"
#include "constants/directions.h"

// The grid height of a height on the map
#define HEIGHT_TO_GRID(height) ((s16)(((height) >> 4) / FX32_ONE))

static const u16 data_ov010_02171458[4] = { 0x8000, 0x0000, 0x4000, 0xc000 };

static BOOL (*const TILE_EXIT_BLOCK_CHECKS[4])(u16 tileClass) = {
    TileExitBlockCheck_Up,
    TileExitBlockCheck_Down,
    TileExitBlockCheck_Left,
    TileExitBlockCheck_Right,
};

static BOOL (*const TILE_ENTER_BLOCK_CHECKS[4])(u16 tileClass) = {
    TileExitBlockCheck_Down,
    TileExitBlockCheck_Up,
    TileExitBlockCheck_Right,
    TileExitBlockCheck_Left,
};

static const s32 DIRECTION_VEC_X[4] = { 0, 0, -1, 1 };

static const s32 DIRECTION_VEC_Z[4] = { -1, 1, 0, 0 };

static const u32 INV_DIR_TABLE[4] = { DIR_DOWN, DIR_UP, DIR_RIGHT, DIR_LEFT };

static const u8 data_ov010_02171460[16] = { 0, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 0 };

static const u8 data_ov010_021714c0[4][4] = {
    { 0, 1, 2, 3 },
    { 3, 2, 0, 1 },
    { 1, 0, 3, 2 },
    { 2, 3, 1, 0 },
};

void func_ov010_02163010(FieldActor *actor) {
    func_ov010_0216d6f4(actor);
    func_ov021_0219b414(actor);
    SetActorFlag(actor, 2);
}

void func_ov010_02163028(FieldActor *actor) {
    if (func_ov010_0216da20(actor, 2) == 0) {
        func_ov010_021630f4(actor);
        func_ov010_0216310c(actor);
        func_ov010_02163130(actor);
        if (CheckActorMovementFlag(actor, 0x200)) {
            func_ov021_02197eb4(actor);
        } else if (func_ov010_0216db10(actor) == FALSE && func_ov010_02163090(actor) == TRUE &&
                   func_ov021_0219b41c(actor) == FALSE) {
            func_ov010_0216d708(actor);
        }
        func_ov010_02163154(actor);
        func_ov010_02163188(actor);
    }
}

BOOL func_ov010_02163090(FieldActor *actor) {
    u32 flags;

    if (IsActorFlag16(actor) == FALSE) {
        if (CheckActorMovementFlag(actor, 0x1800) == 0) {
            return TRUE;
        }
        if (GetActorMoveCode(actor) == 0x32) {
            return TRUE;
        }
        flags = func_ov010_0216d564(actor);
        if ((flags & 0x1000) && func_ov010_0216db68(actor) == FALSE) {
            return FALSE;
        }
        if ((flags & 0x800) && func_ov010_0216dcb4(actor) == FALSE) {
            return FALSE;
        }
    }
    return TRUE;
}

void func_ov010_021630f4(FieldActor *actor) {
    if (CheckActorMovementFlag(actor, 0x1000)) {
        CheckRecalcActorY(actor);
    }
}

void func_ov010_0216310c(FieldActor *actor) {
    if (CheckActorMovementFlag(actor, 0x800) && FldAct_CacheTerrainInfo(actor) == TRUE) {
        func_ov010_0216da70(actor);
    }
}

void func_ov010_02163130(FieldActor *actor) {
    if (CheckActorMovementFlag(actor, 0x20)) {
        func_ov010_021641c8(actor);
        func_ov010_02163230(actor);
    }
    ClearActorMovementFlag(actor, 0xa0);
}

void func_ov010_021632f0(FieldActor *actor);
void func_ov010_02163288(FieldActor *actor);
void func_ov010_02163154(FieldActor *actor) {
    if (CheckActorMovementFlag(actor, 0x80)) {
        func_ov010_021632f0(actor);
    } else if (CheckActorMovementFlag(actor, 0x20)) {
        func_ov010_02163288(actor);
    }
    ClearActorMovementFlag(actor, 0xa0);
}

void func_ov010_02163340(FieldActor *actor);
void func_ov010_02163188(FieldActor *actor) {
    if (CheckActorMovementFlag(actor, 0x100)) {
        func_ov010_021633a8(actor);
    } else if (CheckActorMovementFlag(actor, 0x40)) {
        func_ov010_02163340(actor);
    }
    ClearActorMovementFlag(actor, 0x140);
}




void ActorTerrainEffect_InitParam(FieldActor *actor, ActorTerrainEffectParam *param) {
    MI_CpuFill(param, 0, sizeof(ActorTerrainEffectParam));
    param->tileTypeOrigY = GetCachedOrigYTileUnderActor(actor);
    param->tileType = GetCachedTileUnderActor(actor);
    if (param->tileType != -1) {
        param->tileClass = GetTileClass(param->tileType);
        param->tileFlags = GetTileFlags(param->tileType);
    }
    if (param->tileTypeOrigY != -1) {
        param->tileClassOrigY = GetTileClass(param->tileTypeOrigY);
        param->tileFlagsOrigY = GetTileFlags(param->tileTypeOrigY);
    }
    param->config = GetActorMdlInfo(actor);
    param->effects = Field_GetFieldEffects(GetMMSysField(GetActorMModelSystem(actor)));
    param->season = FieldEffects_GetSeason(param->effects);
}

void func_ov010_02163230(FieldActor *actor) {
    ActorTerrainEffectParam param;

    FldAct_CacheTerrainInfo(actor);
    if (CheckActorMovementFlag(actor, 0x800) == FALSE && func_ov010_0216db24(actor) == TRUE) {
        ActorTerrainEffect_InitParam(actor, &param);
        func_ov010_021634d8(actor, &param);
        func_ov010_02163618(actor, &param);
        func_ov010_021636f0(actor, &param);
        func_ov010_02163418(actor, &param);
        func_ov010_021637e4(actor, &param);
    }
}

void ActorTerrainEffect_TallGrass(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_02163580(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_021636f0(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_021637e4(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_021638c8(FieldActor *actor, ActorTerrainEffectParam *param);

void func_ov010_02163288(FieldActor *actor) {
    ActorTerrainEffectParam param;

    FldAct_CacheTerrainInfo(actor);
    if (CheckActorMovementFlag(actor, 0x800) == FALSE && func_ov010_0216db24(actor) == TRUE) {
        ActorTerrainEffect_InitParam(actor, &param);
        ActorTerrainEffect_TallGrass(actor, &param);
        func_ov010_02163580(actor, &param);
        func_ov010_02163650(actor, &param);
        func_ov010_021636f0(actor, &param);
        func_ov010_0216377c(actor, &param);
        func_ov010_021637e4(actor, &param);
        func_ov010_021638c8(actor, &param);
    }
}


void func_ov010_0216369c(FieldActor *actor, ActorTerrainEffectParam *param);

void func_ov010_021632f0(FieldActor *actor) {
    ActorTerrainEffectParam param;

    FldAct_CacheTerrainInfo(actor);
    if (CheckActorMovementFlag(actor, 0x800) == FALSE && func_ov010_0216db24(actor) == TRUE) {
        ActorTerrainEffect_InitParam(actor, &param);
        func_ov010_021636f0(actor, &param);
        func_ov010_021637e4(actor, &param);
        func_ov010_0216369c(actor, &param);
        func_ov010_021638c8(actor, &param);
    }
}

void func_ov010_02163340(FieldActor *actor) {
    ActorTerrainEffectParam param;

    FldAct_CacheTerrainInfo(actor);
    if (CheckActorMovementFlag(actor, 0x800) == FALSE && func_ov010_0216db24(actor) == TRUE) {
        ActorTerrainEffect_InitParam(actor, &param);
        func_ov010_02163418(actor, &param);
        func_ov010_0216352c(actor, &param);
        func_ov010_021637b0(actor, &param);
        func_ov010_02163650(actor, &param);
        func_ov010_02163868(actor, &param);
        func_ov010_0216372c(actor, &param);
        func_ov010_021638ac(actor, &param);
    }
}

void func_ov010_021633a8(FieldActor *actor) {
    ActorTerrainEffectParam param;

    FldAct_CacheTerrainInfo(actor);
    if (CheckActorMovementFlag(actor, 0x800) == FALSE && func_ov010_0216db24(actor) == TRUE) {
        ActorTerrainEffect_InitParam(actor, &param);
        func_ov010_02163418(actor, &param);
        func_ov010_021637b0(actor, &param);
        func_ov010_02163650(actor, &param);
        func_ov010_02163868(actor, &param);
        func_ov010_0216372c(actor, &param);
        func_ov010_0216355c(actor, &param);
        func_ov010_0216375c(actor, &param);
        func_ov010_021638ac(actor, &param);
    }
}





void func_ov010_02163418(FieldActor *actor, ActorTerrainEffectParam *param) {
    if (func_ov010_0216dc00(actor) == FALSE && MapTile_IsDeepSand(param->tileClass)) {
        // How far an actor sinks into deep sand
        VecFx32 offset = { 0, -FX32_CONST(7), 0 };
        func_ov010_0216d900(actor, &offset);
    } else {
        VecFx32 offset = { 0, 0, 0 };
        func_ov010_0216d900(actor, &offset);
    }
}

u32 func_ov010_02163464(u32 tileClass, u8 season) {
    u32 kind = 0;

    if (func_ov021_021ab4ec(tileClass)) {
        kind = 6;
    } else if (func_ov021_021ab4f8(tileClass)) {
        kind = 7;
    } else if (func_ov021_021ab434(tileClass)) {
        kind = 1;
        if (season == 3) {
            kind = 5;
        }
    } else if (func_ov021_021ab428(tileClass)) {
        if (season == 3) {
            kind = 4;
        }
    } else if (MapTile_IsNormalTallGrassDoubleBtl(tileClass)) {
        kind = 1;
    } else if (MapTile_IsReallyTallGrassSingleBtl(tileClass)) {
        kind = 2;
    } else if (MapTile_IsReallyTallGrassDoubleBtl(tileClass)) {
        kind = 3;
    }
    return kind;
}


void func_ov010_021634d8(FieldActor *actor, ActorTerrainEffectParam *param) {
    if (param->tileFlags & 0x20) {
        func_ov021_021ac760(param->effects, actor, FALSE, func_ov010_02163464(param->tileClass, param->season));
    }
}

void ActorTerrainEffect_TallGrass(FieldActor *actor, ActorTerrainEffectParam *param) {
    u16 dir;
    u32 kind;

    if (param->tileFlags & 0x20) {
        dir = GetActorMotionDir(actor);
        kind = func_ov010_02163464(param->tileClass, param->season);
        if (dir != DIR_UP) {
            func_ov021_021ac760(param->effects, actor, TRUE, kind);
        }
    }
}

void func_ov010_0216352c(FieldActor *actor, ActorTerrainEffectParam *param) {
    u16 dir;
    u32 kind;

    if (param->tileFlags & 0x20) {
        dir = GetActorMotionDir(actor);
        kind = func_ov010_02163464(param->tileClass, param->season);
        if (dir == DIR_UP) {
            func_ov021_021ac760(param->effects, actor, TRUE, kind);
        }
    }
}

void func_ov010_0216355c(FieldActor *actor, ActorTerrainEffectParam *param) {
    if (param->tileFlags & 0x20) {
        func_ov021_021ac760(param->effects, actor, TRUE, func_ov010_02163464(param->tileClass, param->season));
    }
}

BOOL func_ov036_021a2ce0(u32 tileClass);
BOOL func_ov036_021a2cec(u32 tileClass);
void func_ov010_02163580(FieldActor *actor, ActorTerrainEffectParam *param) {
    u32 kind;

    if (param->tileFlagsOrigY & 8) {
        kind = 5;
        if (func_ov021_021ab3d4(param->tileClassOrigY)) {
            if (param->season == 3) {
                kind = 2;
                if (param->config->footprintType == 2) {
                    kind = 3;
                }
            }
        } else if (func_ov021_021ab3e0(param->tileClassOrigY)) {
            if (param->season == 0 || param->season == 3) {
                kind = 2;
                if (param->config->footprintType == 2) {
                    kind = 3;
                }
            }
        } else if (MapTile_IsSnow(param->tileClassOrigY) == TRUE || MapTile_IsSnowNoCycling(param->tileClassOrigY) == TRUE) {
            kind = 2;
            if (param->config->footprintType == 2) {
                kind = 3;
            }
        } else if (MapTile_IsDeepSand(param->tileClassOrigY) == TRUE) {
            kind = 4;
        } else {
            kind = 0;
            if (param->config->footprintType == 2) {
                kind = 1;
            }
        }
        if (kind != 5) {
            func_ov021_021bbdd4(actor, param->effects, kind);
        }
    }
}


void func_ov021_021c4be0(FieldEffects *effects, FieldActor *actor);
void func_ov010_02163618(FieldActor *actor, ActorTerrainEffectParam *param) {
    if (func_ov021_021ab304(param->tileClass) == TRUE) {
        if (func_ov010_0216dbe8(actor) == FALSE) {
            func_ov021_021c4be0(param->effects, actor);
            func_ov010_0216dbcc(actor, TRUE);
        }
    } else {
        func_ov010_0216dbcc(actor, FALSE);
    }
}

void func_ov021_021c4a10(FieldEffects *effects, FieldActor *actor);
void func_ov010_02163650(FieldActor *actor, ActorTerrainEffectParam *param) {
    if (func_ov021_021ab304(param->tileClass) == TRUE) {
        if (func_ov010_0216dbe8(actor) == FALSE) {
            func_ov021_021c4be0(param->effects, actor);
            func_ov010_0216dbcc(actor, TRUE);
        }
    } else if (MapTile_IsSwamp(param->tileClass) == TRUE) {
        func_ov021_021c4a10(param->effects, actor);
    } else {
        func_ov010_0216dbcc(actor, FALSE);
    }
}



void func_ov010_0216369c(FieldActor *actor, ActorTerrainEffectParam *param) {
    func_ov010_0216dbcc(actor, FALSE);
}

BOOL func_ov010_021636a8(FieldActor *actor, ActorTerrainEffectParam *param) {
    if (func_ov010_0216d98c(GetActorMModelSystem(actor)) == FALSE || param->config->enableShadow == FALSE) {
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov010_021636c8(ActorTerrainEffectParam *param) {
    if (!(param->tileFlags & 0x80)) {
        return FALSE;
    }
    if (param->season == 3 && func_ov021_021ab3d4(param->tileClass)) {
        return FALSE;
    }
    return TRUE;
}

void func_ov010_021636f0(FieldActor *actor, ActorTerrainEffectParam *param) {
    if (func_ov010_021636a8(actor, param) == TRUE && func_ov010_021636c8(param) == TRUE &&
        CheckActorMovementFlag(actor, 0x4000) == 0) {
        func_ov021_021ac2c0(actor, param->effects);
        SetActorMovementFlag(actor, 0x4000);
    }
}

void func_ov010_0216372c(FieldActor *actor, ActorTerrainEffectParam *param) {
    if (func_ov010_021636a8(actor, param) == TRUE) {
        if (func_ov010_021636c8(param) == FALSE) {
            SetActorMovementFlag(actor, 0x8000);
        } else {
            ClearActorMovementFlag(actor, 0x8000);
        }
    }
}

void func_ov010_0216375c(FieldActor *actor, ActorTerrainEffectParam *param) {
    if (!(param->tileFlags & 2) && func_ov010_0216dd04(actor) == FALSE) {
        func_ov021_021ac528(actor, param->effects);
    }
}

void func_ov021_021c484c(FieldEffects *effects, s16 x, s16 z, fx32 y);
void func_ov010_0216377c(FieldActor *actor, ActorTerrainEffectParam *param) {
    if (func_ov021_021ab2f8(param->tileClassOrigY) == TRUE) {
        func_ov021_021c484c(param->effects, FldAct_GetInitGPosX(actor), FldAct_GetInitGPosZ(actor), GetActorPosY(actor));
    }
}

void func_ov010_021637b0(FieldActor *actor, ActorTerrainEffectParam *param) {
    if (func_ov021_021ab2f8(param->tileClass) == TRUE) {
        func_ov021_021c484c(param->effects, GetGPosX(actor), GetGPosZ(actor), GetActorPosY(actor));
    }
}

void func_ov010_021637e4(FieldActor *actor, ActorTerrainEffectParam *param) {
    u32 tileType;
    u32 nextTileType;
    u32 tileClass;
    u32 kind;
    MMSys *system;

    if (param->config->enableReflections && FldAct_CheckReflectingFlag(actor) == FALSE) {
        tileType = -1;
        if (param->tileFlags & 0x40) {
            tileType = param->tileType;
        } else {
            nextTileType = func_ov010_02163f40(actor, 1);
            if (GetTileFlags(nextTileType) & 0x40) {
                tileType = nextTileType;
            }
        }
        if (tileType != -1) {
            kind = 1;
            tileClass = GetTileClass(tileType);
            system = GetActorMModelSystem(actor);
            if (func_ov021_021ab440(tileClass) == TRUE || func_ov021_021ab358(tileClass) == TRUE) {
                kind = 2;
            }
            func_ov021_021bbfb8(system, actor, param->effects, kind);
            FldAct_SetReflectingFlag(actor, TRUE);
        }
    }
}




void func_ov010_02163868(FieldActor *actor, ActorTerrainEffectParam *param) {
    u32 tileType;

    if (param->config->enableReflections && FldAct_CheckReflectingFlag(actor)) {
        tileType = func_ov010_02163f40(actor, DIR_DOWN);
        if (tileType == -1) {
            FldAct_SetReflectingFlag(actor, FALSE);
        } else if (!(GetTileFlags(tileType) & 0x40)) {
            FldAct_SetReflectingFlag(actor, FALSE);
        }
    }
}

void func_ov021_021c8f7c(FieldActor *actor, FieldEffects *effects);
void func_ov010_021638ac(FieldActor *actor, ActorTerrainEffectParam *param) {
    if (MapTile_IsElectricField(param->tileClass) == TRUE) {
        func_ov021_021c8f7c(actor, param->effects);
    }
}


void func_ov010_021638c8(FieldActor *actor, ActorTerrainEffectParam *param) {
}


u32 ActorRouteCollCheckCore(FieldActor *actor, const VecFx32 *position, s16 x, s16 y, s16 z, u16 dir) {
    u32 result;
    fx32 height;
    fx32 diff;
    VecFx32 newPosition;

    result = 0;
    newPosition.x = (x << 16) + 0x8000;
    newPosition.y = position->y;
    newPosition.z = (z << 16) + 0x8000;
    if (IsGPosOutsideActorWalkArea(actor, x, z) == TRUE) {
        result |= 1;
    }
    if (CheckBlockedCollPathToPosition(actor, dir, newPosition) == TRUE) {
        result |= 2;
    }
    if (GetHeightFromMap(actor, &newPosition, &height) == TRUE) {
        diff = position->y - height;
        if (diff < 0) {
            diff = -diff;
        }
        if (diff >= FX32_CONST(20)) {
            result |= 8;
        }
        y = HEIGHT_TO_GRID(height);
    } else {
        result |= 8;
    }
    if (CheckActorNewPosOtherActorCollision(actor, x, y, z) == TRUE) {
        result |= 4;
    }
    if (FieldG3DMapper_IsPosOutOfBounds(GetMMSysG3DMapper(GetActorMModelSystem(actor)), &newPosition) == TRUE) {
        result |= 0x10;
    }
    return result;
}

u32 ActorRouteCollCheck(FieldActor *actor, s16 x, s16 y, s16 z, u16 dir) {
    VecFx32 position;

    CopyActorWPos(actor, &position);
    return ActorRouteCollCheckCore(actor, &position, x, y, z, dir);
}

u32 ActorRouteCollCheckOneTileInDir(FieldActor *actor, u16 dir) {
    s16 x = GetGPosX(actor) + GetDirectionVectorCompX(dir);
    s16 y = FldAct_GetGPosY(actor);
    s16 z = GetGPosZ(actor) + GetDirectionVectorCompZ(dir);

    return ActorRouteCollCheck(actor, x, y, z, dir);
}

void func_ov010_02163a08(FieldActor *actor, s16 x, s16 y, s16 *rect) {
    s16 w = GetActorCollWidth(actor) - 1;
    s16 h;

    if (w < 0) {
        w = 0;
    }
    rect[0] = x;
    rect[2] = x + w;
    h = GetActorCollHeight(actor) - 1;
    if (h < 0) {
        h = 0;
    }
    rect[1] = y - h;
    rect[3] = y;
}

BOOL func_ov010_02163a40(s16 *a, s16 *b) {
    s16 len[2];
    s16 p0;
    s16 p1;
    s16 q0;
    s16 q1;

    len[0] = a[2] - a[0];
    if (len[0] < 0) {
        len[0] = -len[0];
    }
    len[1] = b[2] - b[0];
    if (len[1] < 0) {
        len[1] = -len[1];
    }
    if (len[0] > len[1]) {
        p0 = a[0];
        p1 = a[2];
        q0 = b[0];
        q1 = b[2];
    } else {
        p0 = b[0];
        p1 = b[2];
        q0 = a[0];
        q1 = a[2];
    }
    if ((p0 <= q0 && p1 >= q0) || (p0 <= q1 && p1 >= q1)) {
        len[0] = a[3] - a[1];
        if (len[0] < 0) {
            len[0] = -len[0];
        }
        len[1] = b[3] - b[1];
        if (len[1] < 0) {
            len[1] = -len[1];
        }
        if (len[0] > len[1]) {
            p0 = a[1];
            p1 = a[3];
            q0 = b[1];
            q1 = b[3];
        } else {
            p0 = b[1];
            p1 = b[3];
            q0 = a[1];
            q1 = a[3];
        }
        if ((p0 <= q0 && p1 >= q0) || (p0 <= q1 && p1 >= q1)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov010_02163af0(FieldActor *src, FieldActor *actor, s16 x, s16 y, s16 z, s32 flag) {
    s16 rectA[4];
    s16 rectB[4];
    s16 d;

    func_ov010_02163a08(src, x, z, rectA);
    if (CheckActorFlag(actor, 0x80) == FALSE) {
        func_ov010_02163a08(actor, GetGPosX(actor), GetGPosZ(actor), rectB);
        if (func_ov010_02163a40(rectA, rectB) != FALSE) {
            d = y - FldAct_GetGPosY(actor);
            if (d < 0) {
                d = -d;
            }
            if (d < 1) {
                return TRUE;
            }
        }
        if (flag == 1) {
            func_ov010_02163a08(actor, FldAct_GetInitGPosX(actor), FldAct_GetInitGPosZ(actor), rectB);
            if (func_ov010_02163a40(rectA, rectB) != FALSE) {
                d = y - FldAct_GetInitGPosY(actor);
                if (d < 0) {
                    d = -d;
                }
                if (d < 1) {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}




// How far an actor's collision reaches past its position
static inline s16 GetCollExtent(u8 size) {
    return size < 2 ? 0 : size - 1;
}

// Whether an area of the grid overlaps an actor's collision at a position
static inline BOOL CheckAreaOverActor(FieldActor *actor, const GPosXYZ *pos, s16 minX, s16 maxX, s16 minZ, s16 maxZ) {
    s16 extent = GetCollExtent(actor->collisionWidth);

    if (minX > pos->x + extent || maxX < pos->x) {
        return FALSE;
    }
    extent = GetCollExtent(actor->collisionHeight);
    if (minZ > pos->z || maxZ < pos->z - extent) {
        return FALSE;
    }
    return TRUE;
}

BOOL CheckActorNewPosOtherActorCollision(FieldActor *actor, s16 x, s16 y, s16 z) {
    MMSys *system;
    u32 index;
    FieldActor *other;

    index = 0;
    system = GetActorMModelSystem(actor);
    if (NextActor(system, &other, &index) == TRUE) {
        do {
            if (actor != other && func_ov010_02163af0(actor, other, x, y, z, 1) == TRUE) {
                return TRUE;
            }
        } while (NextActor(system, &other, &index) == TRUE);
    }
    return FALSE;
}

BOOL CheckActorVolumeOverGPos(FieldActor *actor, s16 x, s16 z, BOOL checkInit) {
    s16 rect[4];

    func_ov010_02163a08(actor, GetGPosX(actor), GetGPosZ(actor), rect);
    if (rect[0] <= x && rect[2] >= x && rect[1] <= z && rect[3] >= z) {
        return TRUE;
    }
    if (checkInit == TRUE) {
        func_ov010_02163a08(actor, FldAct_GetInitGPosX(actor), FldAct_GetInitGPosZ(actor), rect);
        if (rect[0] <= x && rect[2] >= x && rect[1] <= z && rect[3] >= z) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL IsGPosOutsideActorWalkArea(FieldActor *actor, s16 x, s16 z) {
    s16 center;
    s16 range;
    s16 min;
    s16 max;

    center = GetActorDefaultGPosX(actor);
    range = GetActorWalkAreaW(actor);
    if (range != -1) {
        max = center + range;
        min = center - range;
        if (min > x || max < x) {
            return TRUE;
        }
    }
    center = GetActorDefaultGPosZ(actor);
    range = GetActorWalkAreaH(actor);
    if (range != -1) {
        max = center + range;
        min = center - range;
        if (min > z || max < z) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CheckBlockedCollPathToPosition(FieldActor *actor, u16 dir, VecFx32 position) {
    u8 width;
    u32 firstTileType;
    BOOL found;
    u8 j;
    u8 i;
    u32 tileType;
    u8 height;
    u16 tileClass;
    VecFx32 rowPosition;
    VecFx32 tilePosition;

    if (func_ov010_0216dcb4(actor) == FALSE) {
        found = FALSE;
        width = GetActorCollWidth(actor);
        height = GetActorCollHeight(actor);
        rowPosition = position;
        for (j = 0; j < height; j++, rowPosition.z -= FX32_CONST(16)) {
            for (i = 0, tilePosition = rowPosition; i < width; i++, tilePosition.x += FX32_CONST(16)) {
                if (GetTileTypeAtPosByActor(actor, &tilePosition, &tileType) == FALSE) {
                    return TRUE;
                }
                if (MapTile_BlocksCollision(tileType) == TRUE) {
                    return TRUE;
                }
                if (found == FALSE) {
                    found = TRUE;
                    firstTileType = tileType;
                }
            }
        }
        tileClass = GetTileClass(GetCachedTileUnderActor(actor));
        if (TILE_EXIT_BLOCK_CHECKS[dir](tileClass) == TRUE) {
            return TRUE;
        }
        tileClass = GetTileClass(firstTileType);
        if (TILE_ENTER_BLOCK_CHECKS[dir](tileClass) == TRUE) {
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GetTerrainAtPosByActor(FieldActor *actor, const VecFx32 *position, MapTerrainBuf *terrain) {
    MMSys *system;
    FieldG3DMapper *mapper;

    system = GetActorMModelSystem(actor);
    mapper = GetMMSysG3DMapper(system);
    return FieldG3DMapper_GetTerrain(mapper, position, terrain);
}

BOOL GetTileTypeAtPosByActor(FieldActor *actor, const VecFx32 *position, u32 *tileType) {
    MapTerrainBuf terrain;

    *tileType = 0;
    if (GetTerrainAtPosByActor(actor, position, &terrain) == TRUE) {
        *tileType = terrain.tileType;
        return TRUE;
    }
    return FALSE;
}

BOOL GetHeightFromMap(FieldActor *actor, const VecFx32 *position, fx32 *height) {
    MapTerrainBuf terrain;

    *height = 0;
    if (GetTerrainAtPosByActor(actor, position, &terrain) == TRUE) {
        *height = terrain.height;
        return TRUE;
    }
    return FALSE;
}

void func_ov010_02163e7c(FieldActor *actor, u16 dir) {
    fx32 height;
    VecFx32 position;

    SetActorInitialGPosX(actor, GetGPosX(actor));
    SetActorInitialGPosY(actor, FldAct_GetGPosY(actor));
    SetActorInitialGPosZ(actor, GetGPosZ(actor));
    adjustXPos(actor, GetDirectionVectorCompX(dir));
    adjustYPos(actor, GetDirectionVectorCompZ(dir));
    height = 0;
    ConvGXZToVector(GetGPosX(actor), GetGPosZ(actor), &position);
    position.y = GetActorPosY(actor);
    GetHeightFromMap(actor, &position, &height);
    SetActorGPosY(actor, HEIGHT_TO_GRID(height));
}

void SetActorInitialGPosToNowGPos(FieldActor *actor) {
    SetActorInitialGPosX(actor, GetGPosX(actor));
    SetActorInitialGPosY(actor, FldAct_GetGPosY(actor));
    SetActorInitialGPosZ(actor, GetGPosZ(actor));
}

u32 func_ov010_02163f40(FieldActor *actor, u16 dir) {
    u32 tileType;
    VecFx32 position;

    tileType = -1;
    ConvGXZToVector(GetGPosX(actor), GetGPosZ(actor), &position);
    position.y = GetActorPosY(actor);
    ExpandVecInGridDir(dir, &position, FX32_CONST(16));
    GetTileTypeAtPosByActor(actor, &position, &tileType);
    return tileType;
}

void func_ov010_02163f8c(FieldActor *actor, const VecFx32 *offset) {
    VecFx32 position;

    CopyActorWPos(actor, &position);
    position.x += offset->x;
    position.y += offset->y;
    position.z += offset->z;
    SetActorWPosValue(actor, &position);
}

void func_ov010_02163fc0(FieldActor *actor, u16 dir, fx32 distance) {
    VecFx32 position;

    CopyActorWPos(actor, &position);
    switch (dir) {
    case DIR_UP:
        position.z -= distance;
        break;
    case DIR_DOWN:
        position.z += distance;
        break;
    case DIR_LEFT:
        position.x -= distance;
        break;
    case DIR_RIGHT:
        position.x += distance;
        break;
    }
    SetActorWPosValue(actor, &position);
}

BOOL CheckRecalcActorY(FieldActor *actor) {
    BOOL result;
    fx32 height;
    VecFx32 position;
    VecFx32 terrainPosition;

    CopyActorWPos(actor, &position);
    terrainPosition = position;
    if (CheckActorFlag(actor, 0x2000)) {
        return FALSE;
    }
    if (func_ov010_0216db68(actor) == TRUE) {
        ClearActorMovementFlag(actor, 0x1000);
        return FALSE;
    }
    result = GetHeightFromMap(actor, &terrainPosition, &height);
    if (result == TRUE) {
        position.y = height;
        SetActorWPosValue(actor, &position);
        SetActorInitialGPosY(actor, FldAct_GetGPosY(actor));
        SetActorGPosY(actor, HEIGHT_TO_GRID(height));
        ClearActorMovementFlag(actor, 0x1000);
    } else {
        SetActorMovementFlag(actor, 0x1000);
    }
    return result;
}

BOOL FldAct_CacheTerrainInfo(FieldActor *actor) {
    s32 x;
    s32 y;
    s32 z;
    BOOL foundOrigY;
    BOOL found;
    u32 tileTypeOrigY;
    u32 tileType;
    VecFx32 position;

    foundOrigY = FALSE;
    found = FALSE;
    tileTypeOrigY = -1;
    tileType = -1;
    if (func_ov010_0216dcb4(actor) == FALSE) {
        x = FldAct_GetInitGPosX(actor);
        y = FldAct_GetInitGPosY(actor);
        z = FldAct_GetInitGPosZ(actor);
        position.x = x << 16;
        position.y = y << 16;
        position.z = z << 16;
        foundOrigY = GetTileTypeAtPosByActor(actor, &position, &tileTypeOrigY);
        x = GetGPosX(actor);
        z = GetGPosZ(actor);
        ConvGXZToVector((s16)x, (s16)z, &position);
        position.y = GetActorPosY(actor);
        found = GetTileTypeAtPosByActor(actor, &position, &tileType);
    }
    if (foundOrigY == TRUE) {
        SetCachedOrigYTileUnderActor(actor, tileTypeOrigY);
    }
    if (found == TRUE) {
        SetCachedTileUnderActor(actor, tileType);
    }
    if (found == TRUE) {
        ClearActorMovementFlag(actor, 0x800);
        return TRUE;
    }
    SetActorMovementFlag(actor, 0x800);
    return FALSE;
}

MMSys *GetActorMModelSystem(FieldActor *actor);
FieldActor *FindPlayerFieldActor(MMSys *mmSys);
s16 GetGPosX(FieldActor *actor);
s16 GetGPosZ(FieldActor *actor);



BOOL func_ov010_02164168(FieldActor *actor) {
    FieldActor *player;
    s16 playerX;
    s16 playerZ;
    s16 minX;
    s16 maxX;
    s16 minZ;
    s16 maxZ;
    s16 x;
    s16 z;

    player = FindPlayerFieldActor(GetActorMModelSystem(actor));
    if (player != NULL) {
        playerX = GetGPosX(player);
        playerZ = GetGPosZ(player);
        x = GetGPosX(actor);
        z = GetGPosZ(actor);
        minX = x - 18;
        maxX = x + 17;
        minZ = z - 18;
        maxZ = z + 17;
        if (minZ <= playerZ && maxZ >= playerZ && minX <= playerX && maxX >= playerX) {
            return TRUE;
        }
    }
    return FALSE;
}

u16 GetActorMoveCode(FieldActor *actor);
void func_ov010_0216d708(FieldActor *actor);


void func_ov010_021641c8(FieldActor *actor) {
    u16 moveCode = GetActorMoveCode(actor);

    if ((u16)(moveCode + 0xffcd) <= 3) {
        func_ov010_0216d708(actor);
    }
}

s16 GetDirectionVectorCompX(u32 direction) {
    return DIRECTION_VEC_X[direction];
}

s16 GetDirectionVectorCompZ(u32 direction) {
    return DIRECTION_VEC_Z[direction];
}

void ExpandVecInGridDir(u16 direction, VecFx32 *position, fx32 amount) {
    switch (direction) {
    case 0:
        position->z -= amount;
        break;
    case 1:
        position->z += amount;
        break;
    case 2:
        position->x -= amount;
        break;
    case 3:
        position->x += amount;
        break;
    }
}

void AdjusGridXZByDir(u32 direction, s16 *x, s16 *z, s16 amount) {
    switch (direction) {
    case 0:
        *z = (s16)(*z - amount);
        break;
    case 1:
        *z = (s16)(*z + amount);
        break;
    case 2:
        *x = (s16)(*x - amount);
        break;
    case 3:
        *x = (s16)(*x + amount);
        break;
    }
}

void ConvGXZToVector(s16 x, s16 z, VecFx32 *position) {
    position->x = (x << 16) + 0x8000;
    position->z = (z << 16) + 0x8000;
}

void VecGPosToWPos(s32 x, s32 y, s32 z, VecFx32 *position) {
    position->x = x << 16;
    position->y = y << 16;
    position->z = z << 16;
}

u16 GetInverseDirection(u32 direction) {
    return INV_DIR_TABLE[direction];
}

u16 GetDirFromPosToPos(s32 x1, s32 z1, s32 x2, s32 z2) {
    s32 direction;

    if (x1 > x2) {
        return 2;
    }
    if (x1 < x2) {
        return 3;
    }
    direction = 1;
    if (z1 > z2) {
        direction = 0;
    }
    return direction;
}

u8 func_ov010_021642d4(u32 direction, u16 angle) {
    u16 rotated = data_ov010_02171458[direction] + angle;

    return data_ov010_021714c0[data_ov010_02171460[rotated / 0x1000]][DIR_DOWN];
}


void SetRailActorFlag(FieldActor *actor) {
    SetActorFlag(actor, 0x2000);
}

void InitRailActor(FieldActor *actor, const ZoneNPC *npc) {
    RailPosition position;
    const ZoneNPCRailPosition *rail;

    position.componentId = npc->pos.rail.railIndex;
    position.componentIsLine = TRUE;
    rail = &npc->pos.rail;
    position.railDirection = ConvDirToRailDir((s16)npc->direction);
    position.posFront = rail->frontPos;
    position.posSide = rail->sidePos;
    if (CheckMMSysFlag(GetActorMModelSystem(actor), 0x10)) {
        SetActorInitedPositionRail(actor, &position);
        return;
    }
    ClearActorPositionBlock(actor, sizeof(ActorPositionRail));
    SetActorInitPositionRail(actor, &position);
}

void SetActorInitPositionRail(FieldActor *actor, const RailPosition *position) {
    ActorPositionRail *rail = GetNPCRailPosPtrAddr(actor);

    rail->position = *position;
    rail->unk0 = ACTOR_RAIL_POS_SET;
}

BOOL func_ov010_02164380(FieldActor *actor, RailPosition *position) {
    ActorPositionRail *rail = GetNPCRailPosPtrAddr(actor);

    if (rail->unk0 == ACTOR_RAIL_POS_SET) {
        *position = rail->position;
        return TRUE;
    }
    return FALSE;
}
