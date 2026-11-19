#ifndef POKEHEARTGOLD_FIELD_COMMUNICATION_MANAGER_H
#define POKEHEARTGOLD_FIELD_COMMUNICATION_MANAGER_H

#include "global.h"

struct FieldSystem;
struct Party;
struct SysTask;
struct TrainerCase;

typedef void (*FieldCommTask)(void);

#define FIELD_COMM_MAX_PLAYERS 4

typedef struct FieldCommunicationManager {
    struct TrainerCase *trainerCase[FIELD_COMM_MAX_PLAYERS]; // 0x00
    u8 trainerCaseCopied[FIELD_COMM_MAX_PLAYERS];            // 0x10
    struct FieldSystem *fieldSystem;                         // 0x14
    MATHRandContext32 rand;                                  // 0x18
    FieldCommTask task;                                      // 0x30
    struct SysTask *sysTask;                                 // 0x34
    u16 timer;                                               // 0x38
    u8 connectID;                                            // 0x3A
    u8 padding_3B;                                           // 0x3B
    u8 pauseTask;                                            // 0x3C
    u8 isUnderground;                                        // 0x3D
    u8 isReturningFromBattle;                                // 0x3E
    u8 battleRoomMovement;                                   // 0x3F
    struct Party *party;                                     // 0x40
} FieldCommunicationManager;                                 // size = 0x44

extern FieldCommunicationManager *_021D41C8;

#endif // POKEHEARTGOLD_FIELD_COMMUNICATION_MANAGER_H
