#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "error_handling.h"
#include "pokemon_types_def.h"

struct Party;

struct Pokemon;

typedef struct UnkStruct_02078B40_t {
    u32 personality;
    u16 partyDecrypted : 1;
    u16 boxDecrypted : 1;
    u16 checksumFailed : 1;
    u16 : 13;
    u16 species;
    u16 heldItem;
    u32 otID;
    u32 exp;
    u8 friendship;
    u8 ability;
    u8 hpEV;
    u8 atkEV;
    u8 defEV;
    u8 speedEV;
    u8 spAtkEV;
    u8 spDefEV;
    u16 unk_1C[4];
    u8 unk_24[4];
    u8 unk_28[4];
    u32 hpIV : 5;
    u32 atkIV : 5;
    u32 defIV : 5;
    u32 speedIV : 5;
    u32 spAtkIV : 5;
    u32 spDefIV : 5;
    u32 isEgg : 1;
    u32 unk_2C_31 : 1;
    u8 fatefulEncounter : 1;
    u8 gender : 2;
    u8 form : 5;
    u16 unk_32[11];
    u16 unk_48[8];
    u8 pokeball;
    u8 originLanguage;
    u32 unk_5C;
    u8 level;
    u8 unk_61;
    u16 unk_62;
    u16 unk_64;
    u16 unk_66;
    u16 unk_68;
    u16 unk_6A;
    u16 unk_6C;
    u16 unk_6E;
} UnkStruct_02078B40;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    UnkStruct_02078B40 unk_04[6];
} UnkStruct_0202FD30;

int Party_GetMaxCount(const Party *party);

int Party_GetCount(const Party *party);

Pokemon *Party_GetMonByIndex(const Party *party, int slot);

void sub_02072A98(Pokemon *mon, UnkStruct_02078B40 *param1);

void sub_020306DC(const Party *party, UnkStruct_0202FD30 *param1);

extern u8 *_021D2AF8;
void *sub_0203077C(enum HeapID param_1);
void *sub_020307AC(enum HeapID param_1);

void *sub_020307DC(void);
void *sub_020307F8(void);

void sub_020306DC(const Party *party, UnkStruct_0202FD30 *param1) {
    int v0;
    Pokemon *v1;

    MI_CpuClear8(param1, sizeof(UnkStruct_0202FD30));

    param1->unk_00 = Party_GetMaxCount(party);
    param1->unk_02 = Party_GetCount(party);

    for (v0 = 0; v0 < param1->unk_02; v0++) {
        v1 = Party_GetMonByIndex(party, v0);
        sub_02072A98(v1, &param1->unk_04[v0]);
    }
}


void Party_InitWithMaxSize(Party *party, int capacity);

BOOL Party_AddMon(Party *party, Pokemon *mon);

Pokemon *AllocMonZeroed(enum HeapID heapID);

void SetMonData(Pokemon *mon, int param, const void *value);

void sub_02072D64(UnkStruct_02078B40 *param0, Pokemon *mon);

void sub_02030724(UnkStruct_0202FD30 *param0, Party *party);

void sub_02030724(UnkStruct_0202FD30 *param0, Party *party) {
    int v0;
    Pokemon *v1;
    u8 v2 = 0;

    v1 = AllocMonZeroed(HEAP_ID_FIELD2);

    Party_InitWithMaxSize(party, param0->unk_00);

    for (v0 = 0; v0 < param0->unk_02; v0++) {
        sub_02072D64(&param0->unk_04[v0], v1);
        SetMonData(v1, MON_DATA_BALL_CAPSULE_ID, &v2);
        Party_AddMon(party, v1);
    }

    Heap_Free(v1);
}

void *sub_0203077C(enum HeapID param_1) {
    void *destp;

    if (_021D2AF8 == 0) {
        GF_AssertFail();
    }
    destp = Heap_Alloc(param_1, 100);
    MIi_CpuCopy32((void *)(_021D2AF8 + 0x84), destp, 100);
    return destp;
}

void *sub_020307AC(enum HeapID param_1) {
    void *destp;

    if (_021D2AF8 == 0) {
        GF_AssertFail();
    }
    destp = Heap_Alloc(param_1, 0x80);
    MIi_CpuCopy32((void *)(_021D2AF8 + 4), destp, 0x80);
    return destp;
}

void *sub_020307DC(void) {
    if (_021D2AF8 == 0) {
        GF_AssertFail();
    }
    return _021D2AF8 + 4;
}

void *sub_020307F8(void) {
    if (_021D2AF8 == 0) {
        GF_AssertFail();
    }
    return _021D2AF8 + 0x84;
}
