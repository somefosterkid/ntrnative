// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/irc_compat_menu.h: the lines in use so
// far.
#ifndef BWPORT_APP_IRC_COMPAT_MENU_H
#define BWPORT_APP_IRC_COMPAT_MENU_H

#include "types.h"

// What the screen gives back in IrcCompatibleParam0's result
enum {
    IRC_COMPAT_MENU_RESULT_CHECK,
    IRC_COMPAT_MENU_RESULT_RANKING,
    IRC_COMPAT_MENU_RESULT_QUIT,
};

// IrcCompatibleParam0's visit
enum {
    IRC_COMPAT_VISIT_FIRST,
    IRC_COMPAT_VISIT_AGAIN,
    IRC_COMPAT_VISIT_AFTER_RESULT,
};

#endif // BWPORT_APP_IRC_COMPAT_MENU_H
