#include "global.h"

#include "friend_group.h"
#include "string_util.h"
#include "unk_0202C730.h"


s32 sub_0202C8C4(void *arg0);

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
BOOL StringNotEqualN(const charcode_t *str1, const charcode_t *str2, u32 num);
BOOL sub_0202C8E4(const RecordMixedRNG *entry1, const RecordMixedRNG *entry2);

BOOL sub_0202C88C(SAV_FRIEND_GRP *group, u16 *a1) {
    BOOL BVar1;
    s32 iVar2;

    if (*a1 == 0xffff) {
        return 0;
    }
    iVar2 = 0;
    do {
        BVar1 = StringNotEqualN(a1, (u16 *)group, 8);
        if (BVar1 == 0) {
            return 1;
        }
        iVar2 = iVar2 + 1;
        group = (SAV_FRIEND_GRP *)((u8 *)group + 0x2c);
    } while (iVar2 < 6);
    return 0;
}

s32 sub_0202C8C4(void *arg0) {
    if (*(u16 *)arg0 == 0xFFFF) {
        return 1;
    }
    if (*(u16 *)((u8 *)arg0 + 0x10) == 0xFFFF) {
        return 1;
    }
    return 0;
}

BOOL sub_0202C8E4(const RecordMixedRNG *entry1, const RecordMixedRNG *entry2) {
    if (StringNotEqualN(entry1->playerName, entry2->playerName, 7 + 1)) {
        return 0;
    }

    if (StringNotEqualN(entry1->groupName, entry2->groupName, 7 + 1)) {
        return 0;
    }

    if (entry1->gender != entry2->gender) {
        return 0;
    }

    if (entry1->language != entry2->language) {
        return 0;
    }

    if (entry1->seed != entry2->seed) {
        return 0;
    }

    return 1;
}
