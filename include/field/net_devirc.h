// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/net_devirc.h: the lines in use so far.
#ifndef BWPORT_FIELD_NET_DEVIRC_H
#define BWPORT_FIELD_NET_DEVIRC_H

#include "types.h"
#include "gfl/net.h"

// pokebw2's NetDevIrc_SetTimeout
// How many frames the link may go without a peer before it reports an error, 0 for no limit
void func_ov015_0217998c(u32 frames);
// pokebw2's NetDevIrc_SetPeerFilter
// Sets the check that decides whether a peer's game is one to link with, called with this machine's game code and the
// peer's, or NULL to require that they are the same
void func_ov015_021799c0(BOOL (*filter)(int gameCode, int peerGameCode));
// pokebw2's NetDevIrc_IsConnected
// Whether the link is up, or still within its time to come up
BOOL func_ov015_02179d3c(void);

// pokebw2's NetDevIrc_GetTable
// No twin in pokebw2's C: this prototype is Black 2's, unchecked against Black's code
const GFLNetDevTable *func_ov015_021798d4(void);

#endif // BWPORT_FIELD_NET_DEVIRC_H
