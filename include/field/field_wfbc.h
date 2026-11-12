// Black City's or White Forest's field data and the functions of overlay 21's field_wfbc.c, with the types of its
// tail ov021_02193f34.c. pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2) has FieldWFBC with city, isOther
// and state alone, at offsets 0, 1 and 4; Black's holds the residents and the city's 8x8 grid of blocks.
#ifndef BWPORT_FIELD_FIELD_WFBC_H
#define BWPORT_FIELD_FIELD_WFBC_H

#include "types.h"
#include "struct_decls.h"
#include "field/city_state.h"
#include "field/eventdata_system.h"
#include "nitro/math.h"

// Two bytes passed by value: the first half of a table entry of func_ov021_02193fe8, names by offset
typedef struct {
    u8 unk0_0 : 4;
    u8 unk0_4 : 4;
    u8 unk1;
} Ov021Entry;

// One cell of the 8x8 grid func_ov021_02193fe8 clears, names by offset
typedef struct {
    Ov021Entry entry;
    u16 unk2;
} Ov021Cell;

// The 0x24 bytes func_ov021_02194078 clears: a generator seeded from a city state, names by offset
typedef struct {
    MATHRandContext32 rand;
    u16 unk18;
    u16 unk1A;
    s32 unk1C;
    u32 unk20;
} Ov021Random;

// What func_ov021_02194128 reads its 8x8 grid from (0x600 bytes of archive 0x9a), names by offset
typedef struct {
    Ov021Entry unk0[8][8];
    u32 unk80[][16]; // indexed by Ov021Entry.unk1, then by a parity times 8; the count is unknown
} Ov021Layout;

// A resident's record (0x50 bytes): its data and its CityResident copied in, and its position
typedef struct {
    // 1 while it holds a resident
    u16 unk_00;
    u8 unk_02[2];
    CityResidentData data;
    CityResident resident;
    u16 unk_4c;
    u16 unk_4e;
} FieldWFBCPerson;

// 0x144 bytes
struct FieldWFBC {
    u8 city;
    u8 isOther;
    u8 unk_02[2];
    // CITY_RESIDENT_MAX records
    FieldWFBCPerson *people;
    CityState *state;
    // func_ov020_021867c4's
    CityResidentDataHandle *dataHandle;
    // Archive 0x9a
    ArcTool *arc;
    Ov021Layout *layout;
    Ov021Cell unk_18[8][8];
    // Two bytes a resident: func_ov020_02186830's table
    u8 *unk_118;
    Ov021Random unk_11c;
    MMSys *unk_140;
};

FieldWFBC *func_ov021_021938ac(HeapID heapId);
void func_ov021_021938d4(FieldWFBC *wfbc);
// The number of residents (field_encount.c's encounter rate)
u32 func_ov021_021938f4(FieldWFBC *wfbc);
void func_ov021_021938fc(FieldWFBC *wfbc, CityState *city);
void func_ov021_02193904(FieldWFBC *wfbc);
FieldWFBCPerson *func_ov021_0219390c(FieldWFBC *wfbc, u32 id);
u8 func_ov021_0219391c(FieldWFBC *wfbc);
u8 func_ov021_02193920(FieldWFBC *wfbc);
// Builds one of a chunk's 4x4 blocks into its terrain and props, and returns the chunk's new prop count
u32 func_ov021_02193924(FieldWFBC *wfbc, void *terrain, FieldPropSystem *propSystem, FieldChunk *chunk, u32 propCount,
                        s32 x, s32 z, HeapID heapId);
void func_ov021_021939c0(FieldWFBC *wfbc, EventDataSystem *eventData, BOOL isOther, HeapID heapId);
void func_ov021_02193abc(FieldWFBC *wfbc, FieldCamera *camera, HeapID heapId);
void func_ov021_02193afc(FieldWFBC *wfbc, MMSys *mmSys);
void func_ov021_02193b04(FieldWFBC *wfbc, CityState *city, BOOL isOther, HeapID heapId);
void func_ov021_02193b20(FieldWFBC *wfbc, u32 id);
void func_ov021_02193b40(FieldWFBC *wfbc, u32 id);
void func_ov021_02193b68(FieldWFBC *wfbc, WordSet *wordSet, u32 id, u32 bufId, HeapID heapId);
void func_ov021_02193bc4(FieldWFBC *wfbc, u32 id);
BOOL func_ov021_02193be4(FieldWFBC *wfbc, u32 id);
CityResidentData *func_ov021_02193c00(FieldWFBCPerson *person);
CityResident *func_ov021_02193c04(FieldWFBCPerson *person);
void func_ov021_02193c08(FieldWFBC *wfbc, HeapID heapId);
void func_ov021_02193c80(FieldWFBC *wfbc);
void func_ov021_02193cbc(FieldWFBC *wfbc);
s32 func_ov021_02193d08(FieldWFBC *wfbc);
void func_ov021_02193d30(FieldWFBC *wfbc, CityState *city, BOOL isOther);
void func_ov021_02193e54(FieldWFBC *wfbc, CityState *city);
void func_ov021_02193eb4(FieldWFBC *wfbc);
void func_ov021_02193ee8(FieldWFBC *wfbc);
FieldWFBCPerson *func_ov021_02193efc(FieldWFBC *wfbc, u32 id);

// ov021_02193f34.c: a resident's record, the city's 8x8 grid of blocks and its random generator
FieldWFBCPerson *func_ov021_02193f34(FieldWFBC *wfbc, u32 id);
void func_ov021_02193f6c(FieldWFBCPerson *person);
void func_ov021_02193f80(FieldWFBCPerson *person, CityResident *resident, u8 *pos, CityResidentData *data);
void func_ov021_02193fbc(FieldWFBCPerson *person, CityResident *resident);
void func_ov021_02193fd4(FieldWFBCPerson *person, CityResident *resident);
void func_ov021_02193fe8(Ov021Cell (*grid)[8]);
void func_ov021_02194014(Ov021Cell (*grid)[8], u32 x, u32 z, Ov021Entry entry, u16 value);
Ov021Cell func_ov021_02194038(Ov021Cell (*grid)[8], u32 x, u32 z);
BOOL func_ov021_02194044(Ov021Cell (*grid)[8], u32 param_2, u32 param_3);
void func_ov021_02194078(Ov021Random *random);
void func_ov021_02194084(Ov021Random *random, CityState *state, CityResidentDataHandle *handle);
void func_ov021_02194128(Ov021Random *random, Ov021Layout *layout, Ov021Cell (*grid)[8]);

#endif // BWPORT_FIELD_FIELD_WFBC_H
