#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_41_sprite_resource_collection.h"
#include "g2d_renderer.h"
#include "sprite_resource_manager.h"
#include "mail_message_types.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"

struct Background;

struct BallCapsule;

struct BallSeal;

struct BgConfig;

struct BoxPokemon;

struct MailMessage;

struct Mail;

struct PartyPokemon;

struct Pokemon;

struct PokemonDataBlockA;

struct PokemonDataBlockB;

struct PokemonDataBlockC;

struct PokemonDataBlockD;

struct PokepicTemplate;

struct GF_2DGfxRawResObj;

struct GF_2DGfxRawResMan;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct GF_2DGfxResObjList;

struct SpriteResourceManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct TileRegion;

typedef struct Background {
    void *tilemapBuffer;
    u32 bufferSize;
    u32 baseTile;
    int hOffset;
    int vOffset;
    u8 mode;
    u8 size;
    u8 colorMode;
    u8 tileSize;
    u16 rotation;
    u16 unk22;
    fx32 xScale;
    fx32 yScale;
    int centerX;
    int centerY;
} Background;

typedef struct BgConfig {
    enum HeapID heapID;
    u16 scrollScheduled;
    u16 bufferTransferScheduled;
    struct Background bgs[8];
} BgConfig;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef struct BallCapsule {
    BallSeal seals[8];
} BallCapsule;

typedef u16 charcode_t;

typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;

typedef struct Mail {
    u32 trainerID;
    u8 trainerGender;
    u8 language;
    u8 gameVersion;
    u8 mailType;
    charcode_t trainerName[7 + 1];
    MailIconData iconData[3];
    u16 platExclusiveFormIcons;
    MailMessage sentences[3];
} Mail;

typedef struct PokemonDataBlockA {
    u16 species;
    u16 heldItem;

    u32 otID;
    u32 exp;

    u8 friendship;
    u8 ability;
    u8 markings;
    u8 originLanguage;

    u8 hpEV;
    u8 atkEV;
    u8 defEV;
    u8 speedEV;
    u8 spAtkEV;
    u8 spDefEV;

    u8 cool;
    u8 beauty;
    u8 cute;
    u8 smart;
    u8 tough;
    u8 sheen;

    u32 ribbonsDS1;
} PokemonDataBlockA;

typedef struct PokemonDataBlockB {
    u16 moves[4];
    u8 moveCurrentPPs[4];
    u8 movePPUps[4];
    u32 hpIV : 5;
    u32 atkIV : 5;
    u32 defIV : 5;
    u32 speedIV : 5;
    u32 spAtkIV : 5;
    u32 spDefIV : 5;
    u32 isEgg : 1;
    u32 hasNickname : 1;
    u32 ribbonsGBA;
    u8 fatefulEncounter : 1;
    u8 gender : 2;
    u8 form : 5;
    u8 HGSS_shinyLeaves : 6;
    u8 unused1 : 2;
    u16 unused2;
    u16 EggLocation_PtHGSS;
    u16 MetLocation_PtHGSS;
} PokemonDataBlockB;

typedef struct PokemonDataBlockC {
    charcode_t nickname[10 + 1];

    u8 unused;
    u8 originGame;

    u64 ribbonsDS2;
} PokemonDataBlockC;

typedef struct PokemonDataBlockD {
    u16 otName[7 + 1];
    u8 eggYear;
    u8 eggMonth;
    u8 eggDay;
    u8 metYear;
    u8 metMonth;
    u8 metDay;
    u16 EggLocation_DP;
    u16 MetLocation_DP;
    u8 pokerus;
    u8 pokeball;
    u8 metLevel : 7;
    u8 otGender : 1;
    u8 metTerrain;
    u8 HGSS_Pokeball;
    s8 mood;
} PokemonDataBlockD;

typedef struct BoxPokemon {
    u32 personality;
    u16 partyDecrypted : 1;
    u16 boxDecrypted : 1;
    u16 checksumFailed : 1;
    u16 unused : 13;
    u16 checksum;
    union { PokemonDataBlockA blockA; PokemonDataBlockB blockB; PokemonDataBlockC blockC; PokemonDataBlockD blockD; } dataBlocks[4];
} BoxPokemon;

typedef struct PartyPokemon {
    u32 status;
    u8 level;
    u8 ballCapsuleID;
    u16 hp;
    u16 maxHP;
    u16 attack;
    u16 defense;
    u16 speed;
    u16 spAtk;
    u16 spDef;
    Mail mail;
    BallCapsule ballCapsule;
} PartyPokemon;

typedef struct Pokemon {
    BoxPokemon box;
    PartyPokemon party;
} Pokemon;

typedef struct PokepicTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
} PokepicTemplate;

typedef struct SpriteList SpriteList;

typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;

struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct GF_2DGfxRawResObj {
    int id;
    void *data;
} GF_2DGfxRawResObj;

typedef struct GF_2DGfxRawResMan {
    GF_2DGfxRawResObj *resources;
    int capacity;
    int count;
} GF_2DGfxRawResMan;

typedef struct SpriteResource {
    GF_2DGfxRawResObj *rawResource;
    GfGfxResType type;
    void *data;
} SpriteResource;

typedef struct GF_2DGfxResObjList {
    SpriteResource **resources;
    int capacity;
    int count;
} GF_2DGfxResObjList;

SpriteResource *SpriteResourceCollection_Find(const SpriteResourceCollection *spriteResources, int id);

typedef struct TileRegion {
    int x;
    int y;
    int width;
    int height;
} TileRegion;

void sub_020143E0(enum NarcId narcID, int narcIdx, enum HeapID heapID, const TileRegion *region, void *dest);

void *sub_02014450(enum NarcId narcID, int paletteIndex, enum HeapID heapID);

void *Heap_Alloc(u32 heapID, u32 size);

void Heap_Free(void *ptr);

NNSG2dImageProxy *SpriteTransfer_GetCharProxy(const SpriteResource *resource);

const NNSG2dImagePaletteProxy *SpriteTransfer_GetPaletteProxy(const SpriteResource *resource, NNSG2dImageProxy *imageProxy);

u8 *DrawPokemonPreview(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 palette, u16 baseTile, u16 species, u8 gender, enum HeapID heapID);

u8 *DrawPokemonPreviewFromStruct(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 palette, u16 baseTile, Pokemon *mon, enum HeapID heapID);

void sub_0200F748(SpriteResourceManager *param0, PokepicTemplate *spriteTemplate);

extern const TileRegion _020F5C50;

extern const TileRegion _020F5C40;

void sub_0200F748(SpriteResourceManager *param0, PokepicTemplate *spriteTemplate) {
    u8 *buf;
    u32 offset;
    SpriteResource *charResource, *plttResource;
    NNSG2dImageProxy *imageProxy;
    const NNSG2dImagePaletteProxy *paletteProxy;

    buf = Heap_Alloc(param0->heapID, (((8 * (8 / 2)) * (10 * 10)) * 2));

    TileRegion frame0Region = _020F5C50;
    sub_020143E0((enum NarcId)(spriteTemplate->narcID), spriteTemplate->character, (enum HeapID)(param0->heapID), &frame0Region, buf);

    TileRegion frame1Region = _020F5C40;
    sub_020143E0((enum NarcId)(spriteTemplate->narcID), spriteTemplate->character, (enum HeapID)(param0->heapID), &frame1Region, buf + ((8 * (8 / 2)) * (10 * 10)));

    charResource = SpriteResourceCollection_Find(param0->resourceCollections[GF_GFX_RES_TYPE_CHAR], 89301);
    imageProxy = SpriteTransfer_GetCharProxy(charResource);
    offset = NNS_G2dGetImageLocation(imageProxy, NNS_G2D_VRAM_TYPE_2DMAIN);

    DC_FlushRange(buf, (((8 * (8 / 2)) * (10 * 10)) * 2));
    GX_LoadOBJ(buf, offset, (((8 * (8 / 2)) * (10 * 10)) * 2));

    Heap_Free(buf);

    buf = sub_02014450((enum NarcId)(spriteTemplate->narcID), spriteTemplate->palette, (enum HeapID)(param0->heapID));
    plttResource = SpriteResourceCollection_Find(param0->resourceCollections[GF_GFX_RES_TYPE_PLTT], 89301);
    paletteProxy = SpriteTransfer_GetPaletteProxy(plttResource, imageProxy);
    offset = NNS_G2dGetImagePaletteLocation(paletteProxy, NNS_G2D_VRAM_TYPE_2DMAIN);

    DC_FlushRange(buf, 32);
    GX_LoadOBJPltt(buf, offset, 32);

    Heap_Free(buf);
}
