// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/net_system.h.
#ifndef BWPORT_GFL_NET_SYSTEM_H
#define BWPORT_GFL_NET_SYSTEM_H

#include "types.h"
#include "gfl/heap.h"

// The packets between the machines: queuing commands to send, writing them into packets with a CRC and a sequence
// number, and reading the commands in the packets that arrive

BOOL func_02037700(int size, HeapID heapId);
int func_0203778c(int a0);
BOOL func_020377b8(int a0);
int func_02037894(int a0, int size, int a2, BOOL (*callback)(void));
BOOL func_02037928(int a0, int size);
BOOL func_020379c8(int size);
void func_02037a44(int a0);
int func_02037a6c(BOOL a0, const u8 *mac, int size, void (*callback)(void));
void func_02037b08(void);
// Forgets what has arrived from a machine, as when it disconnects
void func_02037be8(u32 netId);
// Sends and receives, each frame
BOOL func_02037c10(void);
void func_02037d78(void);
BOOL func_02038c18(int command, u8 *data, int size, u32 unused, int dest, int netId, BOOL noCopy);
int func_02038d30(void);
int func_02038d4c(void);
// Whether a machine is connected
BOOL func_02039160(u16 netId);
// The number of machines connected
int func_020392b8(void);
BOOL func_020392ec(void);
// This machine's network ID
u16 func_02039338(void);
// The connected machines but the parent, as bits
u32 func_020393a8(void);
BOOL func_020393d0(void);
BOOL func_02039414(void);
// Whether a command is waiting to be sent
BOOL func_02039468(u16 command, int dest);
BOOL func_020394c0(void);
void func_020394dc(u8 a0);
void func_020394f0(BOOL a0);
// Sets and clears the network error
void func_02039548(void);
void func_02039560(void);
void func_02039578(void);
BOOL func_02039594(void);
// A network ID, or GFL_NET_NETID_SERVER for one past the last machine
int func_020395e0(int netId);

void func_ov093_021d74e4(void);

#endif // BWPORT_GFL_NET_SYSTEM_H
