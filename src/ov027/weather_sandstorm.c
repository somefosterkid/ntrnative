// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov077/weather_sandstorm.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/field_weather.h"
#include "gfl/clact.h"
#include "gfl/random.h"

// The sandstorm weathers. The ROM doesn't name the file; the name is a guess. Grains of sand blow in from the left at
// a speed that changes with a 320-frame cycle, over a background that scrolls with them and the camera

typedef struct {
    // Frames until the fade starts, when appearing or leaving
    int timer;
    // Counts the calls to func_ov027_021eea04, 0 to 319: every 40 the wind moves on to the next of data_ov027_021eeb48 and
    // data_ov027_021eeb68
    int cycle;
    // The background's scroll, 0 to 255
    int scroll;
    int windSpeed;
    // The weather asks for 0x28 bytes of work but uses only these
    int unused[6];
} SandstormWork;

// A grain of sand
typedef struct {
    int frame;
    int lifetime;
    int speedY;
    BOOL done;
    int speedX;
    // Added to speedX every 5 frames
    int accelX;
} SandGrain;
WORK_AREA_CHECK(SandGrain, WEATHER_OBJ_WORK_SIZE);

BOOL func_ov027_021ee740(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov027_021ee7a4(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov027_021ee814(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov027_021ee888(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov027_021ee8d4(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov027_021ee908(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov027_021ee970(WeatherTask *task, u32 arg, HeapID heapId);
void func_ov027_021ee990(WeatherObj *obj);
static void func_ov027_021eea04(WeatherTask *task, int count, HeapID heapId);
void func_ov027_021eeaf4(WeatherTask *task, SandstormWork *work);

// pokebw2's sWindSpeedX
static const int data_ov027_021eeb48[8] = { 6, 10, 10, 6, 10, 12, 20, 12 };
// pokebw2's sWindSpeedY
static const int data_ov027_021eeb68[8] = { 4, 4, 4, 8, 8, 4, 4, 4 };

// pokebw2's SANDSTORM_WEATHER
WeatherData data_ov027_021eeba0 = {
    { 0x38, 1, 0, 0x22, 0x23, 0x21, 0x20, 0x2b, 0x303, 3, 0x102 },
    sizeof(SandstormWork),
    {
        func_ov027_021ee740,
        func_ov027_021ee7a4,
        func_ov027_021ee814,
        func_ov027_021ee888,
        func_ov027_021ee8d4,
        func_ov027_021ee908,
        func_ov027_021ee970,
    },
    func_ov027_021ee990,
};

// pokebw2's SANDSTORM_WEATHER_2
WeatherData data_ov027_021eebec = {
    { 0x38, 1, 1, 0x22, 0x23, 0x21, 0x20, 0x2b, 0x303, 3, 0x102 },
    sizeof(SandstormWork),
    {
        func_ov027_021ee740,
        func_ov027_021ee7a4,
        func_ov027_021ee814,
        func_ov027_021ee888,
        func_ov027_021ee8d4,
        func_ov027_021ee908,
        func_ov027_021ee970,
    },
    func_ov027_021ee990,
};

// pokebw2's Sandstorm_Main


BOOL func_ov027_021ee740(WeatherTask *task, u32 arg, HeapID heapId) {
    SandstormWork *work = func_ov021_021a21b8(task);

    func_ov021_021a21f8(task, 1, 15, 1, 2, -2, 4, 1, func_ov027_021eea04);
    func_ov021_021a22e0(task, 8, 0x7fdf, arg);
    work->timer = 1;
    work->cycle = 0;
    work->scroll = 0;
    work->windSpeed = 0;
    func_ov021_021a2424(task);
    func_ov021_021a25bc(task, 0x6e3);
    return TRUE;
}

BOOL func_ov027_021ee7a4(WeatherTask *task, u32 arg, HeapID heapId) {
    SandstormWork *work = func_ov021_021a21b8(task);
    BOOL done = func_ov021_021a223c(task, heapId);

    if (work->timer > 0) {
        if (--work->timer == 0) {
            func_ov021_021a232c(task, 8, 0x7f5e, 0x5a, arg);
            func_ov021_021a2108(task, 0x3e, func_02014a10(3));
        }
    } else if (func_ov021_021a23b0(task) && done) {
        func_ov021_021a2160(task, 1, 0);
        return TRUE;
    }
    func_ov027_021eeaf4(task, work);
    return FALSE;
}

BOOL func_ov027_021ee814(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a21b8(task);
    func_ov021_021a21f8(task, 1, 2, 1, 2, -2, 4, 1, func_ov027_021eea04);
    func_ov021_021a22e0(task, 8, 0x7f5e, arg);
    func_ov021_021a2254(task, func_ov027_021eea04, 24, 2, 2, heapId);
    func_ov021_021a2160(task, 1, 0);
    func_ov021_021a2114(task, 0x3e, func_02014a10(3));
    return TRUE;
}


static BOOL func_ov027_021ee888(WeatherTask *task, u32 arg, HeapID heapId) {
    SandstormWork *work = func_ov021_021a21b8(task);

    if (func_ov021_021a2248(task, heapId)) {
        work->windSpeed = data_ov027_021eeb48[work->cycle / 40];
        // Never true, since data_ov027_021eeb48 holds no negative speed
        if (work->windSpeed <= -6) {
            func_ov027_021eea04(task, 1, heapId);
        }
    }
    func_ov027_021eeaf4(task, work);
    return FALSE;
}

static BOOL func_ov027_021ee8d4(WeatherTask *task, u32 arg, HeapID heapId) {
    SandstormWork *work = func_ov021_021a21b8(task);

    func_ov021_021a2228(task, 0, 15, 2, -1);
    work->timer = 31;
    func_ov021_021a2160(task, 0, 0);
    func_ov021_021a25cc(task);
    return TRUE;
}


BOOL func_ov027_021ee908(WeatherTask *task, u32 arg, HeapID heapId) {
    SandstormWork *work = func_ov021_021a21b8(task);
    BOOL done = func_ov021_021a223c(task, heapId);

    if (work->timer > 0) {
        if (--work->timer == 0) {
            func_ov021_021a2390(task, 0x7fdf, 0x5a, arg);
        }
    } else if ((arg ? func_ov021_021a23b0(task) : TRUE) && done && !func_ov021_021a21bc(task)) {
        return TRUE;
    }
    func_ov027_021eeaf4(task, work);
    return FALSE;
}


// pokebw2's Sandstorm_Exit
static BOOL func_ov027_021ee970(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a21b8(task);
    func_ov021_021a231c(task, arg);
    func_ov021_021a2120(task, arg);
    return TRUE;
}

void func_ov027_021ee990(WeatherObj *obj) {
    SandGrain *grain = func_ov021_021a2630(obj);
    ClActorPos pos;

    func_ov021_021a2638(obj);
    func_ov021_021a2640(obj, &pos);
    if (!grain->done) {
        pos.x += grain->speedX;
        pos.y += grain->speedY;
        if (grain->frame % 5 == 0) {
            grain->speedX += grain->accelX;
        }
        if (grain->frame++ > grain->lifetime) {
            grain->done = TRUE;
        }
        func_ov021_021a2654(obj, &pos);
    } else {
        func_ov021_021a21e0(obj);
    }
}

static void func_ov027_021eea04(WeatherTask *task, int count, HeapID heapId) {
    SandstormWork *work = func_ov021_021a21b8(task);
    int index;
    int i;

    work->cycle = (work->cycle + 1) % 320;
    index = work->cycle / 40;
    for (i = 0; i < count; i++) {
        WeatherObj *obj;
        SandGrain *grain;
        ClActor *actor;
        int size;
        ClActorPos pos;

        obj = func_ov021_021a21c8(task, heapId);
        if (obj == NULL) {
            return;
        }
        grain = func_ov021_021a2630(obj);
        actor = func_ov021_021a2638(obj);
        grain->frame = 0;
        grain->lifetime = GFL_RandomLC(20) + 15;
        size = 3 - (grain->lifetime - 15) / 6;
        grain->speedY = data_ov027_021eeb68[index] * (size + 1);
        grain->speedX = (size + 1) * data_ov027_021eeb48[index];
        grain->done = FALSE;
        grain->accelX = data_ov027_021eeb48[index];
        if (GFL_RandomLC(1000) == 777) {
            size = 4;
            grain->speedY += grain->speedY / 2;
        }
        func_0204b908(actor, size);
        pos.x = -6 - GFL_RandomLC(24);
        pos.y = GFL_RandomLC(192) - 64;
        func_ov021_021a2654(obj, &pos);
    }
}

void func_ov027_021eeaf4(WeatherTask *task, SandstormWork *work) {
    int x;
    int y;

    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    work->scroll = (work->scroll + 6) % 256;
    func_ov021_021a2190(task, -work->scroll * 2 - x, 0);
    func_ov021_021a21a0(task, -work->scroll * 2 + y, 0);
}




