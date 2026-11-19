#include "global.h"

#include "save.h"

struct RecordMixedRNG;
struct WifiBattleTowerRecord;
u32 sub_020674B0(u32 param0);
typedef struct RecordMixedRNG RecordMixedRNG;
RecordMixedRNG *Save_FriendGroup_Get(SaveData *saveData);
u32 sub_0202C7DC(RecordMixedRNG *rngCollection);
typedef struct WifiBattleTowerRecord WifiBattleTowerRecord;
void sub_0202D638(WifiBattleTowerRecord *record, u32 value);
WifiBattleTowerRecord *Save_FrontierData_Get(SaveData *saveData);
u32 sub_020674B0(u32 rng);

u32 sub_020674A4(u32 rng);
u32 sub_020674B0(u32 rng);
u32 sub_020674BC(SaveData *saveData);

u32 sub_020674A4(u32 rng) {
    return rng * 48828125L + 1;
}

u32 sub_020674B0(u32 rng) {
    return rng * 1566083941 + 1;
}

u32 sub_020674BC(SaveData *saveData) {
    u32 rng = sub_0202C7DC(Save_FriendGroup_Get(saveData));
    rng = sub_020674B0(rng);

    sub_0202D638(Save_FrontierData_Get(saveData), rng);

    return rng;
}
