#ifndef BWPORT_FIELD_WEATHER_H
#define BWPORT_FIELD_WEATHER_H

#include "types.h"
#include "gfl/heap.h"
#include "field/field_weather.h"

typedef struct {
    const WeatherData *data;
    u32 overlayId;
} WeatherOverlay;

extern const WeatherOverlay WEATHER_OVERLAYS[];
void func_ov021_021a1af4(FieldWeather *weather, HeapID heapId);
// pokebw2's func_ov036_02199004: the field's weather and its two tasks
FieldWeather *func_ov021_021a19e8(FieldCamera *camera, void *light, FieldFog *fog, void *fogCtrl, FieldSound *fieldSound,
                                  u16 season, HeapID heapId);
void func_ov021_021a1aac(FieldWeather *weather);
void func_ov021_021a1b38(FieldWeather *weather);
// TRUE while one of the two tasks fades
BOOL func_ov021_021a1b50(FieldWeather *weather);
void func_ov021_021a1b74(FieldWeather *weather, u16 weatherId, u32 heapId);

#endif
