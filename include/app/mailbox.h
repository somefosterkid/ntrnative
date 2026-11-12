// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/mailbox.h: the lines in use so far.
#ifndef BWPORT_APP_MAILBOX_H
#define BWPORT_APP_MAILBOX_H

#include "types.h"
#include "gfl/overlay.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
#define OVERLAY_MAILBOX OVERLAY_ID(215)

extern const GameProcFunctions data_ov136_021c78a4;

struct MailboxProcessData {
    GameData *gameData;
    u32 result;
};

#endif // BWPORT_APP_MAILBOX_H
