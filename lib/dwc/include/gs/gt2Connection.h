// GameSpy GT2's connections (gt2Connection.c): the new connection objects of the socket's attempts.
#ifndef BWPORT_GS_GT2_CONNECTION_H
#define BWPORT_GS_GT2_CONNECTION_H

#include "types.h"
#include "gs/gt2.h"

GT2Result gti2NewOutgoingConnection(GT2Socket socket, GT2Connection *connection, u32 ip, u16 port);
GT2Result gti2NewIncomingConnection(GT2Socket socket, GT2Connection *connection, u32 ip, u16 port);

#endif
