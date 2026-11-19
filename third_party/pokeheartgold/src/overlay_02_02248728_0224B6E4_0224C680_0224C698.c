#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "field_system.h"
#include "unk_02005D10.h"
#include "unk_02062108.h"
#include "overlay_01_021FCD2C.h"
#include "overlay_02.h"

extern u32 ov02_02253770;

u32 ov02_0224C698(u32 param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3, u32 param_4);

u32 ov02_0224C698(u32 param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3, u32 param_4) {
    SysTask *iVar1;
    EventObjectMovementMan *pEVar2;

    iVar1 = ov01_021FCD2C(param_2, HEAP_ID_FIELD1);
    param_3->cameraTask = iVar1;
    ov01_021FCD8C(iVar1, 1, 0xfff6a000, 0xf);
    pEVar2 = EventObjectMovementMan_Create(param_2->followMon.mapObject, (MovementScriptCommand *)&ov02_02253770);
    param_3->followMonMovement = pEVar2;
    param_3->state = param_3->state + 1;
    PlaySE(0x64d);
    return 0;
}
