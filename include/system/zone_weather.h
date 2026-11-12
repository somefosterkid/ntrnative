// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/zone_weather.h, the lines in use so
// far.
#ifndef BWPORT_SYSTEM_ZONE_WEATHER_H
#define BWPORT_SYSTEM_ZONE_WEATHER_H

#include "types.h"
#include "struct_decls.h"

// zone_weather.c: which weather a zone has. The ROM doesn't name the file; the name is a guess. Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except WEATHER_NONE

// No weather chosen, so the next rule decides
#define WEATHER_NONE 0xffff

u8 GetWeatherAll(GameSystem *gsys, u32 zoneId);

// Sets the current weather to the zone's
void ResetWeather(GameSystem *gsys, u32 zoneId);

// Sets the current weather to a forced one, or back to the zone's own from weathers 6 and 7
void UpdateWeatherToDefault(GameData *gameData, u32 zoneId);

#endif // BWPORT_SYSTEM_ZONE_WEATHER_H
