#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01_021FCD2C.h"
#include "unk_02005D10.h"
#include "unk_02062108.h"
#include "overlay_02.h"

extern u32 ov02_022537DC;

u32 ov02_0224C87C(u32 param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3, u32 param_4);

u32 ov02_0224C87C(u32 param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3, u32 param_4) {
    SysTask *iVar1;
    EventObjectMovementMan *pEVar2;
    iVar1 = ov01_021FCD2C(param_2, HEAP_ID_FIELD1);
    param_3->cameraTask = iVar1;
    ov01_021FCD8C(iVar1, 1, 0xfff6a000, 0xf);
    pEVar2 = EventObjectMovementMan_Create(param_3->mapObject, (MovementScriptCommand *)(&ov02_022537DC));
    param_3->playerMovement = pEVar2;
    if (param_3->followMonActive != 0) {
        pEVar2 = EventObjectMovementMan_Create(param_2->followMon.mapObject, (MovementScriptCommand *)(&ov02_022537DC));
        param_3->followMonMovement = pEVar2;
    }
    param_3->state = param_3->state + 1;
    PlaySE(0x602);
    return 0;
}
