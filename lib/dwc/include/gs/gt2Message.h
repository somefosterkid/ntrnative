// GameSpy GT2's message handling (gt2Message.c): the connection and host-error handlers gt2Socket.c calls.
#ifndef BWPORT_GS_GT2_MESSAGE_H
#define BWPORT_GS_GT2_MESSAGE_H

#include "types.h"
#include "gs/gt2.h"

GT2Bool gti2HandleConnectionReset(GT2Socket socket, u32 ip, u16 port);
BOOL gti2HandleHostUnreachable(GT2Socket socket, u32 ip, u16 port, GT2Bool reset);

GT2Bool gti2SendAccept(GT2Connection connection);
GT2Bool gti2SendReject(GT2Connection connection, const GT2Byte *message, int len);
GT2Bool gti2SendClosed(GT2Connection connection);
GT2Bool gti2SendClientChallenge(GT2Connection connection, const char challenge[GTI2_CHALLENGE_LEN]);
GT2Bool gti2SendClose(GT2Connection connection);
GT2Bool gti2ResendMessage(GT2Connection connection, GTI2OutgoingBufferMessage *message);
#endif
