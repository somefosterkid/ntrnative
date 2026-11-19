#include "global.h"

#include "battle/battler_sprite_context.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_controller.h"
#include "battle/battle_message_structs.h"
#include "battle/battle_system.h"
#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "heap.h"
#include "overlay_07.h"
#include "palette.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "unk_02016EDC.h"
#include "wild_encounter.h"
#include "overlay_12_mon_show_data.h"
#include "particle_system.h"
#include "overlay_74_main_menu_util.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_ball_rotation.h"
#include "overlay_07_battle_anim_context.h"
#include "overlay_07_battle_bg_anim.h"
#include "overlay_07_battle_anim_tracking.h"
#include "overlay_12_battle_controller_opponent.h"

typedef struct SpriteAnimFrame SpriteAnimFrame;

typedef struct BattleBackgroundReference BattleBackgroundReference;

typedef struct BattleAnimScriptLoop BattleAnimScriptLoop;

typedef struct PokemonSpriteDrawContext PokemonSpriteDrawContext;

typedef struct SpriteSystemContext SpriteSystemContext;

typedef struct BattleMonOBJData BattleMonOBJData;

typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);

typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);

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

struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
};

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

struct BattleBackgroundReference {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
};

enum BattleAnimSystemArc {
    BATTLE_ANIM_SYSTEM_ARC_BATT_BG = 0,
    BATTLE_ANIM_SYSTEM_ARC_BATT_OBJ,
    BATTLE_ANIM_SYSTEM_ARC_WECHAR,
    BATTLE_ANIM_SYSTEM_ARC_WEPLTT,
    BATTLE_ANIM_SYSTEM_ARC_WECELL,
    BATTLE_ANIM_SYSTEM_ARC_WECELLANM,

    BATTLE_ANIM_SYSTEM_ARC_COUNT,
};

struct BattleAnimScriptLoop {
    u32 *startPos;
    u8 curIteration;
    u8 maxIterations;
    BOOL isActive;
};

struct PokemonSpriteDrawContext {
    struct BattleAnimSystem *battleAnimSys;
    SpriteManager *spriteManager;
    ManagedSprite *sprite;
    BOOL active;
};

struct SpriteSystemContext {
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    PaletteData *plttData;
};

struct BattleMonOBJData {
    enum HeapID heapID;
    int battlerIdx;
    SpriteSystemContext spriteContext;
    int resourceIDs[4];
    ManagedSprite *managedSprites[4];
    PokemonSpriteData *pokemonSpriteDataArray[4];
    u8 battlerTypes[4];
    Pokepic *pokemonSprites[4];
};

s32 ov07_0221C394(void *arg0);

s32 ov07_0221C3B0(void *arg0);

s32 ov07_0221C3C0(void *arg0);

void ov07_02234A20(BattlerSpriteContext *battlerSpriteCtx, enum HeapID heapID);

Pokepic *ov12_022612A4(BattleSystem *battleSys, PokepicManager *monSpriteMan, void *spriteTemplate, int x, int y, int z, int yOffset, int height, int xOffset, int shadowSize, int battler, SpriteAnimFrame *animFrames, PokemonSpriteCallback *callback);

void ov12_02261B80(BattleSystem *battleSys, OpponentData *battlerData, BattleAnimSystem *battleAnimSystem, MoveAnimation *animation);

BattleMonOBJData *ov07_0221FDFC(BattleSystem *, s32);

void ov07_0221FE08(struct BattleMonOBJData *btlMonObjData);

s32 ov12_02261CA8(BattleSystem *, void *, BattlerSpriteContext *, int);


extern const s16 ov07_022377DC[][2];

extern const s16 ov07_022377F4[][3];

extern void *ov12_0226D120;

extern void *ov12_0226D15A;

void ov12_0225C18C(SysTask *arg0, void *arg1);

void ov12_0225C18C(SysTask *arg0, void *arg1) {
    MoveAnimation sp180;
    MoveAnimation sp128;
    BattlerSpriteContext spD8;
    MoveAnimation sp80;
    BallThrow ballThrow;
    PokepicAnimScript sp30[10];

    BallCapsuleSealEffect *temp_r0_2;
    OpponentData *temp_r3_2;
    OpponentData *temp_r3_3;
    PokepicManager *temp_r5_2;
    u32 *temp_r0;
    u32 *temp_r5;
    u8 temp_r1;
    u8 temp_r1_3;
    u8 temp_r1_4;
    u8 temp_r1_5;
    u8 temp_r3;

    temp_r0 = ov12_0223A8DC(*(BattleSystem **)arg1);
    temp_r1 = ((MonShowData *)arg1)->state;
    temp_r5 = temp_r0;
    switch ((u32)temp_r1) {
    case 0:
        ballThrow.type = (s32) * (u8 *)((u8 *)&ov12_0226D120 + ((MonShowData *)arg1)->battlerType);
        ballThrow.heapID = HEAP_ID_BATTLE;
        ballThrow.target = (s32) ((MonShowData *)arg1)->battler;
        ballThrow.ball = (s32) ((MonShowData *)arg1)->capturedBall;
        ballThrow.spriteSystem = BattleSystem_GetSpriteSystem(*(BattleSystem **)arg1);
        ballThrow.paletteData = BattleSystem_GetPaletteData(*(BattleSystem **)arg1);
        ballThrow.bgPrio = 1;
        ballThrow.surface = 0;
        if (((MonShowData *)arg1)->isQuickSendOut == 1) {
            ballThrow.mode = 1;
        } else {
            ballThrow.mode = 0;
        }
        ((MonShowData *)arg1)->ballRotation = ov07_02233DB8(&ballThrow);
        temp_r5_2 = BattleSystem_GetPokepicManager(*(BattleSystem **)arg1);
        NARC_ReadPokepicAnimScript((((MonShowData *)arg1)->battlerData)->narc, sp30, ((MonShowData *)arg1)->species, (u16) ((MonShowData *)arg1)->battlerType);
        temp_r3 = ((MonShowData *)arg1)->battlerType;

        (((MonShowData *)arg1)->battlerData)->monSprite = ov12_022612A4(*(BattleSystem **)arg1, temp_r5_2, &((MonShowData *)arg1)->spriteTemplate, ov07_022377DC[temp_r3][0], ov07_022377F4[temp_r3][1], ov07_022377F4[temp_r3][2], (s32) ((MonShowData *)arg1)->yOffset, (s32) ((MonShowData *)arg1)->height, (s32) ((MonShowData *)arg1)->shadowXOffset, (s32) ((MonShowData *)arg1)->shadowSize, (s32) ((MonShowData *)arg1)->battler, (SpriteAnimFrame *)sp30, NULL);
        Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xC, 0);
        Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xD, 0);
        Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0x2C, 0);
        Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 6, 1);
        *((s32 *)&((MonShowData *)arg1)->btlMonObjData) = 0;
        *((void **)&((MonShowData *)arg1)->btlMonObjData) = ov07_0221FDFC(*(BattleSystem **)arg1, 5);
        ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
        break;
    case 1: {
        BallCapsuleConfig sp24 = { 0 };
        sp24.battlerType = (s32) ((MonShowData *)arg1)->battlerType;
        sp24.mon = BattleSystem_GetPartyMon(*(BattleSystem **)arg1, (s32) ((MonShowData *)arg1)->battler, (s32) ((MonShowData *)arg1)->selectedPartySlot);
        sp24.ballTypeOverride = (s32) ((MonShowData *)arg1)->capturedBall;
        temp_r0_2 = ov07_02232694(HEAP_ID_BATTLE, &sp24);
        *((BallCapsuleSealEffect **)&((MonShowData *)arg1)->ballCapsuleSealEffect) = temp_r0_2;
        ov07_022329B0(temp_r0_2);
        ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
        break;
    }
    case 2:
        if (((ov07_02233F20((((MonShowData *)arg1)->ballRotation)) != 0) && (((MonShowData *)arg1)->isQuickSendOut == 0)) || (ov07_02232A04(((BallCapsuleSealEffect *)((MonShowData *)arg1)->ballCapsuleSealEffect)) != 1) || (ov07_02233EA0((((MonShowData *)arg1)->ballRotation)) != 1)) {
            break;
        }
        if (((MonShowData *)arg1)->btlMonObjData != NULL) {
            ov07_0221FE08(((MonShowData *)arg1)->btlMonObjData);
        }
        Pokepic_StartPaletteFade((((MonShowData *)arg1)->battlerData)->monSprite, 0x10, 0x10, 0, (s32) * (u16 *)(((u8 *)&ov12_0226D15A) + (((MonShowData *)arg1)->capturedBall * 2)));
        Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 6, 0);
        ov07_02232A44(((BallCapsuleSealEffect *)((MonShowData *)arg1)->ballCapsuleSealEffect));
        if (((MonShowData *)arg1)->face == 2) {
            sub_0200602C(0x706U, 0x75);
        } else {
            sub_0200602C(0x706U, -0x75);
        }
        ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
        break;
    case 3:
        if (ov07_02233E88((((MonShowData *)arg1)->ballRotation)) != 1) {
            ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
        }
    case 4:
        if ((Pokepic_GetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xC) == 0x100) && (ov07_02232A54(((BallCapsuleSealEffect *)((MonShowData *)arg1)->ballCapsuleSealEffect)) == 0)) {
            if (((MonShowData *)arg1)->face == 2) {
                Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0x2D, 0);
            }
            temp_r3_2 = ((MonShowData *)arg1)->battlerData;
            ov12_02261F38(*(BattleSystem **)arg1, ((MonShowData *)arg1)->battler, ((MonShowData *)arg1)->battlerType, temp_r3_2->monSprite, temp_r3_2->narc, (s32) ((MonShowData *)arg1)->species, (s32) ((MonShowData *)arg1)->formNum, (s32) ((MonShowData *)arg1)->face, *((s32 *)&((MonShowData *)arg1)->cryMod));
            Pokepic_StartPaletteFade((((MonShowData *)arg1)->battlerData)->monSprite, 0x10, 0, 0, (s32) * (u16 *)(((u8 *)&ov12_0226D15A) + (((MonShowData *)arg1)->capturedBall * 2)));
            ((MonShowData *)arg1)->state = 5U;
            break;
        }
        if (Pokepic_GetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xC) >= 0x100) {
            Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xC, 0x100);
            Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xD, 0x100);
            if (((MonShowData *)arg1)->face == 2) {
                Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0x2D, 0);
            }
            temp_r3_3 = ((MonShowData *)arg1)->battlerData;
            ov12_02261F38(*(BattleSystem **)arg1, ((MonShowData *)arg1)->battler, ((MonShowData *)arg1)->battlerType, temp_r3_3->monSprite, temp_r3_3->narc, (s32) ((MonShowData *)arg1)->species, (s32) ((MonShowData *)arg1)->formNum, (s32) ((MonShowData *)arg1)->face, *((s32 *)&((MonShowData *)arg1)->cryMod));
            Pokepic_StartPaletteFade((((MonShowData *)arg1)->battlerData)->monSprite, 0x10, 0, 1, (s32) * (u16 *)(((u8 *)&ov12_0226D15A) + (((MonShowData *)arg1)->capturedBall * 2)));
            ((MonShowData *)arg1)->state = 5U;
            break;
        }
        Pokepic_AddAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xC, 0x20);
        Pokepic_AddAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xD, 0x20);
        sub_0200914C((((MonShowData *)arg1)->battlerData)->monSprite, (s32) ((MonShowData *)arg1)->height);
        break;
    case 5:
        if (ov07_02232A54(((BallCapsuleSealEffect *)((MonShowData *)arg1)->ballCapsuleSealEffect)) == 0) {
            ((MonShowData *)arg1)->state = 6U;
            break;
        }
        break;
    case 6:
        if ((sub_02017068(ov12_0223B750(*(BattleSystem **)arg1), (s32) ((MonShowData *)arg1)->battler) != 1) || (Pokepic_IsAnimActive((((MonShowData *)arg1)->battlerData)->monSprite) != 0)) {

        } else {
            ov07_02233ECC((((MonShowData *)arg1)->ballRotation));
            ov07_02232AB8(((BallCapsuleSealEffect *)((MonShowData *)arg1)->ballCapsuleSealEffect));
            if (((MonShowData *)arg1)->isShiny != 0) {
                temp_r1_3 = ((MonShowData *)arg1)->battler;
                BattleController_SetMoveAnimation(*(BattleSystem **)arg1, NULL, &sp180, 1, 0xB, (s32)temp_r1_3, (s32)temp_r1_3, 0U);
                ov12_02261B80(*(BattleSystem **)arg1, ((MonShowData *)arg1)->battlerData, (BattleAnimSystem *)temp_r5, &sp180);
                ((MonShowData *)arg1)->state = 7U;
                break;
            }
            ((MonShowData *)arg1)->state = 8U;
            break;
        }
        break;
    case 7:
    case 9:
    case 11:
        ov07_0221C394(temp_r0);
        if (ov07_0221C3B0(temp_r5) == 0) {
            ov07_0221C3C0(temp_r5);
            ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
            break;
        }
        break;
    case 8:
        if (*((s32 *)&((MonShowData *)arg1)->isSubstitute) != 0) {
            temp_r1_4 = ((MonShowData *)arg1)->battler;
            BattleController_SetMoveAnimation(*(BattleSystem **)arg1, NULL, &sp128, 1, 0xF, (s32)temp_r1_4, (s32)temp_r1_4, 0U);
            ov12_02261B80(*(BattleSystem **)arg1, ((MonShowData *)arg1)->battlerData, (BattleAnimSystem *)temp_r5, &sp128);
            ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
            break;
        }
        ((MonShowData *)arg1)->state = 0xFFU;
        break;
    case 10:
        ov12_02261CA8(*(BattleSystem **)arg1, &((MonShowData *)arg1)->moveAnim, &spD8, ((MonShowData *)arg1)->battler);
        ov07_02234A20(&spD8, HEAP_ID_BATTLE);
        temp_r1_5 = ((MonShowData *)arg1)->battler;
        BattleController_SetMoveAnimation(*(BattleSystem **)arg1, NULL, &sp80, 1, 0x10, (s32)temp_r1_5, (s32)temp_r1_5, 0U);
        ov12_02261B80(*(BattleSystem **)arg1, ((MonShowData *)arg1)->battlerData, (BattleAnimSystem *)temp_r5, &sp80);
        (((MonShowData *)arg1)->battlerData)->isAlternateSprite = 1;
        ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
        break;
    default:
        BattleController_EmitClearCommand(*(BattleSystem **)arg1, (s32) ((MonShowData *)arg1)->battler, (s32) ((MonShowData *)arg1)->command);
        Heap_Free(arg1);
        SysTask_Destroy(arg0);
        break;
    }
}
