#include "global.h"

#include "screen_fade.h"
#include "sprite.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "font_types_def.h"
#include "pm_string.h"
#include "sys_task.h"
#include "text.h"
#include "unk_02005D10.h"

s32 ov71_02247C34(void *arg0, s32 *arg1);

struct MessageBank;
struct PokemonSprite;
struct PokemonSpriteManager;
struct PokemonSpriteTransforms;
struct StringTemplateArg;
struct Trade3DModel;
struct Trade3DScene;
struct TradeSequenceData;
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef struct Trade3DModel Trade3DModel;
typedef struct Trade3DScene Trade3DScene;
typedef u16 charcode_t;
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
enum TradeType {
    TRADE_TYPE_NORMAL = 1,
    TRADE_TYPE_SEND_ONLY,
    TRADE_TYPE_RECEIVE_ONLY = 4,
};
typedef struct MessageBank MessageBank;
struct MsgData;
void ReadMsgDataIntoString(const struct MsgData *loader, u32 entryID, String *string);
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
typedef struct PokemonSprite PokemonSprite;
typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;
typedef void(PokemonSpriteCallback)(PokemonSprite *, PokemonSpriteTransforms *);
typedef struct PokemonSpriteTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
} PokemonSpriteTemplate;
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
struct PokemonSprite {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokemonSpriteTemplate template;
    PokemonSpriteTemplate templateBackup;
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
typedef struct PokemonSpriteManager PokemonSpriteManager;
void Pokepic_StartAnim(PokemonSprite *monSprite, int dummy);
u32 GetBoxMonData(BoxPokemon *boxMon, enum PokemonDataParam param, void *dest);
void sub_020729A4(NARC *narc, u8 *cryDelay, u16 species, u16 clientType);
typedef struct StringTemplateArg StringTemplateArg;
typedef struct StringTemplate {
    u32 maxArgs;
    enum HeapID heapID;
    StringTemplateArg *args;
    String *templateBuf;
} StringTemplate;
void StringExpandPlaceholders(const StringTemplate *template, String *dst, const String *fmtString);
typedef struct TradeSequenceData TradeSequenceData;
typedef struct SpriteAnimResources {
    void *cellBuffer;
    void *animBuffer;
    NNSG2dCellDataBank *cellBank;
    NNSG2dAnimBankData *animBank;
} SpriteAnimResources;
StringTemplate *ov71_02247388(TradeSequenceData *sequenceData);
struct MsgData *ov71_0224738C(TradeSequenceData *sequenceData);
const BoxPokemon *ov71_02247390(TradeSequenceData *sequenceData);
u16 ov71_022473A8(TradeSequenceData *sequenceData);
u16 ov71_022473B0(TradeSequenceData *sequenceData);
enum TradeType ov71_022473DC(TradeSequenceData *sequenceData);
void sub_0200E948(Window *window, u32 tile, u32 palette);
void ClearFrameAndWindow2(Window *window, u8 skipTransfer);
typedef struct TradeSendPhase {
    TradeSequenceData *sequenceData;
    int subStepCounter;
    int timer;
    PokemonSpriteManager *spriteManager;
    PokemonSprite *pokemonSprite;
    SpriteAnimFrame animFrames[10];
    Sprite *sprites[2];
    SpriteAnimResources animResources;
    BgConfig *bgConfig;
    Window window;
    String *string0;
    String *string1;
    Trade3DScene *scene;
    Trade3DModel *model;
    SysTask *task0;
    SysTask *task1;
    NARC *pokeDataNarc;
} TradeSendPhase;
int ov71_02247C60(TradeSendPhase *tsPhase, int *subStepCounter);

s32 ov71_02247C34(void *arg0, s32 *arg1) {
    if ((*arg1 == 0) && (IsPaletteFadeFinished() != 0)) {
        Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)arg0 + 0x3C), 1);
        Sprite_SetDrawFlag(*(Sprite **)((u8 *)arg0 + 0x3C), 1);
        return 1;
    }
    return 0;
}

int ov71_02247C60(TradeSendPhase *tsPhase, int *subStepCounter) {
    switch (*subStepCounter) {
    case 0:
        tsPhase->timer = 0;
        (*subStepCounter)++;
        break;
    case 1:
        if (++(tsPhase->timer) > 10) {
            struct MsgData *msgLoader = ov71_0224738C(tsPhase->sequenceData);
            StringTemplate *template = ov71_02247388(tsPhase->sequenceData);
            int msgId, nextStep;

            if (ov71_022473DC(tsPhase->sequenceData) == TRADE_TYPE_NORMAL) {
                msgId = 0;
                nextStep = 2;
            } else {
                msgId = 4;
                nextStep = 3;
            }

            ReadMsgDataIntoString(msgLoader, msgId, tsPhase->string0);
            StringExpandPlaceholders(template, tsPhase->string1, tsPhase->string0);
            AddTextPrinterParameterized(&(tsPhase->window), FONT_MESSAGE, tsPhase->string1, 0, 0, 0xFF, 0L);
            sub_0200E948(&(tsPhase->window), 109, 2);
            CopyWindowToVram(&(tsPhase->window));
            tsPhase->timer = 0;
            (*subStepCounter) = nextStep;
        }
        break;
    case 2:
        if (++(tsPhase->timer) > 60) {
            struct MsgData *msgLoader = ov71_0224738C(tsPhase->sequenceData);
            StringTemplate *template = ov71_02247388(tsPhase->sequenceData);

            ReadMsgDataIntoString(msgLoader, 1, tsPhase->string0);
            StringExpandPlaceholders(template, tsPhase->string1, tsPhase->string0);

            {
                const BoxPokemon *boxMon = ov71_02247390(tsPhase->sequenceData);

                if (GetBoxMonData((BoxPokemon *)boxMon, MON_DATA_IS_EGG, 0L) == 0) {
                    u8 delay;

                    Pokepic_StartAnim(tsPhase->pokemonSprite, 1);
                    sub_020729A4(tsPhase->pokeDataNarc, &delay, ov71_022473A8(tsPhase->sequenceData), 1);
                    sub_020062E0(ov71_022473A8(tsPhase->sequenceData), delay, ov71_022473B0(tsPhase->sequenceData));
                }

                FillWindowPixelBuffer(&(tsPhase->window), 0xf);
                AddTextPrinterParameterized(&(tsPhase->window), FONT_MESSAGE, tsPhase->string1, 0, 0, 0xFF, 0L);
                CopyWindowPixelsToVram_TextMode(&(tsPhase->window));
                tsPhase->timer = 0;
                (*subStepCounter)++;
            }
        }
        break;
    case 3:
        if (++(tsPhase->timer) > 60) {
            ClearFrameAndWindow2(&(tsPhase->window), 0);
            tsPhase->timer = 0;
            (*subStepCounter)++;
        }
        break;
    case 4:
        if (++(tsPhase->timer) > 20) {
            return 1;
        }
        break;
    }

    return 0;
}
