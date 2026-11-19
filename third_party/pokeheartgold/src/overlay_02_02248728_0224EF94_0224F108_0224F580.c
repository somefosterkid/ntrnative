#include "global.h"

#include "map_events.h"
#include "script_manager.h"
#include "follow_mon_interact_work.h"

void ov02_0224F580(FieldSystem *arg0, FollowMonInteractWork *arg1);

void ov02_0224F580(FieldSystem *arg0, FollowMonInteractWork *arg1) {
    FollowMonInteractWork *sp4;
    FieldSystem *sp0;
    s32 var_r6;
    u32 var_r4;
    u32 temp_r0;
    BgEvent *var_r5;
    sp0 = arg0;
    sp4 = arg1;
    var_r4 = 0;
    var_r5 = Field_GetBgEvents(arg0);
    temp_r0 = Field_GetNumBgEvents(arg0);
    if ((temp_r0 != 0) && (var_r5 != 0L)) {
        var_r6 = 0;
        if (((s32)temp_r0) > 0) {
            do {
                if ((var_r5->type == 2) && (FieldSystem_FlagCheck(sp0, HiddenItemScriptNoToFlagId(var_r5->scriptId)) == 0)) {
                    var_r4 += 1;
                }
                var_r6 += 1;
                var_r5 = (BgEvent *)((u8 *)var_r5 + 0x14);
            } while (var_r6 < ((s32)temp_r0));
        }
    }
    sp4->hiddenItemCount = var_r4;
}
