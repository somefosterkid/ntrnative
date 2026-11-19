#include "global.h"

#include "error_handling.h"
#include "overlay_49_unk_struct_ov70_02269800.h"
#include "constants/heap.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct Field3DModelAnimation;

struct Field3dModel;

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_02260AD4_t;

struct UnkStruct_ov70_02269204_t;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;

typedef struct UnkStruct_ov70_02269204_t UnkStruct_ov70_02269204;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

u32 ov49_02258E60(const UnkStruct_ov70_0225CC54 *param0, int param1);

struct UnkStruct_ov70_02269204_t {
    WiFiPlazaSystem *unk_00;
    UnkStruct_ov70_0225CA20 *unk_04;
    UnkStruct_ov70_02260AD4 *unk_08;
    UnkStruct_ov70_0225C894 *unk_0C;
    UnkStruct_ov70_02269800 unk_10[20];
    Field3dModel unk_10550[15];
    void *unk_10640[39];
    Field3DModelAnimation unk_106DC[16];
    NNSFndAllocator unk_1081C;
};

void ov49_02265948(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_02269B10(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_02269B1C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_02269B28(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_02269B34(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_02269B40(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_02269B4C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_02265CB0(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_02265D10(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_02265E54(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_02266088(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_0226A1FC(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_0226A208(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_0226A214(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_0226A220(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_0226A22C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_02266390(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_0226A244(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_0226A294(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_0226643C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_0226648C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_022664DC(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_0226652C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_0226A424(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_0226A478(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov70_0226A4CC(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

UnkStruct_ov70_02269204 *ov49_022652E8(WiFiPlazaSystem *param0, UnkStruct_ov70_0225C894 *param1, UnkStruct_ov70_02260AD4 *param2, UnkStruct_ov70_0225CA20 *param3, enum HeapID heapID, u32 heapID2);

void ov49_02265668(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 seqID);

void ov49_02265890(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, UnkStruct_ov70_0225CC54 *param2, u32 param3);

void ov49_02265890(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, UnkStruct_ov70_0225CC54 *param2, u32 param3) {
    extern void (*const ov49_0226A5A4[27])(UnkStruct_ov70_02269204 *, UnkStruct_ov70_02269800 *);

    extern const u32 ov49_0226A610[27];

    ((param3 < 27) ? (void)0 : GF_AssertFail());

    ov49_02265948(param0, param1);

    param1->unk_08 = param2;
    param1->unk_00 = param3;
    param1->unk_04 = 40 + ov49_02258E60(param2, 5);

    ov49_0226A5A4[param1->unk_00](param0, param1);

    ov49_02265668(param0, param1, ov49_0226A610[param1->unk_00]);
}

BOOL ov49_02266678(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov70_0226A5F0(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov70_0226A5FC(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov70_0226A608(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_0226676C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_02266820(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_02266978(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_022669B0(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_02266A88(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_02266AB0(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

s32 ov49_02265958(void *arg0);

void ov49_022658E4(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_022658E4(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    extern BOOL (*const ov49_0226A538[27])(UnkStruct_ov70_02269204 *, UnkStruct_ov70_02269800 *);

    if (ov49_02265958(param1) == 0) {
        return;
    }

    GF_ASSERT(param1->unk_00 < 27);

    if (ov49_0226A538[param1->unk_00](param0, param1) == 1) {
        ov49_02265948(param0, param1);
    }
}
