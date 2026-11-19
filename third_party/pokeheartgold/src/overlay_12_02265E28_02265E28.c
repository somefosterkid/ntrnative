#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"

struct BattleSystem;

struct ManagedSprite;

struct NARC;

struct PaletteData;

struct SpriteManager;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct Terrain;

typedef struct BattleSystem BattleSystem;

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

typedef struct SpriteResourcesHeaderList SpriteResourcesHeaderList;

BOOL SpriteSystem_LoadCellResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

typedef struct Terrain {
    ManagedSprite *managedSprite;
    BattleSystem *battleSys;
    u8 side;
    u8 terrainType;
    s16 unused_0A;
    s16 unused_0C;
} Terrain;

enum BattleTerrain {
    TERRAIN_PLAIN = 0,
    TERRAIN_SAND = 1,
    TERRAIN_GRASS = 2,
    TERRAIN_PUDDLE = 3,
    TERRAIN_MOUNTAIN = 4,
    TERRAIN_CAVE = 5,
    TERRAIN_SNOW = 6,
    TERRAIN_WATER = 7,
    TERRAIN_ICE = 8,
    TERRAIN_BUILDING = 9,
    TERRAIN_GREAT_MARSH = 10,
    TERRAIN_BRIDGE = 11,
    TERRAIN_AARON = 12,
    TERRAIN_BERTHA = 13,
    TERRAIN_FLINT = 14,
    TERRAIN_LUCIAN = 15,
    TERRAIN_CYNTHIA = 16,
    TERRAIN_DISTORTION_WORLD = 17,
    TERRAIN_BATTLE_TOWER = 18,
    TERRAIN_BATTLE_FACTORY = 19,
    TERRAIN_BATTLE_ARCADE = 20,
    TERRAIN_BATTLE_CASTLE = 21,
    TERRAIN_BATTLE_HALL = 22,
    TERRAIN_GIRATINA = 23,
    TERRAIN_MAX = 24,
};

SpriteSystem *BattleSystem_GetSpriteSystem(BattleSystem *battleSys);

SpriteManager *BattleSystem_GetSpriteManager(BattleSystem *battleSys);

PaletteData *BattleSystem_GetPaletteData(BattleSystem *battleSys);

int ov12_0223B52C(BattleSystem *battleSys);

extern __attribute__((aligned(4))) const u16 ov12_0226E0D0[TERRAIN_MAX];

extern __attribute__((aligned(4))) const u16 ov12_0226E0A0[TERRAIN_MAX];

extern __attribute__((aligned(4))) const u16 ov12_0226E168[TERRAIN_MAX][3];

void ov12_02265E28(Terrain *terrain);

void ov12_02265E28(Terrain *terrain) {
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    int charNarcIdx, charResID, cellNarcIdx, cellResID, animNarcIdx, animResID;
    int bgTimeOffset;
    NARC *objNarc = NARC_New((enum NarcId)8, HEAP_ID_BATTLE);
    spriteSys = BattleSystem_GetSpriteSystem(terrain->battleSys);
    spriteMan = BattleSystem_GetSpriteManager(terrain->battleSys);
    bgTimeOffset = ov12_0223B52C(terrain->battleSys);

    if (terrain->side == 0) {
        charNarcIdx = ov12_0226E0D0[terrain->terrainType];
        charResID = 20013;
        cellNarcIdx = 128;
        cellResID = 20005;
        animNarcIdx = 129;
        animResID = 20005;
    } else {
        charNarcIdx = ov12_0226E0A0[terrain->terrainType];
        charResID = 20014;
        cellNarcIdx = 131;
        cellResID = 20006;
        animNarcIdx = 132;
        animResID = 20006;
    }

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSys, spriteMan, objNarc, charNarcIdx, 1, NNS_G2D_VRAM_TYPE_2DMAIN, charResID);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(BattleSystem_GetPaletteData(terrain->battleSys), PLTTBUF_MAIN_OBJ, spriteSys, spriteMan, objNarc, ov12_0226E168[terrain->terrainType][bgTimeOffset], 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 20009);
    PaletteData_LoadNarc(BattleSystem_GetPaletteData(terrain->battleSys), (enum NarcId)8, ov12_0226E168[terrain->terrainType][bgTimeOffset], HEAP_ID_BATTLE, PLTTBUF_MAIN_BG, ((16) * sizeof(u16)), ((7) * (16)));
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSys, spriteMan, objNarc, cellNarcIdx, 1, cellResID);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSys, spriteMan, objNarc, animNarcIdx, 1, animResID);
    NARC_Delete(objNarc);
}
