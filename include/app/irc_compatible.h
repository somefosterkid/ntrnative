// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/irc_compatible.h: the lines in use so
// far.
#ifndef BWPORT_APP_IRC_COMPATIBLE_H
#define BWPORT_APP_IRC_COMPATIBLE_H

#include "types.h"
#include "app/compatible_irc_sys.h"
#include "struct_decls.h"

typedef struct {
    GameSystem *gsys;
    CompatibleIrcSys *sys;
    CompatibleIrcProfile *profile;
    int result;
    u8 unk10;
    u32 unk14;
} IrcCompatibleParam2;

typedef struct {
    GameSystem *gsys;
    CompatibleIrcSys *sys;
    CompatibleIrcProfile *profile;
    u8 result;
} IrcCompatibleParam3;

// What each of the screens gets, and gives back
struct IrcCompatibleParam0 {
    GameSystem *gsys;
    CompatibleIrcSys *sys;
    // The screen's first or later visit: 0 the first, 1 afterwards and 2 after a visit to the last screen
    int visit;
    // What the screen chose
    int result;
    CompatibleIrcProfile *profile;
};

struct IrcCompatibleParam4 {
    GameSystem *gsys;
};

struct IrcCompatibleParam1 {
    GameSystem *gsys;
    CompatibleIrcSys *sys;
    CompatibleIrcProfile *profile;
    int result;
    u8 unk10;
    u8 unk11;
    u8 unk12;
};

// What the event gives the checker's proc
struct IrcCompatibleParam {
    GameSystem *gsys;
};

#endif // BWPORT_APP_IRC_COMPATIBLE_H
