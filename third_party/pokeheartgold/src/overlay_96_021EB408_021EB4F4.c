#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96_sprite_mgr.h"

struct Ov96Obj *ov96_021EB4F4(Ov96SpriteMgr *mgr, s32 resId, s32 group) {
    s32 i;
    s32 count;
    struct Ov96Obj *p;
    count = mgr->param.maxSprites;
    i = 0;
    if (count > 0) {
        p = mgr->slots;
        do {
            if (p->inUse == 0 && p->resId == resId && p->group == group) {
                p->inUse = 1;
                return p;
            }
            i++;
            p++;
        } while (i < count);
    }
    GF_AssertFail();
    return NULL;
}
