// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/net_whpipe.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_GFL_NET_WHPIPE_H
#define BWPORT_GFL_NET_WHPIPE_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/net.h"

// The wireless pipe's state machine: its work begins with the state function that runs next, which takes the work
typedef void (*NetWhpipeStateFunc)(void *work);

typedef struct {
    NetWhpipeStateFunc state;
} NetWhpipeState;

// Stops the beacon when it was told to lock
void func_ov014_0217807c(void);

// pokebw2's NetWhpipe_SetState
// Sets the state that the pipe's work runs next, as the pipe's files do to move on
void func_ov014_0217727c(NetWhpipeState *work, NetWhpipeStateFunc state);
BOOL func_ov014_0217731c(const void *beacon);
BOOL func_ov014_021773e0(const void *beacon);
void func_ov014_02177f48(void *arg);
// What the device table's functions in net_devwireless.c are built on
BOOL func_ov014_02177280(HeapID heapId, WHCallback callback, void *work, BOOL flag);
void func_ov014_02177310(BOOL (*callback)(u16 netId, u8 *data, u16 size));
void func_ov014_02177560(void);
BOOL func_ov014_021775bc(BOOL a0);
BOOL func_ov014_021776c4(void);
void func_ov014_0217796c(u16 connectBits);
BOOL func_ov014_021779f4(void);
BOOL func_ov014_02177aa8(void);
BOOL func_ov014_02177aec(void);
void func_ov014_02177b0c(int value);
// The beacons that deliveries come by
void *func_ov014_02177b2c(int index);
u8 *func_ov014_02177b58(int index);
// A beacon the scan found, net_whpipe.c's layout
typedef struct NetWhpipeBeacon NetWhpipeBeacon;
// Whether the beacon is of this game, in a mode of flags, and its game's data, with the base of its commands
BOOL func_ov014_02177494(const NetWhpipeBeacon *beacon, u32 flags);
u8 *func_ov014_02177bfc(NetWhpipeBeacon *beacon, u8 *gameCommandBase);

// The game service ID of a beacon the scan found
u8 func_ov014_02177b80(int index);
void func_ov014_02177bac(int index);
void func_ov014_02177bcc(void);
// Whether the Funfest's beacon updates are sent (WH_GetSystemState returns 2)
BOOL func_ov014_02177c1c(void);
BOOL func_ov014_02177c30(u8 *data, int size, BOOL (*callback)(BOOL ok));
BOOL func_ov014_02177d0c(BOOL a0);
BOOL func_ov014_02177e20(BOOL a0, const u8 *mac, int index, void (*callback)(void));
BOOL func_ov014_02177eb0(WHCallback callback);
void func_ov014_02177edc(BOOL a0);
BOOL func_ov014_02178034(void);
void func_ov014_021780c4(int index);
void func_ov014_021780e4(int unused);
// Sets what the scan calls with each beacon it finds and the game's argument
void func_ov014_02178118(void (*callback)(void *arg, const void *beacon, u8 gameCommandBase, u8 childCount), void *arg);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Sends the game's beacon
void func_ov014_0217778c(void);
BOOL func_ov014_02177a94(void);
int func_ov014_02177c3c(void);
int func_ov014_02177c44(void);
u8 func_ov014_02178098(u8 index);
// The Funfest's beacon
void func_ov014_021780fc(u32 enabled);
void func_ov014_02177bf4(int index);
BOOL func_ov014_02177c08(void);

// The game service ID of a beacon the scan found

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code


#endif // BWPORT_GFL_NET_WHPIPE_H
