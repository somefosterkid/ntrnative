#include "global.h"

#include "billboard.h"
#include "overlay_01_unk_ov01_021fda14_work_a.h"
#include "overworld_anim_manager.h"

BOOL sub_02023DA4(Billboard *billboard);

void ov01_021FDB34(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work) {
    if (work->hasBillboard == 1) {
        sub_02023DA4(work->billboard);
    }
}
