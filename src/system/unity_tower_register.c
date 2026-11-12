// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/unity_tower_register.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/unity_tower.h"
#include "save/player_info.h"

// Records the partners of trades in the Unity Tower's survey, a list of the latest 20 with at most 5 from one country.
// The file's name and the functions' are ours

// The visitors from one country that the list keeps
#define VISITORS_PER_COUNTRY_MAX 5

#define REGISTER_EXISTING 1
#define REGISTER_NEW 2

static u32 func_0202eb9c(UnityTowerSurveySave *save, UnityTowerVisitor *visitor);
static u32 func_0202ec24(UnityTowerVisitor *visitors);
static u32 func_0202ec44(UnityTowerVisitor *visitors, u32 country);
static u32 func_0202ec7c(UnityTowerVisitor *visitors, u32 country);
static u32 func_0202ecb0(UnityTowerVisitor *visitors, UnityTowerVisitor *visitor);
static void func_0202ed00(UnityTowerVisitor *visitors, unsigned long index);
static void func_0202ed60(UnityTowerSurveySave *save, UnityTowerVisitor *visitor, BOOL scriptFlag);
static void func_0202ed90(UnityTowerVisitor *visitors, UnityTowerVisitor *visitor, BOOL scriptFlag);

// pokebw2's UnityTowerSurvey_RegisterTrade
u32 func_0202eb7c(UnityTowerSurveySave *save, UnityTowerVisitor *visitor) {
    if (UnityTowerVisitor_GetCountry(&visitor->info) == 0) {
        return 0;
    }
    return func_0202eb9c(save, visitor);
}

// pokebw2's UnityTowerSurvey_RegisterVisitor
static u32 func_0202eb9c(UnityTowerSurveySave *save, UnityTowerVisitor *visitor) {
    UnityTowerVisitor *visitors = func_0200974c(save);
    u32 index = func_0202ecb0(visitors, visitor);
    u32 country;

    if (index != UNITY_TOWER_VISITOR_MAX) {
        BOOL scriptFlag = visitors[index].scriptFlag ? TRUE : FALSE;

        func_0202ed00(visitors, index);
        func_0202ed60(save, visitor, scriptFlag);
        return REGISTER_EXISTING;
    }
    country = UnityTowerVisitor_GetCountry(&visitor->info);
    if (func_0202ec44(visitors, country) < VISITORS_PER_COUNTRY_MAX) {
        if (func_0202ec24(visitors) >= UNITY_TOWER_VISITOR_MAX) {
            func_0202ed00(visitors, 0);
        }
        func_0202ed60(save, visitor, FALSE);
    } else {
        func_0202ed00(visitors, func_0202ec7c(visitors, country));
        func_0202ed60(save, visitor, FALSE);
    }
    return REGISTER_NEW;
}

// pokebw2's UnityTowerVisitors_Count
static u32 func_0202ec24(UnityTowerVisitor *visitors) {
    u32 i;

    for (i = 0; i < UNITY_TOWER_VISITOR_MAX; i++) {
        if (!visitors[i].valid) {
            break;
        }
    }
    return i;
}

// pokebw2's UnityTowerVisitors_CountCountry
static u32 func_0202ec44(UnityTowerVisitor *visitors, u32 country) {
    u32 count = 0;
    u32 i;
    u32 visitorCountry;

    for (i = 0; i < UNITY_TOWER_VISITOR_MAX; i++) {
        if (!visitors[i].valid) {
            break;
        }
        visitorCountry = UnityTowerVisitor_GetCountry(&visitors[i].info);
        if (visitorCountry == country) {
            count++;
        }
    }
    if (count > VISITORS_PER_COUNTRY_MAX) {
        count = VISITORS_PER_COUNTRY_MAX;
    }
    return count;
}

// pokebw2's UnityTowerVisitors_FindCountry
static u32 func_0202ec7c(UnityTowerVisitor *visitors, u32 country) {
    u32 i;
    u32 visitorCountry;

    for (i = 0; i < UNITY_TOWER_VISITOR_MAX; i++) {
        if (!visitors[i].valid) {
            break;
        }
        visitorCountry = UnityTowerVisitor_GetCountry(&visitors[i].info);
        if (visitorCountry == country) {
            return i;
        }
    }
    return UNITY_TOWER_VISITOR_MAX;
}

// pokebw2's UnityTowerVisitors_Find
static u32 func_0202ecb0(UnityTowerVisitor *visitors, UnityTowerVisitor *visitor) {
    u32 id = getIDAsUInt(&visitor->info);
    u32 country = UnityTowerVisitor_GetCountry(&visitor->info);
    u32 visitorId;
    u32 visitorCountry;
    u32 i;

    for (i = 0; i < UNITY_TOWER_VISITOR_MAX; i++) {
        if (!visitors[i].valid) {
            break;
        }
        visitorId = getIDAsUInt(&visitors[i].info);
        if (visitorId == id) {
            visitorCountry = UnityTowerVisitor_GetCountry(&visitors[i].info);
            if (visitorCountry == country) {
                return i;
            }
        }
    }
    return UNITY_TOWER_VISITOR_MAX;
}

// Closes the gap the visitor leaves, and clears the entries left at the end
// pokebw2's UnityTowerVisitors_Remove; index is the SDK's u32 (unsigned long): this tree's u32, an unsigned int,
// gives another loop
static void func_0202ed00(UnityTowerVisitor *visitors, unsigned long index) {
    if (index >= UNITY_TOWER_VISITOR_MAX) {
        return;
    }
    for (; index < UNITY_TOWER_VISITOR_MAX - 1; index++) {
        visitors[index] = visitors[index + 1];
        if (!visitors[index + 1].valid) {
            index++;
            break;
        }
    }
    for (; index < UNITY_TOWER_VISITOR_MAX; index++) {
        visitors[index].valid = FALSE;
    }
}

// pokebw2's UnityTowerSurvey_AddVisitor
static void func_0202ed60(UnityTowerSurveySave *save, UnityTowerVisitor *visitor, BOOL scriptFlag) {
    func_0202ed90(func_0200974c(save), visitor, scriptFlag);
    if (func_02009750(save, UnityTowerVisitor_GetCountry(&visitor->info))) {
        func_020095e8(save);
    }
}

// pokebw2's UnityTowerVisitors_Add
static void func_0202ed90(UnityTowerVisitor *visitors, UnityTowerVisitor *visitor, BOOL scriptFlag) {
    u32 index = func_0202ec24(visitors);

    if (index < UNITY_TOWER_VISITOR_MAX) {
        if (scriptFlag) {
            visitor->scriptFlag = TRUE;
        } else {
            visitor->scriptFlag = FALSE;
        }
        visitor->valid = TRUE;
        visitors[index] = *visitor;
    }
}
