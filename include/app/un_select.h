// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/un_select.h: the lines in use so far.
#ifndef BWPORT_APP_UN_SELECT_H
#define BWPORT_APP_UN_SELECT_H

#include "types.h"
#include "field/unity_tower.h"

// Black 2's, not checked against Black
// The countries of the survey (field/unity_tower.h has the number of visitors it lists)
#define UNITY_TOWER_COUNTRY_MAX 232

// Black 2's; twinned code confirms the offsets of floor, selectedFloor, selectedCountry and selected in Black
struct UnSelectParam {
    // The floor the player is on
    u8 floor;
    // What the player chose: the floor and its country, or 0 for none
    u8 selectedFloor;
    u16 selectedCountry;
    BOOL selected;
    // The countries of the latest visitors, 0 past the last
    u16 visitorCountries[UNITY_TOWER_VISITOR_MAX];
    // For each country from 1, whether a visitor from it has come
    u8 countryVisited[UNITY_TOWER_COUNTRY_MAX];
};

#endif // BWPORT_APP_UN_SELECT_H
