#include "global.h"

#include "billboard.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FF854.h"
#include "overworld_anim_manager.h"

BOOL sub_02023DA4(Billboard *billboard);

void ov01_021FFAB0(OverworldAnimManager *manager, FieldEffectAnimWork *work) {
    sub_02023DA4(work->billboard);
}
