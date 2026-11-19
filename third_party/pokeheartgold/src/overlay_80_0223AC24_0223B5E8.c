#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "sys_task.h"
#include "overlay_07_bg_scroll_context.h"

struct BufferManager;
struct UnkStruct_ov104_0223F174_t;
typedef struct UnkStruct_ov104_0223F174_t UnkStruct_ov104_0223F174;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u32 unk_08;
    u32 unk_0C;
    u16 unk_10;
    u16 unk_12;
    u16 unk_14;
    u16 unk_16;
    u32 unk_18;
    u32 unk_1C;
} UnkStruct_ov104_0223F1B4;
void sub_02014A38(BufferManager *bufferManager);
typedef void (*UnkFuncPtr_ov104_0223F110)(void *);
typedef struct {
    SysTask *unk_00;
    SysTask *unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    UnkFuncPtr_ov104_0223F110 unk_10;
    UnkFuncPtr_ov104_0223F110 unk_14;
    void *unk_18;
} UnkStruct_ov104_0223F0D8;
struct UnkStruct_ov104_0223F174_t {
    UnkStruct_ov104_0223F0D8 unk_00;
    BufferManager *bufferManager;
    UnkStruct_ov104_0223F1B4 unk_20[192];
    UnkStruct_ov104_0223F1B4 unk_1820[192];
    u32 unk_3020;
};
void ov80_0223B4E0(UnkStruct_ov104_0223F0D8 *param0);
void *sub_02014A4C(const BufferManager *bufferManager);

void ov80_0223B5E8(UnkStruct_ov104_0223F174 *param0);
void *ov80_0223B60C(const UnkStruct_ov104_0223F174 *param0);

void ov80_0223B5E8(UnkStruct_ov104_0223F174 *param0) {
    ((param0) ? (void)0 : GF_AssertFail());

    ov80_0223B4E0(&param0->unk_00);

    if (param0->bufferManager != 0L) {
        sub_02014A38(param0->bufferManager);
    }

    Heap_Free(param0);
}

void *ov80_0223B60C(const UnkStruct_ov104_0223F174 *param0) {
    ((param0) ? (void)0 : GF_AssertFail());

    return sub_02014A4C(param0->bufferManager);
}
