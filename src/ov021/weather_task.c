// Black's code at ov021 0x021a1bf4-0x021a2d88. The name is the file name its asserts pass (tools/name_files.py).
// The functions from 0x021a2a68 on were a file of their own until their .rodata object at 0x021d88e0 turned out to sort
// before this file's table at 0x021d88e8: one translation unit.
#include "types.h"
#include "field/field_environment.h"
#include "field/field_fog.h"
#include "field/field_sound.h"
#include "field/field_weather.h"
#include "field/ov021_021a0448.h"
#include "field/field_3dbg.h"
#include "field/weather.h"
#include "field/weather_task.h"
#include "field/ov021_021a0138.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "struct_decls.h"
#include "gfl/g3d.h"
#include "field/field_camera.h"
#include "nitro/math.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"

typedef struct LocalTvtCallButton LocalTvtCallButton;

// The structs are pokebw2's (src/ov036/weather_task.c), at the offsets Black's code reads
// The weather's graphics, which the particles use, and the two background layers' textures
typedef struct WeatherRes {
    u16 loaded;
    u16 chars;
    u16 palette;
    u16 cells;
    u32 bgActive[2];
    ArcTool *arc;
} WeatherRes;

// The weather's sound effect: state 1 playing, 2 waiting to play; ambience: through the field's sound
typedef struct WeatherSound {
    u16 state;
    u16 se;
    u32 unk04;
    u32 ambience;
} WeatherSound;

// Calls func every interval frames, moving value and interval towards their targets
typedef struct WeatherTimer {
    WeatherTask *owner;
    s16 value;
    s16 counter;
    s16 interval;
    s16 stepCount;
    s16 valueTarget;
    s16 intervalTarget;
    s16 intervalStep;
    s16 stepsMax;
    s32 valueStep;
    WeatherSpawnFunc func;
} WeatherTimer;

#define WEATHER_OBJ_COUNT 32

// One of the weather's two tasks (0x784 bytes)
struct WeatherTask {
    ClActUnit *unit;
    FieldFog *fog;
    FieldLightWork *light;
    FieldSound *fieldSound;
    void *fogCtrl;
    FieldCamera *camera;
    Field3DBg **bgs; // the weather's two background layers (FieldWeather.unk_00)
    u16 season;
    const WeatherData *data;
    u8 mode : 4;
    u8 fade : 4;
    u8 unk25;
    u8 state;
    u8 loaded;
    WeatherRes res;
    void *work;
    WeatherSound sound;
    WeatherTimer timer;
    VecFx32 lookAt;
    WeatherObj objs[WEATHER_OBJ_COUNT];
    WeatherObj *objHead;
    int objCount;
    u8 fogPending;
    u8 fogDepthShift;
    u8 fogDuration;
    u32 fogColor;
};

WeatherObj *func_ov021_021a274c(WeatherTask *task);
void func_ov021_021a277c(WeatherTask *task, WeatherObj *obj);
BOOL func_ov021_021a2bc4(WeatherObj *obj);
void func_ov021_021a27b8(WeatherTask *task, WeatherObj *obj);
BOOL func_ov021_021a2b8c(WeatherTimer *timer, HeapID heapId);
BOOL func_ov021_021a0380(LocalTvtCallButton *button);
ClActor *func_ov021_021a2c28(WeatherObj *obj);
static const u8 data_ov021_021d88e8[16] = { 0, 1, 1, 1, 1, 2, 2, 1, 3, 4, 4, 5, 5, 0, 0, 0 };
void func_ov021_021a2978(WeatherRes *res, const WeatherData *data, HeapID heapId);
void func_ov021_021a29a8(WeatherRes *res, const WeatherData *data, HeapID heapId);
void func_ov021_021a29f4(WeatherRes *res);
void func_ov021_021a2994(WeatherRes *res);
void func_ov021_021a2a18(WeatherRes *res, s32 index, Field3DBg *bg, const void *config, HeapID heapId);
BOOL func_ov021_021a2724(WeatherTask *task, WeatherFunc func, HeapID heapId);
void func_ov021_021a27f8(WeatherTask *task);
void func_ov021_021a282c(WeatherTask *task, HeapID heapId);
void func_ov021_021a2860(WeatherTask *task, HeapID heapId);
void func_ov021_021a28cc(WeatherTask *task, HeapID heapId);
void func_ov021_021a2968(WeatherTask *task);
u8 func_ov021_021a2970(WeatherTask *task);
BOOL func_ov021_021a1d78(s32 param_1, s32 param_2);
void func_ov021_021a2050(WeatherTask *task, u32 fade, u32 mode);
void func_ov021_021a26d8(WeatherTask *task);
void func_ov021_021a284c(WeatherTask *task);
void func_ov021_021a28a0(WeatherTask *task);
void func_ov021_021a2924(WeatherTask *task);
BOOL func_ov021_021a2ab8(WeatherTimer *timer, HeapID heapId);
void func_ov021_021a2a68(WeatherRes *res, s32 index, Field3DBg *bg);
void func_ov021_021a2bd4(WeatherObj *obj, WeatherTask *task, const WeatherRes *res, ClActUnit *unit, HeapID heapId);
void func_ov021_021a2c2c(WeatherSound *sound, u32 ambience);
void func_ov021_021a2cc8(WeatherTask *task, u32 depthShift, u32 color, s32 duration);
void func_ov021_021a2c98(WeatherSound *sound, FieldSound *fieldSound);
void func_ov021_021a2c6c(WeatherSound *sound, FieldSound *fieldSound);
void func_ov021_021a2cec(WeatherTask *task);
void func_ov021_021a2d04(WeatherTask *task);
void func_ov021_021a2d30(const MtxFx44 *proj, fx32 numer, fx32 *out1, fx32 *out2);
void func_ov021_021a2c10(WeatherObj *obj);
void func_ov021_021a2bb8(WeatherObj *obj);
void func_ov021_021a2aac(WeatherTimer *timer, u16 a2, u16 a3, u16 a4, s32 a5);
void *func_ov021_021a2c24(WeatherObj *obj);
void func_ov021_021a2c30(WeatherSound *sound, FieldSound *fieldSound, u16 se);
u8 func_ov021_021a2cf8(WeatherTask *task);
void func_ov021_021a2a84(WeatherTimer *timer, WeatherTask *owner, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8,
                         s32 a9, WeatherSpawnFunc a10);

// The layer texture the weather setup copies, its alpha set to 0x1f
static Field3DBgRes data_ov021_021ee710;
u8 func_ov021_021a273c(WeatherTask *task);

u16 func_ov021_021a1bf4(void *weatherSystem) {
    return ((FieldWeather *)weatherSystem)->unk_0c;
}

void func_ov021_021a1bf8(FieldWeather *weather, u32 param_2) {
    weather->unk_28 = param_2;
    GFL_OvlLoad(param_2);
}

void func_ov021_021a1c04(FieldWeather *weather) {
    if (weather->unk_28 != -1) {
        GFL_OvlUnload(weather->unk_28);
        weather->unk_28 = -1;
    }
}

void func_ov021_021a1c1c(TCB *tcb, void *data) {
    FieldWeather *weather = data;
    s32 i;

    i = 0;
    do {
        func_ov021_021a1670(weather->unk_00[i]);
        i = i + 1;
    } while (i < 2);
}

BOOL func_ov021_021a1c34(FieldWeather *weather, u32 param_2) {
    if (weather->unk_0a != 0) {
        weather->unk_14 = param_2;
        return FALSE;
    }
    if (weather->unk_0c == param_2 && param_2 != 0) {
        return FALSE;
    }
    weather->unk_0e = param_2;
    weather->unk_10 = 1;
    weather->unk_08 = func_ov021_021a1d78(weather->unk_0c, weather->unk_0e);
    weather->unk_0a = 1;
    return TRUE;
}

void func_ov021_021a1c68(FieldWeather *weather, u32 param_2) {
    switch (weather->unk_0a) {
    case 0:
        break;
    case 1:
        func_ov021_021a2050(weather->unk_20[0], 1, 1);
        weather->unk_0a = 2;
        return;
    case 2:
        if (func_ov021_021a21b0(weather->unk_20[0]) == 0) {
            func_ov021_021a1c04(weather);
            weather->unk_0a = 3;
            return;
        }
        break;
    case 3:
        func_ov021_021a1bf8(weather, WEATHER_OVERLAYS[weather->unk_0e].overlayId);
        weather->unk_0a = 4;
        return;
    case 4:
        func_ov021_021a1fec(weather->unk_20[0], WEATHER_OVERLAYS[weather->unk_0e].data, 1, 1, 1, weather->unk_10, param_2);
        weather->unk_10 = 1;
        weather->unk_0c = weather->unk_0e;
        weather->unk_0e = 0xffff;
        weather->unk_0a = 0;
        break;
    }
}

void func_ov021_021a1d04(FieldWeather *weather, u32 param_2) {
    WeatherTask *swap;

    switch (weather->unk_0a) {
    case 0:
        break;
    case 1:
        func_ov021_021a1fec(weather->unk_20[1], WEATHER_OVERLAYS[weather->unk_0e].data, 1, 1, 2, weather->unk_10, param_2);
        func_ov021_021a2050(weather->unk_20[0], 1, 2);
        weather->unk_0a = 2;
        return;
    case 2:
        if (func_ov021_021a21b0(weather->unk_20[0]) == 0) {
            swap = weather->unk_20[0];
            weather->unk_20[0] = weather->unk_20[1];
            weather->unk_20[1] = swap;
            weather->unk_0c = weather->unk_0e;
            weather->unk_0e = 0xffff;
            weather->unk_10 = 1;
            weather->unk_0a = 0;
        }
        break;
    }
}




BOOL func_ov021_021a1d78(s32 param_1, s32 param_2) {
    BOOL result;

    result = FALSE;
    switch (param_1) {
        case 1:
            if ((u16)(param_2 + 0xfffc) <= 1) {
                result = TRUE;
            }
            break;
        case 4:
            if (param_2 == 1 || param_2 == 5) {
                result = TRUE;
            }
            break;
        case 5:
            if (param_2 == 4 || param_2 == 1) {
                result = TRUE;
            }
            break;
    }
    return result;
}


WeatherTask *func_ov021_021a1db4(ClActUnit *unit, FieldCamera *camera, void *light, FieldFog *fog, void *fogCtrl,
                                 Field3DBg **bgs, FieldSound *fieldSound, u16 season, HeapID heapId) {
    WeatherTask *task;

    task = GFL_HeapAllocate(heapId, sizeof(WeatherTask), 1, "weather_task.c", 0x17e);
    task->unit = unit;
    task->camera = camera;
    task->fog = fog;
    task->light = light;
    task->fogCtrl = fogCtrl;
    task->bgs = bgs;
    task->fieldSound = fieldSound;
    task->season = season;
    return task;
}

void func_ov021_021a1df8(WeatherTask *param_1) {
    func_ov021_021a20c8(param_1);
    GFL_HeapFree(param_1);
}

void func_ov021_021a1e08(WeatherTask *task, HeapID heapId) {
    if (func_ov021_021a2cf8(task) != 0) {
        if (FieldFog_IsAnimationRunning(task->fog) == 0) {
            func_ov021_021a2d04(task);
            func_ov021_021a2cec(task);
        }
    }
    switch (task->state) {
    case 0:
        break;
    case 1:
        func_ov021_021a282c(task, heapId);
        func_ov021_021a2860(task, heapId);
        func_ov021_021a28cc(task, heapId);
        func_ov021_021a2968(task);
        task->state = 5;
        return;
    case 2:
        func_ov021_021a282c(task, heapId);
        task->state = 3;
        return;
    case 3:
        func_ov021_021a2860(task, heapId);
        task->state = 4;
        return;
    case 4:
        func_ov021_021a28cc(task, heapId);
        task->state = 5;
        return;
    case 5:
        if (func_ov021_021a2724(task, task->data->funcs[0], heapId) != 0) {
            if (task->fade != 0) {
                task->state = 6;
            } else {
                task->state = 7;
            }
        }
        func_ov021_021a2c98(&task->sound, task->fieldSound);
        func_ov021_021a2968(task);
        return;
    case 6:
        if (func_ov021_021a2724(task, task->data->funcs[1], heapId) != 0) {
            task->state = 8;
        }
        func_ov021_021a27f8(task);
        func_ov021_021a2c98(&task->sound, task->fieldSound);
        return;
    case 7:
        if (func_ov021_021a2724(task, task->data->funcs[2], heapId) != 0) {
            task->state = 8;
        }
        func_ov021_021a2c98(&task->sound, task->fieldSound);
        return;
    case 8:
        func_ov021_021a2724(task, task->data->funcs[3], heapId);
        func_ov021_021a27f8(task);
        func_ov021_021a2c98(&task->sound, task->fieldSound);
        return;
    case 9:
        if (func_ov021_021a2724(task, task->data->funcs[4], heapId) != 0) {
            task->state = 10;
        }
        func_ov021_021a27f8(task);
        func_ov021_021a2c98(&task->sound, task->fieldSound);
        return;
    case 10:
        if (func_ov021_021a2724(task, task->data->funcs[5], heapId) != 0) {
            task->state = 11;
        }
        func_ov021_021a27f8(task);
        func_ov021_021a2c98(&task->sound, task->fieldSound);
        return;
    case 11:
        if (func_ov021_021a2970(task) != 0) {
            func_ov021_021a2724(task, task->data->funcs[6], heapId);
        }
        task->state = 12;
        return;
    case 12:
        func_ov021_021a2924(task);
        func_ov021_021a28a0(task);
        func_ov021_021a284c(task);
        func_ov021_021a26d8(task);
        task->state = 0;
        return;
    }
}


void func_ov021_021a1fec(WeatherTask *task, const WeatherData *data, u8 param_3, int param_4, int param_5, u32 param_6,
                         u32 param_7) {
    task->data = data;
    task->mode = param_5;
    task->fade = param_4;
    task->unk25 = param_3;
    if (task->unk25 == 0) {
        task->state = 1;
    } else if (task->unk25 == 1) {
        task->state = 2;
    }
    func_ov021_021a2c2c(&task->sound, param_6);
}

void func_ov021_021a2050(WeatherTask *task, u32 fade, u32 mode) {
    u32 result;
    u8 state;

    result = func_ov021_021a273c(task);
    state = task->state;
    if (state != 0 && result - 4 > 1) {
        task->fade = fade;
        task->mode = mode;
        if (state <= 5) {
            task->state = 0xb;
            return;
        }
        if (task->fade != 0) {
            task->state = 9;
            return;
        }
        task->state = 0xb;
    }
}



void func_ov021_021a20c8(WeatherTask *task) {
    if (task->state != 0) {
        if (task->state <= 0xb) {
            func_ov021_021a2724(task, task->data->funcs[6], 0);
        }
        if (task->state <= 0xc) {
            func_ov021_021a2924(task);
            func_ov021_021a28a0(task);
            func_ov021_021a284c(task);
        }
        func_ov021_021a26d8(task);
    }
}


void func_ov021_021a2108(WeatherTask *task, u32 a1, u32 a2) {
    FieldLight_ChangeData(task->light, a1, a2, 0x3c);
}

void func_ov021_021a2114(WeatherTask *task, u32 a1, u32 a2) {
    FieldLight_ChangeData(task->light, a1, a2, 1);
}

void func_ov021_021a2120(WeatherTask *task, u32 a1) {
    if (a1 == 1) {
        FieldLight_ChangeDataDefault(task->light);
    }
}

void func_ov021_021a2130(WeatherTask *task, u16 a1, u32 a2, u32 a3) {
    func_ov021_021a0618(task->light, a1, a2, a3);
}

BOOL func_ov021_021a213c(WeatherTask *task) {
    return func_ov021_021a065c(task->light);
}



u32 func_ov021_021a2148(WeatherTask *task) {
    return func_ov021_0218e188(task->camera);
}

void func_ov021_021a2154(WeatherTask *task, u16 a1, u8 a2) {
    func_ov021_021a063c(task->light, a1, a2);
}


void func_ov021_021a2160(WeatherTask *task, u32 a1, u32 index) {
    func_ov021_021a16d4(task->bgs[index], a1);
}

void func_ov021_021a2170(WeatherTask *task, int value, u32 index) {
    func_ov021_021a16e4(task->bgs[index], value);
}

void func_ov021_021a2180(WeatherTask *task, int value, u32 index) {
    func_ov021_021a16e8(task->bgs[index], value);
}

void func_ov021_021a2190(WeatherTask *task, int value, u32 index) {
    func_ov021_021a16dc(task->bgs[index], value);
}

void func_ov021_021a21a0(WeatherTask *task, int value, u32 index) {
    func_ov021_021a16e0(task->bgs[index], value);
}

u8 func_ov021_021a21b0(WeatherTask *task) {
    return func_ov021_021a273c(task);
}


void *func_ov021_021a21b8(WeatherTask *task) {
    return task->work;
}

BOOL func_ov021_021a21bc(WeatherTask *task) {
    return task->objCount;
}

WeatherObj *func_ov021_021a21c8(WeatherTask *task, HeapID heapId) {
    WeatherObj *obj;

    obj = func_ov021_021a274c(task);
    if (obj != NULL) {
        func_ov021_021a277c(task, obj);
    }
    return obj;
}

void func_ov021_021a21e0(WeatherObj *obj) {
    if (func_ov021_021a2bc4(obj) != 0) {
        func_ov021_021a27b8(obj->task, obj);
    }
}

void func_ov021_021a21f8(WeatherTask *task, u32 a1, u32 a2, u32 a3, int a4, int a5, int a6, int a7, WeatherSpawnFunc spawn) {
    func_ov021_021a2a84(&task->timer, task, a2, a1, a3, a4, a5, a6, a7, spawn);
}


void func_ov021_021a2228(WeatherTask *task, u8 a1, u8 a2, u8 a3, int a4) {
    func_ov021_021a2aac(&task->timer, a1, a2, a3, a4);
}

BOOL func_ov021_021a223c(WeatherTask *task, HeapID heapId) {
    return func_ov021_021a2ab8(&task->timer, heapId);
}



BOOL func_ov021_021a2248(WeatherTask *task, HeapID heapId) {
    BOOL BVar1;

    BVar1 = func_ov021_021a2b8c(&task->timer, heapId);
    return BVar1;
}

void func_ov021_021a2254(WeatherTask *task, WeatherSpawnFunc spawn, int count, int a3, int a4, HeapID heapId) {
    s32 i;
    WeatherObj *next;
    s32 k;
    WeatherObj *cur;
    s32 n;

    spawn(task, count, heapId);
    n = 0;
    cur = task->objHead;
    if (cur != NULL) {
        next = cur->next;
        for (i = 0; i < count; i++) {
            for (k = 0; k < n; k++) {
                task->data->objFunc(cur);
                if (func_ov021_021a2bc4(cur) == 0) {
                    break;
                }
            }
            cur = next;
            next = cur->next;
            if (i >= a3 && i % a3 == 0) {
                n += a4;
            }
            if (cur == task->objHead) {
                break;
            }
        }
    }
}


void func_ov021_021a22e0(WeatherTask *task, u32 a1, u32 a2, u32 a3) {
    if (a3 == 1) {
        FieldFog_SetAlphaMode(task->fog, 0);
        FieldFog_SetAlpha(task->fog, 0x1f);
        FieldFog_SetDepthShift(task->fog, a1);
        FieldFog_SetOffset(task->fog, a2);
        FieldFog_SetDefaultTable(task->fog);
        FieldFog_SetEnabled(task->fog, 1);
    }
}


void func_ov021_021a231c(WeatherTask *task, u32 a1) {
    if (a1 == 1) {
        FieldFog_SetEnabled(task->fog, 0);
    }
}

void func_ov021_021a232c(WeatherTask *task, u32 a1, u32 a2, u32 a3, u32 a4) {
    s32 half;
    u32 shift;

    if (a4 == 1) {
        FieldFog_Animate(task->fog, a2, a1, a3);
        return;
    }
    if (a4 == 2) {
        if (a1 == FieldFog_GetDepthShift(task->fog)) {
            FieldFog_Animate(task->fog, a2, a1, a3);
            return;
        }
        half = (s32)a3 / 2;
        shift = FieldFog_GetDepthShift(task->fog);
        FieldFog_Animate(task->fog, 0x7fdf, shift, half);
        func_ov021_021a2cc8(task, a1, a2, half);
    }
}


void func_ov021_021a2390(WeatherTask *task, u32 a1, u32 a2, u32 a3) {
    u32 depthShift;

    if (a3 == 1) {
        depthShift = FieldFog_GetDepthShift(task->fog);
        FieldFog_Animate(task->fog, a1, depthShift, a2);
    }
}

BOOL func_ov021_021a23b0(WeatherTask *task) {
    if (FieldFog_IsAnimationRunning(task->fog) != 0) {
        return FALSE;
    }
    if (func_ov021_021a2cf8(task) == 0) {
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov021_021a23d4(WeatherTask *task) {
    if (func_ov021_021a0354(task->fogCtrl) == 1) {
        return 1;
    }
    return 0;
}

BOOL func_ov021_021a23e8(WeatherTask *task) {
    BOOL BVar1;

    BVar1 = func_ov021_021a0380(task->fogCtrl);
    return BVar1;
}

BOOL func_ov021_021a23f4(WeatherTask *task) {
    return func_ov021_021a0390(task->fogCtrl);
}

u32 func_ov021_021a2400(WeatherTask *task) {
    return func_ov021_021a03a4(task->fogCtrl);
}

u32 func_ov021_021a240c(WeatherTask *task) {
    return func_ov021_021a03b8(task->fogCtrl);
}

u32 func_ov021_021a2418(WeatherTask *task) {
    return func_ov021_021a03cc(task->fogCtrl);
}

void func_ov021_021a2424(WeatherTask *task) {
    VecFx32 target;

    if (task->camera != NULL) {
        GFL_G3DCameraGetLookatTarget(FieldCamera_GetG3DCamera(task->camera), &target);
        task->lookAt = target;
    }
}

void func_ov021_021a244c(WeatherTask *task, int *x, int *y) {
    G3DCamera *cam;
    VecFx32 target;
    VecFx32 pos;
    fx32 dx;
    fx32 dz;
    fx32 a;
    fx32 b;
    fx32 dist;
    fx32 sx;
    fx32 sz;
    fx32 fx1;
    fx32 fx2;

    if (task->camera == NULL) {
        *x = 0;
        *y = 0;
        return;
    }
    cam = FieldCamera_GetG3DCamera(task->camera);
    GFL_G3DCameraGetLookatTarget(cam, &target);
    GFL_G3DCameraGetLookatPos(cam, &pos);
    dx = target.x - task->lookAt.x;
    dz = target.z - task->lookAt.z;
    if (MATH_ABS(dx) > 0x3e8000 || MATH_ABS(dz) > 0x64000) {
        task->lookAt = target;
        *x = 0;
        *y = 0;
        return;
    }
    dist = VEC_Distance(&target, &pos);
    func_ov021_021a2d30(NNS_G3dGlbGetProjectionMtx(), dist, &a, &b);
    a = FX_Div(a, 0x100000);
    b = FX_Div(b, 0xc0000);
    sx = 0x1000;
    if (dx < 0) {
        dx = -dx;
        sx = -0x1000;
    }
    fx1 = FX_Div(dx, a);
    if (sx < 0) {
        fx1 = -fx1;
        fx1 += 0x1000;
    }
    sz = 0x1000;
    if (dz < 0) {
        dz = -dz;
        sz = -0x1000;
    }
    fx2 = FX_Div(dz, b);
    if (sz < 0) {
        fx2 = -fx2;
        fx2 += 0x1000;
    }
    if (fx1 + fx2 != 0) {
        task->lookAt = target;
    }
    *x = fx1 >> 12;
    *y = fx2 >> 12;
}


void func_ov021_021a2564(WeatherTask *task, int x, int y) {
    WeatherObj *obj = task->objHead;
    ClActor *actor;
    ClActorPos pos;
    s16 dy;
    s16 dx;

    if (obj != NULL) {
        dx = x;
        dy = y;
        do {
            actor = func_ov021_021a2c28(obj);
            func_0204b53c(actor, &pos);
            pos.x -= dx;
            pos.y -= dy;
            func_0204b528(actor, &pos);
            obj = obj->next;
        } while (obj != task->objHead);
    }
}



void func_ov021_021a25bc(WeatherTask *task, u16 a1) {
    func_ov021_021a2c30(&task->sound, task->fieldSound, a1);
}


void func_ov021_021a25cc(WeatherTask *task) {
    func_ov021_021a2c6c(&task->sound, task->fieldSound);
}

void func_ov021_021a25dc(WeatherTask *task, u32 param_2) {
    BOOL BVar1;

    BVar1 = func_02005c64();
    if (BVar1 == 0) {
        if (task->sound.ambience != 0) {
            FieldSnd_PlayAmbience(task->fieldSound, param_2);
            return;
        }
        GFL_SndSEPlay(param_2);
    }
}

void func_ov021_021a2604(WeatherTask *task, u32 param_2, u32 param_3) {
    if (func_02005c64() == 0) {
        if (task->sound.ambience != 0) {
            FieldSnd_PlayAmbienceEx(task->fieldSound, param_2, param_3);
            return;
        }
        GFL_SndSEPlayEx(param_2, param_3);
    }
}


void *func_ov021_021a2630(WeatherObj *obj) {
    return func_ov021_021a2c24(obj);
}

ClActor *func_ov021_021a2638(WeatherObj *obj) {
    return func_ov021_021a2c28(obj);
}



void func_ov021_021a2640(WeatherObj *obj, ClActorPos *pos) {
    ClActor *actor;

    actor = func_ov021_021a2c28(obj);
    func_0204b454(actor, pos, 0);
}

void func_ov021_021a2654(WeatherObj *obj, ClActorPos *pos) {
    ClActorPos local;

    local = *pos;
    if (local.x > 0x140) {
        local.x %= 0x140;
    } else if (local.x < -0x40) {
        local.x += 0x140;
    }
    if (local.y > 0x100) {
        local.y %= 0x100;
    } else if (local.y < -0x40) {
        local.y += 0x100;
    }
    func_0204b404(func_ov021_021a2c28(obj), &local, 0);
}


void func_ov021_021a26c4(WeatherObj *obj, const ClActorPos *pos) {
    ClActor *actor;

    actor = func_ov021_021a2c28(obj);
    func_0204b404(actor, pos, 0);
}

void func_ov021_021a26d8(WeatherTask *task) {
    ClActUnit *unit;
    FieldFog *fog;
    void *fogCtrl;
    FieldLightWork *light;
    Field3DBg **bgs;
    FieldCamera *camera;
    FieldSound *fieldSound;
    u16 season;

    unit = task->unit;
    fog = task->fog;
    fogCtrl = task->fogCtrl;
    light = task->light;
    camera = task->camera;
    fieldSound = task->fieldSound;
    bgs = task->bgs;
    season = task->season;
    MI_CpuFill8(task, 0, sizeof(WeatherTask));
    task->camera = camera;
    task->unit = unit;
    task->fieldSound = fieldSound;
    task->fog = fog;
    task->season = season;
    task->light = light;
    task->bgs = bgs;
    task->fogCtrl = fogCtrl;
}


BOOL func_ov021_021a2724(WeatherTask *task, WeatherFunc func, HeapID heapId) {
    if (func != NULL) {
        return func(task, task->mode, heapId);
    }
    return 1;
}


u8 func_ov021_021a273c(WeatherTask *task) {
    return data_ov021_021d88e8[task->state];
}

WeatherObj *func_ov021_021a274c(WeatherTask *task) {
    WeatherObj *obj;
    int i = 0;

    obj = task->objs;
    for (; i < WEATHER_OBJ_COUNT; i++) {
        if (!func_ov021_021a2bc4(obj + i)) {
            return &task->objs[i];
        }
    }
    return NULL;
}

void func_ov021_021a277c(WeatherTask *task, WeatherObj *obj) {
    WeatherObj *val;

    task->objCount = task->objCount + 1;
    if (task->objHead == NULL) {
        task->objHead = obj;
        obj->next = obj;
        task->objHead->prev = obj;
        return;
    }
    val = task->objHead->prev;
    obj->prev = val;
    obj->next = val->next;
    val->next->prev = obj;
    val->next = obj;
    func_0204b3dc(obj->actor, 1);
}

void func_ov021_021a27b8(WeatherTask *task, WeatherObj *obj) {
    task->objCount--;
    if (task->objHead == obj) {
        if (obj->next == obj) {
            task->objHead = NULL;
        } else {
            task->objHead = obj->next;
        }
    }
    obj->next->prev = obj->prev;
    obj->prev->next = obj->next;
    obj->next = NULL;
    obj->prev = NULL;
    func_0204b3dc(obj->actor, FALSE);
}




void func_ov021_021a27f8(WeatherTask *task) {
    WeatherObj *val;
    WeatherObj *cur;

    if (task->objHead != NULL && task->data->objFunc != NULL) {
        cur = task->objHead;
        val = cur->next;
        do {
            task->data->objFunc(cur);
            cur = val;
            val = val->next;
            if (cur == task->objHead) {
                return;
            }
        } while (task->objHead != NULL);
    }
}


void func_ov021_021a282c(WeatherTask *task, HeapID heapId) {
    func_ov021_021a2978(&task->res, task->data, heapId);
    func_ov021_021a29a8(&task->res, task->data, heapId);
}

void func_ov021_021a284c(WeatherTask *task) {
    func_ov021_021a29f4(&task->res);
    func_ov021_021a2994(&task->res);
}

void func_ov021_021a2860(WeatherTask *task, HeapID heapId) {
    s32 i;

    for (i = 0; i < task->data->graphics.bgCount; i++) {
        func_ov021_021a2a18(&task->res, i, task->bgs[i], task->data->graphics.bgRes[i], heapId);
    }
}


void func_ov021_021a28a0(WeatherTask *task) {
    s32 i;

    for (i = 0; i < task->data->graphics.bgCount; i++) {
        func_ov021_021a2a68(&task->res, i, task->bgs[i]);
    }
}

void func_ov021_021a28cc(WeatherTask *task, HeapID heapId) {
    s32 i;

    task->work = GFL_HeapAllocate(heapId, task->data->workSize, TRUE, "weather_task.c", 0x874);
    if (task->data->graphics.hasObjs != 0) {
        for (i = 0; i < WEATHER_OBJ_COUNT; i++) {
            func_ov021_021a2bd4(&task->objs[i], task, &task->res, task->unit, heapId);
        }
    }
}


void func_ov021_021a2924(WeatherTask *task) {
    s32 i;

    if (task->work != NULL) {
        GFL_HeapFree(task->work);
        task->work = NULL;
    }
    if (task->data->graphics.hasObjs != 0) {
        for (i = 0; i < WEATHER_OBJ_COUNT; i++) {
            func_ov021_021a2c10(&task->objs[i]);
            func_ov021_021a2bb8(&task->objs[i]);
        }
    }
    func_ov021_021a25cc(task);
}



void func_ov021_021a2968(WeatherTask *task) {
    task->loaded = 1;
}

u8 func_ov021_021a2970(WeatherTask *task) {
    return task->loaded;
}

void func_ov021_021a2978(WeatherRes *res, const WeatherData *data, HeapID heapId) {
    ArcTool *arc;

    if ((data->graphics.hasObjs != 0) || (data->graphics.bgCount != 0)) {
        arc = GFL_ArcSysCreateFileHandle((u32)data->graphics.arcId, heapId);
        res->arc = arc;
    }
}

void func_ov021_021a2994(WeatherRes *res) {
    if (res->arc != NULL) {
        GFL_ArcToolFree(res->arc);
        res->arc = NULL;
    }
}

void func_ov021_021a29a8(WeatherRes *res, const WeatherData *data, HeapID heapId) {
    u32 result;

    if (data->graphics.hasObjs != 0) {
        result = func_0204a6c8(res->arc, (u32)data->graphics.charsFileId, 0, 0, heapId);
        res->chars = (u16)result;
        result = func_0204ac18(res->arc, (u32)data->graphics.paletteFileId, 0, 0, 0, 1, heapId);
        res->palette = (u16)result;
        result = func_0204af28(res->arc, (u32)data->graphics.cellFileId, (u32)data->graphics.cellAnimFileId, heapId);
        res->cells = (u16)result;
        res->loaded = 1;
    }
}

void func_ov021_021a29f4(WeatherRes *res) {
    if (res->loaded != 0) {
        func_0204a8d4((u32)res->chars);
        func_0204ada4((u32)res->palette);
        func_0204afd8((u32)res->cells);
        res->loaded = 0;
    }
}

void func_ov021_021a2a18(WeatherRes *res, s32 index, Field3DBg *bg, const void *config, HeapID heapId) {
    MI_CpuCopy8(config, &data_ov021_021ee710, sizeof(Field3DBgRes));
    if (data_ov021_021ee710.alpha != 0x1f) {
        data_ov021_021ee710.alpha = 0x1f;
    }
    func_ov021_021a1698(bg, res->arc, &data_ov021_021ee710, heapId);
    func_ov021_021a16d4(bg, 0);
    func_ov021_021a16d8(bg, 0x20);
    res->bgActive[index] = 1;
}

// Black's code at ov021 0x021a2a68-0x021a2d88, which shares this file's .rodata (the object at 0x021d88e0 sorts before the table at 0x021d88e8)
const ClActorSetup data_ov021_021d88e0 = { 0, 0, 0, 0x40, 3 };

void FieldFog_AnimateEx(FieldFog *fog, s32 param_2, s32 param_3, u32 param_4, u32 param_5, u16 param_6);

void func_ov021_021a2a68(WeatherRes *res, s32 index, Field3DBg *bg) {
    if (res->bgActive[index] != 0) {
        func_ov021_021a1680(bg);
        res->bgActive[index] = 0;
    }
}

void func_ov021_021a2a84(WeatherTimer *timer, WeatherTask *owner, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8,
                         s32 a9, WeatherSpawnFunc a10) {
    timer->owner = owner;
    timer->counter = 0;
    timer->stepCount = 0;
    timer->value = a4;
    timer->valueTarget = a5;
    timer->interval = a3;
    timer->intervalTarget = a6;
    timer->intervalStep = a7;
    timer->stepsMax = a8;
    timer->valueStep = a9;
    timer->func = a10;
}


void func_ov021_021a2aac(WeatherTimer *timer, u16 a2, u16 a3, u16 a4, s32 a5) {
    timer->valueTarget = a2;
    timer->intervalTarget = a3;
    timer->intervalStep = a4;
    timer->valueStep = a5;
}

BOOL func_ov021_021a2ab8(WeatherTimer *timer, HeapID heapId) {
    u32 flags = 0;
    s32 dir;

    if (timer->intervalStep == 0) {
        flags |= 1;
    } else {
        if (timer->intervalStep > 0) {
            dir = 0;
        } else {
            dir = 1;
        }
        if ((timer->interval >= timer->intervalTarget && dir == 0) || (timer->interval <= timer->intervalTarget && dir == 1)) {
            flags |= 1;
            timer->interval = timer->intervalTarget;
        }
    }
    if (timer->valueStep == 0) {
        flags |= 2;
    } else {
        if (timer->valueStep > 0) {
            dir = 0;
        } else {
            dir = 1;
        }
        if ((timer->value >= timer->valueTarget && dir == 0) || (timer->value <= timer->valueTarget && dir == 1)) {
            flags |= 2;
            timer->value = timer->valueTarget;
        }
    }
    timer->counter = timer->counter - 1;
    if (timer->counter <= 0) {
        timer->func(timer->owner, timer->value, heapId);
        timer->counter = timer->interval;
        if (!(flags & 1)) {
            timer->interval += timer->intervalStep;
        }
        if (!(flags & 2)) {
            timer->stepCount = timer->stepCount + 1;
            if (timer->stepCount >= timer->stepsMax) {
                timer->stepCount = 0;
                timer->value = timer->value + (s16)timer->valueStep;
            }
        }
    }
    return (flags & 1) != 0;
}


BOOL func_ov021_021a2b8c(WeatherTimer *timer, HeapID heapId) {
    timer->counter -= 1;
    if (timer->counter <= 0) {
        timer->func(timer->owner, timer->value, heapId);
        timer->counter = timer->interval;
        return TRUE;
    }
    return FALSE;
}



void func_ov021_021a2bb8(WeatherObj *obj) {
    MI_CpuFill8(obj, 0, sizeof(WeatherObj));
}

BOOL func_ov021_021a2bc4(WeatherObj *obj) {
    if (obj->prev != NULL) {
        return 1;
    }
    return 0;
}

void func_ov021_021a2bd4(WeatherObj *obj, WeatherTask *task, const WeatherRes *res, ClActUnit *unit, HeapID heapId) {
    obj->task = task;
    obj->actor = func_0204b294(unit, res->chars, res->palette, res->cells, &data_ov021_021d88e0, 0, heapId);
    func_0204b3dc(obj->actor, FALSE);
    func_0204ba20(obj->actor, FALSE);
}


void func_ov021_021a2c10(WeatherObj *obj) {
    if (obj->actor != NULL) {
        func_0204b3b4(obj->actor);
        obj->actor = NULL;
    }
}

void *func_ov021_021a2c24(WeatherObj *obj) {
    return obj->work;
}

ClActor *func_ov021_021a2c28(WeatherObj *obj) {
    return obj->actor;
}

void func_ov021_021a2c2c(WeatherSound *sound, u32 ambience) {
    sound->ambience = ambience;
}

void func_ov021_021a2c30(WeatherSound *sound, FieldSound *fieldSound, u16 se) {
    func_ov021_021a2c6c(sound, fieldSound);
    if (func_02005c64() == FALSE) {
        sound->state = 1;
        sound->se = se;
        if (sound->ambience != 0) {
            FieldSnd_PlayAmbience(fieldSound, sound->se);
        } else {
            GFL_SndSEPlay(sound->se);
        }
        return;
    }
    sound->state = 2;
    sound->se = se;
}


void func_ov021_021a2c6c(WeatherSound *sound, FieldSound *fieldSound) {
    s32 player;

    if (sound->state == 1) {
        sound->state = 0;
        if (sound->ambience != 0) {
            FieldSnd_StopAmbience(fieldSound, sound->se);
            return;
        }
        player = GFL_SndSeqGetPlayerIndex(sound->se);
        GFL_SndPlayerStop(player);
    }
}

void func_ov021_021a2c98(WeatherSound *sound, FieldSound *fieldSound) {
    BOOL BVar1;

    if ((sound->state == 2) && (BVar1 = func_02005c64(), BVar1 == 0)) {
        sound->state = 1;
        if (sound->ambience != 0) {
            FieldSnd_PlayAmbience(fieldSound, sound->se);
            return;
        }
        GFL_SndSEPlay(sound->se);
    }
}

void func_ov021_021a2cc8(WeatherTask *task, u32 depthShift, u32 color, s32 duration) {
    task->fogPending = 1;
    task->fogDepthShift = depthShift;
    task->fogColor = color;
    task->fogDuration = (s8)(duration / 2);
}

void func_ov021_021a2cec(WeatherTask *task) {
    task->fogPending = 0;
}

u8 func_ov021_021a2cf8(WeatherTask *task) {
    return task->fogPending;
}

void func_ov021_021a2d04(WeatherTask *task) {
    FieldFog_AnimateEx(task->fog, 0x7fdf, task->fogColor, (u32)task->fogDepthShift, (u16)task->fogDepthShift,
                       (u16)task->fogDuration);
}

void func_ov021_021a2d30(const MtxFx44 *proj, fx32 numer, fx32 *out1, fx32 *out2) {
    *out2 = (fx32)((((s64)FX_Div(numer, proj->m[1][1]) << 13) + 0x800) >> 12);
    *out1 = (fx32)((((s64)FX_Div(numer, proj->m[0][0]) << 13) + 0x800) >> 12);
}
