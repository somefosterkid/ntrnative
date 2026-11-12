// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov076/weather_rain.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/sound.h"
#include "field/field_weather.h"
#include "gfl/clact.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
void func_ov021_021a2390(WeatherTask *task, u32 a1, u32 a2, u32 a3);
void func_ov021_021a2424(WeatherTask *task);
void func_ov021_021a244c(WeatherTask *task, int *x, int *y);
void func_ov021_021a2564(WeatherTask *task, int x, int y);
void func_ov021_021a25cc(WeatherTask *task);
void func_ov021_021a2640(WeatherObj *obj, ClActorPos *pos);

// The rain weathers. The ROM doesn't name the file; the name is a guess. Weather 2 is plain rain: drops that fall
// at a slant and splash. Weather 6 is a thunderstorm: faster drops over a scrolling background, with lightning
// that flashes the screen and rumbles. Weather 7 is rain that gusts: a lull, then a gust that sweeps three kinds of
// streak across two scrolling backgrounds. Overlay 36's weather table (WEATHER_OVERLAYS) loads them

#define WEATHER_SOUND_TYPE 0x3e
#define RAIN_START_COLOR 0x7fdf
#define RAIN_FADE_IN_FRAMES 0x5a
#define RAIN_FADE_OUT_FRAMES 0x32

// Weather 2's work
// Black 2's; twinned code confirms the offsets of timer in Black
typedef struct {
    // 1 when the weather starts
    int timer;
    // The weather asks for 0x28 bytes of work but uses only these
    int unused[9];
} RainWork;

// One of weather 2's drops
typedef struct {
    int frame;
    // The frame (in steps of 2) after which the drop splashes
    int splashFrame;
    int speedY;
    // 0 falling, 1 splashing, 2 gone
    int state;
    int speedX;
    // Random: drops with 7 or more splash on the ground
    int splash;
} RainDrop;
WORK_AREA_CHECK(RainDrop, WEATHER_OBJ_WORK_SIZE);

// One of lightning's flashes
typedef struct {
    u16 flashSpeed;
    u16 flashFrames;
    // The flash's shade
    u16 shade;
    // Frames until the next flash
    s16 delay;
    s16 soundDelay;
    u16 sound;
} LightningFlash;

// The lightning of weather 6
typedef struct {
    u8 state;
    // 0, 1 or 2: how the lightning flickers
    u8 kind;
    // The shape of the strike that was chosen
    u16 type;
    u16 count;
    u16 index;
    LightningFlash flashes[4];
    int timer;
    s16 soundDelay;
    s16 sound;
} Lightning;

// Weather 6's work
typedef struct {
    s16 state;
    s16 timer;
    int scroll;
    int frame;
    Lightning lightning;
} ThunderWork;

// One of weather 6's drops
// Black 2's; twinned code confirms the offsets of speedY, state and speedX in Black
typedef struct {
    int unused00;
    int unused04;
    int speedY;
    // 0 falling, 1 gone
    int state;
    int speedX;
} ThunderDrop;
WORK_AREA_CHECK(ThunderDrop, WEATHER_OBJ_WORK_SIZE);

// The speeds of the gusts of weather 7, 10 times too big
// Black 2's, not checked against Black
typedef struct {
    int x;
    int y;
    int unused;
} GustWind;

// Weather 7's work
// Black 2's; twinned code confirms the offsets of scrollX1, scrollY1, scrollX2, scrollY2, counter, limit, mode,
// rumbleCounter, rumbleLimit, windX, windY and bgScale in Black
typedef struct {
    s16 state;
    s16 timer;
    // The two backgrounds' scroll
    s16 scrollX1;
    s16 scrollY1;
    s16 scrollX2;
    s16 scrollY2;
    // Frames into the mode and where it ends
    s16 counter;
    s16 limit;
    // 0 off, 1 lull, 2 gust, 3 the gust begins, 4 the gust ends
    s16 mode;
    // Frames into the next rumble, and where it comes
    s16 rumbleCounter;
    s16 rumbleLimit;
    // The speed that the particles blow at
    s16 windX;
    s16 windY;
    int bgScale;
} GustWork;

typedef struct {
    s16 x;
    s16 y;
    s16 speedX;
    s16 speedY;
    s16 baseX;
    s16 baseY;
} GustParticleData;

// One of weather 7's streaks
typedef struct {
    int kind;
    GustWork *work;
    GustParticleData data;
} GustParticle;
WORK_AREA_CHECK(GustParticle, WEATHER_OBJ_WORK_SIZE);

// Both kinds of function get the particle's actor, and the move functions ignore it
typedef void (*GustParticleFunc)(WeatherObj *obj, const GustWork *work, GustParticleData *data, ClActor *actor);

static BOOL func_ov026_021ee740(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021ee790(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021ee814(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021ee888(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021ee8b8(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021ee8e4(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021ee95c(WeatherTask *task, u32 arg, HeapID heapId);
static void func_ov026_021ee97c(WeatherObj *obj);
static void func_ov026_021eea18(WeatherTask *task, int count, HeapID heapId);
static BOOL func_ov026_021eeaec(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021eeb44(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021eec5c(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021eecd8(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021eed10(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021eed44(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021eedb0(WeatherTask *task, u32 arg, HeapID heapId);
static void func_ov026_021eedd8(WeatherObj *obj);
static void func_ov026_021eee38(WeatherTask *task, int count, HeapID heapId);
static void func_ov026_021eef0c(WeatherTask *task, ThunderWork *work);
static BOOL func_ov026_021eef58(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021eefa8(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021ef078(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021ef104(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021ef134(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021ef180(WeatherTask *task, u32 arg, HeapID heapId);
static BOOL func_ov026_021ef1d4(WeatherTask *task, u32 arg, HeapID heapId);
static void func_ov026_021ef1f4(WeatherObj *obj);
static void func_ov026_021ef220(WeatherTask *task, int count, HeapID heapId);
static void func_ov026_021ef2d8(WeatherObj *obj, const GustWork *work, GustParticleData *data, ClActor *actor);
static void func_ov026_021ef364(WeatherObj *obj, const GustWork *work, GustParticleData *data, ClActor *actor);
static void func_ov026_021ef3e0(WeatherObj *obj, const GustWork *work, GustParticleData *data, ClActor *actor);
static void func_ov026_021ef454(WeatherObj *obj, const GustWork *work, GustParticleData *data, ClActor *actor);
static void func_ov026_021ef4d0(WeatherObj *obj, const GustWork *work, GustParticleData *data, ClActor *actor);
static void func_ov026_021ef58c(WeatherObj *obj, const GustWork *work, GustParticleData *data, ClActor *actor);
static void func_ov026_021ef614(WeatherTask *task, GustWork *work, u32 arg, BOOL noFade);
static void func_ov026_021ef664(WeatherTask *task, GustWork *work, u32 arg, BOOL noFade);
static void func_ov026_021ef830(WeatherTask *task, GustWork *work);
static void func_ov026_021ef920(WeatherTask *task, GustWork *work, u32 arg, u32 sound);
static void func_ov026_021ef93c(GustWork *work);
static void func_ov026_021ef950(WeatherTask *task, GustWork *work, u32 arg);
static void func_ov026_021ef9b0(Lightning *lightning, int kind);
static void func_ov026_021ef9cc(Lightning *lightning);
static void func_ov026_021ef9d8(Lightning *lightning, WeatherTask *task);
static void func_ov026_021efba4(Lightning *lightning);
static void func_ov026_021efbfc(Lightning *lightning);
static void func_ov026_021efc3c(Lightning *lightning);
static u16 func_ov026_021efcd4(Lightning *lightning);

// pokebw2's sThunderSpeedScale
static const int data_ov026_021efd08[5] = { 10, 10, 13, 10, 16 };

// pokebw2's sLightningFlashFrames
static const u8 data_ov026_021efd1c[32] = {
#include "asset/weather_rain_data_ov026_021efd1c.inc"
};

// pokebw2's sGustBgScale
static const int data_ov026_021efd3c[14] = {
#include "asset/weather_rain_data_ov026_021efd3c.inc"
};

// pokebw2's sGustWind
static const GustWind data_ov026_021efd74[6] = {
#include "asset/weather_rain_data_ov026_021efd74.inc"
};

// pokebw2's GustDriven_Init
// pokebw2's GustSlant_Init
// pokebw2's GustStreak_Init
// pokebw2's sGustParticleInit
static GustParticleFunc data_ov026_021efdc0[3] = { func_ov026_021ef364, func_ov026_021ef454, func_ov026_021ef58c };
// pokebw2's GustDriven_Move
// pokebw2's GustSlant_Move
// pokebw2's GustStreak_Move
// pokebw2's sGustParticleMove
static GustParticleFunc data_ov026_021efdcc[3] = { func_ov026_021ef2d8, func_ov026_021ef3e0, func_ov026_021ef4d0 };

// pokebw2's THUNDERSTORM_WEATHER
WeatherData data_ov026_021efe24 = {
    { 0x38, 1, 1, 0x11, 0xe, 0x10, 0xf, 0x29, 0x202, 3, 0x102 },
    sizeof(ThunderWork),
    {
        func_ov026_021eeaec,
        func_ov026_021eeb44,
        func_ov026_021eec5c,
        func_ov026_021eecd8,
        func_ov026_021eed10,
        func_ov026_021eed44,
        func_ov026_021eedb0,
    },
    func_ov026_021eedd8,
};

// pokebw2's GUSTY_RAIN_WEATHER
WeatherData data_ov026_021efe70 = {
    { 0x38, 1, 2, 6, 7, 5, 4, 0x28, 0x202, 3, 0x102, 0, 0, 0x27, 0x202, 3, 0x102 },
    sizeof(GustWork),
    {
        func_ov026_021eef58,
        func_ov026_021eefa8,
        func_ov026_021ef078,
        func_ov026_021ef104,
        func_ov026_021ef134,
        func_ov026_021ef180,
        func_ov026_021ef1d4,
    },
    func_ov026_021ef1f4,
};

// pokebw2's RAIN_WEATHER
WeatherData data_ov026_021efdd8 = {
    { 0x38, 1, 0, 0xd, 0xe, 0xc, 0xb },
    sizeof(RainWork),
    {
        func_ov026_021ee740,
        func_ov026_021ee790,
        func_ov026_021ee814,
        func_ov026_021ee888,
        func_ov026_021ee8b8,
        func_ov026_021ee8e4,
        func_ov026_021ee95c,
    },
    func_ov026_021ee97c,
};

// pokebw2's Rain_Init
static BOOL func_ov026_021ee740(WeatherTask *task, u32 arg, HeapID heapId) {
    RainWork *work = func_ov021_021a21b8(task);

    func_ov021_021a21f8(task, 1, 8, 2, 0, -4, 2, 1, func_ov026_021eea18);
    func_ov021_021a22e0(task, 8, RAIN_START_COLOR, arg);
    work->timer = 1;
    func_ov021_021a2424(task);
    return TRUE;
}

// pokebw2's Rain_FadeIn
static BOOL func_ov026_021ee790(WeatherTask *task, u32 arg, HeapID heapId) {
    RainWork *work = func_ov021_021a21b8(task);
    BOOL done = func_ov021_021a223c(task, heapId);
    int x;
    int y;

    if (work->timer > 0) {
        if (--work->timer == 0) {
            func_ov021_021a232c(task, 8, 0x7f5f, RAIN_FADE_IN_FRAMES, arg);
            func_ov021_021a2108(task, WEATHER_SOUND_TYPE, func_02014a10(2));
            func_ov021_021a25bc(task, SEQ_SE_FLD_32);
        }
    } else if (func_ov021_021a23b0(task) && done) {
        return TRUE;
    }
    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    return FALSE;
}

// pokebw2's Rain_InitNoFade
static BOOL func_ov026_021ee814(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a21b8(task);
    func_ov021_021a21f8(task, 2, 0, 2, 0, -4, 2, 1, func_ov026_021eea18);
    func_ov021_021a22e0(task, 8, 0x7f5f, arg);
    func_ov021_021a2254(task, func_ov026_021eea18, 20, 10, 1, heapId);
    func_ov021_021a25bc(task, SEQ_SE_FLD_32);
    func_ov021_021a2114(task, WEATHER_SOUND_TYPE, func_02014a10(2));
    return TRUE;
}

// pokebw2's Rain_Main
static BOOL func_ov026_021ee888(WeatherTask *task, u32 arg, HeapID heapId) {
    int x;
    int y;

    func_ov021_021a21b8(task);
    func_ov021_021a2248(task, heapId);
    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    return FALSE;
}

// pokebw2's Rain_InitFadeOut
static BOOL func_ov026_021ee8b8(WeatherTask *task, u32 arg, HeapID heapId) {
    RainWork *work = func_ov021_021a21b8(task);

    func_ov021_021a2228(task, 0, 8, 4, -1);
    work->timer = 1;
    func_ov021_021a25cc(task);
    return TRUE;
}

// pokebw2's Rain_FadeOut
static BOOL func_ov026_021ee8e4(WeatherTask *task, u32 arg, HeapID heapId) {
    RainWork *work = func_ov021_021a21b8(task);
    BOOL done = func_ov021_021a223c(task, heapId);
    int x;
    int y;

    if (work->timer > 0) {
        if (--work->timer == 0) {
            func_ov021_021a2390(task, RAIN_START_COLOR, RAIN_FADE_OUT_FRAMES, arg);
        }
    } else if ((arg ? func_ov021_021a23b0(task) : TRUE) && done && !func_ov021_021a21bc(task)) {
        return TRUE;
    }
    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    return FALSE;
}

// pokebw2's Rain_Exit
static BOOL func_ov026_021ee95c(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a21b8(task);
    func_ov021_021a231c(task, arg);
    func_ov021_021a2120(task, arg);
    return TRUE;
}

// pokebw2's RainDrop_Move
static void func_ov026_021ee97c(WeatherObj *obj) {
    RainDrop *drop = func_ov021_021a2630(obj);
    ClActor *actor = func_ov021_021a2638(obj);
    ClActorPos pos;

    func_ov021_021a2640(obj, &pos);
    switch (drop->state) {
    case 0:
        pos.x += (s16)drop->speedX;
        pos.y += (s16)drop->speedY;
        drop->frame += 2;
        if (drop->frame > drop->splashFrame) {
            if (drop->splash < 7) {
                drop->state = 2;
            } else {
                drop->state = 1;
                drop->frame = 4;
                func_0204b908(actor, 3);
            }
        }
        func_ov021_021a26c4(obj, &pos);
        break;
    case 1:
        if (drop->frame-- <= 0) {
            drop->state = 2;
        }
        break;
    case 2:
        func_ov021_021a21e0(obj);
        break;
    }
}

// pokebw2's Rain_AddDrops
static void func_ov026_021eea18(WeatherTask *task, int count, HeapID heapId) {
    int i;

    func_ov021_021a21b8(task);
    for (i = 0; i < count; i++) {
        WeatherObj *obj;
        RainDrop *drop;
        ClActor *actor;
        u32 random;
        u32 size;
        int slant;
        ClActorPos pos;

        obj = func_ov021_021a21c8(task, heapId);
        if (obj == NULL) {
            return;
        }
        drop = func_ov021_021a2630(obj);
        actor = func_ov021_021a2638(obj);
        random = GFL_RandomLC(0);
        drop->frame = 0;
        size = random % 3;
        func_0204b908(actor, size);
        slant = random % 20;
        drop->speedY = slant + (size + 1) * 16;
        if (size == 2) {
            drop->speedY += 16;
        }
        drop->speedX = slant / 8 + (size + 1) * 8;
        if (size == 2) {
            drop->speedX += 8;
        }
        drop->state = 0;
        drop->splashFrame = (random & 1) + 1;
        pos.x = (u8)random + (size * 15 - 0x48);
        pos.y = -0x60;
        func_ov021_021a26c4(obj, &pos);
        drop->speedX *= 2;
        drop->speedY *= 2;
        drop->splash = GFL_RandomLC(10);
    }
}

// pokebw2's Thunder_Init
static BOOL func_ov026_021eeaec(WeatherTask *task, u32 arg, HeapID heapId) {
    ThunderWork *work = func_ov021_021a21b8(task);

    func_ov026_021ef9b0(&work->lightning, 2);
    func_ov021_021a21f8(task, 1, 15, 2, 0, -3, 1, 1, func_ov026_021eee38);
    func_ov021_021a22e0(task, 9, RAIN_START_COLOR, arg);
    work->timer = 0;
    work->state = 0;
    func_ov021_021a2424(task);
    return TRUE;
}

// pokebw2's Thunder_FadeIn
static BOOL func_ov026_021eeb44(WeatherTask *task, u32 arg, HeapID heapId) {
    ThunderWork *work = func_ov021_021a21b8(task);

    switch (work->state) {
    case 0:
        func_ov021_021a2108(task, WEATHER_SOUND_TYPE, func_02014a10(6));
        work->state++;
        work->timer = RAIN_FADE_IN_FRAMES;
        break;
    case 1:
        if (--work->timer < 0) {
            work->state++;
        }
        break;
    case 2:
        func_ov021_021a2154(task, 0x28c6, 8);
        work->timer = 16;
        work->state++;
        break;
    case 3:
        if (--work->timer < 0) {
            func_ov021_021a2130(task, 0x23de, 4, 60);
            func_ov021_021a25dc(task, SEQ_SE_FLD_33);
            work->state++;
        }
        break;
    case 4:
        if (!func_ov021_021a213c(task)) {
            func_ov021_021a232c(task, 9, 0x7f80, 0x50, arg);
            work->state++;
            work->timer = 0x30;
        }
        break;
    case 5: {
        BOOL done;
        BOOL flashed;

        if (--work->timer == 0) {
            func_ov021_021a25bc(task, SEQ_SE_FLD_31);
            func_ov021_021a2160(task, 1, 0);
        }
        done = func_ov021_021a223c(task, heapId);
        flashed = func_ov021_021a23b0(task);
        if (done && flashed) {
            return TRUE;
        }
        break;
    }
    }
    func_ov026_021eef0c(task, work);
    return FALSE;
}

// pokebw2's Thunder_InitNoFade
static BOOL func_ov026_021eec5c(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a21b8(task);
    func_ov021_021a21f8(task, 2, 0, 2, 0, -3, 1, 1, func_ov026_021eee38);
    func_ov021_021a22e0(task, 9, 0x7f80, arg);
    func_ov021_021a2254(task, func_ov026_021eee38, 20, 5, 1, heapId);
    func_ov021_021a25bc(task, SEQ_SE_FLD_31);
    func_ov021_021a2114(task, WEATHER_SOUND_TYPE, func_02014a10(6));
    func_ov021_021a2160(task, 1, 0);
    return TRUE;
}

// pokebw2's Thunder_Main
static BOOL func_ov026_021eecd8(WeatherTask *task, u32 arg, HeapID heapId) {
    ThunderWork *work = func_ov021_021a21b8(task);

    work->frame = (work->frame + 1) % 300;
    func_ov021_021a2248(task, heapId);
    func_ov026_021ef9d8(&work->lightning, task);
    func_ov026_021eef0c(task, work);
    return FALSE;
}

// pokebw2's Thunder_InitFadeOut
static BOOL func_ov026_021eed10(WeatherTask *task, u32 arg, HeapID heapId) {
    ThunderWork *work = func_ov021_021a21b8(task);

    func_ov021_021a2228(task, 0, 15, 3, -1);
    work->timer = 1;
    func_ov021_021a25cc(task);
    func_ov021_021a2160(task, 0, 0);
    return TRUE;
}

// pokebw2's Thunder_FadeOut
static BOOL func_ov026_021eed44(WeatherTask *task, u32 arg, HeapID heapId) {
    ThunderWork *work = func_ov021_021a21b8(task);
    BOOL done = func_ov021_021a223c(task, heapId);

    if (work->timer > 0) {
        if (--work->timer == 0) {
            func_ov021_021a2390(task, RAIN_START_COLOR, RAIN_FADE_OUT_FRAMES, arg);
        }
    } else if ((arg ? func_ov021_021a23b0(task) : TRUE) && done && !func_ov021_021a21bc(task)) {
        return TRUE;
    }
    func_ov026_021eef0c(task, work);
    return FALSE;
}

// pokebw2's Thunder_Exit
static BOOL func_ov026_021eedb0(WeatherTask *task, u32 arg, HeapID heapId) {
    ThunderWork *work = func_ov021_021a21b8(task);

    func_ov021_021a231c(task, arg);
    func_ov021_021a2120(task, arg);
    func_ov026_021ef9cc(&work->lightning);
    return TRUE;
}

// pokebw2's ThunderDrop_Move
static void func_ov026_021eedd8(WeatherObj *obj) {
    ThunderDrop *drop = func_ov021_021a2630(obj);
    ClActorPos pos;

    func_ov021_021a2638(obj);
    func_ov021_021a2640(obj, &pos);
    switch (drop->state) {
    case 0:
        pos.x += (s16)drop->speedX;
        pos.y += (s16)drop->speedY;
        if (pos.y >= 192) {
            drop->state = 1;
        }
        func_ov021_021a26c4(obj, &pos);
        break;
    case 1:
        func_ov021_021a21e0(obj);
        break;
    }
}

// pokebw2's Thunder_AddDrops
static void func_ov026_021eee38(WeatherTask *task, int count, HeapID heapId) {
    int i;
    ThunderWork *work = func_ov021_021a21b8(task);

    for (i = 0; i < count; i++) {
        WeatherObj *obj;
        ThunderDrop *drop;
        ClActor *actor;
        u32 random;
        u32 size;
        ClActorPos pos;

        obj = func_ov021_021a21c8(task, heapId);
        if (obj == NULL) {
            return;
        }
        drop = func_ov021_021a2630(obj);
        actor = func_ov021_021a2638(obj);
        random = GFL_RandomLC(0);
        size = random % 3;
        func_0204b908(actor, size);
        drop->speedX = (random & 15) + (size + 1) * 14;
        drop->speedY = drop->speedX;
        drop->speedX = data_ov026_021efd08[work->frame / 60] * drop->speedX / 10;
        drop->speedY = data_ov026_021efd08[work->frame / 60] * drop->speedY / 10;
        drop->state = 0;
        pos.x = random % 0x180 - 0x80;
        pos.y = random % 40 - 0x50;
        func_ov021_021a26c4(obj, &pos);
    }
}

// pokebw2's Thunder_ScrollBg
static void func_ov026_021eef0c(WeatherTask *task, ThunderWork *work) {
    int x;
    int y;

    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    work->scroll = (work->scroll + 9) % 256;
    func_ov021_021a2190(task, -(work->scroll + x), 0);
    func_ov021_021a21a0(task, y - work->scroll, 0);
}

// pokebw2's GustyRain_Init
static BOOL func_ov026_021eef58(WeatherTask *task, u32 arg, HeapID heapId) {
    GustWork *work = func_ov021_021a21b8(task);

    func_ov021_021a21f8(task, 1, 4, 1, 1, -4, 1, 1, func_ov026_021ef220);
    func_ov021_021a22e0(task, 9, RAIN_START_COLOR, arg);
    func_ov021_021a2424(task);
    func_ov026_021ef93c(work);
    return TRUE;
}

// pokebw2's GustyRain_FadeIn
static BOOL func_ov026_021eefa8(WeatherTask *task, u32 arg, HeapID heapId) {
    GustWork *work = func_ov021_021a21b8(task);
    BOOL done;
    BOOL flashed;

    switch (work->state) {
    case 0:
        func_ov021_021a2108(task, WEATHER_SOUND_TYPE, func_02014a10(7));
        work->timer = RAIN_FADE_IN_FRAMES;
        work->state++;
        break;
    case 1:
        if (--work->timer <= 0) {
            func_ov026_021ef614(task, work, arg, TRUE);
            work->limit = 0x78;
            work->timer = 8;
            work->state++;
        }
        break;
    case 2:
        if (--work->timer == 0) {
            func_ov021_021a25bc(task, SEQ_SE_FLD_31);
            func_ov021_021a2160(task, 1, 0);
            func_ov021_021a232c(task, 9, 0x7f80, 10, arg);
        }
        done = func_ov021_021a223c(task, heapId);
        flashed = func_ov021_021a23b0(task);
        if (done == TRUE && flashed == TRUE && work->timer <= 0) {
            return TRUE;
        }
        break;
    }
    func_ov026_021ef664(task, work, arg, FALSE);
    func_ov026_021ef830(task, work);
    return FALSE;
}

// pokebw2's GustyRain_InitNoFade
static BOOL func_ov026_021ef078(WeatherTask *task, u32 arg, HeapID heapId) {
    GustWork *work = func_ov021_021a21b8(task);

    func_ov021_021a21f8(task, 1, 1, 1, 1, -4, 1, 1, func_ov026_021ef220);
    func_ov021_021a22e0(task, 9, 0x7f80, arg);
    func_ov021_021a2254(task, func_ov026_021ef220, 20, 5, 1, heapId);
    func_ov021_021a25bc(task, SEQ_SE_FLD_31);
    func_ov026_021ef614(task, work, arg, FALSE);
    func_ov021_021a2160(task, 1, 0);
    func_ov021_021a2114(task, WEATHER_SOUND_TYPE, func_02014a10(7));
    return TRUE;
}

// pokebw2's GustyRain_Main
static BOOL func_ov026_021ef104(WeatherTask *task, u32 arg, HeapID heapId) {
    GustWork *work = func_ov021_021a21b8(task);

    func_ov021_021a2248(task, heapId);
    func_ov026_021ef664(task, work, arg, FALSE);
    func_ov026_021ef830(task, work);
    return FALSE;
}

// pokebw2's GustyRain_InitFadeOut
static BOOL func_ov026_021ef134(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a21b8(task);
    func_ov021_021a2228(task, 0, 4, 4, -1);
    func_ov021_021a25cc(task);
    func_ov021_021a2390(task, RAIN_START_COLOR, RAIN_FADE_OUT_FRAMES, arg);
    func_ov021_021a2160(task, 0, 0);
    func_ov021_021a2160(task, 0, 1);
    return TRUE;
}

// pokebw2's GustyRain_FadeOut
static BOOL func_ov026_021ef180(WeatherTask *task, u32 arg, HeapID heapId) {
    GustWork *work = func_ov021_021a21b8(task);
    BOOL done = func_ov021_021a223c(task, heapId);

    if ((arg ? func_ov021_021a23b0(task) : TRUE) && done && !func_ov021_021a21bc(task)) {
        return TRUE;
    }
    func_ov026_021ef664(task, work, arg, TRUE);
    func_ov026_021ef830(task, work);
    return FALSE;
}

// pokebw2's GustyRain_Exit
static BOOL func_ov026_021ef1d4(WeatherTask *task, u32 arg, HeapID heapId) {
    func_ov021_021a21b8(task);
    func_ov021_021a231c(task, arg);
    func_ov021_021a2120(task, arg);
    return TRUE;
}

// pokebw2's GustParticle_Move
static void func_ov026_021ef1f4(WeatherObj *obj) {
    GustParticle *particle = func_ov021_021a2630(obj);
    ClActor *actor = func_ov021_021a2638(obj);

    data_ov026_021efdcc[particle->kind](obj, particle->work, &particle->data, actor);
}

// pokebw2's Gust_AddParticles
static void func_ov026_021ef220(WeatherTask *task, int count, HeapID heapId) {
    u32 random = GFL_RandomLC(0);
    GustWork *work = func_ov021_021a21b8(task);
    int i;

    if (work->mode == 0) {
        return;
    }
    if (work->mode == 2 && (random & 1)) {
        return;
    }
    for (i = 0; i < count; i++) {
        WeatherObj *obj;
        GustParticle *particle;
        ClActor *actor;

        obj = func_ov021_021a21c8(task, heapId);
        if (obj == NULL) {
            return;
        }
        particle = func_ov021_021a2630(obj);
        actor = func_ov021_021a2638(obj);
        particle->work = work;
        switch (work->mode) {
        case 1:
            particle->kind = 1;
            break;
        case 2:
            if (random % 100 <= 50) {
                particle->kind = 2;
            } else {
                particle->kind = 0;
            }
            break;
        case 3:
        case 4:
            if (random % 100 <= 80) {
                particle->kind = 1;
            } else {
                particle->kind = 2;
            }
            break;
        }
        data_ov026_021efdc0[particle->kind](obj, particle->work, &particle->data, actor);
    }
}

static void func_ov026_021ef2d8(WeatherObj *obj, const GustWork *work, GustParticleData *data, ClActor *actor) {
    s16 windX = work->windX;
    s16 windY;
    ClActorPos pos;

    if (windX == 0) {
        windX = data->baseX;
        windY = data->baseY;
    } else {
        windY = work->windY;
        data->baseX = windX;
        data->baseY = windY;
    }
    func_ov021_021a2640(obj, &pos);
    data->x += data->speedX + windX;
    data->y += data->speedY + windY;
    pos.x = data->x / 8;
    pos.y = data->y / 8;
    if (pos.x >= 256 || pos.y >= 192) {
        func_ov021_021a21e0(obj);
        return;
    }
    func_ov021_021a26c4(obj, &pos);
}

static void func_ov026_021ef364(WeatherObj *obj, const GustWork *work, GustParticleData *data, ClActor *actor) {
    u32 random = GFL_RandomLC(0);
    u32 r = random % 24;
    ClActorPos pos;
    s16 windX;

    data->speedX = r + 16;
    windX = work->windX;
    data->speedY = r + 8;
    data->baseX = windX;
    data->baseY = work->windY;
    pos.x = (random & 7) - 8;
    pos.y = random % 160 - 16;
    func_ov021_021a26c4(obj, &pos);
    data->x = pos.x << 3;
    data->y = pos.y << 3;
    func_0204b878(actor, 0);
    func_0204b92c(actor, 1);
    func_0204b954(actor, 1 << 13);
}

static void func_ov026_021ef3e0(WeatherObj *obj, const GustWork *work, GustParticleData *data, ClActor *actor) {
    ClActorPos pos;

    func_ov021_021a2640(obj, &pos);
    data->x += data->speedX + work->windX;
    data->y += data->speedY + work->windY;
    pos.x = data->x / 8;
    pos.y = data->y / 8;
    if (pos.x >= 256 || pos.y >= 160) {
        func_ov021_021a21e0(obj);
        return;
    }
    func_ov021_021a26c4(obj, &pos);
}

static void func_ov026_021ef454(WeatherObj *obj, const GustWork *work, GustParticleData *data, ClActor *actor) {
    u32 random = GFL_RandomLC(0);
    u32 r = random % 43;
    ClActorPos pos;

    data->speedX = r + 86;
    data->speedY = r * 2 + 258;
    pos.x = random % 336 - 80;
    pos.y = random % 40 - 80;
    func_ov021_021a26c4(obj, &pos);
    data->x = pos.x << 3;
    data->y = pos.y << 3;
    func_0204b878(actor, 1);
    func_0204b92c(actor, 1);
    func_0204b954(actor, 1 << 13);
}

static void func_ov026_021ef4d0(WeatherObj *obj, const GustWork *work, GustParticleData *data, ClActor *actor) {
    ClActorPos pos;
    s16 windX;

    func_ov021_021a2640(obj, &pos);
    windX = work->windX;
    if (windX != 0) {
        data->x += data->speedX + windX;
        data->y += data->speedY + work->windY;
        pos.x = data->x / 8;
        pos.y = data->y / 8;
        data->baseX = data->speedX + windX;
        data->baseY = data->speedY + work->windY;
    } else {
        data->x += data->baseX;
        data->y += data->baseY;
        pos.x = data->x / 8;
        pos.y = data->y / 8;
    }
    if (pos.x >= 256 || pos.y >= 192) {
        func_ov021_021a21e0(obj);
        return;
    }
    func_ov021_021a26c4(obj, &pos);
}

static void func_ov026_021ef58c(WeatherObj *obj, const GustWork *work, GustParticleData *data, ClActor *actor) {
    u32 random = GFL_RandomLC(0);
    u32 r = random % 24;
    ClActorPos pos;

    data->speedX = r + 80;
    data->speedY = r + 14;
    data->baseX = data->speedX + work->windX;
    data->baseY = data->speedY + work->windY;
    pos.x = (random & 7) - 8;
    pos.y = random % 160 - 16;
    func_ov021_021a26c4(obj, &pos);
    data->x = pos.x << 3;
    data->y = pos.y << 3;
    func_0204b878(actor, 2);
    func_0204b92c(actor, 1);
    func_0204b954(actor, 1 << 13);
}

// pokebw2's Gust_StartLull
static void func_ov026_021ef614(WeatherTask *task, GustWork *work, u32 arg, BOOL noFade) {
    u32 random = GFL_RandomLC(0);

    work->mode = 1;
    work->limit = random % 400 + 60;
    work->counter = 0;
    if (!noFade) {
        func_ov021_021a232c(task, 9, 0x7f80, 10, arg);
        func_ov021_021a2160(task, 0, 1);
        func_ov021_021a25bc(task, SEQ_SE_FLD_31);
    }
}

// pokebw2's Gust_Update
void func_ov026_021ef664(WeatherTask *task, GustWork *work, u32 arg, BOOL noFade) {
    switch (work->mode) {
    case 1:
        work->counter++;
        func_ov026_021ef920(task, work, arg, SEQ_SE_FLD_99);
        if (work->counter >= work->limit) {
            work->mode = 3;
            work->limit = 16;
            work->counter = 0;
            if (!noFade) {
                func_ov021_021a232c(task, 9, 0x7f70, 10, arg);
                func_ov021_021a25bc(task, SEQ_SE_FLD_99 + 1);
            }
        }
        break;
    case 2:
        work->counter++;
        func_ov026_021ef920(task, work, arg, SEQ_SE_FLD_99);
        func_ov026_021ef950(task, work, arg);
        if (work->counter >= work->limit) {
            work->mode = 4;
            work->limit = 16;
            work->counter = 0;
            if (!noFade) {
                func_ov021_021a2160(task, 0, 1);
                func_ov021_021a2170(task, 0x1000, 0);
                func_ov021_021a2180(task, 0x1000, 0);
            }
        }
        break;
    case 3:
        work->counter++;
        if (work->counter >= work->limit) {
            u32 random = GFL_RandomLC(0);

            work->mode = 2;
            work->limit = random % 400 + 60;
            work->counter = 0;
            if (!noFade) {
                func_ov021_021a2160(task, 1, 1);
                func_ov021_021a2170(task, 0x800, 0);
                func_ov021_021a2180(task, 0x1800, 0);
            }
        }
        break;
    case 4:
        work->counter++;
        if (work->counter >= work->limit) {
            func_ov026_021ef614(task, work, arg, noFade);
        }
        break;
    }

    switch (work->mode) {
    case 1:
        if (work->limit - work->counter >= 9) {
            work->windX = 0;
            work->windY = 0;
        } else {
            work->windX = 0xa0;
            work->windY = 16;
        }
        break;
    case 2: {
        int index = work->counter % 72 * 6 / 72;

        work->windX = data_ov026_021efd74[index].x;
        work->windY = data_ov026_021efd74[index].y;
        work->bgScale = data_ov026_021efd3c[(u32)work->counter % 224 * 14 / 224];
        break;
    }
    case 3:
    case 4:
        work->windX = 0xa0;
        work->windY = 16;
        work->bgScale = 0x800;
        break;
    }
}

// pokebw2's Gust_ScrollBgs
static void func_ov026_021ef830(WeatherTask *task, GustWork *work) {
    int x;
    int y;

    func_ov021_021a244c(task, &x, &y);
    func_ov021_021a2564(task, x, y);
    if (work->windX != 0) {
        work->scrollX1 = (work->scrollX1 + 9) % 256;
        work->scrollY1 = (work->scrollY1 + 3) % 256;
        work->scrollX2 = (work->scrollX2 + 9) % 256;
        work->scrollY2 = (work->scrollY2 + 2) % 256;
    } else {
        work->scrollX1 = (work->scrollX1 + 3) % 256;
        work->scrollY1 = (work->scrollY1 + 8) % 256;
    }
    func_ov021_021a2190(task, -(work->scrollX1 + x), 0);
    func_ov021_021a21a0(task, y - work->scrollY1, 0);
    func_ov021_021a2190(task, -(work->scrollX2 + x), 1);
    func_ov021_021a21a0(task, y - work->scrollY2, 1);
    func_ov021_021a2170(task, work->bgScale, 1);
    func_ov021_021a2180(task, work->bgScale, 1);
}

// pokebw2's Gust_PlayGustSound
static void func_ov026_021ef920(WeatherTask *task, GustWork *work, u32 arg, u32 sound) {
    if (arg != 0 && work->counter == work->limit - 24) {
        func_ov021_021a25dc(task, sound);
    }
}

// pokebw2's Gust_ResetRumble
static void func_ov026_021ef93c(GustWork *work) {
    work->rumbleCounter = 0;
    work->rumbleLimit = GFL_RandomLC(16) + 50;
}

// pokebw2's Gust_PlayRumble
static void func_ov026_021ef950(WeatherTask *task, GustWork *work, u32 arg) {
    if (arg != 0 && work->counter <= work->limit - 74) {
        work->rumbleCounter++;
        if (work->rumbleCounter >= work->rumbleLimit) {
            if (!GFL_SndPlayerIsActive(GFL_SndSeqGetPlayerIndex(SEQ_SE_FLD_99))) {
                u32 delay = GFL_RandomLC(27) + 100;

                func_ov021_021a2604(task, SEQ_SE_FLD_98, delay);
                func_ov026_021ef93c(work);
            } else {
                work->rumbleCounter = work->rumbleLimit - 10;
            }
        }
    }
}

// pokebw2's Lightning_Init
static void func_ov026_021ef9b0(Lightning *lightning, int kind) {
    MI_CpuFill8(lightning, 0, sizeof(Lightning));
    lightning->timer = GFL_RandomLC(3);
    lightning->kind = kind;
}

// pokebw2's Lightning_Exit
static void func_ov026_021ef9cc(Lightning *lightning) {
    MI_CpuFill8(lightning, 0, sizeof(Lightning));
}

// pokebw2's Lightning_Update
static void func_ov026_021ef9d8(Lightning *lightning, WeatherTask *task) {
    // The three setups below read a column of each of these tables as constants, one entry per kind of strike (the
    // sound's delay, the shade's base and range, the delay's base and range), which leaves the tables in the ROM
    static const s16 sDelayBase[3] = { 0x78, 0xf0, 0xf0 };
    static const s16 sDelayRange[3] = { 0xf0, 300, 300 };
    static const u8 sShadeBase[3] = { 0x12, 0x18, 0x1e };
    static const u8 sSoundDelay[3] = { 0x1c, 0xd, 0xa };
    static const u8 sOdds[3] = { 30, 75, 100 };
    static const u8 sShadeRange[3] = { 3, 3, 1 };

    switch (lightning->state) {
    case 0:
        if (--lightning->timer < 0) {
            lightning->state = 1;
        }
        break;
    case 1: {
        u32 random = GFL_RandomLC(100);
        int odds;

        for (odds = 0; odds < 3; odds++) {
            if (sOdds[odds] >= random) {
                break;
            }
        }
        if (lightning->kind == 0) {
            func_ov026_021efba4(lightning);
        } else {
            switch (odds) {
            case 0:
                if (lightning->kind == 1) {
                    func_ov026_021efba4(lightning);
                } else {
                    func_ov026_021efbfc(lightning);
                }
                break;
            case 1:
                func_ov026_021efbfc(lightning);
                break;
            case 2:
                func_ov026_021efc3c(lightning);
                break;
            }
        }
        lightning->index = 0;
        lightning->state = 2;
        break;
    }
    case 2:
        if (lightning->kind != 2) {
            lightning->state = 4;
        } else {
            func_ov021_021a2154(task, 0x3129, 4);
            lightning->timer = 10;
            lightning->state = 3;
        }
        break;
    case 3:
        if (--lightning->timer <= 0) {
            lightning->state = 4;
        }
        break;
    case 4:
        if (lightning->kind != 0) {
            func_ov021_021a2130(task, func_ov026_021efcd4(lightning), lightning->flashes[lightning->index].flashSpeed,
                                lightning->flashes[lightning->index].flashFrames);
        }
        if (lightning->type == 2) {
            lightning->timer = GFL_RandomLC(lightning->flashes[lightning->index].flashFrames) +
                               lightning->flashes[lightning->index].flashSpeed / 2;
        }
        lightning->soundDelay = lightning->flashes[lightning->index].soundDelay;
        lightning->sound = lightning->flashes[lightning->index].sound;
        if (lightning->kind == 2) {
            if (lightning->index + 1 == lightning->count) {
                lightning->flashes[lightning->index].delay /= 3;
            }
        } else if (lightning->kind == 0) {
            lightning->flashes[lightning->index].delay *= 2;
        }
        lightning->state = 5;
        break;
    case 5:
        if (lightning->soundDelay > 0) {
            if (--lightning->soundDelay == 0) {
                func_ov021_021a25dc(task, lightning->sound);
            }
        }
        if (!func_ov021_021a213c(task)) {
            lightning->state = 6;
        }
        break;
    case 6:
        if (lightning->soundDelay > 0) {
            if (--lightning->soundDelay == 0) {
                func_ov021_021a25dc(task, lightning->sound);
            }
        }
        lightning->flashes[lightning->index].delay--;
        if (lightning->flashes[lightning->index].delay <= 0) {
            if (lightning->count > lightning->index + 1) {
                lightning->index++;
                lightning->state = 4;
            } else {
                lightning->state = 1;
            }
        }
        break;
    }
}

// pokebw2's Lightning_SetupFlickers
static void func_ov026_021efba4(Lightning *lightning) {
    int i;

    lightning->type = 0;
    lightning->count = GFL_RandomLC(2) + 1;
    for (i = 0; i < lightning->count; i++) {
        lightning->flashes[i].shade = GFL_RandomLC(3) + 0x12;
        lightning->flashes[i].flashSpeed = 4;
        lightning->flashes[i].flashFrames = data_ov026_021efd1c[lightning->flashes[i].shade];
        lightning->flashes[i].delay = GFL_RandomLC(0xf0) + 0x78;
        lightning->flashes[i].soundDelay = 0x1c;
        lightning->flashes[i].sound = SEQ_SE_FLD_34;
    }
}

// pokebw2's Lightning_SetupBolt
static void func_ov026_021efbfc(Lightning *lightning) {
    lightning->type = 1;
    lightning->count = 1;
    lightning->flashes[0].shade = GFL_RandomLC(3) + 0x18;
    lightning->flashes[0].flashSpeed = 4;
    lightning->flashes[0].flashFrames = data_ov026_021efd1c[lightning->flashes[0].shade];
    lightning->flashes[0].delay = GFL_RandomLC(300) + 0xf0;
    lightning->flashes[0].soundDelay = 0xd;
    lightning->flashes[0].sound = SEQ_SE_FLD_33;
}

// pokebw2's Lightning_SetupForked
static void func_ov026_021efc3c(Lightning *lightning) {
    int i;

    lightning->type = 2;
    lightning->count = GFL_RandomLC(2) + 1;
    lightning->count++;
    for (i = 0; i < lightning->count - 1; i++) {
        lightning->flashes[i].shade = GFL_RandomLC(3) + 0x12;
        lightning->flashes[i].flashSpeed = 4;
        lightning->flashes[i].flashFrames = 8;
        lightning->flashes[i].delay = GFL_RandomLC(4);
        lightning->flashes[i].soundDelay = 10;
        lightning->flashes[i].sound = SEQ_SE_FLD_34;
    }
    lightning->flashes[i].shade = GFL_RandomLC(1) + 0x1e;
    lightning->flashes[i].flashSpeed = 4;
    lightning->flashes[i].flashFrames = data_ov026_021efd1c[lightning->flashes[i].shade];
    lightning->flashes[i].delay = GFL_RandomLC(300) + 0xf0;
    lightning->flashes[i].soundDelay = 10;
    lightning->flashes[i].sound = SEQ_SE_FLD_33;
}

// pokebw2's Lightning_GetShade
static u16 func_ov026_021efcd4(Lightning *lightning) {
    u16 shade = lightning->flashes[lightning->index].shade;

    return shade | shade << 5 | 0x2000;
}
