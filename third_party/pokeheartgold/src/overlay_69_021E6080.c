#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "geonet_globe.h"
#include "geonet_locations.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "overlay_69.h"
#include "overlay_manager.h"
#include "save_wifi_history.h"
#include "system.h"

BOOL GeonetGlobe_Exit(OverlayManager *man, int *state) {
    GeonetGlobe *globe = OverlayManager_GetData(man);
    enum HeapID heapID = globe->heapID;

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, 0);
    Camera_Delete(globe->camera);
    MessageFormat_Delete(globe->locationTemplate);
    GF3dRender_DeleteSimpleManager();
    Heap_Free(globe->bgConfig);
    Main_SetVBlankIntrCB(NULL, NULL);
    OverlayManager_FreeData(man);
    Heap_Destroy(heapID);

    gSystem.screensFlipped = FALSE;

    return TRUE;
}

void ov69_021E60F8(void);

extern const GraphicsBanks ov69_021E77B8;

void ov69_021E60F8(void) {
    GraphicsBanks banks = ov69_021E77B8;

    GfGfx_SetBanks(&banks);
}

void ov69_021E6118(void);

extern const GraphicsModes ov69_021E7688;

void ov69_021E6118(void) {
    GraphicsModes modes = ov69_021E7688;

    SetBothScreensModesAndDisable(&modes);
}

typedef struct GeonetGlobePlaceRecord {
    u16 type;
    s16 x;
    s16 y;
} GeonetGlobePlaceRecord;

typedef struct GeonetGlobeRegionRecord {
    s16 x;
    s16 y;
} GeonetGlobeRegionRecord;

void ov69_021E6138(GeonetGlobe *globe) {
    NARC *narc = NARC_New(NARC_a_0_9_8, globe->heapID);

    globe->markers.count = 0;

    GeonetGlobePlaceRecord *record;
    u32 byteSize;

    void *placeBuffer = GfGfxLoader_LoadFromOpenNarc_GetSizeOut(narc, 18, 0, globe->heapID, 0, &byteSize);
    record = (GeonetGlobePlaceRecord *)placeBuffer;
    int recordCount = byteSize / sizeof(GeonetGlobePlaceRecord);

    record++;

    for (int i = 1; i < recordCount; i++) {
        if (record->type != 2) {
            ov69_021E623C(globe, globe->markers.count, record->x, record->y, i, 0);
            globe->markers.count++;
        }
        record++;
    }

    Heap_Free(placeBuffer);

    void *regionBuffer;
    GeonetGlobeRegionRecord *regionRecord;
    u32 regionByteSize, narcMemberIndex;
    int countryCount, regionRecordCount;

    int countryIndex = 1;
    countryCount = GeonetLocations_GetLocationListCount();

    while (countryIndex < countryCount) {
        narcMemberIndex = GeonetLocations_GetNarcMemberIndex(countryIndex);
        regionBuffer = GfGfxLoader_LoadFromOpenNarc_GetSizeOut(narc, narcMemberIndex, 0, globe->heapID, 0, &regionByteSize);
        regionRecord = (GeonetGlobeRegionRecord *)regionBuffer;
        regionRecordCount = regionByteSize / sizeof(GeonetGlobeRegionRecord);

        regionRecord++;

        for (int i = 1; i < regionRecordCount; i++) {
            ov69_021E623C(globe, globe->markers.count, regionRecord->x, regionRecord->y, GeonetLocations_GetCountry(countryIndex), i);
            globe->markers.count++;
            regionRecord++;
        }

        Heap_Free(regionBuffer);
        countryIndex++;
    }

    NARC_Delete(narc);
}

extern const MtxFx33 ov69_021E7770;

void ov69_021E7120(MtxFx33 *param0, const VecFx32 *param1);

void ov69_021E623C(GeonetGlobe *globe, u32 index, s16 x, s16 y, u16 country, u16 region) {
    MtxFx33 transform = ov69_021E7770;
    VecFx32 position;

    globe->markers.markers[index].x = x;
    globe->markers.markers[index].y = y;

    position.x = x;
    position.y = y;
    position.z = 0;

    ov69_021E7120(&transform, &position);

    globe->markers.markers[index].transform = transform;
    globe->markers.markers[index].recency = WiFiHistory_GetLocationSeenState(globe->wiFiHistory, country, region);
    globe->markers.markers[index].country = country;
    globe->markers.markers[index].region = region;
}
