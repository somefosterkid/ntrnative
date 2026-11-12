// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), build/include/generated/constants/flags.h: the lines
// in use so far.
#ifndef BWPORT_CONSTANTS_FLAGS_H
#define BWPORT_CONSTANTS_FLAGS_H

#include "types.h"

// Black 2's, not checked against Black
#define EVENT_FLAG_0x097f 0x97f
#define EVENT_FLAG_0x0960 0x960

// Black's, which event_work.c's code confirms: the flags that last a day (Black 2's end at 0xb03)
#define EVENT_FLAG_DAILY_0x0aa0 0xaa0
#define EVENT_FLAG_DAILY_0x0b5f 0xb5f
#define EVENT_FLAG_TEMP_0 0x4000

// Black's, read from the ROM by tools/black_constants.py
#define EVENT_FLAG_ARRIVED_NUVEMA_TOWN 0x9b0
#define EVENT_FLAG_ARRIVED_ACCUMULA_TOWN 0x9b1
#define EVENT_FLAG_ARRIVED_STRIATON_CITY 0x9b2
#define EVENT_FLAG_ARRIVED_NACRENE_CITY 0x9b3
#define EVENT_FLAG_ARRIVED_CASTELIA_CITY 0x9b4
#define EVENT_FLAG_ARRIVED_NIMBASA_CITY 0x9b5
#define EVENT_FLAG_ARRIVED_DRIFTVEIL_CITY 0x9b6
#define EVENT_FLAG_ARRIVED_MISTRALTON_CITY 0x9b7
#define EVENT_FLAG_ARRIVED_ICIRRUS_CITY 0x9b8
#define EVENT_FLAG_ARRIVED_OPELUCID_CITY 0x9b9
#define EVENT_FLAG_ARRIVED_LACUNOSA_TOWN 0x9ba
#define EVENT_FLAG_ARRIVED_POKEMON_LEAGUE 0x9bb
#define EVENT_FLAG_ARRIVED_BLACK_CITY_WHITE_FOREST 0x9bc
#define EVENT_FLAG_ARRIVED_UNDELLA_TOWN 0x9bd
#define EVENT_FLAG_ARRIVED_UNITY_TOWER 0x9be
#define EVENT_FLAG_ARRIVED_VICTORY_ROAD 0x9bf

// Black's, which BtlSetup_LoadGameData checks (Black 2's 0x96b)
#define EVENT_FLAG_0x096d 0x96d

// Black 2's, not checked against Black
#define EVENT_FLAG_CONTINUE_SCRIPT 0x966

#define EVENT_FLAG_RUNNING_SHOES 0x963

#endif // BWPORT_CONSTANTS_FLAGS_H
