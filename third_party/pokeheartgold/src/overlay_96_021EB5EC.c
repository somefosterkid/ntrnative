#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96_sprite_mgr.h"
#include "sprite.h"
#include "unk_0200A090.h"

SpriteResource **ov96_021EB5EC(Ov96SpriteMgr *mgr, s32 resId, u32 type) {
    s32 i;
    Ov96SpriteMgrRes *p;
    if ((3 + 1) <= type) {
        GF_AssertFail();
    }
    i = 0;
    if (0 < mgr->param.maxRes) {
        p = mgr->res;
        do {
            if (resId == p->id) {
                return &mgr->res[i].res[type];
            }
            i = i + 1;
            p = p + 1;
        } while (i < mgr->param.maxRes);
    }
    GF_AssertFail();
    return NULL;
}

void ov96_021EB630(struct Ov96Obj *obj, u32 priority) {
    Sprite_SetDrawPriority(obj->sprite, priority);
}
