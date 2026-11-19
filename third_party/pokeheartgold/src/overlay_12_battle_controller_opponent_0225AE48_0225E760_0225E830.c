#include "global.h"

#include "battle/battler_sprite_context.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_07_battle_anim_system.h"

struct AIContext;

struct ActionSelectData;

struct Background;

struct Bag;

struct BagItem;

struct BagMenuData;

struct BallCapsule;

struct BallRotation;

struct BallSeal;

struct BattleAnimContext;

struct BattleAnimScriptLoop;

struct BattleAnimSystem;

struct BattleBackgroundReference;

struct BattleBagContext;

struct BattleBgAnim;

struct BattleContext;

struct BattleItemUse;

struct BattleMessage;

struct BattleMon;

struct BattleMonOBJData;

struct BattlePartyContext;

struct BattleScriptTaskData;

struct BattleSubscreen;

struct BattleSystem;

struct OpponentData;

struct BattlerDataSysTaskFuncs;

struct BgConfig;

struct BgTrackingTask;

struct BgWaveStrip;

struct BoxPokemon;

struct Camera;

struct CameraAngle;

struct CameraLookAt;

struct CameraPerspectiveProjection;

struct CameraPositionHistory;

struct CharTransferAllocation;

struct ChatotCry;

struct CoordinatesS16;

struct BattleCursor;

struct CustomBgScrollContext;

struct MailMessage;

struct FieldConditions;

struct TextOBJ;

struct FontOAMEntry;

struct G2dRenderer;

struct GiveUpMoveData;

struct BattleHpBar;

struct Indicator;

struct ItemData;

struct ItemPartyParam;

struct Mail;

struct ManagedSprite;

struct MenuButtonPressAnimState;

struct MenuCursor;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct MoveAnimation;

struct MoveContestData;

struct MoveDisplayData;

struct MoveDisplayInfo;

struct MoveEffectsData;

struct MoveFailFlags;

struct MoveSelectData;

struct MoveTable;

struct NARC;

struct PaletteAnimator;

struct PaletteData;

struct PaletteFadeControl;

struct ParticleSystem;

struct Party;

struct PartyBallAnimState;

struct PartyMenuData;

struct PartyPokemon;

struct Pokemon;

struct PokemonDataBlockA;

struct PokemonDataBlockB;

struct PokemonDataBlockC;

struct PokemonDataBlockD;

struct Pokepic;

struct PokemonSpriteDrawContext;

struct PokemonSpriteShadow;

struct PokepicTemplate;

struct PokemonSpriteTrackingTask;

struct PokemonSpriteTransforms;

struct GF_2DGfxRawResObj;

struct GF_2DGfxRawResMan;

struct SPLAlphaAnim;

struct SPLBehavior;

struct SPLChildResource;

struct SPLColorAnim;

struct SPLEmitter;

struct SPLEmitterList;

struct SPLEmitterState;

struct SPLManager;

struct SPLParticle;

struct SPLParticleList;

struct SPLResource;

struct SPLResourceHeader;

struct SPLScaleAnim;

struct SPLTexAnim;

struct SPLTexture;

struct SavedCursorPosition;

struct SelfTurnFlags;

struct SideConditions;

struct Sprite;

struct SpriteAnimFrame;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceCollection;

struct GF_2DGfxResObjList;

struct GF_2DGfxResHeader;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SpriteSystemContext;

struct ManagedSpriteTemplate;

struct SpriteTrackingTask;

struct SubscreenTilemapRect;

struct SysTask;

struct SysTaskManager;

struct TargetMonData;

struct TargetSelectData;

struct TextWindowLayout;

struct PlayerProfile;

struct TurnFlags;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct WaveAnimContext;

struct Window;

typedef struct BattleSystem BattleSystem;

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

typedef struct PaletteFadeControl {
    u16 unfadedMask;
    u16 wait : 6;
    u16 cur : 5;
    u16 end : 5;
    u16 target : 15;
    u16 sign : 1;
    u16 step : 4;
    u16 waitStep : 6;
    u16 unused : 6;
} PaletteFadeControl;

u8 PaletteData_BeginPaletteFade(PaletteData *paletteData, u16 buffersToFade, u16 palettesToFade, s8 wait, u8 cur, u8 end, u16 target);

u16 PaletteData_GetSelectedBuffersBitmask(PaletteData *paletteData);

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

typedef struct ManagedSprite {
    Sprite *sprite;
    SpriteResourcesHeader *resourceHeader;
    SpriteResourcesHeaderList *resourceHeaderList;
    BOOL vramTransfer;
} ManagedSprite;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

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

typedef struct SpriteResourceCollection {
    GF_2DGfxRawResMan *collection;
    SpriteResource *resources;
    void *capacity;
    void *count;
    GfGfxResType type;
} SpriteResourceCollection;

typedef struct GF_2DGfxResHeader {
    void *entries;
    int count;
    GfGfxResType type;
    u8 loadFromNARC;
} GF_2DGfxResHeader;

typedef struct GF_2DGfxResObjList {
    SpriteResource **resources;
    int capacity;
    int count;
} GF_2DGfxResObjList;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

typedef struct SpriteSystem {
    enum HeapID heapID;
    int spriteManagerCount;
    BOOL inUse;
    NNSG2dCellTransferState *cellTransferStates;
    G2dRenderer renderer;
} SpriteSystem;

typedef struct SpriteManager {
    SpriteList *sprites;
    SpriteResourcesHeaderList *resourceHeaders;
    GF_2DGfxResHeader *resourcePaths;
    SpriteResourceCollection *ownedResources[GF_GFX_RES_TYPE_MAX];
    GF_2DGfxResObjList *unownedResources[GF_GFX_RES_TYPE_MAX];
    int loadedResourceCount[GF_GFX_RES_TYPE_MAX];
    int numResourceTypes;
} SpriteManager;

typedef struct SysTask SysTask;

typedef struct SysTaskManager SysTaskManager;

typedef void (*SysTaskFunc)(SysTask *, void *);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};

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

typedef struct BattleHpBar {
    u8 state;
    ManagedSprite *mainSprite;
    ManagedSprite *arrowSprite;
    BattleSystem *battleSys;
    SysTask *task_10;
    Window unused_14;
    u8 battler;
    u8 type;
    u8 selectedPartySlot;
    u8 numSafariBalls;
    s32 curHP;
    s32 maxHP;
    s32 damage;
    s32 hpTemp;
    s32 curExp;
    s32 maxExp;
    s32 expReward;
    s32 expTemp;
    u8 level;
    u8 gender;
    u8 status;
    u8 caughtSpecies;
    u8 command;
    u8 delay;
    u8 expSoundTimer;
    u8 scrollDirection : 1;
    u8 doneScrolling : 1;
    u8 unused_4F_2 : 1;
    u8 numberMode : 1;
    u8 padding : 4;
    SysTask *bobbingAnimTask;
    u16 degrees;
} BattleHpBar;

void ov12_0226498C(BattleHpBar *healthbox, u32 hp, u32 flags);

void ov12_02264DCC(BattleHpBar *healthbox, int damage);

s32 ov12_02264E00(BattleHpBar *healthbox);

u8 BattleHpBar_Util_GetBarTypeFromBattlerSide(u8 battlerType, u32 battleType);

typedef struct BallRotation BallRotation;

typedef struct SpriteSystemContext {
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    PaletteData *plttData;
} SpriteSystemContext;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef struct Pokepic Pokepic;

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);

typedef struct PokepicTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
} PokepicTemplate;

struct PokemonSpriteTransforms {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};

typedef struct PokemonSpriteShadow {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
} PokemonSpriteShadow;

struct Pokepic {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokepicTemplate template;
    PokepicTemplate templateBackup;
    PokemonSpriteTransforms transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokemonSpriteShadow shadow;
    PokemonSpriteShadow shadowBackup;
    SpriteAnimFrame animFrames[10];
};

typedef struct BattleMonOBJData {
    enum HeapID heapID;
    int battlerIdx;
    SpriteSystemContext spriteContext;
    int resourceIDs[4];
    ManagedSprite *managedSprites[4];
    PokemonSpriteData *pokemonSpriteDataArray[4];
    u8 battlerTypes[4];
    Pokepic *pokemonSprites[4];
} BattleMonOBJData;

typedef struct SavedCursorPosition {
    s8 actionMenuX;
    s8 actionMenuY;
    s8 moveMenuX;
    s8 moveMenuY;
    s8 targetSelectX;
    s8 targetSelectY;
    s8 targetingLayout;
    u8 padding;
} SavedCursorPosition;

typedef struct BattlerDataSysTaskFuncs {
    SysTaskFunc setCommandSelection;
    SysTaskFunc showMoveSelectMenu;
    SysTaskFunc showTargetSelectMenu;
    SysTaskFunc showBagMenu;
    SysTaskFunc showPartyMenu;
    SysTaskFunc showYesNoMenu;
} BattlerDataSysTaskFuncs;

typedef struct OpponentData {
    BattlerDataSysTaskFuncs taskFuncs;
    ManagedSprite *managedSprite;
    Pokepic *monSpriteBackup;
    Pokepic *monSprite;
    Window *unused_24;
    BattleHpBar healthbox;
    SavedCursorPosition savedCursorPos;
    BallRotation *ballRotation;
    BattleMonOBJData *btlMonObjData;
    void *unused_8C;
    u8 data[256];
    u8 battler;
    u8 battlerType;
    u8 bootState;
    u8 commandUIReady;
    SysTask *sysTask;
    u16 degrees;
    int isAlternateSprite;
    NARC *narc;
    u8 msgPending;
    u8 unused_1A5[3];
} OpponentData;

enum BattleAction {
    BATTLE_ACTION_PICK_COMMAND = 0,
    BATTLE_ACTION_CHOOSE_TARGET = 1,
    BATTLE_ACTION_TEMP_VALUE = 2,
    BATTLE_ACTION_SELECTED_COMMAND = 3,
    MAX_BATTLE_ACTIONS = 4,
};

enum BattleMessageTag {
    TAG_NONE = 0,
    TAG_NONE_SIDE_CONSCIOUS = 1,
    TAG_NICKNAME = 2,
    TAG_MOVE = 3,
    TAG_STAT = 4,
    TAG_ITEM = 5,
    TAG_NUMBER = 6,
    TAG_NUMBERS = 7,
    TAG_TRNAME = 8,
    TAG_NICKNAME_NICKNAME = 9,
    TAG_NICKNAME_MOVE = 10,
    TAG_NICKNAME_ABILITY = 11,
    TAG_NICKNAME_STAT = 12,
    TAG_NICKNAME_TYPE = 13,
    TAG_NICKNAME_POKE = 14,
    TAG_NICKNAME_ITEM = 15,
    TAG_NICKNAME_POFFIN = 16,
    TAG_NICKNAME_NUM = 17,
    TAG_NICKNAME_TRNAME = 18,
    TAG_NICKNAME_BOX = 19,
    TAG_MOVE_SIDE = 20,
    TAG_MOVE_NICKNAME = 21,
    TAG_MOVE_MOVE = 22,
    TAG_ABILITY_NICKNAME = 23,
    TAG_ITEM_MOVE = 24,
    TAG_NUMBER_NUMBER = 25,
    TAG_TRNAME_TRNAME = 26,
    TAG_TRNAME_NICKNAME = 27,
    TAG_TRNAME_ITEM = 28,
    TAG_TRNAME_NUM = 29,
    TAG_TRCLASS_TRNAME = 30,
    TAG_NICKNAME_NICKNAME_MOVE = 31,
    TAG_NICKNAME_NICKNAME_ABILITY = 32,
    TAG_NICKNAME_NICKNAME_ITEM = 33,
    TAG_NICKNAME_MOVE_MOVE = 34,
    TAG_NICKNAME_MOVE_NUMBER = 35,
    TAG_NICKNAME_ABILITY_NICKNAME = 36,
    TAG_NICKNAME_ABILITY_MOVE = 37,
    TAG_NICKNAME_ABILITY_ITEM = 38,
    TAG_NICKNAME_ABILITY_STAT = 39,
    TAG_NICKNAME_ABILITY_TYPE = 40,
    TAG_NICKNAME_ABILITY_STATUS = 41,
    TAG_NICKNAME_ABILITY_NUMBER = 42,
    TAG_NICKNAME_ITEM_NICKNAME = 43,
    TAG_NICKNAME_ITEM_MOVE = 44,
    TAG_NICKNAME_ITEM_STAT = 45,
    TAG_NICKNAME_ITEM_STATUS = 46,
    TAG_NICKNAME_BOX_BOX = 47,
    TAG_ITEM_NICKNAME_FLAVOR = 48,
    TAG_TRNAME_NICKNAME_NICKNAME = 49,
    TAG_TRCLASS_TRNAME_NICKNAME = 50,
    TAG_TRCLASS_TRNAME_ITEM = 51,
    TAG_NICKNAME_ABILITY_NICKNAME_MOVE = 52,
    TAG_NICKNAME_ABILITY_NICKNAME_ABILITY = 53,
    TAG_NICKNAME_ABILITY_NICKNAME_STAT = 54,
    TAG_NICKNAME_ITEM_NICKNAME_ITEM = 55,
    TAG_TRNAME_NICKNAME_TRNAME_NICKNAME = 56,
    TAG_TRCLASS_TRNAME_NICKNAME_NICKNAME = 57,
    TAG_TRCLASS_TRNAME_NICKNAME_TRNAME = 58,
    TAG_TRCLASS_TRNAME_TRCLASS_TRNAME = 59,
    TAG_TRCLASS_TRNAME_NICKNAME_TRCLASS_TRNAME_NICKNAME = 60,
};

typedef struct MoveAnimation {
    u8 command;
    u8 unk_01;
    u16 move;
    s32 damage;
    u16 power;
    u16 effectChance;
    u16 friendship;
    u16 isSubstitute : 1;
    u16 isTransformed : 1;
    u16 unk_0E_2 : 1;
    u16 unk_0E_3 : 1;
    u16 unk_0E_4 : 12;
    u32 fieldConditions;
    u16 attacker;
    u16 defender;
    u16 species[4];
    u8 genders[4];
    u8 isShiny[4];
    u8 formNums[4];
    u32 personalities[4];
    u32 moveEffectMasks[4];
    int animMode;
    int secondaryAnimID;
    int terrain;
} MoveAnimation;

enum Move {
    MOVE_NONE = 0,
    MOVE_POUND = 1,
    MOVE_KARATE_CHOP = 2,
    MOVE_DOUBLE_SLAP = 3,
    MOVE_COMET_PUNCH = 4,
    MOVE_MEGA_PUNCH = 5,
    MOVE_PAY_DAY = 6,
    MOVE_FIRE_PUNCH = 7,
    MOVE_ICE_PUNCH = 8,
    MOVE_THUNDER_PUNCH = 9,
    MOVE_SCRATCH = 10,
    MOVE_VICE_GRIP = 11,
    MOVE_GUILLOTINE = 12,
    MOVE_RAZOR_WIND = 13,
    MOVE_SWORDS_DANCE = 14,
    MOVE_CUT = 15,
    MOVE_GUST = 16,
    MOVE_WING_ATTACK = 17,
    MOVE_WHIRLWIND = 18,
    MOVE_FLY = 19,
    MOVE_BIND = 20,
    MOVE_SLAM = 21,
    MOVE_VINE_WHIP = 22,
    MOVE_STOMP = 23,
    MOVE_DOUBLE_KICK = 24,
    MOVE_MEGA_KICK = 25,
    MOVE_JUMP_KICK = 26,
    MOVE_ROLLING_KICK = 27,
    MOVE_SAND_ATTACK = 28,
    MOVE_HEADBUTT = 29,
    MOVE_HORN_ATTACK = 30,
    MOVE_FURY_ATTACK = 31,
    MOVE_HORN_DRILL = 32,
    MOVE_TACKLE = 33,
    MOVE_BODY_SLAM = 34,
    MOVE_WRAP = 35,
    MOVE_TAKE_DOWN = 36,
    MOVE_THRASH = 37,
    MOVE_DOUBLE_EDGE = 38,
    MOVE_TAIL_WHIP = 39,
    MOVE_POISON_STING = 40,
    MOVE_TWINEEDLE = 41,
    MOVE_PIN_MISSILE = 42,
    MOVE_LEER = 43,
    MOVE_BITE = 44,
    MOVE_GROWL = 45,
    MOVE_ROAR = 46,
    MOVE_SING = 47,
    MOVE_SUPERSONIC = 48,
    MOVE_SONIC_BOOM = 49,
    MOVE_DISABLE = 50,
    MOVE_ACID = 51,
    MOVE_EMBER = 52,
    MOVE_FLAMETHROWER = 53,
    MOVE_MIST = 54,
    MOVE_WATER_GUN = 55,
    MOVE_HYDRO_PUMP = 56,
    MOVE_SURF = 57,
    MOVE_ICE_BEAM = 58,
    MOVE_BLIZZARD = 59,
    MOVE_PSYBEAM = 60,
    MOVE_BUBBLE_BEAM = 61,
    MOVE_AURORA_BEAM = 62,
    MOVE_HYPER_BEAM = 63,
    MOVE_PECK = 64,
    MOVE_DRILL_PECK = 65,
    MOVE_SUBMISSION = 66,
    MOVE_LOW_KICK = 67,
    MOVE_COUNTER = 68,
    MOVE_SEISMIC_TOSS = 69,
    MOVE_STRENGTH = 70,
    MOVE_ABSORB = 71,
    MOVE_MEGA_DRAIN = 72,
    MOVE_LEECH_SEED = 73,
    MOVE_GROWTH = 74,
    MOVE_RAZOR_LEAF = 75,
    MOVE_SOLAR_BEAM = 76,
    MOVE_POISON_POWDER = 77,
    MOVE_STUN_SPORE = 78,
    MOVE_SLEEP_POWDER = 79,
    MOVE_PETAL_DANCE = 80,
    MOVE_STRING_SHOT = 81,
    MOVE_DRAGON_RAGE = 82,
    MOVE_FIRE_SPIN = 83,
    MOVE_THUNDER_SHOCK = 84,
    MOVE_THUNDERBOLT = 85,
    MOVE_THUNDER_WAVE = 86,
    MOVE_THUNDER = 87,
    MOVE_ROCK_THROW = 88,
    MOVE_EARTHQUAKE = 89,
    MOVE_FISSURE = 90,
    MOVE_DIG = 91,
    MOVE_TOXIC = 92,
    MOVE_CONFUSION = 93,
    MOVE_PSYCHIC = 94,
    MOVE_HYPNOSIS = 95,
    MOVE_MEDITATE = 96,
    MOVE_AGILITY = 97,
    MOVE_QUICK_ATTACK = 98,
    MOVE_RAGE = 99,
    MOVE_TELEPORT = 100,
    MOVE_NIGHT_SHADE = 101,
    MOVE_MIMIC = 102,
    MOVE_SCREECH = 103,
    MOVE_DOUBLE_TEAM = 104,
    MOVE_RECOVER = 105,
    MOVE_HARDEN = 106,
    MOVE_MINIMIZE = 107,
    MOVE_SMOKE_SCREEN = 108,
    MOVE_CONFUSE_RAY = 109,
    MOVE_WITHDRAW = 110,
    MOVE_DEFENSE_CURL = 111,
    MOVE_BARRIER = 112,
    MOVE_LIGHT_SCREEN = 113,
    MOVE_HAZE = 114,
    MOVE_REFLECT = 115,
    MOVE_FOCUS_ENERGY = 116,
    MOVE_BIDE = 117,
    MOVE_METRONOME = 118,
    MOVE_MIRROR_MOVE = 119,
    MOVE_SELFDESTRUCT = 120,
    MOVE_EGG_BOMB = 121,
    MOVE_LICK = 122,
    MOVE_SMOG = 123,
    MOVE_SLUDGE = 124,
    MOVE_BONE_CLUB = 125,
    MOVE_FIRE_BLAST = 126,
    MOVE_WATERFALL = 127,
    MOVE_CLAMP = 128,
    MOVE_SWIFT = 129,
    MOVE_SKULL_BASH = 130,
    MOVE_SPIKE_CANNON = 131,
    MOVE_CONSTRICT = 132,
    MOVE_AMNESIA = 133,
    MOVE_KINESIS = 134,
    MOVE_SOFTBOILED = 135,
    MOVE_HI_JUMP_KICK = 136,
    MOVE_GLARE = 137,
    MOVE_DREAM_EATER = 138,
    MOVE_POISON_GAS = 139,
    MOVE_BARRAGE = 140,
    MOVE_LEECH_LIFE = 141,
    MOVE_LOVELY_KISS = 142,
    MOVE_SKY_ATTACK = 143,
    MOVE_TRANSFORM = 144,
    MOVE_BUBBLE = 145,
    MOVE_DIZZY_PUNCH = 146,
    MOVE_SPORE = 147,
    MOVE_FLASH = 148,
    MOVE_PSYWAVE = 149,
    MOVE_SPLASH = 150,
    MOVE_ACID_ARMOR = 151,
    MOVE_CRABHAMMER = 152,
    MOVE_EXPLOSION = 153,
    MOVE_FURY_SWIPES = 154,
    MOVE_BONEMERANG = 155,
    MOVE_REST = 156,
    MOVE_ROCK_SLIDE = 157,
    MOVE_HYPER_FANG = 158,
    MOVE_SHARPEN = 159,
    MOVE_CONVERSION = 160,
    MOVE_TRI_ATTACK = 161,
    MOVE_SUPER_FANG = 162,
    MOVE_SLASH = 163,
    MOVE_SUBSTITUTE = 164,
    MOVE_STRUGGLE = 165,
    MOVE_SKETCH = 166,
    MOVE_TRIPLE_KICK = 167,
    MOVE_THIEF = 168,
    MOVE_SPIDER_WEB = 169,
    MOVE_MIND_READER = 170,
    MOVE_NIGHTMARE = 171,
    MOVE_FLAME_WHEEL = 172,
    MOVE_SNORE = 173,
    MOVE_CURSE = 174,
    MOVE_FLAIL = 175,
    MOVE_CONVERSION_2 = 176,
    MOVE_AEROBLAST = 177,
    MOVE_COTTON_SPORE = 178,
    MOVE_REVERSAL = 179,
    MOVE_SPITE = 180,
    MOVE_POWDER_SNOW = 181,
    MOVE_PROTECT = 182,
    MOVE_MACH_PUNCH = 183,
    MOVE_SCARY_FACE = 184,
    MOVE_FAINT_ATTACK = 185,
    MOVE_SWEET_KISS = 186,
    MOVE_BELLY_DRUM = 187,
    MOVE_SLUDGE_BOMB = 188,
    MOVE_MUD_SLAP = 189,
    MOVE_OCTAZOOKA = 190,
    MOVE_SPIKES = 191,
    MOVE_ZAP_CANNON = 192,
    MOVE_FORESIGHT = 193,
    MOVE_DESTINY_BOND = 194,
    MOVE_PERISH_SONG = 195,
    MOVE_ICY_WIND = 196,
    MOVE_DETECT = 197,
    MOVE_BONE_RUSH = 198,
    MOVE_LOCK_ON = 199,
    MOVE_OUTRAGE = 200,
    MOVE_SANDSTORM = 201,
    MOVE_GIGA_DRAIN = 202,
    MOVE_ENDURE = 203,
    MOVE_CHARM = 204,
    MOVE_ROLLOUT = 205,
    MOVE_FALSE_SWIPE = 206,
    MOVE_SWAGGER = 207,
    MOVE_MILK_DRINK = 208,
    MOVE_SPARK = 209,
    MOVE_FURY_CUTTER = 210,
    MOVE_STEEL_WING = 211,
    MOVE_MEAN_LOOK = 212,
    MOVE_ATTRACT = 213,
    MOVE_SLEEP_TALK = 214,
    MOVE_HEAL_BELL = 215,
    MOVE_RETURN = 216,
    MOVE_PRESENT = 217,
    MOVE_FRUSTRATION = 218,
    MOVE_SAFEGUARD = 219,
    MOVE_PAIN_SPLIT = 220,
    MOVE_SACRED_FIRE = 221,
    MOVE_MAGNITUDE = 222,
    MOVE_DYNAMIC_PUNCH = 223,
    MOVE_MEGAHORN = 224,
    MOVE_DRAGON_BREATH = 225,
    MOVE_BATON_PASS = 226,
    MOVE_ENCORE = 227,
    MOVE_PURSUIT = 228,
    MOVE_RAPID_SPIN = 229,
    MOVE_SWEET_SCENT = 230,
    MOVE_IRON_TAIL = 231,
    MOVE_METAL_CLAW = 232,
    MOVE_VITAL_THROW = 233,
    MOVE_MORNING_SUN = 234,
    MOVE_SYNTHESIS = 235,
    MOVE_MOONLIGHT = 236,
    MOVE_HIDDEN_POWER = 237,
    MOVE_CROSS_CHOP = 238,
    MOVE_TWISTER = 239,
    MOVE_RAIN_DANCE = 240,
    MOVE_SUNNY_DAY = 241,
    MOVE_CRUNCH = 242,
    MOVE_MIRROR_COAT = 243,
    MOVE_PSYCH_UP = 244,
    MOVE_EXTREME_SPEED = 245,
    MOVE_ANCIENT_POWER = 246,
    MOVE_SHADOW_BALL = 247,
    MOVE_FUTURE_SIGHT = 248,
    MOVE_ROCK_SMASH = 249,
    MOVE_WHIRLPOOL = 250,
    MOVE_BEAT_UP = 251,
    MOVE_FAKE_OUT = 252,
    MOVE_UPROAR = 253,
    MOVE_STOCKPILE = 254,
    MOVE_SPIT_UP = 255,
    MOVE_SWALLOW = 256,
    MOVE_HEAT_WAVE = 257,
    MOVE_HAIL = 258,
    MOVE_TORMENT = 259,
    MOVE_FLATTER = 260,
    MOVE_WILL_O_WISP = 261,
    MOVE_MEMENTO = 262,
    MOVE_FACADE = 263,
    MOVE_FOCUS_PUNCH = 264,
    MOVE_SMELLING_SALT = 265,
    MOVE_FOLLOW_ME = 266,
    MOVE_NATURE_POWER = 267,
    MOVE_CHARGE = 268,
    MOVE_TAUNT = 269,
    MOVE_HELPING_HAND = 270,
    MOVE_TRICK = 271,
    MOVE_ROLE_PLAY = 272,
    MOVE_WISH = 273,
    MOVE_ASSIST = 274,
    MOVE_INGRAIN = 275,
    MOVE_SUPERPOWER = 276,
    MOVE_MAGIC_COAT = 277,
    MOVE_RECYCLE = 278,
    MOVE_REVENGE = 279,
    MOVE_BRICK_BREAK = 280,
    MOVE_YAWN = 281,
    MOVE_KNOCK_OFF = 282,
    MOVE_ENDEAVOR = 283,
    MOVE_ERUPTION = 284,
    MOVE_SKILL_SWAP = 285,
    MOVE_IMPRISON = 286,
    MOVE_REFRESH = 287,
    MOVE_GRUDGE = 288,
    MOVE_SNATCH = 289,
    MOVE_SECRET_POWER = 290,
    MOVE_DIVE = 291,
    MOVE_ARM_THRUST = 292,
    MOVE_CAMOUFLAGE = 293,
    MOVE_TAIL_GLOW = 294,
    MOVE_LUSTER_PURGE = 295,
    MOVE_MIST_BALL = 296,
    MOVE_FEATHER_DANCE = 297,
    MOVE_TEETER_DANCE = 298,
    MOVE_BLAZE_KICK = 299,
    MOVE_MUD_SPORT = 300,
    MOVE_ICE_BALL = 301,
    MOVE_NEEDLE_ARM = 302,
    MOVE_SLACK_OFF = 303,
    MOVE_HYPER_VOICE = 304,
    MOVE_POISON_FANG = 305,
    MOVE_CRUSH_CLAW = 306,
    MOVE_BLAST_BURN = 307,
    MOVE_HYDRO_CANNON = 308,
    MOVE_METEOR_MASH = 309,
    MOVE_ASTONISH = 310,
    MOVE_WEATHER_BALL = 311,
    MOVE_AROMATHERAPY = 312,
    MOVE_FAKE_TEARS = 313,
    MOVE_AIR_CUTTER = 314,
    MOVE_OVERHEAT = 315,
    MOVE_ODOR_SLEUTH = 316,
    MOVE_ROCK_TOMB = 317,
    MOVE_SILVER_WIND = 318,
    MOVE_METAL_SOUND = 319,
    MOVE_GRASS_WHISTLE = 320,
    MOVE_TICKLE = 321,
    MOVE_COSMIC_POWER = 322,
    MOVE_WATER_SPOUT = 323,
    MOVE_SIGNAL_BEAM = 324,
    MOVE_SHADOW_PUNCH = 325,
    MOVE_EXTRASENSORY = 326,
    MOVE_SKY_UPPERCUT = 327,
    MOVE_SAND_TOMB = 328,
    MOVE_SHEER_COLD = 329,
    MOVE_MUDDY_WATER = 330,
    MOVE_BULLET_SEED = 331,
    MOVE_AERIAL_ACE = 332,
    MOVE_ICICLE_SPEAR = 333,
    MOVE_IRON_DEFENSE = 334,
    MOVE_BLOCK = 335,
    MOVE_HOWL = 336,
    MOVE_DRAGON_CLAW = 337,
    MOVE_FRENZY_PLANT = 338,
    MOVE_BULK_UP = 339,
    MOVE_BOUNCE = 340,
    MOVE_MUD_SHOT = 341,
    MOVE_POISON_TAIL = 342,
    MOVE_COVET = 343,
    MOVE_VOLT_TACKLE = 344,
    MOVE_MAGICAL_LEAF = 345,
    MOVE_WATER_SPORT = 346,
    MOVE_CALM_MIND = 347,
    MOVE_LEAF_BLADE = 348,
    MOVE_DRAGON_DANCE = 349,
    MOVE_ROCK_BLAST = 350,
    MOVE_SHOCK_WAVE = 351,
    MOVE_WATER_PULSE = 352,
    MOVE_DOOM_DESIRE = 353,
    MOVE_PSYCHO_BOOST = 354,
    MOVE_ROOST = 355,
    MOVE_GRAVITY = 356,
    MOVE_MIRACLE_EYE = 357,
    MOVE_WAKE_UP_SLAP = 358,
    MOVE_HAMMER_ARM = 359,
    MOVE_GYRO_BALL = 360,
    MOVE_HEALING_WISH = 361,
    MOVE_BRINE = 362,
    MOVE_NATURAL_GIFT = 363,
    MOVE_FEINT = 364,
    MOVE_PLUCK = 365,
    MOVE_TAILWIND = 366,
    MOVE_ACUPRESSURE = 367,
    MOVE_METAL_BURST = 368,
    MOVE_U_TURN = 369,
    MOVE_CLOSE_COMBAT = 370,
    MOVE_PAYBACK = 371,
    MOVE_ASSURANCE = 372,
    MOVE_EMBARGO = 373,
    MOVE_FLING = 374,
    MOVE_PSYCHO_SHIFT = 375,
    MOVE_TRUMP_CARD = 376,
    MOVE_HEAL_BLOCK = 377,
    MOVE_WRING_OUT = 378,
    MOVE_POWER_TRICK = 379,
    MOVE_GASTRO_ACID = 380,
    MOVE_LUCKY_CHANT = 381,
    MOVE_ME_FIRST = 382,
    MOVE_COPYCAT = 383,
    MOVE_POWER_SWAP = 384,
    MOVE_GUARD_SWAP = 385,
    MOVE_PUNISHMENT = 386,
    MOVE_LAST_RESORT = 387,
    MOVE_WORRY_SEED = 388,
    MOVE_SUCKER_PUNCH = 389,
    MOVE_TOXIC_SPIKES = 390,
    MOVE_HEART_SWAP = 391,
    MOVE_AQUA_RING = 392,
    MOVE_MAGNET_RISE = 393,
    MOVE_FLARE_BLITZ = 394,
    MOVE_FORCE_PALM = 395,
    MOVE_AURA_SPHERE = 396,
    MOVE_ROCK_POLISH = 397,
    MOVE_POISON_JAB = 398,
    MOVE_DARK_PULSE = 399,
    MOVE_NIGHT_SLASH = 400,
    MOVE_AQUA_TAIL = 401,
    MOVE_SEED_BOMB = 402,
    MOVE_AIR_SLASH = 403,
    MOVE_X_SCISSOR = 404,
    MOVE_BUG_BUZZ = 405,
    MOVE_DRAGON_PULSE = 406,
    MOVE_DRAGON_RUSH = 407,
    MOVE_POWER_GEM = 408,
    MOVE_DRAIN_PUNCH = 409,
    MOVE_VACUUM_WAVE = 410,
    MOVE_FOCUS_BLAST = 411,
    MOVE_ENERGY_BALL = 412,
    MOVE_BRAVE_BIRD = 413,
    MOVE_EARTH_POWER = 414,
    MOVE_SWITCHEROO = 415,
    MOVE_GIGA_IMPACT = 416,
    MOVE_NASTY_PLOT = 417,
    MOVE_BULLET_PUNCH = 418,
    MOVE_AVALANCHE = 419,
    MOVE_ICE_SHARD = 420,
    MOVE_SHADOW_CLAW = 421,
    MOVE_THUNDER_FANG = 422,
    MOVE_ICE_FANG = 423,
    MOVE_FIRE_FANG = 424,
    MOVE_SHADOW_SNEAK = 425,
    MOVE_MUD_BOMB = 426,
    MOVE_PSYCHO_CUT = 427,
    MOVE_ZEN_HEADBUTT = 428,
    MOVE_MIRROR_SHOT = 429,
    MOVE_FLASH_CANNON = 430,
    MOVE_ROCK_CLIMB = 431,
    MOVE_DEFOG = 432,
    MOVE_TRICK_ROOM = 433,
    MOVE_DRACO_METEOR = 434,
    MOVE_DISCHARGE = 435,
    MOVE_LAVA_PLUME = 436,
    MOVE_LEAF_STORM = 437,
    MOVE_POWER_WHIP = 438,
    MOVE_ROCK_WRECKER = 439,
    MOVE_CROSS_POISON = 440,
    MOVE_GUNK_SHOT = 441,
    MOVE_IRON_HEAD = 442,
    MOVE_MAGNET_BOMB = 443,
    MOVE_STONE_EDGE = 444,
    MOVE_CAPTIVATE = 445,
    MOVE_STEALTH_ROCK = 446,
    MOVE_GRASS_KNOT = 447,
    MOVE_CHATTER = 448,
    MOVE_JUDGMENT = 449,
    MOVE_BUG_BITE = 450,
    MOVE_CHARGE_BEAM = 451,
    MOVE_WOOD_HAMMER = 452,
    MOVE_AQUA_JET = 453,
    MOVE_ATTACK_ORDER = 454,
    MOVE_DEFEND_ORDER = 455,
    MOVE_HEAL_ORDER = 456,
    MOVE_HEAD_SMASH = 457,
    MOVE_DOUBLE_HIT = 458,
    MOVE_ROAR_OF_TIME = 459,
    MOVE_SPACIAL_REND = 460,
    MOVE_LUNAR_DANCE = 461,
    MOVE_CRUSH_GRIP = 462,
    MOVE_MAGMA_STORM = 463,
    MOVE_DARK_VOID = 464,
    MOVE_SEED_FLARE = 465,
    MOVE_OMINOUS_WIND = 466,
    MOVE_SHADOW_FORCE = 467,
    MAX_MOVES = 468,
};

enum PokemonDataParam {
    MON_DATA_PERSONALITY = 0,
    MON_DATA_IS_PARTY_DECRYPTED = 1,
    MON_DATA_IS_BOX_DECRYPTED = 2,
    MON_DATA_CHECKSUM_FAILED = 3,
    MON_DATA_CHECKSUM = 4,
    MON_DATA_SPECIES = 5,
    MON_DATA_HELD_ITEM = 6,
    MON_DATA_OT_ID = 7,
    MON_DATA_EXPERIENCE = 8,
    MON_DATA_FRIENDSHIP = 9,
    MON_DATA_ABILITY = 10,
    MON_DATA_MARKINGS = 11,
    MON_DATA_LANGUAGE = 12,
    MON_DATA_HP_EV = 13,
    MON_DATA_ATK_EV = 14,
    MON_DATA_DEF_EV = 15,
    MON_DATA_SPEED_EV = 16,
    MON_DATA_SPATK_EV = 17,
    MON_DATA_SPDEF_EV = 18,
    MON_DATA_COOL = 19,
    MON_DATA_BEAUTY = 20,
    MON_DATA_CUTE = 21,
    MON_DATA_SMART = 22,
    MON_DATA_TOUGH = 23,
    MON_DATA_SHEEN = 24,
    MON_DATA_SINNOH_CHAMP_RIBBON = 25,
    MON_DATA_ABILITY_RIBBON = 26,
    MON_DATA_GREAT_ABILITY_RIBBON = 27,
    MON_DATA_DOUBLE_ABILITY_RIBBON = 28,
    MON_DATA_MULTI_ABILITY_RIBBON = 29,
    MON_DATA_PAIR_ABILITY_RIBBON = 30,
    MON_DATA_WORLD_ABILITY_RIBBON = 31,
    MON_DATA_ALERT_RIBBON = 32,
    MON_DATA_SHOCK_RIBBON = 33,
    MON_DATA_DOWNCAST_RIBBON = 34,
    MON_DATA_CARELESS_RIBBON = 35,
    MON_DATA_RELAX_RIBBON = 36,
    MON_DATA_SNOOZE_RIBBON = 37,
    MON_DATA_SMILE_RIBBON = 38,
    MON_DATA_GORGEOUS_RIBBON = 39,
    MON_DATA_ROYAL_RIBBON = 40,
    MON_DATA_GORGEOUS_ROYAL_RIBBON = 41,
    MON_DATA_FOOTPRINT_RIBBON = 42,
    MON_DATA_RECORD_RIBBON = 43,
    MON_DATA_HISTORY_RIBBON = 44,
    MON_DATA_LEGEND_RIBBON = 45,
    MON_DATA_RED_RIBBON = 46,
    MON_DATA_GREEN_RIBBON = 47,
    MON_DATA_BLUE_RIBBON = 48,
    MON_DATA_FESTIVAL_RIBBON = 49,
    MON_DATA_CARNIVAL_RIBBON = 50,
    MON_DATA_CLASSIC_RIBBON = 51,
    MON_DATA_PREMIER_RIBBON = 52,
    MON_DATA_UNUSED_RIBBON_53 = 53,
    MON_DATA_MOVE1 = 54,
    MON_DATA_MOVE2 = 55,
    MON_DATA_MOVE3 = 56,
    MON_DATA_MOVE4 = 57,
    MON_DATA_MOVE1_PP = 58,
    MON_DATA_MOVE2_PP = 59,
    MON_DATA_MOVE3_PP = 60,
    MON_DATA_MOVE4_PP = 61,
    MON_DATA_MOVE1_PP_UPS = 62,
    MON_DATA_MOVE2_PP_UPS = 63,
    MON_DATA_MOVE3_PP_UPS = 64,
    MON_DATA_MOVE4_PP_UPS = 65,
    MON_DATA_MOVE1_MAX_PP = 66,
    MON_DATA_MOVE2_MAX_PP = 67,
    MON_DATA_MOVE3_MAX_PP = 68,
    MON_DATA_MOVE4_MAX_PP = 69,
    MON_DATA_HP_IV = 70,
    MON_DATA_ATK_IV = 71,
    MON_DATA_DEF_IV = 72,
    MON_DATA_SPEED_IV = 73,
    MON_DATA_SPATK_IV = 74,
    MON_DATA_SPDEF_IV = 75,
    MON_DATA_IS_EGG = 76,
    MON_DATA_HAS_NICKNAME = 77,
    MON_DATA_COOL_RIBBON = 78,
    MON_DATA_COOL_RIBBON_SUPER = 79,
    MON_DATA_COOL_RIBBON_HYPER = 80,
    MON_DATA_COOL_RIBBON_MASTER = 81,
    MON_DATA_BEAUTY_RIBBON = 82,
    MON_DATA_BEAUTY_RIBBON_SUPER = 83,
    MON_DATA_BEAUTY_RIBBON_HYPER = 84,
    MON_DATA_BEAUTY_RIBBON_MASTER = 85,
    MON_DATA_CUTE_RIBBON = 86,
    MON_DATA_CUTE_RIBBON_SUPER = 87,
    MON_DATA_CUTE_RIBBON_HYPER = 88,
    MON_DATA_CUTE_RIBBON_MASTER = 89,
    MON_DATA_SMART_RIBBON = 90,
    MON_DATA_SMART_RIBBON_SUPER = 91,
    MON_DATA_SMART_RIBBON_HYPER = 92,
    MON_DATA_SMART_RIBBON_MASTER = 93,
    MON_DATA_TOUGH_RIBBON = 94,
    MON_DATA_TOUGH_RIBBON_SUPER = 95,
    MON_DATA_TOUGH_RIBBON_HYPER = 96,
    MON_DATA_TOUGH_RIBBON_MASTER = 97,
    MON_DATA_CHAMPION_RIBBON = 98,
    MON_DATA_WINNING_RIBBON = 99,
    MON_DATA_VICTORY_RIBBON = 100,
    MON_DATA_ARTIST_RIBBON = 101,
    MON_DATA_EFFORT_RIBBON = 102,
    MON_DATA_MARINE_RIBBON = 103,
    MON_DATA_LAND_RIBBON = 104,
    MON_DATA_SKY_RIBBON = 105,
    MON_DATA_COUNTRY_RIBBON = 106,
    MON_DATA_NATIONAL_RIBBON = 107,
    MON_DATA_EARTH_RIBBON = 108,
    MON_DATA_WORLD_RIBBON = 109,
    MON_DATA_FATEFUL_ENCOUNTER = 110,
    MON_DATA_GENDER = 111,
    MON_DATA_FORM = 112,
    MON_DATA_UNUSED_113 = 113,
    MON_DATA_UNUSED_114 = 114,
    MON_DATA_EGG_LOCATION_PTHGSS = 115,
    MON_DATA_MET_LOCATION_PTHGSS = 116,
    MON_DATA_NICKNAME = 117,
    MON_DATA_NICKNAME_AND_FLAG = 118,
    MON_DATA_NICKNAME_STRING = 119,
    MON_DATA_NICKNAME_STRING_AND_FLAG = 120,
    MON_DATA_UNUSED_121 = 121,
    MON_DATA_MET_GAME = 122,
    MON_DATA_SUPER_COOL_RIBBON = 123,
    MON_DATA_SUPER_COOL_RIBBON_GREAT = 124,
    MON_DATA_SUPER_COOL_RIBBON_ULTRA = 125,
    MON_DATA_SUPER_COOL_RIBBON_MASTER = 126,
    MON_DATA_SUPER_BEAUTY_RIBBON = 127,
    MON_DATA_SUPER_BEAUTY_RIBBON_GREAT = 128,
    MON_DATA_SUPER_BEAUTY_RIBBON_ULTRA = 129,
    MON_DATA_SUPER_BEAUTY_RIBBON_MASTER = 130,
    MON_DATA_SUPER_CUTE_RIBBON = 131,
    MON_DATA_SUPER_CUTE_RIBBON_GREAT = 132,
    MON_DATA_SUPER_CUTE_RIBBON_ULTRA = 133,
    MON_DATA_SUPER_CUTE_RIBBON_MASTER = 134,
    MON_DATA_SUPER_SMART_RIBBON = 135,
    MON_DATA_SUPER_SMART_RIBBON_GREAT = 136,
    MON_DATA_SUPER_SMART_RIBBON_ULTRA = 137,
    MON_DATA_SUPER_SMART_RIBBON_MASTER = 138,
    MON_DATA_SUPER_TOUGH_RIBBON = 139,
    MON_DATA_SUPER_TOUGH_RIBBON_GREAT = 140,
    MON_DATA_SUPER_TOUGH_RIBBON_ULTRA = 141,
    MON_DATA_SUPER_TOUGH_RIBBON_MASTER = 142,
    MON_DATA_UNUSED_RIBBON_143 = 143,
    MON_DATA_OT_NAME = 144,
    MON_DATA_OT_NAME_STRING = 145,
    MON_DATA_EGG_YEAR = 146,
    MON_DATA_EGG_MONTH = 147,
    MON_DATA_EGG_DAY = 148,
    MON_DATA_MET_YEAR = 149,
    MON_DATA_MET_MONTH = 150,
    MON_DATA_MET_DAY = 151,
    MON_DATA_EGG_LOCATION = 152,
    MON_DATA_MET_LOCATION = 153,
    MON_DATA_POKERUS = 154,
    MON_DATA_POKEBALL = 155,
    MON_DATA_MET_LEVEL = 156,
    MON_DATA_OT_GENDER = 157,
    MON_DATA_MET_TERRAIN = 158,
    MON_DATA_UNUSED_159 = 159,
    MON_DATA_STATUS = 160,
    MON_DATA_LEVEL = 161,
    MON_DATA_BALL_CAPSULE_ID = 162,
    MON_DATA_HP = 163,
    MON_DATA_MAX_HP = 164,
    MON_DATA_ATK = 165,
    MON_DATA_DEF = 166,
    MON_DATA_SPEED = 167,
    MON_DATA_SP_ATK = 168,
    MON_DATA_SP_DEF = 169,
    MON_DATA_MAIL = 170,
    MON_DATA_BALL_CAPSULE = 171,
    MON_DATA_SPECIES_EXISTS = 172,
    MON_DATA_SANITY_IS_EGG = 173,
    MON_DATA_SPECIES_OR_EGG = 174,
    MON_DATA_COMBINED_IVS = 175,
    MON_DATA_PRINT_GENDER = 176,
    MON_DATA_TYPE_1 = 177,
    MON_DATA_TYPE_2 = 178,
    MON_DATA_SPECIES_NAME = 179,
};

typedef struct TextOBJ TextOBJ;

typedef u16 charcode_t;

typedef struct ItemPartyParam {
    u8 healSleep : 1;
    u8 healPoison : 1;
    u8 healBurn : 1;
    u8 healFreeze : 1;
    u8 healParalysis : 1;
    u8 healConfusion : 1;
    u8 healAttract : 1;
    u8 guardSpec : 1;
    u8 revive : 1;
    u8 reviveAll : 1;
    u8 levelUp : 1;
    u8 evolve : 1;
    u8 atkStages : 4;
    u8 defStages : 4;
    u8 spatkStages : 4;
    u8 spdefStages : 4;
    u8 speedStages : 4;
    u8 accStages : 4;
    u8 critStages : 2;
    u8 ppUp : 1;
    u8 ppMax : 1;
    u8 ppRestore : 1;
    u8 ppRestoreAll : 1;
    u8 hpRestore : 1;
    u8 giveHPEVs : 1;
    u8 giveAtkEVs : 1;
    u8 giveDefEVs : 1;
    u8 giveSpeedEVs : 1;
    u8 giveSpAtkEVs : 1;
    u8 giveSpDefEVs : 1;
    u8 giveFriendshipLow : 1;
    u8 giveFriendshipMed : 1;
    u8 giveFriendshipHigh : 1;
    s8 hpEVs;
    s8 atkEVs;
    s8 defEVs;
    s8 speedEVs;
    s8 spatkEVs;
    s8 spdefEVs;
    u8 hpRestored;
    u8 ppRestored;
    s8 friendshipLow;
    s8 friendshipMed;
    s8 friendshipHigh;
    u8 padding[2];
} ItemPartyParam;

typedef struct ItemData {
    u16 price;
    u8 holdEffect;
    u8 effectParam;
    u8 pluckEffect;
    u8 flingEffect;
    u8 flingPower;
    u8 naturalGiftPower;
    u16 naturalGiftType : 5;
    u16 preventToss : 1;
    u16 canRegister : 1;
    u16 fieldPocket : 4;
    u16 battlePocket : 5;
    u8 fieldUseFunc;
    u8 battleUseCategory;
    u8 partyUse;
    u8 padding_0D;
    union {
        u8 dummy;
        ItemPartyParam partyUseParam;
    };
} __attribute__((aligned(4))) ItemData;

enum ItemDataParam {
    ITEM_PARAM_PRICE,
    ITEM_PARAM_HOLD_EFFECT,
    ITEM_PARAM_EFFECT_PARAM,
    ITEM_PARAM_PREVENT_TOSS,
    ITEM_PARAM_CAN_REGISTER,
    ITEM_PARAM_FIELD_POCKET,
    ITEM_PARAM_FIELD_USE_FUNC,
    ITEM_PARAM_BATTLE_USE_CATEGORY,
    ITEM_PARAM_PLUCK_EFFECT,
    ITEM_PARAM_FLING_EFFECT,
    ITEM_PARAM_FLING_POWER,
    ITEM_PARAM_NATURAL_GIFT_POWER,
    ITEM_PARAM_NATURAL_GIFT_TYPE,
    ITEM_PARAM_BATTLE_POCKET,
    ITEM_PARAM_PARTY_USE,
    ITEM_PARAM_HEAL_SLEEP,
    ITEM_PARAM_HEAL_POISON,
    ITEM_PARAM_HEAL_BURN,
    ITEM_PARAM_HEAL_FREEZE,
    ITEM_PARAM_HEAL_PARALYSIS,
    ITEM_PARAM_HEAL_CONFUSION,
    ITEM_PARAM_HEAL_ATTRACT,
    ITEM_PARAM_GUARD_SPEC,
    ITEM_PARAM_REVIVE,
    ITEM_PARAM_REVIVE_ALL,
    ITEM_PARAM_LEVEL_UP,
    ITEM_PARAM_EVOLVE,
    ITEM_PARAM_ATK_STAGES,
    ITEM_PARAM_DEF_STAGES,
    ITEM_PARAM_SPATK_STAGES,
    ITEM_PARAM_SPDEF_STAGES,
    ITEM_PARAM_SPEED_STAGES,
    ITEM_PARAM_ACC_STAGES,
    ITEM_PARAM_CRIT_STAGES,
    ITEM_PARAM_PP_UP,
    ITEM_PARAM_PP_MAX,
    ITEM_PARAM_PP_RESTORE,
    ITEM_PARAM_PP_RESTORE_ALL,
    ITEM_PARAM_HP_RESTORE,
    ITEM_PARAM_GIVE_HP_EVS,
    ITEM_PARAM_GIVE_ATK_EVS,
    ITEM_PARAM_GIVE_DEF_EVS,
    ITEM_PARAM_GIVE_SPEED_EVS,
    ITEM_PARAM_GIVE_SPATK_EVS,
    ITEM_PARAM_GIVE_SPDEF_EVS,
    ITEM_PARAM_GIVE_FRIENDSHIP_LOW,
    ITEM_PARAM_GIVE_FRIENDSHIP_MED,
    ITEM_PARAM_GIVE_FRIENDSHIP_HIGH,
    ITEM_PARAM_HP_EVS,
    ITEM_PARAM_ATK_EVS,
    ITEM_PARAM_DEF_EVS,
    ITEM_PARAM_SPEED_EVS,
    ITEM_PARAM_SPATK_EVS,
    ITEM_PARAM_SPDEF_EVS,
    ITEM_PARAM_HP_RESTORED,
    ITEM_PARAM_PP_RESTORED,
    ITEM_PARAM_FRIENDSHIP_LOW,
    ITEM_PARAM_FRIENDSHIP_MED,
    ITEM_PARAM_FRIENDSHIP_HIGH,
};

s32 GetItemAttr(u16 item, u16 param, enum HeapID heapID);

typedef struct MoveContestData {
    u8 effect;
    u8 type;
    u8 padding02[2];
} MoveContestData;

typedef struct MoveTable {
    u16 effect;
    u8 class;
    u8 power;
    u8 type;
    u8 accuracy;
    u8 pp;
    u8 effectChance;
    u16 range;
    s8 priority;
    u8 flags;
    MoveContestData contest;
} MoveTable;

typedef struct AIContext {
    u8 evalStep;
    u8 moveSlot;
    u16 move;

    s8 moveScore[4];

    int calcTemp;
    u32 thinkingMask;

    u8 stateFlags;
    u8 thinkingBitShift;
    u8 padding0012;
    u8 padding0013;
    int padding0014;

    u8 moveDamageRolls[4];

    u16 battlerMoves[4][4];
    u8 battlerAbilities[4];
    u16 battlerHeldItems[4];

    u16 trainerItems[2][4];

    u32 scriptStackPointer[8];
    u8 scriptStackSize;

    u8 trainerItemCounts[2];
    u8 attacker;
    u8 defender;
    u8 usedItemType[2];
    u8 usedItemCondition[2];
    u16 usedItem[2];

    u8 selectedTarget[4];

    MoveTable moveTable[MAX_MOVES];
    ItemData *itemTable;

    u16 padding1DD0[4];
    u16 padding1DD8[4];
} AIContext;

typedef struct MoveEffectsData {
    u32 disabledTurns : 3;
    u32 encoredTurns : 3;
    u32 chargedTurns : 2;
    u32 tauntedTurns : 3;
    u32 protectSuccessTurns : 2;
    u32 perishSongTurns : 2;
    u32 rolloutCount : 3;
    u32 furyCutterCount : 3;
    u32 stockpileCount : 3;
    u32 stockpileDefBoosts : 3;
    u32 stockpileSpDefBoosts : 3;
    u32 truant : 1;
    u32 flashFire : 1;

    u32 lockOnTarget : 2;
    u32 mimickedMoveSlot : 4;
    u32 bindTarget : 2;
    u32 meanLookTarget : 2;
    u32 lastResortCount : 3;
    u32 magnetRiseTurns : 3;
    u32 healBlockTurns : 3;
    u32 embargoTurns : 3;
    u32 canUnburden : 1;
    u32 metronomeTurns : 4;
    u32 micleBerry : 1;
    u32 custapBerry : 1;
    u32 quickClaw : 1;
    u32 meFirst : 1;
    u32 padding0004_1F : 1;

    int rechargeTurnNumber;
    int fakeOutTurnNumber;
    int slowStartTurnNumber;
    int meFirstTurnNumber;
    int substituteHP;
    u32 transformedPID;

    u16 disabledMove;
    u16 bindingMove;
    u16 encoredMove;
    u16 encoredMoveSlot;
    u16 lastResortMoves[4];
    u16 choiceLockedMove;
    u16 transformedGender;

    int itemHPRecovery;
} MoveEffectsData;

typedef struct BattleMon {
    u16 species;
    u16 atk;
    u16 def;
    u16 speed;
    u16 spAtk;
    u16 spDef;
    u16 moves[4];
    u32 hpIV : 5;
    u32 atkIV : 5;
    u32 defIV : 5;
    u32 speedIV : 5;
    u32 spAtkIV : 5;
    u32 spDefIV : 5;
    u32 isEgg : 1;
    u32 hasNickname : 1;
    s8 statChanges[8];
    int weight;
    u8 type1;
    u8 type2;
    u8 form : 5;
    u8 shiny : 1;
    u8 unk26_6 : 2;
    u8 ability;
    u32 sendOutFlag : 1;
    u32 intimidateFlag : 1;
    u32 traceFlag : 1;
    u32 downloadFlag : 1;
    u32 anticipationFlag : 1;
    u32 forewarnFlag : 1;
    u32 slowStartFlag : 1;
    u32 slowStartEnded : 1;
    u32 friskFlag : 1;
    u32 moldBreakerFlag : 1;
    u32 pressureFlag : 1;
    u32 unk28_B : 21;
    u8 movePPCur[4];
    u8 movePP[4];
    u8 level;
    u8 friendship;
    u16 nickname[11];
    s32 hp;
    u32 maxHp;
    u16 otName[7 + 1];
    u32 exp;
    u32 personality;
    u32 status;
    u32 status2;
    u32 otid;
    u16 item;
    u16 unk76;
    u8 hitCount;
    u8 msgFlag;
    u8 gender : 4;
    u8 otGender : 4;
    u8 ball;
    u32 moveEffectFlags;
    u32 moveEffectFlagsTemp;
    MoveEffectsData unk88;
} BattleMon;

typedef struct BattleContext BattleContext;

typedef struct BattleMessage {
    u8 commandCode;
    u8 tags;
    u16 id;
    int params[6];
    int digits;
    int battler;
} BattleMessage;

typedef struct FieldConditions {
    u32 weatherTurns;
    u8 futureSightTurns[4];
    u8 wishTurns[4];
    u16 futureSightMove[4];
    int futureSightAttacker[4];
    s32 futureSightDamage[4];
    u8 wishTarget[4];
} FieldConditions;

typedef struct SideConditions {
    u32 reflectUser : 2;
    u32 reflectTurns : 3;
    u32 lightScreenUser : 2;
    u32 lightScreenTurns : 3;
    u32 mistUser : 2;
    u32 mistTurns : 3;
    u32 safeguardUser : 2;
    u32 safeguardTurns : 3;
    u32 followMe : 1;
    u32 followMeUser : 2;
    u32 knockedOffItemsMask : 6;
    u32 padding00_1D : 3;

    u32 spikesLayers : 2;
    u32 toxicSpikesLayers : 2;
    u32 padding04_04 : 28;
} SideConditions;

typedef struct TurnFlags {
    u32 struggling : 1;
    u32 ppDecremented : 1;
    u32 protecting : 1;
    u32 helpingHand : 1;
    u32 magicCoat : 1;
    u32 snatching : 1;
    u32 roosting : 1;
    u32 fleeing : 2;
    u32 enduring : 1;
    u32 padding00_0A : 22;

    int physicalDamageTakenFrom[4];
    int physicalDamageLastAttacker;
    int physicalDamageAttackerMask;

    int specialDamageTakenFrom[4];
    int specialDamageLastAttacker;
    int specialDamageAttackerMask;

    int lastDamageTaken;
    int lastAttacker;
    int assuranceDamageMask;
} TurnFlags;

typedef struct SelfTurnFlags {
    u32 skipPressureCheck : 1;
    u32 lightningRodActivated : 1;
    u32 stormDrainActivated : 1;
    u32 moldBreakerActivated : 1;
    u32 trickRoomActivated : 1;
    u32 focusItemActivated : 1;
    u32 repeatedMoveCount : 3;
    u32 padding00_09 : 23;

    int physicalDamageTaken;
    int physicalDamageLastAttacker;
    int specialDamageTaken;
    int specialDamageLastAttacker;
    int statusFlags;
    int shellBellDamageDealt;
} SelfTurnFlags;

typedef struct MoveFailFlags {
    u32 paralysis : 1;
    u32 noEffect : 1;
    u32 imprison : 1;
    u32 infatuation : 1;
    u32 disabled : 1;
    u32 unk0_5 : 1;
    u32 flinch : 1;
    u32 confusion : 1;
    u32 gravity : 1;
    u32 healBlock : 1;
    u32 unused : 21;
} MoveFailFlags;

typedef struct BattleScriptTaskData {
    BattleSystem *battleSys;
    BattleContext *battleCtx;
    BallRotation *ballRotation;
    ManagedSprite *sprites[2];
    TextOBJ *fontOAM;
    CharTransferAllocation charTransferAllocation;
    int flag;
    int seqNum;
    int ball;
    int tmpData[8];
    void *tmpPtr[2];
} BattleScriptTaskData;

struct BattleContext {
    u8 curCommandState[4];
    u8 nextCommandState[4];

    int command;
    int commandNext;

    int fieldConditionCheckState;
    int fieldConditionCheckTemp;
    int monConditionCheckState;
    int monConditionCheckTemp;
    int sideConditionCheckState;
    int sideConditionCheckTemp;
    int turnStartCheckState;
    int turnStartCheckTemp;
    int afterMoveHitCheckState;
    int afterMoveHitCheckTemp;
    int afterMoveMessageType;
    int afterMoveMessageState;
    int afterMoveEffectState;

    int afterMoveEffectTemp;
    int beforeMoveCheckState;
    int tryMoveCheckState;
    int statusCheckState;
    int abilityCheckState;
    int switchInCheckState;
    int vanishedCheckTemp;

    int padding0060;

    int attacker;
    int attackerTemp;
    int defender;
    int defenderTemp;
    int faintedMon;
    int switchedMon;
    int switchedMonTemp;
    int abilityMon;
    int magicCoatMon;
    int sideEffectType;
    int sideEffectParam;
    int sideEffectFlags;
    int sideEffectMon;
    int lastBattlerId;

    int gainedExp;
    int sharedExp;
    u32 sideGetExpMask[2];

    enum NarcId scriptNarc;
    int scriptFile;
    int scriptCursor;
    int scriptStackPointer;
    enum NarcId scriptStackNarc[4];
    int scriptStackFile[4];
    int scriptStackCursor[4];

    int turnOrderCounter;
    int waitCounter;

    BattleMessage msgBuffer;
    int msgBattlerTemp;
    int msgAttacker;
    int msgDefender;
    int msgMoveTemp;
    int msgItemTemp;
    int msgAbilityTemp;
    int msgTemp;
    int calcTemp;
    int scriptTemp;

    u32 battlerStatusFlags[4];
    u32 payDayCount;
    int totalTurns;
    int totalFainted[4];
    int totalDamage[4];
    int meFirstTurnOrder;

    BattleScriptTaskData *taskData;
    void *tmpData;

    u32 fieldConditionsMask;
    FieldConditions fieldConditions;

    u32 sideConditionsMask[2];
    SideConditions sideConditions[2];

    TurnFlags turnFlags[4];
    SelfTurnFlags selfTurnFlags[4];
    MoveFailFlags moveFailFlags[4];

    AIContext aiContext;
    u32 *aiScriptTemp;
    u32 aiScriptCursor;

    u32 battleStatusMask;
    u32 battleStatusMask2;

    int damage;
    int hitDamage;
    int criticalBoosts;
    int criticalMul;
    int movePower;
    int powerMul;
    int hpCalcTemp;
    int moveType;
    int moveEffectChance;
    int prizeMoneyMul;
    u32 moveStatusFlags;
    u32 sideEffectDirectFlags;
    u32 sideEffectIndirectFlags;
    u32 sideEffectAbilityFlags;
    u8 multiHitCounter;
    u8 multiHitNumHits;
    u8 battlerCounter;
    u8 beatUpCounter;

    BOOL multiHitLoop;
    u32 multiHitCheckFlags;
    u32 multiHitAccuracyCheck;

    u32 clearVolatileStatus[4];

    u8 selectedPartySlot[4];
    u8 switchedPartySlot[4];
    u8 aiSwitchedPartySlot[4];

    u32 battlerActions[4][MAX_BATTLE_ACTIONS];
    u8 battlerActionOrder[4];
    u8 monSpeedOrder[4];
    u32 monSpeedValues[4];

    u8 ioQueue[4][4][16];
    u8 ioBuffer[4][256];
    int battleScript[400];

    BattleMon battleMons[4];

    u32 moveTemp;
    u32 moveCur;
    u32 movePrev;
    u32 moveLockedInto[4];
    u16 moveProtect[4];
    u16 moveHit[4];
    u16 moveHitBattler[4];
    u16 moveHitType[4];
    u16 movePrevByBattler[4];
    u16 moveCopied[4];
    u16 moveCopiedHit[4][4];
    u16 moveSketched[4];
    u16 moveSelected[4];
    u16 moveSlot[4];
    u16 conversion2Move[4];
    u16 conversion2Battler[4];
    u16 conversion2Type[4];
    u16 metronomeMove[4];

    int storedDamage[4];
    int lastHitByBattler[4];
    int battlerSpeedTemp;
    u8 battlersSwitchingMask;
    u8 levelUpMons;
    u16 padding310A;

    u16 speedRand[4];

    int flingTemp;
    int flingScript;

    u8 safariCatchStage;
    u8 safariEscapeCount;
    u8 runAttempts;
    u8 battleEndFlag;

    u8 magnitude;
    u8 fieldWeatherChecked;
    s16 hpTemp;

    u16 recycleItem[4];

    u8 partyOrder[4][6];

    BOOL expJinglePlayed;

    int linkBattleTimeout;

    u8 recordedCommandFlags[4];

    int waitingBattlers;

    u32 battleProgressFlag : 1;
    u32 padding3154_01 : 31;
};

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;

void SysTask_Destroy(SysTask *task);

typedef struct Indicator {
    ManagedSprite *sprite;
    SysTask *animTask;
    int x;
    int y;
    int bounceAngle;
    fx32 subscreenOffset;
    s16 exitTimer;
    u8 exitPending;
    u8 hasDropped;
    u8 isExiting;
    u8 exitStep;
    u8 exitFrameCount;
} Indicator;

typedef struct CoordinatesS16 {
    s16 x;
    union {
        s16 y;
        s16 z;
    };
} CoordinatesS16;

typedef struct SubscreenTilemapRect {
    u8 top;
    u8 bottom;
    u8 left;
    u8 right;
} SubscreenTilemapRect;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
} UnusedStruct_ov16_0226A3F4;

typedef struct {
    SysTask *unk_00;
    u8 *unk_04;
    void *unk_08;
    u8 *unk_0C;
    UnusedStruct_ov16_0226A3F4 unk_10;
    u16 unk_1C;
} UnusedStruct_ov16_02268A14_sub3_sub1;

typedef struct MenuButtonPressAnimState {
    s16 phase;
    s16 waitTimer;
    union {
        struct {
            const s16 *keyframes;
            const SubscreenTilemapRect *subscreenTilemapRect;
            int selectedButton;
            u8 srcBufIndex;
            u8 fontOAMIdx;
            u8 spriteIndex;
            u8 unread;
            u8 freeSprites;
            CoordinatesS16 ov12_0226E2D8;
        } ButtonPressAnimationState;
        struct {
            UnusedStruct_ov16_02268A14_sub3_sub1 *unused_00[4];
            UnusedStruct_ov16_02268A14_sub3_sub1 *unused_10;
            NNSG2dCharacterData *unused_14;
            void *unused_18;
            int selectedTarget;
        } TargetSelectAnimationState;
    };
} MenuButtonPressAnimState;

typedef struct {
    int unk_00;
    s16 unk_04;
    s16 unk_06;
    u8 unk_08;
    u8 unk_09;
} UnusedStruct_ov16_02268A14_sub4;

typedef struct {
    const SubscreenTilemapRect *unk_00;
    u8 unk_04;
    s8 unk_05;
} UnusedStruct_ov16_02268A14_sub1;

typedef struct FontOAMEntry {
    TextOBJ *fontOAM;
    CharTransferAllocation allocation;
    u16 textWidth;
} FontOAMEntry;

typedef struct PartyBallAnimState {
    s16 xOffset;
    u8 expPercent;
    u8 phase;
    u8 unused_04;
    u8 delay;
    u8 frameCount;
    u8 duration;
    u8 bounceCount;
} PartyBallAnimState;

typedef struct MenuCursor {
    u8 isActive;
    s8 y;
    s8 x;
    u8 unused_03;
} MenuCursor;

typedef struct ActionSelectData {
    u8 battlerType;
    u8 battler;
    u8 partySlot;
    u8 ballStatusBattler;
    s16 curHp;
    u16 maxHp;
    u8 isWaitingForPartner;
} ActionSelectData;

typedef struct MoveSelectData {
    u16 moveIDs[4];
    u8 currPP[4];
    u8 maxPP[4];
    u8 battlerType;
} MoveSelectData;

typedef struct TargetMonData {
    u8 partySlot;
    u8 gender : 2;
    u8 isPresent : 1;
    u8 : 5;
    u8 stockStatus;
    u8 padding_03;
    s16 curHP;
    u16 maxHP;
} TargetMonData;

typedef struct TargetSelectData {
    TargetMonData targetMonData[4];
    u8 battlerType;
    u8 targetingLayout;
} TargetSelectData;

typedef struct GiveUpMoveData {
    u16 move;
    u16 unused_02;
} GiveUpMoveData;

typedef union ActiveMenuData {
    ActionSelectData actionData;
    MoveSelectData moveData;
    TargetSelectData targetSelectData;
    GiveUpMoveData giveUpMoveData;
} ActiveMenuData;

typedef struct TextWindowLayout {
    Window window;
    u16 width;
    u16 height;
} TextWindowLayout;

typedef struct MoveDisplayInfo {
    u16 move[4];
    u16 curPP[4];
    u16 maxPP[4];
} MoveDisplayInfo;

typedef struct MoveDisplayData {
    MoveDisplayInfo moveDisplayInfo;
    u16 *moveIcons[4];
    TextWindowLayout moveNameWindows[4];
    TextWindowLayout ppCountWindows[4];
    TextWindowLayout ppLabelWindows[4];
} MoveDisplayData;

typedef struct BattleSubscreen {
    BattleSystem *battleSys;
    u8 *suppressActivationSfxPtr;
    SysTask *animatePartyBallsTask;
    SysTask *menuTransitionTask;
    SysTask *unused_10;
    UnusedStruct_ov16_02268A14_sub1 unused_14;
    ActiveMenuData activeMenuData;
    u16 *tilemapBuffers[7];
    u16 *subscreenPaletteBuf;
    u16 *normalMovePalBuf;
    u16 *speedUpMovePalBuf;
    SysTask *speedUpPaletteTask;
    MoveDisplayData moveDisplayData[4];
    UnkStruct_02012744 *fontOAMManager;
    FontOAMEntry fontOAMEntry[13];
    UnkStruct_02012B20 *unused_5B8;
    ManagedSprite *playerPartyBallSprites[6];
    ManagedSprite *opponentPartyBallSprites[6];
    ManagedSprite *moveSelectSprites[4];
    ManagedSprite *categoryIconSprites[4];
    ManagedSprite *targetSelectPokeIconSprites[4];
    SysTask *pokeIconAnimTasks[4];
    PartyBallAnimState partyBallAnimState[6];
    SysTask *unused_664;
    u8 unused_668;
    u8 unused_669;
    u8 battlerType;
    s8 activeMenuConfigIndex;
    u8 targetingLayout;
    u8 trainerGender;
    u8 isPanelSliding;
    u8 hasCancelButton;
    u8 unused_670;
    s16 unused_672;
    u8 pulseCursorDirection;
    s16 pulseCursorBlendValue;
    SysTask *pulseCursorTask;
    MenuButtonPressAnimState menuButtonPressAnimState;
    UnusedStruct_ov16_02268A14_sub4 unused_6A0;
    s32 panelScrollPos;
    s32 slideSpeed;
    s32 targetOffset;
    struct BattleCursor *cursorRenderer;
    MenuCursor cursor;
    u8 suppressActivationSfx;
    u8 isWaitingForPartner;
    struct {
        Indicator *indicator;
        u8 step;
        u8 phase;
        u8 delay;
    } catchTutorialState;
} BattleSubscreen;

int BattleInput_GetKeyPressed(BattleSubscreen *btlSubscreen);

void BattleInput_SetKeyPressed(BattleSubscreen *btlSubscreen, int isCursorEnabled);

typedef struct ChatotCry {
    BOOL valid;
    s8 audioData[1000];
} ChatotCry;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef struct BallCapsule {
    BallSeal seals[8];
} BallCapsule;

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

typedef struct Party {
    struct { int maxCount; int curCount; Pokemon mons[6]; } core;
    struct { struct { u8 unk_00[5]; } aprijuiceModifiers[6]; } extra;
} Party;

void Party_InitWithMaxSize(Party *party, int capacity);

BOOL Party_AddMon(Party *party, Pokemon *mon);

int Party_GetCount(const Party *party);

typedef struct PlayerProfile {
    charcode_t name[7 + 1];
    u32 id;
    u32 money;
    u8 gender;
    u8 language;
    u8 badgeMask;
    u8 appearance;
    u8 gameCode;
    u8 isMainStoryCleared : 1;
    u8 hasNationalDex : 1;
    u8 : 6;
} PlayerProfile;

u32 GetMonData(Pokemon *mon, enum PokemonDataParam param, void *dest);

typedef struct BattleItemUse {
    u16 item;
    u8 category;
    u8 target;
} BattleItemUse;

enum Item {
    ITEM_NONE = 0,
    ITEM_MASTER_BALL = 1,
    ITEM_ULTRA_BALL = 2,
    ITEM_GREAT_BALL = 3,
    ITEM_POKE_BALL = 4,
    ITEM_SAFARI_BALL = 5,
    ITEM_NET_BALL = 6,
    ITEM_DIVE_BALL = 7,
    ITEM_NEST_BALL = 8,
    ITEM_REPEAT_BALL = 9,
    ITEM_TIMER_BALL = 10,
    ITEM_LUXURY_BALL = 11,
    ITEM_PREMIER_BALL = 12,
    ITEM_DUSK_BALL = 13,
    ITEM_HEAL_BALL = 14,
    ITEM_QUICK_BALL = 15,
    ITEM_CHERISH_BALL = 16,
    ITEM_POTION = 17,
    ITEM_ANTIDOTE = 18,
    ITEM_BURN_HEAL = 19,
    ITEM_ICE_HEAL = 20,
    ITEM_AWAKENING = 21,
    ITEM_PARLYZ_HEAL = 22,
    ITEM_FULL_RESTORE = 23,
    ITEM_MAX_POTION = 24,
    ITEM_HYPER_POTION = 25,
    ITEM_SUPER_POTION = 26,
    ITEM_FULL_HEAL = 27,
    ITEM_REVIVE = 28,
    ITEM_MAX_REVIVE = 29,
    ITEM_FRESH_WATER = 30,
    ITEM_SODA_POP = 31,
    ITEM_LEMONADE = 32,
    ITEM_MOOMOO_MILK = 33,
    ITEM_ENERGYPOWDER = 34,
    ITEM_ENERGY_ROOT = 35,
    ITEM_HEAL_POWDER = 36,
    ITEM_REVIVAL_HERB = 37,
    ITEM_ETHER = 38,
    ITEM_MAX_ETHER = 39,
    ITEM_ELIXIR = 40,
    ITEM_MAX_ELIXIR = 41,
    ITEM_LAVA_COOKIE = 42,
    ITEM_BERRY_JUICE = 43,
    ITEM_SACRED_ASH = 44,
    ITEM_HP_UP = 45,
    ITEM_PROTEIN = 46,
    ITEM_IRON = 47,
    ITEM_CARBOS = 48,
    ITEM_CALCIUM = 49,
    ITEM_RARE_CANDY = 50,
    ITEM_PP_UP = 51,
    ITEM_ZINC = 52,
    ITEM_PP_MAX = 53,
    ITEM_OLD_GATEAU = 54,
    ITEM_GUARD_SPEC = 55,
    ITEM_DIRE_HIT = 56,
    ITEM_X_ATTACK = 57,
    ITEM_X_DEFENSE = 58,
    ITEM_X_SPEED = 59,
    ITEM_X_ACCURACY = 60,
    ITEM_X_SPECIAL = 61,
    ITEM_X_SP_DEF = 62,
    ITEM_POKE_DOLL = 63,
    ITEM_FLUFFY_TAIL = 64,
    ITEM_BLUE_FLUTE = 65,
    ITEM_YELLOW_FLUTE = 66,
    ITEM_RED_FLUTE = 67,
    ITEM_BLACK_FLUTE = 68,
    ITEM_WHITE_FLUTE = 69,
    ITEM_SHOAL_SALT = 70,
    ITEM_SHOAL_SHELL = 71,
    ITEM_RED_SHARD = 72,
    ITEM_BLUE_SHARD = 73,
    ITEM_YELLOW_SHARD = 74,
    ITEM_GREEN_SHARD = 75,
    ITEM_SUPER_REPEL = 76,
    ITEM_MAX_REPEL = 77,
    ITEM_ESCAPE_ROPE = 78,
    ITEM_REPEL = 79,
    ITEM_SUN_STONE = 80,
    ITEM_MOON_STONE = 81,
    ITEM_FIRE_STONE = 82,
    ITEM_THUNDERSTONE = 83,
    ITEM_WATER_STONE = 84,
    ITEM_LEAF_STONE = 85,
    ITEM_TINYMUSHROOM = 86,
    ITEM_BIG_MUSHROOM = 87,
    ITEM_PEARL = 88,
    ITEM_BIG_PEARL = 89,
    ITEM_STARDUST = 90,
    ITEM_STAR_PIECE = 91,
    ITEM_NUGGET = 92,
    ITEM_HEART_SCALE = 93,
    ITEM_HONEY = 94,
    ITEM_GROWTH_MULCH = 95,
    ITEM_DAMP_MULCH = 96,
    ITEM_STABLE_MULCH = 97,
    ITEM_GOOEY_MULCH = 98,
    ITEM_ROOT_FOSSIL = 99,
    ITEM_CLAW_FOSSIL = 100,
    ITEM_HELIX_FOSSIL = 101,
    ITEM_DOME_FOSSIL = 102,
    ITEM_OLD_AMBER = 103,
    ITEM_ARMOR_FOSSIL = 104,
    ITEM_SKULL_FOSSIL = 105,
    ITEM_RARE_BONE = 106,
    ITEM_SHINY_STONE = 107,
    ITEM_DUSK_STONE = 108,
    ITEM_DAWN_STONE = 109,
    ITEM_OVAL_STONE = 110,
    ITEM_ODD_KEYSTONE = 111,
    ITEM_GRISEOUS_ORB = 112,
    ITEM_UNUSED_113 = 113,
    ITEM_UNUSED_114 = 114,
    ITEM_UNUSED_115 = 115,
    ITEM_UNUSED_116 = 116,
    ITEM_UNUSED_117 = 117,
    ITEM_UNUSED_118 = 118,
    ITEM_UNUSED_119 = 119,
    ITEM_UNUSED_120 = 120,
    ITEM_UNUSED_121 = 121,
    ITEM_UNUSED_122 = 122,
    ITEM_UNUSED_123 = 123,
    ITEM_UNUSED_124 = 124,
    ITEM_UNUSED_125 = 125,
    ITEM_UNUSED_126 = 126,
    ITEM_UNUSED_127 = 127,
    ITEM_UNUSED_128 = 128,
    ITEM_UNUSED_129 = 129,
    ITEM_UNUSED_130 = 130,
    ITEM_UNUSED_131 = 131,
    ITEM_UNUSED_132 = 132,
    ITEM_UNUSED_133 = 133,
    ITEM_UNUSED_134 = 134,
    ITEM_ADAMANT_ORB = 135,
    ITEM_LUSTROUS_ORB = 136,
    ITEM_GRASS_MAIL = 137,
    ITEM_FLAME_MAIL = 138,
    ITEM_BUBBLE_MAIL = 139,
    ITEM_BLOOM_MAIL = 140,
    ITEM_TUNNEL_MAIL = 141,
    ITEM_STEEL_MAIL = 142,
    ITEM_HEART_MAIL = 143,
    ITEM_SNOW_MAIL = 144,
    ITEM_SPACE_MAIL = 145,
    ITEM_AIR_MAIL = 146,
    ITEM_MOSAIC_MAIL = 147,
    ITEM_BRICK_MAIL = 148,
    ITEM_CHERI_BERRY = 149,
    ITEM_CHESTO_BERRY = 150,
    ITEM_PECHA_BERRY = 151,
    ITEM_RAWST_BERRY = 152,
    ITEM_ASPEAR_BERRY = 153,
    ITEM_LEPPA_BERRY = 154,
    ITEM_ORAN_BERRY = 155,
    ITEM_PERSIM_BERRY = 156,
    ITEM_LUM_BERRY = 157,
    ITEM_SITRUS_BERRY = 158,
    ITEM_FIGY_BERRY = 159,
    ITEM_WIKI_BERRY = 160,
    ITEM_MAGO_BERRY = 161,
    ITEM_AGUAV_BERRY = 162,
    ITEM_IAPAPA_BERRY = 163,
    ITEM_RAZZ_BERRY = 164,
    ITEM_BLUK_BERRY = 165,
    ITEM_NANAB_BERRY = 166,
    ITEM_WEPEAR_BERRY = 167,
    ITEM_PINAP_BERRY = 168,
    ITEM_POMEG_BERRY = 169,
    ITEM_KELPSY_BERRY = 170,
    ITEM_QUALOT_BERRY = 171,
    ITEM_HONDEW_BERRY = 172,
    ITEM_GREPA_BERRY = 173,
    ITEM_TAMATO_BERRY = 174,
    ITEM_CORNN_BERRY = 175,
    ITEM_MAGOST_BERRY = 176,
    ITEM_RABUTA_BERRY = 177,
    ITEM_NOMEL_BERRY = 178,
    ITEM_SPELON_BERRY = 179,
    ITEM_PAMTRE_BERRY = 180,
    ITEM_WATMEL_BERRY = 181,
    ITEM_DURIN_BERRY = 182,
    ITEM_BELUE_BERRY = 183,
    ITEM_OCCA_BERRY = 184,
    ITEM_PASSHO_BERRY = 185,
    ITEM_WACAN_BERRY = 186,
    ITEM_RINDO_BERRY = 187,
    ITEM_YACHE_BERRY = 188,
    ITEM_CHOPLE_BERRY = 189,
    ITEM_KEBIA_BERRY = 190,
    ITEM_SHUCA_BERRY = 191,
    ITEM_COBA_BERRY = 192,
    ITEM_PAYAPA_BERRY = 193,
    ITEM_TANGA_BERRY = 194,
    ITEM_CHARTI_BERRY = 195,
    ITEM_KASIB_BERRY = 196,
    ITEM_HABAN_BERRY = 197,
    ITEM_COLBUR_BERRY = 198,
    ITEM_BABIRI_BERRY = 199,
    ITEM_CHILAN_BERRY = 200,
    ITEM_LIECHI_BERRY = 201,
    ITEM_GANLON_BERRY = 202,
    ITEM_SALAC_BERRY = 203,
    ITEM_PETAYA_BERRY = 204,
    ITEM_APICOT_BERRY = 205,
    ITEM_LANSAT_BERRY = 206,
    ITEM_STARF_BERRY = 207,
    ITEM_ENIGMA_BERRY = 208,
    ITEM_MICLE_BERRY = 209,
    ITEM_CUSTAP_BERRY = 210,
    ITEM_JABOCA_BERRY = 211,
    ITEM_ROWAP_BERRY = 212,
    ITEM_BRIGHTPOWDER = 213,
    ITEM_WHITE_HERB = 214,
    ITEM_MACHO_BRACE = 215,
    ITEM_EXP_SHARE = 216,
    ITEM_QUICK_CLAW = 217,
    ITEM_SOOTHE_BELL = 218,
    ITEM_MENTAL_HERB = 219,
    ITEM_CHOICE_BAND = 220,
    ITEM_KINGS_ROCK = 221,
    ITEM_SILVERPOWDER = 222,
    ITEM_AMULET_COIN = 223,
    ITEM_CLEANSE_TAG = 224,
    ITEM_SOUL_DEW = 225,
    ITEM_DEEPSEATOOTH = 226,
    ITEM_DEEPSEASCALE = 227,
    ITEM_SMOKE_BALL = 228,
    ITEM_EVERSTONE = 229,
    ITEM_FOCUS_BAND = 230,
    ITEM_LUCKY_EGG = 231,
    ITEM_SCOPE_LENS = 232,
    ITEM_METAL_COAT = 233,
    ITEM_LEFTOVERS = 234,
    ITEM_DRAGON_SCALE = 235,
    ITEM_LIGHT_BALL = 236,
    ITEM_SOFT_SAND = 237,
    ITEM_HARD_STONE = 238,
    ITEM_MIRACLE_SEED = 239,
    ITEM_BLACKGLASSES = 240,
    ITEM_BLACK_BELT = 241,
    ITEM_MAGNET = 242,
    ITEM_MYSTIC_WATER = 243,
    ITEM_SHARP_BEAK = 244,
    ITEM_POISON_BARB = 245,
    ITEM_NEVERMELTICE = 246,
    ITEM_SPELL_TAG = 247,
    ITEM_TWISTEDSPOON = 248,
    ITEM_CHARCOAL = 249,
    ITEM_DRAGON_FANG = 250,
    ITEM_SILK_SCARF = 251,
    ITEM_UPGRADE = 252,
    ITEM_SHELL_BELL = 253,
    ITEM_SEA_INCENSE = 254,
    ITEM_LAX_INCENSE = 255,
    ITEM_LUCKY_PUNCH = 256,
    ITEM_METAL_POWDER = 257,
    ITEM_THICK_CLUB = 258,
    ITEM_STICK = 259,
    ITEM_RED_SCARF = 260,
    ITEM_BLUE_SCARF = 261,
    ITEM_PINK_SCARF = 262,
    ITEM_GREEN_SCARF = 263,
    ITEM_YELLOW_SCARF = 264,
    ITEM_WIDE_LENS = 265,
    ITEM_MUSCLE_BAND = 266,
    ITEM_WISE_GLASSES = 267,
    ITEM_EXPERT_BELT = 268,
    ITEM_LIGHT_CLAY = 269,
    ITEM_LIFE_ORB = 270,
    ITEM_POWER_HERB = 271,
    ITEM_TOXIC_ORB = 272,
    ITEM_FLAME_ORB = 273,
    ITEM_QUICK_POWDER = 274,
    ITEM_FOCUS_SASH = 275,
    ITEM_ZOOM_LENS = 276,
    ITEM_METRONOME = 277,
    ITEM_IRON_BALL = 278,
    ITEM_LAGGING_TAIL = 279,
    ITEM_DESTINY_KNOT = 280,
    ITEM_BLACK_SLUDGE = 281,
    ITEM_ICY_ROCK = 282,
    ITEM_SMOOTH_ROCK = 283,
    ITEM_HEAT_ROCK = 284,
    ITEM_DAMP_ROCK = 285,
    ITEM_GRIP_CLAW = 286,
    ITEM_CHOICE_SCARF = 287,
    ITEM_STICKY_BARB = 288,
    ITEM_POWER_BRACER = 289,
    ITEM_POWER_BELT = 290,
    ITEM_POWER_LENS = 291,
    ITEM_POWER_BAND = 292,
    ITEM_POWER_ANKLET = 293,
    ITEM_POWER_WEIGHT = 294,
    ITEM_SHED_SHELL = 295,
    ITEM_BIG_ROOT = 296,
    ITEM_CHOICE_SPECS = 297,
    ITEM_FLAME_PLATE = 298,
    ITEM_SPLASH_PLATE = 299,
    ITEM_ZAP_PLATE = 300,
    ITEM_MEADOW_PLATE = 301,
    ITEM_ICICLE_PLATE = 302,
    ITEM_FIST_PLATE = 303,
    ITEM_TOXIC_PLATE = 304,
    ITEM_EARTH_PLATE = 305,
    ITEM_SKY_PLATE = 306,
    ITEM_MIND_PLATE = 307,
    ITEM_INSECT_PLATE = 308,
    ITEM_STONE_PLATE = 309,
    ITEM_SPOOKY_PLATE = 310,
    ITEM_DRACO_PLATE = 311,
    ITEM_DREAD_PLATE = 312,
    ITEM_IRON_PLATE = 313,
    ITEM_ODD_INCENSE = 314,
    ITEM_ROCK_INCENSE = 315,
    ITEM_FULL_INCENSE = 316,
    ITEM_WAVE_INCENSE = 317,
    ITEM_ROSE_INCENSE = 318,
    ITEM_LUCK_INCENSE = 319,
    ITEM_PURE_INCENSE = 320,
    ITEM_PROTECTOR = 321,
    ITEM_ELECTIRIZER = 322,
    ITEM_MAGMARIZER = 323,
    ITEM_DUBIOUS_DISC = 324,
    ITEM_REAPER_CLOTH = 325,
    ITEM_RAZOR_CLAW = 326,
    ITEM_RAZOR_FANG = 327,
    ITEM_TM01 = 328,
    ITEM_TM02 = 329,
    ITEM_TM03 = 330,
    ITEM_TM04 = 331,
    ITEM_TM05 = 332,
    ITEM_TM06 = 333,
    ITEM_TM07 = 334,
    ITEM_TM08 = 335,
    ITEM_TM09 = 336,
    ITEM_TM10 = 337,
    ITEM_TM11 = 338,
    ITEM_TM12 = 339,
    ITEM_TM13 = 340,
    ITEM_TM14 = 341,
    ITEM_TM15 = 342,
    ITEM_TM16 = 343,
    ITEM_TM17 = 344,
    ITEM_TM18 = 345,
    ITEM_TM19 = 346,
    ITEM_TM20 = 347,
    ITEM_TM21 = 348,
    ITEM_TM22 = 349,
    ITEM_TM23 = 350,
    ITEM_TM24 = 351,
    ITEM_TM25 = 352,
    ITEM_TM26 = 353,
    ITEM_TM27 = 354,
    ITEM_TM28 = 355,
    ITEM_TM29 = 356,
    ITEM_TM30 = 357,
    ITEM_TM31 = 358,
    ITEM_TM32 = 359,
    ITEM_TM33 = 360,
    ITEM_TM34 = 361,
    ITEM_TM35 = 362,
    ITEM_TM36 = 363,
    ITEM_TM37 = 364,
    ITEM_TM38 = 365,
    ITEM_TM39 = 366,
    ITEM_TM40 = 367,
    ITEM_TM41 = 368,
    ITEM_TM42 = 369,
    ITEM_TM43 = 370,
    ITEM_TM44 = 371,
    ITEM_TM45 = 372,
    ITEM_TM46 = 373,
    ITEM_TM47 = 374,
    ITEM_TM48 = 375,
    ITEM_TM49 = 376,
    ITEM_TM50 = 377,
    ITEM_TM51 = 378,
    ITEM_TM52 = 379,
    ITEM_TM53 = 380,
    ITEM_TM54 = 381,
    ITEM_TM55 = 382,
    ITEM_TM56 = 383,
    ITEM_TM57 = 384,
    ITEM_TM58 = 385,
    ITEM_TM59 = 386,
    ITEM_TM60 = 387,
    ITEM_TM61 = 388,
    ITEM_TM62 = 389,
    ITEM_TM63 = 390,
    ITEM_TM64 = 391,
    ITEM_TM65 = 392,
    ITEM_TM66 = 393,
    ITEM_TM67 = 394,
    ITEM_TM68 = 395,
    ITEM_TM69 = 396,
    ITEM_TM70 = 397,
    ITEM_TM71 = 398,
    ITEM_TM72 = 399,
    ITEM_TM73 = 400,
    ITEM_TM74 = 401,
    ITEM_TM75 = 402,
    ITEM_TM76 = 403,
    ITEM_TM77 = 404,
    ITEM_TM78 = 405,
    ITEM_TM79 = 406,
    ITEM_TM80 = 407,
    ITEM_TM81 = 408,
    ITEM_TM82 = 409,
    ITEM_TM83 = 410,
    ITEM_TM84 = 411,
    ITEM_TM85 = 412,
    ITEM_TM86 = 413,
    ITEM_TM87 = 414,
    ITEM_TM88 = 415,
    ITEM_TM89 = 416,
    ITEM_TM90 = 417,
    ITEM_TM91 = 418,
    ITEM_TM92 = 419,
    ITEM_HM01 = 420,
    ITEM_HM02 = 421,
    ITEM_HM03 = 422,
    ITEM_HM04 = 423,
    ITEM_HM05 = 424,
    ITEM_HM06 = 425,
    ITEM_HM07 = 426,
    ITEM_HM08 = 427,
    ITEM_EXPLORER_KIT = 428,
    ITEM_LOOT_SACK = 429,
    ITEM_RULE_BOOK = 430,
    ITEM_POKE_RADAR = 431,
    ITEM_POINT_CARD = 432,
    ITEM_JOURNAL = 433,
    ITEM_SEAL_CASE = 434,
    ITEM_FASHION_CASE = 435,
    ITEM_SEAL_BAG = 436,
    ITEM_PAL_PAD = 437,
    ITEM_WORKS_KEY = 438,
    ITEM_OLD_CHARM = 439,
    ITEM_GALACTIC_KEY = 440,
    ITEM_RED_CHAIN = 441,
    ITEM_TOWN_MAP = 442,
    ITEM_VS_SEEKER = 443,
    ITEM_COIN_CASE = 444,
    ITEM_OLD_ROD = 445,
    ITEM_GOOD_ROD = 446,
    ITEM_SUPER_ROD = 447,
    ITEM_SPRAYDUCK = 448,
    ITEM_POFFIN_CASE = 449,
    ITEM_BICYCLE = 450,
    ITEM_SUITE_KEY = 451,
    ITEM_OAKS_LETTER = 452,
    ITEM_LUNAR_WING = 453,
    ITEM_MEMBER_CARD = 454,
    ITEM_AZURE_FLUTE = 455,
    ITEM_S_S_TICKET = 456,
    ITEM_CONTEST_PASS = 457,
    ITEM_MAGMA_STONE = 458,
    ITEM_PARCEL = 459,
    ITEM_COUPON_1 = 460,
    ITEM_COUPON_2 = 461,
    ITEM_COUPON_3 = 462,
    ITEM_STORAGE_KEY = 463,
    ITEM_SECRETPOTION = 464,
    ITEM_VS_RECORDER = 465,
    ITEM_GRACIDEA = 466,
    ITEM_SECRET_KEY = 467,
    MAX_ITEMS = 468,
};

typedef struct BattleBackgroundReference {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
} BattleBackgroundReference;

void BattleController_EmitSelectedBagItem(BattleSystem *battleSys, int battler, BattleItemUse message);

void BattleController_EmitClearCommand(BattleSystem *battleSys, int battler, int command);

void BattleController_SetMoveAnimation(BattleSystem *battleSys, BattleContext *battleCtx, MoveAnimation *animation, int param3, int param4, int attacker, int defender, u16 move);

typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;

typedef struct Bag {
    BagItem items[165];
    BagItem keyItems[50];
    BagItem TMsHMs[101];
    BagItem mail[12];
    BagItem medicine[40];
    BagItem berries[64];
    BagItem balls[24];
    BagItem battleItems[30];
    u16 registeredItems[2];
} Bag;

void Heap_Free(void *ptr);

void ov12_02237B0C(BattleSystem *battleSys);

void ov12_02237BB8(BattleSystem *battleSys);

void ov12_02237ED0(BattleSystem *battleSys, int flags);

typedef struct CameraAngle {
    u16 x;
    u16 y;
    u16 z;
    u16 padding_06;
} CameraAngle;

typedef struct CameraLookAt {
    VecFx32 position;
    VecFx32 target;
    VecFx32 up;
} CameraLookAt;

typedef struct CameraPerspectiveProjection {
    fx32 sinFovY;
    fx32 cosFovY;
    fx32 aspectRatio;
    fx32 nearClip;
    fx32 farClip;
} CameraPerspectiveProjection;

typedef struct CameraPositionHistory {
    int historySize;
    int currentHistoryIndex;
    int nextHistoryIndex;
    int delay;
    BOOL delayReached;
    BOOL delayX;
    BOOL delayY;
    BOOL delayZ;
    VecFx32 *positions;
} CameraPositionHistory;

typedef struct Camera {
    CameraPerspectiveProjection perspective;
    CameraLookAt lookAt;
    fx32 distance;
    CameraAngle angle;
    u8 projection;
    u16 fovY;
    VecFx32 prevTargetPos;
    const VecFx32 *targetPos;
    BOOL trackTargetX;
    BOOL trackTargetY;
    BOOL trackTargetZ;
    CameraPositionHistory *history;
} Camera;

typedef struct SPLParticle {
    struct SPLParticle *p_next;
    struct SPLParticle *p_prev;
    VecFx32 ptcl_pos;
    VecFx32 vel;
    u16 rtt;
    s16 rtt_vel;
    u16 life;
    u16 age;
    u16 life_rate_diff_loop;
    u16 life_rate_diff_noloop;
    struct { u16 tex_no : 8; u16 life_rate_offset : 8; } etc;
    struct {
        u16 baseAlpha : 5;
        u16 animAlpha : 5;
        u16 currentPolygonID : 6;
    } alp;
    fx32 base_scl;
    fx16 anm_scl;
    GXRgb clr;
    VecFx32 emtr_pos;
} SPLParticle;

typedef struct SPLParticleList {
    SPLParticle *first;
    int count;
    SPLParticle *last;
} SPLParticleList;

typedef struct SPLBehavior {
    void (*applyFunc)(const void *, struct SPLParticle *, VecFx32 *, struct SPLEmitter *);
    const void *object;
} SPLBehavior;

typedef union SPLResourceFlags {
    u32 all;
    struct {
        u32 emissionType : 4;
        u32 drawType : 2;
        u32 circleAxis : 2;
        u32 hasScaleAnim : 1;
        u32 hasColorAnim : 1;
        u32 hasAlphaAnim : 1;
        u32 hasTexAnim : 1;
        u32 hasRotation : 1;
        u32 randomInitAngle : 1;

        u32 selfMaintaining : 1;
        u32 followEmitter : 1;
        u32 hasChildResource : 1;
        u32 polygonRotAxis : 2;
        u32 polygonReferencePlane : 1;
        u32 randomizeLoopedAnim : 1;
        u32 drawChildrenFirst : 1;
        u32 hideParent : 1;
        u32 useViewSpace : 1;
        u32 hasGravityBehavior : 1;
        u32 hasRandomBehavior : 1;
        u32 hasMagnetBehavior : 1;
        u32 hasSpinBehavior : 1;
        u32 hasCollisionPlaneBehavior : 1;
        u32 hasConvergenceBehavior : 1;
        u32 hasFixedPolygonID : 1;
        u32 childHasFixedPolygonID : 1;
    };
} SPLResourceFlags;

typedef union SPLChildResourceFlags {
    u16 all;
    struct {
        u16 usesBehaviors : 1;
        u16 hasScaleAnim : 1;
        u16 hasAlphaAnim : 1;
        u16 rotationType : 2;
        u16 followEmitter : 1;
        u16 useChildColor : 1;
        u16 drawType : 2;
        u16 polygonRotAxis : 2;
        u16 polygonReferencePlane : 1;
        u16 : 4;
    };
} SPLChildResourceFlags;

typedef union SPLCurveInOut {
    u16 all;
    struct {
        u16 in : 8;
        u16 out : 8;
    };
} SPLCurveInOut;

typedef union SPLCurveInPeakOut {
    u32 all;
    struct {
        u32 in : 8;
        u32 peak : 8;
        u32 out : 8;
        u32 : 8;
    };
} SPLCurveInPeakOut;

typedef struct SPLResourceHeader {
    SPLResourceFlags flags;
    VecFx32 emitterBasePos;
    fx32 emissionCount;
    fx32 radius;
    fx32 length;
    VecFx16 axis;
    GXRgb color;
    fx32 initVelPosAmplifier;
    fx32 initVelAxisAmplifier;
    fx32 baseScale;
    fx16 aspectRatio;
    u16 startDelay;
    s16 minRotation;
    s16 maxRotation;
    u16 initAngle;
    u16 reserved;
    u16 emitterLifeTime;
    u16 particleLifeTime;

    struct {
        u32 baseScale : 8;
        u32 lifeTime : 8;
        u32 initVel : 8;
        u32 : 8;
    } randomAttenuation;

    struct {
        u32 emissionInterval : 8;
        u32 baseAlpha : 8;
        u32 airResistance : 8;
        u32 textureIndex : 8;
        u32 loopFrames : 8;
        u32 dbbScale : 16;
        u32 textureTileCountS : 2;
        u32 textureTileCountT : 2;
        u32 scaleAnimDir : 3;
        u32 dpolFaceEmitter : 1;
        u32 flipTextureS : 1;
        u32 flipTextureT : 1;
        u32 : 30;
    } misc;
    fx16 polygonX;
    fx16 polygonY;
    struct {
        u32 flags : 8;
        u32 : 24;
    } userData;
} SPLResourceHeader;

typedef struct SPLScaleAnim {
    fx16 start;
    fx16 mid;
    fx16 end;
    SPLCurveInOut curve;
    struct {
        u16 loop : 1;
        u16 : 15;
    } flags;
    u16 padding;
} SPLScaleAnim;

typedef struct SPLColorAnim {
    GXRgb start;
    GXRgb end;
    SPLCurveInPeakOut curve;
    struct {
        u16 randomStartColor : 1;
        u16 loop : 1;
        u16 interpolate : 1;
        u16 : 13;
    } flags;
    u16 padding;
} SPLColorAnim;

typedef struct SPLAlphaAnim {
    union {
        u16 all;
        struct {
            u16 start : 5;
            u16 mid : 5;
            u16 end : 5;
            u16 : 1;
        };
    } alpha;
    struct {
        u16 randomRange : 8;
        u16 loop : 1;
        u16 : 7;
    } flags;
    SPLCurveInOut curve;
    u16 padding;
} SPLAlphaAnim;

typedef struct SPLTexAnim {
    u8 textures[8];
    struct {
        u32 frameCount : 8;
        u32 step : 8;
        u32 randomizeInit : 1;
        u32 loop : 1;
        u32 : 14;
    } param;
} SPLTexAnim;

typedef struct SPLChildResource {
    SPLChildResourceFlags flags;
    fx16 randomInitVelMag;
    fx16 endScale;
    u16 lifeTime;
    u8 velocityRatio;
    u8 scaleRatio;
    GXRgb color;
    struct {
        u32 emissionCount : 8;
        u32 emissionDelay : 8;
        u32 emissionInterval : 8;
        u32 texture : 8;
        u32 textureTileCountS : 2;
        u32 textureTileCountT : 2;
        u32 flipTextureS : 1;
        u32 flipTextureT : 1;
        u32 dpolFaceEmitter : 1;
        u32 : 25;
    } misc;
} SPLChildResource;

typedef struct SPLResource {
    SPLResourceHeader *header;
    SPLScaleAnim *scaleAnim;
    SPLColorAnim *colorAnim;
    SPLAlphaAnim *alphaAnim;
    SPLTexAnim *texAnim;
    SPLChildResource *childResource;
    SPLBehavior *behaviors;
    u16 behaviorCount;
    u16 reserved;
} SPLResource;

enum SPLUpdateCallbackType {
    SPL_CALLBACK_PRE_UPDATE = 0,
    SPL_CALLBACK_POST_UPDATE = 1,
};

typedef void (*SPLEmitterUpdateCallback)(struct SPLEmitter *emitter, enum SPLUpdateCallbackType type);

typedef struct SPLEmitterState {
    union {
        u32 all;
        struct
        {
            u32 terminate : 1;
            u32 emissionPaused : 1;
            u32 paused : 1;
            u32 renderingDisabled : 1;
            u32 started : 1;
            u32 : 27;
        };
    };
} SPLEmitterState;

typedef struct SPLEmitter {
    struct SPLEmitter *next;
    struct SPLEmitter *prev;
    SPLParticleList particles;
    SPLParticleList childParticles;
    SPLResource *resource;
    SPLEmitterState state;
    VecFx32 position;
    VecFx32 velocity;
    VecFx32 particleInitVelocity;
    u16 age;
    fx16 emissionCountFractional;
    VecFx16 axis;
    u16 initAngle;
    fx32 emissionCount;
    fx32 radius;
    fx32 length;
    fx32 initVelPositionAmplifier;
    fx32 initVelAxisAmplifier;
    fx32 baseScale;
    u16 particleLifeTime;
    GXRgb color;
    fx32 collisionPlaneHeight;
    fx16 textureS;
    fx16 textureT;
    fx16 childTextureS;
    fx16 childTextureT;
    struct {
        u32 emissionInterval : 8;
        u32 baseAlpha : 8;
        u32 updateCycle : 3;
        u32 reserved : 13;
    } misc;
    VecFx16 crossAxis1;
    VecFx16 crossAxis2;
    SPLEmitterUpdateCallback updateCallback;

    void *userDataPtr;
    union {
        u32 u32;
        u16 u16[2];
        u8 u8[4];
    } userData;
} SPLEmitter;

typedef struct SPLEmitterList {
    SPLEmitter *first;
    int count;
    SPLEmitter *last;
} SPLEmitterList;

typedef union SPLTextureParam {
    u32 all;
    struct {
        u32 format : 4;
        u32 s : 4;
        u32 t : 4;
        u32 repeat : 2;
        u32 flip : 2;
        u32 palColor0 : 1;
        u32 useSharedTexture : 1;
        u32 sharedTexID : 8;
        u32 : 6;
    };
} SPLTextureParam;

typedef struct SPLTexture {
    const void *resource;
    u32 texAddr;
    u32 palAddr;
    SPLTextureParam param;
    u16 width;
    u16 height;
} SPLTexture;

typedef void *(*SPLAllocFunc)(u32);

typedef struct SPLManager {
    SPLAllocFunc alloc;
    SPLEmitterList activeEmitters;
    SPLEmitterList inactiveEmitters;
    SPLParticleList inactiveParticles;
    SPLResource *resources;
    SPLTexture *textures;
    u16 resCount;
    u16 texCount;
    u16 maxEmitters;
    u16 maxParticles;
    struct {
        u32 min : 6;
        u32 max : 6;
        u32 current : 6;
        u32 fix : 6;
        u32 drawOrder : 1;
        u32 unused : 7;
    } polygonID;
    GXPolygonAttrMisc miscPolygonAttr;
    struct {
        SPLEmitter *emitter;
        const MtxFx43 *viewMatrix;
    } renderState;
    u16 currentCycle;
    u16 reserved;
} SPLManager;

typedef struct ParticleSystem {
    SPLManager *manager;
    void *resource;
    SPLEmitter *lastAddedEmitter;
    void *heapStart;
    void *heap;
    void *heapEnd;
    void *texAllocFunc;
    void *palAllocFunc;
    Camera *camera;
    VecFx32 sLotoId;
    u16 cameraFov;
    u8 pad_32[2];
    VecFx32 cameraPos;
    VecFx32 cameraUp;
    VecFx32 cameraTarget;
    u32 texKeys[16];
    u32 plttKeys[16];
    u8 vramAutoRelease;
    u8 unused2;
    u8 id;
    u8 cameraProjection;
} ParticleSystem;

enum BattleAnimSystemArc {
    BATTLE_ANIM_SYSTEM_ARC_BATT_BG = 0,
    BATTLE_ANIM_SYSTEM_ARC_BATT_OBJ,
    BATTLE_ANIM_SYSTEM_ARC_WECHAR,
    BATTLE_ANIM_SYSTEM_ARC_WEPLTT,
    BATTLE_ANIM_SYSTEM_ARC_WECELL,
    BATTLE_ANIM_SYSTEM_ARC_WECELLANM,

    BATTLE_ANIM_SYSTEM_ARC_COUNT,
};

typedef struct BattleAnimContext {
    u8 command;
    u8 unk_01;

    u16 move;
    s32 damage;
    u16 power;
    u16 friendship;
    u32 fieldConditions;
    u16 effectChance;
    u16 terrain;

    u16 attacker;
    u16 defender;
    int currentParticleSystem;
    ParticleSystem *particleSystems[16];
    SPLEmitter *emitters[20];
    SpriteSystem *spriteSystem;

    PokemonSpriteData *pokemonSpriteData[4];
    u8 battlerTypes[4];
    Pokepic *battlerSprites[4];
    u32 battleType;
    u16 battlerSpecies[4];
    u8 battlerGenders[4];
    u8 battlerShinyFlags[4];
    u8 battlerForms[4];
    u32 battlerPersonalities[4];
    u32 battlerMoveEffects[4];
    ChatotCry *chatotCry;
    u8 *bgTiles;
    u16 *bgPaletteBuffer;
    BOOL isTransformed;
} BattleAnimContext;

typedef struct CustomBgScrollContext CustomBgScrollContext;

typedef struct BattleAnimScriptLoop {
    u32 *startPos;
    u8 curIteration;
    u8 maxIterations;
    BOOL isActive;
} BattleAnimScriptLoop;

typedef struct PokemonSpriteDrawContext {
    struct BattleAnimSystem *battleAnimSys;
    SpriteManager *spriteManager;
    ManagedSprite *sprite;
    BOOL active;
} PokemonSpriteDrawContext;

typedef struct BgWaveStrip {
    s16 startScanline;
    s16 endScanline;
    s16 scrollSpeed;
    s16 scrollAccum;
    int baseOffset;
} BgWaveStrip;

typedef struct WaveAnimContext {
    BgWaveStrip strips[16];
    struct CustomBgScrollContext *bgScrollCtx;
} WaveAnimContext;

typedef struct BattleBgAnim {
    BgConfig *bgConfig;
    s16 offsetX;
    s16 offsetY;
    s16 stepX;
    s16 stepY;
    int bg;
    int unusedBg;
    BOOL unused;
    BOOL cancel;
    WaveAnimContext *waveAnimCtx;
} BattleBgAnim;

typedef struct PokemonSpriteTrackingTask {
    u8 frameCount;
    u8 interval;
    Pokepic *sprite;
} PokemonSpriteTrackingData;

typedef struct SpriteTrackingTask {
    ManagedSprite *sprite;
    PokemonSpriteTrackingData data;
    SysTask *task;
} SpriteTrackingTask;

typedef struct BgTrackingTask {
    BgConfig *bg;
    PokemonSpriteTrackingData data;
    SysTask *task;
} BgTrackingTask;

BOOL ov07_0221C394(BattleAnimSystem *system);

BOOL ov07_0221C3B0(BattleAnimSystem *system);

BOOL ov07_0221C3C0(BattleAnimSystem *system);

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

typedef struct PaletteAnimator PaletteAnimator;

typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);

struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};

void sub_0201649C(PaletteAnimator *param0, u8 param1);

u32 BattleSystem_GetBattleType(BattleSystem *battleSys);

OpponentData *BattleSystem_GetOpponentData(BattleSystem *battleSys, int battler);

Party *BattleSystem_GetParty(BattleSystem *battleSys, int battler);

Pokemon *BattleSystem_GetPartyMon(BattleSystem *battleSys, int battler, int slot);

BattleAnimSystem *ov12_0223A8DC(BattleSystem *battleSys);

BattleSubscreen *BattleSystem_GetBattleInput(BattleSystem *battleSys);

struct MsgData *BattleSystem_GetMessageLoader(BattleSystem *battleSys);

PaletteData *BattleSystem_GetPaletteData(BattleSystem *battleSys);

PlayerProfile *BattleSystem_GetPlayerProfile(BattleSystem *battleSys, int battler);

Bag *BattleSystem_GetBag(BattleSystem *battleSys);

u8 ov12_0223AB0C(BattleSystem *battleSys, int battler);

PaletteAnimator *BattleSystem_GetMessageIcon(BattleSystem *battleSys);

int BattleSystem_GetBattlerIdPartner(BattleSystem *battleSys, int battler);

BOOL BattleSystem_AreBattleAnimationsOn(BattleSystem *battleSys);

u8 BattleSystem_GetTextFrameDelay(BattleSystem *battleSys);

void *BattleSystem_GetHpBar(BattleSystem *battleSys, int battler);

u32 BattleSystem_PrintBattleMessage(BattleSystem *battleSys, struct MsgData *msgLoader, BattleMessage *battleMsg, u8 renderDelay);

typedef struct BattleBagContext {
    BattleSystem *battleSys;
    PlayerProfile *trainerInfo;
    Bag *bag;
    enum HeapID heapID;
    s32 battler;
    BOOL isInCatchTutorial;
    u32 embargoRemainingTurns;
    u16 selectedBattleBagItem;
    u8 selectedBattleBagPocket;
    u8 lastUsedItemPocket;
    u16 lastUsedItem;
    u8 hasTwoOpponents;
    u8 opponentHidden;
    u8 opponentSubstituted;
    u8 isCursorEnabled;
    u8 battleBagExited;
    u8 pocketCurrentPagePositions[5];
    u8 pocketCurrentPages[5];
} BattleBagContext;

void ov08_022225D4(BattleBagContext *context);

enum BattlePocketIndex {
    BATTLE_POCKET_INDEX_RECOVER_HP_PP = 0,
    BATTLE_POCKET_INDEX_RECOVER_STATUS,
    BATTLE_POCKET_INDEX_POKE_BALLS,
    BATTLE_POCKET_INDEX_BATTLE_ITEMS,
    BATTLE_POCKET_INDEX_NONE,
};

enum BattlePartyMode {
    BATTLE_PARTY_MODE_SELECT_POKEMON = 0,
    BATTLE_PARTY_MODE_SELECT_POKEMON_NO_CANCEL,
    BATTLE_PARTY_MODE_USE_ITEM,
    BATTLE_PARTY_MODE_LEARN_MOVE,
};

typedef struct BattlePartyContext {
    Party *party;
    void *sLotoId;
    BattleSystem *battleSys;
    enum HeapID heapID;
    u8 unused2;
    u8 selectedPartyIndex;
    u8 doubleBattleFirstSelectedPartySlot;
    u8 unused3;
    u8 playerPokemonPartySlot;
    u8 partnerPokemonPartySlot;
    u32 embargoRemainingTurns[2];
    u16 currentDamage;
    u16 selectedBattleBagItem;
    u16 moveToLearn;
    s32 battler;
    u8 pokemonPartySlots[6];
    u8 isCursorEnabled;
    u8 selectedBattleBagPocket;
    u8 selectedMoveSlot;
    u8 battlePartyMode;
    u8 battlePartyExited;
} BattlePartyContext;

void ov08_0221BE20(BattlePartyContext *context);

BOOL sub_0200602C(u16 seqID, int pan);

u8 TextPrinterCheckActive(u8 printerID);

void ov12_02261B80(BattleSystem *battleSys, OpponentData *battlerData, BattleAnimSystem *battleAnimSystem, MoveAnimation *animation);

typedef struct PartyMenuData {
    BattleSystem *battleSys;
    BattlePartyContext *battlePartyCtx;
    u8 command;
    u8 battler;
    u8 state;
    u8 listMode;
    u8 partySlots[4];
    int canSwitch;
    u16 selectedBattleBagItem;
    u8 doublesSelection;
    u8 isCursorEnabled;
    u8 battlersSwitchingMask;
    u8 unused[3];
    u8 partyOrder[4][6];
} PartyMenuData;

typedef struct BagMenuData {
    BattleSystem *battleSys;
    BattleBagContext *battleBagCtx;
    PartyMenuData *partyMenuData;
    u8 command;
    u8 battler;
    u8 state;
    u8 battlerType;
    u8 isCursorEnabled;
    u8 msgIdx;
    u16 stateAfterDelay;
    u8 hasTwoOpponents;
    u8 semiInvulnerable;
    u8 substitute;
    u8 delay;
    u8 partyOrder[4][6];
    u8 embargoTurns[4];
} BagMenuData;

void ov12_0225E830(SysTask *task, void *data);

void ov12_0225E830(SysTask *task, void *data) {
    BagMenuData *bagMenuData = data;
    PaletteData *paletteData = BattleSystem_GetPaletteData(bagMenuData->battleSys);

    switch (bagMenuData->state) {
    case 0:
        bagMenuData->isCursorEnabled = BattleInput_GetKeyPressed(BattleSystem_GetBattleInput(bagMenuData->battleSys));
        sub_0201649C(BattleSystem_GetMessageIcon(bagMenuData->battleSys), 1);
        PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_MAIN_OBJ), 0xC00, -8, 0, 7, 0);
        PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_SUB_OBJ), 0xFFFF, -8, 0, 16, 0);
        bagMenuData->state++;
        break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(paletteData) == 0) {
            ov12_02237B0C(bagMenuData->battleSys);

            bagMenuData->battleBagCtx = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleBagContext));
            memset(bagMenuData->battleBagCtx, 0, sizeof(BattleBagContext));

            bagMenuData->battleBagCtx->battleSys = bagMenuData->battleSys;
            bagMenuData->battleBagCtx->trainerInfo = BattleSystem_GetPlayerProfile(bagMenuData->battleSys, bagMenuData->battler);
            bagMenuData->battleBagCtx->heapID = HEAP_ID_BATTLE;
            bagMenuData->battleBagCtx->battleBagExited = 0;
            bagMenuData->battleBagCtx->bag = BattleSystem_GetBag(bagMenuData->battleSys);
            bagMenuData->battleBagCtx->battler = bagMenuData->battler;
            bagMenuData->battleBagCtx->isCursorEnabled = bagMenuData->isCursorEnabled;
            bagMenuData->battleBagCtx->hasTwoOpponents = bagMenuData->hasTwoOpponents;
            bagMenuData->battleBagCtx->opponentHidden = bagMenuData->semiInvulnerable;
            bagMenuData->battleBagCtx->opponentSubstituted = bagMenuData->substitute;
            bagMenuData->battleBagCtx->embargoRemainingTurns = bagMenuData->embargoTurns[bagMenuData->battler];
            ov08_022225D4(bagMenuData->battleBagCtx);
            bagMenuData->state = 3;
        }
        break;
    case 2:
        bagMenuData->battleBagCtx->isCursorEnabled = bagMenuData->isCursorEnabled;
        ov08_022225D4(bagMenuData->battleBagCtx);
        bagMenuData->state++;
    case 3:
        if (bagMenuData->battleBagCtx->battleBagExited) {
            bagMenuData->battleBagCtx->battleBagExited = 0;
            bagMenuData->isCursorEnabled = bagMenuData->battleBagCtx->isCursorEnabled;

            if (bagMenuData->battleBagCtx->selectedBattleBagItem) {
                switch (bagMenuData->battleBagCtx->selectedBattleBagPocket) {
                case BATTLE_POCKET_INDEX_RECOVER_STATUS:
                case BATTLE_POCKET_INDEX_RECOVER_HP_PP:
                    bagMenuData->state = 4;
                    break;
                case BATTLE_POCKET_INDEX_BATTLE_ITEMS:
                case BATTLE_POCKET_INDEX_POKE_BALLS:
                    bagMenuData->state = 6;
                    break;
                }
            } else {
                bagMenuData->state = 6;
            }
        }
        break;
    case 4:
        int battler;
        Party *party = BattleSystem_GetParty(bagMenuData->battleSys, bagMenuData->battler);
        Pokemon *mon;

        if (BattleSystem_GetBattleType(bagMenuData->battleSys) & (1 << 1) && (BattleSystem_GetBattleType(bagMenuData->battleSys) & (1 << 3)) == 0) {
            battler = bagMenuData->battler & 1;
        } else {
            battler = bagMenuData->battler;
        }

        Party_InitWithMaxSize(bagMenuData->partyMenuData->battlePartyCtx->party, 6);

        for (int i = 0; i < Party_GetCount(party); i++) {
            mon = BattleSystem_GetPartyMon(bagMenuData->battleSys, battler, bagMenuData->partyOrder[battler][i]);
            Party_AddMon(bagMenuData->partyMenuData->battlePartyCtx->party, mon);
            bagMenuData->partyMenuData->battlePartyCtx->pokemonPartySlots[i] = bagMenuData->partyOrder[battler][i];
        }

        bagMenuData->partyMenuData->battlePartyCtx->battleSys = bagMenuData->battleSys;
        bagMenuData->partyMenuData->battlePartyCtx->heapID = HEAP_ID_BATTLE;
        bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex = 0;
        bagMenuData->partyMenuData->battlePartyCtx->battlePartyExited = 0;
        bagMenuData->partyMenuData->battlePartyCtx->moveToLearn = MOVE_NONE;
        bagMenuData->partyMenuData->battlePartyCtx->battlePartyMode = BATTLE_PARTY_MODE_USE_ITEM;
        bagMenuData->partyMenuData->battlePartyCtx->selectedBattleBagItem = bagMenuData->battleBagCtx->selectedBattleBagItem;
        bagMenuData->partyMenuData->battlePartyCtx->selectedBattleBagPocket = bagMenuData->battleBagCtx->selectedBattleBagPocket;
        bagMenuData->partyMenuData->battlePartyCtx->battler = bagMenuData->battleBagCtx->battler;
        bagMenuData->partyMenuData->battlePartyCtx->isCursorEnabled = bagMenuData->isCursorEnabled;
        bagMenuData->partyMenuData->battlePartyCtx->playerPokemonPartySlot = bagMenuData->partyMenuData->partySlots[bagMenuData->battler];
        bagMenuData->partyMenuData->battlePartyCtx->partnerPokemonPartySlot = bagMenuData->partyMenuData->partySlots[BattleSystem_GetBattlerIdPartner(bagMenuData->battleSys, bagMenuData->battler)];

        if (bagMenuData->battlerType == 4) {
            bagMenuData->partyMenuData->battlePartyCtx->embargoRemainingTurns[0] = bagMenuData->embargoTurns[BattleSystem_GetBattlerIdPartner(bagMenuData->battleSys, bagMenuData->battler)];
            bagMenuData->partyMenuData->battlePartyCtx->embargoRemainingTurns[1] = bagMenuData->embargoTurns[bagMenuData->battler];
        } else {
            bagMenuData->partyMenuData->battlePartyCtx->embargoRemainingTurns[0] = bagMenuData->embargoTurns[bagMenuData->battler];
            bagMenuData->partyMenuData->battlePartyCtx->embargoRemainingTurns[1] = bagMenuData->embargoTurns[BattleSystem_GetBattlerIdPartner(bagMenuData->battleSys, bagMenuData->battler)];
        }

        ov08_0221BE20(bagMenuData->partyMenuData->battlePartyCtx);
        bagMenuData->state++;
        break;
    case 5:
        if (bagMenuData->partyMenuData->battlePartyCtx->battlePartyExited) {
            bagMenuData->isCursorEnabled = bagMenuData->partyMenuData->battlePartyCtx->isCursorEnabled;
            bagMenuData->partyMenuData->battlePartyCtx->battlePartyExited = 0;

            if (bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex == 6) {
                bagMenuData->state = 2;
            } else {
                bagMenuData->state = 6;
            }
        }
        break;
    case 6:
        ov12_02237BB8(bagMenuData->battleSys);
        BattleInput_SetKeyPressed(BattleSystem_GetBattleInput(bagMenuData->battleSys), bagMenuData->isCursorEnabled);
        PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_MAIN_OBJ), 0xC00, -8, 7, 0, 0);
        PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_SUB_OBJ), 0xFFFF, -8, 16, 0, 0);
        bagMenuData->state++;
        break;
    case 7:
        if (PaletteData_GetSelectedBuffersBitmask(paletteData) == 0) {
            sub_0201649C(BattleSystem_GetMessageIcon(bagMenuData->battleSys), 0);

            if (bagMenuData->battleBagCtx->selectedBattleBagItem) {
                bagMenuData->state = 9;

                switch (bagMenuData->battleBagCtx->selectedBattleBagPocket) {
                case BATTLE_POCKET_INDEX_RECOVER_STATUS:
                    if (bagMenuData->battleBagCtx->selectedBattleBagItem == ITEM_REVIVE || bagMenuData->battleBagCtx->selectedBattleBagItem == ITEM_MAX_REVIVE) {
                        bagMenuData->state = 8;
                    } else if ((bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex < 2 && (BattleSystem_GetBattleType(bagMenuData->battleSys) == ((1 << 1) | (1 << 0)) || BattleSystem_GetBattleType(bagMenuData->battleSys) == ((1 << 4) | ((1 << 1) | (1 << 0)))))
                        || bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex < 1) {
                        if (bagMenuData->battleBagCtx->selectedBattleBagItem == ITEM_FULL_RESTORE) {
                            if (BattleSystem_AreBattleAnimationsOn(bagMenuData->battleSys) == 1) {
                                bagMenuData->stateAfterDelay = 17;
                            } else {
                                sub_0200602C(1516, -117);
                                bagMenuData->stateAfterDelay = 21;
                            }
                        } else {
                            if (BattleSystem_AreBattleAnimationsOn(bagMenuData->battleSys) == 1) {
                                bagMenuData->stateAfterDelay = 25;
                            } else {
                                sub_0200602C(1516, -117);
                                bagMenuData->stateAfterDelay = 29;
                            }
                        }
                    } else {
                        bagMenuData->state = 8;
                    }
                    break;
                case BATTLE_POCKET_INDEX_RECOVER_HP_PP:
                    if (((bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex < 2 && (BattleSystem_GetBattleType(bagMenuData->battleSys) == ((1 << 1) | (1 << 0)) || BattleSystem_GetBattleType(bagMenuData->battleSys) == ((1 << 4) | ((1 << 1) | (1 << 0)))))
                            || bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex < 1)
                        && GetItemAttr(bagMenuData->battleBagCtx->selectedBattleBagItem, ITEM_PARAM_HP_RESTORE, HEAP_ID_BATTLE)) {
                        if (BattleSystem_AreBattleAnimationsOn(bagMenuData->battleSys) == 1) {
                            bagMenuData->stateAfterDelay = 17;
                        } else {
                            sub_0200602C(1516, -117);
                            bagMenuData->stateAfterDelay = 21;
                        }
                    } else {
                        bagMenuData->state = 8;
                    }
                    break;
                case BATTLE_POCKET_INDEX_BATTLE_ITEMS:
                    if (bagMenuData->battleBagCtx->selectedBattleBagItem == ITEM_POKE_DOLL || bagMenuData->battleBagCtx->selectedBattleBagItem == ITEM_FLUFFY_TAIL) {
                        bagMenuData->state = 8;
                    } else {
                        if (bagMenuData->battleBagCtx->selectedBattleBagItem == ITEM_GUARD_SPEC) {
                            if (BattleSystem_AreBattleAnimationsOn(bagMenuData->battleSys) == 1) {
                                bagMenuData->stateAfterDelay = 13;
                            } else {
                                sub_0200602C(1516, -117);
                                bagMenuData->stateAfterDelay = 15;
                            }
                        } else {
                            if (BattleSystem_AreBattleAnimationsOn(bagMenuData->battleSys) == 1) {
                                bagMenuData->stateAfterDelay = 11;
                            } else {
                                sub_0200602C(1516, -117);
                                bagMenuData->stateAfterDelay = 15;
                            }
                        }
                    }
                    break;
                case BATTLE_POCKET_INDEX_POKE_BALLS:
                    bagMenuData->state = 8;
                    break;
                }
            } else {
                bagMenuData->state = 8;
            }

            if (bagMenuData->state != 8) {
                ov12_02237ED0(bagMenuData->battleSys, 0);
            }
        }
        break;
    case 8:
        BattleItemUse used;

        if (bagMenuData->battleBagCtx->selectedBattleBagItem == ITEM_NONE) {
            used.item = 0xFF;
        } else {
            used.item = bagMenuData->battleBagCtx->selectedBattleBagItem;
            used.category = bagMenuData->battleBagCtx->selectedBattleBagPocket;

            if (bagMenuData->battleBagCtx->selectedBattleBagPocket == BATTLE_POCKET_INDEX_RECOVER_STATUS
                || bagMenuData->battleBagCtx->selectedBattleBagPocket == BATTLE_POCKET_INDEX_RECOVER_HP_PP) {
                used.target = 1 + bagMenuData->partyMenuData->battlePartyCtx->pokemonPartySlots[bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex];
            }
        }

        BattleController_EmitSelectedBagItem(bagMenuData->battleSys, bagMenuData->battler, used);
        BattleController_EmitClearCommand(bagMenuData->battleSys, bagMenuData->battler, bagMenuData->command);

        Heap_Free(bagMenuData->partyMenuData->battlePartyCtx->party);
        Heap_Free(bagMenuData->partyMenuData->battlePartyCtx);
        Heap_Free(bagMenuData->partyMenuData);
        Heap_Free(bagMenuData->battleBagCtx);
        Heap_Free(data);
        SysTask_Destroy(task);
        break;
    case 9:
        BattleMessage battleMsg;

        battleMsg.id = 1206;
        battleMsg.tags = TAG_ITEM;
        battleMsg.params[0] = bagMenuData->battleBagCtx->selectedBattleBagItem;

        struct MsgData *msgLoader = BattleSystem_GetMessageLoader(bagMenuData->battleSys);

        bagMenuData->msgIdx = BattleSystem_PrintBattleMessage(bagMenuData->battleSys, msgLoader, &battleMsg, BattleSystem_GetTextFrameDelay(bagMenuData->battleSys));
        bagMenuData->delay = 30;
        bagMenuData->state++;
        break;
    case 10:
        if (TextPrinterCheckActive(bagMenuData->msgIdx) == 0) {
            if (--bagMenuData->delay == 0) {
                bagMenuData->state = bagMenuData->stateAfterDelay;
            }
        }
        break;
    case 11: {
        MoveAnimation moveAnim;

        BattleController_SetMoveAnimation(bagMenuData->battleSys, 0L, &moveAnim, 1, 9, bagMenuData->battler, bagMenuData->battler, 0L);
        ov12_02261B80(bagMenuData->battleSys, BattleSystem_GetOpponentData(bagMenuData->battleSys, bagMenuData->battler), ov12_0223A8DC(bagMenuData->battleSys), &moveAnim);
    }
        bagMenuData->state++;
        break;
    case 17:
    case 25: {
        MoveAnimation moveAnim;
        int battler = bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex * 2;
        BattleController_SetMoveAnimation(bagMenuData->battleSys, 0L, &moveAnim, 1, 9, battler, battler, 0L);
        ov12_02261B80(bagMenuData->battleSys, BattleSystem_GetOpponentData(bagMenuData->battleSys, battler), ov12_0223A8DC(bagMenuData->battleSys), &moveAnim);
    }
        bagMenuData->state++;
        break;
    case 13: {
        MoveAnimation moveAnim;

        if (bagMenuData->battleBagCtx->selectedBattleBagItem == ITEM_GUARD_SPEC) {
            BattleController_SetMoveAnimation(bagMenuData->battleSys, 0L, &moveAnim, 0, 0L, bagMenuData->battler, bagMenuData->battler, 54);
            ov12_02261B80(bagMenuData->battleSys, BattleSystem_GetOpponentData(bagMenuData->battleSys, bagMenuData->battler), ov12_0223A8DC(bagMenuData->battleSys), &moveAnim);
        } else if (bagMenuData->battleBagCtx->selectedBattleBagItem == ITEM_DIRE_HIT) {
            BattleController_SetMoveAnimation(bagMenuData->battleSys, 0L, &moveAnim, 0, 0L, bagMenuData->battler, bagMenuData->battler, 116);
            ov12_02261B80(bagMenuData->battleSys, BattleSystem_GetOpponentData(bagMenuData->battleSys, bagMenuData->battler), ov12_0223A8DC(bagMenuData->battleSys), &moveAnim);
        } else {
            BattleController_SetMoveAnimation(bagMenuData->battleSys, 0L, &moveAnim, 1, 12, bagMenuData->battler, bagMenuData->battler, 0L);
            ov12_02261B80(bagMenuData->battleSys, BattleSystem_GetOpponentData(bagMenuData->battleSys, bagMenuData->battler), ov12_0223A8DC(bagMenuData->battleSys), &moveAnim);
        }

        bagMenuData->state++;
    } break;
    case 15: {
        BattleMessage battleMsg;

        battleMsg.id = 1203;
        battleMsg.tags = TAG_NICKNAME_STAT;
        battleMsg.params[0] = bagMenuData->battler | (bagMenuData->partyMenuData->partySlots[bagMenuData->battler] << 8);

        switch (bagMenuData->battleBagCtx->selectedBattleBagItem) {
        case ITEM_X_ATTACK:
            battleMsg.params[1] = 1;
            break;
        case ITEM_X_DEFENSE:
            battleMsg.params[1] = 2;
            break;
        case ITEM_X_SPEED:
            battleMsg.params[1] = 3;
            break;
        case ITEM_X_ACCURACY:
            battleMsg.params[1] = 6;
            break;
        case ITEM_X_SPECIAL:
            battleMsg.params[1] = 4;
            break;
        case ITEM_X_SP_DEF:
            battleMsg.params[1] = 5;
            break;
        case ITEM_GUARD_SPEC:
            battleMsg.id = 1204;
            battleMsg.tags = TAG_NONE;
            break;
        case ITEM_DIRE_HIT:
            battleMsg.id = 1205;
            battleMsg.tags = TAG_NICKNAME;
            break;
        }

        struct MsgData *msgLoader = BattleSystem_GetMessageLoader(bagMenuData->battleSys);
        bagMenuData->msgIdx = BattleSystem_PrintBattleMessage(bagMenuData->battleSys, msgLoader, &battleMsg, BattleSystem_GetTextFrameDelay(bagMenuData->battleSys));
        bagMenuData->delay = 30;
    }
        bagMenuData->state++;
        break;
    case 19:
    case 27: {
        MoveAnimation moveAnim;
        int battler = bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex * 2;
        BattleController_SetMoveAnimation(bagMenuData->battleSys, 0L, &moveAnim, 1, 14, battler, battler, 0L);
        ov12_02261B80(bagMenuData->battleSys, BattleSystem_GetOpponentData(bagMenuData->battleSys, battler), ov12_0223A8DC(bagMenuData->battleSys), &moveAnim);
    }
        bagMenuData->state++;
        break;
    case 21: {

        int battler = bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex * 2;
        int slots = bagMenuData->partyMenuData->battlePartyCtx->pokemonPartySlots[bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex];

        BattleHpBar *healthbox = BattleSystem_GetHpBar(bagMenuData->battleSys, battler);
        MI_CpuClear8(&healthbox->state, sizeof(u8));
        healthbox->type = BattleHpBar_Util_GetBarTypeFromBattlerSide(ov12_0223AB0C(bagMenuData->battleSys, battler), BattleSystem_GetBattleType(bagMenuData->battleSys));

        Pokemon *mon = BattleSystem_GetPartyMon(bagMenuData->battleSys, battler, slots);
        healthbox->curHP = GetMonData(mon, MON_DATA_HP, 0L) - bagMenuData->partyMenuData->battlePartyCtx->currentDamage;
        healthbox->maxHP = GetMonData(mon, MON_DATA_MAX_HP, 0L);
        healthbox->damage = bagMenuData->partyMenuData->battlePartyCtx->currentDamage;

        if (GetMonData(mon, MON_DATA_STATUS, 0L) == 0) {
            healthbox->status = 0;
        }

        ov12_02264DCC(healthbox, healthbox->damage);
    }
        bagMenuData->state++;
        break;
    case 22: {
        int battler = bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex * 2;
        BattleHpBar *healthbox = BattleSystem_GetHpBar(bagMenuData->battleSys, battler);

        if (ov12_02264E00(healthbox) == -1) {
            ov12_0226498C(healthbox, 0L, (1 << 8));
            bagMenuData->state++;
        }
    } break;
    case 23: {
        BattleMessage battleMsg;

        struct MsgData *msgLoader = BattleSystem_GetMessageLoader(bagMenuData->battleSys);
        int battler = bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex * 2;

        if (bagMenuData->partyMenuData->battlePartyCtx->currentDamage) {
            battleMsg.id = 1214;
            battleMsg.tags = TAG_NICKNAME_NUM;
            battleMsg.params[0] = battler | (bagMenuData->partyMenuData->partySlots[battler] << 8);
            battleMsg.params[1] = bagMenuData->partyMenuData->battlePartyCtx->currentDamage;
        } else {
            battleMsg.id = 1250;
            battleMsg.tags = TAG_NICKNAME;
            battleMsg.params[0] = battler | (bagMenuData->partyMenuData->partySlots[battler] << 8);
        }

        bagMenuData->msgIdx = BattleSystem_PrintBattleMessage(bagMenuData->battleSys, msgLoader, &battleMsg, BattleSystem_GetTextFrameDelay(bagMenuData->battleSys));
        bagMenuData->delay = 30;

        bagMenuData->state++;
    } break;
    case 29: {
        BattleMessage battleMsg;
        int msgType;
        int effect = 0;

        int battler = bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex * 2;
        BattleHpBar *healthbox = BattleSystem_GetHpBar(bagMenuData->battleSys, battler);
        int slot = bagMenuData->partyMenuData->battlePartyCtx->pokemonPartySlots[bagMenuData->partyMenuData->battlePartyCtx->selectedPartyIndex];
        Pokemon *mon = BattleSystem_GetPartyMon(bagMenuData->battleSys, battler, slot);

        if (GetMonData(mon, MON_DATA_STATUS, 0L) == 0) {
            healthbox->status = 0;
        }

        ov12_0226498C(healthbox, healthbox->curHP, (1 << 8));

        battleMsg.tags = TAG_NICKNAME;
        battleMsg.params[0] = battler | (bagMenuData->partyMenuData->partySlots[battler] << 8);

        if (GetItemAttr(bagMenuData->battleBagCtx->selectedBattleBagItem, ITEM_PARAM_HEAL_SLEEP, HEAP_ID_BATTLE)) {
            msgType = 0;
            effect++;
        }

        if (GetItemAttr(bagMenuData->battleBagCtx->selectedBattleBagItem, ITEM_PARAM_HEAL_POISON, HEAP_ID_BATTLE)) {
            msgType = 1;
            effect++;
        }

        if (GetItemAttr(bagMenuData->battleBagCtx->selectedBattleBagItem, ITEM_PARAM_HEAL_BURN, HEAP_ID_BATTLE)) {
            msgType = 2;
            effect++;
        }

        if (GetItemAttr(bagMenuData->battleBagCtx->selectedBattleBagItem, ITEM_PARAM_HEAL_FREEZE, HEAP_ID_BATTLE)) {
            msgType = 3;
            effect++;
        }

        if (GetItemAttr(bagMenuData->battleBagCtx->selectedBattleBagItem, ITEM_PARAM_HEAL_PARALYSIS, HEAP_ID_BATTLE)) {
            msgType = 4;
            effect++;
        }

        if (GetItemAttr(bagMenuData->battleBagCtx->selectedBattleBagItem, ITEM_PARAM_HEAL_CONFUSION, HEAP_ID_BATTLE)) {
            msgType = 5;
            effect++;
        }

        if (GetItemAttr(bagMenuData->battleBagCtx->selectedBattleBagItem, ITEM_PARAM_HEAL_ATTRACT, HEAP_ID_BATTLE)) {
            msgType = 6;
            effect++;
        }

        if (effect != 1) {
            battleMsg.id = 1229;
        } else {
            switch (msgType) {
            case 0:
                battleMsg.id = 1210;
                break;
            case 1:
                battleMsg.id = 1207;
                break;
            case 2:
                battleMsg.id = 1209;
                break;
            case 3:
                battleMsg.id = 1211;
                break;
            case 4:
                battleMsg.id = 1208;
                break;
            case 5:
                battleMsg.id = 1212;
                break;
            case 6:
                battleMsg.id = 1213;
                break;
            }
        }

        struct MsgData *msgLoader = BattleSystem_GetMessageLoader(bagMenuData->battleSys);
        bagMenuData->msgIdx = BattleSystem_PrintBattleMessage(bagMenuData->battleSys, msgLoader, &battleMsg, BattleSystem_GetTextFrameDelay(bagMenuData->battleSys));
        bagMenuData->delay = 30;
    }
        bagMenuData->state++;
        break;

    case 12:
    case 14:
    case 18:
    case 20:
    case 26:
    case 28:
        ov07_0221C394(ov12_0223A8DC(bagMenuData->battleSys));

        if (ov07_0221C3B0(ov12_0223A8DC(bagMenuData->battleSys)) == 0) {
            ov07_0221C3C0(ov12_0223A8DC(bagMenuData->battleSys));
            bagMenuData->state++;
        }
        break;
    case 16:
    case 24:
    case 30:
        if (TextPrinterCheckActive(bagMenuData->msgIdx) == 0) {
            if (--bagMenuData->delay == 0) {
                ov12_02237ED0(bagMenuData->battleSys, 1);
                bagMenuData->state = 8;
            }
        }
        break;
    }
}
