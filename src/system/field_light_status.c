// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/field_light_status.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/field_light_status.h"
#include "constants/arc.h"
#include "field/field_weather.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_vec.h"
#include "system/area_data.h"
#include "system/rtc.h"

// The field's light colors at a time of day, read from the light files without a field, which the battle uses to
// light its stage. The name is a guess, since the ROM has no string of this file.

// The archive of the lights that a zone has instead of its area's, by GetZoneStaticLightDataIndex
#define ZONE_LIGHT_NONE 0x0fffffff

// One of a light file's settings, which holds from the one before it until its own time of day
typedef struct {
    // The day period whose first hour the setting ends in, and the minutes after it
    u16 period;
    s16 minute;
    u8 unk04[4];
    GXRgb lightColors[4];
    VecFx16 lightDirs[4];
    GXRgb diffuse;
    GXRgb ambient;
    GXRgb specular;
    GXRgb emission;
    u8 unk30[4];
} FieldLightData;

FieldLightData *func_02014a20(u32 arcId, u32 fileId, u32 *count, HeapID heapId);
void func_02014a80(FieldLightData *data, u32 count, int seconds, u32 season, GXRgb *colors);

// The weather's lights of ARCID_LIGHTS_BATTLE
// pokebw2's sWeatherLightIds
static const u32 data_0209e054[] = {
#include "asset/field_light_status_data_0209e054.inc"
};

// pokebw2's FieldLightStatus_GetWeatherLight

void func_0201493c(u32 zoneId, u32 hour, u32 minute, u32 weather, u32 season, GXRgb *colors, HeapID heapId) {
    FieldLightData *data = NULL;
    u32 count;
    int seconds = minute * 60 + hour * 3600;

    if (weather != 0) {
        data = func_02014a20(ARCID_LIGHTS_BATTLE, func_02014a10(weather), &count, HEAPID_TAIL(heapId));
    }
    if (data == NULL) {
        u32 index = GetZoneStaticLightDataIndex(zoneId);

        if (index != ZONE_LIGHT_NONE) {
            data = func_02014a20(ARCID_LIGHTS_ZONE, index, &count, HEAPID_TAIL(heapId));
        }
    }
    if (data == NULL) {
        AreaData *areaData = AreaData_Create(HEAPID_TAIL(heapId), ZoneData_GetAreaID(zoneId), season);
        u32 lightsId = func_020140dc(areaData);

        data = func_02014a20(ARCID_LIGHTS_FIELD, lightsId, &count, HEAPID_TAIL(heapId));
        AreaData_Free(areaData);
    }
    func_02014a80(data, count, seconds, season, colors);
    GFL_HeapFree(data);
}

u16 func_02014a10(u32 index) {
    return data_0209e054[index];
}

// Reads a light file and normalizes the directions of its lights
// pokebw2's FieldLightStatus_LoadData
FieldLightData *func_02014a20(u32 arcId, u32 fileId, u32 *count, HeapID heapId) {
    u32 num = GFL_ArcSysGetDataLength(arcId, fileId) / sizeof(FieldLightData);
    FieldLightData *data;
    u32 i;
    int j;

    *count = num;
    data = GFL_ArcSysReadHeapNew(arcId, fileId, heapId);
    for (i = 0; i < num; i++) {
        for (j = 0; j < 4; j++) {
            VEC_Fx16Normalize(&data[i].lightDirs[j], &data[i].lightDirs[j]);
        }
    }
    return data;
}

// The colors of the setting that holds at the time of day
// pokebw2's FieldLightStatus_GetColors
void func_02014a80(FieldLightData *data, u32 count, int seconds, u32 season, GXRgb *colors) {
    int prev = 0;
    u32 i;
    FieldLightData *entry;

    for (i = 0; i < count; i++) {
        int hour = GetLightChangeHoursForSeasons(season, data[i].period);
        int end = hour * 3600 + data[i].minute * 60;

        if (prev <= seconds && end > seconds) {
            break;
        }
        prev = end;
    }
    entry = &data[i % count];
    colors[0] = entry->lightColors[0];
    colors[1] = entry->lightColors[1];
    colors[2] = entry->lightColors[2];
    colors[3] = entry->lightColors[3];
    colors[4] = entry->diffuse;
    colors[5] = entry->ambient;
    colors[6] = entry->specular;
    colors[7] = entry->emission;
}
