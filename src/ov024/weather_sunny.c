// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov074/weather_sunny.c, ported by tools/port_file.py.
#include "types.h"
#include "field/field_weather.h"

// pokebw2's SUNNY_WEATHER
WeatherData data_ov024_021ee8c0 = {
    { 0 },
    sizeof(u32),
    {
        func_ov024_021ee740,
        func_ov024_021ee788,
        func_ov024_021ee7f0,
        func_ov024_021ee838,
        func_ov024_021ee83c,
        func_ov024_021ee860,
        func_ov024_021ee874,
    },
    NULL,
};

BOOL func_ov024_021ee740(WeatherTask *task, u32 arg, HeapID heapId) {
    u32 *state;

    if (func_ov021_021a23d4(task)) {
        return FALSE;
    }
    state = func_ov021_021a21b8(task);
    *state = 0;
    if (func_ov021_021a23e8(task)) {
        func_ov021_021a2400(task);
        func_ov021_021a22e0(task, func_ov021_021a240c(task), 0x7fff, arg);
    }
    return TRUE;
}

BOOL func_ov024_021ee788(WeatherTask *task, u32 arg, HeapID heapId) {
    u32 *state = func_ov021_021a21b8(task);
    if (*state == 0) {
        if (func_ov021_021a23e8(task)) {
            u32 value = func_ov021_021a2400(task);
            func_ov021_021a232c(task, func_ov021_021a240c(task), value, 0xa0, arg);
        }
        if (func_ov021_021a23f4(task)) {
            func_ov021_021a2108(task, 0x52, func_ov021_021a2418(task));
        }
        *state = 1;
    }
    if (func_ov021_021a23b0(task)) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov024_021ee7f0(WeatherTask *task, u32 arg, HeapID heapId) {
    if (func_ov021_021a23e8(task)) {
        u32 value = func_ov021_021a2400(task);
        func_ov021_021a22e0(task, func_ov021_021a240c(task), value, arg);
    }
    if (func_ov021_021a23f4(task)) {
        func_ov021_021a2114(task, 0x52, func_ov021_021a2418(task));
    }
    return TRUE;
}

BOOL func_ov024_021ee838(WeatherTask *task, u32 arg, HeapID heapId) {
    return FALSE;
}

BOOL func_ov024_021ee83c(WeatherTask *task, u32 arg, HeapID heapId) {
    if (func_ov021_021a23e8(task)) {
        func_ov021_021a2390(task, 0x7fff, 0x50, arg);
    }
    return TRUE;
}

BOOL func_ov024_021ee860(WeatherTask *task, u32 arg, HeapID heapId) {
    if (func_ov021_021a23b0(task)) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov024_021ee874(WeatherTask *task, u32 arg, HeapID heapId) {
    if (func_ov021_021a23e8(task)) {
        func_ov021_021a231c(task, arg);
    }
    if (func_ov021_021a23f4(task)) {
        func_ov021_021a2120(task, arg);
    }
    return TRUE;
}
