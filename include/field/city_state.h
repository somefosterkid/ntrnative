// Black City's or White Forest's state and residents, and the messages of the city's communication, the functions of
// overlay 10's city_state.c. Only func_ov010_02161d64 and func_ov010_02162370 have twins in pokebw2 (GPL-3.0,
// https://github.com/fuddlesworth/pokebw2), and func_ov010_02161d98 starts as its func_ov012_0215cd8c; pokebw2's
// CityState is the first two fields of Black's. The type and field names are guesses from the code.
#ifndef BWPORT_FIELD_CITY_STATE_H
#define BWPORT_FIELD_CITY_STATE_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

#define CITY_RESIDENT_MAX 10

// A resident of the city
typedef struct {
    u8 exists : 4;
    // Whether name holds a name, which must end within it
    u8 hasName : 4;
    // Below 30
    u8 id;
    // At most 100
    u8 unk2;
    u8 unk3;
    u16 name[8];
    u32 unk14;
} CityResident;

struct CityState {
    u16 initialized;
    // 0 or 1, which of the version's city it is, or 2 before it is set
    u16 city;
    u32 unk4;
    // The lists of func_ov010_02161dc4, 0 and 1
    CityResident residents[CITY_RESIDENT_MAX];
    CityResident visitors[CITY_RESIDENT_MAX];
};

// The two values func_ov010_02161f14 reads from the first resident's data
typedef struct {
    u16 unk0;
    u16 unk2;
} CityResidentInfo;

// A message of the city's communication: the sender's network ID and two values
typedef struct {
    u32 netId;
    u16 unk4;
    u16 unk6;
} CityCommMessage;

// The messages exchanged with the other machines, each valid while its bit of flags is set: bits 0 to 2 for sent,
// bit 3 for received, bit 4 for unk28 and bits 5 to 7 for replies
typedef struct {
    u16 netId;
    u16 flags;
    u16 unk4;
    u16 unk6;
    CityCommMessage sent[3];
    CityCommMessage received;
    CityCommMessage unk28;
    CityCommMessage replies[3];
} CityComm;

void func_ov010_02161d64(CityState *state);
void func_ov010_02161d98(CityState *state);
BOOL func_ov010_02161e4c(CityState *state, u32 id, int list);
void func_ov010_02161e60(CityState *state, int list);
void func_ov010_02161e78(CityResident *resident);
BOOL func_ov010_02161e84(const CityResident *resident);
BOOL func_ov010_02161ecc(const CityResident *resident);
void func_ov010_02161eec(u16 *ids);
void func_ov010_02161f00(CityResidentInfo *info);
void func_ov010_02161f14(CityResidentInfo *info, CityState *state, HeapID heapId);
void func_ov010_02161f54(CityComm *comm);
void func_ov010_02161f68(CityComm *comm);
void func_ov010_02161f74(CityComm *comm, CityState *state, u32 mask);
void func_ov010_02161f90(CityComm *comm, CityState *dest, CityState *src, PlayerInfo *player, u16 id, HeapID heapId);
void func_ov010_02161fec(CityComm *comm, u8 value);
void func_ov010_02161ff0(CityComm *comm, const CityCommMessage *message);
void func_ov010_02162008(CityComm *comm, u16 index, const CityCommMessage *message);
BOOL func_ov010_02162024(CityComm *comm, u16 index, CityCommMessage *dest);
BOOL func_ov010_0216202c(CityComm *comm, CityCommMessage *dest, u8 *value);
void func_ov010_02162038(CityComm *comm, u16 index);
void func_ov010_02162040(CityComm *comm);
void func_ov010_02162048(CityComm *comm, u16 value, u32 kind);
BOOL func_ov010_0216208c(CityComm *comm, u16 unused);
u16 func_ov010_0216209c(CityComm *comm);
void func_ov010_021620a0(CityComm *comm);
BOOL func_ov010_02162370(s32 value);

// Overlay 20: a resident's data from its archive, loaded with func_ov020_021867c4 and freed with func_ov020_02186804
typedef struct {
    u16 unk0[2];
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u8 unkA[0x12];
    u16 unk1C;
    u16 unk1E;
    u16 unk20;
    u16 unk22;
    u16 unk24;
    u16 unk26;
    u16 unk28;
    u16 unk2A;
    u16 unk2C;
    u16 unk2E;
    // Cut short: the data is 0x30 bytes
} CityResidentData;

// The handle func_ov020_021867c4 opens (0x38 bytes): archive 0x9a and the resident data last read from it
typedef struct {
    ArcTool *arc;
    u32 id;
    CityResidentData data;
} CityResidentDataHandle;

// Makes a city state another player sent fit this game: its city from the version of the player
void func_ov020_021861f4(CityState *state, PlayerInfo *info);
CityResidentDataHandle *func_ov020_021867c4(u32 id, HeapID heapId);
CityResidentData *func_ov020_0218682c(CityResidentDataHandle *handle);
void func_ov020_02186804(CityResidentDataHandle *handle);
void func_ov020_02186520(CityState *state, PlayerInfo *player, const CityResident *resident);

#endif // BWPORT_FIELD_CITY_STATE_H
