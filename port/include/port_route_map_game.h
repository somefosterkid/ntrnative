// The Gen 5 adapter of the route pilot (port/include/port_route_map.h: PortRouteAdapter, kPortRouteGen5 here): included
// once by each game's port/extern*/src/system/game_event.c stand-in, so it compiles with that game's headers (Black's src/
// and include/, Black 2's third_party/pokebw2); the stand-in's frame hook sets sPortMapGsys and registers it
// (PortRouteMap_SetAdapter). It reads the field the way the player's own step check does (ov010/ov012
// field_actor_tool.c's ActorRouteCollCheckCore and CheckBlockedCollPathToPosition, ov021/ov036 field_player_grid.c's
// ledge jump): terrain at the tile, its collision bit, the walls on a tile's sides (TileExitBlockCheck_*), the height step
// (20 units at most), water (no surfing here). Nothing is changed in the game; only called while the pilot asks for it.
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_actor_internal.h"
#include "field/field_g3d_mapper.h"
#include "field/field_map.h"
#include "field/field_nogrid_mapper.h"
#include "field/field_player.h"
#include "field/field_rail.h"
#include "field/field_slope_slip.h"
#include "field/event_data.h"
#include "field/zone.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "save/gimmick_savedata.h"
#include "field/player_action.h"
#include "port_route_map.h"
#if defined(BLACK2) || defined(WHITE2)
#include "save/event_work.h"
#include "field/trainer_script.h"
#include "constants/trainers.h"
#else
#include "field/ov021_021bddc0.h"
#include "field/field_exp_obj.h"
#if defined(BLACK2) || defined(WHITE2)
#include "field/height_ex.h"
#endif
#endif

#if defined(BLACK2) || defined(WHITE2)
#define PORT_WARP_ZONE(w) ((w)->destZoneId)
#define PORT_WARP_DIR(w) ((w)->direction)
#define PORT_RAIL_TRYSTEP FieldRailSystem_TryStepLocation
#define PORT_RAIL_ISSWITCH FieldRailTilemap_IsJumpStart
#define PORT_RAIL_LOCPOS FieldRailSystem_GetLocationPos
#define PORT_SAVED_ROCK_AT IsSavedStrengthRockAtPos
#else
#define PORT_SAVED_ROCK_AT func_ov010_0216e5b0   // Black 2's IsSavedStrengthRockAtPos
#define PORT_WARP_ZONE(w) ((w)->unk0)
#define PORT_WARP_DIR(w) ((w)->unk4)
#define PORT_RAIL_TRYSTEP func_ov021_021b7cd8   // Black 2's FieldRailSystem_TryStepLocation
#define PORT_RAIL_ISSWITCH func_ov021_021bb1a0  // Black 2's FieldRailTilemap_IsJumpStart
#define PORT_RAIL_LOCPOS func_ov021_021b7d38    // Black 2's FieldRailSystem_GetLocationPos
#endif

static GameSystem *sPortMapGsys;

// ---- the ROM's terrain of the chunks the mapper has not loaded (pz19, 2026-11-13) ---------------------------------------------
// The mapper keeps only the chunks near the player loaded; the planner took the rest as '?' (open floor) and guessed, and a
// guess through what then loaded as walls (Giant Chasm's ice, Seaside Cave's water, rooms split over chunks) cost whole legs.
// The terrain is static data of the zone's chunk files (the mapper's chunkArcId archive, file chunkDatIds[chunk]), so a tile of
// an unloaded chunk is read the way the game reads a loaded one: the file read from the archive as FieldChunk_BeginLoad does,
// its format picked by its magic as FieldChunk_ChangeDatID does (FIELD_CHUNK_VTABLES: WB, GC, NG, RD) and sampled with that
// format's own getTerrain at the chunk's place (GenLocators: center halfSpan + chunk * span, a matrix layer's height), every
// layer of the matrix there, the nearest to the height taken as FieldG3DMapper_GetTerrain does. Dynamic state (the Plasma
// Frigate's panels, actors) is not in it: the fill flags these cells PORT_MAP_ROM and route_map.c's Recall puts the cells the
// game showed loaded over them. Black / White / Black 2 / White 2 alike (the archive ids are the mapper's own).
#include <stdlib.h>
#include "gfl/arc.h"
#if defined(BLACK2) || defined(WHITE2)
#define PORT_MAPPER_LAYER_H(mp) ((mp)->layerHeight)
#else
#define PORT_MAPPER_LAYER_H(mp) ((fx32)(mp)->unk08)
extern const FieldChunkVTable FIELD_CHUNK_VTABLES[5];
#endif
#define PORT_ROM_CHUNKS 32   // chunk files kept (a zone's matrix around the player), the oldest dropped first
static struct {
    u32 arc, dat, age;
    void *file;
} sPortRomChunk[PORT_ROM_CHUNKS];
static u32 sPortRomAge;
static u8 sPortMapRom[PORT_MAP_N][PORT_MAP_N];   // the fill's tiles whose terrain came from the ROM
static int sPortRomX0 = -(1 << 20), sPortRomZ0;  // the fill's window (sPortMapRom's origin)

static const void *PortMap_RomChunk(u32 arc, u32 dat) {
    int i, old = 0;
    u32 len;
    void *file;

    for (i = 0; i < PORT_ROM_CHUNKS; i++) {
        if (sPortRomChunk[i].file != NULL && sPortRomChunk[i].arc == arc && sPortRomChunk[i].dat == dat) {
            sPortRomChunk[i].age = ++sPortRomAge;
            return sPortRomChunk[i].file;
        }
        if (sPortRomChunk[i].file == NULL || sPortRomChunk[i].age < sPortRomChunk[old].age) {
            old = i;
        }
    }
    len = GFL_ArcSysGetDataLength(arc, dat);
    if (len < 16 || len > (8u << 20) || (file = malloc(len)) == NULL) {
        return NULL;
    }
    GFL_ArcSysRead(file, arc, dat);
    free(sPortRomChunk[old].file);
    sPortRomChunk[old].file = file;
    sPortRomChunk[old].arc = arc;
    sPortRomChunk[old].dat = dat;
    sPortRomChunk[old].age = ++sPortRomAge;
    return file;
}

// the terrain at pos from the ROM's chunk files (every layer of the matrix there), the layer nearest pos->y; FALSE: none
static BOOL PortMap_RomTerrain(FieldG3DMapper *mapper, const VecFx32 *pos, MapTerrainBuf *t) {
    fx32 span = mapper->chunkSpan, layerH = PORT_MAPPER_LAYER_H(mapper), nearestDistance = 0xfff000;
    u32 w = mapper->matrixWidth, h = mapper->matrixHeight, cx, cz, layer, id, i, n = 0, nearest = 0;
    MapTerrainBuf all[16];

    if (mapper->chunkDatIds == NULL || span <= 0 || w == 0 || h == 0 || pos->x < 0 || pos->z < 0) {
        return FALSE;
    }
    cx = (u32)(pos->x / span);
    cz = (u32)(pos->z / span);
    if (cx >= w || cz >= h) {
        return FALSE;
    }
    for (layer = 0; (id = layer * w * h + cz * w + cx) < mapper->chunkIdCount && n < 16; layer++) {
        const void *file;
        u16 magic;
        MapTerrainSamplerOutput out;
        VecFx32 local;
        fx32 baseY = FX_Mul((fx32)(layer << FX32_SHIFT), layerH);

        if (mapper->chunkDatIds[id] == 0xffffffff || (file = PortMap_RomChunk(mapper->chunkArcId, mapper->chunkDatIds[id])) == NULL) {
            continue;
        }
        magic = *(const u16 *)file;
        for (i = 0; FIELD_CHUNK_VTABLES[i].magic != 0xffffffff && FIELD_CHUNK_VTABLES[i].magic != magic; i++) {
        }
        if (FIELD_CHUNK_VTABLES[i].getTerrain == NULL) {
            continue;
        }
        local.x = pos->x - (span / 2 + (fx32)cx * span);
        local.y = pos->y - baseY;
        local.z = pos->z - (span / 2 + (fx32)cz * span);
        memset(&out, 0, sizeof out);
        FIELD_CHUNK_VTABLES[i].getTerrain(&out, file, &local, span, baseY);
        for (i = 0; i < out.layerCount && n < 16; i++) {
            all[n++] = out.layers[i];
        }
    }
    if (n == 0) {
        return FALSE;
    }
    for (i = 0; n > 1 && i < n; i++) {   // FieldG3DMapper_GetTerrain's choice
        fx32 distance = all[i].height - pos->y;

        if (MapTile_IsValid(all[i].tileType) && (distance < 0 ? -distance : distance) < nearestDistance) {
            nearest = i;
            nearestDistance = distance < 0 ? -distance : distance;
        }
    }
    *t = all[nearest];
    return TRUE;
}

static MMSys *sPortMapMMSys;   // PortMap_Fill's actor system: PortMap_Terrain's filled Strength holes

static BOOL PortMap_Terrain(FieldG3DMapper *mapper, int x, int z, fx32 y, MapTerrainBuf *t) {
    VecFx32 pos;
    static int noRom = -1;   // BWPORT_ROUTE_NOROM=1: the old way (unloaded chunks are '?')

    pos.x = (x << 16) + 0x8000;
    pos.y = y;
    pos.z = (z << 16) + 0x8000;
    if (FieldG3DMapper_GetTerrain(mapper, &pos, t)) {
        // a Strength hole a boulder fills is floor: field_player_grid.c's PlayerMoveCollCheck clears the wall bit where the
        // save keeps a boulder (IsSavedStrengthRockAtPos, at the tile's own height); the terrain's wall flag off here
        if (sPortMapMMSys != NULL && MapTile_IsStrengthHole(GetTileClass(t->tileType)) && (GetTileFlags(t->tileType) & 1)) {
            pos.y = t->height;
            if (PORT_SAVED_ROCK_AT(sPortMapMMSys, &pos) == TRUE) {
                t->tileType &= ~(1u << 16);
            }
        }
        return TRUE;
    }
    if (noRom < 0) {
        noRom = getenv("BWPORT_ROUTE_NOROM") != NULL;
    }
    if (noRom || !PortMap_RomTerrain(mapper, &pos, t)) {
        return FALSE;
    }
    if (x - sPortRomX0 >= 0 && z - sPortRomZ0 >= 0 && x - sPortRomX0 < PORT_MAP_N && z - sPortRomZ0 < PORT_MAP_N) {
        sPortMapRom[z - sPortRomZ0][x - sPortRomX0] = 1;
    }
    return TRUE;
}

static int PortMap_LedgeDir(u16 cls) {   // 0 up 1 down 2 left 3 right, -1 none
    if (MapTile_IsLedgeU(cls)) {
        return 0;
    }
    if (MapTile_IsLedgeD(cls)) {
        return 1;
    }
    if (MapTile_IsLedgeL(cls)) {
        return 2;
    }
    if (MapTile_IsLedgeR(cls)) {
        return 3;
    }
    return -1;
}

static BOOL PortMap_ExitBlocked(int dir, u16 cls) {
    switch (dir) {
    case 0: return TileExitBlockCheck_Up(cls);
    case 1: return TileExitBlockCheck_Down(cls);
    case 2: return TileExitBlockCheck_Left(cls);
    default: return TileExitBlockCheck_Right(cls);
    }
}

static BOOL PortMap_EnterBlocked(int dir, u16 cls) {   // as TILE_ENTER_BLOCK_CHECKS: the side the actor comes in by
    return PortMap_ExitBlocked(dir ^ 1, cls);
}

static int PortMap_ZoneAt(int x, int z) {
    MapMatrix *matrix;
    int cx, cz;

    if (sPortMapGsys == NULL || x < 0 || z < 0) {
        return -1;
    }
    matrix = GetMapMatrixSystem(GSYS_GetGameData(sPortMapGsys));
    if (matrix == NULL) {
        return -1;
    }
    cx = x / 32;
    cz = z / 32;
    if (cx >= GetMapMatrixWidth(matrix) || cz >= GetMapMatrixHeight(matrix)) {
        return -1;
    }
    return GetZoneIDAtMatrixXZ(matrix, cx, cz) == 0xffff ? -1 : (int)GetZoneIDAtMatrixXZ(matrix, cx, cz);
}

// ---- rails (no-grid maps): the player's actor has the rail flag (ACTOR_FLAG_RAIL 0x2000) while it moves on one ----

// the rail system and the player's actor, NULL when the player is not on a rail
static FieldRailSystem *PortRail_System(FieldActor **pactor, NoGridMapper **pmapper) {
    Field *field;
    FieldPlayer *player;
    FieldActor *actor;
    NoGridMapper *mapper;
    FieldRailSystem *rail;

    if (sPortMapGsys == NULL || (field = GSYS_GetField(sPortMapGsys)) == NULL || (player = Field_GetPlayer(field)) == NULL ||
        (actor = FieldPlayer_GetActor(player)) == NULL || !CheckActorFlag(actor, 0x2000) ||
        (mapper = FldActSys_GetNoGridMapper(GetActorMModelSystem(actor))) == NULL ||
        (rail = FieldNoGridMapper_GetRailSystem(mapper)) == NULL || rail->playerUnit == NULL || rail->data.lineCount == 0) {
        return NULL;
    }
    *pactor = actor;
    *pmapper = mapper;
    return rail;
}

static void PortRail_From(const RailPosition *r, PortRailPos *p) {
    p->line = r->componentId;
    p->isLine = r->componentIsLine;
    p->dir = r->railDirection;
    p->front = r->posFront;
    p->side = r->posSide;
}

static int PortRail_At(PortRailPos *p) {
    FieldActor *actor;
    NoGridMapper *mapper;
    RailPosition r;

    if (PortRail_System(&actor, &mapper) == NULL) {
        return 0;
    }
    GetActorRailPos(actor, &r);
    PortRail_From(&r, p);
    return 1;
}

// the player's own step check on a rail (field_actor_rail.c's RailActor_GetHitBitsDir and RailActor_CheckCollision: the
// step along the rail, in bounds, the tile's collision flag), on the rail system's own probe unit, which is put back
static FieldRailSystem *PortHybrid_Rail(NoGridMapper **png, FieldG3DMapper **pg3d);
static BOOL PortHybrid_TryStep(FieldRailSystem *rail, const RailPosition *a, int dir, RailPosition *b);

#if !defined(BLACK2) && !defined(WHITE2)
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/fx/fx_vec.h"
extern const u8 data_ov021_021d84d4[20];
extern const u16 data_ov021_021d841c[4];
BOOL func_ov021_021bb188(u16 tileClass);

// a cliff slide (Victory Road's outdoor rails, zone 214): the player on a tile of class 0xa0/0xa1 (rail_attr.c's
// func_ov021_021bb188) holds Down and the step that way is blocked (fieldmap_event.c's func_ov021_0218b24c: the hit bits
// & 3), so the game slides the player down the slope onto the rail below (rail_slipdown.c's INIT state, on the rail
// system's probe unit, which is put back): 1 and *land where it lands, 0 no slide
static int PortRail_Slide(FieldRailSystem *rail, NoGridMapper *mapper, const RailPosition *a, int dir, RailPosition *land) {
    RailUnit save;
    RailPosition railPos;
    VecFx16 v;
    VecFx32 pos, start, end, vec;
    MtxFx33 mtx;
    s16 d3[3];
    u16 angle;
    fx32 height, len, cosv, sinv;
    s32 maxSide;
    u16 count;
    int i, found = 0;

    if (dir != 1 || !a->componentIsLine || rail->playerUnit == NULL ||
        !func_ov021_021bb188((u16)GetTileClass(FieldNoGridMapper_GetTileAtPos(mapper, a)))) {
        return 0;
    }
    save = *rail->playerUnit;
    // the facing vector (fldmmdl_rail.c's func_ov021_0219e784 for the face dir Down, on the unit at *a)
    RailUnit_SetRailPos(rail->playerUnit, a);
    func_ov021_021b8050(rail->playerUnit, d3);
    angle = data_ov021_021d841c[data_ov021_021d84d4[rail->playerUnit->component.line->dir * 4 + dir]];
    cosv = FX_SinCosTable_[(angle >> 4) * 2 + 1];
    sinv = FX_SinCosTable_[(angle >> 4) * 2];
    vec.x = d3[0];
    vec.y = FX_Mul(cosv, d3[1]);
    vec.z = d3[2];
    MTX_RotY33_(&mtx, sinv, cosv);
    MTX_MultVec33(&vec, &mtx, &vec);
    VEC_Normalize(&vec, &vec);
    VEC_Fx16Set(&v, vec.x, vec.y, vec.z);
    v.y = 0;
    VEC_Fx16Normalize(&v, &v);
    // rail_slipdown.c's INIT: from 16 units ahead down the slope's line to the rail it meets
    func_ov021_021b7d38(rail, a, &pos);
    start.x = pos.x + FX_Mul(v.x, FX32_CONST(16));
    start.y = pos.y + FX_Mul(v.y, FX32_CONST(16));
    start.z = pos.z + FX_Mul(v.z, FX32_CONST(16));
    height = start.y + FX32_CONST(10);
    end.y = -FX32_CONST(10);
    len = FX_Mul(height, 0x127b);
    end.x = start.x + FX_Mul(v.x, len);
    end.z = start.z + FX_Mul(v.z, len);
    memset(land, 0, sizeof *land);
    if (CalculateRailCurves(rail, &start, &end, land, &end)) {
        railPos = *land;
        maxSide = func_ov021_021b7d50(rail, land);
        if (!(GetTileFlags(FieldNoGridMapper_GetTileAtPos(mapper, &railPos)) & 1)) {
            found = 1;
        }
        railPos.posSide += 2;
        if (railPos.posSide > maxSide) {
            railPos.posSide = (s16)maxSide;
        }
        count = (u16)(maxSide + railPos.posSide);
        for (i = 0; i < count; i++) {
            railPos.posSide--;
            if (GetTileFlags(FieldNoGridMapper_GetTileAtPos(mapper, &railPos)) & 1) {
                break;
            }
            *land = railPos;
            found = 1;
        }
    }
    *rail->playerUnit = save;
    return found;
}
#endif

static int PortRail_Step(const PortRailPos *from, int dir, PortRailPos *to) {
    FieldActor *actor;
    NoGridMapper *mapper;
    FieldRailSystem *rail = PortRail_System(&actor, &mapper);
    RailPosition a, b;
    BOOL ok;

    if (rail == NULL) {   // the player is on the grid of a hybrid zone: its rails are the zone's all the same (the planner's chains)
        FieldG3DMapper *g3d;

        rail = PortHybrid_Rail(&mapper, &g3d);
    }
    if (rail == NULL) {
        *to = *from;
        return 0;
    }
    memset(&a, 0, sizeof a);
    a.componentId = (u16)from->line;
    a.componentIsLine = (u8)from->isLine;
    a.railDirection = (u8)from->dir;
    a.posFront = (u16)from->front;
    a.posSide = (s16)from->side;
    ok = PortHybrid_TryStep(rail, &a, dir, &b);   // (on the rail system's probe unit, which is put back)
    PortRail_From(&b, to);
    if (ok && (GetTileFlags(FieldNoGridMapper_GetTileAtPos(mapper, &b)) & 1)) {
        // a ledge the key's way (field_player_nogrid.c's FieldPlayerRail_CheckWalkStep: the hit is the tile's collision
        // flag alone and its class is a ledge in dir): the player jumps it (ACMD_JUMP_2, fld_act_rail.c's RailJump_Update:
        // two rail steps in dir), so the step lands one place past the ledge. Dragonspiral Tower's rail floors (zone 209)
        if (PortMap_LedgeDir((u16)GetTileClass(FieldNoGridMapper_GetTileAtPos(mapper, &b))) == dir) {
            RailPosition c;

            a = b;
            ok = PortHybrid_TryStep(rail, &a, dir, &c);
            if (ok) {
                PortRail_From(&c, to);
            }
        } else {
            ok = FALSE;
        }
    }
#if !defined(BLACK2) && !defined(WHITE2)
    if (!ok) {   // a cliff slide (Down from a slide tile, the step blocked): it lands on the rail below
        RailPosition land, s = a;

        s.componentId = (u16)from->line;
        s.componentIsLine = (u8)from->isLine;
        s.railDirection = (u8)from->dir;
        s.posFront = (u16)from->front;
        s.posSide = (s16)from->side;

        if (PortRail_Slide(rail, mapper, &s, dir, &land)) {
            PortRail_From(&land, to);
            ok = TRUE;
        }
    }
#endif
    if (getenv("BWPORT_RAILDBG") != NULL && !ok) {
        fprintf(stderr, "raildbg: rail[%d,%d,%d] dir %d blocked: here class 0x%x flags 0x%x, to rail[%d,%d,%d] class 0x%x flags 0x%x\n",
                from->line, from->front, from->side, dir, (unsigned)GetTileClass(FieldNoGridMapper_GetTileAtPos(mapper, &a)),
                (unsigned)GetTileFlags(FieldNoGridMapper_GetTileAtPos(mapper, &a)), to->line, to->front, to->side,
                (unsigned)GetTileClass(FieldNoGridMapper_GetTileAtPos(mapper, &b)),
                (unsigned)GetTileFlags(FieldNoGridMapper_GetTileAtPos(mapper, &b)));
    }
    return ok ? 1 : 0;
}

// ---- hybrid zones (a grid and rails in one zone: Black 2's Castelia plaza): fieldmap_ctrl_hybrid.c's hand-off (Black
// func_ov021_021a6f5c / 021a7010 with 021a7198 / 021a71e4). Both ways the tile under the player is a switch
// (FieldRailTilemap_IsJumpStart: TILE_CLASS_GRID_RAIL_SWITCH), the held key's way is blocked on the side the player is on,
// and the other side takes a step that way: then the game swaps the controller and the key walks on from there.

// the zone's rail system with the field's no-grid mapper (whether or not the player is on a rail now), NULL: no rails
static FieldRailSystem *PortHybrid_Rail(NoGridMapper **png, FieldG3DMapper **pg3d) {
    Field *field;
    NoGridMapper *ng;
    FieldRailSystem *rail;

    if (sPortMapGsys == NULL || (field = GSYS_GetField(sPortMapGsys)) == NULL ||
        (ng = Field_GetNoGridMapper(field)) == NULL || (rail = FieldNoGridMapper_GetRailSystem(ng)) == NULL ||
        rail->data.lineCount == 0 || (*pg3d = Field_GetG3DMapper(field)) == NULL) {
        return NULL;
    }
    *png = ng;
    return rail;
}

// the rail step check on the rail system's probe unit, which is put back (PortRail_Step's)
static BOOL PortHybrid_TryStep(FieldRailSystem *rail, const RailPosition *a, int dir, RailPosition *b) {
    RailUnit save;
    BOOL ok;

    if (rail->playerUnit == NULL) {
        return PORT_RAIL_TRYSTEP(rail, a, ConvDirToRailDir((u32)dir), b);
    }
    save = *rail->playerUnit;
    ok = PORT_RAIL_TRYSTEP(rail, a, ConvDirToRailDir((u32)dir), b);
    *rail->playerUnit = save;
    return ok;
}

// on a rail at *p, the key held in dir: 1 the game steps off onto the grid (UpdateNogrid: the rail tile is a switch, the
// rail ahead blocked, the grid tile one step ahead free: FindGridPos), 2 it might (the grid there is not loaded yet: the
// mapper loads the chunks near the player), 0 not
static int PortHybrid_ToGridAt(const PortRailPos *p, int dir, int *lx, int *lz) {
    NoGridMapper *ng;
    FieldG3DMapper *g3d;
    FieldRailSystem *rail = PortHybrid_Rail(&ng, &g3d);
    RailPosition a, b;
    MapTerrainBuf t;
    VecFx32 pos;
    u32 ahead;

    if (rail == NULL) {
        return 0;
    }
    memset(&a, 0, sizeof a);
    a.componentId = (u16)p->line;
    a.componentIsLine = (u8)p->isLine;
    a.railDirection = (u8)p->dir;
    a.posFront = (u16)p->front;
    a.posSide = (s16)p->side;
    if (!PORT_RAIL_ISSWITCH(GetTileClass(FieldNoGridMapper_GetTileAtPos(ng, &a)))) {
        return 0;
    }
    b = a;
    ahead = PortHybrid_TryStep(rail, &a, dir, &b) ? FieldNoGridMapper_GetTileAtPos(ng, &b) : 0xffffffffu;   // TILE_TYPE_NONE
    if (!MapTile_BlocksCollision(ahead)) {
        return 0;
    }
    PORT_RAIL_LOCPOS(rail, &a, &pos);
    if (lx != NULL) {   // the grid tile the player is put on: the rail place's own (a tile is 16 units of 1 << 12), the key walks on
        *lx = (int)(pos.x >> 16);
        *lz = (int)(pos.z >> 16);
    }
    ExpandVecInGridDir((u16)dir, &pos, FX32_CONST(16));
    if (!FieldG3DMapper_GetTerrain(g3d, &pos, &t)) {
        return 2;
    }
    return MapTile_BlocksCollision(t.tileType) ? 0 : 1;
}

static int PortHybrid_ToGrid(const PortRailPos *p, int dir) {
    return PortHybrid_ToGridAt(p, dir, NULL, NULL);
}

static int PortHybrid_Land(const PortRailPos *p, int dir, int *x, int *z) {
    return PortHybrid_ToGridAt(p, dir, x, z) != 0;
}

// scratch of the fill (static: 128 x 128 tiles)
static fx32 sPortMapH[PORT_MAP_N][PORT_MAP_N];
static u32 sPortMapType[PORT_MAP_N][PORT_MAP_N];
static u8 sPortMapHave[PORT_MAP_N][PORT_MAP_N];   // 1 terrain found, 2 also reached by the flood
static int sPortMapX0 = -(1 << 20), sPortMapZ0;     // the fill's window (PortHybrid_RailUnder takes the floor it reached)

// on grid tile x,z (the floor the last fill reached it on, else the one nearest the player's height), the key held in
// dir: 1 the game steps onto a rail
// (UpdateGrid: the tile is a switch, the grid tile ahead blocked, a rail through the player's place takes the step:
// FindRailLocation), *to the rail place it starts from; 0 not. switchOnly 0: any tile, nothing ahead checked (a script's
// hand-off: func_ov021_021a6dc4 with func_ov021_021a71e4)
static int PortHybrid_RailUnder(int x, int z, int dir, PortRailPos *to, int switchOnly) {
    NoGridMapper *ng;
    FieldG3DMapper *g3d;
    FieldRailSystem *rail = PortHybrid_Rail(&ng, &g3d);
    FieldActor *actor;
    MapTerrainBuf t;
    VecFx32 pos, ahead, top, hit;
    RailPosition found, next;

    if (rail == NULL || (actor = FieldPlayer_GetActor(Field_GetPlayer(GSYS_GetField(sPortMapGsys)))) == NULL) {
        return 0;
    }
    CopyActorWPos(actor, &pos);
    pos.x = (x << 16) + 0x8000;
    pos.z = (z << 16) + 0x8000;
    if (x - sPortMapX0 >= 0 && z - sPortMapZ0 >= 0 && x - sPortMapX0 < PORT_MAP_N && z - sPortMapZ0 < PORT_MAP_N &&
        sPortMapHave[z - sPortMapZ0][x - sPortMapX0] == 2) {
        pos.y = sPortMapH[z - sPortMapZ0][x - sPortMapX0];   // (an upper floor's switch tile planned from below)
    }
    if (!FieldG3DMapper_GetTerrain(g3d, &pos, &t) || (switchOnly && !PORT_RAIL_ISSWITCH(GetTileClass(t.tileType)))) {
        return 0;
    }
    pos.y = t.height;
    ahead = pos;
    ExpandVecInGridDir((u16)dir, &ahead, FX32_CONST(16));
    if (switchOnly && !MapTile_BlocksCollision(GetTileTypeAtPos(g3d, &ahead))) {
        return 0;
    }
    top = pos;
    top.y += FX32_CONST(4);
    pos.y -= FX32_CONST(4);
    if (!CalculateRailCurves(rail, &pos, &top, &found, &hit) || !PortHybrid_TryStep(rail, &found, dir, &next)) {
        return 0;
    }
    PortRail_From(&found, to);
    return 1;
}

static int PortHybrid_ToRail(int x, int z, int dir, PortRailPos *to) {
    return PortHybrid_RailUnder(x, z, dir, to, 1);
}

static fx32 sPortMapH2[PORT_MAP_N][PORT_MAP_N];   // a tile's second floor (the flood reached it again 20 or more units away)
static u32 sPortMapType2[PORT_MAP_N][PORT_MAP_N];
static u8 sPortMapHave2[PORT_MAP_N][PORT_MAP_N];
static u16 sPortMapQ[PORT_MAP_N * PORT_MAP_N * 2];   // a tile, + PORT_MAP_UPPER: its second floor

#define PORT_MAP_UPPER 0x4000

static fx32 PortMap_Abs(fx32 v) {
    return v < 0 ? -v : v;
}

// the fill's flood reaches tile x,z of the window on floor *t: as its floor, or (reached already on a floor 20 or more units
// away: a walkway over a path) as its second floor; queued at tail. Returns the new tail
static int PortMap_Reach(int x, int z, const MapTerrainBuf *t, int tail) {
    if (sPortMapHave[z][x] != 2) {
        sPortMapH[z][x] = t->height;
        sPortMapType[z][x] = t->tileType;
        sPortMapHave[z][x] = 2;
        sPortMapQ[tail++] = (u16)(z * PORT_MAP_N + x);
    } else if (!sPortMapHave2[z][x] && PortMap_Abs(t->height - sPortMapH[z][x]) >= FX32_CONST(20)) {
        sPortMapH2[z][x] = t->height;
        sPortMapType2[z][x] = t->tileType;
        sPortMapHave2[z][x] = 1;
        sPortMapQ[tail++] = (u16)((z * PORT_MAP_N + x) | PORT_MAP_UPPER);
    }
    return tail;
}

// the fill's flood from the queue's entries head..tail: each tile's floor is the one at the height it is walked onto from (the
// player's step check: a step of 20 or more units, a wall, a side's exit or entry blocked stop it; a ledge is jumped). Returns
// the new tail
static int PortMap_Flood(FieldG3DMapper *mapper, const PortMap *m, int head, int tail) {
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    MapTerrainBuf t;

    while (head < tail) {
        int q = sPortMapQ[head++], up = (q & PORT_MAP_UPPER) != 0, cx = (q & ~PORT_MAP_UPPER) % PORT_MAP_N,
            cz = (q & ~PORT_MAP_UPPER) / PORT_MAP_N, d;
        fx32 ch = up ? sPortMapH2[cz][cx] : sPortMapH[cz][cx];
        u32 ctype = up ? sPortMapType2[cz][cx] : sPortMapType[cz][cx];
        u16 cls = GetTileClass(ctype);

        for (d = 0; d < 4; d++) {
            int nx = cx + dx[d], nz = cz + dz[d];

            if (nx < 0 || nz < 0 || nx >= PORT_MAP_N || nz >= PORT_MAP_N ||
                !PortMap_Terrain(mapper, m->x0 + nx, m->z0 + nz, ch, &t)) {
                continue;
            }
            if (MapTile_BlocksCollision(t.tileType)) {   // a ledge: the jump lands a tile beyond (surfing: an edge tile too)
                int lx = nx + dx[d], lz = nz + dz[d];

                if ((PortMap_LedgeDir(GetTileClass(t.tileType)) == d ||
                     (m->surf && (GetTileFlags(ctype) & 2) && MapTile_IsSurfEdge(GetTileClass(t.tileType)))) &&
                    lx >= 0 && lz >= 0 && lx < PORT_MAP_N && lz < PORT_MAP_N &&
                    PortMap_Terrain(mapper, m->x0 + lx, m->z0 + lz, ch, &t) && !MapTile_BlocksCollision(t.tileType)) {
                    tail = PortMap_Reach(lx, lz, &t, tail);
                }
                continue;
            }
            if (PortMap_Abs(t.height - ch) >= FX32_CONST(20) ||
                ((GetTileFlags(t.tileType) & 2) && !(m->surf && (GetTileFlags(ctype) & 2))) ||
                PortMap_ExitBlocked(d, cls) || PortMap_EnterBlocked(d, GetTileClass(t.tileType))) {
                continue;
            }
            tail = PortMap_Reach(nx, nz, &t, tail);
        }
    }
    return tail;
}

// the steps from tile x,z of the fill on its floor of tile type type0 at height h (kind: that floor's PortMapCell.kind):
// PORT_MAP_* bits. A neighbour with two floors is taken at the one nearer h
static int PortMap_Pass(int x, int z, u32 type0, fx32 h, char kind) {
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    u16 cls = GetTileClass(type0);
    int d, pass = 0;

    for (d = 0; d < 4; d++) {
        int nx = x + dx[d], nz = z + dz[d];
        fx32 nh;
        u32 type;

        if (PortMap_ExitBlocked(d, cls)) {
            continue;
        }
        if (nx < 0 || nz < 0 || nx >= PORT_MAP_N || nz >= PORT_MAP_N) {
            pass |= 1 << d;   // off the window: let the planner try
            continue;
        }
        if (!sPortMapHave[nz][nx]) {
            continue;   // ('?' tiles get theirs below)
        }
        type = sPortMapType[nz][nx];
        nh = sPortMapH[nz][nx];
        if (sPortMapHave2[nz][nx] && PortMap_Abs(sPortMapH2[nz][nx] - h) < PortMap_Abs(nh - h)) {
            type = sPortMapType2[nz][nx];
            nh = sPortMapH2[nz][nx];
        }
        if (kind == '~' && MapTile_BlocksCollision(type)) {   // surfing at an edge tile: the hop lands beyond it
            int bx = nx + dx[d], bz = nz + dz[d];

            if (MapTile_IsSurfEdge(GetTileClass(type)) && bx >= 0 && bz >= 0 && bx < PORT_MAP_N && bz < PORT_MAP_N &&
                sPortMapHave[bz][bx] && !(GetTileFlags(sPortMapType[bz][bx]) & 3)) {
                pass |= 1 << d;   // func_ov021_0218b408: the tile beyond is neither a wall nor water
            }
            continue;
        }
        if (MapTile_BlocksCollision(type) || ((GetTileFlags(type) & 2) && kind != '~') || PortMap_Abs(nh - h) >= FX32_CONST(20) ||
            PortMap_EnterBlocked(d, GetTileClass(type))) {
            continue;   // (a land tile never steps into the water: that is F:surf's start, not a walk)
        }
        pass |= 1 << d;
    }
    return pass;
}

#if defined(BLACK2) || defined(WHITE2)
static const u8 kDriftveilPads[82][5] = {   // the Driftveil Gym's pads (ov096 sPads: length class, x, z, direction, landing switch)
#include "asset/gym_driftveil_sPads.inc"
};
#endif

// PortMapCell.slide of the tile gx,gz when it is one of the Driftveil Gym's pads (0 none; the game stops them once its work
// 0x40b7 is 1)
static int PortMap_PadAt(Field *field, int gx, int gz) {
#if defined(BLACK2) || defined(WHITE2)
    int i;

    if (field == NULL || sPortMapGsys == NULL || !Field_CheckGimmickID(field, GIMMICK_DRIFTVEIL_GYM) ||
        *EventWork_GetWkPtr(GameData_GetEventWork(GSYS_GetGameData(sPortMapGsys)), 0x40b7) == 1) {
        return 0;
    }
    for (i = 0; i < 82; i++) {
        if (kDriftveilPads[i][1] == gx && kDriftveilPads[i][2] == gz) {
            return PORT_MAP_PAD + kDriftveilPads[i][0] * 4 + kDriftveilPads[i][3];
        }
    }
#else
    (void)field;
    (void)gx;
    (void)gz;
#endif
    return 0;
}

#if !defined(BLACK2) && !defined(WHITE2)
typedef struct {
    u16 x, y, z;
} PortCannonSpot;
typedef struct {
    u32 curve, x, y, z, camera;
} PortCannonShot;
static const PortCannonSpot kCannonSpots[10] = {   // the Mistralton Gym's cannons (ov041 data_ov041_021f40f0): x, height, z
#include "asset/ov041_021f36c0_data_ov041_021f40f0.inc"
};
// per cannon, the shot for the player facing up, down, right, left (func_ov041_021f3f44's order): the landing tile x, z (curve
// 0xffffffff: that way does nothing)
static const PortCannonShot kCannonShots[10][4] = {
#include "asset/ov041_021f36c0_data_ov041_021f4320.inc"
};
#endif

// PortMapCell.slide of the tile gx,gz when a Mistralton Gym cannon shoots from it (Black / White: gimmick 5, ov041; 0 none): the
// tile two before the cannon, the key toward it (func_ov041_021f37e0)
static int PortMap_CannonAt(Field *field, int gx, int gz) {
#if !defined(BLACK2) && !defined(WHITE2)
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    static const int kShotDir[4] = {0, 1, 3, 2};   // the shot's column to the player's facing
    int i, s;

    if (field == NULL || !func_ov021_021be288(field, 5)) {
        return 0;
    }
    for (i = 0; i < 10; i++) {
        for (s = 0; s < 4; s++) {
            const PortCannonShot *shot = &kCannonShots[i][s];
            int d = kShotDir[s], sx = kCannonSpots[i].x - 2 * dx[d], sz = kCannonSpots[i].z - 2 * dz[d];
            int n = ((int)shot->x - sx) * dx[d] + ((int)shot->z - sz) * dz[d];   // the tiles to the landing, on the line it faces

            if (shot->curve == 0xffffffff || sx != gx || sz != gz) {
                continue;
            }
            if (n < 1 || n > 32 || ((int)shot->x - sx != 0 && (int)shot->z - sz != 0)) {
                return 0;
            }
            return PORT_MAP_CANNON + (n - 1) * 4 + d;
        }
    }
#else
    (void)field;
    (void)gx;
    (void)gz;
#endif
    return 0;
}

// PortMap's switches in the Icirrus Gym (Black, White: ov045 gym_icirrus.c, gimmick 9; zone 114). A lever (a type 0 trigger
// running script 4, 5 or 6: switch 0, 1, 2) flips its switch (the gimmick's saved flipped[]) and swings the switch's two arm
// actors to the other side of its middle (the scripts' ActorSetGPos); the middles are the switches' first models (the
// gimmick's scene 0, models 0-2). A place (trigger script 7 + i runs func_ov021_021b271c with place kIcirrusPlace[i])
// hands the player onto the rail where it stands when the place's switch is in the state sPlacePreconditions says; the
// trigger's type is the way it is walked onto (type - 1)
#if !defined(BLACK2) && !defined(WHITE2)
static const u16 kIcirrusPre[8][2] = {   // sPlacePreconditions: switch, flipped
#include "asset/gym_icirrus_data_ov045_021f3b60.inc"
};
static const u8 kIcirrusPlace[8] = {0, 1, 2, 3, 5, 4, 7, 6};
#endif

// PortMap's switches in the Opelucid Gym (Black, White: ov046, gimmick 10; zone 121): three dragon statues, each with two
// bits in the gimmick's saved table (Ov046Table: u32 state, u32 bit[2]). Each of the six dragon triggers (type 0, scripts
// 3..8) toggles one bit (the scripts' func_ov021_021b27b4 dragon / bit: script 3 dragon 0 bit 1, 4: 0 0, 5: 1 0, 6: 1 1,
// 7: 2 0, 8: 2 1) and turns its dragon to the state func_ov046_021f3bc8 makes of its bits; func_ov046_021f4078 then turns
// that dragon's rail lines on or off. Switch s = dragon * 2 + bit. The trigger's func_ov021_021b277c (2: west, 3: east;
// func_ov046_021f3b5c / 021f3efc) hops the player a tile on, over the fence the trigger stands at
#if !defined(BLACK2) && !defined(WHITE2)
void func_ov046_021f4078(Field *field, u32 dragon, u32 state);
static const u8 kOpelucidBit[6] = {1, 0, 2, 3, 4, 5};   // trigger script 3 + i: its switch
static const s8 kOpelucidHop[6] = {1, -1, 1, -1, 1, -1};   // trigger script 3 + i: the x its hop adds
static const u8 kOpelucidState[4] = {0, 2, 3, 1};       // func_ov046_021f3bc8: a dragon's state from its bits (bit 0 | bit 1 << 1)
static u16 sPortRailOff[18];                            // the game's disabled lines while the planner's search turns them
static int sPortRailSaved;

static int PortMap_Opelucid(Field *field, PortMap *m) {
    const u32 *t;
    int i;

    if (field == NULL || !func_ov021_021be288(field, 10) || Field_GetGimmickWorkBlock(field, 1) == NULL ||
        (t = GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(sPortMapGsys)), 10)) == NULL) {
        return 0;
    }
    for (i = 0; i < 3; i++) {
        m->sw |= (t[i * 3 + 1] ? 1u : 0) << (i * 2) | (t[i * 3 + 2] ? 1u : 0) << (i * 2 + 1);
    }
    for (i = 0; i < m->ntrig; i++) {
        const PortMapTrig *tr = &m->trig[i];

        if (!tr->rail && tr->script >= 3 && tr->script <= 8 && tr->type == 0 && m->nlever < 8) {
            m->lever[m->nlever].x = tr->x;
            m->lever[m->nlever].z = tr->z;
            m->lever[m->nlever].s = kOpelucidBit[tr->script - 3];
            m->lever[m->nlever].x1 = tr->x + kOpelucidHop[tr->script - 3];
            m->lever[m->nlever].z1 = tr->z;
            m->nlever++;
        }
    }
    return 1;
}
#endif

// the adapter's railSwitches: Opelucid's rail lines with the dragons in switch states sw (each dragon's lines as the
// gimmick sets them, dragons 0..2 in turn as its init does), or the game's lines put back
static void PortMap_RailSwitches(int set, unsigned sw) {
#if !defined(BLACK2) && !defined(WHITE2)
    Field *field;
    NoGridMapper *ng;
    FieldG3DMapper *g3d;
    FieldRailSystem *rail;
    int d;

    if (sPortMapGsys == NULL || (field = GSYS_GetField(sPortMapGsys)) == NULL || !func_ov021_021be288(field, 10) ||
        (rail = PortHybrid_Rail(&ng, &g3d)) == NULL) {
        return;
    }
    if (!set) {
        if (sPortRailSaved) {
            memcpy(rail->disabledLines.lineIds, sPortRailOff, sizeof sPortRailOff);
            sPortRailSaved = 0;
        }
        return;
    }
    if (!sPortRailSaved) {
        memcpy(sPortRailOff, rail->disabledLines.lineIds, sizeof sPortRailOff);
        sPortRailSaved = 1;
    }
    for (d = 0; d < 3; d++) {
        func_ov046_021f4078(field, (u32)d, kOpelucidState[(sw >> (d * 2)) & 3]);
    }
#else
    (void)set;
    (void)sw;
#endif
}

#if defined(BLACK2) || defined(WHITE2)
// Black 2 / White 2's Opelucid Gym (gimmick 10, ov099 gym_opelucid.c): the gym's state is the player's tier and column
// (4 x left, right, centre). Each platform has four lift triggers around its centre tile (scripts 2..5: work 0x4000 + the
// direction, 0 up 1 down 2 left 3 right; up north of the centre, down south, left west, right east). A trigger runs when
// its direction is open in its platform's stage (sStages: tier, column and whether the tier is cleared;
// GymOpelucid_SetVarsOfStage), and the move (GymOpelucid_MoveState: up and down a tier, left and right between a side
// and the centre) carries the player to the centre of the next state's platform. The centres, by tier and column (the
// climb's lifts, route b2 gym7-drayden; the left side mirrors the right about the centre's x), -1 none
static const s8 kOpelucidHub[4][3][2] = {
    {{-1, -1}, {-1, -1}, {15, 28}},
    {{4, 16}, {18, 16}, {11, 20}},
    {{9, 21}, {25, 21}, {17, 25}},
    {{-1, -1}, {-1, -1}, {15, 21}},
};
// the directions sStages opens (bit d: work 0x4000 + d is 1), by tier, column and the tier cleared
static const u8 kOpelucidOpen[4][3][2] = {
    {{0, 0}, {0, 0}, {0, 1}},
    {{8, 8}, {4, 4}, {14, 15}},
    {{8, 8}, {4, 4}, {14, 15}},
    {{0, 0}, {0, 0}, {2, 2}},
};

// gym_opelucid.c's GymOpelucid_IsTier0Cleared .. 3: a Veteran of the tier beaten and its button pressed
static int PortMap_OpelucidCleared(EventWork *ew, int tier) {
    static const u16 kProgress[3] = {0x40cf, 0x40d0, 0x40d1};
    static const u16 kVeteran[3][2] = {{TRAINER_VETERAN_LUCIUS, TRAINER_VETERAN_LUCIUS},
                                       {TRAINER_VETERAN_JERRY, TRAINER_VETERAN_RHONA},
                                       {TRAINER_VETERAN_RON, TRAINER_VETERAN_DENAE}};

    if (tier < 0 || tier > 2 || *EventWork_GetWkPtr(ew, kProgress[tier]) != 2) {
        return 0;
    }
    return TrainerFlagGet(ew, kVeteran[tier][0]) == TRUE || TrainerFlagGet(ew, kVeteran[tier][1]) == TRUE;
}

// the platform (tier, column) whose centre is x,z; 0 none
static int PortMap_OpelucidHubAt(int x, int z, int *tier, int *col) {
    for (*tier = 0; *tier < 4; (*tier)++) {
        for (*col = 0; *col < 3; (*col)++) {
            if (kOpelucidHub[*tier][*col][0] == x && kOpelucidHub[*tier][*col][1] == z) {
                return 1;
            }
        }
    }
    return 0;
}

// the gym's open lifts as levers without a switch (s -1), each live in its platform's state: the planner walks onto one
// and searches on from the centre it lands on, in the next state (route_map.c's LeverPlan). The tier 0 walk crosses the
// Leader's platform's tiles (15,20 .. 15,22 at another height): their lifts are not the tier's
static void PortMap_OpelucidLifts(Field *field, PortMap *m) {
    static const int kCx[4] = {0, 0, 1, -1}, kCz[4] = {1, -1, 0, 0};   // a trigger's tile + this: its platform's centre
    EventWork *ew;
    const u8 *state;   // gym_opelucid.c's OpelucidGymState: column, tier
    int i;

    if (field == NULL || sPortMapGsys == NULL || !Field_CheckGimmickID(field, 10) ||
        (state = GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(sPortMapGsys)), 10)) == NULL ||
        state[0] > 2 || state[1] > 3) {
        return;
    }
    m->sw = (unsigned)(state[1] * 3 + state[0]);   // the switches' state is the gym's: tier * 3 + column
    ew = GameData_GetEventWork(GSYS_GetGameData(sPortMapGsys));
    for (i = 0; i < m->ntrig && m->nlever < PORT_MAP_LEVERS; i++) {
        const PortMapTrig *tr = &m->trig[i];
        int d = (int)tr->workId - 0x4000, t, c, nt, nc;

        if (tr->rail || tr->type != 0 || tr->script < 2 || tr->script > 5 || d < 0 || d > 3 ||
            !PortMap_OpelucidHubAt(tr->x + kCx[d], tr->z + kCz[d], &t, &c) ||
            !((kOpelucidOpen[t][c][PortMap_OpelucidCleared(ew, t)] >> d) & 1)) {
            continue;
        }
        nt = t + (d == 0) - (d == 1);
        nc = d == 2 ? (c == 2 ? 0 : 2) : d == 3 ? (c == 2 ? 1 : 2) : c;
        if (nt < 0 || nt > 3 || kOpelucidHub[nt][nc][0] < 0) {
            continue;
        }
        m->lever[m->nlever].x = tr->x;
        m->lever[m->nlever].z = tr->z;
        m->lever[m->nlever].s = -1;
        m->lever[m->nlever].x1 = kOpelucidHub[nt][nc][0];
        m->lever[m->nlever].z1 = kOpelucidHub[nt][nc][1];
        m->lever[m->nlever].on = t * 3 + c;
        m->lever[m->nlever].to = nt * 3 + nc;
        m->nlever++;
    }
}
#endif

#if defined(BLACK2) || defined(WHITE2)
// Black 2 / White 2's Plasma Frigate (gimmick state 43, ov117 gimmick_plasma_frigate.c): four floor switches, each flips
// four of the sixteen floor panels (GimmickPlasmaFrigate_PressSwitch); a panel's height override area (HeightEx, one per
// panel) is high when it is on, low when off, so the walkable floor depends only on the work block's panelOn
typedef struct {   // gimmick_plasma_frigate.c's PlasmaFrigateWork
    void *expObj;
    Field *field;
    void *state;
    BOOL panelOn[16];
} PortFrigateWork;

typedef struct {   // height_ex.c's HeightExList
    s32 count;
    HeightExEntry *entries;
} PortHeightExList;

typedef struct {
    u8 x, z, w, d;
} PortFrigateArea;

static const PortFrigateArea kFrigateOn[16] = {
#include "asset/gimmick_plasma_frigate_sAreasOn.inc"
};
static const PortFrigateArea kFrigateOff[16] = {
#include "asset/gimmick_plasma_frigate_sAreasOff.inc"
};

static PortFrigateWork *PortMap_FrigateWork(Field *field) {
    if (field == NULL || !Field_CheckGimmickID(field, 43)) {   // (field_gimmick.c: GimmickPlasmaFrigate_Setup is ID 43)
        return NULL;
    }
    return Field_GetGimmickWorkBlock(field, 1);
}

// the frigate's switches as levers: the triggers of scripts 7..10 press switch script - 7, each runs only while its work
// 0x4000 + i is 0 (pressing one sets it to 1 and the other three to 0): the switch pressed last is dead until another is
static int PortMap_Frigate(Field *field, PortMap *m) {
    EventWork *ew;
    int i, x, z;

    if (PortMap_FrigateWork(field) == NULL) {
        return 0;
    }
    ew = GameData_GetEventWork(GSYS_GetGameData(sPortMapGsys));
    m->leverOnce = 1;
    for (i = 0; i < 4; i++) {
        if (*EventWork_GetWkPtr(ew, (u16)(0x4000 + i)) == 1) {
            m->leverLast = i;
        }
    }
    for (i = 0; i < m->ntrig; i++) {
        const PortMapTrig *t = &m->trig[i];

        if (t->rail || t->script < 7 || t->script > 10) {
            continue;
        }
        for (z = t->z; z < t->z + (t->h > 0 ? t->h : 1); z++) {
            for (x = t->x; x < t->x + (t->w > 0 ? t->w : 1) && m->nlever < PORT_MAP_LEVERS; x++) {
                m->lever[m->nlever].x = x;
                m->lever[m->nlever].z = z;
                m->lever[m->nlever].s = (int)t->script - 7;
                m->lever[m->nlever].x1 = x;
                m->lever[m->nlever].z1 = z;
                m->nlever++;
            }
        }
    }
    if (m->zone == 564) {   // White 2's Giant Chasm Frigate: the warp pads (trigger scripts 27..40 move the player to a tile)
        static const struct {
            int script, x1, z1;
        } kPads[14] = {{27, 11, 48}, {28, 7, 16}, {29, 5, 41}, {30, 7, 21}, {31, 25, 41}, {32, 23, 16}, {33, 19, 48},
                       {34, 23, 21}, {35, 5, 54}, {36, 11, 41}, {37, 25, 54}, {38, 11, 54}, {39, 19, 54}, {40, 19, 41}};

        for (i = 0; i < m->ntrig; i++) {
            const PortMapTrig *t = &m->trig[i];
            int k;

            for (k = 0; k < 14 && !t->rail && m->nlever < PORT_MAP_LEVERS; k++) {
                if (t->script == kPads[k].script) {
                    m->lever[m->nlever].x = t->x;
                    m->lever[m->nlever].z = t->z;
                    m->lever[m->nlever].s = -2;   // a warp pad: any state, leaves the player at x1,z1 (the script's ActorSetGPos)
                    m->lever[m->nlever].x1 = kPads[k].x1;
                    m->lever[m->nlever].z1 = kPads[k].z1;
                    m->nlever++;
                }
            }
        }
    }
    return 1;
}

// a copy of GimmickPlasmaFrigate_SetPanelArea: panel's area with the panels in states on[] (3/4 and 11/12 are pairs that
// merge into one area when both are off)
static void PortMap_FrigateArea(HeightExList *list, const BOOL *on, int panel) {
    const PortFrigateArea *a = on[panel] ? &kFrigateOn[panel] : &kFrigateOff[panel];
    int pair = panel == 3 || panel == 4 || panel == 11 || panel == 12, second = panel == 4 || panel == 12, first;
    int linked = second ? panel - 1 : panel + 1;

    if (!on[panel]) {
        first = -1;
        if (pair) {
            if (!on[linked]) {
                first = second ? panel - 1 : panel;
                HeightEx_SetAreaInUse((u8)first, 0, list);
                HeightEx_SetArea((u8)first, kFrigateOff[first].x, kFrigateOff[first].z, kFrigateOff[first].w,
                                 kFrigateOff[first].d + 1u, 0xfffcffd1, 0x800000, list);
            }
            if (first == panel) {
                return;
            }
        }
    } else if (second && !on[linked]) {
        first = panel - 1;
        HeightEx_SetAreaInUse((u8)first, 0, list);
        HeightEx_SetArea((u8)first, kFrigateOff[first].x, kFrigateOff[first].z, kFrigateOff[first].w, kFrigateOff[first].d,
                         0xfffcffd1, 0x800000, list);
    }
    HeightEx_SetAreaInUse((u8)panel, 0, list);
    HeightEx_SetArea((u8)panel, a->x, a->z, a->w, a->d, on[panel] ? 0xffff8000 : 0xfffcffd1, 0x800000, list);
}
#endif

static void PortMap_Switches(Field *field, PortMap *m) {
    m->nlever = m->nplace = m->narm = 0;
    m->sw = 0;
    m->leverOnce = 0;
    m->leverLast = -1;
#if !defined(BLACK2) && !defined(WHITE2)
    if (PortMap_Opelucid(field, m)) {
        return;
    }
    {
        const BOOL *flipped;
        FieldExpObjSystem *exp;
        int i, j;

        if (field == NULL || !func_ov021_021be288(field, 9) || Field_GetGimmickWorkBlock(field, 1) == NULL ||
            (exp = Field_GetExpObjSystem(field)) == NULL ||
            (flipped = GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(sPortMapGsys)), 9)) == NULL) {
            return;
        }
        for (i = 0; i < 3; i++) {
            const SRTMatrix *mid = FieldExpObj_GetActorMatrixPtr(exp, 0, (u16)i);
            int mx = (int)(mid->translation.x >> 16), mz = (int)(mid->translation.z >> 16);

            m->sw |= flipped[i] ? 1u << i : 0;
            for (j = 0; j < m->nnpc && m->narm < 8; j++) {
                if (!m->npc[j].rail && abs(m->npc[j].x - mx) + abs(m->npc[j].z - mz) == 1) {
                    m->arm[m->narm].npc = j;
                    m->arm[m->narm].x1 = 2 * mx - m->npc[j].x;
                    m->arm[m->narm].z1 = 2 * mz - m->npc[j].z;
                    m->arm[m->narm].s = i;
                    m->narm++;
                }
            }
        }
        for (i = 0; i < m->ntrig; i++) {
            const PortMapTrig *t = &m->trig[i];

            if (t->rail) {
                continue;
            }
            if (t->script >= 4 && t->script <= 6 && t->type == 0 && m->nlever < 4) {
                m->lever[m->nlever].x = t->x;
                m->lever[m->nlever].z = t->z;
                m->lever[m->nlever].s = (int)t->script - 4;
                m->lever[m->nlever].x1 = t->x;
                m->lever[m->nlever].z1 = t->z;
                m->nlever++;
            } else if (t->script >= 7 && t->script <= 14 && t->type >= 1 && t->type <= 4 && m->nplace < 8) {
                int p = kIcirrusPlace[t->script - 7];

                m->place[m->nplace].x = t->x;
                m->place[m->nplace].z = t->z;
                m->place[m->nplace].dir = (int)t->type - 1;
                m->place[m->nplace].s = kIcirrusPre[p][0];
                m->place[m->nplace].want = kIcirrusPre[p][1];
                if (PortHybrid_RailUnder(t->x, t->z, (int)t->type - 1, &m->place[m->nplace].to, 0)) {
                    m->nplace++;
                }
            }
        }
    }
#else
    if (PortMap_Frigate(field, m)) {
        return;
    }
    PortMap_OpelucidLifts(field, m);
#endif
}

#if defined(BLACK2) || defined(WHITE2)
static const union {
    u8 raw[4692];
    int t[51][23];
} kMistraltonGrid = {{   // ov094 gym_mistralton.c's sMistraltonGrid: [z][x] 0 free, else a gust stops before it
#include "asset/gym_mistralton_sMistraltonGrid.inc"
}};
#endif

// PortMap's wind: Black 2 / White 2's Mistralton Gym (gimmick 5, ov094 gym_mistralton.c). Once the player has stood on its
// start row, each field event check (GymMistralton_CheckStep) off a platform counts the work block's frames; at 120 a gust
// (Mistralton_CreateRideEvent) blows the player south over sMistraltonGrid's free tiles to before the first that is not free
// or has an actor (Mistralton_CountFreeCells), and the count starts again. The tiles where it does not move him get
// PORT_MAP_SHELTER
static void PortMap_Wind(Field *field, PortMap *m) {
#if defined(BLACK2) || defined(WHITE2)
    typedef struct {   // the head of gym_mistralton.c's MistraltonWork
        u16 unused00;
        u8 stage;
        int started;
        u16 frames;
    } PortMistraltonWork;
    const PortMistraltonWork *w;
    int x, z, i;

    m->wind = 0;
    m->windHold = 0;
    if (!Field_CheckGimmickID(field, GIMMICK_MISTRALTON_GYM) || (w = Field_GetGimmickWorkBlock(field, 1)) == NULL ||
        !w->started) {
        return;
    }
    m->wind = 1;
    m->gust = 120 - w->frames;
    // GymMistralton_CheckStep counts no frame while the player is on a platform (GRID_PLATFORM, 4): a wait there is for ever
    // (b2 gym6-out: with pz19's ROM terrain the hop's way was known from the start and it waited on the platform's edge)
    m->windHold = m->px >= 0 && m->pz >= 0 && m->px < 23 && m->pz < 51 && kMistraltonGrid.t[m->pz][m->px] == 4;
    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            int gx = m->x0 + x, gz = m->z0 + z, shelter;

            if (gx < 0 || gz < 0 || gx >= 23 || gz >= 51) {
                continue;
            }
            shelter = kMistraltonGrid.t[gz][gx] != 0 || gz + 1 >= 51 || kMistraltonGrid.t[gz + 1][gx] != 0;
            for (i = 0; i < m->nnpc && !shelter; i++) {
                shelter = m->npc[i].x == gx && m->npc[i].z == gz + 1;
            }
            if (shelter) {
                m->cell[z][x].flags |= PORT_MAP_SHELTER;
            }
        }
    }
#else
    (void)field;
    m->wind = 0;
#endif
}

static void PortMap_Fill(PortMap *m) {
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    static const char ledge[4] = {'^', 'v', '<', '>'};
    Field *field;
    FieldPlayer *player;
    FieldActor *actor, *other;
    FieldG3DMapper *mapper;
    EventData *ed;
    MMSys *mmsys;
    MapTerrainBuf t;
    VecFx32 wpos;
    int x, z, i, d, head = 0, tail = 0, chunkHave[8][8];
    u32 index;
    fx32 py;

    m->ok = 0;
    if (sPortMapGsys == NULL || (field = GSYS_GetField(sPortMapGsys)) == NULL || (player = Field_GetPlayer(field)) == NULL ||
        (actor = FieldPlayer_GetActor(player)) == NULL || (mapper = Field_GetG3DMapper(field)) == NULL) {
        return;
    }
    sPortMapMMSys = GetActorMModelSystem(actor);
    CopyActorWPos(actor, &wpos);
    py = wpos.y;
    m->px = GetGPosX(actor);
    m->pz = GetGPosZ(actor);
    m->wx = wpos.x >> 16;
    m->wz = wpos.z >> 16;
    m->py = py >> 16;
    m->pdir = GetActorFaceDir(actor) <= 3 ? (int)GetActorFaceDir(actor) : -1;
    m->x0 = m->px - PORT_MAP_N / 2;
    m->z0 = m->pz - PORT_MAP_N / 2;
    m->zone = PlayerState_GetZoneID(GSYS_GetPlayerState(sPortMapGsys));
    m->rail = PortRail_At(&m->rpos);
    m->nline = 0;
    m->surf = FieldPlayer_GetExState(player) == 2;   // FLD_PLAYER_EXSTATE_SURFING
    {
        FieldActor *ra;
        NoGridMapper *rm;
        FieldRailSystem *rail = PortRail_System(&ra, &rm);

        for (i = 0; rail != NULL && i < rail->data.lineCount && i < 128; i++) {
            m->line[i].a = (int)rail->data.lines[i].pointA;
            m->line[i].b = (int)rail->data.lines[i].pointB;
            m->line[i].len = (int)rail->data.lines[i].length;
            m->nline = i + 1;
        }
    }
    {
        MapMatrix *matrix = GetMapMatrixSystem(GSYS_GetGameData(sPortMapGsys));

        m->mw = matrix != NULL ? GetMapMatrixWidth(matrix) : 0;
        m->mh = matrix != NULL ? GetMapMatrixHeight(matrix) : 0;
    }
    memset(sPortMapHave, 0, sizeof sPortMapHave);
    memset(sPortMapHave2, 0, sizeof sPortMapHave2);
    sPortMapX0 = m->x0;
    sPortMapZ0 = m->z0;
    memset(sPortMapRom, 0, sizeof sPortMapRom);
    sPortRomX0 = m->x0;
    sPortRomZ0 = m->z0;
    // the flood from the player: each tile's layer is the one at the height it is walked onto from
    if (PortMap_Terrain(mapper, m->px, m->pz, py, &t)) {
        tail = PortMap_Reach(m->px - m->x0, m->pz - m->z0, &t, tail);
    }
    head = PortMap_Flood(mapper, m, head, tail);
    tail = head;
    {   // then from the rails' points (a hybrid zone): the floors they hand the player onto (Opelucid Gym's upper walkways,
        // which run over the paths below: a tile the flood reaches on both floors gets the second as PORT_MAP_BRIDGE)
        NoGridMapper *rng;
        FieldG3DMapper *rg3d;
        FieldRailSystem *rail = PortHybrid_Rail(&rng, &rg3d);

        for (i = 0; rail != NULL && i < rail->data.pointCount; i++) {
            const VecFx32 *p = &rail->data.points[i].pos;

            x = (int)(p->x >> 16) - m->x0;
            z = (int)(p->z >> 16) - m->z0;
            if (x >= 0 && z >= 0 && x < PORT_MAP_N && z < PORT_MAP_N && PortMap_Terrain(mapper, m->x0 + x, m->z0 + z, p->y, &t) &&
                !MapTile_BlocksCollision(t.tileType)) {
                tail = PortMap_Reach(x, z, &t, tail);
            }
        }
        PortMap_Flood(mapper, m, head, tail);
    }
    // the rest at the player's height (walls, water, places the flood does not reach)
    memset(chunkHave, 0, sizeof chunkHave);
    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            if (sPortMapHave[z][x] == 0 && PortMap_Terrain(mapper, m->x0 + x, m->z0 + z, py, &t)) {
                sPortMapH[z][x] = t.height;
                sPortMapType[z][x] = t.tileType;
                sPortMapHave[z][x] = 1;
            }
            if (sPortMapHave[z][x]) {
                chunkHave[(z + (m->z0 & 31)) / 32 & 7][(x + (m->x0 & 31)) / 32 & 7] = 1;
            }
        }
    }
    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            PortMapCell *c = &m->cell[z][x];
            int gx = m->x0 + x, gz = m->z0 + z, zone = PortMap_ZoneAt(gx, gz);
            VecFx32 pos;

            c->zone = zone < 0 ? 0xffff : (u16)zone;
            c->pass = 0;
            c->y = 0;
            c->cls = 0xff;
            c->flags = 0;
            c->slide = 0;
            c->ihc = 0;
            c->yb = 0;
            c->passb = 0;
            pos.x = (gx << 16) + 0x8000;
            pos.y = py;
            pos.z = (gz << 16) + 0x8000;
            if (!sPortMapHave[z][x]) {   // not loaded (a chunk with no terrain in the window), or off the map
                c->kind = zone < 0 || gx < 0 || gz < 0 || FieldG3DMapper_IsPosOutOfBounds(mapper, &pos) ||
                                  chunkHave[(z + (m->z0 & 31)) / 32 & 7][(x + (m->x0 & 31)) / 32 & 7]
                              ? ' '
                              : '?';
                if (c->kind == '?') {
                    c->pass = 15;
                    c->slide = (unsigned char)PortMap_CannonAt(field, gx, gz);   // (the cannons' tiles are known before the floor loads)
                }
                continue;
            }
            {
                u32 type = sPortMapType[z][x], flags = GetTileFlags(type);
                u16 cls = GetTileClass(type);
                int ld = PortMap_LedgeDir(cls);

                c->cls = cls;
                // the floors that move the player (route_slide.c): ice (map_attr.c), the slope slips (field_slope_slip.c's
                // table, pokebw2's TILE_CLASS_SLOPE_SLIP_RIGHT 0x76, LEFT, UP, DOWN 0x79)
                c->slide = IsTileSlidingIce(cls) ? PORT_MAP_ICE
                           : IsTileSlopeSlip(cls) ? PORT_MAP_SLOPE + (cls == 0x76 ? 3 : cls == 0x77 ? 2 : cls == 0x78 ? 0 : 1)
                                                  : 0;
                if (c->slide == 0) {
                    c->slide = (unsigned char)PortMap_PadAt(field, gx, gz);
                }
                if (c->slide == 0) {
                    c->slide = (unsigned char)PortMap_CannonAt(field, gx, gz);
                }
                c->flags = MapTile_IsReceptionCounter(cls) ? PORT_MAP_COUNTER : 0;   // as fieldmap_event.c's talk check
                if (sPortMapRom[z][x]) {
                    c->flags |= PORT_MAP_ROM;   // not loaded: the ROM's chunk file (PortMap_RomTerrain)
                }
                if (PORT_RAIL_ISSWITCH(cls)) {   // a hybrid zone's hand-off tile (PortHybrid_ToRail)
                    c->flags |= PORT_MAP_SWITCH;
                }
                c->y = (short)(sPortMapH[z][x] >> 16);
                c->yf = (short)(sPortMapH[z][x] >> 12);
                c->gy = (short)((sPortMapH[z][x] >> 4) / FX32_ONE);   // field_actor_tool.c's HEIGHT_TO_GRID
                if (MapTile_IsSurfEdge(cls)) {
                    c->flags |= PORT_MAP_SURFEDGE;
                }
                if (IsTileWaterfall(cls)) {
                    c->flags |= PORT_MAP_FALL;
                }
                if (MapTile_IsElectricRock(cls)) {
                    c->flags |= PORT_MAP_ELECTRIC;
                }
                if (MapTile_IsStrengthHole(cls)) {   // a wall, or floor when a boulder fills it (PortMap_Terrain)
                    c->flags |= PORT_MAP_HOLE;
                }
                c->kind = MapTile_BlocksCollision(type) ? (ld >= 0 ? ledge[ld] : (c->flags & PORT_MAP_SURFEDGE) ? 'e'
                                                                                : (c->flags & PORT_MAP_FALL)  ? 'F'
                                                                                                              : '#')
                          : (flags & 2)                ? '~'
                          : (flags & 4)                ? ','
                                                       : '.';
                if (sPortMapHave2[z][x]) {   // a second floor (PortMap_Reach): the planner crosses it straight
                    u32 t2 = sPortMapType2[z][x];

                    c->flags |= PORT_MAP_BRIDGE;
                    c->yb = (short)(sPortMapH2[z][x] >> 12);
                    c->passb = (unsigned char)PortMap_Pass(x, z, t2, sPortMapH2[z][x], (GetTileFlags(t2) & 2) ? '~' : '.');
                }
                if (c->kind == '#' || c->kind == 'e' || c->kind == 'F' || (c->kind == '~' && !m->surf) || ld >= 0) {
                    continue;
                }
                c->pass = (unsigned char)PortMap_Pass(x, z, type, sPortMapH[z][x], c->kind);
            }
        }
    }
    PortMap_OpenUnloaded(m);   // a walkable tile (the water too, surfing) next to an unloaded one may step onto it
    // the zone's events
    m->nwarp = m->ntrig = m->nbg = m->nnpc = 0;
    ed = GameData_GetEventData(GSYS_GetGameData(sPortMapGsys));
    if (ed != NULL) {
        for (i = 0; i < ed->warpCount && ed->warps != NULL && m->nwarp < 64; i++) {
            const ZoneWarp *w = &ed->warps[i];
            PortMapWarp *o = &m->warp[m->nwarp];

            o->rail = w->isRail == 1;
            if (o->rail) {
                o->line = w->pos.rail.componentId;
                o->x = w->pos.rail.posFront;
                o->z = w->pos.rail.posSide;
                o->w = w->pos.rail.frontSpan;
                o->h = w->pos.rail.sideSpan;
            } else {
                o->line = 0;
                o->x = (s16)w->pos.grid.x / 16;
                o->z = (s16)w->pos.grid.z / 16;
                o->w = w->pos.grid.width;
                o->h = w->pos.grid.height;
            }
            o->zone = PORT_WARP_ZONE(w);
            o->dest = w->destId;
            o->dir = PORT_WARP_DIR(w);
            o->type = w->transitionType;
            m->nwarp++;
        }
        for (i = 0; i < ed->triggerCount && ed->triggers != NULL && m->ntrig < 64; i++) {
            const ZoneTrigger *g = &ed->triggers[i];
            PortMapTrig *o = &m->trig[m->ntrig];

            o->rail = g->isRail == 1;
            if (o->rail) {
                o->line = g->pos.rail.componentId;
                o->x = g->pos.rail.posFront;
                o->z = g->pos.rail.posSide;
                o->w = g->pos.rail.frontSpan;
                o->h = g->pos.rail.sideSpan;
            } else {
                o->line = 0;
                o->x = g->pos.grid.x;
                o->z = g->pos.grid.z;
                o->w = g->pos.grid.width;
                o->h = g->pos.grid.height;
            }
            o->script = g->scrId;
            o->workId = g->workId;
            o->workValue = g->workValue;
            o->type = g->type;
            m->ntrig++;
        }
        for (i = 0; i < ed->entityCount && ed->entities != NULL && m->nbg < 64; i++) {
            const ZoneBGEntity *b = &ed->entities[i];
            const s32 *coords = (const s32 *)&b->pos;
            PortMapBg *o = &m->bg[m->nbg];

            o->rail = b->isRail == 1;
            if (o->rail) {
                o->line = b->pos.rail.componentId;
                o->x = b->pos.rail.posFront;
                o->z = (s16)b->pos.rail.posSide;
            } else {
                o->line = 0;
                o->x = coords[0];
                o->z = coords[1];
            }
            o->script = b->scrId;
            o->dir = b->direction;
            o->cond = b->condition;
            m->nbg++;
        }
    }
    mmsys = GetActorMModelSystem(actor);
    index = 0;
    while (mmsys != NULL && NextActor(mmsys, &other, &index) && m->nnpc < 64) {
        PortMapNpc *o;

        if (other == actor) {
            continue;
        }
        o = &m->npc[m->nnpc++];
        o->id = GetActorUID(other);
        o->x = other->gPos.x;
        o->z = other->gPos.z;
        o->x0 = other->initGPos.x;
        o->z0 = other->initGPos.z;
        o->y = other->gPos.y;   // what CheckActorNewPosOtherActorCollision compares (FldAct_GetGPosY, FldAct_GetInitGPosY)
        o->y0 = other->initGPos.y;
        o->layer = 1;
        o->rail = CheckActorFlag(other, 0x2000) ? 1 : 0;
        o->line = 0;
        if (o->rail) {
            RailPosition r;

            GetActorRailPos(other, &r);
            o->line = r.componentId;
            o->x = o->x0 = r.posFront;
            o->z = o->z0 = r.posSide;
        }
        o->code = FldAct_GetObjCode(other);
        o->move = GetActorMoveCode(other);
        o->w = o->rail ? 1 : other->collisionWidth;
        o->h = o->rail ? 1 : other->collisionHeight;
        o->nocoll = CheckActorFlag(other, 0x80) ? 1 : 0;   // SetActorCollisionEnabled(FALSE): a boulder in its hole
        o->breaks = 0;   // (Gen 5 has no actor a slide smashes)
    }
    PortMap_Switches(field, m);
    PortMap_Wind(field, m);
    m->ok = 1;
}

// G:wall: Castelia Gym's honey walls (Black, White: ov038 gym_insect.c, zone 29, the 7 actors of object code 0). Held at
// the cell before one, a key walks the player through its pass event while its switches are pressed (the event ends when
// the key is let go), to 2 tiles past its cell. Black 2 and White 2: none in the routes yet
static int PortMap_PassActor(unsigned zone, const PortMapNpc *n) {
#if defined(BLACK2) || defined(WHITE2)
    (void)zone;
    (void)n;
    return 0;
#else
    return zone == 29 && !n->rail && n->code == 0 ? 2 : 0;
#endif
}

// G:ride: Nimbasa's roller coaster (Black, White: ov039 gimmick_roller_coaster.c, gimmick 2, zone 63; Black 2, White 2: ov102, gimmick
// 48, zone 585), n the spot (0..8) the player boards at: the spot's tile, facing up. The car that waits at a spot is the live work's
// stations[car]; the switch state (the gimmick's saved switches[car]) says which of the car's two paths it runs, and
// the path's two stations are the spots it stops at (each row of the game's station table): a spot off the car's path is
// closed ("switch T<n> wrong for spot X", T<n> the trigger of its switch). The tables are the games' own asset .inc files and
// the work and save are read through mirrors of the game's file-local structs (same layout in both games; checked by paths[car] / 2
// == car). Nothing is changed in the game
#if defined(BLACK2) || defined(WHITE2)
#define PORT_COASTER_GIMMICK 48
#define PORT_COASTER_ON(f) Field_CheckGimmickID((f), GIMMICK_ROLLER_COASTER)
#else
#define PORT_COASTER_GIMMICK 2
#define PORT_COASTER_ON(f) func_ov021_021be288((f), 2)
#endif
typedef struct {
    void *task;
    u8 ridePath;
    s8 switchId, switchState, waitCar;
    s32 waits[4];
    void *curve;
    u8 curveBuffer[0x21c0];
    u8 unused[8];
    BOOL riding;
    u8 pendingSwitch[4];
    u8 paths[4];
    s8 stations[4];
} PortCoasterWork;
typedef struct {
    s32 frames[4];
    u8 switches[4];
    u16 introSeen[2];
} PortCoasterSave;
typedef struct {
    u16 x, z;
} PortCoasterSpot;
#define NO_SPOT (-1)
// (a declaration per game: tools/port_assets.py rewrites a table whose #include opens its initialiser)
#if defined(BLACK2) || defined(WHITE2)
static const PortCoasterSpot kCoasterSpots[9] = {
#include "asset/gimmick_roller_coaster_sSpots.inc"
};
static const s8 kCoasterStations[8][2] = {
#include "asset/gimmick_roller_coaster_sStationSpots.inc"
};
#else
static const PortCoasterSpot kCoasterSpots[9] = {
#include "asset/gimmick_roller_coaster_data_ov039_021f4c58.inc"
};
static const s8 kCoasterStations[8][2] = {
#include "asset/gimmick_roller_coaster_data_ov039_021f4c28.inc"
};
#endif
#undef NO_SPOT
static const u8 kCoasterCar[9] = {0, 0, 0, 1, 1, 2, 2, 3, 3};   // the car of each spot
#if defined(BLACK2) || defined(WHITE2)
static const u8 kCoasterTrig[4] = {0, 1, 2, 3};                  // Black 2/White 2 zone 585: trigger Tn's script is RollerCoasterCallSwitch n
#else
static const u8 kCoasterTrig[4] = {2, 1, 0, 3};                  // the zone's trigger that throws each car's switch (T2 at 29,33 ...)
#endif

static int PortCoaster_Get(PortCoasterWork **w, PortCoasterSave **s) {
    Field *field = sPortMapGsys != NULL ? GSYS_GetField(sPortMapGsys) : NULL;
    int i;

    if (field == NULL || !PORT_COASTER_ON(field)) {
        return 0;
    }
    *w = Field_GetGimmickWorkBlock(field, 1);
    *s = GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(sPortMapGsys)), PORT_COASTER_GIMMICK);
    for (i = 0; *w != NULL && *s != NULL && i < 4; i++) {
        if ((*w)->paths[i] / 2 != i) {
            return 0;
        }
    }
    return *w != NULL && *s != NULL;
}

// 1 the spot is on its car's current path, else -1 (the switch is wrong), 0 no such spot
static int PortCoaster_OnPath(const PortCoasterSave *s, int n) {
    int path;

    if (n < 0 || n >= 9) {
        return 0;
    }
    path = kCoasterCar[n] * 2 + s->switches[kCoasterCar[n]];
    return kCoasterStations[path][0] == n || kCoasterStations[path][1] == n ? 1 : -1;
}

static const char *PortMap_RideWhy(unsigned zone, int n) {
    static char sBuf[128];
    PortCoasterWork *w;
    PortCoasterSave *s;

    (void)zone;
    if (!PortCoaster_Get(&w, &s) || PortCoaster_OnPath(s, n) >= 0) {
        return NULL;
    }
    snprintf(sBuf, sizeof sBuf, "switch T%d wrong for spot %d (car %d runs path %d: G:trig:%d throws it)", kCoasterTrig[kCoasterCar[n]],
             n, kCoasterCar[n], kCoasterCar[n] * 2 + s->switches[kCoasterCar[n]], kCoasterTrig[kCoasterCar[n]]);
    return sBuf;
}

// -1 not the coaster; else 1 the player rides, 2 the car stands at a station
static int PortMap_RideState(unsigned zone, int n) {
    PortCoasterWork *w;
    PortCoasterSave *s;

    (void)zone;
    if (!PortCoaster_Get(&w, &s) || n < 0 || n >= 9) {
        return -1;
    }
    return (w->riding ? 1 : 0) | (w->stations[kCoasterCar[n]] != -1 ? 2 : 0);
}

// G:ride: Black 2 and White 2's Castelia Gym cocoons (ov091 gym_insect.c: GymInsect_FindRide over sRides; the start tile,
// direction and floor of each entry are taken from pokebw2's sRides, in its order, so N is the game's ride index). A ride
// starts when the player, two tiles before its entry tile and on its floor, holds the key toward it (the field's
// walk-into-a-wall check: fieldmap_event.c). Entries 9 and 15 are zone 488's (rides from the tile the player stands on):
// none here. Ride 8 waits for the gym's flag 0x109 (GymInsect_FindRide's RIDE_OBJECT_0)
static int PortMap_Ride(unsigned zone, int n, int *x, int *z, int *y, int *dir) {
    PortCoasterWork *cw;
    PortCoasterSave *cs;

    if (PortCoaster_Get(&cw, &cs)) {   // the coaster: y stays the player's floor
        int on = PortCoaster_OnPath(cs, n);

        if (on > 0) {
            *x = kCoasterSpots[n].x;
            *z = kCoasterSpots[n].z;
            *dir = 0;
        }
        return on;
    }
#if defined(BLACK2) || defined(WHITE2)
    static const u8 kRides[16][4] = {   // dir, x, y (the floor's height: 0, 10, 20), z of the entry tile
        {0, 16, 0, 11},  {0, 16, 10, 11}, {2, 16, 0, 11}, {0, 3, 10, 3},   {0, 26, 0, 4}, {0, 25, 20, 15},
        {2, 26, 0, 4},   {0, 11, 20, 3},  {0, 8, 0, 4},   {9, 0, 0, 0},    {0, 28, 10, 3}, {0, 20, 20, 5},
        {2, 11, 20, 3},  {0, 4, 20, 14},  {3, 4, 20, 14}, {9, 0, 0, 0},
    };
    Field *field = sPortMapGsys != NULL ? GSYS_GetField(sPortMapGsys) : NULL;
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};

    // Humilau Gym's platforms (ov101 gym_humilau.c sRides, in its order: x, z, dir of the spot; the platform's lane length is the
    // game's business). The ride starts when the player STANDS on the spot and walks into the water ahead
    // (fieldmap_event.c: GymHumilau_CheckRide), so the start tile is the spot itself. It needs a platform two tiles ahead
    // (the game decides: a spot with none just bumps and the pilot says the ride did not start)
    static const u8 kHumilau[37][3] = {
        {5, 10, 3},  {9, 10, 2},  {10, 11, 1}, {16, 11, 1}, {22, 11, 1}, {25, 11, 1}, {25, 15, 0}, {5, 19, 3},  {12, 19, 2},
        {7, 21, 0},  {8, 22, 3},  {24, 22, 2}, {22, 24, 0}, {10, 27, 0}, {6, 29, 1},  {4, 31, 3},  {24, 31, 2}, {9, 35, 1},
        {25, 35, 1}, {4, 37, 3},  {6, 39, 0},  {9, 39, 0},  {25, 39, 0}, {19, 47, 1}, {25, 47, 1}, {14, 49, 3}, {25, 51, 0},
        {19, 54, 0}, {13, 56, 1}, {25, 56, 1}, {5, 58, 3},  {25, 60, 0}, {13, 63, 0}, {16, 63, 0}, {5, 64, 3},  {12, 64, 2},
        {5, 13, 3},
    };

    if (field != NULL && zone != 488 && Field_CheckGimmickID(field, GIMMICK_HUMILAU_GYM)) {
        if (n < 0 || n >= 37) {
            return 0;
        }
        *x = kHumilau[n][0];
        *z = kHumilau[n][1];
        *dir = kHumilau[n][2];
        return 1;
    }
    if (zone == 488 || field == NULL ||!Field_CheckGimmickID(field, GIMMICK_CASTELIA_GYM) || n < 0 || n >= 16 || kRides[n][0] > 3) {
        return 0;
    }
    if (n == 8 && !EventWork_FlagGet(GameData_GetEventWork(GSYS_GetGameData(sPortMapGsys)), 0x109)) {
        return -1;
    }
    *dir = kRides[n][0];
    *x = kRides[n][1] - 2 * dx[*dir];
    *y = kRides[n][2];
    *z = kRides[n][3] - 2 * dz[*dir];
    return 1;
#else
    (void)zone;
    (void)n;
    (void)x;
    (void)z;
    (void)y;
    (void)dir;
    return 0;
#endif
}

// the adapter's gridSwitches: the Plasma Frigate's floor with switches sw pressed once each from now (the game's own
// SetPanelArea steps on its height areas, switch 0 first), m refilled; set 0: the areas as the game had them, refilled
static void PortMap_GridSwitches(PortMap *m, int set, unsigned sw) {
#if defined(BLACK2) || defined(WHITE2)
    static HeightExEntry saved[32];
    static int nsaved = -1;
    Field *field;
    PortFrigateWork *w;
    PortHeightExList *list;
    BOOL on[16];
    int i, j;

    if (sPortMapGsys == NULL || (field = GSYS_GetField(sPortMapGsys)) == NULL || (w = PortMap_FrigateWork(field)) == NULL ||
        (list = (PortHeightExList *)FieldG3DMapper_GetHeightEx(Field_GetG3DMapper(field))) == NULL || list->count > 32) {
        return;
    }
    if (!set) {
        if (nsaved >= 0) {
            memcpy(list->entries, saved, (size_t)nsaved * sizeof *saved);
            nsaved = -1;
            PortMap_Fill(m);
        }
        return;
    }
    if (nsaved < 0) {
        nsaved = list->count;
        memcpy(saved, list->entries, (size_t)nsaved * sizeof *saved);
    } else {
        memcpy(list->entries, saved, (size_t)nsaved * sizeof *saved);
    }
    memcpy(on, w->panelOn, sizeof on);
    for (i = 0; i < 4; i++) {
        if ((sw >> i) & 1) {
            for (j = 4 * i; j < 4 * i + 4; j++) {
                on[j] = !on[j];
                PortMap_FrigateArea((HeightExList *)list, on, j);
            }
        }
    }
    PortMap_Fill(m);
#else
    (void)m;
    (void)set;
    (void)sw;
#endif
}

static const PortRouteAdapter kPortRouteGen5 = {"gen5",        PortMap_Fill,      PortMap_ZoneAt,   PortRail_At,
                                                PortRail_Step, PortHybrid_ToGrid, PortHybrid_ToRail, PortMap_PassActor,
                                                PortMap_Ride,      PortMap_RideState, PortMap_RideWhy, PortHybrid_Land,
                                                PortMap_RailSwitches, PortMap_GridSwitches};
