#include "global.h"

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
BOOL sub_0202C8E4(const RecordMixedRNG *entry1, const RecordMixedRNG *entry2);

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL sub_0202C878(const RecordMixedRNG *rngCollection, u16 entry);
#else
BOOL sub_0202C878(const RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL sub_0202C878(const RecordMixedRNG *rngCollection, u16 entry) {
#else
BOOL sub_0202C878(const RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry) {
#endif
    return sub_0202C8E4(&rngCollection[RECORD_MIXED_RNG_PLAYER_OVERRIDE], &rngCollection[entry]);
}
