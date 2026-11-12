// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/irc_sync_check.h: the lines in use so
// far.
#ifndef BWPORT_APP_IRC_SYNC_CHECK_H
#define BWPORT_APP_IRC_SYNC_CHECK_H

#include "types.h"

// What the screen gives back in IrcCompatibleParam1's result
enum {
    IRC_SYNC_CHECK_RESULT_DONE,
    IRC_SYNC_CHECK_RESULT_CANCELLED,
    IRC_SYNC_CHECK_RESULT_INTERRUPTED,
};

#endif // BWPORT_APP_IRC_SYNC_CHECK_H
