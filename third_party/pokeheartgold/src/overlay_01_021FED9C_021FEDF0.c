#include "global.h"
#include "simple3dmodel.h"
#include "overlay_01_simple_renderers.h"

#include "overlay_01_021F1348.h"

void sub_02069784(Simple3DModel *model);
void sub_020698D0(Simple3DAnimation *anim);

void ov01_021FEDF0(UnkStruct_ov5_021F23FC *param0) {
    sub_02069784(&param0->unk_04);
    sub_020698D0(&param0->unk_18);
}
