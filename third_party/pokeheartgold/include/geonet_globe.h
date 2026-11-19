#ifndef POKEHEARTGOLD_GEONET_GLOBE_H
#define POKEHEARTGOLD_GEONET_GLOBE_H

#include "bg_window.h"
#include "camera.h"
#include "gf_3d_render.h"
#include "heap.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "pm_string.h"
#include "save_wifi_history.h"

struct ListMenu;
struct Menu;

enum GeonetGlobeWindow {
    GEONET_GLOBE_WINDOW_MESSAGE,
    GEONET_GLOBE_WINDOW_LIST_MENU,
    GEONET_GLOBE_WINDOW_EXIT_BUTTON,
    GEONET_GLOBE_WINDOW_LOCATION,
    GEONET_GLOBE_WINDOW_REGION_INFO,
    GEONET_GLOBE_WINDOW_COUNT,
};

enum GeonetGlobeMessageState {
    GEONET_GLOBE_MESSAGE_STATE_START,
    GEONET_GLOBE_MESSAGE_STATE_PRINTING,
    GEONET_GLOBE_MESSAGE_STATE_CONFIRM,
};

enum GeonetGlobeMarkerRecency {
    GEONET_GLOBE_RECENCY_NONE,
    GEONET_GLOBE_RECENCY_TODAY,
    GEONET_GLOBE_RECENCY_PAST,
    GEONET_GLOBE_RECENCY_SELF,
};

enum GeonetGlobeZoomMode {
    GEONET_GLOBE_ZOOM_OUT,
    GEONET_GLOBE_ZOOM_IN,
};

typedef struct GeonetGlobeMarker {
    s16 x;
    s16 y;
    MtxFx33 transform;
    u16 recency;
    u16 country;
    u16 region;
} GeonetGlobeMarker;

typedef struct GeonetGlobeMarkerList {
    u32 count;
    GeonetGlobeMarker markers[1024];
} GeonetGlobeMarkerList;

typedef struct GeonetGlobe {
    enum HeapID heapID;
    SaveWiFiHistory *wiFiHistory;
    Options *options;
    GeonetGlobeMarkerList markers;
    BgConfig *bgConfig;
    Window windows[GEONET_GLOBE_WINDOW_COUNT];
    struct ListMenu *listMenu;
    ListMenuItem *menuChoices;
    struct Menu *menu;
    MsgData *messageLoader;
    int messageState;
    int messagePrinter;
    String *messageText;
    MessageFormat *locationTemplate;
    NNSG3dRenderObj globeRenderObj;
    NNSG3dResMdl *globeModel;
    NNSG3dResFileHeader *globeResource;
    NNSG3dRenderObj markerRenderObjs[5];
    NNSG3dResMdl *markerModels[5];
    NNSG3dResFileHeader *markerResources[5];
    VecFx32 renderOrigin;
    VecFx32 globeScale;
    VecFx32 rotationAngle;
    VecFx32 markerScale;
    Camera *camera;
    CameraAngle unused_C2E0;
    u16 zoomMode;
    u16 unused_C2EA;
    VecFx32 lightVector;
    int renderMode;
    BOOL unk_C2FC;
    BOOL isJapanese;
    u16 canRotateFreely;
    u16 unused_C306;
    int touchKeys;
    int touchDragPhase;
    int lastTouchX;
    int lastTouchY;
    int touchDragX;
    int touchDragY;
    int touchHoldTimer;
    int playerCountry;
    int playerRegion;
    int selectedCountry;
    int selectedRegion;
    BOOL nonJapaneseFlag;
    BOOL inspectMode;
} GeonetGlobe; // sizeof(GeonetGlobe) == 0xC33C

void ov69_021E623C(GeonetGlobe *globe, u32 index, s16 x, s16 y, u16 country, u16 region);
void ov69_021E6138(GeonetGlobe *globe);
BOOL ov69_021E6C30(GeonetGlobe *globe, u32 *index);
BOOL ov69_021E737C(GeonetGlobe *globe);
BOOL ov69_021E7520(int country, int region, String *countryName, String *regionName, enum HeapID heapID);

#endif // POKEHEARTGOLD_GEONET_GLOBE_H
