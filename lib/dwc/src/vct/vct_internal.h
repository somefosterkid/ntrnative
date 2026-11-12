// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/vct/vct_internal.h.
#ifndef POKEBW2_DWC_VCT_INTERNAL_H
#define POKEBW2_DWC_VCT_INTERNAL_H

#include <mem.h>
#include "types.h"

#include "dwc/vct.h"

// The voice chat library libVCT (the ROM carries its version string, "[SDK+Nintendo:libVCT 1.4.1]"), which sends the
// microphone's sound to the machines of a Wi-Fi match and plays what they send. Overlay 29 holds it. The ROM names none
// of its functions, so these names say what the code does.

// Sound is cut into frames of 0x440 bytes of 16-bit samples (544 samples) at 8 kHz, and each frame goes out as one
// packet. A packet starts with a 12-byte header, then holds the frame coded in one of five ways (see VCTCodec).
#define VCT_FRAME_SAMPLES 544
#define VCT_FRAME_BYTES (VCT_FRAME_SAMPLES * 2)
// The most machines whose voices are mixed
#define VCT_SLOT_MAX 3
// The most machines of a match
#define VCT_AID_MAX 32

// The magic number that starts every packet, "TCV_" in memory
#define VCT_PACKET_MAGIC 0x5f564354

// The kind of a packet
#define VCT_TYPE_CONTROL 0x00
#define VCT_TYPE_VOICE 0x40
#define VCT_TYPE_SYNC 0x41
#define VCT_TYPE_BROADCAST 0xff

// How the samples of a frame are coded
typedef enum VCTCodec {
    VCT_CODEC_PCM8,
    VCT_CODEC_MULAW,
    VCT_CODEC_ADPCM2,
    VCT_CODEC_ADPCM3,
    VCT_CODEC_ADPCM4,
    VCT_CODEC_MAX,
} VCTCodec;

// The predictor and step index that every sample of an ADPCM stream is coded against, which the stream's packets
// start with
typedef struct VCTAdpcmState {
    s16 predictor;
    u8 stepIndex;
    u8 pad;
} VCTAdpcmState;

// A voice packet as it goes over the wire
typedef struct VCTVoicePacket {
    u32 magic;
    u8 type;
    // The codec, with the top bit set on the first frame after a silence
    u8 codec;
    u16 sequence;
    u32 timestamp;
    u8 payload[0x454];
} VCTVoicePacket;

// A control packet: a machine asking another to start or stop hearing it
typedef struct VCTControlPacket {
    u32 magic;
    u8 type;
    u8 length;
    u8 command;
    u8 pad7;
    u8 from;
    u8 to;
    u8 flag;
    u8 origin;
    u32 argument;
} VCTControlPacket;

// What a control packet gave the application: an event, and the session it is for
typedef struct VCTEventResult {
    int event;
    VCTSession *session;
} VCTEventResult;

// A voice packet that has come in, on its way to being played
typedef struct VCTPacket VCTPacket;
struct VCTPacket {
    VCTPacket *nextFree;
    VCTPacket *prev;
    VCTPacket *next;
    int codec;
    u8 aid;
    u8 slot;
    u8 pad12[2];
    int dataSize;
    u32 sequence;
    s16 samples[VCT_FRAME_SAMPLES];
    int silence;
    u32 timestamp;
    u64 arrival;
    u64 playTime;
    u8 *data;
};

// What one machine's voice has gone through on the way to being played
typedef struct VCTReceiver {
    u32 avgDelay;
    u32 playoutDelay;
    int queuedCount;
    int skewDifference;
    u32 lastSequenceSeen;
    int lostCount;
    int unk18;
    int lateCount;
    int playedCount;
    int codec;
    u32 lastDelay;
    u32 avgDelayCopy;
    u32 syncSequence;
    int slot;
    int aid;
    VCTPacket *held;
    int unk40;
    u64 firstArrival;
    u32 firstTimestamp;
    int unk50;
    u32 lastSequence;
    u32 lastPlayedSequence;
    int needsReset;
    u32 wrap;
    u32 expectedSequence;
    u16 sequence16;
    u8 pad6a[2];
    u32 warmup;
    int unk70;
    int skewAverage;
    int skewFirst;
    int jitterAverage;
    int lastSkew;
    int burstCount;
    int delayFloor;
    u32 delayCeiling;
    u32 belowCount;
} VCTReceiver;

// What the library keeps of its configuration
typedef struct VCTSettings {
    u8 aid;
    u8 hostAid;
    u8 pad2[2];
    VCTCallback callback;
    void *userData;
    int unkC;
    int mode;
    int unk14;
} VCTSettings;

extern VCTSettings data_ov013_021b60d8;
extern const char VCTi_Signature[];

// Send a packet to the machine with the given ID, from DWC in overlay 11 (neither has a header yet): the first for
// voice, the second for control packets
BOOL func_ov011_02168b18(u8 aid, const void *data, int size);
BOOL func_ov011_02168a08(u8 aid, const void *data, int size);
// Send a packet to a machine from DWC in overlay 9 (no header yet): the same as the ones above
BOOL DWC_SendReliable(u8 aid, const void *data, int size);

// Copies size bytes, a multiple of 4, 32 bytes at a time where it can: NitroSDK's MIi_CpuCopyFast (not in nitro/mi.h yet)
void MIi_CpuCopyFast(const void *src, void *dest, u32 size);

// The timer count, which seeds the packets' sequence numbers
u16 func_0207bb84(void);

// The hardware's divider and square root unit
#define REG_DIVCNT (*(vu16 *)0x04000280)
#define REG_DIV_NUMER_LO (*(vu32 *)0x04000290)
#define REG_DIV_NUMER_HI (*(vu32 *)0x04000294)
#define REG_DIV_DENOM_LO (*(vu32 *)0x04000298)
#define REG_DIV_DENOM_HI (*(vu32 *)0x0400029c)
#define REG_DIV_RESULT_LO (*(vu32 *)0x040002a0)
#define REG_SQRTCNT (*(vu16 *)0x040002b0)
#define REG_SQRT_RESULT (*(vu32 *)0x040002b4)
#define REG_SQRT_PARAM_LO (*(vu32 *)0x040002b8)
#define REG_DIVCNT_BUSY 0x8000
#define REG_SQRTCNT_BUSY 0x8000

// vct_adpcm.c
void VCT_DecodeAdpcm(const u8 *packet, s16 *dst, int size, int bits);
void VCT_EncodeAdpcm(u8 *packet, const s16 *src, int count, VCTAdpcmState *state, int bits);

// vct_audio.c: the frames going out and coming in
BOOL VCT_InitAudio(void);
void VCT_ClearSessionCount(void);
BOOL VCT_AttachSession(VCTSession *session);
void VCT_DetachSession(VCTSession *session);
BOOL VCT_SubmitSamples(const void *samples, int size);
BOOL VCT_ReadSamples(void *dst, int size, u32 *heardMask);
BOOL VCT_SetCodec(int codec);
void VCT_SetVad(BOOL enabled);
void VCT_SetEchoCancel(BOOL enabled);
BOOL VCT_HandleVoicePacket(int aid, const VCTVoicePacket *packet, int size, u64 now);
int VCT_DecodeReceived(void);
BOOL VCT_SendPending(void);

// vct_session.c: the machines
void VCT_ClearSessions(void);
void VCT_CheckTimeout(void);
BOOL VCT_HandleControlPacket(int aid, const VCTControlPacket *packet, int size, VCTEventResult *result);
BOOL VCT_InitSessions(const VCTConfig *config);

// vct_dsp.c: packet memory, microphone levels and echo
BOOL VCT_InitPacketPool(void *buffer, u32 size);
void VCT_ClearPacketPool(void);
VCTPacket *VCT_AllocPacket(void);
void VCT_FreePacket(VCTPacket *packet);
void VCT_ResetCapture(void);
int VCT_UpdateVad(u32 energy, u32 rms, u8 *level);
void VCT_InitCapture(void);
u32 VCT_FilterFrame(s16 *samples, int bytes);
void VCT_InitEcho(void);
void VCT_SaveFarEnd(const s16 *samples, int bytes);
void VCT_CancelEcho(s16 *samples, int bytes, u32 rms);
void VCT_MixSamples(const s16 *a, const s16 *b, s16 *out, int bytes);

// vct_buffer.c: the packets on their way out and in (Black's names)
void func_ov013_021b12fc(void);
void func_ov013_021b133c(int slot);
void func_ov013_021b1398(void);
int func_ov013_021b1404(VCTPacket *packet);
VCTPacket *func_ov013_021b1488(void);
int func_ov013_021b1498(int slot);
VCTPacket *func_ov013_021b14a8(int slot);
int func_ov013_021b14f8(VCTPacket *packet);
void func_ov013_021b1588(u8 *dst, const s16 *src, u32 count);
void func_ov013_021b1634(const u8 *src, s16 *dst, u32 count);
void func_ov013_021b1688(u8 *dst, const s16 *src, u32 count);
void func_ov013_021b16b4(const s8 *src, s16 *dst, u32 count);
u32 func_ov013_021b16e4(int slot);
void func_ov013_021b1720(int slot);
void func_ov013_021b1790(void);
int func_ov013_021b1810(VCTPacket *packet, u32 slot);
VCTPacket *func_ov013_021b190c(int slot);
int func_ov013_021b19ac(int slot);
void func_ov013_021b19bc(VCTReceiver *receiver, int slot, int aid);
void func_ov013_021b1a48(VCTReceiver *receiver);
int func_ov013_021b1a74(VCTReceiver *receiver, VCTPacket *packet);
BOOL func_ov013_021b1b94(VCTReceiver *receiver, VCTPacket *packet);

// Black's names for the overlay 13 functions vct.c calls (pokebw2's names above are the same functions, where known)
void func_ov013_021b0600(void);
int func_ov013_021b111c(void);
void func_ov013_021b23c8(void);
void func_ov013_021b23e0(void);
void func_ov013_021b2f98(void);
BOOL func_ov013_021b0508(void);
BOOL func_ov013_021b0f38(int aid, VCTVoicePacket *packet, int size, u64 now);
BOOL func_ov013_021b2488(int aid, VCTControlPacket *packet, int size, VCTEventResult *result);
BOOL func_ov013_021b2ed8(void *buffer, u32 size);
BOOL func_ov013_021b22b4(VCTConfig *config);

#endif // POKEBW2_DWC_VCT_INTERNAL_H
