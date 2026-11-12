// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/irc_link.h: the lines in use so far.
#ifndef BWPORT_FIELD_IRC_LINK_H
#define BWPORT_FIELD_IRC_LINK_H

#include "types.h"

// Black 2's, not checked against Black
// The frames the link sends between the two machines start with one of these. Any byte under IRC_CMD_MIN is a
// packet of data
#define IRC_CMD_MIN 0xf0

#define IRC_CMD_EMPTY 0xee
#define IRC_CMD_CLOSE 0xf4
#define IRC_CMD_RESEND 0xf6
#define IRC_CMD_ACCEPT 0xf8
#define IRC_CMD_REPLY 0xfa
#define IRC_CMD_PING 0xfc

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's IrcLink_Reset
void func_ov015_02179f74(u8 id);
// pokebw2's IrcLink_SetRecvCallback
void func_ov015_0217a584(void (*callback)(u8 *data, u8 size, u8 cmd, u8 id));
// pokebw2's IrcLink_Close
void func_ov015_0217a134(void);
// pokebw2's IrcLink_IsParent
BOOL func_ov015_0217a014(void);
// pokebw2's IrcLink_GetPeerId
u8 func_ov015_0217a5a4(void);
// pokebw2's IrcLink_Send
void func_ov015_0217a148(u8 *data, u8 size, u8 cmd);
// pokebw2's IrcLink_IsLinked
BOOL func_ov015_0217a024(void);
// pokebw2's IrcLink_Poll
BOOL func_ov015_0217a334(void);
// pokebw2's IrcLink_Ping
void func_ov015_02179fc4(void);

#endif // BWPORT_FIELD_IRC_LINK_H
