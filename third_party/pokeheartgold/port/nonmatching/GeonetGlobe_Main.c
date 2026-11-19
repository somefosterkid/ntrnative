#include "global.h"
typedef struct UnkStruct_ov69_part02 UnkStruct_ov69_part02;
typedef struct WiFiEarthData WiFiEarthData;
typedef struct WiFiEarthMenuItem WiFiEarthMenuItem;
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "pm_string.h"
#include "sys_task.h"
#include "bg_window.h"
#include "camera.h"
#include "constants/sndseq.h"
#include "filesystem.h"
#include "font.h"
#include "geonet_locations.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu.h"
#include "list_menu_2d.h"
#include "list_menu_items.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "render_text.h"
#include "render_window.h"
#include "save_wifi_history.h"
#include "screen_fade.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
struct Background;
struct BgConfig;
struct Camera;
struct CameraAngle;
struct CameraHistory;
struct CameraLookAt;
struct CameraPerspective;
struct GeonetGlobe;
struct GeonetGlobeAngle;
struct GeonetGlobeMenuEntry;
struct GeonetGlobeTouch;
struct GeonetLocation;
struct ListMenu;
struct ListMenu2D;
struct ListMenu2DTemplate;
struct ListMenuCursor;
struct ListMenuItem;
struct ListMenuTemplate;
struct MessageFormat;
struct MessageFormatAttrs;
struct MessageFormatFields;
struct MsgData;
struct NARC;
struct Options;
struct OverlayManager;
struct OverlayManagerTemplate;
struct SaveWiFiHistory;
struct String;
struct SysTask;
struct SysTaskQueue;
struct System;
struct Window;
struct WindowTemplate;
struct _MAT;
struct _MAT_ENTRY;




void ClearWindowPixelBuffer(Window *window, BOOL dont_copy_to_vram);
enum GeonetGlobeState {
    GEONET_GLOBE_STATE_LOAD,
    GEONET_GLOBE_STATE_WAIT_FADE_IN,
    GEONET_GLOBE_STATE_PRINT_WELCOME,
    GEONET_GLOBE_STATE_OPEN_MENU,
    GEONET_GLOBE_STATE_HANDLE_MENU,
    GEONET_GLOBE_STATE_ASK_REGISTER,
    GEONET_GLOBE_STATE_HANDLE_REGISTER_YES_NO,
    GEONET_GLOBE_STATE_SELECT_COUNTRY,
    GEONET_GLOBE_STATE_HANDLE_COUNTRY_LIST,
    GEONET_GLOBE_STATE_SELECT_REGION,
    GEONET_GLOBE_STATE_HANDLE_REGION_LIST,
    GEONET_GLOBE_STATE_SHOW_LOCATION,
    GEONET_GLOBE_STATE_ASK_CONFIRM,
    GEONET_GLOBE_STATE_HANDLE_CONFIRM_YES_NO,
    GEONET_GLOBE_STATE_START_GLOBE,
    GEONET_GLOBE_STATE_GLOBE,
    GEONET_GLOBE_STATE_ZOOM,
    GEONET_GLOBE_STATE_FADE_OUT,
    GEONET_GLOBE_STATE_WAIT_FADE_OUT,
};
enum GeonetGlobeWindow {
    GEONET_GLOBE_WINDOW_MESSAGE,
    GEONET_GLOBE_WINDOW_LIST,
    GEONET_GLOBE_WINDOW_EXIT_BUTTON,
    GEONET_GLOBE_WINDOW_INFO_BUTTON,
    GEONET_GLOBE_WINDOW_LOCATION,
    GEONET_GLOBE_WINDOW_COUNT,
};
typedef struct GeonetLocation {
    s16 x;
    s16 y;
    MtxFx33 rotation;
    u16 seenState;
    u16 country;
    u16 region;
} GeonetLocation;
typedef struct GeonetGlobeAngle {
    s32 x;
    s32 y;
    s32 z;
} GeonetGlobeAngle;
typedef struct GeonetGlobeTouch {
    u32 input;
    u32 state;
    u32 x;
    u32 y;
    s32 dx;
    s32 dy;
    u32 timer;
} GeonetGlobeTouch;
typedef struct GeonetGlobe {
    enum HeapID heapID;
    SaveWiFiHistory *wifiHistory;
    Options *options;
    u32 numLocations;
    GeonetLocation locations[1024 ];
    BgConfig *bgConfig;
    Window windows[GEONET_GLOBE_WINDOW_COUNT];
    struct ListMenu *listMenu;
    ListMenuItem *listMenuItems;
    struct ListMenu2D *yesNoMenu;
    MsgData *msgData;
    int printState;
    int printerId;
    String *printString;
    MessageFormat *msgFormat;
    NNSG3dRenderObj globeObj;
    NNSG3dResMdl *globeMdl;
    void *globeFile;
    NNSG3dRenderObj markerObjs[4];
    NNSG3dRenderObj cursorObj;
    NNSG3dResMdl *markerMdls[4];
    NNSG3dResMdl *cursorMdl;
    void *markerFiles[4];
    void *cursorFile;
    VecFx32 translation;
    VecFx32 globeScale;
    GeonetGlobeAngle rotation;
    VecFx32 markerScale;
    Camera *camera;
    u8 padding_C2E0[8];
    u16 zoomedIn;
    u8 padding_C2EA[2];
    VecFx32 lightVector;
    u32 drawState;
    u32 unk_C2FC;
    u32 isJapanese;
    u16 freeRotation;
    u8 padding_C306[2];
    GeonetGlobeTouch touch;
    u32 playerCountry;
    u32 playerRegion;
    u32 selectedCountry;
    u32 selectedRegion;
    u32 nonJapanese;
    u32 showLocationName;
} GeonetGlobe;
typedef struct GeonetGlobeMenuEntry {
    int msgId;
    int value;
} GeonetGlobeMenuEntry;
void ov69_021E6308(GeonetGlobe *globe);
void ov69_021E64CC(GeonetGlobe *globe, NARC *narc);
BOOL ov69_021E6810(GeonetGlobe *globe, int msgId, BOOL noWait);
void ov69_021E6994(GeonetGlobe *globe, Window *window, const WindowTemplate *windowTemplate, const ListMenuTemplate *menuTemplate, int msgBank, const u8 *msgIds, u32 count);
void ov69_021E6B5C(GeonetGlobe *globe, int country, int region);
void ov69_021E6C14(GeonetGlobe *globe);
BOOL ov69_021E6C30(GeonetGlobe *globe, int *index);
void ov69_021E6D5C(GeonetGlobe *globe);
BOOL ov69_021E7198(GeonetGlobe *globe, u32 newKeys, u32 heldKeys);
BOOL ov69_021E758C(int country);
#ifdef __MWERKS__
extern const WindowTemplate sGeonetPlaceListWindowTemplate;
extern const WindowTemplate sGeonetGlobeListWindowTemplate;
extern const WindowTemplate sGeonetYesNoWindowTemplate;
extern const GeonetGlobeMenuEntry sGeonetMenuEntries[3];
extern const ListMenuTemplate sGeonetMenuTemplate;
extern const ListMenuTemplate sGeonetPlaceListTemplate;
#else
/* by value from the asm bytes: ov69_021E7674 / _021E7644 / ov69_021E7664 / ov69_021E7698 / ov69_021E76E8 / ov69_021E7708 */
static const WindowTemplate sGeonetPlaceListWindowTemplate = { 6, 3, 2, 26, 14, 4, 1 };
static const WindowTemplate sGeonetGlobeListWindowTemplate = { 6, 19, 11, 12, 6, 4, 293 };
static const WindowTemplate sGeonetYesNoWindowTemplate = { 6, 25, 13, 6, 4, 4, 341 };
static const GeonetGlobeMenuEntry sGeonetMenuEntries[3] = { { 10, 0 }, { 11, 1 }, { 12, 2 } };
static const ListMenuTemplate sGeonetMenuTemplate = { .items = NULL, .moveCursorFunc = NULL, .itemPrintFunc = NULL, .window = NULL, .totalItems = 3, .maxShowed = 3, .item_X = 12, .cursorPal = 1, .fillValue = 15, .cursorShadowPal = 2, .data = NULL };
static const ListMenuTemplate sGeonetPlaceListTemplate = { .items = NULL, .moveCursorFunc = NULL, .itemPrintFunc = NULL, .window = NULL, .totalItems = 0, .maxShowed = 7, .item_X = 12, .cursorPal = 1, .fillValue = 15, .cursorShadowPal = 2, .scrollMultiple = 1, .data = NULL };
#endif
BOOL GeonetGlobe_GetLocationNames(int country, int region, String *countryName, String *regionName, enum HeapID heapID);
void ov69_021E6E88(void *arg0, NARC *narc);
void ov69_021E68EC(WiFiEarthData *wifiEarth, Window *window, const WindowTemplate *windowTemplate, const ListMenuTemplate *listMenuTemplate, const WiFiEarthMenuItem *items);
void ov69_021E6A54(void *arg0);




void ov69_021E6F8C(void *arg0);
void ov69_021E62B0(WiFiEarthData *wifiEarth);
void ov69_021E6FE8(WiFiEarthData *wifiEarth);
void ov69_021E706C(WiFiEarthData *wifiEarth);
void ov69_021E6A8C(void *arg0);

s32 ov69_021E737C(WiFiEarthData *wifiEarth);
void ov69_021E67B8(void *arg0);
void ov69_021E6F48(void *arg0);
void ov69_021E7408(UnkStruct_ov69_part02 *param0);

BOOL GeonetGlobe_Main(OverlayManager *man, int *state);
BOOL GeonetGlobe_Main(OverlayManager *man, int *state) {
    GeonetGlobe *globe = OverlayManager_GetData(man);
    BOOL result = 0 ;
    int choice;
    int placeIndex;
    int count;
    int msgBank;
    const u8 *msgIds;
    u32 keys;
    u32 touchInput;
    int nearestLocation;
    NARC *narc;

    switch (*state) {
    case GEONET_GLOBE_STATE_START_GLOBE:
    case GEONET_GLOBE_STATE_GLOBE:
    case GEONET_GLOBE_STATE_ZOOM:
        break;
    default:
        if (System_GetTouchNew()) {
            gSystem.simulatedInputs = 1;
        }
        break;
    }

    switch (*state) {
    case GEONET_GLOBE_STATE_LOAD:
        globe->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 186, globe->heapID);
        narc = NARC_New(NARC_a_1_2_3, globe->heapID);
        ov69_021E6E88(globe, narc);
        ov69_021E64CC(globe, narc);
        NARC_Delete(narc);
        globe->unk_C2FC = 0;
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((GXRgb)(((0) << (0)) | (( 0) << (5)) | (( 0) << (10))))) , 6, 1, globe->heapID);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        *state = GEONET_GLOBE_STATE_WAIT_FADE_IN;
        break;
    case GEONET_GLOBE_STATE_WAIT_FADE_IN:
        if (IsPaletteFadeFinished() == 1 ) {
            *state = GEONET_GLOBE_STATE_PRINT_WELCOME;
        }
        break;
    case GEONET_GLOBE_STATE_PRINT_WELCOME:
        if (ov69_021E6810(globe, 0 , 1 ) == 1 ) {
            if (globe->playerCountry == 0) {
                *state = GEONET_GLOBE_STATE_OPEN_MENU;
            } else {
                *state = GEONET_GLOBE_STATE_START_GLOBE;
            }
        }
        break;
    case GEONET_GLOBE_STATE_OPEN_MENU:
        if (ov69_021E6810(globe, 1 , 1 ) == 1 ) {
            ov69_021E68EC((struct WiFiEarthData *)(globe), &globe->windows[GEONET_GLOBE_WINDOW_LIST], &sGeonetGlobeListWindowTemplate, &sGeonetMenuTemplate, (const struct WiFiEarthMenuItem *)(sGeonetMenuEntries));
            *state = GEONET_GLOBE_STATE_HANDLE_MENU;
        }
        break;
    case GEONET_GLOBE_STATE_HANDLE_MENU:
        choice = ListMenu_ProcessInput(globe->listMenu);
        if (choice != -1 ) {
            ov69_021E6A54(globe);
            PlaySE(1500 );
            switch (choice) {
            case 0:
            case -1 :
            default:
                *state = GEONET_GLOBE_STATE_START_GLOBE;
                break;
            case 1:
                *state = GEONET_GLOBE_STATE_ASK_REGISTER;
                break;
            case -2 :
            case 2:
                *state = GEONET_GLOBE_STATE_FADE_OUT;
                break;
            }
        }
        break;
    case GEONET_GLOBE_STATE_ASK_REGISTER:
        if (ov69_021E6810(globe, 2 , 1 ) == 1 ) {
            globe->yesNoMenu = Std_CreateYesNoMenu(globe->bgConfig, &sGeonetYesNoWindowTemplate, 473, 7, globe->heapID);
            *state = GEONET_GLOBE_STATE_HANDLE_REGISTER_YES_NO;
        }
        break;
    case GEONET_GLOBE_STATE_HANDLE_REGISTER_YES_NO:
        choice = Handle2dMenuInput_DeleteOnFinish(globe->yesNoMenu, globe->heapID);
        if (choice == 0) {
            goto case6_yes;
        }
        if (choice == -2 ) {
            goto case6_no;
        }
        break;
case6_yes:
        if (globe->isJapanese == 1 ) {
            globe->selectedCountry = 103;
            *state = GEONET_GLOBE_STATE_SELECT_REGION;
        } else {
            *state = GEONET_GLOBE_STATE_SELECT_COUNTRY;
        }
        break;
case6_no:
        *state = GEONET_GLOBE_STATE_OPEN_MENU;
        break;
    case GEONET_GLOBE_STATE_SELECT_COUNTRY:
        if (ov69_021E6810(globe, 3 , 1 ) == 1 ) {
            globe->selectedCountry = 0;
            msgIds = GeonetLocations_GetLocationList(0);
            count = GeonetLocations_GetLocationCount(0);
            ov69_021E6994(globe, &globe->windows[GEONET_GLOBE_WINDOW_LIST], &sGeonetPlaceListWindowTemplate, &sGeonetPlaceListTemplate, 798, msgIds, count);
            *state = GEONET_GLOBE_STATE_HANDLE_COUNTRY_LIST;
        }
        break;
    case GEONET_GLOBE_STATE_HANDLE_COUNTRY_LIST:
        choice = ListMenu_ProcessInput(globe->listMenu);
        if (choice != -1 ) {
            ov69_021E6A54(globe);
            PlaySE(1500 );
            if (choice != -2 ) {
                choice = GeonetLocations_GetLocationList(0)[choice];
            }
            if (choice != -2 ) {
                globe->selectedCountry = choice;
                if (ov69_021E758C(globe->selectedCountry) == 1 ) {
                    *state = GEONET_GLOBE_STATE_SELECT_REGION;
                } else {
                    globe->selectedRegion = 0;
                    *state = GEONET_GLOBE_STATE_SHOW_LOCATION;
                }
            } else {
                *state = GEONET_GLOBE_STATE_OPEN_MENU;
            }
        }
        break;
    case GEONET_GLOBE_STATE_SELECT_REGION:
        if (ov69_021E6810(globe, 4 , 1 ) == 1 ) {
            globe->selectedRegion = 0;
            placeIndex = GeonetLocations_GetIndexFromCountry(globe->selectedCountry);
            msgBank = GeonetLocations_GetBankID(placeIndex);
            msgIds = GeonetLocations_GetLocationList(placeIndex);
            count = GeonetLocations_GetLocationCount(placeIndex);
            ov69_021E6994(globe, &globe->windows[GEONET_GLOBE_WINDOW_LIST], &sGeonetPlaceListWindowTemplate, &sGeonetPlaceListTemplate, msgBank, msgIds, count);
            *state = GEONET_GLOBE_STATE_HANDLE_REGION_LIST;
        }
        break;
    case GEONET_GLOBE_STATE_HANDLE_REGION_LIST:
        choice = ListMenu_ProcessInput(globe->listMenu);
        if (choice != -1 ) {
            ov69_021E6A54(globe);
            PlaySE(1500 );
            if (choice != -2 ) {
                choice = GeonetLocations_GetLocationList(GeonetLocations_GetIndexFromCountry(globe->selectedCountry))[choice];
            }
            if (choice != -2 ) {
                globe->selectedRegion = choice;
                *state = GEONET_GLOBE_STATE_SHOW_LOCATION;
            } else if (globe->isJapanese == 1 ) {
                *state = GEONET_GLOBE_STATE_OPEN_MENU;
            } else {
                *state = GEONET_GLOBE_STATE_SELECT_COUNTRY;
            }
        }
        break;
    case GEONET_GLOBE_STATE_SHOW_LOCATION:
        ov69_021E6B5C(globe, globe->selectedCountry, globe->selectedRegion);
        *state = GEONET_GLOBE_STATE_ASK_CONFIRM;
        break;
    case GEONET_GLOBE_STATE_ASK_CONFIRM:
        if (ov69_021E6810(globe, 5 , 1 ) == 1 ) {
            globe->yesNoMenu = Std_CreateYesNoMenu(globe->bgConfig, &sGeonetYesNoWindowTemplate, 473, 7, globe->heapID);
            *state = GEONET_GLOBE_STATE_HANDLE_CONFIRM_YES_NO;
        }
        break;
    case GEONET_GLOBE_STATE_HANDLE_CONFIRM_YES_NO:
        choice = Handle2dMenuInput_DeleteOnFinish(globe->yesNoMenu, globe->heapID);
        if (choice == 0) {
            goto case13_yes;
        }
        if (choice == -2 ) {
            goto case13_no;
        }
        break;
case13_yes:
        ov69_021E6C14(globe);
        WiFiHistory_SetPlayerGlobeInfo(globe->wifiHistory, globe->selectedCountry, globe->selectedRegion);
        globe->playerCountry = globe->selectedCountry;
        globe->playerRegion = globe->selectedRegion;
        *state = GEONET_GLOBE_STATE_START_GLOBE;
        break;
case13_no:
        ov69_021E6C14(globe);
        *state = GEONET_GLOBE_STATE_OPEN_MENU;
        break;
    case GEONET_GLOBE_STATE_START_GLOBE:
        if (globe->isJapanese == 1 && !globe->nonJapanese) {
            globe->freeRotation = 0 ;
        } else {
            globe->freeRotation = 1 ;
        }
        ov69_021E6F8C(globe);
        ov69_021E62B0((WiFiEarthData *)(globe));
        ov69_021E6FE8((WiFiEarthData *)(globe));
        ov69_021E706C((WiFiEarthData *)(globe));
        FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 15, 0, 0, 216, 32);
        DrawFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_EXIT_BUTTON], 0 , 473, 7);
        if (globe->playerCountry != 0) {
            ov69_021E6A8C(globe);
        }
        ov69_021E6D5C(globe);
        globe->showLocationName = 0 ;
        globe->drawState = 1;
        *state = GEONET_GLOBE_STATE_GLOBE;
        break;
    case GEONET_GLOBE_STATE_GLOBE: {
        u16 wasZoomedIn = globe->zoomedIn;

        ov69_021E6308(globe);
        if (ov69_021E6C30(globe, &nearestLocation)) {
            DrawFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_INFO_BUTTON], 0 , 473, 7);
        } else {
            ClearWindowPixelBuffer(&globe->windows[GEONET_GLOBE_WINDOW_INFO_BUTTON], 0 );
        }
        keys = gSystem.newKeys;
        if ((keys & 0x0002 ) || ((touchInput = globe->touch.input) & 0x0002 )) {
            ClearWindowPixelBuffer(&globe->windows[GEONET_GLOBE_WINDOW_EXIT_BUTTON], 0 );
            ClearWindowPixelBuffer(&globe->windows[GEONET_GLOBE_WINDOW_INFO_BUTTON], 0 );
            PlaySE(1501 );
            FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 15, 0, 0, 216, 32);
            if (globe->playerCountry == 0) {
                globe->drawState = 2;
                *state = GEONET_GLOBE_STATE_OPEN_MENU;
            } else {
                ov69_021E6C14(globe);
                *state = GEONET_GLOBE_STATE_FADE_OUT;
            }
            break;
        }
        if ((keys & 0x0400 ) || (touchInput & 0x0400 )) {
            if (!globe->showLocationName) {
                globe->showLocationName = 1 ;
                ov69_021E6D5C(globe);
                if (globe->showLocationName == 1 ) {
                    PlaySE(1501 );
                }
                break;
            }
        }
        if (((keys & (0x0400 | 0x0001 | 0x0002 )) || (touchInput & 0x0400 )) && globe->showLocationName == 1 ) {
            globe->showLocationName = 0 ;
            ov69_021E6D5C(globe);
            break;
        }
        if (ov69_021E7198(globe, keys, gSystem.heldKeys) == 1 && globe->showLocationName == 1 ) {
            globe->showLocationName = 0 ;
            ov69_021E6D5C(globe);
        }
        if (wasZoomedIn != globe->zoomedIn) {
            *state = GEONET_GLOBE_STATE_ZOOM;
            PlaySE(1497 );
        }
        break;
    }
    case GEONET_GLOBE_STATE_ZOOM:
        if (ov69_021E737C((WiFiEarthData *)(globe)) == 1 ) {
            *state = GEONET_GLOBE_STATE_GLOBE;
        }
        break;
    case GEONET_GLOBE_STATE_FADE_OUT:
        globe->unk_C2FC = 0;
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((GXRgb)(((0) << (0)) | (( 0) << (5)) | (( 0) << (10))))) , 6, 1, globe->heapID);
        *state = GEONET_GLOBE_STATE_WAIT_FADE_OUT;
        break;
    case GEONET_GLOBE_STATE_WAIT_FADE_OUT:
        if (IsPaletteFadeFinished() == 1 ) {
            globe->drawState = 1;
            ov69_021E67B8(globe);
            ov69_021E6F48(globe);
            DestroyMsgData(globe->msgData);
            *state = GEONET_GLOBE_STATE_LOAD;
            result = 1 ;
        }
        break;
    }
    ov69_021E7408((UnkStruct_ov69_part02 *)(globe));
    return result;
}
