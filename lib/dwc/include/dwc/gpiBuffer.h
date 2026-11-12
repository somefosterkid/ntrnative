// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gpiBuffer.h, with Black's names.
#ifndef BWPORT_DWC_GPIBUFFER_H
#define BWPORT_DWC_GPIBUFFER_H

#include "dwc/gpi.h"
#include "dwc/nonport.h"

// The texts of GameSpy's GP that grow as they are written (gpiBuffer.c), in overlay 11, and the sending and receiving
// of them over the connection manager's socket and to the peers (see dwc/gp.h for the names)

GPResult gpiAppendCharToBuffer(GPConnection *connection, GPIBuffer *outputBuffer, char c);
GPResult gpiAppendStringToBufferLen(GPConnection *connection, GPIBuffer *outputBuffer, const char *string,
                                    int stringLen);
GPResult gpiAppendStringToBuffer(GPConnection *connection, GPIBuffer *outputBuffer, const char *buffer);
GPResult gpiAppendIntToBuffer(GPConnection *connection, GPIBuffer *outputBuffer, int num);
// The ones to a peer, which go straight out when nothing waits before them and the Udp Engine has room
GPResult func_ov009_02189644(GPConnection *connection, GPIPeer *peer, char c);
int gpiSendOrBufferStringLen(GPConnection *connection, GPIPeer *peer, const char *string, int stringLen);
GPResult gpiSendOrBufferString(GPConnection *connection, GPIPeer *peer, char *string);
// id names the socket in the debug texts ("CM" the connection manager, "SM" the search manager)
GPResult gpiRecvToBuffer(GPConnection *connection, SOCKET sock, GPIBuffer *inputBuffer, int *bytesRead,
                         GPIBool *connClosed, char id[3]);
GPResult gpiSendFromBuffer(GPConnection *connection, SOCKET sock, GPIBuffer *outputBuffer, GPIBool *connClosed,
                           GPIBool clipSentData, char id[3]);
GPResult func_ov009_02189a7c(GPConnection *connection, unsigned int ip, unsigned short port, GPIBuffer *outputBuffer,
                             GPIBool *connClosed, GPIBool clipSentData);
// Reads a peer's message ("\\m\\<type>\\len\\<len>\\msg\\" and the text): message is NULL until a whole one is there
GPResult gpiReadMessageFromBuffer(GPConnection *connection, GPIBuffer *inputBuffer, char **message, int *type,
                                  int *len);
GPResult gpiClipBufferToPosition(GPConnection *connection, GPIBuffer *buffer);

#endif // BWPORT_DWC_GPIBUFFER_H
