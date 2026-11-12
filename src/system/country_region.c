// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/country_region.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "system/country_region.h"

// The countries a player can say they live in, and the regions of the 18 countries that are divided into regions: the
// message file with their names, and the order a choice lists them in. The file's name is a guess: the ROM has no
// string for it. data_0209f8a6, data_0209f8cf, data_0209f914 and data_0209f9fe are swan's
// names, and the other lists are named after them.

typedef struct {
    u8 country;
    // Which country's place data, 18 for the world
    u8 placeDataId;
    // The message file with the names of the regions
    u16 msgFileId;
    const u8 *regionOrder;
    u8 regionCount;
    // The highest region number. Sweden's regions skip a number
    u8 regionMax;
} SubdividedCountry;

// pokebw2's argentina_provinces_list
static const u8 data_0209f955[] = {
    1, 3, 4, 5, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
};

// pokebw2's australia_states_list
static const u8 data_0209f8ae[] = {
    1, 2, 3, 4, 5, 6, 7, 8,
};

// pokebw2's brazil_states_list
static const u8 data_0209f96d[] = {
    1, 3, 4, 5, 6, 7, 9, 8, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 21, 2, 23, 24, 25, 26, 27,
};

// pokebw2's canada_provinces_list
static const u8 data_0209f8c2[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
};

// pokebw2's china_provinces_list
static const u8 data_0209f988[] = {
#include "asset/country_region_data_0209f988.inc"
};

// pokebw2's germany_states_list
static const u8 data_0209f8df[] = {
    1, 2, 3, 4, 5, 6, 7, 9, 8, 10, 11, 12, 13, 14, 15, 16,
};

// pokebw2's spain_communities_list
static const u8 data_0209f8ef[] = {
    1, 2, 3, 4, 5, 6, 7, 9, 8, 10, 11, 12, 16, 13, 14, 15, 17,
};

// pokebw2's finland_provinces_list
static const u8 data_0209f8a0[] = {
    1, 3, 2, 5, 4, 6,
};

// pokebw2's france_regions_list
static const u8 data_0209f93f[] = {
    2, 1, 4, 13, 12, 22, 6, 5, 11, 3, 18, 19, 21, 8, 17, 7, 15, 10, 16, 14, 20, 9,
};

// pokebw2's uk_regions_list
static const u8 data_0209f8b6[] = {
    2, 1, 3, 4, 6, 5, 7, 8, 9, 10, 11, 12,
};

// pokebw2's india_states_list
static const u8 data_0209f9a9[] = {
#include "asset/country_region_data_0209f9a9.inc"
};

// pokebw2's italy_regions_list
static const u8 data_0209f900[] = {
    1, 13, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 17, 16, 18, 19, 20,
};

// pokebw2's japan_prefectures_list
static const u8 data_0209f9cc[] = {
#include "asset/country_region_data_0209f9cc.inc"
};

// pokebw2's norway_counties_list
static const u8 data_0209f929[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 22, 14, 15, 16, 17, 18, 19, 20, 21,
};

// pokebw2's poland_provinces_list
static const u8 data_0209f8cf[] = {
    15, 13, 2, 6, 5, 1, 3, 4, 7, 8, 10, 11, 12, 9, 14, 16,
};

// pokebw2's russia_districts_list
static const u8 data_0209f8a6[] = {
    1, 2, 8, 3, 5, 6, 7, 4,
};

// pokebw2's sweden_county_list
static const u8 data_0209f914[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
};

// pokebw2's state_number_list
static const u8 data_0209f9fe[] = {
#include "asset/country_region_data_0209f9fe.inc"
};

// The first entry stands for a country that isn't divided
static const SubdividedCountry data_0209fa34[] = {
    { COUNTRY_NONE, 18, 0xe7, NULL, 0, 0 },
    { 9, 0, 0xd4, data_0209f955, NELEMS(data_0209f955), 24 },
    { 12, 1, 0xd5, data_0209f8ae, NELEMS(data_0209f8ae), 8 },
    { 28, 2, 0xd6, data_0209f96d, NELEMS(data_0209f96d), 27 },
    { 36, 3, 0xd7, data_0209f8c2, NELEMS(data_0209f8c2), 13 },
    { 43, 4, 0xd8, data_0209f988, NELEMS(data_0209f988), 33 },
    { 79, 5, 0xd9, data_0209f8df, NELEMS(data_0209f8df), 16 },
    { 195, 6, 0xda, data_0209f8ef, NELEMS(data_0209f8ef), 17 },
    { 72, 7, 0xdb, data_0209f8a0, NELEMS(data_0209f8a0), 6 },
    { 73, 8, 0xdc, data_0209f93f, NELEMS(data_0209f93f), 22 },
    { 218, 9, 0xdd, data_0209f8b6, NELEMS(data_0209f8b6), 12 },
    { 95, 10, 0xde, data_0209f9a9, NELEMS(data_0209f9a9), 35 },
    { 102, 11, 0xdf, data_0209f900, NELEMS(data_0209f900), 20 },
    { COUNTRY_JAPAN, 12, 0xe0, data_0209f9cc, NELEMS(data_0209f9cc), 50 },
    { 155, 13, 0xe1, data_0209f929, NELEMS(data_0209f929), 22 },
    { 166, 14, 0xe2, data_0209f8cf, NELEMS(data_0209f8cf), 16 },
    { 174, 15, 0xe3, data_0209f8a6, NELEMS(data_0209f8a6), 8 },
    { 200, 16, 0xe4, data_0209f914, NELEMS(data_0209f914), 22 },
    { 220, 17, 0xe6, data_0209f9fe, NELEMS(data_0209f9fe), 51 },
};

// pokebw2's Country_GetSubdividedCount
u32 func_02025b38(void) {
    return NELEMS(data_0209fa34);
}

u32 getSubdividedCountryArrayNum(u32 country) {
    u32 i;

    for (i = 0; i < NELEMS(data_0209fa34); i++) {
        if (country == data_0209fa34[i].country) {
            return i;
        }
    }
    return 0;
}

u32 CountryHasProvinces(u32 country) {
    u32 i;

    for (i = 0; i < NELEMS(data_0209fa34); i++) {
        if (country == data_0209fa34[i].country) {
            return data_0209fa34[i].regionMax;
        }
    }
    return 0;
}

u32 fetchFileNumOfDividedCountry(u32 country) {
    return getFileNumForSubdividedRegion(getSubdividedCountryArrayNum(country));
}

u32 getFileNumForSubdividedRegion(u32 index) {
    if (index < NELEMS(data_0209fa34)) {
        return data_0209fa34[index].msgFileId;
    }
    return 0xe7;
}

// pokebw2's Country_GetSubdividedCountryCode
u32 func_02025bb8(u32 index) {
    if (index < NELEMS(data_0209fa34)) {
        return data_0209fa34[index].country;
    }
    return COUNTRY_NONE;
}

// pokebw2's Country_GetPlaceDataID
u32 func_02025bd0(u32 index) {
    if (index < NELEMS(data_0209fa34)) {
        return data_0209fa34[index].placeDataId;
    }
    return 18;
}

// pokebw2's Country_GetJapan
u32 func_02025be8(void) {
    return COUNTRY_JAPAN;
}

// pokebw2's Country_IsValidPlace
BOOL func_02025bec(u32 country, u32 region, BOOL japanOnly) {
    if (country >= COUNTRY_CODE_COUNT) {
        return FALSE;
    }
    if (CountryHasProvinces(country) < region) {
        return FALSE;
    }
    if (japanOnly == TRUE && country != COUNTRY_JAPAN && country != COUNTRY_NONE) {
        return FALSE;
    }
    return TRUE;
}

// pokebw2's Country_GetValidCountry
u8 func_02025c1c(u8 country, u8 region, u8 japanOnly) {
    if (!func_02025bec(country, region, japanOnly)) {
        country = COUNTRY_NONE;
    }
    return country;
}

// pokebw2's Country_GetValidRegion
u8 func_02025c30(u8 country, u8 region, u8 japanOnly) {
    if (!func_02025bec(country, region, japanOnly)) {
        region = 0;
    }
    return region;
}

// pokebw2's Country_GetRegionOrder
BOOL func_02025c44(u32 country, const u8 **order, u32 *count) {
    u32 index = getSubdividedCountryArrayNum(country);

    if (index == 0) {
        return FALSE;
    }
    *order = data_0209fa34[index].regionOrder;
    *count = data_0209fa34[index].regionCount;
    return TRUE;
}
