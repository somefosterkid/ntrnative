#include "global.h"

#include "overlay_01_021F1ECC.h"
#include "unk_02005D10.h"
#include "unk_02062108.h"

void ov01_021F30F4(void *taskEnv);

BOOL sub_0205DE38(PlayerAvatar *playerAvatar);

u32 sub_0206234C(int targetDir, u32 movementAction);

BOOL ov01_021F1ECC(TaskManager *taskManager) {
    UnkStruct_ov01_021F1ECC *env;
    LocalMapObject *obj;
    int state;
    u32 uVar3;
    LocalMapObject *obj_00;
    FieldSystem *fieldSystem;

    env = TaskManager_GetEnvironment(taskManager);
    obj = PlayerAvatar_GetMapObject(env->playerAvatar);
    state = env->state;
    obj_00 = env->object;
    switch (state) {
    case 0:
        if (MapObject_AreBitsSetForMovementScriptInit(obj_00) != 0 && (MapObject_AreBitsSetForMovementScriptInit(obj) != 0 || sub_0205DE38(env->playerAvatar) != 0)) {
            uVar3 = sub_0206234C(env->dir, 8);
            MapObject_SetHeldMovement(obj_00, uVar3);
            uVar3 = sub_0206234C(env->dir, 0x20);
            MapObject_SetHeldMovement(obj, uVar3);
            PlaySE(0x626);
            env->state = env->state + 1;
        }
        break;
    case 1:
        if (MapObject_IsMovementPaused(obj_00) != 0 && MapObject_IsMovementPaused(obj) != 0) {
            MapObject_ClearHeldMovementIfActive(obj_00);
            MapObject_ClearHeldMovementIfActive(obj);
            env->state = env->state + 1;
        } else {
            break;
        }
    case 2:
        fieldSystem = env->fieldSystem;
        ov01_021F30F4(env);
        if (fieldSystem->location->mapId == 0xed) {
            return ov01_021F2004(taskManager, fieldSystem, obj_00);
        }
        return 1;
    }
    return 0;
}
