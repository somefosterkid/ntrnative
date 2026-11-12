// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/net_define.h: the lines in use so
// far.
#ifndef BWPORT_SYSTEM_NET_DEFINE_H
#define BWPORT_SYSTEM_NET_DEFINE_H

#include "types.h"
#include "dwc/dwc.h"
#include "struct_decls.h"

#define FRIEND_CHECK_SAME 0

// The same profile under other data
#define FRIEND_CHECK_SAME_PROFILE 1

#define FRIEND_CHECK_NEW 2
#define FRIEND_CHECK_INVALID 3

// Looks for a friend key in the Friend Roster: index gets the friend's place, or the first free place, or -1
int func_0200f43c(WifiList *wifiList, u64 friendKey, int *index);
// Whether the friend data is new to the network's friend list; data of a known profile is replaced by the list's
BOOL func_0200f4e4(DWCFriendData *friendData);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Steps the network each frame, every other frame while the game command base is 0x35
void func_0200f2d4(void);

#endif // BWPORT_SYSTEM_NET_DEFINE_H
