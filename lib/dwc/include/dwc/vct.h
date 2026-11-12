// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/vct.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_VCT_H
#define BWPORT_DWC_VCT_H

#include "types.h"

// A machine that the voice chat is talking to. A machine is asked to take part with a request, answers with a reply,
// and the application hears of what comes of it in an event
typedef struct VCTSession VCTSession;

struct VCTSession {
    // Not 0 while the session is in use: the library's mode
    int active;
    u8 aid;
    // The machine that asked for the machine's voice, when it is not the machine itself
    u8 origin;
    u8 pad6[2];
    // The machines whose voice is heard, a bit each
    u32 mask;
    // 2 once the voice flows both ways
    int state;
    u32 unk10;
    VCTSession *next;
};

// Called with an event for a session. The events are numbers: 1, 4 and 8 for a request, 7 for the voice starting, and
// 2, 3, 9, 11 and 12 for a refusal, a close or the end of it
typedef void (*VCTCallback)(u8 aid, int event, VCTSession *session, void *userData);

typedef struct VCTConfig {
    // Memory for the sessions, and how many there are, at most 8
    VCTSession *sessions;
    u32 sessionCount;
    // 1, 2 or 3. In the first the application chooses to ask and to answer for each machine
    int mode;
    // The ID of this machine in the match, below 32
    u8 aid;
    u8 pad[3];
    // Memory for the packets that come in, aligned to 32 bytes
    void *buffer;
    u32 bufferSize;
    VCTCallback callback;
    void *userData;
} VCTConfig;

// pokebw2's VCT_AllocSession
// The sessions: one for a machine, and its voice's start and end
VCTSession *func_ov013_021b1f2c(u8 aid);
// pokebw2's VCT_ReleaseSession
BOOL func_ov013_021b1f54(VCTSession *session);
// pokebw2's VCT_Request
int func_ov013_021b20b0(VCTSession *session, int command);
// pokebw2's VCT_Respond
int func_ov013_021b2180(VCTSession *session, int command);
// pokebw2's VCT_AddSession
int func_ov013_021b21a4(u8 aid);
// pokebw2's VCT_RemoveSession
int func_ov013_021b2254(u8 aid);

// pokebw2's VCT_AttachSession
BOOL func_ov013_021b0614(VCTSession *session);
// pokebw2's VCT_DetachSession
void func_ov013_021b074c(VCTSession *session);
// pokebw2's VCT_SubmitSamples
// Hands a frame of 0x440 bytes of the microphone's samples over to be sent
BOOL func_ov013_021b07f8(const void *samples, int size);
// pokebw2's VCT_ReadSamples
// Fills a frame with the voices that are due, returning whether there was any, and gives the machines heard as bits
BOOL func_ov013_021b0b48(void *dst, int size, u32 *heardMask);
// pokebw2's VCT_SetCodec
// The five ways to code a frame, from 0: 8-bit samples, mu-law and ADPCM at 2, 3 and 4 bits. Not all are allowed
// in every mode
BOOL func_ov013_021b0c64(int codec);
// pokebw2's VCT_SetVad
// Whether frames with no voice in them are left out
void func_ov013_021b0cd8(BOOL enabled);
// pokebw2's VCT_SetEchoCancel
// Whether to keep the microphone from picking up what the speaker plays
void func_ov013_021b0d08(BOOL enabled);

// pokebw2's VCT_GetCaptureLevels
// Copies the four words of how loud the microphone is: whether somebody is speaking, the level, and two limits
void func_ov013_021b3064(s32 *out);

// pokebw2's VCT_Tick
// Runs the library a tick
void func_ov013_021b386c(void);
// pokebw2's VCT_ReceivePacket
// Takes in a packet that another machine has sent, returning FALSE for one that is not meant for the library
BOOL func_ov013_021b38b0(u8 aid, void *data, int size);
// pokebw2's VCT_Shutdown
void func_ov013_021b3844(void);

// pokebw2's VCT_Init
// Starts the library, returning FALSE for a configuration it cannot work with
BOOL func_ov013_021b370c(VCTConfig *config);

#endif // BWPORT_DWC_VCT_H
