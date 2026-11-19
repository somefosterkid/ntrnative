#include "global.h"

#include "map_object.h"
#include "unk_020689C8.h"
#include "overlay_02_hmcut_in.h"

s32 ov02_0224B784(HMCutIn *);

s32 ov02_0224B938(HMCutIn *arg0);

s32 ov02_0224B938(HMCutIn *arg0) {
    MapObject_SetVisible(arg0->followMonObject, 1);
    sub_0205F484(arg0->followMonObject);
    ov02_0224B784(arg0);
    arg0->state = arg0->state + 1;
    return 1;
}

s32 ov02_0224B90C(HMCutIn *);

s32 ov02_0224B964(HMCutIn *arg0);

s32 ov02_0224B964(HMCutIn *arg0) {
    if (((FollowMonModelCtx *)sub_02068D74(arg0->unk_1F0))->finished == 1) {
        sub_02068B48(arg0->unk_1F0);
        ov02_0224B90C(arg0);
        arg0->state = arg0->state + 1;
        return 0;
    }
    return 0;
}
