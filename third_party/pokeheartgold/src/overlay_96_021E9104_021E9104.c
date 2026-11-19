#include "global.h"

#include "assert.h"
#include "overlay_96_sprite_res_man.h"
#include "unk_0200A090.h"

u8 ov96_021E9180(void *data, u32 offset);

u8 ov96_021E9104(Ov96SpriteResEntry *obj);

u8 ov96_021E9104(Ov96SpriteResEntry *obj) {
    NNSG2dCharacterData *charData;
    int size;
    u8 result;
    int y;
    void *raw;
    u32 v;
    u32 min;
    int x;

    charData = GF2DGfxResObj_GetCharDataPtr(obj->charRes);
    if (obj->desc.isLarge) {
        size = 8;
    } else {
        size = 4;
    }
    result = 0xFF;
    raw = charData->pRawData;
    for (y = 0; y < size; y++) {
        min = 0xFF;
        for (x = 0; x < size; x++) {
            v = ov96_021E9180(raw, (x + y * size) * 32);
            if (v != 0xFF && min > v) {
                min = v;
            }
        }
        if (min != 0xFF) {
            result = min + y * 8;
            break;
        }
    }
    GF_ASSERT(result != 0xFF);
    return result;
}
