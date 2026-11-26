#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "sys_task.h"
#include "task.h"
#include "unk_0205C22C.h"

void sub_0205AEA8(UnkStruct_0205C22C *, struct UnionRoomManager *, MapObjectManager *, struct SavePalPad *);
void sub_0205AF78(UnkStruct_0205C22C *, MapObjectManager *);

void sub_0205AD60(SysTask *task, void *param1);

void sub_0205AD60(SysTask *task, void *param1) {
    UnkStruct_0205C22C *env = param1;
    struct UnionRoomManager *manager = env->unionRoomMan;

    if (!FieldSystem_TaskIsRunning(env->fieldSystem)) {
        env->playerAvatar = env->fieldSystem->playerAvatar;
        sub_0205AEA8(env, manager, env->fieldSystem->mapObjectManager, env->unk_474);
        sub_0205AF78(env, env->fieldSystem->mapObjectManager);
    }
}
