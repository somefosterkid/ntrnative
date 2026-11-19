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
#define PORT_MAP_LEVERS 24   // PortMap's levers (Black 2's Opelucid Gym: up to 14 open lifts; White 2's Frigate: 4 + 14 pads)

enum {   // PortMapCell.pass: the steps the game lets the player take from this tile (no actors counted)
    PORT_MAP_UP = 1,
    PORT_MAP_DOWN = 2,
    PORT_MAP_LEFT = 4,
    PORT_MAP_RIGHT = 8,
};

enum {   // PortMapCell.flags
    PORT_MAP_COUNTER = 1,   // a counter the player talks across (a Center's or a Mart's): A reaches the actor behind it
    PORT_MAP_SWITCH = 2,    // a hybrid zone's switch tile (a grid and rails in one zone): the adapter's gridToRail there
    PORT_MAP_SURFEDGE = 4,  // a surf edge tile (class 0x41, 0x44): a wall for walking; surfing, a step onto it hops over it
    PORT_MAP_FALL = 8,      // a waterfall tile (class 0x40): a wall; surfing at it, the Waterfall script (F:waterfall)
    PORT_MAP_ELECTRIC = 16, // an electric rock (class 0x32, Chargestone Cave): a wall; a pushed crystal stops next to it (F:push)
    PORT_MAP_SHELTER = 32,  // PortMap's wind: a gust leaves the player standing on this tile (route_slide.c's PortRouteWind_Hop)
    PORT_MAP_BRIDGE = 64,   // a second floor 20 or more units from the cell's own (a walkway over a path: Opelucid Gym's upper
                            // floor): yb, passb; the planner crosses it straight on that floor (no stop, no turn there)
    PORT_MAP_ROM = 128,     // a chunk the mapper has not loaded: its terrain is the ROM's chunk file (the adapter reads it the
                            // way the game does), static (no switches' or scripts' changes): route_map.c's Recall puts what
                            // the game showed loaded there over it
    PORT_MAP_STEPON = 512,  // stepping onto the tile starts something that carries the player off (Platinum's Canalave gym: a
                            // platform standing at the player's height rides him as he arrives): the planner crosses it only
                            // as the goal, so a walk to a platform tile is a `G:` of its own and the ride a wait after it
    PORT_MAP_BIKESLOPE = 1024,   // a bike slope (Gen 4's muddy slope: a wall '#' for a walker): the bicycle climbs it going up at its top
                            // speed (the fast gear, 3 steps run up in a line), the planner's PortRouteSlide_BikeRun lands past it
    PORT_MAP_NOBIKE = 2048, // a tile the bicycle may not enter (Gen 4: mud, snow, very tall grass: player_move.c
                            // PlayerAvatar_CheckBikeCollision): a rider's step onto it never happens, so the planner has the
                            // pilot get off first (PortRoutePlan.dismount) and cuts a run at the tile before it
    PORT_MAP_CLIMBNS = 4096,   // a Rock Climb wall tile (Gen 4 TILE_BEHAVIOR_ROCK_CLIMB_N_S; a wall '#' for a walker): climbed going north or
                            // south (ov5_021DFB54.c PlayerAvatar_CanUseRockClimb), by F:climb (route_fieldmove.h)
    PORT_MAP_CLIMBEW = 8192,   // the same climbed going west or east (TILE_BEHAVIOR_ROCK_CLIMB_E_W)
    PORT_MAP_FARJUMP = 16384,   // a ledge ('^' 'v' '<' '>') whose jump lands two tiles past it, not one (Platinum's Distortion World
                            // walkways: a pair of jump-twice tiles facing each other, crossed either way in one 3-tile jump)
    PORT_MAP_HOLE = 256,    // a Strength hole (Gen 5 class 0x1d, MapTile_IsStrengthHole): a wall ('#') until a boulder fills
                            // it, then walkable ('.': the game's PlayerMoveCollCheck lets the player on where the save keeps a
                            // boulder, IsSavedStrengthRockAtPos); the dump prints O (open) and o (filled)
};

typedef struct {
    char kind;            // '.' walk, '#' blocked, '~' water, ',' grass, '^' 'v' '<' '>' a ledge jumped that way, 'e' a surf
                          // edge tile, 'F' a waterfall tile (both blocked for walking),
                          // '?' no terrain loaded there (the mapper keeps the chunks near the player), ' ' off the world
    unsigned char pass;   // PORT_MAP_* bits
    unsigned short zone;  // the map matrix's zone of the tile (0xffff none)
    short y;              // the height in tiles
    short yf;             // the same in 1/16 tiles (the game's own height units: a step of 20 or more is blocked)
    short gy;             // the grid height (Gen 5: HEIGHT_TO_GRID of the layer): an actor blocks the cell only at this height
    short yb;             // PORT_MAP_BRIDGE: the second floor's height in 1/16 tiles (yf's units)
    unsigned char passb;  // PORT_MAP_BRIDGE: the second floor's steps (pass's bits)
    unsigned short cls;   // the tile class (the game's; Gen 5: field_map.h's TILE_CLASS_*), for the dump's reader only
    unsigned short flags; // PORT_MAP_COUNTER, PORT_MAP_SWITCH, ... PORT_MAP_HOLE
    unsigned char slide;  // 0, PORT_MAP_ICE, or PORT_MAP_SLOPE + the direction it pushes (port/src/route_slide.c)
    unsigned char ihc;    // Gen 4 ice with height (Snowpoint's gym): 2 bits per direction (0 up 1 down 2 left 3 right; the game's step order),
                          // 1 the floor rises a quarter tile on, 2 it falls (player_move.c PlayerAvatar_CheckIceHeightChange): 0 elsewhere
} PortMapCell;

enum {   // PortMapCell.slide >= PORT_MAP_PAD: a pad (Black 2 / White 2's Driftveil Gym) = PORT_MAP_PAD + 4 * length class + the
         // direction it works in (the length classes: route_slide.c's kPadTiles)
    PORT_MAP_PAD = 8,
};
enum {   // PortMapCell.slide >= PORT_MAP_CANNON: the tile a cannon shoots from (Black / White's Mistralton Gym): the key toward the
         // cannon there (a wall) lands the player on the line it faces = PORT_MAP_CANNON + 4 * (the tiles to the landing - 1) +
         // the direction (port/src/route_slide.c's PortRouteSlide_Cannon)
    PORT_MAP_CANNON = 128,
};
enum {   // PortMapCell.slide: a step onto the tile goes on by itself (the planner's slide, port/src/route_slide.c)
    PORT_MAP_ICE = 1,     // on the way the player moves until the next tile is in the way or the tile is not ice
    PORT_MAP_SLOPE = 2,   // + 0 up 1 down 2 left 3 right: that way (and on past the slope) until the next tile is in the way
};

// A place on a rail (Gen 5's no-grid maps: Castelia's streets, Skyarrow Bridge): the line (or point) and the tiles along
// it (front) and across it (side); dir is the game's own (the rail direction it last moved in), kept for the step call.
// The dump writes it rail[line,front,side].
typedef struct {
    int line, isLine, front, side, dir;
} PortRailPos;

// PortMapWarp, PortMapTrig, PortMapBg, PortMapNpc with rail = 1 are on a rail: x = front, z = side, w / h = the tiles
// along / across it (spans), line = its line; the grid planner skips them, the rail planner uses only them.
typedef struct {
    int x, z, w, h;       // grid tiles
    int rail, line;
    unsigned zone, dest;  // the zone and the warp it leads to
    int dir;              // the direction the player faces arriving here (DIR_* + 1, 0 none); walking in needs the opposite
    int type;             // the transition type (door, mat, walk-in, gate ... 7: Gen 4's mat, ladder or stairs, route_gen4.c)
} PortMapWarp;

typedef struct {
    int x, z, w, h;
    int rail, line;
    unsigned script, workId, workValue, type;
    int avoid;            // the trigger is live (its work = its value: a step onto it starts its script) and the adapter wants
                          // it walked round (HeartGold / SoulSilver: a gym's pits, a cave's holes, a field of triggers of one
                          // script; never a story scene's own trigger): route_map.c's search costs
                          // a step onto it as a warp's (round it when there is a way round), unless it is the goal
} PortMapTrig;

typedef struct {
    int x, z;
    int rail, line;
    unsigned script;
    unsigned dir;    // the side it is read from (Gen 5: 0 facing up, 1 right, 2 left, 3 down, 4 any, 5 sideways, 6 up or down)
    unsigned cond;   // its condition (Gen 5: 2 a hidden item, read from any side)
} PortMapBg;

typedef struct {
    int id, x, z, x0, z0;   // the actor's id (Gen 5 its UID: the zone's NPC id the scripts use), grid position and the
                            // one it moves from
    int y, y0, layer;       // layer 1: y, y0 are its grid heights there (Gen 5: GPos y) and it blocks a cell only when the
                            // cell's gy is the same (field_actor_tool.c's actor collision: |cell height - actor's| < 1;
                            // Driftveil Gym's stacked girders: actors on another girder are not in the way). 0: any height
    int rail, line;
    unsigned code, move;    // object code, move code
    int w, h;               // its collision footprint (field_actor_tool.c's CheckAreaOverActor: collisionWidth, Height):
                            // the tiles x .. x + w - 1, z - h + 1 .. z; 0 or 1 one tile (Dragonspiral Tower's 2x2 boulder)
    int face;               // Gen 4: its facing (0 up 1 down 2 left 3 right), -1 not known (Gen 5 leaves 0 with look 0)
    int sight;              // Gen 4: an undefeated trainer sees this far in the directions of look (the game's sight range), else 0
    unsigned char look;     // bit d: it sees the tiles in direction d from its own: a trainer facing d (they turn: the face changes),
                            // 15 for one who sees all four ways. The pilot waits at the edge of a watched line (route_test.c)
    int nocoll;             // the game's collision is off for it (actor flag 0x80: a Strength boulder that fell into its
                            // hole, EventStrengthRockPush_Callback): never in the way
    int breaks;             // a slide with speed smashes it (Gen 4's snowball, ov5_021DFB54.c ov5_021E06A8: the actor is deleted): the
                            // planner's block grid marks its tile 2, route_slide.c PortRouteSlide_End passes it at speed 1 or more
} PortMapNpc;

typedef struct {
    int ok;                  // filled (the field is up)
    int x0, z0;              // the grid position of cell[0][0]
    int px, pz, py;          // the player (on a rail: the grid tile the actor last had, stale)
    int wx, wz;              // the tile the player's world position is in (what px, pz say off a rail; on a rail, where it is)
    int pdir;                // the player's facing: 0 up 1 down 2 left 3 right, -1 not known
    unsigned zone;
    int mw, mh;              // the map matrix's size in chunks (32 x 32 tiles each)
    unsigned char surf;      // the player is surfing: the water cells carry pass bits (water to water, and onto the shore; a
                             // step onto an edge tile hops 2 tiles onto land) and the planner walks on '~'. Never land to water
    unsigned char cycle;     // Gen 4: the player is on the bicycle: 1 in its slow gear (speed never above 2), 2 in the fast gear (the
                             // B key toggles it); PORT_MAP_BIKESLOPE tiles are climbable only in gear 2 (the planner asks for the press)
    int wind, gust;          // wind 1: a gust blows the player off any tile that is not PORT_MAP_SHELTER, gust event checks
                             // from now (Black 2 / White 2's Mistralton Gym: the gym's frame count to 120; route_slide.c)
    int windHold;            // the count does not run where the player stands (the gym's platforms): no gust comes while he
                             // waits there, so the walk goes on (route_slide.c's PortRouteWind_Hop never waits there)
    PortMapCell cell[PORT_MAP_N][PORT_MAP_N];   // [z][x]
    int nwarp, ntrig, nbg, nnpc;
    PortMapWarp warp[64];
    PortMapTrig trig[64];
    PortMapBg bg[64];
    PortMapNpc npc[64];      // not the player
    int rail;                // the player moves on a rail now, at rpos (the grid cells are the world's under it)
    PortRailPos rpos;
    int nline;               // the zone's rail lines: the two points each joins (lines meeting at a point join), length
    struct {
        int a, b, len;
    } line[128];
    // a hybrid zone's switches (the adapter's fill; Black and White's Icirrus and Opelucid Gyms, 0 elsewhere): stepping onto
    // a lever's tile flips switch s (0..5) and leaves the player at x1,z1 (the lever's own tile, or where its script moves
    // the player: Opelucid's triggers hop it a tile on); a lever with s -1 is a lift (Black 2 / White 2's Opelucid Gym, where
    // sw is the gym's state, not bits): it runs only while sw is `on`, and leaves sw at `to` and the player at x1,z1 (a tile
    // of another state's lift is walked over as any tile); walking onto a place's tile in dir (0 up 1 down 2 left 3 right) hands the player
    // onto the rail at `to` (the zone's script does it) when switch s is in state want; an arm is actor npc[npc], which
    // stands at x1,z1 once its switch s is flipped. sw: the switches' states now (bit s). Switches that turn rail lines on
    // and off (Opelucid's dragons): the adapter's railSwitches. Switches that change the grid itself (Black 2 / White 2's
    // Plasma Frigate: each switch raises or lowers four floor panels): the adapter's gridSwitches. leverOnce 1: the switch
    // pressed last (leverLast, -1 none) does nothing until another is pressed (its tiles are walked over as any tile)
    int nlever, nplace, narm;
    unsigned sw;
    int leverOnce, leverLast;
    struct {
        int x, z, s, x1, z1, on, to;
    } lever[PORT_MAP_LEVERS];
    struct {
        int x, z, dir, s, want;
        PortRailPos to;
    } place[8];
    struct {
        int npc, x1, z1, s;
    } arm[8];
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
    // optional (rail maps): 1 when the player is on a rail now, with *p its place (cheap: each frame of a walk)
    int (*railAt)(PortRailPos *p);
    // optional (rail maps): where a step in dir (0 up 1 down 2 left 3 right) from *from leads (*to, also when the step is
    // not taken: the warp or actor walked into there); 1 the game takes the step (on the rail, the tile free; actors not
    // counted)
    int (*railStep)(const PortRailPos *from, int dir, PortRailPos *to);
    // optional (hybrid zones: a grid and rails in one zone, the game swaps the player between them where a key is held
    // from a switch tile toward a wall): on a rail at *p, the key held in dir: 1 the player steps off onto the grid, 2 it
    // might (the grid there is not loaded yet), 0 not
    int (*railToGrid)(const PortRailPos *p, int dir);
    // optional (hybrid zones): on grid tile x,z, the key held in dir: 1 the player steps onto a rail (*to: where), 0 not.
    // Only asked on the fill's PORT_MAP_SWITCH tiles
    int (*gridToRail)(int x, int z, int dir, PortRailPos *to);
    // optional: G:wall's pass-through actors (a wall the player walks through by holding a key at the cell before it, once it
    // is open: Black and White's honey walls): the tiles beyond the actor's cell the pass ends on (the player's walk is a
    // straight line through it), 0 when the actor is none. NULL: the game has none
    int (*passActor)(unsigned zone, const PortMapNpc *n);
    // optional: G:ride's rides (a vehicle the player boards by holding a key into its entry: Castelia Gym's cocoons in Black 2
    // and White 2), n the index in the game's own ride table: *x,*z the tile to stand on, *y its floor (the player's
    // height: PortMap's py, the dump's y), *dir the key to hold (0 up 1 down 2 left 3 right). 1 a ride, 0 no ride n here, -1 closed (its switch not
    // done yet). NULL: the game has none
    // *y comes in as the player's floor: an adapter whose rides are on one floor with the player leaves it
    int (*ride)(unsigned zone, int n, int *x, int *z, int *y, int *dir);
    // optional: a ride whose car runs on its own schedule (Nimbasa's roller coaster; n as for ride): -1 no such car here, else a
    // bit set: 1 the player rides it, 2 its car stands at a station. RideStep waits up to 2400 frames for such a car, holds Down once
    // the car has left and stands at a station again (the game's exit), and boards again after a first-boarding intro scene
    int (*rideState)(unsigned zone, int n);
    // optional: why ride n answered -1 (a line for the plan's why); NULL: the generic "closed"
    const char *(*rideWhy)(unsigned zone, int n);
    // optional (hybrid zones; with railToGrid): the grid tile x,z (world) the player is put on when the key in dir is held at rail
    // place *p and railToGrid says 1 or 2 (the rail place's own tile: the key then walks on); 1 when it is known. The planner's
    // grid -> rail -> grid chains need it (route_map.c's Search)
    int (*railLand)(const PortRailPos *p, int dir, int *x, int *z);
    // optional (PortMap's switches that turn rail lines on and off: Black and White's Opelucid Gym dragons): set 1: the zone's
    // rails as they are with the switches in states sw (railStep, gridToRail and railToGrid then answer for those), set 0: back
    // as the game has them. The planner's switch search calls it around each search and puts the rails back after it
    void (*railSwitches)(int set, unsigned sw);
    // optional (PortMap's switches that change the grid: the Plasma Frigate's floor panels): set 1: refill *m as the zone is
    // with switches sw flipped from now (each bit a switch pressed once; the cells' heights and passes change, the lists stay),
    // set 0: refill it as the game has it now. The planner's switch search calls it around each search and once after it
    void (*gridSwitches)(PortMap *m, int set, unsigned sw);
    // optional (Gen 4; token Bk): put the bicycle on the Y button as the bag's Register does, when the bag holds it: 1 done, 0 no
    // bicycle (or no field). A following Ky:4 mounts or dismounts it as the player's Y does
    int (*bike)(void);
    // optional (Gen 4: the Cycling Road's bridge): 1 while the field pushes the player on when no key is held, so that a
    // one-frame tap of a direction key only turns him (the next frame's push turns him back) and the walk's precise-stop mode
    // (route_test.c WalkTo: a tap every 4th frame within one tile of the waypoint) never steps: the pilot then holds the key
    int (*pushed)(void);
    // optional (Gen 4): -1 when the grid tile is on no line of sight of a trainer who has not been beaten (up to his sight range, the
    // four directions); else bit 0: one looks along his line to it now, the other bits change when one of them turns. The pilot stops
    // at the edge of such a line and waits (bounded) for a turn (then for him to look away): a trainer who turns at random sees a
    // player who stands in his line, walks up to him and stands in the way after the fight (Veilstone Gym's first room, one door)
    int (*watched)(int gx, int gz);
} PortRouteAdapter;

// the adapter's fill calls it last: a walkable tile next to an unloaded one ('?') may step onto it, and so may the water
// while the player surfs (Black 2's Seaside Cave, zone 515: the channel north from the lower chunk's lake runs into the
// unloaded upper chunk; without it a surfing G:X,Z there had no path, pz9 2026-11-13)
// It first fills the '?' cells from the cells seen loaded before (in this zone; route_map.c's Recall): a chunk walked away
// from is not guessed open again (Black 2's Giant Chasm, zone 233, pz17)
void PortMap_OpenUnloaded(PortMap *m);
// forget the remembered cells (the planner's unit test, between its maps)
void PortMap_Forget(void);

// the game's field frame hook registers its adapter (each frame: cheap; NULL: no field)
void PortRouteMap_SetAdapter(const PortRouteAdapter *a);
const PortRouteAdapter *PortRouteMap_Adapter(void);

#endif
