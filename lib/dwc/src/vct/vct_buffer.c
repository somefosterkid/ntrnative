// Black's code at ov013 0x021b12fc-0x021b1f2c: pokebw2's lib/dwc/src/vct/vct_buffer.c. The packets that have come in, in
// the order they came, each machine's packets in the order of their sequence numbers, and when each is to be played,
// with the mu-law and 8-bit codecs. It is a file of its own, not Black's vct_audio.c's: its functions reach each static
// from a literal of its own, where vct_audio.c reaches its statics from one base.
#include "vct_internal.h"
#include "vct_dsp.h"

#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/math.h"
#include "gfl/std.h"
#include "dwc/dwc.h"
#include "dwc/vct.h"

// The packets in the order they came
typedef struct VCTList {
    VCTPacket *head;
    VCTPacket *tail;
} VCTList;

static int func_ov013_021b1944(int slot);
static void func_ov013_021b1aa0(VCTReceiver *receiver, u16 sequence);
static BOOL func_ov013_021b1ac0(VCTReceiver *receiver, VCTPacket *packet, u16 sequence);

// The ends of the segments of the mu-law curve
// pokebw2's sSegmentEnds
static int data_ov013_021b3c28[8] = {0x3f, 0x7f, 0xff, 0x1ff, 0x3ff, 0x7ff, 0xfff, 0x1fff};

// pokebw2's sQueueTails
static VCTPacket *data_ov013_021b4bc4[VCT_SLOT_MAX];
// pokebw2's sQueueCounts
static int data_ov013_021b4bb8[VCT_SLOT_MAX];
// Each slot's packets in the order of their sequence numbers
// pokebw2's sQueueHeads
static VCTPacket *data_ov013_021b4bac[VCT_SLOT_MAX];
// pokebw2's sActiveCounts
static int data_ov013_021b4ba0[VCT_SLOT_MAX];
// The packets in the order they came, and how many of each slot's
// pokebw2's sActiveList
static VCTList data_ov013_021b4b98;


void func_ov013_021b12fc(void)
{
    u32 irq = OS_DisableInterrupts();
    int i;

    data_ov013_021b4b98.tail = NULL;
    data_ov013_021b4b98.head = NULL;
    for (i = 0; i < VCT_SLOT_MAX; i++) {
        data_ov013_021b4ba0[i] = 0;
    }
    OS_RestoreInterrupts(irq);
}

void func_ov013_021b133c(int slot)
{
    u32 irq = OS_DisableInterrupts();
    VCTPacket *packet = func_ov013_021b14a8(slot);

    while (packet != NULL) {
        func_ov013_021b14f8(packet);
        func_ov013_021b2fe4(packet);
        packet = func_ov013_021b14a8(slot);
    }
    data_ov013_021b4ba0[slot] = 0;
    OS_RestoreInterrupts(irq);
}

void func_ov013_021b1398(void)
{
    u32 irq = OS_DisableInterrupts();
    VCTPacket *node = data_ov013_021b4b98.head;
    int i;

    while (node != NULL) {
        VCTPacket *packet = node;
        node = node->next;
        func_ov013_021b2fe4(packet);
    }
    data_ov013_021b4b98.head = NULL;
    data_ov013_021b4b98.tail = NULL;
    for (i = 0; i < VCT_SLOT_MAX; i++) {
        data_ov013_021b4ba0[i] = 0;
    }
    OS_RestoreInterrupts(irq);
}



int func_ov013_021b1404(VCTPacket *packet)
{
    u32 irq = OS_DisableInterrupts();

    if (data_ov013_021b4b98.head == NULL) {
        data_ov013_021b4b98.head = packet;
        packet->prev = NULL;
        data_ov013_021b4b98.head->next = NULL;
        data_ov013_021b4b98.tail = data_ov013_021b4b98.head;
    } else {
        packet->prev = data_ov013_021b4b98.tail;
        packet->next = NULL;
        data_ov013_021b4b98.tail->next = packet;
        data_ov013_021b4b98.tail = packet;
    }
    data_ov013_021b4ba0[packet->slot]++;
    OS_RestoreInterrupts(irq);
    return data_ov013_021b4ba0[packet->slot];
}

VCTPacket *func_ov013_021b1488(void)
{
    return data_ov013_021b4b98.head;
}

int func_ov013_021b1498(int slot)
{
    return data_ov013_021b4ba0[slot];
}

VCTPacket *func_ov013_021b14a8(int slot)
{
    VCTPacket *packet = data_ov013_021b4b98.head;
    u32 irq = OS_DisableInterrupts();

    for (; packet != NULL; packet = packet->next) {
        if (packet->slot == slot) {
            OS_RestoreInterrupts(irq);
            return packet;
        }
    }
    OS_RestoreInterrupts(irq);
    return NULL;
}

int func_ov013_021b14f8(VCTPacket *packet)
{
    u32 irq = OS_DisableInterrupts();

    if (packet->prev != NULL) {
        packet->prev->next = packet->next;
    } else {
        data_ov013_021b4b98.head = packet->next;
        if (data_ov013_021b4b98.head != NULL) {
            data_ov013_021b4b98.head->prev = NULL;
        }
    }
    if (packet->next != NULL) {
        packet->next->prev = packet->prev;
    } else {
        data_ov013_021b4b98.tail = packet->prev;
        if (data_ov013_021b4b98.tail != NULL) {
            data_ov013_021b4b98.tail->next = NULL;
        }
    }
    data_ov013_021b4ba0[packet->slot]--;
    OS_RestoreInterrupts(irq);
    return data_ov013_021b4ba0[packet->slot];
}

// The first entry of a segment table that holds the value, or the table's size
static inline int SearchSegment(int value, const int *table, int size)
{
    int i;

    for (i = 0; i < size; i++) {
        if (value <= *table++) {
            return i;
        }
    }
    return size;
}

// Codes samples as 8-bit mu-law
static inline void search(int value, const int *table, int size, int *segment) {
    *segment = 0;
    while (*segment < size) {
        if (value <= *table++) {
            return;
        }
        (*segment)++;
    }
    *segment = size;
}
void func_ov013_021b1588(u8 *dst, const s16 *src, u32 count)
{
    u32 i;

    for (i = 0; i < count; i++) {
        int segment;
        int sign;
        int sample = src[i] >> 2;

        if (sample < 0) {
            sign = 0x7f;
            sample = -sample;
        } else {
            sign = 0xff;
        }
        if (sample > 0x1fdf) {
            sample = 0x1fdf;
        }
        sample += 0x21;
        search(sample, data_ov013_021b3c28, 8, &segment);
        if (segment >= 8) {
            dst[i] = sign ^ 0x7f;
        } else {
            int code = (segment << 4) | ((sample >> (segment + 1)) & 0xf);
            code ^= sign;
            dst[i] = code;
        }
    }
}

void func_ov013_021b1634(const u8 *src, s16 *dst, u32 count)
{
    u32 i;

    for (i = 0; i < count; i++) {
        int v = ~src[i];
        int t = (((v & 0xf) << 3) + 0x84) << ((v & 0x70) >> 4);

        dst[i] = (v & 0x80) ? 0x84 - t : t - 0x84;
    }
}

// Keeps the top eight bits of the samples
void func_ov013_021b1688(u8 *dst, const s16 *src, u32 count)
{
    u32 i;

    for (i = 0; i < count; i++) {
        dst[i] = src[i] >> 8;
    }
}

void func_ov013_021b16b4(const s8 *src, s16 *dst, u32 count)
{
    u32 i;

    for (i = 0; i < count; i++) {
        dst[i] = src[i] << 8;
    }
}

OSIntrMode func_ov013_021b16e4(int slot)
{
    u32 irq = OS_DisableInterrupts();

    data_ov013_021b4bc4[slot] = NULL;
    data_ov013_021b4bac[slot] = NULL;
    data_ov013_021b4bb8[slot] = 0;
    return OS_RestoreInterrupts(irq);
}

void func_ov013_021b1720(int slot)
{
    u32 irq = OS_DisableInterrupts();
    VCTPacket *packet = data_ov013_021b4bac[slot];

    while (packet != NULL) {
        data_ov013_021b4bac[slot] = packet->next;
        if (data_ov013_021b4bac[slot] != NULL) {
            data_ov013_021b4bac[slot]->prev = NULL;
        }
        func_ov013_021b2fe4(packet);
        packet = data_ov013_021b4bac[slot];
    }
    data_ov013_021b4bc4[slot] = NULL;
    data_ov013_021b4bb8[slot] = 0;
    OS_RestoreInterrupts(irq);
}

void func_ov013_021b1790(void)
{
    u32 irq = OS_DisableInterrupts();
    int slot;

    for (slot = 0; slot < VCT_SLOT_MAX; slot++) {
        while (data_ov013_021b4bac[slot] != NULL) {
            VCTPacket *packet = data_ov013_021b4bac[slot];
            data_ov013_021b4bac[slot] = packet->next;
            if (data_ov013_021b4bac[slot] != NULL) {
                data_ov013_021b4bac[slot]->prev = NULL;
            }
            func_ov013_021b2fe4(packet);
        }
        data_ov013_021b4bc4[slot] = NULL;
        data_ov013_021b4bb8[slot] = 0;
    }
    OS_RestoreInterrupts(irq);
}



// Puts a packet in a slot's queue before the packets with higher sequence numbers
int func_ov013_021b1810(VCTPacket *packet, u32 slot)
{
    u32 irq;
    VCTPacket *at;

    if (packet == NULL) {
        return -1;
    }
    if (slot >= VCT_SLOT_MAX) {
        return -1;
    }
    irq = OS_DisableInterrupts();
    if (data_ov013_021b4bac[slot] == NULL) {
        data_ov013_021b4bac[slot] = packet;
        packet->prev = NULL;
        data_ov013_021b4bac[slot]->next = NULL;
        data_ov013_021b4bc4[slot] = data_ov013_021b4bac[slot];
    } else {
        for (at = data_ov013_021b4bc4[slot]; at != NULL; at = at->prev) {
            if (at->sequence < packet->sequence) {
                packet->prev = at;
                packet->next = at->next;
                at->next = packet;
                if (packet->next != NULL) {
                    packet->next->prev = packet;
                }
                if (at == data_ov013_021b4bc4[slot]) {
                    data_ov013_021b4bc4[slot] = packet;
                }
                goto inserted;
            }
        }
        packet->prev = NULL;
        packet->next = data_ov013_021b4bac[slot];
        data_ov013_021b4bac[slot]->prev = packet;
        data_ov013_021b4bac[slot] = packet;
    }
inserted:
    data_ov013_021b4bb8[slot]++;
    OS_RestoreInterrupts(irq);
    return data_ov013_021b4bb8[slot];
}

VCTPacket *func_ov013_021b190c(int slot)
{
    u32 irq = OS_DisableInterrupts();

    if (data_ov013_021b4bac[slot] != NULL) {
        OS_RestoreInterrupts(irq);
        return data_ov013_021b4bac[slot];
    }
    OS_RestoreInterrupts(irq);
    return NULL;
}

static int func_ov013_021b1944(int slot)
{
    u32 irq = OS_DisableInterrupts();

    if (data_ov013_021b4bac[slot] != NULL) {
        VCTPacket *next = data_ov013_021b4bac[slot]->next;

        data_ov013_021b4bac[slot] = next;
        data_ov013_021b4bb8[slot]--;
        if (next != NULL) {
            next->prev = NULL;
        } else {
            data_ov013_021b4bc4[slot] = NULL;
        }
    }
    OS_RestoreInterrupts(irq);
    return data_ov013_021b4bb8[slot];
}

int func_ov013_021b19ac(int slot)
{
    return data_ov013_021b4bb8[slot];
}

void func_ov013_021b19bc(VCTReceiver *receiver, int slot, int aid)
{
    MI_CpuFill8(receiver, 0, 0x28);
    receiver->playoutDelay = 0x134df;
    receiver->lastDelay = 0;
    receiver->avgDelayCopy = 0;
    receiver->slot = slot;
    receiver->aid = aid;
    receiver->jitterAverage = 0;
    receiver->lastSkew = 0;
    receiver->burstCount = 0;
    receiver->delayFloor = 0;
    receiver->delayCeiling = 0x134df;
    receiver->belowCount = 0;
    receiver->firstTimestamp = 0;
    receiver->firstArrival = 0;
    receiver->warmup = 0;
    receiver->unk70 = 0;
    receiver->unk50 = 0;
    receiver->lastSequence = 0;
    receiver->lastPlayedSequence = 0;
    receiver->needsReset = TRUE;
    receiver->held = NULL;
    receiver->unk40 = 0;
    receiver->syncSequence = 0;
}

void func_ov013_021b1a48(VCTReceiver *receiver)
{
    if (receiver->held != NULL) {
        func_ov013_021b2fe4(receiver->held);
        receiver->held = NULL;
    }
    receiver->aid = -1;
}

int func_ov013_021b1a74(VCTReceiver *receiver, VCTPacket *packet)
{
    if (receiver->held != NULL) {
        func_ov013_021b2fe4(receiver->held);
    }
    receiver->held = packet;
    return func_ov013_021b1944(receiver->slot);
}



static void func_ov013_021b1aa0(VCTReceiver *receiver, u16 sequence)
{
    receiver->sequence16 = sequence;
    receiver->expectedSequence = 0x10001;
    receiver->wrap = 0;
    receiver->needsReset = FALSE;
}

// Turns the 16-bit sequence number of a packet into one that goes on past 65535, returning FALSE for one so far off
// that the machine has probably started over
static BOOL func_ov013_021b1ac0(VCTReceiver *receiver, VCTPacket *packet, u16 sequence)
{
    u16 distance = sequence - receiver->sequence16;

    if (distance < 3000) {
        if (sequence < receiver->sequence16) {
            receiver->wrap += 0x10000;
        }
        receiver->sequence16 = sequence;
    } else if (distance <= 0xff9c) {
        if (sequence == receiver->expectedSequence) {
            func_ov013_021b1aa0(receiver, sequence);
            receiver->lastSequence = (u16)(sequence - 1);
        } else {
            receiver->expectedSequence = (u16)(sequence + 1);
            return FALSE;
        }
    } else {
        u32 unwrapped = sequence + receiver->wrap;

        if (unwrapped > receiver->lastSequence + 0xff9c) {
            packet->sequence = unwrapped - 0x10000;
            return TRUE;
        }
    }
    packet->sequence = sequence + receiver->wrap;
    return TRUE;
}

// Works out when a packet is to be played from the time it came and the time its machine stamped it with, and keeps
// track of how much the delay between the two wanders so as to widen or narrow the margin the playing is kept behind.
// Returns FALSE for a packet that is a repeat, out of range or too late or early to be of use
BOOL func_ov013_021b1b94(VCTReceiver *receiver, VCTPacket *packet)
{
    int skew;
    int trend = 0;
    u64 remote;
    u64 local;
    u64 playTime;
    u64 now;

    if (receiver->needsReset) {
        func_ov013_021b1aa0(receiver, packet->sequence);
    } else {
        if (!func_ov013_021b1ac0(receiver, packet, packet->sequence)) {
            receiver->lostCount++;
            return FALSE;
        }
        receiver->lastSequenceSeen = packet->sequence;
        if (receiver->lastSequence == packet->sequence) {
            receiver->lostCount++;
            return FALSE;
        }
        if (receiver->lastSequence + 1 != packet->sequence && packet->silence == 0) {
            int distance = packet->sequence - receiver->lastSequence;

            if (distance < 0) {
                distance = -distance;
            }
            if (distance > 100) {
                receiver->lastSequence = packet->sequence;
                return FALSE;
            }
            if (receiver->lastSequence > packet->sequence) {
                receiver->lateCount++;
            }
        }
    }
    receiver->lastSequence = packet->sequence;
    if (receiver->firstTimestamp == 0) {
        receiver->firstTimestamp = packet->timestamp;
        receiver->firstArrival = packet->arrival;
        receiver->warmup = 0;
    }
    remote = (packet->timestamp - receiver->firstTimestamp) << 6;
    local = packet->arrival - receiver->firstArrival;
    skew = remote - local;
    if (receiver->lastSkew == 0 || packet->silence != 0) {
        receiver->lastSkew = skew;
    } else {
        int change = skew - receiver->lastSkew;

        if (change < 0) {
            change = -change;
        }
        receiver->lastSkew = skew;
        if (change > (receiver->jitterAverage + 0x8b18) * 2 + (u64)0xcc8d) {
            receiver->burstCount = change / 0x8b18 + 2;
        }
        if (receiver->burstCount <= 0) {
            receiver->jitterAverage += (change - receiver->jitterAverage) >> 4;
            receiver->unk18 = receiver->jitterAverage;
        }
    }
    playTime = remote + receiver->firstArrival + receiver->playoutDelay;
    packet->playTime = playTime;
    now = OS_GetTick();
    if (packet->playTime + 0x8b18 < now) {
        return FALSE;
    }
    if (packet->playTime > now + 0x7fd88) {
        return FALSE;
    }
    if (receiver->burstCount <= 0) {
        int average = receiver->jitterAverage;
        int scaled = average * 3;
        int quotient = scaled / 0x8b18;

        if ((u32)scaled > receiver->delayCeiling) {
            receiver->delayCeiling = (quotient + 1) * 0x8b18 + 0x134df;
            receiver->delayFloor = receiver->delayCeiling - 0xd0a4;
            receiver->playoutDelay = quotient * 0x8b18 + 0x134df;
            receiver->belowCount = 0;
        } else if (scaled < receiver->delayFloor) {
            receiver->belowCount++;
            if (receiver->belowCount > 0x46) {
                receiver->delayCeiling = (quotient + 1) * 0x8b18 + 0x134df;
                receiver->delayFloor = quotient * 0x8b18 - 0x458c;
                if (receiver->delayFloor < 0) {
                    receiver->delayFloor = 0;
                }
                receiver->playoutDelay = quotient * 0x8b18u + 0x134df;
                receiver->belowCount = 0;
            }
        }
        if (receiver->playoutDelay > 0x68520) {
            receiver->playoutDelay = 0x68520;
        }
        if (receiver->warmup < 0x10) {
            receiver->warmup++;
            receiver->skewAverage = skew;
            receiver->skewFirst = skew;
        } else {
            receiver->skewAverage = (receiver->skewAverage * 31 + skew) / 32;
        }
        receiver->skewDifference = receiver->skewFirst - receiver->skewAverage;
        if (receiver->skewDifference > 0x8701) {
            trend = 1;
        }
        if (receiver->skewDifference < -0x8701) {
            trend = -1;
            receiver->unk70++;
        }
        if (trend != 0) {
            receiver->warmup = 0;
            receiver->firstTimestamp = 0;
        }
    }
    receiver->burstCount--;
    return TRUE;
}
