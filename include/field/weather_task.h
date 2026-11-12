#ifndef BWPORT_FIELD_WEATHER_TASK_H
#define BWPORT_FIELD_WEATHER_TASK_H

#include "types.h"
#include "gfl/heap.h"
#include "field/field_weather.h"

void func_ov021_021a1c68(FieldWeather *weather, u32 param_2);
void func_ov021_021a1d04(FieldWeather *weather, u32 param_2);
BOOL func_ov021_021a1c34(FieldWeather *weather, u32 param_2);
void func_ov021_021a1e08(WeatherTask *task, HeapID heapId);
void func_ov021_021a1df8(WeatherTask *task);
void func_ov021_021a1c04(FieldWeather *weather);
void func_ov021_021a20c8(WeatherTask *task);
void func_ov021_021a1fec(WeatherTask *task, const WeatherData *data, u8 param_3, int param_4, int param_5, u32 param_6,
                         u32 param_7);
void func_ov021_021a1bf8(FieldWeather *weather, u32 param_2);
// pokebw2's WeatherTask_Create; bgs are the weather's two background layers
WeatherTask *func_ov021_021a1db4(ClActUnit *unit, FieldCamera *camera, void *light, FieldFog *fog, void *fogCtrl,
                                 Field3DBg **bgs, FieldSound *fieldSound, u16 season, HeapID heapId);
void func_ov021_021a1c1c(TCB *tcb, void *data);
u8 func_ov021_021a21b0(WeatherTask *task);

#endif // BWPORT_FIELD_WEATHER_TASK_H
