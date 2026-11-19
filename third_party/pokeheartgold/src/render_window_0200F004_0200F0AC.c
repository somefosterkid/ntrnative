#include "global.h"

#include "constants/heap.h"

#include "wild_encounter.h"
#include "sys_task.h"
#include "filesystem_files_def.h"

typedef enum HeapID HeapID;

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

struct SysTask;

struct SysTaskManager;

struct WaitingIcon;

struct Window;

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

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

extern u32 BgConfig_GetHeapId(BgConfig *bgConfig);

void *BgGetCharPtr(u8 bgLayer);

u8 GetWindowBgId(Window *window);

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef struct BallCapsule {
    BallSeal seals[8];
} BallCapsule;

typedef u16 charcode_t;

typedef struct MailMessage {
    u16 type;
    u16 id;
    u16 words[2];
} MailMessage;

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

typedef struct SysTaskManager SysTaskManager;

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

typedef struct WaitingIcon WaitingIcon;

void *GfGfxLoader_GetCharData(enum NarcId narcID, u32 narcMemberIdx, BOOL compressed, NNSG2dCharacterData **outCharData, enum HeapID heapID);

void *Heap_Alloc(u32 heapID, u32 size);

void Heap_Free(void *ptr);

SysTask * SysTask_CreateOnVBlankQueue(SysTaskFunc func, void *data, u32 priority);

enum WaitDialDrawMode {
    DIAL_DRAW_MODE_LOAD_ONLY = 0,
    DIAL_DRAW_MODE_LOAD_AND_DRAW,
    DIAL_DRAW_MODE_CLEAR,
};

struct WaitingIcon {
    Window *window;
    u8 pixels[(((8 * (8 / 2)) * (2 * 2)) * 8)];
    u8 messageBoxPixels[((8 * (8 / 2)) * (2 * 2))];
    u16 messageBoxTile;
    u8 counter;
    u8 curFrame : 7;
    u8 : 1;
    u8 deleteMode : 2;
    u8 : 6;
};

void sub_0200EA24(void *srcPixels, u16 srcX, u16 srcY, u16 srcWidth, u16 srcHeight, void *destPixels, u16 destWidth, u16 destHeight, u16 destX, u16 destY, u16 blitWidth, u16 blitHeight);

void sub_0200F1D4(WaitingIcon *dial, u32 drawMode);

extern void sub_0200F3D0(SysTask *task, void *data);

u8 *DrawPokemonPreview(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 palette, u16 baseTile, u16 species, u8 gender, enum HeapID heapID);

u8 *DrawPokemonPreviewFromStruct(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 palette, u16 baseTile, Pokemon *mon, enum HeapID heapID);

void *WaitingIcon_New(Window *window, u32 baseTile);

void *WaitingIcon_New(Window *window, u32 baseTile) {
    WaitingIcon *dial;
    u32 heapID;
    u8 *bgCharPtr;
    u8 *dialTiles;
    u8 *tmp;
    void *dialTilesRaw;
    u8 bgLayer;
    u8 i;
    NNSG2dCharacterData *dialCharData;

    heapID = BgConfig_GetHeapId(window->bgConfig);
    bgLayer = GetWindowBgId(window);
    bgCharPtr = BgGetCharPtr(bgLayer);
    dial = Heap_Alloc(heapID, sizeof(WaitingIcon));

    memcpy(dial->messageBoxPixels, &bgCharPtr[(baseTile + 18) * (8 * (8 / 2))], ((8 * (8 / 2)) * (2 * 2)));

    tmp = Heap_Alloc(heapID, ((8 * (8 / 2)) * (2 * 2)));

    memcpy(&tmp[(8 * (8 / 2)) * 0], &bgCharPtr[(baseTile + 10) * (8 * (8 / 2))], (8 * (8 / 2)));
    memcpy(&tmp[(8 * (8 / 2)) * 1], &bgCharPtr[(baseTile + 11) * (8 * (8 / 2))], (8 * (8 / 2)));
    memcpy(&tmp[(8 * (8 / 2)) * 2], &bgCharPtr[(baseTile + 10) * (8 * (8 / 2))], (8 * (8 / 2)));
    memcpy(&tmp[(8 * (8 / 2)) * 3], &bgCharPtr[(baseTile + 11) * (8 * (8 / 2))], (8 * (8 / 2)));

    for (i = 0; i < 8; i++) {
        memcpy(dial->pixels + (((8 * (8 / 2)) * (2 * 2)) * i), tmp, ((8 * (8 / 2)) * (2 * 2)));
    }

    Heap_Free(tmp);

    dialTilesRaw = GfGfxLoader_GetCharData(NARC_a_0_3_8,
        23,
        0,
        &dialCharData,
        (HeapID)heapID);
    dialTiles = dialCharData->pRawData;

    sub_0200EA24(dialTiles,
        0,
        0,
        2 * 8,
        (2 * 8) * 8,
        dial->pixels,
        2 * 8,
        (2 * 8) * 8,
        0,
        0,
        2 * 8,
        (2 * 8) * 8);
    Heap_Free(dialTilesRaw);

    dial->window = window;
    dial->messageBoxTile = baseTile;
    dial->counter = 0;
    dial->curFrame = 0;
    dial->deleteMode = 0;

    SysTask_CreateOnVBlankQueue(sub_0200F3D0, dial, 0);
    sub_0200F1D4(dial, DIAL_DRAW_MODE_LOAD_AND_DRAW);

    return dial;
}
