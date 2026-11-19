#include "global.h"

#include "sprite.h"
#include "overlay_41_unk_struct_ov22_0225_b85_c.h"

void ov41_02246360(void *, u32);

void ov41_02246374(u8 *, u32);

void ov41_02246388(u8 *, u32);

void ov41_0224639C(u8 *, u32);

void ov41_0224BD8C(u8 *param_1);

void ov41_0224BD8C(u8 *param_1) {
    ov41_02246360(param_1 + 0x14, 1000);
    ov41_02246374(param_1 + 0x14, 1000);
    ov41_02246388(param_1 + 0x14, 1000);
    ov41_0224639C(param_1 + 0x14, 1000);
    Sprite_Delete(((UnkStruct_ov22_0225B85C *)param_1)->unk_1FC);
    return;
}
