#include "global.h"

#include "friend_group.h"
#include "unk_0202C730.h"

s32 sub_0202C8C4(FRIEND_GROUP *);

BOOL sub_0202C860(SAV_FRIEND_GRP *group, u16 a1) {
    s32 iVar1;

    iVar1 = sub_0202C8C4(group->groups + a1);
    if (iVar1 == 0) {
        return 1;
    }
    return 0;
}
