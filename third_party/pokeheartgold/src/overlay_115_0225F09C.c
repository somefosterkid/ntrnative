#include "global.h"

#include "sprite.h"
#include "overlay_115.h"


void ov115_0225F09C(UnkStruct_ov5_021E5128 *param0) {
    int v0;

    for (v0 = 0; v0 < 4; v0++) {
        Sprite_Delete(param0->unk_04[v0]);
    }
}
