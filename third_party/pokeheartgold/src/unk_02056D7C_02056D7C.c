#include "global.h"

#include "comm_player_manager.h"
#include "sys_task_api.h"
#include "unk_02035900.h"

s32 sub_02056EF4(void);

void sub_020572DC(SysTask *task, void *data);

s32 sub_02056D7C(CommPlayerManager *arg0, struct FieldSystem *arg1, s32 unused2) {
    int i;

    if (_021D41C4 != NULL) {
        return 0;
    }
    if (_021D41C4 == NULL) {
        _021D41C4 = arg0;
        MI_CpuFill8(arg0, 0U, sizeof(CommPlayerManager));
    }
    _021D41C4->fieldSystem = arg1;
    for (i = 0; i < 8; i++) {
        _021D41C4->positions[i].facing = -1;
        _021D41C4->positions[i].x = 0xFFFF;
        _021D41C4->positions[i].z = 0xFFFF;
        _021D41C4->positions[i].speed = 2;
        _021D41C4->hostPositions[i].facing = -1;
        _021D41C4->hostPositions[i].x = 0xFFFF;
        _021D41C4->hostPositions[i].z = 0xFFFF;
        _021D41C4->hostPositions[i].speed = 2;
        _021D41C4->moveEnabled[i] = 0;
        _021D41C4->inputLocked[i] = 0;
    }
    _021D41C4->localMoveEnabled = 0;
    _021D41C4->fieldResumed = 0;
    _021D41C4->task = SysTask_CreateOnMainQueue(sub_020572DC, _021D41C4->fieldSystem, 0xC8U);
    sub_020374E4();
    sub_02056EF4();
    return 1;
}
