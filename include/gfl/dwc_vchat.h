// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/dwc_vchat.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_GFL_DWC_VCHAT_H
#define BWPORT_GFL_DWC_VCHAT_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/spi/ARM9/mic.h"

// pokebw2's DWCRapVChat_Update
// Runs the voice chat a frame; stalled is whether the connection is too slow to send the voice
void func_ov013_021aee14(BOOL stalled);
// pokebw2's DWCRapVChat_ReceivePacket
// Hands a packet that came from a machine to the library, returning whether it was the library's
BOOL func_ov013_021aef44(int aid, u8 *data, int size);
// pokebw2's DWCRapVChat_RequestEnd
void func_ov013_021af13c(void);
// pokebw2's DWCRapVChat_SetEndCallback
// Sets the function called once the voice chat has ended
void func_ov013_021af18c(void (*callback)(void));
// pokebw2's DWCRapVChat_End
void func_ov013_021af1a0(void);
// pokebw2's DWCRapVChat_GetSpeaking
BOOL func_ov013_021af210(void);
// pokebw2's DWCRapVChat_AddHeard
BOOL func_ov013_021af238(u32 netIds, int myNetId);
// pokebw2's DWCRapVChat_RemoveHeard
BOOL func_ov013_021af2ac(int myNetId);
// pokebw2's DWCRapVChat_SetMuted
void func_ov013_021af30c(BOOL muted);

// pokebw2's DWCRapVChat_ReceivePacket
// Hands a packet that came from a machine to the library, returning whether it was the library's
// pokebw2's DWCRapVChat_Update
// Runs the voice chat a frame; stalled is whether the connection is too slow to send the voice
// pokebw2's DWCRapVChat_AddHeard
// pokebw2's DWCRapVChat_SetEndCallback
// Sets the function called once the voice chat has ended
// pokebw2's DWCRapVChat_RequestEnd
// pokebw2's DWCRapVChat_End
// pokebw2's DWCRapVChat_RemoveHeard
// pokebw2's DWCRapVChat_SetMuted
// pokebw2's DWCRapVChat_GetSpeaking

// Microphone and power calls the file makes, not yet named: starting the microphone's sampling for the DS and for the
// DSi, stopping it for the DS, and setting the amplifier's gain
MICResult MIC_StopAutoSampling(void);
// pokebw2's DWCRapVChat_Start
// Starts the voice chat in a heap, coded as the library's codec number says, for some machines
void func_ov013_021aef6c(HeapID heapId, int codec, int count);

#endif // BWPORT_GFL_DWC_VCHAT_H
