#include "global.h"
#include "pokemon_summary_args.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "overworld_anim_manager.h"
#include "seal_case.h"
#include "pm_string.h"
#include "unk_0202C034.h"
#include "bg_window.h"
#include "camera.h"
#include "options.h"
#include "filesystem.h"
#include "overlay_74_main_menu_util.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "bag_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "overlay_80_frontier_opponent.h"
#include "field/model_attributes.h"
#include "field/area_light.h"
#include "field/dynamic_terrain_height.h"
#include "overlay_01_land_data_manager.h"
#include "map_events_internal.h"
#include "player_avatar.h"
#include "trainer_case.h"
#include "union_room_manager.h"
#include "save_frontier.h"
#include "field/terrain_collision_manager.h"

struct ApplicationManager;

struct ApplicationManagerTemplate;

struct AreaDataFile;

struct AreaDataManager;

struct AreaDataManagerLoadData;

struct AreaLightManager;

struct AreaLightTemplate;

struct Background;

struct Bag;

struct BagCursor;

struct BagItem;

struct BallCapsule;

struct BallSeal;

struct BattleBagCursor;

struct BattleRecords;

struct BattleRegulation;

struct BattleTower;

struct BattleTowerPartnerData;

struct BattleTowerSave;

struct BgConfig;

struct BgEvent;

struct BicycleSlopeAnimation;

struct BoxPokemon;

struct Camera;

struct CameraAngle;

struct CameraLookAt;

struct CameraPerspectiveProjection;

struct CameraPositionHistory;

struct CaptureAttempt;

struct ChatotCry;

struct ColoredArrow;

struct CoordEvent;

struct DynamicTerrainHeightManager;

struct EasyChatSentence;

struct EncodingSeed;

struct FieldBagCursor;

struct FieldBattleDTO;

struct FieldEffectManager;

struct FieldEffectRenderer;

struct FieldProcessManager;

struct FieldTask;

struct FieldWildBattleMetadata;

struct FogManager;

struct FrontierOpponent;

struct FrontierPokemon;

struct FrontierTrainer;

struct G2dRenderer;

struct GameRecords;

struct GrassEncounter;

struct GrassEncounters;

struct HoneyTreeShake;

struct HoneyTreeShakeList;

struct JournalEntry;

struct JournalEntryMon;

struct JournalEntryTitle;

struct JournalEntryTrainer;

struct LandDataManager;

struct ListMenu;

struct ListMenuTemplate;

struct Location;

struct Mail;

struct MapHeaderData;

struct MapLoadMode;

struct MapMatrix;

struct MapMatrixData;

struct MapObject;

struct MapObjectManager;

struct MapObjectsToPreload;

struct MapProp;

struct MapPropAnimation;

struct MapPropAnimationManager;

struct MapPropManager;

struct MapPropMaterialShape;

struct MapPropMaterialShapeIDs;

struct MapPropMaterialShapeIDsLocator;

struct MapPropOneShotAnimation;

struct MapPropOneShotAnimationList;

struct MapPropOneShotAnimationManager;

struct Menu;

struct MenuTemplate;

struct MsgData;

struct ModelAttributes;

struct NARC;

struct ObjectEvent;

struct Options;

struct PCBoxes;

struct PalPad;

struct Party;

struct PartyPokemon;

struct PlayTime;

struct PlayerAvatar;

struct PlayerData;

struct Pokedex;

struct PokedexMemory;

struct Pokemon;

struct PokemonDataBlockA;

struct PokemonDataBlockB;

struct PokemonDataBlockC;

struct PokemonDataBlockD;

struct PokemonSpriteTemplate;

struct Poketch;

struct RadarChain;

struct Resource;

struct ResourceCollection;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct Signpost;

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

struct TerrainAttributes;

struct TerrainCollisionManager;

struct TradeRoom;

struct TradeRoomArgs;

struct TradeRoomMonDisplayData;

struct Trainer;

struct TrainerCase;

struct TrainerCaseBadge;

struct TrainerHeader;

struct TrainerInfo;

struct UndergroundTopScreenContext_t;

struct UnionRoomManager;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov56_02256468_t;

struct UnkStruct_ov5_021DF8FC_t;

struct UnkStruct_ov5_021EB0E0;

struct WarpEvent;

struct WaterEncounter;

struct WaterEncounters;

struct WiFiHistory;

struct WiFiList;

struct WifiBattleTowerRecord;

struct WildEncounters;

struct Window;

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

enum PokemonStat {
    STAT_HP = 0,
    STAT_ATTACK = 1,
    STAT_DEFENSE = 2,
    STAT_SPEED = 3,
    STAT_SPECIAL_ATTACK = 4,
    STAT_SPECIAL_DEFENSE = 5,
    STAT_MAX = 6,
};

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef u16 charcode_t;

typedef struct EasyChatSentence {
    u16 type;
    u16 id;
    u16 words[2];
} EasyChatSentence;

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
    EasyChatSentence sentences[3];
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

typedef struct TradeRoom TradeRoom;

enum Species {
    SPECIES_NONE = 0,
    SPECIES_BULBASAUR = 1,
    SPECIES_IVYSAUR = 2,
    SPECIES_VENUSAUR = 3,
    SPECIES_CHARMANDER = 4,
    SPECIES_CHARMELEON = 5,
    SPECIES_CHARIZARD = 6,
    SPECIES_SQUIRTLE = 7,
    SPECIES_WARTORTLE = 8,
    SPECIES_BLASTOISE = 9,
    SPECIES_CATERPIE = 10,
    SPECIES_METAPOD = 11,
    SPECIES_BUTTERFREE = 12,
    SPECIES_WEEDLE = 13,
    SPECIES_KAKUNA = 14,
    SPECIES_BEEDRILL = 15,
    SPECIES_PIDGEY = 16,
    SPECIES_PIDGEOTTO = 17,
    SPECIES_PIDGEOT = 18,
    SPECIES_RATTATA = 19,
    SPECIES_RATICATE = 20,
    SPECIES_SPEAROW = 21,
    SPECIES_FEAROW = 22,
    SPECIES_EKANS = 23,
    SPECIES_ARBOK = 24,
    SPECIES_PIKACHU = 25,
    SPECIES_RAICHU = 26,
    SPECIES_SANDSHREW = 27,
    SPECIES_SANDSLASH = 28,
    SPECIES_NIDORAN_F = 29,
    SPECIES_NIDORINA = 30,
    SPECIES_NIDOQUEEN = 31,
    SPECIES_NIDORAN_M = 32,
    SPECIES_NIDORINO = 33,
    SPECIES_NIDOKING = 34,
    SPECIES_CLEFAIRY = 35,
    SPECIES_CLEFABLE = 36,
    SPECIES_VULPIX = 37,
    SPECIES_NINETALES = 38,
    SPECIES_JIGGLYPUFF = 39,
    SPECIES_WIGGLYTUFF = 40,
    SPECIES_ZUBAT = 41,
    SPECIES_GOLBAT = 42,
    SPECIES_ODDISH = 43,
    SPECIES_GLOOM = 44,
    SPECIES_VILEPLUME = 45,
    SPECIES_PARAS = 46,
    SPECIES_PARASECT = 47,
    SPECIES_VENONAT = 48,
    SPECIES_VENOMOTH = 49,
    SPECIES_DIGLETT = 50,
    SPECIES_DUGTRIO = 51,
    SPECIES_MEOWTH = 52,
    SPECIES_PERSIAN = 53,
    SPECIES_PSYDUCK = 54,
    SPECIES_GOLDUCK = 55,
    SPECIES_MANKEY = 56,
    SPECIES_PRIMEAPE = 57,
    SPECIES_GROWLITHE = 58,
    SPECIES_ARCANINE = 59,
    SPECIES_POLIWAG = 60,
    SPECIES_POLIWHIRL = 61,
    SPECIES_POLIWRATH = 62,
    SPECIES_ABRA = 63,
    SPECIES_KADABRA = 64,
    SPECIES_ALAKAZAM = 65,
    SPECIES_MACHOP = 66,
    SPECIES_MACHOKE = 67,
    SPECIES_MACHAMP = 68,
    SPECIES_BELLSPROUT = 69,
    SPECIES_WEEPINBELL = 70,
    SPECIES_VICTREEBEL = 71,
    SPECIES_TENTACOOL = 72,
    SPECIES_TENTACRUEL = 73,
    SPECIES_GEODUDE = 74,
    SPECIES_GRAVELER = 75,
    SPECIES_GOLEM = 76,
    SPECIES_PONYTA = 77,
    SPECIES_RAPIDASH = 78,
    SPECIES_SLOWPOKE = 79,
    SPECIES_SLOWBRO = 80,
    SPECIES_MAGNEMITE = 81,
    SPECIES_MAGNETON = 82,
    SPECIES_FARFETCHD = 83,
    SPECIES_DODUO = 84,
    SPECIES_DODRIO = 85,
    SPECIES_SEEL = 86,
    SPECIES_DEWGONG = 87,
    SPECIES_GRIMER = 88,
    SPECIES_MUK = 89,
    SPECIES_SHELLDER = 90,
    SPECIES_CLOYSTER = 91,
    SPECIES_GASTLY = 92,
    SPECIES_HAUNTER = 93,
    SPECIES_GENGAR = 94,
    SPECIES_ONIX = 95,
    SPECIES_DROWZEE = 96,
    SPECIES_HYPNO = 97,
    SPECIES_KRABBY = 98,
    SPECIES_KINGLER = 99,
    SPECIES_VOLTORB = 100,
    SPECIES_ELECTRODE = 101,
    SPECIES_EXEGGCUTE = 102,
    SPECIES_EXEGGUTOR = 103,
    SPECIES_CUBONE = 104,
    SPECIES_MAROWAK = 105,
    SPECIES_HITMONLEE = 106,
    SPECIES_HITMONCHAN = 107,
    SPECIES_LICKITUNG = 108,
    SPECIES_KOFFING = 109,
    SPECIES_WEEZING = 110,
    SPECIES_RHYHORN = 111,
    SPECIES_RHYDON = 112,
    SPECIES_CHANSEY = 113,
    SPECIES_TANGELA = 114,
    SPECIES_KANGASKHAN = 115,
    SPECIES_HORSEA = 116,
    SPECIES_SEADRA = 117,
    SPECIES_GOLDEEN = 118,
    SPECIES_SEAKING = 119,
    SPECIES_STARYU = 120,
    SPECIES_STARMIE = 121,
    SPECIES_MR_MIME = 122,
    SPECIES_SCYTHER = 123,
    SPECIES_JYNX = 124,
    SPECIES_ELECTABUZZ = 125,
    SPECIES_MAGMAR = 126,
    SPECIES_PINSIR = 127,
    SPECIES_TAUROS = 128,
    SPECIES_MAGIKARP = 129,
    SPECIES_GYARADOS = 130,
    SPECIES_LAPRAS = 131,
    SPECIES_DITTO = 132,
    SPECIES_EEVEE = 133,
    SPECIES_VAPOREON = 134,
    SPECIES_JOLTEON = 135,
    SPECIES_FLAREON = 136,
    SPECIES_PORYGON = 137,
    SPECIES_OMANYTE = 138,
    SPECIES_OMASTAR = 139,
    SPECIES_KABUTO = 140,
    SPECIES_KABUTOPS = 141,
    SPECIES_AERODACTYL = 142,
    SPECIES_SNORLAX = 143,
    SPECIES_ARTICUNO = 144,
    SPECIES_ZAPDOS = 145,
    SPECIES_MOLTRES = 146,
    SPECIES_DRATINI = 147,
    SPECIES_DRAGONAIR = 148,
    SPECIES_DRAGONITE = 149,
    SPECIES_MEWTWO = 150,
    SPECIES_MEW = 151,
    SPECIES_CHIKORITA = 152,
    SPECIES_BAYLEEF = 153,
    SPECIES_MEGANIUM = 154,
    SPECIES_CYNDAQUIL = 155,
    SPECIES_QUILAVA = 156,
    SPECIES_TYPHLOSION = 157,
    SPECIES_TOTODILE = 158,
    SPECIES_CROCONAW = 159,
    SPECIES_FERALIGATR = 160,
    SPECIES_SENTRET = 161,
    SPECIES_FURRET = 162,
    SPECIES_HOOTHOOT = 163,
    SPECIES_NOCTOWL = 164,
    SPECIES_LEDYBA = 165,
    SPECIES_LEDIAN = 166,
    SPECIES_SPINARAK = 167,
    SPECIES_ARIADOS = 168,
    SPECIES_CROBAT = 169,
    SPECIES_CHINCHOU = 170,
    SPECIES_LANTURN = 171,
    SPECIES_PICHU = 172,
    SPECIES_CLEFFA = 173,
    SPECIES_IGGLYBUFF = 174,
    SPECIES_TOGEPI = 175,
    SPECIES_TOGETIC = 176,
    SPECIES_NATU = 177,
    SPECIES_XATU = 178,
    SPECIES_MAREEP = 179,
    SPECIES_FLAAFFY = 180,
    SPECIES_AMPHAROS = 181,
    SPECIES_BELLOSSOM = 182,
    SPECIES_MARILL = 183,
    SPECIES_AZUMARILL = 184,
    SPECIES_SUDOWOODO = 185,
    SPECIES_POLITOED = 186,
    SPECIES_HOPPIP = 187,
    SPECIES_SKIPLOOM = 188,
    SPECIES_JUMPLUFF = 189,
    SPECIES_AIPOM = 190,
    SPECIES_SUNKERN = 191,
    SPECIES_SUNFLORA = 192,
    SPECIES_YANMA = 193,
    SPECIES_WOOPER = 194,
    SPECIES_QUAGSIRE = 195,
    SPECIES_ESPEON = 196,
    SPECIES_UMBREON = 197,
    SPECIES_MURKROW = 198,
    SPECIES_SLOWKING = 199,
    SPECIES_MISDREAVUS = 200,
    SPECIES_UNOWN = 201,
    SPECIES_WOBBUFFET = 202,
    SPECIES_GIRAFARIG = 203,
    SPECIES_PINECO = 204,
    SPECIES_FORRETRESS = 205,
    SPECIES_DUNSPARCE = 206,
    SPECIES_GLIGAR = 207,
    SPECIES_STEELIX = 208,
    SPECIES_SNUBBULL = 209,
    SPECIES_GRANBULL = 210,
    SPECIES_QWILFISH = 211,
    SPECIES_SCIZOR = 212,
    SPECIES_SHUCKLE = 213,
    SPECIES_HERACROSS = 214,
    SPECIES_SNEASEL = 215,
    SPECIES_TEDDIURSA = 216,
    SPECIES_URSARING = 217,
    SPECIES_SLUGMA = 218,
    SPECIES_MAGCARGO = 219,
    SPECIES_SWINUB = 220,
    SPECIES_PILOSWINE = 221,
    SPECIES_CORSOLA = 222,
    SPECIES_REMORAID = 223,
    SPECIES_OCTILLERY = 224,
    SPECIES_DELIBIRD = 225,
    SPECIES_MANTINE = 226,
    SPECIES_SKARMORY = 227,
    SPECIES_HOUNDOUR = 228,
    SPECIES_HOUNDOOM = 229,
    SPECIES_KINGDRA = 230,
    SPECIES_PHANPY = 231,
    SPECIES_DONPHAN = 232,
    SPECIES_PORYGON2 = 233,
    SPECIES_STANTLER = 234,
    SPECIES_SMEARGLE = 235,
    SPECIES_TYROGUE = 236,
    SPECIES_HITMONTOP = 237,
    SPECIES_SMOOCHUM = 238,
    SPECIES_ELEKID = 239,
    SPECIES_MAGBY = 240,
    SPECIES_MILTANK = 241,
    SPECIES_BLISSEY = 242,
    SPECIES_RAIKOU = 243,
    SPECIES_ENTEI = 244,
    SPECIES_SUICUNE = 245,
    SPECIES_LARVITAR = 246,
    SPECIES_PUPITAR = 247,
    SPECIES_TYRANITAR = 248,
    SPECIES_LUGIA = 249,
    SPECIES_HO_OH = 250,
    SPECIES_CELEBI = 251,
    SPECIES_TREECKO = 252,
    SPECIES_GROVYLE = 253,
    SPECIES_SCEPTILE = 254,
    SPECIES_TORCHIC = 255,
    SPECIES_COMBUSKEN = 256,
    SPECIES_BLAZIKEN = 257,
    SPECIES_MUDKIP = 258,
    SPECIES_MARSHTOMP = 259,
    SPECIES_SWAMPERT = 260,
    SPECIES_POOCHYENA = 261,
    SPECIES_MIGHTYENA = 262,
    SPECIES_ZIGZAGOON = 263,
    SPECIES_LINOONE = 264,
    SPECIES_WURMPLE = 265,
    SPECIES_SILCOON = 266,
    SPECIES_BEAUTIFLY = 267,
    SPECIES_CASCOON = 268,
    SPECIES_DUSTOX = 269,
    SPECIES_LOTAD = 270,
    SPECIES_LOMBRE = 271,
    SPECIES_LUDICOLO = 272,
    SPECIES_SEEDOT = 273,
    SPECIES_NUZLEAF = 274,
    SPECIES_SHIFTRY = 275,
    SPECIES_TAILLOW = 276,
    SPECIES_SWELLOW = 277,
    SPECIES_WINGULL = 278,
    SPECIES_PELIPPER = 279,
    SPECIES_RALTS = 280,
    SPECIES_KIRLIA = 281,
    SPECIES_GARDEVOIR = 282,
    SPECIES_SURSKIT = 283,
    SPECIES_MASQUERAIN = 284,
    SPECIES_SHROOMISH = 285,
    SPECIES_BRELOOM = 286,
    SPECIES_SLAKOTH = 287,
    SPECIES_VIGOROTH = 288,
    SPECIES_SLAKING = 289,
    SPECIES_NINCADA = 290,
    SPECIES_NINJASK = 291,
    SPECIES_SHEDINJA = 292,
    SPECIES_WHISMUR = 293,
    SPECIES_LOUDRED = 294,
    SPECIES_EXPLOUD = 295,
    SPECIES_MAKUHITA = 296,
    SPECIES_HARIYAMA = 297,
    SPECIES_AZURILL = 298,
    SPECIES_NOSEPASS = 299,
    SPECIES_SKITTY = 300,
    SPECIES_DELCATTY = 301,
    SPECIES_SABLEYE = 302,
    SPECIES_MAWILE = 303,
    SPECIES_ARON = 304,
    SPECIES_LAIRON = 305,
    SPECIES_AGGRON = 306,
    SPECIES_MEDITITE = 307,
    SPECIES_MEDICHAM = 308,
    SPECIES_ELECTRIKE = 309,
    SPECIES_MANECTRIC = 310,
    SPECIES_PLUSLE = 311,
    SPECIES_MINUN = 312,
    SPECIES_VOLBEAT = 313,
    SPECIES_ILLUMISE = 314,
    SPECIES_ROSELIA = 315,
    SPECIES_GULPIN = 316,
    SPECIES_SWALOT = 317,
    SPECIES_CARVANHA = 318,
    SPECIES_SHARPEDO = 319,
    SPECIES_WAILMER = 320,
    SPECIES_WAILORD = 321,
    SPECIES_NUMEL = 322,
    SPECIES_CAMERUPT = 323,
    SPECIES_TORKOAL = 324,
    SPECIES_SPOINK = 325,
    SPECIES_GRUMPIG = 326,
    SPECIES_SPINDA = 327,
    SPECIES_TRAPINCH = 328,
    SPECIES_VIBRAVA = 329,
    SPECIES_FLYGON = 330,
    SPECIES_CACNEA = 331,
    SPECIES_CACTURNE = 332,
    SPECIES_SWABLU = 333,
    SPECIES_ALTARIA = 334,
    SPECIES_ZANGOOSE = 335,
    SPECIES_SEVIPER = 336,
    SPECIES_LUNATONE = 337,
    SPECIES_SOLROCK = 338,
    SPECIES_BARBOACH = 339,
    SPECIES_WHISCASH = 340,
    SPECIES_CORPHISH = 341,
    SPECIES_CRAWDAUNT = 342,
    SPECIES_BALTOY = 343,
    SPECIES_CLAYDOL = 344,
    SPECIES_LILEEP = 345,
    SPECIES_CRADILY = 346,
    SPECIES_ANORITH = 347,
    SPECIES_ARMALDO = 348,
    SPECIES_FEEBAS = 349,
    SPECIES_MILOTIC = 350,
    SPECIES_CASTFORM = 351,
    SPECIES_KECLEON = 352,
    SPECIES_SHUPPET = 353,
    SPECIES_BANETTE = 354,
    SPECIES_DUSKULL = 355,
    SPECIES_DUSCLOPS = 356,
    SPECIES_TROPIUS = 357,
    SPECIES_CHIMECHO = 358,
    SPECIES_ABSOL = 359,
    SPECIES_WYNAUT = 360,
    SPECIES_SNORUNT = 361,
    SPECIES_GLALIE = 362,
    SPECIES_SPHEAL = 363,
    SPECIES_SEALEO = 364,
    SPECIES_WALREIN = 365,
    SPECIES_CLAMPERL = 366,
    SPECIES_HUNTAIL = 367,
    SPECIES_GOREBYSS = 368,
    SPECIES_RELICANTH = 369,
    SPECIES_LUVDISC = 370,
    SPECIES_BAGON = 371,
    SPECIES_SHELGON = 372,
    SPECIES_SALAMENCE = 373,
    SPECIES_BELDUM = 374,
    SPECIES_METANG = 375,
    SPECIES_METAGROSS = 376,
    SPECIES_REGIROCK = 377,
    SPECIES_REGICE = 378,
    SPECIES_REGISTEEL = 379,
    SPECIES_LATIAS = 380,
    SPECIES_LATIOS = 381,
    SPECIES_KYOGRE = 382,
    SPECIES_GROUDON = 383,
    SPECIES_RAYQUAZA = 384,
    SPECIES_JIRACHI = 385,
    SPECIES_DEOXYS = 386,
    SPECIES_TURTWIG = 387,
    SPECIES_GROTLE = 388,
    SPECIES_TORTERRA = 389,
    SPECIES_CHIMCHAR = 390,
    SPECIES_MONFERNO = 391,
    SPECIES_INFERNAPE = 392,
    SPECIES_PIPLUP = 393,
    SPECIES_PRINPLUP = 394,
    SPECIES_EMPOLEON = 395,
    SPECIES_STARLY = 396,
    SPECIES_STARAVIA = 397,
    SPECIES_STARAPTOR = 398,
    SPECIES_BIDOOF = 399,
    SPECIES_BIBAREL = 400,
    SPECIES_KRICKETOT = 401,
    SPECIES_KRICKETUNE = 402,
    SPECIES_SHINX = 403,
    SPECIES_LUXIO = 404,
    SPECIES_LUXRAY = 405,
    SPECIES_BUDEW = 406,
    SPECIES_ROSERADE = 407,
    SPECIES_CRANIDOS = 408,
    SPECIES_RAMPARDOS = 409,
    SPECIES_SHIELDON = 410,
    SPECIES_BASTIODON = 411,
    SPECIES_BURMY = 412,
    SPECIES_WORMADAM = 413,
    SPECIES_MOTHIM = 414,
    SPECIES_COMBEE = 415,
    SPECIES_VESPIQUEN = 416,
    SPECIES_PACHIRISU = 417,
    SPECIES_BUIZEL = 418,
    SPECIES_FLOATZEL = 419,
    SPECIES_CHERUBI = 420,
    SPECIES_CHERRIM = 421,
    SPECIES_SHELLOS = 422,
    SPECIES_GASTRODON = 423,
    SPECIES_AMBIPOM = 424,
    SPECIES_DRIFLOON = 425,
    SPECIES_DRIFBLIM = 426,
    SPECIES_BUNEARY = 427,
    SPECIES_LOPUNNY = 428,
    SPECIES_MISMAGIUS = 429,
    SPECIES_HONCHKROW = 430,
    SPECIES_GLAMEOW = 431,
    SPECIES_PURUGLY = 432,
    SPECIES_CHINGLING = 433,
    SPECIES_STUNKY = 434,
    SPECIES_SKUNTANK = 435,
    SPECIES_BRONZOR = 436,
    SPECIES_BRONZONG = 437,
    SPECIES_BONSLY = 438,
    SPECIES_MIME_JR = 439,
    SPECIES_HAPPINY = 440,
    SPECIES_CHATOT = 441,
    SPECIES_SPIRITOMB = 442,
    SPECIES_GIBLE = 443,
    SPECIES_GABITE = 444,
    SPECIES_GARCHOMP = 445,
    SPECIES_MUNCHLAX = 446,
    SPECIES_RIOLU = 447,
    SPECIES_LUCARIO = 448,
    SPECIES_HIPPOPOTAS = 449,
    SPECIES_HIPPOWDON = 450,
    SPECIES_SKORUPI = 451,
    SPECIES_DRAPION = 452,
    SPECIES_CROAGUNK = 453,
    SPECIES_TOXICROAK = 454,
    SPECIES_CARNIVINE = 455,
    SPECIES_FINNEON = 456,
    SPECIES_LUMINEON = 457,
    SPECIES_MANTYKE = 458,
    SPECIES_SNOVER = 459,
    SPECIES_ABOMASNOW = 460,
    SPECIES_WEAVILE = 461,
    SPECIES_MAGNEZONE = 462,
    SPECIES_LICKILICKY = 463,
    SPECIES_RHYPERIOR = 464,
    SPECIES_TANGROWTH = 465,
    SPECIES_ELECTIVIRE = 466,
    SPECIES_MAGMORTAR = 467,
    SPECIES_TOGEKISS = 468,
    SPECIES_YANMEGA = 469,
    SPECIES_LEAFEON = 470,
    SPECIES_GLACEON = 471,
    SPECIES_GLISCOR = 472,
    SPECIES_MAMOSWINE = 473,
    SPECIES_PORYGON_Z = 474,
    SPECIES_GALLADE = 475,
    SPECIES_PROBOPASS = 476,
    SPECIES_DUSKNOIR = 477,
    SPECIES_FROSLASS = 478,
    SPECIES_ROTOM = 479,
    SPECIES_UXIE = 480,
    SPECIES_MESPRIT = 481,
    SPECIES_AZELF = 482,
    SPECIES_DIALGA = 483,
    SPECIES_PALKIA = 484,
    SPECIES_HEATRAN = 485,
    SPECIES_REGIGIGAS = 486,
    SPECIES_GIRATINA = 487,
    SPECIES_CRESSELIA = 488,
    SPECIES_PHIONE = 489,
    SPECIES_MANAPHY = 490,
    SPECIES_DARKRAI = 491,
    SPECIES_SHAYMIN = 492,
    SPECIES_ARCEUS = 493,
    SPECIES_EGG = 494,
    SPECIES_BAD_EGG = 495,
};

typedef struct ChatotCry {
    BOOL valid;
    s8 audioData[1000];
} ChatotCry;

void DoScheduledBgGpuUpdates(BgConfig *bgConfig);

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

enum NarcID {
    NARC_INDEX_BATTLE__SKILL__WAZA_SEQ = 0,
    NARC_INDEX_BATTLE__SKILL__SUB_SEQ,
    NARC_INDEX_POKETOOL__PERSONAL__PL_PERSONAL,
    NARC_INDEX_POKETOOL__PERSONAL__PL_GROWTBL,
    NARC_INDEX_POKETOOL__POKEGRA__PL_POKEGRA,
    NARC_INDEX_POKETOOL__POKEGRA__HEIGHT,
    NARC_INDEX_POKETOOL__TRGRA__TRBGRA,
    NARC_INDEX_BATTLE__GRAPHIC__PL_BATT_BG,
    NARC_INDEX_WAZAEFFECT__WE,
    NARC_INDEX_POKETOOL__WAZA__PL_WAZA_TBL,
    NARC_INDEX_FIELDDATA__SCRIPT__SCR_SEQ,
    NARC_INDEX_MSGDATA__SCENARIO__SCR_MSG,
    NARC_INDEX_GRAPHIC__POKETCH,
    NARC_INDEX_GRAPHIC__PL_BAG_GRA,
    NARC_INDEX_GRAPHIC__PL_FONT,
    NARC_INDEX_ITEMTOOL__ITEMDATA__PL_ITEM_DATA,
    NARC_INDEX_ITEMTOOL__ITEMDATA__ITEM_ICON,
    NARC_INDEX_GRAPHIC__TMAP_GRA,
    NARC_INDEX_GRAPHIC__BOX,
    NARC_INDEX_POKETOOL__ICONGRA__PL_POKE_ICON,
    NARC_INDEX_GRAPHIC__PL_PLIST_GRA,
    NARC_INDEX_WAZAEFFECT__EFFECTCLACT__WECHAR,
    NARC_INDEX_WAZAEFFECT__EFFECTCLACT__WEPLTT,
    NARC_INDEX_WAZAEFFECT__EFFECTCLACT__WECELL,
    NARC_INDEX_WAZAEFFECT__EFFECTCLACT__WECELLANM,
    NARC_INDEX_GRAPHIC__IMAGECLIP,
    NARC_INDEX_MSGDATA__PL_MSG,
    NARC_INDEX_BATTLE__GRAPHIC__PL_BATT_OBJ,
    NARC_INDEX_DATA__UNDERG_RADAR,
    NARC_INDEX_WAZAEFFECT__EFFECTDATA__WAZA_PARTICLE,
    NARC_INDEX_BATTLE__SKILL__BE_SEQ,
    NARC_INDEX_DATA__NAMEIN,
    NARC_INDEX_FIELDDATA__EVENTDATA__ZONE_EVENT,
    NARC_INDEX_POKETOOL__PERSONAL__WOTBL,
    NARC_INDEX_POKETOOL__PERSONAL__EVO,
    NARC_INDEX_GRAPHIC__FONTOAM,
    NARC_INDEX_GRAPHIC__FIELD_BOARD,
    NARC_INDEX_FIELDDATA__ENCOUNTDATA__D_ENC_DATA,
    NARC_INDEX_GRAPHIC__PL_WINFRAME,
    NARC_INDEX_GRAPHIC__PL_PST_GRA,
    NARC_INDEX_FIELDDATA__BUILD_MODEL__BUILD_MODEL,
    NARC_INDEX_FIELDDATA__MAPMATRIX__MAP_MATRIX,
    NARC_INDEX_FIELDDATA__AREADATA__AREA_DATA,
    NARC_INDEX_FIELDDATA__AREADATA__AREA_BUILD_MODEL__AREA_BUILD,
    NARC_INDEX_FIELDDATA__AREADATA__AREA_MAP_TEX__MAP_TEX_SET,
    NARC_INDEX_CONTEST__GRAPHIC__CONTEST_BG,
    NARC_INDEX_CONTEST__GRAPHIC__CONTEST_OBJ,
    NARC_INDEX_DATA__UGEFFECT_OBJ_GRAPHIC,
    NARC_INDEX_DEMO__TITLE__TITLEDEMO,
    NARC_INDEX_FIELDDATA__AREADATA__AREA_MOVE_MODEL__MOVE_MODEL_LIST,
    NARC_INDEX_DATA__UG_TRAP,
    NARC_INDEX_GRAPHIC__TRAINER_CASE,
    NARC_INDEX_DATA__UG_FOSSIL,
    NARC_INDEX_GRAPHIC__NTAG_GRA,
    NARC_INDEX_DATA__TRADELIST,
    NARC_INDEX_DATA__UG_PARTS,
    NARC_INDEX_GRAPHIC__OPENING,
    NARC_INDEX_POKETOOL__TRAINER__TRDATA,
    NARC_INDEX_POKETOOL__TRAINER__TRPOKE,
    NARC_INDEX_POKETOOL__TRMSG__TRTBL,
    NARC_INDEX_POKETOOL__TRGRA__TRFGRA,
    NARC_INDEX_PARTICLEDATA__PARTICLEDATA,
    NARC_INDEX_GRAPHIC__SHOP_GRA,
    NARC_INDEX_WAZAEFFECT__WE_SUB,
    NARC_INDEX_DATA__UG_ANIM,
    NARC_INDEX_DATA__WEATHER_SYS,
    NARC_INDEX_GRAPHIC__MENU_GRA,
    NARC_INDEX_FIELDDATA__LAND_DATA__LAND_DATA,
    NARC_INDEX_ITEMTOOL__ITEMDATA__NUTS_DATA,
    NARC_INDEX_RESOURCE__ENG__ZUKAN__ZUKAN,
    NARC_INDEX_POKETOOL__POKEFOOT__POKEFOOT,
    NARC_INDEX_FIELDDATA__AREADATA__AREA_BUILD_MODEL__AREABM_TEXSET,
    NARC_INDEX_BATTLE__GRAPHIC__PL_B_PLIST_GRA,
    NARC_INDEX_GRAPHIC__CONFIG_GRA,
    NARC_INDEX_GRAPHIC__UNIONROOM,
    NARC_INDEX_APPLICATION__ZUKANLIST__ZKN_DATA__ZUKAN_DATA,
    NARC_INDEX_GRAPHIC__PMSI,
    NARC_INDEX_BATTLE__GRAPHIC__B_BAG_GRA,
    NARC_INDEX_GRAPHIC__OEKAKI,
    NARC_INDEX_GRAPHIC__MAIL_GRA,
    NARC_INDEX_GRAPHIC__F_NOTE_GRA,
    NARC_INDEX_DATA__MMODEL__MMODEL,
    NARC_INDEX_GRAPHIC__EV_POKESELECT,
    NARC_INDEX_GRAPHIC__PORU_GRA,
    NARC_INDEX_GRAPHIC__TOUCH_SUBWINDOW,
    NARC_INDEX_GRAPHIC__FLD_COMACT,
    NARC_INDEX_GRAPHIC__RECORD,
    NARC_INDEX_GRAPHIC__PORUDEMO,
    NARC_INDEX_GRAPHIC__PORUACT,
    NARC_INDEX_GRAPHIC__PL_WIFINOTE,
    NARC_INDEX_CONTEST__DATA__CONTEST_DATA,
    NARC_INDEX_APPLICATION__CUSTOM_BALL__DATA__CB_DATA,
    NARC_INDEX_DATA__WIFIP2PMATCH,
    NARC_INDEX_GRAPHIC__DEMO_TRADE,
    NARC_INDEX_POKEANIME__PL_POKE_ANM,
    NARC_INDEX_GRAPHIC__MYSIGN,
    NARC_INDEX_FIELDDATA__MM_LIST__MOVE_MODEL_LIST,
    NARC_INDEX_DATA__FIELD_CUTIN,
    NARC_INDEX_DATA__DEMO_CLIMAX,
    NARC_INDEX_WAZAEFFECT__EFFECTDATA__BALL_PARTICLE,
    NARC_INDEX_WAZAEFFECT__PT_DEBUG__DEBUG_PARTICLE,
    NARC_INDEX_GRAPHIC__DENDOU_PC,
    NARC_INDEX_APPLICATION__WIFI_EARTH__WIFI_EARTH_PLACE,
    NARC_INDEX_BATTLE__TR_AI__TR_AI_SEQ,
    NARC_INDEX_GRAPHIC__WORLDTRADE,
    NARC_INDEX_GRAPHIC__DENDOU_DEMO,
    NARC_INDEX_ARC__ENCDATA_EX,
    NARC_INDEX_DATA__MMODEL__FLDEFF,
    NARC_INDEX_BATTLE__GRAPHIC__VS_DEMO_GRA,
    NARC_INDEX_POKETOOL__PL_POKEZUKAN,
    NARC_INDEX_ARC__BM_ANIME,
    NARC_INDEX_ARC__BM_ANIME_LIST,
    NARC_INDEX_GRAPHIC__FIELD_ENCOUNTEFFECT,
    NARC_INDEX_GRAPHIC__WAZA_OSHIE_GRA,
    NARC_INDEX_POKETOOL__POKEANM__PL_POKEANM,
    NARC_INDEX_FIELDDATA__POKEMON_TRADE__FLD_TRADE,
    NARC_INDEX_GRAPHIC__MYSTERY,
    NARC_INDEX_POKETOOL__POKEGRA__PL_OTHERPOKE,
    NARC_INDEX_DEMO__EGG__DATA__EGG_DATA,
    NARC_INDEX_DEMO__EGG__DATA__PARTICLE__EGG_DEMO_PARTICLE,
    NARC_INDEX_ARC__SHIP_DEMO,
    NARC_INDEX_POKETOOL__POKEGRA__HEIGHT_O,
    NARC_INDEX_GRAPHIC__NUTMIXER,
    NARC_INDEX_GRAPHIC__BTOWER,
    NARC_INDEX_DEMO__SHINKA__DATA__PARTICLE__SHINKA_DEMO_PARTICLE,
    NARC_INDEX_DEMO__INTRO__INTRO,
    NARC_INDEX_ARC__TV,
    NARC_INDEX_GRAPHIC__ENDING,
    NARC_INDEX_DEMO__TITLE__OP_DEMO,
    NARC_INDEX_APPLICATION__WIFI_EARTH__WIFI_EARTH,
    NARC_INDEX_GRAPHIC__RANKING,
    NARC_INDEX_ARC__PPARK,
    NARC_INDEX_DEMO__SYOUJYOU__SYOUJYOU,
    NARC_INDEX_DATA__SLOT,
    NARC_INDEX_BATTLE__B_TOWER__BTDTR,
    NARC_INDEX_BATTLE__B_TOWER__BTDPM,
    NARC_INDEX_DEMO__INTRO__INTRO_TV,
    NARC_INDEX_POKETOOL__TRMSG__TRTBLOFS,
    NARC_INDEX_POKETOOL__POKEGRA__POKE_YOFS,
    NARC_INDEX_APPLICATION__ZUKANLIST__ZKN_DATA__ZUKAN_ENC_DIAMOND,
    NARC_INDEX_APPLICATION__ZUKANLIST__ZKN_DATA__ZUKAN_ENC_PEARL,
    NARC_INDEX_GRAPHIC__HIDEN_EFFECT,
    NARC_INDEX_POKETOOL__POKEGRA__POKE_SHADOW,
    NARC_INDEX_FIELDDATA__ENCOUNTDATA__P_ENC_DATA,
    NARC_INDEX_POKETOOL__POKEGRA__POKE_SHADOW_OFX,
    NARC_INDEX_POKETOOL__SHINZUKAN,
    NARC_INDEX_GRAPHIC__WIFI2DCHAR,
    NARC_INDEX_POKETOOL__POKE_EDIT__PL_POKE_DATA,
    NARC_INDEX_DEBUG__CB_EDIT__D_TEST,
    NARC_INDEX_FRONTIER__SCRIPT__FR_SCRIPT,
    NARC_INDEX_FRONTIER_BACKGROUNDS,
    NARC_INDEX_RESOURCE__ENG__FRONTIER_GRAPHIC__FRONTIER_OBJ,
    NARC_INDEX_APPLICATION__CUSTOM_BALL__EDIT__PL_CB_DATA,
    NARC_INDEX_GRAPHIC__WIFI_UNIONOBJ,
    NARC_INDEX_DATA__GURU2,
    NARC_INDEX_DATA__FLDTANIME,
    NARC_INDEX_ARC__SHIP_DEMO_PL,
    NARC_INDEX_PARTICLEDATA__PL_FRONTIER__FRONTIER_PARTICLE,
    NARC_INDEX_GRAPHIC__BUCKET,
    NARC_INDEX_ARC__CODEIN_GRA,
    NARC_INDEX_ARC__AREA_WIN_GRA,
    NARC_INDEX_FIELDDATA__ENCOUNTDATA__PL_ENC_DATA,
    NARC_INDEX_RESOURCE__ENG__BATT_REC__BATT_REC_GRA,
    NARC_INDEX_GRAPHIC__WORLDTIMER,
    NARC_INDEX_ARC__MANENE,
    NARC_INDEX_POKETOOL__POKEGRA__POKEGRA,
    NARC_INDEX_POKETOOL__POKEGRA__OTHERPOKE,
    NARC_INDEX_POKETOOL__POKEGRA__DP_HEIGHT,
    NARC_INDEX_POKETOOL__POKEGRA__DP_HEIGHT_O,
    NARC_INDEX_GRAPHIC__LOBBY_NEWS,
    NARC_INDEX_ARC__EMAIL_GRA,
    NARC_INDEX_RESOURCE__ENG__WIFI_LOBBY_MINIGAME__WLMNGM_TOOL,
    NARC_INDEX_ARC__DEMO_TENGAN_GRA,
    NARC_INDEX_APPLICATION__BALLOON__GRAPHIC__BALLOON_GRA,
    NARC_INDEX_APPLICATION__WIFI_LOBBY__MAP_CONV__WFLBY_MAP,
    NARC_INDEX_GRAPHIC__WIFI_LOBBY,
    NARC_INDEX_DATA__AREALIGHT,
    NARC_INDEX_DATA__UGROUNDEFFECT,
    NARC_INDEX_BATTLE__B_PL_TOWER__PL_BTDTR,
    NARC_INDEX_BATTLE__B_PL_TOWER__PL_BTDPM,
    NARC_INDEX_BATTLE__B_PL_STAGE__PL_BSDPM,
    NARC_INDEX_FIELDDATA__TORNWORLD__TW_ARC,
    NARC_INDEX_FIELDDATA__TORNWORLD__TW_ARC_ATTR,
    NARC_INDEX_DATA__TW_ARC_ETC,
    NARC_INDEX_SCRATCH_OFF_CARDS,
    NARC_INDEX_PARTICLEDATA__PL_POKELIST__POKELIST_PARTICLE,
    NARC_INDEX_GRAPHIC__WIFI_LOBBY_OTHER,
    NARC_INDEX_GRAPHIC__FOOTPRINT_BOARD,
    NARC_INDEX_APPLICATION__ZUKANLIST__ZKN_DATA__ZUKAN_ENC_PLATINUM,
    NARC_INDEX_APPLICATION__BUCKET__BALLSLOW_DATA,
    NARC_INDEX_PARTICLEDATA__PL_ETC__PL_ETC_PARTICLE,
    NARC_INDEX_RESOURCE__ENG__PMS_AIKOTOBA__PMS_AIKOTOBA,
    NARC_INDEX_GRAPHIC__UNIONOBJ2D_ONLYFRONT,
    NARC_INDEX_GRAPHIC__LIBRARY_TV,
    NARC_INDEX_APPLICATION__ZUKANLIST__ZKN_DATA__ZUKAN_DATA_GIRA,
};

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef struct SysTaskManager SysTaskManager;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
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

void OamManager_ApplyAndResetBuffers(void);

typedef struct Resource {
    u8 unk_0[36];
    int id;
    void *data;
} Resource;

typedef struct ResourceCollection {
    Resource *resources;
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

typedef struct TrainerInfo {
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
} TrainerInfo;

typedef struct TrainerHeader {
    u8 monDataType;
    u8 trainerType;
    u8 sprite;
    u8 partySize;
    u16 items[4];
    u32 aiMask;
    u32 battleType;
} TrainerHeader;

typedef struct Trainer {
    TrainerHeader header;
    charcode_t name[7 + 1];
    EasyChatSentence winMsg;
    EasyChatSentence loseMsg;
} Trainer;

typedef struct PCBoxes {
    u32 currentBoxID;
    BoxPokemon boxMons[18][(5 * 6)];
    u16 names[18][20];
    u8 wallpapers[18];
    u8 unlockedWallpapers;
} PCBoxes;

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

enum PaletteLoadLocation {
    PAL_LOAD_MAIN_BG = 0,
    PAL_LOAD_MAIN_OBJ,
    PAL_LOAD_MAIN_BGEXT,
    PAL_LOAD_MAIN_OBJEXT,
    PAL_LOAD_SUB_BG,
    PAL_LOAD_SUB_OBJ,
    PAL_LOAD_SUB_BGEXT,
    PAL_LOAD_SUB_OBJEXT,
};

void GfGfxLoader_GXLoadPal(enum NarcID narcID, u32 narcMemberIdx, enum PaletteLoadLocation loadLocation, u32 palOffset, u32 size, enum HeapID heapID);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

typedef u32 TextColor;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_02095E80_sub1;

typedef struct WiFiHistory {
    u32 unused_00;
    u8 hasInteractedOutsideOfJapan;
    u8 country;
    u8 region;
    u8 geonetCommunicationMap[4080];
} WiFiHistory;

void Heap_Free(void *ptr);

typedef struct Pokedex {
    u32 magic;
    u32 caughtSpecies[((int)(((SPECIES_BAD_EGG - 2) - 1) / 32) + 1)];
    u32 seenSpecies[((int)(((SPECIES_BAD_EGG - 2) - 1) / 32) + 1)];
    u32 seenGenders[2][((int)(((SPECIES_BAD_EGG - 2) - 1) / 32) + 1)];
    u32 spindaPersonality;
    u8 shellosFormOrder;
    u8 gastrodonFormOrder;
    u8 burmyFormOrder;
    u8 wormadamFormOrder;
    u8 unownSeenOrder[28];
    u8 unownCaughtOrder[28];
    u8 caughtLanguages[496];
    u8 canDetectForms;
    u8 enabledInternational;
    u8 dexEnabled;
    u8 nationalDex;
    u32 rotomFormOrder;
    u8 shayminFormOrder;
    u8 giratinaFormOrder;
    u8 pichuFormOrder;
    u8 dummy;
} Pokedex;

typedef struct EncodingSeed {
    u16 byteSum;
    u16 modifier;
} EncodingSeed;

typedef struct GameRecords {
    u32 recordsU32[71];
    u16 recordsU16[77];
    u16 padding;
    EncodingSeed seed;
} GameRecords;

typedef struct PlayTime {
    u16 hours;
    u8 minutes;
    u8 seconds;
} PlayTime;

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

typedef struct ColoredArrow {
    TextColor color;
    String *string;
} ColoredArrow;

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
    u8 cursorPal : 4;
    u8 fillValue : 4;
    u8 cursorShadowPal : 4;
    u8 unk_21_4 : 4;
    u8 lettersSpacing : 6;
    u8 unk_23;
    u8 fontId : 7;
    u8 overrideEnabled : 1;
    struct ListMenuCursor *cursor;
    u16 cursorPos;
    u16 itemsAbove;
    u8 unk_30;
    u8 unk_31;
    u8 taskId;
    u8 unk_33;
    u8 heapID;
};

typedef struct MenuTemplate {
    StringList *choices;
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

typedef struct PalPad {
    u16 trainerName[7 + 1];
    u32 trainerId;
    u8 language;
    u8 gameCode;
    u8 gender;
    u8 padding_17;

    u32 associatedTrainerIds[16];
    u8 associatedTrainerGameCodes[16];
    u8 associatedTrainerLanguages[16];
    u8 associatedTrainerGenders[16];
} PalPad;

typedef struct TradeRoomMonDisplayData {
    u16 species;
    u16 heldItemType;
    u8 pokeBall;
    u8 isEgg;
    u8 form;
    u16 gender;
    u16 iconShouldFlip;
    int ballCapsuleId;
} TradeRoomMonDisplayData;

typedef struct TradeRoomArgs {
    FieldSystem *fieldSystem;
    TrainerInfo *trainerInfo;
    Party *party;
    PalPad *palPad;
    SaveData *saveData;
    WiFiHistory *wiFiHistory;
    Options *options;
    JournalEntry *journalEntry;
    GameRecords *records;
    Pokedex *pokedex;
    int tradeCompleted;
    int receivingPartySlot;
    int dexMode;
    int tradeCount;
    TrainerInfo *partnerTrainerInfoCopy;
    Pokemon *sendingMon;
    Pokemon *receivingMon;
} TradeRoomArgs;

struct TradeRoom {
    FieldSystem *fieldSystem;
    SaveData *saveData;
    TradeRoomArgs *args;
    PokemonSummaryArgs summaryArgs;
    int summarySide;
    ApplicationManager *appMan;
    int subAppActive;
    int phase;
    int connectStep;
    int partySendCount;
    int commMilestone;
    int partyReceiveCount;
    int didConfirmTrade;
    int partnerSyncStatus[2];
    int notificationMsgId;
    BOOL unk_6C;
    UnkStruct_02095E80_sub1 unk_70;
    int selectedSlot[2];
    u16 cursorGlowAngle;
    u16 unk_92[5][17];
    u16 unk_13C;
    u16 unk_13E;
    int subStepResult;
    u16 unk_144[3];
    u32 pendingDirection[2];
    u32 unk_154[2][3];
    int unk_16C;
    int unk_170;
    BgConfig *bgConfig;
    StringTemplate *strTemplate;
    StringTemplate *strTemplate2;
    StringTemplate *unread_180;
    struct MsgData *msgLoader;
    String *unk_188;
    String *playerNameStr;
    String *itemLabelStr;
    SpriteList *spriteList;
    G2dRenderer spriteRenderer;
    SpriteResourceCollection *spriteResourceCollections[4];
    SpriteResource *spriteResources[2][4];
    SpriteResourcesHeader mainSpriteResourcesHeader;
    SpriteResourcesHeader subSpriteResourcesHeader;
    Sprite *cursorSprites[14];
    Sprite *iconSprites[12];
    Sprite *itemIconSprites[12];
    Sprite *capsuleIconSprites[12];
    Sprite *detailCardSprites[6];
    Sprite *convergeIndicatorSprite;
    SysTask *unk_480[7];
    Window windows[34];
    Menu *yesNoMenu;
    StringList *actionMenuChoices;
    Menu *actionMenu;
    int yesNoMenuStep;
    int unk_6CC;
    int unk_6D0;
    int unk_6D4;
    VecFx32 unk_6D8[2];
    int unk_6F0;
    TradeRoomMonDisplayData monDisplayData[13];
    int unk_7B8;
    int unk_7BC;
    int unk_7C0;
    int unk_7C4[7];
    void *unk_7E0;
    NNSG2dCharacterData *unk_7E4;
    void *iconGraphicsBuffers[12];
    NNSG2dCharacterData *iconCharacterData[12];
    u8 previewSpritePixelBuf[2][3200];
    PokemonSpriteTemplate previewSpriteTemplate[2];
    int pendingPreviewLoadSide;
    u8 unk_216C[256];
    int (*subStepCallback)(TradeRoom *);
    Party *playerParty;
    Party *partnerParty;
    ChatotCry *unk_2278;
    PalPad *palPad;
    PalPad palPadNetworkObject;
    int unk_2308;
    int unk_230C;
    int bgScrollOffset;
    int slideAnimFrame;
    int staggerCountdown;
    VecFx32 playerSlidePath[2];
    VecFx32 partnerSlidePath[2];
    u8 partyTransferBuffer[2][1424];
    u8 chatotCryBuffer[2][1004];
    int pendingRegistrationFlags[32];
    int registrationTargetNetId;
    int friendToDeleteSlot;
    StringTemplate *palPadStrTemplate;
    struct MsgData *palPadMsgLoader;
    StringList *friendListChoices;
    ListMenu *friendListMenu;
    Window friendListWindow;
    WiFiList *wifiList;
    int tradeCompletedCount;
    int syncSaveState;
    int lastSyncedValue;
    int lastSyncedCmd;
    void *waitDialHandle;
    int connectionTimeoutArmed;
    int connectionTimeoutFrames;
};

enum ItemBattleCategory {
    ITEM_BATTLE_CATEGORY_RECOVER_HP = 0,
    ITEM_BATTLE_CATEGORY_RECOVER_STATUS = 1,
    ITEM_BATTLE_CATEGORY_POKE_BALLS = 2,
    ITEM_BATTLE_CATEGORY_BATTLE_ITEMS = 3,
    ITEM_BATTLE_CATEGORY_MAX = 4,
};

typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;

typedef struct FieldBagCursor {
    u8 scroll[8];
    u8 index[8];
    u16 pocket;
    u16 dummy12;
} FieldBagCursor;

typedef struct BattleBagCursor {
    u8 scroll[ITEM_BATTLE_CATEGORY_MAX + 1];
    u8 index[ITEM_BATTLE_CATEGORY_MAX + 1];
    u16 lastUsedItemID;
    u16 lastUsedCategory;
    u16 currentCategory;
} BattleBagCursor;

enum BattleBackground {
    BACKGROUND_PLAIN = 0,
    BACKGROUND_WATER,
    BACKGROUND_CITY,
    BACKGROUND_FOREST,
    BACKGROUND_MOUNTAIN,
    BACKGROUND_SNOW,
    BACKGROUND_INDOORS_1,
    BACKGROUND_INDOORS_2,
    BACKGROUND_INDOORS_3,
    BACKGROUND_CAVE_1,
    BACKGROUND_CAVE_2,
    BACKGROUND_CAVE_3,
    BACKGROUND_AARON,
    BACKGROUND_BERTHA,
    BACKGROUND_FLINT,
    BACKGROUND_LUCIAN,
    BACKGROUND_CYNTHIA,
    BACKGROUND_DISTORTION_WORLD,
    BACKGROUND_BATTLE_TOWER,
    BACKGROUND_BATTLE_FACTORY,
    BACKGROUND_BATTLE_ARCADE,
    BACKGROUND_BATTLE_CASTLE,
    BACKGROUND_BATTLE_HALL,
};

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

enum EvolutionMethod {
    EVO_NONE = 0,
    EVO_LEVEL_HAPPINESS = 1,
    EVO_LEVEL_HAPPINESS_DAY = 2,
    EVO_LEVEL_HAPPINESS_NIGHT = 3,
    EVO_LEVEL = 4,
    EVO_TRADE = 5,
    EVO_TRADE_WITH_HELD_ITEM = 6,
    EVO_USE_ITEM = 7,
    EVO_LEVEL_ATK_GT_DEF = 8,
    EVO_LEVEL_ATK_EQ_DEF = 9,
    EVO_LEVEL_ATK_LT_DEF = 10,
    EVO_LEVEL_PID_LOW = 11,
    EVO_LEVEL_PID_HIGH = 12,
    EVO_LEVEL_NINJASK = 13,
    EVO_LEVEL_SHEDINJA = 14,
    EVO_LEVEL_BEAUTY = 15,
    EVO_USE_ITEM_MALE = 16,
    EVO_USE_ITEM_FEMALE = 17,
    EVO_LEVEL_WITH_HELD_ITEM_DAY = 18,
    EVO_LEVEL_WITH_HELD_ITEM_NIGHT = 19,
    EVO_LEVEL_KNOW_MOVE = 20,
    EVO_LEVEL_SPECIES_IN_PARTY = 21,
    EVO_LEVEL_MALE = 22,
    EVO_LEVEL_FEMALE = 23,
    EVO_LEVEL_MAGNETIC_FIELD = 24,
    EVO_LEVEL_MOSS_ROCK = 25,
    EVO_LEVEL_ICE_ROCK = 26,
};

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

enum TimeOfDay {
    TIMEOFDAY_MORNING = 0,
    TIMEOFDAY_DAY = 1,
    TIMEOFDAY_TWILIGHT = 2,
    TIMEOFDAY_NIGHT = 3,
    TIMEOFDAY_LATE_NIGHT = 4,
};

typedef struct BattleRecords {
    int totalTurns;
    int totalFainted;
    int totalDamage;
} BattleRecords;

typedef struct CaptureAttempt {
    int resultMask;
    u16 ballsThrown;
    u16 species;
    u8 gender;
    u8 language;
    u8 metGame;
    u8 hasNickname;
    u16 nickname[11];
    u16 pokeballItemID;
} CaptureAttempt;

typedef struct BattleRegulation {
    u16 name[12];
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

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

enum FaceDirection {
    FACE_NONE = -1,
    FACE_UP = 0,
    FACE_DOWN,
    FACE_LEFT,
    FACE_RIGHT,
};

static inline void Location_Set(Location *location, enum MapHeaderID mapHeaderID, int warpId, int x, int z, int faceDirection) {
    location->mapId = mapHeaderID;
    location->warpId = warpId;
    location->x = x;
    location->y = z;
    location->direction = faceDirection;
}

typedef struct BattleTower {
    int sentinel;
    enum HeapID heapID;
    u32 rng;
    u8 nextOpponentNum;
    u8 currentBattle;
    u8 partySize;
    u8 challengeMode;
    u8 defeatedSevenTrainers : 1;
    u8 beatPalmer : 2;
    u8 isRetiring : 1;
    u8 ribbonsEarned : 1;
    u8 partnerID : 3;
    u8 playerGender;
    u8 partnerGender;
    u16 unk_14;
    u16 unk_16[2];
    u16 startingStreak;
    u16 roomNum;
    u32 victoryRecord;
    u16 unk_24;
    u16 unk_26;
    u16 unk_28;
    u8 partySlots[4];
    u16 selectedSpecies[4];
    u16 selectedItems[4];
    u16 trainerIDs[7 * 2];
    Location unused;
    BattleTowerSave *towerSave;
    WifiBattleTowerRecord *wifiRecord;
    FrontierOpponent opponents[2];
    FrontierOpponent partners[5];
    BattleTowerPartnerData unk_7E8[5];
    u8 partnerConflicts[5];
    u16 commPayload[35];
    u16 commBuffer[35];
    int wonBattle;
    FieldBattleDTO *dto;
    u8 msgsReceived;
    u8 unk_8D5;
    u8 unk_8D6;
    u8 unused2;
    u16 unk_8D8;
    u16 unk_8DA;
} BattleTower;

typedef struct MapLoadMode {
    u32 fieldBottomScreen : 4;
    u32 skipMapAttributes : 4;
    u32 useSimpleTerrainCollisions : 4;
    u32 unk_00_12 : 4;
    u32 useSeparateTerrainAttributes : 4;
    u32 loadExtOverlay : 4;
    u32 separateTerrainAttributesBlockCount : 8;
    u32 unk_04;
} MapLoadMode;

typedef struct MapPropAnimation {
    NNSG3dAnmObj *animationObj;
    BOOL loaded;
    int currentLoop;
    int animeArchiveID;
    BOOL paused;
    void *animationFile;
    int loopCount;
    BOOL looping;
    BOOL reversed;
    int pastoriaGymButtonGroup;
} MapPropAnimation;

typedef struct BicycleSlopeAnimation {
    BOOL loaded;
    NNSG3dRenderObj *renderObj;
    MapPropAnimation *animation;
    int animeArchiveID;
} BicycleSlopeAnimation;

typedef struct MapPropAnimationManager {
    NNSFndAllocator allocator;
    MapPropAnimation animations[16];
    BicycleSlopeAnimation bicycleSlopeAnimations[2];
    NARC *animeNARC;
    NARC *animeListNARC;
} MapPropAnimationManager;

typedef struct MapPropOneShotAnimationList {
    MapPropAnimation *list[4];
    int count;
} MapPropOneShotAnimationList;

typedef struct MapPropOneShotAnimation {
    MapPropOneShotAnimationList animations;
    NNSG3dRenderObj *mapPropRenderObjs[6];
    MapPropAnimation *currentAnimation;
    int mapPropModelID;
    u8 tag;
    u8 unk35;
    u16 unk36;
} MapPropOneShotAnimation;

typedef struct MapPropOneShotAnimationManager {
    struct MapPropOneShotAnimation items[16];
} MapPropOneShotAnimationManager;

typedef struct MapPropMaterialShapeIDsLocator {
    u16 idsCount;
    u16 idsIndex;
} MapPropMaterialShapeIDsLocator;

typedef struct MapPropMaterialShapeIDs {
    u16 materialID;
    u16 shapeID;
} MapPropMaterialShapeIDs;

typedef struct MapPropMaterialShape {
    MapPropMaterialShapeIDsLocator *idsLocators;
    MapPropMaterialShapeIDs *ids;
} MapPropMaterialShape;

typedef struct AreaDataManagerLoadData {
    int areaDataArchiveID;
    MapPropAnimationManager *mapPropAnimMan;
    u8 unk_8[4];
    u16 mapPropModelIDsCount;
    int dummy0C;
} AreaDataManagerLoadData;

typedef struct AreaDataFile {
    u16 mapPropArchivesID;
    u16 mapTextureArchiveID;

    u16 dummy04;
    u16 areaLightArchiveID;
} AreaDataFile;

typedef struct AreaDataManager {
    int dummy00;
    NNSG3dResFileHeader *mapPropModelFiles[768];
    void *mapTextureFile;
    void *mapPropTextureFile;
    NNSG3dResTex *mapTexture;
    NNSG3dResTex *mapPropTexture;
    MapPropMaterialShape *mapPropMatShp;
    AreaDataFile areaData;
    AreaDataManagerLoadData *loadData;

    u16 *mapPropModelIDs;
} AreaDataManager;

typedef struct MapObject MapObject;

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

typedef struct UnkStruct_ov5_021DF8FC_t UnkStruct_ov5_021DF8FC;

typedef struct FieldEffectRenderer {
    u32 id;
    void *context;
} FieldEffectRenderer;

typedef struct FieldEffectManager {
    enum HeapID heapID;
    u32 rendererCount;
    u32 animManCount;
    u32 dummy0C;
    FieldSystem *fieldSystem;
    FieldEffectRenderer *renderers;
    NARC *fldEffNARC;
    OverworldAnimManagerList *animManList;
    UnkStruct_ov5_021DF8FC *unk_20;
} FieldEffectManager;

typedef struct FogManager {
    BOOL enabled;
    GXFogBlend mode;
    GXFogSlope slope;
    int offset;
    GXRgb color;
    int alpha;
    char densityTable[32];
} FogManager;

typedef struct MapProp {
    int modelID;
    BOOL loaded;
    BOOL hidden;
    BOOL applyRotation;
    NNSG3dRenderObj renderObj;
    NNSG3dResMdl *model;
    VecFx32 position;
    VecFx32 rotation;
    VecFx32 scale;
} MapProp;

typedef struct MapPropManager {
    MapProp loadedProps[32];
} MapPropManager;

typedef struct UnkStruct_ov5_021EB0E0 {
    u16 fovY;
    u16 unk_02;
    VecFx32 cameraDistance;
    VecFx32 unk_10;
    fx32 unk_1C;
    MtxFx43 unk_20;
    Camera *camera;
    VecFx32 unk_54;
} UnkStruct_ov5_021EB0E0;

typedef struct Signpost {
    Window window;
    u16 NARCMemberIdx;
    u8 type;
    u8 command : 7;
    u8 active : 1;
} Signpost;

typedef struct UnkStruct_ov56_02256468_t UnkStruct_ov56_02256468;

typedef struct {
    s64 unk_00;
} UnkStruct_ov66_0222DCE0_sub1;

typedef struct UndergroundTopScreenContext_t UndergroundTopScreenContext;

typedef struct FieldTask FieldTask;

typedef BOOL (*FieldTaskFunc)(FieldTask *);

struct FieldTask {
    FieldTask *prev;
    FieldTaskFunc func;
    int state;
    void *env;
    int dummy10;
    void *dummy14;
    FieldSystem *fieldSys;
    int *dummy1C;
};

typedef struct GrassEncounter {
    s8 level;
    u8 padding_01[3];
    int species;
} GrassEncounter;

typedef struct GrassEncounters {
    int encounterRate;
    GrassEncounter encounters[12];
} GrassEncounters;

typedef struct WaterEncounter {
    s8 maxLevel;
    s8 minLevel;
    u8 padding_02[2];
    int species;
} WaterEncounter;

typedef struct WaterEncounters {
    int encounterRate;
    WaterEncounter encounters[5];
} WaterEncounters;

typedef struct WildEncounters {
    GrassEncounters grassEncounters;
    int swarmEncounters[2];
    int dayEncounters[2];
    int nightEncounters[2];
    int radarEncounters[4];
    int encounterRatesForms[5];
    int unownTableID;
    int dualSlotRubyEncounters[2];
    int dualSlotSapphireEncounters[2];
    int dualSlotEmeraldEncouters[2];
    int dualSlotFireredEncounters[2];
    int dualSlotLeafgreenEncounters[2];
    WaterEncounters surfEncounters;
    WaterEncounters unused;
    WaterEncounters oldRodEncounters;
    WaterEncounters goodRodEncounters;
    WaterEncounters superRodEncounters;
} WildEncounters;

typedef struct MapHeaderData {
    u32 numBgEvents;
    u32 numObjectEvents;
    u32 numWarpEvents;
    u32 numCoordEvents;
    const BgEvent *bgEvents;
    const ObjectEvent *objectEvents;
    const WarpEvent *warpEvents;
    const CoordEvent *coordEvents;
    u32 tmpEventsBuf[512];
    u32 initScripts[64];
    WildEncounters wildEncounters;
} MapHeaderData;

typedef struct MapMatrixData {
    u8 height;
    u8 width;
    u16 mapHeaderIDs[(30 * 30)];
    u8 altitudes[(30 * 30)];
    u16 landDataIDs[(30 * 30)];
    char modelNamePrefix[16];
} MapMatrixData;

typedef struct MapMatrix {
    u8 width;
    u8 height;
    u8 matrixID;
    MapMatrixData data;
} MapMatrix;

typedef struct PlayerData {
    u16 cyclingGear;
    u16 runningShoes;
    u32 playerState;
} PlayerData;

typedef struct PokedexMemory {
    u32 currentSpecies;
    u32 bootMode;
} PokedexMemory;

typedef struct RadarChain RadarChain;

typedef struct TerrainAttributes {
    u8 mapMatrixIndexToBlockIndex[225];
    u16 terrainAttributes[(0x800 / sizeof(u16)) * 16];
} TerrainAttributes;

enum Badge {
    BADGE_ID_COAL = 0,
    BADGE_ID_FOREST = 1,
    BADGE_ID_COBBLE = 2,
    BADGE_ID_FEN = 3,
    BADGE_ID_RELIC = 4,
    BADGE_ID_MINE = 5,
    BADGE_ID_ICICLE = 6,
    BADGE_ID_BEACON = 7,
    MAX_BADGES = 8,
};

enum UnionRoomConnectState {
    UR_CONNECT_STATE_NONE = 0,
    UR_CONNECT_STATE_CONNECTED,
    UR_CONNECT_STATE_FAILED,
    UR_CONNECT_STATE_PEER_CONTACTED,
};

enum UnionRoomPendingConnection {
    UR_PENDING_CONNECTION_NONE = 0,
    UR_PENDING_CONNECTION_UNION,
    UR_PENDING_CONNECTION_DRAW,
};

typedef void (*UnionRoomTask)(UnionRoomManager *);

typedef struct HoneyTreeShake {
    u8 shakeValue;
    BOOL isShaking;
} HoneyTreeShake;

typedef struct HoneyTreeShakeList {
    HoneyTreeShake trees[21];
    int unused[21];
} HoneyTreeShakeList;

typedef struct FieldWildBattleMetadata {
    u16 encounterAttempts;
    u16 wildMonDefeated;
} FieldWildBattleMetadata;

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

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

void GF_RunVramTransferTasks(void);

void ov65_0221CE1C(void *param);

void ov65_0221CE1C(void *param) {
    TradeRoom *tradeRoom = (TradeRoom *)param;

    DoScheduledBgGpuUpdates(tradeRoom->bgConfig);

    if (tradeRoom->pendingPreviewLoadSide) {
        int side = tradeRoom->pendingPreviewLoadSide - 1;

        GXS_LoadOBJ(tradeRoom->previewSpritePixelBuf[side], 0 + side * ((8 * (8 / 2)) * 10 * 10), (8 * (8 / 2)) * 10 * 10);
        GfGfxLoader_GXLoadPal((enum NarcID)(tradeRoom->previewSpriteTemplate[side].narcID), tradeRoom->previewSpriteTemplate[side].palette, (enum PaletteLoadLocation)(5), (8 * (8 / 2)) * (2 + side), 32, HEAP_ID_26);

        tradeRoom->pendingPreviewLoadSide = 0;
    }

    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();

    OS_SetIrqCheckFlag(1UL << 0);
}

struct GXBanks;

void GfGfx_SetBanks(const GraphicsBanks *banks);

void ov65_0221CE98(void);

extern const GraphicsBanks ov65_0221FE44;

void ov65_0221CE98(void) {
    GraphicsBanks banks = ov65_0221FE44;

    GfGfx_SetBanks(&banks);
}
