#include "global.h"
#include "union_room_manager.h"
struct UnionRoomManager;

void sub_0205A034(struct UnionRoomManager *arg0, void *arg1, s32 arg2);

void sub_0205A034(UnionRoomManager *arg0, void *arg1, s32 arg2) {
    *((void **)&arg0->task) = arg1;
    *((s32 *)&arg0->timer) = arg2;
}
