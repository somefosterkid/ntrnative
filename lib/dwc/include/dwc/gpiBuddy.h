// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gpiBuddy.h, with Black's names.
#ifndef BWPORT_DWC_GPIBUDDY_H
#define BWPORT_DWC_GPIBUDDY_H

#include "dwc/gpi.h"
#include "dwc/nonport.h"

// The buddy messages and the buddy list of GameSpy's GP (gpiBuddy.c), in overlay 11 (see dwc/gp.h for the names)

// Handle the server's "\\bm\\" buddy messages, "\\bsi\\" status infos and "\\bdy\\" buddy list
GPResult gpiProcessRecvBuddyMessage(GPConnection *connection, const char *input);
GPResult func_ov009_02187d60(GPConnection *connection, const char *input);
GPResult func_ov009_02188638(GPConnection *connection, const char *input);
// Send a buddy message through the server, or to the buddy's peer when it has one (GP_DONT_ROUTE, 0xb00, never
// through the server); peerOp is the operation that waits for the peer's answer, if any
GPResult gpiSendServerBuddyMessage(GPConnection *connection, int profileid, int type, const char *message);
GPResult gpiSendBuddyMessage(GPConnection *connection, int profileid, int type, const char *message, int sendOption,
                             GPIPeerOp *peerOp);
// Answer a peer's request for the keys of our status, and handle its reply to ours
GPResult func_ov009_02188b0c(GPConnection *connection, GPIPeer *peer);
GPResult func_ov009_02188ba0(GPConnection *connection, GPIPeer *peer, char *buffer);
// Authorize a buddy request, and delete a buddy, telling the server if asked to
GPResult gpiAuthBuddyRequest(GPConnection *connection, GPProfile profile);
GPResult gpiDeleteBuddy(GPConnection *connection, GPProfile profile, GPIBool sendServerRequest);
// Sets whether a profile's buddy was deleted
int func_ov009_02189300(GPConnection *connection, int profile, gsi_bool deleted);

#endif // BWPORT_DWC_GPIBUDDY_H
