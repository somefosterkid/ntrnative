// GameSpy GT2's sockets (gt2Socket.c): the socket's new connection objects.
#ifndef BWPORT_GS_GT2_SOCKET_H
#define BWPORT_GS_GT2_SOCKET_H

#include "types.h"
#include "gs/gt2.h"

GT2Result gti2NewSocketConnection(GT2Socket socket, GT2Connection *connection, u32 ip, u16 port);

#endif
