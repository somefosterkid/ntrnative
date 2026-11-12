// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov075/weather_particles.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/field_weather.h"
#include "gfl/clact.h"
#include "gfl/random.h"

// Four weathers with particles, 1, 4, 5 and 8, in the order of their code. The ROM doesn't name the file; the name is
// a guess, and so are the weathers' names, which only say what the particles do: flakes that sway down, a blizzard of
// flakes that rush across a scrolling background, flakes that flutter, and specks that drift. Overlay 36's weather
// table (WEATHER_OVERLAYS) loads them. Like overlay 77's, they share the shape of WeatherData's functions

#define FLAKE_START_COLOR 0x7fdf
#define WEATHER_SOUND_TYPE 0x3e // Black's

// Weather 1's work
// Black 2's; twinned code confirms the offsets of timer, mode and done in Black
typedef struct {
    // Frames until the fade starts, when appearing
    int timer;
    // 1 for the second half of the flakes of the first batch
    int mode;
    // Set when the fade-out ends; the flakes then leave
    int done;
    int unused[9];
} FlakeWork;

// One of weather 1's flakes
// Black 2's; twinned code confirms the offsets of mode, rate, acc, swayIdx, done, state and timer in Black
typedef struct {
    // Points to FlakeWork's mode
    int *mode;
    int unused04[2];
    // The y step is taken when acc passes it
    int rate;
    int acc;
    // The index of data_ov025_021ef790
    int swayIdx;
    int unused18;
    // Points to FlakeWork's done
    int *done;
    // 0 until the weather is done, then 1
    int state;
    // Frames left before the flake goes, once the weather is done
    int timer;
} Flake;
WORK_AREA_CHECK(Flake, WEATHER_OBJ_WORK_SIZE);

// Weather 4's work
// Black 2's; twinned code confirms the offsets of timer, cycle, scrollX, scrollStep and scrollY in Black
typedef struct {
    int timer;
    // Counts the calls to func_ov025_021eee04, 0 to 1023: every 256 the wind changes
    int cycle;
    // The background's scroll, 0 to 255
    int scrollX;
    int unused0C[5];
    // The step of the background's scroll, 0 to 251: data_ov025_021ef765's and data_ov025_021ef75c's entry every 28
    int scrollStep;
    int scrollY;
} BlizzardWork;

// One of weather 4's flakes
typedef struct {
    int frame;
    int lifetime;
    int speedY;
    // 0 while it moves, 1 once its lifetime is over
    int state;
    int speedX;
    // Added to speedX every 4 frames
    int accelX;
} BlizzardFlake;
WORK_AREA_CHECK(BlizzardFlake, WEATHER_OBJ_WORK_SIZE);

// Weather 5's work
// Black 2's; twinned code confirms the offsets of timer in Black
typedef struct {
    int timer;
    int unused[9];
} FlutterWork;

// One of weather 5's flakes
// Black 2's; twinned code confirms the offsets of frame, lifetime, speedY, phase, speedX and seed in Black
typedef struct {
    int frame;
    int lifetime;
    int speedY;
    // 0 while it falls, 1 while it flutters, 2 once it is gone
    int phase;
    int speedX;
    int unused14;
    int seed;
} FlutterFlake;
WORK_AREA_CHECK(FlutterFlake, WEATHER_OBJ_WORK_SIZE);

// Weather 8's work
// Black 2's; twinned code confirms the offsets of timer in Black
typedef struct {
    int timer;
    int unused[9];
} SpeckWork;

// One of weather 8's specks
typedef struct {
    int frame;
    int lifetime;
    int stepX;
    int stepY;
    // The specks move every periodX frames in x and every periodY frames in y
    int periodX;
    int periodY;
} Speck;
WORK_AREA_CHECK(Speck, WEATHER_OBJ_WORK_SIZE);

static BOOL func_ov025_021ee740(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ee798(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ee810(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ee888(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ee8b0(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ee8ec(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ee968(WeatherTask *task, u32 arg, HeapID heapId);
static void func_ov025_021ee980(WeatherObj *obj);
static void func_ov025_021eea34(WeatherTask *task, int count, HeapID heapId);
static BOOL func_ov025_021eeb0c(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021eeb74(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021eebe4(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021eec64(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021eecb0(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021eecec(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021eed58(WeatherTask *task, u32 arg, HeapID heapId);
static void func_ov025_021eed78(WeatherObj *obj);
static void func_ov025_021eee04(WeatherTask *task, int count, HeapID heapId);
static void func_ov025_021eeee0(WeatherTask *task, BlizzardWork *work);
static BOOL func_ov025_021eef94(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021eeff0(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ef068(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ef0d0(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ef100(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ef12c(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ef1a4(WeatherTask *task, u32 arg, HeapID heapId);
static void func_ov025_021ef1c4(WeatherObj *obj);
static void func_ov025_021ef2d4(WeatherTask *task, int count, HeapID heapId);
static BOOL func_ov025_021ef3bc(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ef404(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ef47c(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ef4e4(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ef514(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ef538(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov025_021ef5b0(WeatherTask *task, u32 arg, HeapID heapId);
static void func_ov025_021ef5d0(WeatherObj *obj);
static void func_ov025_021ef634(WeatherTask *task, int count, HeapID heapId);

// What the background's scroll adds each step
// pokebw2's sBlizzardScrollX
static const s8 data_ov025_021ef765[9] = { 7, 8, 8, 8, 8, 8, 7, 7, 7 };
// pokebw2's sBlizzardScrollY
static const s8 data_ov025_021ef75c[9] = { 3, 2, 1, 2, 1, 2, 3, 2, 3 };
// pokebw2's sBlizzardSpeedY
static const int data_ov025_021ef780[4] = { 3, 6, 3, 6 };
// pokebw2's sBlizzardSpeedX
static const int data_ov025_021ef770[4] = { 7, 11, 15, 19 };

// How far a flake moves in x at each step
// pokebw2's sFlakeSway
static const s8 data_ov025_021ef790[0x151] = {
#include "asset/weather_particles_data_ov025_021ef790.inc"
};

// pokebw2's SPECK_WEATHER
WeatherData data_ov025_021ef998 = {
    { 0x38, 1, 0, 0x1b, 0x1c, 0x1a, 0x19 },
    sizeof(SpeckWork),
    {
        func_ov025_021ef3bc,
        func_ov025_021ef404,
        func_ov025_021ef47c,
        func_ov025_021ef4e4,
        func_ov025_021ef514,
        func_ov025_021ef538,
        func_ov025_021ef5b0,
    },
    func_ov025_021ef5d0,
};

// pokebw2's BLIZZARD_WEATHER
WeatherData data_ov025_021ef900 = {
    { 0x38, 1, 1, 0x1f, 0x18, 0x1e, 0x1d, 0x2a, 0x303, 3, 0x102 },
    sizeof(BlizzardWork),
    {
        func_ov025_021eeb0c,
        func_ov025_021eeb74,
        func_ov025_021eebe4,
        func_ov025_021eec64,
        func_ov025_021eecb0,
        func_ov025_021eecec,
        func_ov025_021eed58,
    },
    func_ov025_021eed78,
};

// pokebw2's FLUTTER_WEATHER
WeatherData data_ov025_021ef94c = {
    { 0x38, 1, 0, 2, 3, 1 },
    sizeof(FlutterWork),
    {
        func_ov025_021eef94,
        func_ov025_021eeff0,
        func_ov025_021ef068,
        func_ov025_021ef0d0,
        func_ov025_021ef100,
        func_ov025_021ef12c,
        func_ov025_021ef1a4,
    },
    func_ov025_021ef1c4,
};

// pokebw2's FLAKE_WEATHER
WeatherData data_ov025_021ef9e4 = {
    { 0x38, 1, 0, 0x17, 0x18, 0x16, 0x15 },
    sizeof(FlakeWork),
    {
        func_ov025_021ee740,
        func_ov025_021ee798,
        func_ov025_021ee810,
        func_ov025_021ee888,
        func_ov025_021ee8b0,
        func_ov025_021ee8ec,
        func_ov025_021ee968,
    },
    func_ov025_021ee980,
};

// pokebw2's Flake_Init
static BOOL func_ov025_021ee740(WeatherTask *task, u32 arg, HeapID heapId) {
    FlakeWork *work = func_ov021_021a21b8(task);

    func_ov021_021a21f8(task, 1, 18, 1, 16, -3, 1, 0, func_ov025_021eea34);
    work->mode = 0;
    work->done = 0;
    func_ov021_021a22e0(task, 8, FLAKE_START_COLOR, arg);
    work->timer = 8;
    func_ov021_021a2424(task);
    return TRUE;
}

// pokebw2's Flake_FadeIn
static BOOL func_ov025_021ee798(WeatherTask *task, u32 arg, HeapID heapId) {
    FlakeWork *work = func_ov021_021a21b8(task);
    BOOL fading = func_ov021_021a223c(task, heapId);
    int x;
    int y;

    if (work->timer > 0) {
        if (--work->timer == 0) {
            func_ov021_021a232c(task, 8, 0x7f5f, 0x5a, arg);
            func_ov021_021a2108(task, WEATHER_SOUND_TYPE, func_02014a10(1));
        }
    } else if (func_ov021_021a23b0(task) && fading) {
        return TRUE;
    }
    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    return FALSE;
}

// pokebw2's Flake_InitNoFade
static BOOL func_ov025_021ee810(WeatherTask *task, u32 arg, HeapID heapId) {
    FlakeWork *work = func_ov021_021a21b8(task);

    func_ov021_021a21f8(task, 1, 16, 1, 16, -3, 1, 0, func_ov025_021eea34);
    work->mode = 1;
    work->done = 0;
    func_ov021_021a22e0(task, 8, 0x7f5f, arg);
    func_ov021_021a2254(task, func_ov025_021eea34, 20, 1, 18, heapId);
    func_ov021_021a2114(task, WEATHER_SOUND_TYPE, func_02014a10(1));
    return TRUE;
}

// pokebw2's Flake_Main
static BOOL func_ov025_021ee888(WeatherTask *task, u32 arg, HeapID heapId) {
    int x;
    int y;

    func_ov021_021a2248(task, heapId);
    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    return FALSE;
}

// pokebw2's Flake_InitFadeOut
static BOOL func_ov025_021ee8b0(WeatherTask *task, u32 arg, HeapID heapId) {
    FlakeWork *work = func_ov021_021a21b8(task);
    int x;
    int y;

    func_ov021_021a2228(task, 0, 18, 3, -1);
    work->timer = 16;
    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    return TRUE;
}

// pokebw2's Flake_FadeOut
static BOOL func_ov025_021ee8ec(WeatherTask *task, u32 arg, HeapID heapId) {
    FlakeWork *work = func_ov021_021a21b8(task);
    BOOL fading = func_ov021_021a223c(task, heapId);
    int x;
    int y;

    if (work->timer > 0) {
        if (--work->timer == 0) {
            func_ov021_021a2390(task, FLAKE_START_COLOR, 0x5a, arg);
        }
    } else if ((arg ? func_ov021_021a23b0(task) : TRUE) && fading) {
        work->done = TRUE;
        if (!func_ov021_021a21bc(task)) {
            return TRUE;
        }
    }
    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    return FALSE;
}

// pokebw2's Flake_Exit
static BOOL func_ov025_021ee968(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a231c(task, arg);
    func_ov021_021a2120(task, arg);
    return TRUE;
}

// pokebw2's Flake_Move
static void func_ov025_021ee980(WeatherObj *obj) {
    Flake *flake = func_ov021_021a2630(obj);
    ClActorPos pos;
    int *done;

    func_ov021_021a2638(obj);
    done = flake->done;
    if (flake->state == 0) {
        if (*done != 0) {
            flake->state++;
        }
    } else if (--flake->timer < 0) {
        func_ov021_021a21e0(obj);
        return;
    }
    func_ov021_021a2640(obj, &pos);
    pos.x += data_ov025_021ef790[flake->swayIdx];
    if (flake->acc > flake->rate) {
        pos.y++;
        flake->acc %= flake->rate;
    }
    func_ov021_021a2654(obj, &pos);
    flake->acc += 8;
    flake->swayIdx = (flake->swayIdx + 1) % NELEMS(data_ov025_021ef790);
    if ((pos.y < -0x11c && pos.y > -0x128) || (pos.y > 0xd4 && pos.y < 0xe8)) {
        *flake->mode = TRUE;
        func_ov021_021a21e0(obj);
    }
}

// pokebw2's Flake_Add
static void func_ov025_021eea34(WeatherTask *task, int count, HeapID heapId) {
    FlakeWork *work = func_ov021_021a21b8(task);
    int i = 0;

    if (count > 0) {
        int *mode = &work->mode;
        int *done = &work->done;

        do {
            WeatherObj *obj = func_ov021_021a21c8(task, heapId);
            Flake *flake;
            ClActor *actor;
            ClActorPos pos;

            if (obj == NULL) {
                break;
            }
            flake = func_ov021_021a2630(obj);
            actor = func_ov021_021a2638(obj);
            func_0204b908(actor, GFL_RandomLC(4));
            flake->acc = 0;
            flake->swayIdx = 0;
            flake->state = 0;
            flake->timer = (int)GFL_RandomLC(0) % 30 + 2;
            flake->rate = GFL_RandomLC(8) + 12;
            flake->mode = mode;
            flake->done = done;
            pos.x = GFL_RandomLC(0x19e) - 0x20;
            if (work->mode == 1 && i >= count / 2) {
                pos.y = -8 - 0x20 - GFL_RandomLC(20);
            } else {
                pos.y = -8 - GFL_RandomLC(20);
            }
            func_ov021_021a2654(obj, &pos);
        } while (++i < count);
    }
}

// pokebw2's Blizzard_Init
static BOOL func_ov025_021eeb0c(WeatherTask *task, u32 arg, HeapID heapId) {
    BlizzardWork *work = func_ov021_021a21b8(task);

    func_ov021_021a21f8(task, 1, 6, 1, 2, -2, 2, 1, func_ov025_021eee04);
    func_ov021_021a22e0(task, 4, 0x7fff, arg);
    work->timer = 8;
    work->cycle = 0;
    work->scrollX = 0;
    work->scrollY = 0;
    work->scrollStep = 0;
    func_ov021_021a2424(task);
    func_ov021_021a25bc(task, 0x6e3);
    return TRUE;
}

// pokebw2's Blizzard_FadeIn
static BOOL func_ov025_021eeb74(WeatherTask *task, u32 arg, HeapID heapId) {
    BlizzardWork *work = func_ov021_021a21b8(task);
    BOOL fading = func_ov021_021a223c(task, heapId);

    if (work->timer > 0) {
        if (--work->timer == 0) {
            func_ov021_021a232c(task, 4, 0x7b18, 0x5a, arg);
            func_ov021_021a2108(task, WEATHER_SOUND_TYPE, func_02014a10(4));
        }
    } else if (func_ov021_021a23b0(task) && fading) {
        func_ov021_021a2160(task, 1, 0);
        return TRUE;
    }
    func_ov025_021eeee0(task, work);
    return FALSE;
}

// pokebw2's Blizzard_InitNoFade
static BOOL func_ov025_021eebe4(WeatherTask *task, u32 arg, HeapID heapId) {
    BlizzardWork *work = func_ov021_021a21b8(task);

    func_ov021_021a21f8(task, 1, 2, 1, 2, -2, 2, 1, func_ov025_021eee04);
    func_ov021_021a22e0(task, 4, 0x7b18, arg);
    work->cycle = 0;
    work->scrollX = 0;
    work->scrollY = 0;
    func_ov021_021a2254(task, func_ov025_021eee04, 20, 2, 2, heapId);
    func_ov021_021a2160(task, 1, 0);
    func_ov021_021a2114(task, WEATHER_SOUND_TYPE, func_02014a10(4));
    return TRUE;
}

// pokebw2's Blizzard_Main
static BOOL func_ov025_021eec64(WeatherTask *task, u32 arg, HeapID heapId) {
    BlizzardWork *work = func_ov021_021a21b8(task);

    if (func_ov021_021a2248(task, heapId)) {
        // Never true, since data_ov025_021ef770 holds no negative speed
        if (data_ov025_021ef770[work->cycle / 256] <= -8) {
            func_ov025_021eee04(task, 1, heapId);
        }
    }
    func_ov025_021eeee0(task, work);
    return FALSE;
}

// pokebw2's Blizzard_InitFadeOut
static BOOL func_ov025_021eecb0(WeatherTask *task, u32 arg, HeapID heapId) {
    BlizzardWork *work = func_ov021_021a21b8(task);

    func_ov021_021a2228(task, 0, 3, 1, -1);
    work->timer = 10;
    func_ov025_021eeee0(task, work);
    func_ov021_021a2160(task, 0, 0);
    func_ov021_021a25cc(task);
    return TRUE;
}

// pokebw2's Blizzard_FadeOut
static BOOL func_ov025_021eecec(WeatherTask *task, u32 arg, HeapID heapId) {
    BlizzardWork *work = func_ov021_021a21b8(task);
    BOOL fading = func_ov021_021a223c(task, heapId);

    if (work->timer > 0) {
        if (--work->timer == 0) {
            func_ov021_021a2390(task, 0x7fff, 0x3c, arg);
        }
    } else if ((arg ? func_ov021_021a23b0(task) : TRUE) && fading) {
        // Weather 1's code, where this is the flag that makes the flakes leave, but here it is the background's scroll
        work->scrollX = TRUE;
        if (!func_ov021_021a21bc(task)) {
            return TRUE;
        }
    }
    func_ov025_021eeee0(task, work);
    return FALSE;
}

// pokebw2's Blizzard_Exit
static BOOL func_ov025_021eed58(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a21b8(task);
    func_ov021_021a231c(task, arg);
    func_ov021_021a2120(task, arg);
    return TRUE;
}

// pokebw2's BlizzardFlake_Move
static void func_ov025_021eed78(WeatherObj *obj) {
    BlizzardFlake *flake = func_ov021_021a2630(obj);
    ClActor *actor = func_ov021_021a2638(obj);
    ClActorPos pos;

    func_0204b454(actor, &pos, 0);
    switch (flake->state) {
    case 0:
        pos.x += (s16)flake->speedX;
        pos.y += (s16)flake->speedY;
        if (flake->frame++ > flake->lifetime) {
            flake->state = 1;
        }
        if (flake->frame % 4 == 0) {
            flake->speedX += flake->accelX;
            if (flake->speedY > 1) {
                flake->speedY--;
            }
        }
        func_0204b404(actor, &pos, 0);
        break;
    case 1:
        func_ov021_021a21e0(obj);
        break;
    }
}

// pokebw2's Blizzard_AddFlakes
static void func_ov025_021eee04(WeatherTask *task, int count, HeapID heapId) {
    BlizzardWork *work = func_ov021_021a21b8(task);
    int index;
    int i;

    if (++work->cycle >= 1024) {
        work->cycle = 0;
    }
    index = work->cycle / 256;
    for (i = 0; i < count; i++) {
        WeatherObj *obj = func_ov021_021a21c8(task, heapId);
        BlizzardFlake *flake;
        ClActor *actor;
        int size;
        ClActorPos pos;

        if (obj == NULL) {
            break;
        }
        flake = func_ov021_021a2630(obj);
        actor = func_ov021_021a2638(obj);
        flake->frame = 0;
        flake->lifetime = GFL_RandomLC(3) + 9;
        size = GFL_RandomLC(4);
        func_0204b908(actor, size);
        flake->speedX = (size + 1) * data_ov025_021ef770[index];
        flake->speedY = (size + 1) * data_ov025_021ef780[index];
        flake->state = 0;
        if (size == 3) {
            flake->speedX += data_ov025_021ef770[index];
            flake->speedY += data_ov025_021ef780[index];
        }
        flake->accelX = data_ov025_021ef770[index];
        pos.x = -GFL_RandomLC(24);
        pos.y = GFL_RandomLC(168) - 0x20;
        func_0204b404(actor, &pos, 0);
    }
}

// pokebw2's Blizzard_ScrollBg
static void func_ov025_021eeee0(WeatherTask *task, BlizzardWork *work) {
    int x;
    int y;
    int scrollStep;
    int step;

    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    scrollStep = work->scrollStep;
    step = scrollStep / 28;
    scrollStep = (scrollStep + 1) % (NELEMS(data_ov025_021ef765) * 28);
    work->scrollStep = scrollStep;
    work->scrollX = work->scrollX + data_ov025_021ef765[step] - x / 2;
    work->scrollY = work->scrollY + data_ov025_021ef75c[step] - y / 2;
    if (work->scrollX < 0) {
        work->scrollX += 256;
    }
    if (work->scrollY < 0) {
        work->scrollY += 256;
    }
    work->scrollX %= 256;
    work->scrollY %= 256;
    func_ov021_021a2190(task, -work->scrollX, 0);
    func_ov021_021a21a0(task, -work->scrollY, 0);
}

// pokebw2's Flutter_Init
static BOOL func_ov025_021eef94(WeatherTask *task, u32 arg, HeapID heapId) {
    FlutterWork *work = func_ov021_021a21b8(task);

    func_ov021_021a21f8(task, 0, 8, 1, 1, -4, 2, 1, func_ov025_021ef2d4);
    func_ov021_021a22e0(task, 9, FLAKE_START_COLOR, arg);
    work->timer = 1;
    func_ov021_021a2424(task);
    func_ov021_021a25bc(task, 0x6e4);
    return TRUE;
}

// pokebw2's Flutter_FadeIn
static BOOL func_ov025_021eeff0(WeatherTask *task, u32 arg, HeapID heapId) {
    FlutterWork *work = func_ov021_021a21b8(task);
    BOOL fading = func_ov021_021a223c(task, heapId);
    int x;
    int y;

    if (work->timer > 0) {
        if (--work->timer == 0) {
            func_ov021_021a232c(task, 9, 0x7f80, 0x3c, arg);
            func_ov021_021a2108(task, WEATHER_SOUND_TYPE, func_02014a10(5));
        }
    } else if (func_ov021_021a23b0(task) && fading) {
        return TRUE;
    }
    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    return FALSE;
}

// pokebw2's Flutter_InitNoFade
static BOOL func_ov025_021ef068(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a21b8(task);
    func_ov021_021a21f8(task, 1, 1, 1, 1, -4, 2, 1, func_ov025_021ef2d4);
    func_ov021_021a22e0(task, 9, 0x7f80, arg);
    func_ov021_021a2254(task, func_ov025_021ef2d4, 20, 10, 1, heapId);
    func_ov021_021a2114(task, WEATHER_SOUND_TYPE, func_02014a10(5));
    return TRUE;
}

// pokebw2's Flutter_Main
static BOOL func_ov025_021ef0d0(WeatherTask *task, u32 arg, HeapID heapId) {
    int x;
    int y;

    func_ov021_021a21b8(task);
    func_ov021_021a2248(task, heapId);
    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    return FALSE;
}

// pokebw2's Flutter_InitFadeOut
static BOOL func_ov025_021ef100(WeatherTask *task, u32 arg, HeapID heapId) {
    FlutterWork *work = func_ov021_021a21b8(task);

    func_ov021_021a2228(task, 0, 8, 4, -1);
    work->timer = 1;
    func_ov021_021a25cc(task);
    return TRUE;
}

// pokebw2's Flutter_FadeOut
static BOOL func_ov025_021ef12c(WeatherTask *task, u32 arg, HeapID heapId) {
    FlutterWork *work = func_ov021_021a21b8(task);
    BOOL fading = func_ov021_021a223c(task, heapId);
    int x;
    int y;

    if (work->timer > 0) {
        if (--work->timer == 0) {
            func_ov021_021a2390(task, FLAKE_START_COLOR, 0x3c, arg);
        }
    } else if ((arg ? func_ov021_021a23b0(task) : TRUE) && fading && !func_ov021_021a21bc(task)) {
        return TRUE;
    }
    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    return FALSE;
}

// pokebw2's Flutter_Exit
static BOOL func_ov025_021ef1a4(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a21b8(task);
    func_ov021_021a231c(task, arg);
    func_ov021_021a2120(task, arg);
    return TRUE;
}

// pokebw2's FlutterFlake_Move
static void func_ov025_021ef1c4(WeatherObj *obj) {
    FlutterFlake *flake = func_ov021_021a2630(obj);
    ClActor *actor = func_ov021_021a2638(obj);
    ClActorPos pos;

    func_ov021_021a2640(obj, &pos);
    switch (flake->phase) {
    case 0:
        if (++flake->frame > flake->lifetime) {
            flake->phase = 1;
            flake->frame = flake->seed % 4 * 2 + 8;
            flake->speedX = 2;
            flake->speedY = 2;
            func_0204b908(actor, func_0204b918(actor) + 3);
            return;
        }
        pos.x += (s16)flake->speedX;
        pos.y += (s16)flake->speedY;
        func_ov021_021a2654(obj, &pos);
        break;
    case 1:
        pos.x += (s16)flake->speedX;
        if (flake->frame % 2 == 0) {
            pos.y += (s16)flake->speedY;
        } else {
            pos.y -= (s16)flake->speedY;
        }
        if (flake->frame % 4 == 0) {
            if (flake->speedY - 1 >= 0) {
                flake->speedY--;
            }
            if (flake->speedX - 1 >= 0) {
                flake->speedX--;
            }
        }
        func_ov021_021a26c4(obj, &pos);
        if (flake->frame-- <= 0) {
            flake->phase = 2;
        }
        break;
    case 2:
        func_ov021_021a21e0(obj);
        break;
    }
}

// pokebw2's Flutter_AddFlakes
static void func_ov025_021ef2d4(WeatherTask *task, int count, HeapID heapId) {
    int i;

    func_ov021_021a21b8(task);
    for (i = 0; i < count; i++) {
        WeatherObj *obj = func_ov021_021a21c8(task, heapId);
        FlutterFlake *flake;
        ClActor *actor;
        u32 seed;
        int kind;
        int m;
        ClActorPos pos;

        if (obj == NULL) {
            break;
        }
        flake = func_ov021_021a2630(obj);
        actor = func_ov021_021a2638(obj);
        seed = GFL_RandomLC(0);
        flake->frame = 0;
        if (seed % 100 < 30) {
            kind = 0;
        } else if (seed % 100 < 95) {
            kind = 1;
        } else if (seed % 100 < 100) {
            kind = 2;
        }
        func_0204b908(actor, 2 - kind);
        m = seed % 23;
        flake->speedY = m + (kind + 1) * 10;
        if (kind == 2) {
            flake->speedY += 3;
        }
        flake->speedX = m / 4 + (kind + 1) * 4;
        if (kind == 2) {
            flake->speedX -= 2;
        }
        flake->phase = 0;
        flake->lifetime = (seed & 3) + 1;
        pos.x = seed % 0x10e + (kind * 15 - 0x40);
        pos.y = (seed & 0xf) - 8;
        func_ov021_021a2654(obj, &pos);
        flake->seed = seed;
    }
}

// pokebw2's Speck_Init
static BOOL func_ov025_021ef3bc(WeatherTask *task, u32 arg, HeapID heapId) {
    SpeckWork *work = func_ov021_021a21b8(task);

    func_ov021_021a21f8(task, 2, 16, 20, 2, -2, 4, 2, func_ov025_021ef634);
    func_ov021_021a22e0(task, 8, 0x7fff, arg);
    work->timer = 0;
    return TRUE;
}

// pokebw2's Speck_FadeIn
static BOOL func_ov025_021ef404(WeatherTask *task, u32 arg, HeapID heapId) {
    SpeckWork *work = func_ov021_021a21b8(task);
    BOOL fading = func_ov021_021a223c(task, heapId);
    int x;
    int y;

    if (work->timer == 0) {
        work->timer--;
        func_ov021_021a232c(task, 8, 0x7f57, 0x5a, arg);
        func_ov021_021a2108(task, WEATHER_SOUND_TYPE, func_02014a10(8));
    } else if (func_ov021_021a23b0(task) && fading) {
        return TRUE;
    }
    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    return FALSE;
}

// pokebw2's Speck_InitNoFade
static BOOL func_ov025_021ef47c(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a21b8(task);
    func_ov021_021a21f8(task, 20, 2, 20, 2, -2, 4, 2, func_ov025_021ef634);
    func_ov021_021a22e0(task, 8, 0x7f57, arg);
    func_ov021_021a2254(task, func_ov025_021ef634, 20, 10, 1, heapId);
    func_ov021_021a2114(task, WEATHER_SOUND_TYPE, func_02014a10(8));
    return TRUE;
}

// pokebw2's Speck_Main
static BOOL func_ov025_021ef4e4(WeatherTask *task, u32 arg, HeapID heapId) {
    int x;
    int y;

    func_ov021_021a21b8(task);
    func_ov021_021a2248(task, heapId);
    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    return FALSE;
}

// pokebw2's Speck_InitFadeOut
static BOOL func_ov025_021ef514(WeatherTask *task, u32 arg, HeapID heapId) {
    SpeckWork *work = func_ov021_021a21b8(task);

    func_ov021_021a2228(task, 0, 16, 2, -2);
    work->timer = 100;
    return TRUE;
}

// pokebw2's Speck_FadeOut
static BOOL func_ov025_021ef538(WeatherTask *task, u32 arg, HeapID heapId) {
    SpeckWork *work = func_ov021_021a21b8(task);
    BOOL fading = func_ov021_021a223c(task, heapId);
    int x;
    int y;

    if (work->timer > 0) {
        if (--work->timer == 0) {
            func_ov021_021a2390(task, 0x7fff, 0x78, arg);
        }
    } else if ((arg ? func_ov021_021a23b0(task) : TRUE) && fading && !func_ov021_021a21bc(task)) {
        return TRUE;
    }
    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    return FALSE;
}

// pokebw2's Speck_Exit
static BOOL func_ov025_021ef5b0(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a21b8(task);
    func_ov021_021a231c(task, arg);
    func_ov021_021a2120(task, arg);
    return TRUE;
}

// pokebw2's Speck_Move
static void func_ov025_021ef5d0(WeatherObj *obj) {
    Speck *speck = func_ov021_021a2630(obj);
    ClActorPos pos;

    func_ov021_021a2638(obj);
    func_ov021_021a2640(obj, &pos);
    speck->frame++;
    if (speck->frame % speck->periodX == 0) {
        pos.x += (s16)speck->stepX;
    }
    if (speck->frame % speck->periodY == 0) {
        pos.y += (s16)speck->stepY;
    }
    // BUG: The new position is never set, so the specks stay where they appear
#ifdef BUGFIX
    func_ov021_021a2654(obj, &pos);
#endif
    if (speck->frame >= speck->lifetime) {
        func_ov021_021a21e0(obj);
    }
}

// pokebw2's Speck_Add
static void func_ov025_021ef634(WeatherTask *task, int count, HeapID heapId) {
    int i;

    func_ov021_021a21b8(task);
    for (i = 0; i < count; i++) {
        WeatherObj *obj = func_ov021_021a21c8(task, heapId);
        int a;
        int b;
        Speck *speck;
        ClActor *actor;
        int frame;
        int low;
        int high;
        int range;
        ClActorPos pos;

        if (obj == NULL) {
            break;
        }
        speck = func_ov021_021a2630(obj);
        actor = func_ov021_021a2638(obj);
        a = GFL_RandomLC(0);
        b = GFL_RandomLC(0);
        speck->frame = 0;
        speck->lifetime = a % 5 + 7;
        speck->stepX = a % 2 == 0 ? 1 : -1;
        speck->stepY = 1;
        speck->periodX = a % 6 + 3;
        speck->periodY = b % 5 + 4;
        frame = a % 20;
        pos.x = a % 0x180 - 0x40;
        pos.y = b % 256 - 8;
        func_ov021_021a2654(obj, &pos);
        low = 50 - pos.x / 3;
        range = 0xce - pos.x / 3;
        if (range < 0) {
            high = low - b % -range;
        } else {
            high = low + b % range;
        }
        if (low <= pos.y && high >= pos.y) {
            speck->lifetime *= 2;
        } else {
            frame = a % 4;
        }
        func_0204b908(actor, frame);
    }
}
