// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/pair_sys.h: the lines in use so far.
#ifndef BWPORT_FIELD_PAIR_SYS_H
#define BWPORT_FIELD_PAIR_SYS_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"

#include "struct_decls.h"
// Black 2's, not checked against Black
// What GameData keeps of the follower (GetFieldFollowerCfg)
struct FieldFollowWk {
    u16 heapId;
    // The follower's index, 0xff for none
    u8 index;
    // The actor ID the follower had before it followed
    u8 originActorId;
    u16 objCode : 15;
    // Whether the follower stays after a map change
    u16 isPersistent : 1;
    u16 scrId;
    // The trainer who battles alongside the player
    u16 trainerId;
};

#endif // BWPORT_FIELD_PAIR_SYS_H
