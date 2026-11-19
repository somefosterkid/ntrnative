#include "global.h"
#include "simple3dmodel.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_simple_renderers.h"

void sub_02069784(Simple3DModel *model);

void ov01_021FE2B8(UnkStruct_021F1CF8 *param0) {
    int v0;

    for (v0 = 0; v0 < 10; v0++) {
        sub_02069784(&param0->unk_04[v0]);
        sub_02069784(&param0->unk_CC[v0]);
    }
}
