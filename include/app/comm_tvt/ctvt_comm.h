// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_tvt/ctvt_comm.h: the lines in use
// so far.
#ifndef BWPORT_APP_COMM_TVT_CTVT_COMM_H
#define BWPORT_APP_COMM_TVT_CTVT_COMM_H

#include "types.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "app/comm_tvt/draw_system.h"

// Black 2's; twinned code confirms the values of CTVT_CONNECT_DISCONNECT and CTVT_CONNECT_EXISTING in Black
// How to connect, which CtvtComm_SetNextConnectType asks for
enum {
    CTVT_CONNECT_NONE,
    CTVT_CONNECT_DISCONNECT,
    CTVT_CONNECT_PARENT,
    CTVT_CONNECT_SCAN,
    CTVT_CONNECT_MAC,
    CTVT_CONNECT_EXISTING,
};

// Black 2's; twinned code confirms the offsets of memberCount, cameraEnabled, inviteOnly and inviteMacs in Black
// The beacon of this machine, which the machines that scan see
struct CtvtCommBeacon {
    PlayerInfo player;
    u8 memberCount;
    u8 cameraEnabled : 1;
    // Only the machines listed in inviteMacs may join
    u8 inviteOnly : 1;
    u8 inviteMacs[3][6];
};

// pokebw2's CtvtComm_Create
CtvtComm *func_ov172_021b7ba8(CommTvtWork *sys, HeapID heapId);
// The beacon this machine sends
CtvtCommBeacon *func_ov172_021b8b7c(CommTvtWork *sys, CtvtComm *comm);
// pokebw2's CtvtComm_StartSync
// Sends the timing number no to the other members, to synchronize on
void func_ov172_021b8b34(CommTvtWork *sys, CtvtComm *comm, u32 no);
// pokebw2's CtvtComm_IsSynced
// Whether the other members sent the timing number no
BOOL func_ov172_021b8b48(CommTvtWork *sys, CtvtComm *comm, u32 no);
// Starts the timer that ends a call nobody answers
void func_ov172_021b8c14(CommTvtWork *sys, CtvtComm *comm);
// Whether that timer is done
BOOL func_ov172_021b8c20(CommTvtWork *sys, CtvtComm *comm);
// Whether the comm is connected
BOOL func_ov172_021b8108(CommTvtWork *sys, CtvtComm *comm);
// pokebw2's CtvtComm_SetNextConnectType
void func_ov172_021b80dc(CommTvtWork *sys, CtvtComm *comm, u32 type);
// pokebw2's CtvtComm_SetParentMac
void func_ov172_021b80ec(CommTvtWork *sys, CtvtComm *comm, const u8 *mac);
// pokebw2's CtvtComm_IsInvited
// Whether mac is one of the machines the beacon invites
BOOL func_ov172_021b85d0(const CtvtCommBeacon *beacon, const u8 *mac);

// Black 2's; twinned code confirms the values of CTVT_PACKET_READY in Black
// What the commands of type 0x2000 tell the other members
enum {
    CTVT_PACKET_READY,
    CTVT_PACKET_TALKER,
    CTVT_PACKET_ZOOM,
    CTVT_PACKET_DRAW_INDEX,
    CTVT_PACKET_UNK4,
    CTVT_PACKET_UNK5,
    CTVT_PACKET_REQUEST_TALK,
    CTVT_PACKET_PLAY_VOICE,
    CTVT_PACKET_CANCEL_TALK,
    CTVT_PACKET_TALK_DONE,
    CTVT_PACKET_UNK_A,
};

// pokebw2's CtvtComm_Disconnect
void func_ov172_021b8084(CommTvtWork *sys, CtvtComm *comm);
// pokebw2's CtvtComm_SendPacket
BOOL func_ov172_021b8740(CommTvtWork *sys, CtvtComm *comm, u8 type, u32 value);
// pokebw2's CtvtComm_GetDrawSlot
DrawCommand *func_ov172_021b8bb8(CommTvtWork *sys, CtvtComm *comm, BOOL *full);
// pokebw2's CtvtComm_CommitDrawSlot
void func_ov172_021b8bfc(CommTvtWork *sys, CtvtComm *comm);
// pokebw2's CtvtComm_GetConnectType
int func_ov172_021b80e4(CommTvtWork *sys, CtvtComm *comm);

// Black 2's without its canExchangePhotos; Black sends 0x21 bytes
// What each member tells the others about itself. The player's info is a copy of its bytes, so this is sent
// without padding
struct CtvtCommMemberInfo {
    u8 playerInfo[sizeof(PlayerInfo)];
    u8 cameraEnabled;
};

// pokebw2's CtvtComm_IsMemberActive
BOOL func_ov172_021b8ba4(CommTvtWork *sys, CtvtComm *comm, u8 member);
// pokebw2's CtvtComm_IsMemberInfoReceived
BOOL func_ov172_021b8b8c(CommTvtWork *sys, CtvtComm *comm, u8 member);
// pokebw2's CtvtComm_GetMemberInfo
CtvtCommMemberInfo *func_ov172_021b8b94(CommTvtWork *sys, CtvtComm *comm, u8 member);

// Black 2's; twinned code confirms the offsets of size in Black
// A chunk of the talker's voice, packed with IMA ADPCM
struct CtvtVoicePacket {
    u16 size;
    u16 playSize;
    u8 isLast;
    u8 chunk;
    // An index into the playback speeds
    u8 speed;
    u8 padding;
    s8 data[0x200];
};

// pokebw2's CtvtComm_IsVoiceBusy
BOOL func_ov172_021b8b74(CommTvtWork *sys, CtvtComm *comm);
// pokebw2's CtvtComm_SendVoice
BOOL func_ov172_021b891c(CommTvtWork *sys, CtvtComm *comm, CtvtVoicePacket *packet);

// pokebw2's CtvtComm_IsDone
BOOL func_ov172_021b80cc(CommTvtWork *sys, CtvtComm *comm);
// pokebw2's CtvtComm_GetTalker
u8 func_ov172_021b8b68(CommTvtWork *sys, CtvtComm *comm);
// pokebw2's CtvtComm_GetVoicePacket
void *func_ov172_021b8b84(CommTvtWork *sys, CtvtComm *comm);
// pokebw2's CtvtComm_GetSelfInfo
CtvtCommMemberInfo *func_ov172_021b8b9c(CommTvtWork *sys, CtvtComm *comm);
// pokebw2's CtvtComm_HasMemberCamera
BOOL func_ov172_021b8bac(CommTvtWork *sys, CtvtComm *comm, u8 member);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CtvtComm_SendZoom
void func_ov172_021b8720(CommTvtWork *sys, CtvtComm *comm, BOOL zoomed);
// pokebw2's CtvtComm_GetUnk3c8
BOOL func_ov172_021b8c34(CommTvtWork *sys, CtvtComm *comm);
// pokebw2's CtvtComm_Delete
void func_ov172_021b7dcc(CommTvtWork *sys, CtvtComm *comm);
// pokebw2's CtvtComm_Update
void func_ov172_021b7e34(CommTvtWork *sys, CtvtComm *comm);

// Black 2's, not checked against Black
#define CTVT_COMM_MEMBERS 4
#define CTVT_COMM_PICTURE_BUFFERS 3

#define CTVT_COMM_DRAW_QUEUE_SIZE 64

// Black 2's, not checked against Black
typedef struct {
    BOOL active;
    BOOL infoReceived;
    BOOL infoApplied;
    BOOL isSelf;
    BOOL wantsToTalk;
    u8 pictureBuffer;
    int pictureState;
    CtvtCommMemberInfo info;
} CtvtCommMember;

// Black 2's up to the draw queue, Black's own from there on: Black has none of Black 2's minigame fields (its size
// is 0x3e0)
struct CtvtComm {
    u32 unk0;
    u8 memberCount;
    int netState;
    CtvtCommBeacon beacon;
    CtvtCommMemberInfo selfInfo;
    CtvtCommMember members[CTVT_COMM_MEMBERS];
    void *pictureBuffers[CTVT_COMM_PICTURE_BUFFERS];
    u8 pictureBufferMask;
    int connectType;
    int nextConnectType;
    u8 parentMac[6];
    // The members to send the own picture to, and those ready for it
    u8 photoTargetMask;
    u8 photoReadyMask;
    BOOL photoSending;
    void *jpegWork;
    void *photoRaw;
    void *photoJpeg;
    BOOL voiceBusy;
    BOOL voiceReady;
    u16 voiceSize;
    u32 voiceSpeed;
    CtvtVoicePacket *voicePacket;
    s16 *voiceBuffer;
    u8 drawWriteIndex;
    u8 drawReadIndex;
    DrawCommand drawQueue[CTVT_COMM_DRAW_QUEUE_SIZE];
    u8 talker;
    BOOL talkRequestsChanged;
    BOOL sendInfo;
    BOOL sendZoom;
    BOOL zoom;
    BOOL unk3c0;
    BOOL sendUnk3c0;
    BOOL unk3c8;
    BOOL sendUnk3c8;
    u16 inviteTimer;
    BOOL sendTalker;
    u8 nextTalker;
    CommTvtWork *sys;
};

#endif // BWPORT_APP_COMM_TVT_CTVT_COMM_H
