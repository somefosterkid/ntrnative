#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_70_gtsapplication_state.h"
#include "unk_0202C034.h"
#include "bag_types_def.h"
#include "options.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "palette.h"
#include "overlay_74_main_menu_util.h"
#include "transform_data.h"
#include "pokemon_anim.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokedex.h"
#include "camera.h"
#include "pokemon_summary_app.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "particle_system.h"
#include "evolution_data.h"
#include "list_menu.h"
#include "msgdata.h"
#include "sav_chatot.h"
#include "pokepic.h"
#include "sav_system_info.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "overlay_55.h"
#include "gts_player_data.h"

struct AffineSpriteListTemplate;

struct OverlayManager;

struct OverlayManagerTemplate;

struct Background;

struct Bag;

struct BagItem;

struct BallCapsule;

struct BallSeal;

struct BgConfig;

struct BoxPokemon;

struct Camera;

struct CameraAngle;

struct CameraLookAt;

struct CameraPerspective;

struct CameraPositionHistory;

struct SOUND_CHATOT;

struct ColoredArrow;

struct DWCstInetControl;

struct MailMessage;

struct EncodingSeed;

struct EvolutionData;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct GTSApplicationState;

struct GTSBoxPokemonCriteria;

struct GTSCharpadScrollState;

struct GTSPlayerData;

struct GTSPokemonCriteria;

struct GTSPokemonListing;

struct GTSPokemonListingMon;

struct GTSPokemonRequirements;

struct GTSTradedPokemonLocation;

struct GameTime;

struct GlobalTrade;

struct JournalEntry;

struct JournalEntryMon;

struct JournalEntryTitle;

struct JournalEntryTrainer;

struct ListMenu;

struct ListMenuTemplate;

struct Location;

struct Mail;

struct Menu;

struct MenuTemplate;

struct MessageBank;

struct MessageBankEntry;


struct NARC;

struct Options;

struct PCBoxes;

struct PaletteAnimator;

struct PaletteBuffer;

struct PaletteData;

struct SelectedPaletteData;

struct ParticleSystem;

struct Party;

struct PartyPokemon;

struct Pokedex;

struct Pokemon;

struct PokemonAnim;

struct PokemonAnimManager;

struct PokemonDataBlockA;

struct PokemonDataBlockB;

struct PokemonDataBlockC;

struct PokemonDataBlockD;

struct Pokepic;

struct PokepicManager;

struct PokepicShadow;

struct PokepicTemplate;

struct PokepicDrawParam;

struct PokemonSummary;

struct Poketch;

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

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct Sprite;

struct SpriteAnimFrame;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskQueue;

struct SysInfo;

struct TradeAnimationTemplate;

struct PlayerProfile;

struct TransformData;

struct WiFiHistory;

struct WiFiList;

struct Window;

struct WorldExchangeTrainer;

struct WorldExchangeTrainerError;

struct tagDWCWDSData;






typedef struct WiFiHistory {
    u32 unused_00;
    u8 hasInteractedOutsideOfJapan;
    u8 country;
    u8 region;
    u8 geonetCommunicationMap[4080];
} WiFiHistory;


enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};

enum SaveTableEntryID {
    SAVE_TABLE_ENTRY_SYSTEM = 0,
    SAVE_TABLE_ENTRY_PLAYER,
    SAVE_TABLE_ENTRY_PARTY,
    SAVE_TABLE_ENTRY_BAG,
    SAVE_TABLE_ENTRY_VARS_FLAGS,
    SAVE_TABLE_ENTRY_POKETCH,
    SAVE_TABLE_ENTRY_FIELD_PLAYER_STATE,
    SAVE_TABLE_ENTRY_POKEDEX,
    SAVE_TABLE_ENTRY_DAYCARE,
    SAVE_TABLE_ENTRY_PAL_PAD,
    SAVE_TABLE_ENTRY_MISC,
    SAVE_TABLE_ENTRY_FIELD_OVERWORLD_STATE,
    SAVE_TABLE_ENTRY_UNDERGROUND,
    SAVE_TABLE_ENTRY_REGULATION_BATTLES,
    SAVE_TABLE_ENTRY_IMAGE_CLIPS,
    SAVE_TABLE_ENTRY_MAILBOX,
    SAVE_TABLE_ENTRY_POFFINS,
    SAVE_TABLE_ENTRY_RECORD_MIXED_RNG,
    SAVE_TABLE_ENTRY_JOURNAL,
    SAVE_TABLE_ENTRY_TRAINER_CASE,
    SAVE_TABLE_ENTRY_GAME_RECORDS,
    SAVE_TABLE_ENTRY_SEAL_CASE,
    SAVE_TABLE_ENTRY_CHATOT,
    SAVE_TABLE_ENTRY_FRONTIER,
    SAVE_TABLE_ENTRY_RIBBONS,
    SAVE_TABLE_ENTRY_ENCOUNTERS,
    SAVE_TABLE_ENTRY_GLOBAL_TRADE,
    SAVE_TABLE_ENTRY_TV_BROADCAST,
    SAVE_TABLE_ENTRY_RANKINGS,
    SAVE_TABLE_ENTRY_WIFI_LIST,
    SAVE_TABLE_ENTRY_WIFI_HISTORY,
    SAVE_TABLE_ENTRY_MYSTERY_GIFT,
    SAVE_TABLE_ENTRY_PAL_PARK_TRANSFER,
    SAVE_TABLE_ENTRY_LINK_CONTEST_RECORDS,
    SAVE_TABLE_ENTRY_UNLOCKED_EASY_CHAT_WORDS,
    SAVE_TABLE_ENTRY_EMAIL,
    SAVE_TABLE_ENTRY_WIFI_PLAZA_QUESTIONS,
    SAVE_TABLE_ENTRY_PC_BOXES,

    SAVE_TABLE_ENTRY_MAX
};

typedef struct SaveBlockInfo {
    u8 saveBlockID;
    u8 sectorStartPos;
    u8 sectorsInUse;
    u32 offset;
    u32 size;
} SaveBlockInfo;

typedef struct SavePageInfo {
    int pageID;
    u32 size;
    u32 location;
    u16 checksum;
    u16 blockID;
} SavePageInfo;

typedef struct SaveDataBody {
    u8 data[(0x1000) * 32];
} SaveDataBody;

typedef struct SaveDataState {
    BOOL fullSaveMode;
    int startBlock;
    int currentBlock;
    int endBlock;
    s32 lockID;
    int mainSequence;
    u32 globalCounterBackup;
    u32 blockCounterBackup[SAVE_BLOCK_ID_MAX];
    volatile BOOL locked;
} SaveDataState;

typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;



void Heap_Free(void *ptr);

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef u16 charcode_t;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;


void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};


typedef void (*TransformFunc)(TransformData *, PokemonAnim *);

typedef struct PokemonAnimManager {
    PokemonAnim *anims;
    enum HeapID heapID;
    u8 flipSprite;
    u8 animCount;
} PokemonAnimManager;


typedef struct AffineSpriteListTemplate {
    SpriteList *list;
    const SpriteResourcesHeader *resourceData;
    VecFx32 position;
    VecFx32 affineScale;
    u16 affineZRotation;
    u32 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    enum HeapID heapID;
} AffineSpriteListTemplate;

enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};


typedef struct EncodingSeed {
    u16 byteSum;
    u16 modifier;
} EncodingSeed;

typedef struct GlobalTrade {
    u8 storedPokemon[sizeof(Pokemon)];
    u16 pokemonListed;
    u16 neverSet_EE;
    u32 depositTradeDatestamp;
    u32 searchTradeDatestamp;
} GlobalTrade;

typedef struct JournalEntryTitle {
    u32 year : 7;
    u32 month : 4;
    u32 week : 3;
    u32 day : 5;
    u32 mapID : 13;
} JournalEntryTitle;

typedef struct JournalEntryMon {
    u8 battleResult;
    u8 stringVariant : 2;
    u8 timeOfDay : 4;
    u8 gender : 2;
    u16 species;
} JournalEntryMon;

typedef struct JournalEntryTrainer {
    u16 standard : 1;
    u16 trainerID : 15;
    u16 mapID;
} JournalEntryTrainer;

typedef struct JournalEntry {
    JournalEntryTitle title;
    u32 locationEvents[4];
    JournalEntryMon mon;
    JournalEntryTrainer trainer;
    u8 onlineEvents[2][42];
} JournalEntry;

typedef struct PCBoxes {
    u32 currentBoxID;
    BoxPokemon boxMons[18][(5 * 6)];
    u16 names[18][20];
    u8 wallpapers[18];
    u8 unlockedWallpapers;
} PCBoxes;

typedef struct GameTime {
    BOOL canary;
    RTCDate date;
    RTCTime time;
    u32 day;
    s64 startTimestamp;
    s64 firstCompletionTimestamp;
    u32 penaltyInMinutes;
} GameTime;






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

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;


typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

typedef u32 TextColor;

enum TradeType {
    TRADE_TYPE_NORMAL = 1,
    TRADE_TYPE_SEND_ONLY,
    TRADE_TYPE_RECEIVE_ONLY = 4,
};

enum TradeBackground {
    TRADE_BACKGROUND_DAY = 0,
    TRADE_BACKGROUND_EVENING,
    TRADE_BACKGROUND_NIGHT,
    TRADE_BACKGROUND_WIFI,
};


typedef struct {
    enum HeapID heapID;
    int unk_04;
} UnkStruct_0207C894;

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

typedef void *(*SPLAllocFunc)(u32);

typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);

typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);

typedef struct {
    UnkStruct_0207C894 unk_00;
    SPLEmitter *unk_08;
    ParticleSystem *unk_0C;
} UnkStruct_0207C8C4;

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct Poketch {
    u8 poketchEnabled : 1;
    u8 pedometerEnabled : 1;
    u8 dotArtModifiedByPlayer : 1;
    u8 screenColor : 3;

    s8 appCount;
    s8 appIndex;
    u8 appRegistry[32];

    u32 stepCount;

    u16 alarmSet : 1;
    u16 alarmHour : 5;
    u16 alarmMinute : 6;

    u8 dotArtData[120];

    u32 calendarMarkBitmap;
    u8 calendarMonth;

    struct {
        u8 x;
        u8 y;
    } markMapPositions[6];

    struct {
        u16 species;
        u16 icon;
        u32 form;
    } pokemonHistoryQueue[12];
} Poketch;

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

typedef struct ColoredArrow {
    TextColor color;
    String *string;
} ColoredArrow;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct MenuTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} MenuTemplate;

typedef struct Menu {
    MenuTemplate template;
    ColoredArrow *cursor;
    u32 cancelKeys;
    u8 dummy14;
    u8 cursorPos;
    u8 width;
    u8 xOffset;
    u8 yOffset;
    u8 letterWidth;
    u8 lineHeight;
    u8 lastAction;
    u8 heapID;
} Menu;







typedef struct GTSBoxPokemonCriteria {
    GTSPokemonCriteria criteria[(5 * 6)];
} GTSBoxPokemonCriteria;


enum ScreenArgument {
    SCREEN_ARGUMENT_NONE = 0,
    SCREEN_ARGUMENT_RESUME_LISTING = 3,
    SCREEN_ARGUMENT_CHOOSE_OFFER_MON = 5,
    SCREEN_ARGUMENT_CHOOSE_LISTED_MON,
    SCREEN_ARGUMENT_DEPOSIT_POKEMON,
    SCREEN_ARGUMENT_TAKE_BACK_POKEMON,
    SCREEN_ARGUMENT_EXCHANGE_POKEMON,
    SCREEN_ARGUMENT_EXCHANGE_POKEMON_ALT,
    SCREEN_ARGUMENT_CHECK_SERVER,
    SCREEN_ARGUMENT_SAVE_AFTER_EVOLVE,
};

enum MapHeaderID {
    MAP_HEADER_EVERYWHERE,
    MAP_HEADER_NOTHING,
    MAP_HEADER_UNDERGROUND,
    MAP_HEADER_JUBILIFE_CITY,
    MAP_HEADER_JUBILIFE_CITY_MART,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_HOUSE_1,
    MAP_HEADER_JUBILIFE_CITY_POKECENTER_1F,
    MAP_HEADER_JUBILIFE_CITY_POKECENTER_2F,
    MAP_HEADER_POKETCH_CO_1F,
    MAP_HEADER_POKETCH_CO_2F,
    MAP_HEADER_POKETCH_CO_3F,
    MAP_HEADER_JUBILIFE_TV_1F,
    MAP_HEADER_JUBILIFE_TV_2F,
    MAP_HEADER_JUBILIFE_TV_3F,
    MAP_HEADER_JUBILIFE_TV_4F,
    MAP_HEADER_JUBILIFE_TV_2F_GALLERY,
    MAP_HEADER_JUBILIFE_TV_3F_GLOBAL_RANKING_ROOM,
    MAP_HEADER_JUBILIFE_TV_3F_GROUP_RANKING_ROOM,
    MAP_HEADER_JUBILIFE_TV_ELEVATOR,
    MAP_HEADER_JUBILIFE_CITY_SOUTH_HOUSE_1F,
    MAP_HEADER_JUBILIFE_CITY_SOUTH_HOUSE_2F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_SOUTH_HOUSE_3F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_SOUTH_HOUSE_4F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_HOUSE_2,
    MAP_HEADER_JUBILIFE_CITY_CONDOMINIUMS_1F,
    MAP_HEADER_JUBILIFE_CITY_CONDOMINIUMS_2F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_CONDOMINIUMS_3F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_CONDOMINIUMS_4F,
    MAP_HEADER_GLOBAL_TERMINAL_1F,
    MAP_HEADER_TRAINERS_SCHOOL,
    MAP_HEADER_JUBILIFE_CITY_SOUTHWEST_HOUSE_1F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_HOUSE_3,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_HOUSE_4,
    MAP_HEADER_CANALAVE_CITY,
    MAP_HEADER_CANALAVE_CITY_MART,
    MAP_HEADER_CANALAVE_CITY_GYM,
    MAP_HEADER_CANALAVE_CITY_POKECENTER_1F,
    MAP_HEADER_CANALAVE_CITY_POKECENTER_2F,
    MAP_HEADER_CANALAVE_LIBRARY_1F,
    MAP_HEADER_CANALAVE_LIBRARY_2F,
    MAP_HEADER_CANALAVE_LIBRARY_3F,
    MAP_HEADER_CANALAVE_CITY_SOUTHEAST_HOUSE,
    MAP_HEADER_CANALAVE_CITY_EAST_HOUSE,
    MAP_HEADER_CANALAVE_CITY_HARBOR_INN,
    MAP_HEADER_CANALAVE_CITY_SAILOR_ELDRITCH_HOUSE,
    MAP_HEADER_OREBURGH_CITY,
    MAP_HEADER_OREBURGH_CITY_MART,
    MAP_HEADER_OREBURGH_CITY_GYM,
    MAP_HEADER_OREBURGH_CITY_POKECENTER_1F,
    MAP_HEADER_OREBURGH_CITY_POKECENTER_2F,
    MAP_HEADER_OREBURGH_CITY_NORTHWEST_HOUSE_1F,
    MAP_HEADER_OREBURGH_CITY_NORTHWEST_HOUSE_2F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_NORTHWEST_HOUSE_3F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_NORTHWEST_HOUSE_4F,
    MAP_HEADER_OREBURGH_CITY_NORTH_HOUSE_1F,
    MAP_HEADER_OREBURGH_CITY_NORTH_HOUSE_2F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_NORTH_HOUSE_3F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_NORTH_HOUSE_4F,
    MAP_HEADER_OREBURGH_CITY_MIDDLE_HOUSE,
    MAP_HEADER_MINING_MUSEUM,
    MAP_HEADER_OREBURGH_CITY_WEST_HOUSE,
    MAP_HEADER_OREBURGH_CITY_EAST_HOUSE_1F,
    MAP_HEADER_OREBURGH_CITY_EAST_HOUSE_2F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_EAST_HOUSE_3F,
    MAP_HEADER_OREBURGH_CITY_SOUTH_HOUSE,
    MAP_HEADER_ETERNA_CITY,
    MAP_HEADER_ETERNA_CITY_MART,
    MAP_HEADER_ETERNA_CITY_GYM,
    MAP_HEADER_ETERNA_CITY_DP_GYM,
    MAP_HEADER_ETERNA_CITY_POKECENTER_1F,
    MAP_HEADER_ETERNA_CITY_POKECENTER_2F,
    MAP_HEADER_CYCLE_SHOP,
    MAP_HEADER_TEAM_GALACTIC_ETERNA_BUILDING_1F,
    MAP_HEADER_TEAM_GALACTIC_ETERNA_BUILDING_2F,
    MAP_HEADER_TEAM_GALACTIC_ETERNA_BUILDING_3F,
    MAP_HEADER_TEAM_GALACTIC_ETERNA_BUILDING_4F,
    MAP_HEADER_ETERNA_CITY_CONDOMINIUMS_1F,
    MAP_HEADER_ETERNA_CITY_CONDOMINIUMS_2F,
    MAP_HEADER_ETERNA_CITY_CONDOMINIUMS_3F,
    MAP_HEADER_UNUSED_ETERNA_CITY_CONDOMINIUMS_4F,
    MAP_HEADER_ROUTE_206_CYCLING_ROAD_NORTH_GATE,
    MAP_HEADER_ETERNA_CITY_HERB_SHOP,
    MAP_HEADER_ETERNA_CITY_SOUTH_HOUSE,
    MAP_HEADER_ETERNA_CITY_EAST_HOUSE,
    MAP_HEADER_ETERNA_CITY_UNDERGROUND_MAN_HOUSE,
    MAP_HEADER_UNUSED_ETERNA_CITY_HOUSE,
    MAP_HEADER_HEARTHOME_CITY,
    MAP_HEADER_HEARTHOME_CITY_MART,
    MAP_HEADER_HEARTHOME_CITY_GYM_ENTRANCE_ROOM,
    MAP_HEADER_HEARTHOME_CITY_GYM_TRAINER_ROOM_1,
    MAP_HEADER_HEARTHOME_CITY_GYM_TRAINER_ROOM_2,
    MAP_HEADER_HEARTHOME_CITY_GYM_LEADER_ROOM,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_1,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_ELEVATOR_ROOM_1,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_2,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_ELEVATOR_ROOM_2,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_3,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_4,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_5,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_6,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_LEADER_ROOM,
    MAP_HEADER_HEARTHOME_CITY_POKECENTER_1F,
    MAP_HEADER_HEARTHOME_CITY_POKECENTER_2F,
    MAP_HEADER_HEARTHOME_CITY_SOUTHEAST_HOUSE_1F,
    MAP_HEADER_HEARTHOME_CITY_SOUTHEAST_HOUSE_2F,
    MAP_HEADER_HEARTHOME_CITY_SOUTHEAST_HOUSE_ELEVATOR,
    MAP_HEADER_HEARTHOME_CITY_POKEMON_FAN_CLUB,
    MAP_HEADER_HEARTHOME_CITY_WEST_GATE_TO_AMITY_SQUARE,
    MAP_HEADER_HEARTHOME_CITY_EAST_GATE_TO_AMITY_SQUARE,
    MAP_HEADER_ROUTE_208_GATE_TO_HEARTHOME_CITY,
    MAP_HEADER_ROUTE_209_GATE_TO_HEARTHOME_CITY,
    MAP_HEADER_ROUTE_212_GATE_TO_HEARTHOME_CITY,
    MAP_HEADER_HEARTHOME_CITY_NORTHEAST_HOUSE_1F,
    MAP_HEADER_HEARTHOME_CITY_NORTHEAST_HOUSE_2F,
    MAP_HEADER_HEARTHOME_CITY_NORTHEAST_HOUSE_ELEVATOR,
    MAP_HEADER_HEARTHOME_CITY_NORTHWEST_HOUSE,
    MAP_HEADER_POFFIN_HOUSE,
    MAP_HEADER_CONTEST_HALL_LOBBY,
    MAP_HEADER_CONTEST_HALL_STAGE_ONGOING_CONTEST,
    MAP_HEADER_FOREIGN_BUILDING,
    MAP_HEADER_PASTORIA_CITY,
    MAP_HEADER_PASTORIA_CITY_MART,
    MAP_HEADER_PASTORIA_CITY_GYM,
    MAP_HEADER_PASTORIA_CITY_POKECENTER_1F,
    MAP_HEADER_PASTORIA_CITY_POKECENTER_2F,
    MAP_HEADER_PASTORIA_CITY_OBSERVATORY_GATE_1F,
    MAP_HEADER_PASTORIA_CITY_OBSERVATORY_GATE_2F,
    MAP_HEADER_PASTORIA_CITY_SOUTHWEST_HOUSE,
    MAP_HEADER_PASTORIA_CITY_MIDDLE_HOUSE,
    MAP_HEADER_PASTORIA_CITY_EAST_HOUSE,
    MAP_HEADER_PASTORIA_CITY_NORTH_HOUSE,
    MAP_HEADER_PASTORIA_CITY_NORTHEAST_HOUSE,
    MAP_HEADER_VEILSTONE_CITY,
    MAP_HEADER_VEILSTONE_CITY_GYM,
    MAP_HEADER_VEILSTONE_CITY_POKECENTER_1F,
    MAP_HEADER_VEILSTONE_CITY_POKECENTER_2F,
    MAP_HEADER_GAME_CORNER,
    MAP_HEADER_VEILSTONE_STORE_1F,
    MAP_HEADER_VEILSTONE_STORE_2F,
    MAP_HEADER_VEILSTONE_STORE_3F,
    MAP_HEADER_VEILSTONE_STORE_4F,
    MAP_HEADER_VEILSTONE_STORE_5F,
    MAP_HEADER_VEILSTONE_STORE_ELEVATOR,
    MAP_HEADER_VEILSTONE_CITY_GALACTIC_WAREHOUSE,
    MAP_HEADER_VEILSTONE_CITY_PRIZE_EXCHANGE,
    MAP_HEADER_VEILSTONE_CITY_SOUTHEAST_HOUSE,
    MAP_HEADER_VEILSTONE_CITY_NORTHWEST_HOUSE,
    MAP_HEADER_VEILSTONE_CITY_NORTHEAST_HOUSE,
    MAP_HEADER_VEILSTONE_CITY_SOUTHWEST_HOUSE,
    MAP_HEADER_ROUTE_215_GATE_TO_VEILSTONE_CITY,
    MAP_HEADER_SUNYSHORE_CITY,
    MAP_HEADER_SUNYSHORE_CITY_POKECENTER_1F,
    MAP_HEADER_SUNYSHORE_CITY_POKECENTER_2F,
    MAP_HEADER_SUNYSHORE_CITY_MART,
    MAP_HEADER_SUNYSHORE_CITY_GYM_ROOM_1,
    MAP_HEADER_SUNYSHORE_CITY_GYM_ROOM_2,
    MAP_HEADER_SUNYSHORE_CITY_GYM_ROOM_3,
    MAP_HEADER_SUNYSHORE_MARKET,
    MAP_HEADER_SUNYSHORE_CITY_NORTHEAST_HOUSE,
    MAP_HEADER_SUNYSHORE_CITY_WEST_HOUSE,
    MAP_HEADER_SUNYSHORE_CITY_NORTHWEST_HOUSE,
    MAP_HEADER_UNUSED_SUNYSHORE_CITY_HOUSE_1,
    MAP_HEADER_UNUSED_SUNYSHORE_CITY_HOUSE_2,
    MAP_HEADER_SUNYSHORE_CITY_EAST_HOUSE,
    MAP_HEADER_VISTA_LIGHTHOUSE,
    MAP_HEADER_SNOWPOINT_CITY,
    MAP_HEADER_SNOWPOINT_CITY_MART,
    MAP_HEADER_SNOWPOINT_CITY_GYM,
    MAP_HEADER_SNOWPOINT_CITY_POKECENTER_1F,
    MAP_HEADER_SNOWPOINT_CITY_POKECENTER_2F,
    MAP_HEADER_SNOWPOINT_CITY_WEST_HOUSE,
    MAP_HEADER_SNOWPOINT_CITY_EAST_HOUSE,
    MAP_HEADER_POKEMON_LEAGUE,
    MAP_HEADER_POKEMON_LEAGUE_SOUTH_POKECENTER_1F,
    MAP_HEADER_POKEMON_LEAGUE_SOUTH_POKECENTER_2F,
    MAP_HEADER_POKEMON_LEAGUE_NORTH_POKECENTER_1F,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_AARON_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_AARON_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_BERTHA_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_BERTHA_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_FLINT_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_FLINT_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_LUCIAN_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_LUCIAN_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_CHAMPION_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_CHAMPION_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_HALLWAY_TO_HALL_OF_FAME,
    MAP_HEADER_POKEMON_LEAGUE_HALL_OF_FAME,
    MAP_HEADER_FIGHT_AREA,
    MAP_HEADER_FIGHT_AREA_POKECENTER_1F,
    MAP_HEADER_FIGHT_AREA_POKECENTER_2F,
    MAP_HEADER_FIGHT_AREA_MART,
    MAP_HEADER_UNUSED_BATTLE_PARK_GATE_TO_FIGHT_AREA,
    MAP_HEADER_ROUTE_225_GATE_TO_FIGHT_AREA,
    MAP_HEADER_FIGHT_AREA_MIDDLE_HOUSE,
    MAP_HEADER_FIGHT_AREA_SOUTH_HOUSE,
    MAP_HEADER_UNUSED_FIGHT_AREA_HOUSE,
    MAP_HEADER_UNKNOWN_197,
    MAP_HEADER_OREBURGH_MINE_B1F,
    MAP_HEADER_OREBURGH_MINE_B2F,
    MAP_HEADER_VALLEY_WINDWORKS_OUTSIDE,
    MAP_HEADER_VALLEY_WINDWORKS_BUILDING,
    MAP_HEADER_ETERNA_FOREST_OUTSIDE,
    MAP_HEADER_ETERNA_FOREST,
    MAP_HEADER_FUEGO_IRONWORKS_OUTSIDE,
    MAP_HEADER_FUEGO_IRONWORKS_BUILDING,
    MAP_HEADER_UNKNOWN_206,
    MAP_HEADER_MT_CORONET_1F_SOUTH,
    MAP_HEADER_MT_CORONET_2F,
    MAP_HEADER_MT_CORONET_3F,
    MAP_HEADER_MT_CORONET_OUTSIDE_NORTH,
    MAP_HEADER_MT_CORONET_OUTSIDE_SOUTH,
    MAP_HEADER_MT_CORONET_4F_ROOMS_1_AND_2,
    MAP_HEADER_MT_CORONET_4F_ROOM_3,
    MAP_HEADER_MT_CORONET_5F,
    MAP_HEADER_MT_CORONET_6F,
    MAP_HEADER_MT_CORONET_1F_TUNNEL_ROOM,
    MAP_HEADER_MT_CORONET_1F_NORTH_ROOM_2,
    MAP_HEADER_MT_CORONET_1F_NORTH_ROOM_1,
    MAP_HEADER_MT_CORONET_B1F,
    MAP_HEADER_SPEAR_PILLAR,
    MAP_HEADER_SPEAR_PILLAR_DISTORTED,
    MAP_HEADER_UNKNOWN_222,
    MAP_HEADER_PASTORIA_CITY_DP_GREAT_MARSH,
    MAP_HEADER_UNKNOWN_224,
    MAP_HEADER_SOLACEON_RUINS_MANIAC_TUNNEL_ROOM,
    MAP_HEADER_SOLACEON_RUINS_ROOM_1,
    MAP_HEADER_SOLACEON_RUINS_ROOM_2_NORTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_1_NORTHWEST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_2,
    MAP_HEADER_SOLACEON_RUINS_ROOM_1_SOUTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_3,
    MAP_HEADER_SOLACEON_RUINS_ROOM_2_SOUTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_6_SOUTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_5_SOUTHWEST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_3_NORTHWEST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_3_SOUTHWEST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_4,
    MAP_HEADER_SOLACEON_RUINS_ROOM_6,
    MAP_HEADER_SOLACEON_RUINS_ROOM_5,
    MAP_HEADER_SOLACEON_RUINS_ROOM_7,
    MAP_HEADER_SOLACEON_RUINS_ROOM_4_SOUTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_6_NORTHWEST_DEAD_END,
    MAP_HEADER_UNKNOWN_243,
    MAP_HEADER_VICTORY_ROAD_1F,
    MAP_HEADER_VICTORY_ROAD_2F,
    MAP_HEADER_VICTORY_ROAD_B1F,
    MAP_HEADER_VICTORY_ROAD_1F_ROOM_2,
    MAP_HEADER_VICTORY_ROAD_1F_ROOM_1,
    MAP_HEADER_VICTORY_ROAD_1F_ROOM_3,
    MAP_HEADER_UNKNOWN_250,
    MAP_HEADER_PAL_PARK,
    MAP_HEADER_UNKNOWN_252,
    MAP_HEADER_AMITY_SQUARE,
    MAP_HEADER_RAVAGED_PATH,
    MAP_HEADER_UNKNOWN_255,
    MAP_HEADER_FLOAROMA_MEADOW,
    MAP_HEADER_FLOAROMA_MEADOW_HOUSE,
    MAP_HEADER_OREBURGH_GATE_1F,
    MAP_HEADER_OREBURGH_GATE_B1F,
    MAP_HEADER_FULLMOON_ISLAND,
    MAP_HEADER_FULLMOON_ISLAND_FOREST,
    MAP_HEADER_STARK_MOUNTAIN_OUTSIDE,
    MAP_HEADER_STARK_MOUNTAIN_ROOM_1,
    MAP_HEADER_STARK_MOUNTAIN_ROOM_2,
    MAP_HEADER_STARK_MOUNTAIN_ROOM_3,
    MAP_HEADER_UNKNOWN_266,
    MAP_HEADER_SENDOFF_SPRING,
    MAP_HEADER_TURNBACK_CAVE_ENTRANCE,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_ROOM,
    MAP_HEADER_TURNBACK_CAVE_GIRATINA_ROOM,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_1,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_2,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_3,
    MAP_HEADER_FLOWER_PARADISE,
    MAP_HEADER_UNKNOWN_275,
    MAP_HEADER_UNKNOWN_276,
    MAP_HEADER_UNKNOWN_277,
    MAP_HEADER_SNOWPOINT_TEMPLE_1F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B1F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B2F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B3F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B4F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B5F,
    MAP_HEADER_WAYWARD_CAVE_1F,
    MAP_HEADER_WAYWARD_CAVE_B1F,
    MAP_HEADER_RUIN_MANIAC_CAVE_SHORT,
    MAP_HEADER_TROPHY_GARDEN,
    MAP_HEADER_IRON_ISLAND,
    MAP_HEADER_IRON_ISLAND_1F,
    MAP_HEADER_IRON_ISLAND_B1F_LEFT_ROOM,
    MAP_HEADER_IRON_ISLAND_B1F_RIGHT_ROOM,
    MAP_HEADER_IRON_ISLAND_B2F_RIGHT_ROOM,
    MAP_HEADER_IRON_ISLAND_B2F_LEFT_ROOM,
    MAP_HEADER_IRON_ISLAND_B3F,
    MAP_HEADER_OLD_CHATEAU,
    MAP_HEADER_OLD_CHATEAU_DINING_AREA,
    MAP_HEADER_OLD_CHATEAU_SIDE_ROOMS,
    MAP_HEADER_OLD_CHATEAU_CORRIDOR,
    MAP_HEADER_OLD_CHATEAU_BACK_WEST_ROOM,
    MAP_HEADER_OLD_CHATEAU_BACK_MIDDLE_WEST_ROOM,
    MAP_HEADER_OLD_CHATEAU_BACK_MIDDLE_ROOM,
    MAP_HEADER_OLD_CHATEAU_BACK_MIDDLE_EAST_ROOM,
    MAP_HEADER_OLD_CHATEAU_BACK_EAST_ROOM,
    MAP_HEADER_UNKNOWN_304,
    MAP_HEADER_GALACTIC_HQ_1F,
    MAP_HEADER_GALACTIC_HQ_2F,
    MAP_HEADER_GALACTIC_HQ_3F,
    MAP_HEADER_GALACTIC_HQ_4F,
    MAP_HEADER_GALACTIC_HQ_B1F,
    MAP_HEADER_GALACTIC_HQ_B2F,
    MAP_HEADER_LAKE_VERITY_LOW_WATER,
    MAP_HEADER_LAKE_VERITY,
    MAP_HEADER_VERITY_CAVERN,
    MAP_HEADER_LAKE_VALOR_DRAINED,
    MAP_HEADER_LAKE_VALOR,
    MAP_HEADER_VALOR_CAVERN,
    MAP_HEADER_LAKE_ACUITY_LOW_WATER,
    MAP_HEADER_LAKE_ACUITY,
    MAP_HEADER_ACUITY_CAVERN,
    MAP_HEADER_NEWMOON_ISLAND,
    MAP_HEADER_NEWMOON_ISLAND_FOREST,
    MAP_HEADER_UNUSED_BATTLE_PARK,
    MAP_HEADER_UNUSED_BATTLE_PARK_EXCHANGE_SERVICE_CORNER,
    MAP_HEADER_UNKNOWN_324,
    MAP_HEADER_UNKNOWN_325,
    MAP_HEADER_BATTLE_TOWER,
    MAP_HEADER_BATTLE_TOWER_ELEVATOR,
    MAP_HEADER_BATTLE_TOWER_CORRIDOR,
    MAP_HEADER_BATTLE_TOWER_CORRIDOR_MULTI,
    MAP_HEADER_BATTLE_TOWER_BATTLE_ROOM,
    MAP_HEADER_BATTLE_TOWER_MULTI_BATTLE_ROOM,
    MAP_HEADER_COMMUNICATION_CLUB_COLOSSEUM_2P,
    MAP_HEADER_COMMUNICATION_CLUB_COLOSSEUM_4P,
    MAP_HEADER_VERITY_LAKEFRONT,
    MAP_HEADER_UNUSED_VERITY_LAKEFRONT_HOUSE,
    MAP_HEADER_VALOR_LAKEFRONT,
    MAP_HEADER_RESTAURANT,
    MAP_HEADER_GRAND_LAKE_VALOR_LAKEFRONT_EAST_HOUSE,
    MAP_HEADER_GRAND_LAKE_VALOR_LAKEFRONT_WEST_HOUSE,
    MAP_HEADER_ACUITY_LAKEFRONT,
    MAP_HEADER_SPRING_PATH,
    MAP_HEADER_ROUTE_201,
    MAP_HEADER_ROUTE_202,
    MAP_HEADER_ROUTE_203,
    MAP_HEADER_ROUTE_204_SOUTH,
    MAP_HEADER_ROUTE_204_NORTH,
    MAP_HEADER_ROUTE_205_SOUTH,
    MAP_HEADER_ROUTE_205_HOUSE,
    MAP_HEADER_ROUTE_205_NORTH,
    MAP_HEADER_ROUTE_206,
    MAP_HEADER_ROUTE_206_CYCLING_ROAD_SOUTH_GATE,
    MAP_HEADER_UNUSED_GATE_BETWEEN_ETERNA_CITY_ROUTE_206,
    MAP_HEADER_ROUTE_207,
    MAP_HEADER_ROUTE_208,
    MAP_HEADER_ROUTE_208_HOUSE,
    MAP_HEADER_ROUTE_209,
    MAP_HEADER_ROUTE_209_LOST_TOWER_1F,
    MAP_HEADER_ROUTE_209_LOST_TOWER_2F,
    MAP_HEADER_ROUTE_209_LOST_TOWER_3F,
    MAP_HEADER_ROUTE_209_LOST_TOWER_4F,
    MAP_HEADER_ROUTE_209_LOST_TOWER_5F,
    MAP_HEADER_ROUTE_210_SOUTH,
    MAP_HEADER_ROUTE_210_NORTH,
    MAP_HEADER_ROUTE_210_GRANDMA_WILMA_HOUSE,
    MAP_HEADER_ROUTE_211_WEST,
    MAP_HEADER_ROUTE_211_EAST,
    MAP_HEADER_ROUTE_212_NORTH,
    MAP_HEADER_POKEMON_MANSION,
    MAP_HEADER_POKEMON_MANSION_MAIDS_ROOM,
    MAP_HEADER_POKEMON_MANSION_OFFICE,
    MAP_HEADER_ROUTE_212_SOUTH,
    MAP_HEADER_ROUTE_212_HOUSE,
    MAP_HEADER_ROUTE_213,
    MAP_HEADER_ROUTE_213_GATE_TO_PASTORIA_CITY,
    MAP_HEADER_FOOTSTEP_HOUSE,
    MAP_HEADER_GRAND_LAKE_ROUTE_213_LOBBY,
    MAP_HEADER_GRAND_LAKE_ROUTE_213_EAST_HOUSE,
    MAP_HEADER_GRAND_LAKE_ROUTE_213_NORTHWEST_HOUSE,
    MAP_HEADER_GRAND_LAKE_ROUTE_213_NORTHEAST_HOUSE,
    MAP_HEADER_ROUTE_214,
    MAP_HEADER_ROUTE_214_GATE_TO_VEILSTONE_CITY,
    MAP_HEADER_ROUTE_215,
    MAP_HEADER_ROUTE_216,
    MAP_HEADER_ROUTE_216_HOUSE,
    MAP_HEADER_ROUTE_217,
    MAP_HEADER_ROUTE_217_WEST_HOUSE,
    MAP_HEADER_ROUTE_217_NORTHEAST_HOUSE,
    MAP_HEADER_ROUTE_218,
    MAP_HEADER_ROUTE_218_GATE_TO_JUBILIFE_CITY,
    MAP_HEADER_ROUTE_218_GATE_TO_CANALAVE_CITY,
    MAP_HEADER_ROUTE_219,
    MAP_HEADER_ROUTE_221,
    MAP_HEADER_PAL_PARK_LOBBY,
    MAP_HEADER_ROUTE_221_HOUSE,
    MAP_HEADER_ROUTE_222,
    MAP_HEADER_ROUTE_222_WEST_HOUSE,
    MAP_HEADER_ROUTE_222_EAST_HOUSE,
    MAP_HEADER_ROUTE_222_GATE_TO_SUNYSHORE_CITY,
    MAP_HEADER_ROUTE_224,
    MAP_HEADER_ROUTE_225,
    MAP_HEADER_UNKNOWN_401,
    MAP_HEADER_UNKNOWN_402,
    MAP_HEADER_ROUTE_227,
    MAP_HEADER_UNKNOWN_404,
    MAP_HEADER_UNKNOWN_405,
    MAP_HEADER_ROUTE_228,
    MAP_HEADER_ROUTE_229,
    MAP_HEADER_UNKNOWN_408,
    MAP_HEADER_UNKNOWN_409,
    MAP_HEADER_RECORD_MIXING_ROOM,
    MAP_HEADER_TWINLEAF_TOWN,
    MAP_HEADER_TWINLEAF_TOWN_RIVAL_HOUSE_1F,
    MAP_HEADER_TWINLEAF_TOWN_RIVAL_HOUSE_2F,
    MAP_HEADER_TWINLEAF_TOWN_PLAYER_HOUSE_1F,
    MAP_HEADER_TWINLEAF_TOWN_PLAYER_HOUSE_2F,
    MAP_HEADER_TWINLEAF_TOWN_NORTHEAST_HOUSE,
    MAP_HEADER_TWINLEAF_TOWN_SOUTHWEST_HOUSE,
    MAP_HEADER_SANDGEM_TOWN,
    MAP_HEADER_SANDGEM_TOWN_MART,
    MAP_HEADER_SANDGEM_TOWN_POKECENTER_1F,
    MAP_HEADER_SANDGEM_TOWN_POKECENTER_2F,
    MAP_HEADER_SANDGEM_TOWN_POKEMON_RESEARCH_LAB,
    MAP_HEADER_SANDGEM_TOWN_COUNTERPART_HOUSE_1F,
    MAP_HEADER_SANDGEM_TOWN_COUNTERPART_HOUSE_2F,
    MAP_HEADER_SANDGEM_TOWN_HOUSE,
    MAP_HEADER_FLOAROMA_TOWN,
    MAP_HEADER_FLOAROMA_TOWN_MART,
    MAP_HEADER_FLOAROMA_TOWN_POKECENTER_1F,
    MAP_HEADER_FLOAROMA_TOWN_POKECENTER_2F,
    MAP_HEADER_FLOWER_SHOP,
    MAP_HEADER_FLOAROMA_TOWN_SOUTHEAST_HOUSE,
    MAP_HEADER_FLOAROMA_TOWN_MIDDLE_HOUSE,
    MAP_HEADER_SOLACEON_TOWN,
    MAP_HEADER_SOLACEON_TOWN_MART,
    MAP_HEADER_SOLACEON_TOWN_POKECENTER_1F,
    MAP_HEADER_SOLACEON_TOWN_POKECENTER_2F,
    MAP_HEADER_POKEMON_DAY_CARE,
    MAP_HEADER_SOLACEON_TOWN_NORTHEAST_HOUSE,
    MAP_HEADER_SOLACEON_TOWN_POKEMON_NEWS_PRESS,
    MAP_HEADER_SOLACEON_TOWN_NORTH_HOUSE,
    MAP_HEADER_SOLACEON_TOWN_EAST_HOUSE,
    MAP_HEADER_CELESTIC_TOWN,
    MAP_HEADER_CELESTIC_TOWN_POKECENTER_1F,
    MAP_HEADER_CELESTIC_TOWN_POKECENTER_2F,
    MAP_HEADER_CELESTIC_TOWN_NORTH_HOUSE,
    MAP_HEADER_CELESTIC_TOWN_NORTHWEST_HOUSE,
    MAP_HEADER_CELESTIC_TOWN_NORTHEAST_HOUSE,
    MAP_HEADER_CELESTIC_TOWN_SOUTHWEST_HOUSE,
    MAP_HEADER_CELESTIC_TOWN_CAVE,
    MAP_HEADER_SURVIVAL_AREA,
    MAP_HEADER_SURVIVAL_AREA_MART,
    MAP_HEADER_SURVIVAL_AREA_POKECENTER_1F,
    MAP_HEADER_SURVIVAL_AREA_POKECENTER_2F,
    MAP_HEADER_BATTLEGROUND,
    MAP_HEADER_SURVIVAL_AREA_SOUTH_HOUSE,
    MAP_HEADER_SURVIVAL_AREA_NORTH_HOUSE,
    MAP_HEADER_RESORT_AREA,
    MAP_HEADER_UNUSED_RESORT_AREA_MART,
    MAP_HEADER_RESORT_AREA_POKECENTER_1F,
    MAP_HEADER_RESORT_AREA_POKECENTER_2F,
    MAP_HEADER_RESORT_AREA_RIBBON_SYNDICATE_1F,
    MAP_HEADER_RESORT_AREA_RIBBON_SYNDICATE_2F,
    MAP_HEADER_RESORT_AREA_RIBBON_SYNDICATE_ELEVATOR,
    MAP_HEADER_VILLA,
    MAP_HEADER_RESORT_AREA_HOUSE,
    MAP_HEADER_UNION_ROOM,
    MAP_HEADER_ROUTE_220,
    MAP_HEADER_ROUTE_223,
    MAP_HEADER_ROUTE_226,
    MAP_HEADER_UNKNOWN_470,
    MAP_HEADER_ROUTE_230,
    MAP_HEADER_SEABREAK_PATH,
    MAP_HEADER_UNKNOWN_473,
    MAP_HEADER_JUBILIFE_CITY_POKECENTER_B1F,
    MAP_HEADER_CANALAVE_CITY_POKECENTER_B1F,
    MAP_HEADER_OREBURGH_CITY_POKECENTER_B1F,
    MAP_HEADER_ETERNA_CITY_POKECENTER_B1F,
    MAP_HEADER_HEARTHOME_CITY_POKECENTER_B1F,
    MAP_HEADER_PASTORIA_CITY_POKECENTER_B1F,
    MAP_HEADER_VEILSTONE_CITY_POKECENTER_B1F,
    MAP_HEADER_SUNYSHORE_CITY_POKECENTER_B1F,
    MAP_HEADER_SNOWPOINT_CITY_POKECENTER_B1F,
    MAP_HEADER_POKEMON_LEAGUE_SOUTH_POKECENTER_B1F,
    MAP_HEADER_FIGHT_AREA_POKECENTER_B1F,
    MAP_HEADER_SANDGEM_TOWN_POKECENTER_B1F,
    MAP_HEADER_FLOAROMA_TOWN_POKECENTER_B1F,
    MAP_HEADER_SOLACEON_TOWN_POKECENTER_B1F,
    MAP_HEADER_CELESTIC_TOWN_POKECENTER_B1F,
    MAP_HEADER_SURVIVAL_AREA_POKECENTER_B1F,
    MAP_HEADER_RESORT_AREA_POKECENTER_B1F,
    MAP_HEADER_CANALAVE_CITY_WEST_HOUSE,
    MAP_HEADER_CAFE,
    MAP_HEADER_BATTLE_TOWER_BATTLE_SALON,
    MAP_HEADER_GALACTIC_HQ_CONTROL_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_NORTH_POKECENTER_2F,
    MAP_HEADER_POKEMON_LEAGUE_NORTH_POKECENTER_B1F,
    MAP_HEADER_GALACTIC_HQ_LABORATORY,
    MAP_HEADER_ROUTE_225_HOUSE,
    MAP_HEADER_ROUTE_226_HOUSE,
    MAP_HEADER_ROUTE_227_HOUSE,
    MAP_HEADER_ROUTE_228_GATE_TO_ROUTE_226,
    MAP_HEADER_ROUTE_228_NORTH_HOUSE,
    MAP_HEADER_ROUTE_228_SOUTH_HOUSE,
    MAP_HEADER_GREAT_MARSH_1,
    MAP_HEADER_GREAT_MARSH_2,
    MAP_HEADER_GREAT_MARSH_3,
    MAP_HEADER_GREAT_MARSH_4,
    MAP_HEADER_GREAT_MARSH_5,
    MAP_HEADER_GREAT_MARSH_6,
    MAP_HEADER_HALL_OF_ORIGIN,
    MAP_HEADER_UNKNOWN_511,
    MAP_HEADER_RUIN_MANIAC_CAVE_LONG,
    MAP_HEADER_MANIAC_TUNNEL,
    MAP_HEADER_IRON_ISLAND_HOUSE,
    MAP_HEADER_SOLACEON_RUINS_ROOM_5_SOUTHEAST_DEADEND,
    MAP_HEADER_VISTA_LIGHTHOUSE_ELEVATOR,
    MAP_HEADER_JUBILIFE_CITY_SOUTHWEST_HOUSE_2F,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_4,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_5,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_6,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_1,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_2,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_3,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_4,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_5,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_6,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_1,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_2,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_3,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_4,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_5,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_6,
    MAP_HEADER_UNKNOWN_533,
    MAP_HEADER_UNKNOWN_534,
    MAP_HEADER_UNKNOWN_535,
    MAP_HEADER_UNKNOWN_536,
    MAP_HEADER_UNKNOWN_537,
    MAP_HEADER_UNKNOWN_538,
    MAP_HEADER_UNKNOWN_539,
    MAP_HEADER_UNKNOWN_540,
    MAP_HEADER_UNKNOWN_541,
    MAP_HEADER_UNKNOWN_542,
    MAP_HEADER_UNKNOWN_543,
    MAP_HEADER_UNKNOWN_544,
    MAP_HEADER_UNKNOWN_545,
    MAP_HEADER_UNKNOWN_546,
    MAP_HEADER_UNKNOWN_547,
    MAP_HEADER_UNKNOWN_548,
    MAP_HEADER_UNKNOWN_549,
    MAP_HEADER_UNKNOWN_550,
    MAP_HEADER_UNKNOWN_551,
    MAP_HEADER_UNKNOWN_552,
    MAP_HEADER_UNKNOWN_553,
    MAP_HEADER_UNKNOWN_554,
    MAP_HEADER_UNKNOWN_555,
    MAP_HEADER_UNKNOWN_556,
    MAP_HEADER_UNKNOWN_557,
    MAP_HEADER_CONTEST_HALL_STAGE_NO_CONTEST,
    MAP_HEADER_BATTLE_FRONTIER,
    MAP_HEADER_BATTLE_FRONTIER_GATE_TO_FIGHT_AREA,
    MAP_HEADER_UNKNOWN_561,
    MAP_HEADER_BATTLE_FACTORY,
    MAP_HEADER_BATTLE_HALL,
    MAP_HEADER_BATTLE_CASTLE,
    MAP_HEADER_BATTLE_ARCADE,
    MAP_HEADER_VEILSTONE_STORE_B1F,
    MAP_HEADER_GLOBAL_TERMINAL_2F,
    MAP_HEADER_GLOBAL_TERMINAL_3F,
    MAP_HEADER_GALACTIC_HQ_HALL,
    MAP_HEADER_UNKNOWN_570,
    MAP_HEADER_ROTOMS_ROOM,
    MAP_HEADER_UNKNOWN_572,
    MAP_HEADER_DISTORTION_WORLD_1F,
    MAP_HEADER_DISTORTION_WORLD_B1F,
    MAP_HEADER_DISTORTION_WORLD_B2F,
    MAP_HEADER_DISTORTION_WORLD_B3F,
    MAP_HEADER_DISTORTION_WORLD_B4F,
    MAP_HEADER_UNKNOWN_578,
    MAP_HEADER_DISTORTION_WORLD_B5F,
    MAP_HEADER_DISTORTION_WORLD_B6F,
    MAP_HEADER_DISTORTION_WORLD_B7F,
    MAP_HEADER_DISTORTION_WORLD_GIRATINA_ROOM,
    MAP_HEADER_DISTORTION_WORLD_TURNBACK_CAVE_ROOM,
    MAP_HEADER_SPEAR_PILLAR_DIALGA,
    MAP_HEADER_SPEAR_PILLAR_PALKIA,
    MAP_HEADER_WIFI_PLAZA_ENTRANCE,
    MAP_HEADER_IRON_ISLAND_IRON_RUINS,
    MAP_HEADER_IRON_RUINS,
    MAP_HEADER_MT_CORONET_ICEBERG_RUINS,
    MAP_HEADER_ICEBERG_RUINS,
    MAP_HEADER_ROUTE_228_ROCK_PEAK_RUINS,
    MAP_HEADER_ROCK_PEAK_RUINS,

    MAP_HEADER_INVALID,
    MAP_HEADER_DYNAMIC = 4095,
};

static inline void Location_Set(Location *location, enum MapHeaderID mapHeaderID, int warpId, int x, int z, int faceDirection) {
    location->mapId = mapHeaderID;
    location->warpId = warpId;
    location->x = x;
    location->y = z;
    location->direction = faceDirection;
}

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);

static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

void GTSApplication_InitAffineTemplate(AffineSpriteListTemplate *template, GTSApplicationState *appState, SpriteResourcesHeader *spriteResourceHeader, enum NNS_G2D_VRAM_TYPE vramType);

void GTSApplication_SetNextScreenWithArgument(GTSApplicationState *appState, int nextScreen, enum ScreenArgument screenArgument);

int ov70_02238E64(GTSApplicationState *appState);

int ov70_02238E64(GTSApplicationState *appState) {
    return Options_GetTextFrameDelay(appState->playerData->options);
}
