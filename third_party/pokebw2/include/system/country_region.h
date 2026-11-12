#ifndef POKEBW2_SYSTEM_COUNTRY_REGION_H
#define POKEBW2_SYSTEM_COUNTRY_REGION_H

#include "types.h"

// The countries a player can say they live in, and the regions of the countries that are divided into regions
// (country_region.c, a guessed name). Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except
// those with a Country_ prefix, gSelectableCountryCount and gSelectableCountries.

// Country codes run from 1, with 0 for none
#define COUNTRY_NONE 0
#define COUNTRY_JAPAN 105
#define COUNTRY_CODE_COUNT 233

// The countries the player can choose from, in the order the choice lists them
extern const u8 gSelectableCountries[130];
extern const int gSelectableCountryCount;

// How many countries are divided into regions, counting the entry for an undivided country
u32 Country_GetSubdividedCount(void);
// The index of a country among those divided into regions, or 0 if it isn't divided
u32 getSubdividedCountryArrayNum(u32 country);
// The highest region number of a country, or 0 if it isn't divided into regions
u32 CountryHasProvinces(u32 country);
// The message file with the names of a country's regions
u32 fetchFileNumOfDividedCountry(u32 country);
u32 getFileNumForSubdividedRegion(u32 index);
u32 Country_GetSubdividedCountryCode(u32 index);
u32 Country_GetPlaceDataID(u32 index);
u32 Country_GetJapan(void);
// Whether a country and region can be chosen: a country divided into regions needs a region, and with japanOnly only
// Japan or no country is valid
BOOL Country_IsValidPlace(u32 country, u32 region, BOOL japanOnly);
// The country, or 0 if the country and region aren't valid
u8 Country_GetValidCountry(u8 country, u8 region, u8 japanOnly);
// The region, or 0 if the country and region aren't valid
u8 Country_GetValidRegion(u8 country, u8 region, u8 japanOnly);
// The regions of a country in the order they are listed, and how many there are; FALSE if it isn't divided
BOOL Country_GetRegionOrder(u32 country, const u8 **order, u32 *count);

#endif // POKEBW2_SYSTEM_COUNTRY_REGION_H
