// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/net_lower_data.h.
#ifndef BWPORT_GFL_NET_LOWER_DATA_H
#define BWPORT_GFL_NET_LOWER_DATA_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Sending data larger than a packet in pieces, a little each frame, to buffers the receiving machines set up

void func_0203e5f8(HeapID heapId, BOOL a1);
void func_0203e6c0(void);
// Starts sending, to the machines in dest, from the server's handle or this machine's
void func_0203e758(void *data, u32 size, int dest, BOOL fromServer);
// Sets up and frees the buffer for what a machine sends
void func_0203e808(u32 bufferSize, int netId, HeapID heapId, void *buffer);
void func_0203e8c8(int netId);
void *func_0203e96c(int netId);
u32 func_0203e9dc(int netId);
// Whether all of the data has been sent, and whether all of a machine's has arrived
BOOL func_0203ea54(void);
BOOL func_0203ea70(int netId);
// Sends the next piece each frame
void func_0203ea9c(void);
// The command handlers for the data and for its header
void *func_0203ec7c(int netId, void *work, int size);
void func_0203ecd8(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_0203ee18(int netId, int size, const void *data, void *work, NetHandle *handle);

#endif // BWPORT_GFL_NET_LOWER_DATA_H
