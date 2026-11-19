#include "global.h"

#include "approaching_trainer_data.h"
#include "map_object.h"
#include "sys_task.h"

enum ApproachingTrainerTaskState {
    STATE_UNPAUSE_MOVEMENT = 0,
    STATE_TRY_FACE_DIRECTION,
    STATE_CHECK_REVEAL_OR_FACE_DIRECTION,
    STATE_FACE_DIRECTION,
    STATE_WAIT_FACE_DIRECTION,
    STATE_UNK_05,
    STATE_UNK_06,
    STATE_REVEAL_TRAINER,
    STATE_WAIT_REVEAL_TRAINER,
    STATE_DELAY_CHECK_NEXT_TO_PLAYER,
    STATE_CHECK_NEXT_TO_PLAYER,
    STATE_STEP_TOWARDS_PLAYER,
    STATE_WAIT_STEP_TOWARDS_PLAYER,
    STATE_DELAY_NEXT_TO_PLAYER,
    STATE_TRY_PLAYER_FACE_TRAINER,
    STATE_WAIT_PLAYER_FACE_TRAINER,
    STATE_SWITCH_MOVEMENT_TYPE_NONE,
    STATE_DONE,
};

// sApproachingTrainerTask: one handler per ApproachingTrainerTaskState (was read as `extern u32` + state, a 4-byte
// stride the host's 8-byte function pointers do not have).
extern int (*const _020FE1A4[])(ApproachingTrainerData *data);

void sub_02064630(SysTask *task, void *inData);
int sub_0206464C(ApproachingTrainerData *data);

void sub_02064630(SysTask *task, void *inData) {
    ApproachingTrainerData *data = inData;
    int ret;

    do {
        ret = _020FE1A4[data->state](data);
    } while (ret == 1);
}

int sub_0206464C(ApproachingTrainerData *data) {
    LocalMapObject *mapObj = data->mapObj;

    if (MapObject_CheckSingleMovement(mapObj) == TRUE) {
        MapObject_UnpauseMovement(mapObj);
    }

    data->state = STATE_TRY_FACE_DIRECTION;
    return 1;
}
