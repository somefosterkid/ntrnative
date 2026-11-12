// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/net_state.h, the lines in use so far.
#ifndef BWPORT_GFL_NET_STATE_H
#define BWPORT_GFL_NET_STATE_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/net.h"

// The state of the connection: a function for each state that runs every frame, for the parent and the children
// of a wireless connection, Wi-Fi, and ending the connection

// pokebw2's func_020421ac
void func_0203c554(BOOL a0);
BOOL func_0203c938(u8 a0);
u8 func_0203c990(void);
// Records a network error: its code, and type nonzero to set the network error
void func_0203c9ac(int code, u32 a1, u32 a2, int type);
GFLNetErrorInfo *func_0203ca98(void);

void func_0203bff0(void);
void func_0203c044(int netId, int size, const void *data, void *work, NetHandle *handle);
// Where the Wi-Fi connection is: 0 connecting, 1 connected, 3 to 5 failed, 2 otherwise
int func_0203c3ec(void);
void func_0203c5c8(int netId, int size, const void *data, void *work, NetHandle *handle);
u8 func_0203c5ec(void);
BOOL func_0203c600(void);
BOOL func_0203c628(void);
void func_0203c8e8(u8 a0);
void func_0203c900(int a0);
void func_0203c968(void);
BOOL func_0203cb00(void);
// Runs the state, each frame. Returns TRUE when the network is not running
BOOL func_0203b1d8(void);
void func_0203beac(int netId, int size, const void *data, void *work, NetHandle *handle);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Ends the network, calling callback when it has
void func_0203bf9c(void (*callback)(void *work));
void func_0203c010(void);
BOOL func_0203c2b0(int a0);
BOOL func_0203c304(void);
BOOL func_0203c35c(void);
void func_0203c3b4(void);
void func_0203c774(void (*callback)(void *work));
void func_0203c8a8(void);
void func_0203ca04(void);
// Starts the network, calling callback when it is ready
void func_0203afcc(HeapID heapId, void (*callback)(void *work));
// Connects to the parent with a MAC address, or becomes the parent
void func_0203b0c4(const u8 *mac, BOOL parent);
void func_0203b1b0(void);
void func_0203b2d0(HeapID unused);
void func_0203ba98(int a0, void (*callback)(void *work), int a2);
void func_0203bd30(int mode, int a1, const u8 *mac);
// Command handlers
void func_0203be5c(int netId, int size, const void *data, void *work, NetHandle *handle);

#endif // BWPORT_GFL_NET_STATE_H
