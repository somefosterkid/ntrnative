#include "global.h"

#include "field_system.h"
#include "union_room_manager.h"
#include "unk_02058AEC.h"

void sub_0205A034(struct UnionRoomManager *, void (*)(struct UnionRoomManager *), int);

void sub_0205ABBC(struct UnionRoomManager *);

void sub_02059EBC(struct UnionRoomManager *);

void sub_0205A410(int arg0, int arg1, void *arg2, void *arg3) {
    FieldSystem *fieldSystem = arg3;

    sub_0205A034(fieldSystem->unk80, sub_02059EBC, 2);
    sub_0205ABBC(fieldSystem->unk80);
}
