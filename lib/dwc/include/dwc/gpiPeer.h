// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gpiPeer.h, with Black's names.
#ifndef BWPORT_DWC_GPIPEER_H
#define BWPORT_DWC_GPIPEER_H

#include "dwc/gpi.h"
#include "dwc/gsUdpEngine.h"
#include "dwc/nonport.h"

// The peers of GameSpy's GP (gpiPeer.c), in overlay 11: the buddies that messages go to directly, through the Udp
// Engine (dwc/gsUdpEngine.h), once they have authorized each other with the signatures the server gives (see dwc/gp.h
// for the names)

void gpiDestroyPeer(GPConnection *connection, GPIPeer *peer);
GPResult gpiProcessPeers(GPConnection *connection);
GPIPeer *gpiGetConnectedPeer(const GPConnection *connection, int profileid);
GPIPeer *gpiAddPeer(GPConnection *connection, int profileid, GPIBool initiate);
GPResult gpiPeerGetSig(GPConnection *connection, GPIPeer *peer);
GPResult gpiPeerStartConnect(GPConnection *connection, GPIPeer *peer);
GPResult gpiPeerAddMessage(GPConnection *connection, GPIPeer *peer, int type, const char *message);
GPResult gpiPeerStartTransferMessage(GPConnection *connection, GPIPeer *peer, int type,
                                     const GPITransferID *transferID);
GPResult gpiPeerFinishTransferMessage(GPConnection *connection, GPIPeer *peer, const char *message, int len);
// The Udp Engine's callbacks (dwc/gsUdpEngine.h), with the connection as their user data
void func_ov009_0218f5f4(unsigned int ip, unsigned short port, GSUdpCloseReason reason, void *userData);
void func_ov009_0218f65c(unsigned int ip, unsigned short port, unsigned char *data, int len, gsi_bool reliable,
                            void *userData);
void func_ov009_0218f7b8(unsigned int ip, unsigned short port, GSUdpErrorCode error, gsi_bool rejected,
                             void *userData);
void func_ov009_0218f880(unsigned int ip, unsigned short port, unsigned int latency, void *userData);
void func_ov009_0218f884(GPIPeer *peer, GPIPeerOp *operation);
void func_ov009_0218f900(GPIPeer *peer, GPIPeerOp *operation);

#endif // BWPORT_DWC_GPIPEER_H
