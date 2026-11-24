// The route pilot's Gen 4 adapter (port/include/port_route_map.h's PortRouteAdapter) and frame hook, Platinum (pret's
// pokeplatinum names; HeartGold / SoulSilver in the second half of this file, under HEARTGOLD). Compiled into pl.o with the game's
// headers (port/meson.build's main_files); its calls into the exe (PortRoute_Frame, PortRouteMap_SetAdapter) bind at the
// exe's link. The one game-side hunk: pokeplatinum-route.patch calls PortRoute4_Frame at the end of each field frame
// (field_system.c HandleInputsEventsAndProcesses). Test-only, as port_route.h; nothing here runs unless the pilot is on.
//
// What the fill gives the planner (one layer, as port_route_map.h's Gen 4 note says):
//   - kind from the tile attributes (TerrainCollisionManager: the collision bit, the behavior): '#' collision, '~' surfable,
//     ',' tall grass, '^' 'v' '<' '>' the jump ledges, '.' else; '?' a chunk the land data manager has not loaded.
//   - pass: the step to a neighbour the game takes (map_object_move.c sub_02064004: the side blocks of both tiles, the
//     null behavior; terrain_collision_manager.c: the collision bit and a height step of 20 units or more).
//   - y / yf: the BDHC height at the tile from the player's height (TerrainCollisionManager_GetHeight). Not done: a second
//     plate (bridges: PORT_MAP_BRIDGE), the Distortion World, dynamic heights.
//   - warps (1 x 1), coord events (triggers), BG events, map objects; zoneAt from the live MapMatrix.
#include <string.h>

#if !defined(HEARTGOLD)   // Platinum
#include "field/field_system.h"
#include "field/field_system_sub2_t.h"
#include "struct_defs/events.h"

#include "field_map_change.h"
#include "field_system.h"
#include "field_task.h"
#include "poketch.h"
#include "script_manager.h"
#include "location.h"
#include "map_header_data.h"
#include "map_matrix.h"
#include "map_object.h"
#include "map_object_move.h"
#include "map_tile_behavior.h"
#include "overlay005/map_name_popup.h"
#include "menu.h"
#include "player_avatar.h"
#include "start_menu.h"
#include "terrain_collision_manager.h"
#include "bag.h"
#include "constants/items.h"
#include "constants/field/dynamic_map_features.h"
#include "dynamic_map_features.h"
#include "persisted_map_features.h"
#include "savedata_misc.h"
#include "heap.h"
#include "savedata.h"
#include "battle/battle_lib.h"
#include "battle/battle_system.h"
#include "move_table.h"
#include "overlay005/map_prop.h"
#include "overlay005/land_data.h"
#include "overlay005/dynamic_terrain_height.h"
#include "terrain_collision_manager.h"
#include "constants/quadrant.h"
#include "generated/abilities.h"
#include "generated/bg_event_dirs.h"
#include "generated/bg_event_types.h"
#include "generated/vars_flags.h"
#include "generated/pokemon_types.h"
#include "constants/battle.h"
#include "encounter.h"
#include "generated/trainer_types.h"
#include "script_manager.h"
#include "field_battle_data_transfer.h"
#include "overlay006/wild_encounters.h"
#include "party.h"
#include "applications/town_map/defs.h"
#include "applications/town_map/fly_locations.h"
#include "applications/town_map/map_blocks.h"
#include "pokemon.h"
#include "save_player.h"
#include "trainer_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "system.h"

#include "port_route.h"
#include "port_route_map.h"

void PortRoute4_Frame(FieldSystem *fieldSystem);
void PortSpeed4_MoveLog(int gx, int gz, int px, int pz, int fgx, int fgz, int fstate, int moveState);   // port/game/speed_gen4.c (BWPORT_MOVELOG4)
void PortRoute4_StartMenu(const StartMenu *menu, int saveOption, int pokemonOption, int bagOption);
void PortRoute4_YesNo(int row);
void PortRoute4_Saved(void);
void PortRoute4_DeepMud(void);

static int sPr4Mud;   // the deep-mud task ran this frame (pokeplatinum-route.patch: FieldTask_StuckInDeepMud calls PortRoute4_DeepMud)

// the deep-mud task (overlay005, ov5_021DFB54.c): stepping onto a mud tile starts it and it ends only after five turns (a
// direction held that differs from the one the player faces, once the last turn's animation is done); the frame hook sets
// PortRouteState.mud to the key that turns him, so the pilot presses turns in turn until the task ends (docs/platinum.md)
void PortRoute4_DeepMud(void) {
    sPr4Mud = 1;
}

extern int gPortRouteFieldYesNo;   // port/src/route_test.c: the field's yes/no list is up, 1 + its cursor's row (Py, Pn)

#define PR4_TILE (16 << FX32_SHIFT)   // a tile in world units (MAP_OBJECT_TILE_SIZE)

static FieldSystem *sPr4Fs;   // set by the frame hook while the field map runs

static int Pr4_FieldUp(const FieldSystem *fs) {
    return fs != NULL && fs->runningFieldMap && fs->processManager != NULL && fs->processManager->parent != NULL &&
           fs->playerAvatar != NULL && fs->landDataMan != NULL && fs->mapMatrix != NULL && fs->terrainCollisionMan != NULL &&
           fs->mapHeaderData != NULL;
}

// the map matrix's zone (map header) at grid tile x,z: 32 x 32 tile chunks, the live matrix. The land data's own offset
// (the Distortion World floors sit at an offset of their matrix: terrain_collision_manager.c GetTileAttributes subtracts it
// the same way; 0 everywhere else) is taken off first, so a floor's cells are found where the player stands
static int Pr4_ZoneAt(int x, int z) {
    const MapMatrix *mm = sPr4Fs != NULL ? sPr4Fs->mapMatrix : NULL;

    if (mm != NULL && sPr4Fs->landDataMan != NULL) {
        x -= LandDataManager_GetOffsetTileX(sPr4Fs->landDataMan);
        z -= LandDataManager_GetOffsetTileZ(sPr4Fs->landDataMan);
    }
    if (mm == NULL || x < 0 || z < 0 || x / 32 >= MapMatrix_GetWidth(mm) || z / 32 >= MapMatrix_GetHeight(mm)) {
        return -1;
    }
    return MapMatrix_GetMapHeaderIDAtCoords(mm, x / 32, z / 32);
}

static int Pr4_Attr(int x, int z, u16 *a) {
    if (x < 0 || z < 0 || Pr4_ZoneAt(x, z) < 0) {
        return 0;
    }
    return sPr4Fs->terrainCollisionMan->getTileAttributes(sPr4Fs, x, z, a) ? 1 : 0;
}

static BOOL (*const kPr4ExitBlock[4])(u8) = {TileBehavior_BlocksMovementNorthward, TileBehavior_BlocksMovementSouthward,
                                             TileBehavior_BlocksMovementWestward, TileBehavior_BlocksMovementEastward};
static BOOL (*const kPr4EnterBlock[4])(u8) = {TileBehavior_BlocksMovementSouthward, TileBehavior_BlocksMovementNorthward,
                                              TileBehavior_BlocksMovementEastward, TileBehavior_BlocksMovementWestward};

static u16 sPr4A[PORT_MAP_N][PORT_MAP_N];
static u8 sPr4Have[PORT_MAP_N][PORT_MAP_N];
static fx32 sPr4H[PORT_MAP_N][PORT_MAP_N];

// a warp's walk-in (PortMapWarp.dir: the facing it is arrived with, + 1; the planner presses the opposite on it): the tile
// behaviors field_control.c's Field_CheckMapTransition / Field_CheckTransition take (a mat or stairs: on it, the key that
// way into the wall; a door: the key into it from a neighbour, dir 0)
static int Pr4_WarpDir(u8 b) {
    if (TileBehavior_IsWarpEntranceSouth(b) || TileBehavior_IsWarpSouth(b)) {
        return DIR_NORTH + 1;   // press south
    }
    if (TileBehavior_IsWarpEntranceNorth(b) || TileBehavior_IsWarpNorth(b)) {
        return DIR_SOUTH + 1;
    }
    if (TileBehavior_IsWarpEntranceEast(b) || TileBehavior_IsWarpEast(b) || TileBehavior_IsWarpStairsEast(b)) {
        return DIR_WEST + 1;
    }
    if (TileBehavior_IsWarpEntranceWest(b) || TileBehavior_IsWarpWest(b) || TileBehavior_IsWarpStairsWest(b)) {
        return DIR_EAST + 1;
    }
    return 0;
}

// the dynamic map features' collision (dynamic_map_features.c: the gyms with moving parts, the villa's furniture, the Distortion
// World's two teleport tiles): terrain_collision_manager.c TerrainCollisionManager_WillPlayerCollide asks the hook first, with
// the avatar's height, and the terrain attributes only when it answers FALSE. The fill asks it per tile at the avatar's
// height, only while a feature is active (its data is built: Pastoria's gym keeps none) and, for Canalave's per-floor tables
// (32 x 32 tiles, unchecked in the game), inside the table. Sets or clears the attribute's collision bit (bit 15).
static int Pr4_DynOn(FieldSystem *fs) {
    int id;

    if (fs->unk_04 == NULL || fs->saveData == NULL) {
        return 0;
    }
    id = PersistedMapFeatures_GetID(MiscSaveBlock_GetPersistedMapFeatures(FieldSystem_GetSaveData(fs)));
    if (id == DYNAMIC_MAP_FEATURES_NONE || id < 0 || id >= DYNAMIC_MAP_FEATURES_COUNT) {
        return 0;
    }
    if (id == DYNAMIC_MAP_FEATURES_PASTORIA_GYM) {   // its hook compares the avatar's own height with the three water levels: the
        return 0;                                    // grid is read at one height, and the gym's walk changes it (the dynamic heights are read)
    }
    return fs->unk_04->dynamicMapFeaturesData != NULL ? id : 0;
}

static void Pr4_DynTile(FieldSystem *fs, int id, int gx, int gz, fx32 py, u16 *a) {
    BOOL coll = FALSE;

    if (id == DYNAMIC_MAP_FEATURES_CANALAVE_GYM && (gx < 0 || gz < 0 || gx >= 32 || gz >= 32 || py < 0 || py >= 40 * PR4_TILE)) {
        return;
    }
    if (DynamicMapFeatures_CheckCollision(fs, gx, gz, py, &coll)) {
        *a = coll ? (u16)(*a | 0x8000) : (u16)(*a & 0x7fff);
    }
}

BOOL CanalaveGym_PortPlatformAt(FieldSystem *fs, int index, int *x, int *y, int *z);   // pokeplatinum-route2.patch
int DistWorld_PortJumpPoint(FieldSystem *fs, int index, int v[12]);   // pokeplatinum-route3.patch

static int Pr4_Sees(MapObject *obj);

int DistWorld_PortPlatform(FieldSystem *fs, int index, int v[10], u16 *attr, int n);   // pokeplatinum-route3.patch

// The Distortion World's floating platforms (the floating floors, walls and ceilings: ov9 FloatingPlatform templates). On one
// of them the game moves the player by the platform's own grid (player_move.c PlayerAvatar_GetStepDistortionWorldPos,
// GetCurrentFloatingPlatformTileAttributes), not by the map's terrain, so the planner gets that grid. Its axes are put
// where the keys go, so the pilot's walk needs no key remap: a floating floor is the world's own x,z; on a west wall the
// planner's X is PR4_DW_W - z (Right walks z-1) and its Z is PR4_DW_W - y (Up climbs y+1); an east wall X = PR4_DW_W + z
// (Right z+1), Z as the west wall; a ceiling X = x, Z = PR4_DW_W - z (Up z+1). The player's tile, his fine position, the
// actors on the same plane and the dump use these axes while he is on a wall or a ceiling.
#define PR4_DW_W 512
#define PR4_DW_MAXP 64
#define PR4_DW_POOL 65536

static struct {
    int zone, n;
    int v[PR4_DW_MAXP][10];   // DistWorld_PortPlatform's: kind, bounds (x y z, size x y z), tile counts (vertical, horizontal)
    int off[PR4_DW_MAXP], len[PR4_DW_MAXP];
    u16 pool[PR4_DW_POOL];
} sPr4Dw = {-1};

// the avatar's gravity: 0 the main floor (the map's terrain), else 1 + the platform kind (1 floating floor, 2 west wall,
// 3 east wall, 4 ceiling)
static int Pr4_DwKind(FieldSystem *fs) {
    int s = PlayerAvatar_MapDistortionState(fs->playerAvatar);

    return s >= AVATAR_DISTORTION_STATE_FLOOR && s <= AVATAR_DISTORTION_STATE_CEILING ? s - AVATAR_DISTORTION_STATE_FLOOR + 1 : 0;
}

static void Pr4_DwLoad(FieldSystem *fs) {
    int i, used = 0, v[10];

    if (sPr4Dw.zone == (int)fs->location->mapHeaderID) {
        return;
    }
    sPr4Dw.zone = (int)fs->location->mapHeaderID;
    sPr4Dw.n = DistWorld_PortPlatform(fs, -1, v, NULL, 0);
    if (sPr4Dw.n > PR4_DW_MAXP) {
        sPr4Dw.n = PR4_DW_MAXP;
    }
    for (i = 0; i < sPr4Dw.n; i++) {
        int len;

        DistWorld_PortPlatform(fs, i, sPr4Dw.v[i], NULL, 0);
        len = sPr4Dw.v[i][7] * sPr4Dw.v[i][8];
        if (len < 0 || used + len > PR4_DW_POOL) {
            len = 0;
        }
        sPr4Dw.off[i] = used;
        sPr4Dw.len[i] = len;
        if (len > 0) {
            DistWorld_PortPlatform(fs, i, v, &sPr4Dw.pool[used], len);
        }
        used += len;
        if (getenv("BWPORT_DWDUMP") != NULL) {
            fprintf(stderr, "dwplat: zone %d p %d kind %d at %d %d %d size %d %d %d grid %d x %d\n", sPr4Dw.zone, i, sPr4Dw.v[i][0],
                    sPr4Dw.v[i][1], sPr4Dw.v[i][2], sPr4Dw.v[i][3], sPr4Dw.v[i][4], sPr4Dw.v[i][5], sPr4Dw.v[i][6], sPr4Dw.v[i][7],
                    sPr4Dw.v[i][8]);
        }
    }
}

// the world tile (x, y, z) of the planner's cell X,Z on a platform of kind k (x0 y0 z0: the player's tile, its fixed axis)
static void Pr4_DwWorld(int k, int X, int Z, int x0, int y0, int *x, int *y, int *z) {
    *x = x0;
    *y = y0;
    switch (k) {
    case 1: *x = X; *z = Z; break;
    case 2: *z = PR4_DW_W - X; *y = PR4_DW_W - Z; break;
    case 3: *z = X - PR4_DW_W; *y = PR4_DW_W - Z; break;
    default: *x = X; *z = PR4_DW_W - Z; break;
    }
}

// and back: the planner's cell of world tile (x, y, z)
static void Pr4_DwPlan(int k, int x, int y, int z, int *X, int *Z) {
    switch (k) {
    case 1: *X = x; *Z = z; break;
    case 2: *X = PR4_DW_W - z; *Z = PR4_DW_W - y; break;
    case 3: *X = PR4_DW_W + z; *Z = PR4_DW_W - y; break;
    default: *X = x; *Z = PR4_DW_W - z; break;
    }
}

// the attributes of world tile (x, y, z) on the platforms of kind k (the game's order: the current platform first, then
// the first of the same kind that holds the tile: player_move.c's step onto another platform). 0 none
static int Pr4_DwAttr(FieldSystem *fs, int k, int x, int y, int z, u16 *a) {
    int pass, i;

    for (pass = 0; pass < 2; pass++) {
        for (i = 0; i < sPr4Dw.n; i++) {
            const int *v = sPr4Dw.v[i];
            int vert, horz, idx;

            if (v[0] != k - 1 || (pass == 0) != (v[9] != 0) || x < v[1] || x > v[1] + v[4] || y < v[2] || y > v[2] + v[5] ||
                z < v[3] || z > v[3] + v[6]) {
                continue;
            }
            switch (k) {
            case 1: vert = x - v[1]; horz = z - v[3]; break;
            case 2: vert = v[5] - (y - v[2]); horz = z - v[3]; break;
            case 3: vert = y - v[2]; horz = z - v[3]; break;
            default: vert = v[4] - (x - v[1]); horz = z - v[3]; break;
            }
            idx = vert + horz * v[7];
            if (vert < 0 || horz < 0 || idx >= sPr4Dw.len[i]) {
                continue;
            }
            *a = sPr4Dw.pool[sPr4Dw.off[i] + idx];
            return 1;
        }
    }
    return 0;
}

// the player's tile and fine position in the planner's axes (k 0: the world's own)
static void Pr4_DwPlayer(FieldSystem *fs, int k, int *gx, int *gz, int *fx, int *fz) {
    MapObject *self = PlayerAvatar_GetMapObject(fs->playerAvatar);
    const VecFx32 *p = PlayerAvatar_GetPos(fs->playerAvatar);
    int x = MapObject_GetX(self), y = MapObject_GetY(self) / 2, z = MapObject_GetZ(self);
    int ux = p->x >> FX32_SHIFT, uy = p->y >> FX32_SHIFT, uz = p->z >> FX32_SHIFT;

    if (k < 2) {
        *gx = PlayerAvatar_GetXPos(fs->playerAvatar);
        *gz = PlayerAvatar_GetZPos(fs->playerAvatar);
        *fx = ux;
        *fz = uz;
        return;
    }
    Pr4_DwPlan(k, x, y, z, gx, gz);
    // a tile's centre is at 16 t + 8 in x and z, 16 t in y (MapObject: y in half tiles); the planner's centre is at 16 T + 8
    switch (k) {
    case 2: *fx = PR4_DW_W * 16 + 16 - uz; *fz = PR4_DW_W * 16 + 8 - uy; break;
    case 3: *fx = PR4_DW_W * 16 + uz; *fz = PR4_DW_W * 16 + 8 - uy; break;
    default: *fx = ux; *fz = PR4_DW_W * 16 + 16 - uz; break;
    }
}

// the cells of a floating platform's plane (Pr4_Fill's part for it): the kind, the steps (collision, the next tile on a
// platform of the kind), the actors standing on the plane
static void Pr4_DwFill(PortMap *m, FieldSystem *fs, int k) {
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    MapObject *self = PlayerAvatar_GetMapObject(fs->playerAvatar), *obj;
    int px = MapObject_GetX(self), py = MapObject_GetY(self) / 2, x, z, d, idx = 0;

    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            int wx, wy, wz;

            Pr4_DwWorld(k, m->x0 + x, m->z0 + z, px, py, &wx, &wy, &wz);
            sPr4Have[z][x] = (u8)Pr4_DwAttr(fs, k, wx, wy, wz, &sPr4A[z][x]);
        }
    }
    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            PortMapCell *c = &m->cell[z][x];
            u16 a = sPr4A[z][x];

            memset(c, 0, sizeof *c);
            c->zone = (u16)m->zone;
            c->cls = 0xff;
            if (!sPr4Have[z][x]) {
                c->kind = ' ';
                continue;
            }
            c->cls = (u8)(a & 0xff);
            c->kind = (a >> 15) & 1 ? '#' : TileBehavior_IsSurfable(c->cls) ? '~' : '.';   // B4F's ceiling has water (F:surf)
            if (c->kind == '#') {
                continue;
            }
            for (d = 0; d < 4; d++) {
                int nx = x + dx[d], nz = z + dz[d];

                if (nx >= 0 && nz >= 0 && nx < PORT_MAP_N && nz < PORT_MAP_N && sPr4Have[nz][nx] && !((sPr4A[nz][nx] >> 15) & 1)) {
                    c->pass |= (unsigned char)(1 << d);
                }
            }
        }
    }
    m->nwarp = m->ntrig = m->nbg = m->nnpc = 0;
    while (fs->mapObjMan != NULL && m->nnpc < 64 && MapObjectMan_FindObjectWithStatus(fs->mapObjMan, &obj, &idx, MAP_OBJ_STATUS_0)) {
        PortMapNpc *o;
        int ox = MapObject_GetX(obj), oy = MapObject_GetY(obj) / 2, oz = MapObject_GetZ(obj);

        if (obj == self || (k == 1 || k == 4 ? oy != py : ox != px)) {
            continue;
        }
        o = &m->npc[m->nnpc++];
        memset(o, 0, sizeof *o);
        o->id = (int)MapObject_GetLocalID(obj);
        Pr4_DwPlan(k, ox, oy, oz, &o->x, &o->z);
        o->x0 = o->x;
        o->z0 = o->z;
        o->code = MapObject_GetGraphicsID(obj);
        o->move = MapObject_GetMovementType(obj);
        o->face = -1;
        o->nocoll = MapObject_CheckStatus(obj, MAP_OBJ_STATUS_18) ? 1 : 0;
    }
    m->ok = 1;
}

static void Pr4_Fill(PortMap *m) {
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    FieldSystem *fs = sPr4Fs;
    PlayerAvatar *pa;
    MapObject *self, *obj;
    const VecFx32 *ppos;
    fx32 py;
    int x, z, d, i, idx, dyn, dwk;

    m->ok = 0;
    if (!Pr4_FieldUp(fs)) {
        return;
    }
    pa = fs->playerAvatar;
    self = PlayerAvatar_GetMapObject(pa);
    ppos = PlayerAvatar_GetPos(pa);
    py = ppos->y;
    m->px = m->wx = PlayerAvatar_GetXPos(pa);
    m->pz = m->wz = PlayerAvatar_GetZPos(pa);
    m->py = py >> 16;
    d = PlayerAvatar_GetFacingDir(pa);
    m->pdir = d >= 0 && d <= 3 ? d : -1;
    m->x0 = m->px - PORT_MAP_N / 2;
    m->z0 = m->pz - PORT_MAP_N / 2;
    m->zone = fs->location->mapHeaderID;
    m->mw = MapMatrix_GetWidth(fs->mapMatrix);
    m->mh = MapMatrix_GetHeight(fs->mapMatrix);
    m->surf = PlayerAvatar_GetPlayerState(pa) == PLAYER_AVATAR_SURFING;   // the water is walkable while the player surfs (route_map.c)
    m->cycle = PlayerAvatar_GetPlayerState(pa) == PLAYER_AVATAR_CYCLING ? (PlayerAvatar_GetCyclingGear(pa) == 1 ? 2 : 1) : 0;   // gear 1 is the fast one (player_move.c)
    m->rail = 0;
    m->nline = m->nlever = m->nplace = m->narm = 0;
    m->wind = m->gust = m->windHold = 0;
    dwk = Pr4_DwKind(fs);
    if (dwk != 0) {   // on a floating platform: its own grid (Pr4_DwFill), in the planner's axes for it
        Pr4_DwLoad(fs);
        Pr4_DwPlayer(fs, dwk, &m->px, &m->pz, &x, &z);
        m->wx = m->px;
        m->wz = m->pz;
        m->x0 = m->px - PORT_MAP_N / 2;
        m->z0 = m->pz - PORT_MAP_N / 2;
        Pr4_DwFill(m, fs, dwk);
        if (dwk == 4 && m->pdir >= 0 && m->pdir <= 1) {
            m->pdir ^= 1;   // a ceiling's avatar faces the world's way: Up (z+1 there) reads as down, so the key's own facing
        }
        return;
    }
    dyn = Pr4_DynOn(fs);
    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            int gx = m->x0 + x, gz = m->z0 + z;

            sPr4Have[z][x] = (u8)Pr4_Attr(gx, gz, &sPr4A[z][x]);
            if (dyn != 0 && sPr4Have[z][x]) {
                Pr4_DynTile(fs, dyn, gx, gz, py, &sPr4A[z][x]);
            }
            sPr4H[z][x] = sPr4Have[z][x] ? TerrainCollisionManager_GetHeight(fs, py, gx * PR4_TILE + PR4_TILE / 2,
                                                                              gz * PR4_TILE + PR4_TILE / 2, NULL)
                                         : 0;
        }
    }
    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            PortMapCell *c = &m->cell[z][x];
            int gx = m->x0 + x, gz = m->z0 + z, zone = Pr4_ZoneAt(gx, gz);
            u16 a = sPr4A[z][x];
            u8 b = (u8)(a & 0xff);
            int coll = (a >> 15) & 1;

            memset(c, 0, sizeof *c);
            c->zone = zone < 0 ? 0xffff : (u16)zone;
            c->cls = 0xff;
            if (!sPr4Have[z][x]) {
                c->kind = zone < 0 ? ' ' : '?';
                c->pass = c->kind == '?' ? 15 : 0;
                continue;
            }
            c->cls = b;
            c->y = (short)(sPr4H[z][x] >> 16);
            c->yf = (short)(sPr4H[z][x] >> FX32_SHIFT);
            c->kind = TileBehavior_IsJumpNorth(b) || TileBehavior_IsJumpNorthTwice(b)   ? '^'
                      : TileBehavior_IsJumpSouth(b) || TileBehavior_IsJumpSouthTwice(b) ? 'v'
                      : TileBehavior_IsJumpWest(b) || TileBehavior_IsJumpWestTwice(b)   ? '<'
                      : TileBehavior_IsJumpEast(b) || TileBehavior_IsJumpEastTwice(b)   ? '>'
                      : TileBehavior_IsWaterfall(b) ? 'F'   // a waterfall wall: F:waterfall climbs it from the water in front (the player surfs)
                      : coll || TileBehavior_IsBikeSlope(b) ? '#'   // a bike slope climbs only at the bicycle's top speed (player_move.c BikeSlope): a walker slides back, the pilot does not plan a run-up
                      : TileBehavior_IsBridgeOverWater(b) ? '.'   // a bridge walks (its flag says surfable: the water under it is)
                      : TileBehavior_IsSurfable(b)  ? '~'
                      : TileBehavior_IsTallGrass(b) || TileBehavior_IsVeryTallGrass(b) ? ','
                                                                                        : '.';
            if (TileBehavior_IsRockClimbNorthSouth(b)) {
                c->flags |= PORT_MAP_CLIMBNS;   // a Rock Climb wall: F:climb (route_fieldmove.h) faces it, A, Yes
            } else if (TileBehavior_IsRockClimbEastWest(b)) {
                c->flags |= PORT_MAP_CLIMBEW;
            }
            if (c->kind == 'F') {
                c->flags |= PORT_MAP_FALL;
            }
            if (TileBehavior_IsBikeSlope(b)) {
                c->flags |= PORT_MAP_BIKESLOPE;   // a wall to a walker; the bicycle's fast gear climbs it (route_slide.c PortRouteSlide_BikeRun)
            }
            if (TileBehavior_IsJumpNorthTwice(b) || TileBehavior_IsJumpSouthTwice(b) || TileBehavior_IsJumpWestTwice(b) || TileBehavior_IsJumpEastTwice(b)) {
                c->flags |= PORT_MAP_FARJUMP;   // the Distortion World's walkway notches (behaviours 0x5a-0x5d, in pairs facing each other):
                // a step onto one its way is the 3-tile jump (player_move.c PlayerAvatar_WillJumpTwice, 2 px x 24 frames)
            }
            if (TileBehavior_IsIce(b)) {
                fx32 h0 = sPr4H[z][x];

                c->slide = PORT_MAP_ICE;   // sliding ice (player_move.c PlayerAvatar_TileMove_Ice, Snowpoint's gym floor): the planner's slide edge (route_slide.c PortRouteSlide_End)
                for (d = 0; d < 4; d++) {   // the game's check, a quarter tile on (PlayerAvatar_CheckIceHeightChange): the speed the slide has left
                    fx32 h1 = TerrainCollisionManager_GetHeight(fs, py, (gx * 16 + 8 + dx[d] * 4) * FX32_ONE, (gz * 16 + 8 + dz[d] * 4) * FX32_ONE, NULL);

                    c->ihc |= (unsigned char)((h1 > h0 ? 1 : h1 < h0 ? 2 : 0) << (2 * d));
                }
            }
            if (TileBehavior_IsMud(b) || TileBehavior_IsMudWithGrass(b) || TileBehavior_IsSnow(b) || TileBehavior_IsVeryTallGrass(b)) {
                c->flags |= PORT_MAP_NOBIKE;   // PlayerAvatar_CheckBikeCollision refuses a rider these (a bridge over snow aside)
            }
            if (c->kind != '.' && c->kind != ',' && c->kind != '~') {
                continue;
            }
            for (d = 0; d < 4; d++) {
                int nx = x + dx[d], nz = z + dz[d];
                u16 na;
                u8 nb;
                fx32 dh;

                if (nx < 0 || nz < 0 || nx >= PORT_MAP_N || nz >= PORT_MAP_N) {
                    continue;
                }
                if (!sPr4Have[nz][nx]) {
                    c->pass |= Pr4_ZoneAt(m->x0 + nx, m->z0 + nz) >= 0 ? (1 << d) : 0;   // not loaded: may be open
                    continue;
                }
                na = sPr4A[nz][nx];
                nb = (u8)(na & 0xff);
                dh = sPr4H[nz][nx] - sPr4H[z][x];
                if ((na >> 15) & 1 || nb == GetNullTileBehaviorID() || kPr4ExitBlock[d](b) || kPr4EnterBlock[d](nb) ||
                    dh >= 20 * FX32_ONE || dh <= -20 * FX32_ONE) {
                    continue;
                }
                c->pass |= (unsigned char)(1 << d);
            }
        }
    }
    if (dyn == DYNAMIC_MAP_FEATURES_CANALAVE_GYM) {   // a platform standing at the avatar's height rides him when a step ends on it
        for (i = 0; i < 24; i++) {
            int px, pyt, pz;

            if (CanalaveGym_PortPlatformAt(fs, i, &px, &pyt, &pz) && pyt == (int)(py >> 16)) {
                x = px - m->x0;
                z = pz - m->z0;
                if (x >= 0 && z >= 0 && x < PORT_MAP_N && z < PORT_MAP_N) {
                    m->cell[z][x].flags |= PORT_MAP_STEPON;
                }
            }
        }
    }
    PortMap_OpenUnloaded(m);
    // the zone's events (MapHeaderData: tile units already)
    m->nwarp = m->ntrig = m->nbg = m->nnpc = 0;
    {
        const MapHeaderData *hd = fs->mapHeaderData;

        for (i = 0; hd->warpEvents != NULL && i < (int)hd->numWarpEvents && m->nwarp < 64; i++) {
            const WarpEvent *w = &hd->warpEvents[i];
            PortMapWarp *o = &m->warp[m->nwarp++];
            u16 wa = 0;

            memset(o, 0, sizeof *o);
            o->x = w->x;
            o->z = w->z;
            o->w = o->h = 1;
            o->zone = w->destHeaderID;
            o->dest = w->destWarpID;
            o->dir = Pr4_WarpDir(Pr4_Attr(w->x, w->z, &wa) ? (u8)(wa & 0xff) : 0xff);
            // a mat or stairs that needs the key held on it: type 7 (route_map.c: on it, or the step from the one side that
            // keeps the press); a door or a tile with no behavior: 0, the Gen 5 "any side" class
            o->type = o->dir != 0 ? 7 : 0;
            // a panel: a warp into this very map (Galactic HQ's warp panels) fires when stepped on, so a path never crosses
            // one by chance (the planner treats the cell like a moving platform: only as a goal); G:door:I or G:X,Z stands on it
            if ((int)w->destHeaderID == (int)fs->location->mapHeaderID) {
                int cx = w->x - m->x0, cz = w->z - m->z0;

                if (cx >= 0 && cz >= 0 && cx < PORT_MAP_N && cz < PORT_MAP_N) {
                    m->cell[cz][cx].flags |= PORT_MAP_STEPON;
                }
            }
        }
        for (i = 0; hd->coordEvents != NULL && i < (int)hd->numCoordEvents && m->ntrig < 64; i++) {
            const CoordEvent *g = &hd->coordEvents[i];
            PortMapTrig *o = &m->trig[m->ntrig++];

            memset(o, 0, sizeof *o);
            o->x = g->x;
            o->z = g->z;
            o->w = g->width;
            o->h = g->length;
            o->script = g->script;
            o->workId = g->var;
            o->workValue = g->value;
        }
        for (i = 0; hd->bgEvents != NULL && i < (int)hd->numBgEvents && m->nbg < 64; i++) {
            const BgEvent *g = &hd->bgEvents[i];
            PortMapBg *o = &m->bg[m->nbg++];

            memset(o, 0, sizeof *o);
            o->x = g->x;
            o->z = g->z;
            o->script = g->script;
            o->dir = 4;   // any side (BgEvent.playerFacingDir's coding not mapped yet)
            o->cond = g->type;
        }
    }
    idx = 0;
    while (fs->mapObjMan != NULL && m->nnpc < 64 && MapObjectMan_FindObjectWithStatus(fs->mapObjMan, &obj, &idx, MAP_OBJ_STATUS_0)) {
        PortMapNpc *o;

        if (obj == self) {
            continue;
        }
        o = &m->npc[m->nnpc++];
        memset(o, 0, sizeof *o);
        o->id = (int)MapObject_GetLocalID(obj);
        o->x = MapObject_GetX(obj);
        o->z = MapObject_GetZ(obj);
        o->x0 = MapObject_GetXPrev(obj);   // the tile it steps from (the game's collision holds it with the current one), not the spawn tile an actor moved by a script left
        o->z0 = MapObject_GetZPrev(obj);
        o->code = MapObject_GetGraphicsID(obj);
        o->move = MapObject_GetMovementType(obj);
        o->face = MapObject_GetFacingDir(obj);
        if (o->face < 0 || o->face > 3) {
            o->face = -1;
        }
        if (Pr4_Sees(obj)) {
            o->sight = MapObject_GetDataAt(obj, 0);
            o->look = MapObject_GetTrainerType(obj) == TRAINER_TYPE_VIEW_ALL_DIRECTIONS ? 15 : o->face >= 0 ? (unsigned char)(1 << o->face) : 0;
        }
        o->nocoll = MapObject_CheckStatus(obj, MAP_OBJ_STATUS_18) ? 1 : 0;   // as sub_02063F00's collision skips it
        o->breaks = o->code == 0x76;   // the snowball (Snowpoint's gym): ov5_021E06A8 deletes it ahead of a sliding avatar with speed
    }
    m->ok = 1;
}

// the object is a trainer who has not been beaten and so sees the player: along its facing (the types the game treats as normal: they
// turn about, see trainer_encounter.c GetTrainerType) or all four ways, as far as the object's first data word says
static int Pr4_Sees(MapObject *obj) {
    u32 tt = MapObject_GetTrainerType(obj);

    return (tt == TRAINER_TYPE_NORMAL || (tt >= TRAINER_TYPE_FACE_SIDES && tt <= TRAINER_TYPE_SPIN_CLOCKWISE) || tt == TRAINER_TYPE_VIEW_ALL_DIRECTIONS) &&
           !Script_IsTrainerDefeated(sPr4Fs, (u16)Script_GetTrainerID((u16)MapObject_GetScript(obj)));
}

// the pilot's watched-line query (route_test.c WatchWait): -1 when the tile is on none of the four lines (up to the sight range) of a
// trainer who has not been beaten; else bit 0 set when one of them looks along his line to it now (or sees all ways), the other bits
// a signature of the facings of the trainers whose lines hold it (it changes when one of them turns). The game's IsPathInterrupted is
// not applied: a wall in between only makes the wait longer than it needs to be
static int Pr4_Watched(int gx, int gz) {
    MapObject *obj = NULL;
    int idx = 0, any = 0, now = 0, sig = 0;

    if (!Pr4_FieldUp(sPr4Fs) || sPr4Fs->mapObjMan == NULL) {
        return -1;
    }
    while (MapObjectMan_FindObjectWithStatus(sPr4Fs->mapObjMan, &obj, &idx, MAP_OBJ_STATUS_0)) {
        int ax = MapObject_GetX(obj), az = MapObject_GetZ(obj), r, d, f = MapObject_GetFacingDir(obj);
        int all = MapObject_GetTrainerType(obj) == TRAINER_TYPE_VIEW_ALL_DIRECTIONS;

        if (!Pr4_Sees(obj) || MapObject_GetMovementType(obj) < 2 || MapObject_GetMovementType(obj) > 13) {
            continue;   // (only the movement types that turn: 2 look around, 3 .. 5 wander, 6 .. 13 look two or three ways; a fixed one is no wait)
        }
        r = MapObject_GetDataAt(obj, 0);
        for (d = 0; d < 4; d++) {
            int in = d == 0   ? gx == ax && gz < az && gz >= az - r
                     : d == 1 ? gx == ax && gz > az && gz <= az + r
                     : d == 2 ? gz == az && gx < ax && gx >= ax - r
                              : gz == az && gx > ax && gx <= ax + r;

            if (in) {
                any = 1;
                now |= all || f == d;
                sig = sig * 5 + f + 1;
            }
        }
    }
    return any ? (sig << 1) | now : -1;
}

// Bk: the bicycle on the Y button (the bag's Register on it), only when the bag holds it (the story gives it: no item is added)
static int Pr4_Bike(void) {
    Bag *bag;

    if (!Pr4_FieldUp(sPr4Fs)) {
        return 0;
    }
    bag = SaveData_GetBag(sPr4Fs->saveData);
    if (!Bag_CanRemoveItem(bag, ITEM_BICYCLE, 1, HEAP_ID_FIELD2)) {
        return 0;
    }
    Bag_RegisterItem(bag, ITEM_BICYCLE);
    return 1;
}

// the Cycling Road's bridge under the bicycle (player_move.c PlayerAvatar_IsOnCyclingRoadBridge): with no key held the avatar
// is forced south, a tap of another key is turned back at once. The pilot's walk holds its key there
static int Pr4_Pushed(void) {
    PlayerAvatar *pa;
    MapObject *obj;

    if (!Pr4_FieldUp(sPr4Fs)) {
        return 0;
    }
    pa = sPr4Fs->playerAvatar;
    obj = PlayerAvatar_GetMapObject(pa);
    return PlayerAvatar_GetPlayerState(pa) == PLAYER_AVATAR_CYCLING && PlayerAvatar_IsOnCyclingRoad(pa) &&
           MapObject_IsOnElevatedBridge(obj, MapObject_GetCurrTileBehavior(obj)) == TRUE;
}

static const PortRouteAdapter kPortRouteGen4 = {"gen4", Pr4_Fill, Pr4_ZoneAt, .bike = Pr4_Bike, .pushed = Pr4_Pushed, .watched = Pr4_Watched};

// The pilot's S, Py and Pn on Platinum (pokeplatinum-route.patch's other hunks). The field menu (start_menu.c StartMenu_Main,
// each frame of its task): while it takes keys (START_MENU_STATE_SELECT) gPortRouteMenu says where the cursor and SAVE are, as
// Black / White's grid menu with one column (slot = 2 * row: the pilot's MenuDir then only goes up or down). The save: the
// menu's START_MENU_STATE_SAVE starts the save script (common script 5 or 34: its yes/no questions, the write, the message),
// during which the menu's task waits; ScrCmd_TrySaveGame's write (FieldSystem_Save, done in that call) makes it 11 (Black's
// "written" side of route_test.c's SaveStep: A at the messages after it), the menu's next state ends it (-1). Gen 5's values:
// -1 no save, 0 its questions (A), 8 or more the write happened.
static int sPr4Save = -1;

void PortRoute4_StartMenu(const StartMenu *menu, int saveOption, int pokemonOption, int bagOption) {
    PortRouteMenu *m = &gPortRouteMenu;
    int i, n;

    if (menu->state == START_MENU_STATE_SAVE) {
        sPr4Save = 0;
    } else if (sPr4Save >= 0) {
        sPr4Save = -1;   // the save script is over (START_MENU_STATE_SAVE_WAIT)
    }
    if (menu->state != START_MENU_STATE_SELECT || menu->menu == NULL) {
        return;
    }
    n = (int)menu->spriteCount - 1;   // StartMenu_InitSprites: the options and the cursor
    n = n < 0 ? 0 : n > MAX_START_MENU_OPTIONS ? MAX_START_MENU_OPTIONS : n;
    m->input = 1;
    m->grid = 1;
    m->slot = 2 * Menu_GetCursorPos(menu->menu);
    m->saveSlot = m->pokeSlot = m->bagSlot = -1;
    for (i = 0; i < n; i++) {
        if (menu->options[i] == saveOption) {
            m->saveSlot = 2 * i;
        } else if (menu->options[i] == pokemonOption) {
            m->pokeSlot = 2 * i;
        } else if (menu->options[i] == bagOption) {
            m->bagSlot = 2 * i;
        }
    }
    m->onSave = m->saveSlot >= 0 && m->slot == m->saveSlot;
    m->vblank = OS_GetVBlankCount();
}

// F:fly on Platinum (port/src/route_fly.h; pokeplatinum-wroutefly.patch's hunks). The pilot goes X, Pokemon, the party menu, the
// Fly user, its context menu's Fly entry, the Town Map, as the player does; each of those screens tells it its state, in the structs
// Gen 5's menus fill (port_route.h: gPortRoutePList, gPortRouteTownMap). None of them updates the field system, so the pilot's V-blank
// fallback (route_test.c PortRoute_VBlank) is what plays them.
//   - the Fly user: the first party member with Fly in a move slot; under the test cheat (as the other field moves) slot 0
//     "knows" it when nobody does: the party menu's context menu then adds the Fly entry for it (PortRoute4_CheatFlyEntry).
//   - the party menu (applications/party_menu/main.c PartyMenu_Main, each frame): keys move the cursor in the DEFAULT state; its
//     context menu (HANDLE_PARTY_MENU_ACTION: the first level, HANDLE_CONTEXT_MENU_INPUT: an item's) is the pilot's submenu, Fly's entry numbered 16 + its move slot as Gen 5's list does.
//   - the Town Map (applications/town_map/main.c TownMap_Main, each frame): the cursor is a cell of a 30 x 30 grid (the D-pad moves
//     it one cell and queues the move: no key is read until it is done), a place's cell is any one on which the game's own
//     TownMap_GetFlyLocationAtPos answers with that place (found once, by asking), the A pick works when the cursor stands on an
//     unlocked place. The pick ends the map with flyLocationSelected (TownMap_Exit).
static void Pr4_FlySlot(FieldSystem *fs) {
    Party *party = SaveData_GetParty(fs->saveData);
    int i, m, n = Party_GetCurrentCount(party);

    gPortRouteFlySlot = -1;
    for (i = 0; i < n && gPortRouteFlySlot < 0; i++) {
        Pokemon *mon = Party_GetPokemonBySlotIndex(party, i);

        for (m = 0; m < 4; m++) {
            if (Pokemon_GetValue(mon, MON_DATA_MOVE1 + m, NULL) == 19) {   // MOVE_FLY
                gPortRouteFlySlot = i;
                gPortRouteFlyMove = m;
                break;
            }
        }
    }
    if (gPortRouteFlySlot < 0 && n > 0 && PortRoute_CheatWin(PORT_ROUTE_GAME)) {
        gPortRouteFlySlot = 0;
        gPortRouteFlyMove = 0;
    }
}

static int sPr4PartyEntries[8], sPr4PartyEntryCount;

// party_menu/main.c: the context menu being drawn (entries: its action ids; fly: Fly's id)
void PortRoute4_PartyContext(const u8 *entries, int count, int fly) {
    int i;

    sPr4PartyEntryCount = count > 8 ? 8 : count;
    for (i = 0; i < sPr4PartyEntryCount; i++) {
        sPr4PartyEntries[i] = entries[i] == fly ? 16 + (gPortRouteFlyMove < 0 ? 0 : gPortRouteFlyMove) : entries[i];
    }
}

// party_menu/main.c GetContextMenuEntriesForPartyMon: the cheat's Fly entry for the slot the pilot chose (no member knows Fly)
int PortRoute4_CheatFlyEntry(int slot) {
    return PortRoute_CheatWin(PORT_ROUTE_GAME) && gPortRouteFlySlot == slot;
}

// party_menu/main.c PartyMenu_Main, each frame: takesKeys = the DEFAULT state (the cursor moves, A opens the context menu),
// menuCursor = the context menu's cursor while it takes input (-1 else), slot = the party cursor (6 and 7: the buttons)
void PortRoute4_PartyMenu(int takesKeys, int menuCursor, int slot, int mode) {
    PortRoutePList *l = &gPortRoutePList;
    unsigned now = OS_GetVBlankCount();
    int i;

    l->vblank = now;
    l->state = takesKeys ? 2 : 9;
    l->subState = 1;
    l->cursorPos = slot;
    l->mode = mode;
    if (menuCursor >= 0) {
        l->menuVblank = now;
        l->menuCount = sPr4PartyEntryCount;
        l->menuCursor = menuCursor;
        for (i = 0; i < sPr4PartyEntryCount; i++) {
            l->menuItems[i] = sPr4PartyEntries[i];
        }
    }
}

static const TownMapAppData *sPr4TmFor;
static int sPr4TmCells[NUM_FLY_LOCATIONS][2];

// town_map/main.c TownMap_Main, each frame: takesKeys = the main loop (not the fades or the load)
void PortRoute4_TownMap(const TownMapAppData *appData, int takesKeys) {
    PortRouteTownMap *t = &gPortRouteTownMap;
    const TownMapGraphicsManager *gm = appData->graphicsMan;
    const TownMapAppFlyLocationsManager *fl;
    int i, x, z;

    if (gm == NULL || gm->flyLocations == NULL) {
        return;
    }
    fl = gm->flyLocations;
    if (sPr4TmFor != appData) {   // the cells of the places: asked of the game itself, the first one found for each
        sPr4TmFor = appData;
        for (i = 0; i < NUM_FLY_LOCATIONS; i++) {
            sPr4TmCells[i][0] = sPr4TmCells[i][1] = -1;
        }
        for (z = 6; z <= 28; z++) {
            for (x = 2; x <= 28; x++) {
                const TownMapAppFlyLocation *f;
                int h = MainMapMatrixData_GetMapHeaderIDAtCoords(appData->mainMapMatrixData, (u32)x, (u32)z);

                if (TownMap_GetMapBlockAtPosition(appData->mapBlockList, x, z, appData->unlockedHiddenLocations) == NULL) {
                    continue;
                }
                f = TownMap_GetFlyLocationAtPos(gm->flyLocations, (enum MapHeaderID)h, x, z);
                if (f != NULL) {
                    i = (int)(f - fl->flyLocationsList);
                    if (i >= 0 && i < NUM_FLY_LOCATIONS && sPr4TmCells[i][0] < 0) {
                        sPr4TmCells[i][0] = x;
                        sPr4TmCells[i][1] = z;
                    }
                }
            }
        }
    }
    t->vblank = OS_GetVBlankCount();
    t->alive = 1;
    t->grid = 1;
    t->mode = appData->mode;
    t->result = 0;
    t->zoomed = gm->showingZoomedInMap;
    t->moving = !takesKeys || gm->queuedMovement != 0 || gm->zoomedInMapTransitionStage > 1;
    t->cx = gm->cursorX;
    t->cy = gm->cursorZ;
    t->sx = t->sy = 0;
    t->hit = t->ring = -1;
    if (gm->hoveredBlock != NULL) {
        const TownMapAppFlyLocation *f = TownMap_GetFlyLocationAtPos(gm->flyLocations, (enum MapHeaderID)gm->hoveredLocationMapHeader, gm->cursorX, gm->cursorZ);

        if (f != NULL) {
            t->hit = t->ring = (int)(f - fl->flyLocationsList);
        }
    }
    t->selected = -1;
    t->count = fl->count > 64 ? 64 : fl->count;
    for (i = 0; i < t->count; i++) {
        t->p[i].zone = fl->flyLocationsList[i].descriptor.mapHeader;
        t->p[i].x = i < NUM_FLY_LOCATIONS ? sPr4TmCells[i][0] : -1;
        t->p[i].y = i < NUM_FLY_LOCATIONS ? sPr4TmCells[i][1] : -1;
        t->p[i].canFly = fl->flyLocationsList[i].isUnlocked != FALSE && t->p[i].x >= 0;
        t->p[i].hidden = 0;
    }
}

// town_map/main.c TownMap_Exit: the map is over (a pick: flySelected, the header it picked)
void PortRoute4_TownMapEnd(int flySelected, int mapHeader) {
    gPortRouteTownMap.vblank = OS_GetVBlankCount();
    gPortRouteTownMap.alive = 0;
    gPortRouteTownMap.result = flySelected ? 2 : 0;   // 2 = TOWNMAP_RESULT_FLY, as Gen 5's
    gPortRouteTownMap.flyZone = mapHeader;
    sPr4TmFor = NULL;
}

// scrcmd.c ScriptContext_WaitForYesNoResult (a script's yes/no, the save's questions too): the cursor's row each frame it
// waits, -1 once it has the answer
void PortRoute4_YesNo(int row) {
    gPortRouteFieldYesNo = row < 0 ? 0 : 1 + (row & 1);
}

// scrcmd.c ScrCmd_TrySaveGame, after the write
void PortRoute4_Saved(void) {
    if (sPr4Save >= 0) {
        sPr4Save = 11;
    }
}

// The battle pilot (pokeplatinum-route.patch's battle/battle_display.c hunks; Gen 5's are the btlv_core.c / btl_client.c
// stand-ins). The player's menus are on the touch screen, but each also takes keys (battle_subscreen.c BattleSystem_MenuKeys:
// the first key press shows the cursor, the next A picks; B on the yes/no is No), so route_test.c's event 2 (A every 4th
// frame, B while gPortRouteYesNo is 2) drives them as on Gen 5. route_test.c reports the battle (event 2) while the battle
// overlay (16, PortGame.ovlBattle) is loaded: in PortRoute_Frame and in PortRoute_VBlank's fallback (a Platinum battle's only
// pilot frames: the field system does not update while it runs). kind: 0 the action menu (Fight, Bag, Pokemon, Run), 1 the move menu, 2 the yes/no
// (arg: its yesNoType), 3 the forget-a-move screen (arg: the new move), -1 the yes/no or the screen has its answer (arg).
extern int gPortRouteYesNo;   // port/src/route_test.c: 1 answer with A, 2 with B

static void Pr4_IntBattle(int kind);   // the interaction sweep (below)

void PortRoute4_BattleMenu(int kind, int arg) {
    static int sLast = -2;
    int on = getenv("BWPORT_PILOT") != NULL;

    Pr4_IntBattle(kind);
    if (kind == 0) {
        gPortRouteBattle.menuVblank = OS_GetVBlankCount();
    } else if (kind == 2) {
        // yesNoType (battle_script.c, battle_controller_player.c): 0 forfeit (Frontier), 1 forget a move for the new one,
        // 2 give up learning it, 3 use the next Pokemon (wild), 4 switch before the trainer's next one, 5 nickname the catch.
        // No (B) to 0, 1, 4, 5: keep the moves, keep the Pokemon in (Gen 5's answers); Yes (A) to 2 (else 1 comes back) and 3.
        gPortRouteYesNo = arg == 2 || arg == 3 ? 1 : 2;
        // The pilot's A pulse (every 4th frame, set a frame ahead) can land on the question's very first frame, before
        // gPortRouteYesNo says "answer B": the menu then reads A as Yes (a Yes to "switch Pokemon?" opens the party screen and the
        // leg waits there for ever: Platinum ver16, Candice, with Instant text, whose question has no printing delay to keep
        // the pulse's phase). Drop that one A: the question is answered No by the B pulses that follow.
        if (gPortRouteYesNo == 2 && sLast != 2 && (gSystem.pressedKeys & PAD_BUTTON_A)) {
            gSystem.pressedKeys &= ~PAD_BUTTON_A;
            fprintf(stderr, "pilot: battle yes/no question: the A pulse of its first frame dropped (V-blank %u)\n",
                    (unsigned)OS_GetVBlankCount());
        }
    } else if (kind == 3) {
        gPortRouteYesNo = 2;   // reached only after a Yes to type 1: B backs out (no move forgotten)
    } else if (kind < 0) {
        gPortRouteYesNo = 0;
    }
    if (on && kind != sLast) {
        fprintf(stderr, "pilot: battle menu %d arg %d at V-blank %u\n", kind, arg, (unsigned)OS_GetVBlankCount());
    }
    sLast = kind;
}

// An ability that turns a move type into no damage (the type chart does not know them): a Levitate target against Ground,
// Volt Absorb / Lightning Rod / Motor Drive against Electric, Water Absorb / Dry Skin against Water, Flash Fire against Fire.
// Without this the pilot keeps a Ground move against a floating target for ever (route 214's Chingling).
static int PortRoute4_AbilityNoDamage(int ability, int moveType) {
    switch (ability) {
    case ABILITY_LEVITATE:
        return moveType == TYPE_GROUND;
    case ABILITY_VOLT_ABSORB:
    case ABILITY_LIGHTNING_ROD:
    case ABILITY_MOTOR_DRIVE:
        return moveType == TYPE_ELECTRIC;
    case ABILITY_WATER_ABSORB:
    case ABILITY_DRY_SKIN:
        return moveType == TYPE_WATER;
    case ABILITY_FLASH_FIRE:
        return moveType == TYPE_FIRE;
    }
    return 0;
}

// the move menu's answer (input: 1..4 the move slot + 1, else a cancel): Gen 5's rule (PortRoute_BestMove): the A's move stays
// while it can hurt the first opponent that has HP, else the usable one with the best type matchup x power. A move of power 0
// is a status move (eff 0); fixed-damage moves have power 1. No usable move hurts it: the A's move (Gen 5 switches; not here).
// BWPORT_PT_TESTSLOT (the battle sweep, Pr4_TestSlot below) answers instead, without the pilot.
static int Pr4_TestSlot(int input, const u16 *moves);

int PortRoute4_BattleMove(BattleSystem *battleSys, int battler, int input, const u16 *moves, const u8 *ppCur, int invalidMoves) {
    BattleContext *ctx = BattleSystem_GetBattleContext(battleSys);
    int eff[4], power[4], usable[4], n = 0, i, target = -1, slot;

    if (input >= 1 && input <= 4) {
        Pr4_IntBattle(100);
    }
    if (input >= 1 && input <= 4 && getenv("BWPORT_PT_TESTSLOT") != NULL) {
        return Pr4_TestSlot(input, moves);
    }
    if (getenv("BWPORT_PILOT") == NULL || input < 1 || input > 4) {
        return input;
    }
    for (i = 0; i < BattleSystem_GetMaxBattlers(battleSys); i++) {
        if (BattleSystem_GetBattlerSide(battleSys, i) != BattleSystem_GetBattlerSide(battleSys, battler) &&
            BattleMon_Get(ctx, i, BATTLEMON_CUR_HP, NULL) > 0) {
            target = i;
            break;
        }
    }
    if (target < 0) {
        return input;
    }
    for (i = 0; i < 4 && moves[i] != 0; i++) {
        int pw = (int)MoveTable_LoadParam(moves[i], MOVEATTRIBUTE_POWER);
        int mtype = (int)MoveTable_LoadParam(moves[i], MOVEATTRIBUTE_TYPE);
        int mul = BattleSystem_TypeMatchupMultiplier((u8)mtype, (u8)BattleMon_Get(ctx, target, BATTLEMON_TYPE_1, NULL),
                                                     (u8)BattleMon_Get(ctx, target, BATTLEMON_TYPE_2, NULL));

        if (PortRoute4_AbilityNoDamage((int)BattleMon_Get(ctx, target, BATTLEMON_ABILITY, NULL), mtype)) {
            mul = 0;
        }
        // the multiplier is x40 (40 = x1): 0, 10, 20, 40, 80, 160 -> Gen 5's codes 0 .. 5
        eff[i] = pw == 0 || mul == 0 ? 0 : mul <= 10 ? 1 : mul <= 20 ? 2 : mul <= 40 ? 3 : mul <= 80 ? 4 : 5;
        power[i] = pw;
        usable[i] = ppCur[i] != 0 && (invalidMoves & (1 << i)) == 0;
        n++;
    }
    slot = PortRoute_BestMove(n, eff, power, usable, input - 1);
    if (slot >= 0 && slot != input - 1) {
        fprintf(stderr, "pilot: battle move: slot %d (move %u) for slot %d (move %u) at V-blank %u\n", slot, (unsigned)moves[slot],
                input - 1, (unsigned)moves[input - 1], (unsigned)OS_GetVBlankCount());
        return slot + 1;
    }
    if (slot < 0) {
        fprintf(stderr, "pilot: battle move: no usable move hurts battler %d: slot %d stays\n", target, input - 1);
    }
    return input;
}

// The double battle's target menu (battle_subscreen.c BattleSystem_Cursor_Battler, the first key press shows its cursor): the
// game puts the cursor on the button it saved at the last pick, whether or not that Pokemon is still there. After the pilot's
// first turn the picked opponent has fainted: its button is blank, A does nothing on it and the menu waits for ever. With the
// pilot on, a saved button without a Pokemon is not restored: the game's own start (the first present button) is used, as it is
// on a first pick. button: 0..3 a target slot, 4 the cancel button (kept). present: the four slots' flags.
int PortRoute4_TargetCursorKeep(int button, const u8 *present) {
    if (getenv("BWPORT_PILOT") == NULL || button < 0 || button > 3 || present[button] == 1) {
        return 1;
    }
    fprintf(stderr, "pilot: target menu: saved button %d has no Pokemon, cursor restarts on a present one at V-blank %u\n", button,
            (unsigned)OS_GetVBlankCount());
    return 0;
}

// The test cheats (docs/platinum.md "Test cheats", Gen 5's as docs/port-runtime.md "Test cheat" says; pokeplatinum-route.patch
// calls them). BWPORT_TESTCHEAT=win (PortRoute_CheatWin): the battles play in full, but the player's side (side 0) moves
// first (the turn order's speeds: player 32x, opponents 1/10; a move's priority and Quick Claw still count, as on Gen 5),
// its moves always hit and use no PP (CheatSide), its hits take all the target's HP and the hits on it do none (CheatDamage,
// at BattleControllerPlayer_UpdateHP: every move's damage on its target, fixed-damage and OHKO moves too), and the HP the battle
// scripts take from it outside a hit (CheatHpCalc, at battle_script.c BtlCmd_UpdateHealthBar / BtlCmd_UpdateHealthBarValue: burn,
// poison, hail, sandstorm, Leech Seed, Curse, Nightmare, recoil, Spikes, a move's own HP cost) is none either (int5, 2026-11-19:
// a burn or the hail fainted the lead in chains whose frames the speed options moved). Not covered: status moves, stat drops.
// BWPORT_NOENCOUNTER=1 (PortRoute_NoEncounter, the pilot's E:on / E:off): no wild encounter on a step (walking, surfing;
// field_control.c Field_CheckWildEncounter, as a Repel would); fishing, scripts, trainers and the Pal Park still happen.
int PortRoute4_CheatSide(BattleSystem *battleSys, int battler) {
    return (PortRoute_CheatWin(PORT_ROUTE_GAME) || getenv("BWPORT_PT_TESTHIT") != NULL) && BattleSystem_GetBattlerSide(battleSys, battler) == 0;
}

void PortRoute4_CheatSpeed(BattleSystem *battleSys, int battler1, int battler2, u32 *speed1, u32 *speed2) {
    if (!PortRoute_CheatWin(PORT_ROUTE_GAME)) {
        return;
    }
    u32 *sp[2] = {speed1, speed2};
    int b[2] = {battler1, battler2}, i;
    for (i = 0; i < 2; i++) {
        *sp[i] = BattleSystem_GetBattlerSide(battleSys, b[i]) == 0 ? *sp[i] * 32 : *sp[i] / 10;
    }
}

int PortRoute4_CheatDamage(BattleSystem *battleSys, int attacker, int defender, int damage, int defenderHP) {
    int n = damage;

    if (damage == 0 || !PortRoute_CheatWin(PORT_ROUTE_GAME)) {
        return damage;
    }
    if (BattleSystem_GetBattlerSide(battleSys, defender) == 0) {
        n = 0;
    } else if (BattleSystem_GetBattlerSide(battleSys, attacker) == 0) {
        n = -(defenderHP > 0 ? defenderHP : 1);
    }
    fprintf(stderr, "cheat: damage atk %d def %d %d -> %d (V-blank %u)\n", attacker, defender, damage, n, (unsigned)OS_GetVBlankCount());
    return n;
}

// The HP change a battle script applies to `battler` (hpCalcTemp: < 0 a loss): with the cheat on, a loss of the player's side is 0.
int PortRoute4_CheatHpCalc(BattleSystem *battleSys, int battler, int hpCalc) {
    if (hpCalc >= 0 || !PortRoute_CheatWin(PORT_ROUTE_GAME) || BattleSystem_GetBattlerSide(battleSys, battler) != 0) {
        return hpCalc;
    }
    fprintf(stderr, "cheat: script HP battler %d %d -> 0 (V-blank %u)\n", battler, hpCalc, (unsigned)OS_GetVBlankCount());
    return 0;
}

int PortRoute4_NoEncounter(void) {
    return PortRoute_NoEncounter(PORT_ROUTE_GAME);
}

// The field moves the scripts and the field's checks ask the party for (scrcmd_party.c ScrCmd_FindPartySlotWithMove: Cut, Rock Smash,
// Strength, Rock Climb, Surf, Defog, Waterfall; unk_02054884.c Party_HasMonWithMove: the field's Surf and Waterfall): with the test
// cheat on (PortRoute_CheatWin) slot 0 "knows" them, as Gen 5's PortRoute_CheatFieldMove (the same list and the same F: tokens)
// plus Platinum's Whirlpool, Rock Climb and Defog. The badge checks stay real: the story gate is not cheated. 1 = slot 0 knows it.
int PortRoute4_CheatFieldMove(int move) {
    switch (move) {
    case 250:   // MOVE_WHIRLPOOL
    case 431:   // MOVE_ROCK_CLIMB
    case 432:   // MOVE_DEFOG
        return PortRoute_CheatWin(PORT_ROUTE_GAME);
    default:
        return PortRoute_CheatFieldMove(PORT_ROUTE_GAME, (unsigned)move);
    }
}

// The battle sweep's test battle (tools/pt_battle_sweep.py, docs/platinum.md "Battle sweep"; crash finding only, a throwaway run:
// never save it to the save library; HeartGold / SoulSilver's twin BWPORT_HG_TESTBATTLE is on the hgss branch):
// BWPORT_PT_TESTBATTLE=SPECIES:LEVEL[:M1,M2,M3,M4[:OPPSPECIES[/OPPLEVEL][:O1,O2,O3,O4]]]. At the first free field frame slot 0 of
// the party is replaced by one Pokemon of that species and level (the script's give command's init: random IVs, the player's
// trainer data, the Poke Ball; an empty party gets it added), then a wild battle starts as the field's own grass encounter does
// (Encounter_NewVsWild on a FieldBattleDTO made by CreateWildMon_Scripted) against OPPSPECIES at OPPLEVEL (default: the same
// species and level). A move list sets the moves of that Pokemon / the wild one (Pokemon_SetMoveSlot: the move and its full PP;
// 0 empties a slot; no list: the learnset's). BWPORT_PT_TESTSLOT=N (1-4) answers every move menu with slot N, =stop ends the
// test there (with TESTEXIT), =cycle with the next slot that has a move each turn (1, 2, 3, 4, 1 ...; PortRoute4_BattleMove,
// called with the move the A pulse picked). BWPORT_PT_TESTHIT: our moves always hit and use no PP (CheatSide without the rest of
// BWPORT_TESTCHEAT). BWPORT_PT_TESTEXIT: the exe exits 0 when the field is free again after the battle ("test: battle over"),
// and in cycle mode at the menu after each move was chosen once ("test: moves done"). Every chosen move is logged
// ("test: move slot S move M turn T"), all lines on stderr. Nothing here runs unless BWPORT_PT_TESTBATTLE is set.
static int sPr4TestState;   // 0 none, 1 the battle asked, 2 the field busy with it (or the battle), 3 over

static int Pr4_TestMoves(const char *p, u16 *moves) {   // "M1,M2,M3,M4" (fewer: the rest 0); returns the chars read
    const char *p0 = p;
    int i, v, n;

    for (i = 0; i < 4; i++) {
        moves[i] = 0;
    }
    for (i = 0; i < 4 && sscanf(p, "%d%n", &v, &n) == 1; i++) {
        moves[i] = (u16)v;
        p += n;
        if (*p != ',') {
            break;
        }
        p++;
    }
    return (int)(p - p0);
}

static void Pr4_TestSetMoves(Pokemon *mon, const u16 *moves) {
    int i;

    for (i = 0; i < 4; i++) {
        Pokemon_SetMoveSlot(mon, moves[i], (u8)i);
    }
}

static int Pr4_TestSlot(int input, const u16 *moves) {
    static int sTurn, sNext;
    const char *e = getenv("BWPORT_PT_TESTSLOT");
    int slot = input - 1;

    if (strcmp(e, "stop") == 0) {   // the send-out sweep: the first move menu is the end
        fprintf(stderr, "test: moves done (0) at V-blank %u\n", (unsigned)OS_GetVBlankCount());
        if (getenv("BWPORT_PT_TESTEXIT") != NULL) {
            _exit(0);
        }
    } else if (strcmp(e, "cycle") == 0) {
        if (sNext >= 4 || moves[sNext] == 0) {   // each move once: the turn after the last
            fprintf(stderr, "test: moves done (%d) at V-blank %u\n", sNext, (unsigned)OS_GetVBlankCount());
            if (getenv("BWPORT_PT_TESTEXIT") != NULL) {
                _exit(0);
            }
            sNext = 0;
        }
        slot = sNext++;
    } else if (atoi(e) >= 1 && atoi(e) <= 4 && moves[atoi(e) - 1] != 0) {
        slot = atoi(e) - 1;
    }
    fprintf(stderr, "test: move slot %d move %u turn %d at V-blank %u\n", slot + 1, (unsigned)moves[slot], ++sTurn,
            (unsigned)OS_GetVBlankCount());
    return slot + 1;
}

static void Pr4_TestBattle(FieldSystem *fs) {
    const char *e = getenv("BWPORT_PT_TESTBATTLE"), *p;
    int species = 0, level = 0, oppSpecies, oppLevel, n = 0, setOur = 0, setOpp = 0;
    u16 our[4], opp[4];
    Party *party;
    Pokemon *mon;
    FieldBattleDTO *dto;

    if (sPr4TestState != 0 || e == NULL || sscanf(e, "%d:%d%n", &species, &level, &n) != 2 || species <= 0 || level <= 0) {
        return;
    }
    sPr4TestState = 1;
    oppSpecies = species;
    oppLevel = level;
    p = e + n;
    if (*p == ':') {
        p++;
        setOur = *p != ':' && *p != '\0';
        p += Pr4_TestMoves(p, our);
    }
    if (*p == ':') {
        p++;
        if (sscanf(p, "%d%n", &oppSpecies, &n) == 1) {
            p += n;
            if (*p == '/' && sscanf(p + 1, "%d%n", &oppLevel, &n) == 1) {
                p += 1 + n;
            }
        }
    }
    if (*p == ':') {
        p++;
        setOpp = *p != '\0';
        p += Pr4_TestMoves(p, opp);
    }
    party = SaveData_GetParty(fs->saveData);
    mon = Pokemon_New(HEAP_ID_FIELD2);
    Pokemon_Init(mon);
    Pokemon_InitWith(mon, species, level, INIT_IVS_RANDOM, FALSE, 0, OTID_NOT_SET, 0);
    Pokemon_SetCatchData(mon, SaveData_GetTrainerInfo(fs->saveData), ITEM_POKE_BALL, fs->location->mapHeaderID, 0, HEAP_ID_FIELD2);
    if (setOur) {
        Pr4_TestSetMoves(mon, our);
    }
    if (Party_GetCurrentCount(party) == 0) {
        Party_AddPokemon(party, mon);
    } else {
        Party_AddPokemonBySlotIndex(party, 0, mon);
    }
    Heap_Free(mon);
    dto = FieldBattleDTO_New(HEAP_ID_FIELD2, BATTLE_TYPE_WILD_MON);
    FieldBattleDTO_Init(dto, fs);
    CreateWildMon_Scripted(fs, (u16)oppSpecies, (u8)oppLevel, dto);
    if (setOpp) {
        Pr4_TestSetMoves(Party_GetPokemonBySlotIndex(dto->parties[BATTLER_ENEMY_1], 0), opp);
    }
    MapNamePopUp_Hide(fs->unk_04->mapPopup);   // as the warp's start below (the popup's SysTask outlives the map)
    Encounter_NewVsWild(fs, dto);
    fprintf(stderr, "test: wild battle species %d level %d vs %d level %d at V-blank %u\n", species, level, oppSpecies, oppLevel,
            (unsigned)OS_GetVBlankCount());
}

// each field frame: the test battle's start (a free field) and end (the field free again after it)
static void Pr4_TestFrame(FieldSystem *fs, int free) {
    if (sPr4TestState == 0) {
        if (free) {
            Pr4_TestBattle(fs);   // BWPORT_PT_TESTBATTLE only: a field task from the next frame
        }
    } else if (sPr4TestState == 1 && !free) {
        sPr4TestState = 2;
    } else if (sPr4TestState == 2 && free) {
        sPr4TestState = 3;
        fprintf(stderr, "test: battle over at V-blank %u\n", (unsigned)OS_GetVBlankCount());
        if (getenv("BWPORT_PT_TESTEXIT") != NULL) {
            _exit(0);
        }
    }
}

// The crash sweep's debug warp (tools/pt_sweep.py, docs/platinum.md "Crash sweep"; crash finding only, never in a route or a
// library save): BWPORT_PT_WARP=MAP:X:Y[:END]. At the first frame the field is free the player is warped to map header MAP at
// tile X,Y (the matrix's global tile, as a warp event's) by the script command's warp (scrcmd.c ScrCmd_Warp:
// FieldTask_StartMapChangeFull with warp id -1, the fade, the map change, the fade in), started from a field task of ours as a
// script's task would. From then on each map the free player stands in is logged, and at V-blank END (0: never) the exe exits
// 0. BWPORT_PT_WARP_LOG=FILE gets the lines (`warp F M`, `zone M X Z F`, `end F`), unbuffered. HeartGold / SoulSilver's twin
// (BWPORT_HG_WARP) is on the hgss branch.
static int sPr4WarpMap, sPr4WarpX, sPr4WarpZ, sPr4WarpState;
static int sPr4WarpDir = DIR_SOUTH, sPr4WarpDone, sPr4Landed;   // the facing it lands with; its task is over; the free player stood on MAP (the interaction sweep starts then)

static BOOL Pr4_WarpTask(FieldTask *task) {
    if (sPr4WarpState++ == 0) {
        FieldTask_StartMapChangeFull(task, (enum MapHeaderID)sPr4WarpMap, -1, sPr4WarpX, sPr4WarpZ, sPr4WarpDir);
        return FALSE;
    }
    sPr4WarpDone = 1;
    return TRUE;   // the warp's task has ended (the task manager runs it before this one again)
}

static void Pr4_Sweep(FieldSystem *fs, int up, int freeNow) {
    static int state = -1, map, x, z, last = -1;
    static unsigned end;
    static FILE *log;
    unsigned fr;

    if (state < 0) {
        const char *e = getenv("BWPORT_PT_WARP"), *l = getenv("BWPORT_PT_WARP_LOG");

        state = 9;
        end = 0;
        if (e != NULL && sscanf(e, "%d:%d:%d:%u", &map, &x, &z, &end) >= 3) {
            state = 0;
            sPr4WarpMap = map;
            sPr4WarpX = x;
            sPr4WarpZ = z;
            log = l != NULL ? fopen(l, "w") : NULL;
            if (log != NULL) {
                setvbuf(log, NULL, _IONBF, 0);
            }
        }
    }
    if (state == 9) {
        return;
    }
    fr = OS_GetVBlankCount();
    if (up && freeNow && state == 0) {
        if (log != NULL) {
            fprintf(log, "warp %u %d\n", fr, (int)fs->location->mapHeaderID);
        }
        sPr4WarpState = 0;
        MapNamePopUp_Hide(fs->unk_04->mapPopup);   // as field_system.c does before an event task starts (the popup's SysTask outlives the map)
        FieldSystem_CreateTask(fs, Pr4_WarpTask, NULL);
        state = 1;
    } else if (up && freeNow && state == 1 && (int)fs->location->mapHeaderID != last) {
        last = (int)fs->location->mapHeaderID;
        if (last == sPr4WarpMap) {
            sPr4Landed = 1;
        }
        if (log != NULL) {
            fprintf(log, "zone %d %d %d %u\n", last, (int)PlayerAvatar_GetXPos(fs->playerAvatar),
                    (int)PlayerAvatar_GetZPos(fs->playerAvatar), fr);
        }
    }
    if (state == 1 && end != 0 && fr >= end) {
        if (log != NULL) {
            fprintf(log, "end %u\n", fr);
        }
        _exit(0);
    }
}

// ---- The interaction sweep (tools/pt_int_sweep.py, docs/platinum.md "Interaction sweep"; crash finding only, as the debug warp;
// the twin of HG/SS's Pr4_Int* on the hgss branch, same log lines and verdicts) ----
// BWPORT_PT_WARP=MAP:X:Z lands the player on MAP (Pr4_Sweep), then BWPORT_PT_INT=START[:BUDGET[:MAXN]] runs the map's events from
// index START: each object event (a person), BG event (a sign, a hidden item, a wall sign) and coord event (a trigger tile) in
// turn. Per event: the game's own warp to a free tile next to it (the player facing it), A (a trigger tile: the step onto it,
// after the event's variable is set to the value that fires it), then A / B / START pulses while the script runs, until the
// field is free for 30 V-blanks (the row is `ok`), BUDGET V-blanks pass (`timeout`, default 1500) or a battle reaches its
// action menu (`battle`: the exe exits 0, the tool starts again at the next event). MAXN events per run (default 40), then exit 0.
// BWPORT_PT_INT_LOG=FILE (unbuffered): `count N nobj nbg ncoord`, `ev I KIND SUB X Z SCRIPT A B C` for every event (KIND 0 object:
// SUB trainer type, A local id, B hidden flag, C movement type; 1 BG: SUB type, A facing; 2 coord: A var, B value, C width*1000 +
// length), `begin I F`, `at I F X Z FACE CAND`, `script I F ID` (ScriptContext_LoadAndStart: the hook in
// pokeplatinum-route4.patch), `end I F VERDICT scripts=N map=M`, `exit F WHY`. Keys: the pulses go through the key register
// (route_test.c). BWPORT_PT_INT_NOFORCE: a coord event's variable stays as it is. BWPORT_PT_INT_NOSHOW: a person hidden by its flag
// stays hidden (default: the flag is cleared for the warp and set again at the row's end: `show I flag N`).
void PortRoute_SetKeysExternal(unsigned bits);   // port/src/route_test.c

enum { PR4_INT_OFF, PR4_INT_WAIT, PR4_INT_PICK, PR4_INT_WARP, PR4_INT_SETTLE, PR4_INT_PRESS, PR4_INT_RUN };

typedef struct {
    int kind;   // 0 object, 1 BG, 2 coord
    int sub, x, z, script, a, b, c, w, h, var, val;
} Pr4IntEv;

#define PR4_INT_MAXEV 700
#define PR4_INT_CANDS 8

static struct {
    int on, state, start, budget, maxn, n, idx, ci, nc, ran, scripts, scriptsBefore, forced, nobj, nbg, ncoord, shown, pre;
    unsigned t0, pressF, busyF, freeF;
    int cx[PR4_INT_CANDS], cz[PR4_INT_CANDS], cf[PR4_INT_CANDS];
    int forceVar, forceOld;
    unsigned bAfter;
    FILE *log;
    Pr4IntEv ev[PR4_INT_MAXEV];
} sInt;

// the pulses of a running script: A A A B A A A START, each held 4 V-blanks, 6 off (START finishes a name entry's keyboard; with
// the field up it is an A: START opens the field menu there). From 600 V-blanks on: B B B A (a shop's or a counter's menus only
// close with B; the A-heavy pattern buys again and again)
static unsigned Pr4_IntPulse(unsigned dt, int up) {
    if (dt % 10 >= 4) {
        return 0;
    }
    if (dt >= sInt.bAfter) {
        return (dt / 10) % 4 == 3 ? 1u : 2u;
    }
    switch ((dt / 10) % 8) {
    case 3: return 2u;
    case 7: return up ? 1u : 8u;   // (START with the field up would open the menu)
    default: return 1u;
    }
}

static void Pr4_IntExit(unsigned fr, const char *why) {
    fprintf(sInt.log, "exit %u %s\n", fr, why);
    fflush(sInt.log);
    _exit(0);
}

static void Pr4_IntEnd(FieldSystem *fs, unsigned fr, const char *verdict) {
    if (sInt.forced) {   // the coord event's variable back
        u16 *v = FieldSystem_GetVarPointer(fs, (u16)sInt.forceVar);

        if (v != NULL) {
            *v = (u16)sInt.forceOld;
        }
        sInt.forced = 0;
    }
    if (sInt.shown != 0) {   // the person's hiding flag back
        FieldSystem_SetFlag(fs, (u16)sInt.shown);
        sInt.shown = 0;
    }
    PortRoute_SetKeysExternal(0);
    fprintf(sInt.log, "end %d %u %s scripts=%d map=%d\n", sInt.idx, fr, verdict, sInt.scripts - sInt.scriptsBefore,
            fs->location != NULL ? (int)fs->location->mapHeaderID : -1);
    sInt.idx++;
    sInt.ran++;
    sInt.state = PR4_INT_PICK;
    sInt.t0 = fr;
}

void PortRoute4_ScriptStart(u16 script) {   // ScriptContext_LoadAndStart (pokeplatinum-route4.patch): every script a context loads
    if (!sInt.on || sInt.log == NULL) {
        return;
    }
    sInt.scripts++;
    fprintf(sInt.log, "script %d %u %u\n", sInt.idx, (unsigned)OS_GetVBlankCount(), (unsigned)script);
}

// PortRoute4_BattleMenu(0): the action menu is up (a battle is not part of the field's frames: the sweep's hook does not run
// while it does), PortRoute4_BattleMove(input 1..4): a move was chosen. Either ends the row and the run: `battle`.
static void Pr4_IntBattle(int kind) {
    unsigned fr = OS_GetVBlankCount();

    if (!sInt.on || sInt.state < PR4_INT_SETTLE || (kind != 0 && kind != 100)) {
        return;
    }
    fprintf(sInt.log, "battle %d %u %s\n", sInt.idx, fr, kind == 0 ? "menu" : "move");
    fprintf(sInt.log, "end %d %u battle scripts=%d map=-1\n", sInt.idx, fr, sInt.scripts - sInt.scriptsBefore);
    Pr4_IntExit(fr, "battle");
}

static void Pr4_IntLoad(FieldSystem *fs) {   // the map's events, once, after the landing
    const ObjectEvent *oe = MapHeaderData_GetObjectEvents(fs);
    const BgEvent *be = MapHeaderData_GetBgEvents(fs);
    const CoordEvent *ce = MapHeaderData_GetCoordEvents(fs);
    int i, n = 0;

    sInt.nobj = (int)MapHeaderData_GetNumObjectEvents(fs);
    sInt.nbg = MapHeaderData_GetNumBgEvents(fs);
    sInt.ncoord = MapHeaderData_GetNumCoordEvents(fs);
    for (i = 0; i < sInt.nobj && n < PR4_INT_MAXEV; i++, n++) {
        const ObjectEvent *o = &oe[i];
        Pr4IntEv *e = &sInt.ev[n];

        memset(e, 0, sizeof *e);
        e->kind = 0;
        e->sub = o->trainerType;
        e->x = o->x;
        e->z = o->z;
        e->script = o->script;
        e->a = o->localID;
        e->b = o->hiddenFlag;
        e->c = o->movementType;
    }
    for (i = 0; i < sInt.nbg && n < PR4_INT_MAXEV; i++, n++) {
        const BgEvent *g = &be[i];
        Pr4IntEv *e = &sInt.ev[n];

        memset(e, 0, sizeof *e);
        e->kind = 1;
        e->sub = g->type;
        e->x = g->x;
        e->z = g->z;
        e->script = g->script;
        e->a = g->playerFacingDir;
    }
    for (i = 0; i < sInt.ncoord && n < PR4_INT_MAXEV; i++, n++) {
        const CoordEvent *c = &ce[i];
        Pr4IntEv *e = &sInt.ev[n];

        memset(e, 0, sizeof *e);
        e->kind = 2;
        e->x = c->x;
        e->z = c->z;
        e->script = c->script;
        e->w = c->width;
        e->h = c->length;
        e->var = c->var;
        e->val = c->value;
    }
    sInt.n = n;
    fprintf(sInt.log, "count %d %d %d %d\n", n, sInt.nobj, sInt.nbg, sInt.ncoord);
    for (i = 0; i < n; i++) {
        const Pr4IntEv *e = &sInt.ev[i];

        fprintf(sInt.log, "ev %d %d %d %d %d %d %d %d %d\n", i, e->kind, e->sub, e->x, e->z, e->script,
                e->kind == 2 ? e->var : e->a, e->kind == 2 ? e->val : e->b, e->kind == 2 ? e->w * 1000 + e->h : e->c);
    }
}

// a tile the player can stand on: 0 blocked (outside the matrix, another object, the collision bit), 1 free, 2 unknown (the land
// chunk is not loaded: the game keeps the player's surroundings only)
static int Pr4_IntTile(FieldSystem *fs, int x, int z) {
    u16 a;
    MapObject *o;

    if (x < 0 || z < 0 || Pr4_ZoneAt(x, z) < 0) {
        return 0;
    }
    o = sub_0206326C(fs->mapObjMan, x, z, 0);
    if (o != NULL && o != PlayerAvatar_GetMapObject(fs->playerAvatar)) {
        return 0;
    }
    if (!fs->terrainCollisionMan->getTileAttributes(fs, x, z, &a)) {
        return 2;
    }
    return ((a >> 15) & 1) ? 0 : 1;
}

// a table / counter tile (sub_0203C9D4: the person is talked to across it, one tile further)
static int Pr4_IntTable(int x, int z) {
    u16 a;

    return Pr4_Attr(x, z, &a) && TileBehavior_IsTable((u8)(a & 0xff));
}

// the tiles to stand on for event E, the facing toward it: where a facing d looks (DIR_NORTH 0, SOUTH 1, WEST 2, EAST 3)
static void Pr4_IntCands(FieldSystem *fs, const Pr4IntEv *e, int loaded, int *unknown) {
    static const int kDx[4] = {0, 0, -1, 1}, kDz[4] = {-1, 1, 0, 0};
    int faces = 0xf, d, dist;

    sInt.nc = 0;
    *unknown = 0;
    if (e->kind == 1 && e->sub != BG_EVENT_TYPE_HIDDEN_ITEM) {   // a facing or wall sign event: BG_EVENT_DIR_* (0 N 1 E 2 W 3 S 4 any 5 east/west 6 north/south)
        switch (e->a) {
        case BG_EVENT_DIR_NORTH: faces = 1 << DIR_NORTH; break;
        case BG_EVENT_DIR_EAST: faces = 1 << DIR_EAST; break;
        case BG_EVENT_DIR_WEST: faces = 1 << DIR_WEST; break;
        case BG_EVENT_DIR_SOUTH: faces = 1 << DIR_SOUTH; break;
        case BG_EVENT_DIR_WEST_EAST: faces = (1 << DIR_WEST) | (1 << DIR_EAST); break;
        case BG_EVENT_DIR_NORTH_SOUTH: faces = (1 << DIR_NORTH) | (1 << DIR_SOUTH); break;
        default: faces = 0xf; break;
        }
        if (e->sub == BG_EVENT_TYPE_WALL_SIGN) {   // (FieldEvent_GetInteractedWallSignScript: facing north whatever the event says)
            faces |= 1 << DIR_NORTH;
        }
    }
    for (dist = 1; dist <= 2; dist++) {
        for (d = 0; d < 4 && sInt.nc < PR4_INT_CANDS; d++) {
            int tx = e->x - kDx[d] * dist, tz = e->z - kDz[d] * dist;
            int st;

            if (!(faces & (1 << d))) {
                continue;
            }
            st = Pr4_IntTile(fs, tx, tz);
            if (st == 2 && !loaded) {
                *unknown = 1;
                continue;
            }
            if (st == 0) {
                continue;
            }
            if (e->kind == 2) {   // outside the region, one step from its tile
                if (dist != 1 || (tx >= e->x && tx < e->x + e->w && tz >= e->z && tz < e->z + e->h)) {
                    continue;
                }
            } else if (dist == 2 && (e->kind != 0 || !Pr4_IntTable(e->x - kDx[d], e->z - kDz[d]))) {   // a person across a table only
                continue;
            }
            sInt.cx[sInt.nc] = tx;
            sInt.cz[sInt.nc] = tz;
            sInt.cf[sInt.nc] = d;
            sInt.nc++;
        }
    }
}

static void Pr4_IntWarp(FieldSystem *fs, unsigned fr) {
    sPr4WarpX = sInt.cx[sInt.ci];
    sPr4WarpZ = sInt.cz[sInt.ci];
    sPr4WarpDir = sInt.cf[sInt.ci];
    sPr4WarpState = 0;
    sPr4WarpDone = 0;
    MapNamePopUp_Hide(fs->unk_04->mapPopup);   // as Pr4_Sweep does before its warp task
    FieldSystem_CreateTask(fs, Pr4_WarpTask, NULL);
    sInt.state = PR4_INT_WARP;
    sInt.t0 = fr;
}

static void Pr4_IntFrame(FieldSystem *fs, int up, int freeNow) {
    static int sInit = -1;
    unsigned fr = OS_GetVBlankCount();
    Pr4IntEv *e;

    if (sInit < 0) {
        const char *s = getenv("BWPORT_PT_INT"), *l = getenv("BWPORT_PT_INT_LOG");

        sInit = 0;
        if (s != NULL && l != NULL && getenv("BWPORT_PT_WARP") != NULL) {
            sInt.start = 0;
            sInt.budget = 1500;
            sInt.maxn = 40;
            sInt.bAfter = getenv("BWPORT_PT_INT_BAFTER") != NULL ? (unsigned)atoi(getenv("BWPORT_PT_INT_BAFTER")) : 600u;
            sscanf(s, "%d:%d:%d", &sInt.start, &sInt.budget, &sInt.maxn);
            sInt.log = fopen(l, "w");
            if (sInt.log != NULL) {
                setvbuf(sInt.log, NULL, _IONBF, 0);
                sInt.on = 1;
                sInt.state = PR4_INT_WAIT;
                sInt.idx = sInt.start;
                sInit = 1;
            }
        }
    }
    if (!sInt.on) {
        return;
    }
    sPr4Fs = up ? fs : NULL;
    if (sInt.state == PR4_INT_WAIT) {
        if (up && freeNow && sPr4Landed && fr >= 1200) {
            Pr4_IntLoad(fs);
            sInt.state = PR4_INT_PICK;
            sInt.t0 = fr;
        } else if (fr > 6000) {
            Pr4_IntExit(fr, "no-landing");
        } else if (sPr4WarpState >= 1 && !(up && freeNow)) {   // an arrival script (a cutscene, an app) holds the field: the pulses
            PortRoute_SetKeysExternal(Pr4_IntPulse(fr, up));
        } else if (sPr4WarpState >= 1) {
            PortRoute_SetKeysExternal(0);
        }
        return;
    }
    if (sInt.state == PR4_INT_PICK) {
        int unknown;

        if (sInt.idx >= sInt.n) {
            Pr4_IntExit(fr, "done");
        }
        if (sInt.ran >= sInt.maxn) {
            Pr4_IntExit(fr, "segment");
        }
        if (!(up && freeNow)) {   // a scene holds the field between two rows (an OnFrame script after the landing): the pulses
            PortRoute_SetKeysExternal(Pr4_IntPulse(fr - sInt.t0, up));
            if (fr - sInt.t0 > (unsigned)sInt.budget) {
                Pr4_IntExit(fr, "stuck-before-warp");
            }
            return;
        }
        PortRoute_SetKeysExternal(0);
        e = &sInt.ev[sInt.idx];
        sInt.pre = 0;
        fprintf(sInt.log, "begin %d %u %d\n", sInt.idx, fr, (int)fs->location->mapHeaderID);   // (the map the player is in: a script may have left the sweep's)
        if (e->kind == 0 && e->script == 0xFFFF) {   // a person with no script (decoration): nothing for A to start
            Pr4_IntEnd(fs, fr, "unset");
            return;
        }
        Pr4_IntCands(fs, e, 0, &unknown);
        if ((sInt.nc == 0 && unknown) || (sInt.nc != 0 && (int)fs->location->mapHeaderID != sPr4WarpMap)) {
            // the tiles around it are not loaded, or the last script left the map (the tiles are another map's): stand on its tile first
            sInt.pre = 1;
            sInt.cx[0] = e->x;
            sInt.cz[0] = e->z;
            sInt.cf[0] = DIR_SOUTH;
            sInt.nc = 1;
        } else if (sInt.nc == 0) {
            Pr4_IntEnd(fs, fr, "no-spot");
            return;
        }
        if (e->kind == 0 && e->b != 0 && getenv("BWPORT_PT_INT_NOSHOW") == NULL && FieldSystem_CheckFlag(fs, (u16)e->b)) {
            FieldSystem_ClearFlag(fs, (u16)e->b);   // a person the story keeps hidden here: shown for the warp (the map loads its objects again)
            sInt.shown = e->b;
            fprintf(sInt.log, "show %d flag %d\n", sInt.idx, e->b);
        }
        sInt.ci = 0;
        sInt.scriptsBefore = sInt.scripts;
        Pr4_IntWarp(fs, fr);
        return;
    }
    e = &sInt.ev[sInt.idx];
    if (sInt.state == PR4_INT_WARP) {
        if (sPr4WarpDone && up && freeNow && PlayerAvatar_GetXPos(fs->playerAvatar) == sInt.cx[sInt.ci] &&
            PlayerAvatar_GetZPos(fs->playerAvatar) == sInt.cz[sInt.ci]) {
            fprintf(sInt.log, "at %d %u %d %d %d %d\n", sInt.idx, fr, sInt.cx[sInt.ci], sInt.cz[sInt.ci],
                    PlayerAvatar_GetFacingDir(fs->playerAvatar), sInt.ci);
            if (sInt.pre) {   // on the event's tile: the land around it is loaded, so now the real spot
                int unknown;

                sInt.pre = 0;
                Pr4_IntCands(fs, e, 1, &unknown);
                if (sInt.nc == 0) {
                    Pr4_IntEnd(fs, fr, "no-spot");
                } else {
                    sInt.ci = 0;
                    Pr4_IntWarp(fs, fr);
                }
                return;
            }
            sInt.scriptsBefore = sInt.scripts;   // (the map's arrival scripts are over: what begins now is the event's)
            sInt.state = PR4_INT_SETTLE;
            sInt.t0 = fr;
            sInt.freeF = fr;
        } else if (fr - sInt.t0 > 900) {
            fprintf(sInt.log, "end %d %u warp-timeout scripts=%d map=%d\n", sInt.idx, fr, sInt.scripts - sInt.scriptsBefore,
                    fs->location != NULL ? (int)fs->location->mapHeaderID : -1);
            Pr4_IntExit(fr, "warp-timeout");
        }
        return;
    }
    if (sInt.scripts != sInt.scriptsBefore && sInt.state != PR4_INT_RUN) {   // a script began on its own (a trainer's sight, a landing trigger)
        sInt.state = PR4_INT_RUN;
        sInt.pressF = fr;
        sInt.busyF = fr;
    }
    if (sInt.state == PR4_INT_SETTLE) {
        if (!(up && freeNow)) {
            sInt.freeF = fr;
        } else if (fr - sInt.freeF >= 20) {   // the fade-in is over, the field takes keys
            if (e->kind == 0 && MapObjMan_LocalMapObjByIndex(fs->mapObjMan, e->a) == NULL) {
                Pr4_IntEnd(fs, fr, "absent");
                return;
            }
            if (e->kind == 1 && e->sub == BG_EVENT_TYPE_HIDDEN_ITEM && FieldSystem_CheckFlag(fs, Script_GetHiddenItemFlag((u16)e->script))) {
                Pr4_IntEnd(fs, fr, "collected");
                return;
            }
            if (e->kind == 2 && getenv("BWPORT_PT_INT_NOFORCE") == NULL && e->var >= VARS_START && e->var < SCRIPT_LOCAL_VARS_START) {
                u16 *v = FieldSystem_GetVarPointer(fs, (u16)e->var);

                if (v != NULL && *v != (u16)e->val) {
                    sInt.forceVar = e->var;
                    sInt.forceOld = *v;
                    *v = (u16)e->val;
                    sInt.forced = 1;
                    fprintf(sInt.log, "force %d var %d %d -> %d\n", sInt.idx, e->var, sInt.forceOld, e->val);
                }
            }
            sInt.state = PR4_INT_PRESS;
            sInt.pressF = fr;
            fprintf(sInt.log, "press %d %u\n", sInt.idx, fr);
        }
        return;
    }
    if (sInt.state == PR4_INT_PRESS) {
        unsigned dt = fr - sInt.pressF;
        static const unsigned kDirBits[4] = {0x40, 0x80, 0x20, 0x10};   // up down left right (KEYINPUT bits)

        if (e->kind == 2) {
            PortRoute_SetKeysExternal(dt < 24 ? kDirBits[sInt.cf[sInt.ci]] : 0);
        } else {
            PortRoute_SetKeysExternal(dt < 4 ? 1u : 0u);
        }
        if (dt >= (e->kind == 2 ? 90u : 50u)) {   // nothing began: the next tile to try, else a row without a script
            PortRoute_SetKeysExternal(0);
            if (++sInt.ci < sInt.nc) {
                Pr4_IntWarp(fs, fr);
            } else {
                Pr4_IntEnd(fs, fr, "no-script");
            }
        }
        return;
    }
    if (sInt.state == PR4_INT_RUN) {
        unsigned dt = fr - sInt.pressF;

        // the pulses only while something runs (a free field would talk to the person again)
        PortRoute_SetKeysExternal(!(up && freeNow) ? Pr4_IntPulse(dt, up) : 0u);
        if (!(up && freeNow)) {
            sInt.busyF = fr;
        }
        if (fr - sInt.busyF >= 30) {
            Pr4_IntEnd(fs, fr, "ok");
        } else if (dt >= (unsigned)sInt.budget) {
            fprintf(sInt.log, "end %d %u timeout scripts=%d map=%d\n", sInt.idx, fr, sInt.scripts - sInt.scriptsBefore,
                    fs->location != NULL ? (int)fs->location->mapHeaderID : -1);
            Pr4_IntExit(fr, "timeout");   // the field is not free: nothing to warp from
        }
    }
}

// each field frame (field_system.c's HandleInputsEventsAndProcesses, after the frame's tasks and processes): the pilot's
// state. Off the field map (a battle, a menu app, a map change) the last field position, an event running
// BWPORT_PROPDUMP=LO,HI: each time the set changes, a line for each loaded map prop (every loaded chunk's) whose model id is in
// LO..HI: the id, the chunk slot, and the tile x,z it stands on (docs/platinum.md, the Pastoria gym's buttons).
static void Pr4_PropDump(FieldSystem *fs) {
    static int sZone = -1, sSig = 0;
    const char *e = getenv("BWPORT_PROPDUMP");
    int lo, hi, i, j, pass, sig = 0;

    if (e == NULL || sscanf(e, "%d,%d", &lo, &hi) != 2 || fs->landDataMan == NULL || fs->mapMatrix == NULL) {
        return;
    }
    for (pass = 0; pass < 2; pass++) {
        if (pass == 1) {
            if (sZone == (int)fs->location->mapHeaderID && sSig == sig) {
                return;
            }
            sZone = (int)fs->location->mapHeaderID;
            sSig = sig;
        }
        for (i = 0; i < QUADRANT_COUNT; i++) {
            MapPropManager *mgr = NULL;
            VecFx32 org;

            LandDataManager_GetLoadedMapPropManager((u8)i, fs->landDataMan, &mgr);
            if (mgr == NULL) {
                continue;
            }
            TerrainCollisionManager_GetMapAbsoluteOrigin(LandDataManager_GetLoadedMapMatrixIndex(fs->landDataMan, (u8)i),
                                                         MapMatrix_GetWidth(fs->mapMatrix), &org);
            for (j = 0; j < MAX_LOADED_MAP_PROPS; j++) {
                MapProp *mp = MapPropManager_GetLoadedProp(mgr, (u8)j);
                int id = MapProp_GetModelID(mp);
                VecFx32 pos;

                if (id < lo || id > hi) {
                    continue;
                }
                pos = MapProp_GetPosition(mp);
                if (pass == 0) {
                    sig = sig * 31 + id + (int)((pos.x + org.x) >> 12) * 7 + (int)((pos.z + org.z) >> 12);
                } else {
                    fprintf(stderr, "propdump: zone %d quadrant %d slot %d model %d height %d tile %d,%d\n", sZone, i, j, id,
                            (int)(pos.y >> 12), (int)((pos.x + org.x) >> 12) / 16, (int)((pos.z + org.z) >> 12) / 16);
                }
            }
        }
    }
}

// BWPORT_GYMDUMP=1: once per map change, one line a tile of the map's two chunks (x 0..31, z 0..63): the tile attributes (the
// behavior and the collision bit), the static terrain height (BDHC only: a reference far below, so the dynamic plate does
// not win), the height's source (0 none, 1 BDHC, 2 plate), the dynamic plate index there (-1 none) and the plate's current
// height (docs/platinum.md, the Pastoria gym: the data its solver reads).
static void Pr4_GymDump(FieldSystem *fs) {
    static int sZone = -1, sP = -1;
    int x, z;

    if (getenv("BWPORT_GYMDUMP") == NULL || fs->terrainCollisionMan == NULL || fs->dynamicTerrainHeightMan == NULL) {
        return;
    }
    if (sZone == (int)fs->location->mapHeaderID) {
        int p = (int)(DynamicTerrainHeightManager_GetHeight(0, fs->dynamicTerrainHeightMan) >> 12);

        if (p != sP) {
            sP = p;
            fprintf(stderr, "gymdump: plate0 now %d at V-blank %u (player %d,%d)\n", p, (unsigned)OS_GetVBlankCount(),
                    PlayerAvatar_GetXPos(fs->playerAvatar), PlayerAvatar_GetZPos(fs->playerAvatar));
        }
        return;
    }
    sZone = (int)fs->location->mapHeaderID;
    fprintf(stderr, "gymdump: zone %d plate0 height %d\n", sZone, (int)(DynamicTerrainHeightManager_GetHeight(0, fs->dynamicTerrainHeightMan) >> 12));
    for (z = 0; z < 64; z++) {
        for (x = 0; x < 32; x++) {
            u16 a = 0;
            u8 src = 0, pi = 0;
            fx32 h;
            int plate;

            if (!sPr4Fs->terrainCollisionMan->getTileAttributes(fs, x, z, &a)) {
                continue;
            }
            h = TerrainCollisionManager_GetHeight(fs, -1600 * FX32_ONE, x * PR4_TILE + PR4_TILE / 2, z * PR4_TILE + PR4_TILE / 2, &src);
            plate = DynamicTerrainHeightManager_GetPlateIndexOfTile(x, z, fs->dynamicTerrainHeightMan, &pi) ? (int)pi : -1;
            fprintf(stderr, "gymdump: %d %d b %d c %d h %d s %d p %d\n", x, z, (int)(a & 0xff), (int)((a >> 15) & 1), (int)(h >> 12),
                    (int)src, plate);
        }
    }
}

// BWPORT_PT_SCRIPT=ID[:FROM] (test only, off by default; the twin of HG/SS's BWPORT_HG_SCRIPT, tools/plays/platinum/pc-*): the first free
// field frame at or after V-blank FROM (and after the BWPORT_PT_WARP warp's task) starts the game's own script ID as a map's scene
// script is started (ScriptManager_Set, no object). 2018 = CommonScript_PC (the Pokemon Center PC: its messages and menus then take the
// play's keys, and the script's own OpenPokemonStorage opens the box app).
static void Pr4_ScriptHook(FieldSystem *fs, int freeNow) {
    static int id = -2, done;
    static unsigned from;

    if (id == -2) {
        const char *e = getenv("BWPORT_PT_SCRIPT");
        unsigned f = 0;

        id = -1;
        if (e != NULL && sscanf(e, "%d:%u", &id, &f) >= 1 && id >= 0) {
            from = f;
        } else {
            id = -1;
        }
    }
    if (id < 0 || done || !freeNow || OS_GetVBlankCount() < from || (getenv("BWPORT_PT_WARP") != NULL && sPr4WarpState < 2)) {
        return;
    }
    done = 1;
    fprintf(stderr, "test: script %d started at V-blank %u\n", id, (unsigned)OS_GetVBlankCount());
    ScriptManager_Set(fs, (u16)id, NULL);
}

// BWPORT_PT_POKETCH=1 (test only, off by default; tools/plays/platinum/menu-poketch): the first free field frame enables the Poketch and
// registers every app (POKETCH_APPID_DIGITALWATCH .. the last), so the play's taps on the app-switch buttons (the two at x 224-255,
// y 32-96 and 96-160 of the touch screen) walk through all of them, each one's own overlay and heap.
static void Pr4_PoketchHook(FieldSystem *fs, int freeNow) {
    static int on = -1, done;

    if (on < 0) {
        on = getenv("BWPORT_PT_POKETCH") != NULL;
    }
    if (!on || done || !freeNow) {
        return;
    }
    done = 1;
    {
        Poketch *poketch = SaveData_GetPoketch(fs->saveData);
        int i, n = 0;

        Poketch_Enable(poketch);
        for (i = 0; i < 40; i++) {
            n += Poketch_RegisterApp(poketch, (enum PoketchAppID)i) ? 1 : 0;
        }
        fprintf(stderr, "test: poketch apps registered: %d at V-blank %u\n", n, (unsigned)OS_GetVBlankCount());
    }
}

// BWPORT_HDUMP=x0,z0,x1,z1,V: once, at V-blank V (the field up), the terrain height of each tile of the box asked at a series of
// object heights (tiles x 16: 0 8 16 24 32 48 64 80 96 128): a map with layers (Sunyshore's bridges over its avenues) answers
// with the layer nearest the asked height, so the columns show which layers a tile has
static void Pr4_HDump(FieldSystem *fs) {
    static int sDone = 0;
    static const int kAsk[10] = {0, 8, 16, 24, 32, 48, 64, 80, 96, 128};
    const char *e = getenv("BWPORT_HDUMP");
    int x0, z0, x1, z1, v, x, z, i;

    if (e == NULL || sDone || sscanf(e, "%d,%d,%d,%d,%d", &x0, &z0, &x1, &z1, &v) != 5 || (int)OS_GetVBlankCount() < v) {
        return;
    }
    sDone = 1;
    fprintf(stderr, "hdump: zone %d box %d,%d..%d,%d asked 0 8 16 24 32 48 64 80 96 128\n", (int)fs->location->mapHeaderID, x0, z0, x1, z1);
    for (z = z0; z <= z1; z++) {
        for (x = x0; x <= x1; x++) {
            u16 a = 0;
            int ok = fs->terrainCollisionMan->getTileAttributes(fs, x, z, &a);

            fprintf(stderr, "hdump: %d %d b %02x c %d h", x, z, ok ? (int)(a & 0xff) : -1, ok ? (int)((a >> 15) & 1) : -1);
            for (i = 0; i < 10; i++) {
                fx32 h = TerrainCollisionManager_GetHeight(fs, kAsk[i] * FX32_ONE, x * PR4_TILE + PR4_TILE / 2, z * PR4_TILE + PR4_TILE / 2, NULL);

                fprintf(stderr, " %d", (int)(h >> 12));
            }
            fprintf(stderr, "\n");
        }
    }
}

// BWPORT_DWDUMP: the Distortion World floor's floating platform jump points, once per zone (stderr): the facing, the bounds
// (start x y z, size x y z), the displacement, the platform kind, the handler
static void Pr4_DwDump(FieldSystem *fs) {
    static int sZone = -1, sKind = -1;
    int i, n, v[12];

    if (getenv("BWPORT_DWDUMP") == NULL) {
        return;
    }
    if (Pr4_DwKind(fs) != sKind) {   // onto another plane: the player's tile (y in tiles), world position and the planner's tile
        MapObject *self = PlayerAvatar_GetMapObject(fs->playerAvatar);
        const VecFx32 *p = PlayerAvatar_GetPos(fs->playerAvatar);
        int gx, gz, fx, fz;

        sKind = Pr4_DwKind(fs);
        Pr4_DwPlayer(fs, sKind, &gx, &gz, &fx, &fz);
        fprintf(stderr, "dwkind: %d at %d %d %d pos %d %d %d plan %d,%d fine %d,%d (V-blank %u)\n", sKind, MapObject_GetX(self),
                MapObject_GetY(self) / 2, MapObject_GetZ(self), (int)(p->x >> FX32_SHIFT), (int)(p->y >> FX32_SHIFT),
                (int)(p->z >> FX32_SHIFT), gx, gz, fx, fz, (unsigned)OS_GetVBlankCount());
    }
    Pr4_DwLoad(fs);   // the floor's floating platforms too (dwplat: once per zone), before the player stands on one
    if (sZone == (int)fs->location->mapHeaderID) {
        return;
    }
    n = DistWorld_PortJumpPoint(fs, -1, v);
    if (n == 0) {
        return;
    }
    sZone = (int)fs->location->mapHeaderID;
    for (i = 0; i < n; i++) {
        DistWorld_PortJumpPoint(fs, i, v);
        fprintf(stderr, "dwdump: zone %d jp %d dir %d at %d %d %d size %d %d %d by %d %d %d kind %d h %d\n", sZone, i, v[0], v[1],
                v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11]);
    }
}

// BWPORT_LDUMP=x0,z0,x1,z1,V: once, at V-blank V (the field up), a line per tile of the box (stderr): x z, the tile attribute (hex),
// the side blocks of the tile as a leaver and as an entered tile (4 bits, u d l r), and the terrain height answered to an object
// at each height from -160 to 160 (steps of 8), written as the asked height and the answer where the answer changes: a cave with
// layers (Victory Road's bridges over its corridors) shows its layers per tile, which a one-layer dump cannot
static void Pr4_LDump(FieldSystem *fs) {
    static int sDone = 0;
    const char *e = getenv("BWPORT_LDUMP");
    int x0, z0, x1, z1, v, x, z, ask, d;

    if (e == NULL || sDone || sscanf(e, "%d,%d,%d,%d,%d", &x0, &z0, &x1, &z1, &v) != 5 || (int)OS_GetVBlankCount() < v) {
        return;
    }
    sDone = 1;
    fprintf(stderr, "ldump: zone %d box %d,%d..%d,%d\n", (int)fs->location->mapHeaderID, x0, z0, x1, z1);
    for (z = z0; z <= z1; z++) {
        for (x = x0; x <= x1; x++) {
            u16 a = 0;
            int ok = fs->terrainCollisionMan->getTileAttributes(fs, x, z, &a);
            u8 b = (u8)(a & 0xff);
            int ex = 0, en = 0, last = -9999;

            if (!ok) {
                continue;
            }
            for (d = 0; d < 4; d++) {
                ex |= kPr4ExitBlock[d](b) ? (1 << d) : 0;
                en |= kPr4EnterBlock[d](b) ? (1 << d) : 0;
            }
            fprintf(stderr, "ldump: %d %d a %04x ex %x en %x nul %d h", x, z, (int)a, ex, en, b == GetNullTileBehaviorID() ? 1 : 0);
            for (ask = -160; ask <= 160; ask += 8) {
                fx32 h = TerrainCollisionManager_GetHeight(fs, ask * FX32_ONE, x * PR4_TILE + PR4_TILE / 2, z * PR4_TILE + PR4_TILE / 2, NULL);

                if ((int)(h >> 12) != last) {
                    last = (int)(h >> 12);
                    fprintf(stderr, " %d:%d", ask, last);
                }
            }
            fprintf(stderr, "\n");
        }
    }
}

void PortRoute4_Frame(FieldSystem *fs) {
    static PortRouteState rs = {PORT_ROUTE_GAME, 0xffff, 0, 0, 1, -1, -1, 0, 0};

    Pr4_Sweep(fs, Pr4_FieldUp(fs), Pr4_FieldUp(fs) && !fs->processManager->pause && !FieldSystem_IsRunningTask(fs));
    Pr4_ScriptHook(fs, Pr4_FieldUp(fs) && !fs->processManager->pause && !FieldSystem_IsRunningTask(fs));
    Pr4_IntFrame(fs, Pr4_FieldUp(fs), Pr4_FieldUp(fs) && !fs->processManager->pause && !FieldSystem_IsRunningTask(fs));
    Pr4_PoketchHook(fs, Pr4_FieldUp(fs) && !fs->processManager->pause && !FieldSystem_IsRunningTask(fs));
    if (Pr4_FieldUp(fs)) {
        sPr4Fs = fs;
        PortSpeed4_MoveLog(PlayerAvatar_GetXPos(fs->playerAvatar), PlayerAvatar_GetZPos(fs->playerAvatar), PlayerAvatar_GetPos(fs->playerAvatar)->x >> FX32_SHIFT, PlayerAvatar_GetPos(fs->playerAvatar)->z >> FX32_SHIFT, -1, -1, -1,
                           PlayerAvatar_GetPlayerMoveState(fs->playerAvatar));
        Pr4_FlySlot(fs);
        Pr4_PropDump(fs);
        Pr4_GymDump(fs);
        Pr4_HDump(fs);
        Pr4_DwDump(fs);
        Pr4_LDump(fs);
        rs.zone = fs->location->mapHeaderID;
        Pr4_DwPlayer(fs, Pr4_DwKind(fs), &rs.gx, &rs.gz, &rs.fx, &rs.fz);   // the world's x,z off the floating platforms
        // free: the field takes keys (field_system.c's HandleFieldInput: not paused, the map running, no field task: a
        // script, a warp, the start menu are tasks)
        rs.event = fs->processManager->pause || FieldSystem_IsRunningTask(fs) ? 1 : 0;
        if (getenv("BWPORT_PT_TESTBATTLE") != NULL) {
            Pr4_TestFrame(fs, !rs.event);
        }
        // the deep-mud task is running (it ran this frame and did not end in it): the key of the direction opposite the one the
        // player faces, a turn each time (the facing changes at once, the task takes the next turn when its animation is done)
        rs.mud = 0;
        if (sPr4Mud && FieldSystem_IsRunningTask(fs)) {
            switch (PlayerAvatar_GetFacingDir(fs->playerAvatar)) {
            case DIR_NORTH: rs.mud = 0x80; break;   // down
            case DIR_SOUTH: rs.mud = 0x40; break;   // up
            case DIR_WEST: rs.mud = 0x10; break;    // right
            default: rs.mud = 0x20; break;          // left
            }
        }
        sPr4Mud = 0;
        if (!rs.event) {   // nothing runs: no question and no save left over (a script that ended another way)
            gPortRouteFieldYesNo = 0;
            sPr4Save = -1;
        }
        rs.save = sPr4Save;
        PortRouteMap_SetAdapter(&kPortRouteGen4);
    } else {
        sPr4Fs = NULL;
        rs.event = 1;
        PortRouteMap_SetAdapter(NULL);
    }
    PortRoute_Frame(&rs);
}

#else   // HEARTGOLD: HeartGold and SoulSilver (one module, the version is gGameVersion: docs/hgss.md "SoulSilver")
// The same adapter and hooks on pret's pokeheartgold names (port/subprojects/pokeheartgold-route.patch calls them):
//   - the frame hook: field_system.c FieldSystem_Main, after the frame's tasks and processes (each frame the field runs);
//   - the field menu (start_menu.c, the touch screen's icons): Task_StartMenu_HandleInput while it takes keys, SAVE chosen
//     (Task_StartMenu_HandleSelection_Save), the menu back from the save (sub_0203D288); touch_save_app.c after its write
//     (Field_SaveGameNormal); scrcmd_c.c sub_020416E4, a script's yes/no each frame it waits.
// The tiles: fs->unk60 holds the field's two terrain callbacks (Platinum's TerrainCollisionManager: the height, the 16-bit
// attributes: bit 15 collision, the low byte the behavior), the side blocks are the game's own tables of the step check
// (unk_0205FD20 sub_02060DEC: _020FD4CC on the tile left, _020FD4BC on the tile entered).
#include "config.h"
#include "battle/battle_setup.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"
#include "encounter.h"
#include "field/encounter_check.h"
#include "field_system.h"
#include "move.h"
#include "party.h"
#include "pokemon.h"
#include "script_manager.h"
#include "script_pokemon_util.h"
#include "map_events_internal.h"
#include "map_matrix.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "player_avatar.h"
#include "start_menu.h"
#include "task.h"
#include "script_manager.h"
#include "field_warp_tasks.h"
#include "bag.h"
#include "daycare.h"
#include "player_data.h"
#include "pokedex.h"
#include "pokemon_storage_system.h"
#include "save_arrays.h"
#include "save_vars_flags.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "port_route_story.h"
#include "save_pokegear.h"
#include "overlay_01_021F6830.h"
#include "overlay_01_field_menu_manager.h"
#include "list_menu_2d.h"
#include "overlay_27_ov27task_data.h"
#include "sys_task.h"
#include "update_dex_received.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokegear_card.h"
#include "party_menu.h"
#include "application/pokegear/map/pokegear_map_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "port_route.h"
#include "port_route_map.h"

void PortRoute4_Frame(FieldSystem *fieldSystem);
void PortSpeed4_MoveLog(int gx, int gz, int px, int pz, int fgx, int fgz, int fstate, int moveState);   // port/game/speed_gen4.c (BWPORT_MOVELOG4)
void PortRoute4_StartMenu(FieldSystem *fieldSystem);
void PortRoute4_SaveStart(void);
void PortRoute4_SaveEnd(void);
void PortRoute4_YesNo(int row);
void PortRoute4_Saved(void);

extern int gPortRouteFieldYesNo;   // port/src/route_test.c: the field's yes/no list is up, 1 + its cursor's row (Py, Pn)
extern BOOL (*const _020FD4CC[4])(u8);   // unk_0205FD20.s: the tile left blocks the step (up, down, left, right)
extern BOOL (*const _020FD4BC[4])(u8);   // the tile entered blocks it
extern const u8 ov27_0225D0B4[7 * 4 * 3];   // overlay_27.s: the field menu's cursor moves (icon, key, 3 candidates)

#define PR4_TILE (16 << FX32_SHIFT)   // a tile in world units

typedef struct {   // fs->unk60 (unk_02054648's TerrainCollisionManager)
    fx32 (*getHeight)(const FieldSystem *, fx32, fx32, fx32, u8 *);
    BOOL (*getTileAttributes)(const FieldSystem *, int, int, u16 *);
} Pr4Terrain;

static FieldSystem *sPr4Fs;   // set by the frame hook while the field map runs

static const char *Pr4_Game(void) {
    return GAME_IS_HEARTGOLD ? "HG" : "SS";
}

static int Pr4_FieldUp(const FieldSystem *fs) {
    return fs != NULL && fs->runningFieldMap && fs->processManager != NULL && fs->processManager->parent != NULL &&
           fs->playerAvatar != NULL && fs->unk60 != NULL && fs->mapMatrix != NULL && fs->mapEvents != NULL &&
           fs->location != NULL;
}

// (declared here: dladdr's Dl_info is the same four fields on glibc and bionic; hang_watch.c does the same)
typedef struct { const char *dli_fname; void *dli_fbase; const char *dli_sname; void *dli_saddr; } Pr4DlInfo;
int dladdr(const void *addr, Pr4DlInfo *info);

static long Pr4_ImageOffset(const void *p) {
    Pr4DlInfo di;

    return p != NULL && dladdr(p, &di) ? (long)((const char *)p - (const char *)di.dli_fbase) : 0;
}

// The game's part of the state line run_frames.c prints at the frame cap (tools/sweep_wall.py stuck_top): whether the field is up,
// its task (an image offset: addr2line -f -e the exe), and when the task is the script engine's the running script's id, the
// context's mode (0 stopped, 1 bytecode, 2 native wait) and the native function it waits in (an offset). In a battle or an app
// the field's task is the one that called it.
extern void (*PORT_StateHook)(char *buf, size_t n);   // port/src/run_frames.c

void PortRoute_StateLine(char *buf, size_t n) {
    const FieldSystem *fs = sPr4Fs;
    TaskManager *tm = fs != NULL ? fs->taskman : NULL;
    ScriptEnvironment *env = tm != NULL ? TaskManager_GetEnvironment(tm) : NULL;
    int len;

    buf[0] = 0;
    if (fs == NULL) {
        snprintf(buf, n, " field=none");
        return;
    }
    len = snprintf(buf, n, " field=%s task=+0x%lx", Pr4_FieldUp(fs) ? "up" : "down", Pr4_ImageOffset((const void *)(tm != NULL ? tm->func : NULL)));
    if (tm != NULL && tm->func == Task_RunScripts && env != NULL && env->check == Unk80_10_C_MAGIC && len > 0 && (size_t)len < n) {
        const ScriptContext *ctx = env->scriptContexts[0];

        snprintf(buf + len, n - (size_t)len, " script=%u mode=%d native=+0x%lx", (unsigned)env->activeScriptNumber,
                 ctx != NULL ? (int)ctx->mode : -1, ctx != NULL ? Pr4_ImageOffset((const void *)ctx->native_ptr) : 0);
    }
}

// the map matrix's zone (map header) at grid tile x,z: 32 x 32 tile chunks, the live matrix
static int Pr4_ZoneAt(int x, int z) {
    MapMatrix *mm = sPr4Fs != NULL ? sPr4Fs->mapMatrix : NULL;

    if (mm == NULL || x < 0 || z < 0 || x / 32 >= MapMatrix_GetWidth(mm) || z / 32 >= MapMatrix_GetHeight(mm)) {
        return -1;
    }
    return MapMatrix_GetMapHeader(mm, x / 32, z / 32);
}

static int Pr4_Attr(int x, int z, u16 *a) {
    if (x < 0 || z < 0 || Pr4_ZoneAt(x, z) < 0) {
        return 0;
    }
    return ((const Pr4Terrain *)sPr4Fs->unk60)->getTileAttributes(sPr4Fs, x, z, a) ? 1 : 0;
}

static u16 sPr4A[PORT_MAP_N][PORT_MAP_N];
static u8 sPr4Have[PORT_MAP_N][PORT_MAP_N];
static fx32 sPr4H[PORT_MAP_N][PORT_MAP_N];
static u8 sPr4HOk[PORT_MAP_N][PORT_MAP_N];   // the tile's height is known (its chunk's terrain is loaded)

// a warp tile's PortMapWarp.dir (the facing arriving there + 1: 0 up, 1 down, 2 left, 3 right; 0 none): Platinum's mapping
static int Pr4_WarpDir(u8 b) {
    if (MetatileBehavior_IsWarpEntranceSouth(b) || MetatileBehavior_IsWarpSouth(b)) {
        return 0 + 1;   // press south
    }
    if (MetatileBehavior_IsWarpEntranceNorth(b) || MetatileBehavior_IsWarpNorth(b)) {
        return 1 + 1;
    }
    if (MetatileBehavior_IsWarpEntranceEast(b) || MetatileBehavior_IsWarpEast(b) || MetatileBehavior_IsWarpStairsEast(b)) {
        return 2 + 1;
    }
    if (MetatileBehavior_IsWarpEntranceWest(b) || MetatileBehavior_IsWarpWest(b) || MetatileBehavior_IsWarpStairsWest(b)) {
        return 3 + 1;
    }
    if (MetatileBehavior_IsLadderNorth(b)) {   // a ladder up: on it, the key north (CheckMapTransition)
        return 1 + 1;
    }
    if (MetatileBehavior_IsLadderSouth(b)) {
        return 0 + 1;
    }
    return 0;
}

static void Pr4_Fill(PortMap *m) {
    static const int dx[4] = {0, 0, -1, 1}, dz[4] = {-1, 1, 0, 0};
    FieldSystem *fs = sPr4Fs;
    const Pr4Terrain *tc;
    PlayerAvatar *pa;
    LocalMapObject *self, *obj;
    const VecFx32 *ppos;
    fx32 py;
    int x, z, d, i;
    s32 idx;

    memset(m, 0, sizeof *m);
    if (fs == NULL || !Pr4_FieldUp(fs)) {
        return;
    }
    tc = (const Pr4Terrain *)fs->unk60;
    pa = fs->playerAvatar;
    self = PlayerAvatar_GetMapObject(pa);
    ppos = PlayerAvatar_GetPositionVector(pa);
    py = ppos->y;
    m->px = m->wx = (int)PlayerAvatar_GetXCoord(pa);
    m->pz = m->wz = (int)PlayerAvatar_GetZCoord(pa);
    m->py = py >> 16;
    d = (int)PlayerAvatar_GetFacingDirection(pa);
    m->pdir = d >= 0 && d <= 3 ? d : -1;
    m->x0 = m->px - PORT_MAP_N / 2;
    m->z0 = m->pz - PORT_MAP_N / 2;
    m->zone = (unsigned)fs->location->mapId;
    m->mw = MapMatrix_GetWidth(fs->mapMatrix);
    m->mh = MapMatrix_GetHeight(fs->mapMatrix);
    m->surf = PlayerAvatar_GetState(pa) == PLAYER_STATE_SURFING;   // the water is walkable while the player surfs (route_map.c)
    m->rail = 0;
    m->nline = m->nlever = m->nplace = m->narm = 0;
    m->wind = m->gust = m->windHold = 0;
    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            int gx = m->x0 + x, gz = m->z0 + z;

            u8 hsrc = 1;   // CALCULATED_HEIGHT_SOURCE_NONE (0): no height data (yet) for the tile

            sPr4Have[z][x] = (u8)Pr4_Attr(gx, gz, &sPr4A[z][x]);
            sPr4H[z][x] = sPr4Have[z][x] ? tc->getHeight(fs, py, gx * PR4_TILE + PR4_TILE / 2, gz * PR4_TILE + PR4_TILE / 2, &hsrc) : 0;
            sPr4HOk[z][x] = hsrc != 0;   // a chunk whose attributes are in and whose terrain heights are not (the next one, loading)
                                         // reads height 0: no cliff test against it (it made Route 32's way south a 48-unit wall)
        }
    }
    for (z = 0; z < PORT_MAP_N; z++) {
        for (x = 0; x < PORT_MAP_N; x++) {
            PortMapCell *c = &m->cell[z][x];
            int gx = m->x0 + x, gz = m->z0 + z, zone = Pr4_ZoneAt(gx, gz);
            u16 a = sPr4A[z][x];
            u8 b = (u8)(a & 0xff);
            int coll = (a >> 15) & 1;

            memset(c, 0, sizeof *c);
            c->zone = zone < 0 ? 0xffff : (u16)zone;
            c->cls = 0xff;
            if (!sPr4Have[z][x]) {
                c->kind = zone < 0 ? ' ' : '?';
                c->pass = c->kind == '?' ? 15 : 0;
                continue;
            }
            c->cls = b;
            c->y = (short)(sPr4H[z][x] >> 16);
            c->yf = (short)(sPr4H[z][x] >> FX32_SHIFT);
            c->kind = MetatileBehavior_IsJumpNorth(b)   ? '^'
                      : MetatileBehavior_IsJumpSouth(b) ? 'v'
                      : MetatileBehavior_IsJumpWest(b)  ? '<'
                      : MetatileBehavior_IsJumpEast(b)  ? '>'
                      : MetatileBehavior_IsWaterfall(b) ? 'F'   // a waterfall wall: F:waterfall climbs it from the water in front (the player surfs)
                      : MetatileBehavior_IsWhirlpool(b) ? 'W'   // a whirlpool: F:whirlpool crosses it from the water in front (std_field_whirlpool)
                      : coll                            ? '#'
                      : MetatileBehavior_IsSurfableWater(b) ? '~'
                      : MetatileBehavior_IsTallGrass(b) || MetatileBehavior_IsVeryTallGrass(b) ? ','
                                                                                                : '.';
            if (MetatileBehavior_IsRockClimbNorthSouth(b)) {
                c->flags |= PORT_MAP_CLIMBNS;   // a Rock Climb wall: F:climb (route_fieldmove.h) faces it, A, Yes
            } else if (MetatileBehavior_IsRockClimbEastWest(b)) {
                c->flags |= PORT_MAP_CLIMBEW;
            }
            if (c->kind == 'F') {
                c->flags |= PORT_MAP_FALL;
            }
            if (MetatileBehavior_IsIce(b)) {
                fx32 h0 = sPr4H[z][x];

                // sliding ice (the tile-move table _020FCB88 -> sub_0205D0A8; Mahogany's gym, the Ice Path): the planner's slide
                // edge (route_slide.c PortRouteSlide_End), as Platinum's. The speed the slide has left: sub_0205D240 reads the
                // height a quarter tile on (no height there: no change), sub_0205D2A0 slows (up) or speeds (down) the slide.
                c->slide = PORT_MAP_ICE;
                for (d = 0; d < 4; d++) {
                    u8 qsrc = 1;
                    fx32 h1 = tc->getHeight(fs, py, (gx * 16 + 8 + dx[d] * 4) * FX32_ONE, (gz * 16 + 8 + dz[d] * 4) * FX32_ONE, &qsrc);

                    c->ihc |= (unsigned char)((qsrc == 0 ? 0 : h1 > h0 ? 1 : h1 < h0 ? 2 : 0) << (2 * d));
                }
            }
            if (c->kind != '.' && c->kind != ',' && c->kind != '~') {
                continue;
            }
            for (d = 0; d < 4; d++) {
                int nx = x + dx[d], nz = z + dz[d];
                u16 na;
                u8 nb;
                fx32 dh;

                if (nx < 0 || nz < 0 || nx >= PORT_MAP_N || nz >= PORT_MAP_N) {
                    continue;
                }
                if (!sPr4Have[nz][nx]) {
                    c->pass |= Pr4_ZoneAt(m->x0 + nx, m->z0 + nz) >= 0 ? (1 << d) : 0;   // not loaded: may be open
                    continue;
                }
                na = sPr4A[nz][nx];
                nb = (u8)(na & 0xff);
                dh = sPr4H[nz][nx] - sPr4H[z][x];
                if ((na >> 15) & 1 || nb == GetMetatileBehavior_None() || _020FD4CC[d](b) || _020FD4BC[d](nb) ||
                    (sPr4HOk[z][x] && sPr4HOk[nz][nx] && (dh >= 20 * FX32_ONE || dh <= -20 * FX32_ONE))) {
                    continue;
                }
                c->pass |= (unsigned char)(1 << d);
            }
        }
    }
    PortMap_OpenUnloaded(m);
    // the zone's events (MapEvents: tile units already)
    m->nwarp = m->ntrig = m->nbg = m->nnpc = 0;
    {
        const MapEvents *ev = fs->mapEvents;

        for (i = 0; ev->warp_events != NULL && i < (int)ev->num_warp_events && m->nwarp < 64; i++) {
            const WarpEvent *w = &ev->warp_events[i];
            PortMapWarp *o = &m->warp[m->nwarp++];
            u16 wa = 0;

            memset(o, 0, sizeof *o);
            o->x = w->x;
            o->z = w->z;
            o->w = o->h = 1;
            o->zone = w->header;
            o->dest = w->anchor;
            o->dir = Pr4_WarpDir(Pr4_Attr(w->x, w->z, &wa) ? (u8)(wa & 0xff) : 0xff);
            // a mat, ladder or stairs that needs the key held on it: type 7 (route_map.c: on it, or the step from the one side
            // that keeps the press); a door or a tile with no behavior: 0, the Gen 5 "any side" class
            o->type = o->dir != 0 ? 7 : 0;
        }
        for (i = 0; ev->coord_events != NULL && i < (int)ev->num_coord_events && m->ntrig < 64; i++) {
            const CoordEvent *g = &ev->coord_events[i];
            PortMapTrig *o = &m->trig[m->ntrig++];

            memset(o, 0, sizeof *o);
            o->x = g->x;
            o->z = g->z;
            o->w = g->w;
            o->h = g->h;
            o->script = g->scriptId;
            o->workId = g->var;
            o->workValue = g->val;
            // live (sub_0203DE04's own test: the work's value is the trigger's); kept only for a hazard field below
            o->avoid = FieldSystem_VarGet(fs, g->var) == g->val;
        }
        // A live trigger is walked round (route_map.c's Search, unless it is the goal) only when another trigger of the zone
        // runs the same script: a field of hazards, one rectangle per pit / hole / fall tile (Ecruteak gym: 15 rectangles of
        // script 3). A story scene is a rectangle with its own script that the route must cross (Cherrygrove's rival and
        // north-exit scenes, Route 30's apricorn scene): avoiding those made the chains' r30-in, apricorn and rival walk round
        // them and time out (fixrel1).
        for (i = 0; i < m->ntrig; i++) {
            int j, twin = 0;

            for (j = 0; j < m->ntrig && !twin; j++) {
                twin = j != i && m->trig[j].script == m->trig[i].script;
            }
            m->trig[i].avoid = m->trig[i].avoid && twin;
        }
        for (i = 0; ev->bg_events != NULL && i < (int)ev->num_bg_events && m->nbg < 64; i++) {
            const BgEvent *g = &ev->bg_events[i];
            PortMapBg *o = &m->bg[m->nbg++];

            memset(o, 0, sizeof *o);
            o->x = g->x;
            o->z = g->z;
            o->script = g->scriptId;
            o->dir = 4;   // any side (BgEvent.dir's coding not mapped yet)
            o->cond = g->type;
        }
    }
    idx = 0;
    while (fs->mapObjectManager != NULL && m->nnpc < 64 &&
           MapObjectManager_GetNextObjectWithFlagFromIndex(fs->mapObjectManager, &obj, &idx, MAPOBJECTFLAG_ACTIVE)) {
        PortMapNpc *o;

        if (obj == self) {
            continue;
        }
        o = &m->npc[m->nnpc++];
        memset(o, 0, sizeof *o);
        o->id = (int)MapObject_GetID(obj);
        o->x = (int)MapObject_GetXCoord(obj);
        o->z = (int)MapObject_GetZCoord(obj);
        o->x0 = (int)MapObject_GetInitialX(obj);
        o->z0 = (int)MapObject_GetInitialZ(obj);
        o->code = (int)MapObject_GetSpriteID(obj);
        o->move = (int)MapObject_GetMovement(obj);
        o->nocoll = MapObject_GetFlagsBitsMask(obj, MAPOBJECTFLAG_UNK18) ? 1 : 0;   // Platinum's MAP_OBJ_STATUS_18 bit
    }
    m->ok = 1;
}

static const PortRouteAdapter kPortRouteGen4 = {"gen4", Pr4_Fill, Pr4_ZoneAt};

// The pilot's S, Py and Pn. The field menu is the touch screen's icons (overlay 27): FieldSystem_ShouldDrawStartMenuIcon
// says which of the 7 places (START_MENU_ICON_*: two columns, 0-3 and 4-6) are shown, fs->unkD3 is the cursor as an index
// among the shown ones, and a key moves it by overlay 27's table (ov27_0225B360: per place and key, the first shown of 3
// candidates; the columns wrap). gPortRouteMenu gives the grid (slot = column + 2 * row) and, since the moves skip hidden
// icons and wrap, `key`: the first key of the shortest way to SAVE on that table (route_test.c MenuDir takes it as is).
// The save is the touch save app (touch_save_app.c, the bottom screen; its questions take A): SAVE chosen makes the state 0,
// its write (Field_SaveGameNormal) 11, the menu taking keys again -1 (sub_0203D288, in the frame it switches back: an A
// the pilot presses at the last message can not reach the menu and save again).
static int sPr4Save = -1;

static int Pr4_MenuSlot(int icon) {
    return icon < 0 ? -1 : icon < 4 ? 2 * icon : 2 * (icon - 4) + 1;
}

static int Pr4_MenuMove(int from, int key, const u8 *shown) {
    int c;

    for (c = 0; c < 3; c++) {
        int n = ov27_0225D0B4[from * 12 + key * 3 + c];

        if (n < 7 && shown[n]) {
            return n;
        }
    }
    return -1;
}

static unsigned Pr4_MenuKey(int from, int to, const u8 *shown) {
    static const unsigned kKey[4] = {0x40u, 0x80u, 0x20u, 0x10u};   // the table's order: up, down, left, right
    int first[7], q[7], h = 0, t = 0, i, k;

    if (from < 0 || to < 0 || from == to || !shown[to]) {
        return 0;
    }
    for (i = 0; i < 7; i++) {
        first[i] = -1;
    }
    first[from] = 4;
    q[t++] = from;
    while (h < t) {
        int p = q[h++];

        for (k = 0; k < 4; k++) {
            int n = Pr4_MenuMove(p, k, shown);

            if (n < 0 || n == p || first[n] >= 0) {
                continue;
            }
            first[n] = p == from ? k : first[p];
            if (n == to) {
                return kKey[first[n]];
            }
            q[t++] = n;
        }
    }
    return 0;
}

// start_menu.c Task_StartMenu_HandleInput, each frame the menu reads keys
void PortRoute4_StartMenu(FieldSystem *fs) {
    PortRouteMenu *m = &gPortRouteMenu;
    u8 shown[7];
    int i, k = 0, cur = -1;

    for (i = 0; i < 7; i++) {
        shown[i] = FieldSystem_ShouldDrawStartMenuIcon(fs, (StartMenuIcon)i) ? 1 : 0;
        if (shown[i] && k++ == (int)fs->unkD3) {
            cur = i;
        }
    }
    m->input = 1;
    m->grid = 1;
    m->slot = Pr4_MenuSlot(cur);
    m->saveSlot = shown[START_MENU_ICON_SAVE] ? Pr4_MenuSlot(START_MENU_ICON_SAVE) : -1;
    m->pokeSlot = shown[START_MENU_ICON_POKEMON] ? Pr4_MenuSlot(START_MENU_ICON_POKEMON) : -1;   // F:fly
    m->pokeKey = Pr4_MenuKey(cur, START_MENU_ICON_POKEMON, shown);
    m->bagSlot = -1;   // (F:teach: not yet)
    m->onSave = cur == START_MENU_ICON_SAVE;
    m->key = Pr4_MenuKey(cur, START_MENU_ICON_SAVE, shown);
    m->vblank = OS_GetVBlankCount();
}

// start_menu.c Task_StartMenu_HandleSelection_Save: the touch save app starts
void PortRoute4_SaveStart(void) {
    sPr4Save = 0;
}

// start_menu.c sub_0203D288: the save app is gone, the menu reads keys from the next frame
void PortRoute4_SaveEnd(void) {
    sPr4Save = -1;
}

// scrcmd_c.c sub_020416E4 (a script's yes/no): the cursor's row each frame it waits, -1 once it has the answer
void PortRoute4_YesNo(int row) {
    gPortRouteFieldYesNo = row < 0 ? 0 : 1 + (row & 1);
}

// A script's menu (Pl:N, port/src/route_test.c ListStep): pokeheartgold-route.patch's hunks in overlay 1's field menu tasks
// (ScrCmd_MenuExec's 2D menu ov01_021EDE8C, the scrolling list ov01_021EE49C: the shrine's quiz, the elevators' floors) call this
// each frame they read keys (kind 1 the 2D menu, 2 the list) and at the answer (kind 0, the value the script gets, -2 for B).
// The rows are the MenuItemAdd order; a 2D menu of columns fills them top to bottom, column by column (list_menu_2d.c
// TryMove2dMenuCursorInternal): gPortRouteListHigh.
extern int gPortRouteList, gPortRouteListCount, gPortRouteListRow, gPortRouteListResult, gPortRouteListHigh;

void PortRoute4_FieldMenu(const FieldMenuManager *menuManager, int kind, int answer) {
    if (kind == 0) {
        gPortRouteList = 0;
        gPortRouteListResult = answer;
        return;
    }
    gPortRouteList = 1;
    if (kind == 1) {
        gPortRouteListCount = menuManager->optionsCount;   // the MenuItemAdd count
        gPortRouteListRow = Get2dMenuSelection(menuManager->menu);
        gPortRouteListHigh = menuManager->menuTemplate.itemsWide > 1 ? menuManager->menuTemplate.itemsHigh : 0;
    } else {
        gPortRouteListCount = menuManager->listMenuTemplate.totalItems;
        gPortRouteListRow = menuManager->cursorPos;
        gPortRouteListHigh = 0;
    }
}

// touch_save_app.c, after Field_SaveGameNormal (the write, synchronous)
void PortRoute4_Saved(void) {
    if (sPr4Save >= 0) {
        sPr4Save = 11;
    }
}

int PortRoute4_NoEncounter(void) {
    return PortRoute_NoEncounter(Pr4_Game());
}

// F:fly (port/src/route_fly.h) on HeartGold / SoulSilver. The Fly user: the first party member with Fly in a move slot (the
// party menu lists real moves only; no cheat entry), each field frame.
static void Pr4_FlySlot(FieldSystem *fs) {
    Party *party = SaveArray_Party_Get(fs->saveData);
    int n = Party_GetCount(party), i, m;

    gPortRouteFlySlot = gPortRouteFlyMove = -1;
    for (i = 0; i < n && gPortRouteFlySlot < 0; i++) {
        Pokemon *mon = Party_GetMonByIndex(party, i);

        if (GetMonData(mon, MON_DATA_IS_EGG, NULL)) {
            continue;
        }
        for (m = 0; m < MAX_MON_MOVES; m++) {
            if (GetMonData(mon, MON_DATA_MOVE1 + m, NULL) == MOVE_FLY) {
                gPortRouteFlySlot = i;
                gPortRouteFlyMove = m;
                break;
            }
        }
    }
}

// party_menu.c (pokeheartgold-route.patch): each frame the list takes keys (contextMenu 0: the cursor is the party slot, the grid
// slot = column + 2 * row as Platinum's) and each frame of a member's context menu (1: its entries' action ids, Fly's as 16 + its
// move slot, route_fly.h's numbering; the cursor's entry).
void PortRoute4_PartyMenu(PartyMenu *pm, int contextMenu) {
    PortRoutePList *l = &gPortRoutePList;

    if (!contextMenu) {
        l->vblank = OS_GetVBlankCount();
        l->state = 2;
        l->subState = 1;
        l->cursorPos = pm->partyMonIndex;
        l->mode = pm->args->context;
    } else {
        PartyMenuContextMenuCursor *c = pm->contextMenuCursor;
        int i;

        l->menuCount = c->numItems < 8 ? c->numItems : 8;
        for (i = 0; i < l->menuCount; i++) {
            int a = (int)c->menu.items[i].value;

            // the other actions keep their ids, moved out of 16-19 (route_fly.h's move slots)
            l->menuItems[i] = a == PARTY_MON_CONTEXT_MENU_FLY ? 16 + (gPortRouteFlyMove < 0 ? 0 : gPortRouteFlyMove)
                            : a >= 16 && a < 20                ? 100 + a
                                                               : a;
        }
        l->menuCursor = c->selection;
        l->menuVblank = OS_GetVBlankCount();

    }
}

// fly_map.c (the Pokegear's map in its fly mode), each frame it takes input and each frame of a flypoint's context menu (A takes
// its first entry, the flight). A grid: the cursor is the cell playerX, playerY - 2 (FlyMap_HandleKeyInput's), a key moves it one
// cell after the last move's animation (moving); the places are gMapFlypointParams (x, y, width, height cells; mapIDforWarp the
// zone), flyable when the game's own PokegearMap_GetFlyDestinationAtCoord answers it.
void PortRoute4_FlyMap(PokegearMapAppData *mapApp) {
    PortRouteTownMap *t = &gPortRouteTownMap;
    int i, cx = mapApp->playerX, cy = mapApp->playerY - 2;

    t->vblank = OS_GetVBlankCount();
    t->alive = 1;
    t->grid = 1;
    t->mode = mapApp->type;
    t->result = 0;
    t->zoomed = mapApp->zoomed;
    t->moving = mapApp->draggingMarking || mapApp->unk_139_2;
    t->zone = mapApp->mapID;
    t->cx = cx;
    t->cy = cy;
    t->sx = t->sy = 0;
    t->hit = t->ring = t->selected = -1;
    t->count = PGMAP_NUM_FLYPOINTS;
    for (i = 0; i < PGMAP_NUM_FLYPOINTS; i++) {
        const MapFlypointParam *f = &gMapFlypointParams[i];

        t->p[i].zone = f->mapIDforWarp;
        t->p[i].x = f->x;
        t->p[i].y = f->y;
        t->p[i].hidden = 0;
        t->p[i].canFly = PokegearMap_GetFlyDestinationAtCoord(mapApp, f->x, f->y) == f->mapIDforWarp;
        if (t->hit < 0 && t->p[i].canFly && cx >= f->x && cx < f->x + f->width && cy >= f->y && cy < f->y + f->height) {
            t->hit = t->ring = i;
        }
    }
}

// fly_map.c FlyMap_Exit: the map is over (a pick: flySelected, the map it flies to)
void PortRoute4_TownMapEnd(int flySelected, int mapId) {
    gPortRouteTownMap.vblank = OS_GetVBlankCount();
    gPortRouteTownMap.alive = 0;
    gPortRouteTownMap.result = flySelected ? 2 : 0;   // 2 = TOWNMAP_RESULT_FLY, as Gen 5's
    gPortRouteTownMap.flyZone = mapId;
}

// The field moves the scripts and the field's checks ask the party for (scrcmd_party.c ScrCmd_GetPartySlotWithMove: the std scripts of
// Cut, Rock Smash, Strength, Rock Climb, Surf, Waterfall, Whirlpool; script_pokemon_util.c GetIdxOfFirstPartyMonWithMove: the field's
// Surf and Waterfall checks, field_control.c; pokeheartgold-route.patch): with the test cheat on (PortRoute_CheatWin) slot 0 "knows"
// them, as Platinum's (same list and the same F: tokens; HeartGold / SoulSilver have Whirlpool and no Defog). The badge checks stay real.
int PortRoute4_CheatFieldMove(int move) {
    switch (move) {
    case MOVE_WHIRLPOOL:
    case MOVE_ROCK_CLIMB:
        return PortRoute_CheatWin(Pr4_Game());
    default:
        return PortRoute_CheatFieldMove(Pr4_Game(), (unsigned)move);
    }
}

// The battle pilot (pokeheartgold-route.patch's overlay 12 hunks: the player's tasks of ov12_02260EA4's default set, the
// same tasks as Platinum's battle_display.c): ov12_0225DAD4 the action menu (state 5) -> kind 0, ov12_0225E250 the move menu
// (state 1) -> kind 1 and its answer through PortRoute4_BattleMove, ov12_0225FA44 the yes/no (state 2) -> kind 2 (arg the
// yesNoType, -1 at the answer), ov12_022609F8 the forget-a-move screen (state 2) -> kind 3. Each menu also takes keys
// (battle_input.c BattleInput_CheckTouch -> BattleInput_CheckCursorInput), so route_test.c's event 2 drives them (the battle
// overlay 12 is loaded; FS_OVERLAY_ID gives the real ids on HG). The answers and the move rule are Platinum's.
extern int gPortRouteYesNo;   // port/src/route_test.c: 1 answer with A, 2 with B

static void Pr4_IntBattle(int kind);   // the interaction sweep (below)

void PortRoute4_BattleMenu(int kind, int arg) {
    static int sLast = -2;

    if (kind == 0) {
        Pr4_IntBattle(0);
    }

    if (kind == 0) {
        gPortRouteBattle.menuVblank = OS_GetVBlankCount();
    } else if (kind == 2) {   // No (B) to 0 forfeit, 1 forget a move, 4 switch, 5 nickname; Yes (A) to 2 give up the move, 3 next mon
        gPortRouteYesNo = arg == 2 || arg == 3 ? 1 : 2;
    } else if (kind == 3) {
        gPortRouteYesNo = 2;
    } else if (kind < 0) {
        gPortRouteYesNo = 0;
    }
    if (getenv("BWPORT_PILOT") != NULL && kind != sLast) {
        fprintf(stderr, "pilot: battle menu %d arg %d at V-blank %u\n", kind, arg, (unsigned)OS_GetVBlankCount());
    }
    sLast = kind;
}

// the move menu's answer (1..4 the slot + 1, else a cancel): PortRoute_BestMove on CalculateTypeEffectiveness (x40) x power
static int Pr4_TestSlot(int input, const u16 *moves);

int PortRoute4_BattleMove(BattleSystem *battleSys, int battler, int input, const u16 *moves, const u8 *ppCur, int invalidMoves) {
    BattleContext *ctx = BattleSystem_GetBattleContext(battleSys);
    int eff[4], power[4], usable[4], n = 0, i, target = -1, slot;

    if (input >= 1 && input <= 4) {
        Pr4_IntBattle(100);
    }
    if (input >= 1 && input <= 4 && getenv("BWPORT_HG_TESTLIST") != NULL) {
        extern int PortRoute4_TestList(BattleSystem *, int);   // below (the sweep's case list)

        return PortRoute4_TestList(battleSys, battler);
    }
    if (input >= 1 && input <= 4 && getenv("BWPORT_HG_TESTSLOT") != NULL) {
        return Pr4_TestSlot(input, moves);
    }
    if (getenv("BWPORT_PILOT") == NULL || input < 1 || input > 4) {
        return input;
    }
    for (i = 0; i < BattleSystem_GetMaxBattlers(battleSys); i++) {
        if (BattleSystem_GetFieldSide(battleSys, i) != BattleSystem_GetFieldSide(battleSys, battler) &&
            BattleMon_Get(ctx, i, BMON_DATA_HP, NULL) > 0) {
            target = i;
            break;
        }
    }
    if (target < 0) {
        return input;
    }
    for (i = 0; i < 4 && moves[i] != 0; i++) {
        int pw = (int)GetMoveAttr(moves[i], MOVEATTR_POWER);
        int mul = CalculateTypeEffectiveness((u8)GetMoveAttr(moves[i], MOVEATTR_TYPE), (u8)BattleMon_Get(ctx, target, BMON_DATA_TYPE_1, NULL),
                                             (u8)BattleMon_Get(ctx, target, BMON_DATA_TYPE_2, NULL));

        eff[i] = pw == 0 || mul == 0 ? 0 : mul <= 10 ? 1 : mul <= 20 ? 2 : mul <= 40 ? 3 : mul <= 80 ? 4 : 5;
        power[i] = pw;
        usable[i] = ppCur[i] != 0 && (invalidMoves & (1 << i)) == 0;
        n++;
    }
    slot = PortRoute_BestMove(n, eff, power, usable, input - 1);
    if (slot >= 0 && slot != input - 1) {
        fprintf(stderr, "pilot: battle move: slot %d (move %u) for slot %d (move %u) at V-blank %u\n", slot, (unsigned)moves[slot],
                input - 1, (unsigned)moves[input - 1], (unsigned)OS_GetVBlankCount());
        return slot + 1;
    }
    if (slot < 0) {
        fprintf(stderr, "pilot: battle move: no usable move hurts battler %d: slot %d stays\n", target, input - 1);
    }
    return input;
}

// The test cheats (docs/platinum.md "Test cheats"; the same switches): side 0 (BattleSystem_GetFieldSide) is the player's.
// battle_controller_player.c: ov12_0224B1FC (the PP use) and BattleSystem_CheckMoveHit (FALSE = hits) ask CheatSide,
// BattleControllerPlayer_HpCalc (after the OHKO line) CheatDamage; battle_command.c BtlCmd_UpdateHealthbar /
// BtlCmd_UpdateHealthbarValue CheatHpCalc (the HP a script takes outside a hit: burn, poison, weather, recoil; Platinum's comment);
// overlay_12_0224E4FC.c CheckSortSpeed CheatSpeed; field_control.c FieldSystem_CheckWildEncounter (after the Pal Park branch) NoEncounter.
// BWPORT_HG_TESTLIST (the battle sweep, Pr4_TestList below): both sides always hit and use no PP, no hit takes more than an eighth
// of the defender's HP and no script loss (recoil, a self-KO) its last one: neither Pokemon faints.
static int Pr4_SweepOn(void) {
    return getenv("BWPORT_HG_TESTLIST") != NULL;
}

int PortRoute4_CheatSide(BattleSystem *battleSys, int battler) {
    if (Pr4_SweepOn()) {
        BattleContext *ctx = BattleSystem_GetBattleContext(battleSys);

        fprintf(stderr, "sweep: exec battler %d move %u at V-blank %u\n", battler, (unsigned)ctx->moveNoCur, (unsigned)OS_GetVBlankCount());
        return 1;
    }
    return (PortRoute_CheatWin(Pr4_Game()) || getenv("BWPORT_HG_TESTHIT") != NULL) && BattleSystem_GetFieldSide(battleSys, battler) == 0;
}

void PortRoute4_CheatSpeed(BattleSystem *battleSys, int battler1, int battler2, u32 *speed1, u32 *speed2) {
    if (!PortRoute_CheatWin(Pr4_Game())) {
        return;
    }
    *speed1 = BattleSystem_GetFieldSide(battleSys, battler1) == 0 ? *speed1 * 32 : *speed1 / 10;
    *speed2 = BattleSystem_GetFieldSide(battleSys, battler2) == 0 ? *speed2 * 32 : *speed2 / 10;
}

int PortRoute4_CheatDamage(BattleSystem *battleSys, int attacker, int defender, int damage, int defenderHP) {
    int n = damage;

    if (Pr4_SweepOn()) {   // a loss is negative: at most an eighth of the defender's HP (at least 1)
        int cap = defenderHP / 8 > 0 ? defenderHP / 8 : 1;

        return damage < 0 && -damage > cap ? -cap : damage;
    }
    if (damage == 0 || !PortRoute_CheatWin(Pr4_Game())) {
        return damage;
    }
    if (BattleSystem_GetFieldSide(battleSys, defender) == 0) {
        n = 0;
    } else if (BattleSystem_GetFieldSide(battleSys, attacker) == 0) {
        n = -(defenderHP > 0 ? defenderHP : 1);
    }
    fprintf(stderr, "cheat: damage atk %d def %d %d -> %d (V-blank %u)\n", attacker, defender, damage, n, (unsigned)OS_GetVBlankCount());
    return n;
}

// The HP change a battle script applies to `battler` (hpCalc: < 0 a loss): with the cheat on, a loss of the player's side is 0.
int PortRoute4_CheatHpCalc(BattleSystem *battleSys, int battler, int hpCalc) {
    if (Pr4_SweepOn()) {   // a loss never takes the last HP
        int hp = BattleSystem_GetBattleContext(battleSys)->battleMons[battler].hp;

        return hpCalc < 0 && -hpCalc >= hp ? (hp > 1 ? 1 - hp : 0) : hpCalc;
    }
    if (hpCalc >= 0 || !PortRoute_CheatWin(Pr4_Game()) || BattleSystem_GetFieldSide(battleSys, battler) != 0) {
        return hpCalc;
    }
    fprintf(stderr, "cheat: script HP battler %d %d -> 0 (V-blank %u)\n", battler, hpCalc, (unsigned)OS_GetVBlankCount());
    return 0;
}

// BWPORT_HG_TESTBATTLE=SPECIES:LEVEL[:M1,M2,M3,M4[:OPPSPECIES[/OPPLEVEL][:O1,O2,O3,O4]]] (test only, a throwaway run: never
// save it to the save library; tools/hg_battle_sweep.py): at the first free field frame a party of none gets one Pokemon of that
// species and level (GiveMon, as the script's give command), then the field's own wild battle starts (sub_02050B08, a grass
// encounter's path) against OPPSPECIES at OPPLEVEL (default: the same species and level). A move list sets the moves of the
// party's first Pokemon / the wild one (MonSetMoveInSlot: the move and its PP; 0 empties a slot; no list: the learnset's).
// BWPORT_HG_TESTSLOT=N (1-4) answers every move menu with slot N, =stop ends the test there (with TESTEXIT), =cycle with the next slot that has a move each turn
// (1, 2, 3, 4, 1 ...; port/game's PortRoute4_BattleMove; the A pulse opens the menu). BWPORT_HG_TESTHIT: our moves always
// hit (CheatSide without the rest of BWPORT_TESTCHEAT). BWPORT_HG_TESTEXIT: the exe exits 0 when the field is free again
// after the battle ("test: battle over"), and in cycle mode at the menu after each move was chosen once ("test: moves done").
// Every chosen move is logged ("test: move slot S move M turn T"), all lines on stderr.
static int sPr4TestState;   // 0 none, 1 the battle asked, 2 the field busy with it (or the battle), 3 over

static int Pr4_TestMoves(const char *p, u16 *moves) {   // "M1,M2,M3,M4" (fewer: the rest 0); returns the chars read
    const char *p0 = p;
    int i, v, n;

    for (i = 0; i < 4; i++) {
        moves[i] = 0;
    }
    for (i = 0; i < 4 && sscanf(p, "%d%n", &v, &n) == 1; i++) {
        moves[i] = (u16)v;
        p += n;
        if (*p != ',') {
            break;
        }
        p++;
    }
    return (int)(p - p0);
}

static void Pr4_TestSetMoves(Pokemon *mon, const u16 *moves) {
    int i;

    for (i = 0; i < 4; i++) {
        MonSetMoveInSlot(mon, moves[i], (u8)i);
    }
}

static int Pr4_TestSlot(int input, const u16 *moves) {
    static int sTurn, sNext;
    const char *e = getenv("BWPORT_HG_TESTSLOT");
    int slot = input - 1, n = 0, i;

    for (i = 0; i < 4; i++) {
        n += moves[i] != 0;
    }
    if (strcmp(e, "stop") == 0) {   // the send-out sweep: the first move menu is the end
        fprintf(stderr, "test: moves done (0) at V-blank %u\n", (unsigned)OS_GetVBlankCount());
        if (getenv("BWPORT_HG_TESTEXIT") != NULL) {
            _exit(0);
        }
    } else if (strcmp(e, "cycle") == 0) {
        if (sNext >= 4 || moves[sNext] == 0) {   // each move once: the turn after the last
            fprintf(stderr, "test: moves done (%d) at V-blank %u\n", sNext, (unsigned)OS_GetVBlankCount());
            if (getenv("BWPORT_HG_TESTEXIT") != NULL) {
                _exit(0);
            }
            sNext = 0;
        }
        slot = sNext++;
    } else if (atoi(e) >= 1 && atoi(e) <= 4 && moves[atoi(e) - 1] != 0) {
        slot = atoi(e) - 1;
    }
    fprintf(stderr, "test: move slot %d move %u turn %d at V-blank %u\n", slot + 1, (unsigned)moves[slot], ++sTurn,
            (unsigned)OS_GetVBlankCount());
    (void)n;
    return slot + 1;
}

extern void PORT_SweepCase(int k, const char *what);   // port/src/run_frames.c (the sweep's case lines)
extern void PORT_SweepEnd(const char *why, int exitNow);

// BWPORT_HG_TESTLIST=O:F,O:F,... (the battle sweep, tools/battle_sweep.py; needs BWPORT_HG_TESTBATTLE): one case per turn. At each
// move menu the next pair is applied to the battle's own Pokemon (the BattleMons, as Transform or Mimic do): our first slot knows move
// O, every slot of the foe move F (so the AI's pick is F), all PP 35, HP full, status cleared. The menu's answer is slot 1.
// (The wild foe's AI picks after our menu's answer: F applies to the same turn.) Past the last pair: `sweep: end list`, exit 0
// with BWPORT_HG_TESTEXIT. The first foe move is the battle's own spec. A battle that ends earlier
// (a blown-away wild Pokemon) prints `sweep: end battle` from Pr4_TestFrame; the harness resumes after that case.
static u16 sListOur[1024], sListFoe[1024];
static int sListN = -1, sListPos;

static void Pr4_ListParse(void) {
    const char *p = getenv("BWPORT_HG_TESTLIST");
    int o, f, n;

    sListN = 0;
    while (p != NULL && sListN < 1024 && sscanf(p, "%d:%d%n", &o, &f, &n) == 2) {
        sListOur[sListN] = (u16)o;
        sListFoe[sListN++] = (u16)f;
        p += n;
        if (*p == ',') {
            p++;
        }
    }
}

int PortRoute4_TestList(BattleSystem *battleSys, int battler) {
    BattleContext *ctx = BattleSystem_GetBattleContext(battleSys);
    int k, b, i;
    char what[48];

    if (sListN < 0) {
        Pr4_ListParse();
    }
    k = sListPos++;
    if (k >= sListN) {
        PORT_SweepEnd("list", getenv("BWPORT_HG_TESTEXIT") != NULL);
        return 1;
    }
    for (b = 0; b < BattleSystem_GetMaxBattlers(battleSys); b++) {
        BattleMon *m = &ctx->battleMons[b];

        m->hp = (s32)m->maxHp;
        m->status = 0;
        m->status2 = 0;
        if (BattleSystem_GetFieldSide(battleSys, b) != BattleSystem_GetFieldSide(battleSys, battler)) {
            for (i = 0; i < 4; i++) {
                m->moves[i] = sListFoe[k];
                m->movePPCur[i] = m->movePP[i] = 35;
            }
        } else if (b == battler) {
            m->moves[0] = sListOur[k];
            m->movePPCur[0] = m->movePP[0] = 35;
        }
    }
    snprintf(what, sizeof what, "our %u foe %u", (unsigned)sListOur[k], (unsigned)sListFoe[k]);
    PORT_SweepCase(k, what);
    return 1;
}

static void Pr4_TestBattle(FieldSystem *fs) {
    const char *e = getenv("BWPORT_HG_TESTBATTLE"), *p;
    int species = 0, level = 0, oppSpecies, oppLevel, n = 0, setOur = 0, setOpp = 0;
    u16 our[4], opp[4];
    BattleSetup *setup;
    Party *party;

    if (sPr4TestState != 0 || e == NULL || sscanf(e, "%d:%d%n", &species, &level, &n) != 2 || species <= 0 || level <= 0) {
        return;
    }
    sPr4TestState = 1;
    oppSpecies = species;
    oppLevel = level;
    p = e + n;
    if (*p == ':') {
        p++;
        setOur = *p != ':' && *p != '\0';
        p += Pr4_TestMoves(p, our);
    }
    if (*p == ':') {
        p++;
        if (sscanf(p, "%d%n", &oppSpecies, &n) == 1) {
            p += n;
            if (*p == '/' && sscanf(p + 1, "%d%n", &oppLevel, &n) == 1) {
                p += 1 + n;
            }
        }
    }
    if (*p == ':') {
        p++;
        setOpp = *p != '\0';
        p += Pr4_TestMoves(p, opp);
    }
    party = SaveArray_Party_Get(fs->saveData);
    if (Party_GetCount(party) == 0) {
        GiveMon(HEAP_ID_FIELD2, fs->saveData, species, level, 0, 0, 0, fs->location->mapId, 24);
    }
    if (setOur) {
        Pr4_TestSetMoves(Party_GetMonByIndex(party, 0), our);
    }
    setup = BattleSetup_New(HEAP_ID_FIELD2, 0);
    BattleSetup_InitFromFieldSystem(setup, fs);
    FieldSystem_GenerateSingleWildPokemon(fs, (u16)oppSpecies, (u8)oppLevel, FALSE, setup);
    if (setOpp) {
        Pr4_TestSetMoves(Party_GetMonByIndex(setup->party[BATTLER_ENEMY], 0), opp);
    }
    sub_02050B08(fs, setup);
    fprintf(stderr, "test: wild battle species %d level %d vs %d level %d at V-blank %u\n", species, level, oppSpecies, oppLevel,
            (unsigned)OS_GetVBlankCount());
}

// each field frame: the test battle's start (a free field) and end (the field free again after it)
static void Pr4_TestFrame(FieldSystem *fs, int free) {
    if (sPr4TestState == 0) {
        if (free) {
            Pr4_TestBattle(fs);   // BWPORT_HG_TESTBATTLE only: a field task from the next frame
        }
    } else if (sPr4TestState == 1 && !free) {
        sPr4TestState = 2;
    } else if (sPr4TestState == 2 && free) {
        sPr4TestState = 3;
        fprintf(stderr, "test: battle over at V-blank %u\n", (unsigned)OS_GetVBlankCount());
        if (Pr4_SweepOn()) {
            PORT_SweepEnd("battle", getenv("BWPORT_HG_TESTEXIT") != NULL);
        }
        if (getenv("BWPORT_HG_TESTEXIT") != NULL) {
            _exit(0);
        }
    }
}

// The app sweep's cheats (tools/hg_app_sweep.py; test only: a throwaway save, never a route or the save library): the list of
// words in BWPORT_HG_CHEAT is applied once, at the first free field frame, before the debug warp. `items`: one or more of
// every pocket's kind, the eight HMs, the bicycle, the three rods, the Escape Rope and the Repels, and 99999 money; `badges`:
// all sixteen; `party`: the party filled to six, the new members knowing the field moves (and the dex entries a received
// Pokemon gets); `box`: 30 Pokemon in the boxes; `gear`: the phone, map and radio cards; `natdex`: the national dex flag; `egg`: every egg of the party one step from hatching.
static void Pr4_AppCheat(FieldSystem *fs, int free) {
    static const u16 sItems[][2] = {
        {ITEM_POKE_BALL, 20}, {ITEM_GREAT_BALL, 10}, {ITEM_ULTRA_BALL, 5}, {ITEM_POTION, 10}, {ITEM_SUPER_POTION, 5},
        {ITEM_FULL_HEAL, 5}, {ITEM_REVIVE, 3}, {ITEM_REPEL, 10}, {ITEM_SUPER_REPEL, 5}, {ITEM_MAX_REPEL, 5}, {ITEM_ESCAPE_ROPE, 5},
        {ITEM_X_ATTACK, 3}, {ITEM_ORAN_BERRY, 5}, {ITEM_GRASS_MAIL, 1}, {ITEM_TM01, 1}, {ITEM_TM01 + 1, 1}, {ITEM_TM01 + 2, 1},
        {ITEM_TM01 + 3, 1}, {ITEM_TM05, 1}, {ITEM_HM01, 1}, {ITEM_HM02, 1}, {ITEM_HM03, 1}, {ITEM_HM04, 1}, {ITEM_HM05, 1},
        {ITEM_HM06, 1}, {ITEM_HM07, 1}, {ITEM_HM08, 1}, {ITEM_BICYCLE, 1}, {ITEM_OLD_ROD, 1}, {ITEM_GOOD_ROD, 1},
        {ITEM_SUPER_ROD, 1}, {ITEM_COIN_CASE, 1}, {ITEM_TOWN_MAP, 1},
    };
    static const u16 sMoves[4][4] = {
        {MOVE_CUT, MOVE_SURF, MOVE_STRENGTH, MOVE_FLASH},
        {MOVE_ROCK_SMASH, MOVE_HEADBUTT, MOVE_FLY, MOVE_WATERFALL},
        {MOVE_DEFOG, MOVE_WHIRLPOOL, MOVE_ROCK_CLIMB, MOVE_TELEPORT},
        {MOVE_DIG, MOVE_SWEET_SCENT, 0, 0},
    };
    static const u16 sSpecies[5] = {151, 25, 133, 63, 7};
    static int done;
    const char *e = getenv("BWPORT_HG_CHEAT");
    SaveData *save = fs->saveData;
    int i;

    if (done || e == NULL || !free) {
        return;
    }
    done = 1;
    if (strstr(e, "items") != NULL) {
        Bag *bag = Save_Bag_Get(save);

        for (i = 0; i < (int)(sizeof(sItems) / sizeof(sItems[0])); i++) {
            Bag_AddItem(bag, sItems[i][0], sItems[i][1], HEAP_ID_FIELD2);
        }
        PlayerProfile_SetMoney(Save_PlayerData_GetProfile(save), 99999);
    }
    if (strstr(e, "badges") != NULL) {
        for (i = 0; i < 16; i++) {
            PlayerProfile_SetBadgeFlag(Save_PlayerData_GetProfile(save), i);
        }
    }
    if (strstr(e, "party") != NULL) {
        Party *party = SaveArray_Party_Get(save);

        for (i = 0; i < 5 && Party_GetCount(party) < 6; i++) {
            int n = Party_GetCount(party);

            GiveMon(HEAP_ID_FIELD2, save, sSpecies[i], 30 + i, 0, 0, 0, 4, 24);
            if (Party_GetCount(party) > n && i < 4) {
                Pr4_TestSetMoves(Party_GetMonByIndex(party, Party_GetCount(party) - 1), sMoves[i]);
            }
        }
    }
    if (strstr(e, "box") != NULL) {
        PCStorage *pc = SaveArray_PCStorage_Get(save);
        Pokemon *mon = AllocMonZeroed(HEAP_ID_FIELD2);

        for (i = 0; i < 30; i++) {
            ZeroMonData(mon);
            CreateMon(mon, 1 + 3 * i, 10 + i, 32, FALSE, 0, 0, 0);
            UpdatePokedexWithReceivedSpecies(save, mon);
            PCStorage_PlaceMonInFirstEmptySlotInAnyBox(pc, Mon_GetBoxMon(mon));
        }
        Heap_Free(mon);
    }
    if (strstr(e, "gear") != NULL) {
        SavePokegear *gear = SaveData_Pokegear_Get(save);

        SavePokegear_RegisterCard(gear, GEARCARD_PHONE);
        SavePokegear_RegisterCard(gear, GEARCARD_MAP);
        SavePokegear_RegisterCard(gear, GEARCARD_RADIO);
        Pokegear_SetMapUnlockLevel(gear, 2);
    }
    if (strstr(e, "natdex") != NULL) {
        Pokedex_SetNatDexFlag(Save_Pokedex_Get(save));
    }
    if (strstr(e, "egg") != NULL) {
        Party *party = SaveArray_Party_Get(save);
        u32 cycles = 0;

        // every egg of the party has no cycle left and the day care's step counter is one step short of a cycle (255 steps):
        // the next step ends the cycle and the egg hatches (HandleDaycareStep)
        for (i = 0; i < Party_GetCount(party); i++) {
            Pokemon *mon = Party_GetMonByIndex(party, i);

            if (GetMonData(mon, MON_DATA_IS_EGG, NULL)) {
                SetMonData(mon, MON_DATA_FRIENDSHIP, &cycles);
            }
        }
        Save_Daycare_SetEggCycleCounter(Save_Daycare_Get(save), 254);
    }
    fprintf(stderr, "test: app cheat %s at V-blank %u\n", e, (unsigned)OS_GetVBlankCount());
}

// The crash sweep's debug warp (tools/hg_sweep.py, docs/hgss.md "Crash sweep"; crash finding only, never in a route or a
// library save): BWPORT_HG_WARP=MAP:X:Y[:END]. At the first frame the field is free the player is warped to MAP at tile X,Y
// (global tile coordinates, as a warp event's) by the script command's warp (ScrCmd_Warp's CallTask_ScriptWarp: leave the
// overworld, the map change, restore it; warpId -1 = the coordinates), called from a field task of ours as a script's task
// would. (sub_020538C0 is Fly's: its landing task crashes on the pin, ov02_02249444's raw +0x40.) From then on each map the
// free player stands in is logged, and at V-blank END (0: never) the exe exits 0. BWPORT_HG_WARP_LOG=FILE gets the lines
// (`warp F Z`, `zone Z X Y F`, `pos F X Z in|off`, `end F`), unbuffered.
static int sPr4WarpMap = -1, sPr4WarpX, sPr4WarpY, sPr4WarpState;   // -1: no warp (BWPORT_HG_WARP=-1 or unset)
static int sPr4WarpDir = DIR_SOUTH, sPr4WarpDone, sPr4Landed;   // the facing it lands with; its task is over; the free player stood on MAP (the interaction sweep starts then)

void HgCov_Dump(void);   // port/game/hg_cov.c: the functions reached so far to BWPORT_HG_COV (-Dhgss_cov=true builds)

static BOOL Pr4_WarpTask(TaskManager *taskManager) {
    if (sPr4WarpState++ == 0) {
        CallTask_ScriptWarp(taskManager, (u32)sPr4WarpMap, -1, sPr4WarpX, sPr4WarpY, sPr4WarpDir);
        return FALSE;
    }
    sPr4WarpDone = 1;
    return TRUE;   // the warp's task has ended (TaskManager_Call runs it before this one again)
}

// BWPORT_HG_SCRIPT=ID[:FROM] (test only, off by default; tools/hg_app_sweep.py): the first free field frame at or after V-blank
// FROM (and after the warp's task, when BWPORT_HG_WARP warps) starts the game's own script ID the way a map's scene script is
// started (StartMapSceneScript: the script task of the field, no object), e.g. 2010 = the Pokemon Center PC's script (std_pokecenter_pc,
// scr_seq_0003_010): its menus then take the pilot's keys and the script's own command opens the box app (overlay 14).
static void Pr4_ScriptHook(FieldSystem *fs, int up, int freeNow) {
    static int id = -2, done;
    static unsigned from;

    if (id == -2) {
        const char *e = getenv("BWPORT_HG_SCRIPT");
        unsigned f = 0;

        id = -1;
        if (e != NULL && sscanf(e, "%d:%u", &id, &f) >= 1 && id >= 0) {
            from = f;
        } else {
            id = -1;
        }
    }
    if (id < 0 || done || !up || !freeNow || OS_GetVBlankCount() < from || (sPr4WarpMap >= 0 && sPr4WarpState < 2)) {
        return;
    }
    done = 1;
    fprintf(stderr, "test: script %d started at V-blank %u\n", id, (unsigned)OS_GetVBlankCount());
    StartMapSceneScript(fs, (u16)id, NULL);
}

static void Pr4_Sweep(FieldSystem *fs, int up, int freeNow) {
    static int state = -1, map, x, y, last = -1, lastX = -99999, lastZ = -99999;
    static unsigned end;
    static FILE *log;
    unsigned fr;

    if (state < 0) {
        const char *e = getenv("BWPORT_HG_WARP"), *l = getenv("BWPORT_HG_WARP_LOG");

        state = 9;
        end = 0;
        if (e != NULL && sscanf(e, "%d:%d:%d:%u", &map, &x, &y, &end) >= 3) {
            state = 0;
            sPr4WarpMap = map;
            sPr4WarpX = x;
            sPr4WarpY = y;
            if (getenv("BWPORT_HG_WARP_DIR") != NULL) {
                sPr4WarpDir = atoi(getenv("BWPORT_HG_WARP_DIR"));
            }
            log = l != NULL ? fopen(l, "w") : NULL;
            if (log != NULL) {
                setvbuf(log, NULL, _IONBF, 0);
            }
        }
    }
    if (state == 9) {
        return;
    }
    fr = OS_GetVBlankCount();
    if (up && freeNow && state == 0) {
        if (log != NULL) {
            fprintf(log, "warp %u %d\n", fr, (int)fs->location->mapId);
        }
        sPr4WarpState = 0;
        if (sPr4WarpMap >= 0) {   // MAP -1: no warp, the player stays where the save stands (tools/hg_app_sweep.py)
            FieldSystem_CreateTask(fs, Pr4_WarpTask, NULL);
        }
        state = 1;
    } else if (up && freeNow && state == 1 && (int)fs->location->mapId != last) {
        last = (int)fs->location->mapId;
        if (last == sPr4WarpMap) {
            sPr4Landed = 1;
        }
        if (log != NULL) {
            fprintf(log, "zone %d %d %d %u\n", last, (int)PlayerAvatar_GetXCoord(fs->playerAvatar),
                    (int)PlayerAvatar_GetZCoord(fs->playerAvatar), fr);
        }
    }
    if (state == 1 && up && log != NULL && fs->playerAvatar != NULL && fs->mapMatrix != NULL) {
        // each tile the player stands on (`pos F X Z in|off`): `off` = outside the map matrix's blocks (hg_sweep tags a crash
        // with the last position off the map as a harness artefact: the random walk left the map, nothing the game allows)
        int px = (int)PlayerAvatar_GetXCoord(fs->playerAvatar), pz = (int)PlayerAvatar_GetZCoord(fs->playerAvatar);

        if (px != lastX || pz != lastZ) {
            lastX = px;
            lastZ = pz;
            fprintf(log, "pos %u %d %d %s\n", fr, px, pz,
                    px < 0 || pz < 0 || px / 32 >= MapMatrix_GetWidth(fs->mapMatrix) || pz / 32 >= MapMatrix_GetHeight(fs->mapMatrix)
                        ? "off" : "in");
        }
    }
    if (state == 1 && (fr & 255) == 0) {
        HgCov_Dump();
    }
    if (state == 1 && end != 0 && fr >= end) {
        HgCov_Dump();
        if (log != NULL) {
            fprintf(log, "end %u\n", fr);
        }
        _exit(0);
    }
}

// ---- The interaction sweep (tools/hg_int_sweep.py, docs/hgss.md "Interaction sweep"; crash finding only, as the debug warp) ----
// BWPORT_HG_WARP=MAP:X:Y lands the player on MAP (Pr4_Sweep), then BWPORT_HG_INT=START[:BUDGET[:MAXN]] runs the map's events
// from index START: each object event (a person), BG event (sign, hidden item, plain) and coord event (a trigger tile) in turn.
// Per event: the game's own warp to a free tile next to it (the player facing it), A (a trigger tile: the step onto it, after
// the event's variable is set to the value that fires it), then A / B / START pulses while the script runs, until the field is free for
// 30 V-blanks (the row is `ok`), BUDGET V-blanks pass (`timeout`, default 1500) or a battle reaches its first move menu
// (`battle`: the exe exits 0, the tool starts again at the next event). MAXN events per run (default 40), then exit 0.
// BWPORT_HG_INT_LOG=FILE (unbuffered): `count N nobj nbg ncoord`, `ev I KIND SUB X Z SCRIPT A B C` for every event,
// `begin I F`, `at I F X Z FACE CAND`, `script I F ID` (SetupScriptEngine: the hook in pokeheartgold-route.patch),
// `end I F VERDICT scripts=N map=M`, `exit F WHY`. Keys: the pulses go through the key register (route_test.c).
// BWPORT_HG_INT_NOFORCE: a coord event's variable stays as it is. BWPORT_HG_INT_NOSHOW: a person hidden by its flag stays hidden
// (default: the flag is cleared for the warp and set again at the row's end: `show I flag N`).
void PortRoute_SetKeysExternal(unsigned bits);   // port/src/route_test.c

enum { PR4_INT_OFF, PR4_INT_WAIT, PR4_INT_PICK, PR4_INT_WARP, PR4_INT_SETTLE, PR4_INT_PRESS, PR4_INT_RUN };

typedef struct {
    int kind;   // 0 object, 1 BG, 2 coord
    int sub, x, z, script, a, b, c, w, h, var, val;
} Pr4IntEv;

#define PR4_INT_MAXEV 700
#define PR4_INT_CANDS 8

static struct {
    int on, state, start, budget, maxn, n, idx, ci, nc, ran, scripts, scriptsBefore, battleMenu, forced, nobj, nbg, ncoord, shown, pre;
    unsigned t0, pressF, busyF, battleF, freeF;
    int cx[PR4_INT_CANDS], cz[PR4_INT_CANDS], cf[PR4_INT_CANDS];
    int forceVar, forceOld;
    FILE *log;
    Pr4IntEv ev[PR4_INT_MAXEV];
} sInt;

// the pulses of a running script: A A A B A A A START, each held 4 V-blanks, 6 off (START finishes a name entry's keyboard; with
// the field up it is an A: START opens the field menu there)
static unsigned Pr4_IntPulse(unsigned dt, int up) {
    if (dt % 10 >= 4) {
        return 0;
    }
    switch ((dt / 10) % 8) {
    case 3: return 2u;
    case 7: return up ? 1u : 8u;   // (START with the field up would open the menu)
    default: return 1u;
    }
}

static void Pr4_IntExit(unsigned fr, const char *why) {
    fprintf(sInt.log, "exit %u %s\n", fr, why);
    fflush(sInt.log);
    _exit(0);
}

static void Pr4_IntEnd(FieldSystem *fs, unsigned fr, const char *verdict) {
    if (sInt.forced) {   // the coord event's variable back
        FieldSystem_VarSet(fs, (u16)sInt.forceVar, (u16)sInt.forceOld);
        sInt.forced = 0;
    }
    if (sInt.shown != 0) {   // the person's hiding flag back
        FieldSystem_FlagSet(fs, (u16)sInt.shown);
        sInt.shown = 0;
    }
    PortRoute_SetKeysExternal(0);
    fprintf(sInt.log, "end %d %u %s scripts=%d map=%d\n", sInt.idx, fr, verdict, sInt.scripts - sInt.scriptsBefore,
            fs->location != NULL ? (int)fs->location->mapId : -1);
    sInt.idx++;
    sInt.ran++;
    sInt.state = PR4_INT_PICK;
    sInt.t0 = fr;
}

void PortRoute4_ScriptStart(u16 script) {   // SetupScriptEngine (pokeheartgold-route.patch): every script the game starts
    if (!sInt.on || sInt.log == NULL) {
        return;
    }
    sInt.scripts++;
    fprintf(sInt.log, "script %d %u %u\n", sInt.idx, (unsigned)OS_GetVBlankCount(), (unsigned)script);
}

static void Pr4_IntBattle(int kind) {   // PortRoute4_BattleMenu (0: the action menu) / PortRoute4_BattleMove (100: a move was chosen)
    unsigned fr = OS_GetVBlankCount();

    if (!sInt.on || sInt.state != PR4_INT_RUN) {
        return;
    }
    if (kind == 0 && !sInt.battleMenu) {
        sInt.battleMenu = 1;
        sInt.battleF = fr;
        fprintf(sInt.log, "battle %d %u menu\n", sInt.idx, fr);
    } else if (kind == 100) {
        fprintf(sInt.log, "end %d %u battle-move scripts=%d map=-1\n", sInt.idx, fr, sInt.scripts - sInt.scriptsBefore);
        Pr4_IntExit(fr, "battle");
    }
}

static void Pr4_IntLoad(FieldSystem *fs) {   // the map's events, once, after the landing
    const MapEvents *me = fs->mapEvents;
    int i, n = 0;

    sInt.nobj = (int)me->num_object_events;
    sInt.nbg = (int)me->num_bg_events;
    sInt.ncoord = (int)me->num_coord_events;
    for (i = 0; i < sInt.nobj && n < PR4_INT_MAXEV; i++, n++) {
        const ObjectEvent *o = &me->object_events[i];
        Pr4IntEv *e = &sInt.ev[n];

        memset(e, 0, sizeof *e);
        e->kind = 0;
        e->sub = o->type;
        e->x = o->x;
        e->z = o->z;
        e->script = o->scriptId;
        e->a = o->id;
        e->b = o->eventFlag;
        e->c = o->movement;
    }
    for (i = 0; i < sInt.nbg && n < PR4_INT_MAXEV; i++, n++) {
        const BgEvent *g = &me->bg_events[i];
        Pr4IntEv *e = &sInt.ev[n];

        memset(e, 0, sizeof *e);
        e->kind = 1;
        e->sub = g->type;
        e->x = g->x;
        e->z = g->z;
        e->script = g->scriptId;
        e->a = g->dir;
    }
    for (i = 0; i < sInt.ncoord && n < PR4_INT_MAXEV; i++, n++) {
        const CoordEvent *c = &me->coord_events[i];
        Pr4IntEv *e = &sInt.ev[n];

        memset(e, 0, sizeof *e);
        e->kind = 2;
        e->x = c->x;
        e->z = c->z;
        e->script = c->scriptId;
        e->w = c->w;
        e->h = c->h;
        e->var = c->var;
        e->val = c->val;
    }
    sInt.n = n;
    fprintf(sInt.log, "count %d %d %d %d\n", n, sInt.nobj, sInt.nbg, sInt.ncoord);
    for (i = 0; i < n; i++) {
        const Pr4IntEv *e = &sInt.ev[i];

        fprintf(sInt.log, "ev %d %d %d %d %d %d %d %d %d\n", i, e->kind, e->sub, e->x, e->z, e->script,
                e->kind == 2 ? e->var : e->a, e->kind == 2 ? e->val : e->b, e->kind == 2 ? e->w * 1000 + e->h : e->c);
    }
}

// a tile the player can stand on: 0 blocked (outside the matrix, another object, the collision bit), 1 free, 2 unknown (the land
// chunk is not loaded: the game keeps the player's surroundings only)
static int Pr4_IntTile(FieldSystem *fs, int x, int z) {
    u16 a;
    LocalMapObject *o;

    if (x < 0 || z < 0 || Pr4_ZoneAt(x, z) < 0) {
        return 0;
    }
    o = MapObjectManager_GetFirstObjectWithXAndZ(fs->mapObjectManager, (u32)x, (u32)z, FALSE);
    if (o != NULL && o != PlayerAvatar_GetMapObject(fs->playerAvatar)) {
        return 0;
    }
    if (!((const Pr4Terrain *)fs->unk60)->getTileAttributes(fs, x, z, &a)) {
        return 2;
    }
    return ((a >> 15) & 1) ? 0 : 1;
}

static int Pr4_IntCollides(int x, int z) {
    u16 a;

    return Pr4_Attr(x, z, &a) && ((a >> 15) & 1);
}

// the tiles to stand on for event E, the facing toward it: where a facing d looks (DIR_NORTH 0, SOUTH 1, WEST 2, EAST 3)
static void Pr4_IntCands(FieldSystem *fs, const Pr4IntEv *e, int loaded, int *unknown) {
    static const int kDx[4] = {0, 0, -1, 1}, kDz[4] = {-1, 1, 0, 0};
    int faces = 0xf, d, dist;

    sInt.nc = 0;
    *unknown = 0;
    if (e->kind == 1 && e->sub != 2) {   // a plain or sign BG event: BG_DIR_* (0 N 1 E 2 W 3 S 4 any 5 east/west 6 north/south)
        switch (e->a) {
        case 0: faces = 1 << DIR_NORTH; break;
        case 1: faces = 1 << DIR_EAST; break;
        case 2: faces = 1 << DIR_WEST; break;
        case 3: faces = 1 << DIR_SOUTH; break;
        case 5: faces = (1 << DIR_WEST) | (1 << DIR_EAST); break;
        case 6: faces = (1 << DIR_NORTH) | (1 << DIR_SOUTH); break;
        default: faces = 0xf; break;
        }
    }
    for (dist = 1; dist <= 2; dist++) {
        for (d = 0; d < 4 && sInt.nc < PR4_INT_CANDS; d++) {
            int tx = e->x - kDx[d] * dist, tz = e->z - kDz[d] * dist;

            int st;

            if (!(faces & (1 << d))) {
                continue;
            }
            st = Pr4_IntTile(fs, tx, tz);
            if (st == 2 && !loaded) {
                *unknown = 1;
                continue;
            }
            if (st == 0) {
                continue;
            }
            if (e->kind == 2) {   // outside the region, one step from its tile
                if (dist != 1 || (tx >= e->x && tx < e->x + e->w && tz >= e->z && tz < e->z + e->h)) {
                    continue;
                }
            } else if (dist == 2 && !Pr4_IntCollides(e->x - kDx[d], e->z - kDz[d])) {   // across a counter only
                continue;
            }
            sInt.cx[sInt.nc] = tx;
            sInt.cz[sInt.nc] = tz;
            sInt.cf[sInt.nc] = d;
            sInt.nc++;
        }
    }
}

static void Pr4_IntWarp(FieldSystem *fs, unsigned fr) {
    sPr4WarpX = sInt.cx[sInt.ci];
    sPr4WarpY = sInt.cz[sInt.ci];
    sPr4WarpDir = sInt.cf[sInt.ci];
    sPr4WarpState = 0;
    sPr4WarpDone = 0;
    FieldSystem_CreateTask(fs, Pr4_WarpTask, NULL);
    sInt.state = PR4_INT_WARP;
    sInt.t0 = fr;
}

static void Pr4_IntFrame(FieldSystem *fs, int up, int freeNow) {
    static int sInit = -1;
    unsigned fr = OS_GetVBlankCount();
    Pr4IntEv *e;

    if (sInit < 0) {
        const char *s = getenv("BWPORT_HG_INT"), *l = getenv("BWPORT_HG_INT_LOG");

        sInit = 0;
        if (s != NULL && l != NULL && getenv("BWPORT_HG_WARP") != NULL) {
            sInt.start = 0;
            sInt.budget = 1500;
            sInt.maxn = 40;
            sscanf(s, "%d:%d:%d", &sInt.start, &sInt.budget, &sInt.maxn);
            sInt.log = fopen(l, "w");
            if (sInt.log != NULL) {
                setvbuf(sInt.log, NULL, _IONBF, 0);
                sInt.on = 1;
                sInt.state = PR4_INT_WAIT;
                sInt.idx = sInt.start;
                sInit = 1;
            }
        }
    }
    if (!sInt.on) {
        return;
    }
    sPr4Fs = up ? fs : NULL;
    if (sInt.state == PR4_INT_WAIT) {
        if (up && freeNow && sPr4Landed && fr >= 1200) {   // (the reload's key script is over at V-blank 1100)
            Pr4_IntLoad(fs);
            sInt.state = PR4_INT_PICK;
            sInt.t0 = fr;
        } else if (fr > 6000) {
            Pr4_IntExit(fr, "no-landing");
        } else if (fr >= 1300 && !(up && freeNow)) {   // an arrival script (a cutscene, an app) holds the field: the pulses
            PortRoute_SetKeysExternal(Pr4_IntPulse(fr, up));
        } else if (fr >= 1300) {
            PortRoute_SetKeysExternal(0);
        }
        return;
    }
    if (sInt.state == PR4_INT_PICK) {
        int unknown;

        if (sInt.idx >= sInt.n) {
            Pr4_IntExit(fr, "done");
        }
        if (sInt.ran >= sInt.maxn) {
            Pr4_IntExit(fr, "segment");
        }
        if (!(up && freeNow)) {
            if (fr - sInt.t0 > 900) {
                Pr4_IntExit(fr, "stuck-before-warp");
            }
            return;
        }
        e = &sInt.ev[sInt.idx];
        sInt.pre = 0;
        fprintf(sInt.log, "begin %d %u\n", sInt.idx, fr);
        if (e->kind == 0 && e->script == 0xFFFF) {   // a person with no script (decoration): nothing for A to start
            Pr4_IntEnd(fs, fr, "unset");
            return;
        }
        Pr4_IntCands(fs, e, 0, &unknown);
        if (sInt.nc == 0 && unknown) {   // the tiles around it are not loaded: stand on its tile first
            sInt.pre = 1;
            sInt.cx[0] = e->x;
            sInt.cz[0] = e->z;
            sInt.cf[0] = DIR_SOUTH;
            sInt.nc = 1;
        } else if (sInt.nc == 0) {
            Pr4_IntEnd(fs, fr, "no-spot");
            return;
        }
        if (e->kind == 0 && e->b != 0 && getenv("BWPORT_HG_INT_NOSHOW") == NULL && FieldSystem_FlagCheck(fs, (u16)e->b)) {
            FieldSystem_FlagClear(fs, (u16)e->b);   // a person the story keeps hidden here: shown for the warp (the map loads its objects again)
            sInt.shown = e->b;
            fprintf(sInt.log, "show %d flag %d\n", sInt.idx, e->b);
        }
        sInt.ci = 0;
        sInt.scriptsBefore = sInt.scripts;
        sInt.battleMenu = 0;
        Pr4_IntWarp(fs, fr);
        return;
    }
    e = &sInt.ev[sInt.idx];
    if (sInt.state == PR4_INT_WARP) {
        if (sPr4WarpDone && up && freeNow && (int)PlayerAvatar_GetXCoord(fs->playerAvatar) == sInt.cx[sInt.ci] &&
            (int)PlayerAvatar_GetZCoord(fs->playerAvatar) == sInt.cz[sInt.ci]) {
            fprintf(sInt.log, "at %d %u %d %d %d %d\n", sInt.idx, fr, sInt.cx[sInt.ci], sInt.cz[sInt.ci],
                    (int)PlayerAvatar_GetFacingDirection(fs->playerAvatar), sInt.ci);
            if (sInt.pre) {   // on the event's tile: the land around it is loaded, so now the real spot
                int unknown;

                sInt.pre = 0;
                Pr4_IntCands(fs, e, 1, &unknown);
                if (sInt.nc == 0) {
                    Pr4_IntEnd(fs, fr, "no-spot");
                } else {
                    sInt.ci = 0;
                    Pr4_IntWarp(fs, fr);
                }
                return;
            }
            sInt.state = PR4_INT_SETTLE;
            sInt.t0 = fr;
            sInt.freeF = fr;
        } else if (fr - sInt.t0 > 900) {
            fprintf(sInt.log, "end %d %u warp-timeout scripts=%d map=%d\n", sInt.idx, fr, sInt.scripts - sInt.scriptsBefore,
                    fs->location != NULL ? (int)fs->location->mapId : -1);
            Pr4_IntExit(fr, "warp-timeout");
        }
        return;
    }
    if (sInt.scripts != sInt.scriptsBefore && sInt.state != PR4_INT_RUN) {   // a script began on its own (a trainer's sight, a landing trigger)
        sInt.state = PR4_INT_RUN;
        sInt.pressF = fr;
        sInt.busyF = fr;
    }
    if (sInt.state == PR4_INT_SETTLE) {
        if (!(up && freeNow)) {
            sInt.freeF = fr;
        } else if (fr - sInt.freeF >= 20) {   // the fade-in is over, the field takes keys
            if (e->kind == 0 && MapObjectManager_GetFirstActiveObjectByID(fs->mapObjectManager, (u32)e->a) == NULL) {
                Pr4_IntEnd(fs, fr, "absent");
                return;
            }
            if (e->kind == 1 && e->sub == 2 && FieldSystem_FlagCheck(fs, HiddenItemScriptNoToFlagId((u16)e->script))) {
                Pr4_IntEnd(fs, fr, "collected");
                return;
            }
            if (e->kind == 2 && getenv("BWPORT_HG_INT_NOFORCE") == NULL && e->var >= 0x4000 &&
                FieldSystem_VarGet(fs, (u16)e->var) != (u16)e->val) {
                sInt.forceVar = e->var;
                sInt.forceOld = FieldSystem_VarGet(fs, (u16)e->var);
                FieldSystem_VarSet(fs, (u16)e->var, (u16)e->val);
                sInt.forced = 1;
                fprintf(sInt.log, "force %d var %d %d -> %d\n", sInt.idx, e->var, sInt.forceOld, e->val);
            }
            sInt.state = PR4_INT_PRESS;
            sInt.pressF = fr;
        }
        return;
    }
    if (sInt.state == PR4_INT_PRESS) {
        unsigned dt = fr - sInt.pressF;
        static const unsigned kDirBits[4] = {0x40, 0x80, 0x20, 0x10};   // up down left right (KEYINPUT bits)

        if (e->kind == 2) {
            PortRoute_SetKeysExternal(dt < 24 ? kDirBits[sInt.cf[sInt.ci]] : 0);
        } else {
            PortRoute_SetKeysExternal(dt < 4 ? 1u : 0u);
        }
        if (dt == 0) {
            fprintf(sInt.log, "press %d %u\n", sInt.idx, fr);
        }
        if (dt >= (e->kind == 2 ? 90u : 50u)) {   // nothing began: the next tile to try, else a row without a script
            PortRoute_SetKeysExternal(0);
            if (++sInt.ci < sInt.nc) {
                Pr4_IntWarp(fs, fr);
            } else {
                Pr4_IntEnd(fs, fr, "no-script");
            }
        }
        return;
    }
    if (sInt.state == PR4_INT_RUN) {
        unsigned dt = fr - sInt.pressF;

        // the pulses only while something runs (a free field would talk to the person again)
        PortRoute_SetKeysExternal(!(up && freeNow) ? Pr4_IntPulse(dt, up) : 0u);
        if (!(up && freeNow)) {
            sInt.busyF = fr;
        }
        if (sInt.battleMenu && fr - sInt.battleF >= 240) {   // the action menu, the move menu not asked for
            fprintf(sInt.log, "end %d %u battle scripts=%d map=-1\n", sInt.idx, fr, sInt.scripts - sInt.scriptsBefore);
            Pr4_IntExit(fr, "battle");
        }
        if (fr - sInt.busyF >= 30) {
            Pr4_IntEnd(fs, fr, "ok");
        } else if (dt >= (unsigned)sInt.budget) {
            fprintf(sInt.log, "end %d %u timeout scripts=%d map=%d\n", sInt.idx, fr, sInt.scripts - sInt.scriptsBefore,
                    fs->location != NULL ? (int)fs->location->mapId : -1);
            Pr4_IntExit(fr, "timeout");   // the field is not free: nothing to warp from
        }
    }
}

// each field frame (field_system.c FieldSystem_Main, after the frame's tasks and processes): the pilot's state. Off the field
// map (a menu app, a map change) the last field position, an event running
// The story readout's filler (port/src/route_story.c: the Ws: token, the map dump's story section): the saved flags and the
// work values (vars) read through the game's own accessors (the VarsFlags save block; its temp flags are the 0x4000 range of
// the flag accessor), never from save offsets. Test-only: PortRouteStory_Frame runs it only when the pilot, a map dump or
// BWPORT_STORYLOG is on. The zone's events are not listed (full is ignored): story.py reads those from the build's scripts.
static unsigned char sPr4Flags[NUM_FLAGS / 8];
static unsigned short sPr4Vars[NUM_VARS];

static void Pr4_StoryFill(PortStory *s, int full) {
    SaveVarsFlags *vf;
    int i;

    (void)full;
    memset(s, 0, sizeof *s);
    if (sPr4Fs == NULL || sPr4Fs->saveData == NULL || (vf = Save_VarsFlags_Get(sPr4Fs->saveData)) == NULL) {
        return;
    }
    memset(sPr4Flags, 0, sizeof sPr4Flags);
    for (i = 1; i < NUM_FLAGS; i++) {
        if (Save_VarsFlags_CheckFlagInArray(vf, (u16)i)) {
            sPr4Flags[i / 8] |= (unsigned char)(1 << (i % 8));
        }
    }
    for (i = 0; i < NUM_VARS; i++) {
        sPr4Vars[i] = *Save_VarsFlags_GetVarAddr(vf, (u16)(VAR_BASE + i));
    }
    for (i = 0; i < PORT_STORY_NTEMP; i++) {
        if (Save_VarsFlags_CheckFlagInArray(vf, (u16)(TEMP_FLAG_BASE + i))) {
            s->temp[i / 8] |= (unsigned char)(1 << (i % 8));
        }
    }
    s->ok = 1;
    s->zone = (unsigned)sPr4Fs->location->mapId;
    s->flags = sPr4Flags;
    s->nflags = NUM_FLAGS;
    s->works = sPr4Vars;
    s->nworks = NUM_VARS;
}

// A script's touch-screen yes/no (ScrCmd_GetMenuChoice: the field's choice sub-app, index 3, overlay 27's task data): while its state
// is 4 it takes keys (A = the cursor's row, up / down move it, B = No); `selection` is the row. Published as the script list's yes/no
// is (gPortRouteFieldYesNo 1 + row), so Py / Pn answer it. Not cleared when the app is gone but only when this hook set it.
static int sPr4TouchYN;

static void Pr4_TouchYesNo(FieldSystem *fs) {
    int row = -1;

    if (fs->unk_D8 != NULL) {
        SubAppTaskData *sub = SysTask_GetData(fs->unk_D8);

        if (sub != NULL && sub->appIdx == 3 && sub->appTask != NULL) {
            const Ov27TaskData *t = SysTask_GetData(sub->appTask);

            if (t != NULL && t->state == 4) {
                row = t->selection & 1;
            }
        }
    }
    if (row >= 0) {
        gPortRouteFieldYesNo = 1 + row;
        sPr4TouchYN = 1;
    } else if (sPr4TouchYN) {
        gPortRouteFieldYesNo = 0;
        sPr4TouchYN = 0;
    }
}

void PortRoute4_Frame(FieldSystem *fs) {
    static PortRouteState rs = {"HG", 0xffff, 0, 0, 1, -1, -1, 0, 0};

    PORT_StateHook = PortRoute_StateLine;   // port/src/run_frames.c prints it at the frame cap
    rs.game = Pr4_Game();
    Pr4_AppCheat(fs, Pr4_FieldUp(fs) && !fs->processManager->isPaused && !FieldSystem_TaskIsRunning(fs));
    Pr4_Sweep(fs, Pr4_FieldUp(fs), Pr4_FieldUp(fs) && !fs->processManager->isPaused && !FieldSystem_TaskIsRunning(fs));
    Pr4_ScriptHook(fs, Pr4_FieldUp(fs), Pr4_FieldUp(fs) && !fs->processManager->isPaused && !FieldSystem_TaskIsRunning(fs));
    Pr4_IntFrame(fs, Pr4_FieldUp(fs), Pr4_FieldUp(fs) && !fs->processManager->isPaused && !FieldSystem_TaskIsRunning(fs));
    if (Pr4_FieldUp(fs)) {
        const VecFx32 *p = PlayerAvatar_GetPositionVector(fs->playerAvatar);

        sPr4Fs = fs;
        PortSpeed4_MoveLog((int)PlayerAvatar_GetXCoord(fs->playerAvatar), (int)PlayerAvatar_GetZCoord(fs->playerAvatar), p->x >> FX32_SHIFT, p->z >> FX32_SHIFT,
                           fs->followMon.active ? (int)MapObject_GetXCoord(fs->followMon.mapObject) : -1,
                           fs->followMon.active ? (int)MapObject_GetZCoord(fs->followMon.mapObject) : -1, (int)fs->followMon.unk1C,
                           (int)PlayerAvatar_GetPlayerMoveState(fs->playerAvatar));
        Pr4_TouchYesNo(fs);
        Pr4_FlySlot(fs);
        rs.zone = (unsigned)fs->location->mapId;
        rs.gx = (int)PlayerAvatar_GetXCoord(fs->playerAvatar);
        rs.gz = (int)PlayerAvatar_GetZCoord(fs->playerAvatar);
        rs.fx = p->x >> FX32_SHIFT;
        rs.fz = p->z >> FX32_SHIFT;
        // free: the field takes keys (FieldSystem_IsPlayerMovementAllowed: not paused, the map running, no field task: a
        // script, a warp, the start menu are tasks)
        rs.event = fs->processManager->isPaused || FieldSystem_TaskIsRunning(fs) ? 1 : 0;
        if (getenv("BWPORT_HG_TESTBATTLE") != NULL) {
            Pr4_TestFrame(fs, !rs.event);
        }
        if (!rs.event) {   // nothing runs: no question and no save left over (a script that ended another way)
            gPortRouteFieldYesNo = 0;
            sPr4Save = -1;
        }
        rs.save = sPr4Save;
        PortRouteMap_SetAdapter(&kPortRouteGen4);
        gPortRouteStoryFill = Pr4_StoryFill;
        PortRouteStory_Frame(rs.game);
    } else {
        sPr4Fs = NULL;
        rs.event = 1;
        PortRouteMap_SetAdapter(NULL);
    }
    PortRoute_Frame(&rs);
}
#endif
