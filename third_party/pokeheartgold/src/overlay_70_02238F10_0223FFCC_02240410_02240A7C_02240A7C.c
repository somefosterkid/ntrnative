#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_70_gtsapplication_state.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gts_player_data.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct Bag;

struct BagItem;

struct BallCapsule;

struct BallSeal;

struct BgConfig;

struct BoxPokemon;

struct Camera;

struct CameraAngle;

struct CameraLookAt;

struct CameraPerspectiveProjection;

struct CameraPositionHistory;

struct ChatotCry;

struct ListMenuCursor;

struct MailMessage;

struct EncodingSeed;

struct G3DPipelineBuffers;

struct GTSApplicationState;

struct GTSBoxPokemonCriteria;

struct GTSPlayerData;

struct GTSPokemonCriteria;

struct GameStats;

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

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct NARC;

struct Options;

struct PCBoxes;

struct PaletteAnimator;

struct PaletteBuffer;

struct PaletteData;

struct PaletteFadeControl;

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

struct PokemonSpriteShadow;

struct PokepicTemplate;

struct PokemonSpriteTransforms;

struct PokemonSummary;

struct Poketch;

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

struct String;

struct ListMenuItem;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskManager;

struct SysInfo;

struct PlayerProfile;

struct TransformData;

struct SaveVarsFlags;

struct WiFiHistory;

struct WiFiList;

struct Window;

typedef struct WiFiList WiFiList;

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

typedef struct Options {
    u16 textSpeed : 4;
    u16 soundMode : 2;
    u16 battleStyle : 1;
    u16 battleScene : 1;
    u16 buttonMode : 2;
    u16 frame : 5;
    u16 : 1;
} Options;

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

void Heap_Free(void *ptr);


typedef struct ChatotCry {
    BOOL valid;
    s8 audioData[1000];
} ChatotCry;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;


typedef u16 charcode_t;

typedef struct String {
    u16 maxsize;
    u16 size;
    u32 magic;
    u16 data[0];
} String;


typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;









typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

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

typedef struct PaletteBuffer {
    u16 *unfaded;
    u16 *faded;
    u32 size;
    PaletteFadeControl selected;
} PaletteBuffer;

typedef struct PaletteData {
    PaletteBuffer buffers[14];
    u16 selectedFlag : 2;
    u16 selectedBuffer : 14;
    u16 transparentBit : 14;
    u16 callbackFlag : 1;
    u16 autoTransparent : 1;
    u8 forceExit;
    u8 unused[3];
} PaletteData;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);


BOOL Party_AddMon(Party *party, Pokemon *mon);

int Party_GetCount(const Party *party);

typedef struct Pokepic Pokepic;

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);


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

typedef struct PokepicManager {
    Pokepic sprites[4];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy plttProxy;
    enum HeapID heapID;
    u32 charBaseAddr;
    u32 charSize;
    u32 plttBaseAddr;
    u32 plttSize;
    u8 *charRawData;
    u16 *plttRawData;
    u16 *plttRawDataUnfaded;
    NNSG2dCharacterData charData;
    NNSG2dPaletteData plttData;
    u8 dummy330;
    u8 needLoadChar;
    u8 needLoadPltt;
    u8 excludeIdentity;
    u32 hideShadows;
} PokepicManager;

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

typedef struct TransformData TransformData;

typedef struct PokemonAnim PokemonAnim;

struct TransformData {
    BOOL active;
    int vars[8];
    int *transformMemberPtr;
    int *animMemberPtr;
    u8 calcType;
    u8 startDelay;
    u8 pad_2E[2];
    int originalValue;
    int dummy_34;
    int dummy_38;
    int offsetX;
    int offsetY;
    int scaleX;
    int scaleY;
    int rotationZ;
    void *func;
};

struct PokemonAnim {
    Pokepic *sprite;
    SysTask *task;
    void *scriptData;
    u32 *scriptPtr;
    BOOL active;
    int animNum;
    int waitFrame;
    int endAnim;
    BOOL completed;
    int vars[8];
    int commandCount;
    int loopMax;
    int loopCounter;
    u32 *loopStart;
    int startDelay;
    int originalX;
    int originalY;
    int translateX;
    int translateY;
    int offsetX;
    int offsetY;
    int scaleX;
    int scaleY;
    int rotationZ;
    struct { BOOL active; int vars[8]; int *transformMemberPtr; int *animMemberPtr; u8 calcType; u8 startDelay; u8 unk_30_pad[2]; int originalValue; int dummy_34; int dummy_38; int offsetX; int offsetY; int scaleX; int scaleY; int rotationZ; void *func; } transforms[4];
    u8 flipSprite;
    u8 waitForTransform;
    u8 yNormalization;
    u8 fadeActive;
};

typedef struct PokemonAnimManager {
    PokemonAnim *anims;
    enum HeapID heapID;
    u8 flipSprite;
    u8 animCount;
} PokemonAnimManager;

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

typedef struct SpriteResourceCollection {
    GF_2DGfxRawResMan *collection;
    SpriteResource *resources;
    void *capacity;
    void *count;
    GfGfxResType type;
} SpriteResourceCollection;

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

u32 GetMonData(Pokemon *mon, int attr, void *dest);

void SetMonData(Pokemon *mon, int attr, const void *value);

BoxPokemon *Mon_GetBoxMon(Pokemon *mon);

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

typedef struct GameStats {
    u32 recordsU32[71];
    u16 recordsU16[77];
    u16 padding;
    EncodingSeed seed;
} GameStats;

typedef struct GlobalTrade {
    u8 storedPokemon[sizeof(Pokemon)];
    u16 pokemonListed;
    u16 neverSet_EE;
    u32 depositTradeDatestamp;
    u32 searchTradeDatestamp;
} GlobalTrade;

void sub_0202DB5C(GlobalTrade *globalTrade, s16 listed);

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

BOOL PCStorage_PlaceMonInBoxFirstEmptySlot(PCBoxes *pcBoxes, u32 boxID, BoxPokemon *boxMon);

BOOL PCStorage_FindFirstEmptySlot(const PCBoxes *pcBoxes, int *boxIndexDest, int *monPosInBoxDest);

typedef struct GameTime {
    BOOL canary;
    RTCDate date;
    RTCTime time;
    u32 day;
    s64 startTimestamp;
    s64 firstCompletionTimestamp;
    u32 penaltyInMinutes;
} GameTime;

typedef struct SysInfo {
    s64 rtcOffset;
    u8 macAddress[6];
    u8 ownerBirthMonth;
    u8 ownerBirthDayOfMonth;
    GameTime gameTime;
    u8 isMysteryGiftUnlocked;
    u8 padding_49[3];
    s32 dwcProfileId;
    u32 unused_50[3];
} SysInfo;

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

typedef struct MessageFormat {
    u32 maxArgs;
    enum HeapID heapID;
    StringTemplateArg *args;
    String *templateBuf;
} MessageFormat;

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

typedef u32 TextColor;

typedef struct PokemonSummary PokemonSummary;

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

typedef struct {
    enum HeapID heapID;
    int unk_04;
} UnkStruct_0207C894;

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

typedef struct OverlayManager OverlayManager;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct OverlayManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} OverlayManagerTemplate;

struct OverlayManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    OverlayManager *parent;
    OverlayManager *child;
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

typedef struct ListMenuCursor {
    TextColor color;
    String *string;
} ListMenuCursor;

typedef struct ListMenuItem {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
} ListMenuItem;

typedef struct ListMenu ListMenu;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct ListMenuTemplate {
    const ListMenuItem *choices;
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

typedef struct ListMenu2DTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} ListMenu2DTemplate;

typedef struct ListMenu2D {
    ListMenu2DTemplate template;
    ListMenuCursor *cursor;
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
} ListMenu2D;

typedef struct GTSBoxPokemonCriteria {
    GTSPokemonCriteria criteria[(5 * 6)];
} GTSBoxPokemonCriteria;

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

typedef struct Location {
    enum MapHeaderID mapHeaderID;
    int warpId;
    int x;
    int z;
    int faceDirection;
} Location;

static inline void Location_Set(Location *location, enum MapHeaderID mapHeaderID, int warpId, int x, int z, int faceDirection) {
    location->mapHeaderID = mapHeaderID;
    location->warpId = warpId;
    location->x = x;
    location->z = z;
    location->faceDirection = faceDirection;
}

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

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

void UpdatePokedexWithReceivedSpecies(SaveData *saveData, const Pokemon *mon);

enum VarFlag {
    FLAG_UNUSED_0x0000 = 0,
    MAP_LOCAL_FLAGS_START = 1,
    FLAG_MAP_LOCAL_0x01 = 1,
    FLAG_MAP_LOCAL_0x02 = 2,
    FLAG_MAP_LOCAL_0x03 = 3,
    FLAG_MAP_LOCAL_0x04 = 4,
    FLAG_MAP_LOCAL_0x05 = 5,
    FLAG_MAP_LOCAL_0x06 = 6,
    FLAG_MAP_LOCAL_0x07 = 7,
    FLAG_MAP_LOCAL_0x08 = 8,
    FLAG_MAP_LOCAL_0x09 = 9,
    FLAG_MAP_LOCAL_0x0A = 10,
    FLAG_MAP_LOCAL_0x0B = 11,
    FLAG_MAP_LOCAL_0x0C = 12,
    FLAG_MAP_LOCAL_0x0D = 13,
    FLAG_MAP_LOCAL_0x0E = 14,
    FLAG_MAP_LOCAL_0x0F = 15,
    FLAG_MAP_LOCAL_0x10 = 16,
    FLAG_MAP_LOCAL_0x11 = 17,
    FLAG_MAP_LOCAL_0x12 = 18,
    FLAG_MAP_LOCAL_0x13 = 19,
    FLAG_MAP_LOCAL_0x14 = 20,
    FLAG_MAP_LOCAL_0x15 = 21,
    FLAG_MAP_LOCAL_0x16 = 22,
    FLAG_MAP_LOCAL_0x17 = 23,
    FLAG_MAP_LOCAL_0x18 = 24,
    FLAG_MAP_LOCAL_0x19 = 25,
    FLAG_MAP_LOCAL_0x1A = 26,
    FLAG_MAP_LOCAL_0x1B = 27,
    FLAG_MAP_LOCAL_0x1C = 28,
    FLAG_MAP_LOCAL_0x1D = 29,
    FLAG_MAP_LOCAL_0x1E = 30,
    FLAG_MAP_LOCAL_SAVE_EXTRA_BLOCK = 31,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_1 = 32,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_2 = 33,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_3 = 34,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_4 = 35,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_5 = 36,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_6 = 37,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_7 = 38,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_8 = 39,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_9 = 40,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_10 = 41,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_11 = 42,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_12 = 43,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_13 = 44,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_14 = 45,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_15 = 46,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_16 = 47,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_17 = 48,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_18 = 49,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_19 = 50,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_20 = 51,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_21 = 52,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_22 = 53,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_23 = 54,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_24 = 55,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_25 = 56,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_26 = 57,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_27 = 58,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_28 = 59,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_29 = 60,
    FLAG_MAP_LOCAL_HIDE_OBSTACLE_30 = 61,
    FLAG_MAP_LOCAL_0x3E = 62,
    FLAG_MAP_LOCAL_0x3F = 63,
    FLAG_MAP_LOCAL_0x40 = 64,
    MAP_LOCAL_FLAGS_END = 64,
    FLAG_UNK_0x0041 = 65,
    FLAG_UNK_0x0042 = 66,
    FLAG_UNK_0x0043 = 67,
    FLAG_UNK_0x0044 = 68,
    FLAG_UNK_0x0045 = 69,
    FLAG_UNK_0x0046 = 70,
    FLAG_UNK_0x0047 = 71,
    FLAG_UNK_0x0048 = 72,
    FLAG_UNK_0x0049 = 73,
    FLAG_UNK_0x004A = 74,
    FLAG_UNK_0x004B = 75,
    FLAG_UNK_0x004C = 76,
    FLAG_UNK_0x004D = 77,
    FLAG_UNK_0x004E = 78,
    FLAG_UNK_0x004F = 79,
    FLAG_UNK_0x0050 = 80,
    FLAG_UNK_0x0051 = 81,
    FLAG_UNK_0x0052 = 82,
    FLAG_UNK_0x0053 = 83,
    FLAG_UNK_0x0054 = 84,
    FLAG_UNK_0x0055 = 85,
    FLAG_UNK_0x0056 = 86,
    FLAG_UNK_0x0057 = 87,
    FLAG_UNK_0x0058 = 88,
    FLAG_UNK_0x0059 = 89,
    FLAG_UNK_0x005A = 90,
    FLAG_UNK_0x005B = 91,
    FLAG_UNK_0x005C = 92,
    FLAG_UNK_0x005D = 93,
    FLAG_UNK_0x005E = 94,
    FLAG_UNK_0x005F = 95,
    FLAG_UNK_0x0060 = 96,
    FLAG_UNK_0x0061 = 97,
    FLAG_UNK_0x0062 = 98,
    FLAG_UNK_0x0063 = 99,
    FLAG_UNK_0x0064 = 100,
    FLAG_UNK_0x0065 = 101,
    FLAG_UNK_0x0066 = 102,
    FLAG_UNK_0x0067 = 103,
    FLAG_UNK_0x0068 = 104,
    FLAG_POKECENTER_GOLD_TRAINER_CARD_SEEN = 105,
    FLAG_POKECENTER_IDENTIFIED_POKERUS = 106,
    FLAG_RECEIVED_SUPER_ROD = 107,
    FLAG_RECEIVED_ROUTE_201_POTION = 108,
    FLAG_UNUSED_0x006D = 109,
    FLAG_TALKED_TO_RIVAL_HOUSE_RIVAL_MOM_RIVAL_LEFT = 110,
    FLAG_UNUSED_0x006F = 111,
    FLAG_CHOSE_UNION_ROOM_APPEARANCE = 112,
    FLAG_DUMMY_0x0071 = 113,
    FLAG_TALKED_TO_SCRATCH_OFF_CARDS_ATTENDANT = 114,
    FLAG_DUMMY_0x0073 = 115,
    FLAG_RECEIVED_GARDENIA_TM86 = 116,
    FLAG_RECEIVED_ROARK_TM76 = 117,
    FLAG_RECEIVED_UNUSED_ETERNA_CITY_CONDOMINIUMS_4F_TM04 = 118,
    FLAG_USED_ROTOM_APPLIANCE = 119,
    FLAG_TALKED_TO_PASTORIA_CITY_MIDDLE_HOUSE_BUG_CATCHER = 120,
    FLAG_RECEIVED_EXPLORER_KIT = 121,
    FLAG_ROARK_RETURNED_TO_OREBURGH_GYM = 122,
    FLAG_USED_DEFOG_IN_ROUTE_209_LOST_TOWER_5F = 123,
    FLAG_RECEIVED_UNUSED_OREBURGH_CITY_NORTHWEST_HOUSE_3F_HARD_STONE = 124,
    FLAG_RECEIVED_FANTINA_TM65 = 125,
    FLAG_RECEIVED_UNUSED_JUBILIFE_CITY_CONDOMINIUMS_4F_FOCUS_BAND = 126,
    FLAG_DUMMY_0x007F = 127,
    FLAG_RECEIVED_FLOWER_SHOP_SPRAYDUCK = 128,
    FLAG_TEAM_GALACTIC_LEFT_ETERNA_BUILDING = 129,
    FLAG_RECEIVED_BICYCLE = 130,
    FLAG_RECEIVED_FLOAROMA_TOWN_MIDDLE_HOUSE_TM88 = 131,
    FLAG_RECEIVED_OLD_ROD = 132,
    FLAG_TRADED_FOR_KAZZA_ABRA = 133,
    FLAG_TRADED_FOR_CHARAP_CHATOT = 134,
    FLAG_TALKED_TO_MOM = 135,
    FLAG_ENTERED_DRESSING_ROOM = 136,
    FLAG_OBTAINED_ALL_BATTLE_FRONTIER_GOLD_PRINTS = 137,
    FLAG_TALKED_TO_OREBURGH_CITY_RIVAL = 138,
    FLAG_TALKED_TO_ETERNA_FOREST_CHERYL = 139,
    FLAG_RECEIVED_HEARTHOME_CITY_SOUTHEAST_HOUSE_2F_SHELL_BELL = 140,
    FLAG_RECEIVED_HEARTHOME_CITY_POKEMON_FAN_CLUB_POFFIN_CASE = 141,
    FLAG_MAP_LOCAL_REMOVE_OBJECT = 142,
    FLAG_RECEIVED_PARCEL = 143,
    FLAG_HAS_POKEDEX = 144,
    FLAG_TALKED_TO_ROUTE_205_SOUTH_LITTLE_GIRL = 145,
    FLAG_RECEIVED_BYRON_TM91 = 146,
    FLAG_RECEIVED_HM06 = 147,
    FLAG_UNUSED_0x0094 = 148,
    FLAG_UNUSED_0x0095 = 149,
    FLAG_UNUSED_0x0096 = 150,
    FLAG_RECEIVED_VEILSTONE_CITY_NORTHEAST_HOUSE_PORYGON = 151,
    FLAG_CAUGHT_ROAMING_LEGENDARY_BIRDS = 152,
    FLAG_ACTIVATED_ROAMING_LEGENDARY_BIRDS = 153,
    FLAG_RECEIVED_SUNYSHORE_CITY_HM07 = 154,
    FLAG_AGREED_WITH_LOOKER_TO_ENTER_HIDEOUT = 155,
    FLAG_RECEIVED_WAKE_TM55 = 156,
    FLAG_RECEIVED_MAYLENE_TM60 = 157,
    FLAG_RECEIVED_CANDICE_TM72 = 158,
    FLAG_OBTAINED_FLOAROMA_MEADOW_WORKS_KEY = 159,
    FLAG_DEFEATED_FLOAROMA_MEADOW_GRUNTS = 160,
    FLAG_RECEIVED_ROUTE_209_LOST_TOWER_5F_SPELL_TAG = 161,
    FLAG_RECEIVED_GOOD_ROD = 162,
    FLAG_RECEIVED_PASTORIA_CITY_OBSERVATORY_GATE_1F_MATCHUPCHECKER = 163,
    FLAG_UNUSED_0x00A4 = 164,
    FLAG_COULD_NOT_RECEIVE_OREBURGH_MINE_B1F_FLAME_PLATE = 165,
    FLAG_DELIVERED_OLD_CHARM = 166,
    FLAG_EXAMINED_CELESTIC_TOWN_CAVE_PAINTING = 167,
    FLAG_LAKE_VALOR_EXPLODED = 168,
    FLAG_TALKED_TO_LAKE_ACUITY_LOW_WATER_RIVAL = 169,
    FLAG_DUMMY_0x00AA = 170,
    FLAG_RECEIVED_LOCAL_DEX_DIPLOMA = 171,
    FLAG_RECEIVED_NATIONAL_DEX_DIPLOMA = 172,
    FLAG_DEFEATED_GALACTIC_HQ_CONTROL_ROOM_SATURN = 173,
    FLAG_UNUSED_0x00AE = 174,
    FLAG_POKEMON_LEAGUE_DOOR_GUARD_MOVED_AWAY = 175,
    FLAG_DEFEATED_AARON = 176,
    FLAG_DEFEATED_BERTHA = 177,
    FLAG_DEFEATED_FLINT = 178,
    FLAG_DEFEATED_LUCIAN = 179,
    FLAG_DEFEATED_CYNTHIA = 180,
    FLAG_TALKED_TO_VEILSTONE_CITY_NORTHEAST_HOUSE_POKEFAN_M = 181,
    FLAG_RECEIVED_VOLKNER_TM57 = 182,
    FLAG_DUMMY_0x00B7 = 183,
    FLAG_TALKED_TO_LAKE_VERITY_LOW_WATER_PROF_ROWAN = 184,
    FLAG_TALKED_TO_VEILSTONE_STORE_B1F_PROF_ROWAN = 185,
    FLAG_TEAM_GALACTIC_LEFT_LAKE_VERITY = 186,
    FLAG_TALKED_TO_UNION_ROOM_TEALA = 187,
    FLAG_RECEIVED_VEILSTONE_CITY_SOUTHEAST_HOUSE_COIN_CASE = 188,
    FLAG_TALKED_TO_UNDERGROUND_MAN_HOUSE_SCIENTIST_M = 189,
    FLAG_SPOKEN_TO_BATTLE_HALL_RECORD_KEEPER = 190,
    FLAG_RECEIVED_JUBILIFE_CITY_CONDOMINIUMS_1F_QUICK_CLAW = 191,
    FLAG_RECEIVED_OREBURGH_CITY_NORTHWEST_HOUSE_2F_DUSK_BALL = 192,
    FLAG_RECEIVED_OREBURGH_CITY_NORTH_HOUSE_2F_HEAL_BALL = 193,
    FLAG_RECEIVED_ETERNA_CITY_CONDOMINIUMS_2F_TM67 = 194,
    FLAG_TALKED_TO_GLOBAL_TERMINAL_1F_RECEPTIONIST_GTS = 195,
    FLAG_TALKED_TO_VEILSTONE_CITY_PRIZE_EXCHANGE_GUITARIST = 196,
    FLAG_RECEIVED_ROUTE_204_NORTH_TM78 = 197,
    FLAG_RECEIVED_ROUTE_211_EAST_TM77 = 198,
    FLAG_RECEIVED_ROUTE_210_SOUTH_TM51 = 199,
    FLAG_VISITED_BATTLEGROUND = 200,
    FLAG_RECEIVED_CANALAVE_CITY_SOUTHEAST_HOUSE_TM48 = 201,
    FLAG_RECEIVED_GRAND_LAKE_ROUTE_213_NORTHWEST_HOUSE_TM92 = 202,
    FLAG_RECEIVED_SURVIVAL_AREA_SOUTH_HOUSE_TM42 = 203,
    FLAG_RECEIVED_VEILSTONE_CITY_TM63 = 204,
    FLAG_RECEIVED_ROUTE_215_TM66 = 205,
    FLAG_RECEIVED_ROUTE_222_TM56 = 206,
    FLAG_RECEIVED_ALL_GLOBAL_TERMINAL_1F_BACKDROPS = 207,
    FLAG_CAUGHT_DIALGA = 208,
    FLAG_CAUGHT_PALKIA = 209,
    FLAG_RECEIVED_SOLACEON_RUINS_ROOM_2_GREEN_SHARD = 210,
    FLAG_TALKED_TO_PAL_PARK_LOBBY_RECEPTIONIST = 211,
    FLAG_UNUSED_0x00D4 = 212,
    FLAG_RECEIVED_FIGHT_AREA_MART_SCOPE_LENS = 213,
    FLAG_ARRESTED_CHARON_STARK_MOUNTAIN = 214,
    FLAG_BATTLED_STARK_MOUNTAIN_ROOM_3_HEATRAN = 215,
    FLAG_TALKED_TO_POKEMON_NEWS_PRESS_GYM_GUIDE = 216,
    FLAG_RECEIVED_ROUTE_225_HOUSE_FRESH_WATER = 217,
    FLAG_RECEIVED_ROUTE_229_NUGGET = 218,
    FLAG_MR_FUEGO_ASKED_FOR_TRADE = 219,
    FLAG_COULD_NOT_RECEIVE_POKEMON_NEWS_PRESS_REWARD = 220,
    FLAG_TALKED_TO_STARK_MOUNTAIN_ROOM_2_BUCK = 221,
    FLAG_RECEIVED_ROUTE_217_WEST_HOUSE_ICICLE_PLATE = 222,
    FLAG_TALKED_TO_VICTORY_ROAD_1F_ROOM_2_MARLEY = 223,
    FLAG_TALKED_TO_WAYWARD_CAVE_1F_MIRA = 224,
    FLAG_TALKED_TO_IRON_ISLAND_B2F_LEFT_ROOM_RILEY = 225,
    FLAG_COULD_NOT_RECEIVE_RIOLU_EGG = 226,
    FLAG_TRAVELED_WITH_CHERYL = 227,
    FLAG_TRAVELED_WITH_MIRA = 228,
    FLAG_RECEIVED_RIOLU_EGG_FROM_RILEY = 229,
    FLAG_TRAVELED_WITH_MARLEY = 230,
    FLAG_BUCK_LEFT_BATTLEGROUND = 231,
    FLAG_DID_NOT_LOAN_HM_DEFOG = 232,
    FLAG_TALKED_TO_PASTORIA_CITY_EAST_HOUSE_MOVE_MANIAC = 233,
    FLAG_RIVAL_LEFT_HOME = 234,
    FLAG_DUMMY_0x00EB = 235,
    FLAG_UNUSED_0x00EC = 236,
    FLAG_RECEIVED_COUPON_1 = 237,
    FLAG_RECEIVED_COUPON_2 = 238,
    FLAG_RECEIVED_COUPON_3 = 239,
    FLAG_TALKED_TO_COUNTERPART_SISTER_WITH_NATIONAL_DEX = 240,
    FLAG_TALKED_TO_TRAINERS_SCHOOL_RIVAL = 241,
    FLAG_RECEIVED_FASHION_CASE = 242,
    FLAG_RECEIVED_POKETCH = 243,
    FLAG_TRADED_FOR_GASPAR_HAUNTER = 244,
    FLAG_TRADED_FOR_FOPPA_MAGIKARP = 245,
    FLAG_ENABLED_POKEDEX_LANGUAGE_DETECTION = 246,
    FLAG_TALKED_TO_JULIA_SUNYSHORE_CITY_NORTHEAST_HOUSE = 247,
    FLAG_MOM_WARNED_PLAYER = 248,
    FLAG_TALKED_TO_DR_FOOTSTEP = 249,
    FLAG_JOINED_RIBBON_SYNDICATE = 250,
    FLAG_SKIP_POKEMON_MANSION_OFFICE_BOOK_SEEN_MANAPHY_CHECK = 251,
    FLAG_COULD_NOT_RECEIVE_UNDERGROUND_MAN_REWARD = 252,
    FLAG_TALKED_TO_GRAND_LAKE_ROUTE_213_LOBBY_LOOKER = 253,
    FLAG_STORED_POKEMON_AT_DAY_CARE = 254,
    FLAG_TALKED_TO_STATS_JUDGE = 255,
    FLAG_TALKED_TO_ROUTE_210_GRANDMA_WILMA = 256,
    FLAG_UNUSED_0x0101 = 257,
    FLAG_PASTORIA_CITY_GRUNT_M_MOVED_EAST = 258,
    FLAG_TALKED_TO_PASTORIA_CITY_GRUNT_M = 259,
    FLAG_ROUTE_213_GRUNT_M_LEFT = 260,
    FLAG_RECEIVED_VEILSTONE_STORE_5F_STICKY_BARB = 261,
    FLAG_TALKED_TO_VALOR_LAKEFRONT_GRUNT_M = 262,
    FLAG_USED_SECRETPOTION = 263,
    FLAG_TALKED_TO_ROUTE_228_NORTH_HOUSE_EXPERT_M = 264,
    FLAG_RECEIVED_OREBURGH_CITY_SUPER_POTION = 265,
    FLAG_RECEIVED_OREBURGH_CITY_EAST_HOUSE_2F_GREAT_BALL = 266,
    FLAG_RECEIVED_ROUTE_206_CYCLING_ROAD_NORTH_GATE_EXP_SHARE = 267,
    FLAG_DEFEATED_TRAINERS_SCHOOL_SCHOOL_KID_HARRISON = 268,
    FLAG_DEFEATED_TRAINERS_SCHOOL_SCHOOL_KID_CHRISTINE = 269,
    FLAG_USED_STORAGE_KEY = 270,
    FLAG_UNLOCKED_VALLEY_WINDWORKS_DOOR = 271,
    FLAG_SHOWN_COMPLETE_LOCAL_DEX = 272,
    FLAG_HAS_ACTIVE_UNDERGROUND_MISSION = 273,
    FLAG_RECEIVED_TRAINERS_SCHOOL_POTION = 274,
    FLAG_ACCEPTED_UNDERGROUND_MAN_AS_MENTOR = 275,
    FLAG_UNUSED_0x0114 = 276,
    FLAG_TALKED_TO_ROUTE_202_COUNTERPART = 277,
    FLAG_RECEIVED_ROUTE_209_LOST_TOWER_5F_CLEANSE_TAG = 278,
    FLAG_RECEIVED_SOLACEON_TOWN_EAST_HOUSE_SEAL_CASE = 279,
    FLAG_TALKED_TO_ROUTE_213_GRUNT_M = 280,
    FLAG_RECEIVED_ETERNA_CITY_SOUTH_HOUSE_UPGRADE = 281,
    FLAG_AWAKENED_REGIGIGAS = 282,
    FLAG_CAUGHT_REGIGIGAS = 283,
    FLAG_RECEIVED_PASTORIA_CITY_MIDDLE_HOUSE_MACHO_BRACE = 284,
    FLAG_RECEIVED_IRON_ISLAND_HOUSE_METAL_COAT = 285,
    FLAG_CAUGHT_ARCEUS = 286,
    FLAG_MET_CRESSELIA = 287,
    FLAG_CAUGHT_HEATRAN = 288,
    FLAG_CAUGHT_GIRATINA = 289,
    FLAG_UNUSED_0x0122 = 290,
    FLAG_CAUGHT_SHAYMIN = 291,
    FLAG_TALKED_TO_CELESTIC_TOWN_CAVE_CYNTHIA = 292,
    FLAG_TALKED_TO_BATTLEGROUND_BUCK = 293,
    FLAG_CAUGHT_AZELF = 294,
    FLAG_CAUGHT_UXIE = 295,
    FLAG_RECEIVED_PASTORIA_CITY_ACCESSORY_STARTER_MASK = 296,
    FLAG_RECEIVED_VEILSTONE_STORE_1F_ACCESSORY_STARTER_MASK = 297,
    FLAG_RECEIVED_ETERNA_FOREST_OUTSIDE_ACCESSORY_BIG_TREE = 298,
    FLAG_RECEIVED_ROUTE_206_CYCLING_ROAD_SOUTH_GATE_ACCESSORY_FLAG = 299,
    FLAG_WOKE_UP_CANALAVE_CITY_SAILOR_ELDRITCH_HOUSE_LITTLE_BOY = 300,
    FLAG_WROTE_ON_ROUTE_224_TABLET = 301,
    FLAG_RECEIVED_GAME_CORNER_20_COINS_OLD_MAN = 302,
    FLAG_RECEIVED_GAME_CORNER_50_COINS_FARMER = 303,
    FLAG_RECEIVED_GAME_CORNER_TM64 = 304,
    FLAG_RECEIVED_HEARTHOME_CITY_NORTHWEST_HOUSE_EEVEE = 305,
    FLAG_SPEAR_PILLAR_IS_DISTORTED = 306,
    FLAG_TALKED_TO_CANALAVE_CITY_SAILOR_ELDRITCH_HOUSE_LITTLE_BOY = 307,
    FLAG_RECEIVED_CELESTIC_TOWN_NORTHWEST_HOUSE_CHOICE_SPECS = 308,
    FLAG_RECEIVED_CELESTIC_TOWN_NORTHWEST_HOUSE_BLACKGLASSES = 309,
    FLAG_RECEIVED_CELESTIC_TOWN_NORTHWEST_HOUSE_WISE_GLASSES = 310,
    FLAG_COULD_NOT_RECEIVE_ROUTE_221_HOUSE_REWARD = 311,
    FLAG_RECEIVED_FUEGO_IRONWORKS_BUILDING_STAR_PIECE = 312,
    FLAG_RECEIVED_POKEMON_MANSION_MAIDS_ROOM_SOOTHE_BELL = 313,
    FLAG_RECEIVED_GRAND_LAKE_VALOR_LAKEFRONT_EAST_HOUSE_WHITE_FLUTE = 314,
    FLAG_RECEIVED_ROUTE_217_NORTHEAST_HOUSE_SPELL_TAG = 315,
    FLAG_TRAVELED_TO_NEWMOON_ISLAND = 316,
    FLAG_RECEIVED_JUBILIFE_TV_2F_ACCESSORY_STARTER_MASK = 317,
    FLAG_DEFEATED_COMMANDER_SATURN_VALOR_CAVERN = 318,
    FLAG_RECEIVED_ROUTE_208_ODD_KEYSTONE = 319,
    FLAG_RECEIVED_PASTORIA_CITY_NORTHEAST_HOUSE_RED_SCARF = 320,
    FLAG_RECEIVED_PASTORIA_CITY_NORTHEAST_HOUSE_BLUE_SCARF = 321,
    FLAG_RECEIVED_PASTORIA_CITY_NORTHEAST_HOUSE_PINK_SCARF = 322,
    FLAG_RECEIVED_PASTORIA_CITY_NORTHEAST_HOUSE_GREEN_SCARF = 323,
    FLAG_RECEIVED_PASTORIA_CITY_NORTHEAST_HOUSE_YELLOW_SCARF = 324,
    FLAG_UNLOCKED_DIALGA_PALKIA_SPEAR_PILLAR = 325,
    FLAG_STARTED_COUNTERPART_POKE_RADAR_TUTORIAL = 326,
    FLAG_DUMMY_0x0147 = 327,
    FLAG_DUMMY_0x0148 = 328,
    FLAG_CAUGHT_OLD_CHATEAU_ROTOM = 329,
    FLAG_RECEIVED_HEARTHOME_CITY_GLOBE = 330,
    FLAG_RECEIVED_HEARTHOME_CITY_GYM_STATUE = 331,
    FLAG_RECEIVED_HEARTHOME_CITY_CUTE_CUP = 332,
    FLAG_RECEIVED_HEARTHOME_CITY_COOL_CUP = 333,
    FLAG_RECEIVED_HEARTHOME_CITY_BEAUTY_CUP = 334,
    FLAG_RECEIVED_HEARTHOME_CITY_TOUGH_CUP = 335,
    FLAG_RECEIVED_HEARTHOME_CITY_CLEVER_CUP = 336,
    FLAG_RECEIVED_HEARTHOME_CITY_BLUE_CRYSTAL = 337,
    FLAG_RECEIVED_HEARTHOME_CITY_PINK_CRYSTAL = 338,
    FLAG_RECEIVED_HEARTHOME_CITY_RED_CRYSTAL = 339,
    FLAG_RECEIVED_HEARTHOME_CITY_YELLOW_CRYSTAL = 340,
    FLAG_VEILSTONE_CITY_GRUNT_DROPPED_STORAGE_KEY = 341,
    FLAG_BLOCK_PASTORIA_CITY_CROAGUNK_EVENT = 342,
    FLAG_SAILED_TO_BATTLE_ZONE = 343,
    FLAG_CAUGHT_DARKRAI = 344,
    FLAG_TALKED_TO_CONTEST_HALL_LOBBY_RICH_BOY = 345,
    FLAG_RECEIVED_CONTEST_HALL_LOBBY_MILD_POFFIN = 346,
    FLAG_RECEIVED_ALL_HEARTHOME_CITY_GOODS = 347,
    FLAG_TALKED_TO_MOM_ABOUT_RIVAL_SNOWPOINT_CITY = 348,
    FLAG_TALKED_TO_MOM_ABOUT_NATIONAL_DEX_PROGRESS = 349,
    FLAG_GALACTIC_LEFT_LAKE_VALOR = 350,
    FLAG_UNUSED_0x015F = 351,
    FLAG_RECEIVED_MT_CORONET_2F_BLACK_FLUTE = 352,
    FLAG_TALKED_TO_GRAND_LAKE_ROUTE_213_NORTHWEST_HOUSE_PSYCHIC = 353,
    FLAG_VOLKNER_RETURNED_TO_GYM = 354,
    FLAG_UNUSED_0x0163 = 355,
    FLAG_TALKED_TO_POKEMON_MANSION_OFFICE_OLD_MAN_NATIONAL_DEX = 356,
    FLAG_SAVED_GAME_CORNER_TM64 = 357,
    FLAG_STARTED_FIVE_MAID_KNOCKOUT_CHALLENGE = 358,
    FLAG_UNUSED_0x0167 = 359,
    FLAG_UNUSED_0x0168 = 360,
    FLAG_UNUSED_0x0169 = 361,
    FLAG_UNUSED_0x016A = 362,
    FLAG_UNUSED_0x016B = 363,
    FLAG_UNUSED_0x016C = 364,
    FLAG_UNUSED_0x016D = 365,
    FLAG_UNUSED_0x016E = 366,
    FLAG_UNUSED_0x016F = 367,
    FLAG_UNUSED_0x0170 = 368,
    FLAG_UNUSED_0x0171 = 369,
    FLAG_HIDE_ROUTE_201_RIVAL = 370,
    FLAG_HIDE_TWINLEAF_TOWN_PLAYER_HOUSE_2F_RIVAL = 371,
    FLAG_HIDE_TWINLEAF_TOWN_RIVAL = 372,
    FLAG_HIDE_LAKE_VERITY_LOW_WATER_PROF_ROWAN = 373,
    FLAG_HIDE_LAKE_VERITY_LOW_WATER_CYRUS = 374,
    FLAG_HIDE_SANDGEM_TOWN_COUNTERPART = 375,
    FLAG_HIDE_ROUTE_201_PROF_ROWAN = 376,
    FLAG_HIDE_ROUTE_201_COUNTERPART = 377,
    FLAG_HIDE_JUBILIFE_CITY_COUNTERPART = 378,
    FLAG_HIDE_ROUTE_203_RIVAL = 379,
    FLAG_HIDE_OREBURGH_CITY_RIVAL = 380,
    FLAG_HIDE_ROUTE_201_BRIEFCASE = 381,
    FLAG_HIDE_ROUTE_212_BLOCKADE = 382,
    FLAG_HIDE_ROUTE_205_SOUTH_YOUNGSTER = 383,
    FLAG_HIDE_ETERNA_CITY_CYNTHIA = 384,
    FLAG_HIDE_JUBILIFE_CITY_LOOKER = 385,
    FLAG_HIDE_MT_CORONET_2F_CAVE_PAINTING_SHARDS = 386,
    FLAG_HIDE_POKECENTER_DAILY_TRAINER_1 = 387,
    FLAG_HIDE_POKECENTER_DAILY_TRAINER_2 = 388,
    FLAG_HIDE_VEILSTONE_STORE_B1F_PROF_ROWAN = 389,
    FLAG_HIDE_CANALAVE_LIBRARY_1F_LUCIAN = 390,
    FLAG_UNUSED_0x0187 = 391,
    FLAG_HIDE_ROUTE_202_COUNTERPART = 392,
    FLAG_HIDE_ETERNA_CITY_CYRUS = 393,
    FLAG_HIDE_OREBURGH_MINE_B2F_ROARK = 394,
    FLAG_HIDE_JUBILIFE_CITY_COLLECTOR = 395,
    FLAG_HIDE_ETERNA_CITY_RIVAL = 396,
    FLAG_HIDE_HEARTHOME_CITY_GYM_GUIDE = 397,
    FLAG_HIDE_VALLEY_WINDWORKS_OUTSIDE_GRUNT_M = 398,
    FLAG_HIDE_LAKE_VERITY_LOW_WATER_STARLY = 399,
    FLAG_HIDE_CONTEST_HALL_LOBBY_FANTINA = 400,
    FLAG_HIDE_VEILSTONE_CRASHER_WAKE = 401,
    FLAG_HIDE_CYCLE_SHOP_POKEFAN_M = 402,
    FLAG_UNUSED_0x0193 = 403,
    FLAG_HIDE_LAKE_VERITY_LOW_WATER_COUNTERPART = 404,
    FLAG_DUMMY_0x0193 = 405,
    FLAG_HIDE_LAKE_VERITY_LOW_WATER_RIVAL = 406,
    FLAG_HIDE_SANDGEM_TOWN_RIVAL = 407,
    FLAG_HIDE_SANDGEM_TOWN_LAB_PROF_ROWAN = 408,
    FLAG_HIDE_SANDGEM_TOWN_LAB_COUNTERPART = 409,
    FLAG_DUMMY_0x019A = 410,
    FLAG_HIDE_ETERNA_FOREST_CHERYL = 411,
    FLAG_HIDE_JUBILIFE_GALACTIC_GRUNTS = 412,
    FLAG_HIDE_JUBILIFE_ROWAN = 413,
    FLAG_HIDE_FLOAROMA_MEADOW_WORKS_KEY = 414,
    FLAG_HIDE_FLOAROMA_MEADOW_GRUNT_M_WEST = 415,
    FLAG_HIDE_FLOAROMA_MEADOW_GRUNT_M_EAST = 416,
    FLAG_HIDE_FLOAROMA_TOWN_GRUNTS = 417,
    FLAG_HIDE_ROUTE_205_SOUTH_GRUNTS = 418,
    FLAG_HIDE_GAME_CORNER_LOOKER = 419,
    FLAG_HIDE_VALLEY_WINDWORKS_BUILDING_GALACTIC_GRUNT_1 = 420,
    FLAG_HIDE_VALLEY_WINDWORKS_BUILDING_TEAM_GALACTIC = 421,
    FLAG_HIDE_CELESTIC_TOWN_CAVE_CYNTHIA = 422,
    FLAG_HIDE_ROARK_UNDERGROUND = 423,
    FLAG_HIDE_VEILSTONE_COUNTERPART = 424,
    FLAG_HIDE_PASTORIA_CITY_RIVAL = 425,
    FLAG_HIDE_ACUITY_LAKEFRONT_RIVAL = 426,
    FLAG_HIDE_MT_CORONET_1F_SOUTH_CYRUS = 427,
    FLAG_HIDE_CELESTIC_TOWN_ELDER = 428,
    FLAG_HIDE_VALOR_LAKEFRONT_CYNTHIA = 429,
    FLAG_HIDE_VALOR_LAKEFRONT_CAMERAMEN = 430,
    FLAG_HIDE_CELESTIC_TOWN_GRUNT_M = 431,
    FLAG_HIDE_ROUTE_210_SOUTH_PSYDUCK = 432,
    FLAG_HIDE_ROUTE_210_SOUTH_CYNTHIA = 433,
    FLAG_HIDE_CANALAVE_CITY_RIVAL_BRIDGE = 434,
    FLAG_HIDE_CANALAVE_CITY_PROF_ROWAN = 435,
    FLAG_HIDE_CANALAVE_CITY_SAILOR = 436,
    FLAG_HIDE_CANALAVE_CITY_COUNTERPART = 437,
    FLAG_HIDE_CANALAVE_LIBRARY_COUNTERPART = 438,
    FLAG_HIDE_CANALAVE_LIBRARY_ROWAN = 439,
    FLAG_HIDE_CANALAVE_LIBRARY_RIVAL = 440,
    FLAG_HIDE_LAKE_ACUITY_RIVAL = 441,
    FLAG_HIDE_LAKE_ACUITY_JUPITER = 442,
    FLAG_HIDE_TEAM_GALACTIC_ETERNA_BUILDING_1F_LOOKER = 443,
    FLAG_HIDE_LAKE_VALOR_GALACTIC = 444,
    FLAG_HIDE_CELESTIC_TOWN_CYNTHIA = 445,
    FLAG_HIDE_GALACTIC_HQ_4F_CYRUS = 446,
    FLAG_HIDE_VEILSTONE_GALACTIC_GRUNTS = 447,
    FLAG_HIDE_LAKE_VERITY_TEAM_GALACTIC = 448,
    FLAG_HIDE_LAKE_VALOR_DRAINED_MAGIKARP = 449,
    FLAG_HIDE_TEAM_GALACTIC_ETERNA_BUILDING_1F_GRUNT_M_LOOKER = 450,
    FLAG_HIDE_CELESTIC_TOWN_NORTH_HOUSE_ELDER = 451,
    FLAG_UNUSED_0x01C4 = 452,
    FLAG_HIDE_SPEAR_PILLAR_RIVAL = 453,
    FLAG_UNUSED_0x01C6 = 454,
    FLAG_HIDE_SPEAR_PILLAR_DISTORTED_TEAM_GALACTIC = 455,
    FLAG_HIDE_SPEAR_PILLAR_GRUNTS = 456,
    FLAG_HIDE_SPEAR_PILLAR_MARS_JUPITER = 457,
    FLAG_HIDE_SPEAR_PILLAR_CYRUS = 458,
    FLAG_HIDE_BATTLE_TOWER_BATTLE_ROOM_OPPONENT = 459,
    FLAG_HIDE_ROUTE_207_COUNTERPART = 460,
    FLAG_HIDE_SPEAR_PILLAR_DISTORTED_CYNTHIA = 461,
    FLAG_UNUSED_0x01CE = 462,
    FLAG_UNUSED_0x01CF = 463,
    FLAG_UNUSED_0x01D0 = 464,
    FLAG_HIDE_DAY_CARE_GYM_GUIDE = 465,
    FLAG_HIDE_PAL_PARK_LOBBY_PROF_OAK = 466,
    FLAG_HIDE_FIGHT_AREA_RIVAL = 467,
    FLAG_HIDE_FIGHT_AREA_BUCK = 468,
    FLAG_HIDE_VALOR_LAKEFRONT_RIVAL = 469,
    FLAG_HIDE_BATTLEGROUND_BUCK = 470,
    FLAG_HIDE_ROUTE_227_RIVAL = 471,
    FLAG_HIDE_ROUTE_227_CRASHER_WAKE = 472,
    FLAG_HIDE_SURVIVAL_AREA_RIVAL = 473,
    FLAG_HIDE_STARK_MOUNTAIN_ROOM_2_BUCK = 474,
    FLAG_HIDE_STARK_MOUNTAIN_ROOM_3_BUCK = 475,
    FLAG_HIDE_STARK_MOUNTAIN_ROOM_3_TEAM_GALACTIC = 476,
    FLAG_HIDE_STARK_MOUNTAIN_ROOM_3_HEATRAN = 477,
    FLAG_HIDE_STARK_MOUNTAIN_ROOM_3_MAGMA_STONE = 478,
    FLAG_HIDE_VERITY_CAVERN_MESPRIT = 479,
    FLAG_HIDE_VALOR_CAVERN_AZELF = 480,
    FLAG_HIDE_ACUITY_CAVERN_UXIE = 481,
    FLAG_HIDE_FIGHT_AREA_BATTLERS_AND_SPECTATORS = 482,
    FLAG_HIDE_FIGHT_AREA_PALMER = 483,
    FLAG_HIDE_WAYWARD_CAVE_1F_MIRA = 484,
    FLAG_HIDE_IRON_ISLAND_B2F_LEFT_ROOM_RILEY = 485,
    FLAG_HIDE_VICTORY_ROAD_1F_ROOM_2_MARLEY = 486,
    FLAG_HIDE_IRON_ISLAND_B2F_LEFT_ROOM_GRUNTS = 487,
    FLAG_HIDE_ROUTE_213_LOOKER = 488,
    FLAG_HIDE_BATTLE_TOWER_TEALA = 489,
    FLAG_HIDE_MULTI_BATTLE_ROOM_OPPONENT_1 = 490,
    FLAG_HIDE_MULTI_BATTLE_ROOM_OPPONENT_2 = 491,
    FLAG_HIDE_BATTLE_SALON_CHERYL = 492,
    FLAG_HIDE_BATTLE_SALON_MIRA = 493,
    FLAG_HIDE_BATTLE_SALON_RILEY = 494,
    FLAG_HIDE_BATTLE_SALON_MARLEY = 495,
    FLAG_HIDE_BATTLE_SALON_BUCK = 496,
    FLAG_HIDE_TWINLEAF_TOWN_PLAYER_HOUSE_1F_RIVAL_MOM = 497,
    FLAG_HIDE_TWINLEAF_TOWN_RIVAL_HOUSE_2F_RIVAL = 498,
    FLAG_HIDE_SNOWPOINT_CITY_CANDICE = 499,
    FLAG_HIDE_TRAINERS_SCHOOL_RIVAL = 500,
    FLAG_HIDE_JUBILIFE_CITY_CLOWNS_1_AND_2 = 501,
    FLAG_HIDE_JUBILIFE_CITY_POKETCH_CO_PRESIDENT = 502,
    FLAG_HIDE_POKETCH_CO_1F_POKETCH_CO_PRESIDENT = 503,
    FLAG_HIDE_ROUTE_205_SOUTH_LITTLE_GIRL = 504,
    FLAG_HIDE_VALLEY_WINDWORKS_BUILDING_LITTLE_GIRL = 505,
    FLAG_HIDE_VALLEY_WINDWORKS_OUTSIDE_LOOKER = 506,
    FLAG_HIDE_PASTORIA_CITY_CRASHER_WAKE = 507,
    FLAG_HIDE_ETERNA_FOREST_GARDENIA = 508,
    FLAG_HIDE_ETERNA_CITY_GALACTIC_GRUNTS = 509,
    FLAG_HIDE_ETERNA_CITY_POKECENTER_1F_BUNEARY = 510,
    FLAG_HIDE_TEAM_GALACTIC_ETERNA_BUILDING_4F_HOSTAGES = 511,
    FLAG_HIDE_CYCLE_SHOP_CLEFAIRY = 512,
    FLAG_HIDE_CONTEST_HALL_STAGE_CONTESTANT_1 = 513,
    FLAG_HIDE_CONTEST_HALL_STAGE_CONTESTANT_2 = 514,
    FLAG_HIDE_CONTEST_HALL_STAGE_CONTESTANT_3 = 515,
    FLAG_HIDE_CONTEST_HALL_STAGE_CONTESTANT_4 = 516,
    FLAG_HIDE_CONTEST_HALL_STAGE_CONTESTANT_WINNER = 517,
    FLAG_HIDE_HEARTHOME_CITY_ROUTE_209_BLOCKADE = 518,
    FLAG_HIDE_HEARTHOME_CITY_ROUTE_209_GATE_RIVAL = 519,
    FLAG_HIDE_CONTEST_HALL_LOBBY_MOM = 520,
    FLAG_HIDE_CONTEST_HALL_LOBBY_KEIRA = 521,
    FLAG_HIDE_MART_MYSTERY_GIFT_DELIVERYMAN = 522,
    FLAG_HIDE_VALLEY_WINDWORKS_OUTSIDE_DRIFLOON = 523,
    FLAG_HIDE_PASTORIA_CITY_GRUNT_M = 524,
    FLAG_HIDE_VEILSTONE_CITY_GALACTIC_WAREHOUSE_LOOKER = 525,
    FLAG_HIDE_BATTLE_TOWER_REPORTER = 526,
    FLAG_HIDE_PASTORIA_CITY_GYM_CRASHER_WAKE = 527,
    FLAG_HIDE_HEARTHOME_CITY_POKEMON_FAN_CLUB_REPORTER = 528,
    FLAG_HIDE_GRAND_LAKE_ROUTE_213_LOBBY_LOOKER = 529,
    FLAG_HIDE_POKETCH_CO_1F_REPORTER = 530,
    FLAG_HIDE_CONTEST_HALL_LOBBY_REPORTER = 531,
    FLAG_HIDE_IRON_ISLAND_RILEY = 532,
    FLAG_HIDE_JUBILIFE_TV_2F_REPORTER = 533,
    FLAG_HIDE_CANALAVE_CITY_WEST_HOUSE_REPORTER = 534,
    FLAG_HIDE_POFFIN_HOUSE_REPORTER = 535,
    FLAG_HIDE_ETERNA_CITY_GARDENIA = 536,
    FLAG_HIDE_HEARTHOME_CITY_GATE_TO_AMITY_SQUARE_REPORTER = 537,
    FLAG_HIDE_ROUTE_213_GRUNT_M = 538,
    FLAG_HIDE_VALOR_LAKEFRONT_GRUNT_M = 539,
    FLAG_HIDE_ROUTE_218_GATE_TO_CANALAVE_CITY_SCIENTIST_M = 540,
    FLAG_HIDE_POKEMON_LEAGUE_NORTH_POKECENTER_1F_RIVAL = 541,
    FLAG_HIDE_SOLACEON_TOWN_RIVAL = 542,
    FLAG_UNUSED_0x021F = 543,
    FLAG_HIDE_HEARTHOME_CITY_HIKER_3 = 544,
    FLAG_HIDE_ETERNA_CITY_GYM_GARDENIA = 545,
    FLAG_HIDE_ROUTE_206_CYCLING_ROAD_NORTH_GATE_SCIENTIST_M = 546,
    FLAG_HIDE_VEILSTONE_CITY_GRUNT_M_STORAGE_KEY = 547,
    FLAG_HIDE_GALACTIC_HQ_1F_DOOR = 548,
    FLAG_HIDE_GALACTIC_HQ_3F_DOOR = 549,
    FLAG_HIDE_GALACTIC_HQ_4F_DOOR = 550,
    FLAG_HIDE_GALACTIC_HQ_B2F_DOOR = 551,
    FLAG_HIDE_VEILSTONE_CITY_GALACTIC_WAREHOUSE_GALACTIC_HQ_DOOR = 552,
    FLAG_HIDE_CELESTIC_TOWN_CAVE_ELDER = 553,
    FLAG_HIDE_SANDGEM_TOWN_LAB_PROF_OAK = 554,
    FLAG_HIDE_SURVIVAL_AREA_BUCK = 555,
    FLAG_HIDE_STARK_MOUNTAIN_OUTSIDE_GRUNTS = 556,
    FLAG_HIDE_STARK_MOUNTAIN_OUTSIDE_BUCK = 557,
    FLAG_HIDE_STARK_MOUNTAIN_OUTSIDE_LOOKER = 558,
    FLAG_HIDE_STARK_MOUNTAIN_ROOM_1_TEAM_GALACTIC = 559,
    FLAG_HIDE_STARK_MOUNTAIN_ROOM_3_POLICEMEN = 560,
    FLAG_HIDE_STARK_MOUNTAIN_ROOM_3_LOOKER = 561,
    FLAG_HIDE_STARK_MOUNTAIN_ROOM_3_CROAGUNK = 562,
    FLAG_HIDE_STARK_MOUNTAIN_ROOM_3_ROCK_SMASH_LOOKER = 563,
    FLAG_HIDE_SNOWPOINT_CITY_POKECENTER_1F_MAYLENE = 564,
    FLAG_HIDE_MT_CORONET_2F_CAVE_PAINTING = 565,
    FLAG_HIDE_GALACTIC_HQ_CONTROL_ROOM_LAKE_GUARDIANS = 566,
    FLAG_HIDE_GALACTIC_HQ_TEAM_GALACTIC = 567,
    FLAG_HIDE_JUBILIFE_CITY_CLOWN_3 = 568,
    FLAG_HIDE_SUNYSHORE_CITY_FLINT = 569,
    FLAG_HIDE_POKEMON_LEAGUE_HALLWAY_TO_HALL_OF_FAME_CYNTHIA = 570,
    FLAG_HIDE_POKEMON_LEAGUE_HALLWAY_TO_HALL_OF_FAME_PROF_ROWAN = 571,
    FLAG_HIDE_HEARTHOME_CITY_KEIRA = 572,
    FLAG_HIDE_HEARTHOME_CITY_BUNEARY = 573,
    FLAG_UNUSED_0x023E = 574,
    FLAG_HIDE_POKECENTER_BASEMENT_BLOCKADE = 575,
    FLAG_HIDE_NEWMOON_ISLAND_FOREST_DARKRAI = 576,
    FLAG_HIDE_CANALAVE_CITY_HARBOR_INN_GYM_GUIDE = 577,
    FLAG_HIDE_ETERNA_CITY_SOUTH_HOUSE_PROF_OAK = 578,
    FLAG_HIDE_SNOWPOINT_TEMPLE_B5F_REGIGIGAS = 579,
    FLAG_HIDE_RESTAURANT_ISMAEL_HARLEY = 580,
    FLAG_HIDE_RESTAURANT_ROMAN_KYLIE = 581,
    FLAG_HIDE_RESTAURANT_LEONARDO_REBECCA = 582,
    FLAG_HIDE_RESTAURANT_EUGENE_ALISON = 583,
    FLAG_HIDE_RESTAURANT_ESTEBAN_MEREDITH = 584,
    FLAG_HIDE_RESTAURANT_EMANUEL_BLYTHE = 585,
    FLAG_HIDE_RESTAURANT_DARRYL_VALERIE = 586,
    FLAG_HIDE_RESTAURANT_KENDRICK_GABRIELLA = 587,
    FLAG_HIDE_RESTAURANT_EMILIO_KAYLEE = 588,
    FLAG_HIDE_IRON_ISLAND_HOUSE_BYRON = 589,
    FLAG_HIDE_HALL_OF_ORIGIN_ARCEUS = 590,
    FLAG_HIDE_FULLMOON_ISLAND_FOREST_CRESSELIA = 591,
    FLAG_HIDE_TURNBACK_CAVE_GIRATINA_ROOM_GIRATINA = 592,
    FLAG_HIDE_FLOWER_PARADISE_SHAYMIN = 593,
    FLAG_HIDE_ROUTE_224_PROF_OAK = 594,
    FLAG_HIDE_GALACTIC_HQ_HALL_CYRUS = 595,
    FLAG_HIDE_GALACTIC_HQ_HALL_LOOKER = 596,
    FLAG_HIDE_SUNYSHORE_CITY_RIVAL = 597,
    FLAG_HIDE_SNOWPOINT_CITY_CYNTHIA = 598,
    FLAG_HIDE_POKEMON_MANSION_FIVE_MAID_KNOCKOUT_MAID = 599,
    FLAG_HIDE_POKEMON_MANSION_FIVE_MAID_KNOCKOUT_BOSS = 600,
    FLAG_HIDE_GALACTIC_HQ_HALL_GRUNTS = 601,
    FLAG_HIDE_VILLA_SCHOOL_KID_M = 602,
    FLAG_HIDE_CANALAVE_CITY_SAILOR_ELDRITCH_HOUSE_SAILOR_ELDRITCH = 603,
    FLAG_HIDE_ROUTE_224_SHAYMIN = 604,
    FLAG_HIDE_VALOR_LAKEFRONT_BEAUTY = 605,
    FLAG_HIDE_GRAND_LAKE_VALOR_LAKEFRONT_EAST_HOUSE_BEAUTY = 606,
    FLAG_HIDE_ROUTE_217_NORTHEAST_HOUSE_POKEMON_BREEDER_F = 607,
    FLAG_HIDE_ROUTE_212_NORTH_POLICEMAN_BOBBY_NO_BATTLE = 608,
    FLAG_HIDE_ROUTE_212_NORTH_POLICEMAN_BOBBY = 609,
    FLAG_HIDE_ROUTE_212_NORTH_POLICEMAN_ALEX_NO_BATTLE = 610,
    FLAG_HIDE_ROUTE_212_NORTH_POLICEMAN_ALEX = 611,
    FLAG_HIDE_ROUTE_212_NORTH_POLICEMAN_DYLAN_NO_BATTLE = 612,
    FLAG_HIDE_ROUTE_212_NORTH_POLICEMAN_DYLAN = 613,
    FLAG_HIDE_ROUTE_212_NORTH_POLICEMAN_CALEB_NO_BATTLE = 614,
    FLAG_HIDE_ROUTE_212_NORTH_POLICEMAN_CALEB = 615,
    FLAG_HIDE_ROUTE_212_SOUTH_POLICEMAN_DANNY_NO_BATTLE = 616,
    FLAG_HIDE_ROUTE_212_SOUTH_POLICEMAN_DANNY = 617,
    FLAG_HIDE_ROUTE_222_POLICEMAN_THOMAS_NO_BATTLE = 618,
    FLAG_HIDE_ROUTE_222_POLICEMAN_THOMAS = 619,
    FLAG_HIDE_ROUTE_209_JOGGER_RICHARD_NO_BATTLE = 620,
    FLAG_HIDE_ROUTE_209_JOGGER_RICHARD = 621,
    FLAG_HIDE_ROUTE_209_JOGGER_RAUL_NO_BATTLE = 622,
    FLAG_HIDE_ROUTE_209_JOGGER_RAUL = 623,
    FLAG_HIDE_ROUTE_210_SOUTH_JOGGER_WYATT_NO_BATTLE = 624,
    FLAG_HIDE_ROUTE_210_SOUTH_JOGGER_WYATT = 625,
    FLAG_HIDE_ROUTE_215_JOGGER_SCOTT_NO_BATTLE = 626,
    FLAG_HIDE_ROUTE_215_JOGGER_SCOTT = 627,
    FLAG_HIDE_ROUTE_215_JOGGER_CRAIG_NO_BATTLE = 628,
    FLAG_HIDE_ROUTE_215_JOGGER_CRAIG = 629,
    FLAG_HIDE_BATTLE_PARK_PALMER = 630,
    FLAG_HIDE_BATTLE_PARK_RIVAL = 631,
    FLAG_HIDE_TURNBACK_CAVE_GIRATINA_ROOM_ITEM = 632,
    FLAG_HIDE_NEWMOON_ISLAND_SAILOR = 633,
    FLAG_HIDE_SNOWPOINT_CITY_SAILOR_SS_SPIRAL = 634,
    FLAG_HIDE_OLD_CHATEAU_DINING_AREA_OLD_MAN = 635,
    FLAG_HIDE_OLD_CHATEAU_BACK_MIDDLE_EAST_ROOM_TWIN = 636,
    FLAG_HIDE_VALOR_LAKEFRONT_COLLECTOR = 637,
    FLAG_HIDE_VICTORY_ROAD_1F_COLLECTOR = 638,
    FLAG_HIDE_ROUTE_221_WORKER_WEST = 639,
    FLAG_HIDE_ROUTE_221_WORKER_EAST = 640,
    FLAG_HIDE_VISTA_LIGHTHOUSE_VOLKNER = 641,
    FLAG_HIDE_POKEMON_LEAGUE_AARON_ROOM_ENTRANCE_DOOR = 642,
    FLAG_HIDE_POKEMON_LEAGUE_BERTHA_ROOM_ENTRANCE_DOOR = 643,
    FLAG_HIDE_POKEMON_LEAGUE_FLINT_ROOM_ENTRANCE_DOOR = 644,
    FLAG_HIDE_POKEMON_LEAGUE_LUCIAN_ROOM_ENTRANCE_DOOR = 645,
    FLAG_HIDE_POKEMON_LEAGUE_AARON_ROOM_EXIT_DOOR = 646,
    FLAG_HIDE_POKEMON_LEAGUE_BERTHA_ROOM_EXIT_DOOR = 647,
    FLAG_HIDE_POKEMON_LEAGUE_FLINT_ROOM_EXIT_DOOR = 648,
    FLAG_HIDE_POKEMON_LEAGUE_LUCIAN_ROOM_EXIT_DOOR = 649,
    FLAG_HIDE_VEILSTONE_CITY_LOOKER = 650,
    FLAG_HIDE_POKEMON_MANSION_OFFICE_POLICEMAN = 651,
    FLAG_HIDE_LAKE_VERITY_PROF_ROWAN = 652,
    FLAG_HIDE_LAKE_VERITY_COUNTERPART = 653,
    FLAG_HIDE_ROUTE_227_BUCK = 654,
    FLAG_HIDE_MT_CORONET_GALACTIC_GRUNTS = 655,
    FLAG_HIDE_JUBILIFE_TV_3F_GLOBAL_RANKING_ROOM_WORKER = 656,
    FLAG_HIDE_JUBILIFE_TV_3F_GROUP_RANKING_ROOM_WORKER = 657,
    FLAG_HIDE_GALACTIC_HQ_1F_SATURN = 658,
    FLAG_HIDE_LAKE_ACUITY_LOW_WATER_RIVAL = 659,
    FLAG_HIDE_FIGHT_AREA_BLOCKADE = 660,
    FLAG_HIDE_MT_CORONET_1F_NORTH_ROOM_1_GRUNT_F = 661,
    FLAG_HIDE_VERITY_CAVERN_PROF_OAK = 662,
    FLAG_HIDE_ROUTE_217_MAYLENE = 663,
    FLAG_HIDE_VEILSTONE_CITY_GYM_MAYLENE = 664,
    FLAG_HIDE_ROUTE_218_BLOCKADE = 665,
    FLAG_HIDE_MT_CORONET_1F_NORTH_ROOM_1_GRUNTS_M = 666,
    FLAG_HIDE_CELESTIC_TOWN_CAVE_CYRUS = 667,
    FLAG_HIDE_HEARTHOME_CITY_GYM_BOLLARDS = 668,
    FLAG_HIDE_PASTORIA_CITY_CROAGUNK = 669,
    FLAG_HIDE_GALACTIC_HQ_CONTROL_ROOM_CHARON = 670,
    FLAG_HIDE_MT_CORONET_2F_LOOKER = 671,
    FLAG_HIDE_SENDOFF_SPRING_CYNTHIA = 672,
    FLAG_HIDE_AMITY_SQUARE_FOLLOWER_MON = 673,
    FLAG_HIDE_BATTLEGROUND_TRAINER_1 = 674,
    FLAG_HIDE_BATTLEGROUND_TRAINER_2 = 675,
    FLAG_HIDE_BATTLEGROUND_TRAINER_3 = 676,
    FLAG_HIDE_BATTLEGROUND_TRAINER_4 = 677,
    FLAG_HIDE_RESORT_AREA_SCHOOL_KID_M = 678,
    FLAG_HIDE_VILLA_RIVAL = 679,
    FLAG_HIDE_VILLA_COUNTERPART = 680,
    FLAG_HIDE_VILLA_PROF_ROWAN = 681,
    FLAG_HIDE_RESORT_AREA_PROF_ROWAN = 682,
    FLAG_HIDE_VILLA_MOM = 683,
    FLAG_HIDE_VILLA_ROARK = 684,
    FLAG_HIDE_RESORT_AREA_ROARK = 685,
    FLAG_HIDE_VILLA_GARDENIA = 686,
    FLAG_HIDE_RESORT_AREA_GARDENIA = 687,
    FLAG_HIDE_VILLA_FANTINA = 688,
    FLAG_HIDE_VILLA_MAYLENE = 689,
    FLAG_HIDE_RESORT_AREA_MAYLENE = 690,
    FLAG_HIDE_VILLA_CRASHER_WAKE = 691,
    FLAG_HIDE_VILLA_BYRON = 692,
    FLAG_HIDE_RESORT_AREA_BYRON = 693,
    FLAG_HIDE_VILLA_CANDICE = 694,
    FLAG_HIDE_VILLA_VOLKNER = 695,
    FLAG_HIDE_VILLA_CYNTHIA = 696,
    FLAG_HIDE_VILLA_FLINT = 697,
    FLAG_HIDE_SPEAR_PILLAR_DISTORTED_MESPRIT = 698,
    FLAG_HIDE_ROTOMS_ROOM_MICROWAVE = 699,
    FLAG_HIDE_ROTOMS_ROOM_WASHING_MACHINE = 700,
    FLAG_HIDE_ROTOMS_ROOM_REFRIGERATOR = 701,
    FLAG_HIDE_ROTOMS_ROOM_FAN = 702,
    FLAG_HIDE_ROTOMS_ROOM_LAWN_MOWER = 703,
    FLAG_HIDE_ROTOMS_ROOM_PROF_ROWAN = 704,
    FLAG_HIDE_BATTLE_HALL_REPORTER = 705,
    FLAG_HIDE_ROUTE_214_GATE_TO_VEILSTONE_CITY_REPORTER = 706,
    FLAG_HIDE_BATTLE_FRONTIER_REPORTER = 707,
    FLAG_HIDE_SANDGEM_TOWN_PROF_ROWAN = 708,
    FLAG_HIDE_VILLA_DELIVERY_GUY = 709,
    FLAG_HIDE_TEAM_GALACTIC_ETERNA_BUILDING_1F_WALL_BLOCKING_ROTOMS_ROOM = 710,
    FLAG_HIDE_VILLA_BOOK = 711,
    FLAG_HIDE_OREBURGH_MINE_B1F_HIKER = 712,
    FLAG_HIDE_CANALAVE_LIBRARY_2F_HIKER = 713,
    FLAG_HIDE_ROUTE_224_MARLEY = 714,
    FLAG_HIDE_BATTLE_HALL_MAJOR_NPC = 715,
    FLAG_HIDE_CANALAVE_CITY_RIVAL_LIBRARY = 716,
    FLAG_UNUSED_0x02CD = 717,
    FLAG_UNUSED_0x02CE = 718,
    FLAG_UNUSED_0x02CF = 719,
    FLAG_UNUSED_0x02D0 = 720,
    FLAG_UNUSED_0x02D1 = 721,
    FLAG_UNUSED_0x02D2 = 722,
    FLAG_UNUSED_0x02D3 = 723,
    FLAG_UNUSED_0x02D4 = 724,
    FLAG_UNUSED_0x02D5 = 725,
    FLAG_UNUSED_0x02D6 = 726,
    FLAG_UNUSED_0x02D7 = 727,
    FLAG_UNUSED_0x02D8 = 728,
    FLAG_UNUSED_0x02D9 = 729,
    HIDDEN_ITEM_FLAGS_START = 730,
    FLAG_OBTAINED_HIDDEN_VALLEY_WINDWORKS_OUTSIDE_MAX_ELIXIR = 730,
    FLAG_OBTAINED_HIDDEN_ETERNA_FOREST_INSECT_PLATE = 731,
    FLAG_OBTAINED_HIDDEN_ETERNA_CITY_DRACO_PLATE = 732,
    FLAG_OBTAINED_HIDDEN_ROUTE_211_WEST_GREAT_BALL = 733,
    FLAG_OBTAINED_HIDDEN_ROUTE_206_ETHER = 734,
    FLAG_OBTAINED_HIDDEN_ROUTE_206_TINYMUSHROOM = 735,
    FLAG_OBTAINED_HIDDEN_WAYWARD_CAVE_1F_YELLOW_SHARD = 736,
    FLAG_OBTAINED_HIDDEN_VALOR_LAKEFRONT_PP_UP = 737,
    FLAG_OBTAINED_HIDDEN_ROUTE_207_OR_WAYWARD_CAVE_1F_RED_SHARD = 738,
    FLAG_OBTAINED_HIDDEN_WAYWARD_CAVE_1F_GREAT_BALL = 739,
    FLAG_OBTAINED_HIDDEN_WAYWARD_CAVE_B1F_DUSK_STONE = 740,
    FLAG_OBTAINED_HIDDEN_WAYWARD_CAVE_B1F_STARDUST = 741,
    FLAG_OBTAINED_HIDDEN_ROUTE_207_RARE_CANDY = 742,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_SOUTH_STARDUST = 743,
    FLAG_OBTAINED_HIDDEN_ROUTE_208_STAR_PIECE = 744,
    FLAG_OBTAINED_HIDDEN_ROUTE_208_TINYMUSHROOM = 745,
    FLAG_OBTAINED_HIDDEN_HEARTHOME_CITY_BIG_MUSHROOM = 746,
    FLAG_OBTAINED_HIDDEN_ROUTE_212_NORTH_PEARL = 747,
    FLAG_OBTAINED_HIDDEN_ROUTE_212_NORTH_ULTRA_BALL = 748,
    FLAG_OBTAINED_HIDDEN_ROUTE_212_SOUTH_MAX_ETHER = 749,
    FLAG_OBTAINED_HIDDEN_ROUTE_212_SOUTH_BIG_MUSHROOM = 750,
    FLAG_OBTAINED_HIDDEN_ROUTE_212_SOUTH_RARE_CANDY = 751,
    FLAG_OBTAINED_HIDDEN_ROUTE_212_SOUTH_ULTRA_BALL = 752,
    FLAG_OBTAINED_HIDDEN_ROUTE_212_SOUTH_MAX_REPEL = 753,
    FLAG_OBTAINED_HIDDEN_ROUTE_212_SOUTH_HYPER_POTION = 754,
    FLAG_OBTAINED_HIDDEN_ROUTE_212_SOUTH_FULL_HEAL = 755,
    FLAG_OBTAINED_HIDDEN_ROUTE_212_SOUTH_DAWN_STONE = 756,
    FLAG_OBTAINED_HIDDEN_PASTORIA_CITY_ZINC = 757,
    FLAG_OBTAINED_HIDDEN_ROUTE_213_PEARL_1 = 758,
    FLAG_OBTAINED_HIDDEN_ROUTE_213_BIG_PEARL_1 = 759,
    FLAG_OBTAINED_HIDDEN_ROUTE_213_HP_UP = 760,
    FLAG_OBTAINED_HIDDEN_VALOR_LAKEFRONT_SUN_STONE = 761,
    FLAG_OBTAINED_HIDDEN_ROUTE_209_GREAT_BALL = 762,
    FLAG_OBTAINED_HIDDEN_ROUTE_209_STARDUST = 763,
    FLAG_OBTAINED_HIDDEN_SOLACEON_TOWN_BIG_MUSHROOM = 764,
    FLAG_OBTAINED_HIDDEN_ROUTE_209_REVIVE = 765,
    FLAG_OBTAINED_HIDDEN_SOLACEON_RUINS_ROOM_1_NORTHWEST_DEAD_END_FIRE_STONE = 766,
    FLAG_OBTAINED_HIDDEN_SOLACEON_RUINS_ROOM_3_SOUTHWEST_DEAD_END_WATER_STONE = 767,
    FLAG_OBTAINED_HIDDEN_SOLACEON_RUINS_ROOM_5_SOUTHEAST_DEAD_END_THUNDERSTONE = 768,
    FLAG_OBTAINED_HIDDEN_ROUTE_210_SOUTH_FULL_HEAL = 769,
    FLAG_OBTAINED_HIDDEN_VEILSTONE_CITY_PP_UP = 770,
    FLAG_OBTAINED_HIDDEN_VEILSTONE_CITY_CALCIUM = 771,
    FLAG_OBTAINED_HIDDEN_ROUTE_214_BIG_MUSHROOM = 772,
    FLAG_OBTAINED_HIDDEN_ROUTE_214_CARBOS = 773,
    FLAG_OBTAINED_HIDDEN_ROUTE_210_NORTH_MEADOW_PLATE = 774,
    FLAG_OBTAINED_HIDDEN_ROUTE_210_NORTH_SHINY_STONE = 775,
    FLAG_OBTAINED_HIDDEN_FUEGO_IRONWORKS_BUILDING_PROTEIN = 776,
    FLAG_OBTAINED_HIDDEN_FUEGO_IRONWORKS_BUILDING_STAR_PIECE = 777,
    FLAG_OBTAINED_HIDDEN_ROUTE_220_BIG_PEARL = 778,
    FLAG_OBTAINED_HIDDEN_ROUTE_221_BIG_MUSHROOM = 779,
    FLAG_OBTAINED_HIDDEN_ROUTE_221_HYPER_POTION = 780,
    FLAG_OBTAINED_HIDDEN_IRON_ISLAND_STAR_PIECE = 781,
    FLAG_OBTAINED_HIDDEN_IRON_ISLAND_B1F_RIGHT_ROOM_STAR_PIECE = 782,
    FLAG_OBTAINED_HIDDEN_IRON_ISLAND_B2F_RIGHT_ROOM_STAR_PIECE = 783,
    FLAG_OBTAINED_HIDDEN_IRON_ISLAND_B2F_LEFT_ROOM_STAR_PIECE_1 = 784,
    FLAG_OBTAINED_HIDDEN_IRON_ISLAND_B2F_LEFT_ROOM_STAR_PIECE_2 = 785,
    FLAG_OBTAINED_HIDDEN_IRON_ISLAND_B2F_LEFT_ROOM_IRON_PLATE = 786,
    FLAG_OBTAINED_HIDDEN_LAKE_VALOR_DRAINED_HP_UP = 787,
    FLAG_OBTAINED_HIDDEN_FLOAROMA_MEADOW_HONEY_1 = 788,
    FLAG_OBTAINED_HIDDEN_FLOAROMA_MEADOW_HONEY_2 = 789,
    FLAG_OBTAINED_HIDDEN_ROUTE_211_EAST_CALCIUM = 790,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_NORTH_ROOM_2_YELLOW_SHARD = 791,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_NORTH_ROOM_1_ENERGYPOWDER = 792,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_NORTH_ROOM_1_NUGGET = 793,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_NORTH_ROOM_1_STARDUST = 794,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_NORTH_ROOM_1_RED_SHARD = 795,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_NORTH_ROOM_1_STAR_PIECE = 796,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_B1F_GREEN_SHARD = 797,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_B1F_BLUE_SHARD = 798,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_B1F_RARE_CANDY = 799,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_B1F_STAR_PIECE = 800,
    FLAG_OBTAINED_HIDDEN_ROUTE_216_PP_UP = 801,
    FLAG_OBTAINED_HIDDEN_ROUTE_217_IRON = 802,
    FLAG_OBTAINED_HIDDEN_ROUTE_217_MAX_REVIVE = 803,
    FLAG_OBTAINED_HIDDEN_GALACTIC_HQ_1F_PP_UP = 804,
    FLAG_OBTAINED_HIDDEN_GALACTIC_HQ_2F_POKE_DOLL = 805,
    FLAG_OBTAINED_HIDDEN_GALACTIC_HQ_2F_MAX_POTION = 806,
    FLAG_OBTAINED_HIDDEN_GALACTIC_HQ_3F_RARE_CANDY = 807,
    FLAG_OBTAINED_HIDDEN_GALACTIC_HQ_3F_ELIXIR = 808,
    FLAG_OBTAINED_HIDDEN_GALACTIC_HQ_3F_NUGGET = 809,
    FLAG_OBTAINED_HIDDEN_GALACTIC_HQ_4F_RAZOR_CLAW = 810,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_2F_RED_SHARD = 811,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_2F_STAR_PIECE = 812,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_3F_MAX_REPEL = 813,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_OUTSIDE_STAR_PIECE_1 = 814,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_OUTSIDE_NORTH_STARDUST = 815,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_OUTSIDE_NORTH_FULL_RESTORE = 816,
    FLAG_OBTAINED_HIDDEN_OREBURGH_CITY_PEARL = 817,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_OUTSIDE_NORTH_MOON_STONE = 818,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_OUTSIDE_SOUTH_BLUE_SHARD = 819,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_OUTSIDE_SOUTH_MAX_POTION = 820,
    FLAG_OBTAINED_HIDDEN_ROUTE_207_POKE_BALL = 821,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_OUTSIDE_STAR_PIECE_2 = 822,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_4F_ROOMS_1_AND_2_SUN_STONE = 823,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_4F_ROOMS_1_AND_2_FULL_HEAL = 824,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_4F_ROOMS_1_AND_2_MAX_REVIVE = 825,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_4F_ROOMS_1_AND_2_STARDUST = 826,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_4F_ROOMS_1_AND_2_YELLOW_SHARD = 827,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_4F_ROOMS_1_AND_2_HEAL_POWDER = 828,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_4F_ROOMS_1_AND_2_STONE_PLATE = 829,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_4F_ROOM_3_MAX_ELIXIR = 830,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_5F_RARE_CANDY = 831,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_5F_FULL_RESTORE = 832,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_TUNNEL_ROOM_STARDUST = 833,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_TUNNEL_ROOM_HEAL_POWDER = 834,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_TUNNEL_ROOM_GREEN_SHARD_1 = 835,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_TUNNEL_ROOM_RARE_CANDY = 836,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_TUNNEL_ROOM_MAX_POTION = 837,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_TUNNEL_ROOM_GREEN_SHARD_2 = 838,
    FLAG_OBTAINED_HIDDEN_ROUTE_222_FULL_RESTORE = 839,
    FLAG_OBTAINED_HIDDEN_SUNYSHORE_CITY_ZAP_PLATE = 840,
    FLAG_OBTAINED_HIDDEN_SUNYSHORE_CITY_CARBOS = 841,
    FLAG_OBTAINED_HIDDEN_ROUTE_223_HEART_SCALE_1 = 842,
    FLAG_OBTAINED_HIDDEN_VICTORY_ROAD_1F_NUGGET = 843,
    FLAG_OBTAINED_HIDDEN_VICTORY_ROAD_1F_ULTRA_BALL = 844,
    FLAG_OBTAINED_HIDDEN_VICTORY_ROAD_1F_MAX_REVIVE = 845,
    FLAG_OBTAINED_HIDDEN_VICTORY_ROAD_2F_FULL_RESTORE = 846,
    FLAG_OBTAINED_HIDDEN_VICTORY_ROAD_2F_RARE_CANDY = 847,
    FLAG_OBTAINED_HIDDEN_VICTORY_ROAD_B1F_HEART_SCALE = 848,
    FLAG_OBTAINED_HIDDEN_VICTORY_ROAD_B1F_MAX_ELIXIR = 849,
    FLAG_OBTAINED_HIDDEN_POKEMON_LEAGUE_SKY_PLATE = 850,
    FLAG_OBTAINED_HIDDEN_VICTORY_ROAD_1F_ROOM_2_ULTRA_BALL_1 = 851,
    FLAG_OBTAINED_HIDDEN_VICTORY_ROAD_1F_ROOM_2_HP_UP = 852,
    FLAG_OBTAINED_HIDDEN_VICTORY_ROAD_1F_ROOM_2_HEART_SCALE = 853,
    FLAG_OBTAINED_HIDDEN_ROUTE_224_RARE_CANDY = 854,
    FLAG_OBTAINED_HIDDEN_ROUTE_224_PEARL_1 = 855,
    FLAG_OBTAINED_HIDDEN_ROUTE_224_HEART_SCALE = 856,
    FLAG_OBTAINED_HIDDEN_ROUTE_224_BIG_PEARL = 857,
    FLAG_OBTAINED_HIDDEN_VICTORY_ROAD_1F_ROOM_2_ULTRA_BALL_2 = 858,
    FLAG_OBTAINED_HIDDEN_VICTORY_ROAD_1F_ROOM_2_RARE_CANDY = 859,
    FLAG_OBTAINED_HIDDEN_ROUTE_225_RARE_CANDY = 860,
    FLAG_OBTAINED_HIDDEN_SURVIVAL_AREA_PP_UP = 861,
    FLAG_OBTAINED_HIDDEN_ROUTE_226_TINYMUSHROOM = 862,
    FLAG_OBTAINED_HIDDEN_ROUTE_226_PP_MAX = 863,
    FLAG_OBTAINED_HIDDEN_ROUTE_226_BIG_MUSHROOM = 864,
    FLAG_OBTAINED_HIDDEN_ROUTE_226_HEART_SCALE = 865,
    FLAG_OBTAINED_HIDDEN_ROUTE_227_STAR_PIECE = 866,
    FLAG_OBTAINED_HIDDEN_ROUTE_227_MAX_REPEL = 867,
    FLAG_OBTAINED_HIDDEN_STARK_MOUNTAIN_ROOM_1_STAR_PIECE = 868,
    FLAG_OBTAINED_HIDDEN_STARK_MOUNTAIN_ROOM_2_FLAME_PLATE = 869,
    FLAG_OBTAINED_HIDDEN_STARK_MOUNTAIN_ROOM_2_STARDUST = 870,
    FLAG_OBTAINED_HIDDEN_STARK_MOUNTAIN_ROOM_2_IRON = 871,
    FLAG_OBTAINED_HIDDEN_ROUTE_228_CALCIUM = 872,
    FLAG_OBTAINED_HIDDEN_ROUTE_228_STAR_PIECE = 873,
    FLAG_OBTAINED_HIDDEN_ROUTE_228_FULL_RESTORE = 874,
    FLAG_OBTAINED_HIDDEN_ROUTE_229_THUNDERSTONE = 875,
    FLAG_OBTAINED_HIDDEN_RESORT_AREA_NUGGET = 876,
    FLAG_OBTAINED_HIDDEN_ROUTE_230_ULTRA_BALL = 877,
    FLAG_OBTAINED_HIDDEN_ROUTE_230_WATER_STONE = 878,
    FLAG_OBTAINED_HIDDEN_ROUTE_210_SOUTH_ULTRA_BALL_1 = 879,
    FLAG_OBTAINED_HIDDEN_ROUTE_205_NORTH_SUPER_POTION = 880,
    FLAG_OBTAINED_HIDDEN_ROUTE_207_GREAT_BALL = 881,
    FLAG_OBTAINED_HIDDEN_ROUTE_213_HYPER_POTION = 882,
    FLAG_OBTAINED_HIDDEN_ROUTE_209_SUPER_REPEL = 883,
    FLAG_OBTAINED_HIDDEN_ROUTE_210_SOUTH_RARE_CANDY = 884,
    FLAG_OBTAINED_HIDDEN_FUEGO_IRONWORKS_OUTSIDE_BIG_MUSHROOM = 885,
    FLAG_OBTAINED_HIDDEN_FUEGO_IRONWORKS_OUTSIDE_ULTRA_BALL = 886,
    FLAG_OBTAINED_HIDDEN_ROUTE_222_TINYMUSHROOM = 887,
    FLAG_OBTAINED_HIDDEN_ROUTE_222_ULTRA_BALL = 888,
    FLAG_OBTAINED_HIDDEN_ROUTE_226_RARE_CANDY = 889,
    FLAG_OBTAINED_HIDDEN_VERITY_LAKEFRONT_TINYMUSHROOM = 890,
    FLAG_OBTAINED_HIDDEN_JUBILIFE_CITY_STARDUST = 891,
    FLAG_OBTAINED_HIDDEN_WAYWARD_CAVE_1F_GREEN_SHARD = 892,
    FLAG_OBTAINED_HIDDEN_POKEMON_MANSION_MAIDS_ROOM_PROTEIN = 893,
    FLAG_OBTAINED_HIDDEN_ROUTE_213_PEARL_2 = 894,
    FLAG_OBTAINED_HIDDEN_ROUTE_213_PEARL_3 = 895,
    FLAG_OBTAINED_HIDDEN_ROUTE_213_MAX_REVIVE = 896,
    FLAG_OBTAINED_HIDDEN_OLD_CHATEAU_DINING_AREA_HONEY = 897,
    FLAG_OBTAINED_HIDDEN_ROUTE_214_RAZOR_FANG = 898,
    FLAG_OBTAINED_HIDDEN_ROUTE_219_ZINC = 899,
    FLAG_OBTAINED_HIDDEN_ROUTE_224_PEARL_2 = 900,
    FLAG_OBTAINED_HIDDEN_ROUTE_224_ZINC = 901,
    FLAG_OBTAINED_HIDDEN_ROUTE_224_ULTRA_BALL = 902,
    FLAG_OBTAINED_HIDDEN_ROUTE_225_BIG_MUSHROOM = 903,
    FLAG_OBTAINED_HIDDEN_GRAND_LAKE_ROUTE_213_NORTHEAST_HOUSE_MAX_REVIVE = 904,
    FLAG_OBTAINED_HIDDEN_STARK_MOUNTAIN_ROOM_2_MAX_REVIVE = 905,
    FLAG_OBTAINED_HIDDEN_STARK_MOUNTAIN_ROOM_2_STAR_PIECE = 906,
    FLAG_OBTAINED_HIDDEN_STARK_MOUNTAIN_ROOM_2_ULTRA_BALL = 907,
    FLAG_OBTAINED_HIDDEN_SNOWPOINT_CITY_RARE_CANDY = 908,
    FLAG_OBTAINED_HIDDEN_SNOWPOINT_TEMPLE_B3F_FULL_RESTORE = 909,
    FLAG_OBTAINED_HIDDEN_VALOR_LAKEFRONT_BIG_MUSHROOM = 910,
    FLAG_OBTAINED_HIDDEN_ROUTE_215_REVIVE = 911,
    FLAG_OBTAINED_HIDDEN_ROUTE_215_ULTRA_BALL = 912,
    FLAG_OBTAINED_HIDDEN_ROUTE_215_IRON = 913,
    FLAG_OBTAINED_HIDDEN_ROUTE_213_HEART_SCALE_1 = 914,
    FLAG_OBTAINED_HIDDEN_ROUTE_213_BIG_PEARL_2 = 915,
    FLAG_OBTAINED_HIDDEN_ROUTE_213_HEART_SCALE_2 = 916,
    FLAG_OBTAINED_HIDDEN_ROUTE_209_LOST_TOWER_2F_HONEY = 917,
    FLAG_OBTAINED_HIDDEN_ROUTE_209_LOST_TOWER_4F_GREAT_BALL = 918,
    FLAG_OBTAINED_HIDDEN_ROUTE_210_SOUTH_HYPER_POTION = 919,
    FLAG_OBTAINED_HIDDEN_ROUTE_221_ULTRA_BALL = 920,
    FLAG_OBTAINED_HIDDEN_ROUTE_221_CARBOS = 921,
    FLAG_OBTAINED_HIDDEN_ROUTE_221_MAX_REVIVE = 922,
    FLAG_OBTAINED_HIDDEN_ROUTE_217_ULTRA_BALL_1 = 923,
    FLAG_OBTAINED_HIDDEN_ROUTE_217_REVIVAL_HERB = 924,
    FLAG_OBTAINED_HIDDEN_ROUTE_217_MAX_REPEL = 925,
    FLAG_OBTAINED_HIDDEN_ROUTE_217_ULTRA_BALL_2 = 926,
    FLAG_OBTAINED_HIDDEN_ROUTE_217_PROTEIN = 927,
    FLAG_OBTAINED_HIDDEN_ROUTE_217_MAX_POTION = 928,
    FLAG_OBTAINED_HIDDEN_ROUTE_217_FULL_HEAL = 929,
    FLAG_OBTAINED_HIDDEN_ROUTE_217_NUGGET = 930,
    FLAG_OBTAINED_HIDDEN_TWINLEAF_TOWN_ODD_KEYSTONE = 931,
    FLAG_OBTAINED_HIDDEN_ROUTE_210_SOUTH_ULTRA_BALL_2 = 932,
    FLAG_OBTAINED_HIDDEN_ROUTE_225_LEAF_STONE = 933,
    FLAG_OBTAINED_HIDDEN_ROUTE_225_ULTRA_BALL = 934,
    FLAG_OBTAINED_HIDDEN_ROUTE_228_RARE_CANDY = 935,
    FLAG_OBTAINED_HIDDEN_ROUTE_228_MAX_REPEL = 936,
    FLAG_OBTAINED_HIDDEN_ROUTE_228_PP_MAX = 937,
    FLAG_OBTAINED_HIDDEN_WAYWARD_CAVE_1F_SUPER_POTION = 938,
    FLAG_OBTAINED_HIDDEN_WAYWARD_CAVE_1F_BLUE_SHARD = 939,
    FLAG_OBTAINED_HIDDEN_GREAT_MARSH_1_RARE_CANDY = 940,
    FLAG_OBTAINED_HIDDEN_GREAT_MARSH_2_TOXIC_PLATE = 941,
    FLAG_OBTAINED_HIDDEN_GREAT_MARSH_3_LEAF_STONE = 942,
    FLAG_OBTAINED_HIDDEN_GREAT_MARSH_4_HYPER_POTION = 943,
    FLAG_OBTAINED_HIDDEN_GREAT_MARSH_5_BIG_MUSHROOM = 944,
    FLAG_OBTAINED_HIDDEN_GREAT_MARSH_6_MAX_REPEL = 945,
    FLAG_OBTAINED_HIDDEN_ROUTE_213_SUITE_KEY = 946,
    FLAG_OBTAINED_HIDDEN_ROUTE_205_SOUTH_STARDUST = 947,
    FLAG_OBTAINED_HIDDEN_CANALAVE_CITY_BIG_PEARL = 948,
    FLAG_OBTAINED_HIDDEN_FLOAROMA_MEADOW_HONEY_3 = 949,
    FLAG_OBTAINED_HIDDEN_FLOAROMA_MEADOW_HONEY_4 = 950,
    FLAG_OBTAINED_HIDDEN_FLOAROMA_MEADOW_HONEY_5 = 951,
    FLAG_OBTAINED_HIDDEN_FLOAROMA_MEADOW_HONEY_6 = 952,
    FLAG_OBTAINED_HIDDEN_FLOAROMA_MEADOW_HYPER_POTION = 953,
    FLAG_OBTAINED_HIDDEN_FLOAROMA_MEADOW_REVIVE = 954,
    FLAG_OBTAINED_HIDDEN_FLOAROMA_MEADOW_FULL_HEAL = 955,
    FLAG_OBTAINED_HIDDEN_FLOAROMA_MEADOW_PP_UP = 956,
    FLAG_OBTAINED_HIDDEN_FLOAROMA_MEADOW_MAX_REVIVE = 957,
    FLAG_OBTAINED_HIDDEN_ETERNA_CITY_MOON_STONE = 958,
    FLAG_UNUSED_0x03BF = 959,
    FLAG_UNUSED_0x03C0 = 960,
    FLAG_UNUSED_0x03C1 = 961,
    FLAG_UNUSED_0x03C2 = 962,
    FLAG_UNUSED_0x03C3 = 963,
    FLAG_UNUSED_0x03C4 = 964,
    FLAG_UNUSED_0x03C5 = 965,
    FLAG_UNUSED_0x03C6 = 966,
    FLAG_UNUSED_0x03C7 = 967,
    FLAG_UNUSED_0x03C8 = 968,
    FLAG_UNUSED_0x03C9 = 969,
    FLAG_UNUSED_0x03CA = 970,
    FLAG_UNUSED_0x03CB = 971,
    FLAG_UNUSED_0x03CC = 972,
    FLAG_UNUSED_0x03CD = 973,
    FLAG_UNUSED_0x03CE = 974,
    FLAG_UNUSED_0x03CF = 975,
    FLAG_UNUSED_0x03D0 = 976,
    FLAG_UNUSED_0x03D1 = 977,
    FLAG_UNUSED_0x03D2 = 978,
    FLAG_UNUSED_0x03D3 = 979,
    FLAG_UNUSED_0x03D4 = 980,
    FLAG_UNUSED_0x03D5 = 981,
    FLAG_UNUSED_0x03D6 = 982,
    FLAG_UNUSED_0x03D7 = 983,
    FLAG_UNUSED_0x03D8 = 984,
    FLAG_UNUSED_0x03D9 = 985,
    FLAG_OBTAINED_HIDDEN_ROUTE_207_REVIVE = 986,
    FLAG_OBTAINED_HIDDEN_OREBURGH_CITY_HEART_SCALE = 987,
    FLAG_OBTAINED_HIDDEN_ROUTE_213_PEARL_4 = 988,
    FLAG_OBTAINED_HIDDEN_VEILSTONE_CITY_STAR_PIECE = 989,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_OUTSIDE_SOUTH_REVIVAL_HERB = 990,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_4F_ROOMS_1_AND_2_ULTRA_BALL = 991,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_OUTSIDE_NORTH_REVIVAL_HERB = 992,
    FLAG_OBTAINED_HIDDEN_ROUTE_222_BIG_MUSHROOM = 993,
    FLAG_OBTAINED_HIDDEN_ROUTE_222_PEARL = 994,
    FLAG_OBTAINED_HIDDEN_ROUTE_222_HEART_SCALE_1 = 995,
    FLAG_OBTAINED_HIDDEN_ROUTE_222_HEART_SCALE_2 = 996,
    FLAG_OBTAINED_HIDDEN_ROUTE_223_HEART_SCALE_2 = 997,
    FLAG_OBTAINED_HIDDEN_STARK_MOUNTAIN_ROOM_2_FIRE_STONE = 998,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_OUTSIDE_SOUTH_ENERGY_ROOT = 999,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_OUTSIDE_NORTH_ENERGY_ROOT = 1000,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_SOUTH_ENERGYPOWDER = 1001,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_1F_SOUTH_REVIVE = 1002,
    FLAG_OBTAINED_HIDDEN_CELESTIC_TOWN_KINGS_ROCK = 1003,
    FLAG_OBTAINED_HIDDEN_IRON_ISLAND_IRON_RUINS_NUGGET = 1004,
    FLAG_OBTAINED_HIDDEN_MT_CORONET_ICEBERG_RUINS_NUGGET = 1005,
    FLAG_OBTAINED_HIDDEN_ROUTE_228_ROCK_PEAK_RUINS_NUGGET = 1006,
    FLAG_OBTAINED_HIDDEN_UNUSED_NUGGET_1 = 1007,
    FLAG_OBTAINED_HIDDEN_UNUSED_NUGGET_2 = 1008,
    FLAG_OBTAINED_HIDDEN_UNUSED_NUGGET_3 = 1009,
    FLAG_OBTAINED_HIDDEN_UNUSED_NUGGET_4 = 1010,
    FLAG_OBTAINED_HIDDEN_UNUSED_NUGGET_5 = 1011,
    FLAG_OBTAINED_HIDDEN_UNUSED_NUGGET_6 = 1012,
    FLAG_OBTAINED_HIDDEN_UNUSED_NUGGET_7 = 1013,
    HIDDEN_ITEM_FLAGS_END = 1013,
    FLAG_OBTAINED_ROUTE_202_POTION = 1014,
    FLAG_OBTAINED_ROUTE_203_REPEL = 1015,
    FLAG_OBTAINED_ROUTE_203_POKE_BALL = 1016,
    FLAG_OBTAINED_OREBURGH_GATE_B1F_TM01 = 1017,
    FLAG_OBTAINED_OREBURGH_GATE_B1F_TM31 = 1018,
    FLAG_OBTAINED_OREBURGH_GATE_B1F_TM70 = 1019,
    FLAG_OBTAINED_OREBURGH_GATE_B1F_EARTH_PLATE = 1020,
    FLAG_OBTAINED_OREBURGH_MINE_B2F_ESCAPE_ROPE = 1021,
    FLAG_OBTAINED_OREBURGH_MINE_B2F_POTION = 1022,
    FLAG_OBTAINED_ROUTE_204_SOUTH_SEA_INCENSE = 1023,
    FLAG_OBTAINED_ROUTE_204_SOUTH_HP_UP = 1024,
    FLAG_OBTAINED_RAVAGED_PATH_TM39 = 1025,
    FLAG_OBTAINED_RAVAGED_PATH_LUCK_INCENSE = 1026,
    FLAG_OBTAINED_RAVAGED_PATH_TM03 = 1027,
    FLAG_OBTAINED_ROUTE_204_NORTH_TM09 = 1028,
    FLAG_OBTAINED_VALLEY_WINDWORKS_OUTSIDE_TM24 = 1029,
    FLAG_OBTAINED_ROUTE_205_SOUTH_HEAL_BALL = 1030,
    FLAG_OBTAINED_ROUTE_205_SOUTH_X_SP_DEF = 1031,
    FLAG_OBTAINED_ETERNA_FOREST_ANTIDOTE = 1032,
    FLAG_OBTAINED_ETERNA_FOREST_PARLYZ_HEAL = 1033,
    FLAG_OBTAINED_ETERNA_FOREST_NET_BALL = 1034,
    FLAG_OBTAINED_ETERNA_FOREST_GREEN_SHARD = 1035,
    FLAG_OBTAINED_OLD_CHATEAU_SIDE_ROOMS_OLD_GATEAU = 1036,
    FLAG_OBTAINED_ROUTE_205_NORTH_GUARD_SPEC = 1037,
    FLAG_OBTAINED_ETERNA_CITY_TM46 = 1038,
    FLAG_UNUSED_0x040F = 1039,
    FLAG_OBTAINED_ETERNA_CITY_SUPER_POTION = 1040,
    FLAG_OBTAINED_ROUTE_211_WEST_TM12 = 1041,
    FLAG_OBTAINED_ETERNA_FOREST_OUTSIDE_TM82 = 1042,
    FLAG_OBTAINED_ETERNA_FOREST_OUTSIDE_SILVER_POWDER = 1043,
    FLAG_OBTAINED_ROUTE_206_POISON_BARB = 1044,
    FLAG_OBTAINED_ROUTE_206_PP_UP = 1045,
    FLAG_OBTAINED_ROUTE_206_SUPER_REPEL = 1046,
    FLAG_OBTAINED_ROUTE_206_BURN_HEAL = 1047,
    FLAG_OBTAINED_WAYWARD_CAVE_1F_TM32 = 1048,
    FLAG_OBTAINED_WAYWARD_CAVE_1F_REVIVE = 1049,
    FLAG_OBTAINED_WAYWARD_CAVE_1F_ESCAPE_ROPE = 1050,
    FLAG_OBTAINED_MT_CORONET_4F_ROOMS_1_AND_2_ADAMANT_ORB = 1051,
    FLAG_OBTAINED_WAYWARD_CAVE_1F_TM26 = 1052,
    FLAG_OBTAINED_MT_CORONET_4F_ROOMS_1_AND_2_LUSTROUS_ORB = 1053,
    FLAG_OBTAINED_WAYWARD_CAVE_B1F_MAX_ETHER = 1054,
    FLAG_OBTAINED_WAYWARD_CAVE_B1F_RARE_CANDY = 1055,
    FLAG_OBTAINED_WAYWARD_CAVE_B1F_GRIP_CLAW = 1056,
    FLAG_OBTAINED_ROUTE_207_POKE_BALL = 1057,
    FLAG_OBTAINED_ROUTE_207_REVIVE = 1058,
    FLAG_OBTAINED_ROUTE_207_IRON = 1059,
    FLAG_OBTAINED_MT_CORONET_1F_SOUTH_PROTEIN = 1060,
    FLAG_OBTAINED_MT_CORONET_1F_SOUTH_DAWN_STONE = 1061,
    FLAG_OBTAINED_ROUTE_208_CARBOS = 1062,
    FLAG_OBTAINED_ROUTE_208_GREAT_BALL = 1063,
    FLAG_OBTAINED_ROUTE_208_ETHER = 1064,
    FLAG_OBTAINED_AMITY_SQUARE_TM45 = 1065,
    FLAG_OBTAINED_AMITY_SQUARE_TM43 = 1066,
    FLAG_OBTAINED_AMITY_SQUARE_SPOOKY_PLATE = 1067,
    FLAG_OBTAINED_ROUTE_212_NORTH_TM11 = 1068,
    FLAG_OBTAINED_ROUTE_212_NORTH_ROSE_INCENSE = 1069,
    FLAG_OBTAINED_ROUTE_212_NORTH_ELIXIR = 1070,
    FLAG_OBTAINED_POKEMON_MANSION_OFFICE_TM87 = 1071,
    FLAG_OBTAINED_POKEMON_MANSION_MAIDS_ROOM_LUXURY_BALL = 1072,
    FLAG_OBTAINED_ROUTE_212_SOUTH_FULL_HEAL = 1073,
    FLAG_OBTAINED_ROUTE_212_SOUTH_TM84 = 1074,
    FLAG_OBTAINED_ROUTE_212_SOUTH_TM62 = 1075,
    FLAG_OBTAINED_ROUTE_212_SOUTH_ZINC = 1076,
    FLAG_OBTAINED_ROUTE_212_SOUTH_HYPER_POTION = 1077,
    FLAG_OBTAINED_ROUTE_212_SOUTH_TM06 = 1078,
    FLAG_OBTAINED_ROUTE_212_SOUTH_REVIVE = 1079,
    FLAG_OBTAINED_GREAT_MARSH_1_YELLOW_SHARD = 1080,
    FLAG_OBTAINED_GREAT_MARSH_1_POKE_BALL = 1081,
    FLAG_OBTAINED_GREAT_MARSH_2_BLUE_SHARD = 1082,
    FLAG_OBTAINED_GREAT_MARSH_4_POKE_BALL = 1083,
    FLAG_OBTAINED_GREAT_MARSH_3_GREAT_BALL = 1084,
    FLAG_OBTAINED_GREAT_MARSH_4_GREEN_SHARD = 1085,
    FLAG_OBTAINED_GREAT_MARSH_5_GREAT_BALL = 1086,
    FLAG_OBTAINED_GREAT_MARSH_6_RED_SHARD = 1087,
    FLAG_OBTAINED_PASTORIA_CITY_MYSTIC_WATER = 1088,
    FLAG_OBTAINED_ROUTE_213_TM05 = 1089,
    FLAG_OBTAINED_ROUTE_213_TM40 = 1090,
    FLAG_OBTAINED_ROUTE_213_YELLOW_SHARD = 1091,
    FLAG_OBTAINED_ROUTE_213_MAX_REVIVE = 1092,
    FLAG_OBTAINED_ROUTE_213_WATER_STONE = 1093,
    FLAG_OBTAINED_ROUTE_213_PP_UP = 1094,
    FLAG_OBTAINED_ROUTE_213_PROTEIN = 1095,
    FLAG_OBTAINED_ROUTE_204_SOUTH_PARLYZ_HEAL = 1096,
    FLAG_OBTAINED_VALOR_LAKEFRONT_TM85 = 1097,
    FLAG_OBTAINED_VALOR_LAKEFRONT_ULTRA_BALL = 1098,
    FLAG_OBTAINED_ETERNA_FOREST_ETHER = 1099,
    FLAG_OBTAINED_ROUTE_209_GREAT_BALL = 1100,
    FLAG_OBTAINED_ROUTE_209_HYPER_POTION = 1101,
    FLAG_OBTAINED_ROUTE_209_CALCIUM = 1102,
    FLAG_OBTAINED_ROUTE_209_TM47 = 1103,
    FLAG_OBTAINED_ROUTE_209_TM19 = 1104,
    FLAG_OBTAINED_ROUTE_209_LOST_TOWER_2F_OVAL_STONE = 1105,
    FLAG_OBTAINED_FULLMOON_ISLAND_FOREST_LUNAR_WING = 1106,
    FLAG_OBTAINED_ROUTE_209_LOST_TOWER_3F_REVIVE = 1107,
    FLAG_OBTAINED_ROUTE_209_LOST_TOWER_4F_TM27 = 1108,
    FLAG_OBTAINED_SOLACEON_RUINS_ROOM_7_MIND_PLATE = 1109,
    FLAG_OBTAINED_SOLACEON_RUINS_ROOM_7_ODD_INCENSE = 1110,
    FLAG_OBTAINED_SOLACEON_RUINS_ROOM_7_NUGGET = 1111,
    FLAG_OBTAINED_SOLACEON_RUINS_ROOM_7_HM05 = 1112,
    FLAG_OBTAINED_ROUTE_210_SOUTH_SUPER_REPEL = 1113,
    FLAG_OBTAINED_ROUTE_210_SOUTH_MAX_REPEL = 1114,
    FLAG_OBTAINED_ROUTE_221_PURE_INCENSE = 1115,
    FLAG_OBTAINED_ROUTE_210_SOUTH_NEST_BALL = 1116,
    FLAG_OBTAINED_ROUTE_210_SOUTH_HYPER_POTION = 1117,
    FLAG_OBTAINED_ROUTE_215_FIST_PLATE = 1118,
    FLAG_OBTAINED_ROUTE_215_HYPER_POTION = 1119,
    FLAG_OBTAINED_ROUTE_215_MAX_ETHER = 1120,
    FLAG_OBTAINED_ROUTE_215_FULL_HEAL = 1121,
    FLAG_OBTAINED_ROUTE_215_TM34 = 1122,
    FLAG_OBTAINED_VEILSTONE_CITY_FULL_INCENSE = 1123,
    FLAG_OBTAINED_VEILSTONE_CITY_RARE_CANDY = 1124,
    FLAG_OBTAINED_VEILSTONE_CITY_GALACTIC_WAREHOUSE_DUSK_STONE = 1125,
    FLAG_OBTAINED_ROUTE_214_BIG_ROOT = 1126,
    FLAG_OBTAINED_ROUTE_214_RED_SHARD = 1127,
    FLAG_OBTAINED_ROUTE_214_MAX_REPEL = 1128,
    FLAG_OBTAINED_ROUTE_214_RARE_CANDY = 1129,
    FLAG_OBTAINED_RUIN_MANIAC_CAVE_OR_TUNNEL_TM28 = 1130,
    FLAG_OBTAINED_ROUTE_210_NORTH_SMOKE_BALL = 1131,
    FLAG_OBTAINED_ROUTE_210_NORTH_WAVE_INCENSE = 1132,
    FLAG_OBTAINED_ROUTE_210_NORTH_TM30 = 1133,
    FLAG_OBTAINED_FUEGO_IRONWORKS_BUILDING_TM35 = 1134,
    FLAG_OBTAINED_FUEGO_IRONWORKS_BUILDING_ROCK_INCENSE = 1135,
    FLAG_OBTAINED_FUEGO_IRONWORKS_BUILDING_CALCIUM = 1136,
    FLAG_OBTAINED_FUEGO_IRONWORKS_BUILDING_FIRE_STONE = 1137,
    FLAG_OBTAINED_ROUTE_219_ANTIDOTE = 1138,
    FLAG_OBTAINED_ROUTE_220_SPLASH_PLATE = 1139,
    FLAG_OBTAINED_ROUTE_205_SOUTH_REPEL = 1140,
    FLAG_OBTAINED_ROUTE_220_CARBOS = 1141,
    FLAG_OBTAINED_ROUTE_220_MAX_REPEL = 1142,
    FLAG_OBTAINED_ROUTE_221_ULTRA_BALL = 1143,
    FLAG_OBTAINED_ROUTE_221_PROTEIN = 1144,
    FLAG_OBTAINED_ROUTE_221_TM81 = 1145,
    FLAG_OBTAINED_ROUTE_218_RARE_CANDY = 1146,
    FLAG_OBTAINED_ROUTE_218_HYPER_POTION = 1147,
    FLAG_OBTAINED_CANALAVE_CITY_TM89 = 1148,
    FLAG_OBTAINED_IRON_ISLAND_B1F_LEFT_ROOM_PROTECTOR = 1149,
    FLAG_OBTAINED_IRON_ISLAND_B1F_RIGHT_ROOM_ESCAPE_ROPE = 1150,
    FLAG_OBTAINED_IRON_ISLAND_B1F_RIGHT_ROOM_MAX_REPEL = 1151,
    FLAG_OBTAINED_IRON_ISLAND_B2F_RIGHT_ROOM_ELIXIR = 1152,
    FLAG_OBTAINED_IRON_ISLAND_B2F_RIGHT_ROOM_TM23 = 1153,
    FLAG_OBTAINED_IRON_ISLAND_B2F_LEFT_ROOM_DUSK_BALL = 1154,
    FLAG_OBTAINED_IRON_ISLAND_B2F_LEFT_ROOM_MAX_POTION = 1155,
    FLAG_OBTAINED_IRON_ISLAND_B2F_LEFT_ROOM_ULTRA_BALL = 1156,
    FLAG_OBTAINED_IRON_ISLAND_B2F_LEFT_ROOM_HP_UP = 1157,
    FLAG_OBTAINED_IRON_ISLAND_B3F_SHINY_STONE = 1158,
    FLAG_OBTAINED_ROUTE_207_SUPER_POTION = 1159,
    FLAG_OBTAINED_FLOAROMA_MEADOW_MIRACLE_SEED = 1160,
    FLAG_OBTAINED_ROUTE_211_EAST_TM29 = 1161,
    FLAG_OBTAINED_MT_CORONET_1F_NORTH_ROOM_1_TM69 = 1162,
    FLAG_OBTAINED_MT_CORONET_1F_NORTH_ROOM_1_ESCAPE_ROPE = 1163,
    FLAG_OBTAINED_MT_CORONET_1F_NORTH_ROOM_1_RARE_CANDY = 1164,
    FLAG_OBTAINED_MT_CORONET_B1F_SOFT_SAND = 1165,
    FLAG_OBTAINED_MT_CORONET_B1F_LIGHT_CLAY = 1166,
    FLAG_OBTAINED_MT_CORONET_B1F_ULTRA_BALL = 1167,
    FLAG_OBTAINED_MT_CORONET_B1F_FULL_RESTORE = 1168,
    FLAG_OBTAINED_MT_CORONET_B1F_MAX_ELIXIR = 1169,
    FLAG_OBTAINED_MT_CORONET_B1F_REVIVE = 1170,
    FLAG_OBTAINED_ROUTE_216_TM13 = 1171,
    FLAG_OBTAINED_ROUTE_216_MENTAL_HERB = 1172,
    FLAG_OBTAINED_ROUTE_216_FULL_HEAL = 1173,
    FLAG_OBTAINED_ROUTE_217_TM07 = 1174,
    FLAG_OBTAINED_ROUTE_217_HM08 = 1175,
    FLAG_OBTAINED_ROUTE_217_IRON = 1176,
    FLAG_OBTAINED_ACUITY_LAKEFRONT_ULTRA_BALL = 1177,
    FLAG_OBTAINED_GALACTIC_HQ_B2F_TM36 = 1178,
    FLAG_OBTAINED_GALACTIC_HQ_1F_TM49 = 1179,
    FLAG_OBTAINED_GALACTIC_HQ_3F_TM21 = 1180,
    FLAG_OBTAINED_GALACTIC_HQ_2F_REVIVE = 1181,
    FLAG_OBTAINED_GALACTIC_HQ_3F_MAX_ELIXIR = 1182,
    FLAG_OBTAINED_OREBURGH_GATE_B1F_BIG_PEARL = 1183,
    FLAG_OBTAINED_MT_CORONET_2F_MAX_REVIVE = 1184,
    FLAG_OBTAINED_MT_CORONET_2F_TM80 = 1185,
    FLAG_OBTAINED_MT_CORONET_2F_MAX_REPEL = 1186,
    FLAG_OBTAINED_MT_CORONET_2F_ESCAPE_ROPE = 1187,
    FLAG_OBTAINED_MT_CORONET_1F_NORTH_ROOM_1_TM02 = 1188,
    FLAG_OBTAINED_ROUTE_222_CARBOS = 1189,
    FLAG_OBTAINED_ROUTE_222_FULL_RESTORE = 1190,
    FLAG_OBTAINED_SUNYSHORE_CITY_THUNDERSTONE = 1191,
    FLAG_OBTAINED_ROUTE_223_ULTRA_BALL = 1192,
    FLAG_OBTAINED_ROUTE_223_DIVE_BALL = 1193,
    FLAG_OBTAINED_ROUTE_223_TM18 = 1194,
    FLAG_OBTAINED_VICTORY_ROAD_1F_TM41 = 1195,
    FLAG_OBTAINED_VICTORY_ROAD_1F_MAX_REPEL = 1196,
    FLAG_OBTAINED_VICTORY_ROAD_1F_RARE_CANDY = 1197,
    FLAG_OBTAINED_VICTORY_ROAD_1F_ZINC = 1198,
    FLAG_OBTAINED_VICTORY_ROAD_1F_RAZOR_CLAW = 1199,
    FLAG_OBTAINED_VICTORY_ROAD_2F_MAX_ELIXIR = 1200,
    FLAG_OBTAINED_VICTORY_ROAD_2F_FULL_RESTORE = 1201,
    FLAG_OBTAINED_VICTORY_ROAD_2F_TM79 = 1202,
    FLAG_OBTAINED_VICTORY_ROAD_2F_TM71 = 1203,
    FLAG_OBTAINED_VICTORY_ROAD_B1F_TM59 = 1204,
    FLAG_OBTAINED_VICTORY_ROAD_1F_ROOM_2_FULL_RESTORE = 1205,
    FLAG_OBTAINED_VICTORY_ROAD_1F_ROOM_2_DUSK_STONE = 1206,
    FLAG_OBTAINED_VICTORY_ROAD_1F_ROOM_2_NUGGET = 1207,
    FLAG_OBTAINED_ROUTE_224_RAZOR_CLAW = 1208,
    FLAG_OBTAINED_ROUTE_224_RARE_CANDY = 1209,
    FLAG_OBTAINED_ROUTE_224_DESTINY_KNOT = 1210,
    FLAG_OBTAINED_VICTORY_ROAD_1F_ROOM_2_LEFTOVERS = 1211,
    FLAG_OBTAINED_ROUTE_225_RAZOR_FANG = 1212,
    FLAG_OBTAINED_ROUTE_225_RARE_CANDY = 1213,
    FLAG_OBTAINED_ROUTE_225_HP_UP = 1214,
    FLAG_OBTAINED_ROUTE_225_LAX_INCENSE = 1215,
    FLAG_OBTAINED_ROUTE_225_DUBIOUS_DISC = 1216,
    FLAG_OBTAINED_ROUTE_225_DAWN_STONE = 1217,
    FLAG_OBTAINED_ROUTE_225_REVIVE = 1218,
    FLAG_OBTAINED_ROUTE_226_TM53 = 1219,
    FLAG_OBTAINED_ROUTE_226_LAGGING_TAIL = 1220,
    FLAG_OBTAINED_ROUTE_226_CARBOS = 1221,
    FLAG_OBTAINED_ROUTE_227_CHARCOAL = 1222,
    FLAG_OBTAINED_ROUTE_227_ZINC = 1223,
    FLAG_OBTAINED_STARK_MOUNTAIN_OUTSIDE_MAX_ELIXIR = 1224,
    FLAG_OBTAINED_STARK_MOUNTAIN_OUTSIDE_LIFE_ORB = 1225,
    FLAG_OBTAINED_STARK_MOUNTAIN_ROOM_1_FULL_HEAL = 1226,
    FLAG_OBTAINED_STARK_MOUNTAIN_ROOM_1_ESCAPE_ROPE = 1227,
    FLAG_OBTAINED_STARK_MOUNTAIN_ROOM_1_PP_UP = 1228,
    FLAG_OBTAINED_STARK_MOUNTAIN_ROOM_1_FULL_RESTORE = 1229,
    FLAG_OBTAINED_STARK_MOUNTAIN_ROOM_2_TM50 = 1230,
    FLAG_OBTAINED_STARK_MOUNTAIN_ROOM_2_IRON_BALL = 1231,
    FLAG_OBTAINED_STARK_MOUNTAIN_ROOM_2_MAX_ELIXIR = 1232,
    FLAG_OBTAINED_STARK_MOUNTAIN_ROOM_2_NUGGET = 1233,
    FLAG_OBTAINED_STARK_MOUNTAIN_ROOM_2_RARE_CANDY = 1234,
    FLAG_OBTAINED_STARK_MOUNTAIN_ROOM_2_ULTRA_BALL = 1235,
    FLAG_OBTAINED_STARK_MOUNTAIN_ROOM_2_MAX_REVIVE = 1236,
    FLAG_OBTAINED_STARK_MOUNTAIN_ROOM_2_CALCIUM = 1237,
    FLAG_OBTAINED_ROUTE_228_PROTECTOR = 1238,
    FLAG_OBTAINED_ROUTE_228_IRON = 1239,
    FLAG_OBTAINED_ROUTE_228_SHINY_STONE = 1240,
    FLAG_OBTAINED_ROUTE_228_SHED_SHELL = 1241,
    FLAG_OBTAINED_ROUTE_228_TM37 = 1242,
    FLAG_OBTAINED_ROUTE_229_REAPER_CLOTH = 1243,
    FLAG_OBTAINED_ROUTE_229_PROTEIN = 1244,
    FLAG_OBTAINED_ROUTE_229_FULL_RESTORE = 1245,
    FLAG_OBTAINED_RESORT_AREA_NUGGET = 1246,
    FLAG_OBTAINED_ROUTE_230_RARE_CANDY = 1247,
    FLAG_OBTAINED_FLOAROMA_MEADOW_LEAF_STONE = 1248,
    FLAG_OBTAINED_OREBURGH_MINE_B1F_POKE_BALL = 1249,
    FLAG_OBTAINED_VEILSTONE_CITY_GALACTIC_WAREHOUSE_HM02 = 1250,
    FLAG_OBTAINED_GALACTIC_HQ_B2F_GALACTIC_KEY = 1251,
    FLAG_UNUSED_0x04E4 = 1252,
    FLAG_OBTAINED_LAKE_VERITY_TM38 = 1253,
    FLAG_OBTAINED_LAKE_VALOR_TM25 = 1254,
    FLAG_OBTAINED_LAKE_ACUITY_TM14 = 1255,
    FLAG_OBTAINED_SNOWPOINT_TEMPLE_B1F_CALCIUM = 1256,
    FLAG_OBTAINED_SNOWPOINT_TEMPLE_B4F_FULL_HEAL = 1257,
    FLAG_OBTAINED_GALACTIC_HQ_B2F_ZINC = 1258,
    FLAG_OBTAINED_PAL_PARK_RAZOR_CLAW = 1259,
    FLAG_OBTAINED_ROUTE_216_HP_UP = 1260,
    FLAG_OBTAINED_OLD_CHATEAU_BACK_WEST_ROOM_DREAD_PLATE = 1261,
    FLAG_OBTAINED_OLD_CHATEAU_BACK_EAST_ROOM_TM90 = 1262,
    FLAG_OBTAINED_ETERNA_FOREST_POTION = 1263,
    FLAG_OBTAINED_AMITY_SQUARE_AMULET_COIN = 1264,
    FLAG_OBTAINED_RAVAGED_PATH_POTION = 1265,
    FLAG_OBTAINED_VALLEY_WINDWORKS_OUTSIDE_POTION = 1266,
    FLAG_OBTAINED_ROUTE_205_SOUTH_SUPER_POTION = 1267,
    FLAG_OBTAINED_ROUTE_204_NORTH_AWAKENING = 1268,
    FLAG_UNUSED_0x04F5 = 1269,
    FLAG_OBTAINED_ROUTE_203_X_DEFENSE = 1270,
    FLAG_OBTAINED_ROUTE_218_X_ACCURACY = 1271,
    FLAG_OBTAINED_JUBILIFE_CITY_POTION = 1272,
    FLAG_OBTAINED_TRAINERS_SCHOOL_X_ATTACK = 1273,
    FLAG_OBTAINED_OREBURGH_CITY_DIRE_HIT = 1274,
    FLAG_OBTAINED_OREBURGH_CITY_YELLOW_SHARD = 1275,
    FLAG_OBTAINED_MT_CORONET_1F_NORTH_ROOM_1_ICE_HEAL = 1276,
    FLAG_OBTAINED_TEAM_GALACTIC_ETERNA_BUILDING_2F_X_SPEED = 1277,
    FLAG_OBTAINED_TEAM_GALACTIC_ETERNA_BUILDING_3F_X_SPECIAL = 1278,
    FLAG_OBTAINED_TEAM_GALACTIC_ETERNA_BUILDING_4F_BLUE_SHARD = 1279,
    FLAG_OBTAINED_TEAM_GALACTIC_ETERNA_BUILDING_4F_UPGRADE = 1280,
    FLAG_OBTAINED_OLD_CHATEAU_DINING_AREA_RARE_CANDY = 1281,
    FLAG_OBTAINED_OLD_CHATEAU_DINING_AREA_BIG_PEARL = 1282,
    FLAG_OBTAINED_ROUTE_209_ETHER = 1283,
    FLAG_OBTAINED_SOLACEON_TOWN_PP_UP = 1284,
    FLAG_OBTAINED_ROUTE_215_HP_UP = 1285,
    FLAG_OBTAINED_VEILSTONE_CITY_STAR_PIECE = 1286,
    FLAG_OBTAINED_ROUTE_212_NORTH_IRON = 1287,
    FLAG_OBTAINED_ROUTE_212_NORTH_GREEN_SHARD = 1288,
    FLAG_OBTAINED_ROUTE_212_SOUTH_BLUE_SHARD = 1289,
    FLAG_OBTAINED_ROUTE_212_SOUTH_ELIXIR = 1290,
    FLAG_OBTAINED_SOLACEON_TOWN_RARE_CANDY = 1291,
    FLAG_OBTAINED_TEAM_GALACTIC_ETERNA_BUILDING_4F_REVIVE = 1292,
    FLAG_OBTAINED_ROUTE_210_NORTH_RED_SHARD = 1293,
    FLAG_OBTAINED_ROUTE_210_NORTH_ZINC = 1294,
    FLAG_OBTAINED_FUEGO_IRONWORKS_BUILDING_BLUE_SHARD = 1295,
    FLAG_OBTAINED_FUEGO_IRONWORKS_BUILDING_RED_SHARD = 1296,
    FLAG_OBTAINED_FUEGO_IRONWORKS_BUILDING_GREEN_SHARD = 1297,
    FLAG_OBTAINED_FUEGO_IRONWORKS_BUILDING_YELLOW_SHARD = 1298,
    FLAG_OBTAINED_FLOAROMA_MEADOW_ULTRA_BALL = 1299,
    FLAG_OBTAINED_FLOAROMA_MEADOW_RARE_CANDY = 1300,
    FLAG_OBTAINED_IRON_ISLAND_B2F_LEFT_ROOM_RED_SHARD = 1301,
    FLAG_OBTAINED_IRON_ISLAND_B1F_LEFT_ROOM_YELLOW_SHARD = 1302,
    FLAG_OBTAINED_IRON_ISLAND_B2F_LEFT_ROOM_NUGGET = 1303,
    FLAG_OBTAINED_IRON_ISLAND_B1F_RIGHT_ROOM_REVIVE = 1304,
    FLAG_OBTAINED_ROUTE_216_MAX_POTION = 1305,
    FLAG_OBTAINED_ROUTE_216_REVIVE = 1306,
    FLAG_OBTAINED_ROUTE_217_BLUE_SHARD = 1307,
    FLAG_OBTAINED_ROUTE_217_ULTRA_BALL = 1308,
    FLAG_OBTAINED_ROUTE_217_RARE_CANDY = 1309,
    FLAG_OBTAINED_GALACTIC_HQ_3F_PROTEIN = 1310,
    FLAG_OBTAINED_GALACTIC_HQ_LABORATORY_FULL_RESTORE = 1311,
    FLAG_OBTAINED_GALACTIC_HQ_1F_DUBIOUS_DISC = 1312,
    FLAG_OBTAINED_GALACTIC_HQ_2F_GREEN_SHARD = 1313,
    FLAG_OBTAINED_ROUTE_222_QUICK_BALL = 1314,
    FLAG_OBTAINED_ROUTE_222_PP_UP = 1315,
    FLAG_OBTAINED_ROUTE_223_RARE_CANDY = 1316,
    FLAG_OBTAINED_ROUTE_224_UPGRADE = 1317,
    FLAG_OBTAINED_ROUTE_224_REPEAT_BALL = 1318,
    FLAG_OBTAINED_ROUTE_224_PP_MAX = 1319,
    FLAG_OBTAINED_ROUTE_224_ULTRA_BALL = 1320,
    FLAG_OBTAINED_ROUTE_225_GREEN_SHARD = 1321,
    FLAG_OBTAINED_ROUTE_227_YELLOW_SHARD = 1322,
    FLAG_OBTAINED_ROUTE_230_BLUE_SHARD = 1323,
    FLAG_OBTAINED_ACUITY_LAKEFRONT_REAPER_CLOTH = 1324,
    FLAG_OBTAINED_SNOWPOINT_CITY_ULTRA_BALL = 1325,
    FLAG_OBTAINED_SURVIVAL_AREA_RED_SHARD = 1326,
    FLAG_OBTAINED_VICTORY_ROAD_B1F_ULTRA_BALL = 1327,
    FLAG_OBTAINED_VICTORY_ROAD_B1F_CALCIUM = 1328,
    FLAG_OBTAINED_SURVIVAL_AREA_RARE_CANDY = 1329,
    FLAG_OBTAINED_VALOR_LAKEFRONT_IRON = 1330,
    FLAG_OBTAINED_ROUTE_211_EAST_CARBOS = 1331,
    FLAG_OBTAINED_ROUTE_214_MAGMARIZER = 1332,
    FLAG_OBTAINED_VALLEY_WINDWORKS_OUTSIDE_ELECTIRIZER = 1333,
    FLAG_OBTAINED_CELESTIC_TOWN_DRAGON_FANG = 1334,
    FLAG_OBTAINED_ROUTE_207_TIMER_BALL = 1335,
    FLAG_OBTAINED_DISTORTION_WORLD_TURNBACK_CAVE_ROOM_GRISEOUS_ORB = 1336,
    FLAG_OBTAINED_IRON_ISLAND_IRON_RUINS_METAL_COAT = 1337,
    FLAG_OBTAINED_MT_CORONET_ICEBERG_RUINS_NEVERMELTICE = 1338,
    FLAG_OBTAINED_ROUTE_228_ROCK_PEAK_RUINS_HARD_STONE = 1339,
    FLAG_UNUSED_0x053C = 1340,
    FLAG_UNUSED_0x053D = 1341,
    FLAG_UNUSED_0x053E = 1342,
    FLAG_UNUSED_0x053F = 1343,
    FLAG_UNUSED_0x0540 = 1344,
    FLAG_UNUSED_0x0541 = 1345,
    FLAG_UNUSED_0x0542 = 1346,
    FLAG_UNUSED_0x0543 = 1347,
    FLAG_UNUSED_0x0544 = 1348,
    FLAG_UNUSED_0x0545 = 1349,
    FLAG_UNUSED_0x0546 = 1350,
    FLAG_UNUSED_0x0547 = 1351,
    FLAG_UNUSED_0x0548 = 1352,
    FLAG_UNUSED_0x0549 = 1353,
    FLAG_UNUSED_0x054A = 1354,
    FLAG_UNUSED_0x054B = 1355,
    FLAG_UNUSED_0x054C = 1356,
    FLAG_UNUSED_0x054D = 1357,
    FLAG_UNUSED_0x054E = 1358,
    FLAG_UNUSED_0x054F = 1359,
    TRAINER_DEFEATED_FLAGS_START = 1360,
    FLAG_DEFEATED_TRAINER_NONE = 1360,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_TRISTAN = 1361,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_LOGAN = 1362,
    FLAG_DEFEATED_TRAINER_LASS_NATALIE = 1363,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_MICHAEL = 1364,
    FLAG_DEFEATED_TRAINER_DUMMY_005 = 1365,
    FLAG_DEFEATED_TRAINER_DUMMY_006 = 1366,
    FLAG_DEFEATED_TRAINER_DUMMY_007 = 1367,
    FLAG_DEFEATED_TRAINER_DUMMY_008 = 1368,
    FLAG_DEFEATED_TRAINER_DUMMY_009 = 1369,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_TYLER = 1370,
    FLAG_DEFEATED_TRAINER_LASS_SAMANTHA = 1371,
    FLAG_DEFEATED_TRAINER_LASS_SARAH = 1372,
    FLAG_DEFEATED_TRAINER_BUG_CATCHER_BRANDON = 1373,
    FLAG_DEFEATED_TRAINER_AROMA_LADY_TAYLOR = 1374,
    FLAG_DEFEATED_TRAINER_TWINS_LIV_AND_LIZ = 1375,
    FLAG_DEFEATED_TRAINER_CAMPER_JACOB = 1376,
    FLAG_DEFEATED_TRAINER_PICNICKER_SIENA = 1377,
    FLAG_DEFEATED_TRAINER_HIKER_DANIEL = 1378,
    FLAG_DEFEATED_TRAINER_HIKER_NICHOLAS = 1379,
    FLAG_DEFEATED_TRAINER_BATTLE_GIRL_KELSEY = 1380,
    FLAG_DEFEATED_TRAINER_AROMA_LADY_ELIZABETH = 1381,
    FLAG_DEFEATED_TRAINER_FISHERMAN_ANDREW = 1382,
    FLAG_DEFEATED_TRAINER_FISHERMAN_JOSEPH = 1383,
    FLAG_DEFEATED_TRAINER_FISHERMAN_ZACHARY = 1384,
    FLAG_DEFEATED_TRAINER_CYCLIST_AXEL = 1385,
    FLAG_DEFEATED_TRAINER_CYCLIST_JAMES = 1386,
    FLAG_DEFEATED_TRAINER_CYCLIST_JOHN = 1387,
    FLAG_DEFEATED_TRAINER_CYCLIST_RYAN = 1388,
    FLAG_DEFEATED_TRAINER_CYCLIST_MEGAN = 1389,
    FLAG_DEFEATED_TRAINER_CYCLIST_NICOLE = 1390,
    FLAG_DEFEATED_TRAINER_CYCLIST_KAYLA = 1391,
    FLAG_DEFEATED_TRAINER_CYCLIST_RACHEL = 1392,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_AUSTIN = 1393,
    FLAG_DEFEATED_TRAINER_CAMPER_ANTHONY = 1394,
    FLAG_DEFEATED_TRAINER_PICNICKER_LAUREN = 1395,
    FLAG_DEFEATED_TRAINER_HIKER_KEVIN = 1396,
    FLAG_DEFEATED_TRAINER_HIKER_JUSTIN = 1397,
    FLAG_DEFEATED_TRAINER_BATTLE_GIRL_HELEN = 1398,
    FLAG_DEFEATED_TRAINER_HIKER_ROBERT = 1399,
    FLAG_DEFEATED_TRAINER_HIKER_ALEXANDER = 1400,
    FLAG_DEFEATED_TRAINER_HIKER_JONATHAN = 1401,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_KYLE = 1402,
    FLAG_DEFEATED_TRAINER_FISHERMAN_CODY = 1403,
    FLAG_DEFEATED_TRAINER_AROMA_LADY_HANNAH = 1404,
    FLAG_DEFEATED_TRAINER_ARTIST_WILLIAM = 1405,
    FLAG_DEFEATED_TRAINER_BREEDER_ALBERT = 1406,
    FLAG_DEFEATED_TRAINER_BREEDER_JENNIFER = 1407,
    FLAG_DEFEATED_TRAINER_COWGIRL_SHELLEY = 1408,
    FLAG_DEFEATED_TRAINER_JOGGER_RICHARD = 1409,
    FLAG_DEFEATED_TRAINER_DUMMY_050 = 1410,
    FLAG_DEFEATED_TRAINER_DUMMY_051 = 1411,
    FLAG_DEFEATED_TRAINER_DUMMY_052 = 1412,
    FLAG_DEFEATED_TRAINER_POKE_KID_DANIELLE = 1413,
    FLAG_DEFEATED_TRAINER_DUMMY_054 = 1414,
    FLAG_DEFEATED_TRAINER_YOUNG_COUPLE_TY_AND_SUE = 1415,
    FLAG_DEFEATED_TRAINER_BREEDER_KAHLIL = 1416,
    FLAG_DEFEATED_TRAINER_BREEDER_AMBER = 1417,
    FLAG_DEFEATED_TRAINER_DUMMY_058 = 1418,
    FLAG_DEFEATED_TRAINER_DUMMY_059 = 1419,
    FLAG_DEFEATED_TRAINER_DUMMY_060 = 1420,
    FLAG_DEFEATED_TRAINER_DUMMY_061 = 1421,
    FLAG_DEFEATED_TRAINER_DUMMY_062 = 1422,
    FLAG_DEFEATED_TRAINER_DUMMY_063 = 1423,
    FLAG_DEFEATED_TRAINER_DUMMY_064 = 1424,
    FLAG_DEFEATED_TRAINER_TWINS_TERI_AND_TIA = 1425,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_ERNEST = 1426,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_ALYSSA = 1427,
    FLAG_DEFEATED_TRAINER_VETERAN_BRIAN = 1428,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_ADAM = 1429,
    FLAG_DEFEATED_TRAINER_NINJA_BOY_JOEL = 1430,
    FLAG_DEFEATED_TRAINER_NINJA_BOY_NATHAN = 1431,
    FLAG_DEFEATED_TRAINER_NINJA_BOY_DAVIDO = 1432,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_PATRICK = 1433,
    FLAG_DEFEATED_TRAINER_BIRD_KEEPER_BRIANNA = 1434,
    FLAG_DEFEATED_TRAINER_DOUBLE_TEAM_ZAC_AND_JEN = 1435,
    FLAG_DEFEATED_TRAINER_BIRD_KEEPER_ALEXANDRA = 1436,
    FLAG_DEFEATED_TRAINER_DUMMY_077 = 1437,
    FLAG_DEFEATED_TRAINER_NINJA_BOY_ZACH = 1438,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_SEAN = 1439,
    FLAG_DEFEATED_TRAINER_NINJA_BOY_NICK = 1440,
    FLAG_DEFEATED_TRAINER_BIRD_KEEPER_KATHERINE = 1441,
    FLAG_DEFEATED_TRAINER_RICH_BOY_JASON = 1442,
    FLAG_DEFEATED_TRAINER_LADY_MELISSA = 1443,
    FLAG_DEFEATED_TRAINER_GENTLEMAN_JEREMY = 1444,
    FLAG_DEFEATED_TRAINER_SOCIALITE_REINA = 1445,
    FLAG_DEFEATED_TRAINER_DUMMY_086 = 1446,
    FLAG_DEFEATED_TRAINER_DUMMY_087 = 1447,
    FLAG_DEFEATED_TRAINER_POLICEMAN_BOBBY = 1448,
    FLAG_DEFEATED_TRAINER_POLICEMAN_ALEX = 1449,
    FLAG_DEFEATED_TRAINER_POLICEMAN_DYLAN = 1450,
    FLAG_DEFEATED_TRAINER_FISHERMAN_JUAN = 1451,
    FLAG_DEFEATED_TRAINER_FISHERMAN_JOSH = 1452,
    FLAG_DEFEATED_TRAINER_FISHERMAN_TRAVIS = 1453,
    FLAG_DEFEATED_TRAINER_RANGER_TAYLOR = 1454,
    FLAG_DEFEATED_TRAINER_RANGER_JEFFREY = 1455,
    FLAG_DEFEATED_TRAINER_RANGER_ALLISON = 1456,
    FLAG_DEFEATED_TRAINER_DUMMY_097 = 1457,
    FLAG_DEFEATED_TRAINER_DUMMY_098 = 1458,
    FLAG_DEFEATED_TRAINER_SCIENTIST_STEFANO = 1459,
    FLAG_DEFEATED_TRAINER_DUMMY_100 = 1460,
    FLAG_DEFEATED_TRAINER_DUMMY_101 = 1461,
    FLAG_DEFEATED_TRAINER_POLICEMAN_CALEB = 1462,
    FLAG_DEFEATED_TRAINER_SWIMMER_SHELTIN = 1463,
    FLAG_DEFEATED_TRAINER_SWIMMER_EVAN = 1464,
    FLAG_DEFEATED_TRAINER_SWIMMER_HALEY = 1465,
    FLAG_DEFEATED_TRAINER_SWIMMER_MARY = 1466,
    FLAG_DEFEATED_TRAINER_TUBER_JARED = 1467,
    FLAG_DEFEATED_TRAINER_TUBER_CHELSEA = 1468,
    FLAG_DEFEATED_TRAINER_DUMMY_109 = 1469,
    FLAG_DEFEATED_TRAINER_SAILOR_PAUL = 1470,
    FLAG_DEFEATED_TRAINER_FISHERMAN_KENNETH = 1471,
    FLAG_DEFEATED_TRAINER_DUMMY_112 = 1472,
    FLAG_DEFEATED_TRAINER_RUIN_MANIAC_BRYAN = 1473,
    FLAG_DEFEATED_TRAINER_RUIN_MANIAC_RONALD = 1474,
    FLAG_DEFEATED_TRAINER_DUMMY_115 = 1475,
    FLAG_DEFEATED_TRAINER_DUMMY_116 = 1476,
    FLAG_DEFEATED_TRAINER_DUMMY_117 = 1477,
    FLAG_DEFEATED_TRAINER_DUMMY_118 = 1478,
    FLAG_DEFEATED_TRAINER_PSYCHIC_MITCHELL = 1479,
    FLAG_DEFEATED_TRAINER_PSYCHIC_ABIGAIL = 1480,
    FLAG_DEFEATED_TRAINER_PI_CARLOS = 1481,
    FLAG_DEFEATED_TRAINER_DUMMY_122 = 1482,
    FLAG_DEFEATED_TRAINER_DUMMY_123 = 1483,
    FLAG_DEFEATED_TRAINER_DUMMY_124 = 1484,
    FLAG_DEFEATED_TRAINER_DUMMY_125 = 1485,
    FLAG_DEFEATED_TRAINER_DUMMY_126 = 1486,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_GREGORY = 1487,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_DEREK = 1488,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_NATHANIEL = 1489,
    FLAG_DEFEATED_TRAINER_JOGGER_SCOTT = 1490,
    FLAG_DEFEATED_TRAINER_DUMMY_131 = 1491,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_BLAKE = 1492,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_GARRETT = 1493,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_LAURA = 1494,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_MARIA = 1495,
    FLAG_DEFEATED_TRAINER_SKIER_BRADLEY = 1496,
    FLAG_DEFEATED_TRAINER_SKIER_EDWARD = 1497,
    FLAG_DEFEATED_TRAINER_SKIER_KAITLYN = 1498,
    FLAG_DEFEATED_TRAINER_SKIER_ANDREA = 1499,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_DALTON = 1500,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_OLIVIA = 1501,
    FLAG_DEFEATED_TRAINER_SKIER_SHAWN = 1502,
    FLAG_DEFEATED_TRAINER_SKIER_BJORN = 1503,
    FLAG_DEFEATED_TRAINER_SKIER_LEXIE = 1504,
    FLAG_DEFEATED_TRAINER_SKIER_MADISON = 1505,
    FLAG_DEFEATED_TRAINER_NINJA_BOY_MATTHEW = 1506,
    FLAG_DEFEATED_TRAINER_NINJA_BOY_ETHAN = 1507,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_LUKE = 1508,
    FLAG_DEFEATED_TRAINER_DUMMY_149 = 1509,
    FLAG_DEFEATED_TRAINER_DUMMY_150 = 1510,
    FLAG_DEFEATED_TRAINER_DUMMY_151 = 1511,
    FLAG_DEFEATED_TRAINER_DUMMY_152 = 1512,
    FLAG_DEFEATED_TRAINER_FISHERMAN_MIGUEL = 1513,
    FLAG_DEFEATED_TRAINER_FISHERMAN_LUC = 1514,
    FLAG_DEFEATED_TRAINER_DUMMY_155 = 1515,
    FLAG_DEFEATED_TRAINER_DUMMY_156 = 1516,
    FLAG_DEFEATED_TRAINER_DUMMY_157 = 1517,
    FLAG_DEFEATED_TRAINER_DUMMY_158 = 1518,
    FLAG_DEFEATED_TRAINER_SWIMMER_ADRIAN = 1519,
    FLAG_DEFEATED_TRAINER_SWIMMER_ERIK = 1520,
    FLAG_DEFEATED_TRAINER_SWIMMER_VINCENT = 1521,
    FLAG_DEFEATED_TRAINER_SWIMMER_JESSICA = 1522,
    FLAG_DEFEATED_TRAINER_SWIMMER_ERICA = 1523,
    FLAG_DEFEATED_TRAINER_SWIMMER_KATELYN = 1524,
    FLAG_DEFEATED_TRAINER_DUMMY_165 = 1525,
    FLAG_DEFEATED_TRAINER_SWIMMER_DILLON = 1526,
    FLAG_DEFEATED_TRAINER_SWIMMER_VANESSA = 1527,
    FLAG_DEFEATED_TRAINER_FISHERMAN_CORY = 1528,
    FLAG_DEFEATED_TRAINER_DUMMY_169 = 1529,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_JAKE = 1530,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_SHANNON = 1531,
    FLAG_DEFEATED_TRAINER_FISHERMAN_BRETT = 1532,
    FLAG_DEFEATED_TRAINER_FISHERMAN_ALEC = 1533,
    FLAG_DEFEATED_TRAINER_FISHERMAN_GEORGE = 1534,
    FLAG_DEFEATED_TRAINER_FISHERMAN_COLE = 1535,
    FLAG_DEFEATED_TRAINER_DUMMY_176 = 1536,
    FLAG_DEFEATED_TRAINER_DUMMY_177 = 1537,
    FLAG_DEFEATED_TRAINER_DUMMY_178 = 1538,
    FLAG_DEFEATED_TRAINER_DUMMY_179 = 1539,
    FLAG_DEFEATED_TRAINER_SAILOR_LUTHER = 1540,
    FLAG_DEFEATED_TRAINER_DUMMY_181 = 1541,
    FLAG_DEFEATED_TRAINER_DUMMY_182 = 1542,
    FLAG_DEFEATED_TRAINER_SWIMMER_WESLEY = 1543,
    FLAG_DEFEATED_TRAINER_SWIMMER_RICARDO = 1544,
    FLAG_DEFEATED_TRAINER_SWIMMER_FRANCISCO = 1545,
    FLAG_DEFEATED_TRAINER_SWIMMER_COLTON = 1546,
    FLAG_DEFEATED_TRAINER_SWIMMER_TROY = 1547,
    FLAG_DEFEATED_TRAINER_SWIMMER_OSCAR = 1548,
    FLAG_DEFEATED_TRAINER_SWIMMER_MIRANDA = 1549,
    FLAG_DEFEATED_TRAINER_SWIMMER_AUBREE = 1550,
    FLAG_DEFEATED_TRAINER_SWIMMER_PAIGE = 1551,
    FLAG_DEFEATED_TRAINER_SWIMMER_CRYSTAL = 1552,
    FLAG_DEFEATED_TRAINER_SWIMMER_CASSANDRA = 1553,
    FLAG_DEFEATED_TRAINER_SWIMMER_GABRIELLE = 1554,
    FLAG_DEFEATED_TRAINER_WORKER_COLIN = 1555,
    FLAG_DEFEATED_TRAINER_WORKER_MASON = 1556,
    FLAG_DEFEATED_TRAINER_DUMMY_197 = 1557,
    FLAG_DEFEATED_TRAINER_DUMMY_198 = 1558,
    FLAG_DEFEATED_TRAINER_DUMMY_199 = 1559,
    FLAG_DEFEATED_TRAINER_DUMMY_200 = 1560,
    FLAG_DEFEATED_TRAINER_BUG_CATCHER_JACK = 1561,
    FLAG_DEFEATED_TRAINER_BUG_CATCHER_PHILLIP = 1562,
    FLAG_DEFEATED_TRAINER_BUG_CATCHER_DONALD = 1563,
    FLAG_DEFEATED_TRAINER_LASS_BRIANA = 1564,
    FLAG_DEFEATED_TRAINER_PSYCHIC_ELIJAH = 1565,
    FLAG_DEFEATED_TRAINER_PSYCHIC_LINDSEY = 1566,
    FLAG_DEFEATED_TRAINER_DUMMY_207 = 1567,
    FLAG_DEFEATED_TRAINER_DUMMY_208 = 1568,
    FLAG_DEFEATED_TRAINER_DUMMY_209 = 1569,
    FLAG_DEFEATED_TRAINER_DUMMY_210 = 1570,
    FLAG_DEFEATED_TRAINER_DUMMY_211 = 1571,
    FLAG_DEFEATED_TRAINER_DUMMY_212 = 1572,
    FLAG_DEFEATED_TRAINER_DUMMY_213 = 1573,
    FLAG_DEFEATED_TRAINER_DUMMY_214 = 1574,
    FLAG_DEFEATED_TRAINER_DUMMY_215 = 1575,
    FLAG_DEFEATED_TRAINER_DUMMY_216 = 1576,
    FLAG_DEFEATED_TRAINER_DUMMY_217 = 1577,
    FLAG_DEFEATED_TRAINER_DUMMY_218 = 1578,
    FLAG_DEFEATED_TRAINER_DUMMY_219 = 1579,
    FLAG_DEFEATED_TRAINER_DUMMY_220 = 1580,
    FLAG_DEFEATED_TRAINER_DUMMY_221 = 1581,
    FLAG_DEFEATED_TRAINER_DUMMY_222 = 1582,
    FLAG_DEFEATED_TRAINER_DUMMY_223 = 1583,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_OMAR = 1584,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_HENRY = 1585,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_MARIAH = 1586,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_SYDNEY = 1587,
    FLAG_DEFEATED_TRAINER_VETERAN_EDGAR = 1588,
    FLAG_DEFEATED_TRAINER_VETERAN_CLAYTON = 1589,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_ONDREJ = 1590,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_CLINTON = 1591,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_DAVID = 1592,
    FLAG_DEFEATED_TRAINER_BIRD_KEEPER_HANA = 1593,
    FLAG_DEFEATED_TRAINER_PSYCHIC_BRYCE = 1594,
    FLAG_DEFEATED_TRAINER_PSYCHIC_VALENCIA = 1595,
    FLAG_DEFEATED_TRAINER_DOUBLE_TEAM_JO_AND_PAT = 1596,
    FLAG_DEFEATED_TRAINER_DOUBLE_TEAM_AL_AND_KAY = 1597,
    FLAG_DEFEATED_TRAINER_DUMMY_238 = 1598,
    FLAG_DEFEATED_TRAINER_DUMMY_239 = 1599,
    FLAG_DEFEATED_TRAINER_VETERAN_GRANT = 1600,
    FLAG_DEFEATED_TRAINER_DUMMY_241 = 1601,
    FLAG_DEFEATED_TRAINER_DUMMY_242 = 1602,
    FLAG_DEFEATED_TRAINER_DUMMY_243 = 1603,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_JONATHON = 1604,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_DARIUS = 1605,
    FLAG_DEFEATED_TRAINER_LEADER_ROARK = 1606,
    FLAG_DEFEATED_TRAINER_RIVAL_ROUTE_203_PIPLUP = 1607,
    FLAG_DEFEATED_TRAINER_RIVAL_ROUTE_203_TURTWIG = 1608,
    FLAG_DEFEATED_TRAINER_RIVAL_ROUTE_203_CHIMCHAR = 1609,
    FLAG_DEFEATED_TRAINER_LEADER_BYRON = 1610,
    FLAG_DEFEATED_TRAINER_DUMMY_251 = 1611,
    FLAG_DEFEATED_TRAINER_DUMMY_252 = 1612,
    FLAG_DEFEATED_TRAINER_DUMMY_253 = 1613,
    FLAG_DEFEATED_TRAINER_DUMMY_254 = 1614,
    FLAG_DEFEATED_TRAINER_DUMMY_255 = 1615,
    FLAG_DEFEATED_TRAINER_WORKER_JACKSON = 1616,
    FLAG_DEFEATED_TRAINER_WORKER_GARY = 1617,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_PHILIP = 1618,
    FLAG_DEFEATED_TRAINER_AROMA_LADY_JENNA = 1619,
    FLAG_DEFEATED_TRAINER_AROMA_LADY_ANGELA = 1620,
    FLAG_DEFEATED_TRAINER_ELITE_FOUR_AARON = 1621,
    FLAG_DEFEATED_TRAINER_ELITE_FOUR_BERTHA = 1622,
    FLAG_DEFEATED_TRAINER_ELITE_FOUR_FLINT = 1623,
    FLAG_DEFEATED_TRAINER_ELITE_FOUR_LUCIAN = 1624,
    FLAG_DEFEATED_TRAINER_CAMPER_CURTIS = 1625,
    FLAG_DEFEATED_TRAINER_CAMPER_DREW = 1626,
    FLAG_DEFEATED_TRAINER_CHAMPION_CYNTHIA = 1627,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_SERGIO = 1628,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_ISAIAH = 1629,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_SAVANNAH = 1630,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_ALICIA = 1631,
    FLAG_DEFEATED_TRAINER_COLLECTOR_DOUGLAS = 1632,
    FLAG_DEFEATED_TRAINER_COLLECTOR_BRADY = 1633,
    FLAG_DEFEATED_TRAINER_COLLECTOR_IVAN = 1634,
    FLAG_DEFEATED_TRAINER_COLLECTOR_FERNANDO = 1635,
    FLAG_DEFEATED_TRAINER_COLLECTOR_EDWIN = 1636,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_HAYDEN = 1637,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_DENNIS = 1638,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_CESAR = 1639,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_ALLEN = 1640,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_ZACHERY = 1641,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_RUBEN = 1642,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_BREANNA = 1643,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_CATHERINE = 1644,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_DESTINY = 1645,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_JAMIE = 1646,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_MAYA = 1647,
    FLAG_DEFEATED_TRAINER_PSYCHIC_MAXWELL = 1648,
    FLAG_DEFEATED_TRAINER_PSYCHIC_BRITTNEY = 1649,
    FLAG_DEFEATED_TRAINER_BELLE_AND_PA_AVA_AND_MATT = 1650,
    FLAG_DEFEATED_TRAINER_DUMMY_291 = 1651,
    FLAG_DEFEATED_TRAINER_RANCHER_MARCO = 1652,
    FLAG_DEFEATED_TRAINER_FISHERMAN_ERICK = 1653,
    FLAG_DEFEATED_TRAINER_TWINS_EMMA_AND_LIL = 1654,
    FLAG_DEFEATED_TRAINER_COMMANDER_MARS_VALLEY_WINDWORKS = 1655,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_FLOAROMA_MEADOW_1 = 1656,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_FLOAROMA_MEADOW_2 = 1657,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_VALLEY_WINDWORKS_2 = 1658,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_VALLEY_WINDWORKS_3 = 1659,
    FLAG_DEFEATED_TRAINER_GUITARIST_TONY = 1660,
    FLAG_DEFEATED_TRAINER_GUITARIST_JERRY = 1661,
    FLAG_DEFEATED_TRAINER_GUITARIST_PRESTON = 1662,
    FLAG_DEFEATED_TRAINER_GUITARIST_LONNIE = 1663,
    FLAG_DEFEATED_TRAINER_RUIN_MANIAC_CALVIN = 1664,
    FLAG_DEFEATED_TRAINER_RUIN_MANIAC_LARRY = 1665,
    FLAG_DEFEATED_TRAINER_JOGGER_WYATT = 1666,
    FLAG_DEFEATED_TRAINER_JOGGER_CRAIG = 1667,
    FLAG_DEFEATED_TRAINER_JOGGER_RAUL = 1668,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_COLBY = 1669,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_DARREN = 1670,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_RAFAEL = 1671,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_JEFFERY = 1672,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_CARL = 1673,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_RICKY = 1674,
    FLAG_DEFEATED_TRAINER_LEADER_GARDENIA = 1675,
    FLAG_DEFEATED_TRAINER_LEADER_WAKE = 1676,
    FLAG_DEFEATED_TRAINER_LEADER_MAYLENE = 1677,
    FLAG_DEFEATED_TRAINER_LEADER_FANTINA = 1678,
    FLAG_DEFEATED_TRAINER_LEADER_CANDICE = 1679,
    FLAG_DEFEATED_TRAINER_LEADER_VOLKNER = 1680,
    FLAG_DEFEATED_TRAINER_DUMMY_321 = 1681,
    FLAG_DEFEATED_TRAINER_LASS_MADELINE = 1682,
    FLAG_DEFEATED_TRAINER_LASS_KAITLIN = 1683,
    FLAG_DEFEATED_TRAINER_LASS_CAROLINE = 1684,
    FLAG_DEFEATED_TRAINER_LASS_MOLLY = 1685,
    FLAG_DEFEATED_TRAINER_HIKER_LOUIS = 1686,
    FLAG_DEFEATED_TRAINER_PARASOL_LADY_ALEXA = 1687,
    FLAG_DEFEATED_TRAINER_PARASOL_LADY_SABRINA = 1688,
    FLAG_DEFEATED_TRAINER_PICNICKER_DIANA = 1689,
    FLAG_DEFEATED_TRAINER_PICNICKER_CHEYENNE = 1690,
    FLAG_DEFEATED_TRAINER_POKE_KID_MEGHAN = 1691,
    FLAG_DEFEATED_TRAINER_POLICEMAN_DANNY = 1692,
    FLAG_DEFEATED_TRAINER_POLICEMAN_THOMAS = 1693,
    FLAG_DEFEATED_TRAINER_DUMMY_334 = 1694,
    FLAG_DEFEATED_TRAINER_RICH_BOY_TREY = 1695,
    FLAG_DEFEATED_TRAINER_DUMMY_336 = 1696,
    FLAG_DEFEATED_TRAINER_SAILOR_MARC = 1697,
    FLAG_DEFEATED_TRAINER_SAILOR_SKYLER = 1698,
    FLAG_DEFEATED_TRAINER_SAILOR_DAMIAN = 1699,
    FLAG_DEFEATED_TRAINER_SCHOOL_KID_CHANCE = 1700,
    FLAG_DEFEATED_TRAINER_SCHOOL_KID_FORREST = 1701,
    FLAG_DEFEATED_TRAINER_SCHOOL_KID_HARRISON = 1702,
    FLAG_DEFEATED_TRAINER_SCHOOL_KID_MACKENZIE = 1703,
    FLAG_DEFEATED_TRAINER_SCHOOL_KID_TIERA = 1704,
    FLAG_DEFEATED_TRAINER_SCHOOL_KID_CHRISTINE = 1705,
    FLAG_DEFEATED_TRAINER_BEAUTY_CYNDY = 1706,
    FLAG_DEFEATED_TRAINER_DUMMY_347 = 1707,
    FLAG_DEFEATED_TRAINER_BEAUTY_LINDSAY = 1708,
    FLAG_DEFEATED_TRAINER_DUMMY_349 = 1709,
    FLAG_DEFEATED_TRAINER_DUMMY_350 = 1710,
    FLAG_DEFEATED_TRAINER_DUMMY_351 = 1711,
    FLAG_DEFEATED_TRAINER_DUMMY_352 = 1712,
    FLAG_DEFEATED_TRAINER_DUMMY_353 = 1713,
    FLAG_DEFEATED_TRAINER_DUMMY_354 = 1714,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_DALLAS = 1715,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_SEBASTIAN = 1716,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_DONNY = 1717,
    FLAG_DEFEATED_TRAINER_TUBER_TRENTON = 1718,
    FLAG_DEFEATED_TRAINER_TUBER_CONNER = 1719,
    FLAG_DEFEATED_TRAINER_TUBER_MARIEL = 1720,
    FLAG_DEFEATED_TRAINER_TUBER_HOLLY = 1721,
    FLAG_DEFEATED_TRAINER_VETERAN_ARMANDO = 1722,
    FLAG_DEFEATED_TRAINER_DUMMY_363 = 1723,
    FLAG_DEFEATED_TRAINER_DUMMY_364 = 1724,
    FLAG_DEFEATED_TRAINER_DUMMY_365 = 1725,
    FLAG_DEFEATED_TRAINER_WAITRESS_KATI = 1726,
    FLAG_DEFEATED_TRAINER_WORKER_GERARDO = 1727,
    FLAG_DEFEATED_TRAINER_INTERVIEWERS_ROXY_AND_OLI_UNUSED = 1728,
    FLAG_DEFEATED_TRAINER_CAMERAMAN_TEVIN_UNUSED = 1729,
    FLAG_DEFEATED_TRAINER_REPORTER_KINSEY_UNUSED = 1730,
    FLAG_DEFEATED_TRAINER_IDOL_GRACE_UNUSED = 1731,
    FLAG_DEFEATED_TRAINER_BATTLE_GIRL_TYLER = 1732,
    FLAG_DEFEATED_TRAINER_BIRD_KEEPER_AUTUMN = 1733,
    FLAG_DEFEATED_TRAINER_DUMMY_374 = 1734,
    FLAG_DEFEATED_TRAINER_DUMMY_375 = 1735,
    FLAG_DEFEATED_TRAINER_DUMMY_376 = 1736,
    FLAG_DEFEATED_TRAINER_CAMPER_ZACKARY = 1737,
    FLAG_DEFEATED_TRAINER_CAMPER_LAWRENCE = 1738,
    FLAG_DEFEATED_TRAINER_CAMPER_DIEGO = 1739,
    FLAG_DEFEATED_TRAINER_CAMPER_PARKER = 1740,
    FLAG_DEFEATED_TRAINER_COLLECTOR_DEAN = 1741,
    FLAG_DEFEATED_TRAINER_COLLECTOR_JAMAL = 1742,
    FLAG_DEFEATED_TRAINER_COLLECTOR_TERRY = 1743,
    FLAG_DEFEATED_TRAINER_DUMMY_384 = 1744,
    FLAG_DEFEATED_TRAINER_DUMMY_385 = 1745,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_JOE = 1746,
    FLAG_DEFEATED_TRAINER_DUMMY_387 = 1747,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_JONAH = 1748,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_MICAH = 1749,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_ARTHUR = 1750,
    FLAG_DEFEATED_TRAINER_DUMMY_391 = 1751,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_BRENDA = 1752,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_BRANDI = 1753,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_CLARICE = 1754,
    FLAG_DEFEATED_TRAINER_PSYCHIC_KODY = 1755,
    FLAG_DEFEATED_TRAINER_PSYCHIC_LANDON = 1756,
    FLAG_DEFEATED_TRAINER_PSYCHIC_DEANDRE = 1757,
    FLAG_DEFEATED_TRAINER_PSYCHIC_RACHAEL = 1758,
    FLAG_DEFEATED_TRAINER_PSYCHIC_DESIREE = 1759,
    FLAG_DEFEATED_TRAINER_PSYCHIC_KENDRA = 1760,
    FLAG_DEFEATED_TRAINER_FISHERMAN_WALTER = 1761,
    FLAG_DEFEATED_TRAINER_DUMMY_402 = 1762,
    FLAG_DEFEATED_TRAINER_GALACTIC_BOSS_CYRUS_GALACTIC_HQ = 1763,
    FLAG_DEFEATED_TRAINER_GALACTIC_BOSS_CYRUS_DISTORTION_WORLD = 1764,
    FLAG_DEFEATED_TRAINER_COMMANDER_MARS_LAKE_VERITY = 1765,
    FLAG_DEFEATED_TRAINER_COMMANDER_JUPITER_TEAM_GALACTIC_ETERNA_BUILDING = 1766,
    FLAG_DEFEATED_TRAINER_COMMANDER_JUPITER_SPEAR_PILLAR = 1767,
    FLAG_DEFEATED_TRAINER_COMMANDER_SATURN_VALOR_CAVERN = 1768,
    FLAG_DEFEATED_TRAINER_COMMANDER_SATURN_GALACTIC_HQ = 1769,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_TEAM_GALACTIC_ETERNA_BUILDING_1F_1 = 1770,
    FLAG_DEFEATED_TRAINER_DUMMY_411 = 1771,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_TEAM_GALACTIC_ETERNA_BUILDING_2F_1 = 1772,
    FLAG_DEFEATED_TRAINER_DUMMY_413 = 1773,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_JUBILIFE_CITY_1 = 1774,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_JUBILIFE_CITY_2 = 1775,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_CELESTIC_TOWN = 1776,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_LAKE_VALOR_1 = 1777,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_LAKE_VALOR_2 = 1778,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_LAKE_VERITY_1 = 1779,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_LAKE_VERITY_3 = 1780,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_TEAM_GALACTIC_ETERNA_BUILDING_1F_2 = 1781,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_TEAM_GALACTIC_ETERNA_BUILDING_2F_2 = 1782,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_TEAM_GALACTIC_ETERNA_BUILDING_3F = 1783,
    FLAG_DEFEATED_TRAINER_DUMMY_424 = 1784,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_LAKE_VALOR_3 = 1785,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_LAKE_VERITY_2 = 1786,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_LAKE_VERITY_4 = 1787,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_GALACTIC_HQ_1F = 1788,
    FLAG_DEFEATED_TRAINER_DUMMY_429 = 1789,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_GALACTIC_HQ_2F_3 = 1790,
    FLAG_DEFEATED_TRAINER_GUITARIST_ARTURO_UNUSED = 1791,
    FLAG_DEFEATED_TRAINER_DUMMY_432 = 1792,
    FLAG_DEFEATED_TRAINER_DUMMY_433 = 1793,
    FLAG_DEFEATED_TRAINER_DUMMY_434 = 1794,
    FLAG_DEFEATED_TRAINER_DUMMY_435 = 1795,
    FLAG_DEFEATED_TRAINER_DUMMY_436 = 1796,
    FLAG_DEFEATED_TRAINER_DUMMY_437 = 1797,
    FLAG_DEFEATED_TRAINER_DUMMY_438 = 1798,
    FLAG_DEFEATED_TRAINER_DUMMY_439 = 1799,
    FLAG_DEFEATED_TRAINER_DUMMY_440 = 1800,
    FLAG_DEFEATED_TRAINER_DUMMY_441 = 1801,
    FLAG_DEFEATED_TRAINER_RUIN_MANIAC_HARRY = 1802,
    FLAG_DEFEATED_TRAINER_RUIN_MANIAC_GERALD = 1803,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_MILES = 1804,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_KENDAL = 1805,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_EDDIE = 1806,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_WILLIE = 1807,
    FLAG_DEFEATED_TRAINER_DUMMY_448 = 1808,
    FLAG_DEFEATED_TRAINER_DUMMY_449 = 1809,
    FLAG_DEFEATED_TRAINER_LASS_CASSIDY = 1810,
    FLAG_DEFEATED_TRAINER_HIKER_THEODORE = 1811,
    FLAG_DEFEATED_TRAINER_HIKER_DAMON = 1812,
    FLAG_DEFEATED_TRAINER_HIKER_MAURICE = 1813,
    FLAG_DEFEATED_TRAINER_HIKER_REGINALD = 1814,
    FLAG_DEFEATED_TRAINER_HIKER_LORENZO = 1815,
    FLAG_DEFEATED_TRAINER_PICNICKER_KARINA = 1816,
    FLAG_DEFEATED_TRAINER_PICNICKER_SUMMER = 1817,
    FLAG_DEFEATED_TRAINER_PICNICKER_TORI = 1818,
    FLAG_DEFEATED_TRAINER_PICNICKER_ANA = 1819,
    FLAG_DEFEATED_TRAINER_CLOWN_LEE_UNUSED = 1820,
    FLAG_DEFEATED_TRAINER_DUMMY_461 = 1821,
    FLAG_DEFEATED_TRAINER_DUMMY_462 = 1822,
    FLAG_DEFEATED_TRAINER_DUMMY_463 = 1823,
    FLAG_DEFEATED_TRAINER_DUMMY_464 = 1824,
    FLAG_DEFEATED_TRAINER_POKE_KID_ARIEL_UNUSED = 1825,
    FLAG_DEFEATED_TRAINER_DUMMY_466 = 1826,
    FLAG_DEFEATED_TRAINER_DUMMY_467 = 1827,
    FLAG_DEFEATED_TRAINER_DUMMY_468 = 1828,
    FLAG_DEFEATED_TRAINER_DUMMY_469 = 1829,
    FLAG_DEFEATED_TRAINER_RIVAL_ROUTE_209_PIPLUP = 1830,
    FLAG_DEFEATED_TRAINER_RIVAL_ROUTE_209_TURTWIG = 1831,
    FLAG_DEFEATED_TRAINER_RIVAL_ROUTE_209_CHIMCHAR = 1832,
    FLAG_DEFEATED_TRAINER_RIVAL_PASTORIA_CITY_PIPLUP = 1833,
    FLAG_DEFEATED_TRAINER_RIVAL_PASTORIA_CITY_TURTWIG = 1834,
    FLAG_DEFEATED_TRAINER_RIVAL_PASTORIA_CITY_CHIMCHAR = 1835,
    FLAG_DEFEATED_TRAINER_RIVAL_CANALAVE_CITY_PIPLUP = 1836,
    FLAG_DEFEATED_TRAINER_RIVAL_CANALAVE_CITY_TURTWIG = 1837,
    FLAG_DEFEATED_TRAINER_RIVAL_CANALAVE_CITY_CHIMCHAR = 1838,
    FLAG_DEFEATED_TRAINER_RIVAL_POKEMON_LEAGUE_PIPLUP = 1839,
    FLAG_DEFEATED_TRAINER_RIVAL_POKEMON_LEAGUE_TURTWIG = 1840,
    FLAG_DEFEATED_TRAINER_RIVAL_POKEMON_LEAGUE_CHIMCHAR = 1841,
    FLAG_DEFEATED_TRAINER_SAILOR_ZACHARIAH = 1842,
    FLAG_DEFEATED_TRAINER_SAILOR_SAMSON = 1843,
    FLAG_DEFEATED_TRAINER_DUMMY_484 = 1844,
    FLAG_DEFEATED_TRAINER_DUMMY_485 = 1845,
    FLAG_DEFEATED_TRAINER_SCIENTIST_SHAUN = 1846,
    FLAG_DEFEATED_TRAINER_DUMMY_487 = 1847,
    FLAG_DEFEATED_TRAINER_NINJA_BOY_FABIAN = 1848,
    FLAG_DEFEATED_TRAINER_NINJA_BOY_BRENNAN = 1849,
    FLAG_DEFEATED_TRAINER_NINJA_BOY_BRUCE = 1850,
    FLAG_DEFEATED_TRAINER_BEAUTY_DEVON = 1851,
    FLAG_DEFEATED_TRAINER_BEAUTY_NICOLA = 1852,
    FLAG_DEFEATED_TRAINER_DUMMY_493 = 1853,
    FLAG_DEFEATED_TRAINER_SWIMMER_CLAIRE = 1854,
    FLAG_DEFEATED_TRAINER_DUMMY_495 = 1855,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_WAYNE = 1856,
    FLAG_DEFEATED_TRAINER_TUBER_JACKY = 1857,
    FLAG_DEFEATED_TRAINER_TUBER_CAITLYN = 1858,
    FLAG_DEFEATED_TRAINER_VETERAN_TERRELL = 1859,
    FLAG_DEFEATED_TRAINER_VETERAN_BRENDEN = 1860,
    FLAG_DEFEATED_TRAINER_DUMMY_501 = 1861,
    FLAG_DEFEATED_TRAINER_DUMMY_502 = 1862,
    FLAG_DEFEATED_TRAINER_WORKER_NOEL = 1863,
    FLAG_DEFEATED_TRAINER_WORKER_BRADEN = 1864,
    FLAG_DEFEATED_TRAINER_WORKER_BRENDON = 1865,
    FLAG_DEFEATED_TRAINER_WORKER_QUENTIN = 1866,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_GALACTIC_HQ_B2F_1 = 1867,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_GALACTIC_HQ_B2F_2 = 1868,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_GALACTIC_HQ_3F_1 = 1869,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_GALACTIC_HQ_2F_1 = 1870,
    FLAG_DEFEATED_TRAINER_DUMMY_511 = 1871,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_GALACTIC_HQ_3F_2 = 1872,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_GALACTIC_HQ_2F_2 = 1873,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_GALACTIC_HQ_3F_3 = 1874,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_MT_CORONET_3F_1 = 1875,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_MT_CORONET_4F_1 = 1876,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_MT_CORONET_4F_2 = 1877,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_MT_CORONET_TUNNEL_ROOM_1 = 1878,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_MT_CORONET_TUNNEL_ROOM_2 = 1879,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_MT_CORONET_5F_1 = 1880,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_SPEAR_PILLAR_1 = 1881,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_GALACTIC_HQ_3F_4 = 1882,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_MT_CORONET_3F_2 = 1883,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_MT_CORONET_TUNNEL_ROOM_3 = 1884,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_MT_CORONET_5F_2 = 1885,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_MT_CORONET_6F = 1886,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_SPEAR_PILLAR_2 = 1887,
    FLAG_DEFEATED_TRAINER_COMMANDER_MARS_SPEAR_PILLAR = 1888,
    FLAG_DEFEATED_TRAINER_ROUGHNECK_KIRBY = 1889,
    FLAG_DEFEATED_TRAINER_POKEFAN_LEONARD = 1890,
    FLAG_DEFEATED_TRAINER_POKEFAN_REBEKAH = 1891,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_OLIVER = 1892,
    FLAG_DEFEATED_TRAINER_BELLE_AND_PA_BETH_AND_BOB = 1893,
    FLAG_DEFEATED_TRAINER_YOUNG_COUPLE_MIKE_AND_NAT = 1894,
    FLAG_DEFEATED_TRAINER_AROMA_LADY_ALISON = 1895,
    FLAG_DEFEATED_TRAINER_ARTIST_ISMAEL = 1896,
    FLAG_DEFEATED_TRAINER_BREEDER_KAYLEE = 1897,
    FLAG_DEFEATED_TRAINER_CAMERAMAN_DARRYL = 1898,
    FLAG_DEFEATED_TRAINER_COLLECTOR_EUGENE = 1899,
    FLAG_DEFEATED_TRAINER_POKEFAN_MEREDITH = 1900,
    FLAG_DEFEATED_TRAINER_PI_KENDRICK = 1901,
    FLAG_DEFEATED_TRAINER_GENTLEMAN_LEONARDO = 1902,
    FLAG_DEFEATED_TRAINER_SOCIALITE_REBECCA = 1903,
    FLAG_DEFEATED_TRAINER_LASS_BLYTHE = 1904,
    FLAG_DEFEATED_TRAINER_RICH_BOY_ROMAN = 1905,
    FLAG_DEFEATED_TRAINER_LADY_KYLIE = 1906,
    FLAG_DEFEATED_TRAINER_REPORTER_VALERIE = 1907,
    FLAG_DEFEATED_TRAINER_SCHOOL_KID_ESTEBAN = 1908,
    FLAG_DEFEATED_TRAINER_SCIENTIST_EMILIO = 1909,
    FLAG_DEFEATED_TRAINER_BEAUTY_GABRIELLA = 1910,
    FLAG_DEFEATED_TRAINER_BEAUTY_HARLEY = 1911,
    FLAG_DEFEATED_TRAINER_VETERAN_EMANUEL = 1912,
    FLAG_DEFEATED_TRAINER_RUIN_MANIAC_KARL = 1913,
    FLAG_DEFEATED_TRAINER_BIRD_KEEPER_AUDREY = 1914,
    FLAG_DEFEATED_TRAINER_BIRD_KEEPER_GENEVA = 1915,
    FLAG_DEFEATED_TRAINER_BIRD_KEEPER_KRYSTAL = 1916,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_GEOFFREY = 1917,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_DARIEN = 1918,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_KEEGAN = 1919,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_STANLEY = 1920,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_DRAKE = 1921,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_KENNY = 1922,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_RODOLFO = 1923,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_SAUL = 1924,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_JOSE = 1925,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_FELIX = 1926,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_QUINN = 1927,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_GRAHAM = 1928,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_KEENAN = 1929,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_STEFAN = 1930,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_SKYLAR = 1931,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_ABEL = 1932,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_DEANNA = 1933,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_MOIRA = 1934,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_DANA = 1935,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_MIKAYLA = 1936,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_MEAGAN = 1937,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_SANDRA = 1938,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_KASSANDRA = 1939,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_JASMIN = 1940,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_NATASHA = 1941,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_MONIQUE = 1942,
    FLAG_DEFEATED_TRAINER_PSYCHIC_CORBIN = 1943,
    FLAG_DEFEATED_TRAINER_PSYCHIC_STERLING = 1944,
    FLAG_DEFEATED_TRAINER_PSYCHIC_DAISY = 1945,
    FLAG_DEFEATED_TRAINER_PSYCHIC_CHELSEY = 1946,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_DAVON = 1947,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_GRIFFIN = 1948,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_RAY = 1949,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_JARRETT = 1950,
    FLAG_DEFEATED_TRAINER_RANGER_KYLER = 1951,
    FLAG_DEFEATED_TRAINER_RANGER_DESHAWN = 1952,
    FLAG_DEFEATED_TRAINER_RANGER_DWAYNE = 1953,
    FLAG_DEFEATED_TRAINER_RANGER_ASHLEE = 1954,
    FLAG_DEFEATED_TRAINER_RANGER_FELICIA = 1955,
    FLAG_DEFEATED_TRAINER_RANGER_KRISTA = 1956,
    FLAG_DEFEATED_TRAINER_SWIMMER_GLENN = 1957,
    FLAG_DEFEATED_TRAINER_SWIMMER_KURT = 1958,
    FLAG_DEFEATED_TRAINER_SWIMMER_SAM = 1959,
    FLAG_DEFEATED_TRAINER_SWIMMER_WADE = 1960,
    FLAG_DEFEATED_TRAINER_SWIMMER_JOANNA = 1961,
    FLAG_DEFEATED_TRAINER_SWIMMER_SOPHIA = 1962,
    FLAG_DEFEATED_TRAINER_SWIMMER_MALLORY = 1963,
    FLAG_DEFEATED_TRAINER_SWIMMER_LYDIA = 1964,
    FLAG_DEFEATED_TRAINER_DUMMY_605 = 1965,
    FLAG_DEFEATED_TRAINER_VETERAN_HARLAN = 1966,
    FLAG_DEFEATED_TRAINER_RIVAL_SPEAR_PILLAR_PIPLUP = 1967,
    FLAG_DEFEATED_TRAINER_CHERYL_ETERNA_FOREST = 1968,
    FLAG_DEFEATED_TRAINER_RILEY_IRON_ISLAND = 1969,
    FLAG_DEFEATED_TRAINER_MARLEY_VICTORY_ROAD = 1970,
    FLAG_DEFEATED_TRAINER_BUCK_STARK_MOUNTAIN = 1971,
    FLAG_DEFEATED_TRAINER_MIRA_WAYWARD_CAVE = 1972,
    FLAG_DEFEATED_TRAINER_LUCAS_JUBILIFE_CITY_CHIMCHAR = 1973,
    FLAG_DEFEATED_TRAINER_LUCAS_JUBILIFE_CITY_PIPLUP = 1974,
    FLAG_DEFEATED_TRAINER_LUCAS_JUBILIFE_CITY_TURTWIG = 1975,
    FLAG_DEFEATED_TRAINER_DAWN_JUBILIFE_CITY_CHIMCHAR = 1976,
    FLAG_DEFEATED_TRAINER_DAWN_JUBILIFE_CITY_PIPLUP = 1977,
    FLAG_DEFEATED_TRAINER_DAWN_JUBILIFE_CITY_TURTWIG = 1978,
    FLAG_DEFEATED_TRAINER_RIVAL_SPEAR_PILLAR_TURTWIG = 1979,
    FLAG_DEFEATED_TRAINER_RIVAL_SPEAR_PILLAR_CHIMCHAR = 1980,
    FLAG_DEFEATED_TRAINER_LUCAS_VEILSTONE_CITY_TURTWIG = 1981,
    FLAG_DEFEATED_TRAINER_LUCAS_VEILSTONE_CITY_CHIMCHAR = 1982,
    FLAG_DEFEATED_TRAINER_LUCAS_VEILSTONE_CITY_PIPLUP = 1983,
    FLAG_DEFEATED_TRAINER_DAWN_VEILSTONE_CITY_TURTWIG = 1984,
    FLAG_DEFEATED_TRAINER_DAWN_VEILSTONE_CITY_CHIMCHAR = 1985,
    FLAG_DEFEATED_TRAINER_DAWN_VEILSTONE_CITY_PIPLUP = 1986,
    FLAG_DEFEATED_TRAINER_AROMA_LADY_ELIZABETH_REMATCH_1 = 1987,
    FLAG_DEFEATED_TRAINER_AROMA_LADY_ELIZABETH_REMATCH_2 = 1988,
    FLAG_DEFEATED_TRAINER_AROMA_LADY_ELIZABETH_REMATCH_3 = 1989,
    FLAG_DEFEATED_TRAINER_ARTIST_WILLIAM_REMATCH_1 = 1990,
    FLAG_DEFEATED_TRAINER_ARTIST_WILLIAM_REMATCH_2 = 1991,
    FLAG_DEFEATED_TRAINER_ARTIST_WILLIAM_REMATCH_3 = 1992,
    FLAG_DEFEATED_TRAINER_BATTLE_GIRL_HELEN_REMATCH_1 = 1993,
    FLAG_DEFEATED_TRAINER_BATTLE_GIRL_HELEN_REMATCH_2 = 1994,
    FLAG_DEFEATED_TRAINER_BATTLE_GIRL_HELEN_REMATCH_3 = 1995,
    FLAG_DEFEATED_TRAINER_BIRD_KEEPER_BRIANNA_REMATCH_1 = 1996,
    FLAG_DEFEATED_TRAINER_BIRD_KEEPER_BRIANNA_REMATCH_2 = 1997,
    FLAG_DEFEATED_TRAINER_BIRD_KEEPER_AUDREY_REMATCH = 1998,
    FLAG_DEFEATED_TRAINER_BREEDER_ALBERT_REMATCH_1 = 1999,
    FLAG_DEFEATED_TRAINER_BREEDER_ALBERT_REMATCH_2 = 2000,
    FLAG_DEFEATED_TRAINER_BREEDER_ALBERT_REMATCH_3 = 2001,
    FLAG_DEFEATED_TRAINER_BREEDER_JENNIFER_REMATCH_1 = 2002,
    FLAG_DEFEATED_TRAINER_BREEDER_JENNIFER_REMATCH_2 = 2003,
    FLAG_DEFEATED_TRAINER_BREEDER_JENNIFER_REMATCH_3 = 2004,
    FLAG_DEFEATED_TRAINER_CAMPER_ZACKARY_REMATCH_1 = 2005,
    FLAG_DEFEATED_TRAINER_CAMPER_ZACKARY_REMATCH_2 = 2006,
    FLAG_DEFEATED_TRAINER_CAMPER_ZACKARY_REMATCH_3 = 2007,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_DALTON_REMATCH = 2008,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_OLIVIA_REMATCH = 2009,
    FLAG_DEFEATED_TRAINER_COLLECTOR_IVAN_REMATCH = 2010,
    FLAG_DEFEATED_TRAINER_DUMMY_058_REMATCH_1 = 2011,
    FLAG_DEFEATED_TRAINER_DUMMY_058_REMATCH_2 = 2012,
    FLAG_DEFEATED_TRAINER_CYCLIST_JOHN_REMATCH_1 = 2013,
    FLAG_DEFEATED_TRAINER_CYCLIST_JOHN_REMATCH_2 = 2014,
    FLAG_DEFEATED_TRAINER_CYCLIST_JOHN_REMATCH_3 = 2015,
    FLAG_DEFEATED_TRAINER_CYCLIST_KAYLA_REMATCH_1 = 2016,
    FLAG_DEFEATED_TRAINER_CYCLIST_KAYLA_REMATCH_2 = 2017,
    FLAG_DEFEATED_TRAINER_CYCLIST_KAYLA_REMATCH_3 = 2018,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_PATRICK_REMATCH_1 = 2019,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_PATRICK_REMATCH_2 = 2020,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_HAYDEN_REMATCH = 2021,
    FLAG_DEFEATED_TRAINER_DRAGON_TAMER_GEOFFREY_REMATCH = 2022,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_JAKE_REMATCH = 2023,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_DENNIS_REMATCH_1 = 2024,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_DENNIS_REMATCH_2 = 2025,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_RODOLFO_REMATCH = 2026,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_SAUL_REMATCH = 2027,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_SHANNON_REMATCH = 2028,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_MAYA_REMATCH_1 = 2029,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_MAYA_REMATCH_2 = 2030,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_DEANNA_REMATCH = 2031,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_MOIRA_REMATCH = 2032,
    FLAG_DEFEATED_TRAINER_PSYCHIC_MITCHELL_REMATCH_1 = 2033,
    FLAG_DEFEATED_TRAINER_PSYCHIC_MITCHELL_REMATCH_2 = 2034,
    FLAG_DEFEATED_TRAINER_PSYCHIC_MAXWELL_REMATCH = 2035,
    FLAG_DEFEATED_TRAINER_PSYCHIC_CORBIN_REMATCH = 2036,
    FLAG_DEFEATED_TRAINER_PSYCHIC_ABIGAIL_REMATCH_1 = 2037,
    FLAG_DEFEATED_TRAINER_PSYCHIC_ABIGAIL_REMATCH_2 = 2038,
    FLAG_DEFEATED_TRAINER_PSYCHIC_BRITTNEY_REMATCH = 2039,
    FLAG_DEFEATED_TRAINER_PSYCHIC_DAISY_REMATCH = 2040,
    FLAG_DEFEATED_TRAINER_RANCHER_MARCO_REMATCH_1 = 2041,
    FLAG_DEFEATED_TRAINER_RANCHER_MARCO_REMATCH_2 = 2042,
    FLAG_DEFEATED_TRAINER_FISHERMAN_JOSEPH_REMATCH_1 = 2043,
    FLAG_DEFEATED_TRAINER_FISHERMAN_JOSEPH_REMATCH_2 = 2044,
    FLAG_DEFEATED_TRAINER_FISHERMAN_MIGUEL_REMATCH = 2045,
    FLAG_DEFEATED_TRAINER_FISHERMAN_ALEC_REMATCH = 2046,
    FLAG_DEFEATED_TRAINER_TWINS_TERI_AND_TIA_REMATCH_1 = 2047,
    FLAG_DEFEATED_TRAINER_TWINS_TERI_AND_TIA_REMATCH_2 = 2048,
    FLAG_DEFEATED_TRAINER_PI_CARLOS_REMATCH_1 = 2049,
    FLAG_DEFEATED_TRAINER_PI_CARLOS_REMATCH_2 = 2050,
    FLAG_DEFEATED_TRAINER_GENTLEMAN_JEREMY_REMATCH_1 = 2051,
    FLAG_DEFEATED_TRAINER_GENTLEMAN_JEREMY_REMATCH_2 = 2052,
    FLAG_DEFEATED_TRAINER_GENTLEMAN_JEREMY_REMATCH_3 = 2053,
    FLAG_DEFEATED_TRAINER_GUITARIST_TONY_REMATCH = 2054,
    FLAG_DEFEATED_TRAINER_RUIN_MANIAC_HARRY_REMATCH_1 = 2055,
    FLAG_DEFEATED_TRAINER_RUIN_MANIAC_HARRY_REMATCH_2 = 2056,
    FLAG_DEFEATED_TRAINER_JOGGER_CRAIG_REMATCH_1 = 2057,
    FLAG_DEFEATED_TRAINER_JOGGER_CRAIG_REMATCH_2 = 2058,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_PHILIP_REMATCH = 2059,
    FLAG_DEFEATED_TRAINER_BLACK_BELT_DAVON_REMATCH = 2060,
    FLAG_DEFEATED_TRAINER_SOCIALITE_REINA_REMATCH_1 = 2061,
    FLAG_DEFEATED_TRAINER_SOCIALITE_REINA_REMATCH_2 = 2062,
    FLAG_DEFEATED_TRAINER_LASS_SAMANTHA_REMATCH_1 = 2063,
    FLAG_DEFEATED_TRAINER_LASS_SAMANTHA_REMATCH_2 = 2064,
    FLAG_DEFEATED_TRAINER_LASS_SAMANTHA_REMATCH_3 = 2065,
    FLAG_DEFEATED_TRAINER_LASS_MADELINE_REMATCH_1 = 2066,
    FLAG_DEFEATED_TRAINER_LASS_MADELINE_REMATCH_2 = 2067,
    FLAG_DEFEATED_TRAINER_LASS_MADELINE_REMATCH_3 = 2068,
    FLAG_DEFEATED_TRAINER_HIKER_THEODORE_REMATCH_1 = 2069,
    FLAG_DEFEATED_TRAINER_HIKER_THEODORE_REMATCH_2 = 2070,
    FLAG_DEFEATED_TRAINER_HIKER_THEODORE_REMATCH_3 = 2071,
    FLAG_DEFEATED_TRAINER_BUG_CATCHER_BRANDON_REMATCH_1 = 2072,
    FLAG_DEFEATED_TRAINER_BUG_CATCHER_BRANDON_REMATCH_2 = 2073,
    FLAG_DEFEATED_TRAINER_BUG_CATCHER_BRANDON_REMATCH_3 = 2074,
    FLAG_DEFEATED_TRAINER_PARASOL_LADY_ALEXA_REMATCH_1 = 2075,
    FLAG_DEFEATED_TRAINER_PARASOL_LADY_ALEXA_REMATCH_2 = 2076,
    FLAG_DEFEATED_TRAINER_PARASOL_LADY_ALEXA_REMATCH_3 = 2077,
    FLAG_DEFEATED_TRAINER_PICNICKER_KARINA_REMATCH_1 = 2078,
    FLAG_DEFEATED_TRAINER_PICNICKER_KARINA_REMATCH_2 = 2079,
    FLAG_DEFEATED_TRAINER_PICNICKER_KARINA_REMATCH_3 = 2080,
    FLAG_DEFEATED_TRAINER_POKE_KID_DANIELLE_REMATCH_1 = 2081,
    FLAG_DEFEATED_TRAINER_POKE_KID_DANIELLE_REMATCH_2 = 2082,
    FLAG_DEFEATED_TRAINER_POKE_KID_DANIELLE_REMATCH_3 = 2083,
    FLAG_DEFEATED_TRAINER_POLICEMAN_DANNY_REMATCH_1 = 2084,
    FLAG_DEFEATED_TRAINER_POLICEMAN_DANNY_REMATCH_2 = 2085,
    FLAG_DEFEATED_TRAINER_POLICEMAN_DANNY_REMATCH_3 = 2086,
    FLAG_DEFEATED_TRAINER_RICH_BOY_TREY_REMATCH_1 = 2087,
    FLAG_DEFEATED_TRAINER_RICH_BOY_TREY_REMATCH_2 = 2088,
    FLAG_DEFEATED_TRAINER_LADY_MELISSA_REMATCH_1 = 2089,
    FLAG_DEFEATED_TRAINER_LADY_MELISSA_REMATCH_2 = 2090,
    FLAG_DEFEATED_TRAINER_LADY_MELISSA_REMATCH_3 = 2091,
    FLAG_DEFEATED_TRAINER_RANGER_TAYLOR_REMATCH_1 = 2092,
    FLAG_DEFEATED_TRAINER_RANGER_TAYLOR_REMATCH_2 = 2093,
    FLAG_DEFEATED_TRAINER_RANGER_TAYLOR_REMATCH_3 = 2094,
    FLAG_DEFEATED_TRAINER_RANGER_KYLER_REMATCH = 2095,
    FLAG_DEFEATED_TRAINER_RANGER_ALLISON_REMATCH_1 = 2096,
    FLAG_DEFEATED_TRAINER_RANGER_ALLISON_REMATCH_2 = 2097,
    FLAG_DEFEATED_TRAINER_RANGER_ALLISON_REMATCH_3 = 2098,
    FLAG_DEFEATED_TRAINER_RANGER_ASHLEE_REMATCH = 2099,
    FLAG_DEFEATED_TRAINER_SAILOR_MARC_REMATCH_1 = 2100,
    FLAG_DEFEATED_TRAINER_SAILOR_MARC_REMATCH_2 = 2101,
    FLAG_DEFEATED_TRAINER_SAILOR_ZACHARIAH_REMATCH = 2102,
    FLAG_DEFEATED_TRAINER_SCIENTIST_SHAUN_REMATCH_1 = 2103,
    FLAG_DEFEATED_TRAINER_SCIENTIST_SHAUN_REMATCH_2 = 2104,
    FLAG_DEFEATED_TRAINER_SCIENTIST_SHAUN_REMATCH_3 = 2105,
    FLAG_DEFEATED_TRAINER_NINJA_BOY_ZACH_REMATCH_1 = 2106,
    FLAG_DEFEATED_TRAINER_NINJA_BOY_ZACH_REMATCH_2 = 2107,
    FLAG_DEFEATED_TRAINER_NINJA_BOY_ZACH_REMATCH_3 = 2108,
    FLAG_DEFEATED_TRAINER_BEAUTY_CYNDY_REMATCH_1 = 2109,
    FLAG_DEFEATED_TRAINER_BEAUTY_CYNDY_REMATCH_2 = 2110,
    FLAG_DEFEATED_TRAINER_SKIER_EDWARD_REMATCH_1 = 2111,
    FLAG_DEFEATED_TRAINER_SKIER_EDWARD_REMATCH_2 = 2112,
    FLAG_DEFEATED_TRAINER_SKIER_BJORN_REMATCH_1 = 2113,
    FLAG_DEFEATED_TRAINER_SKIER_BJORN_REMATCH_2 = 2114,
    FLAG_DEFEATED_TRAINER_SKIER_ANDREA_REMATCH_1 = 2115,
    FLAG_DEFEATED_TRAINER_SKIER_ANDREA_REMATCH_2 = 2116,
    FLAG_DEFEATED_TRAINER_SKIER_MADISON_REMATCH_1 = 2117,
    FLAG_DEFEATED_TRAINER_SKIER_MADISON_REMATCH_2 = 2118,
    FLAG_DEFEATED_TRAINER_SWIMMER_WESLEY_REMATCH = 2119,
    FLAG_DEFEATED_TRAINER_SWIMMER_FRANCISCO_REMATCH = 2120,
    FLAG_DEFEATED_TRAINER_SWIMMER_GLENN_REMATCH = 2121,
    FLAG_DEFEATED_TRAINER_SWIMMER_MIRANDA_REMATCH = 2122,
    FLAG_DEFEATED_TRAINER_SWIMMER_CRYSTAL_REMATCH = 2123,
    FLAG_DEFEATED_TRAINER_SWIMMER_JOANNA_REMATCH = 2124,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_TRISTAN_REMATCH_1 = 2125,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_TRISTAN_REMATCH_2 = 2126,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_TRISTAN_REMATCH_3 = 2127,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_DALLAS_REMATCH_1 = 2128,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_DALLAS_REMATCH_2 = 2129,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_DALLAS_REMATCH_3 = 2130,
    FLAG_DEFEATED_TRAINER_TUBER_JARED_REMATCH_1 = 2131,
    FLAG_DEFEATED_TRAINER_TUBER_JARED_REMATCH_2 = 2132,
    FLAG_DEFEATED_TRAINER_TUBER_CHELSEA_REMATCH_1 = 2133,
    FLAG_DEFEATED_TRAINER_VETERAN_BRIAN_REMATCH_1 = 2134,
    FLAG_DEFEATED_TRAINER_TUBER_CHELSEA_REMATCH_2 = 2135,
    FLAG_DEFEATED_TRAINER_VETERAN_BRIAN_REMATCH_2 = 2136,
    FLAG_DEFEATED_TRAINER_VETERAN_ARMANDO_REMATCH = 2137,
    FLAG_DEFEATED_TRAINER_DUMMY_605_REMATCH = 2138,
    FLAG_DEFEATED_TRAINER_DUMMY_779 = 2139,
    FLAG_DEFEATED_TRAINER_DUMMY_780 = 2140,
    FLAG_DEFEATED_TRAINER_DUMMY_781 = 2141,
    FLAG_DEFEATED_TRAINER_DUMMY_782 = 2142,
    FLAG_DEFEATED_TRAINER_DUMMY_783 = 2143,
    FLAG_DEFEATED_TRAINER_DUMMY_784 = 2144,
    FLAG_DEFEATED_TRAINER_DUMMY_785 = 2145,
    FLAG_DEFEATED_TRAINER_DUMMY_786 = 2146,
    FLAG_DEFEATED_TRAINER_DUMMY_787 = 2147,
    FLAG_DEFEATED_TRAINER_DUMMY_788 = 2148,
    FLAG_DEFEATED_TRAINER_DUMMY_789 = 2149,
    FLAG_DEFEATED_TRAINER_DUMMY_790 = 2150,
    FLAG_DEFEATED_TRAINER_DUMMY_791 = 2151,
    FLAG_DEFEATED_TRAINER_DUMMY_792 = 2152,
    FLAG_DEFEATED_TRAINER_DUMMY_793 = 2153,
    FLAG_DEFEATED_TRAINER_DUMMY_794 = 2154,
    FLAG_DEFEATED_TRAINER_DUMMY_795 = 2155,
    FLAG_DEFEATED_TRAINER_DUMMY_796 = 2156,
    FLAG_DEFEATED_TRAINER_DUMMY_797 = 2157,
    FLAG_DEFEATED_TRAINER_DUMMY_798 = 2158,
    FLAG_DEFEATED_TRAINER_DUMMY_799 = 2159,
    FLAG_DEFEATED_TRAINER_DUMMY_800 = 2160,
    FLAG_DEFEATED_TRAINER_DUMMY_801 = 2161,
    FLAG_DEFEATED_TRAINER_DUMMY_802 = 2162,
    FLAG_DEFEATED_TRAINER_DUMMY_803 = 2163,
    FLAG_DEFEATED_TRAINER_DUMMY_804 = 2164,
    FLAG_DEFEATED_TRAINER_DUMMY_805 = 2165,
    FLAG_DEFEATED_TRAINER_DUMMY_806 = 2166,
    FLAG_DEFEATED_TRAINER_DUMMY_807 = 2167,
    FLAG_DEFEATED_TRAINER_DUMMY_808 = 2168,
    FLAG_DEFEATED_TRAINER_DUMMY_809 = 2169,
    FLAG_DEFEATED_TRAINER_DUMMY_810 = 2170,
    FLAG_DEFEATED_TRAINER_DUMMY_811 = 2171,
    FLAG_DEFEATED_TRAINER_DUMMY_812 = 2172,
    FLAG_DEFEATED_TRAINER_DUMMY_813 = 2173,
    FLAG_DEFEATED_TRAINER_DUMMY_814 = 2174,
    FLAG_DEFEATED_TRAINER_DUMMY_815 = 2175,
    FLAG_DEFEATED_TRAINER_DUMMY_816 = 2176,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_ROLAND_UNUSED_1 = 2177,
    FLAG_DEFEATED_TRAINER_LASS_CARRIE_UNUSED_1 = 2178,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_NORMAN_UNUSED_1 = 2179,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_ROLAND_UNUSED_2 = 2180,
    FLAG_DEFEATED_TRAINER_LASS_CARRIE_UNUSED_2 = 2181,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_NORMAN_UNUSED_2 = 2182,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_ROLAND_UNUSED_3 = 2183,
    FLAG_DEFEATED_TRAINER_LASS_CARRIE_UNUSED_3 = 2184,
    FLAG_DEFEATED_TRAINER_YOUNGSTER_NORMAN_UNUSED_3 = 2185,
    FLAG_DEFEATED_TRAINER_TOWER_TYCOON_PALMER_DUMMY = 2186,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_ANTON = 2187,
    FLAG_DEFEATED_TRAINER_ACE_TRAINER_BRENNA = 2188,
    FLAG_DEFEATED_TRAINER_SCIENTIST_DARRIUS = 2189,
    FLAG_DEFEATED_TRAINER_SCIENTIST_FREDRICK = 2190,
    FLAG_DEFEATED_TRAINER_SCIENTIST_TRAVON = 2191,
    FLAG_DEFEATED_TRAINER_POKE_KID_JANET = 2192,
    FLAG_DEFEATED_TRAINER_DUMMY_833 = 2193,
    FLAG_DEFEATED_TRAINER_DUMMY_834 = 2194,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_IRON_ISLAND_1 = 2195,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_IRON_ISLAND_2 = 2196,
    FLAG_DEFEATED_TRAINER_RIVAL_SURVIVAL_AREA_1_PIPLUP = 2197,
    FLAG_DEFEATED_TRAINER_RIVAL_SURVIVAL_AREA_1_TURTWIG = 2198,
    FLAG_DEFEATED_TRAINER_RIVAL_SURVIVAL_AREA_1_CHIMCHAR = 2199,
    FLAG_DEFEATED_TRAINER_RIVAL_SURVIVAL_AREA_UNUSED_PIPLUP = 2200,
    FLAG_DEFEATED_TRAINER_RIVAL_SURVIVAL_AREA_UNUSED_TURTWIG = 2201,
    FLAG_DEFEATED_TRAINER_RIVAL_SURVIVAL_AREA_UNUSED_CHIMCHAR = 2202,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_VALLEY_WINDWORKS_1 = 2203,
    FLAG_DEFEATED_TRAINER_WORKER_DILLAN = 2204,
    FLAG_DEFEATED_TRAINER_WORKER_HOLDEN = 2205,
    FLAG_DEFEATED_TRAINER_WORKER_CONRAD = 2206,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_VALOR_LAKEFRONT = 2207,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_VEILSTONE_CITY_1 = 2208,
    FLAG_DEFEATED_TRAINER_GALACTIC_GRUNT_VEILSTONE_CITY_2 = 2209,
    FLAG_DEFEATED_TRAINER_RIVAL_ROUTE_201_PIPLUP = 2210,
    FLAG_DEFEATED_TRAINER_RIVAL_ROUTE_201_TURTWIG = 2211,
    FLAG_DEFEATED_TRAINER_RIVAL_ROUTE_201_CHIMCHAR = 2212,
    FLAG_DEFEATED_TRAINER_LEADER_CANDICE_REMATCH = 2213,
    FLAG_DEFEATED_TRAINER_LEADER_MAYLENE_REMATCH = 2214,
    FLAG_DEFEATED_TRAINER_LEADER_VOLKNER_REMATCH = 2215,
    FLAG_DEFEATED_TRAINER_LEADER_BYRON_REMATCH = 2216,
    FLAG_DEFEATED_TRAINER_LEADER_GARDENIA_REMATCH = 2217,
    FLAG_DEFEATED_TRAINER_LEADER_ROARK_REMATCH = 2218,
    FLAG_DEFEATED_TRAINER_LEADER_WAKE_REMATCH = 2219,
    FLAG_DEFEATED_TRAINER_LEADER_FANTINA_REMATCH = 2220,
    FLAG_DEFEATED_TRAINER_RILEY_BATTLEGROUND = 2221,
    FLAG_DEFEATED_TRAINER_BUCK_BATTLEGROUND = 2222,
    FLAG_DEFEATED_TRAINER_MARLEY_BATTLEGROUND = 2223,
    FLAG_DEFEATED_TRAINER_MIRA_BATTLEGROUND = 2224,
    FLAG_DEFEATED_TRAINER_CHERYL_BATTLEGROUND = 2225,
    FLAG_DEFEATED_TRAINER_ELITE_FOUR_AARON_REMATCH = 2226,
    FLAG_DEFEATED_TRAINER_ELITE_FOUR_BERTHA_REMATCH = 2227,
    FLAG_DEFEATED_TRAINER_ELITE_FOUR_FLINT_REMATCH = 2228,
    FLAG_DEFEATED_TRAINER_ELITE_FOUR_LUCIAN_REMATCH = 2229,
    FLAG_DEFEATED_TRAINER_CHAMPION_CYNTHIA_REMATCH = 2230,
    FLAG_DEFEATED_TRAINER_RIVAL_SURVIVAL_AREA_2_PIPLUP = 2231,
    FLAG_DEFEATED_TRAINER_RIVAL_SURVIVAL_AREA_2_TURTWIG = 2232,
    FLAG_DEFEATED_TRAINER_RIVAL_SURVIVAL_AREA_2_CHIMCHAR = 2233,
    FLAG_DEFEATED_TRAINER_CAMERAMAN_TEVIN = 2234,
    FLAG_DEFEATED_TRAINER_CAMERAMAN_TEVIN_REMATCH_1 = 2235,
    FLAG_DEFEATED_TRAINER_CAMERAMAN_TEVIN_REMATCH_2 = 2236,
    FLAG_DEFEATED_TRAINER_GUITARIST_ARTURO = 2237,
    FLAG_DEFEATED_TRAINER_GUITARIST_ARTURO_REMATCH_1 = 2238,
    FLAG_DEFEATED_TRAINER_GUITARIST_ARTURO_REMATCH_2 = 2239,
    FLAG_DEFEATED_TRAINER_IDOL_GRACE = 2240,
    FLAG_DEFEATED_TRAINER_IDOL_GRACE_REMATCH_1 = 2241,
    FLAG_DEFEATED_TRAINER_IDOL_GRACE_REMATCH_2 = 2242,
    FLAG_DEFEATED_TRAINER_INTERVIEWERS_ROXY_AND_OLI = 2243,
    FLAG_DEFEATED_TRAINER_INTERVIEWERS_ROXY_AND_OLI_REMATCH_1 = 2244,
    FLAG_DEFEATED_TRAINER_INTERVIEWERS_ROXY_AND_OLI_REMATCH_2 = 2245,
    FLAG_DEFEATED_TRAINER_CLOWN_LEE = 2246,
    FLAG_DEFEATED_TRAINER_CLOWN_LEE_REMATCH_1 = 2247,
    FLAG_DEFEATED_TRAINER_CLOWN_LEE_REMATCH_2 = 2248,
    FLAG_DEFEATED_TRAINER_POKE_KID_ARIEL = 2249,
    FLAG_DEFEATED_TRAINER_POKE_KID_ARIEL_REMATCH_1 = 2250,
    FLAG_DEFEATED_TRAINER_POKE_KID_ARIEL_REMATCH_2 = 2251,
    FLAG_DEFEATED_TRAINER_RICH_BOY_LIAM = 2252,
    FLAG_DEFEATED_TRAINER_LADY_CELESTE = 2253,
    FLAG_DEFEATED_TRAINER_REPORTER_KINSEY = 2254,
    FLAG_DEFEATED_TRAINER_REPORTER_KINSEY_REMATCH_1 = 2255,
    FLAG_DEFEATED_TRAINER_REPORTER_KINSEY_REMATCH_2 = 2256,
    FLAG_DEFEATED_TRAINER_MAID_BELINDA = 2257,
    FLAG_DEFEATED_TRAINER_MAID_SOPHIE = 2258,
    FLAG_DEFEATED_TRAINER_MAID_EMILY = 2259,
    FLAG_DEFEATED_TRAINER_MAID_ELENA = 2260,
    FLAG_DEFEATED_TRAINER_MAID_CLARE = 2261,
    FLAG_DEFEATED_TRAINER_HALL_MATRON_ARGENTA_DUMMY = 2262,
    FLAG_DEFEATED_TRAINER_FACTORY_HEAD_THORTON_DUMMY = 2263,
    FLAG_DEFEATED_TRAINER_ARCADE_STAR_DAHLIA_DUMMY = 2264,
    FLAG_DEFEATED_TRAINER_CASTLE_VALET_DARACH_DUMMY = 2265,
    FLAG_DEFEATED_TRAINER_CAMERAMAN_TEVIN_REMATCH_3 = 2266,
    FLAG_DEFEATED_TRAINER_GUITARIST_ARTURO_REMATCH_3 = 2267,
    FLAG_DEFEATED_TRAINER_IDOL_GRACE_REMATCH_3 = 2268,
    FLAG_DEFEATED_TRAINER_INTERVIEWERS_ROXY_AND_OLI_REMATCH_3 = 2269,
    FLAG_DEFEATED_TRAINER_CLOWN_LEE_REMATCH_3 = 2270,
    FLAG_DEFEATED_TRAINER_POKE_KID_ARIEL_REMATCH_3 = 2271,
    FLAG_DEFEATED_TRAINER_REPORTER_KINSEY_REMATCH_3 = 2272,
    FLAG_DEFEATED_TRAINER_GALACTIC_BOSS_CYRUS_CELESTIC_TOWN_RUINS = 2273,
    FLAG_DEFEATED_TRAINER_CAMERAMAN_TEVIN_REMATCH_4 = 2274,
    FLAG_DEFEATED_TRAINER_GUITARIST_ARTURO_REMATCH_4 = 2275,
    FLAG_DEFEATED_TRAINER_IDOL_GRACE_REMATCH_4 = 2276,
    FLAG_DEFEATED_TRAINER_INTERVIEWERS_ROXY_AND_OLI_REMATCH_4 = 2277,
    FLAG_DEFEATED_TRAINER_CLOWN_LEE_REMATCH_4 = 2278,
    FLAG_DEFEATED_TRAINER_POKE_KID_ARIEL_REMATCH_4 = 2279,
    FLAG_DEFEATED_TRAINER_REPORTER_KINSEY_REMATCH_4 = 2280,
    FLAG_DEFEATED_TRAINER_LEADER_VOLKNER_FIGHT_AREA = 2281,
    FLAG_DEFEATED_TRAINER_ELITE_FOUR_FLINT_FIGHT_AREA = 2282,
    FLAG_DEFEATED_TRAINER_RIVAL_FIGHT_AREA_PIPLUP = 2283,
    FLAG_DEFEATED_TRAINER_RIVAL_FIGHT_AREA_TURTWIG = 2284,
    FLAG_DEFEATED_TRAINER_RIVAL_FIGHT_AREA_CHIMCHAR = 2285,
    FLAG_DEFEATED_TRAINER_COMMANDER_MARS_STARK_MOUNTAIN = 2286,
    FLAG_DEFEATED_TRAINER_COMMANDER_JUPITER_STARK_MOUNTAIN = 2287,
    TRAINER_DEFEATED_FLAGS_END = 2287,
    FLAG_UNK_0x08F0 = 2288,
    FLAG_UNK_0x08F1 = 2289,
    FLAG_UNK_0x08F2 = 2290,
    FLAG_UNK_0x08F3 = 2291,
    FLAG_UNK_0x08F4 = 2292,
    FLAG_UNK_0x08F5 = 2293,
    FLAG_UNK_0x08F6 = 2294,
    FLAG_UNK_0x08F7 = 2295,
    FLAG_UNK_0x08F8 = 2296,
    FLAG_UNK_0x08F9 = 2297,
    FLAG_UNK_0x08FA = 2298,
    FLAG_UNK_0x08FB = 2299,
    FLAG_UNK_0x08FC = 2300,
    FLAG_UNK_0x08FD = 2301,
    FLAG_UNK_0x08FE = 2302,
    FLAG_UNK_0x08FF = 2303,
    FLAG_UNK_0x0900 = 2304,
    FLAG_UNK_0x0901 = 2305,
    FLAG_UNK_0x0902 = 2306,
    FLAG_UNK_0x0903 = 2307,
    FLAG_UNK_0x0904 = 2308,
    FLAG_UNK_0x0905 = 2309,
    FLAG_UNK_0x0906 = 2310,
    FLAG_UNK_0x0907 = 2311,
    FLAG_UNK_0x0908 = 2312,
    FLAG_UNK_0x0909 = 2313,
    FLAG_UNK_0x090A = 2314,
    FLAG_UNK_0x090B = 2315,
    FLAG_UNK_0x090C = 2316,
    FLAG_UNK_0x090D = 2317,
    FLAG_UNK_0x090E = 2318,
    FLAG_UNK_0x090F = 2319,
    FLAG_UNK_0x0910 = 2320,
    FLAG_UNK_0x0911 = 2321,
    FLAG_UNK_0x0912 = 2322,
    FLAG_UNK_0x0913 = 2323,
    FLAG_UNK_0x0914 = 2324,
    FLAG_UNK_0x0915 = 2325,
    FLAG_UNK_0x0916 = 2326,
    FLAG_UNK_0x0917 = 2327,
    FLAG_UNK_0x0918 = 2328,
    FLAG_UNK_0x0919 = 2329,
    FLAG_UNK_0x091A = 2330,
    FLAG_UNK_0x091B = 2331,
    FLAG_UNK_0x091C = 2332,
    FLAG_UNK_0x091D = 2333,
    FLAG_UNK_0x091E = 2334,
    FLAG_UNK_0x091F = 2335,
    FLAG_UNK_0x0920 = 2336,
    FLAG_UNK_0x0921 = 2337,
    FLAG_UNK_0x0922 = 2338,
    FLAG_UNK_0x0923 = 2339,
    FLAG_UNK_0x0924 = 2340,
    FLAG_UNK_0x0925 = 2341,
    FLAG_UNK_0x0926 = 2342,
    FLAG_UNK_0x0927 = 2343,
    FLAG_UNK_0x0928 = 2344,
    FLAG_UNK_0x0929 = 2345,
    FLAG_UNK_0x092A = 2346,
    FLAG_UNK_0x092B = 2347,
    FLAG_UNK_0x092C = 2348,
    FLAG_UNK_0x092D = 2349,
    FLAG_UNK_0x092E = 2350,
    FLAG_UNK_0x092F = 2351,
    FLAG_UNK_0x0930 = 2352,
    FLAG_UNK_0x0931 = 2353,
    FLAG_UNK_0x0932 = 2354,
    FLAG_UNK_0x0933 = 2355,
    FLAG_UNK_0x0934 = 2356,
    FLAG_UNK_0x0935 = 2357,
    FLAG_UNK_0x0936 = 2358,
    FLAG_UNK_0x0937 = 2359,
    FLAG_UNK_0x0938 = 2360,
    FLAG_UNK_0x0939 = 2361,
    FLAG_UNK_0x093A = 2362,
    FLAG_UNK_0x093B = 2363,
    FLAG_UNK_0x093C = 2364,
    FLAG_UNK_0x093D = 2365,
    FLAG_UNK_0x093E = 2366,
    FLAG_UNK_0x093F = 2367,
    FLAG_UNK_0x0940 = 2368,
    FLAG_UNK_0x0941 = 2369,
    FLAG_UNK_0x0942 = 2370,
    FLAG_UNK_0x0943 = 2371,
    FLAG_UNK_0x0944 = 2372,
    FLAG_UNK_0x0945 = 2373,
    FLAG_UNK_0x0946 = 2374,
    FLAG_UNK_0x0947 = 2375,
    FLAG_UNK_0x0948 = 2376,
    FLAG_UNK_0x0949 = 2377,
    FLAG_UNK_0x094A = 2378,
    FLAG_UNK_0x094B = 2379,
    FLAG_UNK_0x094C = 2380,
    FLAG_UNK_0x094D = 2381,
    FLAG_UNK_0x094E = 2382,
    FLAG_UNK_0x094F = 2383,
    FLAG_UNK_0x0950 = 2384,
    FLAG_UNK_0x0951 = 2385,
    FLAG_UNK_0x0952 = 2386,
    FLAG_UNK_0x0953 = 2387,
    FLAG_UNK_0x0954 = 2388,
    FLAG_UNK_0x0955 = 2389,
    FLAG_UNK_0x0956 = 2390,
    FLAG_UNK_0x0957 = 2391,
    FLAG_UNK_0x0958 = 2392,
    FLAG_UNK_0x0959 = 2393,
    FLAG_UNK_0x095A = 2394,
    FLAG_UNK_0x095B = 2395,
    FLAG_UNK_0x095C = 2396,
    FLAG_UNK_0x095D = 2397,
    FLAG_UNK_0x095E = 2398,
    FLAG_UNK_0x095F = 2399,
    FLAG_BAG_ACQUIRED = 2400,
    FLAG_HAS_PARTNER = 2401,
    FLAG_STRENGTH_ACTIVE = 2402,
    FLAG_JOURNAL_ACQUIRED = 2403,
    FLAG_GAME_COMPLETED = 2404,
    FLAG_STEP = 2405,
    FLAG_COMMUNICATION_CLUB_ACCESSIBLE = 2406,
    FLAG_SAFARI_GAME_ACTIVE = 2407,
    FLAG_CONTEST_MASTER_COOL = 2408,
    FLAG_CONTEST_MASTER_BEAUTY = 2409,
    FLAG_CONTEST_MASTER_CUTE = 2410,
    FLAG_CONTEST_MASTER_SMART = 2411,
    FLAG_CONTEST_MASTER_TOUGH = 2412,
    FLAG_VS_SEEKER_USED = 2413,
    FLAG_ENTERED_UNDERGROUND = 2414,
    FLAG_DIGGING_FOR_FOSSILS = 2415,
    FLAG_SPHERE_ACQUIRED = 2416,
    FLAG_CREATED_SECRET_BASE = 2417,
    FLAG_DECORATED_SECRET_BASE = 2418,
    FLAG_DELIVERED_STOLEN_FLAG = 2419,
    FLAG_UNUSED_0x0974 = 2420,
    FLAG_UNUSED_0x0975 = 2421,
    FLAG_UNUSED_0x0976 = 2422,
    FLAG_DISTORTION_WORLD_STEPPING_STONES = 2423,
    FLAG_CONTEST_HALL_VISITED = 2424,
    FLAG_IN_PAL_PARK = 2425,
    FLAG_FLASH_ACTIVE = 2426,
    FLAG_DEFOG_ACTIVE = 2427,
    FLAG_POKETCH_HIDDEN = 2428,
    FLAG_FREED_GALACTIC_HQ_POKEMON = 2429,
    FLAG_MET_BEBE = 2430,
    FLAG_UNLOCKED_VS_SEEKER_LVL_1 = 2431,
    FLAG_UNLOCKED_VS_SEEKER_LVL_2 = 2432,
    FLAG_UNLOCKED_VS_SEEKER_LVL_3 = 2433,
    FLAG_UNLOCKED_VS_SEEKER_LVL_4 = 2434,
    FLAG_UNLOCKED_VS_SEEKER_LVL_5 = 2435,
    FLAG_ALT_MUSIC_LAKE_VALOR = 2436,
    FLAG_ALT_MUSIC_GALACTIC_HQ = 2437,
    FLAG_ALT_MUSIC_GALACTIC_HQ_1F = 2438,
    FLAG_ALT_MUSIC_GALACTIC_ETERNA_BUILDING = 2439,
    FLAG_ALT_MUSIC_VALLEY_WINDWORKS_BUILDING = 2440,
    FLAG_ALT_MUSIC_FLOAROMA_MEADOW = 2441,
    FLAG_ALT_MUSIC_ROUTE_224 = 2442,
    FLAG_ALT_MUSIC_CHAMPION_ROOM = 2443,
    FLAG_CONNECTED_TO_WIFI = 2444,
    FLAG_DUMMY_0x098D = 2445,
    FLAG_ALT_MUSIC_LAKE_VERITY = 2446,
    FLAG_ALT_MUSIC_LAKE_ACUITY = 2447,
    FLAG_UNUSED_0x0990 = 2448,
    FLAG_UNUSED_0x0991 = 2449,
    FLAG_HAS_SEEN_UNDERGROUND_ROARK_INTRO = 2450,
    FLAG_ALT_MUSIC_ROWANS_LAB = 2451,
    FLAG_ON_CYCLING_ROAD = 2452,
    FLAG_ALT_MUSIC_PAL_PARK = 2453,
    FLAG_FORCE_BIKING_IN_GATE = 2454,
    FLAG_VILLA_FURNITURE_START = 2455,
    FLAG_VILLA_FURNITURE_TABLE = 2455,
    FLAG_VILLA_FURNITURE_BIG_SOFA = 2456,
    FLAG_VILLA_FURNITURE_SMALL_SOFA = 2457,
    FLAG_VILLA_FURNITURE_BED = 2458,
    FLAG_VILLA_FURNITURE_NIGHT_TABLE = 2459,
    FLAG_VILLA_FURNITURE_TV = 2460,
    FLAG_VILLA_FURNITURE_AUDIO_SYSTEM = 2461,
    FLAG_VILLA_FURNITURE_BOOKSHELF = 2462,
    FLAG_VILLA_FURNITURE_RACK = 2463,
    FLAG_VILLA_FURNITURE_HOUSEPLANT = 2464,
    FLAG_VILLA_FURNITURE_PC_DESK = 2465,
    FLAG_VILLA_FURNITURE_MUSIC_BOX = 2466,
    FLAG_VILLA_FURNITURE_POKEMON_BUST = 2467,
    FLAG_VILLA_FURNITURE_POKEMON_BUST_SILVER = 2468,
    FLAG_VILLA_FURNITURE_PIANO = 2469,
    FLAG_VILLA_FURNITURE_GUEST_SET = 2470,
    FLAG_VILLA_FURNITURE_WALL_CLOCK = 2471,
    FLAG_VILLA_FURNITURE_MASTERPIECE = 2472,
    FLAG_VILLA_FURNITURE_TEA_SET = 2473,
    FLAG_VILLA_FURNITURE_CHANDELIER = 2474,
    FLAG_VILLA_VISITOR_INSIDE = 2475,
    FLAG_VILLA_VISITOR_OUTSIDE = 2476,
    FLAG_DISTORTION_WORLD_PUZZLE_FINISHED = 2477,
    FLAG_DISTORTION_WORLD_GIRATINA_SHADOW_1 = 2478,
    FLAG_DISTORTION_WORLD_GIRATINA_SHADOW_2 = 2479,
    SYSTEM_FLAGS_FIRST_ARRIVAL_TO_ZONE = 2480,
    FLAG_FIRST_ARRIVAL_TWINLEAF_TOWN = 2480,
    FLAG_FIRST_ARRIVAL_SANDGEM_TOWN = 2481,
    FLAG_FIRST_ARRIVAL_FLOAROMA_TOWN = 2482,
    FLAG_FIRST_ARRIVAL_SOLACEON_TOWN = 2483,
    FLAG_FIRST_ARRIVAL_CELESTIC_TOWN = 2484,
    FLAG_FIRST_ARRIVAL_SURVIVAL_AREA = 2485,
    FLAG_FIRST_ARRIVAL_RESORT_AREA = 2486,
    FLAG_FIRST_ARRIVAL_JUBILIFE_CITY = 2487,
    FLAG_FIRST_ARRIVAL_CANALAVE_CITY = 2488,
    FLAG_FIRST_ARRIVAL_OREBURGH_CITY = 2489,
    FLAG_FIRST_ARRIVAL_ETERNA_CITY = 2490,
    FLAG_FIRST_ARRIVAL_HEARTHOME_CITY = 2491,
    FLAG_FIRST_ARRIVAL_PASTORIA_CITY = 2492,
    FLAG_FIRST_ARRIVAL_VEILSTONE_CITY = 2493,
    FLAG_FIRST_ARRIVAL_SUNYSHORE_CITY = 2494,
    FLAG_FIRST_ARRIVAL_SNOWPOINT_CITY = 2495,
    FLAG_FIRST_ARRIVAL_OUTSIDE_VICTORY_ROAD = 2496,
    FLAG_FIRST_ARRIVAL_FIGHT_AREA = 2497,
    FLAG_FIRST_ARRIVAL_OREBURGH_MINE = 2498,
    FLAG_FIRST_ARRIVAL_VALLEY_WINDWORKS = 2499,
    FLAG_FIRST_ARRIVAL_ETERNA_FOREST = 2500,
    FLAG_FIRST_ARRIVAL_FUEGO_IRONWORKS = 2501,
    FLAG_FIRST_ARRIVAL_MYSTERY_ZONE_D05 = 2502,
    FLAG_FIRST_ARRIVAL_SPEAR_PILLAR = 2503,
    FLAG_FIRST_ARRIVAL_MYSTERY_ZONE_D06 = 2504,
    FLAG_FIRST_ARRIVAL_SOLACEON_RUINS = 2505,
    FLAG_FIRST_ARRIVAL_VICTORY_ROAD = 2506,
    FLAG_FIRST_ARRIVAL_PAL_PARK = 2507,
    FLAG_FIRST_ARRIVAL_AMITY_SQUARE = 2508,
    FLAG_FIRST_ARRIVAL_RAVAGED_PATH = 2509,
    FLAG_FIRST_ARRIVAL_FLOAROMA_MEADOW = 2510,
    FLAG_FIRST_ARRIVAL_OREBURGH_GATE = 2511,
    FLAG_FIRST_ARRIVAL_FULLMOON_ISLAND = 2512,
    FLAG_FIRST_ARRIVAL_STARK_MOUNTAIN_EXTERIOR = 2513,
    FLAG_FIRST_ARRIVAL_STARK_MOUNTAIN_INTERIOR = 2514,
    FLAG_FIRST_ARRIVAL_SENDOFF_SPRING_UNUSED = 2515,
    FLAG_FIRST_ARRIVAL_TURNBACK_CAVE = 2516,
    FLAG_FIRST_ARRIVAL_FLOWER_PARADISE = 2517,
    FLAG_FIRST_ARRIVAL_SNOWPOINT_TEMPLE = 2518,
    FLAG_FIRST_ARRIVAL_WAYWARD_CAVE = 2519,
    FLAG_FIRST_ARRIVAL_RUIN_MANIAC_CAVE = 2520,
    FLAG_FIRST_ARRIVAL_TROPHY_GARDEN = 2521,
    FLAG_FIRST_ARRIVAL_IRON_ISLAND_EXTERIOR = 2522,
    FLAG_FIRST_ARRIVAL_IRON_ISLAND_INTERIOR = 2523,
    FLAG_FIRST_ARRIVAL_OLD_CHATEAU = 2524,
    FLAG_FIRST_ARRIVAL_GALACTIC_HQ = 2525,
    FLAG_FIRST_ARRIVAL_LAKE_VERITY_UNUSED = 2526,
    FLAG_FIRST_ARRIVAL_VERITY_CAVERN = 2527,
    FLAG_FIRST_ARRIVAL_LAKE_VALOR_UNUSED = 2528,
    FLAG_FIRST_ARRIVAL_VALOR_CAVERN = 2529,
    FLAG_FIRST_ARRIVAL_LAKE_ACUITY_UNUSED = 2530,
    FLAG_FIRST_ARRIVAL_ACUITY_CAVERN = 2531,
    FLAG_FIRST_ARRIVAL_NEWMOON_ISLAND = 2532,
    FLAG_FIRST_ARRIVAL_BATTLE_PARK = 2533,
    FLAG_FIRST_ARRIVAL_CYCLING_ROAD_UNUSED = 2534,
    FLAG_FIRST_ARRIVAL_BERRY_MASTERS_HOUSE = 2535,
    FLAG_FIRST_ARRIVAL_LOST_TOWER = 2536,
    FLAG_FIRST_ARRIVAL_ROUTE_210_COFFEE_SHOP = 2537,
    FLAG_FIRST_ARRIVAL_DRAGON_MASTERS_HOUSE = 2538,
    FLAG_FIRST_ARRIVAL_POKEMON_MANSION = 2539,
    FLAG_FIRST_ARRIVAL_ROUTE_212_MOVE_TUTOR = 2540,
    FLAG_FIRST_ARRIVAL_DOCTOR_FOOTSTEPS_HOUSE = 2541,
    FLAG_FIRST_ARRIVAL_VALOR_LAKEFRONT_RESTAURANT = 2542,
    FLAG_FIRST_ARRIVAL_PIKACHU_FAN_CLUB = 2543,
    FLAG_FIRST_ARRIVAL_POKEMON_SIZE_JUDGE = 2544,
    FLAG_FIRST_ARRIVAL_THE_MEISTERS_HOUSE = 2545,
    FLAG_FIRST_ARRIVAL_ROUTE_221 = 2546,
    FLAG_FIRST_ARRIVAL_POKE_PARK_FRONT_GATE = 2547,
    FLAG_FIRST_ARRIVAL_POKEMON_LEAGUE = 2548,
    FLAG_UNK_0x09F5 = 2549,
    FLAG_UNK_0x09F6 = 2550,
    FLAG_UNK_0x09F7 = 2551,
    FLAG_UNK_0x09F8 = 2552,
    FLAG_UNK_0x09F9 = 2553,
    FLAG_UNK_0x09FA = 2554,
    FLAG_UNK_0x09FB = 2555,
    FLAG_UNK_0x09FC = 2556,
    FLAG_UNK_0x09FD = 2557,
    FLAG_UNK_0x09FE = 2558,
    FLAG_UNK_0x09FF = 2559,
    FLAG_UNK_0x0A00 = 2560,
    FLAG_UNK_0x0A01 = 2561,
    FLAG_UNK_0x0A02 = 2562,
    FLAG_UNK_0x0A03 = 2563,
    FLAG_UNK_0x0A04 = 2564,
    FLAG_UNK_0x0A05 = 2565,
    FLAG_UNK_0x0A06 = 2566,
    FLAG_UNK_0x0A07 = 2567,
    FLAG_UNK_0x0A08 = 2568,
    FLAG_UNK_0x0A09 = 2569,
    FLAG_UNK_0x0A0A = 2570,
    FLAG_UNK_0x0A0B = 2571,
    FLAG_UNK_0x0A0C = 2572,
    FLAG_UNK_0x0A0D = 2573,
    FLAG_UNK_0x0A0E = 2574,
    FLAG_UNK_0x0A0F = 2575,
    FLAG_UNK_0x0A10 = 2576,
    FLAG_UNK_0x0A11 = 2577,
    FLAG_UNK_0x0A12 = 2578,
    FLAG_UNK_0x0A13 = 2579,
    FLAG_UNK_0x0A14 = 2580,
    FLAG_UNK_0x0A15 = 2581,
    FLAG_UNK_0x0A16 = 2582,
    FLAG_UNK_0x0A17 = 2583,
    FLAG_UNK_0x0A18 = 2584,
    FLAG_UNK_0x0A19 = 2585,
    FLAG_UNK_0x0A1A = 2586,
    FLAG_UNK_0x0A1B = 2587,
    FLAG_UNK_0x0A1C = 2588,
    FLAG_UNK_0x0A1D = 2589,
    FLAG_UNK_0x0A1E = 2590,
    FLAG_UNK_0x0A1F = 2591,
    FLAG_UNK_0x0A20 = 2592,
    FLAG_UNK_0x0A21 = 2593,
    FLAG_UNK_0x0A22 = 2594,
    FLAG_UNK_0x0A23 = 2595,
    FLAG_UNK_0x0A24 = 2596,
    FLAG_UNK_0x0A25 = 2597,
    FLAG_UNK_0x0A26 = 2598,
    FLAG_UNK_0x0A27 = 2599,
    FLAG_UNK_0x0A28 = 2600,
    FLAG_UNK_0x0A29 = 2601,
    FLAG_UNK_0x0A2A = 2602,
    FLAG_UNK_0x0A2B = 2603,
    FLAG_UNK_0x0A2C = 2604,
    FLAG_UNK_0x0A2D = 2605,
    FLAG_UNK_0x0A2E = 2606,
    FLAG_UNK_0x0A2F = 2607,
    FLAG_UNK_0x0A30 = 2608,
    FLAG_UNK_0x0A31 = 2609,
    FLAG_UNK_0x0A32 = 2610,
    FLAG_UNK_0x0A33 = 2611,
    FLAG_UNK_0x0A34 = 2612,
    FLAG_UNK_0x0A35 = 2613,
    FLAG_UNK_0x0A36 = 2614,
    FLAG_UNK_0x0A37 = 2615,
    FLAG_UNK_0x0A38 = 2616,
    FLAG_UNK_0x0A39 = 2617,
    FLAG_UNK_0x0A3A = 2618,
    FLAG_UNK_0x0A3B = 2619,
    FLAG_UNK_0x0A3C = 2620,
    FLAG_UNK_0x0A3D = 2621,
    FLAG_UNK_0x0A3E = 2622,
    FLAG_UNK_0x0A3F = 2623,
    FLAG_UNK_0x0A40 = 2624,
    FLAG_UNK_0x0A41 = 2625,
    FLAG_UNK_0x0A42 = 2626,
    FLAG_UNK_0x0A43 = 2627,
    FLAG_UNK_0x0A44 = 2628,
    FLAG_UNK_0x0A45 = 2629,
    FLAG_UNK_0x0A46 = 2630,
    FLAG_UNK_0x0A47 = 2631,
    FLAG_UNK_0x0A48 = 2632,
    FLAG_UNK_0x0A49 = 2633,
    FLAG_UNK_0x0A4A = 2634,
    FLAG_UNK_0x0A4B = 2635,
    FLAG_UNK_0x0A4C = 2636,
    FLAG_UNK_0x0A4D = 2637,
    FLAG_UNK_0x0A4E = 2638,
    FLAG_UNK_0x0A4F = 2639,
    FLAG_UNK_0x0A50 = 2640,
    FLAG_UNK_0x0A51 = 2641,
    FLAG_UNK_0x0A52 = 2642,
    FLAG_UNK_0x0A53 = 2643,
    FLAG_UNK_0x0A54 = 2644,
    FLAG_UNK_0x0A55 = 2645,
    FLAG_UNK_0x0A56 = 2646,
    FLAG_UNK_0x0A57 = 2647,
    FLAG_UNK_0x0A58 = 2648,
    FLAG_UNK_0x0A59 = 2649,
    FLAG_UNK_0x0A5A = 2650,
    FLAG_UNK_0x0A5B = 2651,
    FLAG_UNK_0x0A5C = 2652,
    FLAG_UNK_0x0A5D = 2653,
    FLAG_UNK_0x0A5E = 2654,
    FLAG_UNK_0x0A5F = 2655,
    FLAG_UNK_0x0A60 = 2656,
    FLAG_UNK_0x0A61 = 2657,
    FLAG_UNK_0x0A62 = 2658,
    FLAG_UNK_0x0A63 = 2659,
    FLAG_UNK_0x0A64 = 2660,
    FLAG_UNK_0x0A65 = 2661,
    FLAG_UNK_0x0A66 = 2662,
    FLAG_UNK_0x0A67 = 2663,
    FLAG_UNK_0x0A68 = 2664,
    FLAG_UNK_0x0A69 = 2665,
    FLAG_UNK_0x0A6A = 2666,
    FLAG_UNK_0x0A6B = 2667,
    FLAG_UNK_0x0A6C = 2668,
    FLAG_UNK_0x0A6D = 2669,
    FLAG_UNK_0x0A6E = 2670,
    FLAG_UNK_0x0A6F = 2671,
    FLAG_UNK_0x0A70 = 2672,
    FLAG_UNK_0x0A71 = 2673,
    FLAG_UNK_0x0A72 = 2674,
    FLAG_UNK_0x0A73 = 2675,
    FLAG_UNK_0x0A74 = 2676,
    FLAG_UNK_0x0A75 = 2677,
    FLAG_UNK_0x0A76 = 2678,
    FLAG_UNK_0x0A77 = 2679,
    FLAG_UNK_0x0A78 = 2680,
    FLAG_UNK_0x0A79 = 2681,
    FLAG_UNK_0x0A7A = 2682,
    FLAG_UNK_0x0A7B = 2683,
    FLAG_UNK_0x0A7C = 2684,
    FLAG_UNK_0x0A7D = 2685,
    FLAG_UNK_0x0A7E = 2686,
    FLAG_UNK_0x0A7F = 2687,
    FLAG_UNK_0x0A80 = 2688,
    FLAG_UNK_0x0A81 = 2689,
    FLAG_UNK_0x0A82 = 2690,
    FLAG_UNK_0x0A83 = 2691,
    FLAG_UNK_0x0A84 = 2692,
    FLAG_UNK_0x0A85 = 2693,
    FLAG_UNK_0x0A86 = 2694,
    FLAG_UNK_0x0A87 = 2695,
    FLAG_UNK_0x0A88 = 2696,
    FLAG_UNK_0x0A89 = 2697,
    FLAG_UNK_0x0A8A = 2698,
    FLAG_UNK_0x0A8B = 2699,
    FLAG_UNK_0x0A8C = 2700,
    FLAG_UNK_0x0A8D = 2701,
    FLAG_UNK_0x0A8E = 2702,
    FLAG_UNK_0x0A8F = 2703,
    FLAG_UNK_0x0A90 = 2704,
    FLAG_UNK_0x0A91 = 2705,
    FLAG_UNK_0x0A92 = 2706,
    FLAG_UNK_0x0A93 = 2707,
    FLAG_UNK_0x0A94 = 2708,
    FLAG_UNK_0x0A95 = 2709,
    FLAG_UNK_0x0A96 = 2710,
    FLAG_UNK_0x0A97 = 2711,
    FLAG_UNK_0x0A98 = 2712,
    FLAG_UNK_0x0A99 = 2713,
    FLAG_UNK_0x0A9A = 2714,
    FLAG_UNK_0x0A9B = 2715,
    FLAG_UNK_0x0A9C = 2716,
    FLAG_UNK_0x0A9D = 2717,
    FLAG_UNK_0x0A9E = 2718,
    FLAG_UNK_0x0A9F = 2719,
    DAILY_FLAGS_START = 2720,
    FLAG_DAILY_RECEIVED_ROUTE_208_HOUSE_RANDOM_BERRY = 2720,
    FLAG_DAILY_RECEIVED_CELESTIC_TOWN_POKECENTER_1F_GREAT_BALL = 2721,
    FLAG_DAILY_ADDED_TROPHY_GARDEN_MON = 2722,
    FLAG_DAILY_RECEIVED_PASTORIA_CITY_SOUTHWEST_HOUSE_RANDOM_BERRY = 2723,
    FLAG_DAILY_TOLD_STORY_SUNYSHORE_CITY_NORTHEAST_HOUSE = 2724,
    FLAG_DAILY_GOT_SPA_TREATMENT_RESORT_AREA_RIBBON_SYNDICATE_2F = 2725,
    FLAG_DAILY_0x0AA6 = 2726,
    FLAG_DAILY_VISIT_TOMORROW_SUNYSHORE_CITY_NORTHEAST_HOUSE = 2727,
    FLAG_DAILY_WON_AGAINST_VALLEY_WINDWORKS_OUTSIDE_DRIFLOON = 2728,
    FLAG_DAILY_CHECKED_LUCKY_NUMBER = 2729,
    FLAG_DAILY_RECEIVED_FLOWER_SHOP_BERRY = 2730,
    FLAG_DAILY_SET_RESTAURANT_TRAINERS = 2731,
    FLAG_DAILY_GOT_MASSAGE_VEILSTONE_CITY_SOUTHWEST_HOUSE = 2732,
    FLAG_DAILY_HEARD_TRENDY_SAYING_SNOWPOINT_CITY_EAST_HOUSE = 2733,
    FLAG_DAILY_RECEIVED_ROUTE_221_HOUSE_REWARD = 2734,
    FLAG_DAILY_0x0AAF = 2735,
    FLAG_DAILY_BATTLED_OLD_CHATEAU_ROTOM = 2736,
    FLAG_DAILY_RECEIVED_POKEMON_NEWS_PRESS_REWARD = 2737,
    FLAG_DAILY_DEFEATED_SURVIVAL_AREA_RIVAL = 2738,
    FLAG_DAILY_RECEIVED_LOTTERY_PRIZE = 2739,
    FLAG_DAILY_RECEIVED_GREAT_MARSH_6_RANDOM_SHARD = 2740,
    FLAG_DAILY_RECEIVED_AMITY_SQUARE_MAN_GIFT = 2741,
    FLAG_DAILY_SET_FIVE_MAID_KNOCKOUT_TURN_TARGET = 2742,
    FLAG_DAILY_SET_BATTLEGROUND_TRAINERS = 2743,
    FLAG_DAILY_DEFEATED_BATTLEGROUND_ROARK = 2744,
    FLAG_DAILY_DEFEATED_BATTLEGROUND_GARDENIA = 2745,
    FLAG_DAILY_DEFEATED_BATTLEGROUND_CRASHER_WAKE = 2746,
    FLAG_DAILY_DEFEATED_BATTLEGROUND_MAYLENE = 2747,
    FLAG_DAILY_DEFEATED_IDOL_GRACE = 2748,
    FLAG_DAILY_DEFEATED_REPORTER_KINSEY = 2749,
    FLAG_DAILY_DEFEATED_CAMERAMAN_TEVIN = 2750,
    FLAG_DAILY_DEFEATED_CLOWN_LEE = 2751,
    FLAG_DAILY_DEFEATED_INTERVIEWERS_ROXY_AND_OLI = 2752,
    FLAG_DAILY_DEFEATED_POKE_KID_ARIEL = 2753,
    FLAG_DAILY_DEFEATED_GUITARIST_ARTURO = 2754,
    FLAG_DAILY_RECEIVED_GLOBAL_TERMINAL_1F_BACKDROP = 2755,
    FLAG_DAILY_SET_BATTLE_HALL_MAJOR_NPC = 2756,
    FLAG_DAILY_DEFEATED_BATTLEGROUND_FANTINA = 2757,
    FLAG_DAILY_DEFEATED_BATTLEGROUND_CANDICE = 2758,
    FLAG_DAILY_DEFEATED_BATTLEGROUND_BYRON = 2759,
    FLAG_DAILY_DEFEATED_BATTLEGROUND_VOLKNER = 2760,
    FLAG_DAILY_DEFEATED_BATTLEGROUND_CHERYL = 2761,
    FLAG_DAILY_DEFEATED_BATTLEGROUND_RILEY = 2762,
    FLAG_DAILY_DEFEATED_BATTLEGROUND_MARLEY = 2763,
    FLAG_DAILY_DEFEATED_BATTLEGROUND_BUCK = 2764,
    FLAG_DAILY_DEFEATED_BATTLEGROUND_MIRA = 2765,
    FLAG_DAILY_0x0ACE = 2766,
    FLAG_DAILY_0x0ACF = 2767,
    FLAG_DAILY_0x0AD0 = 2768,
    FLAG_DAILY_0x0AD1 = 2769,
    FLAG_DAILY_0x0AD2 = 2770,
    FLAG_DAILY_0x0AD3 = 2771,
    FLAG_DAILY_0x0AD4 = 2772,
    FLAG_DAILY_0x0AD5 = 2773,
    FLAG_DAILY_0x0AD6 = 2774,
    FLAG_DAILY_0x0AD7 = 2775,
    FLAG_DAILY_0x0AD8 = 2776,
    FLAG_DAILY_0x0AD9 = 2777,
    FLAG_DAILY_0x0ADA = 2778,
    FLAG_DAILY_0x0ADB = 2779,
    FLAG_DAILY_0x0ADC = 2780,
    FLAG_DAILY_0x0ADD = 2781,
    FLAG_DAILY_0x0ADE = 2782,
    FLAG_DAILY_0x0ADF = 2783,
    FLAG_DAILY_0x0AE0 = 2784,
    FLAG_DAILY_0x0AE1 = 2785,
    FLAG_DAILY_0x0AE2 = 2786,
    FLAG_DAILY_0x0AE3 = 2787,
    FLAG_DAILY_0x0AE4 = 2788,
    FLAG_DAILY_0x0AE5 = 2789,
    FLAG_DAILY_0x0AE6 = 2790,
    FLAG_DAILY_0x0AE7 = 2791,
    FLAG_DAILY_0x0AE8 = 2792,
    FLAG_DAILY_0x0AE9 = 2793,
    FLAG_DAILY_0x0AEA = 2794,
    FLAG_DAILY_0x0AEB = 2795,
    FLAG_DAILY_0x0AEC = 2796,
    FLAG_DAILY_0x0AED = 2797,
    FLAG_DAILY_0x0AEE = 2798,
    FLAG_DAILY_0x0AEF = 2799,
    FLAG_DAILY_0x0AF0 = 2800,
    FLAG_DAILY_0x0AF1 = 2801,
    FLAG_DAILY_0x0AF2 = 2802,
    FLAG_DAILY_0x0AF3 = 2803,
    FLAG_DAILY_0x0AF4 = 2804,
    FLAG_DAILY_0x0AF5 = 2805,
    FLAG_DAILY_0x0AF6 = 2806,
    FLAG_DAILY_0x0AF7 = 2807,
    FLAG_DAILY_0x0AF8 = 2808,
    FLAG_DAILY_0x0AF9 = 2809,
    FLAG_DAILY_0x0AFA = 2810,
    FLAG_DAILY_0x0AFB = 2811,
    FLAG_DAILY_0x0AFC = 2812,
    FLAG_DAILY_0x0AFD = 2813,
    FLAG_DAILY_0x0AFE = 2814,
    FLAG_DAILY_0x0AFF = 2815,
    FLAG_DAILY_0x0B00 = 2816,
    FLAG_DAILY_0x0B01 = 2817,
    FLAG_DAILY_0x0B02 = 2818,
    FLAG_DAILY_0x0B03 = 2819,
    FLAG_DAILY_0x0B04 = 2820,
    FLAG_DAILY_0x0B05 = 2821,
    FLAG_DAILY_0x0B06 = 2822,
    FLAG_DAILY_0x0B07 = 2823,
    FLAG_DAILY_0x0B08 = 2824,
    FLAG_DAILY_0x0B09 = 2825,
    FLAG_DAILY_0x0B0A = 2826,
    FLAG_DAILY_0x0B0B = 2827,
    FLAG_DAILY_0x0B0C = 2828,
    FLAG_DAILY_0x0B0D = 2829,
    FLAG_DAILY_0x0B0E = 2830,
    FLAG_DAILY_0x0B0F = 2831,
    FLAG_DAILY_0x0B10 = 2832,
    FLAG_DAILY_0x0B11 = 2833,
    FLAG_DAILY_0x0B12 = 2834,
    FLAG_DAILY_0x0B13 = 2835,
    FLAG_DAILY_0x0B14 = 2836,
    FLAG_DAILY_0x0B15 = 2837,
    FLAG_DAILY_0x0B16 = 2838,
    FLAG_DAILY_0x0B17 = 2839,
    FLAG_DAILY_0x0B18 = 2840,
    FLAG_DAILY_0x0B19 = 2841,
    FLAG_DAILY_0x0B1A = 2842,
    FLAG_DAILY_0x0B1B = 2843,
    FLAG_DAILY_0x0B1C = 2844,
    FLAG_DAILY_0x0B1D = 2845,
    FLAG_DAILY_0x0B1E = 2846,
    FLAG_DAILY_0x0B1F = 2847,
    FLAG_DAILY_0x0B20 = 2848,
    FLAG_DAILY_0x0B21 = 2849,
    FLAG_DAILY_0x0B22 = 2850,
    FLAG_DAILY_0x0B23 = 2851,
    FLAG_DAILY_0x0B24 = 2852,
    FLAG_DAILY_0x0B25 = 2853,
    FLAG_DAILY_0x0B26 = 2854,
    FLAG_DAILY_0x0B27 = 2855,
    FLAG_DAILY_0x0B28 = 2856,
    FLAG_DAILY_0x0B29 = 2857,
    FLAG_DAILY_0x0B2A = 2858,
    FLAG_DAILY_0x0B2B = 2859,
    FLAG_DAILY_0x0B2C = 2860,
    FLAG_DAILY_0x0B2D = 2861,
    FLAG_DAILY_0x0B2E = 2862,
    FLAG_DAILY_0x0B2F = 2863,
    FLAG_DAILY_0x0B30 = 2864,
    FLAG_DAILY_0x0B31 = 2865,
    FLAG_DAILY_0x0B32 = 2866,
    FLAG_DAILY_0x0B33 = 2867,
    FLAG_DAILY_0x0B34 = 2868,
    FLAG_DAILY_0x0B35 = 2869,
    FLAG_DAILY_0x0B36 = 2870,
    FLAG_DAILY_0x0B37 = 2871,
    FLAG_DAILY_0x0B38 = 2872,
    FLAG_DAILY_0x0B39 = 2873,
    FLAG_DAILY_0x0B3A = 2874,
    FLAG_DAILY_0x0B3B = 2875,
    FLAG_DAILY_0x0B3C = 2876,
    FLAG_DAILY_0x0B3D = 2877,
    FLAG_DAILY_0x0B3E = 2878,
    FLAG_DAILY_0x0B3F = 2879,
    FLAG_DAILY_0x0B40 = 2880,
    FLAG_DAILY_0x0B41 = 2881,
    FLAG_DAILY_0x0B42 = 2882,
    FLAG_DAILY_0x0B43 = 2883,
    FLAG_DAILY_0x0B44 = 2884,
    FLAG_DAILY_0x0B45 = 2885,
    FLAG_DAILY_0x0B46 = 2886,
    FLAG_DAILY_0x0B47 = 2887,
    FLAG_DAILY_0x0B48 = 2888,
    FLAG_DAILY_0x0B49 = 2889,
    FLAG_DAILY_0x0B4A = 2890,
    FLAG_DAILY_0x0B4B = 2891,
    FLAG_DAILY_0x0B4C = 2892,
    FLAG_DAILY_0x0B4D = 2893,
    FLAG_DAILY_0x0B4E = 2894,
    FLAG_DAILY_0x0B4F = 2895,
    FLAG_DAILY_0x0B50 = 2896,
    FLAG_DAILY_0x0B51 = 2897,
    FLAG_DAILY_0x0B52 = 2898,
    FLAG_DAILY_0x0B53 = 2899,
    FLAG_DAILY_0x0B54 = 2900,
    FLAG_DAILY_0x0B55 = 2901,
    FLAG_DAILY_0x0B56 = 2902,
    FLAG_DAILY_0x0B57 = 2903,
    FLAG_DAILY_0x0B58 = 2904,
    FLAG_DAILY_0x0B59 = 2905,
    FLAG_DAILY_0x0B5A = 2906,
    FLAG_DAILY_0x0B5B = 2907,
    FLAG_DAILY_0x0B5C = 2908,
    FLAG_DAILY_0x0B5D = 2909,
    FLAG_DAILY_0x0B5E = 2910,
    FLAG_DAILY_0x0B5F = 2911,
    DAILY_FLAGS_END = 2911,
    FLAG_UNK_0x0B60 = 2912,
    FLAG_UNK_0x0B61 = 2913,
    FLAG_UNK_0x0B62 = 2914,
    FLAG_UNK_0x0B63 = 2915,
    FLAG_UNK_0x0B64 = 2916,
    FLAG_UNK_0x0B65 = 2917,
    FLAG_UNK_0x0B66 = 2918,
    FLAG_UNK_0x0B67 = 2919,
    FLAG_UNK_0x0B68 = 2920,
    FLAG_UNK_0x0B69 = 2921,
    FLAG_UNK_0x0B6A = 2922,
    FLAG_UNK_0x0B6B = 2923,
    FLAG_UNK_0x0B6C = 2924,
    FLAG_UNK_0x0B6D = 2925,
    FLAG_UNK_0x0B6E = 2926,
    FLAG_UNK_0x0B6F = 2927,
    FLAG_UNK_0x0B70 = 2928,
    FLAG_UNK_0x0B71 = 2929,
    FLAG_UNK_0x0B72 = 2930,
    FLAG_UNK_0x0B73 = 2931,
    FLAG_UNK_0x0B74 = 2932,
    FLAG_UNK_0x0B75 = 2933,
    FLAG_UNK_0x0B76 = 2934,
    FLAG_UNK_0x0B77 = 2935,
    FLAG_UNK_0x0B78 = 2936,
    FLAG_UNK_0x0B79 = 2937,
    FLAG_UNK_0x0B7A = 2938,
    FLAG_UNK_0x0B7B = 2939,
    FLAG_UNK_0x0B7C = 2940,
    FLAG_UNK_0x0B7D = 2941,
    FLAG_UNK_0x0B7E = 2942,
    FLAG_UNK_0x0B7F = 2943,
    FLAG_UNK_0x0B80 = 2944,
    FLAG_UNK_0x0B81 = 2945,
    FLAG_UNK_0x0B82 = 2946,
    FLAG_UNK_0x0B83 = 2947,
    FLAG_UNK_0x0B84 = 2948,
    FLAG_UNK_0x0B85 = 2949,
    FLAG_UNK_0x0B86 = 2950,
    FLAG_UNK_0x0B87 = 2951,
    FLAG_UNK_0x0B88 = 2952,
    FLAG_UNK_0x0B89 = 2953,
    FLAG_UNK_0x0B8A = 2954,
    FLAG_UNK_0x0B8B = 2955,
    FLAG_UNK_0x0B8C = 2956,
    FLAG_UNK_0x0B8D = 2957,
    FLAG_UNK_0x0B8E = 2958,
    FLAG_UNK_0x0B8F = 2959,
    FLAG_UNK_0x0B90 = 2960,
    FLAG_UNK_0x0B91 = 2961,
    FLAG_UNK_0x0B92 = 2962,
    FLAG_UNK_0x0B93 = 2963,
    FLAG_UNK_0x0B94 = 2964,
    FLAG_UNK_0x0B95 = 2965,
    FLAG_UNK_0x0B96 = 2966,
    FLAG_UNK_0x0B97 = 2967,
    FLAG_UNK_0x0B98 = 2968,
    FLAG_UNK_0x0B99 = 2969,
    FLAG_UNK_0x0B9A = 2970,
    FLAG_UNK_0x0B9B = 2971,
    FLAG_UNK_0x0B9C = 2972,
    FLAG_UNK_0x0B9D = 2973,
    FLAG_UNK_0x0B9E = 2974,
    FLAG_UNK_0x0B9F = 2975,
    FLAG_UNK_0x0BA0 = 2976,
    FLAG_UNK_0x0BA1 = 2977,
    FLAG_UNK_0x0BA2 = 2978,
    FLAG_UNK_0x0BA3 = 2979,
    FLAG_UNK_0x0BA4 = 2980,
    FLAG_UNK_0x0BA5 = 2981,
    FLAG_UNK_0x0BA6 = 2982,
    FLAG_UNK_0x0BA7 = 2983,
    FLAG_UNK_0x0BA8 = 2984,
    FLAG_UNK_0x0BA9 = 2985,
    FLAG_UNK_0x0BAA = 2986,
    FLAG_UNK_0x0BAB = 2987,
    FLAG_UNK_0x0BAC = 2988,
    FLAG_UNK_0x0BAD = 2989,
    FLAG_UNK_0x0BAE = 2990,
    FLAG_UNK_0x0BAF = 2991,
    FLAG_UNK_0x0BB0 = 2992,
    FLAG_UNK_0x0BB1 = 2993,
    FLAG_UNK_0x0BB2 = 2994,
    FLAG_UNK_0x0BB3 = 2995,
    FLAG_UNK_0x0BB4 = 2996,
    FLAG_UNK_0x0BB5 = 2997,
    FLAG_UNK_0x0BB6 = 2998,
    FLAG_UNK_0x0BB7 = 2999,
    FLAG_UNK_0x0BB8 = 3000,
    FLAG_UNK_0x0BB9 = 3001,
    FLAG_UNK_0x0BBA = 3002,
    FLAG_UNK_0x0BBB = 3003,
    FLAG_UNK_0x0BBC = 3004,
    FLAG_UNK_0x0BBD = 3005,
    FLAG_UNK_0x0BBE = 3006,
    FLAG_UNK_0x0BBF = 3007,
    FLAG_UNK_0x0BC0 = 3008,
    FLAG_UNK_0x0BC1 = 3009,
    FLAG_UNK_0x0BC2 = 3010,
    FLAG_UNK_0x0BC3 = 3011,
    FLAG_UNK_0x0BC4 = 3012,
    FLAG_UNK_0x0BC5 = 3013,
    FLAG_UNK_0x0BC6 = 3014,
    FLAG_UNK_0x0BC7 = 3015,
    FLAG_UNK_0x0BC8 = 3016,
    FLAG_UNK_0x0BC9 = 3017,
    FLAG_UNK_0x0BCA = 3018,
    FLAG_UNK_0x0BCB = 3019,
    FLAG_UNK_0x0BCC = 3020,
    FLAG_UNK_0x0BCD = 3021,
    FLAG_UNK_0x0BCE = 3022,
    FLAG_UNK_0x0BCF = 3023,
    FLAG_UNK_0x0BD0 = 3024,
    FLAG_UNK_0x0BD1 = 3025,
    FLAG_UNK_0x0BD2 = 3026,
    FLAG_UNK_0x0BD3 = 3027,
    FLAG_UNK_0x0BD4 = 3028,
    FLAG_UNK_0x0BD5 = 3029,
    FLAG_UNK_0x0BD6 = 3030,
    FLAG_UNK_0x0BD7 = 3031,
    FLAG_UNK_0x0BD8 = 3032,
    FLAG_UNK_0x0BD9 = 3033,
    FLAG_UNK_0x0BDA = 3034,
    FLAG_UNK_0x0BDB = 3035,
    FLAG_UNK_0x0BDC = 3036,
    FLAG_UNK_0x0BDD = 3037,
    FLAG_UNK_0x0BDE = 3038,
    FLAG_UNK_0x0BDF = 3039,
    FLAG_UNK_0x0BE0 = 3040,
    FLAG_UNK_0x0BE1 = 3041,
    FLAG_UNK_0x0BE2 = 3042,
    FLAG_UNK_0x0BE3 = 3043,
    FLAG_UNK_0x0BE4 = 3044,
    FLAG_UNK_0x0BE5 = 3045,
    FLAG_UNK_0x0BE6 = 3046,
    FLAG_UNK_0x0BE7 = 3047,
    FLAG_UNK_0x0BE8 = 3048,
    FLAG_UNK_0x0BE9 = 3049,
    FLAG_UNK_0x0BEA = 3050,
    FLAG_UNK_0x0BEB = 3051,
    FLAG_UNK_0x0BEC = 3052,
    FLAG_UNK_0x0BED = 3053,
    FLAG_UNK_0x0BEE = 3054,
    FLAG_UNK_0x0BEF = 3055,
    FLAG_UNK_0x0BF0 = 3056,
    FLAG_UNK_0x0BF1 = 3057,
    FLAG_UNK_0x0BF2 = 3058,
    FLAG_UNK_0x0BF3 = 3059,
    FLAG_UNK_0x0BF4 = 3060,
    FLAG_UNK_0x0BF5 = 3061,
    FLAG_UNK_0x0BF6 = 3062,
    FLAG_UNK_0x0BF7 = 3063,
    FLAG_UNK_0x0BF8 = 3064,
    FLAG_UNK_0x0BF9 = 3065,
    FLAG_UNK_0x0BFA = 3066,
    FLAG_UNK_0x0BFB = 3067,
    FLAG_UNK_0x0BFC = 3068,
    FLAG_UNK_0x0BFD = 3069,
    FLAG_UNK_0x0BFE = 3070,
    FLAG_UNK_0x0BFF = 3071,
    FLAG_UNK_0x0C00 = 3072,
    FLAG_UNK_0x0C01 = 3073,
    FLAG_UNK_0x0C02 = 3074,
    FLAG_UNK_0x0C03 = 3075,
    FLAG_UNK_0x0C04 = 3076,
    FLAG_UNK_0x0C05 = 3077,
    FLAG_UNK_0x0C06 = 3078,
    FLAG_UNK_0x0C07 = 3079,
    FLAG_UNK_0x0C08 = 3080,
    FLAG_UNK_0x0C09 = 3081,
    FLAG_UNK_0x0C0A = 3082,
    FLAG_UNK_0x0C0B = 3083,
    FLAG_UNK_0x0C0C = 3084,
    FLAG_UNK_0x0C0D = 3085,
    FLAG_UNK_0x0C0E = 3086,
    FLAG_UNK_0x0C0F = 3087,
    FLAG_UNK_0x0C10 = 3088,
    FLAG_UNK_0x0C11 = 3089,
    FLAG_UNK_0x0C12 = 3090,
    FLAG_UNK_0x0C13 = 3091,
    FLAG_UNK_0x0C14 = 3092,
    FLAG_UNK_0x0C15 = 3093,
    FLAG_UNK_0x0C16 = 3094,
    FLAG_UNK_0x0C17 = 3095,
    FLAG_UNK_0x0C18 = 3096,
    FLAG_UNK_0x0C19 = 3097,
    FLAG_UNK_0x0C1A = 3098,
    FLAG_UNK_0x0C1B = 3099,
    FLAG_UNK_0x0C1C = 3100,
    FLAG_UNK_0x0C1D = 3101,
    FLAG_UNK_0x0C1E = 3102,
    FLAG_UNK_0x0C1F = 3103,
    FLAG_UNK_0x0C20 = 3104,
    FLAG_UNK_0x0C21 = 3105,
    FLAG_UNK_0x0C22 = 3106,
    FLAG_UNK_0x0C23 = 3107,
    FLAG_UNK_0x0C24 = 3108,
    FLAG_UNK_0x0C25 = 3109,
    FLAG_UNK_0x0C26 = 3110,
    FLAG_UNK_0x0C27 = 3111,
    FLAG_UNK_0x0C28 = 3112,
    FLAG_UNK_0x0C29 = 3113,
    FLAG_UNK_0x0C2A = 3114,
    FLAG_UNK_0x0C2B = 3115,
    FLAG_UNK_0x0C2C = 3116,
    FLAG_UNK_0x0C2D = 3117,
    FLAG_UNK_0x0C2E = 3118,
    FLAG_UNK_0x0C2F = 3119,
    FLAG_UNK_0x0C30 = 3120,
    FLAG_UNK_0x0C31 = 3121,
    FLAG_UNK_0x0C32 = 3122,
    FLAG_UNK_0x0C33 = 3123,
    FLAG_UNK_0x0C34 = 3124,
    FLAG_UNK_0x0C35 = 3125,
    FLAG_UNK_0x0C36 = 3126,
    FLAG_UNK_0x0C37 = 3127,
    FLAG_UNK_0x0C38 = 3128,
    FLAG_UNK_0x0C39 = 3129,
    FLAG_UNK_0x0C3A = 3130,
    FLAG_UNK_0x0C3B = 3131,
    FLAG_UNK_0x0C3C = 3132,
    FLAG_UNK_0x0C3D = 3133,
    FLAG_UNK_0x0C3E = 3134,
    FLAG_UNK_0x0C3F = 3135,
    FLAG_UNK_0x0C40 = 3136,
    FLAG_UNK_0x0C41 = 3137,
    FLAG_UNK_0x0C42 = 3138,
    FLAG_UNK_0x0C43 = 3139,
    FLAG_UNK_0x0C44 = 3140,
    FLAG_UNK_0x0C45 = 3141,
    FLAG_UNK_0x0C46 = 3142,
    FLAG_UNK_0x0C47 = 3143,
    FLAG_UNK_0x0C48 = 3144,
    FLAG_UNK_0x0C49 = 3145,
    FLAG_UNK_0x0C4A = 3146,
    FLAG_UNK_0x0C4B = 3147,
    FLAG_UNK_0x0C4C = 3148,
    FLAG_UNK_0x0C4D = 3149,
    FLAG_UNK_0x0C4E = 3150,
    FLAG_UNK_0x0C4F = 3151,
    FLAG_UNK_0x0C50 = 3152,
    FLAG_UNK_0x0C51 = 3153,
    FLAG_UNK_0x0C52 = 3154,
    FLAG_UNK_0x0C53 = 3155,
    FLAG_UNK_0x0C54 = 3156,
    FLAG_UNK_0x0C55 = 3157,
    FLAG_UNK_0x0C56 = 3158,
    FLAG_UNK_0x0C57 = 3159,
    FLAG_UNK_0x0C58 = 3160,
    FLAG_UNK_0x0C59 = 3161,
    FLAG_UNK_0x0C5A = 3162,
    FLAG_UNK_0x0C5B = 3163,
    FLAG_UNK_0x0C5C = 3164,
    FLAG_UNK_0x0C5D = 3165,
    FLAG_UNK_0x0C5E = 3166,
    FLAG_UNK_0x0C5F = 3167,
    FLAG_UNK_0x0C60 = 3168,
    FLAG_UNK_0x0C61 = 3169,
    FLAG_UNK_0x0C62 = 3170,
    FLAG_UNK_0x0C63 = 3171,
    FLAG_UNK_0x0C64 = 3172,
    FLAG_UNK_0x0C65 = 3173,
    FLAG_UNK_0x0C66 = 3174,
    FLAG_UNK_0x0C67 = 3175,
    FLAG_UNK_0x0C68 = 3176,
    FLAG_UNK_0x0C69 = 3177,
    FLAG_UNK_0x0C6A = 3178,
    FLAG_UNK_0x0C6B = 3179,
    FLAG_UNK_0x0C6C = 3180,
    FLAG_UNK_0x0C6D = 3181,
    FLAG_UNK_0x0C6E = 3182,
    FLAG_UNK_0x0C6F = 3183,
    FLAG_UNK_0x0C70 = 3184,
    FLAG_UNK_0x0C71 = 3185,
    FLAG_UNK_0x0C72 = 3186,
    FLAG_UNK_0x0C73 = 3187,
    FLAG_UNK_0x0C74 = 3188,
    FLAG_UNK_0x0C75 = 3189,
    FLAG_UNK_0x0C76 = 3190,
    FLAG_UNK_0x0C77 = 3191,
    FLAG_UNK_0x0C78 = 3192,
    FLAG_UNK_0x0C79 = 3193,
    FLAG_UNK_0x0C7A = 3194,
    FLAG_UNK_0x0C7B = 3195,
    FLAG_UNK_0x0C7C = 3196,
    FLAG_UNK_0x0C7D = 3197,
    FLAG_UNK_0x0C7E = 3198,
    FLAG_UNK_0x0C7F = 3199,
    FLAG_UNK_0x0C80 = 3200,
    FLAG_UNK_0x0C81 = 3201,
    FLAG_UNK_0x0C82 = 3202,
    FLAG_UNK_0x0C83 = 3203,
    FLAG_UNK_0x0C84 = 3204,
    FLAG_UNK_0x0C85 = 3205,
    FLAG_UNK_0x0C86 = 3206,
    FLAG_UNK_0x0C87 = 3207,
    FLAG_UNK_0x0C88 = 3208,
    FLAG_UNK_0x0C89 = 3209,
    FLAG_UNK_0x0C8A = 3210,
    FLAG_UNK_0x0C8B = 3211,
    FLAG_UNK_0x0C8C = 3212,
    FLAG_UNK_0x0C8D = 3213,
    FLAG_UNK_0x0C8E = 3214,
    FLAG_UNK_0x0C8F = 3215,
    FLAG_UNK_0x0C90 = 3216,
    FLAG_UNK_0x0C91 = 3217,
    FLAG_UNK_0x0C92 = 3218,
    FLAG_UNK_0x0C93 = 3219,
    FLAG_UNK_0x0C94 = 3220,
    FLAG_UNK_0x0C95 = 3221,
    FLAG_UNK_0x0C96 = 3222,
    FLAG_UNK_0x0C97 = 3223,
    FLAG_UNK_0x0C98 = 3224,
    FLAG_UNK_0x0C99 = 3225,
    FLAG_UNK_0x0C9A = 3226,
    FLAG_UNK_0x0C9B = 3227,
    FLAG_UNK_0x0C9C = 3228,
    FLAG_UNK_0x0C9D = 3229,
    FLAG_UNK_0x0C9E = 3230,
    FLAG_UNK_0x0C9F = 3231,
    FLAG_UNK_0x0CA0 = 3232,
    FLAG_UNK_0x0CA1 = 3233,
    FLAG_UNK_0x0CA2 = 3234,
    FLAG_UNK_0x0CA3 = 3235,
    FLAG_UNK_0x0CA4 = 3236,
    FLAG_UNK_0x0CA5 = 3237,
    FLAG_UNK_0x0CA6 = 3238,
    FLAG_UNK_0x0CA7 = 3239,
    FLAG_UNK_0x0CA8 = 3240,
    FLAG_UNK_0x0CA9 = 3241,
    FLAG_UNK_0x0CAA = 3242,
    FLAG_UNK_0x0CAB = 3243,
    FLAG_UNK_0x0CAC = 3244,
    FLAG_UNK_0x0CAD = 3245,
    FLAG_UNK_0x0CAE = 3246,
    FLAG_UNK_0x0CAF = 3247,
    FLAG_UNK_0x0CB0 = 3248,
    FLAG_UNK_0x0CB1 = 3249,
    FLAG_UNK_0x0CB2 = 3250,
    FLAG_UNK_0x0CB3 = 3251,
    FLAG_UNK_0x0CB4 = 3252,
    FLAG_UNK_0x0CB5 = 3253,
    FLAG_UNK_0x0CB6 = 3254,
    FLAG_UNK_0x0CB7 = 3255,
    FLAG_UNK_0x0CB8 = 3256,
    FLAG_UNK_0x0CB9 = 3257,
    FLAG_UNK_0x0CBA = 3258,
    FLAG_UNK_0x0CBB = 3259,
    FLAG_UNK_0x0CBC = 3260,
    FLAG_UNK_0x0CBD = 3261,
    FLAG_UNK_0x0CBE = 3262,
    FLAG_UNK_0x0CBF = 3263,
    FLAG_UNK_0x0CC0 = 3264,
    FLAG_UNK_0x0CC1 = 3265,
    FLAG_UNK_0x0CC2 = 3266,
    FLAG_UNK_0x0CC3 = 3267,
    FLAG_UNK_0x0CC4 = 3268,
    FLAG_UNK_0x0CC5 = 3269,
    FLAG_UNK_0x0CC6 = 3270,
    FLAG_UNK_0x0CC7 = 3271,
    FLAG_UNK_0x0CC8 = 3272,
    FLAG_UNK_0x0CC9 = 3273,
    FLAG_UNK_0x0CCA = 3274,
    FLAG_UNK_0x0CCB = 3275,
    FLAG_UNK_0x0CCC = 3276,
    FLAG_UNK_0x0CCD = 3277,
    FLAG_UNK_0x0CCE = 3278,
    FLAG_UNK_0x0CCF = 3279,
    FLAG_UNK_0x0CD0 = 3280,
    FLAG_UNK_0x0CD1 = 3281,
    FLAG_UNK_0x0CD2 = 3282,
    FLAG_UNK_0x0CD3 = 3283,
    FLAG_UNK_0x0CD4 = 3284,
    FLAG_UNK_0x0CD5 = 3285,
    FLAG_UNK_0x0CD6 = 3286,
    FLAG_UNK_0x0CD7 = 3287,
    FLAG_UNK_0x0CD8 = 3288,
    FLAG_UNK_0x0CD9 = 3289,
    FLAG_UNK_0x0CDA = 3290,
    FLAG_UNK_0x0CDB = 3291,
    FLAG_UNK_0x0CDC = 3292,
    FLAG_UNK_0x0CDD = 3293,
    FLAG_UNK_0x0CDE = 3294,
    FLAG_UNK_0x0CDF = 3295,
    FLAG_UNK_0x0CE0 = 3296,
    FLAG_UNK_0x0CE1 = 3297,
    FLAG_UNK_0x0CE2 = 3298,
    FLAG_UNK_0x0CE3 = 3299,
    FLAG_UNK_0x0CE4 = 3300,
    FLAG_UNK_0x0CE5 = 3301,
    FLAG_UNK_0x0CE6 = 3302,
    FLAG_UNK_0x0CE7 = 3303,
    FLAG_UNK_0x0CE8 = 3304,
    FLAG_UNK_0x0CE9 = 3305,
    FLAG_UNK_0x0CEA = 3306,
    FLAG_UNK_0x0CEB = 3307,
    FLAG_UNK_0x0CEC = 3308,
    FLAG_UNK_0x0CED = 3309,
    FLAG_UNK_0x0CEE = 3310,
    FLAG_UNK_0x0CEF = 3311,
    FLAG_UNK_0x0CF0 = 3312,
    FLAG_UNK_0x0CF1 = 3313,
    FLAG_UNK_0x0CF2 = 3314,
    FLAG_UNK_0x0CF3 = 3315,
    FLAG_UNK_0x0CF4 = 3316,
    FLAG_UNK_0x0CF5 = 3317,
    FLAG_UNK_0x0CF6 = 3318,
    FLAG_UNK_0x0CF7 = 3319,
    FLAG_UNK_0x0CF8 = 3320,
    FLAG_UNK_0x0CF9 = 3321,
    FLAG_UNK_0x0CFA = 3322,
    FLAG_UNK_0x0CFB = 3323,
    FLAG_UNK_0x0CFC = 3324,
    FLAG_UNK_0x0CFD = 3325,
    FLAG_UNK_0x0CFE = 3326,
    FLAG_UNK_0x0CFF = 3327,
    FLAG_UNK_0x0D00 = 3328,
    FLAG_UNK_0x0D01 = 3329,
    FLAG_UNK_0x0D02 = 3330,
    FLAG_UNK_0x0D03 = 3331,
    FLAG_UNK_0x0D04 = 3332,
    FLAG_UNK_0x0D05 = 3333,
    FLAG_UNK_0x0D06 = 3334,
    FLAG_UNK_0x0D07 = 3335,
    FLAG_UNK_0x0D08 = 3336,
    FLAG_UNK_0x0D09 = 3337,
    FLAG_UNK_0x0D0A = 3338,
    FLAG_UNK_0x0D0B = 3339,
    FLAG_UNK_0x0D0C = 3340,
    FLAG_UNK_0x0D0D = 3341,
    FLAG_UNK_0x0D0E = 3342,
    FLAG_UNK_0x0D0F = 3343,
    FLAG_UNK_0x0D10 = 3344,
    FLAG_UNK_0x0D11 = 3345,
    FLAG_UNK_0x0D12 = 3346,
    FLAG_UNK_0x0D13 = 3347,
    FLAG_UNK_0x0D14 = 3348,
    FLAG_UNK_0x0D15 = 3349,
    FLAG_UNK_0x0D16 = 3350,
    FLAG_UNK_0x0D17 = 3351,
    FLAG_UNK_0x0D18 = 3352,
    FLAG_UNK_0x0D19 = 3353,
    FLAG_UNK_0x0D1A = 3354,
    FLAG_UNK_0x0D1B = 3355,
    FLAG_UNK_0x0D1C = 3356,
    FLAG_UNK_0x0D1D = 3357,
    FLAG_UNK_0x0D1E = 3358,
    FLAG_UNK_0x0D1F = 3359,
    FLAG_UNK_0x0D20 = 3360,
    FLAG_UNK_0x0D21 = 3361,
    FLAG_UNK_0x0D22 = 3362,
    FLAG_UNK_0x0D23 = 3363,
    FLAG_UNK_0x0D24 = 3364,
    FLAG_UNK_0x0D25 = 3365,
    FLAG_UNK_0x0D26 = 3366,
    FLAG_UNK_0x0D27 = 3367,
    FLAG_UNK_0x0D28 = 3368,
    FLAG_UNK_0x0D29 = 3369,
    FLAG_UNK_0x0D2A = 3370,
    FLAG_UNK_0x0D2B = 3371,
    FLAG_UNK_0x0D2C = 3372,
    FLAG_UNK_0x0D2D = 3373,
    FLAG_UNK_0x0D2E = 3374,
    FLAG_UNK_0x0D2F = 3375,
    FLAG_UNK_0x0D30 = 3376,
    FLAG_UNK_0x0D31 = 3377,
    FLAG_UNK_0x0D32 = 3378,
    FLAG_UNK_0x0D33 = 3379,
    FLAG_UNK_0x0D34 = 3380,
    FLAG_UNK_0x0D35 = 3381,
    FLAG_UNK_0x0D36 = 3382,
    FLAG_UNK_0x0D37 = 3383,
    FLAG_UNK_0x0D38 = 3384,
    FLAG_UNK_0x0D39 = 3385,
    FLAG_UNK_0x0D3A = 3386,
    FLAG_UNK_0x0D3B = 3387,
    FLAG_UNK_0x0D3C = 3388,
    FLAG_UNK_0x0D3D = 3389,
    FLAG_UNK_0x0D3E = 3390,
    FLAG_UNK_0x0D3F = 3391,
    FLAG_UNK_0x0D40 = 3392,
    FLAG_UNK_0x0D41 = 3393,
    FLAG_UNK_0x0D42 = 3394,
    FLAG_UNK_0x0D43 = 3395,
    FLAG_UNK_0x0D44 = 3396,
    FLAG_UNK_0x0D45 = 3397,
    FLAG_UNK_0x0D46 = 3398,
    FLAG_UNK_0x0D47 = 3399,
    FLAG_UNK_0x0D48 = 3400,
    FLAG_UNK_0x0D49 = 3401,
    FLAG_UNK_0x0D4A = 3402,
    FLAG_UNK_0x0D4B = 3403,
    FLAG_UNK_0x0D4C = 3404,
    FLAG_UNK_0x0D4D = 3405,
    FLAG_UNK_0x0D4E = 3406,
    FLAG_UNK_0x0D4F = 3407,
    FLAG_UNK_0x0D50 = 3408,
    FLAG_UNK_0x0D51 = 3409,
    FLAG_UNK_0x0D52 = 3410,
    FLAG_UNK_0x0D53 = 3411,
    FLAG_UNK_0x0D54 = 3412,
    FLAG_UNK_0x0D55 = 3413,
    FLAG_UNK_0x0D56 = 3414,
    FLAG_UNK_0x0D57 = 3415,
    FLAG_UNK_0x0D58 = 3416,
    FLAG_UNK_0x0D59 = 3417,
    FLAG_UNK_0x0D5A = 3418,
    FLAG_UNK_0x0D5B = 3419,
    FLAG_UNK_0x0D5C = 3420,
    FLAG_UNK_0x0D5D = 3421,
    FLAG_UNK_0x0D5E = 3422,
    FLAG_UNK_0x0D5F = 3423,
    FLAG_UNK_0x0D60 = 3424,
    FLAG_UNK_0x0D61 = 3425,
    FLAG_UNK_0x0D62 = 3426,
    FLAG_UNK_0x0D63 = 3427,
    FLAG_UNK_0x0D64 = 3428,
    FLAG_UNK_0x0D65 = 3429,
    FLAG_UNK_0x0D66 = 3430,
    FLAG_UNK_0x0D67 = 3431,
    FLAG_UNK_0x0D68 = 3432,
    FLAG_UNK_0x0D69 = 3433,
    FLAG_UNK_0x0D6A = 3434,
    FLAG_UNK_0x0D6B = 3435,
    FLAG_UNK_0x0D6C = 3436,
    FLAG_UNK_0x0D6D = 3437,
    FLAG_UNK_0x0D6E = 3438,
    FLAG_UNK_0x0D6F = 3439,
    FLAG_UNK_0x0D70 = 3440,
    FLAG_UNK_0x0D71 = 3441,
    FLAG_UNK_0x0D72 = 3442,
    FLAG_UNK_0x0D73 = 3443,
    FLAG_UNK_0x0D74 = 3444,
    FLAG_UNK_0x0D75 = 3445,
    FLAG_UNK_0x0D76 = 3446,
    FLAG_UNK_0x0D77 = 3447,
    FLAG_UNK_0x0D78 = 3448,
    FLAG_UNK_0x0D79 = 3449,
    FLAG_UNK_0x0D7A = 3450,
    FLAG_UNK_0x0D7B = 3451,
    FLAG_UNK_0x0D7C = 3452,
    FLAG_UNK_0x0D7D = 3453,
    FLAG_UNK_0x0D7E = 3454,
    FLAG_UNK_0x0D7F = 3455,
    FLAG_UNK_0x0D80 = 3456,
    FLAG_UNK_0x0D81 = 3457,
    FLAG_UNK_0x0D82 = 3458,
    FLAG_UNK_0x0D83 = 3459,
    FLAG_UNK_0x0D84 = 3460,
    FLAG_UNK_0x0D85 = 3461,
    FLAG_UNK_0x0D86 = 3462,
    FLAG_UNK_0x0D87 = 3463,
    FLAG_UNK_0x0D88 = 3464,
    FLAG_UNK_0x0D89 = 3465,
    FLAG_UNK_0x0D8A = 3466,
    FLAG_UNK_0x0D8B = 3467,
    FLAG_UNK_0x0D8C = 3468,
    FLAG_UNK_0x0D8D = 3469,
    FLAG_UNK_0x0D8E = 3470,
    FLAG_UNK_0x0D8F = 3471,
    FLAG_UNK_0x0D90 = 3472,
    FLAG_UNK_0x0D91 = 3473,
    FLAG_UNK_0x0D92 = 3474,
    FLAG_UNK_0x0D93 = 3475,
    FLAG_UNK_0x0D94 = 3476,
    FLAG_UNK_0x0D95 = 3477,
    FLAG_UNK_0x0D96 = 3478,
    FLAG_UNK_0x0D97 = 3479,
    FLAG_UNK_0x0D98 = 3480,
    FLAG_UNK_0x0D99 = 3481,
    FLAG_UNK_0x0D9A = 3482,
    FLAG_UNK_0x0D9B = 3483,
    FLAG_UNK_0x0D9C = 3484,
    FLAG_UNK_0x0D9D = 3485,
    FLAG_UNK_0x0D9E = 3486,
    FLAG_UNK_0x0D9F = 3487,
    FLAG_UNK_0x0DA0 = 3488,
    FLAG_UNK_0x0DA1 = 3489,
    FLAG_UNK_0x0DA2 = 3490,
    FLAG_UNK_0x0DA3 = 3491,
    FLAG_UNK_0x0DA4 = 3492,
    FLAG_UNK_0x0DA5 = 3493,
    FLAG_UNK_0x0DA6 = 3494,
    FLAG_UNK_0x0DA7 = 3495,
    FLAG_UNK_0x0DA8 = 3496,
    FLAG_UNK_0x0DA9 = 3497,
    FLAG_UNK_0x0DAA = 3498,
    FLAG_UNK_0x0DAB = 3499,
    FLAG_UNK_0x0DAC = 3500,
    FLAG_UNK_0x0DAD = 3501,
    FLAG_UNK_0x0DAE = 3502,
    FLAG_UNK_0x0DAF = 3503,
    FLAG_UNK_0x0DB0 = 3504,
    FLAG_UNK_0x0DB1 = 3505,
    FLAG_UNK_0x0DB2 = 3506,
    FLAG_UNK_0x0DB3 = 3507,
    FLAG_UNK_0x0DB4 = 3508,
    FLAG_UNK_0x0DB5 = 3509,
    FLAG_UNK_0x0DB6 = 3510,
    FLAG_UNK_0x0DB7 = 3511,
    FLAG_UNK_0x0DB8 = 3512,
    FLAG_UNK_0x0DB9 = 3513,
    FLAG_UNK_0x0DBA = 3514,
    FLAG_UNK_0x0DBB = 3515,
    FLAG_UNK_0x0DBC = 3516,
    FLAG_UNK_0x0DBD = 3517,
    FLAG_UNK_0x0DBE = 3518,
    FLAG_UNK_0x0DBF = 3519,
    FLAG_UNK_0x0DC0 = 3520,
    FLAG_UNK_0x0DC1 = 3521,
    FLAG_UNK_0x0DC2 = 3522,
    FLAG_UNK_0x0DC3 = 3523,
    FLAG_UNK_0x0DC4 = 3524,
    FLAG_UNK_0x0DC5 = 3525,
    FLAG_UNK_0x0DC6 = 3526,
    FLAG_UNK_0x0DC7 = 3527,
    FLAG_UNK_0x0DC8 = 3528,
    FLAG_UNK_0x0DC9 = 3529,
    FLAG_UNK_0x0DCA = 3530,
    FLAG_UNK_0x0DCB = 3531,
    FLAG_UNK_0x0DCC = 3532,
    FLAG_UNK_0x0DCD = 3533,
    FLAG_UNK_0x0DCE = 3534,
    FLAG_UNK_0x0DCF = 3535,
    FLAG_UNK_0x0DD0 = 3536,
    FLAG_UNK_0x0DD1 = 3537,
    FLAG_UNK_0x0DD2 = 3538,
    FLAG_UNK_0x0DD3 = 3539,
    FLAG_UNK_0x0DD4 = 3540,
    FLAG_UNK_0x0DD5 = 3541,
    FLAG_UNK_0x0DD6 = 3542,
    FLAG_UNK_0x0DD7 = 3543,
    FLAG_UNK_0x0DD8 = 3544,
    FLAG_UNK_0x0DD9 = 3545,
    FLAG_UNK_0x0DDA = 3546,
    FLAG_UNK_0x0DDB = 3547,
    FLAG_UNK_0x0DDC = 3548,
    FLAG_UNK_0x0DDD = 3549,
    FLAG_UNK_0x0DDE = 3550,
    FLAG_UNK_0x0DDF = 3551,
    FLAG_UNK_0x0DE0 = 3552,
    FLAG_UNK_0x0DE1 = 3553,
    FLAG_UNK_0x0DE2 = 3554,
    FLAG_UNK_0x0DE3 = 3555,
    FLAG_UNK_0x0DE4 = 3556,
    FLAG_UNK_0x0DE5 = 3557,
    FLAG_UNK_0x0DE6 = 3558,
    FLAG_UNK_0x0DE7 = 3559,
    FLAG_UNK_0x0DE8 = 3560,
    FLAG_UNK_0x0DE9 = 3561,
    FLAG_UNK_0x0DEA = 3562,
    FLAG_UNK_0x0DEB = 3563,
    FLAG_UNK_0x0DEC = 3564,
    FLAG_UNK_0x0DED = 3565,
    FLAG_UNK_0x0DEE = 3566,
    FLAG_UNK_0x0DEF = 3567,
    FLAG_UNK_0x0DF0 = 3568,
    FLAG_UNK_0x0DF1 = 3569,
    FLAG_UNK_0x0DF2 = 3570,
    FLAG_UNK_0x0DF3 = 3571,
    FLAG_UNK_0x0DF4 = 3572,
    FLAG_UNK_0x0DF5 = 3573,
    FLAG_UNK_0x0DF6 = 3574,
    FLAG_UNK_0x0DF7 = 3575,
    FLAG_UNK_0x0DF8 = 3576,
    FLAG_UNK_0x0DF9 = 3577,
    FLAG_UNK_0x0DFA = 3578,
    FLAG_UNK_0x0DFB = 3579,
    FLAG_UNK_0x0DFC = 3580,
    FLAG_UNK_0x0DFD = 3581,
    FLAG_UNK_0x0DFE = 3582,
    FLAG_UNK_0x0DFF = 3583,
    FLAG_UNK_0x0E00 = 3584,
    FLAG_UNK_0x0E01 = 3585,
    FLAG_UNK_0x0E02 = 3586,
    FLAG_UNK_0x0E03 = 3587,
    FLAG_UNK_0x0E04 = 3588,
    FLAG_UNK_0x0E05 = 3589,
    FLAG_UNK_0x0E06 = 3590,
    FLAG_UNK_0x0E07 = 3591,
    FLAG_UNK_0x0E08 = 3592,
    FLAG_UNK_0x0E09 = 3593,
    FLAG_UNK_0x0E0A = 3594,
    FLAG_UNK_0x0E0B = 3595,
    FLAG_UNK_0x0E0C = 3596,
    FLAG_UNK_0x0E0D = 3597,
    FLAG_UNK_0x0E0E = 3598,
    FLAG_UNK_0x0E0F = 3599,
    FLAG_UNK_0x0E10 = 3600,
    FLAG_UNK_0x0E11 = 3601,
    FLAG_UNK_0x0E12 = 3602,
    FLAG_UNK_0x0E13 = 3603,
    FLAG_UNK_0x0E14 = 3604,
    FLAG_UNK_0x0E15 = 3605,
    FLAG_UNK_0x0E16 = 3606,
    FLAG_UNK_0x0E17 = 3607,
    FLAG_UNK_0x0E18 = 3608,
    FLAG_UNK_0x0E19 = 3609,
    FLAG_UNK_0x0E1A = 3610,
    FLAG_UNK_0x0E1B = 3611,
    FLAG_UNK_0x0E1C = 3612,
    FLAG_UNK_0x0E1D = 3613,
    FLAG_UNK_0x0E1E = 3614,
    FLAG_UNK_0x0E1F = 3615,
    FLAG_UNK_0x0E20 = 3616,
    FLAG_UNK_0x0E21 = 3617,
    FLAG_UNK_0x0E22 = 3618,
    FLAG_UNK_0x0E23 = 3619,
    FLAG_UNK_0x0E24 = 3620,
    FLAG_UNK_0x0E25 = 3621,
    FLAG_UNK_0x0E26 = 3622,
    FLAG_UNK_0x0E27 = 3623,
    FLAG_UNK_0x0E28 = 3624,
    FLAG_UNK_0x0E29 = 3625,
    FLAG_UNK_0x0E2A = 3626,
    FLAG_UNK_0x0E2B = 3627,
    FLAG_UNK_0x0E2C = 3628,
    FLAG_UNK_0x0E2D = 3629,
    FLAG_UNK_0x0E2E = 3630,
    FLAG_UNK_0x0E2F = 3631,
    FLAG_UNK_0x0E30 = 3632,
    FLAG_UNK_0x0E31 = 3633,
    FLAG_UNK_0x0E32 = 3634,
    FLAG_UNK_0x0E33 = 3635,
    FLAG_UNK_0x0E34 = 3636,
    FLAG_UNK_0x0E35 = 3637,
    FLAG_UNK_0x0E36 = 3638,
    FLAG_UNK_0x0E37 = 3639,
    FLAG_UNK_0x0E38 = 3640,
    FLAG_UNK_0x0E39 = 3641,
    FLAG_UNK_0x0E3A = 3642,
    FLAG_UNK_0x0E3B = 3643,
    FLAG_UNK_0x0E3C = 3644,
    FLAG_UNK_0x0E3D = 3645,
    FLAG_UNK_0x0E3E = 3646,
    FLAG_UNK_0x0E3F = 3647,
    FLAG_UNK_0x0E40 = 3648,
    FLAG_UNK_0x0E41 = 3649,
    FLAG_UNK_0x0E42 = 3650,
    FLAG_UNK_0x0E43 = 3651,
    FLAG_UNK_0x0E44 = 3652,
    FLAG_UNK_0x0E45 = 3653,
    FLAG_UNK_0x0E46 = 3654,
    FLAG_UNK_0x0E47 = 3655,
    FLAG_UNK_0x0E48 = 3656,
    FLAG_UNK_0x0E49 = 3657,
    FLAG_UNK_0x0E4A = 3658,
    FLAG_UNK_0x0E4B = 3659,
    FLAG_UNK_0x0E4C = 3660,
    FLAG_UNK_0x0E4D = 3661,
    FLAG_UNK_0x0E4E = 3662,
    FLAG_UNK_0x0E4F = 3663,
    FLAG_UNK_0x0E50 = 3664,
    FLAG_UNK_0x0E51 = 3665,
    FLAG_UNK_0x0E52 = 3666,
    FLAG_UNK_0x0E53 = 3667,
    FLAG_UNK_0x0E54 = 3668,
    FLAG_UNK_0x0E55 = 3669,
    FLAG_UNK_0x0E56 = 3670,
    FLAG_UNK_0x0E57 = 3671,
    FLAG_UNK_0x0E58 = 3672,
    FLAG_UNK_0x0E59 = 3673,
    FLAG_UNK_0x0E5A = 3674,
    FLAG_UNK_0x0E5B = 3675,
    FLAG_UNK_0x0E5C = 3676,
    FLAG_UNK_0x0E5D = 3677,
    FLAG_UNK_0x0E5E = 3678,
    FLAG_UNK_0x0E5F = 3679,
    FLAG_UNK_0x0E60 = 3680,
    FLAG_UNK_0x0E61 = 3681,
    FLAG_UNK_0x0E62 = 3682,
    FLAG_UNK_0x0E63 = 3683,
    FLAG_UNK_0x0E64 = 3684,
    FLAG_UNK_0x0E65 = 3685,
    FLAG_UNK_0x0E66 = 3686,
    FLAG_UNK_0x0E67 = 3687,
    FLAG_UNK_0x0E68 = 3688,
    FLAG_UNK_0x0E69 = 3689,
    FLAG_UNK_0x0E6A = 3690,
    FLAG_UNK_0x0E6B = 3691,
    FLAG_UNK_0x0E6C = 3692,
    FLAG_UNK_0x0E6D = 3693,
    FLAG_UNK_0x0E6E = 3694,
    FLAG_UNK_0x0E6F = 3695,
    FLAG_UNK_0x0E70 = 3696,
    FLAG_UNK_0x0E71 = 3697,
    FLAG_UNK_0x0E72 = 3698,
    FLAG_UNK_0x0E73 = 3699,
    FLAG_UNK_0x0E74 = 3700,
    FLAG_UNK_0x0E75 = 3701,
    FLAG_UNK_0x0E76 = 3702,
    FLAG_UNK_0x0E77 = 3703,
    FLAG_UNK_0x0E78 = 3704,
    FLAG_UNK_0x0E79 = 3705,
    FLAG_UNK_0x0E7A = 3706,
    FLAG_UNK_0x0E7B = 3707,
    FLAG_UNK_0x0E7C = 3708,
    FLAG_UNK_0x0E7D = 3709,
    FLAG_UNK_0x0E7E = 3710,
    FLAG_UNK_0x0E7F = 3711,
    FLAG_UNK_0x0E80 = 3712,
    FLAG_UNK_0x0E81 = 3713,
    FLAG_UNK_0x0E82 = 3714,
    FLAG_UNK_0x0E83 = 3715,
    FLAG_UNK_0x0E84 = 3716,
    FLAG_UNK_0x0E85 = 3717,
    FLAG_UNK_0x0E86 = 3718,
    FLAG_UNK_0x0E87 = 3719,
    FLAG_UNK_0x0E88 = 3720,
    FLAG_UNK_0x0E89 = 3721,
    FLAG_UNK_0x0E8A = 3722,
    FLAG_UNK_0x0E8B = 3723,
    FLAG_UNK_0x0E8C = 3724,
    FLAG_UNK_0x0E8D = 3725,
    FLAG_UNK_0x0E8E = 3726,
    FLAG_UNK_0x0E8F = 3727,
    FLAG_UNK_0x0E90 = 3728,
    FLAG_UNK_0x0E91 = 3729,
    FLAG_UNK_0x0E92 = 3730,
    FLAG_UNK_0x0E93 = 3731,
    FLAG_UNK_0x0E94 = 3732,
    FLAG_UNK_0x0E95 = 3733,
    FLAG_UNK_0x0E96 = 3734,
    FLAG_UNK_0x0E97 = 3735,
    FLAG_UNK_0x0E98 = 3736,
    FLAG_UNK_0x0E99 = 3737,
    FLAG_UNK_0x0E9A = 3738,
    FLAG_UNK_0x0E9B = 3739,
    FLAG_UNK_0x0E9C = 3740,
    FLAG_UNK_0x0E9D = 3741,
    FLAG_UNK_0x0E9E = 3742,
    FLAG_UNK_0x0E9F = 3743,
    FLAG_UNK_0x0EA0 = 3744,
    FLAG_UNK_0x0EA1 = 3745,
    FLAG_UNK_0x0EA2 = 3746,
    FLAG_UNK_0x0EA3 = 3747,
    FLAG_UNK_0x0EA4 = 3748,
    FLAG_UNK_0x0EA5 = 3749,
    FLAG_UNK_0x0EA6 = 3750,
    FLAG_UNK_0x0EA7 = 3751,
    FLAG_UNK_0x0EA8 = 3752,
    FLAG_UNK_0x0EA9 = 3753,
    FLAG_UNK_0x0EAA = 3754,
    FLAG_UNK_0x0EAB = 3755,
    FLAG_UNK_0x0EAC = 3756,
    FLAG_UNK_0x0EAD = 3757,
    FLAG_UNK_0x0EAE = 3758,
    FLAG_UNK_0x0EAF = 3759,
    FLAG_UNK_0x0EB0 = 3760,
    FLAG_UNK_0x0EB1 = 3761,
    FLAG_UNK_0x0EB2 = 3762,
    FLAG_UNK_0x0EB3 = 3763,
    FLAG_UNK_0x0EB4 = 3764,
    FLAG_UNK_0x0EB5 = 3765,
    FLAG_UNK_0x0EB6 = 3766,
    FLAG_UNK_0x0EB7 = 3767,
    FLAG_UNK_0x0EB8 = 3768,
    FLAG_UNK_0x0EB9 = 3769,
    FLAG_UNK_0x0EBA = 3770,
    FLAG_UNK_0x0EBB = 3771,
    FLAG_UNK_0x0EBC = 3772,
    FLAG_UNK_0x0EBD = 3773,
    FLAG_UNK_0x0EBE = 3774,
    FLAG_UNK_0x0EBF = 3775,
    FLAG_UNK_0x0EC0 = 3776,
    FLAG_UNK_0x0EC1 = 3777,
    FLAG_UNK_0x0EC2 = 3778,
    FLAG_UNK_0x0EC3 = 3779,
    FLAG_UNK_0x0EC4 = 3780,
    FLAG_UNK_0x0EC5 = 3781,
    FLAG_UNK_0x0EC6 = 3782,
    FLAG_UNK_0x0EC7 = 3783,
    FLAG_UNK_0x0EC8 = 3784,
    FLAG_UNK_0x0EC9 = 3785,
    FLAG_UNK_0x0ECA = 3786,
    FLAG_UNK_0x0ECB = 3787,
    FLAG_UNK_0x0ECC = 3788,
    FLAG_UNK_0x0ECD = 3789,
    FLAG_UNK_0x0ECE = 3790,
    FLAG_UNK_0x0ECF = 3791,
    FLAG_UNK_0x0ED0 = 3792,
    FLAG_UNK_0x0ED1 = 3793,
    FLAG_UNK_0x0ED2 = 3794,
    FLAG_UNK_0x0ED3 = 3795,
    FLAG_UNK_0x0ED4 = 3796,
    FLAG_UNK_0x0ED5 = 3797,
    FLAG_UNK_0x0ED6 = 3798,
    FLAG_UNK_0x0ED7 = 3799,
    FLAG_UNK_0x0ED8 = 3800,
    FLAG_UNK_0x0ED9 = 3801,
    FLAG_UNK_0x0EDA = 3802,
    FLAG_UNK_0x0EDB = 3803,
    FLAG_UNK_0x0EDC = 3804,
    FLAG_UNK_0x0EDD = 3805,
    FLAG_UNK_0x0EDE = 3806,
    FLAG_UNK_0x0EDF = 3807,
    FLAG_UNK_0x0EE0 = 3808,
    FLAG_UNK_0x0EE1 = 3809,
    FLAG_UNK_0x0EE2 = 3810,
    FLAG_UNK_0x0EE3 = 3811,
    FLAG_UNK_0x0EE4 = 3812,
    FLAG_UNK_0x0EE5 = 3813,
    FLAG_UNK_0x0EE6 = 3814,
    FLAG_UNK_0x0EE7 = 3815,
    FLAG_UNK_0x0EE8 = 3816,
    FLAG_UNK_0x0EE9 = 3817,
    FLAG_UNK_0x0EEA = 3818,
    FLAG_UNK_0x0EEB = 3819,
    FLAG_UNK_0x0EEC = 3820,
    FLAG_UNK_0x0EED = 3821,
    FLAG_UNK_0x0EEE = 3822,
    FLAG_UNK_0x0EEF = 3823,
    FLAG_UNK_0x0EF0 = 3824,
    FLAG_UNK_0x0EF1 = 3825,
    FLAG_UNK_0x0EF2 = 3826,
    FLAG_UNK_0x0EF3 = 3827,
    FLAG_UNK_0x0EF4 = 3828,
    FLAG_UNK_0x0EF5 = 3829,
    FLAG_UNK_0x0EF6 = 3830,
    FLAG_UNK_0x0EF7 = 3831,
    FLAG_UNK_0x0EF8 = 3832,
    FLAG_UNK_0x0EF9 = 3833,
    FLAG_UNK_0x0EFA = 3834,
    FLAG_UNK_0x0EFB = 3835,
    FLAG_UNK_0x0EFC = 3836,
    FLAG_UNK_0x0EFD = 3837,
    FLAG_UNK_0x0EFE = 3838,
    FLAG_UNK_0x0EFF = 3839,
    FLAG_UNK_0x0F00 = 3840,
    FLAG_UNK_0x0F01 = 3841,
    FLAG_UNK_0x0F02 = 3842,
    FLAG_UNK_0x0F03 = 3843,
    FLAG_UNK_0x0F04 = 3844,
    FLAG_UNK_0x0F05 = 3845,
    FLAG_UNK_0x0F06 = 3846,
    FLAG_UNK_0x0F07 = 3847,
    FLAG_UNK_0x0F08 = 3848,
    FLAG_UNK_0x0F09 = 3849,
    FLAG_UNK_0x0F0A = 3850,
    FLAG_UNK_0x0F0B = 3851,
    FLAG_UNK_0x0F0C = 3852,
    FLAG_UNK_0x0F0D = 3853,
    FLAG_UNK_0x0F0E = 3854,
    FLAG_UNK_0x0F0F = 3855,
    FLAG_UNK_0x0F10 = 3856,
    FLAG_UNK_0x0F11 = 3857,
    FLAG_UNK_0x0F12 = 3858,
    FLAG_UNK_0x0F13 = 3859,
    FLAG_UNK_0x0F14 = 3860,
    FLAG_UNK_0x0F15 = 3861,
    FLAG_UNK_0x0F16 = 3862,
    FLAG_UNK_0x0F17 = 3863,
    FLAG_UNK_0x0F18 = 3864,
    FLAG_UNK_0x0F19 = 3865,
    FLAG_UNK_0x0F1A = 3866,
    FLAG_UNK_0x0F1B = 3867,
    FLAG_UNK_0x0F1C = 3868,
    FLAG_UNK_0x0F1D = 3869,
    FLAG_UNK_0x0F1E = 3870,
    FLAG_UNK_0x0F1F = 3871,
    FLAG_UNK_0x0F20 = 3872,
    FLAG_UNK_0x0F21 = 3873,
    FLAG_UNK_0x0F22 = 3874,
    FLAG_UNK_0x0F23 = 3875,
    FLAG_UNK_0x0F24 = 3876,
    FLAG_UNK_0x0F25 = 3877,
    FLAG_UNK_0x0F26 = 3878,
    FLAG_UNK_0x0F27 = 3879,
    FLAG_UNK_0x0F28 = 3880,
    FLAG_UNK_0x0F29 = 3881,
    FLAG_UNK_0x0F2A = 3882,
    FLAG_UNK_0x0F2B = 3883,
    FLAG_UNK_0x0F2C = 3884,
    FLAG_UNK_0x0F2D = 3885,
    FLAG_UNK_0x0F2E = 3886,
    FLAG_UNK_0x0F2F = 3887,
    FLAG_UNK_0x0F30 = 3888,
    FLAG_UNK_0x0F31 = 3889,
    FLAG_UNK_0x0F32 = 3890,
    FLAG_UNK_0x0F33 = 3891,
    FLAG_UNK_0x0F34 = 3892,
    FLAG_UNK_0x0F35 = 3893,
    FLAG_UNK_0x0F36 = 3894,
    FLAG_UNK_0x0F37 = 3895,
    FLAG_UNK_0x0F38 = 3896,
    FLAG_UNK_0x0F39 = 3897,
    FLAG_UNK_0x0F3A = 3898,
    FLAG_UNK_0x0F3B = 3899,
    FLAG_UNK_0x0F3C = 3900,
    FLAG_UNK_0x0F3D = 3901,
    FLAG_UNK_0x0F3E = 3902,
    FLAG_UNK_0x0F3F = 3903,
    FLAG_UNK_0x0F40 = 3904,
    FLAG_UNK_0x0F41 = 3905,
    FLAG_UNK_0x0F42 = 3906,
    FLAG_UNK_0x0F43 = 3907,
    FLAG_UNK_0x0F44 = 3908,
    FLAG_UNK_0x0F45 = 3909,
    FLAG_UNK_0x0F46 = 3910,
    FLAG_UNK_0x0F47 = 3911,
    FLAG_UNK_0x0F48 = 3912,
    FLAG_UNK_0x0F49 = 3913,
    FLAG_UNK_0x0F4A = 3914,
    FLAG_UNK_0x0F4B = 3915,
    FLAG_UNK_0x0F4C = 3916,
    FLAG_UNK_0x0F4D = 3917,
    FLAG_UNK_0x0F4E = 3918,
    FLAG_UNK_0x0F4F = 3919,
    FLAG_UNK_0x0F50 = 3920,
    FLAG_UNK_0x0F51 = 3921,
    FLAG_UNK_0x0F52 = 3922,
    FLAG_UNK_0x0F53 = 3923,
    FLAG_UNK_0x0F54 = 3924,
    FLAG_UNK_0x0F55 = 3925,
    FLAG_UNK_0x0F56 = 3926,
    FLAG_UNK_0x0F57 = 3927,
    FLAG_UNK_0x0F58 = 3928,
    FLAG_UNK_0x0F59 = 3929,
    FLAG_UNK_0x0F5A = 3930,
    FLAG_UNK_0x0F5B = 3931,
    FLAG_UNK_0x0F5C = 3932,
    FLAG_UNK_0x0F5D = 3933,
    FLAG_UNK_0x0F5E = 3934,
    FLAG_UNK_0x0F5F = 3935,
    FLAG_UNK_0x0F60 = 3936,
    FLAG_UNK_0x0F61 = 3937,
    FLAG_UNK_0x0F62 = 3938,
    FLAG_UNK_0x0F63 = 3939,
    FLAG_UNK_0x0F64 = 3940,
    FLAG_UNK_0x0F65 = 3941,
    FLAG_UNK_0x0F66 = 3942,
    FLAG_UNK_0x0F67 = 3943,
    FLAG_UNK_0x0F68 = 3944,
    FLAG_UNK_0x0F69 = 3945,
    FLAG_UNK_0x0F6A = 3946,
    FLAG_UNK_0x0F6B = 3947,
    FLAG_UNK_0x0F6C = 3948,
    FLAG_UNK_0x0F6D = 3949,
    FLAG_UNK_0x0F6E = 3950,
    FLAG_UNK_0x0F6F = 3951,
    FLAG_UNK_0x0F70 = 3952,
    FLAG_UNK_0x0F71 = 3953,
    FLAG_UNK_0x0F72 = 3954,
    FLAG_UNK_0x0F73 = 3955,
    FLAG_UNK_0x0F74 = 3956,
    FLAG_UNK_0x0F75 = 3957,
    FLAG_UNK_0x0F76 = 3958,
    FLAG_UNK_0x0F77 = 3959,
    FLAG_UNK_0x0F78 = 3960,
    FLAG_UNK_0x0F79 = 3961,
    FLAG_UNK_0x0F7A = 3962,
    FLAG_UNK_0x0F7B = 3963,
    FLAG_UNK_0x0F7C = 3964,
    FLAG_UNK_0x0F7D = 3965,
    FLAG_UNK_0x0F7E = 3966,
    FLAG_UNK_0x0F7F = 3967,
    FLAG_UNK_0x0F80 = 3968,
    FLAG_UNK_0x0F81 = 3969,
    FLAG_UNK_0x0F82 = 3970,
    FLAG_UNK_0x0F83 = 3971,
    FLAG_UNK_0x0F84 = 3972,
    FLAG_UNK_0x0F85 = 3973,
    FLAG_UNK_0x0F86 = 3974,
    FLAG_UNK_0x0F87 = 3975,
    FLAG_UNK_0x0F88 = 3976,
    FLAG_UNK_0x0F89 = 3977,
    FLAG_UNK_0x0F8A = 3978,
    FLAG_UNK_0x0F8B = 3979,
    FLAG_UNK_0x0F8C = 3980,
    FLAG_UNK_0x0F8D = 3981,
    FLAG_UNK_0x0F8E = 3982,
    FLAG_UNK_0x0F8F = 3983,
    FLAG_UNK_0x0F90 = 3984,
    FLAG_UNK_0x0F91 = 3985,
    FLAG_UNK_0x0F92 = 3986,
    FLAG_UNK_0x0F93 = 3987,
    FLAG_UNK_0x0F94 = 3988,
    FLAG_UNK_0x0F95 = 3989,
    FLAG_UNK_0x0F96 = 3990,
    FLAG_UNK_0x0F97 = 3991,
    FLAG_UNK_0x0F98 = 3992,
    FLAG_UNK_0x0F99 = 3993,
    FLAG_UNK_0x0F9A = 3994,
    FLAG_UNK_0x0F9B = 3995,
    FLAG_UNK_0x0F9C = 3996,
    FLAG_UNK_0x0F9D = 3997,
    FLAG_UNK_0x0F9E = 3998,
    FLAG_UNK_0x0F9F = 3999,
    FLAG_UNK_0x0FA0 = 4000,
    FLAG_UNK_0x0FA1 = 4001,
    FLAG_UNK_0x0FA2 = 4002,
    FLAG_UNK_0x0FA3 = 4003,
    FLAG_UNK_0x0FA4 = 4004,
    FLAG_UNK_0x0FA5 = 4005,
    FLAG_UNK_0x0FA6 = 4006,
    FLAG_UNK_0x0FA7 = 4007,
    FLAG_UNK_0x0FA8 = 4008,
    FLAG_UNK_0x0FA9 = 4009,
    FLAG_UNK_0x0FAA = 4010,
    FLAG_UNK_0x0FAB = 4011,
    FLAG_UNK_0x0FAC = 4012,
    FLAG_UNK_0x0FAD = 4013,
    FLAG_UNK_0x0FAE = 4014,
    FLAG_UNK_0x0FAF = 4015,
    FLAG_UNK_0x0FB0 = 4016,
    FLAG_UNK_0x0FB1 = 4017,
    FLAG_UNK_0x0FB2 = 4018,
    FLAG_UNK_0x0FB3 = 4019,
    FLAG_UNK_0x0FB4 = 4020,
    FLAG_UNK_0x0FB5 = 4021,
    FLAG_UNK_0x0FB6 = 4022,
    FLAG_UNK_0x0FB7 = 4023,
    FLAG_UNK_0x0FB8 = 4024,
    FLAG_UNK_0x0FB9 = 4025,
    FLAG_UNK_0x0FBA = 4026,
    FLAG_UNK_0x0FBB = 4027,
    FLAG_UNK_0x0FBC = 4028,
    FLAG_UNK_0x0FBD = 4029,
    FLAG_UNK_0x0FBE = 4030,
    FLAG_UNK_0x0FBF = 4031,
    FLAG_UNK_0x0FC0 = 4032,
    FLAG_UNK_0x0FC1 = 4033,
    FLAG_UNK_0x0FC2 = 4034,
    FLAG_UNK_0x0FC3 = 4035,
    FLAG_UNK_0x0FC4 = 4036,
    FLAG_UNK_0x0FC5 = 4037,
    FLAG_UNK_0x0FC6 = 4038,
    FLAG_UNK_0x0FC7 = 4039,
    FLAG_UNK_0x0FC8 = 4040,
    FLAG_UNK_0x0FC9 = 4041,
    FLAG_UNK_0x0FCA = 4042,
    FLAG_UNK_0x0FCB = 4043,
    FLAG_UNK_0x0FCC = 4044,
    FLAG_UNK_0x0FCD = 4045,
    FLAG_UNK_0x0FCE = 4046,
    FLAG_UNK_0x0FCF = 4047,
    FLAG_UNK_0x0FD0 = 4048,
    FLAG_UNK_0x0FD1 = 4049,
    FLAG_UNK_0x0FD2 = 4050,
    FLAG_UNK_0x0FD3 = 4051,
    FLAG_UNK_0x0FD4 = 4052,
    FLAG_UNK_0x0FD5 = 4053,
    FLAG_UNK_0x0FD6 = 4054,
    FLAG_UNK_0x0FD7 = 4055,
    FLAG_UNK_0x0FD8 = 4056,
    FLAG_UNK_0x0FD9 = 4057,
    FLAG_UNK_0x0FDA = 4058,
    FLAG_UNK_0x0FDB = 4059,
    FLAG_UNK_0x0FDC = 4060,
    FLAG_UNK_0x0FDD = 4061,
    FLAG_UNK_0x0FDE = 4062,
    FLAG_UNK_0x0FDF = 4063,
    FLAG_UNK_0x0FE0 = 4064,
    FLAG_UNK_0x0FE1 = 4065,
    FLAG_UNK_0x0FE2 = 4066,
    FLAG_UNK_0x0FE3 = 4067,
    FLAG_UNK_0x0FE4 = 4068,
    FLAG_UNK_0x0FE5 = 4069,
    FLAG_UNK_0x0FE6 = 4070,
    FLAG_UNK_0x0FE7 = 4071,
    FLAG_UNK_0x0FE8 = 4072,
    FLAG_UNK_0x0FE9 = 4073,
    FLAG_UNK_0x0FEA = 4074,
    FLAG_UNK_0x0FEB = 4075,
    FLAG_UNK_0x0FEC = 4076,
    FLAG_UNK_0x0FED = 4077,
    FLAG_UNK_0x0FEE = 4078,
    FLAG_UNK_0x0FEF = 4079,
    FLAG_UNK_0x0FF0 = 4080,
    FLAG_UNK_0x0FF1 = 4081,
    FLAG_UNK_0x0FF2 = 4082,
    FLAG_UNK_0x0FF3 = 4083,
    FLAG_UNK_0x0FF4 = 4084,
    FLAG_UNK_0x0FF5 = 4085,
    FLAG_UNK_0x0FF6 = 4086,
    FLAG_UNK_0x0FF7 = 4087,
    FLAG_UNK_0x0FF8 = 4088,
    FLAG_UNK_0x0FF9 = 4089,
    FLAG_UNK_0x0FFA = 4090,
    FLAG_UNK_0x0FFB = 4091,
    FLAG_UNK_0x0FFC = 4092,
    FLAG_UNK_0x0FFD = 4093,
    FLAG_UNK_0x0FFE = 4094,
    FLAG_UNK_0x0FFF = 4095,
    VARS_START = 16384,
    MAP_LOCAL_VARS_START = 16384,
    VAR_MAP_LOCAL_0x00 = 16384,
    VAR_MAP_LOCAL_0x01 = 16385,
    VAR_MAP_LOCAL_0x02 = 16386,
    VAR_MAP_LOCAL_0x03 = 16387,
    VAR_MAP_LOCAL_0x04 = 16388,
    VAR_MAP_LOCAL_0x05 = 16389,
    VAR_MAP_LOCAL_0x06 = 16390,
    VAR_MAP_LOCAL_0x07 = 16391,
    VAR_MAP_LOCAL_0x08 = 16392,
    VAR_MAP_LOCAL_0x09 = 16393,
    VAR_MAP_LOCAL_0x0A = 16394,
    VAR_MAP_LOCAL_0x0B = 16395,
    VAR_MAP_LOCAL_0x0C = 16396,
    VAR_MAP_LOCAL_0x0D = 16397,
    VAR_MAP_LOCAL_0x0E = 16398,
    VAR_MAP_LOCAL_0x0F = 16399,
    VAR_MAP_LOCAL_0x10 = 16400,
    VAR_MAP_LOCAL_0x11 = 16401,
    VAR_MAP_LOCAL_0x12 = 16402,
    VAR_MAP_LOCAL_0x13 = 16403,
    VAR_MAP_LOCAL_0x14 = 16404,
    VAR_MAP_LOCAL_0x15 = 16405,
    VAR_MAP_LOCAL_0x16 = 16406,
    VAR_MAP_LOCAL_0x17 = 16407,
    VAR_MAP_LOCAL_0x18 = 16408,
    VAR_MAP_LOCAL_0x19 = 16409,
    VAR_MAP_LOCAL_0x1A = 16410,
    VAR_MAP_LOCAL_0x1B = 16411,
    VAR_MAP_LOCAL_0x1C = 16412,
    VAR_MAP_LOCAL_0x1D = 16413,
    VAR_MAP_LOCAL_0x1E = 16414,
    VAR_MAP_LOCAL_0x1F = 16415,
    MAP_LOCAL_VARS_END = 16415,
    OBJ_GFX_VARS_START = 16416,
    VAR_OBJ_GFX_ID_0 = 16416,
    VAR_OBJ_GFX_ID_1 = 16417,
    VAR_OBJ_GFX_ID_2 = 16418,
    VAR_OBJ_GFX_ID_3 = 16419,
    VAR_OBJ_GFX_ID_4 = 16420,
    VAR_OBJ_GFX_ID_5 = 16421,
    VAR_OBJ_GFX_ID_6 = 16422,
    VAR_OBJ_GFX_ID_7 = 16423,
    VAR_OBJ_GFX_ID_8 = 16424,
    VAR_OBJ_GFX_ID_9 = 16425,
    VAR_OBJ_GFX_ID_A = 16426,
    VAR_OBJ_GFX_ID_B = 16427,
    VAR_OBJ_GFX_ID_C = 16428,
    VAR_OBJ_GFX_ID_D = 16429,
    VAR_OBJ_GFX_ID_E = 16430,
    VAR_OBJ_GFX_ID_F = 16431,
    VAR_PLAYER_STARTER = 16432,
    VAR_UNUSED_0x4031 = 16433,
    VAR_RANDOM_INTERVIEW_QUESTION = 16434,
    VAR_VS_SEEKER_BATTERY_LEVEL = 16435,
    VAR_VS_SEEKER_STEP_COUNT = 16436,
    VAR_SIZE_CONTEST_RECORD = 16437,
    VAR_HIDDEN_LOCATION_FULL_MOON_ISLAND = 16438,
    VAR_HIDDEN_LOCATION_NEW_MOON_ISLAND = 16439,
    VAR_HIDDEN_LOCATION_SPRING_PATH = 16440,
    VAR_HIDDEN_LOCATION_SEABREAK_PATH = 16441,
    VAR_AMITY_SQUARE_STEP_COUNT = 16442,
    VAR_NEWS_PRESS_DEADLINE = 16443,
    VAR_LOTTERY_TRAINER_ID_LOW_HALF = 16444,
    VAR_LOTTERY_TRAINER_ID_HIGH_HALF = 16445,
    VAR_SPIRITOMB_COUNTER = 16446,
    VAR_PARTNER_TRAINER_ID = 16447,
    VAR_CONSECUTIVE_BONUS_ROUND_WINS = 16448,
    VAR_DAILY_RANDOM_LEVEL = 16449,
    VAR_DEPARTMENT_STORE_REGULAR_COUNTER = 16450,
    VAR_DISTRIBUTION_EVENT_DARKRAI = 16451,
    VAR_DISTRIBUTION_EVENT_SHAYMIN = 16452,
    VAR_DISTRIBUTION_EVENT_ARCEUS = 16453,
    VAR_DISTRIBUTION_EVENT_ROTOM = 16454,
    VAR_UNDERGROUND_FOSSILS_UNEARTHED = 16455,
    VAR_UNDERGROUND_TRAPS_SET = 16456,
    VAR_UNDERGROUND_TALK_COUNTER = 16457,
    VAR_FRIENDSHIP_INCREMENT_STEP_COUNTER = 16458,
    VAR_ETERNA_GYM_FLOWER_CLOCK_STATE = 16459,
    VAR_FIVE_MAID_KNOCKOUT_TURN_TARGET = 16460,
    VAR_TOTAL_TURNS_LAST_BATTLE = 16461,
    VAR_RESORT_VILLA_VISITOR = 16462,
    VAR_BATTLE_TOWER_PRINT_STATE = 16463,
    VAR_BATTLE_FACTORY_PRINT_STATE = 16464,
    VAR_BATTLE_HALL_PRINT_STATE = 16465,
    VAR_BATTLE_CASTLE_PRINT_STATE = 16466,
    VAR_BATTLE_ARCADE_PRINT_STATE = 16467,
    VAR_UNDERGROUND_ITEMS_GIVEN_AWAY = 16468,
    VAR_DISTORTION_WORLD_PROGRESS = 16469,
    VAR_ARCEUS_EVENT_STATE = 16470,
    VAR_SHAYMIN_EVENT_STATE = 16471,
    VAR_ROAMING_CRESSELIA_STATE = 16472,
    VAR_ROAMING_MESPRIT_STATE = 16473,
    VAR_RESORT_VILLA_VISITOR_MESSAGE_NUM = 16474,
    VAR_DISTORTION_WORLD_CYRUS_APPEARANCE = 16475,
    VAR_WIFI_PLAZA_RETURN_WARP_ID = 16476,
    VAR_WIFI_PLAZA_ATTENDANT_ID = 16477,
    VAR_ROAMING_MOLTRES_STATE = 16478,
    VAR_ROAMING_ZAPDOS_STATE = 16479,
    VAR_ROAMING_ARTICUNO_STATE = 16480,
    VAR_RANDOM_BATTLEGROUND_MESSAGE = 16481,
    VAR_BATTLE_HALL_NEXT_TOTAL_RECORD_MILESTONE = 16482,
    VAR_BATTLE_HALL_MAJOR_NPC = 16483,
    VAR_WIFI_FRONTIER_CLEARED = 16484,
    VAR_BATTLEGROUND_TRAINER_1 = 16485,
    VAR_BATTLEGROUND_TRAINER_2 = 16486,
    VAR_BATTLEGROUND_TRAINER_3 = 16487,
    VAR_BATTLEGROUND_TRAINER_4 = 16488,
    VAR_IRON_RUINS_STATE = 16489,
    VAR_ICEBERG_RUINS_STATE = 16490,
    VAR_ROCK_PEAK_RUINS_STATE = 16491,
    VAR_UNUSED_0x406C = 16492,
    VAR_UNUSED_0x406D = 16493,
    VAR_UNUSED_0x406E = 16494,
    VAR_UNUSED_0x406F = 16495,
    VAR_TWINLEAF_TOWN_GUITARIST_TRIGGER_STATE = 16496,
    VAR_SANDGEM_TOWN_STATE = 16497,
    VAR_UNK_0x4072 = 16498,
    VAR_SOLACEON_TOWN_STATE = 16499,
    VAR_CELESTIC_TOWN_STATE = 16500,
    VAR_BATTLE_FRONTIER_DUMMY_STATE = 16501,
    VAR_JUBILIFE_LOOKER_PAL_PAD_STATE = 16502,
    VAR_JUBILIFE_CITY_STATE = 16503,
    VAR_CANALAVE_CITY_STATE = 16504,
    VAR_OREBURGH_CITY_STATE = 16505,
    VAR_ETERNA_CITY_STATE = 16506,
    VAR_ROUTE_209_GATE_TO_HEARTHOME_CITY_STATE = 16507,
    VAR_PASTORIA_CITY_STATE = 16508,
    VAR_VEILSTONE_WAREHOUSE_GUARDS_FIGHTABLE = 16509,
    VAR_SUNYSHORE_CITY_STATE = 16510,
    VAR_SNOWPOINT_CITY_STATE = 16511,
    VAR_ENTERED_WIFI_PLAZA = 16512,
    VAR_FIGHT_AREA_STATE = 16513,
    VAR_VERITY_LAKEFRONT_STATE = 16514,
    VAR_DUMMY_0x4083 = 16515,
    VAR_ACUITY_LAKEFRONT_STATE = 16516,
    VAR_ROUTE_224_STATE = 16517,
    VAR_FOLLOWER_RIVAL_STATE = 16518,
    VAR_ROUTE_202_STATE = 16519,
    VAR_ROUTE_203_RIVAL_STATE = 16520,
    VAR_VALLEY_WINDWORKS_STATE = 16521,
    VAR_HALLOWED_TOWER_STATE = 16522,
    VAR_ROUTE_227_WAKE_RIVAL_STATE = 16523,
    VAR_ROUTE_207_COUNTERPART_TRIGGER_STATE = 16524,
    VAR_ROUTE_224_PROF_OAK_STATE = 16525,
    VAR_GALACTIC_HQ_HALL_STATE = 16526,
    VAR_UNUSED_0x408F = 16527,
    VAR_VICTORY_ROAD_1F_ROOM_2_FOLLOWER_MARLEY_STATE = 16528,
    VAR_WAYWARD_CAVE_1F_FOLLOWER_MIRA_STATE = 16529,
    VAR_IRON_ISLAND_B2F_LEFT_ROOM_FOLLOWER_RILEY_STATE = 16530,
    VAR_OREBURGH_GATE_1F_HIKER_STATE = 16531,
    VAR_STARK_MOUNTAIN_ROOM_2_FOLLOWER_BUCK_STATE = 16532,
    VAR_VISITED_LAKE_VERITY_WITH_RIVAL = 16533,
    VAR_MT_CORONET_1F_SOUTH_STATE = 16534,
    VAR_LAKE_VERITY_PROF_ROWAN_STATE = 16535,
    VAR_SPEAR_PILLAR_STATE = 16536,
    VAR_FOLLOWER_MON_ACTIVE = 16537,
    VAR_FOLLOWER_MON_SPECIES = 16538,
    VAR_FOLLOWER_MON_NEXT_PICKUP_TYPE = 16539,
    VAR_FOLLOWER_MON_PICKUP_ACCESSORY_ID = 16540,
    VAR_FOLLOWER_MON_PARTY_ID = 16541,
    VAR_STARK_MOUNTAIN_ROOM_3_STATE = 16542,
    VAR_FOLLOWER_MON_PICKUP_ITEM_ID = 16543,
    VAR_STARK_MOUNTAIN_OUTSIDE_STATE = 16544,
    VAR_VILLA_STATE = 16545,
    VAR_UNUSED_0x40A2 = 16546,
    VAR_RIVAL_HOUSE_STATE = 16547,
    VAR_PLAYER_HOUSE_STATE = 16548,
    VAR_PLAYER_HOUSE_RIVAL_STATE = 16549,
    VAR_SANDGEM_TOWN_LAB_STATE = 16550,
    VAR_RESORT_AREA_RIBBON_SYNDICATE_1F_STATE = 16551,
    VAR_ROUTE_217_STATE = 16552,
    VAR_MT_CORONET_2F_STATE = 16553,
    VAR_EXITED_DISTORTION_WORLD_STATE = 16554,
    VAR_AMITY_SQUARE_GIFT_ID = 16555,
    VAR_AMITY_SQUARE_STATE = 16556,
    VAR_STARK_MOUNTAIN_ROOM_1_STATE = 16557,
    VAR_ETERNA_GYM_TRAINERS_BEATEN = 16558,
    VAR_UNUSED_0x40AF = 16559,
    VAR_UNUSED_0x40B0 = 16560,
    VAR_ETERNA_FOREST_FOLLOWER_CHERYL_STATE = 16561,
    VAR_CANALAVE_LIBRARY_STATE = 16562,
    VAR_UNUSED_0x40B3 = 16563,
    VAR_REVIVED_POKEMON_SPECIES = 16564,
    VAR_SUNYSHORE_CITY_EAST_HOUSE_SCIENTIST_STATE = 16565,
    VAR_CURRENT_UNDERGROUND_MISSION = 16566,
    VAR_BATTLE_FACTORY_LOBBY_LOAD_ACTION = 16567,
    VAR_BATTLE_FACTORY_CHALLENGE_TYPE = 16568,
    VAR_BATTLE_FACTORY_CHALLENGE_LEVEL = 16569,
    VAR_BATTLE_HALL_LOBBY_LOAD_ACTION = 16570,
    VAR_BATTLE_HALL_CHALLENGE_TYPE = 16571,
    VAR_BATTLE_CASTLE_LOBBY_LOAD_ACTION = 16572,
    VAR_BATTLE_CASTLE_CHALLENGE_TYPE = 16573,
    VAR_BATTLE_ARCADE_CHALLENGE_TYPE = 16574,
    VAR_BATTLE_ARCADE_LOBBY_LOAD_ACTION = 16575,
    VAR_UNUSED_0x40C0 = 16576,
    VAR_BATTLEGROUND_STATE = 16577,
    VAR_RANDOM_SURVIVAL_AREA_RIVAL_MESSAGE = 16578,
    VAR_SPEAR_PILLAR_DISTORTED_STATE = 16579,
    VAR_SPEAR_PILLAR_DIALGA_STATE = 16580,
    VAR_SPEAR_PILLAR_PALKIA_STATE = 16581,
    VAR_PAL_PARK_LOBBY_STATE = 16582,
    VAR_PASTORIA_CITY_CROAGUNK_SCENE_STATE = 16583,
    VAR_RESORT_AREA_STATE = 16584,
    VAR_UNUSED_0x40C9 = 16585,
    VAR_FOLLOWER_RIVAL_TIMES_TALKED = 16586,
    VAR_FOLLOWER_UNUSED_TIMES_TALKED = 16587,
    VAR_ONGOING_CONTEST = 16588,
    VAR_ETERNA_FOREST_CHERYL_OLD_CHATEAU_CUTSCENE_STATE = 16589,
    VAR_ELEVATOR_FLOORS_ABOVE = 16590,
    VAR_VALLEY_WINDWORKS_TEAM_GALACTIC_STATE = 16591,
    VAR_HAS_SEEN_UNDERGROUND_ROARK_INTRO = 16592,
    VAR_HAS_ENTERED_HEARTHOME_GYM_BEFORE = 16593,
    VAR_UNUSED_0x40D2 = 16594,
    VAR_LAKE_ACUITY_STATE = 16595,
    VAR_VISITED_POKEMON_CENTER_B1F = 16596,
    VAR_COMMUNICATION_LOAD_ACTION = 16597,
    VAR_GALACTIC_HQ_4F_STATE = 16598,
    VAR_PASTORIA_CITY_OBSERVATORY_GATE_1F_STATE = 16599,
    VAR_BATTLE_TOWER_LOBBY_LOAD_ACTION = 16600,
    VAR_BATTLE_TOWER_BATTLE_ROOM_LOAD_ACTION = 16601,
    VAR_SAFARI_GAME_STATE = 16602,
    VAR_BATTLE_TOWER_ELEVATOR_LOAD_ACTION = 16603,
    VAR_BATTLE_TOWER_CORRIDOR_LOAD_ACTION = 16604,
    VAR_BATTLE_TOWER_CORRIDOR_MULTI_LOAD_ACTION = 16605,
    VAR_BATTLE_TOWER_MULTI_BATTLE_ROOM_LOAD_ACTION = 16606,
    VAR_BATTLE_TOWER_BATTLE_SALON_STATE = 16607,
    VAR_CATCHING_SHOW_RECORD = 16608,
    VAR_FOLLOWER_BUCK_TIMES_TALKED = 16609,
    VAR_FOLLOWER_MIRA_TIMES_TALKED = 16610,
    VAR_FOLLOWER_RILEY_TIMES_TALKED = 16611,
    VAR_FOLLOWER_MARLEY_TIMES_TALKED = 16612,
    VAR_POKEMON_NEWS_PRESS_REQUESTED_POKEMON = 16613,
    VAR_TWINLEAF_TOWN_RIVAL_TRIGGER_STATE = 16614,
    VAR_POKETCH_CAMPAIGN_STATE = 16615,
    VAR_FOLLOWER_CHERYL_TIMES_TALKED = 16616,
    VAR_FLOAROMA_MEADOW_STATE = 16617,
    VAR_UNUSED_0x40EA = 16618,
    VAR_UNUSED_0x40EB = 16619,
    VAR_UNUSED_0x40EC = 16620,
    VAR_AVAILABLE_MYSTERY_GIFT_EXISTS = 16621,
    VAR_UNUSED_0x40EE = 16622,
    VAR_RIVAL_BEAT_SUNYSHORE_GYM = 16623,
    VAR_GTS_ACCESS_STATE = 16624,
    VAR_CELESTIC_TOWN_ELDER_STATE = 16625,
    VAR_LOTTERY_PRIZE_ITEM = 16626,
    VAR_PAL_PARK_STATE = 16627,
    VAR_DUMMY_0x40F4 = 16628,
    VAR_VEILSTONE_CITY_CRASHER_WAKE_STATE = 16629,
    VAR_HEARTHOME_CITY_STATE = 16630,
    VAR_CONTEST_HALL_LOBBY_STATE = 16631,
    VAR_DARKRAI_EVENT_STATE = 16632,
    VAR_PLAYER_HOUSE_SPECIAL_PROGRAM_STATE = 16633,
    VAR_UNUSED_0x40FA = 16634,
    VAR_RESTAURANT_RANDOM_MESSAGE_ISMAEL_HARLEY = 16635,
    VAR_RESTAURANT_RANDOM_MESSAGE_ROMAN_KYLIE = 16636,
    VAR_RESTAURANT_RANDOM_MESSAGE_LEONARDO_REBECCA = 16637,
    VAR_RESTAURANT_RANDOM_MESSAGE_EUGENE_ALISON = 16638,
    VAR_RESTAURANT_RANDOM_MESSAGE_ESTEBAN_MEREDITH = 16639,
    VAR_RESTAURANT_RANDOM_MESSAGE_EMANUEL_BLYTHE = 16640,
    VAR_RESTAURANT_RANDOM_MESSAGE_DARRYL_VALERIE = 16641,
    VAR_RESTAURANT_RANDOM_MESSAGE_KENDRICK_GABRIELLA = 16642,
    VAR_RESTAURANT_RANDOM_MESSAGE_EMILIO_KAYLEE = 16643,
    VAR_RESTAURANT_STATE = 16644,
    VAR_ROUTE_218_GATE_TO_CANALAVE_CITY_STATE = 16645,
    VAR_LUNAR_WING_EVENT_STATE = 16646,
    VAR_VEILSTONE_STORE_1F_ACCESSORY_STARTER_MASK = 16647,
    VAR_ROUTE_221_HOUSE_REWARD_INDEX = 16648,
    VAR_UNUSED_0x4109 = 16649,
    VAR_JUBILIFE_TV_2F_ACCESSORY_STARTER_MASK = 16650,
    VAR_TURNBACK_CAVE_PILLARS_SEEN = 16651,
    VAR_TURNBACK_CAVE_ROOMS_VISITED = 16652,
    VAR_GALACTIC_HQ_CONTROL_ROOM_STATE = 16653,
    VAR_BATTLE_PARK_STATE = 16654,
    VAR_PLAYER_HOUSE_POSTGAME_STATE = 16655,
    VAR_OLD_CHATEAU_DINING_AREA_OLD_MAN_STATE = 16656,
    VAR_OLD_CHATEAU_BACK_MIDDLE_EAST_ROOM_TWIN_STATE = 16657,
    VAR_VALOR_LAKEFRONT_BLOCK_SUNYSHORE_STATE = 16658,
    VAR_BATTLE_FRONTIER_GATE_TO_FIGHT_AREA_STATE = 16659,
    VAR_ETERNA_CITY_BLOCK_EXITS_STATE = 16660,
    VAR_NUM_PLATES_OBTAINED = 16661,
    VAR_POKEMON_MANSION_OFFICE_BLOCK_STATUE_STATE = 16662,
    VAR_POKEMON_NEWS_PRESS_POKE_BALL_REWARD = 16663,
    VAR_HALL_OF_ORIGIN_STATE = 16664,
    VAR_ROUTE_227_BUCK_STATE = 16665,
    VAR_VEILSTONE_CITY_COUNTERPART_NEEDS_HELP_STATE = 16666,
    VAR_ROTOMS_ROOM_STATE = 16667,
    VAR_UNUSED_0x411C = 16668,
    VAR_TEAM_GALACTIC_ETERNA_BUILDING_1F_STATE = 16669,
    VAR_VALLEY_WINDWORKS_LOOKER_STATE = 16670,
    VAR_VEILSTONE_CITY_GALACTIC_WAREHOUSE_STATE = 16671,
    VARS_END = 16672,
    SCRIPT_LOCAL_VARS_START = 32768,
    VAR_0x8000 = 32768,
    VAR_0x8001 = 32769,
    VAR_0x8002 = 32770,
    VAR_0x8003 = 32771,
    VAR_0x8004 = 32772,
    VAR_0x8005 = 32773,
    VAR_0x8006 = 32774,
    VAR_0x8007 = 32775,
    VAR_0x8008 = 32776,
    VAR_0x8009 = 32777,
    VAR_0x800A = 32778,
    VAR_0x800B = 32779,
    VAR_0x800C = 32780,
    VAR_RESULT = 32780,
    VAR_0x800D = 32781,
    VAR_LAST_TALKED = 32781,
    SCRIPT_LOCAL_VARS_END = 32781,
    FR_VAR_0x800E = 32782,
    FR_VAR_0x800F = 32783,
    FR_VAR_0x8010 = 32784,
};

typedef struct SaveVarsFlags {
    u16 vars[(VARS_END - VARS_START)];
    u8 flags[2912 / 8];
} SaveVarsFlags;

SaveVarsFlags *Save_VarsFlags_Get(SaveData *saveData);

u16 Save_VarsFlags_GetVar404C(SaveVarsFlags *varsFlags);

void Save_VarsFlags_SetVar404C(SaveVarsFlags *varsFlags, u16 state);

void ov70_02240CA0(GlobalTrade *globalTrade, int type);

void ov70_02240A7C(GTSApplicationState *appState, Pokemon *mon, int boxIndex, int exchangedFromRemote);

void ov70_02240A7C(GTSApplicationState *appState, Pokemon *mon, int boxIndex, int exchangedFromRemote) {
    int unused = GetMonData(mon, MON_DATA_HELD_ITEM, 0L);

    UpdatePokedexWithReceivedSpecies(appState->playerData->saveData, mon);
    boxIndex = 18;

    if (Party_GetCount(appState->playerData->party) == 6) {
        boxIndex = 0;
    }

    if (exchangedFromRemote) {
        u8 friendship = 70;

        if (GetMonData(mon, MON_DATA_SPECIES, 0L) == SPECIES_ARCEUS) {
            if (GetMonData(mon, MON_DATA_FATEFUL_ENCOUNTER, 0L) || ((GetMonData(mon, MON_DATA_MET_LOCATION, 0L) == 86) && (GetMonData(mon, MON_DATA_FATEFUL_ENCOUNTER, 0L) == 0))) {
                SaveVarsFlags *varsFlags = Save_VarsFlags_Get(appState->playerData->saveData);

                if (Save_VarsFlags_GetVar404C(varsFlags) == 0) {
                    Save_VarsFlags_SetVar404C(varsFlags, 1);
                }
            }
        }

        SetMonData(mon, MON_DATA_FRIENDSHIP, &friendship);
        SetMonData(mon, MON_DATA_GENDER, 0L);

        ov70_02240CA0(appState->playerData->globalTrade, 0);
    }

    if (boxIndex == 18) {
        Party_AddMon(appState->playerData->party, mon);
        int partyIndex = Party_GetCount(appState->playerData->party);

        appState->tradedPokemonLocation.boxIndex = 18;
        appState->tradedPokemonLocation.index = partyIndex - 1;
    } else {
        int boxPosition = 0;

        PCStorage_FindFirstEmptySlot(appState->playerData->pcBoxes, &boxIndex, &boxPosition);
        PCStorage_PlaceMonInBoxFirstEmptySlot(appState->playerData->pcBoxes, boxIndex, Mon_GetBoxMon(mon));

        appState->tradedPokemonLocation.boxIndex = boxIndex;
        appState->tradedPokemonLocation.index = boxPosition;
    }

    sub_0202DB5C(appState->playerData->globalTrade, 0);
}

void ov70_02240B9C(GTSApplicationState *appState, Pokemon *mon, int boxIndex);

void ov70_02240B9C(GTSApplicationState *appState, Pokemon *mon, int boxIndex) {
    UpdatePokedexWithReceivedSpecies(appState->playerData->saveData, mon);

    boxIndex = 18;

    if (Party_GetCount(appState->playerData->party) == 6) {
        boxIndex = 0;
    }

    if (GetMonData(mon, MON_DATA_SPECIES, 0L) == SPECIES_ARCEUS) {
        if (GetMonData(mon, MON_DATA_FATEFUL_ENCOUNTER, 0L) || ((GetMonData(mon, MON_DATA_MET_LOCATION, 0L) == 86) && (GetMonData(mon, MON_DATA_FATEFUL_ENCOUNTER, 0L) == 0))) {
            SaveVarsFlags *varsFlags = Save_VarsFlags_Get(appState->playerData->saveData);

            if (Save_VarsFlags_GetVar404C(varsFlags) == 0) {
                Save_VarsFlags_SetVar404C(varsFlags, 1);
            }
        }
    }

    u8 friendship = 70;
    SetMonData(mon, MON_DATA_FRIENDSHIP, &friendship);

    SetMonData(mon, MON_DATA_GENDER, 0L);

    if (boxIndex == 18) {
        Party_AddMon(appState->playerData->party, mon);
        int partyIndex = Party_GetCount(appState->playerData->party);

        appState->tradedPokemonLocation.boxIndex = 18;
        appState->tradedPokemonLocation.index = partyIndex - 1;
    } else {
        int boxPosition = 0;

        PCStorage_FindFirstEmptySlot(appState->playerData->pcBoxes, &boxIndex, &boxPosition);
        PCStorage_PlaceMonInBoxFirstEmptySlot(appState->playerData->pcBoxes, boxIndex, Mon_GetBoxMon(mon));

        appState->tradedPokemonLocation.boxIndex = boxIndex;
        appState->tradedPokemonLocation.index = boxPosition;
    }

    ov70_02240CA0(appState->playerData->globalTrade, 1);
}
