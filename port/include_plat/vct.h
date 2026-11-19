// port/include_plat/vct.h: libvct as pret pokeplatinum names and uses it (<vct.h>, VCT_*), declarations only (the online
// side is stubbed on the PC). It stands in for Black's lib/dwc/include/dwc/vct.h, which has the same library under its
// func_ov013_* names: the same include guard keeps the two out of one file. The layout of VCTSession is Black's; the
// numbers of the constants are in the order the library's own callers (Black's dwc_rap_vchat.c, lib/dwc/src/vct) use
// them, and Platinum's code only compares them with each other.
#ifndef BWPORT_DWC_VCT_H
#define BWPORT_DWC_VCT_H

#include "types.h"

#define VCT_ERROR_NONE 0

// The library's mode (VCTConfig.mode); Black's lib/dwc/src/vct takes 1, 2 and 3
typedef enum VCTMode {
    VCT_MODE_PHONE = 1,      // one session that the application asks for and answers
    VCT_MODE_CONFERENCE = 2, // a session for each machine of the match, with no asking
} VCTMode;

// Platinum's conference is the match's 4 machines: one session for each of the others, and a seat for each machine
#define VCT_MAX_CONFERENCE_CLIENT 4

// 8000 Hz samples of a frame, and the memory VCT_Init is given for each session: Black's dwc_rap_vchat.c gives
// 0x23c0 bytes a session, which these two multiply to
#define VCT_AUDIO_FRAME_LENGTH 68
#define VCT_AUDIO_BUFFER_SIZE 0x8f0
#define VCT_DEFAULT_AUDIO_BUFFER_COUNT 4

typedef enum VCTCodec {
    VCT_CODEC_2BIT_ADPCM,
    VCT_CODEC_3BIT_ADPCM,
    VCT_CODEC_4BIT_ADPCM,
    VCT_CODEC_G711_ULAW,
} VCTCodec;

typedef enum VCTRequest {
    VCT_REQUEST_INVITE,
    VCT_REQUEST_CANCEL,
    VCT_REQUEST_BYE,
} VCTRequest;

typedef enum VCTResponse {
    VCT_RESPONSE_OK,
    VCT_RESPONSE_BUSY_HERE,
    VCT_RESPONSE_TERMINATED,
} VCTResponse;

// What a session's callback is told (Black's library calls it with 7 once the voice flows both ways and 9 at an end)
typedef enum VCTEvent {
    VCT_EVENT_NONE,
    VCT_EVENT_INCOMING,
    VCT_EVENT_CANCEL,
    VCT_EVENT_RESPONDBYE,
    VCT_EVENT_REJECT,
    VCT_EVENT_BUSY,
    VCT_EVENT_TIMEOUT,
    VCT_EVENT_CONNECTED,
    VCT_EVENT_ABORT,
    VCT_EVENT_DISCONNECTED,
} VCTEvent;

// A session's state: Black's library sets 2 while the voice flows both ways, and answers an invitation (command 0) of a
// session in state 4
typedef enum VCTState {
    VCT_STATE_NONE = 0,
    VCT_STATE_CONNECTED = 2,
    VCT_STATE_INCOMING = 4,
} VCTState;

typedef struct VCTSession VCTSession;
struct VCTSession {
    // Not 0 while the session is in use: the library's mode (Black's lib/dwc spells it active)
    int mode;
    u8 aid;
    // The machine that asked for the machine's voice, when it is not the machine itself
    u8 origin;
    u8 pad6[2];
    // The machines whose voice is heard, a bit each
    u32 mask;
    int state;
    u32 unk10;
    VCTSession *next;
};

typedef void (*VCTCallback)(u8 aid, VCTEvent event, VCTSession *session, void *userData);

typedef struct VCTConfig {
    VCTMode mode;
    // Memory for the sessions of the others, and how many there are
    VCTSession *session;
    u32 numSession;
    // The ID of this machine in the match
    u8 aid;
    VCTCallback callback;
    void *userData;
    // Memory for the packets that come in, aligned to 32 bytes
    void *audioBuffer;
    u32 audioBufferSize;
} VCTConfig;

// The voice activity of the microphone: a scale of its level, and whether it is speech
typedef struct VCTVADInfo {
    s32 scale;
    s32 activity;
} VCTVADInfo;

BOOL VCT_Init(VCTConfig *config);
void VCT_Cleanup(void);
void VCT_Main(void);
VCTSession *VCT_CreateSession(u8 aid);
BOOL VCT_DeleteSession(VCTSession *session);
int VCT_Request(VCTSession *session, int request);
int VCT_Response(VCTSession *session, int response);
int VCT_AddConferenceClient(u8 aid);
BOOL VCT_StartStreaming(VCTSession *session);
void VCT_StopStreaming(VCTSession *session);
BOOL VCT_SendAudio(const void *samples, int size);
BOOL VCT_ReceiveAudio(void *dst, int size, u32 *heardMask);
BOOL VCT_SetCodec(int codec);
void VCT_EnableVAD(BOOL enable);
void VCT_EnableEchoCancel(BOOL enable);
void VCT_GetVADInfo(VCTVADInfo *info);
BOOL VCT_HandleData(u8 aid, void *data, int size);

#endif // BWPORT_DWC_VCT_H
