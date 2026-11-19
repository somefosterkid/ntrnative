#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "camera.h"
#include "error_handling.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_89.h"

struct UnkStruct_ov113_0225DBCC_t;

struct UnkStruct_ov113_0225E6B8_t;

typedef struct UnkStruct_ov113_0225DBCC_t UnkStruct_ov113_0225DBCC;

typedef struct UnkStruct_ov113_0225E6B8_t UnkStruct_ov113_0225E6B8;

typedef struct {
    fx32 unk_00;
    s16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov113_0225FE30;

typedef struct {
    VecFx32 unk_00;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0225FEE0;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0225FF8C;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_02260064;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0226013C;

typedef struct {
    CameraAngle cameraAngle;
    fx32 unk_08;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E[2];
} UnkStruct_ov113_02260218;

typedef struct {
    fx32 unk_00;
    s16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov113_022602E4;

typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    fx32 unk_18;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E[2];
} UnkStruct_ov113_02260394;

typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    fx32 unk_18;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E[2];
} UnkStruct_ov113_0226046C;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_02260544;

typedef struct {
    UnkStruct_ov113_0225FE30 unk_00;
    UnkStruct_ov113_0225FEE0 unk_08;
    UnkStruct_ov113_0225FF8C unk_18;
    UnkStruct_ov113_02260064 unk_28;
    UnkStruct_ov113_0226013C unk_38;
    UnkStruct_ov113_02260218 unk_48;
    UnkStruct_ov113_022602E4 unk_58;
    UnkStruct_ov113_02260394 unk_60;
    UnkStruct_ov113_0226046C unk_80;
    UnkStruct_ov113_02260544 unk_A0;
} UnkStruct_ov113_0225EB20_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
} UnkStruct_ov113_0225EDCC;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
} UnkStruct_ov113_0225FD80;

typedef struct {
    UnkStruct_ov113_0225EB20_sub1 unk_00;
    UnkStruct_ov113_0225FD80 unk_B0;
    UnkStruct_ov113_0225E6B8 *unk_B4[128];
    UnkStruct_ov113_0225E6B8 *unk_2B4[128];
    NNSG3dResTex *unk_4B4[32];
    u32 unk_534[2];
    UnkStruct_ov113_0225EDCC unk_53C[128];
    u8 unk_73C;
    u8 unk_73D;
    u8 unk_73E;
    u8 unk_73F;
    u8 unk_740;
    s16 unk_742;
} UnkStruct_ov113_0225EB20;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u32 unk_04;
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov113_02260818;

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

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void sub_020181B0(Field3dObject *obj, Field3dModel *model);

void sub_020182A0(Field3dObject *obj, BOOL visible);

void sub_020182A8(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

void sub_020182C4(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
} UnkStruct_ov113_0225EDA4;

typedef struct {
    s32 unk_00;
    UnkStruct_ov113_0225EDA4 unk_04;
} UnkStruct_ov113_0225EF0C;

typedef struct {
    int unk_00;
    fx32 unk_04;
    u8 unk_08;
    u8 unk_09;
    UnkStruct_ov113_0225EDA4 unk_0C;
} UnkStruct_ov113_0225EF78;

typedef struct {
    Field3dObject unk_00[3];
    u16 unk_168;
    u8 unk_16A;
    UnkStruct_ov113_0225EDA4 unk_16C[4];
} UnkStruct_ov113_0225F060;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    UnkStruct_ov113_0225EDA4 unk_0C;
} UnkStruct_ov113_0225F1E4;

typedef struct {
    int unk_00;
    fx32 unk_04;
    int unk_08;
    fx32 unk_0C;
    UnkStruct_ov113_0225EDA4 unk_10;
} UnkStruct_ov113_0225F368;

typedef struct {
    Field3dObject unk_00[3];
    u16 unk_168;
    u8 unk_16A;
    u8 unk_16B;
    u8 unk_16C;
    u8 unk_16D;
    UnkStruct_ov113_0225EDA4 unk_170;
} UnkStruct_ov113_0225F51C;

typedef struct {
    int unk_00;
    fx32 unk_04;
    fx32 unk_08;
    UnkStruct_ov113_0225EDA4 unk_0C;
} UnkStruct_ov113_0225F728;

typedef struct {
    fx32 unk_00;
    u16 unk_04;
    u8 unk_06;
    UnkStruct_ov113_0225EDA4 unk_08;
} UnkStruct_ov113_0225F818;

typedef struct {
    Field3dObject unk_00[2];
    fx32 unk_F0;
    u16 unk_F4;
    u8 unk_F6;
    u8 unk_F7;
    UnkStruct_ov113_0225EDA4 unk_F8[3];
} UnkStruct_ov113_0225F890;

typedef struct {
    Field3dObject unk_00[2];
    fx32 unk_F0;
    u16 unk_F4;
    u8 unk_F6;
    u8 unk_F7;
    UnkStruct_ov113_0225EDA4 unk_F8[3];
} UnkStruct_ov113_0225FAB0;

typedef struct {
    fx32 unk_00;
    u16 unk_04;
    u8 unk_06;
    UnkStruct_ov113_0225EDA4 unk_08;
} UnkStruct_ov113_0225FCD0;

typedef union {
    UnkStruct_ov113_0225EF0C val1;
    UnkStruct_ov113_0225EF78 val2;
    UnkStruct_ov113_0225F060 val3;
    UnkStruct_ov113_0225F1E4 val4;
    UnkStruct_ov113_0225F368 val5;
    UnkStruct_ov113_0225F51C val6;
    UnkStruct_ov113_0225F728 val7;
    UnkStruct_ov113_0225F818 val8;
    UnkStruct_ov113_0225F890 val9;
    UnkStruct_ov113_0225FAB0 val10;
    UnkStruct_ov113_0225FCD0 val11;
} UnkStruct_ov113_0225E6B8_sub1;

struct UnkStruct_ov113_0225E6B8_t {
    UnkStruct_ov113_02260818 unk_00;
    Field3dModel unk_0C;
    Field3dObject unk_1C;
    UnkStruct_ov113_0225E6B8_sub1 unk_94;
    u32 unk_240_0 : 4;
    u32 unk_240_4 : 4;
    u32 unk_240_8 : 8;
    u32 unk_240_16 : 8;
    u32 unk_240_24 : 8;
};

void ov89_0225AC68(UnkStruct_ov113_0225DBCC *param0, int param1, int param2, fx32 *param3, fx32 *param4);

BOOL ov89_0225A878(UnkStruct_ov113_0225EB20 *param0, Field3dModel *param1, NARC *param2, NARC *param3, const UnkStruct_ov113_02260818 *param4, BOOL param5);

UnkStruct_ov113_0225E6B8 *ov89_0225A7BC(UnkStruct_ov113_0225DBCC *param0, UnkStruct_ov113_0225EB20 *param1, const UnkStruct_ov113_02260818 *param2, NARC *param3, NARC *param4, BOOL param5);

void ov89_0225A988(UnkStruct_ov113_0225EB20 *param0, NNSG3dResTex *param1);

UnkStruct_ov113_0225E6B8 *ov89_0225A7BC(UnkStruct_ov113_0225DBCC *param0, UnkStruct_ov113_0225EB20 *param1, const UnkStruct_ov113_02260818 *param2, NARC *param3, NARC *param4, BOOL param5) {
    UnkStruct_ov113_0225E6B8 *v0;
    fx32 v1, v2;
    BOOL v3;

    v0 = Heap_Alloc((enum HeapID)125, sizeof(UnkStruct_ov113_0225E6B8));
    MI_CpuClear8(v0, sizeof(UnkStruct_ov113_0225E6B8));

    v0->unk_240_24 = 0xff;
    v0->unk_00 = *param2;

    if (v0->unk_00.unk_02 > (SPECIES_BAD_EGG - 2)) {
        ((0) ? (void)0 : GF_AssertFail());
        v0->unk_00.unk_02 = 132;
    }

    v3 = ov89_0225A878(param1, &v0->unk_0C, param3, param4, &v0->unk_00, param5);

    if (v3 == 0) {
        return 0L;
    }

    sub_020181B0(&v0->unk_1C, &v0->unk_0C);
    ov89_0225AC68(param0, param2->unk_09, param2->unk_0A, &v1, &v2);
    sub_020182A8(&v0->unk_1C, v1, v2, (((fx32)0x0000000000001000L) * 16));
    sub_020182C4(&v0->unk_1C, ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))), ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))), ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))));
    sub_020182A0(&v0->unk_1C, 0);

    return v0;
}

void ov89_0225AA24(NNSG3dResTex *param0, NARC *param1, NARC *param2, const UnkStruct_ov113_02260818 *param3, BOOL param4);

BOOL ov89_0225A9B4(NNSG3dResTex *param0);

BOOL ov89_0225A958(UnkStruct_ov113_0225EB20 *param0, NNSG3dResTex *param1);

BOOL ov89_0225A878(UnkStruct_ov113_0225EB20 *param0, Field3dModel *param1, NARC *param2, NARC *param3, const UnkStruct_ov113_02260818 *param4, BOOL param5) {
    param1->dataRaw = GfGfxLoader_LoadFromOpenNarc(param2, 29, 0, HEAP_ID_125, 0);

    {
        BOOL v0;

        GF_ASSERT(param1->dataRaw);

        param1->modelSet = NNS_G3dGetMdlSet(param1->dataRaw);
        param1->model = NNS_G3dGetMdlByIdx(param1->modelSet, 0);
        param1->texture = NNS_G3dGetTex(param1->dataRaw);

        if (param1->texture) {
            ov89_0225AA24(param1->texture, param2, param3, param4, param5);
            v0 = ov89_0225A9B4(param1->texture);

            if ((v0 == 0) || (ov89_0225A958(param0, param1->texture) == 0)) {
                NNSG3dTexKey v1;
                NNSG3dTexKey v2;
                NNSG3dPlttKey v3;

                if (v0 == 1) {
                    NNS_G3dTexReleaseTexKey(param1->texture, &v1, &v2);
                    NNS_GfdFreeTexVram(v1);
                    NNS_GfdFreeTexVram(v2);
                    v3 = NNS_G3dPlttReleasePlttKey(param1->texture);
                    NNS_GfdFreePlttVram(v3);
                }

                if (param1->dataRaw) {
                    Heap_Free(param1->dataRaw);
                }

                memset(param1, 0, sizeof(Field3dModel));
                return 0;
            }

            GF3dRender_BindModelSet(param1->dataRaw, param1->texture);
        }
    }

    return 1;
}

BOOL ov89_0225A958(UnkStruct_ov113_0225EB20 *param0, NNSG3dResTex *param1) {
    int v0;

    for (v0 = 0; v0 < 32; v0++) {
        if (param0->unk_4B4[v0] == 0L) {
            param0->unk_4B4[v0] = param1;
            return 1;
        }
    }

    return 0;
}

void ov89_0225A988(UnkStruct_ov113_0225EB20 *param0, NNSG3dResTex *param1) {
    int v0;

    for (v0 = 0; v0 < 32; v0++) {
        if (param0->unk_4B4[v0] == param1) {
            param0->unk_4B4[v0] = 0L;
            return;
        }
    }
}

BOOL ov89_0225A9B4(NNSG3dResTex *param0) {
    u32 v0, v1;
    NNSG3dTexKey v2;
    NNSG3dPlttKey v3;

    param0->texInfo.vramKey = 0;

    v0 = NNS_G3dTexGetRequiredSize(param0);
    v1 = NNS_G3dPlttGetRequiredSize(param0);
    v2 = NNS_GfdAllocTexVram(v0, 0, 0);

    if (v2 == (u32)0x0) {
        return 0;
    }

    v3 = NNS_GfdAllocPlttVram(v1, param0->tex4x4Info.flag & NNS_G3D_RESPLTT_USEPLTT4, 0);

    if (v3 == (u32)0x0) {
        NNS_GfdFreeTexVram(v2);
        return 0;
    }

    NNS_G3dTexSetTexKey(param0, v2, 0);
    NNS_G3dPlttSetPlttKey(param0, v3);

    return 1;
}
