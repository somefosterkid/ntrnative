// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/dwc_rap.h, the lines in use so far.
#ifndef BWPORT_GFL_DWC_RAP_H
#define BWPORT_GFL_DWC_RAP_H

#include "types.h"
#include "dwc/dwc.h"
#include "gfl/net.h"

// The Wi-Fi connection's wrapper of the DWC library, overlay 9 (pokebw2's func_ov011_02152404)
BOOL func_ov009_02157170(u32 a0, u32 a1);

// Called with a Wi-Fi library error the connection has queued: its code, its type and its kind, and the work
typedef void (*DWCRapErrorFunc)(void *work, int code, int type, int error);

// Asked about a connection event, with its work and three of the event's values
typedef u32 (*DWCRapEventFunc)(void *work, int a1, int event, int a3);

// Asked whether to take a machine in: its friend index, or -1 for one that is nobody's friend, and the game's work
typedef BOOL (*DWCRapRequestFunc)(int index, void *work);

// Called when a connection is made, with the machine's ID and its work
typedef void (*DWCRapConnectFunc)(u16 netId, void *work);

// Called when a machine has gone, with its ID and its work
typedef void (*DWCRapDisconnectFunc)(u8 netId, void *work);

s16 func_ov009_02156b70(void);

// pokebw2's DWCRap_SetEventFunc
// Sets a function asked about connection events, with its work
void func_ov009_02156de0(DWCRapEventFunc func, void *work);

// pokebw2's DWCRap_ResetVoiceChat
void func_ov009_02156400(void);
// pokebw2's DWCRap_SetRecvFuncs
// Sets the functions that machines' data goes to: the one called on the parent, and the one on the children
void func_ov009_02155648(GFLNetRecvFunc parentFunc, GFLNetRecvFunc childFunc);
// pokebw2's DWCRap_SetRequestFunc
// Sets the function asked whether to take a machine in
void func_ov009_02155664(DWCRapRequestFunc func);
// pokebw2's DWCRap_Process
// Runs the connection for a frame
int func_ov009_0215592c(int a0);
// pokebw2's DWCRap_SendPacket
// Sends data to the other machines, as a packet of the type
BOOL func_ov009_02155a80(const void *data, int size, int type);
// pokebw2's DWCRap_Send
BOOL func_ov009_02155b14(const void *data, int size);
// pokebw2's DWCRap_GetNetId
// The machine's ID, or -1 when it is not in a match
int func_ov009_021562ec(void);
void func_ov009_02156330(void);
// pokebw2's DWCRap_StartVoiceChat
// Starts and ends the voice chat
void func_ov009_021563c0(void);
// pokebw2's DWCRap_StopVoiceChat
void func_ov009_021563d0(void);
// pokebw2's DWCRap_IsVoiceChatActive
// Whether the voice chat is on
int func_ov009_021563ec(void);
// pokebw2's DWCRap_ResetVoiceChat
// pokebw2's DWCRap_SetMic
void func_ov009_0215642c(BOOL a0);
// pokebw2's DWCRap_RequestClose
BOOL func_ov009_02156690(BOOL a0);
// pokebw2's DWCRap_ResetToReady
BOOL func_ov009_02156730(void);
// pokebw2's DWCRap_Shutdown
void func_ov009_0215677c(void);
// pokebw2's DWCRap_GetFriendStatusData
// A friend's location data, and their state
char *func_ov009_02156860(int index);
// pokebw2's DWCRap_GetFriendStatus
u8 func_ov009_02156874(int index);
// pokebw2's DWCRap_GetFriendIndex
// The friend index of the connection
int func_ov009_02156a3c(void);
// pokebw2's DWCRap_GetLastNetId
int func_ov009_02156b8c(void);
s16 func_ov009_02156ba8(void);
void func_ov009_02156bc4(s16 a0);
// pokebw2's DWCRap_IsUserDataDirty
BOOL func_ov009_02156c84(void);
// pokebw2's DWCRap_ClearUserDataDirty
void func_ov009_02156c98(void);
// pokebw2's DWCRap_IsReady
BOOL func_ov009_02156cac(void);
// pokebw2's DWCRap_IsConnected
BOOL func_ov009_02156cd0(void);
// pokebw2's DWCRap_GetStatus
int func_ov009_02156d5c(void);
void func_ov009_02156d70(int a0);
// pokebw2's DWCRap_IsEnding
BOOL func_ov009_02156d84(void);
// pokebw2's DWCRap_SetReportError
void func_ov009_02156db0(int a0);
// pokebw2's DWCRap_SetErrorFunc
// Sets the function called when the connection is lost
void func_ov009_02156dc4(DWCRapErrorFunc func, void *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's DWCRap_GetErrorKind
// The kind of a Wi-Fi library error, from its code and type
int func_ov009_02156460(int code, int type);
// pokebw2's DWCRap_IsInitialized
// Whether the Wi-Fi connection has been started
BOOL func_ov009_02155388(void);
// pokebw2's DWCRap_Init
// Starts the connection with the player's user data and friend list, returning 0, or 1 or 2 if the user data is
// invalid
int func_ov009_0215539c(void *userData, DWCFriendData *friendList);
// pokebw2's DWCRap_StartMatch
// Starts matching with whoever has the same key, as numEntry machines
BOOL func_ov009_0215568c(const char *key, int numEntry, BOOL a2, int a3);
// pokebw2's DWCRap_StartMatchQuery
// Starts a match of numEntry machines on a query, with a function that rates the candidates
BOOL func_ov009_0215580c(const char *query, int numEntry, DWCEvalPlayerCallback evalFunc, void *evalWork);
// pokebw2's DWCRap_SetOwnStatusData
// Sets the data that tells friends where this player is, from 0 to 0x20 bytes
void func_ov009_0215683c(const char *data, int size);
// pokebw2's DWCRap_Connect
// Connects to a friend, or to anybody if the index is negative; returns 0, or a negative number when it can't yet
int func_ov009_02156888(int friendIndex, int numEntry, BOOL a2);
BOOL func_ov009_02156b68(void);

#endif // BWPORT_GFL_DWC_RAP_H
