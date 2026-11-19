#include "global.h"
#include "simple3dmodel.h"
#include "overlay_01_simple_renderers.h"
#include "overlay_01_021FE200.h"

void ov01_021FE230(UnkStruct_021F1CF8 *param0);

void *ov01_021FE200(FieldEffectManager *param0) {
    UnkStruct_021F1CF8 *v0 = ov01_021F1430(param0, (sizeof(UnkStruct_021F1CF8)), 0, 0);
    v0->unk_00 = param0;

    ov01_021FE230(v0);
    return v0;
}
