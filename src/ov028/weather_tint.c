// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov078/weather_tint.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/field_weather.h"

// The weathers that only tint the screen. The ROM doesn't name the file; the name is a guess. Each fades a level and an
// RGB555 colour in and out and plays a sound. Weather 9 is fixed. The others (10, 11, 13 and 14) pick the level and the
// colour by the camera's defaults index, and share all but their first function

#define TINT_START_COLOR 0x7fdf
#define TINT_FADE_IN_FRAMES 0x5a
#define TINT_FADE_OUT_FRAMES 0x3c
#define TINT_SOUND_TYPE 0x3e
#define TINT_DEFAULTS_COUNT 34

// Weather 9's work
typedef struct {
    // 0 until the fade-in starts
    int state;
    int unused[9];
    u32 level;
    u32 color;
} Tint9Work;

// The work of the other weathers
typedef struct {
    // The index of the sound
    int sound;
    // 0 until the fade-in starts
    int state;
    int unused[9];
    u32 level;
    u32 color;
} TintWork;

static BOOL func_ov028_021ee740(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021ee770(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021ee7c0(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021ee7ec(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021ee7f0(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021ee800(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021ee84c(WeatherTask *task, u32 arg, HeapID heapId);
static void func_ov028_021ee864(WeatherObj *obj);
static BOOL func_ov028_021ee868(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021ee8b0(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021ee8f8(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021ee940(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021ee988(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021ee9d8(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021eea04(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021eea08(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021eea18(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov028_021eea64(WeatherTask *task, u32 arg, HeapID heapId);
static void func_ov028_021eea7c(WeatherObj *obj);

// The level of the tint by the camera's defaults index
// pokebw2's sLevels
static const u8 data_ov028_021eea80[TINT_DEFAULTS_COUNT] = {
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8,
};

// The colour of the tint by the camera's defaults index
// pokebw2's sColors
static const u32 data_ov028_021eeaa4[TINT_DEFAULTS_COUNT] = {
    0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x0000,
    0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f,
    0x7f5f, 0x7f5f, 0x0a08, 0x7f5f, 0x7f6f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f, 0x7f5f,
    0x7f5f,
};

// pokebw2's TINT_WEATHER_9
WeatherData data_ov028_021eebd8 = {
    { 0x38 },
    sizeof(Tint9Work),
    {
        func_ov028_021ee740,
        func_ov028_021ee770,
        func_ov028_021ee7c0,
        func_ov028_021ee7ec,
        func_ov028_021ee7f0,
        func_ov028_021ee800,
        func_ov028_021ee84c,
    },
    func_ov028_021ee864,
};

// pokebw2's TINT_WEATHER_10
WeatherData data_ov028_021eeb8c = {
    { 0x38 },
    sizeof(TintWork),
    {
        func_ov028_021ee868,
        func_ov028_021ee988,
        func_ov028_021ee9d8,
        func_ov028_021eea04,
        func_ov028_021eea08,
        func_ov028_021eea18,
        func_ov028_021eea64,
    },
    func_ov028_021eea7c,
};

// pokebw2's TINT_WEATHER_11
WeatherData data_ov028_021eec24 = {
    { 0x38 },
    sizeof(TintWork),
    {
        func_ov028_021ee8b0,
        func_ov028_021ee988,
        func_ov028_021ee9d8,
        func_ov028_021eea04,
        func_ov028_021eea08,
        func_ov028_021eea18,
        func_ov028_021eea64,
    },
    func_ov028_021eea7c,
};

// pokebw2's TINT_WEATHER_13
WeatherData data_ov028_021eec70 = {
    { 0x38 },
    sizeof(TintWork),
    {
        func_ov028_021ee8f8,
        func_ov028_021ee988,
        func_ov028_021ee9d8,
        func_ov028_021eea04,
        func_ov028_021eea08,
        func_ov028_021eea18,
        func_ov028_021eea64,
    },
    func_ov028_021eea7c,
};

// pokebw2's TINT_WEATHER_14
WeatherData data_ov028_021eeb40 = {
    { 0x38 },
    sizeof(TintWork),
    {
        func_ov028_021ee940,
        func_ov028_021ee988,
        func_ov028_021ee9d8,
        func_ov028_021eea04,
        func_ov028_021eea08,
        func_ov028_021eea18,
        func_ov028_021eea64,
    },
    func_ov028_021eea7c,
};

// pokebw2's Tint9_Init
static BOOL func_ov028_021ee740(WeatherTask *task, u32 arg, HeapID heapId) {
    Tint9Work *work = func_ov021_021a21b8(task);

    func_ov021_021a22e0(task, 9, TINT_START_COLOR, arg);
    work->state = 0;
    work->level = 9;
    work->color = TINT_START_COLOR - 0x67;
    return TRUE;
}

// pokebw2's Tint9_FadeIn
static BOOL func_ov028_021ee770(WeatherTask *task, u32 arg, HeapID heapId) {
    Tint9Work *work = func_ov021_021a21b8(task);

    if (work->state == 0) {
        func_ov021_021a232c(task, work->level, work->color, TINT_FADE_IN_FRAMES, arg);
        func_ov021_021a2108(task, TINT_SOUND_TYPE, func_02014a10(9));
        work->state--;
    } else if (func_ov021_021a23b0(task)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Tint9_InitNoFade
static BOOL func_ov028_021ee7c0(WeatherTask *task, u32 arg, HeapID heapId) {
    Tint9Work *work = func_ov021_021a21b8(task);

    func_ov021_021a22e0(task, work->level, work->color, arg);
    func_ov021_021a2114(task, TINT_SOUND_TYPE, func_02014a10(9));
    return TRUE;
}

// pokebw2's Tint9_Main
static BOOL func_ov028_021ee7ec(WeatherTask *task, u32 arg, HeapID heapId) {
    return FALSE;
}

// pokebw2's Tint9_InitFadeOut
static BOOL func_ov028_021ee7f0(WeatherTask *task, u32 arg, HeapID heapId) {
    Tint9Work *work = func_ov021_021a21b8(task);

    work->state = 0;
    return TRUE;
}

// pokebw2's Tint9_FadeOut
static BOOL func_ov028_021ee800(WeatherTask *task, u32 arg, HeapID heapId) {
    Tint9Work *work = func_ov021_021a21b8(task);

    if (work->state == 0) {
        work->state--;
        func_ov021_021a2390(task, TINT_START_COLOR, TINT_FADE_OUT_FRAMES, arg);
    } else if ((arg != 0 ? func_ov021_021a23b0(task) : TRUE) && !func_ov021_021a21bc(task)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Tint9_Exit
static BOOL func_ov028_021ee84c(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a231c(task, arg);
    func_ov021_021a2120(task, arg);
    return TRUE;
}

// pokebw2's Tint9_ObjFunc
static void func_ov028_021ee864(WeatherObj *obj) {
}

// pokebw2's Tint10_Init
static BOOL func_ov028_021ee868(WeatherTask *task, u32 arg, HeapID heapId) {
    u32 index = func_ov021_021a2148(task);
    TintWork *work = func_ov021_021a21b8(task);

    work->state = 0;
    work->sound = 10;
    work->level = data_ov028_021eea80[index];
    work->color = data_ov028_021eeaa4[index];
    func_ov021_021a22e0(task, work->level, TINT_START_COLOR, arg);
    return TRUE;
}

// pokebw2's Tint11_Init
static BOOL func_ov028_021ee8b0(WeatherTask *task, u32 arg, HeapID heapId) {
    u32 index = func_ov021_021a2148(task);
    TintWork *work = func_ov021_021a21b8(task);

    work->state = 0;
    work->sound = 11;
    work->level = data_ov028_021eea80[index];
    work->color = data_ov028_021eeaa4[index];
    func_ov021_021a22e0(task, work->level, TINT_START_COLOR, arg);
    return TRUE;
}

// pokebw2's Tint13_Init
static BOOL func_ov028_021ee8f8(WeatherTask *task, u32 arg, HeapID heapId) {
    u32 index = func_ov021_021a2148(task);
    TintWork *work = func_ov021_021a21b8(task);

    work->state = 0;
    work->sound = 13;
    work->level = data_ov028_021eea80[index];
    work->color = data_ov028_021eeaa4[index];
    func_ov021_021a22e0(task, work->level, TINT_START_COLOR, arg);
    return TRUE;
}

// pokebw2's Tint14_Init
static BOOL func_ov028_021ee940(WeatherTask *task, u32 arg, HeapID heapId) {
    u32 index = func_ov021_021a2148(task);
    TintWork *work = func_ov021_021a21b8(task);

    work->state = 0;
    work->sound = 14;
    work->level = data_ov028_021eea80[index];
    work->color = data_ov028_021eeaa4[index];
    func_ov021_021a22e0(task, work->level, TINT_START_COLOR, arg);
    return TRUE;
}

// pokebw2's Tint_FadeIn
static BOOL func_ov028_021ee988(WeatherTask *task, u32 arg, HeapID heapId) {
    TintWork *work = func_ov021_021a21b8(task);

    if (work->state == 0) {
        func_ov021_021a232c(task, work->level, work->color, TINT_FADE_IN_FRAMES, arg);
        func_ov021_021a2108(task, TINT_SOUND_TYPE, func_02014a10(work->sound));
        work->state--;
    } else if (func_ov021_021a23b0(task)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Tint_InitNoFade
static BOOL func_ov028_021ee9d8(WeatherTask *task, u32 arg, HeapID heapId) {
    TintWork *work = func_ov021_021a21b8(task);

    func_ov021_021a22e0(task, work->level, work->color, arg);
    func_ov021_021a2114(task, TINT_SOUND_TYPE, func_02014a10(work->sound));
    return TRUE;
}

// pokebw2's Tint_Main
static BOOL func_ov028_021eea04(WeatherTask *task, u32 arg, HeapID heapId) {
    return FALSE;
}

// pokebw2's Tint_InitFadeOut
static BOOL func_ov028_021eea08(WeatherTask *task, u32 arg, HeapID heapId) {
    TintWork *work = func_ov021_021a21b8(task);

    work->state = 0;
    return TRUE;
}

// pokebw2's Tint_FadeOut
static BOOL func_ov028_021eea18(WeatherTask *task, u32 arg, HeapID heapId) {
    TintWork *work = func_ov021_021a21b8(task);

    if (work->state == 0) {
        work->state--;
        func_ov021_021a2390(task, TINT_START_COLOR, TINT_FADE_OUT_FRAMES, arg);
    } else if ((arg != 0 ? func_ov021_021a23b0(task) : TRUE) && !func_ov021_021a21bc(task)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Tint_Exit
static BOOL func_ov028_021eea64(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a231c(task, arg);
    func_ov021_021a2120(task, arg);
    return TRUE;
}

// pokebw2's Tint_ObjFunc
static void func_ov028_021eea7c(WeatherObj *obj) {
}
