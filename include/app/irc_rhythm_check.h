// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/irc_rhythm_check.h: the lines in use so
// far.
#ifndef BWPORT_APP_IRC_RHYTHM_CHECK_H
#define BWPORT_APP_IRC_RHYTHM_CHECK_H

#include "types.h"

// What the screen gives back in IrcCompatibleParam2's result
enum {
    IRC_RHYTHM_CHECK_RESULT_DONE,
    IRC_RHYTHM_CHECK_RESULT_CANCELLED,
    IRC_RHYTHM_CHECK_RESULT_INTERRUPTED,
};

#endif // BWPORT_APP_IRC_RHYTHM_CHECK_H
