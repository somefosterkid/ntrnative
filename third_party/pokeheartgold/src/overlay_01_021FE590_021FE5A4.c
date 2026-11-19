#include "global.h"
#include "simple3dmodel.h"
#include "overlay_01_simple_renderers.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_021FE590.h"
#include "sys_task.h"

void ov01_021FE5B4(BerryPatchEffectCounter *param0);
void ov01_021FE5BC(BerryPatchEffectCounter *param0);

void ov01_021FE5A4(void *a0) {
    ov01_021FE61C(a0);
    ov01_021F1448(a0);
}

void ov01_021FE5B4(BerryPatchEffectCounter *param0) {
    param0->counter++;
}

void ov01_021FE5BC(BerryPatchEffectCounter *param0) {
    param0->counter--;

    ((param0->counter >= 0) ? (void)0 : GF_AssertFail());
}
