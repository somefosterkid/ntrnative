#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"

struct UnkStruct_ov17_0224DF54_t;
struct UnkStruct_ov63_0222BCE8_t;
struct UnkStruct_ov70_0225C894_t;
typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    u8 unk_08_0 : 1;
    u8 unk_08_1 : 1;
    u8 unk_08_2 : 1;
    u8 : 5;
} UnkStruct_ov17_0224EDE0;
typedef struct UnkStruct_ov17_0224DF54_t UnkStruct_ov17_0224DF54;
typedef struct {
    SysTask *unk_00;
    UnkStruct_ov17_0224DF54 *unk_04;
    UnkStruct_ov17_0224EDE0 unk_08;
    u8 unk_11;
    u8 unk_12;
} UnkStruct_ov17_0224D69C;
void ov42_02227F28(SysTask *task);
typedef struct UnkStruct_ov63_0222BCE8_t UnkStruct_ov63_0222BCE8;
typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;
struct UnkStruct_ov70_0225C894_t {
    UnkStruct_ov63_0222BCE8 *unk_00;
};
void ov42_02227FA4(Sprite *gfxElem);
typedef struct ChooseStarterRotation {
    fx32 verticalOffset;
    fx32 verticalOffsetMultiplier;
    int frameCountMax;
    int frameCount;
} ChooseStarterRotation;
typedef struct ChooseStarterCursor {
    Sprite *sprite;
    SpriteResource *resources[6];
    VecFx32 position;
    SysTask *movementTask;
    ChooseStarterRotation starterRotation;
} ChooseStarterCursor;
u32 ov42_02227FDC(const UnkStruct_ov63_0222BCE8 *param0, u16 param1, u16 param2);
u32 ov49_022589B8(const UnkStruct_ov70_0225C894 *param0, u16 param1, u16 param2);

void ov49_02258994(UnkStruct_ov17_0224D69C *param0);
u16 ov49_022589A8(const UnkStruct_ov70_0225C894 *param0);
void ov49_022589AC(ChooseStarterCursor *cursor);
u32 ov49_022589B8(const UnkStruct_ov70_0225C894 *param0, u16 param1, u16 param2);
u32 ov49_022589C4(const UnkStruct_ov70_0225C894 *param0, u16 param1, u16 param2);
BOOL ov49_022589D8(const UnkStruct_ov70_0225C894 *param0, u32 param1, u16 *param2, u16 *param3, u32 param4);

void ov49_02258994(UnkStruct_ov17_0224D69C *param0) {
    ov42_02227F28(param0->unk_00);
    Heap_Free(param0);
}

u16 ov49_022589A8(const UnkStruct_ov70_0225C894 *param0) {
    return 35;
}

void ov49_022589AC(ChooseStarterCursor *cursor) {
    ov42_02227FA4(cursor->sprite);
}

u32 ov49_022589B8(const UnkStruct_ov70_0225C894 *param0, u16 param1, u16 param2) {
    u32 v0 = ov42_02227FDC(param0->unk_00, param1, param2);
    return v0 >> 15;
}

u32 ov49_022589C4(const UnkStruct_ov70_0225C894 *param0, u16 param1, u16 param2) {
    u32 v0 = ov42_02227FDC(param0->unk_00, param1, param2);
    v0 &= 0x7fff;

    return v0;
}

BOOL ov49_022589D8(const UnkStruct_ov70_0225C894 *param0, u32 param1, u16 *param2, u16 *param3, u32 param4) {
    int v0, v1, v2;
    u32 v3;

    v2 = 0;

    for (v0 = 0; v0 < 42; v0++) {
        for (v1 = 0; v1 < 35; v1++) {
            v3 = ov49_022589B8(param0, v1, v0);

            if (v3 == param1) {
                if (v2 >= param4) {
                    *param2 = v1;
                    *param3 = v0;
                    return 1;
                }

                v2++;
            }
        }
    }

    return 0;
}
