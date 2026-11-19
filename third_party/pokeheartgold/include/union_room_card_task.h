#ifndef POKEHEARTGOLD_UNION_ROOM_CARD_TASK_H
#define POKEHEARTGOLD_UNION_ROOM_CARD_TASK_H

#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_trainer_card.h"
#include "pm_string.h"
#include "trainer_case.h"

// Environment of the union room "<player> shows a trainer card" task: sub_02059D44 allocates it (the peer's net id picks
// the card) and sub_02059B64 runs it (message, fade, then TrainerCard_LaunchApp on the copy of the card).
typedef struct UnionRoomPeerCardTask {
    String *message;
    String *formattedMessage;
    Window window;
    MessageFormat *msgFormat;
    MsgData *msgData;
    u32 printerId;
    u32 netId;
    u32 state;
    TrainerCardAppArgs args;
} UnionRoomPeerCardTask;

// sizeof(UnionRoomPeerCardTask) == 0x6a8 on the DS

// Environment of the union room trainer card view task (sub_02069464 allocates it, sub_02069498 runs it): shows the card
// sub_0205ABD8 sent for.
typedef struct UnionRoomCardViewTask {
    int state;
    TrainerCase *card;
    TrainerCardAppArgs args;
} UnionRoomCardViewTask;

// sizeof(UnionRoomCardViewTask) == 0x684 on the DS

#endif // POKEHEARTGOLD_UNION_ROOM_CARD_TASK_H
