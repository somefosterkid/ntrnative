#include "global.h"

#include "sys_task.h"

void ov74_02233F5C(void);

void ov74_02233F84(void);

typedef struct GBAPokemonSubstruct0 {
    u16 species;
    u16 heldItem;
    u32 experience;
    u8 ppBonuses;
    u8 friendship;
    u16 padding;
} GBAPokemonSubstruct0;
typedef struct GBAPokemonSubstruct1 {
    u16 moves[4];
    u8 pp[4];
} GBAPokemonSubstruct1;
typedef struct GBAPokemonSubstruct2 {
    u8 hpEV;
    u8 attackEV;
    u8 defenseEV;
    u8 speedEV;
    u8 spAttackEV;
    u8 spDefenseEV;
    u8 cool;
    u8 beauty;
    u8 cute;
    u8 smart;
    u8 tough;
    u8 sheen;
} GBAPokemonSubstruct2;
typedef struct GBAPokemonSubstruct3 {
    u32 pokerus : 8;
    u32 metLocation : 8;
    u32 metLevel : 7;
    u32 metGame : 4;
    u32 pokeball : 4;
    u32 otGender : 1;
    u32 hpIV : 5;
    u32 attackIV : 5;
    u32 defenseIV : 5;
    u32 speedIV : 5;
    u32 spAttackIV : 5;
    u32 spDefenseIV : 5;
    u32 isEgg : 1;
    u32 abilityNum : 1;
    u32 coolRibbon : 3;
    u32 beautyRibbon : 3;
    u32 cuteRibbon : 3;
    u32 smartRibbon : 3;
    u32 toughRibbon : 3;
    u32 championRibbon : 1;
    u32 winningRibbon : 1;
    u32 victoryRibbon : 1;
    u32 artistRibbon : 1;
    u32 effortRibbon : 1;
    u32 marineRibbon : 1;
    u32 landRibbon : 1;
    u32 skyRibbon : 1;
    u32 countryRibbon : 1;
    u32 nationalRibbon : 1;
    u32 earthRibbon : 1;
    u32 worldRibbon : 1;
    u32 unusedRibbons : 4;

    u32 modernFatefulEncounter : 1;
} GBAPokemonSubstruct3;
union GBAPokemonSubstruct {
    GBAPokemonSubstruct0 type0;
    GBAPokemonSubstruct1 type1;
    GBAPokemonSubstruct2 type2;
    GBAPokemonSubstruct3 type3;
    u16 raw[(((sizeof(GBAPokemonSubstruct0)) > (sizeof(GBAPokemonSubstruct1)) ? (sizeof(GBAPokemonSubstruct0)) : (sizeof(GBAPokemonSubstruct1))) > ((sizeof(GBAPokemonSubstruct2)) > (sizeof(GBAPokemonSubstruct3)) ? (sizeof(GBAPokemonSubstruct2)) : (sizeof(GBAPokemonSubstruct3))) ? ((sizeof(GBAPokemonSubstruct0)) > (sizeof(GBAPokemonSubstruct1)) ? (sizeof(GBAPokemonSubstruct0)) : (sizeof(GBAPokemonSubstruct1))) : ((sizeof(GBAPokemonSubstruct2)) > (sizeof(GBAPokemonSubstruct3)) ? (sizeof(GBAPokemonSubstruct2)) : (sizeof(GBAPokemonSubstruct3)))) / sizeof(u16)];
};
typedef struct GBABoxPokemon {
    u32 personality;
    u32 otId;
    u8 nickname[10];
    u8 language;
    u8 isBadEgg : 1;
    u8 hasSpecies : 1;
    u8 isEgg : 1;
    u8 unused : 5;
    u8 otName[7];
    u8 markings;
    u16 checksum;
    u16 unknown;
    union {
        u32 raw[12];
        union GBAPokemonSubstruct substructs[4];
    } secure;
} GBABoxPokemon;
union GBAPokemonSubstruct *ov74_02233F8C(GBABoxPokemon *gbaBoxMon, u32 personality, u8 substructType);
enum GbaPokemonAttribute {
    GBA_MON_DATA_PERSONALITY,
    GBA_MON_DATA_OT_ID,
    GBA_MON_DATA_NICKNAME,
    GBA_MON_DATA_LANGUAGE,
    GBA_MON_DATA_SANITY_IS_BAD_EGG,
    GBA_MON_DATA_SANITY_HAS_SPECIES,
    GBA_MON_DATA_SANITY_IS_EGG,
    GBA_MON_DATA_OT_NAME,
    GBA_MON_DATA_MARKINGS,
    GBA_MON_DATA_CHECKSUM,
    GBA_MON_DATA_ENCRYPT_SEPARATOR,
    GBA_MON_DATA_SPECIES,
    GBA_MON_DATA_HELD_ITEM,
    GBA_MON_DATA_MOVE1,
    GBA_MON_DATA_MOVE2,
    GBA_MON_DATA_MOVE3,
    GBA_MON_DATA_MOVE4,
    GBA_MON_DATA_PP1,
    GBA_MON_DATA_PP2,
    GBA_MON_DATA_PP3,
    GBA_MON_DATA_PP4,
    GBA_MON_DATA_PP_BONUSES,
    GBA_MON_DATA_COOL,
    GBA_MON_DATA_BEAUTY,
    GBA_MON_DATA_CUTE,
    GBA_MON_DATA_EXP,
    GBA_MON_DATA_HP_EV,
    GBA_MON_DATA_ATK_EV,
    GBA_MON_DATA_DEF_EV,
    GBA_MON_DATA_SPEED_EV,
    GBA_MON_DATA_SPATK_EV,
    GBA_MON_DATA_SPDEF_EV,
    GBA_MON_DATA_FRIENDSHIP,
    GBA_MON_DATA_SMART,
    GBA_MON_DATA_POKERUS,
    GBA_MON_DATA_MET_LOCATION,
    GBA_MON_DATA_MET_LEVEL,
    GBA_MON_DATA_MET_GAME,
    GBA_MON_DATA_POKEBALL,
    GBA_MON_DATA_HP_IV,
    GBA_MON_DATA_ATK_IV,
    GBA_MON_DATA_DEF_IV,
    GBA_MON_DATA_SPEED_IV,
    GBA_MON_DATA_SPATK_IV,
    GBA_MON_DATA_SPDEF_IV,
    GBA_MON_DATA_IS_EGG,
    GBA_MON_DATA_ABILITY_NUM,
    GBA_MON_DATA_TOUGH,
    GBA_MON_DATA_SHEEN,
    GBA_MON_DATA_OT_GENDER,
    GBA_MON_DATA_COOL_RIBBON,
    GBA_MON_DATA_BEAUTY_RIBBON,
    GBA_MON_DATA_CUTE_RIBBON,
    GBA_MON_DATA_SMART_RIBBON,
    GBA_MON_DATA_TOUGH_RIBBON,
    GBA_MON_DATA_STATUS,
    GBA_MON_DATA_LEVEL,
    GBA_MON_DATA_HP,
    GBA_MON_DATA_MAX_HP,
    GBA_MON_DATA_ATK,
    GBA_MON_DATA_DEF,
    GBA_MON_DATA_SPEED,
    GBA_MON_DATA_SPATK,
    GBA_MON_DATA_SPDEF,
    GBA_MON_DATA_MAIL,
    GBA_MON_DATA_SPECIES_OR_EGG,
    GBA_MON_DATA_IVS,
    GBA_MON_DATA_CHAMPION_RIBBON,
    GBA_MON_DATA_WINNING_RIBBON,
    GBA_MON_DATA_VICTORY_RIBBON,
    GBA_MON_DATA_ARTIST_RIBBON,
    GBA_MON_DATA_EFFORT_RIBBON,
    GBA_MON_DATA_MARINE_RIBBON,
    GBA_MON_DATA_LAND_RIBBON,
    GBA_MON_DATA_SKY_RIBBON,
    GBA_MON_DATA_COUNTRY_RIBBON,
    GBA_MON_DATA_NATIONAL_RIBBON,
    GBA_MON_DATA_EARTH_RIBBON,
    GBA_MON_DATA_WORLD_RIBBON,
    GBA_MON_DATA_UNUSED_RIBBONS,
    GBA_MON_DATA_MODERN_FATEFUL_ENCOUNTER,
    GBA_MON_DATA_KNOWN_MOVES,
    GBA_MON_DATA_RIBBON_COUNT,
    GBA_MON_DATA_RIBBONS,
    GBA_MON_DATA_ATK2,
    GBA_MON_DATA_DEF2,
    GBA_MON_DATA_SPEED2,
    GBA_MON_DATA_SPATK2,
    GBA_MON_DATA_SPDEF2,
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
void ov74_02234468(GBABoxPokemon *gbaBoxMon);
void ov74_02234488(GBABoxPokemon *gbaBoxMon);
u16 ov74_022344A8(GBABoxPokemon *gbaBoxMon);
union GBAPokemonSubstruct *ov74_02233F8C(GBABoxPokemon *gbaBoxMon, u32 personality, u8 substructType);
void ov74_02234468(GBABoxPokemon *gbaBoxMon);
void ov74_02234488(GBABoxPokemon *gbaBoxMon);
u16 ov74_022344A8(GBABoxPokemon *gbaBoxMon);
u32 AGB_GetBoxMonData(GBABoxPokemon *gbaBoxMon, int field, u8 *param2);
void ov74_022348B0(GBABoxPokemon *gbaBoxMon, int field, const u8 *dataArg);

void ov74_02233F84(void) {
    ov74_02233F5C();
}

union GBAPokemonSubstruct *ov74_02233F8C(GBABoxPokemon *gbaBoxMon, u32 personality, u8 substructType) {
    union GBAPokemonSubstruct *substruct = 0L;

    switch (personality % 24) {
    case 0: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs0[0];
            break;
        case 1:
            substruct = &substructs0[1];
            break;
        case 2:
            substruct = &substructs0[2];
            break;
        case 3:
            substruct = &substructs0[3];
            break;
        }
        break;
    }
    case 1: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs1[0];
            break;
        case 1:
            substruct = &substructs1[1];
            break;
        case 2:
            substruct = &substructs1[3];
            break;
        case 3:
            substruct = &substructs1[2];
            break;
        }
        break;
    }
    case 2: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs2[0];
            break;
        case 1:
            substruct = &substructs2[2];
            break;
        case 2:
            substruct = &substructs2[1];
            break;
        case 3:
            substruct = &substructs2[3];
            break;
        }
        break;
    }
    case 3: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs3[0];
            break;
        case 1:
            substruct = &substructs3[3];
            break;
        case 2:
            substruct = &substructs3[1];
            break;
        case 3:
            substruct = &substructs3[2];
            break;
        }
        break;
    }
    case 4: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs4[0];
            break;
        case 1:
            substruct = &substructs4[2];
            break;
        case 2:
            substruct = &substructs4[3];
            break;
        case 3:
            substruct = &substructs4[1];
            break;
        }
        break;
    }
    case 5: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs5[0];
            break;
        case 1:
            substruct = &substructs5[3];
            break;
        case 2:
            substruct = &substructs5[2];
            break;
        case 3:
            substruct = &substructs5[1];
            break;
        }
        break;
    }
    case 6: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs6[1];
            break;
        case 1:
            substruct = &substructs6[0];
            break;
        case 2:
            substruct = &substructs6[2];
            break;
        case 3:
            substruct = &substructs6[3];
            break;
        }
        break;
    }
    case 7: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs7[1];
            break;
        case 1:
            substruct = &substructs7[0];
            break;
        case 2:
            substruct = &substructs7[3];
            break;
        case 3:
            substruct = &substructs7[2];
            break;
        }
        break;
    }
    case 8: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs8[2];
            break;
        case 1:
            substruct = &substructs8[0];
            break;
        case 2:
            substruct = &substructs8[1];
            break;
        case 3:
            substruct = &substructs8[3];
            break;
        }
        break;
    }
    case 9: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs9[3];
            break;
        case 1:
            substruct = &substructs9[0];
            break;
        case 2:
            substruct = &substructs9[1];
            break;
        case 3:
            substruct = &substructs9[2];
            break;
        }
        break;
    }
    case 10: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs10[2];
            break;
        case 1:
            substruct = &substructs10[0];
            break;
        case 2:
            substruct = &substructs10[3];
            break;
        case 3:
            substruct = &substructs10[1];
            break;
        }
        break;
    }
    case 11: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs11[3];
            break;
        case 1:
            substruct = &substructs11[0];
            break;
        case 2:
            substruct = &substructs11[2];
            break;
        case 3:
            substruct = &substructs11[1];
            break;
        }
        break;
    }
    case 12: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs12[1];
            break;
        case 1:
            substruct = &substructs12[2];
            break;
        case 2:
            substruct = &substructs12[0];
            break;
        case 3:
            substruct = &substructs12[3];
            break;
        }
        break;
    }
    case 13: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs13[1];
            break;
        case 1:
            substruct = &substructs13[3];
            break;
        case 2:
            substruct = &substructs13[0];
            break;
        case 3:
            substruct = &substructs13[2];
            break;
        }
        break;
    }
    case 14: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs14[2];
            break;
        case 1:
            substruct = &substructs14[1];
            break;
        case 2:
            substruct = &substructs14[0];
            break;
        case 3:
            substruct = &substructs14[3];
            break;
        }
        break;
    }
    case 15: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs15[3];
            break;
        case 1:
            substruct = &substructs15[1];
            break;
        case 2:
            substruct = &substructs15[0];
            break;
        case 3:
            substruct = &substructs15[2];
            break;
        }
        break;
    }
    case 16: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs16[2];
            break;
        case 1:
            substruct = &substructs16[3];
            break;
        case 2:
            substruct = &substructs16[0];
            break;
        case 3:
            substruct = &substructs16[1];
            break;
        }
        break;
    }
    case 17: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs17[3];
            break;
        case 1:
            substruct = &substructs17[2];
            break;
        case 2:
            substruct = &substructs17[0];
            break;
        case 3:
            substruct = &substructs17[1];
            break;
        }
        break;
    }
    case 18: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs18[1];
            break;
        case 1:
            substruct = &substructs18[2];
            break;
        case 2:
            substruct = &substructs18[3];
            break;
        case 3:
            substruct = &substructs18[0];
            break;
        }
        break;
    }
    case 19: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs19[1];
            break;
        case 1:
            substruct = &substructs19[3];
            break;
        case 2:
            substruct = &substructs19[2];
            break;
        case 3:
            substruct = &substructs19[0];
            break;
        }
        break;
    }
    case 20: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs20[2];
            break;
        case 1:
            substruct = &substructs20[1];
            break;
        case 2:
            substruct = &substructs20[3];
            break;
        case 3:
            substruct = &substructs20[0];
            break;
        }
        break;
    }
    case 21: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs21[3];
            break;
        case 1:
            substruct = &substructs21[1];
            break;
        case 2:
            substruct = &substructs21[2];
            break;
        case 3:
            substruct = &substructs21[0];
            break;
        }
        break;
    }
    case 22: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs22[2];
            break;
        case 1:
            substruct = &substructs22[3];
            break;
        case 2:
            substruct = &substructs22[1];
            break;
        case 3:
            substruct = &substructs22[0];
            break;
        }
        break;
    }
    case 23: {
        union GBAPokemonSubstruct *substructs0 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs1 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs2 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs3 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs4 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs5 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs6 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs7 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs8 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs9 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs10 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs11 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs12 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs13 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs14 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs15 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs16 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs17 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs18 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs19 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs20 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs21 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs22 = gbaBoxMon->secure.substructs;
        union GBAPokemonSubstruct *substructs23 = gbaBoxMon->secure.substructs;
        switch (substructType) {
        case 0:
            substruct = &substructs23[3];
            break;
        case 1:
            substruct = &substructs23[2];
            break;
        case 2:
            substruct = &substructs23[1];
            break;
        case 3:
            substruct = &substructs23[0];
            break;
        }
        break;
    }
    }

    return substruct;
}

void ov74_02234468(GBABoxPokemon *gbaBoxMon) {
    int v0;
    u32 *v1 = (u32 *)&gbaBoxMon->secure;

    for (v0 = 0; v0 < (sizeof(GBAPokemonSubstruct0) + sizeof(GBAPokemonSubstruct1) + sizeof(GBAPokemonSubstruct2) + sizeof(GBAPokemonSubstruct3)) / 4; v0++) {
        v1[v0] ^= gbaBoxMon->otId;
        v1[v0] ^= gbaBoxMon->personality;
    }
}

void ov74_02234488(GBABoxPokemon *gbaBoxMon) {
    int v0;
    u32 *v1 = (u32 *)&gbaBoxMon->secure;

    for (v0 = 0; v0 < (sizeof(GBAPokemonSubstruct0) + sizeof(GBAPokemonSubstruct1) + sizeof(GBAPokemonSubstruct2) + sizeof(GBAPokemonSubstruct3)) / 4; v0++) {
        v1[v0] ^= gbaBoxMon->personality;
        v1[v0] ^= gbaBoxMon->otId;
    }
}

u16 ov74_022344A8(GBABoxPokemon *gbaBoxMon) {
    u16 checksum = 0;
    union GBAPokemonSubstruct *substruct0 = ov74_02233F8C(gbaBoxMon, gbaBoxMon->personality, 0);
    union GBAPokemonSubstruct *substruct1 = ov74_02233F8C(gbaBoxMon, gbaBoxMon->personality, 1);
    union GBAPokemonSubstruct *substruct2 = ov74_02233F8C(gbaBoxMon, gbaBoxMon->personality, 2);
    union GBAPokemonSubstruct *substruct3 = ov74_02233F8C(gbaBoxMon, gbaBoxMon->personality, 3);

    for (int i = 0; i < ((int)(sizeof(substruct0->raw) / sizeof(*(substruct0->raw)))); i++) {
        checksum += substruct0->raw[i];
    }

    for (int i = 0; i < ((int)(sizeof(substruct1->raw) / sizeof(*(substruct1->raw)))); i++) {
        checksum += substruct1->raw[i];
    }

    for (int i = 0; i < ((int)(sizeof(substruct2->raw) / sizeof(*(substruct2->raw)))); i++) {
        checksum += substruct2->raw[i];
    }

    for (int i = 0; i < ((int)(sizeof(substruct3->raw) / sizeof(*(substruct3->raw)))); i++) {
        checksum += substruct3->raw[i];
    }

    return checksum;
}

u32 AGB_GetBoxMonData(GBABoxPokemon *gbaBoxMon, int field, u8 *param2) {
    u32 retVal = 0;
    GBAPokemonSubstruct0 *substruct0 = 0;
    GBAPokemonSubstruct1 *substruct1 = 0;
    GBAPokemonSubstruct2 *substruct2 = 0;
    GBAPokemonSubstruct3 *substruct3 = 0;
    u16 checksum;

    if (field > GBA_MON_DATA_ENCRYPT_SEPARATOR) {
        substruct0 = (GBAPokemonSubstruct0 *)ov74_02233F8C(gbaBoxMon, gbaBoxMon->personality, 0);
        substruct1 = (GBAPokemonSubstruct1 *)ov74_02233F8C(gbaBoxMon, gbaBoxMon->personality, 1);
        substruct2 = (GBAPokemonSubstruct2 *)ov74_02233F8C(gbaBoxMon, gbaBoxMon->personality, 2);
        substruct3 = (GBAPokemonSubstruct3 *)ov74_02233F8C(gbaBoxMon, gbaBoxMon->personality, 3);

        ov74_02234468(gbaBoxMon);
        checksum = ov74_022344A8(gbaBoxMon);

        if (checksum != gbaBoxMon->checksum) {
            gbaBoxMon->isBadEgg = 1;
            gbaBoxMon->isEgg = 1;
            substruct3->isEgg = 1;
        }
    }

    switch (field) {
    case GBA_MON_DATA_PERSONALITY:
        retVal = gbaBoxMon->personality;
        break;
    case GBA_MON_DATA_OT_ID:
        retVal = gbaBoxMon->otId;
        break;
    case GBA_MON_DATA_NICKNAME:
        if (gbaBoxMon->isBadEgg) {
            retVal = 0;
        } else {
            for (retVal = 0; retVal < 10; retVal++) {
                param2[retVal] = gbaBoxMon->nickname[retVal];
            }
        }

        param2[retVal] = 0xff;
        break;
    case GBA_MON_DATA_LANGUAGE:
        retVal = gbaBoxMon->language;
        break;
    case GBA_MON_DATA_SANITY_IS_BAD_EGG:
        retVal = gbaBoxMon->isBadEgg;
        break;
    case GBA_MON_DATA_SANITY_HAS_SPECIES:
        retVal = gbaBoxMon->hasSpecies;
        break;
    case GBA_MON_DATA_SANITY_IS_EGG:
        retVal = gbaBoxMon->isEgg;
        break;
    case GBA_MON_DATA_OT_NAME:
        for (retVal = 0; retVal < 7; retVal++) {
            param2[retVal] = gbaBoxMon->otName[retVal];
        }

        param2[retVal] = 0xffff;
        break;
    case GBA_MON_DATA_MARKINGS:
        retVal = gbaBoxMon->markings;
        break;
    case GBA_MON_DATA_CHECKSUM:
        retVal = gbaBoxMon->checksum;
        break;
    case GBA_MON_DATA_ENCRYPT_SEPARATOR:
        retVal = gbaBoxMon->unknown;
        break;
    case GBA_MON_DATA_SPECIES:
        if (gbaBoxMon->isBadEgg) {
            retVal = 412;
        } else {
            retVal = substruct0->species;
        }
        break;
    case GBA_MON_DATA_HELD_ITEM:
        retVal = substruct0->heldItem;
        break;
    case GBA_MON_DATA_EXP:
        retVal = substruct0->experience;
        break;
    case GBA_MON_DATA_PP_BONUSES:
        retVal = substruct0->ppBonuses;
        break;
    case GBA_MON_DATA_FRIENDSHIP:
        retVal = substruct0->friendship;
        break;
    case GBA_MON_DATA_MOVE1:
    case GBA_MON_DATA_MOVE2:
    case GBA_MON_DATA_MOVE3:
    case GBA_MON_DATA_MOVE4:
        retVal = substruct1->moves[field - GBA_MON_DATA_MOVE1];
        break;
    case GBA_MON_DATA_PP1:
    case GBA_MON_DATA_PP2:
    case GBA_MON_DATA_PP3:
    case GBA_MON_DATA_PP4:
        retVal = substruct1->pp[field - GBA_MON_DATA_PP1];
        break;
    case GBA_MON_DATA_HP_EV:
        retVal = substruct2->hpEV;
        break;
    case GBA_MON_DATA_ATK_EV:
        retVal = substruct2->attackEV;
        break;
    case GBA_MON_DATA_DEF_EV:
        retVal = substruct2->defenseEV;
        break;
    case GBA_MON_DATA_SPEED_EV:
        retVal = substruct2->speedEV;
        break;
    case GBA_MON_DATA_SPATK_EV:
        retVal = substruct2->spAttackEV;
        break;
    case GBA_MON_DATA_SPDEF_EV:
        retVal = substruct2->spDefenseEV;
        break;
    case GBA_MON_DATA_COOL:
        retVal = substruct2->cool;
        break;
    case GBA_MON_DATA_BEAUTY:
        retVal = substruct2->beauty;
        break;
    case GBA_MON_DATA_CUTE:
        retVal = substruct2->cute;
        break;
    case GBA_MON_DATA_SMART:
        retVal = substruct2->smart;
        break;
    case GBA_MON_DATA_TOUGH:
        retVal = substruct2->tough;
        break;
    case GBA_MON_DATA_SHEEN:
        retVal = substruct2->sheen;
        break;
    case GBA_MON_DATA_POKERUS:
        retVal = substruct3->pokerus;
        break;
    case GBA_MON_DATA_MET_LOCATION:
        retVal = substruct3->metLocation;
        break;
    case GBA_MON_DATA_MET_LEVEL:
        retVal = substruct3->metLevel;
        break;
    case GBA_MON_DATA_MET_GAME:
        retVal = substruct3->metGame;
        break;
    case GBA_MON_DATA_POKEBALL:
        retVal = substruct3->pokeball;
        break;
    case GBA_MON_DATA_OT_GENDER:
        retVal = substruct3->otGender;
        break;
    case GBA_MON_DATA_HP_IV:
        retVal = substruct3->hpIV;
        break;
    case GBA_MON_DATA_ATK_IV:
        retVal = substruct3->attackIV;
        break;
    case GBA_MON_DATA_DEF_IV:
        retVal = substruct3->defenseIV;
        break;
    case GBA_MON_DATA_SPEED_IV:
        retVal = substruct3->speedIV;
        break;
    case GBA_MON_DATA_SPATK_IV:
        retVal = substruct3->spAttackIV;
        break;
    case GBA_MON_DATA_SPDEF_IV:
        retVal = substruct3->spDefenseIV;
        break;
    case GBA_MON_DATA_IS_EGG:
        retVal = substruct3->isEgg;
        break;
    case GBA_MON_DATA_ABILITY_NUM:
        retVal = substruct3->abilityNum;
        break;
    case GBA_MON_DATA_COOL_RIBBON:
        retVal = substruct3->coolRibbon;
        break;
    case GBA_MON_DATA_BEAUTY_RIBBON:
        retVal = substruct3->beautyRibbon;
        break;
    case GBA_MON_DATA_CUTE_RIBBON:
        retVal = substruct3->cuteRibbon;
        break;
    case GBA_MON_DATA_SMART_RIBBON:
        retVal = substruct3->smartRibbon;
        break;
    case GBA_MON_DATA_TOUGH_RIBBON:
        retVal = substruct3->toughRibbon;
        break;
    case GBA_MON_DATA_CHAMPION_RIBBON:
        retVal = substruct3->championRibbon;
        break;
    case GBA_MON_DATA_WINNING_RIBBON:
        retVal = substruct3->winningRibbon;
        break;
    case GBA_MON_DATA_VICTORY_RIBBON:
        retVal = substruct3->victoryRibbon;
        break;
    case GBA_MON_DATA_ARTIST_RIBBON:
        retVal = substruct3->artistRibbon;
        break;
    case GBA_MON_DATA_EFFORT_RIBBON:
        retVal = substruct3->effortRibbon;
        break;
    case GBA_MON_DATA_MARINE_RIBBON:
        retVal = substruct3->marineRibbon;
        break;
    case GBA_MON_DATA_LAND_RIBBON:
        retVal = substruct3->landRibbon;
        break;
    case GBA_MON_DATA_SKY_RIBBON:
        retVal = substruct3->skyRibbon;
        break;
    case GBA_MON_DATA_COUNTRY_RIBBON:
        retVal = substruct3->countryRibbon;
        break;
    case GBA_MON_DATA_NATIONAL_RIBBON:
        retVal = substruct3->nationalRibbon;
        break;
    case GBA_MON_DATA_EARTH_RIBBON:
        retVal = substruct3->earthRibbon;
        break;
    case GBA_MON_DATA_WORLD_RIBBON:
        retVal = substruct3->worldRibbon;
        break;
    case GBA_MON_DATA_UNUSED_RIBBONS:
        retVal = substruct3->unusedRibbons;
        break;
    case GBA_MON_DATA_MODERN_FATEFUL_ENCOUNTER:
        retVal = substruct3->modernFatefulEncounter;
        break;
    case GBA_MON_DATA_SPECIES_OR_EGG:
        retVal = substruct0->species;

        if (retVal == SPECIES_NONE) {
            break;
        }

        if (substruct3->isEgg || gbaBoxMon->isBadEgg) {
            retVal = 412;
        }

        break;
    case GBA_MON_DATA_IVS:
        retVal = substruct3->hpIV
            | (substruct3->attackIV << 5)
            | (substruct3->defenseIV << 10)
            | (substruct3->speedIV << 15)
            | (substruct3->spAttackIV << 20)
            | (substruct3->spDefenseIV << 25);
        break;
    }

    if (field > GBA_MON_DATA_ENCRYPT_SEPARATOR) {
        ov74_02234488(gbaBoxMon);
    }

    return retVal;
}

void ov74_022348B0(GBABoxPokemon *gbaBoxMon, int field, const u8 *dataArg) {
    GBAPokemonSubstruct0 *substruct0 = 0;
    GBAPokemonSubstruct1 *substruct1 = 0;
    GBAPokemonSubstruct2 *substruct2 = 0;
    GBAPokemonSubstruct3 *substruct3 = 0;
    u16 checksum;

    if (field > GBA_MON_DATA_ENCRYPT_SEPARATOR) {
        substruct0 = (GBAPokemonSubstruct0 *)ov74_02233F8C(gbaBoxMon, gbaBoxMon->personality, 0);
        substruct1 = (GBAPokemonSubstruct1 *)ov74_02233F8C(gbaBoxMon, gbaBoxMon->personality, 1);
        substruct2 = (GBAPokemonSubstruct2 *)ov74_02233F8C(gbaBoxMon, gbaBoxMon->personality, 2);
        substruct3 = (GBAPokemonSubstruct3 *)ov74_02233F8C(gbaBoxMon, gbaBoxMon->personality, 3);

        ov74_02234468(gbaBoxMon);
        checksum = ov74_022344A8(gbaBoxMon);

        if (checksum != gbaBoxMon->checksum) {
            gbaBoxMon->isBadEgg = 1;
            gbaBoxMon->isEgg = 1;
            substruct3->isEgg = 1;
            ov74_02234488(gbaBoxMon);
            return;
        }
    }

    switch (field) {
    case GBA_MON_DATA_NICKNAME: {
        s32 i;
        for (i = 0; i < 10; i++) {
            gbaBoxMon->nickname[i] = dataArg[i];
        }
        break;
    }
    case GBA_MON_DATA_LANGUAGE:
        (gbaBoxMon->language) = *dataArg;
        break;
    case GBA_MON_DATA_SANITY_IS_BAD_EGG:
        (gbaBoxMon->isBadEgg) = *dataArg;
        break;
    case GBA_MON_DATA_SANITY_HAS_SPECIES:
        (gbaBoxMon->hasSpecies) = *dataArg;
        break;
    case GBA_MON_DATA_SANITY_IS_EGG:
        (gbaBoxMon->isEgg) = *dataArg;
        break;
    case GBA_MON_DATA_OT_NAME: {
        s32 i;
        for (i = 0; i < 7; i++) {
            gbaBoxMon->otName[i] = dataArg[i];
        }
        break;
    }
    case GBA_MON_DATA_SPECIES: {
        (substruct0->species) = dataArg[0] + (dataArg[1] << 8);
        if (substruct0->species) {
            gbaBoxMon->hasSpecies = 1;
        } else {
            gbaBoxMon->hasSpecies = 0;
        }
        break;
    }
    }

    if (field > GBA_MON_DATA_ENCRYPT_SEPARATOR) {
        gbaBoxMon->checksum = ov74_022344A8(gbaBoxMon);
        ov74_02234488(gbaBoxMon);
    }
}
