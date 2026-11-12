// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/snd.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_NITRO_SND_H
#define BWPORT_NITRO_SND_H

#include "types.h"

// NitroSDK's enums (ints)
typedef int SNDWaveFormat;
typedef int SNDChannelLoop;
typedef int SNDChannelDataShift;
typedef int SNDOutput;
typedef int SNDChannelOut;

// Channels: SND_SetChannelVolume, SND_SetChannelPan and SND_SetupChannelPcm, under swan's names
#define SND_WAVE_FORMAT_PCM8 0

#define SND_WAVE_FORMAT_PCM16 1
#define SND_CHANNEL_LOOP_REPEAT 1
#define SND_CHANNEL_VOLUME_MAX 127
#define SND_CHANNEL_PAN_CENTER 64

// Timers and alarms: NitroSDK's SND_StartTimer, SND_StopTimer and SND_SetupAlarm, by their code
typedef void (*SNDAlarmHandler)(void *arg);

void SND_StartTimer(u32 chBitMask, u32 capBitMask, u32 alarmBitMask, u32 flags);
void SND_StopTimer(u32 chBitMask, u32 capBitMask, u32 alarmBitMask, u32 flags);
void SND_SetupChannelPcm(int chNo, SNDWaveFormat format, const void *dataAddr, SNDChannelLoop loop, int loopStart,
                         int dataLen, int volume, SNDChannelDataShift shift, int timer, int pan);
void SND_SetupAlarm(int alarmNo, u32 tick, u32 period, SNDAlarmHandler handler, void *arg);
u16 SND_CalcChannelVolume(int dB);

// NitroSDK's decibel table (snd_util.c), by volume (0 to 127), and SND_CalcDecibel, its read (nitro/snd/common/util.h)
#define SND_DECIBEL_TABLE_SIZE 128

extern const s16 SNDi_DecibelTable[SND_DECIBEL_TABLE_SIZE];

static inline s16 SND_CalcDecibel(int scale) {
    return SNDi_DecibelTable[scale];
}
void SND_SetChannelVolume(u32 chBitMask, int volume, int shift);
// NitroSDK's, which Black lacks (only stream.c's NNS_SndStrmSetChannelPan calls it, and Black's link drops that)
void SND_SetChannelPan(u32 chBitMask, int pan);

// Commands: NitroSDK's SND_FlushCommand and SND_WaitForCommandProc by their code, and SND_GetCurrentCommandTag under
// swan's name
#define SND_COMMAND_BLOCK 1

// NitroSDK's SND_CalcChannelVolume: a volume in decibels as a channel volume in the low byte and a data shift in the
// high one
#define SND_CHANNEL_VOLUME(v) ((u8)(v))

#define SND_CHANNEL_DATASHIFT(v) ((v) >> 8)

// NitroSDK's SND_SetChannelVolume and SND_StopTimer, by their code (lib/nitro/src/snd/snd_interface.c)
void SND_SetChannelTimer(u32 chBitMask, int timer);
u32 SND_GetChannelStatus(void);

// Sound data files: a sequence, a bank of instruments and a wave archive, each a binary file with one data block
typedef struct SNDBinaryFileHeader {
    char signature[4];
    u16 byteOrder;
    u16 version;
    u32 fileSize;
    u16 headerSize;
    u16 dataBlocks;
} SNDBinaryFileHeader;

typedef struct SNDBinaryBlockHeader {
    u32 kind;
    u32 size;
} SNDBinaryBlockHeader;

typedef struct SNDSequenceData {
    SNDBinaryFileHeader fileHeader;
    SNDBinaryBlockHeader blockHeader;
    u32 baseOffset;
    u32 data[];
} SNDSequenceData;

typedef struct SNDBankData SNDBankData;
typedef struct SNDWaveData SNDWaveData;

// A wave archive: its waves' offsets in the file. Loaded in the heap, the table holds the waves' addresses
typedef struct SNDWaveArc {
    SNDBinaryFileHeader fileHeader;
    SNDBinaryBlockHeader blockHeader;
    struct SNDWaveArcLink *topLink;
    u32 reserved[7];
    u32 waveCount;
    u32 waveOffset[];
} SNDWaveArc;

// An instrument of a bank, and where SND_GetNextInstData goes on from
#define SND_INST_PCM 1

typedef struct SNDInstParam {
    u16 wave[2];
    u8 original_key;
    u8 attack;
    u8 decay;
    u8 sustain;
    u8 release;
    u8 pan;
} SNDInstParam;

typedef struct SNDInstData {
    u8 type;
    u8 padding_;
    SNDInstParam param;
} SNDInstData;

typedef struct SNDInstPos {
    u32 prgNo;
    u32 index;
} SNDInstPos;

// The driver: NitroSDK's SND_Init, SND_RecvCommandReply, which returns the next command the ARM7 is done with or NULL,
// and SND_IsFinishedCommandTag, by their code
#define SND_COMMAND_NOBLOCK 0

typedef struct SNDCommand SNDCommand;

const SNDCommand *SND_RecvCommandReply(u32 flags);

void SND_LockChannel(u32 chBitMask, u32 flags);

// NitroSDK's SND_LockChannel and SND_UnlockChannel, by their code; SND_SetChannelTimer and SND_SetMasterVolume under
// swan's names; SND_SetOutputSelector, SND_SetMasterPan and SND_ResetMasterPan by their code
#define SND_CHANNEL_TIMER_MIN 0x10

#define SND_CHANNEL_TIMER_MAX 0xffff
#define SND_CHANNEL_LOOP_1SHOT 2
#define SND_CHANNEL_DATASHIFT_NONE 0

// The channels' timer OS_GetTick, half the system OS_GetTick
#define SND_TIMER_CLOCK 16756991

// What the driver reports: NitroSDK's SND_ReadDriverInfo, which copies the driver's state to info,
// SND_GetPlayerStatus and SND_GetChannelStatus, bit masks of the players and channels at work, and SND_ReadTrackInfo
// from a copied state, by their code, and SND_GetPlayerTickCounter. Only the size of the driver's state is used here
typedef struct SNDDriverInfo {
    u8 data[0x11e0];
} SNDDriverInfo;

void SND_ReadDriverInfo(SNDDriverInfo *info);

// A track's state, as NNS_SndPlayerReadDriverTrackInfo reads it. Only its size is used here
typedef struct SNDTrackInfo {
    u8 data[0x1a];
} SNDTrackInfo;

u32 SND_GetWaveDataCount(const SNDWaveArc *waveArc);
SNDInstPos SND_GetFirstInstDataPos(const SNDBankData *bank);
BOOL SND_GetNextInstData(const SNDBankData *bank, SNDInstData *inst, SNDInstPos *pos);

void SND_SetWaveDataAddress(SNDWaveArc *waveArc, int index, const SNDWaveData *address);
const SNDWaveData *SND_GetWaveDataAddress(const SNDWaveArc *waveArc, int index);
void SND_AssignWaveArc(SNDBankData *bank, int index, SNDWaveArc *waveArc);
void SND_DestroyBank(SNDBankData *bank);
void SND_DestroyWaveArc(SNDWaveArc *waveArc);
void SND_InvalidateSeqData(const void *start, const void *end);
void SND_InvalidateBankData(const void *start, const void *end);
void SND_InvalidateWaveData(const void *start, const void *end);

// The SDK's sequence player calls, by player number (NitroSDK's SND_SetTrackMute and the rest of its interface)
void SND_SetPlayerChannelPriority(int playerNo, int prio);
void SND_SetTrackMute(int playerNo, u32 trackBitMask, BOOL flag);
void SND_SetTrackMuteEx(int playerNo, u32 trackBitMask, int mute);
void SND_SetTrackVolume(int playerNo, u32 trackBitMask, int volume);
void SND_SetTrackPitch(int playerNo, u32 trackBitMask, int pitch);
void SND_SetTrackPan(int playerNo, u32 trackBitMask, int pan);
void SND_SetTrackModDepth(int playerNo, u32 trackBitMask, int depth);
void SND_SetTrackModSpeed(int playerNo, u32 trackBitMask, int speed);
void SND_SetPlayerTempoRatio(int playerNo, int ratio);
void SND_PauseSeq(int playerNo, BOOL flag);
void SND_SetPlayerVolume(int playerNo, int volume);
void SND_StopSeq(int playerNo);
void SND_StartPreparedSeq(int playerNo);
u32 SND_GetPlayerStatus(void);
#define SND_VOLUME_DB_MIN -723
void SND_PrepareSeq(int playerNo, const void *base, u32 offset, const SNDBankData *bank);
void SND_SetTrackAllocatableChannel(int playerNo, u32 trackBitMask, u32 chBitMask);

// NitroSDK's SND_UnlockChannel, by its code: releases the channels in chBitMask from the locks of flags
void SND_UnlockChannel(u32 chBitMask, u32 flags);

// NitroSDK's SND_SetOutputSelector, by its code (lib/nitro/src/snd/snd_interface.c): the outputs of the left and right
// speakers, and the output of channels 1 and 3
void SND_SetOutputSelector(SNDOutput left, SNDOutput right, SNDChannelOut channel1, SNDChannelOut channel3);

// The queue of snd_command.c: the pool's free commands, the ready ones that the next flush sends as a packet, the
// packets that the ARM7 has not returned yet, and the tags that tell them apart. The pool's commands are chained, and
// a packet is the chain of the ready commands
#define SND_PXI_FIFO_MESSAGE_BUFSIZE 8

typedef struct SNDCommandQueue {
    SNDCommand *freeHead;  // the pool's commands that nothing uses
    u32 receivedTag;       // the tag of the last packet taken back from the ARM7
    SNDCommand *readyHead; // the commands pushed since the last flush
    SNDCommand *readyTail;
    SNDCommand *freeTail;
    int sentRead;  // where the next packet to come back is in sWaitingCommandListQueue
    int sentWrite; // where the next packet to go is
    int sentCount; // how many packets are on their way
    u32 nextTag;   // the tag of the packet that the ready commands will make
} SNDCommandQueue;

void SND_CommandInit(void);
// The alarms' handlers, which snd_alarm.c keeps: NitroSDK's SND_AlarmInit, SNDi_IncAlarmId, SNDi_SetAlarmHandler, which
// returns the alarm's new ID, and SNDi_CallAlarmHandler, which takes the number and the ID that the ARM7 sends
void SND_AlarmInit(void);
void SND_Init(void);
void SNDi_LockMutex(void);
void SNDi_UnlockMutex(void);

// What the driver has: players, 16 tracks for each and 16 variables, channels and alarms
#define SND_PLAYER_NUM 16
#define SND_TRACK_NUM_PER_PLAYER 16
#define SND_PLAYER_VARIABLE_MAX 16
#define SND_CHANNEL_NUM 16
#define SND_ALARM_NUM 8


typedef enum {
    SND_SEQ_MUTE_OFF,
    SND_SEQ_MUTE_NO_STOP,
    SND_SEQ_MUTE_RELEASE,
    SND_SEQ_MUTE_STOP,

} SNDSeqMute;



BOOL SND_FlushCommand(u32 flags);

u32 SND_GetCurrentCommandTag(void); // swan's sndGetSentPacketCount

// NitroSDK's SND_CalcChannelVolume: a volume in decibels as a channel volume in the low byte and a data shift in the
// high one
#define SND_CHANNEL_VOLUME(v) ((u8)(v))
#define SND_CHANNEL_DATASHIFT(v) ((v) >> 8)


u16 SND_CalcChannelVolume(int dB);

u32 SND_GetPlayerTickCounter(int playerNo);

BOOL SND_IsFinishedCommandTag(u32 tag);

BOOL SND_ReadTrackInfo(const SNDDriverInfo *driverInfo, int playerNo, int trackNo, SNDTrackInfo *trackInfo);

void SND_ResetMasterPan(void);

void SND_SetMasterPan(int pan);

void SND_SetMasterVolume(int volume);               // swan's sndSetMasterVolume
void SND_SetOutputSelector(SNDOutput left, SNDOutput right, SNDChannelOut channel1, SNDChannelOut channel3);

void SND_WaitForCommandProc(u32 tag);


#endif // BWPORT_NITRO_SND_H
