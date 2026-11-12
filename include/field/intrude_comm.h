// Overlay 11's intrude_comm.c, named by the ROM's string: the communication with other players' games in the Entralink,
// and its missions. Black's own; Black 2 kept only stubs of it. The layouts come from Black's code (pokebw2 has none)
#ifndef BWPORT_FIELD_INTRUDE_COMM_H
#define BWPORT_FIELD_INTRUDE_COMM_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "field/city_state.h"
#include "field/comm_player.h"
#include "field/symbol_map.h"
#include "save/high_link.h"
#include "save/player_info.h"

#define INTRUDE_PLAYER_MAX 3
// A net ID that is no player
#define INTRUDE_NET_ID_NONE 4

// What the game puts in its beacon while it waits for players
typedef struct {
    u8 unk_00;
    // How many machines are connected, and the mode the communication was started in
    u8 unk_01_0 : 4;
    u8 unk_01_4 : 4;
    u8 unk_02_0 : 1;
    u8 unk_02_1 : 5;
    u8 unk_02_6 : 2;
    u8 unk_03[5];
    PlayerInfo player;
    u8 unk_28[0x3c];
} IntrudeBeacon;

// Where a player is and what it is doing, as the players send it to each other: the position, then the zone, the
// season (unk_1A_6) and the game version (unk_1B_0)
typedef struct {
    CommPlayerStatus status;
    u16 zoneId;
    u16 unk_16_0 : 3;
    // GameData_Is30FPSMode's
    u16 unk_16_3 : 6;
    u16 unk_16_9 : 6;
    // The lid is closed (PAD_DetectFold)
    u16 unk_16_15 : 1;
    u16 unk_18;
    u8 unk_1A_0 : 4;
    u8 unk_1A_4 : 1;
    u8 unk_1A_5 : 1;
    u8 unk_1A_6 : 2;
    u8 unk_1B_0 : 6;
    u8 unk_1B_6 : 1;
    u8 unk_1B_7 : 1;
} IntrudeCommUnk8C;

// A player's data as it is sent when it joins
typedef struct {
    PlayerInfo info;
    PassPowerLevel power;
    IntrudeCommUnk8C status;
} IntrudePlayerData;

// A mission
typedef struct {
    u32 unk_00_0 : 15;
    u32 unk_00_15 : 7;
    // The time the mission takes, in seconds
    u32 unk_00_22 : 10;
    // The kind of the mission, which picks its title and how its text names the target
    u32 kind : 4;
    u32 unk_04_4 : 3;
    u32 unk_04_7 : 11;
    // The mission's text
    u32 msgId : 11;
    u32 unk_04_29 : 3;
    u8 unk_08_0 : 3;
    u8 unk_08_3 : 5;
    u8 unk_09;
    u16 unk_0A[3];
    u8 unk_10[3];
    u8 unk_13;
    u16 unk_14;
    u16 unk_16;
} IntrudeMission;

// A player that a mission's text names
typedef struct {
    u16 name[8];
    // The net ID
    u8 unk_10;
    u8 gender : 1;
    u8 unk_11_1 : 7;
    // Copied and cleared with the rest (0x14 bytes)
    u8 unk_12[2];
} IntrudePlayer;

// A mission and the player who gave it
typedef struct {
    IntrudeMission mission;
    IntrudePlayer player;
    // The net ID of the player the mission is for, INTRUDE_NET_ID_NONE while there is none
    u8 netId;
    u8 unk_2D;
    u8 unk_2E;
    u8 unk_2F;
    u16 unk_30;
    u16 unk_32;
} IntrudeMissionEntry;

// A mission another player asks for, as it arrives
typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02[2];
    IntrudeMissionEntry entry;
} IntrudeMissionRequest;

// A mission a player entrusts to the others, with the net IDs of those who took it
typedef struct {
    IntrudeMissionEntry entry;
    u8 netIds[INTRUDE_PLAYER_MAX];
    u8 unk_37;
    u8 unk_38[4];
} IntrudeMissionEntrust;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} IntrudeUnk15C;

// An event asked of another player: its net ID, who answered, whether the answer is in, the answer, and the kind of the
// event
typedef struct {
    u8 netId;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06[2];
} IntrudeEventReq;

// The pass power levels another player sends, cut short
typedef struct {
    u8 unk_00[0x10];
    u16 unk_10;
    u16 unk_12;
} IntrudeUnk8E4;

// What a player tells of its Entralink, from the high link save
typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u8 unk_0C[2];
    u8 unk_0E;
    u8 unk_0F;
} IntrudeHighLinkData;

// A player of the mission: its pass power levels and its name
typedef struct {
    PassPowerLevel power;
    // Whether the player is set
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
    u8 unk_13;
    IntrudePlayer player;
} IntrudeCommPlayer;

// The missions' work, at 0x1ac of the communication's
typedef struct {
    IntrudeCommPlayer players[INTRUDE_PLAYER_MAX];
    // The mission in progress
    IntrudeMissionEntry entry;
    // The mission another player entrusted
    IntrudeMissionEntrust unk_0AC;
    IntrudeMissionRequest unk_0E8[INTRUDE_PLAYER_MAX];
    // When the mission started, RTC_ConvertDaySecondsCached
    u32 unk_190;
    u8 unk_194[INTRUDE_PLAYER_MAX];
    u8 unk_197[INTRUDE_PLAYER_MAX];
    u8 unk_19A;
    u8 unk_19B;
    u8 unk_19C;
    u8 unk_19D;
    u8 unk_19E;
    u8 unk_19F;
    u8 unk_1A0;
    u8 unk_1A1;
    u8 unk_1A2;
    u8 unk_1A3;
    u8 unk_1A4;
    u8 unk_1A5_0 : 1;
    u8 unk_1A5_1 : 1;
    u8 unk_1A5_2 : 1;
    u8 unk_1A5_3 : 1;
    u8 unk_1A5_4 : 1;
    u8 unk_1A5_5 : 2;
    u8 unk_1A5_7 : 1;
    u8 unk_1A6[2];
} IntrudeMissionWork;

// The communication's work
struct IntrudeComm {
    GameSystem *gsys;
    GameCommSys *commSys;
    CommPlayerSys *commPlayer;
    IntrudeBeacon beacon;
    // This player's status, and the others' by net ID
    IntrudeCommUnk8C unk_070;
    IntrudeCommUnk8C unk_08C[INTRUDE_PLAYER_MAX];
    // Where each other player was last
    CommPlayerStatus unk_0E0[INTRUDE_PLAYER_MAX];
    // The event another player asked for
    IntrudeMissionRequest unk_11C;
    IntrudeEventReq unk_154;
    IntrudeUnk15C unk_15C;
    // This player's data as it was sent
    IntrudePlayerData unk_160;
    IntrudeMissionWork unk_1AC;
    // What the mission earned, sent to the others
    IntrudeUnk15C unk_354;
    u8 unk_358[4];
    CityComm unk_35C;
    // The buffers data is sent from
    IntrudePlayerData unk_3A4;
    IntrudeMissionEntry unk_3F0;
    IntrudeMissionEntrust unk_424;
    CityState unk_460;
    // The buffers data arrives in
    IntrudeCommPlayer unk_648;
    IntrudePlayerData unk_670[INTRUDE_PLAYER_MAX];
    IntrudeMissionEntry unk_754[INTRUDE_PLAYER_MAX];
    IntrudeMissionEntrust unk_7F0;
    SymbolMapList unk_82C;
    SymbolMapList unk_880;
    IntrudeUnk15C unk_8D4[INTRUDE_PLAYER_MAX];
    u8 unk_8E0;
    u8 unk_8E1;
    u8 unk_8E2[2];
    IntrudeUnk8E4 unk_8E4;
    u8 unk_8F8_0 : 1;
    u8 unk_8F8_1 : 1;
    u8 unk_8F8_2 : 1;
    u8 unk_8F8_3 : 1;
    u8 unk_8F8_4 : 1;
    u8 unk_8F8_5 : 1;
    u8 unk_8F8_6 : 1;
    u8 unk_8F8_7 : 1;
    u8 unk_8F9;
    u8 unk_8FA;
    // A bit for each other player that has sent its data
    u8 unk_8FB;
    u8 unk_8FC;
    // The communication's state
    u8 unk_8FD;
    u8 unk_8FE;
    u8 unk_8FF;
    u8 unk_900;
    u8 unk_901;
    // How many machines are connected
    u8 unk_902;
    u8 unk_903;
    u8 unk_904_0 : 1;
    u8 unk_904_1 : 1;
    u8 unk_904_2 : 2;
    u8 unk_904_4 : 4;
    u8 unk_905;
    u8 unk_906;
    u8 unk_907;
    u8 unk_908;
    u8 unk_909;
    u16 unk_90A;
    u8 unk_90C;
    u8 unk_90D;
    s16 unk_90E;
    u8 unk_910;
    u8 unk_911;
    u8 unk_912[2];
    IntrudeHighLinkData unk_914;
    u8 unk_924;
    // The beacons seen: how many, for how long, and their IDs
    u8 unk_925;
    u8 unk_926;
    u8 unk_927;
    StrBuf *unk_928[2];
    u32 unk_930[2];
    u8 unk_938[2];
    u8 unk_93A[2];
    s8 unk_93C;
    s8 unk_93D;
    u8 unk_93E;
    u8 unk_93F_0 : 1;
    u8 unk_93F_1 : 1;
    u8 unk_93F_2 : 1;
    u8 unk_93F_3 : 1;
    u8 unk_93F_4 : 2;
    u8 unk_93F_6 : 1;
    u8 unk_93F_7 : 1;
    u32 unk_940_0 : 31;
    u32 unk_940_31 : 1;
    u8 unk_944;
    u8 unk_945[INTRUDE_PLAYER_MAX];
    u8 unk_948[INTRUDE_PLAYER_MAX];
    u8 unk_94B;
};

// The communication's work, or NULL while there is none (pokebw2's func_ov012_02153608)
IntrudeComm *func_ov010_02158344(GameCommSys *commSys);
void func_ov011_021767f0(GameCommSys *commSys, GameData *gameData, VecFx32 *pos);
BOOL func_ov011_02176850(IntrudeComm *work, u8 netId);
BOOL func_ov011_02176870(IntrudeComm *work);
void func_ov011_0217688c(IntrudeComm *work, u8 value);
// Whether a mission is in progress, and the mission, or NULL
BOOL func_ov011_02175b14(IntrudeMissionWork *work);
BOOL func_ov011_02175da8(IntrudeMissionWork *work);
// Pairs of zones that join at an edge
typedef struct {
    u16 zoneA;
    u16 zoneB;
    u8 unk_04;
    u8 unk_05;
} IntrudeZonePair;
extern const IntrudeZonePair data_ov011_02176aa0[8];
IntrudeMissionEntry *func_ov011_02175b34(IntrudeMissionWork *work);
u32 func_ov011_02176230(IntrudeComm *work, IntrudeMissionWork *missionWork);
BOOL func_ov011_02176258(IntrudeComm *work);
// Puts a mission's words in a word set: the player's name, and its target
void func_ov011_021762c0(WordSet *wordSet, u32 index, const IntrudePlayer *player, HeapID heapId);
void func_ov011_021762d8(const IntrudeMission *mission, IntrudePlayer *player, WordSet *wordSet, HeapID heapId);
// Which player's forest this one shows, by net ID, or 4 for its own
u8 func_ov011_021767a0(GameCommSys *commSys, GameData *gameData);
SymbolMapList *func_ov011_02176880(IntrudeComm *work);
BOOL func_ov011_02174660(u8 value);
// What the field and its events ask of the communication
BOOL func_ov011_02173a2c(IntrudeComm *work, u32 netId, const IntrudeMissionEntry *entry, u8 kind);
BOOL func_ov011_02173b9c(u32 netId, u8 kind);
BOOL func_ov011_02173c04(u32 netId, u8 value);
BOOL func_ov011_02173c6c(IntrudeComm *work, u8 netId);
BOOL func_ov011_02173d2c(IntrudeComm *work, u32 index);
BOOL func_ov011_02173e2c(IntrudeComm *work, const IntrudeMissionEntry *entry);
BOOL func_ov011_02173fc0(IntrudeComm *work, IntrudeMissionWork *missionWork);
BOOL func_ov011_0217405c(IntrudeComm *work, IntrudeMissionWork *missionWork);
BOOL func_ov011_02174258(IntrudeComm *work);
BOOL func_ov011_0217440c(void);
BOOL func_ov011_0217446c(void);
BOOL func_ov011_021744d0(IntrudeComm *work, u8 netId, u8 value);
BOOL func_ov011_0217456c(IntrudeComm *work, u8 netId, u32 type);
BOOL func_ov011_021745f8(u8 netId, const void *data);
BOOL func_ov011_021746ac(IntrudeComm *work, u8 netId);
void func_ov011_02175004(IntrudeComm *work);
void func_ov011_02175060(IntrudeComm *work, u32 netId);
void func_ov011_02175274(IntrudeComm *work, s32 x);
BOOL func_ov011_02175e90(IntrudeComm *comm, IntrudeMissionWork *work);
s32 func_ov011_02175de4(IntrudeComm *comm, IntrudeMissionWork *work);
void func_ov011_02175368(IntrudeComm *work);
void func_ov011_0217538c(IntrudeComm *work, u32 netId);
IntrudeUnk15C *func_ov011_0217537c(IntrudeComm *work);
u32 func_ov011_02175514(IntrudeComm *work);
void func_ov011_0217553c(IntrudeComm *work);
void func_ov011_02175548(PlayerInfo *info, u16 *view, u8 *a, u8 *b);
u16 func_ov011_02175598(const IntrudeCommUnk8C *status, PlayerInfo *info);
u8 func_ov011_021756a4(IntrudeComm *work);
void func_ov011_021756f0(IntrudeComm *work, BOOL flag);
// Sets the player whose Entralink this one is in
void func_ov011_02175710(IntrudeComm *work, u32 netId);
void func_ov011_0217574c(IntrudeComm *work, BOOL flag);
s32 func_ov011_021758c8(IntrudeMissionWork *work);
BOOL func_ov011_02175a24(IntrudeMissionWork *work);
void func_ov011_02175b4c(IntrudeComm *comm, IntrudeMissionWork *work);
BOOL func_ov011_02175b8c(IntrudeMissionWork *work);
void func_ov011_02175bac(IntrudeMissionWork *work);
BOOL func_ov011_02175bbc(IntrudeMissionWork *work);
BOOL func_ov011_02175be4(IntrudeMissionWork *work);
u32 func_ov011_02175c40(IntrudeMissionWork *work);
BOOL func_ov011_02175cf4(IntrudeComm *comm, IntrudeMissionWork *work);
void func_ov011_021753b8(IntrudeComm *work);
BOOL func_ov011_02173ae0(IntrudeComm *work, u32 netId, u8 param_3, u8 param_4);
GameEvent *func_ov011_021766cc(GameSystem *gsys, void *param_2);
void func_ov011_02175d20(IntrudeMissionWork *work);
u32 func_ov011_02175dcc(IntrudeMissionWork *work);
u8 func_ov011_02175fb4(IntrudeCommPlayer *player);
u8 func_ov011_021761ec(IntrudeMissionWork *work);
void func_ov011_02176200(IntrudeMissionWork *work);
void func_ov011_02175648(GameData *gameData, IntrudeHighLinkData *data);
BOOL func_ov011_0217689c(IntrudeComm *work, u16 zoneId);
IntrudeMissionEntrust *func_ov011_02175bcc(IntrudeMissionWork *work);
BOOL func_ov011_0217620c(IntrudeMissionWork *work, u32 netId);
void func_ov011_02174e14(IntrudeComm *work, u32 value);
void func_ov011_021757a8(IntrudeMissionWork *work);
BOOL func_ov011_021754b4(IntrudeComm *, u32 *);


#endif // BWPORT_FIELD_INTRUDE_COMM_H
