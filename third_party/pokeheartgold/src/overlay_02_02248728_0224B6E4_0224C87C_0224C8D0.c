#include "global.h"
struct FieldSystem;

#include "field_types_def.h"
#include "player_avatar.h"
#include "unk_02062108.h"
#include "overlay_02.h"

extern u32 ov02_022537B8;

u32 ov02_0224C8D0(u32 param_1, struct FieldSystem *param_2, FieldMoveTaskEnvironment *param_3);

u32 ov02_0224C8D0(u32 param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3) {
    BOOL BVar1;
    EventObjectMovementMan *pEVar2;
    s32 sVar3;
    Field3dObjectTask *iVar4;
    BVar1 = EventObjectMovementMan_IsFinish(param_3->playerMovement);
    if (BVar1 == 0) {
        return 0;
    }
    EventObjectMovementMan_Delete(param_3->playerMovement);
    pEVar2 = EventObjectMovementMan_Create(param_3->mapObject, (MovementScriptCommand *)(&ov02_022537B8));
    param_3->playerMovement = pEVar2;
    if (param_3->followMonActive != 0) {
        EventObjectMovementMan_Delete(param_3->followMonMovement);
        pEVar2 = EventObjectMovementMan_Create(param_2->followMon.mapObject, (MovementScriptCommand *)(&ov02_022537B8));
        param_3->followMonMovement = pEVar2;
    }
    sVar3 = PlayerAvatar_GetState(param_2->playerAvatar);
    if (sVar3 != 2) {
        if (!(param_3->followMonActive == 0)) {
            iVar4 = ov02_0224DDF4(param_3->fieldSystem);
            param_3->unk_2C = iVar4;
        } else {
            iVar4 = ov02_0224DDE0(param_3->fieldSystem);
            param_3->unk_2C = iVar4;
        }
    }
    param_3->state = (param_3->state) + 1;
    return 0;
}
