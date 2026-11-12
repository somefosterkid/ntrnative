// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov312/geonet.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "field/unity_tower.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/math_util.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nitro/memorymap.h"
#include "nitro/math.h"
#include "save/save_control.h"
#include "system/app_keycursor.h"
#include "system/app_taskmenu.h"
#include "system/bmp_menu.h"
#include "system/bmp_menulist.h"
#include "system/bmp_menuwork.h"
#include "system/bmp_winframe.h"
#include "system/country_region.h"
#include "system/game_beacon.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/version.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/event_mapchange_2.h"

// Geonet, the globe of the Pokémon Centers: the player registers where they live, and the globe shows the places of
// the people they have traded and battled with. The ROM has no name for this file; "geonet.c" is a guess. It is the
// Generation 4 globe's code (pokeplatinum's wifi_earth) moved to this game's libraries

#define HEAPID_GEONET 0x7c
// Black's, three more than Black 2's 194 (geonet.c's code)
// The globe's models and the screens' graphics
#define ARCID_GEONET 197
// The places of the globe: member 18 has those of the countries, and each country with regions has a member of its
// own
#define ARCID_GEONET_PLACE 53
#define NARC_GEONET_PLACE_COUNTRIES 18

#define NARC_GEONET_EARTH_NSBMD 0
#define NARC_GEONET_BG_NCGR 5
#define NARC_GEONET_BG_NCLR 6
#define NARC_GEONET_BG_NSCR 7

#define COUNTRY_USA 220

#define PLACE_MAX 1024

// The marks: one for each UNITY_TOWER_REGION_* state but none, and the one at the middle of the screen
#define MARK_CURSOR 4
#define MARK_COUNT 5

#define BUTTON_VIEW 0
#define BUTTON_QUIT 1
#define BUTTON_COUNT 2

#define WINDOW_INFO 0
#define WINDOW_PLACE 3
#define WINDOW_COUNT 4

// The frames' characters and palettes
#define FRAME_MESSAGE_CHAR 4
#define FRAME_MESSAGE_PALETTE 6
#define FRAME_MENU_CHAR 0x22
#define FRAME_MENU_PALETTE 7

// How far the camera is from the globe, and how fast it moves between the two
#define CAMERA_DISTANCE_NEAR (FX32_ONE * 80)
#define CAMERA_DISTANCE_FAR (FX32_ONE * 296)
#define CAMERA_SPEED (FX32_ONE * 8)

// The globe's rotation in the Japanese game, which shows only Japan until the player has met a foreign place
#define JAPAN_ROTATION_X_MIN 0x1300
#define JAPAN_ROTATION_X_MAX 0x2020
#define JAPAN_ROTATION_Y_MIN (-0x3380)
#define JAPAN_ROTATION_Y_MAX (-0x27e0)
#define WORLD_ROTATION_X_MAX 0x3e00

enum {
    SEQ_INIT,
    SEQ_FADE_IN,
    SEQ_WELCOME,
    SEQ_MENU_MESSAGE,
    SEQ_MENU,
    SEQ_REGISTER_MESSAGE,
    SEQ_REGISTER_CONFIRM,
    SEQ_COUNTRY_LIST,
    SEQ_COUNTRY_MESSAGE,
    SEQ_COUNTRY_SELECT,
    SEQ_REGION_LIST,
    SEQ_REGION_MESSAGE,
    SEQ_REGION_SELECT,
    SEQ_PLACE_SHOW,
    SEQ_PLACE_MESSAGE,
    SEQ_PLACE_CONFIRM,
    SEQ_EARTH_INIT,
    SEQ_EARTH,
    SEQ_EARTH_ZOOM,
    SEQ_EARTH_QUIT,
    SEQ_FADE_OUT,
    SEQ_END,
};

enum {
    DRAW_NONE,
    DRAW_EARTH,
    DRAW_CLEAR,
};

typedef struct {
    s16 x;
    s16 y;
    MtxFx33 rotation;
    // UNITY_TOWER_REGION_*
    u16 state;
    u16 country;
    u16 region;
} GeonetPlace;

// A country's entry in the places' file, and a region's in its country's file
typedef struct {
    u16 state;
    s16 x;
    s16 y;
} GeonetCountryPlace;

typedef struct {
    s16 x;
    s16 y;
} GeonetRegionPlace;

typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palette;
    u8 charOffset;
} GeonetWindowSetup;

typedef struct {
    BmpWin *window;
    PrintWindow print;
} GeonetWindow;

typedef struct {
    u32 message;
    u32 value;
} GeonetMenuItem;

typedef struct {
    u16 mark;
    u16 file;
} GeonetMarkFile;

typedef struct {
    int x;
    int y;
    int width;
} GeonetButtonPos;

typedef struct {
    int x;
    int y;
} GeonetAngles;

typedef struct {
    int heapId;
    GameData *gameData;
    SaveControl *save;
    PlayerInfo *playerInfo;
    UnityTowerSurveySave *survey;
    u32 placeCount;
    GeonetPlace places[PLACE_MAX];
    BmpWin *listWindow;
    GeonetWindow windows[WINDOW_COUNT];
    BmpMenuList *list;
    ListMenuOption *options;
    BmpMenu *yesNo;
    AppTaskMenuRes *buttonRes;
    AppTaskMenuWin *buttons[BUTTON_COUNT];
    StrBuf *buttonText[BUTTON_COUNT];
    Font *font;
    MsgData *msgData;
    PrintStream *stream;
    PrintWindow listPrint;
    PrintQueue *queue;
    u8 textColors[3];
    KeyCursor *keyCursor;
    AppTaskMenuItem buttonItems[BUTTON_COUNT];
    TCBExManager *tcbManager;
    WordSet *wordSet;
    StrBuf *message;
    int messageState;
    u8 unusedC0B4[0x228];
    VecFx32 translation;
    VecFx32 scale;
    VecFx32 rotation;
    VecFx32 markScale;
    // Whether the camera is near the globe
    u16 zoom;
    int drawState;
    int unusedC314;
    int isJapan;
    // Whether the whole globe can be seen
    u16 worldMode;
    // The keys that the touch screen stands for
    int touchKeys;
    int touchState;
    int touchPrevX;
    int touchPrevY;
    int touchMoveX;
    int touchMoveY;
    int tapTimer;
    int myCountry;
    int myRegion;
    int selCountry;
    int selRegion;
    int foreignRegionMet;
    // Whether the window shows the place at the middle of the screen
    int infoMode;
    u32 keysPressed;
    u32 keysHeld;
    BOOL touchPressed;
    BOOL touchHeld;
    u32 touchX;
    u32 touchY;
    G3DCamera *camera;
    G3DLight *light;
    void *earthRes;
    void *markRes[MARK_COUNT];
    G3DModel *earthModel;
    G3DModel *markModels[MARK_COUNT];
    G3DActor *earthActor;
    G3DActor *markActors[MARK_COUNT];
} GeonetWork;

static BOOL func_ov219_02183b60(GameProc *proc, u32 *state, void *param, void *procWork);
static BOOL func_ov219_02183c78(GameProc *proc, u32 *state, void *param, void *procWork);
void func_ov219_02183cbc(GeonetWork *work, int button);
static void func_ov219_02183cec(GeonetWork *work);
static BOOL func_ov219_02183d14(GeonetWork *work, u32 *state);
static BOOL func_ov219_02184470(GameProc *proc, u32 *state, void *param, void *procWork);
static void func_ov219_021844e4(void);
static void func_ov219_02184558(HeapID heapId);
static void func_ov219_021845a4(void);
static void func_ov219_021845b4(GeonetWork *work);
static void func_ov219_021846b8(GeonetWork *work, int index, s16 x, s16 y, u16 country, u16 region);
void func_ov219_02184734(GeonetWork *work);
u32 func_ov219_02184790(u32 country);
void func_ov219_02184798(GeonetWork *work);
void func_ov219_021848f4(GeonetWork *work, int prevX, int prevY, int *keysX, int *moveX, int *keysY,
                                int *moveY);
BmpWin *func_ov219_02184968(const GeonetWindowSetup *setup, int show);
void func_ov219_021849a4(GeonetWork *work, ArcTool *arc);
void func_ov219_02184bac(GeonetWork *work);
void func_ov219_02184c2c(GeonetWork *work, int button);
void func_ov219_02184c8c(GeonetWork *work, int button);
void func_ov219_02184cb0(GeonetWork *work);
void func_ov219_02184d30(GeonetWork *work, PrintWindow *print, BmpWin *window, StrBuf *strbuf, u8 x, u8 y);
BOOL func_ov219_02184d70(GeonetWork *work, u32 message, BOOL close);
void func_ov219_02184eac(BmpMenuList *list, intptr_t value, u8 init);
void func_ov219_02184ec0(GeonetWork *work, const GeonetWindowSetup *setup, const BmpMenuListHeader *header, const GeonetMenuItem *items);
static void func_ov219_02184fc8(GeonetWork *work, const BmpMenuListHeader *header, u32 count);
void func_ov219_02185074(GeonetWork *work, const GeonetWindowSetup *setup, const BmpMenuListHeader *header, u32 textBank);
void func_ov219_021850ec(GeonetWork *work, const GeonetWindowSetup *setup, const BmpMenuListHeader *header, u32 textBank, const u8 *order, u32 count);
void func_ov219_0218516c(GeonetWork *work);
void func_ov219_0218519c(GeonetWork *work);
void func_ov219_02185254(GeonetWork *work, u32 country, u32 region);
void func_ov219_02185310(GeonetWork *work);
BOOL func_ov219_0218532c(GeonetWork *work, u32 *index);
void func_ov219_0218544c(GeonetWork *work);
void func_ov219_02185580(GeonetWork *work, ArcTool *arc);
void func_ov219_0218562c(GeonetWork *work);
void func_ov219_021856cc(GeonetWork *work);
void func_ov219_02185728(GeonetWork *work);
void func_ov219_02185794(GeonetWork *work);
void func_ov219_02185824(GeonetWork *work);
static void func_ov219_02185848(MtxFx33 *mtx, const VecFx32 *rotation);
static void func_ov219_021858bc(MtxFx33 *mtx, const VecFx32 *rotation);
BOOL func_ov219_02185934(GeonetWork *work, u32 pressed, u32 held);
BOOL func_ov219_02185ac8(GeonetWork *work);
static void func_ov219_02185b74(GeonetWork *work);
BOOL func_ov219_02185c84(u32 country, u32 region, StrBuf *countryName, StrBuf *regionName, int heapId);
BOOL func_ov219_02185cf4(u32 country);
void func_ov219_02185d08(GeonetAngles *angles);
u32 func_ov219_02185d64(const GeonetAngles *a, const GeonetAngles *b);

// pokebw2's sGeonetListWindow
static const GeonetWindowSetup data_ov219_02185dc4 = {6, 3, 2, 26, 13, 4, 0x3e};

// pokebw2's sGeonetLights
static const LightSetup data_ov219_02185ddc[] = {
    {0, {{0, 0, -FX16_ONE + 1}, GX_RGB(31, 31, 31)}},
};

// pokebw2's sGeonetMenuWindow
static const GeonetWindowSetup data_ov219_02185db6 = {6, 18, 11, 13, 6, 4, 0x42};

static const LightSetupList data_ov219_02185dd4 = {data_ov219_02185ddc, NELEMS(data_ov219_02185ddc)};

// The View and Quit buttons
static const TouchRect data_ov219_02185e00[] = {
    {168, 191, 0, 71},
    {168, 191, 184, 255},
    {TOUCH_RECT_END, 0, 0, 0},
};

static const VecFx32 data_ov219_02185df4 = {0, 0, 0};

// pokebw2's sGeonetYesNoSetup
static const ConfirmDialogSetup data_ov219_02185db0 = {6, 24, 13, 4, 0x174};

static const GeonetMarkFile data_ov219_02185e18[] = {
    {UNITY_TOWER_REGION_OWN, 1},
    {MARK_CURSOR, 2},
    {UNITY_TOWER_REGION_TODAY, 3},
    {UNITY_TOWER_REGION_BEFORE, 4},
};

static const GeonetButtonPos data_ov219_02185e40[BUTTON_COUNT] = {
    {0, 21, 9},
    {23, 21, 9},
};

static const GeonetWindowSetup data_ov219_02185dbd = {6, 2, 19, 28, 4, 4, 0x90};

static const VecFx32 data_ov219_02185e0c = {0, 0, CAMERA_DISTANCE_FAR};

// pokebw2's sGeonetMenuItems
static const GeonetMenuItem data_ov219_02185e28[] = {
    {EventMapchange2_Text_010, 0},
    {EventMapchange2_Text_011, 1},
    {EventMapchange2_Text_012, 2},
};

static const BGSetup data_ov219_02185e58 = {0, 0, 0x800, 0, 1, 0, 14, 0, 0x4000, 0, 0, 0, 0};

// pokebw2's sGeonetMenuHeader
static const BmpMenuListHeader data_ov219_02185e98 = {
    NULL, NULL, NULL, NULL, 3, 3, 0, 12, 0, 0, 1, 15, 2, 0, 0, BMPMENULIST_SKIP_NONE, 0, BMPMENULIST_CURSOR_SHOW,
    NULL, 0, 0, NULL, NULL, NULL, NULL,
};

// pokebw2's sGeonetListHeader
static const BmpMenuListHeader data_ov219_02185ecc = {
    NULL, NULL, NULL, NULL, 0, 7, 0, 12, 0, 0, 1, 15, 2, 0, 0, BMPMENULIST_SKIP_LR_KEY, 0, BMPMENULIST_CURSOR_SHOW,
    NULL, 0, 0, NULL, NULL, NULL, NULL,
};

// pokebw2's GeonetProc_End
// pokebw2's GeonetProc_Init
// pokebw2's GeonetProc_Main
const GameProcFunctions data_ov219_02185f0c = {func_ov219_02183b60, func_ov219_02183c78, func_ov219_02184470};

static const GeonetWindowSetup data_ov219_02185dcb = {6, 2, 1, 27, 6, 4, 0xd2};

// Where the globe is dragged: the screen above the buttons, and the gap between them
// pokebw2's sGeonetTouchRects
static const TouchRect data_ov219_02185de6[] = {
    {0, 167, 0, 239},
    {168, 191, 72, 183},
    {TOUCH_RECT_END, 0, 0, 0},
};

static const BGSetup data_ov219_02185e78 = {0, 0, 0x800, 0, 1, 0, 15, 1, 0x4000, 0, 3, 0, 0};

static BOOL func_ov219_02183b60(GameProc *proc, u32 *state, void *param, void *procWork) {
    GeonetWork *work;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_GEONET, 0x80000);
    work = GFL_ProcInitSubsystem(proc, sizeof(GeonetWork), HEAPID_GEONET);
    MI_CpuFill8(work, 0, sizeof(GeonetWork));
    work->heapId = HEAPID_GEONET;
    work->gameData = param;
    work->save = GameData_GetSaveControl(param);
    work->playerInfo = GetGameDataPlayerInfo(work->gameData);
    work->survey = getUnityTower_SurveySaveBlkAddrress(work->save);
    if (region == 1) {
        work->isJapan = TRUE;
    } else {
        work->isJapan = FALSE;
    }
    work->myCountry = UnityTowerVisitor_GetCountry(work->playerInfo);
    work->myRegion = UnityTowerVisitor_GetProvince(work->playerInfo);
    work->foreignRegionMet = func_02009524(work->survey);
    work->buttons[BUTTON_VIEW] = NULL;
    work->buttons[BUTTON_QUIT] = NULL;
    func_ov219_021844e4();
    func_ov219_02184558(work->heapId);
    work->wordSet = GFL_WordSetSystemCreate(8, 64, work->heapId);
    work->drawState = DRAW_NONE;
    GFL_BGSysSetDisplayLayout(GX_DISP_SELECT_SUB_MAIN);
    work->tcbManager = GFL_TCBExMgrCreate(work->heapId, work->heapId, 32, 32);
    work->keyCursor = func_02028590(15, TRUE, FALSE, work->heapId);
    work->queue = func_0201c440(work->heapId);
    func_0203d34c(TRUE, work->heapId);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    func_ov219_021845b4(work);
    return TRUE;
}

static BOOL func_ov219_02183c78(GameProc *proc, u32 *state, void *param, void *procWork) {
    GeonetWork *work = procWork;
    BOOL result;

    GFL_TCBExMgrUpdate(work->tcbManager);
    result = func_ov219_02183d14(work, state);
    if (work->earthActor != NULL) {
        func_ov219_02185b74(work);
    }
    func_ov219_02183cec(work);
    func_0201c4e4(work->queue);
    return result;
}

// Shows a button as the cursor's, or makes the quit button flash
// pokebw2's Geonet_PressButton
void func_ov219_02183cbc(GeonetWork *work, int button) {
    if (work->buttons[button] != NULL) {
        if (button == BUTTON_VIEW) {
            func_02028218(work->buttons[BUTTON_VIEW], TRUE);
        } else if (button == BUTTON_QUIT) {
            func_0202822c(work->buttons[BUTTON_QUIT], TRUE);
        }
    }
}

// pokebw2's Geonet_UpdateButtons
static void func_ov219_02183cec(GeonetWork *work) {
    if (work->buttons[BUTTON_VIEW] != NULL) {
        func_02028178(work->buttons[BUTTON_VIEW]);
    }
    if (work->buttons[BUTTON_QUIT] != NULL) {
        func_02028178(work->buttons[BUTTON_QUIT]);
    }
}

static BOOL func_ov219_02183d14(GeonetWork *work, u32 *state) {
    BOOL result = FALSE;

    work->keysPressed = GCTX_HIDGetPressedKeys();
    work->keysHeld = GCTX_HIDGetHeldKeys();
    if (func_02035ba4(data_ov219_02185de6) != TOUCH_RECT_NONE) {
        work->touchHeld = func_02035cb8(&work->touchX, &work->touchY);
    } else {
        work->touchHeld = FALSE;
    }
    if (work->touchHeld == TRUE) {
        work->touchPressed = func_02035c60();
    } else {
        work->touchPressed = func_02035d20(&work->touchX, &work->touchY);
    }

    switch (*state) {
    case SEQ_INIT: {
        ArcTool *arc;

        work->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_EVENT_MAPCHANGE_2, work->heapId);
        arc = GFL_ArcSysCreateFileHandle(ARCID_GEONET, work->heapId);
        func_ov219_02185580(work, arc);
        func_ov219_021849a4(work, arc);
        GFL_ArcToolFree(arc);
        work->unusedC314 = 0;
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, work->heapId);
        GFL_BGSysSetBGEnabled(6, TRUE);
        GFL_BGSysSetBGEnabled(2, TRUE);
        GFL_BGSysSetBGEnabled(7, TRUE);
        GFL_BGSysSetBGEnabled(3, TRUE);
        *state = SEQ_FADE_IN;
        break;
    }
    case SEQ_FADE_IN:
        if (GFL_WipeIsFinished() == TRUE) {
            *state = SEQ_WELCOME;
        }
        break;
    case SEQ_WELCOME:
        if (func_ov219_02184d70(work, EventMapchange2_Text_000, TRUE) == TRUE) {
            if (work->myCountry == COUNTRY_NONE) {
                *state = SEQ_MENU_MESSAGE;
            } else {
                *state = SEQ_EARTH_INIT;
            }
        }
        break;
    case SEQ_MENU_MESSAGE:
        if (func_ov219_02184d70(work, EventMapchange2_Text_001, TRUE) == TRUE) {
            func_ov219_02184ec0(work, &data_ov219_02185db6, &data_ov219_02185e98, data_ov219_02185e28);
            *state = SEQ_MENU;
        }
        break;
    case SEQ_MENU:
        if (func_0201c6b4(work->queue)) {
            u32 choice = (u32)BmpMenuList_Update(work->list);

            if (choice != BMPMENULIST_NULL) {
                func_ov219_0218516c(work);
                switch (choice) {
                default:
                case 0:
                    *state = SEQ_EARTH_INIT;
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                    break;
                case 1:
                    *state = SEQ_REGISTER_MESSAGE;
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                    break;
                case 2:
                case BMPMENULIST_CANCEL:
                    *state = SEQ_FADE_OUT;
                    GFL_SndSEPlay(SEQ_SE_CANCEL1);
                    break;
                }
            }
        }
        break;
    case SEQ_REGISTER_MESSAGE:
        if (func_ov219_02184d70(work, EventMapchange2_Text_002, TRUE) == TRUE) {
            work->yesNo =
                ShopUI_CreateConfirmDialog(&data_ov219_02185db0, FRAME_MENU_CHAR, FRAME_MENU_PALETTE, 0, work->heapId);
            *state = SEQ_REGISTER_CONFIRM;
        }
        break;
    case SEQ_REGISTER_CONFIRM:
        switch (func_0201febc(work->yesNo)) {
        case 0:
            if (work->isJapan == TRUE) {
                work->selCountry = func_02025be8();
                *state = SEQ_REGION_LIST;
            } else {
                *state = SEQ_COUNTRY_LIST;
            }
            break;
        case BMPMENU_CANCEL:
            *state = SEQ_MENU_MESSAGE;
            break;
        }
        break;
    case SEQ_COUNTRY_LIST:
        work->selCountry = COUNTRY_NONE;
        func_ov219_02185074(work, &data_ov219_02185dc4, &data_ov219_02185ecc,
                            TEXT_BANK_EVENT_MAPCHANGE_LOAD_COUNTRY_TO_STRBUF);
        *state = SEQ_COUNTRY_MESSAGE;
        break;
    case SEQ_COUNTRY_MESSAGE:
        if (func_ov219_02184d70(work, EventMapchange2_Text_003, TRUE) == TRUE) {
            *state = SEQ_COUNTRY_SELECT;
        }
        break;
    case SEQ_COUNTRY_SELECT:
        if (func_0201c6b4(work->queue)) {
            s32 choice = (s32)BmpMenuList_Update(work->list);

            if (choice != BMPMENULIST_NULL) {
                func_ov219_0218516c(work);
                switch (choice) {
                default:
                    work->selCountry = choice;
                    if (func_ov219_02185cf4(choice) == TRUE) {
                        *state = SEQ_REGION_LIST;
                    } else {
                        work->selRegion = 0;
                        *state = SEQ_PLACE_SHOW;
                    }
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                    break;
                case BMPMENULIST_CANCEL:
                    *state = SEQ_MENU_MESSAGE;
                    GFL_SndSEPlay(SEQ_SE_CANCEL1);
                    break;
                }
            }
        }
        break;
    case SEQ_REGION_LIST: {
        const u8 *order;
        u32 count;

        work->selRegion = 0;
        func_02025c44(work->selCountry, &order, &count);
        func_ov219_021850ec(work, &data_ov219_02185dc4, &data_ov219_02185ecc, fetchFileNumOfDividedCountry(work->selCountry),
                            order, count);
        *state = SEQ_REGION_MESSAGE;
        break;
    }
    case SEQ_REGION_MESSAGE:
        if (func_ov219_02184d70(work, EventMapchange2_Text_004, TRUE) == TRUE) {
            *state = SEQ_REGION_SELECT;
        }
        break;
    case SEQ_REGION_SELECT:
        if (func_0201c6b4(work->queue)) {
            s32 choice = (s32)BmpMenuList_Update(work->list);

            if (choice != BMPMENULIST_NULL) {
                func_ov219_0218516c(work);
                switch (choice) {
                default:
                    work->selRegion = choice;
                    *state = SEQ_PLACE_SHOW;
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                    break;
                case BMPMENULIST_CANCEL:
                    GFL_SndSEPlay(SEQ_SE_CANCEL1);
                    if (work->isJapan == TRUE) {
                        *state = SEQ_MENU_MESSAGE;
                    } else {
                        *state = SEQ_COUNTRY_LIST;
                    }
                    break;
                }
            }
        }
        break;
    case SEQ_PLACE_SHOW:
        func_ov219_02185254(work, work->selCountry, work->selRegion);
        *state = SEQ_PLACE_MESSAGE;
        break;
    case SEQ_PLACE_MESSAGE:
        if (func_ov219_02184d70(work, EventMapchange2_Text_005, TRUE) == TRUE) {
            work->yesNo =
                ShopUI_CreateConfirmDialog(&data_ov219_02185db0, FRAME_MENU_CHAR, FRAME_MENU_PALETTE, 0, work->heapId);
            *state = SEQ_PLACE_CONFIRM;
        }
        break;
    case SEQ_PLACE_CONFIRM:
        switch (func_0201febc(work->yesNo)) {
        case 0:
            func_ov219_02185310(work);
            func_0200947c(work->survey, work->playerInfo, work->selCountry, work->selRegion);
            func_02027120(work->selCountry, work->selRegion);
            work->myCountry = work->selCountry;
            work->myRegion = work->selRegion;
            *state = SEQ_EARTH_INIT;
            break;
        case BMPMENU_CANCEL:
            func_ov219_02185310(work);
            *state = SEQ_MENU_MESSAGE;
            break;
        }
        break;
    case SEQ_EARTH_INIT:
        if (work->isJapan == TRUE && work->foreignRegionMet == 0) {
            work->worldMode = FALSE;
        } else {
            work->worldMode = TRUE;
        }
        func_ov219_021856cc(work);
        func_ov219_02184734(work);
        func_ov219_02185794(work);
        GFL_BitmapFill(BmpWin_GetBitmap(work->windows[WINDOW_INFO].window), 15);
        BmpWin_FlushChar(work->windows[WINDOW_INFO].window);
        func_ov219_02184c2c(work, BUTTON_QUIT);
        if (work->myCountry != COUNTRY_NONE) {
            func_ov219_0218519c(work);
        }
        func_ov219_0218544c(work);
        work->infoMode = FALSE;
        work->drawState = DRAW_EARTH;
        *state = SEQ_EARTH;
        break;
    case SEQ_EARTH: {
        u16 zoom = work->zoom;
        u32 index;

        if (!func_0201c6b4(work->queue)) {
            break;
        }
        func_ov219_02184798(work);
        if (func_ov219_0218532c(work, &index)) {
            func_ov219_02184c2c(work, BUTTON_VIEW);
        } else {
            func_ov219_02184c8c(work, BUTTON_VIEW);
        }
        if ((work->keysPressed & PAD_BUTTON_B) || (work->touchKeys & PAD_BUTTON_B)) {
            func_ov219_02184c8c(work, BUTTON_VIEW);
            func_ov219_02183cbc(work, BUTTON_QUIT);
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            GFL_BitmapFill(BmpWin_GetBitmap(work->windows[WINDOW_INFO].window), 15);
            BmpWin_FlushChar(work->windows[WINDOW_INFO].window);
            *state = SEQ_EARTH_QUIT;
            break;
        }
        if (((work->keysPressed & PAD_BUTTON_X) || (work->touchKeys & PAD_BUTTON_X)) && work->infoMode == FALSE) {
            work->infoMode = TRUE;
            func_ov219_02183cbc(work, BUTTON_VIEW);
            func_ov219_0218544c(work);
            if (work->infoMode == TRUE) {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
            }
            break;
        }
        if (((work->keysPressed & (PAD_BUTTON_X | PAD_BUTTON_B | PAD_BUTTON_A)) || (work->touchKeys & PAD_BUTTON_X))
            && work->infoMode == TRUE) {
            work->infoMode = FALSE;
            func_ov219_0218544c(work);
            break;
        }
        if (func_ov219_02185934(work, work->keysPressed, work->keysHeld) == TRUE && work->infoMode == TRUE) {
            work->infoMode = FALSE;
            func_ov219_0218544c(work);
        }
        if (zoom != work->zoom) {
            *state = SEQ_EARTH_ZOOM;
            if (work->zoom == FALSE) {
                GFL_SndSEPlay(SEQ_SE_SYS_10);
            } else {
                GFL_SndSEPlay(SEQ_SE_SYS_09);
            }
        }
        break;
    }
    case SEQ_EARTH_QUIT:
        if (func_02028230(work->buttons[BUTTON_QUIT])) {
            if (work->myCountry == COUNTRY_NONE) {
                work->drawState = DRAW_CLEAR;
                *state = SEQ_MENU_MESSAGE;
            } else {
                func_ov219_02185310(work);
                *state = SEQ_FADE_OUT;
            }
            func_ov219_02184c8c(work, BUTTON_QUIT);
        }
        break;
    case SEQ_EARTH_ZOOM:
        if (func_ov219_02185ac8(work) == TRUE) {
            *state = SEQ_EARTH;
        }
        break;
    case SEQ_FADE_OUT:
        work->unusedC314 = 0;
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, work->heapId);
        *state = SEQ_END;
        break;
    case SEQ_END:
        if (GFL_WipeIsFinished() == TRUE) {
            work->drawState = DRAW_EARTH;
            result = TRUE;
            func_ov219_02184cb0(work);
            func_ov219_0218562c(work);
            GFL_MsgDataFree(work->msgData);
            *state = SEQ_INIT;
        }
        break;
    }

    PrintWindow_Flush(&work->listPrint, work->queue);
    PrintWindow_Flush(&work->windows[WINDOW_PLACE].print, work->queue);
    PrintWindow_Flush(&work->windows[2].print, work->queue);
    PrintWindow_Flush(&work->windows[1].print, work->queue);
    PrintWindow_Flush(&work->windows[WINDOW_INFO].print, work->queue);
    return result;
}

static BOOL func_ov219_02184470(GameProc *proc, u32 *state, void *param, void *procWork) {
    GeonetWork *work = procWork;
    int heapId = work->heapId;

    func_0201c6ec(work->queue);
    func_0201c4c0(work->queue);
    GFL_BGSysSetBGEnabled(6, FALSE);
    GFL_BGSysSetBGEnabled(2, FALSE);
    GFL_BGSysSetBGEnabled(7, FALSE);
    GFL_BGSysSetBGEnabled(3, FALSE);
    GFL_BGSysSetDisplayLayout(GX_DISP_SELECT_MAIN_SUB);
    func_02028604(work->keyCursor);
    GFL_TCBExMgrFree(work->tcbManager);
    GFL_WordSetSystemFree(work->wordSet);
    func_ov219_021845a4();
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(heapId);
    return TRUE;
}

// pokebw2's Geonet_SetVRAMBanks
static void func_ov219_021844e4(void) {
    BGSysVRAMConfig config = {
        GX_VRAM_C, GX_VRAM_BGEXTPLTT_NONE, GX_VRAM_H, GX_VRAM_SUB_BGEXTPLTT_NONE, GX_VRAM_F, GX_VRAM_OBJEXTPLTT_NONE,
        GX_VRAM_I, GX_VRAM_SUB_OBJEXTPLTT_NONE, GX_VRAM_A | GX_VRAM_B, GX_VRAM_E, 0x10, 0x10,
    };

    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MIi_CpuClearFast(0, (void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    GX_DisableBankForLCDC();
    MIi_CpuClearFast(0xc0, (void *)HW_OAM, HW_OAM_SIZE);
    MIi_CpuClearFast(0, (void *)HW_BG_PLTT, HW_PLTT_SIZE);
    MIi_CpuClearFast(0xc0, (void *)HW_DB_OAM, HW_OAM_SIZE);
    MIi_CpuClearFast(0, (void *)HW_DB_BG_PLTT, HW_PLTT_SIZE);
    GFL_BGSysSetVRAMBanks(&config);
}

// pokebw2's Geonet_InitGraphics
static void func_ov219_02184558(HeapID heapId) {
    BGSysLCDConfig config = {GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, TRUE};

    GFL_BGSysCreate(heapId);
    GFL_BGSysSetLCDConfig(&config);
    GFL_G3DSysCreate(FALSE, 3, FALSE, 2, 0, heapId, NULL);
    GFL_BGSysSet3DBGPriority(1);
    BmpWin_InitAllocator(heapId);
    GFL_BGSysEnableEngines();
}

// pokebw2's Geonet_FreeGraphics
static void func_ov219_021845a4(void) {
    BmpWin_FreeAllocator();
    GFL_G3DSysFree();
    GFL_BGSysFree();
}

// Reads where every country and region is on the globe. The first entry of each file is not a place
// pokebw2's Geonet_LoadPlaces
static void func_ov219_021845b4(GeonetWork *work) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_GEONET_PLACE, work->heapId);
    u32 size;
    u32 regionSize;
    GeonetCountryPlace *countries;
    GeonetRegionPlace *regions;
    GeonetCountryPlace *country;
    GeonetRegionPlace *region;
    int subdividedCount;
    int regionCount;
    int count;
    int i;
    int j;

    work->placeCount = 0;
    countries = GFL_ArcToolReadHeapNewLZGetLen(arc, NARC_GEONET_PLACE_COUNTRIES, FALSE, work->heapId, &size);
    count = size / sizeof(GeonetCountryPlace);
    country = countries + 1;
    for (i = 1; i < count; i++) {
        // A country with regions has its regions' places instead
        if (country->state != 2) {
            func_ov219_021846b8(work, work->placeCount, country->x, country->y, i, 0);
            work->placeCount++;
        }
        country++;
    }
    GFL_HeapFree(countries);

    for (j = 1, subdividedCount = func_02025b38(); j < subdividedCount; j++) {
        regions = GFL_ArcToolReadHeapNewLZGetLen(arc, func_02025bd0(j), FALSE, work->heapId, &regionSize);
        regionCount = regionSize / sizeof(GeonetRegionPlace);
        region = regions + 1;
        for (i = 1; i < regionCount; i++) {
            func_ov219_021846b8(work, work->placeCount, region->x, region->y, func_02025bb8(j), i);
            work->placeCount++;
            region++;
        }
        GFL_HeapFree(regions);
    }
    GFL_ArcToolFree(arc);
}

// pokebw2's Geonet_AddPlace
static void func_ov219_021846b8(GeonetWork *work, int index, s16 x, s16 y, u16 country, u16 region) {
    MtxFx33 rotation = {FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0, FX32_ONE};
    VecFx32 angles;

    work->places[index].x = x;
    work->places[index].y = y;
    angles.x = x;
    angles.y = y;
    angles.z = 0;
    func_ov219_021858bc(&rotation, &angles);
    work->places[index].rotation = rotation;
    work->places[index].state = func_0200949c(work->survey, country, region);
    work->places[index].country = country;
    work->places[index].region = region;
}

// Marks the player's place as their own, and turns the globe to it
// pokebw2's Geonet_MarkOwnPlace
void func_ov219_02184734(GeonetWork *work) {
    u32 i;

    for (i = 0; i < work->placeCount; i++) {
        if (work->myCountry == work->places[i].country && work->myRegion == work->places[i].region) {
            work->places[i].state = UNITY_TOWER_REGION_OWN;
            work->rotation.x = work->places[i].x;
            work->rotation.y = work->places[i].y;
        }
    }
}

// pokebw2's Geonet_GetSubdividedIndex
u32 func_ov219_02184790(u32 country) {
    return getSubdividedCountryArrayNum(country);
}

// Turns the touch screen into keys: the buttons into X and B, dragging into the directions, and a tap into A
// pokebw2's Geonet_ReadTouch
void func_ov219_02184798(GeonetWork *work) {
    int keys = 0;
    s32 button;
    int keysX;
    int moveX;
    int keysY;
    int moveY;

    work->touchKeys = 0;
    button = func_02035c08(data_ov219_02185e00);
    if (button == BUTTON_VIEW) {
        keys = PAD_BUTTON_X;
    } else if (button == BUTTON_QUIT) {
        keys = PAD_BUTTON_B;
    }

    if (work->touchPressed) {
        if (keys != 0) {
            work->touchKeys = keys;
            return;
        }
        work->touchState = 0;
        work->touchMoveX = 0;
        work->touchMoveY = 0;
        work->tapTimer = 0;
        work->touchKeys = 0;
        work->touchPrevX = work->touchX;
        work->touchPrevY = work->touchY;
        work->tapTimer = 4;
    }

    if (work->touchHeld) {
        switch (work->touchState) {
        case 0:
            if (work->tapTimer == 0) {
                work->touchState++;
            } else {
                work->tapTimer--;
            }
            // fallthrough
        case 1:
            if (keys == 0) {
                func_ov219_021848f4(work, work->touchPrevX, work->touchPrevY, &keysX, &moveX, &keysY, &moveY);
                work->touchKeys = keysX | keysY;
                work->touchMoveX = moveX;
                work->touchMoveY = moveY;
                work->touchPrevX = work->touchX;
                work->touchPrevY = work->touchY;
            }
            break;
        }
    } else {
        if (keys == 0 && work->tapTimer != 0 && work->touchMoveX < 4 && work->touchMoveY < 4) {
            work->touchKeys = PAD_BUTTON_A;
        }
        work->touchState = 0;
        work->touchMoveX = 0;
        work->touchMoveY = 0;
        work->tapTimer = 0;
    }
}

void func_ov219_021848f4(GeonetWork *work, int prevX, int prevY, int *keysX, int *moveX, int *keysY,
                                int *moveY) {
    int dirX = 0;
    int dirY = 0;
    int x = 0;
    int y = 0;

    if (work->touchX != 0xffff) {
        x = work->touchX - prevX;
        if (x < 0) {
            x ^= -1;
            dirX = PAD_KEY_RIGHT;
        } else if (x > 0) {
            dirX = PAD_KEY_LEFT;
        }
    }
    *keysX = dirX;
    *moveX = x & 0x3f;

    if (work->touchY != 0xffff) {
        y = work->touchY - prevY;
        if (y < 0) {
            y ^= -1;
            dirY = PAD_KEY_DOWN;
        } else if (y > 0) {
            dirY = PAD_KEY_UP;
        }
    }
    *keysY = dirY;
    *moveY = y & 0x3f;
}

// pokebw2's Geonet_CreateWindow
BmpWin *func_ov219_02184968(const GeonetWindowSetup *setup, int show) {
    BmpWin *window =
        BmpWin_CreateDynamic(setup->bg, setup->x, setup->y, setup->width, setup->height, setup->palette, TRUE);

    GFL_BitmapFill(BmpWin_GetBitmap(window), 15);
    if (show == TRUE) {
        BmpWin_FlushMap(window);
    }
    return window;
}

// pokebw2's Geonet_LoadBG
void func_ov219_021849a4(GeonetWork *work, ArcTool *arc) {
    work->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, work->heapId);
    GFL_TextRndGetGlobalColors(&work->textColors[0], &work->textColors[1], &work->textColors[2]);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);

    GFL_BGSysCreateBG(6, &data_ov219_02185e58, 0);
    GFL_BGSysClearScr(6);
    GFL_BGSysCreateBG(7, &data_ov219_02185e78, 0);
    GFL_BGSysLoadArcNCGRStatic(arc, NARC_GEONET_BG_NCGR, 7, 0, 0, FALSE, work->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, NARC_GEONET_BG_NCLR, PALTYPE_SUB_BG, 0, 0x80, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, NARC_GEONET_BG_NSCR, 7, 0, 0, FALSE, work->heapId);
    LoadSysMsgBox(6, FRAME_MESSAGE_CHAR, FRAME_MESSAGE_PALETTE, 0, work->heapId);
    LoadSysMsgBox(6, FRAME_MENU_CHAR, FRAME_MENU_PALETTE, 0, work->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 0x80, 0x20, work->heapId);
    GFL_BGSysClearCharCore(6, 0x20, 0, work->heapId);
    GFL_BGSysResetStdPalette(6, GX_RGB(19, 26, 17));
    work->windows[WINDOW_INFO].window = func_ov219_02184968(&data_ov219_02185dbd, TRUE);
    BmpWin_DrawFrame(work->windows[WINDOW_INFO].window, 0, FRAME_MESSAGE_CHAR, FRAME_MESSAGE_PALETTE);
    BmpWin_FlushChar(work->windows[WINDOW_INFO].window);
    work->messageState = 0;

    GFL_BGSysCreateBG(2, &data_ov219_02185e58, 0);
    GFL_BGSysClearScr(2);
    GFL_BGSysSet3DBGPriority(1);
    GFL_BGSysCreateBG(3, &data_ov219_02185e78, 0);
    GFL_BGSysLoadArcNCGRStatic(arc, NARC_GEONET_BG_NCGR, 3, 0, 0, FALSE, work->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, NARC_GEONET_BG_NCLR, PALTYPE_MAIN_BG, 0, 0x80, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, NARC_GEONET_BG_NSCR, 3, 0, 0, FALSE, work->heapId);
    LoadSysMsgBox(2, FRAME_MENU_CHAR, FRAME_MENU_PALETTE, 0, work->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 0x80, 0x20, work->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 0x100, 0x20, work->heapId);
    GFL_BGSysClearCharCore(2, 0x20, 0, work->heapId);
    GFL_BGSysResetStdPalette(2, GX_RGB(0, 0, 0));
    func_ov219_02184bac(work);
}

// pokebw2's Geonet_InitButtons
void func_ov219_02184bac(GeonetWork *work) {
    work->buttonText[BUTTON_VIEW] = GFL_MsgDataLoadStrbufNew(work->msgData, EventMapchange2_Text_013);
    work->buttonText[BUTTON_QUIT] = GFL_MsgDataLoadStrbufNew(work->msgData, EventMapchange2_Text_016);
    work->buttonRes = func_02027f60(2, 9, work->font, work->queue, work->heapId);
    work->buttonItems[BUTTON_VIEW].str = work->buttonText[BUTTON_VIEW];
    work->buttonItems[BUTTON_VIEW].color = PRINT_COLOR(14, 15, 3);
    work->buttonItems[BUTTON_VIEW].type = 0;
    work->buttonItems[BUTTON_QUIT].str = work->buttonText[BUTTON_QUIT];
    work->buttonItems[BUTTON_QUIT].color = PRINT_COLOR(14, 15, 3);
    work->buttonItems[BUTTON_QUIT].type = APP_TASKMENU_ITEM_RETURN;
}

// pokebw2's Geonet_CreateButton
void func_ov219_02184c2c(GeonetWork *work, int button) {
    if (work->buttons[button] == NULL) {
        work->buttons[button] =
            func_02027fec(work->buttonRes, &work->buttonItems[button], data_ov219_02185e40[button].x,
                                  data_ov219_02185e40[button].y, data_ov219_02185e40[button].width, work->heapId);
    }
}

// pokebw2's Geonet_FreeButton
void func_ov219_02184c8c(GeonetWork *work, int button) {
    if (work->buttons[button] != NULL) {
        func_02028148(work->buttons[button]);
        GFL_BGSysQueueScrLoad(2);
        work->buttons[button] = NULL;
    }
}

// pokebw2's Geonet_FreeBG
void func_ov219_02184cb0(GeonetWork *work) {
    func_ov219_02184c8c(work, BUTTON_VIEW);
    func_ov219_02184c8c(work, BUTTON_QUIT);
    func_02027fd8(work->buttonRes);
    GFL_StrBufFree(work->buttonText[BUTTON_VIEW]);
    GFL_StrBufFree(work->buttonText[BUTTON_QUIT]);
    work->buttons[BUTTON_VIEW] = NULL;
    work->buttons[BUTTON_QUIT] = NULL;
    BmpWin_Free(work->windows[WINDOW_INFO].window);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(7);
    GFL_FontFree(work->font);
    GFL_TextRndUpdateColorIndexLUT(work->textColors[0], work->textColors[1], work->textColors[2]);
}

// pokebw2's Geonet_Print
void func_ov219_02184d30(GeonetWork *work, PrintWindow *print, BmpWin *window, StrBuf *strbuf, u8 x, u8 y) {
    PrintWindow_Init(print, window);
    PrintWindow_PrintNoColor(print, work->queue, x, y, strbuf, work->font);
}

// Prints a message a character at a time. Returns whether it is done, which is once A is pressed unless `close`
// pokebw2's Geonet_PrintMessage
BOOL func_ov219_02184d70(GeonetWork *work, u32 message, BOOL close) {
    BOOL done = FALSE;

    switch (work->messageState) {
    case 0:
        GFL_BitmapFill(BmpWin_GetBitmap(work->windows[WINDOW_INFO].window), 15);
        work->message = GFL_StrBufCreate(0x400, work->heapId);
        GFL_MsgDataLoadStrbuf(work->msgData, message, work->message);
        work->stream = func_0201cce4(work->windows[WINDOW_INFO].window, 0, 0, work->message, work->font,
                                     func_02012ffc(), work->tcbManager, 0, work->heapId, 15);
        work->messageState = 1;
        break;
    case 1: {
        u32 streamState = func_0201ce30(work->stream);

        func_020286c4(work->keyCursor, work->stream, work->windows[WINDOW_INFO].window);
        switch (streamState) {
        case PRINT_STREAM_RUNNING:
            if (GCTX_HIDGetHeldKeys() & PAD_BUTTON_A) {
                func_0201ce5c(work->stream, 0);
            }
            break;
        case PRINT_STREAM_PAUSED:
            if ((work->keysPressed & PAD_BUTTON_A) || (work->keysPressed & PAD_BUTTON_B)) {
                func_0201ce38(work->stream);
                GFL_SndSEPlay(SEQ_SE_MESSAGE);
            }
            break;
        case PRINT_STREAM_DONE:
            work->messageState = 2;
            break;
        }
        break;
    }
    case 2:
        if (close || (work->keysPressed & PAD_BUTTON_A)) {
            GFL_StrBufFree(work->message);
            func_0201ce48(work->stream);
            work->messageState = 0;
            done = TRUE;
        }
        break;
    }
    return done;
}

// pokebw2's Geonet_ListCursorCallback
void func_ov219_02184eac(BmpMenuList *list, intptr_t value, u8 init) {
    if (!init) {
        GFL_SndSEPlay(SEQ_SE_MESSAGE);
    }
}

void func_ov219_02184ec0(GeonetWork *work, const GeonetWindowSetup *setup, const BmpMenuListHeader *header,
                                const GeonetMenuItem *items) {
    BmpMenuListHeader listHeader;
    int i;

    work->listWindow = func_ov219_02184968(setup, TRUE);
    work->options = ListMenuCore_CreateOptionList(header->count, work->heapId);
    for (i = 0; i < header->count; i++) {
        ListMenuCore_AppendMsgOption(work->options, work->msgData, items[i].message, items[i].value, work->heapId);
    }
    PrintWindow_Init(&work->listPrint, work->listWindow);
    listHeader = *header;
    listHeader.options = work->options;
    listHeader.window = work->listWindow;
    listHeader.cursorCallback = func_ov219_02184eac;
    listHeader.fontSizeX = 12;
    listHeader.fontSizeY = GFL_FontGetCharHeight(work->font);
    listHeader.msgData = work->msgData;
    listHeader.printWindow = &work->listPrint;
    listHeader.queue = work->queue;
    listHeader.font = work->font;
    work->list = BmpMenuList_Create(&listHeader, 0, 0, work->heapId);
    func_02020cf8(work->list, work->heapId);
    BmpWin_DrawFrame(work->listWindow, 0, FRAME_MENU_CHAR, FRAME_MENU_PALETTE);
    BmpWin_FlushChar(work->listWindow);
}


static void func_ov219_02184fc8(GeonetWork *work, const BmpMenuListHeader *header, u32 count) {
    BmpMenuListHeader listHeader;

    PrintWindow_Init(&work->listPrint, work->listWindow);
    listHeader = *header;
    listHeader.options = work->options;
    listHeader.count = count;
    listHeader.window = work->listWindow;
    listHeader.cursorCallback = func_ov219_02184eac;
    listHeader.fontSizeX = 12;
    listHeader.fontSizeY = GFL_FontGetCharHeight(work->font);
    listHeader.printWindow = &work->listPrint;
    listHeader.queue = work->queue;
    listHeader.font = work->font;
    work->list = BmpMenuList_Create(&listHeader, 0, 0, work->heapId);
    func_02020cf8(work->list, work->heapId);
    BmpWin_DrawFrame(work->listWindow, 0, FRAME_MENU_CHAR, FRAME_MENU_PALETTE);
    BmpWin_FlushChar(work->listWindow);
}


static const u32 data_ov219_02185f7c[130] = {
#include "asset/geonet_data_ov219_02185f7c.inc"
};

static inline void GeonetAddCountryOption(GeonetWork *work, MsgData *msgData, u32 msg) {
    ListMenuCore_AppendMsgOption(work->options, msgData, msg, msg, work->heapId);
}
void func_ov219_02185074(GeonetWork *work, const GeonetWindowSetup *setup, const BmpMenuListHeader *header, u32 textBank) {
    MsgData *msgData;
    int i;

    work->listWindow = func_ov219_02184968(setup, TRUE);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, textBank, work->heapId);
    work->options = ListMenuCore_CreateOptionList(0x82, work->heapId);
    // The condition's ternary keeps MWCC from inverting the loop (it enters at the bottom test); the index cast of
    // the value keeps the two table reads apart until the backend merges them (the original's mov r3, r2)
    for (i = 0; i < 0x82 ? TRUE : FALSE; i++) {
        ListMenuCore_AppendMsgOption(work->options, msgData, data_ov219_02185f7c[i], data_ov219_02185f7c[(u32)i], work->heapId);
    }
    GFL_MsgDataFree(msgData);
    func_ov219_02184fc8(work, header, 0x82);
}

void func_ov219_021850ec(GeonetWork *work, const GeonetWindowSetup *setup, const BmpMenuListHeader *header,
                                u32 textBank, const u8 *order, u32 count) {
    MsgData *msgData;
    u32 i;

    work->listWindow = func_ov219_02184968(setup, TRUE);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, textBank, work->heapId);
    work->options = ListMenuCore_CreateOptionList(0x82, work->heapId);
    for (i = 0; i < count; i++) {
        ListMenuCore_AppendMsgOption(work->options, msgData, order[i], order[i], work->heapId);
    }
    GFL_MsgDataFree(msgData);
    func_ov219_02184fc8(work, header, count);
}



// pokebw2's Geonet_FreeList
void func_ov219_0218516c(GeonetWork *work) {
    func_0201f7a8(work->listWindow, 0);
    BmpWin_Free(work->listWindow);
    BmpMenuList_Free(work->list, NULL, NULL);
    ListMenuCore_FreeOptionList(work->options);
}

// pokebw2's Geonet_ShowRegisteredPlace
void func_ov219_0218519c(GeonetWork *work) {
    StrBuf *strbuf = GFL_StrBufCreate(0x400, work->heapId);
    StrBuf *format = GFL_StrBufCreate(0x400, work->heapId);

    work->windows[WINDOW_PLACE].window = func_ov219_02184968(&data_ov219_02185dcb, TRUE);
    loadCountryToStrbuf(work->wordSet, 0, work->myCountry);
    loadCountryAreaToStrbuf(work->wordSet, 1, work->myCountry, work->myRegion);
    GFL_MsgDataLoadStrbuf(work->msgData, EventMapchange2_Text_014, format);
    GFL_WordSetFormatStrbuf(work->wordSet, strbuf, format);
    func_ov219_02184d30(work, &work->windows[WINDOW_PLACE].print, work->windows[WINDOW_PLACE].window, strbuf, 0, 0);
    GFL_StrBufFree(format);
    GFL_StrBufFree(strbuf);
    BmpWin_DrawFrame(work->windows[WINDOW_PLACE].window, 0, FRAME_MENU_CHAR, FRAME_MENU_PALETTE);
    BmpWin_FlushChar(work->windows[WINDOW_PLACE].window);
}

// pokebw2's Geonet_ShowSelectedPlace
void func_ov219_02185254(GeonetWork *work, u32 country, u32 region) {
    StrBuf *countryName = GFL_StrBufCreate(64, work->heapId);
    StrBuf *regionName = GFL_StrBufCreate(64, work->heapId);

    work->windows[WINDOW_PLACE].window = func_ov219_02184968(&data_ov219_02185dcb, TRUE);
    func_ov219_02185c84(country, region, countryName, regionName, work->heapId);
    if (region != 0) {
        func_ov219_02184d30(work, &work->windows[WINDOW_PLACE].print, work->windows[WINDOW_PLACE].window, regionName, 0,
                     16);
    }
    func_ov219_02184d30(work, &work->windows[WINDOW_PLACE].print, work->windows[WINDOW_PLACE].window, countryName, 0, 0);
    GFL_StrBufFree(regionName);
    GFL_StrBufFree(countryName);
    BmpWin_DrawFrame(work->windows[WINDOW_PLACE].window, 0, FRAME_MENU_CHAR, FRAME_MENU_PALETTE);
    BmpWin_Transfer(work->windows[WINDOW_PLACE].window);
}

// pokebw2's Geonet_ClosePlaceWindow
void func_ov219_02185310(GeonetWork *work) {
    func_0201f7a8(work->windows[WINDOW_PLACE].window, 0);
    BmpWin_Free(work->windows[WINDOW_PLACE].window);
}

// Finds the marked place nearest the middle of the screen, among those within 0x80 of it. Returns whether there is
// one
// pokebw2's Geonet_FindNearestPlace
BOOL func_ov219_0218532c(GeonetWork *work, u32 *index) {
    BOOL found = FALSE;
    s16 minX = work->rotation.x - 0x80;
    s16 maxX = work->rotation.x + 0x80;
    int minY = (s16)(work->rotation.y - 0x80);
    int maxY = (s16)(work->rotation.y + 0x80);
    int wrapMinY;
    int wrapMaxY;
    u32 nearest = work->placeCount;
    u32 nearestDistance = 0x100;
    GeonetAngles center;
    GeonetAngles angles;
    u32 i;

    center.x = work->rotation.x;
    center.y = work->rotation.y;
    func_ov219_02185d08(&center);

    // The range crosses the 180th meridian
    if (MATH_ABS(maxY - minY) > 0x100) {
        wrapMaxY = wrapMinY = (s16)work->rotation.y;
        if (wrapMinY > 0) {
            maxY = minY = wrapMinY - 0x10000;
        } else {
            maxY = minY = wrapMinY + 0x10000;
        }
        wrapMinY -= 0x80;
        wrapMaxY += 0x80;
        minY -= 0x80;
        maxY += 0x80;
    } else {
        wrapMaxY = maxY;
        wrapMinY = minY;
    }

    for (i = 0; i < work->placeCount; i++) {
        if (work->places[i].x > minX && work->places[i].x < maxX
            && ((work->places[i].y > minY && work->places[i].y < maxY)
                || (work->places[i].y > wrapMinY && work->places[i].y < wrapMaxY))
            && work->places[i].state != UNITY_TOWER_REGION_NONE) {
            u32 distance;

            angles.x = work->places[i].x;
            angles.y = work->places[i].y;
            func_ov219_02185d08(&angles);
            distance = func_ov219_02185d64(&center, &angles);
            if (distance < nearestDistance) {
                nearestDistance = distance;
                nearest = i;
            }
        }
    }

    *index = nearest;
    if (nearest != work->placeCount) {
        found = TRUE;
    }
    return found;
}

// Shows how to use the globe, or the name of the place at the middle of the screen, which the globe turns to
// pokebw2's Geonet_UpdateInfo
void func_ov219_0218544c(GeonetWork *work) {
    u32 index;

    if (work->infoMode == FALSE) {
        StrBuf *strbuf = GFL_StrBufCreate(0x400, work->heapId);

        GFL_BitmapFill(BmpWin_GetBitmap(work->windows[WINDOW_INFO].window), 15);
        GFL_MsgDataLoadStrbuf(work->msgData, EventMapchange2_Text_015, strbuf);
        func_ov219_02184d30(work, &work->windows[WINDOW_INFO].print, work->windows[WINDOW_INFO].window, strbuf, 0, 0);
        GFL_StrBufFree(strbuf);
    } else if (func_ov219_0218532c(work, &index)) {
        StrBuf *countryName = GFL_StrBufCreate(64, work->heapId);
        StrBuf *regionName = GFL_StrBufCreate(64, work->heapId);

        GFL_BitmapFill(BmpWin_GetBitmap(work->windows[WINDOW_INFO].window), 15);
        func_ov219_02185c84(work->places[index].country, work->places[index].region, countryName, regionName,
                              work->heapId);
        if (work->places[index].region != 0) {
            func_ov219_02184d30(work, &work->windows[WINDOW_INFO].print, work->windows[WINDOW_INFO].window, regionName, 0,
                         16);
        }
        func_ov219_02184d30(work, &work->windows[WINDOW_INFO].print, work->windows[WINDOW_INFO].window, countryName, 0, 0);
        GFL_StrBufFree(regionName);
        GFL_StrBufFree(countryName);
        work->rotation.x = work->places[index].x;
        work->rotation.y = work->places[index].y;
    } else {
        work->infoMode = FALSE;
    }
    BmpWin_FlushChar(work->windows[WINDOW_INFO].window);
}

// pokebw2's Geonet_Load3D
void func_ov219_02185580(GeonetWork *work, ArcTool *arc) {
    u32 i;

    work->earthRes = GFL_G3DSysReadArcSysResource(ARCID_GEONET, NARC_GEONET_EARTH_NSBMD);
    GFL_G3DResUploadTexData(work->earthRes);
    work->earthModel = GFL_G3DMdlCreate(work->earthRes, 0, work->earthRes);
    work->earthActor = GFL_G3DActorCreate(work->earthModel, NULL, 0);
    for (i = 0; i < NELEMS(data_ov219_02185e18); i++) {
        u16 mark = data_ov219_02185e18[i].mark;

        work->markRes[mark] = GFL_G3DSysReadArcSysResource(ARCID_GEONET, data_ov219_02185e18[i].file);
        GFL_G3DResUploadTexData(work->markRes[mark]);
        work->markModels[mark] = GFL_G3DMdlCreate(work->markRes[mark], 0, work->markRes[mark]);
        work->markActors[mark] = GFL_G3DActorCreate(work->markModels[mark], NULL, 0);
    }
    func_ov219_02185728(work);
    func_ov219_02185824(work);
}

// pokebw2's Geonet_Free3D
void func_ov219_0218562c(GeonetWork *work) {
    u32 i;

    GFL_G3DCameraFree(work->camera);
    GFL_G3DLightFree(work->light);
    GFL_G3DActorFree(work->earthActor);
    GFL_G3DMdlFree(work->earthModel);
    GFL_G3DResFreeTexData(work->earthRes);
    GFL_G3DResFree(work->earthRes);
    for (i = 0; i < NELEMS(data_ov219_02185e18); i++) {
        u16 mark = data_ov219_02185e18[i].mark;

        GFL_G3DActorFree(work->markActors[mark]);
        GFL_G3DMdlFree(work->markModels[mark]);
        GFL_G3DResFreeTexData(work->markRes[mark]);
        GFL_G3DResFree(work->markRes[mark]);
    }
    work->earthActor = NULL;
}

// pokebw2's Geonet_InitEarth
void func_ov219_021856cc(GeonetWork *work) {
    work->translation.x = 0;
    work->translation.y = 0;
    work->translation.z = 0;
    work->scale.x = FX32_ONE;
    work->scale.y = FX32_ONE;
    work->scale.z = FX32_ONE;
    work->rotation.x = 0x1a40;
    work->rotation.y = 0x7c00;
    work->rotation.z = 0;
    work->markScale.x = FX32_ONE;
    work->markScale.y = FX32_ONE;
    work->markScale.z = FX32_ONE;
}

// pokebw2's Geonet_CreateCamera
void func_ov219_02185728(GeonetWork *work) {
    HeapID heapId = work->heapId;
    VecFx32 up = {0, FX32_ONE, 0};

    work->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)),
                                       FX_CosIdx(DEG_TO_IDX(20)), FX32_ONE * 4 / 3, 0, FX32_ONE, FX32_ONE * 1024, 0,
                                       &data_ov219_02185e0c, &up, &data_ov219_02185df4, heapId);
    GFL_G3DCameraFlush(work->camera);
}

// Puts the camera where it starts: far from the globe if the player has no place or lives in the United States, and
// near it otherwise
// pokebw2's Geonet_ResetCamera
void func_ov219_02185794(GeonetWork *work) {
    fx32 near = 0;
    fx32 far = FX32_ONE * 100;

    GFL_G3DCameraSetLookatPos(work->camera, &data_ov219_02185e0c);
    GFL_G3DCameraSetLookatTarget(work->camera, &data_ov219_02185df4);
    GFL_G3DCameraPerspectiveSetFOVSin(work->camera, FX_SinIdx(0x5c1));
    GFL_G3DCameraPerspectiveSetFOVCos(work->camera, FX_CosIdx(0x5c1));
    GFL_G3DCameraSetProjectionZNear(work->camera, &near);
    GFL_G3DCameraSetProjectionZFar(work->camera, &far);

    if (work->worldMode == FALSE) {
        work->zoom = TRUE;
    } else if (work->myCountry == COUNTRY_NONE) {
        work->zoom = FALSE;
    } else if (work->myCountry == COUNTRY_USA) {
        work->zoom = FALSE;
    } else {
        work->zoom = TRUE;
    }
    while (func_ov219_02185ac8(work) != TRUE) {
    }
}

// pokebw2's Geonet_CreateLight
void func_ov219_02185824(GeonetWork *work) {
    work->light = GFL_G3DLightCreate(&data_ov219_02185dd4, work->heapId);
    GFL_G3DLightFlush(work->light);
}

// pokebw2's Geonet_GetEarthRotation
static void func_ov219_02185848(MtxFx33 *mtx, const VecFx32 *rotation) {
    MtxFx33 tmp;

    MTX_RotY33_(mtx, FX_SinIdx((u16)rotation->y), FX_CosIdx((u16)rotation->y));
    MTX_RotX33_(&tmp, FX_SinIdx((u16)rotation->x), FX_CosIdx((u16)rotation->x));
    MTX_Concat33(mtx, &tmp, mtx);
    MTX_RotZ33_(&tmp, FX_SinIdx((u16)rotation->z), FX_CosIdx((u16)rotation->z));
    MTX_Concat33(mtx, &tmp, mtx);
}

// The rotation that puts a mark at a place. The sine and cosine of the last rotation are in each other's place,
// which turns the mark a quarter of the way around
// pokebw2's Geonet_GetPlaceRotation
static void func_ov219_021858bc(MtxFx33 *mtx, const VecFx32 *rotation) {
    MtxFx33 tmp;

    MTX_RotY33_(mtx, FX_SinIdx((u16)rotation->x), FX_CosIdx((u16)rotation->x));
    MTX_RotX33_(&tmp, FX_SinIdx((u16)-rotation->y), FX_CosIdx((u16)-rotation->y));
    MTX_Concat33(mtx, &tmp, mtx);
    MTX_RotZ33_(&tmp, FX_CosIdx((u16)rotation->z), FX_SinIdx((u16)rotation->z));
    MTX_Concat33(mtx, &tmp, mtx);
}

// Turns the globe with the keys or the touch screen, and A moves the camera. Returns whether a key did anything
// pokebw2's Geonet_Rotate
BOOL func_ov219_02185934(GeonetWork *work, u32 pressed, u32 held) {
    u16 speedX;
    u16 speedY;
    BOOL moved = FALSE;
    s16 rotationX = work->rotation.x;
    s16 rotationY = work->rotation.y;

    if ((pressed & PAD_BUTTON_A) || (work->touchKeys & PAD_BUTTON_A)) {
        if (work->worldMode == TRUE) {
            if (work->zoom == FALSE) {
                work->zoom = TRUE;
            } else {
                work->zoom = FALSE;
            }
        }
        return TRUE;
    }

    if (work->zoom == FALSE) {
        if (work->touchMoveX != 0 || work->touchMoveY != 0) {
            speedX = work->touchMoveX * 0x55;
            speedY = work->touchMoveY * 0x55;
        } else {
            speedX = 0x200;
            speedY = 0x200;
        }
    } else {
        if (work->touchMoveX != 0 || work->touchMoveY != 0) {
            speedX = work->touchMoveX * 10;
            speedY = work->touchMoveY * 10;
        } else {
            speedX = 0x20;
            speedY = 0x20;
        }
    }

    if ((held & PAD_KEY_LEFT) || (work->touchKeys & PAD_KEY_LEFT)) {
        if (work->worldMode == TRUE) {
            work->rotation.y += speedX;
        } else if (rotationY < JAPAN_ROTATION_Y_MAX) {
            work->rotation.y += speedX;
        }
        moved = TRUE;
    }
    if ((held & PAD_KEY_RIGHT) || (work->touchKeys & PAD_KEY_RIGHT)) {
        if (work->worldMode == TRUE) {
            work->rotation.y -= speedX;
        } else if (rotationY > JAPAN_ROTATION_Y_MIN) {
            work->rotation.y -= speedX;
        }
        moved = TRUE;
    }
    if ((held & PAD_KEY_UP) || (work->touchKeys & PAD_KEY_UP)) {
        if (work->worldMode == TRUE) {
            if (rotationX + speedY < WORLD_ROTATION_X_MAX) {
                work->rotation.x += speedY;
            } else {
                work->rotation.x = WORLD_ROTATION_X_MAX;
            }
        } else if (rotationX < JAPAN_ROTATION_X_MAX) {
            work->rotation.x += speedY;
        }
        moved = TRUE;
    }
    if ((held & PAD_KEY_DOWN) || (work->touchKeys & PAD_KEY_DOWN)) {
        if (work->worldMode == TRUE) {
            if (rotationX - speedY > -WORLD_ROTATION_X_MAX) {
                work->rotation.x -= speedY;
            } else {
                work->rotation.x = -WORLD_ROTATION_X_MAX;
            }
        } else if (rotationX > JAPAN_ROTATION_X_MIN) {
            work->rotation.x -= speedY;
        }
        moved = TRUE;
    }
    return moved;
}

// Moves the camera a step toward where the zoom puts it, the marks growing as it moves away. Returns whether it is
// there
// pokebw2's Geonet_MoveCamera
BOOL func_ov219_02185ac8(GeonetWork *work) {
    VecFx32 pos;
    VecFx32 target;
    VecFx32 direction;
    fx32 distance;
    BOOL done = FALSE;

    GFL_G3DCameraGetLookatPos(work->camera, &pos);
    GFL_G3DCameraGetLookatTarget(work->camera, &target);
    VEC_Subtract(&pos, &target, &direction);
    distance = VEC_Mag(&direction);
    VEC_Normalize(&direction, &direction);

    switch (work->zoom) {
    case TRUE:
        if (distance > CAMERA_DISTANCE_NEAR + CAMERA_SPEED) {
            distance -= CAMERA_SPEED;
            work->markScale.x -= 0x80;
            work->markScale.y = work->markScale.x;
        } else {
            distance = CAMERA_DISTANCE_NEAR;
            done = TRUE;
        }
        break;
    case FALSE:
        if (distance < CAMERA_DISTANCE_FAR - CAMERA_SPEED) {
            distance += CAMERA_SPEED;
            work->markScale.x += 0x80;
            work->markScale.y = work->markScale.x;
        } else {
            distance = CAMERA_DISTANCE_FAR;
            done = TRUE;
        }
        break;
    }

    VEC_MultAdd(distance, &direction, &target, &pos);
    GFL_G3DCameraSetLookatPos(work->camera, &pos);
    return done;
}

// pokebw2's Geonet_Draw
static void func_ov219_02185b74(GeonetWork *work) {
    SRTMatrix status;
    SRTMatrix markStatus;
    u32 i;

    switch (work->drawState) {
    case DRAW_NONE:
        break;
    case DRAW_CLEAR:
        GFL_G3DSysReset();
        GFL_G3DSysReqSwapBuffers();
        work->drawState = DRAW_NONE;
        break;
    case DRAW_EARTH:
        GFL_G3DLightFlush(work->light);
        GFL_G3DCameraFlush(work->camera);
        GFL_G3DSysReset();
        GFL_G3DSysMtxViewFlush();

        status.translation = work->translation;
        MTX_Identity33_(&status.rotation);
        status.scale = work->markScale;
        GFL_G3DSysDrawObj(work->markActors[MARK_CURSOR], &status);

        status.scale = work->scale;
        func_ov219_02185848(&status.rotation, &work->rotation);
        GFL_G3DSysDrawObj(work->earthActor, &status);

        markStatus.translation = work->translation;
        markStatus.scale = work->markScale;
        for (i = 0; i < work->placeCount; i++) {
            MTX_Concat33(&work->places[i].rotation, &status.rotation, &markStatus.rotation);
            if (work->places[i].state != UNITY_TOWER_REGION_NONE) {
                GFL_G3DSysDrawObj(work->markActors[work->places[i].state], &markStatus);
            }
        }
        GFL_G3DSysReqSwapBuffers();
        break;
    }
}

// Loads the names of a country and of its region. Returns whether the country has regions
// pokebw2's Geonet_LoadPlaceNames
BOOL func_ov219_02185c84(u32 country, u32 region, StrBuf *countryName, StrBuf *regionName, int heapId) {
    u32 subdivided = func_ov219_02184790(country);
    BOOL hasRegions = FALSE;
    MsgData *msgData;

    msgData =
        GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_EVENT_MAPCHANGE_LOAD_COUNTRY_TO_STRBUF, heapId);
    GFL_MsgDataLoadStrbuf(msgData, country, countryName);
    GFL_MsgDataFree(msgData);
    if (subdivided == 0) {
        subdivided = 1;
        region = 0;
    } else {
        hasRegions = TRUE;
    }
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, getFileNumForSubdividedRegion(subdivided), heapId);
    GFL_MsgDataLoadStrbuf(msgData, region, regionName);
    GFL_MsgDataFree(msgData);
    return hasRegions;
}

// pokebw2's Geonet_IsSubdivided
BOOL func_ov219_02185cf4(u32 country) {
    if (func_ov219_02184790(country) != 0) {
        return TRUE;
    }
    return FALSE;
}

// Brings angles into 0 to 0xfffe
// pokebw2's Geonet_WrapAngles
void func_ov219_02185d08(GeonetAngles *angles) {
    if (angles->x >= 0) {
        angles->x %= 0xffff;
    } else {
        angles->x += (MATH_ABS(angles->x) / 0xffff + 1) * 0xffff;
    }
    if (angles->y >= 0) {
        angles->y %= 0xffff;
    } else {
        angles->y += (MATH_ABS(angles->y) / 0xffff + 1) * 0xffff;
    }
}

// pokebw2's Geonet_GetDistance
u32 func_ov219_02185d64(const GeonetAngles *a, const GeonetAngles *b) {
    int x = MATH_ABS(a->x - b->x);
    int y = MATH_ABS(a->y - b->y);

    if (x > func_0203f734(180)) {
        x = 0xffff - x;
    }
    if (y > func_0203f734(180)) {
        y = 0xffff - y;
    }
    return FX_Sqrt((x * x + y * y) << FX32_SHIFT) >> FX32_SHIFT;
}
