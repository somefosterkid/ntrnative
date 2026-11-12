// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/vct/vct_audio.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "vct_internal.h"
#include "vct_dsp.h"
#include "vct_adpcm.h"

#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/math.h"
#include "nitro/cp.h"
#include "gfl/std.h"
#include "dwc/dwc.h"
#include "dwc/vct.h"

// The sound going out and coming in. A frame of the microphone is filtered, coded and sent to every machine that
// listens; the packets of the other machines wait in a queue each, and are decoded, mixed and played once their time
// comes.

// The two frames going out: one is filled while the other is sent
typedef struct VCTSendQueue {
    u8 frames[2 * sizeof(VCTVoicePacket)];
    u32 readIndex;
    u32 writeIndex;
    VCTVoicePacket *sending;
    u32 pendingMask;
    u32 scheduleColumn;
    int scheduleRow;
} VCTSendQueue;

BOOL func_ov013_021b0f38(int aid, VCTVoicePacket *packet, int size, u64 now);
VCTVoicePacket *func_ov013_021b0d18(void);
BOOL func_ov013_021b0904(u8 *dst, int size, int slot, u32 *heardMask);

// The frame sizes of the codecs in units of 0x44 bytes, and the bytes more
// pokebw2's sFrameSizes
static const u8 data_ov013_021b3ad0[VCT_CODEC_MAX][2] = {{8, 0}, {8, 0}, {2, 4}, {3, 4}, {4, 4}};

// How many packets to send in each of the ticks that follow a frame, in a row for each number of machines to send to
// pokebw2's sSendSchedule
static const u8 data_ov013_021b3ada[22] = {1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 3, 0};

// Whether to leave out frames with no voice in them
// pokebw2's sVadEnabled
static int data_ov013_021b3c24  ATTRIBUTE_ALIGN(4)= TRUE;

// libVCT's separate statics (pokebw2's sAudio struct), defined in the order MWCC's size sort (-ipa file) turns
// into Black's addresses
// libVCT's sMarker: set when the next frame out starts a stream
static BOOL data_ov013_021b3c98;
// libVCT's sEnableEchoCancel
static BOOL data_ov013_021b3c9c;
// libVCT's sNumStream: the sessions streaming
static int data_ov013_021b3ca4;
// libVCT's sChangeVAD: set when the VAD is turned off, so the next frame out starts a stream
static BOOL data_ov013_021b3c8c;
// libVCT's sAudioPacketSize: the size of a packet, header and all
static int data_ov013_021b3c90;
// libVCT's sVADStatus
static int data_ov013_021b3cac;
// libVCT's sBitmap: the machines that are heard, one bit for each ID
static u32 data_ov013_021b3c88;
// libVCT's sAdpcmState
static VCTAdpcmState data_ov013_021b3c94;
// libVCT's sAudioDataSize
static u32 data_ov013_021b3ca8;
// libVCT's sCodec
static VCTCodec data_ov013_021b3ca0;
// libVCT's sAudioSequence
static u16 data_ov013_021b3c84;
// pokebw2's sSlotSessions
static VCTSession *data_ov013_021b3cb0[VCT_SLOT_MAX];
// pokebw2's sReceivers
static VCTReceiver data_ov013_021b3cbc[VCT_SLOT_MAX];
// pokebw2's sMixBuffer
static s16 data_ov013_021b3e78[VCT_FRAME_SAMPLES];
// pokebw2's sSendQueue
static VCTSendQueue data_ov013_021b42c0 ATTRIBUTE_ALIGN(32);


static inline void vct_reset_send_info(void)
{
    data_ov013_021b42c0.readIndex = 0;
    data_ov013_021b42c0.writeIndex = 0;
    data_ov013_021b42c0.pendingMask = 0;
    data_ov013_021b42c0.sending = NULL;
    data_ov013_021b42c0.scheduleColumn = 0;
    data_ov013_021b42c0.scheduleRow = 0;
}
BOOL func_ov013_021b0508(void)
{
    u32 i;

    data_ov013_021b3c84 = OS_GetTickLo();
    data_ov013_021b3c24 = TRUE;
    data_ov013_021b3ca4 = 0;
    data_ov013_021b3c88 = 0;
    if (data_ov013_021b60d8.mode == 1) {
        data_ov013_021b3ca0 = VCT_CODEC_PCM8;
    } else {
        data_ov013_021b3ca0 = VCT_CODEC_ADPCM4;
    }
    data_ov013_021b3ca8 = VCT_FRAME_BYTES;
    data_ov013_021b3c90 = data_ov013_021b3ad0[data_ov013_021b3ca0][0] * 0x44 + data_ov013_021b3ad0[data_ov013_021b3ca0][1] + 0xc;
    vct_reset_send_info();
    func_ov013_021b3250();
    for (i = 0; i < VCT_SLOT_MAX; i++) {
        data_ov013_021b3cb0[i] = NULL;
        func_ov013_021b16e4(i);
        func_ov013_021b19bc(&data_ov013_021b3cbc[i], i, -1);
    }
    func_ov013_021b12fc();
    func_ov013_021b3364();
    return TRUE;
}

void func_ov013_021b0600(void)
{
    data_ov013_021b3ca4 = 0;
}

BOOL func_ov013_021b0614(VCTSession *session)
{
    u32 slot;

    if (!session) {
        return FALSE;
    }
    if (data_ov013_021b3ca4 == VCT_SLOT_MAX) {
        return FALSE;
    }
    if (session->state != 2 && session->state != 3) {
        return FALSE;
    }
    for (slot = 0; slot < VCT_SLOT_MAX; slot++) {
        if (data_ov013_021b3cb0[slot] == session) {
            return TRUE;
        }
    }
    for (slot = 0; slot < VCT_SLOT_MAX; slot++) {
        if (data_ov013_021b3cb0[slot] == NULL) {
            data_ov013_021b3cb0[slot] = session;
            break;
        }
    }
    data_ov013_021b3ca4++;
    data_ov013_021b3c88 |= session->mask;
    func_ov013_021b19bc(&data_ov013_021b3cbc[slot], slot, session->origin);
    func_ov013_021b16e4(slot);
    if (session->state != 2) {
        return TRUE;
    }
    if (data_ov013_021b3ca4 == 1) {
        func_ov013_021b3018();
        data_ov013_021b3c84 = OS_GetTickLo();
        data_ov013_021b3c94.predictor = 0;
        data_ov013_021b3c94.stepIndex = 0;
        data_ov013_021b3c98 = TRUE;
        data_ov013_021b42c0.readIndex = 0;
        data_ov013_021b42c0.writeIndex = 0;
        data_ov013_021b42c0.pendingMask = 0;
        data_ov013_021b42c0.sending = NULL;
        data_ov013_021b42c0.scheduleColumn = 0;
        data_ov013_021b42c0.scheduleRow = 0;
    }
    return TRUE;
}


void func_ov013_021b074c(VCTSession *session)
{
    u32 slot;

    for (slot = 0; slot < VCT_SLOT_MAX; slot++) {
        if (data_ov013_021b3cb0[slot] == session) {
            data_ov013_021b3cb0[slot] = NULL;
            data_ov013_021b3ca4--;
            func_ov013_021b1a48(&data_ov013_021b3cbc[slot]);
            func_ov013_021b1720(slot);
            func_ov013_021b133c(slot);
            data_ov013_021b3c88 &= ~session->mask;
            break;
        }
    }
    if (data_ov013_021b3ca4 != 0) {
        return;
    }
    data_ov013_021b3c98 = TRUE;
    func_ov013_021b3018();
    data_ov013_021b3c88 = 0;
}


static inline u32 TicksToUnits(u64 ticks, u32 unit)
{
    return ticks / unit;
}

// Hands a frame of the microphone's samples to the library to send, returning whether it took it
BOOL func_ov013_021b07f8(const void *samples, int size)
{
    VCTVoicePacket *packet;
    u32 i;
    BOOL hasSender = FALSE;

    if (size != data_ov013_021b3ca8) {
        return FALSE;
    }
    if (data_ov013_021b3ca4 == 0) {
        return FALSE;
    }
    for (i = 0; i < VCT_SLOT_MAX; i++) {
        if (data_ov013_021b3cb0[i] != NULL && data_ov013_021b3cb0[i]->state == 2) {
            hasSender = TRUE;
            break;
        }
    }
    if (!hasSender) {
        return FALSE;
    }
    packet = (VCTVoicePacket *)&data_ov013_021b42c0.frames[(data_ov013_021b42c0.writeIndex & 1) ? 0 : sizeof(VCTVoicePacket)];
    packet->magic = VCT_PACKET_MAGIC;
    if (data_ov013_021b3ca0 >= VCT_CODEC_ADPCM2) {
        MIi_CpuCopyFast(samples, &packet->payload[4], size);
    } else {
        MIi_CpuCopyFast(samples, packet->payload, size);
    }
    packet->type = VCT_TYPE_VOICE;
    packet->sequence = data_ov013_021b3c84++;
    packet->timestamp = OS_GetTick() / 64;
    data_ov013_021b42c0.writeIndex++;
    return TRUE;
}

BOOL func_ov013_021b0904(u8 *dst, int size, int slot, u32 *heardMask)
{
    int more;
    VCTPacket *packet;
    VCTReceiver *receiver = &data_ov013_021b3cbc[slot];
    BOOL first = TRUE;
    u64 now;

    more = func_ov013_021b19ac(slot);
    while (more) {
        now = OS_GetTick();
        packet = func_ov013_021b190c(slot);
        if (packet != NULL) {
            if (packet->playTime > now + 0x34) {
                first = FALSE;
                if (packet->silence == 0 && receiver->held != NULL) {
                    receiver->playedCount++;
                    MIi_CpuCopyFast(receiver->held->data, dst, size);
                    func_ov013_021b2fe4(receiver->held);
                    receiver->held = NULL;
                    if (heardMask != NULL) {
                        *heardMask |= 1 << packet->aid;
                    }
                    return TRUE;
                }
                return FALSE;
            }
            if (now > packet->playTime + 0x8b4c) {
                receiver->lastPlayedSequence = packet->sequence;
                more = func_ov013_021b1a74(receiver, packet);
                func_ov013_021b190c(slot);
                first = FALSE;
                continue;
            }
            if (receiver->lastPlayedSequence + 1 != packet->sequence && receiver->lastPlayedSequence != 0
                && receiver->lastPlayedSequence <= packet->sequence) {
                if (packet->silence == 0) {
                    receiver->lostCount++;
                }
            }
            receiver->lastPlayedSequence = packet->sequence;
            if (first && receiver->unk70 != 0) {
                more = func_ov013_021b1a74(receiver, packet);
                func_ov013_021b190c(slot);
                receiver->unk70--;
                continue;
            }
            {
                u32 delay = (u32)(now - packet->arrival);
                u32 change = delay - receiver->lastDelay;

                receiver->lastDelay = delay;
                receiver->avgDelay = receiver->avgDelayCopy = delay + ((change - delay) >> 4);
            }
            MIi_CpuCopyFast(packet->data, dst, size);
            func_ov013_021b1a74(receiver, packet);
            if (heardMask != NULL) {
                *heardMask |= 1 << packet->aid;
            }
            return TRUE;
        } else {
            receiver->queuedCount = more;
            return FALSE;
        }
    }
    if (receiver->held != NULL) {
        func_ov013_021b2fe4(receiver->held);
        receiver->held = NULL;
    }
    return FALSE;
}


static inline BOOL vct_receive_mixed_audio(void *dst, u32 size, u32 *heardMask)
{
    u32 i;
    BOOL flag = FALSE;

    if (size == data_ov013_021b3ca8 && data_ov013_021b3ca4 != 0) {
        for (i = 0; i < VCT_SLOT_MAX; i++) {
            if (func_ov013_021b0904(dst, size, i, heardMask) == TRUE) {
                flag = TRUE;
                break;
            }
        }

        if (flag) {
            i++;
            if (i == VCT_SLOT_MAX) {
                flag = TRUE;
            } else {
                for (u32 j = i; j < VCT_SLOT_MAX; j++) {
                    if (func_ov013_021b0904(data_ov013_021b3e78, size, j, heardMask) == TRUE) {
                        func_ov013_021b3624(dst, data_ov013_021b3e78, dst, size);
                    }
                }
            }
        } else {
            MI_CpuClearFast(dst, size);
        }
        return flag;
    } else {
        MI_CpuClearFast(dst, size);
        return FALSE;
    }
}

BOOL func_ov013_021b0b48(void *dst, int size, u32 *heardMask)
{
    if (heardMask != NULL) {
        *heardMask = 0;
    }

    BOOL heard = vct_receive_mixed_audio(dst, size, heardMask);

    if (data_ov013_021b3c9c) {
        func_ov013_021b33e0(dst, size);
    }

    return heard;
}


BOOL func_ov013_021b0c64(int codec)
{
    if (codec >= VCT_CODEC_MAX) {
        return FALSE;
    }
    if (data_ov013_021b60d8.mode != 1 && (u32)codec <= VCT_CODEC_MULAW) {
        return FALSE;
    }
    data_ov013_021b3c90 = data_ov013_021b3ad0[codec][0] * 0x44 + data_ov013_021b3ad0[codec][1] + 0xc;
    data_ov013_021b3ca0 = codec;
    data_ov013_021b3c94.predictor = 0;
    data_ov013_021b3c94.stepIndex = 0;
    return TRUE;
}


void func_ov013_021b0cd8(BOOL enabled)
{
    data_ov013_021b3c24 = enabled;
    func_ov013_021b3018();
    if (!enabled) {
        data_ov013_021b3c8c = TRUE;
    }
}


void func_ov013_021b0d08(BOOL enabled)
{
    data_ov013_021b3c9c = enabled;
}

VCTVoicePacket *func_ov013_021b0d18(void)
{
    u8 *samples;
    VCTVoicePacket *packet;
    u32 energy;
    u32 rms;
    VCTCodec codec = data_ov013_021b3ca0;
    u32 queued = data_ov013_021b42c0.writeIndex - data_ov013_021b42c0.readIndex;

    if (queued > 1) {
        data_ov013_021b42c0.readIndex = data_ov013_021b42c0.writeIndex - 1;
        queued = 1;
    }
    if (queued == 0) {
        return NULL;
    }
    packet = (VCTVoicePacket *)&data_ov013_021b42c0.frames[(data_ov013_021b42c0.readIndex & 1) ? 0 : sizeof(VCTVoicePacket)];
    samples = packet->payload;
    if (codec >= VCT_CODEC_ADPCM2) {
        samples += 4;
    }
    energy = func_ov013_021b3278((s16 *)samples, data_ov013_021b3ca8);
    reg_CP_SQRTCNT = 0;
    *(vu32 *)0x040002b8 = energy;
    while (reg_CP_SQRTCNT & REG_CP_SQRTCNT_BUSY_MASK) {
    }
    rms = reg_CP_SQRT_RESULT;
    if (data_ov013_021b3c9c) {
        func_ov013_021b3604((s16 *)samples, data_ov013_021b3ca8, rms);
    }
    packet->codec = codec;
    if (data_ov013_021b3c24) {
        data_ov013_021b3cac = func_ov013_021b3084(energy, rms, NULL);
        switch (data_ov013_021b3cac) {
        case 0:
            data_ov013_021b42c0.readIndex++;
            return NULL;
        case 1:
            packet->codec |= 0x80;
            break;
        case 3:
            packet->type = VCT_TYPE_SYNC;
            break;
        }
    }
    if (!data_ov013_021b3c24) {
        if (data_ov013_021b3c8c) {
            packet->codec |= 0x80;
            data_ov013_021b3c8c = FALSE;
        }
    }
    if (data_ov013_021b3c98) {
        packet->codec |= 0x80;
        data_ov013_021b3c98 = FALSE;
    }
    if (codec >= VCT_CODEC_ADPCM2) {
        if (packet->codec & 0x80) {
            data_ov013_021b3c94.predictor = 0;
            data_ov013_021b3c94.stepIndex = 0;
        }
        func_ov013_021b04a8(samples - 4, (s16 *)samples, data_ov013_021b3ca8 / 2, &data_ov013_021b3c94, codec);
    } else if (codec == VCT_CODEC_MULAW) {
        func_ov013_021b1588(samples, (s16 *)samples, data_ov013_021b3ca8 / 2);
    } else if (codec == VCT_CODEC_PCM8) {
        func_ov013_021b1688(samples, (s16 *)samples, data_ov013_021b3ca8 / 2);
    }
    data_ov013_021b42c0.pendingMask = data_ov013_021b3c88;
    data_ov013_021b42c0.pendingMask &= ~(1 << data_ov013_021b60d8.aid);
    data_ov013_021b42c0.scheduleRow = MATH_CountPopulation(data_ov013_021b42c0.pendingMask) - 1;
    if (data_ov013_021b42c0.scheduleRow > 7) {
        return NULL;
    }
    data_ov013_021b42c0.scheduleColumn = 0;
    return packet;
}

BOOL func_ov013_021b0f38(int aid, VCTVoicePacket *packet, int size, u64 now)
{
    u32 i;
    VCTPacket *queued;
    VCTReceiver *receiver = NULL;
    int codec;
    u8 *data;

    if (data_ov013_021b3ca4 == 0) {
        return FALSE;
    }
    for (i = 0; i < VCT_SLOT_MAX; i++) {
        VCTReceiver *candidate = &data_ov013_021b3cbc[i];

        if (aid == candidate->aid) {
            receiver = candidate;
            break;
        }
    }
    if (receiver == NULL) {
        return FALSE;
    }
    if (packet->type == VCT_TYPE_SYNC) {
        receiver->syncSequence = packet->sequence;
        receiver->firstTimestamp = 0;
        receiver->warmup = 1;
        return FALSE;
    }
    if (packet->type != VCT_TYPE_VOICE) {
        return FALSE;
    }
    codec = packet->codec & 0x7f;
    receiver->codec = codec;
    if (codec >= VCT_CODEC_MAX) {
        return FALSE;
    }
    if (size != data_ov013_021b3ad0[codec][0] * 0x44 + data_ov013_021b3ad0[codec][1] + 0xc) {
        return FALSE;
    }
    if (!(data_ov013_021b3c88 & (aid != 0 ? 1 << aid : 1))) {
        return FALSE;
    }
    queued = func_ov013_021b2fb0();
    if (queued == NULL) {
        func_ov013_021b1720(receiver->slot);
        func_ov013_021b133c(receiver->slot);
        queued = func_ov013_021b2fb0();
        if (queued == NULL) {
            func_ov013_021b1790();
            func_ov013_021b1398();
            queued = func_ov013_021b2fb0();
        }
        if (queued == NULL) {
            return FALSE;
        }
    }
    data = (u8 *)&queued->silence - (size - 0xc);
    MI_CpuCopy8(packet->payload, data, size - 0xc);
    queued->slot = receiver->slot;
    queued->codec = codec;
    queued->data = data;
    queued->dataSize = size - 0xc;
    queued->arrival = now;
    queued->aid = aid;
    queued->silence = packet->codec & 0x80;
    queued->timestamp = packet->timestamp;
    queued->sequence = packet->sequence;
    if (!func_ov013_021b1b94(receiver, queued)) {
        func_ov013_021b2fe4(queued);
        return FALSE;
    }
    func_ov013_021b1404(queued);
    return TRUE;
}



int func_ov013_021b111c(void)
{
    int i;
    int count;
    int slot;
    u32 irq;
    VCTPacket *first = func_ov013_021b1488();

    if (first == NULL) {
        return 0;
    }
    irq = OS_DisableInterrupts();
    slot = first->slot;
    count = func_ov013_021b1498(slot);
    for (i = 0; i < count; i++) {
        VCTPacket *packet = func_ov013_021b14a8(slot);

        if (packet->codec >= VCT_CODEC_ADPCM2) {
            func_ov013_021b043c(packet->data, packet->samples, packet->dataSize, packet->codec);
        } else if (packet->codec == VCT_CODEC_MULAW) {
            func_ov013_021b1634(packet->data, packet->samples, packet->dataSize);
        } else {
            func_ov013_021b16b4((const s8 *)packet->data, packet->samples, packet->dataSize);
        }
        packet->data = (u8 *)packet->samples;
        func_ov013_021b14f8(packet);
        if (func_ov013_021b1810(packet, packet->slot) < 0) {
            func_ov013_021b2fe4(packet);
        }
    }
    OS_RestoreInterrupts(irq);
    return count;
}

static inline void NextFrame(void)
{
    data_ov013_021b42c0.readIndex++;
    data_ov013_021b42c0.sending = NULL;
    data_ov013_021b42c0.pendingMask = 0;
}

BOOL VCT_SendPending(void)
{
    int i;
    int count;
    u32 leading;

    if (data_ov013_021b3ca4 == 0) {
        return TRUE;
    }
    if (!data_ov013_021b42c0.sending) {
        data_ov013_021b42c0.sending = func_ov013_021b0d18();
        if (data_ov013_021b42c0.sending && data_ov013_021b3c9c) {
            return FALSE;
        }
        return TRUE;
    }
    count = 1;
    if (data_ov013_021b60d8.mode == 2) {
        count = (data_ov013_021b3ada + data_ov013_021b42c0.scheduleRow * 3)[data_ov013_021b42c0.scheduleColumn++];
    }
    for (i = 0; i < count; i++) {
        leading = MATH_CountLeadingZeros(data_ov013_021b42c0.pendingMask);
        if (leading == 32) {
            break;
        }
        data_ov013_021b42c0.pendingMask &= ~(0x80000000 >> leading);
        func_ov009_02177044((u8)(31 - leading), data_ov013_021b42c0.sending, data_ov013_021b3c90);
        if (data_ov013_021b42c0.pendingMask == 0) {
            NextFrame();
            break;
        }
    }
    return TRUE;
}
