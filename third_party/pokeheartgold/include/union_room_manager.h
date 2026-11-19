#ifndef POKEHEARTGOLD_UNION_ROOM_MANAGER_H
#define POKEHEARTGOLD_UNION_ROOM_MANAGER_H

// UnionRoomManager: fieldSystem->unk80, Heap_Alloc(HEAP_ID_31, sizeof(UnionRoomManager)) in
// sub_02059E1C (DS 0x190). Formerly also seen as UnkStruct_02059E1C.

#include "mail_message_types.h"

struct FieldSystem;
struct PlayerProfile;
struct SaveData;
struct SysTask;
struct TrainerCase;
struct WMBssDesc;

typedef struct UnionRoomManager {
    struct FieldSystem *fieldSystem;                // 0x000
    struct SaveData *saveData;                      // 0x004
    struct PlayerProfile *trainerInfo;              // 0x008
    struct SysTask *sysTask;                        // 0x00C
    void (*task)(struct UnionRoomManager *manager); // 0x010
    int timer;                                      // 0x014
    u32 connectionID;                               // 0x018
    u32 connectState;                               // 0x01C enum UnionRoomConnectState
    u32 pendingConnection;                          // 0x020 enum UnionRoomPendingConnection
    int unused_24;
    int unused_28;
    int unused_2C;
    u32 peerActivity;
    u32 proposedActivity;
    u32 unused_38;
    BOOL answerDeclined;
    u32 activityAnswer;
    BOOL chatEnded;
    u16 unused_48[10][2];                           // 0x048
    u16 unused_70[40][2];                           // 0x070
    struct WMBssDesc *bssDesc[16];                  // 0x110
    MATHRandContext32 rand;                         // 0x150
    int unused_168;
    int unused_16C;
    int unused_170;
    u16 unused_174;
    u8 partySelectionResults[2];                    // 0x176
    MailMessage pendingSentence;                    // 0x178
    BOOL sentencePending;                           // 0x180
    struct TrainerCase *trainerCase;                // 0x184
    struct TrainerCase *receivedTrainerCases[2];    // 0x188
} UnionRoomManager;

// sizeof(UnionRoomManager) == 0x190

struct TrainerCase *sub_0205ABD8(struct UnionRoomManager *manager);
void sub_0205AC4C(struct UnionRoomManager *manager);
void sub_0205AC70(struct UnionRoomManager *manager);

#endif // POKEHEARTGOLD_UNION_ROOM_MANAGER_H
