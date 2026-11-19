#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "geonet_globe.h"
#include "gf_3d_render.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "render_text.h"
#include "save_wifi_history.h"
#include "system.h"
#include "text.h"

s32 ov69_021E60F8(void);

s32 ov69_021E6118(void);

s32 GeonetGlobe_Init(OverlayManager *man, s32 *state);

s32 GeonetGlobe_Init(OverlayManager *man, s32 *state) {
    GeonetGlobe *globe;
    SaveData *saveData;
    SaveWiFiHistory *wifiHistory;
    s32 v0;

    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    Heap_Create(HEAP_ID_3, HEAP_ID_49, 0x200080000U);
    globe = OverlayManager_CreateAndGetData(man, sizeof(GeonetGlobe), HEAP_ID_49);
    memset(globe, 0, sizeof(GeonetGlobe));
    globe->heapID = HEAP_ID_49;
    if (gGameLanguage == 1) {
        v0 = 1;
    } else {
        v0 = 0;
    }
    globe->isJapanese = v0;
    saveData = OverlayManager_GetArgs(man);
    wifiHistory = Save_WiFiHistory_Get(saveData);
    globe->wiFiHistory = wifiHistory;
    globe->playerCountry = WifiHistory_GetPlayerCountry(wifiHistory);
    globe->playerRegion = WiFiHistory_GetPlayerRegion(globe->wiFiHistory);
    globe->nonJapaneseFlag = WiFiHistory_GetNonJapaneseFlag(globe->wiFiHistory);
    globe->options = Save_PlayerData_GetOptionsAddr(saveData);
    ov69_021E60F8();
    ov69_021E6118();
    GF3dRender_InitSimpleManager((enum HeapID)(u8)globe->heapID);
    globe->bgConfig = BgConfig_Alloc(globe->heapID);
    GfGfx_BothDispOn();
    ResetAllTextPrinters();
    globe->locationTemplate = MessageFormat_New_Custom(8U, 0x40U, globe->heapID);
    globe->camera = Camera_New(globe->heapID);
    globe->renderMode = 0;
    gSystem.screensFlipped = 1;
    GfGfx_SwapDisplay();
    SetKeyRepeatTimers(4, 8);
    TextFlags_SetCanABSpeedUpPrint(1);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    ov69_021E6138(globe);
    return 1;
}
