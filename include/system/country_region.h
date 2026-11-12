// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/country_region.h, the lines in use
// so far.
#ifndef BWPORT_SYSTEM_COUNTRY_REGION_H
#define BWPORT_SYSTEM_COUNTRY_REGION_H

#include "types.h"

// The countries a player can say they live in, and the regions of the countries that are divided into regions
// (country_region.c, a guessed name). Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

// The highest region number of a country, or 0 if it isn't divided into regions
u32 CountryHasProvinces(u32 country);

// The index of a country among those divided into regions, or 0 if it isn't divided
u32 getSubdividedCountryArrayNum(u32 country);

// Country codes run from 1, with 0 for none
#define COUNTRY_NONE 0

// Black 2's, not checked against Black
#define COUNTRY_JAPAN 105

#define COUNTRY_CODE_COUNT 233

// pokebw2's Country_GetSubdividedCountryCode
u32 func_02025bb8(u32 index);
// pokebw2's Country_GetPlaceDataID
u32 func_02025bd0(u32 index);
// pokebw2's Country_GetJapan
u32 func_02025be8(void);
// pokebw2's Country_GetValidCountry
// The country, or 0 if the country and region aren't valid
u8 func_02025c1c(u8 country, u8 region, u8 japanOnly);
// pokebw2's Country_GetValidRegion
// The region, or 0 if the country and region aren't valid
u8 func_02025c30(u8 country, u8 region, u8 japanOnly);
// pokebw2's Country_GetRegionOrder
// The regions of a country in the order they are listed, and how many there are; FALSE if it isn't divided
BOOL func_02025c44(u32 country, const u8 **order, u32 *count);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The message file with the names of a country's regions
u32 fetchFileNumOfDividedCountry(u32 country);
u32 getFileNumForSubdividedRegion(u32 index);
// pokebw2's Country_GetSubdividedCount
// How many countries are divided into regions, counting the entry for an undivided country
u32 func_02025b38(void);
// pokebw2's Country_IsValidPlace
// Whether a country and region can be chosen: a country divided into regions needs a region, and with japanOnly only
// Japan or no country is valid
BOOL func_02025bec(u32 country, u32 region, BOOL japanOnly);

#endif // BWPORT_SYSTEM_COUNTRY_REGION_H
