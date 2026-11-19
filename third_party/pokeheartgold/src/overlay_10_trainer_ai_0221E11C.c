#include "global.h"

#include "battle/battler_sprite_context.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"

struct BallRotation;

struct BattleAnimSystem;

struct BattleContext;

struct BattleSystem;

struct CustomBgScrollContext;

struct TextOBJ;

struct PaletteAnimator;

struct PokemonAnim;

struct Pokepic;

struct PokemonSpriteTransforms;

struct SPLEmitter;

struct SPLParticle;

struct SpriteList;

struct SysTask;

struct SysTaskManager;

struct TransformData;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct WaitingIcon;

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

enum BattleAction {
    BATTLE_ACTION_PICK_COMMAND = 0,
    BATTLE_ACTION_CHOOSE_TARGET = 1,
    BATTLE_ACTION_TEMP_VALUE = 2,
    BATTLE_ACTION_SELECTED_COMMAND = 3,
    MAX_BATTLE_ACTIONS = 4,
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

enum ItemBattleCategory {
    ITEM_BATTLE_CATEGORY_RECOVER_HP = 0,
    ITEM_BATTLE_CATEGORY_RECOVER_STATUS = 1,
    ITEM_BATTLE_CATEGORY_POKE_BALLS = 2,
    ITEM_BATTLE_CATEGORY_BATTLE_ITEMS = 3,
    ITEM_BATTLE_CATEGORY_MAX = 4,
};

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

typedef struct BattleSystem BattleSystem;

enum TimeOfDay {
    TIMEOFDAY_MORNING = 0,
    TIMEOFDAY_DAY = 1,
    TIMEOFDAY_TWILIGHT = 2,
    TIMEOFDAY_NIGHT = 3,
    TIMEOFDAY_LATE_NIGHT = 4,
};

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

typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;

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

typedef struct ManagedSpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animIdx;
    int priority;
    int plttIdx;
    enum NNS_G2D_VRAM_TYPE vramType;
    int resources[GF_GFX_RES_TYPE_MAX];
    int bgPriority;
    BOOL vramTransfer;
} ManagedSpriteTemplate;

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



typedef struct OpponentData OpponentData;

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

typedef struct String {
    u16 maxsize;
    u16 size;
    u32 magic;
    u16 data[0];
} String;

typedef struct MailMessage {
    u16 type;
    u16 id;
    u16 words[2];
} MailMessage;

typedef struct Trainer {
    TrainerHeader header;
    charcode_t name[7 + 1];
    MailMessage winMsg;
    MailMessage loseMsg;
} Trainer;

typedef struct TextOBJ TextOBJ;

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

typedef struct ChatotCry {
    BOOL valid;
    s8 audioData[1000];
} ChatotCry;

typedef struct WiFiHistory {
    u32 unused_00;
    u8 hasInteractedOutsideOfJapan;
    u8 country;
    u8 region;
    u8 geonetCommunicationMap[4080];
} WiFiHistory;

typedef struct SaveData SaveData;

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

typedef struct BagCursor {
    FieldBagCursor field;
    BattleBagCursor battle;
} BagCursor;

typedef struct Options {
    u16 textSpeed : 4;
    u16 soundMode : 2;
    u16 battleStyle : 1;
    u16 battleScene : 1;
    u16 buttonMode : 2;
    u16 frame : 5;
    u16 : 1;
} Options;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef struct BallCapsule {
    BallSeal seals[8];
} BallCapsule;

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

typedef struct PCBoxes {
    u32 currentBoxID;
    BoxPokemon boxMons[18][(5 * 6)];
    u16 names[18][20];
    u8 wallpapers[18];
    u8 unlockedWallpapers;
} PCBoxes;

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

typedef struct FieldBattleDTO {
    u32 battleType;
    Party *parties[4];
    int resultMask;
    int trainerIDs[4];
    Trainer trainer[4];
    PlayerProfile *trainerInfo[4];
    Bag *bag;
    BagCursor *bagCursor;
    Pokedex *pokedex;
    PCBoxes *pcBoxes;
    ChatotCry *chatotCries[4];
    Poketch *poketch;
    WiFiHistory *wiFiHistory;
    Options *options;
    CaptureAttempt *captureAttempt;
    BattleRecords battleRecords;
    GameStats *records;
    JournalEntry *journalEntry;
    PalPad *palPad;
    enum BattleBackground background;
    enum BattleTerrain terrain;
    int mapLabelTextID;
    int mapHeaderID;
    enum TimeOfDay timeOfDay;
    enum EvolutionMethod mapEvolutionMethod;
    BOOL visitedContestHall;
    BOOL metBebe;
    int caughtBattlerIdx;
    int fieldWeather;
    int leveledUpMonsMask;
    u32 systemVersion[4];
    u32 battleStatusMask;
    int countSafariBalls;
    u32 rulesetMask;
    void *postBattleData;
    u32 seed;
    int linkPlayerPositions[4];
    u16 networkID;
    u8 recordingType;
    u8 dummy18B;
    int totalTurnsElapsed;
    u8 *subscreenCursorOn;
    u8 recordedChatter[4];
    SaveData *saveData;
    u32 recordingStopped;
} FieldBattleDTO;

typedef struct TrainerIntroData {
    FieldBattleDTO *dto;
    Party *party[4];
    String *trainerNames[4];
    enum HeapID heapID;
    u8 mode;
    u8 playerSide;
    u8 battleResult;
    u8 isDone;
    u8 recordingType;
} TrainerIntroData;

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;

struct BattleCursor;

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

enum PartyGaugeSide {
    PARTY_GAUGE_OURS,
    PARTY_GAUGE_THEIRS,
    PARTY_GAUGE_COUNT
};

enum HidePartyGaugeType {
    HIDE_PARTY_GAUGE_BATTLE_START,
    HIDE_PARTY_GAUGE_MID_BATTLE,
};

enum PartyGaugePosition {
    PARTY_GAUGE_POSITION_HIGH,
    PARTY_GAUGE_POSITION_MIDDLE,
    PARTY_GAUGE_POSITION_LOW,
};

enum HideArrowType {
    HIDE_ARROW_FADE_AND_SCROLL,
    HIDE_ARROW_FADE_IN_PLACE,
};

typedef struct {
    ManagedSprite *managedSprite;
    SysTask *task;
    enum HideArrowType hideType;
    enum PartyGaugeSide side;
    enum PartyGaugePosition position;
    s32 x;
    s16 alpha;
    u8 state;
    u8 delay;
} PartyGaugeArrow;

typedef struct {
    ManagedSprite *managedSprite;
    SysTask *task;
    enum PartyGaugeSide side;
    enum PartyGaugePosition position;
    enum HidePartyGaugeType hideType;
    s8 *pokeballCount;
    s16 *arrowAlpha;
    s32 xStart;
    s32 xEnd;
    s32 xOverflow;
    s16 delay;
    u16 sdatID;
    u8 state;
    u8 ballSlot;
    u8 flipAnimation;
    u8 startDelay;
} PartyGaugePokeballs;

typedef struct PartyGauge {
    PartyGaugeArrow arrow;
    PartyGaugePokeballs pokeballs[6];
    s8 pokeballCount;
} PartyGauge;

typedef struct BattleBackgroundReference {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
} BattleBackgroundReference;

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

typedef struct BattleAnimSystem {
    enum HeapID heapID;
    enum NarcId moveArcID;
    BOOL isContest;
    BOOL isActive;
    BOOL moveActive;
    void *scriptData;
    u32 *scriptPtr;
    u32 *callStack[3];
    BattleAnimScriptLoop loopStack[3];
    PokemonSpriteDrawContext pokemonSpriteDrawContexts[2];
    u8 cameraProjections[16];
    u8 particleSystemCameraFlip[16];
    s8 unused_88;
    u8 scriptDelay;
    u16 activeAnimTasks;
    u16 activeSoundTasks;
    u8 pad_92[2];
    s32 scriptVars[10];
    void *executeAnimScriptFunc;
    BattleAnimContext *context;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    SpriteManager *spriteManagers[4];
    ManagedSprite *sprites[10];
    ManagedSpriteTemplate lastSpriteTemplate;
    SpriteManager *pokemonSpriteManager;
    ManagedSprite *pokemonSprites[5];
    BOOL usedPokemonSprites[5];
    SpriteTrackingTask *spriteTrackingTasks[5];
    BgTrackingTask *bgTrackingTask;
    u8 bgSwitchState;
    u8 soundEffectWaitTimer;
    u8 endDelayFrames;
    u8 unused_17B;
    BattleBgAnim *bgAnim;
    BattleBackgroundReference battleBgRefs;
    int baseBgPalettes;
    u8 bgLayerPriorities[4];
    NARC *arcs[BATTLE_ANIM_SYSTEM_ARC_COUNT];
    u8 pad_1BC[12];
} BattleAnimSystem;

typedef struct FontSpecialCharsContext {
    void *ncgrBuffer;
    NNSG2dCharacterData *charData;
    charcode_t charcodes[16];
    u32 bgColor;
} FontSpecialCharsContext;

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

typedef struct MsgData {
    u16 mode;
    u16 heapID;
    u16 narcID;
    u16 bankID;

    union {
        MessageBank *bank;
        NARC *narc;
    };
} MsgData;

typedef struct WaitingIcon WaitingIcon;

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

struct BattleSystem {
    G3DPipelineBuffers *unk0;
    BgConfig *bgConfig;
    Window *window;
    MsgData *msgData;
    MsgData *unk10;
    MessageFormat *msgFormat;
    String *msgBuffer;
    SysTask *unk1C;
    SysTask *unk20;
    SysTask *unk24;
    PaletteData *palette;
    u32 battleType;
    BattleContext *ctx;
    OpponentData *opponentData[4];
    int maxBattlers;
    PlayerProfile *playerProfile[4];
    Bag *bag;
    BagCursor *bagCursor;
    Pokedex *pokedex;
    PCBoxes *storage;
    Party *trainerParty[4];
    ChatotCry *chatotVoice[4];
    PokepicManager *pokepicManager;
    BattleAnimSystem *unk8C;
    SpriteSystem *spriteRenderer;
    SpriteManager *gfxHandler;
    Poketch *unk98;
    CaptureAttempt *unk9C;
    u16 trainerId[4];
    u8 trainerGender[4];
    Trainer trainers[4];
    struct { ManagedSprite *unk0; BattleSystem *battleSystem; u8 unk8; u8 unk9; s16 unkA; s16 unkC; u16 unused; } unk17C[2];
    BattleSubscreen *battleInput;
    PartyGauge *unk1A0[2];
    FontSpecialCharsContext *hpNumPrinter;
    FontSpecialCharsContext *levelNumPrinter;
    PaletteAnimator *msgIcon;
    Options *options;
    PalPad *unk1B8;
    WaitingIcon *unk1BC;
    u8 *unk1C0;
    TrainerIntroData *unk1C4;
    PokemonAnimManager *unk1C8;
    NNSG2dCellTransferState *unk1CC;
    PokemonSpriteData unk1D0[4];
    BattleRecords unk210;
    GameStats *gameStats;
    u8 *unk220;
    u16 *unk224;
    u8 sendBuffer[4096];
    u8 recvBuffer[4096];
    u16 unk2228[112];
    u16 unk2308[112];
    u16 unk23E8;
    u16 unk23EA;
    u16 unk23EC;
    u16 unk23EE;
    u16 unk23F0;
    u16 unk23F2;
    u8 *unk23F4;
    u8 *unk23F8;
    u8 unk23FC;
    u8 unk23FD;
    u8 unk23FE;
    u8 unk240F_0 : 1;
    u8 unk240F_1 : 1;
    u8 unk240E_F : 1;
    u8 criticalHpMusic : 2;
    u8 criticalHpMusicDelay : 3;
    enum BattleTerrain terrain;
    int backgroundId;
    int location;
    u32 battleSpecial;
    int timezone;
    int safariBallCnt;
    u8 unk2418[4];
    u32 unk241C;
    u8 battleOutcomeFlag;
    u8 unk2421;
    u16 unk2422;
    int unk2424;
    int unk2428;
    int weather;
    int metBill;
    u32 unk2434;
    int unk2438;
    int unk243C;
    int unk2440;
    u8 unk2442;
    u8 unk2445;
    u16 unk2446;
    u32 rand;
    u32 randTemp;
    u16 unk244C[4];
    u16 unk2454[4];
    u16 unk245C[4];
    int unk2464[4];
    u32 isRecordingPaused : 1;
    u32 unk2474_1 : 1;
    u32 unk2474_2 : 1;
    u32 isFishing : 1;
    u32 unk2474_4 : 28;
    u32 unk2478;
    SysTask *unk247C;
    u8 chatotVoiceParam[4];
    Pokemon *bugContestCaughtMon;
    u8 unk248C[4];
};

int GetBattlerLearnedMoveCount(BattleSystem *battleSys, BattleContext *battleCtx, int battler);

int ov10_0221EEF0(BattleContext *battleCtx);

void ov10_0221EF24(BattleContext *battleCtx, int i);

u8 ov10_0221EF34(BattleContext *battleCtx, u8 inBattler);

void ov10_0221E11C(BattleSystem *battleSys, BattleContext *battleCtx);

u8 CheckSortSpeed(BattleSystem *battleSys, BattleContext *battleCtx, int battler1, int battler2, BOOL ignoreQuickClaw);
int BattleSystem_GetMaxBattlers(BattleSystem *battleSys);
void ov10_0221E178(BattleSystem *battleSys, BattleContext *battleCtx);
void ov10_0221E19C(BattleSystem *battleSys, BattleContext *battleCtx);
void ov10_0221E1CC(BattleSystem *battleSys, BattleContext *battleCtx);

void ov10_0221E290(BattleSystem *battleSys, BattleContext *battleCtx);

void ov10_0221E11C(BattleSystem *battleSys, BattleContext *battleCtx) {
    ov10_0221EF24(battleCtx, 1);

    int inBattler = ov10_0221EEF0(battleCtx);
    int jump = ov10_0221EEF0(battleCtx);
    u8 battler = ov10_0221EF34(battleCtx, inBattler);
    int numKnownMoves = GetBattlerLearnedMoveCount(battleSys, battleCtx, battler);

    if (battleCtx->battleMons[battler].unk88.lastResortCount >= (numKnownMoves - 1) && numKnownMoves > 1) {
        ov10_0221EF24(battleCtx, jump);
    }
}

void ov10_0221E178(BattleSystem *battleSys, BattleContext *battleCtx) {
    ov10_0221EF24(battleCtx, 1);
    (battleCtx->aiContext).calcTemp = (battleCtx->aiContext.moveTable[(battleCtx->aiContext).move]).class;
}

void ov10_0221E19C(BattleSystem *battleSys, BattleContext *battleCtx) {
    ov10_0221EF24(battleCtx, 1);
    (battleCtx->aiContext).calcTemp = (battleCtx->aiContext.moveTable[battleCtx->movePrevByBattler[(battleCtx->aiContext).defender]]).class;
}

void ov10_0221E1CC(BattleSystem *battleSys, BattleContext *battleCtx) {

    int i, j;
    int speedOrder[4];
    int cmp1, cmp2;
    int maxBattlers;
    int battler;
    int inBattler;

    ov10_0221EF24(battleCtx, 1);

    inBattler = ov10_0221EEF0(battleCtx);
    battler = ov10_0221EF34(battleCtx, inBattler);
    maxBattlers = BattleSystem_GetMaxBattlers(battleSys);

    for (i = 0; i < maxBattlers; i++) {
        speedOrder[i] = i;
    }

    for (i = 0; i < maxBattlers - 1; i++) {
        for (j = i + 1; j < maxBattlers; j++) {
            cmp1 = speedOrder[i];
            cmp2 = speedOrder[j];

            if (CheckSortSpeed(battleSys, battleCtx, cmp1, cmp2, 1)) {
                speedOrder[i] = cmp2;
                speedOrder[j] = cmp1;
            }
        }
    }

    for (i = 0; i < maxBattlers; i++) {
        if (speedOrder[i] == battler) {
            (battleCtx->aiContext).calcTemp = i;
            break;
        }
    }
}

void ov10_0221E290(BattleSystem *battleSys, BattleContext *battleCtx) {
    ov10_0221EF24(battleCtx, 1);

    int inBattler = ov10_0221EEF0(battleCtx);
    int battler = ov10_0221EF34(battleCtx, inBattler);

    (battleCtx->aiContext).calcTemp = battleCtx->totalTurns - battleCtx->battleMons[battler].unk88.fakeOutTurnNumber;
}
