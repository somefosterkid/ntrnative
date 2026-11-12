// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/scrcmd_weather.c, which is the same code
// in Black 2/White 2's overlay 12. Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0).
#include "types.h"
#include "field/field.h"
#include "field/field_environment.h"
#include "field/field_script.h"
#include "field/field_weather.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/zone_weather.h"

// The script command that sets the field's weather (a descriptive name)

// The zone's own weather
#define WEATHER_OF_ZONE 0xffff

BOOL s0136_FieldSetWeather(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    GameData *gameData;
    Field *field;
    void *weather;
    u32 weatherId;
    u16 immediate;

    FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    gameData = FieldScriptEnv_GetGameData(env);
    field = GSYS_GetField(gsys);
    weather = Field_GetWeatherSystem(field);
    weatherId = ScriptReadAny(vm, env);
    immediate = ScriptReadAny(vm, env);
    if (weatherId == WEATHER_OF_ZONE) {
        weatherId = GetWeatherAll(gsys, Field_GetPlayerStateZoneID(field));
    }
    if (immediate) {
        func_ov021_021a1be4(weather, weatherId);
    } else {
        func_ov021_021a1bdc(weather, weatherId);
    }
    SetNowWeather(gameData, weatherId);
    return FALSE;
}
