#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_022006A8.h"
#include "overworld_anim_manager.h"

int ov01_02200780(OverworldAnimManager *param0, void *param1);
void ov01_022007D0(OverworldAnimManager *param0, void *param1);

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_022006A8.s (same label), with the context size from the type, not the DS literal.
const OverworldAnimManagerFuncs ov01_02209308 = OVERWORLD_ANIM_MANAGER_FUNCS(UnkStruct_ov5_021F3FB8, ov01_02200780, ov01_022007D0, ov01_022007DC, ov01_022007F8);
#endif

void ov01_022007DC(OverworldAnimManager *manager, UnkStruct_ov5_021F3FB8 *work) {
    if (sub_0205F0A8(work->unk_10.unk_0C, work->unk_04, work->unk_08) == 0) {
        ov01_021F1640(manager);
    }
}
