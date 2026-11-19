#include "global.h"

#include "field_types_def.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01.h"
#include "overlay_01_022031C0.h"
#include "player_avatar.h"
#include "task.h"
#include "unk_02054648.h"
#include "unk_02062108.h"

void ov01_02206028(LocalMapObject *, LocalMapObject *);

u32 sub_0206234C(u32, u32);

BOOL ov01_02205F00(TaskManager *param_1);

BOOL ov01_02205F00(TaskManager *param_1) {
    FieldSystem *fieldSystem;
    s32 *ptr;
    u32 *puVar1;
    s32 iVar2;
    LocalMapObject *pLVar3;
    u32 uVar4;
    BOOL BVar5;

    fieldSystem = TaskManager_GetFieldSystem(param_1);
    ptr = TaskManager_GetEnvironment(param_1);
    puVar1 = TaskManager_GetStatePtr(param_1);
    switch (*puVar1) {
    case 0:
        sub_020659CC(fieldSystem->followMon.mapObject);
        sub_0205F484(fieldSystem->followMon.mapObject);
        *puVar1 = *puVar1 + 1;
        break;
    case 1:
        BVar5 = MapObject_AreBitsSetForMovementScriptInit(fieldSystem->followMon.mapObject);
        if (BVar5 == 1) {
            pLVar3 = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
            ov01_02206028(pLVar3, fieldSystem->followMon.mapObject);
            *puVar1 = *puVar1 + 1;
        }
        break;
    case 2:
        BVar5 = MapObject_AreBitsSetForMovementScriptInit(fieldSystem->followMon.mapObject);
        if (BVar5 == 1) {
            pLVar3 = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
            uVar4 = MapObject_GetFacingDirection(pLVar3);
            MapObject_SetFacingDirection(fieldSystem->followMon.mapObject, uVar4);
            *puVar1 = *puVar1 + 1;
        }
        break;
    case 3:
        iVar2 = *ptr;
        *ptr = iVar2 + 1;
        if (10 < iVar2 + 1) {
            *puVar1 = *puVar1 + 1;
        }
        break;
    case 4:
        BVar5 = MapObject_AreBitsSetForMovementScriptInit(fieldSystem->followMon.mapObject);
        if (BVar5 == 1) {
            sub_0206A040(fieldSystem->followMon.mapObject, 0);
            pLVar3 = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
            uVar4 = MapObject_GetFacingDirection(pLVar3);
            uVar4 = sub_0206234C(uVar4, 0x34);
            MapObject_SetHeldMovement(fieldSystem->followMon.mapObject, uVar4);
            *puVar1 = *puVar1 + 1;
        }
        break;
    case 5:
        BVar5 = MapObject_AreBitsSetForMovementScriptInit(fieldSystem->followMon.mapObject);
        if (BVar5 == 1) {
            ov01_0220329C(fieldSystem->followMon.mapObject, 2);
            sub_0206A054(fieldSystem);
            *puVar1 = *puVar1 + 1;
        }
        break;
    case 6:
        Heap_Free(ptr);
        return 1;
    }
    return 0;
}
