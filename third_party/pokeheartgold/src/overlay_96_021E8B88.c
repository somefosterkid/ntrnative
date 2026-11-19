#include "global.h"

#include "error_handling.h"
#include "overlay_96_sprite_res_man.h"

BOOL ov96_021E8B88(Ov96SpriteResMan *man) {
    return man->unk_C;
}

Ov96SpriteResEntry *ov96_021E8B8C(Ov96SpriteResMan *man, u8 index) {
    if (index >= man->maxEntries) {
        GF_AssertFail();
    }
    return &man->entries[index];
}

Sprite *ov96_021E8BAC(Ov96SpriteResEntry *entry) {
    return entry->sprite;
}

Ov96SpriteResDesc *ov96_021E8BB0(Ov96SpriteResEntry *entry) {
    return &entry->desc;
}
