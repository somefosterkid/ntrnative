#include "global.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_68_move_relearner.h"
#include "pm_string.h"
#include "pokemon.h"

u16 ov68_021E6BEC(MoveRelearner *controller);
u16 ov68_021E6BFC(MoveRelearner *controller);

enum {
    MOVE_REMINDER_STR_ASK_TEACH_WHICH_TO_MON = 0,
    MOVE_REMINDER_STR_ASK_SHOULD_TEACH_MOVE,
    MOVE_REMINDER_STR_ASK_GIVE_UP_TEACHING_MON,
    MOVE_REMINDER_STR_LEARNED_MOVE_NO_FANFARE,
    MOVE_REMINDER_STR_ASK_MORE_THAN_FOUR_MOVES,
    MOVE_REMINDER_STR_1_2_AND_POOF,
    MOVE_REMINDER_STR_LEARNED_MOVE_FANFARE,
    MOVE_REMINDER_STR_ASK_STOP_TRYING_TO_TEACH_MOVE,
    MOVE_REMINDER_STR_MON_DID_NOT_LEARN_MOVE,
    MOVE_REMINDER_STR_PAYMENT,
    MOVE_REMINDER_STR_ASK_FORGET_THIS_MOVE,
    MAX_MOVE_REMINDER_STR
};

extern const u32 ov68_021E7DA4[2][MAX_MOVE_REMINDER_STR];

void ov68_021E6A2C(MoveRelearner *controller, u32 str);

void ov68_021E6A2C(MoveRelearner *controller, u32 str) {
    switch (str) {
    case MOVE_REMINDER_STR_ASK_TEACH_WHICH_TO_MON:
        BufferBoxMonNickname(controller->msgFormat, 0, Mon_GetBoxMon(controller->args->mon));
        break;

    case MOVE_REMINDER_STR_ASK_SHOULD_TEACH_MOVE:
        BufferMoveName(controller->msgFormat, 1, ov68_021E6BEC(controller));
        break;

    case MOVE_REMINDER_STR_ASK_GIVE_UP_TEACHING_MON:
        BufferBoxMonNickname(controller->msgFormat, 0, Mon_GetBoxMon(controller->args->mon));
        break;

    case MOVE_REMINDER_STR_LEARNED_MOVE_NO_FANFARE:
        BufferBoxMonNickname(controller->msgFormat, 0, Mon_GetBoxMon(controller->args->mon));
        BufferMoveName(controller->msgFormat, 1, ov68_021E6BEC(controller));
        break;

    case MOVE_REMINDER_STR_ASK_MORE_THAN_FOUR_MOVES:
        BufferBoxMonNickname(controller->msgFormat, 0, Mon_GetBoxMon(controller->args->mon));
        BufferMoveName(controller->msgFormat, 1, ov68_021E6BEC(controller));
        break;

    case MOVE_REMINDER_STR_1_2_AND_POOF:
        BufferBoxMonNickname(controller->msgFormat, 0, Mon_GetBoxMon(controller->args->mon));
        BufferMoveName(controller->msgFormat, 1, ov68_021E6BFC(controller));
        break;

    case MOVE_REMINDER_STR_LEARNED_MOVE_FANFARE:
        BufferBoxMonNickname(controller->msgFormat, 0, Mon_GetBoxMon(controller->args->mon));
        BufferMoveName(controller->msgFormat, 1, ov68_021E6BEC(controller));
        break;

    case MOVE_REMINDER_STR_ASK_STOP_TRYING_TO_TEACH_MOVE:
        BufferMoveName(controller->msgFormat, 1, ov68_021E6BEC(controller));
        break;

    case MOVE_REMINDER_STR_MON_DID_NOT_LEARN_MOVE:
        BufferBoxMonNickname(controller->msgFormat, 0, Mon_GetBoxMon(controller->args->mon));
        BufferMoveName(controller->msgFormat, 1, ov68_021E6BEC(controller));
        break;

    case MOVE_REMINDER_STR_PAYMENT:
        BufferPlayersName(controller->msgFormat, 2, controller->args->profile);
        break;

    case MOVE_REMINDER_STR_ASK_FORGET_THIS_MOVE:
        BufferMoveName(controller->msgFormat, 0, ov68_021E6BFC(controller));
        break;
    }

    String *string = NewString_ReadMsgData(controller->msgData, ov68_021E7DA4[controller->args->type][str]);
    StringExpandPlaceholders(controller->msgFormat, controller->string, string);
    String_Delete(string);
}
