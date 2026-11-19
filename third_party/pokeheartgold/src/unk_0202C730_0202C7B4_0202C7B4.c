#include "global.h"

#include "friend_group.h"

u32 Save_FriendGroup_GetGroupId(SAV_FRIEND_GRP *group, s32 a1);

u32 Save_FriendGroup_GetGroupId(SAV_FRIEND_GRP *group, s32 a1) {
    return group->groups[a1].unk_24;
}
