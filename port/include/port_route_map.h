// The map the route pilot sees (port/src/route_test.c: BWPORT_MAPDUMP, the Md and G tokens), and the one interface between
// the pilot's game-neutral core and a game's field engine.
//
// THE CORE (no game headers): port/src/route_map.c (the dump, the G tokens' parser, the planner: A* on this map) and
// port/src/route_test.c (the tokens, walking a plan, facing and talking, the waits). It only reads a PortMap and the
// PortRouteState the game hands it each field frame (port_route.h).
//
// THE ADAPTER (one per field engine, compiled with that game's headers): fills a PortMap from the game's own field and
// says where the map matrix's zones are. It registers itself with PortRouteMap_SetAdapter each field frame (the field is up).
//   Gen 5, Black / White / Black 2 / White 2: port/include/port_route_map_game.h, included once by each game's
//   port/extern*/src/system/game_event.c stand-in (its frame hook also fills PortRouteState).
//
// A Gen 4 adapter (Platinum, HeartGold / SoulSilver, Diamond / Pearl; pret pokeplatinum names, not written yet) fits as is:
//   - fill: player PlayerAvatar_GetXPos / GetZPos (tiles already), PlayerAvatar_GetFacingDir (DIR_* 0 up 1 down 2 left 3
//     right as Gen 5); zone = the map header ID (MapMatrix_GetMapHeaderIDAtCoords, 32 x 32 tile chunks as Gen 5); a tile's
//     kind from TerrainCollisionManager_CheckCollision / _GetTileBehavior (map_tile_behavior.h: the jump ledges, water,
//     tall grass, the counter, the side blocks) - one layer, so `y` stays 0 and `pass` is the side blocks plus the
//     neighbour's collision; actors from the MapObjectManager (MapObject_GetLocalID as `id`, the object event's id the
//     scripts use, x / z, initial x / z, GetGraphicsID as `code`, GetMovementType as `move`); warps, triggers, BG events
//     from MapHeaderData's WarpEvent / CoordEvent / BgEvent (tile units already: no / 16; a Gen 4 warp is 1 x 1:
//     w = h = 1; dest = its destHeaderID / destWarpID).
//   - zoneAt: MapMatrix_GetMapHeaderIDAtCoords(matrix, x / 32, z / 32).
//   - PortRouteState (port_route.h): the field frame hook of that game's FieldSystem (zone, grid and fine position,
//     an event running, the save and text states or -1).
// Test-only, as the rest of port_route.h.
#ifndef BWPORT_PORT_ROUTE_MAP_H
#define BWPORT_PORT_ROUTE_MAP_H

#define PORT_MAP_N 128   // the window: N x N tiles around the player (the dump prints the middle 64 x 64)

enum {   // PortMapCell.pass: the steps the game lets the player take from this tile (no actors counted)
    PORT_MAP_UP = 1,
    PORT_MAP_DOWN = 2,
    PORT_MAP_LEFT = 4,
    PORT_MAP_RIGHT = 8,
};

enum {   // PortMapCell.flags
    PORT_MAP_COUNTER = 1,   // a counter the player talks across (a Center's or a Mart's): A reaches the actor behind it
};

typedef struct {
    char kind;            // '.' walk, '#' blocked, '~' water, ',' grass, '^' 'v' '<' '>' a ledge jumped that way,
                          // '?' no terrain loaded there (the mapper keeps the chunks near the player), ' ' off the world
    unsigned char pass;   // PORT_MAP_* bits
    unsigned short zone;  // the map matrix's zone of the tile (0xffff none)
    short y;              // the height in tiles
    unsigned short cls;   // the tile class (the game's; Gen 5: field_map.h's TILE_CLASS_*), for the dump's reader only
    unsigned char flags;  // PORT_MAP_COUNTER
} PortMapCell;

typedef struct {
    int x, z, w, h;       // grid tiles
    unsigned zone, dest;  // the zone and the warp it leads to
    int dir;              // the direction the player faces arriving here (DIR_* + 1, 0 none)
    int type;             // the transition type (door, mat, walk-in, gate ...)
} PortMapWarp;

typedef struct {
    int x, z, w, h;
    unsigned script, workId, workValue, type;
} PortMapTrig;

typedef struct {
    int x, z;
    unsigned script, dir;
} PortMapBg;

typedef struct {
    int id, x, z, x0, z0;   // the actor's id (Gen 5 its UID: the zone's NPC id the scripts use), grid position and the
                            // one it moves from
    unsigned code, move;    // object code, move code
} PortMapNpc;

typedef struct {
    int ok;                  // filled (the field is up)
    int x0, z0;              // the grid position of cell[0][0]
    int px, pz, py;          // the player
    int pdir;                // the player's facing: 0 up 1 down 2 left 3 right, -1 not known
    unsigned zone;
    int mw, mh;              // the map matrix's size in chunks (32 x 32 tiles each)
    PortMapCell cell[PORT_MAP_N][PORT_MAP_N];   // [z][x]
    int nwarp, ntrig, nbg, nnpc;
    PortMapWarp warp[64];
    PortMapTrig trig[64];
    PortMapBg bg[64];
    PortMapNpc npc[64];      // not the player
} PortMap;

// A field engine's side of the pilot. All of it is called from the game thread inside the field frame hook only.
typedef struct {
    const char *engine;                // "gen5": the dump's first line names it
    // required: fill *m around the player (x0 = px - PORT_MAP_N / 2, z0 likewise), every cell (kind, pass: the steps the
    // game takes from it, zone, flags) and the lists; m->ok = 1, or m->ok = 0 when there is no field now
    void (*fill)(PortMap *m);
    // the map matrix's zone at any grid tile of the world (no chunk load needed), -1 off the matrix; NULL: no matrix
    // (the planner's G:warp then knows only the window's zones, the dump prints no zone table)
    int (*zoneAt)(int x, int z);
} PortRouteAdapter;

// the game's field frame hook registers its adapter (each frame: cheap; NULL: no field)
void PortRouteMap_SetAdapter(const PortRouteAdapter *a);
const PortRouteAdapter *PortRouteMap_Adapter(void);

#endif
