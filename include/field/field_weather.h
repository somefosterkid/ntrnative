// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_weather.h, the lines in use so
// far.
#ifndef BWPORT_FIELD_FIELD_WEATHER_H
#define BWPORT_FIELD_FIELD_WEATHER_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/clact.h"
#include "gfl/tcb.h"
#include "field/field_3dbg.h"
#include "work_area.h"

// The field's weathers, kept by the weather system in overlay 21 (overlay 36 in Black 2)

// What a weather's functions get: one of the weather's two tasks (src/ov021/weather_task.c has its layout)
typedef struct WeatherTask WeatherTask;
// pokebw2's FieldWeather: the field's weather, which runs its two tasks (weather.c). Black's layout, by offset: only
// the fields the weather code reads so far
typedef struct FieldWeather FieldWeather;
struct FieldWeather {
    Field3DBg *unk_00[2]; // the two background layers, field_3dbg.c's
    u16 unk_08; // an index into the function table data_ov021_021dde6c
    u16 unk_0a;
    u16 unk_0c;
    u16 unk_0e;
    u32 unk_10;
    u16 unk_14;
    u8 unk_16[0x18 - 0x16];
    ClActUnit *unk_18;
    ClActRenderer *unk_1c;
    WeatherTask *unk_20[2];
    s32 unk_28;
    TCB *unk_2c;
};

void func_ov021_021a1bdc(FieldWeather *weather, u32 weatherId);
// Sets the weather at once, without a transition
void func_ov021_021a1be4(FieldWeather *weather, u32 weatherId);

typedef struct WeatherObj WeatherObj;

// Black 2's, not checked against Black
// Called from FieldWeather_Update with the heap it was given
typedef BOOL (*WeatherFunc)(WeatherTask *task, u32 arg, HeapID heapId);

// pokebw2's: the weather's graphics: its archive, whether it has particles, how many of the two background layers it
// uses, the files of the particles' characters, palette, cells and cell animations, and each layer's texture (the
// offsets weather_task.c reads)
typedef struct {
    u16 arcId;
    u16 hasObjs;
    u16 bgCount;
    u16 charsFileId;
    u16 paletteFileId;
    u16 cellFileId;
    u16 cellAnimFileId;
    u16 bgRes[2][6];
    u16 unk26;
} WeatherGraphics;

// Black 2's; the offsets weather_task.c reads agree
typedef struct {
    WeatherGraphics graphics;
    // The size of the work that func_ov036_021997e4 returns
    u32 workSize;
    WeatherFunc funcs[7];
    // Moves one particle, every frame
    void (*objFunc)(WeatherObj *obj);
} WeatherData;

BOOL func_ov024_021ee740(WeatherTask *task, u32 arg, HeapID heapId);
BOOL func_ov024_021ee83c(WeatherTask *task, u32 arg, HeapID heapId);
BOOL func_ov024_021ee874(WeatherTask *task, u32 arg, HeapID heapId);

// Black 2's, not checked against Black
// Adds count particles
typedef void (*WeatherSpawnFunc)(WeatherTask *task, int count, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL func_ov021_021a23d4(WeatherTask *task);
void *func_ov021_021a21b8(WeatherTask *task);
BOOL func_ov021_021a23e8(WeatherTask *task);
u32 func_ov021_021a2400(WeatherTask *task);
void func_ov021_021a22e0(WeatherTask *task, u32 a1, u32 a2, u32 a3);
u32 func_ov021_021a240c(WeatherTask *task);
void func_ov021_021a232c(WeatherTask *task, u32 a1, u32 a2, u32 a3, u32 a4);
BOOL func_ov021_021a23f4(WeatherTask *task);
void func_ov021_021a2108(WeatherTask *task, u32 a1, u32 a2);
u32 func_ov021_021a2418(WeatherTask *task);
BOOL func_ov021_021a23b0(WeatherTask *task);
void func_ov021_021a2114(WeatherTask *task, u32 a1, u32 a2);
void func_ov021_021a2390(WeatherTask *task, u32 a1, u32 a2, u32 a3);
void func_ov021_021a231c(WeatherTask *task, u32 a1);
void func_ov021_021a2120(WeatherTask *task, u32 a1);
BOOL func_ov024_021ee788(WeatherTask *task, u32 arg, HeapID heapId);
BOOL func_ov024_021ee7f0(WeatherTask *task, u32 arg, HeapID heapId);
BOOL func_ov024_021ee838(WeatherTask *task, u32 arg, HeapID heapId);
BOOL func_ov024_021ee860(WeatherTask *task, u32 arg, HeapID heapId);
// pokebw2's FieldLightStatus_GetWeatherLight
// Main: an entry of the u16 table at sWeatherLightIds, which the weather overlays pass as a sound
u16 func_02014a10(u32 index);
BOOL func_ov021_021a21bc(WeatherTask *task);
u32 func_ov021_021a2148(WeatherTask *task);
void func_ov021_021a21f8(WeatherTask *task, u32 a1, u32 a2, u32 a3, int a4, int a5, int a6, int a7,
                         WeatherSpawnFunc spawn);
void func_ov021_021a25bc(WeatherTask *task, u16 a1);
BOOL func_ov021_021a223c(WeatherTask *task, HeapID heapId);
void func_ov021_021a2160(WeatherTask *task, u32 a1, u32 index);
void func_ov021_021a2254(WeatherTask *task, WeatherSpawnFunc spawn, int count, int a3, int a4, HeapID heapId);
BOOL func_ov021_021a2248(WeatherTask *task, HeapID heapId);
void func_ov021_021a2228(WeatherTask *task, u8 a1, u8 a2, u8 a3, int a4);
void *func_ov021_021a2630(WeatherObj *obj);
ClActor *func_ov021_021a2638(WeatherObj *obj);
void func_ov021_021a21e0(WeatherObj *obj);
WeatherObj *func_ov021_021a21c8(WeatherTask *task, HeapID heapId);
void func_ov021_021a2190(WeatherTask *task, int value, u32 index);
void func_ov021_021a21a0(WeatherTask *task, int value, u32 index);
void func_ov021_021a25cc(WeatherTask *task);
void func_ov021_021a25dc(WeatherTask *task, u32 sound);
void func_ov021_021a2604(WeatherTask *task, u32 sound, u32 volume);
void func_ov021_021a2424(WeatherTask *task);
void func_ov021_021a2640(WeatherObj *obj, ClActorPos *pos);
void func_ov021_021a2654(WeatherObj *obj, ClActorPos *pos);
void func_ov021_021a244c(WeatherTask *task, int *x, int *y);
void func_ov021_021a2564(WeatherTask *task, int x, int y);
void func_ov021_021a26c4(WeatherObj *obj, const ClActorPos *pos);
void func_ov021_021a2154(WeatherTask *task, u16 a1, u8 a2);
void func_ov021_021a2130(WeatherTask *task, u16 a1, u32 a2, u32 a3);
BOOL func_ov021_021a213c(WeatherTask *task);
void func_ov021_021a2170(WeatherTask *task, int value, u32 index);
void func_ov021_021a2180(WeatherTask *task, int value, u32 index);

// The particle's own work in WeatherObj (func_ov021_021a2630's), which each weather casts to its particle: Black's 0x28
// bytes, more on a PC target, where the snow's Flake (two pointers) is 0x30. Each particle's file checks that it fits
#define WEATHER_OBJ_WORK_SIZE WORK_AREA_SIZE(0x28)

// One of a weather's particles (pokebw2's names), in its task's circular list; Black's layout as the functions that
// reach it have it, 0x38 bytes on ARM (weather_task.c clears sizeof(WeatherObj))
struct WeatherObj {
    WeatherTask *task;
    ClActor *actor;
    u8 work[WEATHER_OBJ_WORK_SIZE];
    WeatherObj *prev;
    WeatherObj *next;
};

#endif // BWPORT_FIELD_FIELD_WEATHER_H
