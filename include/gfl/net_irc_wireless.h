// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/net_irc_wireless.h.
#ifndef BWPORT_GFL_NET_IRC_WIRELESS_H
#define BWPORT_GFL_NET_IRC_WIRELESS_H

#include "types.h"
#include "gfl/net.h"
#include "struct_decls.h"

// Connecting over infrared first and then wirelessly: the machines meet over infrared, swap MAC addresses, and then
// find each other's wireless beacons by them

// Starts it in place of a GFL_NET_TYPE 4 network, returning the init data to start the infrared network with
const GFLNetInitData *func_0203d8f0(const GFLNetInitData *pNetInit, void *work);
void func_0203d9ec(void);
void func_0203da20(void);
// Whether it is running
BOOL func_0203da4c(void);
void func_0203da68(void *a0);
void (*func_0203da7c(void (*a0)(void *work), void (*a1)(void *work, BOOL a1), void (*a2)(void *work)))(void *work);
// Adds a MAC address to the ones collected
void func_0203dac4(const u8 *mac);
// A collected MAC address
void func_0203dbb0(u8 *mac, int index);
// Steps it each frame
void func_0203dc70(void);
// The command handler for the MAC addresses another machine collected
void func_0203e484(int netId, int size, const void *data, void *work, NetHandle *handle);
BOOL func_0203e4e8(void);
void func_0203e538(int netId);
u8 func_0203e590(void);
void func_0203e5ac(int gameCommandBase);

#endif // BWPORT_GFL_NET_IRC_WIRELESS_H
