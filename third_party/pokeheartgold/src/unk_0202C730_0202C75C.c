#include "global.h"

#include "friend_group.h"
#include "math_util.h"
#include "pm_string.h"

extern SAV_FRIEND_GRP *_021D2AF0;

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
void sub_0202C78C(RecordMixedRNG *rngCollection, u32 stepCount);

void Save_FriendGroup_Init(void *ptr) {
    SAV_FRIEND_GRP *savFriendGrp = ptr;
    s32 iVar1;
    FRIEND_GROUP *pFVar2;

    MIi_CpuClearFast(0, savFriendGrp, 0x108);
    iVar1 = 0;
    pFVar2 = savFriendGrp->groups;
    do {
        ((FRIEND_GROUP *)pFVar2->unk_0)->unk_0[0] = 0xffff;
        pFVar2->unk_10[0] = 0xffff;
        iVar1 = iVar1 + 1;
        pFVar2 = pFVar2 + 1;
    } while (iVar1 < 6);
    _021D2AF0 = savFriendGrp;
    return;
}

void sub_0202C78C(RecordMixedRNG *rngCollection, u32 stepCount) {
    for (u32 i = 0; i < RECORD_MIXED_RNG_MAX; i++) {
        for (u32 j = 0; j < stepCount; j++) {
            rngCollection[i].rand = PRandom(rngCollection[i].rand);
        }
    }
}
