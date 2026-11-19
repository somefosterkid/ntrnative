#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"

struct Background;

struct Bag;

struct BagCursor;

struct BagItem;

struct BallCapsule;

struct BallSeal;

struct BattleBagCursor;

struct BattleRecords;

struct BgConfig;

struct BoxPokemon;

struct Camera;

struct CameraAngle;

struct CameraLookAt;

struct CameraPerspectiveProjection;

struct CameraPositionHistory;

struct CaptureAttempt;

struct ChatotCry;

struct ColoredArrow;

struct EasyChatSentence;

struct EncodingSeed;

struct FieldBagCursor;

struct FieldBattleDTO;

struct FontOAM;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct GameRecords;

struct JournalEntry;

struct JournalEntryMon;

struct JournalEntryTitle;

struct JournalEntryTrainer;

struct Mail;

struct ManagedSprite;

struct Menu;

struct MenuTemplate;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct NARC;

struct Options;

struct PCBoxes;

struct PalPad;

struct PaletteBuffer;

struct PaletteData;

struct PaletteFadeControl;

struct ParticleSystem;

struct Party;

struct PartyPokemon;

struct Pokedex;

struct Pokemon;

struct PokemonDataBlockA;

struct PokemonDataBlockB;

struct PokemonDataBlockC;

struct PokemonDataBlockD;

struct Poketch;

struct Resource;

struct ResourceCollection;

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

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourceList;

struct SpriteResourceTable;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct String;

struct StringList;

struct StringTemplate;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct Trainer;

struct TrainerHeader;


struct TrainerIntroData;

struct UnkStruct_02012744_t;

struct UnkStruct_ov10_0221FB28_t;

struct WiFiHistory;

struct Window;

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

typedef struct ChatotCry {
    BOOL valid;
    s8 audioData[1000];
} ChatotCry;

typedef struct TrainerHeader {
    u8 monDataType;
    u8 trainerType;
    u8 sprite;
    u8 partySize;
    u16 items[4];
    u32 aiMask;
    u32 battleType;
} TrainerHeader;

typedef u16 charcode_t;

typedef struct EasyChatSentence {
    u16 type;
    u16 id;
    u16 words[2];
} EasyChatSentence;

typedef struct WiFiHistory {
    u32 unused_00;
    u8 hasInteractedOutsideOfJapan;
    u8 country;
    u8 region;
    u8 geonetCommunicationMap[4080];
} WiFiHistory;

enum ItemBattleCategory {
    ITEM_BATTLE_CATEGORY_RECOVER_HP = 0,
    ITEM_BATTLE_CATEGORY_RECOVER_STATUS = 1,
    ITEM_BATTLE_CATEGORY_POKE_BALLS = 2,
    ITEM_BATTLE_CATEGORY_BATTLE_ITEMS = 3,
    ITEM_BATTLE_CATEGORY_MAX = 4,
};

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

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;

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

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

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

typedef struct SpriteResourceTable {
    void *entries;
    int count;
    enum SpriteResourceType type;
    u8 loadFromNARC;
} SpriteResourceTable;


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

typedef struct FontOAM FontOAM;

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "pm_string.h"
#include "trainer_data.h"
#include "bag_cursor.h"
#include "options.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "player_data.h"
#include "pokedex.h"
#include "overlay_12_field_battle_dto.h"
#include "battle/trainer_intro_data.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "particle_system.h"
#include "screen_fade_internal.h"
#include "overlay_05.h"

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

enum VramManagerType {
    VRAM_MANAGER_TYPE_LINKED_LIST = 0,
    VRAM_MANAGER_TYPE_FRAME,
};

enum TextureVramSize {
    TEXTURE_VRAM_SIZE_128K = 1,
    TEXTURE_VRAM_SIZE_256K,
    TEXTURE_VRAM_SIZE_384K,
    TEXTURE_VRAM_SIZE_512K,
};

enum PaletteVramSize {
    PALETTE_VRAM_SIZE_16K = 1,
    PALETTE_VRAM_SIZE_32K,
    PALETTE_VRAM_SIZE_48K,
    PALETTE_VRAM_SIZE_64K,
    PALETTE_VRAM_SIZE_80K,
    PALETTE_VRAM_SIZE_96K,
};

typedef void (*G3DPipelineSetupCallback)(void);

G3DPipelineBuffers *GF_3DVramMan_Create(enum HeapID heapID, enum VramManagerType texVramManagerType, enum TextureVramSize texVramSize, enum VramManagerType plttVramManagerType, enum PaletteVramSize plttVramSize, G3DPipelineSetupCallback cb);

static inline G3DPipelineBuffers *G3DPipeline_Init(enum HeapID heapID, enum TextureVramSize texVramSize, enum PaletteVramSize plttVramSize, G3DPipelineSetupCallback cb) {
    return GF_3DVramMan_Create(heapID, VRAM_MANAGER_TYPE_LINKED_LIST, texVramSize, VRAM_MANAGER_TYPE_LINKED_LIST, plttVramSize, cb);
}

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


typedef struct StringList {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
} StringList;

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

void sub_0203A880(void);

void Main_SetVBlankIntrCB(Callback cb, void *data);

void App_StartScreenFade(u8 fadeOut, enum HeapID heapID);

typedef struct {
    u32 unk_00;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E;
    u8 unk_0F;
    u8 unk_10;
    u8 unk_11;
    u32 unk_14;
} UnkStruct_ov10_0221FB28_sub1;

void ov05_0221DB94(UnkStruct_ov10_0221FB28 *param0);

void ov05_0221DC60(UnkStruct_ov10_0221FB28 *param0);

void ov05_0221E274(UnkStruct_ov10_0221FB28 *param0);

void ov05_0221E07C(UnkStruct_ov10_0221FB28 *param0);

void ov05_0221DD08(UnkStruct_ov10_0221FB28 *param0);

void ov05_0221DE38(UnkStruct_ov10_0221FB28 *param0);

void ov05_0221E5E4(UnkStruct_ov10_0221FB28 *param0);

u8 ov05_0221BE04(UnkStruct_ov10_0221FB28 *param0);

u8 ov05_0221BE04(UnkStruct_ov10_0221FB28 *param0) {
    switch (param0->unk_B82) {
    case 0:
        ov05_0221DB94(param0);
        ov05_0221DC60(param0);
        break;
    case 1:
        ov05_0221E07C(param0);
        ov05_0221E274(param0);
        break;
    case 2:
        param0->unk_B82 = 0;
        param0->unk_B81 = 32;
        ov05_0221DD08(param0);
        ov05_0221E5E4(param0);
        ov05_0221E944(param0);
        ov05_0221DE38(param0);
        sub_0203A880();
        App_StartScreenFade(0, param0->trainerIntroData->heapID);
        Main_SetVBlankIntrCB(ov05_0221CE88, param0);
        return 1;
    }

    param0->unk_B82++;
    return 0;
}

void ov05_0221CEB8(UnkStruct_ov10_0221FB28 *param0, GXBG0As param1, int param2);

void ov05_0221D094(UnkStruct_ov10_0221FB28 *param0);

void ov05_0221D228(void);

void ov05_0221D240(UnkStruct_ov10_0221FB28 *param0);

void ov05_0221D6C4(UnkStruct_ov10_0221FB28 *param0, int param1, int param2);

void ov05_0221D9F0(UnkStruct_ov10_0221FB28 *param0);

u8 ov05_0221BE80(UnkStruct_ov10_0221FB28 *param0);

u8 ov05_0221BE80(UnkStruct_ov10_0221FB28 *param0) {
    switch (param0->unk_B82) {
    case 0:
        ov05_0221CEB8(param0, GX_BG0_AS_3D, 0);
        ov05_0221D094(param0);
        ov05_0221D228();
        break;
    case 1:
        ov05_0221D6C4(param0, 0, 0);
        ov05_0221D240(param0);
        sub_0203A880();
        ov05_0221D9F0(param0);
        break;
    case 2:
        App_StartScreenFade(0, param0->trainerIntroData->heapID);
        Main_SetVBlankIntrCB(ov05_0221CE88, param0);
        param0->unk_B82 = 0;
        param0->unk_B7C = 8;
        param0->unk_B7D = 4;
        param0->unk_B74 = 16;
        return 1;
    }

    param0->unk_B82++;
    return 0;
}


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


enum TextBank {
    TEXT_BANK_MOVES_USED_IN_BATTLE = 0,
    TEXT_BANK_DIPLOMA = 1,
    TEXT_BANK_BATTLE_BAG = 2,
    TEXT_BANK_BATTLE_PARTY = 3,
    TEXT_BANK_UNK_0004 = 4,
    TEXT_BANK_UNK_0005 = 5,
    TEXT_BANK_UNK_0006 = 6,
    TEXT_BANK_BAG = 7,
    TEXT_BANK_UNK_0008 = 8,
    TEXT_BANK_UNK_0009 = 9,
    TEXT_BANK_UNK_0010 = 10,
    TEXT_BANK_COMMUNICATION_CLUB = 11,
    TEXT_BANK_BALL_SEAL_NAMES = 12,
    TEXT_BANK_BALL_SEAL_NAMES_PLURAL = 13,
    TEXT_BANK_MAIN_MENU_ALERTS = 14,
    TEXT_BANK_WFC_FACILITY_SELECTOR = 15,
    TEXT_BANK_BATTLE_FRONTIER_RECORDS = 16,
    TEXT_BANK_BG_EVENTS = 17,
    TEXT_BANK_POKEMON_STORAGE_SYSTEM = 18,
    TEXT_BANK_BOX_MESSAGES = 19,
    TEXT_BANK_UNK_0020 = 20,
    TEXT_BANK_FRONTIER_TRAINER_NAMES = 21,
    TEXT_BANK_BATTLE_TOWER_RECORDS_APP = 22,
    TEXT_BANK_JUBILIFE_CITY = 23,
    TEXT_BANK_JUBILIFE_CITY_MART = 24,
    TEXT_BANK_UNUSED_JUBILIFE_CITY_HOUSE_1 = 25,
    TEXT_BANK_JUBILIFE_CITY_POKECENTER_1F = 26,
    TEXT_BANK_POKETCH_CO_1F = 27,
    TEXT_BANK_POKETCH_CO_2F = 28,
    TEXT_BANK_POKETCH_CO_3F = 29,
    TEXT_BANK_JUBILIFE_TV_1F = 30,
    TEXT_BANK_JUBILIFE_TV_2F = 31,
    TEXT_BANK_JUBILIFE_TV_3F = 32,
    TEXT_BANK_JUBILIFE_TV_4F = 33,
    TEXT_BANK_JUBILIFE_TV_2F_GALLERY = 34,
    TEXT_BANK_JUBILIFE_TV_3F_GLOBAL_RANKING_ROOM = 35,
    TEXT_BANK_JUBILIFE_TV_3F_GROUP_RANKING_ROOM = 36,
    TEXT_BANK_JUBILIFE_TV_ELEVATOR = 37,
    TEXT_BANK_JUBILIFE_CITY_SOUTH_HOUSE_1F = 38,
    TEXT_BANK_JUBILIFE_CITY_SOUTH_HOUSE_2F = 39,
    TEXT_BANK_UNUSED_JUBILIFE_CITY_SOUTH_HOUSE_3F = 40,
    TEXT_BANK_UNUSED_JUBILIFE_CITY_HOUSE_2 = 41,
    TEXT_BANK_JUBILIFE_CITY_CONDOMINIUMS_1F = 42,
    TEXT_BANK_JUBILIFE_CITY_CONDOMINIUMS_2F = 43,
    TEXT_BANK_UNUSED_JUBILIFE_CITY_CONDOMINIUMS_3F = 44,
    TEXT_BANK_UNUSED_JUBILIFE_CITY_CONDOMINIUMS_4F = 45,
    TEXT_BANK_GLOBAL_TERMINAL_1F = 46,
    TEXT_BANK_GLOBAL_TERMINAL_2F = 47,
    TEXT_BANK_GLOBAL_TERMINAL_3F = 48,
    TEXT_BANK_TRAINERS_SCHOOL = 49,
    TEXT_BANK_JUBILIFE_CITY_SOUTHWEST_HOUSE_1F = 50,
    TEXT_BANK_JUBILIFE_CITY_SOUTHWEST_HOUSE_2F = 51,
    TEXT_BANK_UNUSED_JUBILIFE_CITY_HOUSE_3 = 52,
    TEXT_BANK_UNUSED_JUBILIFE_CITY_HOUSE_4 = 53,
    TEXT_BANK_CANALAVE_CITY = 54,
    TEXT_BANK_CANALAVE_CITY_MART = 55,
    TEXT_BANK_CANALAVE_CITY_GYM = 56,
    TEXT_BANK_CANALAVE_CITY_POKECENTER_1F = 57,
    TEXT_BANK_CANALAVE_LIBRARY_1F = 58,
    TEXT_BANK_CANALAVE_LIBRARY_2F = 59,
    TEXT_BANK_CANALAVE_LIBRARY_3F = 60,
    TEXT_BANK_CANALAVE_CITY_SOUTHEAST_HOUSE = 61,
    TEXT_BANK_CANALAVE_CITY_EAST_HOUSE = 62,
    TEXT_BANK_CANALAVE_CITY_HARBOR_INN = 63,
    TEXT_BANK_CANALAVE_CITY_SAILOR_ELDRITCH_HOUSE = 64,
    TEXT_BANK_OREBURGH_CITY = 65,
    TEXT_BANK_OREBURGH_CITY_MART = 66,
    TEXT_BANK_OREBURGH_CITY_GYM = 67,
    TEXT_BANK_OREBURGH_CITY_POKECENTER_1F = 68,
    TEXT_BANK_OREBURGH_CITY_POKECENTER_B1F = 69,
    TEXT_BANK_OREBURGH_CITY_NORTHWEST_HOUSE_1F = 70,
    TEXT_BANK_OREBURGH_CITY_NORTHWEST_HOUSE_2F = 71,
    TEXT_BANK_UNUSED_OREBURGH_CITY_NORTHWEST_HOUSE_3F = 72,
    TEXT_BANK_UNUSED_OREBURGH_CITY_NORTHWEST_HOUSE_4F = 73,
    TEXT_BANK_OREBURGH_CITY_NORTH_HOUSE_1F = 74,
    TEXT_BANK_OREBURGH_CITY_NORTH_HOUSE_2F = 75,
    TEXT_BANK_UNUSED_OREBURGH_CITY_NORTH_HOUSE_3F = 76,
    TEXT_BANK_UNUSED_OREBURGH_CITY_NORTH_HOUSE_4F = 77,
    TEXT_BANK_OREBURGH_CITY_MIDDLE_HOUSE = 78,
    TEXT_BANK_MINING_MUSEUM = 79,
    TEXT_BANK_OREBURGH_CITY_WEST_HOUSE = 80,
    TEXT_BANK_OREBURGH_CITY_EAST_HOUSE_1F = 81,
    TEXT_BANK_OREBURGH_CITY_EAST_HOUSE_2F = 82,
    TEXT_BANK_UNUSED_OREBURGH_CITY_EAST_HOUSE_3F = 83,
    TEXT_BANK_OREBURGH_CITY_SOUTH_HOUSE = 84,
    TEXT_BANK_ETERNA_CITY = 85,
    TEXT_BANK_ETERNA_CITY_MART = 86,
    TEXT_BANK_ETERNA_CITY_GYM = 87,
    TEXT_BANK_ETERNA_CITY_DP_GYM = 88,
    TEXT_BANK_ETERNA_CITY_POKECENTER_1F = 89,
    TEXT_BANK_ETERNA_CITY_POKECENTER_2F = 90,
    TEXT_BANK_CYCLE_SHOP = 91,
    TEXT_BANK_TEAM_GALACTIC_ETERNA_BUILDING_1F = 92,
    TEXT_BANK_TEAM_GALACTIC_ETERNA_BUILDING_2F = 93,
    TEXT_BANK_TEAM_GALACTIC_ETERNA_BUILDING_3F = 94,
    TEXT_BANK_TEAM_GALACTIC_ETERNA_BUILDING_4F = 95,
    TEXT_BANK_ROTOMS_ROOM = 96,
    TEXT_BANK_ETERNA_CITY_CONDOMINIUMS_1F = 97,
    TEXT_BANK_ETERNA_CITY_CONDOMINIUMS_2F = 98,
    TEXT_BANK_ETERNA_CITY_CONDOMINIUMS_3F = 99,
    TEXT_BANK_UNUSED_ETERNA_CITY_CONDOMINIUMS_4F = 100,
    TEXT_BANK_ROUTE_206_CYCLING_ROAD_NORTH_GATE = 101,
    TEXT_BANK_ETERNA_CITY_HERB_SHOP = 102,
    TEXT_BANK_ETERNA_CITY_SOUTH_HOUSE = 103,
    TEXT_BANK_ETERNA_CITY_EAST_HOUSE = 104,
    TEXT_BANK_ETERNA_CITY_UNDERGROUND_MAN_HOUSE = 105,
    TEXT_BANK_HEARTHOME_CITY = 106,
    TEXT_BANK_HEARTHOME_CITY_MART = 107,
    TEXT_BANK_HEARTHOME_CITY_GYM_ENTRANCE_ROOM = 108,
    TEXT_BANK_HEARTHOME_CITY_GYM_LEADER_ROOM = 109,
    TEXT_BANK_HEARTHOME_CITY_DP_GYM_ELEVATOR_ROOM_1 = 110,
    TEXT_BANK_HEARTHOME_CITY_DP_GYM_ELEVATOR_ROOM_2 = 111,
    TEXT_BANK_HEARTHOME_CITY_DP_GYM_LEADER_ROOM = 112,
    TEXT_BANK_HEARTHOME_CITY_POKECENTER_1F = 113,
    TEXT_BANK_HEARTHOME_CITY_SOUTHEAST_HOUSE_1F = 114,
    TEXT_BANK_HEARTHOME_CITY_SOUTHEAST_HOUSE_2F = 115,
    TEXT_BANK_HEARTHOME_CITY_SOUTHEAST_HOUSE_ELEVATOR = 116,
    TEXT_BANK_HEARTHOME_CITY_POKEMON_FAN_CLUB = 117,
    TEXT_BANK_HEARTHOME_CITY_WEST_GATE_TO_AMITY_SQUARE = 118,
    TEXT_BANK_HEARTHOME_CITY_EAST_GATE_TO_AMITY_SQUARE = 119,
    TEXT_BANK_ROUTE_208_GATE_TO_HEARTHOME_CITY = 120,
    TEXT_BANK_ROUTE_209_GATE_TO_HEARTHOME_CITY = 121,
    TEXT_BANK_ROUTE_212_GATE_TO_HEARTHOME_CITY = 122,
    TEXT_BANK_HEARTHOME_CITY_NORTHEAST_HOUSE_1F = 123,
    TEXT_BANK_HEARTHOME_CITY_NORTHEAST_HOUSE_2F = 124,
    TEXT_BANK_HEARTHOME_CITY_NORTHEAST_HOUSE_ELEVATOR = 125,
    TEXT_BANK_HEARTHOME_CITY_NORTHWEST_HOUSE = 126,
    TEXT_BANK_POFFIN_HOUSE = 127,
    TEXT_BANK_CONTEST_HALL_LOBBY = 128,
    TEXT_BANK_CONTEST_HALL_STAGE_NO_CONTEST = 129,
    TEXT_BANK_FOREIGN_BUILDING = 130,
    TEXT_BANK_PASTORIA_CITY = 131,
    TEXT_BANK_PASTORIA_CITY_MART = 132,
    TEXT_BANK_PASTORIA_CITY_GYM = 133,
    TEXT_BANK_PASTORIA_CITY_POKECENTER_1F = 134,
    TEXT_BANK_PASTORIA_CITY_POKECENTER_2F = 135,
    TEXT_BANK_PASTORIA_CITY_OBSERVATORY_GATE_1F = 136,
    TEXT_BANK_PASTORIA_CITY_OBSERVATORY_GATE_2F = 137,
    TEXT_BANK_PASTORIA_CITY_SOUTHWEST_HOUSE = 138,
    TEXT_BANK_PASTORIA_CITY_MIDDLE_HOUSE = 139,
    TEXT_BANK_PASTORIA_CITY_EAST_HOUSE = 140,
    TEXT_BANK_PASTORIA_CITY_NORTH_HOUSE = 141,
    TEXT_BANK_PASTORIA_CITY_NORTHEAST_HOUSE = 142,
    TEXT_BANK_VEILSTONE_CITY = 143,
    TEXT_BANK_VEILSTONE_CITY_GYM = 144,
    TEXT_BANK_VEILSTONE_CITY_POKECENTER_1F = 145,
    TEXT_BANK_VEILSTONE_CITY_POKECENTER_B1F = 146,
    TEXT_BANK_GAME_CORNER = 147,
    TEXT_BANK_VEILSTONE_STORE_1F = 148,
    TEXT_BANK_VEILSTONE_STORE_2F = 149,
    TEXT_BANK_VEILSTONE_STORE_3F = 150,
    TEXT_BANK_VEILSTONE_STORE_4F = 151,
    TEXT_BANK_VEILSTONE_STORE_5F = 152,
    TEXT_BANK_VEILSTONE_STORE_ELEVATOR = 153,
    TEXT_BANK_VEILSTONE_STORE_B1F = 154,
    TEXT_BANK_VEILSTONE_CITY_GALACTIC_WAREHOUSE = 155,
    TEXT_BANK_VEILSTONE_CITY_PRIZE_EXCHANGE = 156,
    TEXT_BANK_VEILSTONE_CITY_SOUTHEAST_HOUSE = 157,
    TEXT_BANK_VEILSTONE_CITY_NORTHWEST_HOUSE = 158,
    TEXT_BANK_VEILSTONE_CITY_NORTHEAST_HOUSE = 159,
    TEXT_BANK_VEILSTONE_CITY_SOUTHWEST_HOUSE = 160,
    TEXT_BANK_ROUTE_215_GATE_TO_VEILSTONE_CITY = 161,
    TEXT_BANK_SUNYSHORE_CITY = 162,
    TEXT_BANK_SUNYSHORE_CITY_MART = 163,
    TEXT_BANK_SUNYSHORE_CITY_GYM_ROOM_1 = 164,
    TEXT_BANK_SUNYSHORE_CITY_GYM_ROOM_3 = 165,
    TEXT_BANK_SUNYSHORE_CITY_POKECENTER_1F = 166,
    TEXT_BANK_SUNYSHORE_MARKET = 167,
    TEXT_BANK_SUNYSHORE_CITY_NORTHEAST_HOUSE = 168,
    TEXT_BANK_SUNYSHORE_CITY_WEST_HOUSE = 169,
    TEXT_BANK_SUNYSHORE_CITY_NORTHWEST_HOUSE = 170,
    TEXT_BANK_UNUSED_SUNYSHORE_CITY_HOUSE_1 = 171,
    TEXT_BANK_UNUSED_SUNYSHORE_CITY_HOUSE_2 = 172,
    TEXT_BANK_SUNYSHORE_CITY_EAST_HOUSE = 173,
    TEXT_BANK_VISTA_LIGHTHOUSE = 174,
    TEXT_BANK_SUNYSHORE_CITY_EAST_HOUSE_UNUSED = 175,
    TEXT_BANK_SNOWPOINT_CITY = 176,
    TEXT_BANK_SNOWPOINT_CITY_MART = 177,
    TEXT_BANK_SNOWPOINT_CITY_GYM = 178,
    TEXT_BANK_SNOWPOINT_CITY_POKECENTER_1F = 179,
    TEXT_BANK_SNOWPOINT_CITY_WEST_HOUSE = 180,
    TEXT_BANK_SNOWPOINT_CITY_EAST_HOUSE = 181,
    TEXT_BANK_POKEMON_LEAGUE = 182,
    TEXT_BANK_POKEMON_LEAGUE_SOUTH_POKECENTER_1F = 183,
    TEXT_BANK_POKEMON_LEAGUE_NORTH_POKECENTER_1F = 184,
    TEXT_BANK_POKEMON_LEAGUE_AARON_ROOM = 185,
    TEXT_BANK_POKEMON_LEAGUE_BERTHA_ROOM = 186,
    TEXT_BANK_POKEMON_LEAGUE_FLINT_ROOM = 187,
    TEXT_BANK_POKEMON_LEAGUE_LUCIAN_ROOM = 188,
    TEXT_BANK_POKEMON_LEAGUE_CHAMPION_ROOM = 189,
    TEXT_BANK_POKEMON_LEAGUE_HALLWAY_TO_HALL_OF_FAME = 190,
    TEXT_BANK_POKEMON_LEAGUE_HALL_OF_FAME = 191,
    TEXT_BANK_FIGHT_AREA = 192,
    TEXT_BANK_FIGHT_AREA_MART = 193,
    TEXT_BANK_FIGHT_AREA_POKECENTER_1F = 194,
    TEXT_BANK_UNUSED_BATTLE_PARK_GATE_TO_FIGHT_AREA = 195,
    TEXT_BANK_ROUTE_225_GATE_TO_FIGHT_AREA = 196,
    TEXT_BANK_FIGHT_AREA_MIDDLE_HOUSE = 197,
    TEXT_BANK_FIGHT_AREA_SOUTH_HOUSE = 198,
    TEXT_BANK_BATTLE_CASTLE_SELF_APP = 199,
    TEXT_BANK_BATTLE_CASTLE_SCENE = 200,
    TEXT_BANK_BATTLE_CASTLE_OPPONENT_APP = 201,
    TEXT_BANK_NATURE_NAMES = 202,
    TEXT_BANK_POKEMON_CENTER_DAILY_TRAINERS = 203,
    TEXT_BANK_CONTEST_TEXT = 204,
    TEXT_BANK_CONTEST_OPPONENT_NAMES = 205,
    TEXT_BANK_CONTEST_DANCE_COMPETITION = 206,
    TEXT_BANK_CONTEST_JUDGE_NAMES = 207,
    TEXT_BANK_LINK_CONTEST_RECORDS = 208,
    TEXT_BANK_CONTEST_VISUAL_COMPETITION = 209,
    TEXT_BANK_CONTEST_EFFECTS = 210,
    TEXT_BANK_CONTEST_ACTING_COMPETITION = 211,
    TEXT_BANK_UNK_0212 = 212,
    TEXT_BANK_COMMON_STRINGS = 213,
    TEXT_BANK_NETWORK_ERRORS = 214,
    TEXT_BANK_DUMMY_0215 = 215,
    TEXT_BANK_DUMMY_0216 = 216,
    TEXT_BANK_CONTEST_COMMON = 217,
    TEXT_BANK_CONTEST_RESULTS = 218,
    TEXT_BANK_STATUS_CONDITION_NAMES = 219,
    TEXT_BANK_OPTIONS_MENU = 220,
    TEXT_BANK_POKEMON_CENTER_2F_COMMON = 221,
    TEXT_BANK_OREBURGH_MINE_B1F = 222,
    TEXT_BANK_OREBURGH_MINE_B2F = 223,
    TEXT_BANK_VALLEY_WINDWORKS_OUTSIDE = 224,
    TEXT_BANK_VALLEY_WINDWORKS_BUILDING = 225,
    TEXT_BANK_ETERNA_FOREST_OUTSIDE = 226,
    TEXT_BANK_ETERNA_FOREST = 227,
    TEXT_BANK_FUEGO_IRONWORKS_OUTSIDE = 228,
    TEXT_BANK_FUEGO_IRONWORKS_BUILDING = 229,
    TEXT_BANK_MT_CORONET_1F_SOUTH = 230,
    TEXT_BANK_MT_CORONET_2F = 231,
    TEXT_BANK_MT_CORONET_1F_TUNNEL_ROOM = 232,
    TEXT_BANK_MT_CORONET_1F_NORTH_ROOM_1 = 233,
    TEXT_BANK_SPEAR_PILLAR = 234,
    TEXT_BANK_SPEAR_PILLAR_DISTORTED = 235,
    TEXT_BANK_HALL_OF_ORIGIN = 236,
    TEXT_BANK_SPEAR_PILLAR_DIALGA = 237,
    TEXT_BANK_SPEAR_PILLAR_PALKIA = 238,
    TEXT_BANK_GREAT_MARSH_1_UNUSED = 239,
    TEXT_BANK_GREAT_MARSH_1 = 240,
    TEXT_BANK_GREAT_MARSH_2 = 241,
    TEXT_BANK_GREAT_MARSH_3 = 242,
    TEXT_BANK_GREAT_MARSH_4 = 243,
    TEXT_BANK_GREAT_MARSH_5 = 244,
    TEXT_BANK_GREAT_MARSH_6 = 245,
    TEXT_BANK_SOLACEON_RUINS_MANIAC_TUNNEL_ROOM = 246,
    TEXT_BANK_SOLACEON_RUINS_ROOM_1 = 247,
    TEXT_BANK_SOLACEON_RUINS_ROOM_2 = 248,
    TEXT_BANK_SOLACEON_RUINS_ROOM_7 = 249,
    TEXT_BANK_VICTORY_ROAD_1F = 250,
    TEXT_BANK_VICTORY_ROAD_1F_ROOM_2 = 251,
    TEXT_BANK_PAL_PARK = 252,
    TEXT_BANK_AMITY_SQUARE = 253,
    TEXT_BANK_UNK_0254 = 254,
    TEXT_BANK_FLOAROMA_MEADOW = 255,
    TEXT_BANK_FLOAROMA_MEADOW_HOUSE = 256,
    TEXT_BANK_OREBURGH_GATE_1F = 257,
    TEXT_BANK_OREBURGH_GATE_B1F = 258,
    TEXT_BANK_FULLMOON_ISLAND = 259,
    TEXT_BANK_FULLMOON_ISLAND_FOREST = 260,
    TEXT_BANK_STARK_MOUNTAIN_OUTSIDE = 261,
    TEXT_BANK_STARK_MOUNTAIN_ROOM_1 = 262,
    TEXT_BANK_STARK_MOUNTAIN_ROOM_2 = 263,
    TEXT_BANK_STARK_MOUNTAIN_ROOM_3 = 264,
    TEXT_BANK_SENDOFF_SPRING = 265,
    TEXT_BANK_TURNBACK_CAVE_ENTRANCE = 266,
    TEXT_BANK_TURNBACK_CAVE_PILLAR_ROOM = 267,
    TEXT_BANK_TURNBACK_CAVE_GIRATINA_ROOM = 268,
    TEXT_BANK_FLOWER_PARADISE = 269,
    TEXT_BANK_SNOWPOINT_TEMPLE_1F = 270,
    TEXT_BANK_SNOWPOINT_TEMPLE_B5F = 271,
    TEXT_BANK_WAYWARD_CAVE_1F = 272,
    TEXT_BANK_RUIN_MANIAC_CAVE_SHORT = 273,
    TEXT_BANK_RUIN_MANIAC_CAVE_LONG = 274,
    TEXT_BANK_MANIAC_TUNNEL = 275,
    TEXT_BANK_IRON_ISLAND = 276,
    TEXT_BANK_IRON_ISLAND_B2F_LEFT_ROOM = 277,
    TEXT_BANK_IRON_ISLAND_HOUSE = 278,
    TEXT_BANK_OLD_CHATEAU = 279,
    TEXT_BANK_OLD_CHATEAU_SIDE_ROOMS = 280,
    TEXT_BANK_OLD_CHATEAU_BACK_MIDDLE_WEST_ROOM = 281,
    TEXT_BANK_GALACTIC_HQ_1F = 282,
    TEXT_BANK_GALACTIC_HQ_2F = 283,
    TEXT_BANK_GALACTIC_HQ_3F = 284,
    TEXT_BANK_GALACTIC_HQ_4F = 285,
    TEXT_BANK_GALACTIC_HQ_B1F = 286,
    TEXT_BANK_GALACTIC_HQ_B2F = 287,
    TEXT_BANK_GALACTIC_HQ_CONTROL_ROOM = 288,
    TEXT_BANK_GALACTIC_HQ_LABORATORY = 289,
    TEXT_BANK_GALACTIC_HQ_HALL = 290,
    TEXT_BANK_LAKE_VERITY_LOW_WATER = 291,
    TEXT_BANK_LAKE_VERITY = 292,
    TEXT_BANK_VERITY_CAVERN = 293,
    TEXT_BANK_LAKE_VALOR_DRAINED = 294,
    TEXT_BANK_LAKE_VALOR = 295,
    TEXT_BANK_VALOR_CAVERN = 296,
    TEXT_BANK_LAKE_ACUITY_LOW_WATER = 297,
    TEXT_BANK_LAKE_ACUITY = 298,
    TEXT_BANK_ACUITY_CAVERN = 299,
    TEXT_BANK_NEWMOON_ISLAND = 300,
    TEXT_BANK_NEWMOON_ISLAND_FOREST = 301,
    TEXT_BANK_UNUSED_BATTLE_PARK = 302,
    TEXT_BANK_UNUSED_BATTLE_PARK_EXCHANGE_SERVICE_CORNER = 303,
    TEXT_BANK_BATTLE_TOWER = 304,
    TEXT_BANK_BATTLE_TOWER_BATTLE_ROOM = 305,
    TEXT_BANK_BATTLE_TOWER_MULTI_BATTLE_ROOM = 306,
    TEXT_BANK_BATTLE_TOWER_BATTLE_SALON = 307,
    TEXT_BANK_BATTLE_FRONTIER = 308,
    TEXT_BANK_BATTLE_FRONTIER_GATE_TO_FIGHT_AREA = 309,
    TEXT_BANK_BATTLE_FACTORY = 310,
    TEXT_BANK_BATTLE_HALL = 311,
    TEXT_BANK_BATTLE_CASTLE = 312,
    TEXT_BANK_BATTLE_ARCADE = 313,
    TEXT_BANK_DISTORTION_WORLD_1F = 314,
    TEXT_BANK_DISTORTION_WORLD_B1F = 315,
    TEXT_BANK_DISTORTION_WORLD_B2F = 316,
    TEXT_BANK_DISTORTION_WORLD_B3F = 317,
    TEXT_BANK_DISTORTION_WORLD_B6F = 318,
    TEXT_BANK_DISTORTION_WORLD_B7F = 319,
    TEXT_BANK_DISTORTION_WORLD_GIRATINA_ROOM = 320,
    TEXT_BANK_DISTORTION_WORLD_TURNBACK_CAVE_ROOM = 321,
    TEXT_BANK_IRON_RUINS = 322,
    TEXT_BANK_ICEBERG_RUINS = 323,
    TEXT_BANK_ROCK_PEAK_RUINS = 324,
    TEXT_BANK_UNK_0325 = 325,
    TEXT_BANK_UNK_0326 = 326,
    TEXT_BANK_UNK_0327 = 327,
    TEXT_BANK_UNK_0328 = 328,
    TEXT_BANK_UNK_0329 = 329,
    TEXT_BANK_UNK_0330 = 330,
    TEXT_BANK_UNK_0331 = 331,
    TEXT_BANK_UNK_0332 = 332,
    TEXT_BANK_UNK_0333 = 333,
    TEXT_BANK_UNK_0334 = 334,
    TEXT_BANK_UNK_0335 = 335,
    TEXT_BANK_UNK_0336 = 336,
    TEXT_BANK_UNK_0337 = 337,
    TEXT_BANK_UNK_0338 = 338,
    TEXT_BANK_UNK_0339 = 339,
    TEXT_BANK_UNK_0340 = 340,
    TEXT_BANK_UNK_0341 = 341,
    TEXT_BANK_UNK_0342 = 342,
    TEXT_BANK_UNK_0343 = 343,
    TEXT_BANK_UNK_0344 = 344,
    TEXT_BANK_UNK_0345 = 345,
    TEXT_BANK_UNK_0346 = 346,
    TEXT_BANK_UNK_0347 = 347,
    TEXT_BANK_UNK_0348 = 348,
    TEXT_BANK_DUMMY_0349 = 349,
    TEXT_BANK_TRADE = 350,
    TEXT_BANK_HALL_OF_FAME = 351,
    TEXT_BANK_PC_HALL_OF_FAME = 352,
    TEXT_BANK_UNK_0353 = 353,
    TEXT_BANK_TRADE_ROOM = 354,
    TEXT_BANK_DUMMY_0355 = 355,
    TEXT_BANK_WIFI_EARTH = 356,
    TEXT_BANK_EGG_HATCH = 357,
    TEXT_BANK_UNK_0358 = 358,
    TEXT_BANK_UNK_0359 = 359,
    TEXT_BANK_UNK_0360 = 360,
    TEXT_BANK_MENU_ENTRIES = 361,
    TEXT_BANK_DUMMY_0362 = 362,
    TEXT_BANK_BATTLE_FACTORY_OT_NAME = 363,
    TEXT_BANK_BATTLE_FACTORY_APP = 364,
    TEXT_BANK_BATTLE_FACTORY_SCENE = 365,
    TEXT_BANK_JOURNAL_ENTRIES = 366,
    TEXT_BANK_START_MENU = 367,
    TEXT_BANK_BATTLE_STRINGS = 368,
    TEXT_BANK_VISIBLE_ITEMS = 369,
    TEXT_BANK_NPC_TRADE_NAMES = 370,
    TEXT_BANK_FURNITURE_NAMES = 371,
    TEXT_BANK_MYSTERY_GIFT_PHRASE = 372,
    TEXT_BANK_BLACK_OUT_SCENE = 373,
    TEXT_BANK_GROUP_CONNECTION = 374,
    TEXT_BANK_UNK_0375 = 375,
    TEXT_BANK_SPIN_TRADE = 376,
    TEXT_BANK_UNK_0377 = 377,
    TEXT_BANK_GYM_NAMES = 378,
    TEXT_BANK_MYSTERY_GIFT_DELIVERYMAN = 379,
    TEXT_BANK_HIDDEN_ITEMS = 380,
    TEXT_BANK_FIELD_MOVES = 381,
    TEXT_BANK_WIFI_PLAZA_ENTRANCE = 382,
    TEXT_BANK_POKEDEX_RATINGS = 383,
    TEXT_BANK_UNK_0384 = 384,
    TEXT_BANK_UNK_0385 = 385,
    TEXT_BANK_CONTEST_ACCESSORY_NAMES = 386,
    TEXT_BANK_CONTEST_ACCESSORY_NAMES_WITH_ARTICLES = 387,
    TEXT_BANK_CONTEST_BACKDROP_NAMES = 388,
    TEXT_BANK_ROWAN_INTRO = 389,
    TEXT_BANK_DUMMY_0390 = 390,
    TEXT_BANK_ITEM_DESCRIPTIONS = 391,
    TEXT_BANK_ITEM_NAMES = 392,
    TEXT_BANK_ITEM_NAMES_WITH_ARTICLES = 393,
    TEXT_BANK_ITEM_NAMES_PLURAL = 394,
    TEXT_BANK_BAG_POCKET_NAMES = 395,
    TEXT_BANK_BAG_POCKET_NAMES_WITH_ICONS = 396,
    TEXT_BANK_BERRY_TREES = 397,
    TEXT_BANK_BERRY_TAGS = 398,
    TEXT_BANK_VERITY_LAKEFRONT = 399,
    TEXT_BANK_UNUSED_VERITY_LAKEFRONT_HOUSE = 400,
    TEXT_BANK_VALOR_LAKEFRONT = 401,
    TEXT_BANK_RESTAURANT = 402,
    TEXT_BANK_GRAND_LAKE_VALOR_LAKEFRONT_EAST_HOUSE = 403,
    TEXT_BANK_GRAND_LAKE_VALOR_LAKEFRONT_WEST_HOUSE = 404,
    TEXT_BANK_ACUITY_LAKEFRONT = 405,
    TEXT_BANK_SAVE_CORRUPTED = 406,
    TEXT_BANK_UNK_0407 = 407,
    TEXT_BANK_MAILBOX = 408,
    TEXT_BANK_MAIL = 409,
    TEXT_BANK_UNK_0410 = 410,
    TEXT_BANK_UNK_0411 = 411,
    TEXT_BANK_SPECIES_NAME = 412,
    TEXT_BANK_SPECIES_NAME_WITH_ARTICLES = 413,
    TEXT_BANK_MONTH_NAMES = 414,
    TEXT_BANK_TV_PROGRAMS_INTERVIEWS = 415,
    TEXT_BANK_TV_PROGRAMS_TRAINER_SIGHTINGS = 416,
    TEXT_BANK_TV_PROGRAMS_RECORDS = 417,
    TEXT_BANK_TV_PROGRAMS_SINNOH_NOW = 418,
    TEXT_BANK_TV_PROGRAMS_VARIETY_HOUR = 419,
    TEXT_BANK_UNK_0420 = 420,
    TEXT_BANK_MYSTERY_GIFT_MENU = 421,
    TEXT_BANK_NAMING_SCREEN = 422,
    TEXT_BANK_BERRY_DESCRIPTIONS = 423,
    TEXT_BANK_BERRY_NAMES = 424,
    TEXT_BANK_DRAWING = 425,
    TEXT_BANK_DUMMY_0426 = 426,
    TEXT_BANK_GENERIC_NAMES = 427,
    TEXT_BANK_UNK_0428 = 428,
    TEXT_BANK_FOLLOWER_PARTNERS = 429,
    TEXT_BANK_POKEMON_CENTER_B1F_COMMON = 430,
    TEXT_BANK_RECORD_CHATOT_CRY = 431,
    TEXT_BANK_POFFIN_COMMON = 432,
    TEXT_BANK_LOCATION_NAMES = 433,
    TEXT_BANK_MYSTERY_GIFT_EVENT_NAMES = 434,
    TEXT_BANK_SPECIAL_MET_LOCATION_NAMES = 435,
    TEXT_BANK_EASY_CHAT_GROUPS = 436,
    TEXT_BANK_EASY_CHAT = 437,
    TEXT_BANK_UNK_0438 = 438,
    TEXT_BANK_TRAINER_WORDS = 439,
    TEXT_BANK_PEOPLE_WORDS = 440,
    TEXT_BANK_GREETINGS = 441,
    TEXT_BANK_LIFESTYLE_WORDS = 442,
    TEXT_BANK_FEELINGS = 443,
    TEXT_BANK_TOUGH_WORDS = 444,
    TEXT_BANK_UNION_WORDS = 445,
    TEXT_BANK_LOSS_SENTENCES = 446,
    TEXT_BANK_GENERAL_SENTENCES = 447,
    TEXT_BANK_PRE_BATTLE_SENTENCES = 448,
    TEXT_BANK_UNION_ROOM_SENTENCES = 449,
    TEXT_BANK_WIN_SENTENCES = 450,
    TEXT_BANK_UNK_0451 = 451,
    TEXT_BANK_UNK_0452 = 452,
    TEXT_BANK_PARTY_MENU = 453,
    TEXT_BANK_MIGRATE_FROM_GBA = 454,
    TEXT_BANK_POKEMON_SUMMARY_SCREEN = 455,
    TEXT_BANK_POKETCH_MOVE_TESTER = 456,
    TEXT_BANK_POKETCH_APP_NAMES = 457,
    TEXT_BANK_POKETCH_POKEMON_HISTORY = 458,
    TEXT_BANK_POKETCH_BERRY_SEARCHER = 459,
    TEXT_BANK_DUMMY_0460 = 460,
    TEXT_BANK_POKETCH_LINK_SEARCHER = 461,
    TEXT_BANK_POFFIN_CUTSCENE = 462,
    TEXT_BANK_POFFIN_CASE = 463,
    TEXT_BANK_POFFIN_MAKING = 464,
    TEXT_BANK_POFFIN_TYPES = 465,
    TEXT_BANK_ROUTE_201 = 466,
    TEXT_BANK_ROUTE_202 = 467,
    TEXT_BANK_ROUTE_203 = 468,
    TEXT_BANK_ROUTE_204_SOUTH = 469,
    TEXT_BANK_ROUTE_204_NORTH = 470,
    TEXT_BANK_ROUTE_205_SOUTH = 471,
    TEXT_BANK_ROUTE_205_HOUSE = 472,
    TEXT_BANK_ROUTE_205_NORTH = 473,
    TEXT_BANK_ROUTE_206 = 474,
    TEXT_BANK_ROUTE_206_CYCLING_ROAD_SOUTH_GATE = 475,
    TEXT_BANK_UNUSED_GATE_BETWEEN_ETERNA_CITY_ROUTE_206 = 476,
    TEXT_BANK_ROUTE_207 = 477,
    TEXT_BANK_ROUTE_208 = 478,
    TEXT_BANK_ROUTE_208_HOUSE = 479,
    TEXT_BANK_ROUTE_209 = 480,
    TEXT_BANK_ROUTE_209_LOST_TOWER_1F = 481,
    TEXT_BANK_ROUTE_209_LOST_TOWER_5F = 482,
    TEXT_BANK_ROUTE_210_SOUTH = 483,
    TEXT_BANK_CAFE = 484,
    TEXT_BANK_ROUTE_210_NORTH = 485,
    TEXT_BANK_ROUTE_210_GRANDMA_WILMA_HOUSE = 486,
    TEXT_BANK_ROUTE_211_WEST = 487,
    TEXT_BANK_ROUTE_211_EAST = 488,
    TEXT_BANK_ROUTE_212_NORTH = 489,
    TEXT_BANK_POKEMON_MANSION = 490,
    TEXT_BANK_POKEMON_MANSION_MAIDS_ROOM = 491,
    TEXT_BANK_POKEMON_MANSION_OFFICE = 492,
    TEXT_BANK_ROUTE_212_SOUTH = 493,
    TEXT_BANK_ROUTE_212_HOUSE = 494,
    TEXT_BANK_ROUTE_213 = 495,
    TEXT_BANK_ROUTE_213_GATE_TO_PASTORIA_CITY = 496,
    TEXT_BANK_FOOTSTEP_HOUSE = 497,
    TEXT_BANK_GRAND_LAKE_ROUTE_213_LOBBY = 498,
    TEXT_BANK_GRAND_LAKE_ROUTE_213_EAST_HOUSE = 499,
    TEXT_BANK_GRAND_LAKE_ROUTE_213_NORTHWEST_HOUSE = 500,
    TEXT_BANK_GRAND_LAKE_ROUTE_213_NORTHEAST_HOUSE = 501,
    TEXT_BANK_ROUTE_214 = 502,
    TEXT_BANK_ROUTE_214_GATE_TO_VEILSTONE_CITY = 503,
    TEXT_BANK_ROUTE_215 = 504,
    TEXT_BANK_ROUTE_216 = 505,
    TEXT_BANK_ROUTE_216_HOUSE = 506,
    TEXT_BANK_ROUTE_217 = 507,
    TEXT_BANK_ROUTE_217_WEST_HOUSE = 508,
    TEXT_BANK_ROUTE_217_NORTHEAST_HOUSE = 509,
    TEXT_BANK_ROUTE_218 = 510,
    TEXT_BANK_ROUTE_218_GATE_TO_JUBILIFE_CITY = 511,
    TEXT_BANK_ROUTE_218_GATE_TO_CANALAVE_CITY = 512,
    TEXT_BANK_ROUTE_219 = 513,
    TEXT_BANK_ROUTE_221 = 514,
    TEXT_BANK_PAL_PARK_LOBBY = 515,
    TEXT_BANK_ROUTE_221_HOUSE = 516,
    TEXT_BANK_ROUTE_222 = 517,
    TEXT_BANK_ROUTE_222_WEST_HOUSE = 518,
    TEXT_BANK_ROUTE_222_EAST_HOUSE = 519,
    TEXT_BANK_ROUTE_222_GATE_TO_SUNYSHORE_CITY = 520,
    TEXT_BANK_ROUTE_224 = 521,
    TEXT_BANK_ROUTE_225 = 522,
    TEXT_BANK_ROUTE_225_HOUSE = 523,
    TEXT_BANK_UNK_0524 = 524,
    TEXT_BANK_ROUTE_227 = 525,
    TEXT_BANK_ROUTE_227_HOUSE = 526,
    TEXT_BANK_ROUTE_228 = 527,
    TEXT_BANK_ROUTE_228_GATE_TO_ROUTE_226 = 528,
    TEXT_BANK_ROUTE_228_NORTH_HOUSE = 529,
    TEXT_BANK_ROUTE_228_SOUTH_HOUSE = 530,
    TEXT_BANK_ROUTE_229 = 531,
    TEXT_BANK_RANKINGS_MACHINE = 532,
    TEXT_BANK_UNK_0533 = 533,
    TEXT_BANK_SAVE_INFO_WINDOW = 534,
    TEXT_BANK_RIBBON_NAMES = 535,
    TEXT_BANK_DUMMY_0536 = 536,
    TEXT_BANK_BATTLE_ARCADE_SCENE = 537,
    TEXT_BANK_SAFARI_GAME = 538,
    TEXT_BANK_VS_SEEKER = 539,
    TEXT_BANK_SCRATCH_OFF_CARDS_APP = 540,
    TEXT_BANK_SCRATCH_OFF_CARDS = 541,
    TEXT_BANK_UNK_0542 = 542,
    TEXT_BANK_UNK_0543 = 543,
    TEXT_BANK_UNK_0544 = 544,
    TEXT_BANK_SEQ_NAMES = 545,
    TEXT_BANK_UNK_0546 = 546,
    TEXT_BANK_DAY_CARE_COMMON = 547,
    TEXT_BANK_END_CREDITS = 548,
    TEXT_BANK_BATTLE_HALL_SCENE = 549,
    TEXT_BANK_MAIN_MENU_OPTIONS = 550,
    TEXT_BANK_POKEMON_STAT_NAMES = 551,
    TEXT_BANK_COUNTERPART_TALK = 552,
    TEXT_BANK_COUNTERPART_NAMES = 553,
    TEXT_BANK_TWINLEAF_TOWN = 554,
    TEXT_BANK_TWINLEAF_TOWN_RIVAL_HOUSE_1F = 555,
    TEXT_BANK_TWINLEAF_TOWN_RIVAL_HOUSE_2F = 556,
    TEXT_BANK_TWINLEAF_TOWN_PLAYER_HOUSE_1F = 557,
    TEXT_BANK_TWINLEAF_TOWN_PLAYER_HOUSE_2F = 558,
    TEXT_BANK_TWINLEAF_TOWN_NORTHEAST_HOUSE = 559,
    TEXT_BANK_TWINLEAF_TOWN_SOUTHWEST_HOUSE = 560,
    TEXT_BANK_SANDGEM_TOWN = 561,
    TEXT_BANK_SANDGEM_TOWN_MART = 562,
    TEXT_BANK_SANDGEM_TOWN_POKECENTER_1F = 563,
    TEXT_BANK_SANDGEM_TOWN_POKECENTER_2F = 564,
    TEXT_BANK_SANDGEM_TOWN_POKEMON_RESEARCH_LAB = 565,
    TEXT_BANK_SANDGEM_TOWN_COUNTERPART_HOUSE_1F = 566,
    TEXT_BANK_SANDGEM_TOWN_COUNTERPART_HOUSE_2F = 567,
    TEXT_BANK_SANDGEM_TOWN_HOUSE = 568,
    TEXT_BANK_FLOAROMA_TOWN = 569,
    TEXT_BANK_FLOAROMA_TOWN_MART = 570,
    TEXT_BANK_FLOAROMA_TOWN_POKECENTER_1F = 571,
    TEXT_BANK_FLOWER_SHOP = 572,
    TEXT_BANK_FLOAROMA_TOWN_SOUTHEAST_HOUSE = 573,
    TEXT_BANK_FLOAROMA_TOWN_MIDDLE_HOUSE = 574,
    TEXT_BANK_SOLACEON_TOWN = 575,
    TEXT_BANK_SOLACEON_TOWN_MART = 576,
    TEXT_BANK_SOLACEON_TOWN_POKECENTER_1F = 577,
    TEXT_BANK_POKEMON_DAY_CARE = 578,
    TEXT_BANK_SOLACEON_TOWN_NORTHEAST_HOUSE = 579,
    TEXT_BANK_SOLACEON_TOWN_POKEMON_NEWS_PRESS = 580,
    TEXT_BANK_SOLACEON_TOWN_NORTH_HOUSE = 581,
    TEXT_BANK_SOLACEON_TOWN_EAST_HOUSE = 582,
    TEXT_BANK_CELESTIC_TOWN = 583,
    TEXT_BANK_CELESTIC_TOWN_POKECENTER_1F = 584,
    TEXT_BANK_CELESTIC_TOWN_NORTH_HOUSE = 585,
    TEXT_BANK_CELESTIC_TOWN_NORTHWEST_HOUSE = 586,
    TEXT_BANK_CELESTIC_TOWN_NORTHEAST_HOUSE = 587,
    TEXT_BANK_CELESTIC_TOWN_SOUTHWEST_HOUSE = 588,
    TEXT_BANK_CELESTIC_TOWN_CAVE = 589,
    TEXT_BANK_SURVIVAL_AREA = 590,
    TEXT_BANK_SURVIVAL_AREA_MART = 591,
    TEXT_BANK_SURVIVAL_AREA_POKECENTER_1F = 592,
    TEXT_BANK_BATTLEGROUND = 593,
    TEXT_BANK_SURVIVAL_AREA_SOUTH_HOUSE = 594,
    TEXT_BANK_SURVIVAL_AREA_NORTH_HOUSE = 595,
    TEXT_BANK_RESORT_AREA = 596,
    TEXT_BANK_UNUSED_RESORT_AREA_MART = 597,
    TEXT_BANK_RESORT_AREA_POKECENTER_1F = 598,
    TEXT_BANK_RESORT_AREA_RIBBON_SYNDICATE_1F = 599,
    TEXT_BANK_RESORT_AREA_RIBBON_SYNDICATE_2F = 600,
    TEXT_BANK_RESORT_AREA_RIBBON_SYNDICATE_ELEVATOR = 601,
    TEXT_BANK_VILLA = 602,
    TEXT_BANK_VILLA_FURNITURE_NAMES = 603,
    TEXT_BANK_RESORT_AREA_HOUSE = 604,
    TEXT_BANK_BATTLE_VIDEO = 605,
    TEXT_BANK_FLAVOR_NAMES = 606,
    TEXT_BANK_ROWAN_INTRO_TV_APP = 607,
    TEXT_BANK_TIMES_OF_DAY = 608,
    TEXT_BANK_TITLE_SCREEN = 609,
    TEXT_BANK_ABILITY_NAMES = 610,
    TEXT_BANK_ABILITY_NAMES_UPPERCASE = 611,
    TEXT_BANK_ABILITY_DESCRIPTIONS = 612,
    TEXT_BANK_UNK_0613 = 613,
    TEXT_BANK_FRONTIER_TRAINER_MESSAGES = 614,
    TEXT_BANK_TOWN_MAP = 615,
    TEXT_BANK_TRAINER_CARD = 616,
    TEXT_BANK_NPC_TRAINER_MESSAGES = 617,
    TEXT_BANK_NPC_TRAINER_NAMES = 618,
    TEXT_BANK_TRAINER_CLASS_NAMES = 619,
    TEXT_BANK_TRAINER_CLASS_NAMES_WITH_ARTICLES = 620,
    TEXT_BANK_TV_REPORTER_INTERVIEWS = 621,
    TEXT_BANK_TV_PROGRAMS = 622,
    TEXT_BANK_TV_COMMERCIALS = 623,
    TEXT_BANK_POKEMON_TYPE_NAMES = 624,
    TEXT_BANK_UNDERGROUND = 625,
    TEXT_BANK_UNDERGROUND_GOODS = 626,
    TEXT_BANK_UNDERGROUND_GOODS_WITH_ARTICLES = 627,
    TEXT_BANK_UNDERGROUND_ITEMS = 628,
    TEXT_BANK_UNDERGROUND_ITEM_NAMES_WITH_ARTICLES = 629,
    TEXT_BANK_UNDERGROUND_TRAPS = 630,
    TEXT_BANK_UNDERGROUND_TRAP_NAMES_WITH_ARTICLES = 631,
    TEXT_BANK_UNDERGROUND_ANSWERS = 632,
    TEXT_BANK_UNDERGROUND_QUESTIONS = 633,
    TEXT_BANK_UNDERGROUND_COMMON = 634,
    TEXT_BANK_UNION_ROOM = 635,
    TEXT_BANK_UNDERGROUND_NPCS = 636,
    TEXT_BANK_UNDERGROUND_BASE_DECORATION = 637,
    TEXT_BANK_UNDERGROUND_CAPTURE_FLAG = 638,
    TEXT_BANK_UNDERGROUND_PC = 639,
    TEXT_BANK_UNDERGROUND_RECORDS = 640,
    TEXT_BANK_DUMMY_0641 = 641,
    TEXT_BANK_ROUTE_226 = 642,
    TEXT_BANK_ROUTE_226_HOUSE = 643,
    TEXT_BANK_ROUTE_230 = 644,
    TEXT_BANK_MOVE_REMINDER = 645,
    TEXT_BANK_MOVE_DESCRIPTIONS = 646,
    TEXT_BANK_MOVE_NAMES = 647,
    TEXT_BANK_MOVE_NAMES_UPPERCASE = 648,
    TEXT_BANK_PLAZA_EVENT_NAMES = 649,
    TEXT_BANK_UNK_0650 = 650,
    TEXT_BANK_PLAZA_MINIGAME_NAMES = 651,
    TEXT_BANK_PLAZA_ITEM_NAMES = 652,
    TEXT_BANK_UNK_0653 = 653,
    TEXT_BANK_UNK_0654 = 654,
    TEXT_BANK_UNK_0655 = 655,
    TEXT_BANK_UNK_0656 = 656,
    TEXT_BANK_UNK_0657 = 657,
    TEXT_BANK_UNK_0658 = 658,
    TEXT_BANK_UNK_0659 = 659,
    TEXT_BANK_UNK_0660 = 660,
    TEXT_BANK_UNK_0661 = 661,
    TEXT_BANK_UNK_0662 = 662,
    TEXT_BANK_GREETINGS_EN = 663,
    TEXT_BANK_GREETINGS_FR = 664,
    TEXT_BANK_GREETINGS_DE = 665,
    TEXT_BANK_GREETINGS_IT = 666,
    TEXT_BANK_GREETINGS_JP = 667,
    TEXT_BANK_GREETINGS_ES = 668,
    TEXT_BANK_DUMMY_0669 = 669,
    TEXT_BANK_UNK_0670 = 670,
    TEXT_BANK_GTS = 671,
    TEXT_BANK_UNK_0672 = 672,
    TEXT_BANK_UNK_0673 = 673,
    TEXT_BANK_UNK_0674 = 674,
    TEXT_BANK_PAL_PAD = 675,
    TEXT_BANK_COUNTRY_REGION_NAMES_ARGENTINA = 676,
    TEXT_BANK_COUNTRY_REGION_NAMES_AUSTRALIA = 677,
    TEXT_BANK_COUNTRY_REGION_NAMES_BRAZIL = 678,
    TEXT_BANK_COUNTRY_REGION_NAMES_CANADA = 679,
    TEXT_BANK_COUNTRY_REGION_NAMES_CHINA = 680,
    TEXT_BANK_COUNTRY_REGION_NAMES_GERMANY = 681,
    TEXT_BANK_COUNTRY_REGION_NAMES_SPAIN = 682,
    TEXT_BANK_COUNTRY_REGION_NAMES_FINLAND = 683,
    TEXT_BANK_COUNTRY_REGION_NAMES_FRANCE = 684,
    TEXT_BANK_COUNTRY_REGION_NAMES_UNITED_KINGDOM = 685,
    TEXT_BANK_COUNTRY_REGION_NAMES_INDIA = 686,
    TEXT_BANK_COUNTRY_REGION_NAMES_ITALY = 687,
    TEXT_BANK_COUNTRY_REGION_NAMES_JAPAN = 688,
    TEXT_BANK_COUNTRY_REGION_NAMES_NORWAY = 689,
    TEXT_BANK_COUNTRY_REGION_NAMES_POLAND = 690,
    TEXT_BANK_COUNTRY_REGION_NAMES_RUSSIA = 691,
    TEXT_BANK_COUNTRY_REGION_NAMES_SWEDEN = 692,
    TEXT_BANK_COUNTRY_REGION_NAMES_UNITED_STATES = 693,
    TEXT_BANK_COUNTRY_NAMES = 694,
    TEXT_BANK_UNK_0695 = 695,
    TEXT_BANK_UNK_0696 = 696,
    TEXT_BANK_POKEDEX = 697,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_DIAMOND = 698,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_PEARL = 699,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_UNUSED = 700,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_FR = 701,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_DE = 702,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_IT = 703,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_ES = 704,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_JP = 705,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_EN = 706,
    TEXT_BANK_SPECIES_WEIGHT = 707,
    TEXT_BANK_SPECIES_WEIGHT_GIRA = 708,
    TEXT_BANK_SPECIES_HEIGHT = 709,
    TEXT_BANK_SPECIES_HEIGHT_GIRA = 710,
    TEXT_BANK_SPECIES_CATEGORY = 711,
    TEXT_BANK_SPECIES_NAME_WITH_NATDEX_NUMBER_EN = 712,
    TEXT_BANK_SPECIES_NAME_WITH_NATDEX_NUMBER_FR = 713,
    TEXT_BANK_SPECIES_NAME_WITH_NATDEX_NUMBER_DE = 714,
    TEXT_BANK_SPECIES_NAME_WITH_NATDEX_NUMBER_IT = 715,
    TEXT_BANK_SPECIES_NAME_WITH_NATDEX_NUMBER_ES = 716,
    TEXT_BANK_SPECIES_NAME_WITH_NATDEX_NUMBER_JP = 717,
    TEXT_BANK_SPECIES_CATEGORY_EN = 718,
    TEXT_BANK_SPECIES_CATEGORY_FR = 719,
    TEXT_BANK_SPECIES_CATEGORY_DE = 720,
    TEXT_BANK_SPECIES_CATEGORY_IT = 721,
    TEXT_BANK_SPECIES_CATEGORY_ES = 722,
    TEXT_BANK_SPECIES_CATEGORY_JP = 723,
};

enum MessageLoaderMode {
    MSG_LOADER_PRELOAD_ENTIRE_BANK,
    MSG_LOADER_LOAD_ON_DEMAND,
};

struct MsgData *NewMsgDataFromNarc(enum MessageLoaderMode mode, enum NarcID narcID, u32 bankID, u32 heapID);

struct MessageFormat *MessageFormat_New(u32 heapID);

PlayerProfile *sub_02034818(int netId);

BOOL sub_0202FD28(SaveData *saveData, int param1, int *param2, int param3);

void ov05_0221D7AC(UnkStruct_ov10_0221FB28 *param0);

u8 ov05_0221BF08(UnkStruct_ov10_0221FB28 *param0);

u8 ov05_0221BF08(UnkStruct_ov10_0221FB28 *param0) {
    switch (param0->unk_B82) {
    case 0:
        ov05_0221CEB8(param0, GX_BG0_AS_2D, 1);
        ov05_0221D094(param0);
        ov05_0221D140(param0);
        ov05_0221D228();
        {
            BOOL v0;
            int v1;

            v0 = sub_0202FD28(param0->trainerIntroData->setup->saveData, param0->trainerIntroData->heapID, &v1, 0);
            param0->unk_BCC = v1;
        }
        {
            int v2;
            PlayerProfile *v3;

            param0->unk_BD0 = 0;

            for (v2 = 0; v2 < (7 + 1); v2++) {
                v3 = sub_02034818(v2);

                if (v3 != 0L) {
                    if (PlayerProfile_GetVersion(v3) == 0) {
                        param0->unk_BD0 = 1;
                        break;
                    }
                }
            }
        }
        break;
    case 1:
        ov05_0221D6C4(param0, -32, -16);
        ov05_0221D7AC(param0);
        sub_0203A880();
        break;
    case 2:
        App_StartScreenFade(0, param0->trainerIntroData->heapID);
        Main_SetVBlankIntrCB(ov05_0221CE88, param0);
        param0->unk_B82 = 0;
        param0->unk_B7C = 4;
        param0->unk_B7D = 2;
        param0->unk_B74 = 12;
        param0->unk_BAC = NewMsgDataFromNarc(MSG_LOADER_PRELOAD_ENTIRE_BANK, (enum NarcID)27, (enum TextBank)638, param0->trainerIntroData->heapID);
        param0->unk_BB0 = MessageFormat_New(param0->trainerIntroData->heapID);
        param0->unk_BB4 = String_New(2 * 160, param0->trainerIntroData->heapID);
        param0->unk_BBC = 1;

        return 1;
    }

    param0->unk_B82++;
    return 0;
}

BOOL IsPaletteFadeFinished(void);

u8 ov05_0221C018(UnkStruct_ov10_0221FB28 *param0);

u8 ov05_0221C018(UnkStruct_ov10_0221FB28 *param0) {
    if (IsPaletteFadeFinished() == 0) {
        return 0;
    }

    if (param0->unk_B81 == param0->unk_B82) {
        param0->unk_B82 = 0;
        return 1;
    }

    param0->unk_B82++;
    return 0;
}
