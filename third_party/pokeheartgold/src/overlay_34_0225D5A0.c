#include "global.h"

#include "field_types_def.h"
#include "overlay_01.h"
#include "sprite.h"
#include "task.h"
#include "unk_02009D48.h"
#include "overlay_34.h"

void ov34_0225D5A0(SysTask *task, void *data) {
    Ov34Work *work = data;
    FieldSystem *fieldSystem;
    BOOL BVar1;
    BgConfig *bgConfig;

    fieldSystem = work->fieldSystem;
    bgConfig = work->bgConfig;

    if (fieldSystem->unk84 != NULL) {
        switch (work->state) {
        case 2:
            BVar1 = FieldSystem_TaskIsRunning(fieldSystem);
            if (BVar1 == 0) {
                ov34_0225D558(work, bgConfig);
            }
            SpriteList_RenderAndAnimateSprites(work->spriteList);
            return;
        case 3:
            ov01_021F6A9C(fieldSystem, 0, NULL);
            work->state = 4;
            break;
        case 0:
        case 1:
        case 4:
        case 5:
            break;
        }
    }
    return;
}
