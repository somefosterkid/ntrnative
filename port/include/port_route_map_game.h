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
#include "field/event_data.h"
#include "field/zone.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "port_route_map.h"

#if defined(BLACK2) || defined(WHITE2)
#define PORT_WARP_ZONE(w) ((w)->destZoneId)
#define PORT_WARP_DIR(w) ((w)->direction)
#else
#define PORT_WARP_ZONE(w) ((w)->unk0)
#define PORT_WARP_DIR(w) ((w)->unk4)
#endif

static GameSystem *sPortMapGsys;

static BOOL PortMap_Terrain(FieldG3DMapper *mapper, int x, int z, fx32 y, MapTerrainBuf *t) {
    VecFx32 pos;

    pos.x = (x << 16) + 0x8000;
    pos.y = y;
    pos.z = (z << 16) + 0x8000;
    return FieldG3DMapper_GetTerrain(mapper, &pos, t);
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

// scratch of the fill (static: 128 x 128 tiles)
static fx32 sPortMapH[PORT_MAP_N][PORT_MAP_N];
static u32 sPortMapType[PORT_MAP_N][PORT_MAP_N];
static u8 sPortMapHave[PORT_MAP_N][PORT_MAP_N];   // 1 terrain found, 2 also reached by the flood
static u16 sPortMapQ[PORT_MAP_N * PORT_MAP_N];

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
    CopyActorWPos(actor, &wpos);
    py = wpos.y;
    m->px = GetGPosX(actor);
    m->pz = GetGPosZ(actor);
    m->py = py >> 16;
    m->pdir = GetActorFaceDir(actor) <= 3 ? (int)GetActorFaceDir(actor) : -1;
    m->x0 = m->px - PORT_MAP_N / 2;
    m->z0 = m->pz - PORT_MAP_N / 2;
    m->zone = PlayerState_GetZoneID(GSYS_GetPlayerState(sPortMapGsys));
    {
        MapMatrix *matrix = GetMapMatrixSystem(GSYS_GetGameData(sPortMapGsys));

        m->mw = matrix != NULL ? GetMapMatrixWidth(matrix) : 0;
        m->mh = matrix != NULL ? GetMapMatrixHeight(matrix) : 0;
    }
    memset(sPortMapHave, 0, sizeof sPortMapHave);
    // the flood from the player: each tile's layer is the one at the height it is walked onto from
    if (PortMap_Terrain(mapper, m->px, m->pz, py, &t)) {
        x = m->px - m->x0;
        z = m->pz - m->z0;
        sPortMapH[z][x] = t.height;
        sPortMapType[z][x] = t.tileType;
        sPortMapHave[z][x] = 2;
        sPortMapQ[tail++] = (u16)(z * PORT_MAP_N + x);
    }
    while (head < tail) {
        int cx = sPortMapQ[head] % PORT_MAP_N, cz = sPortMapQ[head] / PORT_MAP_N;
        u16 cls = GetTileClass(sPortMapType[cz][cx]);

        head++;
        for (d = 0; d < 4; d++) {
            int nx = cx + dx[d], nz = cz + dz[d];
            fx32 diff;

            if (nx < 0 || nz < 0 || nx >= PORT_MAP_N || nz >= PORT_MAP_N || sPortMapHave[nz][nx] == 2) {
                continue;
            }
            if (!PortMap_Terrain(mapper, m->x0 + nx, m->z0 + nz, sPortMapH[cz][cx], &t)) {
                continue;
            }
            if (MapTile_BlocksCollision(t.tileType)) {   // a ledge: the jump lands a tile beyond
                int lx = nx + dx[d], lz = nz + dz[d];

                if (PortMap_LedgeDir(GetTileClass(t.tileType)) == d && lx >= 0 && lz >= 0 && lx < PORT_MAP_N &&
                    lz < PORT_MAP_N && sPortMapHave[lz][lx] != 2 &&
                    PortMap_Terrain(mapper, m->x0 + lx, m->z0 + lz, sPortMapH[cz][cx], &t) &&
                    !MapTile_BlocksCollision(t.tileType)) {
                    sPortMapH[lz][lx] = t.height;
                    sPortMapType[lz][lx] = t.tileType;
                    sPortMapHave[lz][lx] = 2;
                    sPortMapQ[tail++] = (u16)(lz * PORT_MAP_N + lx);
                }
                continue;
            }
            diff = t.height - sPortMapH[cz][cx];
            if (diff < 0) {
                diff = -diff;
            }
            if (diff >= FX32_CONST(20) || (GetTileFlags(t.tileType) & 2) || PortMap_ExitBlocked(d, cls) ||
                PortMap_EnterBlocked(d, GetTileClass(t.tileType))) {
                continue;
            }
            sPortMapH[nz][nx] = t.height;
            sPortMapType[nz][nx] = t.tileType;
            sPortMapHave[nz][nx] = 2;
            sPortMapQ[tail++] = (u16)(nz * PORT_MAP_N + nx);
        }
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
                }
                continue;
            }
            {
                u32 type = sPortMapType[z][x], flags = GetTileFlags(type);
                u16 cls = GetTileClass(type);
                int ld = PortMap_LedgeDir(cls);

                c->cls = cls;
                c->flags = MapTile_IsReceptionCounter(cls) ? PORT_MAP_COUNTER : 0;   // as fieldmap_event.c's talk check
                c->y = (short)(sPortMapH[z][x] >> 16);
                c->kind = MapTile_BlocksCollision(type) ? (ld >= 0 ? ledge[ld] : '#')
                          : (flags & 2)                ? '~'
                          : (flags & 4)                ? ','
                                                       : '.';
                if (c->kind == '#' || c->kind == '~' || ld >= 0) {
                    continue;
                }
                for (d = 0; d < 4; d++) {
                    int nx = x + dx[d], nz = z + dz[d];
                    fx32 diff;

                    if (PortMap_ExitBlocked(d, cls)) {
                        continue;
                    }
                    if (nx < 0 || nz < 0 || nx >= PORT_MAP_N || nz >= PORT_MAP_N) {
                        c->pass |= 1 << d;   // off the window: let the planner try
                        continue;
                    }
                    if (!sPortMapHave[nz][nx]) {
                        continue;   // ('?' tiles get theirs below)
                    }
                    type = sPortMapType[nz][nx];
                    diff = sPortMapH[nz][nx] - sPortMapH[z][x];
                    if (diff < 0) {
                        diff = -diff;
                    }
                    if (MapTile_BlocksCollision(type) || (GetTileFlags(type) & 2) || diff >= FX32_CONST(20) ||
                        PortMap_EnterBlocked(d, GetTileClass(type))) {
                        continue;
                    }
                    c->pass |= 1 << d;
                }
            }
        }
    }
    for (z = 0; z < PORT_MAP_N; z++) {   // a walkable tile next to an unloaded one may step onto it
        for (x = 0; x < PORT_MAP_N; x++) {
            PortMapCell *c = &m->cell[z][x];

            if (c->kind != '.' && c->kind != ',') {
                continue;
            }
            for (d = 0; d < 4; d++) {
                int nx = x + dx[d], nz = z + dz[d];

                if (nx >= 0 && nz >= 0 && nx < PORT_MAP_N && nz < PORT_MAP_N && m->cell[nz][nx].kind == '?') {
                    c->pass |= 1 << d;
                }
            }
        }
    }
    // the zone's events
    m->nwarp = m->ntrig = m->nbg = m->nnpc = 0;
    ed = GameData_GetEventData(GSYS_GetGameData(sPortMapGsys));
    if (ed != NULL) {
        for (i = 0; i < ed->warpCount && ed->warps != NULL && m->nwarp < 64; i++) {
            const ZoneWarp *w = &ed->warps[i];
            PortMapWarp *o = &m->warp[m->nwarp];

            if (w->isRail == 1) {
                continue;
            }
            o->x = (s16)w->pos.grid.x / 16;
            o->z = (s16)w->pos.grid.z / 16;
            o->w = w->pos.grid.width;
            o->h = w->pos.grid.height;
            o->zone = PORT_WARP_ZONE(w);
            o->dest = w->destId;
            o->dir = PORT_WARP_DIR(w);
            o->type = w->transitionType;
            m->nwarp++;
        }
        for (i = 0; i < ed->triggerCount && ed->triggers != NULL && m->ntrig < 64; i++) {
            const ZoneTrigger *g = &ed->triggers[i];
            PortMapTrig *o = &m->trig[m->ntrig];

            if (g->isRail == 1) {
                continue;
            }
            o->x = g->pos.grid.x;
            o->z = g->pos.grid.z;
            o->w = g->pos.grid.width;
            o->h = g->pos.grid.height;
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

            if (b->isRail == 1) {
                continue;
            }
            o->x = coords[0];
            o->z = coords[1];
            o->script = b->scrId;
            o->dir = b->direction;
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
        o->code = FldAct_GetObjCode(other);
        o->move = GetActorMoveCode(other);
    }
    m->ok = 1;
}

static const PortRouteAdapter kPortRouteGen5 = {"gen5", PortMap_Fill, PortMap_ZoneAt};
