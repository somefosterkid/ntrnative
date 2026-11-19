#ifndef POKEHEARTGOLD_COMM_PLAYER_MANAGER_H
#define POKEHEARTGOLD_COMM_PLAYER_MANAGER_H

// hk400: the heap block the comm field-player system (unk_02056D7C_*) keeps in _021D41C4 (Heap_Alloc'd by the
// unk_02058034 comm start-up with sub_02057180() = 0xF4 bytes on the DS). The comments are the DS offsets.

struct FieldSystem;
struct PlayerAvatar;
struct SysTask;

typedef struct CommPlayerPosition {
    u16 x;
    u16 z;
    s8 facing;
    u8 speed;
    u8 unk_6;
    u8 bumped;
} CommPlayerPosition; // 0x8

typedef struct CommPlayerManager {
    u8 unk_00[4]; // 0x0
    struct PlayerAvatar *avatars[8]; // 0x4
    u8 active[8]; // 0x24
    struct SysTask *task; // 0x2C
    struct FieldSystem *fieldSystem; // 0x30
    CommPlayerPosition hostPositions[8]; // 0x34
    CommPlayerPosition positions[8]; // 0x74
    u8 moveEnabled[8]; // 0xB4
    u8 inputLocked[8]; // 0xBC
    u8 unk_C4[8]; // 0xC4
    u8 positionChanged[8]; // 0xCC
    u8 hostMoveTimer[8]; // 0xD4
    u8 moveTimer[8]; // 0xDC
    u8 unk_E4[2]; // 0xE4
    u8 lockOnSpot[5]; // 0xE6
    u8 localPositionSent; // 0xEB
    u8 sendAllPositions; // 0xEC
    u8 localMoveEnabled; // 0xED
    u8 unk_EE; // 0xEE
    u8 fieldResumed; // 0xEF
    u8 localMoveDelay; // 0xF0
    u8 localWalkEnabled; // 0xF1
    u8 unk_F2[2]; // 0xF2
} CommPlayerManager; // 0xF4

extern CommPlayerManager *_021D41C4;

s32 sub_02056D7C(CommPlayerManager *manager, struct FieldSystem *fieldSystem, s32 unused);
void sub_0205704C(int netId, const CommPlayerPosition *position);
u32 sub_02057180(void);
// The callers (task func sub_020572DC, sub_020573F0) pass the FieldSystem / the manager; the bodies read _021D41C4 instead.
void sub_020570C8(struct FieldSystem *unused);
void sub_0205712C(struct FieldSystem *unused);
void sub_02057314(struct FieldSystem *unused);
void sub_02057550(s32 unused, CommPlayerManager *manager);

#endif // POKEHEARTGOLD_COMM_PLAYER_MANAGER_H
