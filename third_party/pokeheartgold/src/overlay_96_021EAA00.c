#include "global.h"

#include "error_handling.h"
#include "sys_task_api.h"
#include "overlay_96_sprite_res_man.h"

BOOL ov96_021EAA00(Ov96SpriteSet *set) {
    return set->loaded;
}

Ov96SpriteSetElem *ov96_021EAA04(Ov96SpriteSet *set, s32 index) {
    if (index >= set->count) {
        GF_AssertFail();
    }
    return &set->elems[index];
}

Ov96SpriteResEntry *ov96_021EAA20(Ov96SpriteSetElem *elem) {
    return elem->entry;
}

void ov96_021EAA24(SysTask *task, void *data) {
    Ov96SpriteSet *set = data;
    u32 i;
    s32 priority;

    if (ov96_021E8B88(set->man) != 0) {
        i = 0;
        priority = set->drawPriority;
        if (set->count > 0) {
            do {
                set->elems[i].entry = ov96_021E8B8C(set->man, i);
                ov96_021EABA8(&set->elems[i], priority);
                priority += 1;
                set->elems[i].unk_28 = ov96_021E90FC(set->elems[i].entry);
                i += 1;
            } while ((s32)i < set->count);
        }
        set->loaded = 1;
        SysTask_Destroy(task);
    }
}
