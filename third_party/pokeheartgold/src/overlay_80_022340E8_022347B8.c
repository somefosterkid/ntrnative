#include "global.h"

#include "constants/heap.h"
#include "overlay_49_unk_struct_ov70_02263344_t.h"
#include "overlay_103_unk_struct_ov70_022630_a4_t.h"

struct UnkStruct_ov70_0225DEE8_t;
struct UnkStruct_ov70_022630A4_t;
struct UnkStruct_ov70_02263344_t;
typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;
typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;
typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;
typedef BOOL (*UnkFuncPtr_ov70_0226DA18)(UnkStruct_ov70_02263344 *, UnkStruct_ov70_0225DEE8 *, u32);
typedef struct {
    UnkFuncPtr_ov70_0226DA18 unk_00;
    UnkFuncPtr_ov70_0226DA18 unk_04;
} UnkStruct_ov70_0226DA18;
typedef struct {
    const UnkStruct_ov70_0226DA18 *unk_00;
    void *unk_04;
    void *unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0226308C;
void ov80_02234588(UnkStruct_ov70_022630A4 *param0, u8 param1);

void ov80_022347B8(UnkStruct_ov70_022630A4 *param0);

void ov80_022347B8(UnkStruct_ov70_022630A4 *param0) {
    ov80_02234588(param0, 1);
}
