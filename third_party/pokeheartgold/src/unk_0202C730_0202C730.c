#include "global.h"

#include "friend_group.h"
#include "pm_string.h"


enum RecordMixedRNGEntry {
    RECORD_MIXED_RNG_PLAYER_ORIGINAL,
    RECORD_MIXED_RNG_PLAYER_OVERRIDE,
    RECORD_MIXED_RNG_QUEUE_0,
    RECORD_MIXED_RNG_QUEUE_1,
    RECORD_MIXED_RNG_QUEUE_2,
    RECORD_MIXED_RNG_QUEUE_3,

    RECORD_MIXED_RNG_MAX,
};
typedef u16 charcode_t;
typedef struct RecordMixedRNG {
    charcode_t groupName[7 + 1];
    charcode_t playerName[7 + 1];
    u8 gender;
    u8 language;
    u16 dummy;
    u32 seed;
    u32 rand;
} RecordMixedRNG;
#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_0202C738(RecordMixedRNG *rngCollection, u16 srcEntry, enum RecordMixedRNGEntry destEntry);
#else
void sub_0202C738(RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry srcEntry, enum RecordMixedRNGEntry destEntry);
#endif

u32 Save_FriendGroup_sizeof(void) {
    return 0x108U;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_0202C738(RecordMixedRNG *rngCollection, u16 srcEntry, enum RecordMixedRNGEntry destEntry) {
#else
void sub_0202C738(RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry srcEntry, enum RecordMixedRNGEntry destEntry) {
#endif
    rngCollection[destEntry] = rngCollection[srcEntry];
}
