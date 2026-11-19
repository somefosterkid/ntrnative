#include "global.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_02261E10_t;

struct UnkStruct_ov70_0226C914_t;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

typedef struct UnkStruct_ov70_0226C914_t UnkStruct_ov70_0226C914;

int ov45_0222A35C(const WiFiPlazaSystem *param0);

int ov45_0222B1DC(const WiFiPlazaSystem *param0);

u32 ov45_0222B1EC(const WiFiPlazaSystem *param0);

void ov49_0225E624(UnkStruct_ov70_02261E10 *param0, BOOL param1);

void *Heap_Alloc(u32 heapID, u32 size);

typedef union {
    struct {
        u32 unk_00;
    } val1;
    struct {
        u32 unk_00;
    } val2;
    struct {
        u32 unk_00;
    } val3;
} UnkStruct_ov70_0226C914_sub1;

struct UnkStruct_ov70_0226C914_t {
    const WiFiPlazaSystem *unk_00;
    UnkStruct_ov70_02261E10 *unk_04;
    u8 unk_08;
    u8 unk_09;
    u16 unk_0A;
    u8 pad_C[4];
};

void ov49_02268C74(UnkStruct_ov70_0226C914 *param0, u32 param1, u32 param2);

void ov49_02268D94(UnkStruct_ov70_0226C914 *param0);

UnkStruct_ov70_0226C914 *ov49_02268A0C(const WiFiPlazaSystem *param0, UnkStruct_ov70_02261E10 *param1, u32 heapID);

UnkStruct_ov70_0226C914 *ov49_02268A0C(const WiFiPlazaSystem *param0, UnkStruct_ov70_02261E10 *param1, u32 heapID) {
    UnkStruct_ov70_0226C914 *v0;
    u32 v1;

    v0 = Heap_Alloc(heapID, sizeof(UnkStruct_ov70_0226C914));
    memset(v0, 0, sizeof(UnkStruct_ov70_0226C914));

    v0->unk_00 = param0;
    v0->unk_04 = param1;
    v1 = ov45_0222B1DC(param0);
    v0->unk_0A = ov45_0222A35C(param0);

    if ((0 != v0->unk_0A) && (v1 != 0)) {
        u32 v2;

        ov49_0225E624(param1, 1);
        ov49_02268D94(v0);

        v2 = ov45_0222B1EC(param0);
        ov49_02268C74(v0, v1, v2);
    }

    return v0;
}
