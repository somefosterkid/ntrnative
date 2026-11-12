// Black's code at ov021 0x021a19e8-0x021a1bf4. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_weather.h"
#include "field/weather.h"
#include "field/weather_task.h"
#include "field/field_3dbg.h"


extern void (*data_ov021_021dde6c[2])(FieldWeather *task, u32 param_2);
extern const ClActSurfaceSetup data_ov021_021d8840[];
extern const Field3DBgInit data_ov021_021d8850[2];
extern const WeatherData data_ov024_021ee8c0;
extern const WeatherData data_ov025_021ef9e4;
extern const WeatherData data_ov026_021efdd8;
extern const WeatherData data_ov027_021eeba0;
extern const WeatherData data_ov025_021ef900;
extern const WeatherData data_ov025_021ef94c;
extern const WeatherData data_ov026_021efe24;
extern const WeatherData data_ov026_021efe70;
extern const WeatherData data_ov025_021ef998;
extern const WeatherData data_ov028_021eeb8c;
extern const WeatherData data_ov028_021eebd8;
extern const WeatherData data_ov028_021eec24;
extern const WeatherData data_ov027_021eebec;
extern const WeatherData data_ov028_021eec70;
extern const WeatherData data_ov028_021eeb40;




FieldWeather *func_ov021_021a19e8(FieldCamera *param_1, void *param_2, FieldFog *param_3, void *param_4, FieldSound *param_5, u16 param_6, HeapID param_7) {
    FieldWeather *task;
    s32 i;

    task = GFL_HeapAllocate(param_7, sizeof(FieldWeather), TRUE, "weather.c", 0x158);
    i = 0;
    do {
        task->unk_00[i] = func_ov021_021a15a4(&data_ov021_021d8850[i], param_7);
        i = i + 1;
    } while (i < 2);
    task->unk_08 = 0;
    task->unk_0a = 0;
    task->unk_0c = 0xffff;
    task->unk_0e = 0xffff;
    task->unk_14 = 0xffff;
    task->unk_28 = -1;
    task->unk_18 = func_0204b100(0x40, 8, param_7);
    task->unk_1c = func_0204b034(data_ov021_021d8840, 1, param_7);
    func_0204b258(task->unk_18, task->unk_1c);
    i = 0;
    do {
        task->unk_20[i] = func_ov021_021a1db4(task->unk_18, param_1, param_2, param_3, param_4, task->unk_00, param_5, param_6, param_7);
        i = i + 1;
    } while (i < 2);
    task->unk_2c = GFL_VBlankTCBAdd(func_ov021_021a1c1c, task, 0x40);
    return task;
}

void func_ov021_021a1aac(FieldWeather *task) {
    s32 i;

    GFL_TCBRemove(task->unk_2c);
    i = 0;
    do {
        func_ov021_021a1df8(task->unk_20[i]);
        i = i + 1;
    } while (i < 2);
    func_ov021_021a1c04(task);
    func_0204b084(task->unk_1c);
    func_0204b1cc(task->unk_18);
    i = 0;
    do {
        func_ov021_021a15d4(task->unk_00[i]);
        i = i + 1;
    } while (i < 2);
    GFL_HeapFree(task);
}

void func_ov021_021a1af4(FieldWeather *task, HeapID heapId) {
    s32 i;

    data_ov021_021dde6c[task->unk_08](task, heapId);
    if (task->unk_0a == 0 && task->unk_14 != 0xffff) {
        func_ov021_021a1bdc(task, task->unk_14);
        task->unk_14 = 0xffff;
    }
    i = 0;
    do {
        func_ov021_021a1e08(task->unk_20[i], heapId);
        i = i + 1;
    } while (i < 2);
}

void func_ov021_021a1b38(FieldWeather *task) {
    s32 i;

    if (task != NULL) {
        i = 0;
        do {
            func_ov021_021a15f4(task->unk_00[i]);
            i = i + 1;
        } while (i < 2);
    }
}

BOOL func_ov021_021a1b50(FieldWeather *task) {
    s32 i;

    if (task != NULL) {
        i = 0;
        do {
            if (func_ov021_021a21b0(task->unk_20[i]) == 1) {
                return 1;
            }
            i = i + 1;
        } while (i < 2);
    }
    return 0;
}

void func_ov021_021a1b74(FieldWeather *task, u16 param_2, u32 param_3) {
    s32 i;

    if (param_2 != task->unk_0c || param_2 == 0) {
        i = 0;
        do {
            func_ov021_021a20c8(task->unk_20[i]);
            i = i + 1;
        } while (i < 2);
        func_ov021_021a1c04(task);
        task->unk_0a = 0;
        func_ov021_021a1bf8(task, WEATHER_OVERLAYS[param_2].overlayId);
        func_ov021_021a1fec(task->unk_20[0], WEATHER_OVERLAYS[param_2].data, 0, 0, 1, 1, param_3);
        task->unk_0c = param_2;
        task->unk_0e = 0xffff;
    }
}


void func_ov021_021a1bdc(FieldWeather *weather, u32 weatherId) {
    func_ov021_021a1c34(weather, weatherId);
}


void func_ov021_021a1be4(FieldWeather *weather, u32 weatherId) {
    func_ov021_021a1c34(weather, weatherId);
    weather->unk_10 = 0;
}

void (*data_ov021_021dde6c[2])(FieldWeather *task, u32 param_2) = {
    func_ov021_021a1c68,
    func_ov021_021a1d04,
};

const ClActSurfaceSetup data_ov021_021d8840[1] = {{0, 0, 0x100, 0xc0, 0, 1}};
const Field3DBgInit data_ov021_021d8850[2] = {
    {0x100, 0xc0, FX32_CONST(80), 0},
    {0x100, 0xc0, FX32_CONST(88), 0},
};
const WeatherOverlay WEATHER_OVERLAYS[15] = {
    {&data_ov024_021ee8c0, 0x18},
    {&data_ov025_021ef9e4, 0x19},
    {&data_ov026_021efdd8, 0x1a},
    {&data_ov027_021eeba0, 0x1b},
    {&data_ov025_021ef900, 0x19},
    {&data_ov025_021ef94c, 0x19},
    {&data_ov026_021efe24, 0x1a},
    {&data_ov026_021efe70, 0x1a},
    {&data_ov025_021ef998, 0x19},
    {&data_ov028_021eebd8, 0x1c},
    {&data_ov028_021eeb8c, 0x1c},
    {&data_ov028_021eec24, 0x1c},
    {&data_ov027_021eebec, 0x1b},
    {&data_ov028_021eec70, 0x1c},
    {&data_ov028_021eeb40, 0x1c},
};
