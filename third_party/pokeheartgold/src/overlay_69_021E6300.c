#include "global.h"

#include "sys_task.h"

u32 GeonetLocations_GetIndexFromCountry(u32 country);

u32 ov69_021E6300(u32 country);

u32 ov69_021E6300(u32 country) {
    return GeonetLocations_GetIndexFromCountry(country);
}
