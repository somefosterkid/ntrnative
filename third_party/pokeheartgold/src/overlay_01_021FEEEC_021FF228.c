#include "global.h"

#include "billboard.h"
#include "overlay_01_021FEEEC.h"
#include "overworld_anim_manager.h"

BOOL sub_02023DA4(Billboard *billboard);

void ov01_021FF228(OverworldAnimManager *manager, FieldEffectAnimWork *work) {
    sub_02023DA4(work->billboard);
}
