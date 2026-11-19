#include "global.h"

#include "overlay_01_021F1ECC.h"
#include "script_manager.h"

s32 ov01_021F1F8C(FieldSystem *fieldSystem, LocalMapObject *object);

s32 ov01_021F2004(TaskManager *taskman, FieldSystem *fieldSystem, LocalMapObject *object) {
    if (ov01_021F1F8C(fieldSystem, object) == 0) {
        return 1;
    }
    StartScriptFromMenu(taskman, 0x271FU, NULL);
    return 0;
}
