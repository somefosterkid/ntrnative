#include "global.h"
struct FieldSystem;

#include "map_object.h"
#include "field_system.h"
#include "follow_mon_interact_work.h"

void ov02_0224F728(struct FieldSystem *arg0, FollowMonInteractWork *arg1);

void ov02_0224F728(FieldSystem *arg0, FollowMonInteractWork *arg1) {
    u32 v0;

    v0 = MapObject_GetFacingDirection(arg0->followMon.mapObject);
    switch (v0) {
    case 0:
        arg1->unk_17 = 3;
        return;
    case 1:
        arg1->unk_17 = 4;
        return;
    case 2:
        arg1->unk_17 = 2;
        return;
    case 3:
        arg1->unk_17 = 1;
        return;
    default:
        arg1->unk_17 = 0;
        return;
    }
}
