#include "global.h"

#include "heap.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_90_unk_struct_ov70_0225_ca20_t.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

s32 ov45_0222A33C(void *);

s32 ov45_0222CE2C(void *, s32);

s32 ov45_0222D38C(void *arg0);

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
void ov45_0222D410(ChooseStarterCursor *cursor);
struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222BEC0_t;
struct UnkStruct_ov63_0222D848_t;
struct UnkStruct_ov66_02234548_t;
struct UnkStruct_ov66_02234798_t;
struct UnkStruct_ov70_0225C894_t;
struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;
typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;
typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;
typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;
typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;
typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0225D220;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    s8 unk_02;
    s8 unk_03;
    s16 unk_04;
    s16 unk_06;
} UnkStruct_ov70_0225D93C;
typedef union {
    struct {
        s16 unk_00;
        s16 unk_02;
    } val1;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
    } val2;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
        fx32 unk_14;
    } val3;
    struct {
        s32 unk_00;
        VecFx32 unk_04;
    } val4;
} UnkStruct_ov70_0225CC54_sub1;
struct UnkStruct_ov70_0225CC54_t {
    UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov66_02234798 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    UnkStruct_ov70_0225CC54_sub1 unk_0C;
    void (*unk_24)(struct UnkStruct_ov70_0225CC54_t *, UnkStruct_ov70_0225CA20 *);
};
void ov45_0222D434(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1, int param2);
void ov45_0222D41C(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1);

void ov45_0222D428(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1);

s32 ov45_0222D38C(void *arg0) {
    if (ov45_0222A33C(*(void **)((u8 *)arg0 + 0x3C)) != 0) {
        ov45_0222CE2C(arg0, 0xA);
    } else {
        ov45_0222CE2C(arg0, 0);
    }
    return 0;
}

s32 ov45_0222CDC4(void *, void **, s32 *);

extern s32 ov45_02254B24;

void ov45_0222D3B0(void *arg0, void **arg1);

void ov45_0222D3B0(void *arg0, void **arg1) {
    *(s8 *)((u8 *)(*arg1) + 4) = 0;
    ov45_0222CDC4(arg0, arg1, &ov45_02254B24);
}

extern s32 ov45_02254B14;

void ov45_0222D3C4(void *arg0, void **arg1);

void ov45_0222D3C4(void *arg0, void **arg1) {
    *(s8 *)((u8 *)(*arg1) + 4) = 1;
    ov45_0222CDC4(arg0, arg1, &ov45_02254B14);
}

void ov45_0222D3D8(void *arg0, void **arg1, enum HeapID arg2);

void ov45_0222D3D8(void *arg0, void **arg1, enum HeapID heapID) {
    void *v0;
    void *v1;

    v0 = Heap_Alloc(heapID, 0xCU);
    *arg1 = v0;
    *(s8 *)v0 = 0;
    *(s8 *)((u8 *)v0 + 1) = 0;
    *(s8 *)((u8 *)v0 + 2) = 0;
    *(s8 *)((u8 *)v0 + 3) = 0;
    *(s8 *)((u8 *)v0 + 4) = 0;
    *(s8 *)((u8 *)v0 + 5) = 0;
    *(s8 *)((u8 *)v0 + 6) = 0;
    *(s8 *)((u8 *)v0 + 7) = 0;
    *(s8 *)((u8 *)v0 + 8) = 0;
    *(s8 *)((u8 *)v0 + 9) = 0;
    *(s8 *)((u8 *)v0 + 0xA) = 0;
    *(s8 *)((u8 *)v0 + 0xB) = 0;
    v1 = *arg1;
    *(s32 *)v1 = (s32) * (s32 *)((u8 *)arg0 + 0x40);
    *(s32 *)((u8 *)v1 + 4) = (s32) * (s32 *)((u8 *)arg0 + 0x3C);
}

void ov45_0222D410(ChooseStarterCursor *cursor) {
    Heap_Free(cursor->sprite);
}

void ov45_0222D41C(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1) {
    ov45_0222D434(param0, param1, 0);
}

void ov45_0222D428(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1) {
    ov45_0222D434(param0, param1, 1);
}
