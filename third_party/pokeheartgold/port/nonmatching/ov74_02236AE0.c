#include "global.h"
#include "wild_encounter.h"

struct ApplicationManager;
struct ApplicationManagerTemplate;
struct Background;
struct BallCapsule;
struct BallSeal;
struct BattleRegulation;
struct BgConfig;
struct BoxPokemon;
struct ColoredArrow;
struct DWCstInetControl;
struct EasyChatSentence;
struct G2dRenderer;
struct ListMenu;
struct ListMenuTemplate;
struct Mail;
struct MessageBank;
struct MessageBankEntry;
struct MessageLoader;
struct MysteryGiftAppData;
struct MysteryGiftCosmeticsData;
struct MysteryGiftEventData;
struct MysteryGiftEventHeader;
struct MysteryGiftItemData;
struct MysteryGiftPokemonData;
struct NARC;
struct Options;
struct PGT;
struct PartyPokemon;
struct Pokemon;
struct PokemonDataBlockA;
struct PokemonDataBlockB;
struct PokemonDataBlockC;
struct PokemonDataBlockD;
struct Resource;
struct ResourceCollection;
struct SaveBlockInfo;
struct SaveData;
struct SaveDataBody;
struct SaveDataState;
struct SavePageInfo;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct SpriteResourcesHeader;
struct String;
struct StringList;
struct StringTemplate;
struct StringTemplateArg;
struct StringTemplateArgHeader;
struct SysTask;
struct SysTaskManager;
struct System;
struct Window;
struct WonderCard;
struct tagDWCWDSData;
enum HeapID {
    HEAP_ID_SYSTEM = 0,
    HEAP_ID_SAVE,
    HEAP_ID_DEBUG,
    HEAP_ID_APPLICATION,
    HEAP_ID_FIELD1,
    HEAP_ID_BATTLE,
    HEAP_ID_BAG,
    HEAP_ID_POKETCH_MAIN,
    HEAP_ID_POKETCH_APP,
    HEAP_ID_BOX_DATA,
    HEAP_ID_BOX_GRAPHICS,
    HEAP_ID_FIELD2,
    HEAP_ID_PARTY_MENU,
    HEAP_ID_13,
    HEAP_ID_14,
    HEAP_ID_COMMUNICATION,
    HEAP_ID_16,
    HEAP_ID_TOWN_MAP,
    HEAP_ID_NAMING_SCREEN_APP,
    HEAP_ID_POKEMON_SUMMARY_SCREEN,
    HEAP_ID_20,
    HEAP_ID_21,
    HEAP_ID_22,
    HEAP_ID_23,
    HEAP_ID_24,
    HEAP_ID_TRAINER_CASE,
    HEAP_ID_TRADE_ROOM,
    HEAP_ID_27,
    HEAP_ID_28,
    HEAP_ID_MINING,
    HEAP_ID_DISTORTION_WORLD_WARP,
    HEAP_ID_31,
    HEAP_ID_FIELD3,
    HEAP_ID_UNDERGROUND,
    HEAP_ID_34,
    HEAP_ID_35,
    HEAP_ID_BERRY_TAG,
    HEAP_ID_POKEDEX,
    HEAP_ID_OPTIONS_MENU,
    HEAP_ID_DRAWING,
    HEAP_ID_MAIL,
    HEAP_ID_MAIL_VIEWER,
    HEAP_ID_JOURNAL,
    HEAP_ID_43,
    HEAP_ID_POFFIN_CASE,
    HEAP_ID_POFFIN_MANAGER,
    HEAP_ID_POFFIN_CUTSCENE,
    HEAP_ID_CHOOSE_STARTER_APP,
    HEAP_ID_48,
    HEAP_ID_NINTENDO_WFC,
    HEAP_ID_50,
    HEAP_ID_51,
    HEAP_ID_52,
    HEAP_ID_53,
    HEAP_ID_54,
    HEAP_ID_POFFIN_BERRY_SELECTION,
    HEAP_ID_56,
    HEAP_ID_TRADE_SEQUENCE,
    HEAP_ID_TRADE_SEQUENCE_PHASE,
    HEAP_ID_59,
    HEAP_ID_60,
    HEAP_ID_PC_HALL_OF_FAME,
    HEAP_ID_62,
    HEAP_ID_HALL_OF_FAME,
    HEAP_ID_64,
    HEAP_ID_65,
    HEAP_ID_66,
    HEAP_ID_MOVE_REMINDER,
    HEAP_ID_68,
    HEAP_ID_69,
    HEAP_ID_70,
    HEAP_ID_EGG_HATCH,
    HEAP_ID_BOAT_CUTSCENE,
    HEAP_ID_EVOLUTION,
    HEAP_ID_BATTLE_TOWER_RECORDS_APP,
    HEAP_ID_END_CREDITS,
    HEAP_ID_76,
    HEAP_ID_GAME_START,
    HEAP_ID_MIGRATE_FROM_GBA,
    HEAP_ID_79,
    HEAP_ID_RANKINGS_MACHINE,
    HEAP_ID_MAIN_MENU,
    HEAP_ID_ROWAN_INTRO,
    HEAP_ID_ROWAN_INTRO_TV,
    HEAP_ID_DIPLOMA,
    HEAP_ID_RANGER_LINK,
    HEAP_ID_MYSTERY_GIFT_APP,
    HEAP_ID_WONDER_CARDS_APP,
    HEAP_ID_88,
    HEAP_ID_89,
    HEAP_ID_90,
    HEAP_ID_NETWORK_ICON,
    HEAP_ID_92,
    HEAP_ID_BATTLE_FACTORY_APP,
    HEAP_ID_94,
    HEAP_ID_95,
    HEAP_ID_96,
    HEAP_ID_97,
    HEAP_ID_BATTLE_HALL_APP,
    HEAP_ID_99,
    HEAP_ID_BATTLE_CASTLE_APP,
    HEAP_ID_101,
    HEAP_ID_102,
    HEAP_ID_BATTLE_ARCADE_APP,
    HEAP_ID_104,
    HEAP_ID_105,
    HEAP_ID_106,
    HEAP_ID_107,
    HEAP_ID_108,
    HEAP_ID_109,
    HEAP_ID_110,
    HEAP_ID_SPEAR_PILLAR_CUTSCENE,
    HEAP_ID_112,
    HEAP_ID_113,
    HEAP_ID_FRONTIER_RECORD_APP,
    HEAP_ID_SCRATCH_OFF_CARD_APP,
    HEAP_ID_116,
    HEAP_ID_117,
    HEAP_ID_118,
    HEAP_ID_119,
    HEAP_ID_LIBRARY_TV,
    HEAP_ID_PARTY_MENU1,
    HEAP_ID_PARTY_MENU2,
    HEAP_ID_MAX,
};
typedef struct ApplicationManager ApplicationManager;
typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);
typedef struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} ApplicationManagerTemplate;
struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};
void *OverlayManager_GetData(ApplicationManager *appMan, int *);
typedef enum {
    DWC_WDS_STATE_IDLE         = 0,
    DWC_WDS_STATE_PROCESS      = 1,
    DWC_WDS_STATE_COMPLETED    = 2,
    DWC_WDS_STATE_FAILED       = 3,
    DWC_WDS_STATE_ERROR        = 4
} DWCWDSState;
typedef struct tagDWCWDSData {
    u8 ssid[32 ];
    u8 wep[32 ];
    int wepMode;
    char apnum[10];
} DWCWDSData;
typedef struct DWCstInetControl {
    volatile int ac_state;
    u16 state;
    u16 online;
    u16 dmaNo;
    u16 powerMode;
    u8 wontExecuteWDS;
    DWCWDSState processWDSstate;
    DWCWDSData nspotInfo;



} DWCInetControl;
typedef enum {
    DWC_CONNECTINET_AUTH_TEST,
    DWC_CONNECTINET_AUTH_DEVELOP,
    DWC_CONNECTINET_AUTH_RELEASE,
    DWC_CONNECTINET_AUTH_LAST
} DWCInetAuthType;
extern void DWC_InitInetEx(DWCInetControl * inetCntl, u16 dmaNo, u16 powerMode, u16 ssl_priority);
extern void DWC_SetAuthServer(DWCInetAuthType type);
extern void DWC_ConnectInetAsync(void);
extern BOOL DWC_CheckInet(void);
extern void DWC_ProcessInet(void);
extern void DWC_CleanupInet(void);
extern BOOL DWC_CleanupInetAsync(void);
extern BOOL DWC_UpdateConnection(void);
typedef enum {
    DWC_ETYPE_NO_ERROR = 0,
    DWC_ETYPE_LIGHT,
    DWC_ETYPE_SHOW_ERROR,
    DWC_ETYPE_SHUTDOWN_FM,
    DWC_ETYPE_SHUTDOWN_GHTTP,
    DWC_ETYPE_SHUTDOWN_ND,
    DWC_ETYPE_DISCONNECT,
    DWC_ETYPE_FATAL,
    DWC_ETYPE_NUM
} DWCErrorType;
extern void DWC_ClearError(void);
typedef enum {
    DWC_ALLOCTYPE_AUTH,
    DWC_ALLOCTYPE_AC,
    DWC_ALLOCTYPE_BM,
    DWC_ALLOCTYPE_UTIL,
    DWC_ALLOCTYPE_BASE,
    DWC_ALLOCTYPE_GS,
    DWC_ALLOCTYPE_ND,

    DWC_ALLOCTYPE_ENC,
    DWC_ALLOCTYPE_MAIL,

    DWC_ALLOCTYPE_LAST
} DWCAllocType;
typedef void *(*DWCAllocEx)(DWCAllocType name, u32 size, int align);
typedef void (*DWCFreeEx )(DWCAllocType name, void * ptr, u32 size);
void DWC_SetMemFunc(DWCAllocEx alloc, DWCFreeEx free);
typedef enum {
    DWC_NASLOGIN_STATE_DIRTY = 0,
    DWC_NASLOGIN_STATE_IDLE,
    DWC_NASLOGIN_STATE_HTTP,
    DWC_NASLOGIN_STATE_SUCCESS,
    DWC_NASLOGIN_STATE_ERROR,
    DWC_NASLOGIN_STATE_CANCELED,
    DWC_NASLOGIN_STATE_MAX
} DWCNasLoginState;
BOOL DWC_NASLoginAsync(void);
DWCNasLoginState DWC_NASLoginProcess(void);
void DWC_NASLoginAbort(void);
typedef enum {
    DWC_ND_ERROR_NONE,
    DWC_ND_ERROR_ALLOC,
    DWC_ND_ERROR_BUSY,
    DWC_ND_ERROR_HTTP,
    DWC_ND_ERROR_BUFFULL,
    DWC_ND_ERROR_DLSERVER,
    DWC_ND_ERROR_CANCELED,
    DWC_ND_ERROR_PARAM,
    DWC_ND_ERROR_FATAL,
    DWC_ND_ERROR_MAX
} DWCNdError;
typedef enum {
    DWC_ND_CBREASON_INITIALIZE,
    DWC_ND_CBREASON_GETFILELISTNUM,
    DWC_ND_CBREASON_GETFILELIST,
    DWC_ND_CBREASON_GETFILE,
    DWC_ND_CBREASON_MAX
} DWCNdCallbackReason;
typedef struct {
    char name[32 + 1];
    short explain[50 + 1];
    char param1[10 + 1];
    char param2[10 + 1];
    char param3[10 + 1];
    unsigned int size;
} DWCNdFileInfo;
typedef void (*DWCNdCallback)(DWCNdCallbackReason reason, DWCNdError errpr, int servererror);
BOOL DWC_NdInitAsync(DWCNdCallback callback, char * gamecd, char * passwd);
BOOL DWC_NdSetAttr(const char * attr1, const char * attr2, const char * attr3);
BOOL DWC_NdGetFileListNumAsync(int * entrynum);
BOOL DWC_NdGetFileListAsync(DWCNdFileInfo * filelist, unsigned int offset, unsigned int num);
BOOL DWC_NdGetFileAsync(DWCNdFileInfo * fileinfo, char * buf, unsigned int size);
BOOL DWC_NdCancelAsync(void);
BOOL DWC_NdGetProgress(u32 * received, u32 * contentlen);
typedef struct Background {
    void *tilemapBuffer;
    u32 bufferSize;
    u32 baseTile;

    int xOffset;
    int yOffset;

    u8 type;
    u8 screenSize;
    u8 colorMode;
    u8 tileSize;

    u16 rotation;

    fx32 xScale;
    fx32 yScale;
    int xCenter;
    int yCenter;
} Background;
typedef struct BgConfig {
    enum HeapID heapID;
    u16 scrollScheduled;
    u16 bufferTransferScheduled;
    Background bgs[8];
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
    u8 data[(0x1000) * 32 ];
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
typedef struct SaveData {
    BOOL backupExists;
    BOOL dataExists;
    BOOL isNewGameData;
    BOOL fullSaveRequired;
    u32 loadCheckStatus;
    SaveDataBody body;
    u32 globalCounter;
    u32 blockCounters[SAVE_BLOCK_ID_MAX];
    u8 blockOffsets[SAVE_BLOCK_ID_MAX];
    SavePageInfo pageInfo[SAVE_TABLE_ENTRY_MAX];
    SaveBlockInfo blockInfo[SAVE_BLOCK_ID_MAX];
    SaveDataState state;
    int sectorSwitch;
    u32 sectorCounter;
} SaveData;
typedef struct Options {
    u16 textSpeed : 4;
    u16 soundMode : 2;
    u16 battleStyle : 1;
    u16 battleScene : 1;
    u16 buttonMode : 2;
    u16 frame : 5;
    u16 : 1;
} Options;
typedef u16 charcode_t;
typedef struct String {

    u16 maxSize;


    u16 size;


    u32 integrity;






    charcode_t data[1];
} String;
typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;
typedef u32 TextColor;
typedef struct ColoredArrow {
    TextColor color;
    String *string;
} ColoredArrow;
typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;
typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;
typedef struct MessageLoader {
    u16 mode;
    u16 heapID;
    u16 narcID;
    u16 bankID;

    union {
        MessageBank *bank;
        NARC *narc;
    };
} MessageLoader;
typedef struct StringList {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
} StringList;
typedef struct ListMenu ListMenu;
typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);
typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);
typedef struct ListMenuTemplate {
    const StringList *choices;
    CursorCallback cursorCallback;
    PrintCallback printCallback;
    Window *window;
    u16 count;
    u16 maxDisplay;
    u8 headerXOffset;
    u8 textXOffset;
    u8 cursorXOffset;
    u8 yOffset : 4;
    u8 textColorFg : 4;
    u8 textColorBg : 4;
    u8 textColorShadow : 4;
    u16 letterSpacing : 3;
    u16 lineSpacing : 4;
    u16 pagerMode : 2;
    u16 fontID : 6;
    u16 cursorType : 1;
    void *parent;
} ListMenuTemplate;
struct ListMenu {
    ListMenuTemplate template;
    struct {
        u8 textColorFg : 4;
        u8 textColorBg : 4;
        u8 textColorShadow : 4;
        u8 letterSpacing : 6;
        u8 dummy : 6;
        u8 fontID : 7;
        u8 prefer : 1;
    } altFont;
    ColoredArrow *cursor;
    u16 listPos;
    u16 cursorPos;
    u8 dummy2C;
    u8 dummy2D;
    u8 dummy2E;
    u8 lastAction;
    u8 heapID;
};
enum Item {
    ITEM_NONE         =   0,
    ITEM_MASTER_BALL  =   1,
    ITEM_ULTRA_BALL   =   2,
    ITEM_GREAT_BALL   =   3,
    ITEM_POKE_BALL    =   4,
    ITEM_SAFARI_BALL  =   5,
    ITEM_NET_BALL     =   6,
    ITEM_DIVE_BALL    =   7,
    ITEM_NEST_BALL    =   8,
    ITEM_REPEAT_BALL  =   9,
    ITEM_TIMER_BALL   =  10,
    ITEM_LUXURY_BALL  =  11,
    ITEM_PREMIER_BALL =  12,
    ITEM_DUSK_BALL    =  13,
    ITEM_HEAL_BALL    =  14,
    ITEM_QUICK_BALL   =  15,
    ITEM_CHERISH_BALL =  16,
    ITEM_POTION       =  17,
    ITEM_ANTIDOTE     =  18,
    ITEM_BURN_HEAL    =  19,
    ITEM_ICE_HEAL     =  20,
    ITEM_AWAKENING    =  21,
    ITEM_PARLYZ_HEAL  =  22,
    ITEM_FULL_RESTORE =  23,
    ITEM_MAX_POTION   =  24,
    ITEM_HYPER_POTION =  25,
    ITEM_SUPER_POTION =  26,
    ITEM_FULL_HEAL    =  27,
    ITEM_REVIVE       =  28,
    ITEM_MAX_REVIVE   =  29,
    ITEM_FRESH_WATER  =  30,
    ITEM_SODA_POP     =  31,
    ITEM_LEMONADE     =  32,
    ITEM_MOOMOO_MILK  =  33,
    ITEM_ENERGYPOWDER =  34,
    ITEM_ENERGY_ROOT  =  35,
    ITEM_HEAL_POWDER  =  36,
    ITEM_REVIVAL_HERB =  37,
    ITEM_ETHER        =  38,
    ITEM_MAX_ETHER    =  39,
    ITEM_ELIXIR       =  40,
    ITEM_MAX_ELIXIR   =  41,
    ITEM_LAVA_COOKIE  =  42,
    ITEM_BERRY_JUICE  =  43,
    ITEM_SACRED_ASH   =  44,
    ITEM_HP_UP        =  45,
    ITEM_PROTEIN      =  46,
    ITEM_IRON         =  47,
    ITEM_CARBOS       =  48,
    ITEM_CALCIUM      =  49,
    ITEM_RARE_CANDY   =  50,
    ITEM_PP_UP        =  51,
    ITEM_ZINC         =  52,
    ITEM_PP_MAX       =  53,
    ITEM_OLD_GATEAU   =  54,
    ITEM_GUARD_SPEC   =  55,
    ITEM_DIRE_HIT     =  56,
    ITEM_X_ATTACK     =  57,
    ITEM_X_DEFENSE    =  58,
    ITEM_X_SPEED      =  59,
    ITEM_X_ACCURACY   =  60,
    ITEM_X_SPECIAL    =  61,
    ITEM_X_SP_DEF     =  62,
    ITEM_POKE_DOLL    =  63,
    ITEM_FLUFFY_TAIL  =  64,
    ITEM_BLUE_FLUTE   =  65,
    ITEM_YELLOW_FLUTE =  66,
    ITEM_RED_FLUTE    =  67,
    ITEM_BLACK_FLUTE  =  68,
    ITEM_WHITE_FLUTE  =  69,
    ITEM_SHOAL_SALT   =  70,
    ITEM_SHOAL_SHELL  =  71,
    ITEM_RED_SHARD    =  72,
    ITEM_BLUE_SHARD   =  73,
    ITEM_YELLOW_SHARD =  74,
    ITEM_GREEN_SHARD  =  75,
    ITEM_SUPER_REPEL  =  76,
    ITEM_MAX_REPEL    =  77,
    ITEM_ESCAPE_ROPE  =  78,
    ITEM_REPEL        =  79,
    ITEM_SUN_STONE    =  80,
    ITEM_MOON_STONE   =  81,
    ITEM_FIRE_STONE   =  82,
    ITEM_THUNDERSTONE =  83,
    ITEM_WATER_STONE  =  84,
    ITEM_LEAF_STONE   =  85,
    ITEM_TINYMUSHROOM =  86,
    ITEM_BIG_MUSHROOM =  87,
    ITEM_PEARL        =  88,
    ITEM_BIG_PEARL    =  89,
    ITEM_STARDUST     =  90,
    ITEM_STAR_PIECE   =  91,
    ITEM_NUGGET       =  92,
    ITEM_HEART_SCALE  =  93,
    ITEM_HONEY        =  94,
    ITEM_GROWTH_MULCH =  95,
    ITEM_DAMP_MULCH   =  96,
    ITEM_STABLE_MULCH =  97,
    ITEM_GOOEY_MULCH  =  98,
    ITEM_ROOT_FOSSIL  =  99,
    ITEM_CLAW_FOSSIL  = 100,
    ITEM_HELIX_FOSSIL = 101,
    ITEM_DOME_FOSSIL  = 102,
    ITEM_OLD_AMBER    = 103,
    ITEM_ARMOR_FOSSIL = 104,
    ITEM_SKULL_FOSSIL = 105,
    ITEM_RARE_BONE    = 106,
    ITEM_SHINY_STONE  = 107,
    ITEM_DUSK_STONE   = 108,
    ITEM_DAWN_STONE   = 109,
    ITEM_OVAL_STONE   = 110,
    ITEM_ODD_KEYSTONE = 111,
    ITEM_GRISEOUS_ORB = 112,
    ITEM_UNUSED_113   = 113,
    ITEM_UNUSED_114   = 114,
    ITEM_UNUSED_115   = 115,
    ITEM_UNUSED_116   = 116,
    ITEM_UNUSED_117   = 117,
    ITEM_UNUSED_118   = 118,
    ITEM_UNUSED_119   = 119,
    ITEM_UNUSED_120   = 120,
    ITEM_UNUSED_121   = 121,
    ITEM_UNUSED_122   = 122,
    ITEM_UNUSED_123   = 123,
    ITEM_UNUSED_124   = 124,
    ITEM_UNUSED_125   = 125,
    ITEM_UNUSED_126   = 126,
    ITEM_UNUSED_127   = 127,
    ITEM_UNUSED_128   = 128,
    ITEM_UNUSED_129   = 129,
    ITEM_UNUSED_130   = 130,
    ITEM_UNUSED_131   = 131,
    ITEM_UNUSED_132   = 132,
    ITEM_UNUSED_133   = 133,
    ITEM_UNUSED_134   = 134,
    ITEM_ADAMANT_ORB  = 135,
    ITEM_LUSTROUS_ORB = 136,
    ITEM_GRASS_MAIL   = 137,
    ITEM_FLAME_MAIL   = 138,
    ITEM_BUBBLE_MAIL  = 139,
    ITEM_BLOOM_MAIL   = 140,
    ITEM_TUNNEL_MAIL  = 141,
    ITEM_STEEL_MAIL   = 142,
    ITEM_HEART_MAIL   = 143,
    ITEM_SNOW_MAIL    = 144,
    ITEM_SPACE_MAIL   = 145,
    ITEM_AIR_MAIL     = 146,
    ITEM_MOSAIC_MAIL  = 147,
    ITEM_BRICK_MAIL   = 148,
    ITEM_CHERI_BERRY  = 149,
    ITEM_CHESTO_BERRY = 150,
    ITEM_PECHA_BERRY  = 151,
    ITEM_RAWST_BERRY  = 152,
    ITEM_ASPEAR_BERRY = 153,
    ITEM_LEPPA_BERRY  = 154,
    ITEM_ORAN_BERRY   = 155,
    ITEM_PERSIM_BERRY = 156,
    ITEM_LUM_BERRY    = 157,
    ITEM_SITRUS_BERRY = 158,
    ITEM_FIGY_BERRY   = 159,
    ITEM_WIKI_BERRY   = 160,
    ITEM_MAGO_BERRY   = 161,
    ITEM_AGUAV_BERRY  = 162,
    ITEM_IAPAPA_BERRY = 163,
    ITEM_RAZZ_BERRY   = 164,
    ITEM_BLUK_BERRY   = 165,
    ITEM_NANAB_BERRY  = 166,
    ITEM_WEPEAR_BERRY = 167,
    ITEM_PINAP_BERRY  = 168,
    ITEM_POMEG_BERRY  = 169,
    ITEM_KELPSY_BERRY = 170,
    ITEM_QUALOT_BERRY = 171,
    ITEM_HONDEW_BERRY = 172,
    ITEM_GREPA_BERRY  = 173,
    ITEM_TAMATO_BERRY = 174,
    ITEM_CORNN_BERRY  = 175,
    ITEM_MAGOST_BERRY = 176,
    ITEM_RABUTA_BERRY = 177,
    ITEM_NOMEL_BERRY  = 178,
    ITEM_SPELON_BERRY = 179,
    ITEM_PAMTRE_BERRY = 180,
    ITEM_WATMEL_BERRY = 181,
    ITEM_DURIN_BERRY  = 182,
    ITEM_BELUE_BERRY  = 183,
    ITEM_OCCA_BERRY   = 184,
    ITEM_PASSHO_BERRY = 185,
    ITEM_WACAN_BERRY  = 186,
    ITEM_RINDO_BERRY  = 187,
    ITEM_YACHE_BERRY  = 188,
    ITEM_CHOPLE_BERRY = 189,
    ITEM_KEBIA_BERRY  = 190,
    ITEM_SHUCA_BERRY  = 191,
    ITEM_COBA_BERRY   = 192,
    ITEM_PAYAPA_BERRY = 193,
    ITEM_TANGA_BERRY  = 194,
    ITEM_CHARTI_BERRY = 195,
    ITEM_KASIB_BERRY  = 196,
    ITEM_HABAN_BERRY  = 197,
    ITEM_COLBUR_BERRY = 198,
    ITEM_BABIRI_BERRY = 199,
    ITEM_CHILAN_BERRY = 200,
    ITEM_LIECHI_BERRY = 201,
    ITEM_GANLON_BERRY = 202,
    ITEM_SALAC_BERRY  = 203,
    ITEM_PETAYA_BERRY = 204,
    ITEM_APICOT_BERRY = 205,
    ITEM_LANSAT_BERRY = 206,
    ITEM_STARF_BERRY  = 207,
    ITEM_ENIGMA_BERRY = 208,
    ITEM_MICLE_BERRY  = 209,
    ITEM_CUSTAP_BERRY = 210,
    ITEM_JABOCA_BERRY = 211,
    ITEM_ROWAP_BERRY  = 212,
    ITEM_BRIGHTPOWDER = 213,
    ITEM_WHITE_HERB   = 214,
    ITEM_MACHO_BRACE  = 215,
    ITEM_EXP_SHARE    = 216,
    ITEM_QUICK_CLAW   = 217,
    ITEM_SOOTHE_BELL  = 218,
    ITEM_MENTAL_HERB  = 219,
    ITEM_CHOICE_BAND  = 220,
    ITEM_KINGS_ROCK   = 221,
    ITEM_SILVERPOWDER = 222,
    ITEM_AMULET_COIN  = 223,
    ITEM_CLEANSE_TAG  = 224,
    ITEM_SOUL_DEW     = 225,
    ITEM_DEEPSEATOOTH = 226,
    ITEM_DEEPSEASCALE = 227,
    ITEM_SMOKE_BALL   = 228,
    ITEM_EVERSTONE    = 229,
    ITEM_FOCUS_BAND   = 230,
    ITEM_LUCKY_EGG    = 231,
    ITEM_SCOPE_LENS   = 232,
    ITEM_METAL_COAT   = 233,
    ITEM_LEFTOVERS    = 234,
    ITEM_DRAGON_SCALE = 235,
    ITEM_LIGHT_BALL   = 236,
    ITEM_SOFT_SAND    = 237,
    ITEM_HARD_STONE   = 238,
    ITEM_MIRACLE_SEED = 239,
    ITEM_BLACKGLASSES = 240,
    ITEM_BLACK_BELT   = 241,
    ITEM_MAGNET       = 242,
    ITEM_MYSTIC_WATER = 243,
    ITEM_SHARP_BEAK   = 244,
    ITEM_POISON_BARB  = 245,
    ITEM_NEVERMELTICE = 246,
    ITEM_SPELL_TAG    = 247,
    ITEM_TWISTEDSPOON = 248,
    ITEM_CHARCOAL     = 249,
    ITEM_DRAGON_FANG  = 250,
    ITEM_SILK_SCARF   = 251,
    ITEM_UPGRADE      = 252,
    ITEM_SHELL_BELL   = 253,
    ITEM_SEA_INCENSE  = 254,
    ITEM_LAX_INCENSE  = 255,
    ITEM_LUCKY_PUNCH  = 256,
    ITEM_METAL_POWDER = 257,
    ITEM_THICK_CLUB   = 258,
    ITEM_STICK        = 259,
    ITEM_RED_SCARF    = 260,
    ITEM_BLUE_SCARF   = 261,
    ITEM_PINK_SCARF   = 262,
    ITEM_GREEN_SCARF  = 263,
    ITEM_YELLOW_SCARF = 264,
    ITEM_WIDE_LENS    = 265,
    ITEM_MUSCLE_BAND  = 266,
    ITEM_WISE_GLASSES = 267,
    ITEM_EXPERT_BELT  = 268,
    ITEM_LIGHT_CLAY   = 269,
    ITEM_LIFE_ORB     = 270,
    ITEM_POWER_HERB   = 271,
    ITEM_TOXIC_ORB    = 272,
    ITEM_FLAME_ORB    = 273,
    ITEM_QUICK_POWDER = 274,
    ITEM_FOCUS_SASH   = 275,
    ITEM_ZOOM_LENS    = 276,
    ITEM_METRONOME    = 277,
    ITEM_IRON_BALL    = 278,
    ITEM_LAGGING_TAIL = 279,
    ITEM_DESTINY_KNOT = 280,
    ITEM_BLACK_SLUDGE = 281,
    ITEM_ICY_ROCK     = 282,
    ITEM_SMOOTH_ROCK  = 283,
    ITEM_HEAT_ROCK    = 284,
    ITEM_DAMP_ROCK    = 285,
    ITEM_GRIP_CLAW    = 286,
    ITEM_CHOICE_SCARF = 287,
    ITEM_STICKY_BARB  = 288,
    ITEM_POWER_BRACER = 289,
    ITEM_POWER_BELT   = 290,
    ITEM_POWER_LENS   = 291,
    ITEM_POWER_BAND   = 292,
    ITEM_POWER_ANKLET = 293,
    ITEM_POWER_WEIGHT = 294,
    ITEM_SHED_SHELL   = 295,
    ITEM_BIG_ROOT     = 296,
    ITEM_CHOICE_SPECS = 297,
    ITEM_FLAME_PLATE  = 298,
    ITEM_SPLASH_PLATE = 299,
    ITEM_ZAP_PLATE    = 300,
    ITEM_MEADOW_PLATE = 301,
    ITEM_ICICLE_PLATE = 302,
    ITEM_FIST_PLATE   = 303,
    ITEM_TOXIC_PLATE  = 304,
    ITEM_EARTH_PLATE  = 305,
    ITEM_SKY_PLATE    = 306,
    ITEM_MIND_PLATE   = 307,
    ITEM_INSECT_PLATE = 308,
    ITEM_STONE_PLATE  = 309,
    ITEM_SPOOKY_PLATE = 310,
    ITEM_DRACO_PLATE  = 311,
    ITEM_DREAD_PLATE  = 312,
    ITEM_IRON_PLATE   = 313,
    ITEM_ODD_INCENSE  = 314,
    ITEM_ROCK_INCENSE = 315,
    ITEM_FULL_INCENSE = 316,
    ITEM_WAVE_INCENSE = 317,
    ITEM_ROSE_INCENSE = 318,
    ITEM_LUCK_INCENSE = 319,
    ITEM_PURE_INCENSE = 320,
    ITEM_PROTECTOR    = 321,
    ITEM_ELECTIRIZER  = 322,
    ITEM_MAGMARIZER   = 323,
    ITEM_DUBIOUS_DISC = 324,
    ITEM_REAPER_CLOTH = 325,
    ITEM_RAZOR_CLAW   = 326,
    ITEM_RAZOR_FANG   = 327,
    ITEM_TM01         = 328,
    ITEM_TM02         = 329,
    ITEM_TM03         = 330,
    ITEM_TM04         = 331,
    ITEM_TM05         = 332,
    ITEM_TM06         = 333,
    ITEM_TM07         = 334,
    ITEM_TM08         = 335,
    ITEM_TM09         = 336,
    ITEM_TM10         = 337,
    ITEM_TM11         = 338,
    ITEM_TM12         = 339,
    ITEM_TM13         = 340,
    ITEM_TM14         = 341,
    ITEM_TM15         = 342,
    ITEM_TM16         = 343,
    ITEM_TM17         = 344,
    ITEM_TM18         = 345,
    ITEM_TM19         = 346,
    ITEM_TM20         = 347,
    ITEM_TM21         = 348,
    ITEM_TM22         = 349,
    ITEM_TM23         = 350,
    ITEM_TM24         = 351,
    ITEM_TM25         = 352,
    ITEM_TM26         = 353,
    ITEM_TM27         = 354,
    ITEM_TM28         = 355,
    ITEM_TM29         = 356,
    ITEM_TM30         = 357,
    ITEM_TM31         = 358,
    ITEM_TM32         = 359,
    ITEM_TM33         = 360,
    ITEM_TM34         = 361,
    ITEM_TM35         = 362,
    ITEM_TM36         = 363,
    ITEM_TM37         = 364,
    ITEM_TM38         = 365,
    ITEM_TM39         = 366,
    ITEM_TM40         = 367,
    ITEM_TM41         = 368,
    ITEM_TM42         = 369,
    ITEM_TM43         = 370,
    ITEM_TM44         = 371,
    ITEM_TM45         = 372,
    ITEM_TM46         = 373,
    ITEM_TM47         = 374,
    ITEM_TM48         = 375,
    ITEM_TM49         = 376,
    ITEM_TM50         = 377,
    ITEM_TM51         = 378,
    ITEM_TM52         = 379,
    ITEM_TM53         = 380,
    ITEM_TM54         = 381,
    ITEM_TM55         = 382,
    ITEM_TM56         = 383,
    ITEM_TM57         = 384,
    ITEM_TM58         = 385,
    ITEM_TM59         = 386,
    ITEM_TM60         = 387,
    ITEM_TM61         = 388,
    ITEM_TM62         = 389,
    ITEM_TM63         = 390,
    ITEM_TM64         = 391,
    ITEM_TM65         = 392,
    ITEM_TM66         = 393,
    ITEM_TM67         = 394,
    ITEM_TM68         = 395,
    ITEM_TM69         = 396,
    ITEM_TM70         = 397,
    ITEM_TM71         = 398,
    ITEM_TM72         = 399,
    ITEM_TM73         = 400,
    ITEM_TM74         = 401,
    ITEM_TM75         = 402,
    ITEM_TM76         = 403,
    ITEM_TM77         = 404,
    ITEM_TM78         = 405,
    ITEM_TM79         = 406,
    ITEM_TM80         = 407,
    ITEM_TM81         = 408,
    ITEM_TM82         = 409,
    ITEM_TM83         = 410,
    ITEM_TM84         = 411,
    ITEM_TM85         = 412,
    ITEM_TM86         = 413,
    ITEM_TM87         = 414,
    ITEM_TM88         = 415,
    ITEM_TM89         = 416,
    ITEM_TM90         = 417,
    ITEM_TM91         = 418,
    ITEM_TM92         = 419,
    ITEM_HM01         = 420,
    ITEM_HM02         = 421,
    ITEM_HM03         = 422,
    ITEM_HM04         = 423,
    ITEM_HM05         = 424,
    ITEM_HM06         = 425,
    ITEM_HM07         = 426,
    ITEM_HM08         = 427,
    ITEM_EXPLORER_KIT = 428,
    ITEM_LOOT_SACK    = 429,
    ITEM_RULE_BOOK    = 430,
    ITEM_POKE_RADAR   = 431,
    ITEM_POINT_CARD   = 432,
    ITEM_JOURNAL      = 433,
    ITEM_SEAL_CASE    = 434,
    ITEM_FASHION_CASE = 435,
    ITEM_SEAL_BAG     = 436,
    ITEM_PAL_PAD      = 437,
    ITEM_WORKS_KEY    = 438,
    ITEM_OLD_CHARM    = 439,
    ITEM_GALACTIC_KEY = 440,
    ITEM_RED_CHAIN    = 441,
    ITEM_TOWN_MAP     = 442,
    ITEM_VS_SEEKER    = 443,
    ITEM_COIN_CASE    = 444,
    ITEM_OLD_ROD      = 445,
    ITEM_GOOD_ROD     = 446,
    ITEM_SUPER_ROD    = 447,
    ITEM_SPRAYDUCK    = 448,
    ITEM_POFFIN_CASE  = 449,
    ITEM_BICYCLE      = 450,
    ITEM_SUITE_KEY    = 451,
    ITEM_OAKS_LETTER  = 452,
    ITEM_LUNAR_WING   = 453,
    ITEM_MEMBER_CARD  = 454,
    ITEM_AZURE_FLUTE  = 455,
    ITEM_S_S_TICKET   = 456,
    ITEM_CONTEST_PASS = 457,
    ITEM_MAGMA_STONE  = 458,
    ITEM_PARCEL       = 459,
    ITEM_COUPON_1     = 460,
    ITEM_COUPON_2     = 461,
    ITEM_COUPON_3     = 462,
    ITEM_STORAGE_KEY  = 463,
    ITEM_SECRETPOTION = 464,
    ITEM_VS_RECORDER  = 465,
    ITEM_GRACIDEA     = 466,
    ITEM_SECRET_KEY   = 467,
    MAX_ITEMS         = 468,
};
enum PoketchAppID {
    POKETCH_APPID_DIGITALWATCH      =  0,
    POKETCH_APPID_CALCULATOR        =  1,
    POKETCH_APPID_MEMOPAD           =  2,
    POKETCH_APPID_PEDOMETER         =  3,
    POKETCH_APPID_PARTYSTATUS       =  4,
    POKETCH_APPID_FRIENDSHIPCHECKER =  5,
    POKETCH_APPID_DOWSINGMACHINE    =  6,
    POKETCH_APPID_BERRYSEARCHER     =  7,
    POKETCH_APPID_DAYCARECHECKER    =  8,
    POKETCH_APPID_POKEMONHISTORY    =  9,
    POKETCH_APPID_COUNTER           = 10,
    POKETCH_APPID_ANALOGWATCH       = 11,
    POKETCH_APPID_MARKINGMAP        = 12,
    POKETCH_APPID_LINKSEARCHER      = 13,
    POKETCH_APPID_COINTOSS          = 14,
    POKETCH_APPID_MOVETESTER        = 15,
    POKETCH_APPID_CALENDAR          = 16,
    POKETCH_APPID_DOTART            = 17,
    POKETCH_APPID_ROULETTE          = 18,
    POKETCH_APPID_TRAINERCOUNTER    = 19,
    POKETCH_APPID_KITCHENTIMER      = 20,
    POKETCH_APPID_COLORCHANGER      = 21,
    POKETCH_APPID_MATCHUPCHECKER    = 22,
    POKETCH_APPID_STOPWATCH         = 23,
    POKETCH_APPID_ALARMCLOCK        = 24,
    POKETCH_APPID_MAX               = 25,
    POKETCH_APPID_NONE              = -1,
};
typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;
typedef struct BallCapsule {
    BallSeal seals[8 ];
} BallCapsule;
typedef struct EasyChatSentence {
    u16 type;
    u16 id;
    u16 words[2 ];
} EasyChatSentence;
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
    MailIconData iconData[3 ];
    u16 platExclusiveFormIcons;
    EasyChatSentence sentences[3 ];
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
    u16 moves[4 ];
    u8 moveCurrentPPs[4 ];
    u8 movePPUps[4 ];

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
    u8 sLotoId;
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
    u16 unused;
} PokemonDataBlockD;
typedef union {
    PokemonDataBlockA blockA;
    PokemonDataBlockB blockB;
    PokemonDataBlockC blockC;
    PokemonDataBlockD blockD;
} PokemonDataBlock;
typedef struct BoxPokemon {
    u32 personality;
    u16 partyDecrypted : 1;
                u16 boxDecrypted : 1;
                u16 checksumFailed : 1;
                u16 unused : 13;
    u16 checksum;
    PokemonDataBlock dataBlocks[4];
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
typedef struct BattleRegulation {
    u16 name[12 ];
    u16 maxTotalLevel;
    u8 teamSize;
    u8 maxLevel;
    s8 heightRestriction;
    s8 weightRestriction;
    u8 evolutionRestriction : 1;
    u8 specialSpeciesRestriction : 1;
    u8 speciesUniqueness : 1;
    u8 itemUniqueness : 1;
    u8 moveRestrictions : 1;
} BattleRegulation;
typedef struct MysteryGiftPokemonData {
    BOOL hasCustomOT;
    Pokemon pokemon;
    u8 specialRibbonsDescIDs[10];
    u8 padding_FA[6];
} MysteryGiftPokemonData;
typedef struct MysteryGiftItemData {
    enum Item item;
    BOOL shouldPlayAnimation;
} MysteryGiftItemData;
enum MysteryGiftCosmeticsType {
    MG_COSMETICS_SEAL = 1,
    MG_COSMETICS_ACCESSORY,
    MG_COSMETICS_BACKDROP,
};
typedef struct MysteryGiftCosmeticsData {
    enum MysteryGiftCosmeticsType type;
    int id;
} MysteryGiftCosmeticsData;
typedef union GiftData {
    MysteryGiftPokemonData pokemonGiftData;
    MysteryGiftItemData itemGiftData;
    int decorationGoodID;
    MysteryGiftCosmeticsData cosmeticGiftData;
    enum PoketchAppID poketchAppID;
    BattleRegulation battleReg;
} GiftData;
typedef struct PGT {
    u16 type;
    u16 wondercardSlot : 2;
    u16 : 14;
    GiftData data;
} PGT;
typedef struct MysteryGiftEventHeader {
    u16 title[36 ];
    u32 validGames;
    u16 id;
    u8 unique : 1;
    u8 unk_4E_1 : 1;
    u8 hasWonderCard : 1;
    u8 savePgt : 1;
    u8 shareable : 1;
    u8 fromSharing : 1;
    u8 : 2;
    u8 padding_4F;
} MysteryGiftEventHeader;
typedef struct WonderCard {
    PGT pgt;
    MysteryGiftEventHeader eventHeader;
    u16 description[250 ];
    u8 sharesLeft;
    u8 padding_349;
    u16 spritesSpecies[3 ];
    u8 timesShared;
    u8 padding_351[3];
    s32 receivedDate;
} WonderCard;
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



    u32 animData[29 ];
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
typedef struct SpriteResourcesHeader {
    const NNSG2dImageProxy *imageProxy;
    const NNSG2dCharacterData *charData;
    const NNSG2dImagePaletteProxy *paletteProxy;
    NNSG2dCellDataBank *cellBank;
    const NNSG2dCellAnimBankData *cellAnimBank;
    const NNSG2dMultiCellDataBank *multiCellBank;
    const NNSG2dMultiCellAnimBankData *multiCellAnimBank;
    BOOL isVRamTransfer;
    u8 priority;
    u8 padding_21[3];
} SpriteResourcesHeader;
typedef struct Resource {
    int id;
    void *data;
} Resource;
typedef struct ResourceCollection {
    Resource *resources;
    int capacity;
    int count;
} ResourceCollection;
enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};
typedef struct SpriteResource {
    Resource *rawResource;
    enum SpriteResourceType type;
    void *data;
} SpriteResource;
typedef struct SpriteResourceCollection {
    ResourceCollection *collection;
    SpriteResource *resources;
    int capacity;
    int count;
    enum SpriteResourceType type;
} SpriteResourceCollection;
typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;
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
typedef struct StringTemplate {
    u32 maxArgs;
    enum HeapID heapID;
    StringTemplateArg *args;
    String *templateBuf;
} StringTemplate;
enum MysteryGiftAppState {
    MG_APP_STATE_SETUP = 0,
    MG_APP_STATE_INIT_GRAPHICS,
    MG_APP_STATE_WAIT_SCREEN_TRANSITION,
    MG_APP_STATE_WAIT_MAIN_MENU_INPUT,
    MG_APP_STATE_WAIT_RECEPTION_METHOD_CHOICE,
    MG_APP_STATE_WAIT_CONFIRM_WIRELESS_COMMS,
    MG_APP_STATE_UNUSED_6,

    MG_APP_STATE_SEARCH_FOR_FRIEND_GIFT,
    MG_APP_STATE_WAIT_CONFIRM_RECEIVE_FRIEND_GIFT,
    MG_APP_STATE_CONNECT_WITH_FRIEND,
    MG_APP_STATE_RECEIVING_FROM_FRIEND,
    MG_APP_STATE_SAVE_GIFT_FROM_FRIEND,
    MG_APP_STATE_CHECK_FRIEND_CONNECTION_STILL_VALID,
    MG_APP_STATE_WAIT_FRIEND_READY_TO_DISCONNECT,
    MG_APP_STATE_PERSON_SHARING_DISCONNECTED,
    MG_APP_STATE_RECEIVE_FROM_FRIEND_SUCCESS,
    MG_APP_STATE_FRIEND_LEFT_BEFORE_SENDING,

    MG_APP_STATE_NO_GIFT_FOUND,
    MG_APP_STATE_RETURN_TO_MENU,
    MG_APP_STATE_FOUND_WIFI_OR_GBA_GIFT,
    MG_APP_STATE_WAIT_ACCEPT_WIFI_OR_GBA_GIFT,
    MG_APP_STATE_GOTO_SHOW_RECEIVING_GIFT_MSG,
    MG_APP_STATE_SHOW_RECEIVING_GIFT_MSG,
    MG_APP_STATE_SAVE_RECEIVED_MYSTERY_GIFT,
    MG_APP_STATE_WAIT_MYSTERY_GIFT_SAVED,
    MG_APP_STATE_WAIT_BEFORE_EXIT_AFTER_RECEIVING_GIFT,
    MG_APP_STATE_UNUSED_26,
    MG_APP_STATE_SEARCH_FOR_WIFI_EVENT,


    MG_APP_STATE_UNUSED_28,
    MG_APP_STATE_UNUSED_29,
    MG_APP_STATE_UNUSED_30,

    MG_APP_STATE_WAIT_CONFIRM_RECEIVE_LOCAL_WIRELESS_GIFT,
    MG_APP_STATE_SEARCH_FOR_LOCAL_WIRELESS_EVENT,
    MG_APP_STATE_RECEIVE_FROM_LOCAL_WIRELESS,
    MG_APP_STATE_WAIT_LOCAL_WIRELESS_GIFT_SAVED,
    MG_APP_STATE_WAIT_FOR_ANIMATION_FINISHED,
    MG_APP_STATE_DISCONNECT_FROM_LOCAL_WIRELESS,


    MG_APP_STATE_UNUSED_37,
    MG_APP_STATE_UNUSED_38,
    MG_APP_STATE_UNUSED_39,
    MG_APP_STATE_UNUSED_40,
    MG_APP_STATE_UNUSED_41,
    MG_APP_STATE_UNUSED_42,
    MG_APP_STATE_UNUSED_43,
    MG_APP_STATE_UNUSED_44,
    MG_APP_STATE_UNUSED_45,
    MG_APP_STATE_UNUSED_46,
    MG_APP_STATE_UNUSED_47,
    MG_APP_STATE_UNUSED_48,

    MG_APP_STATE_FROM_FRIEND_OR_CANT_RECEIVE,
    MG_APP_STATE_WAIT_MESSAGE_BOX_FOR_TRANSITION,
    MG_APP_STATE_WAIT_INPUT_RETURN_TO_MENU,
    MG_APP_STATE_ASK_CONFIRM_RECEIVE_SHARED_GIFT,
    MG_APP_STATE_EXIT_TO_TITLE_SCREEN,
    MG_APP_STATE_EXIT_TO_WONDER_CARDS_APP,
    MG_APP_STATE_EXIT_AFTER_RECEIVING_GIFT,
    MG_APP_STATE_SHOW_RECEIVED_WONDERCARD,
    MG_APP_STATE_RESET_SYSTEM,

    MG_APP_KEEP_PREVIOUS_STATE = -1,
};
enum MysteryGiftAnimationStatus {
    MG_ANIMATION_STATUS_DONE = 0,
    MG_ANIMATION_STATUS_BEGIN,
    MG_ANIMATION_STATUS_PROCEED_IMPLOSION
};
typedef struct MysteryGiftEventData {
    MysteryGiftEventHeader header;
    WonderCard wonderCard;
} MysteryGiftEventData;
typedef struct MysteryGiftAppData MysteryGiftAppData;
typedef void (*MysteryGiftAppMainCallbackFuncPtr)(MysteryGiftAppData *);
typedef BOOL (*DWCCallbackFuncPtr)(void);
struct MysteryGiftAppData {
    BgConfig *bgConfig;
    SaveData *saveData;
    Options *options;
    StringTemplate *strTemplate;
    MessageLoader *msgLoader;
    String *string;
    Window messageBox;
    Window menuWindows[3 ];
    Window wonderCardTitleWindow;
    int msgBoxPrinterDelay;
    int textPrinterId;
    int unused_70;
    enum MysteryGiftAppState queuedState;
    ListMenu *listMenu;
    StringList *listMenuOptions;
    int canReceiveGiftStatus;
    int foundGiftType;
    int unused_88;
    MysteryGiftEventData eventData;
    int wirelessCommsTimeout;
    int unk_438;
    int delay;
    BOOL exitToWondercardsApp;
    SpriteList *unusedSpriteList;
    G2dRenderer unusedG2dRenderer;
    SpriteResourceCollection *unusedSpriteResourceCollections[SPRITE_RESOURCE_MAX];
    SpriteResource *unusedSpriteResources[SPRITE_RESOURCE_MAX];
    SpriteResourcesHeader unusedSpriteResourcesHeader;
    Sprite *dowloadArrowSprite;
    int unk_62C;
    int unk_630;
    int unk_634;
    u8 wirelessDistributionBuffer[4096 ];
    int receptionMethod;
    enum MysteryGiftAppState *statePtr;
    int giftSearchResultDelay;
    int nasAuthState;
    u8 *unused_1648;
    DWCInetControl unk_164C;
    BOOL unused_16AC;
    int unk_16B0;
    int unk_16B4;
    char wifiDistributionBuffer[4096 ];
    int unused_26B8;
    u32 unk_26BC;
    u32 unk_26C0;
    u32 unk_26C4;
    BOOL wifiCommErrored;
    int wifiCommErrorStringID;
    int wifiCommErrorCode;
    DWCErrorType wifiCommErrorType;
    BOOL wifiExitRequested;
    DWCCallbackFuncPtr dwcCallback;
    Window wifiCommErrorWindow;
    WonderCard wonderCard;
    MysteryGiftAppMainCallbackFuncPtr mainCallback;
    BOOL cancelSave;
    void *waitDial;
    enum MysteryGiftAnimationStatus animationStatus;
};
void MysteryGiftApp_ShowWiFiCommError(MysteryGiftAppData *appData);
void CommManager_StartMysteryGiftWifi(SaveData *saveData);
void sub_0203A880();
typedef void (*Callback)(void *);
typedef void (*UnkCallback)(void);
enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};
typedef struct System {
    Callback vblankCallback;
    void *vblankCallbackData;
    Callback hblankCallback;
    void *hblankCallbackData;
    UnkCallback dummyCallback_10;
    UnkCallback dummyCallback_14;
    SysTaskManager *mainTaskMgr;
    SysTaskManager *vBlankTaskMgr;
    SysTaskManager *postVBlankTaskMgr;
    SysTaskManager *printTaskMgr;
    u32 *unused_28;
    u32 vblankCounter;
    u32 frameCounter;
    enum ButtonMode buttonMode;
    u32 heldKeysRaw;
    u32 pressedKeysRaw;
    u32 pressedKeysRepeatableRaw;
    u32 heldKeys;
    u32 pressedKeys;
    u32 pressedKeysRepeatable;
    int autorepeatTimer;
    int autorepeatRate;
    int autorepeatDelay;
    u16 touchX;
    u16 touchY;
    u16 touchPressed;
    u16 touchHeld;
    u8 touchAutoSampling;
    u8 whichScreenIs3D;
    u8 gbaCartridgeVersion;
    u8 inhibitSleep;
    u8 inhibitReset;
    u8 padding_69[3];
    BOOL showTitleScreenIntro;
    u32 *heapCanary;
} System;
extern System gSystem;
void sub_02034D8C(void);
BOOL sub_02034DB8(void);
void LoadDwcOverlay(void);
void Overlay_LoadHttpOverlay(void);
/* invented: pooled statics at ov74_0223E304 */
typedef struct {
    BOOL unk_00;
    BOOL unk_04;
    int unk_08;
    int unk_0C;
    volatile BOOL unk_10;
} Ov74Statics;
extern Ov74Statics ov74_0223E304;
extern DWCNdFileInfo ov74_0223E318[10];
void *ov74_022369A8(DWCAllocType param0, u32 param1, int param2);
void ov74_022369C8(DWCAllocType param0, void *param1, u32 param2);
int ov97_02238DA4(MysteryGiftAppData *param0);
BOOL ov97_02238DF8(MysteryGiftAppData *param0);
void ov74_02236A54(MysteryGiftAppData *param0, int *param1, int param2);
void ov74_02236A78(MysteryGiftAppData *param0, int param1, int *param2, int param3, int param4);
void ov74_02236AAC(DWCNdCallbackReason param0, DWCNdError param1, int param2);
void ov74_02236AC8(void);

int ov74_02236AE0(ApplicationManager *appMan, int *param1);

int ov74_022369D8(MysteryGiftAppData *param0);
void LoadOVY38(void);
s32 sub_02039FD8(enum HeapID heapID);
void sub_020394A0(SaveData *saveData);
BOOL ov74_02236A2C(MysteryGiftAppData *param0);
void ov74_0222ACD8(MysteryGiftAppData *appData);
int ov74_02236AE0(ApplicationManager *appMan, int *param1)
{
    DWCNasLoginState v1;
    MysteryGiftAppData *v2 = OverlayManager_GetData(appMan, param1);

    if (v2->dwcCallback) {
        if (v2->dwcCallback() == 1) {
            *param1 = ov74_022369D8(v2);
        }
    }

    switch (*param1) {
    case 4096:
        sub_02034D8C();
        LoadDwcOverlay();
        LoadOVY38();
        sub_02039FD8(HEAP_ID_APPLICATION);
        *param1 = 4097;
        break;
    case 4097:
        if (sub_02034DB8()) {
            DWC_SetMemFunc(ov74_022369A8, ov74_022369C8);
            sub_020394A0(v2->saveData);
            *param1 = 4098;
        }
        break;
    case 4098:
        DWC_InitInetEx(&v2->unk_164C, 2 , 1 , 20 );
        DWC_SetAuthServer(DWC_CONNECTINET_AUTH_RELEASE);
        DWC_ConnectInetAsync();
        sub_0203A880();
        *param1 = 4099;
        v2->wifiExitRequested = 0;
        break;
    case 4099:
        DWC_ProcessInet();

        if (DWC_CheckInet() == 0) {
            (void)0;
        } else {
            if (ov74_02236A2C(v2) == 1) {
                if (v2->wifiExitRequested == 1) {
                    ov74_0223E304.unk_08 = 3;
                    *param1 = 4109;
                } else {
                    *param1 = 4100;
                }
            } else {
                *param1 = ov74_022369D8(v2);
            }
        }

        if (gSystem.pressedKeys & 0x0002 ) {
            v2->wifiExitRequested = 1;
        }
        break;
    case 4100:
        if (DWC_NASLoginAsync() == 0) {
            *param1 = ov74_022369D8(v2);
        } else {
            *param1 = 4101;
        }
        break;
    case 4101:
        v1 = DWC_NASLoginProcess();

        if (v1 == DWC_NASLOGIN_STATE_SUCCESS) {
            *param1 = 4102;
            v2->dwcCallback = DWC_UpdateConnection;
        } else if (v1 == DWC_NASLOGIN_STATE_ERROR) {
            *param1 = ov74_022369D8(v2);
            DWC_CleanupInet();
        } else if (v1 == DWC_NASLOGIN_STATE_CANCELED) {
            ov74_0223E304.unk_08 = 3;
            *param1 = 4109;
        }

        if (gSystem.pressedKeys & 0x0002 ) {
            DWC_NASLoginAbort();
        }
        break;
    case 4102:
        if (DWC_NdInitAsync(ov74_02236AAC, "IPGE", "DwuCXmdJfPVLBnKA") == 0) {
            *param1 = ov74_022369D8(v2);
            break;
        }

        ov74_02236A54(v2, param1, 4103);
        break;
    case 4103:
        if (v2->wifiExitRequested == 1) {
            ov74_02236A78(v2, 3, param1, 4108, 4108);
            break;
        }

        if (DWC_NdSetAttr("", "", "") == 0) {
            *param1 = ov74_022369D8(v2);
            break;
        }

        *param1 = 4104;
        break;
    case 4104:
        if (DWC_NdGetFileListNumAsync(&v2->unk_16B4) == 0) {
            *param1 = ov74_022369D8(v2);
            break;
        }

        ov74_02236A54(v2, param1, 4105);
        break;
    case 4105:
        if (v2->unk_16B4 != 1) {
            ov74_02236A78(v2, 2, param1, 4109, 4109);
        } else {
            if (DWC_NdGetFileListAsync(ov74_0223E318, 0, 10) == 0) {
                *param1 = ov74_022369D8(v2);
                break;
            }

            ov74_02236A54(v2, param1, 4106);
        }
        break;
    case 4106:
        if (DWC_NdGetFileAsync(&ov74_0223E318[0], v2->wifiDistributionBuffer, 4096) == 0) {
            *param1 = ov74_022369D8(v2);
            break;
        }

        *param1 = 4107;
        v2->unk_26C4 = 0;
        break;
    case 4107:
        if (ov74_0223E304.unk_00 == 0) {
            if (gSystem.pressedKeys & 0x0002 ) {
                ov74_02236A78(v2, 3, param1, 4108, 4108);
            } else {
                if (DWC_NdGetProgress(&v2->unk_26BC, &v2->unk_26C0) == 1) {
                    int pct = (v2->unk_26BC * 100) / v2->unk_26C0;
                    if (v2->unk_26C4 != pct) {
                        v2->unk_26C4 = pct;
                    }
                }
            }
        } else {
            if (ov74_0223E304.unk_0C != DWC_ND_ERROR_NONE) {
                *param1 = ov74_022369D8(v2);
            } else if (v2->wifiExitRequested == 0) {
                ov74_02236A78(v2, 1, param1, 4109, 4109);
            } else {
                ov74_02236A78(v2, 3, param1, 4109, 4109);
            }
        }
        break;
    case 4108:
        if (DWC_NdCancelAsync() == 0) {
            *param1 = 4109;
        } else {
            v2->dwcCallback = 0L ;
            ov74_02236AC8();
            return ov74_0223E304.unk_08;
        }
        break;
    case 4109:
        if (DWC_CleanupInetAsync() == 1) {
            v2->dwcCallback = 0L ;
            ov74_02236AC8();
            return ov74_0223E304.unk_08;
        }
        break;
    case 4110:
        break;
    case 4111:
        if (ov74_0223E304.unk_10 == 1) {
            ov74_0222ACD8(v2);

            if ((v2->wifiCommErrorType == DWC_ETYPE_SHUTDOWN_ND) || (v2->wifiCommErrorType == DWC_ETYPE_DISCONNECT)) {
                ov74_02236A78(v2, 3, param1, 4113, 4112);
            } else {
                *param1 = 4113;
            }
        }
        break;
    case 4113:
        DWC_CleanupInet();
        *param1 = 4112;
        break;
    case 4112:
        if (gSystem.pressedKeys & 0x0001 ) {
            v2->dwcCallback = 0L ;
            DWC_ClearError();
            ov74_02236AC8();

            return 4;
        }
        break;
    case 4114:
        if (ov74_0223E304.unk_00 == 1) {
            ov74_0223E304.unk_00 = 0;

            if (ov74_0223E304.unk_0C != DWC_ND_ERROR_NONE) {
                *param1 = ov74_022369D8(v2);
            } else {
                *param1 = v2->unk_16B0;
            }
        } else if (gSystem.pressedKeys & 0x0002 ) {
            v2->wifiExitRequested = 1;
        }
        break;
    case 4115:
        if (ov74_0223E304.unk_04 == 1) {
            ov74_0223E304.unk_04 = 0;
            *param1 = v2->unk_16B0;
        } else if (gSystem.pressedKeys & 0x0002 ) {
            v2->wifiExitRequested = 1;
        }

        break;
    }

    return 0;
}
