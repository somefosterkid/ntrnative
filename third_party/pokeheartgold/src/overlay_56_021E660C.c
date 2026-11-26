#include "global.h"
#include "sprite_resources_header.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "palette.h"
#include "constants/heap.h"
#include "yes_no_prompt.h"
#include "overlay_55.h"
#include "overlay_56.h"
#include "overlay_56_mail_viewer_app.h"
#include "overlay_41_sprite_resource_collection.h"
#include "list_menu_items.h"

struct FieldSystem;
struct TVEpisode;

struct Background;

struct BgConfig;

struct ColoredArrow;

struct MailMessage;

struct G2dRenderer;

struct ManagedSprite;

struct Menu;

struct MenuTemplate;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct NARC;

struct Options;

struct PaletteBuffer;

struct PaletteData;

struct SelectedPaletteData;

struct GF_2DGfxRawResObj;

struct GF_2DGfxRawResMan;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceCollection;

struct GF_2DGfxResObjList;

struct GF_2DGfxResHeader;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct String;

struct ListMenuItem;

struct SysTask;

struct SysTaskQueue;

struct Window;

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

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};


typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};

enum BgLayer {
    BG_LAYER_MAIN_0 = 0,
    BG_LAYER_MAIN_1,
    BG_LAYER_MAIN_2,
    BG_LAYER_MAIN_3,
    BG_LAYER_SUB_0,
    BG_LAYER_SUB_1,
    BG_LAYER_SUB_2,
    BG_LAYER_SUB_3,

    BG_LAYER_MAX,
};

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

enum StandardWindowType {
    STANDARD_WINDOW_SYSTEM = 0,
    STANDARD_WINDOW_FIELD,
    STANDARD_WINDOW_UNDERGROUND,
};

#if defined(SDK_PORT) || defined(NONMATCHING)
void LoadUserFrameGfx1(BgConfig *bgConfig, GFBgLayer bgLayer, u16 tileOffset, u8 palOffset, u8 standardWindowType, u32 heapID);
#else
void LoadUserFrameGfx1(BgConfig *bgConfig, u8 bgLayer, u16 tileOffset, u8 palOffset, u8 standardWindowType, u32 heapID);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void LoadUserFrameGfx2(BgConfig *bgConfig, GFBgLayer bgLayer, u16 tileOffset, u8 palOffset, u8 messageBoxFrame, u32 heapID);
#else
void LoadUserFrameGfx2(BgConfig *bgConfig, u8 bgLayer, u16 tileOffset, u8 palOffset, u8 messageBoxFrame, u32 heapID);
#endif

SysTask * SysTask_CreateOnMainQueue(SysTaskFunc func, void *data, u32 priority);

void ov56_021E63FC(SysTask *sysTask, void *data);

void ov56_021E660C(MailViewerApp *mailViewerApp);

void ov56_021E696C(MailViewerApp *mailViewerApp);
void ov56_021E6A7C(MailViewerApp *mailViewerApp);

void ov56_021E660C(MailViewerApp *mailViewerApp) {
    FreeBgTilemapBuffer(mailViewerApp->bgConfig, BG_LAYER_SUB_0);
    FreeBgTilemapBuffer(mailViewerApp->bgConfig, BG_LAYER_MAIN_3);
    FreeBgTilemapBuffer(mailViewerApp->bgConfig, BG_LAYER_MAIN_2);
    FreeBgTilemapBuffer(mailViewerApp->bgConfig, BG_LAYER_MAIN_1);
    FreeBgTilemapBuffer(mailViewerApp->bgConfig, BG_LAYER_MAIN_0);
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    Heap_Free(mailViewerApp->bgConfig);
}

void ov56_021E6650(MailViewerApp *mailViewerApp);

void ov56_021E6650(MailViewerApp *mailViewerApp) {
    NNSG2dCharacterData *charData;
    NNSG2dPaletteData *plttData;
    int tilesNarcIndex = 12 + mailViewerApp->args->mailType;
    int screenDataNarcIndex = 24 + mailViewerApp->args->mailType;
    int plttNarcIndex = 0 + mailViewerApp->args->mailType;
    NARC *narc = NARC_New(NARC_a_0_7_9, mailViewerApp->heapID);

    LoadUserFrameGfx1(mailViewerApp->bgConfig, BG_LAYER_MAIN_0, 1, 4, STANDARD_WINDOW_SYSTEM, mailViewerApp->heapID);
    LoadUserFrameGfx2(mailViewerApp->bgConfig, BG_LAYER_MAIN_0, 1 + (3 * 3), 6, mailViewerApp->frame, mailViewerApp->heapID);

    u32 size = NARC_GetMemberSize(narc, tilesNarcIndex);
    void *narcMemberBuffer = Heap_AllocAtEnd(mailViewerApp->heapID, size);
    NARC_ReadWholeMember(narc, tilesNarcIndex, narcMemberBuffer);

    NNS_G2dGetUnpackedCharacterData(narcMemberBuffer, &charData);
    BG_LoadCharTilesData(mailViewerApp->bgConfig, BG_LAYER_MAIN_1, charData->pRawData, charData->szByte, 0);
    BG_LoadCharTilesData(mailViewerApp->bgConfig, BG_LAYER_SUB_0, charData->pRawData, charData->szByte, 0);
    Heap_Free(narcMemberBuffer);

    size = NARC_GetMemberSize(narc, plttNarcIndex);
    narcMemberBuffer = Heap_AllocAtEnd(mailViewerApp->heapID, size);
    NARC_ReadWholeMember(narc, plttNarcIndex, narcMemberBuffer);

    NNS_G2dGetUnpackedPaletteData(narcMemberBuffer, &plttData);
    BG_LoadPlttData(BG_LAYER_SUB_0, plttData->pRawData, plttData->szByte, 0);

    mailViewerApp->paletteData = PaletteData_Init(mailViewerApp->heapID);

    PaletteData_AllocBuffers(mailViewerApp->paletteData, PLTTBUF_MAIN_BG, 7 * 32, mailViewerApp->heapID);
    PaletteData_AllocBuffers(mailViewerApp->paletteData, PLTTBUF_MAIN_OBJ, 3 * 32, mailViewerApp->heapID);
    PaletteData_LoadPalette(mailViewerApp->paletteData, plttData->pRawData, PLTTBUF_MAIN_BG, 0, 3 * 32);

    if (mailViewerApp->mode == MAIL_VIEWER_MODE_WRITE) {
        PaletteData_LoadPalette(mailViewerApp->paletteData, &((u16 *)plttData->pRawData)[3 * 16], PLTTBUF_MAIN_BG, 1 * 16, 32);
    }

    PaletteData_LoadNarc(mailViewerApp->paletteData, NARC_poketool_icongra_poke_icon, 0, mailViewerApp->heapID, PLTTBUF_MAIN_OBJ, 3 * 32, 0 * 16);
    PaletteData_LoadNarc(mailViewerApp->paletteData, NARC_graphic_font, 7, mailViewerApp->heapID, PLTTBUF_MAIN_BG, 32, 3 * 16);
    PaletteData_LoadNarc(mailViewerApp->paletteData, NARC_graphic_font, 8, mailViewerApp->heapID, PLTTBUF_MAIN_BG, 32, 5 * 16);
    PaletteData_LoadNarc(mailViewerApp->paletteData, NARC_a_0_3_8, 25, mailViewerApp->heapID, PLTTBUF_MAIN_BG, 32, 4 * 16);
    PaletteData_LoadNarc(mailViewerApp->paletteData, NARC_a_0_3_8, 26 + mailViewerApp->frame, mailViewerApp->heapID, PLTTBUF_MAIN_BG, 32, 6 * 16);
    PaletteData_BlendPalette(mailViewerApp->paletteData, PLTTBUF_MAIN_BG, 0, 16 * 7, 16, 0);
    PaletteData_BlendPalette(mailViewerApp->paletteData, PLTTBUF_MAIN_OBJ, 0, 16 * 3, 16, 0);
    PaletteData_SetAutoTransparent(mailViewerApp->paletteData, TRUE);
    PaletteData_PushTransparentBuffers(mailViewerApp->paletteData);
    Heap_Free(narcMemberBuffer);

    size = NARC_GetMemberSize(narc, screenDataNarcIndex);
    mailViewerApp->backgroundNSCRBuffer = Heap_Alloc(mailViewerApp->heapID, size);
    NARC_ReadWholeMember(narc, screenDataNarcIndex, mailViewerApp->backgroundNSCRBuffer);
    NNS_G2dGetUnpackedScreenData(mailViewerApp->backgroundNSCRBuffer, &mailViewerApp->backgroundScreenData);

    size = NARC_GetMemberSize(narc, 36);
    mailViewerApp->interfaceNSCRBuffer = Heap_Alloc(mailViewerApp->heapID, size);
    NARC_ReadWholeMember(narc, 36, mailViewerApp->interfaceNSCRBuffer);
    NNS_G2dGetUnpackedScreenData(mailViewerApp->interfaceNSCRBuffer, &mailViewerApp->interfaceScreenData);

    NARC_Delete(narc);
    FillBgTilemapRect(mailViewerApp->bgConfig, BG_LAYER_SUB_0, (2 << 12) | 1, 0, 0, 32, 32, 17);
    CopyToBgTilemapRect(mailViewerApp->bgConfig, BG_LAYER_MAIN_3, 0, 0, 32, 24, mailViewerApp->backgroundScreenData->rawData, 0, 0, mailViewerApp->backgroundScreenData->screenWidth / 8, mailViewerApp->backgroundScreenData->screenHeight / 8);
    ScheduleBgTilemapBufferTransfer(mailViewerApp->bgConfig, BG_LAYER_MAIN_3);
    ScheduleBgTilemapBufferTransfer(mailViewerApp->bgConfig, BG_LAYER_SUB_0);

    if (mailViewerApp->mode != MAIL_VIEWER_MODE_READ) {
        CopyToBgTilemapRect(mailViewerApp->bgConfig, BG_LAYER_MAIN_2, 0, 0, 32, 24, mailViewerApp->interfaceScreenData->rawData, 0, 0, mailViewerApp->interfaceScreenData->screenWidth / 8, mailViewerApp->interfaceScreenData->screenHeight / 8);
        ScheduleBgTilemapBufferTransfer(mailViewerApp->bgConfig, BG_LAYER_MAIN_2);

        mailViewerApp->blendFraction = 0;
        mailViewerApp->blendDirection = 0;

        SysTask_CreateOnMainQueue(ov56_021E63FC, mailViewerApp, 0);
        mailViewerApp->unk34 = ov56_021E5D40;
        mailViewerApp->unk38 = ov56_021E5D34;
    }
}

void ov56_021E692C(MailViewerApp *mailViewerApp);

void ov56_021E692C(MailViewerApp *mailViewerApp) {
    Heap_Free(mailViewerApp->interfaceNSCRBuffer);
    Heap_Free(mailViewerApp->backgroundNSCRBuffer);
    PaletteData_FreeBuffers(mailViewerApp->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(mailViewerApp->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_Free(mailViewerApp->paletteData);

    mailViewerApp->paletteData = NULL;

    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_NONE, 31, 0);
}

void ov56_021E696C(MailViewerApp *mailViewerApp) {
    AddWindowParameterized(mailViewerApp->bgConfig, &mailViewerApp->windows[MAIL_VIEWER_WINDOW_SENTENCE_1], 1, 3, 3, 0x1a, 4, 1, 0x397);
    AddWindowParameterized(mailViewerApp->bgConfig, &mailViewerApp->windows[MAIL_VIEWER_WINDOW_SENTENCE_2], 1, 3, 8, 0x1a, 4, 1, 0x32f);
    AddWindowParameterized(mailViewerApp->bgConfig, &mailViewerApp->windows[MAIL_VIEWER_WINDOW_SENTENCE_3], 1, 3, 0xd, 0x1a, 4, 1, 0x2c7);
    AddWindowParameterized(mailViewerApp->bgConfig, &mailViewerApp->windows[MAIL_VIEWER_WINDOW_CONFIRM], 1, 3, 0x14, 8, 2, 1, 0x2b7);
    AddWindowParameterized(mailViewerApp->bgConfig, &mailViewerApp->windows[MAIL_VIEWER_WINDOW_CANCEL], 1, 0x15, 0x14, 8, 2, 1, 0x2a7);
    AddWindowParameterized(mailViewerApp->bgConfig, &mailViewerApp->windows[MAIL_VIEWER_WINDOW_MESSAGE], 0, 2, 0x13, 0x1b, 4, 5, 0x23b);
    s32 iVar2 = 0;
    Window *window = &mailViewerApp->windows[0];
    do {
        FillWindowPixelBuffer(window, 0);
        if (iVar2 < 5) {
            CopyWindowToVram(window);
        }
        iVar2 = iVar2 + 1;
        window = window + 1;
    } while (iVar2 < 6);
    mailViewerApp->yesNoPrompt = YesNoPrompt_Create(mailViewerApp->heapID);
    return;
}

void ov56_021E6A7C(MailViewerApp *mailViewerApp) {
    Window *window;
    s32 iVar1;

    YesNoPrompt_Destroy(mailViewerApp->yesNoPrompt);
    iVar1 = 0;
    window = &mailViewerApp->windows[0];
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 6);
    return;
}
