#include "global.h"

#include "bg_window.h"
#include "geonet_globe.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"

void ov69_021E6A54(GeonetGlobe *globe);

void ov69_021E6A54(GeonetGlobe *globe) {
    ClearWindowPixelBuffer(&globe->windows[GEONET_GLOBE_WINDOW_LIST_MENU], 0);
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_LIST_MENU]);
    DestroyListMenu(globe->listMenu, NULL, NULL);
    ListMenuItems_Delete(globe->menuChoices);
}

extern const WindowTemplate ov69_021E764C;

void ov69_021E6A8C(GeonetGlobe *globe);

void ov69_021E6A8C(GeonetGlobe *globe) {
    String *string;
    String *v0;

    string = String_New(0x400U, globe->heapID);
    v0 = String_New(0x400U, globe->heapID);
    AddWindow(globe->bgConfig, &globe->windows[GEONET_GLOBE_WINDOW_REGION_INFO], &ov69_021E764C);
    FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_REGION_INFO], 0xFU, 0U, 0U, 0xD8U, 0x30U);
    DrawFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_REGION_INFO], 0, 0x1D9U, 7U);
    BufferCountryName(globe->locationTemplate, 0U, globe->playerCountry);
    BufferCityName(globe->locationTemplate, 1U, globe->playerCountry, globe->playerRegion);
    ReadMsgDataIntoString(globe->messageLoader, 0xE, v0);
    StringExpandPlaceholders(globe->locationTemplate, string, v0);
    AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_REGION_INFO], FONT_SYSTEM, string, 0U, 0U, 0U, NULL);
    String_Delete(v0);
    String_Delete(string);
    CopyWindowToVram(&globe->windows[GEONET_GLOBE_WINDOW_REGION_INFO]);
}
